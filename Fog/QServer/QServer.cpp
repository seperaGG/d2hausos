#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\QServer\\QServer.cpp"
#include "FogLocal.h"
#include <winsock.h>
#include <stdlib.h>
#include <string.h>
#include "StormImports.h"
#include "QServer.h"
#include "D2Macros.h"

static __inline void QServerFreeBlocks(QSERVER *ptServer);
static __inline void QServerAddAddress(QSERVER *ptServer, DWORD dwAddress);
static __inline void QServerReleaseAddress(QSERVER *ptServer, DWORD dwAddress);
static __inline void QServerExpireRates(QSERVER *ptServer, DWORD dwNow);
static __inline void QServerFreeRates(QSERVER *ptServer);
BOOL __stdcall QServerFlushMessages(QSERVER *ptServer);
BOOL __stdcall QServerBroadcast(QSERVER *ptServer, const BYTE *pbData, int nBytes);
DWORD __stdcall QServerGetAddress(QSERVER *ptServer, SOCKET hSocket);

BOOL __stdcall QServerIsLocal(QSERVER *ptServer)
{
	if (!ptServer)
		return FALSE;
	return ptServer->nMode == 1 || ptServer->nMode == 2;
}

QSERVER * __stdcall QServerCreate(int nMode, int nNumMsgLists, u_short nPort, BOOL fForce98, QSPARSE pfnParse,
	QSSOCKETHOOK pfnAccept, QSSOCKETHOOK pfnClose, QSLOCALSEND pfnLocalSend)
{
	QSERVER *ptServer;
	WSADATA tWsaData;
	OSVERSIONINFOA tVersion;
	int i;

#line 90
	ptServer = (QSERVER *)D2ALLOC2(sizeof(QSERVER));
	for (i = 0; i < (int)(sizeof(QSERVER) / sizeof(DWORD)); i++)
		((DWORD *)ptServer)[i] = 0;
	ptServer->pfnParse = pfnParse;
	ptServer->pfnAccept = pfnAccept;
	ptServer->pfnClose = pfnClose;
	ptServer->nNumClients = 0;
	ptServer->ptRateHead = NULL;
	ptServer->ptRateTail = NULL;
	ptServer->pfnLocalSend = pfnLocalSend;
	ptServer->dwLastAgeTick = (DWORD)-1;
	ptServer->ptBlocks = NULL;
	ptServer->dwLastAcceptTick = 0;
	ptServer->fBlockAddresses = TRUE;
#line 114
	ptServer->pptMsgHeads = (QSMSG **)D2ALLOC2(nNumMsgLists * sizeof(QSMSG *));
	for (i = 0; i < nNumMsgLists; i++)
		ptServer->pptMsgHeads[i] = NULL;
#line 118
	ptServer->pptMsgTails = (QSMSG **)D2ALLOC2(nNumMsgLists * sizeof(QSMSG *));
	for (i = 0; i < nNumMsgLists; i++)
		ptServer->pptMsgTails[i] = NULL;
	ptServer->nNumMsgLists = nNumMsgLists;
	ptServer->ptFreeMsgs = NULL;
	InitializeCriticalSection(&ptServer->tLock);
	ptServer->nMode = nMode;
	if (QServerIsLocal(ptServer))
		return ptServer;
	if (WSAStartup(257, &tWsaData))
		return ptServer;
	for (i = 0; i < (int)(sizeof(tVersion) / sizeof(DWORD)); i++)
		((DWORD *)&tVersion)[i] = 0;
	tVersion.dwOSVersionInfoSize = sizeof(tVersion);
	if (GetVersionExA(&tVersion) && tVersion.dwPlatformId == VER_PLATFORM_WIN32_NT && !fForce98)
		ptServer->fCompletionPort = TRUE;
	else
		ptServer->fCompletionPort = FALSE;
	if (ptServer->fCompletionPort)
		QServerNTStart(ptServer, nPort);
	else
		QServer98Start(ptServer, nPort);
	return ptServer;
}

void __stdcall QServerSetAddressLimit(QSERVER *ptServer, DWORD dwLimit)
{
	if (!ptServer)
		return;
	if (dwLimit <= 0)
		dwLimit = 1;
	EnterCriticalSection(&ptServer->tLock);
	ptServer->nAddressLimit = dwLimit;
	LeaveCriticalSection(&ptServer->tLock);
}

static BOOL QServerSendToClient(QSERVER *ptServer, SOCKET hSocket, const BYTE *pbData, int nBytes)
{
	QSCLIENT *ptClient;

	ptClient = QServerFindClient(ptServer, hSocket);
	if (!ptClient)
		return FALSE;
	if (ptServer->fCompletionPort)
		return QServerNTSend(ptServer, ptClient, pbData, nBytes);
	return QServer98Send(ptServer, hSocket, pbData, nBytes);
}

BOOL __stdcall QServerBroadcast(QSERVER *ptServer, const BYTE *pbData, int nBytes)
{
	QSCLIENT *ptEntry;
	int i;

	EnterCriticalSection(&ptServer->tLock);
	for (i = 0; i < QSERVER_CLIENT_HASH; i++)
	{
		for (ptEntry = ptServer->aptClients[i]; ptEntry; ptEntry = ptEntry->ptNext)
		{
			QServerSendToClient(ptServer, ptEntry->hSocket, pbData, nBytes);
		}
	}
	LeaveCriticalSection(&ptServer->tLock);
	return TRUE;
}

BOOL __stdcall QServerDestroy(QSERVER *ptServer, const BYTE *pbData, int nBytes)
{
	QSMSG *ptNext;

	if (!ptServer)
		return TRUE;
	if (pbData)
		QServerBroadcast(ptServer, pbData, nBytes);
	if (!QServerIsLocal(ptServer))
	{
		if (ptServer->fCompletionPort)
			QServerNTStop(ptServer);
		else
			QServer98Stop(ptServer);
	}
	QServerFreeRates(ptServer);
	QServerFreeBlocks(ptServer);
	QServerFlushMessages(ptServer);
	closesocket(ptServer->hListenSocket);
	WSACleanup();
	EnterCriticalSection(&ptServer->tLock);
	DeleteCriticalSection(&ptServer->tLock);
	while (ptServer->ptFreeMsgs)
	{
		ptNext = ptServer->ptFreeMsgs->ptNext;
#line 223
		D2FREE2(ptServer->ptFreeMsgs);
		ptServer->ptFreeMsgs = ptNext;
	}
#line 226
	D2FREE2(ptServer->pptMsgHeads);
#line 227
	D2FREE2(ptServer->pptMsgTails);
#line 231
	D2FREE2(ptServer);
	return TRUE;
}

DWORD __stdcall QServerWait(QSERVER *ptServer, DWORD dwMilliseconds)
{
	if (ptServer->fCompletionPort)
		return 0;
	return WaitForSingleObject(ptServer->hMsgEvent, dwMilliseconds);
}

BOOL __stdcall QServerFlushMessages(QSERVER *ptServer)
{
	QSMSG *ptMsg;
	QSMSG *ptNext;
	int i;

	for (i = 0; i < ptServer->nNumMsgLists; i++)
	{
		EnterCriticalSection(&ptServer->tLock);
		ptMsg = ptServer->pptMsgHeads[i];
		ptServer->pptMsgHeads[i] = NULL;
		ptServer->pptMsgTails[i] = NULL;
		LeaveCriticalSection(&ptServer->tLock);
		while (ptMsg)
		{
			ptNext = ptMsg;
			ptMsg = ptMsg->ptNext;
#line 272
			D2FREE2(ptNext);
		}
	}
	return TRUE;
}

void __fastcall QServerAgeBlocks(QSERVER *ptServer)
{
	QSBLOCK *ptBlock;
	QSBLOCK *ptPrev;
	QSBLOCK *ptNext;
	DWORD dwNow;
	DWORD dwElapsed;
	DWORD dwSeconds;

	if (ptServer->dwLastAgeTick == (DWORD)-1)
		ptServer->dwLastAgeTick = GetTickCount();
	dwNow = GetTickCount();
	dwElapsed = dwNow - ptServer->dwLastAgeTick;
	if (dwElapsed < 1000)
		return;
	dwSeconds = dwElapsed / 1000;
	ptServer->dwLastAgeTick = dwNow;
	ptPrev = NULL;
	for (ptBlock = ptServer->ptBlocks; ptBlock; ptBlock = ptNext)
	{
		ptNext = ptBlock->ptNext;
		if (ptBlock->dwBlockSeconds)
		{
			if (ptBlock->dwBlockSeconds > dwSeconds)
				ptBlock->dwBlockSeconds -= dwSeconds;
			else
			{
				ptBlock->dwBlockSeconds = 0;
				if (dwSeconds < QSERVER_BLOCK_TICKS)
					ptBlock->dwLifeSeconds = QSERVER_BLOCK_TICKS - dwSeconds;
				else
					ptBlock->dwLifeSeconds = 0;
				ptBlock->dwLifeSeconds += dwSeconds;
			}
		}
		if (ptBlock->dwLifeSeconds)
		{
			if (ptBlock->dwLifeSeconds > dwSeconds)
				ptBlock->dwLifeSeconds -= dwSeconds;
			else if (ptPrev)
			{
				ptPrev->ptNext = ptBlock->ptNext;
#line 331
				D2FREE2(ptBlock);
				ptBlock = ptPrev;
			}
			else
			{
				ptServer->ptBlocks = ptServer->ptBlocks->ptNext;
#line 337
				D2FREE2(ptBlock);
				ptBlock = NULL;
			}
		}
		ptPrev = ptBlock;
	}
}

QSCLIENT * __fastcall QServerFindClient(QSERVER *ptServer, SOCKET hSocket)
{
	QSCLIENT *ptClient;

	EnterCriticalSection(&ptServer->tLock);
	for (ptClient = ptServer->aptClients[hSocket & (QSERVER_CLIENT_HASH - 1)]; ptClient;
		ptClient = ptClient->ptNext)
	{
		if (ptClient->hSocket == hSocket)
		{
			LeaveCriticalSection(&ptServer->tLock);
			return ptClient;
		}
	}
	LeaveCriticalSection(&ptServer->tLock);
	return NULL;
}

QSCLIENT * __fastcall QServerAddClient(QSERVER *ptServer, SOCKET hSocket, DWORD dwAddress)
{
	QSCLIENT *ptClient;
	struct linger tLinger;
	int nBucket;
	int i;

#line 380
	ptClient = (QSCLIENT *)D2ALLOC2(sizeof(QSCLIENT));
	for (i = 0; i < (int)(sizeof(QSCLIENT) / sizeof(DWORD)); i++)
		((DWORD *)ptClient)[i] = 0;
	ptClient->dwAddress = dwAddress;
	ptClient->hSocket = hSocket;
	nBucket = hSocket & (QSERVER_CLIENT_HASH - 1);
	ptClient->dwFloodTick = 0;
	ptClient->dwFloodCount = 0;
	ptClient->ptSendQueue = NULL;
	InitializeCriticalSection(&ptClient->tLock);
	tLinger.l_onoff = 0;
	tLinger.l_linger = 0;
	setsockopt(hSocket, SOL_SOCKET, SO_LINGER, (const char *)&tLinger, sizeof(tLinger));
	EnterCriticalSection(&ptServer->tLock);
	ptClient->ptNext = ptServer->aptClients[nBucket];
	ptServer->aptClients[nBucket] = ptClient;
	LeaveCriticalSection(&ptServer->tLock);
	QServerAddAddress(ptServer, dwAddress);
	InterlockedIncrement(&ptClient->lRefCount);
	ptServer->nNumClients++;
	return ptClient;
}

static __inline void __fastcall QServerUnlinkClient(QSERVER *ptServer, SOCKET hSocket);

void __fastcall QServerRemoveClient(QSERVER *ptServer, SOCKET hSocket)
{
	int nResult;

	if (ptServer->pfnClose)
		ptServer->pfnClose(ptServer, hSocket);
	nResult = closesocket(hSocket);
	if (nResult)
		WSAGetLastError();
	QServerUnlinkClient(ptServer, hSocket);
	if (nResult == SOCKET_ERROR)
		WSAGetLastError();
}

void __fastcall QServerBlockAddress(QSERVER *ptServer, struct in_addr tAddress)
{
	QSBLOCK *ptBlock;

	ptBlock = ptServer->ptBlocks;
	if (!ptServer->fBlockAddresses)
		return;
	if (ptServer->pfnBlockHook)
		ptServer->pfnBlockHook(tAddress.s_addr);
	for (; ptBlock; ptBlock = ptBlock->ptNext)
	{
		if (ptBlock->dwAddress == tAddress.s_addr)
		{
			ptBlock->dwBlockSeconds = QSERVER_BLOCK_TICKS;
			return;
		}
	}
#line 447
	ptBlock = (QSBLOCK *)D2ALLOC2(sizeof(QSBLOCK));
	ptBlock->dwAddress = tAddress.s_addr;
	ptBlock->dwLifeSeconds = QSERVER_BLOCK_TICKS;
	ptBlock->dwBlockSeconds = 0;
	ptBlock->ptNext = ptServer->ptBlocks;
	ptServer->ptBlocks = ptBlock;
}

BOOL __fastcall QServerIsBlocked(QSERVER *ptServer, struct in_addr tAddress)
{
	QSBLOCK *ptBlock;

	for (ptBlock = ptServer->ptBlocks; ptBlock; ptBlock = ptBlock->ptNext)
	{
		if (ptBlock->dwAddress == tAddress.s_addr)
		{
			if (!ptBlock->dwBlockSeconds)
				return FALSE;
			ptBlock->dwBlockSeconds = QSERVER_BLOCK_TICKS;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL __fastcall QServerAddressFull(QSERVER *ptServer, DWORD *pdwAddress)
{
	QSADDRESS *ptEntry;
	DWORD dwAddress;

	if (ptServer->nAddressLimit < 0)
		return FALSE;
	dwAddress = *pdwAddress;
	for (ptEntry = ptServer->aptAddresses[dwAddress % QSERVER_IP_HASH]; ptEntry; ptEntry = ptEntry->ptNext)
	{
		if (ptEntry->dwAddress == dwAddress)
			return ptEntry->nNumConnections >= ptServer->nAddressLimit;
	}
	return FALSE;
}

BOOL __fastcall QServerAllowConnect(QSERVER *ptServer, DWORD *pdwAddress)
{
	QSRATE *ptRate;
	DWORD dwAddress;
	DWORD dwTick;
	DWORD dwLimit;

	if (ptServer->nAddressLimit < 1)
		return TRUE;
	dwAddress = *pdwAddress;
	dwTick = GetTickCount();
	dwLimit = ptServer->nAddressLimit * 2 + 3;
	QServerExpireRates(ptServer, GetTickCount());
	for (ptRate = ptServer->ptRateHead; ptRate; ptRate = ptRate->ptNext)
	{
		if (ptRate->dwAddress != dwAddress)
			continue;
		if (dwTick - ptRate->dwWindowTick < 15000)
		{
			if (++ptRate->dwNumConnects > dwLimit)
				return FALSE;
		}
		else
		{
			ptRate->dwWindowTick = dwTick;
			ptRate->dwNumConnects = 1;
		}
	}
#line 538
	ptRate = (QSRATE *)D2ALLOC2(sizeof(QSRATE));
	ptRate->dwWindowTick = dwTick;
	ptRate->dwNumConnects = 1;
	ptRate->ptNext = NULL;
	ptRate->dwAddress = dwAddress;
	if (!ptServer->ptRateHead)
	{
		ptServer->ptRateHead = ptRate;
		ptServer->ptRateTail = ptRate;
	}
	else
	{
		ptServer->ptRateTail->ptNext = ptRate;
		ptServer->ptRateTail = ptRate;
	}
	return TRUE;
}

BOOL __fastcall QServerDropAddress(QSERVER *ptServer, DWORD *pdwAddress)
{
	QSCLIENT *ptClient;
	SOCKET hSocket;
	DWORD dwAddress;
	int i;

	dwAddress = *pdwAddress;
	hSocket = 0;
	EnterCriticalSection(&ptServer->tLock);
	for (i = 0; i < QSERVER_CLIENT_HASH; i++)
	{
		for (ptClient = ptServer->aptClients[i]; ptClient; ptClient = ptClient->ptNext)
		{
			if (ptClient->dwAddress == dwAddress)
			{
				hSocket = ptClient->hSocket;
				break;
			}
		}
		if (hSocket)
			break;
	}
	LeaveCriticalSection(&ptServer->tLock);
	if (hSocket)
		QServerDisconnect(ptServer, hSocket);
	return TRUE;
}

int __stdcall QServerGetMessage(QSERVER *ptServer, int nMsgType, BYTE *pbBuffer, DWORD dwBuffSize)
{
	QSMSG *ptMsg;
	QSMSG *ptFree;
	int nCopy;
	int nBytes;
	int i;

	if (nMsgType >= ptServer->nNumMsgLists)
		return -1;
	EnterCriticalSection(&ptServer->tLock);
	ptMsg = ptServer->pptMsgHeads[nMsgType];
	if (ptMsg)
	{
#line 612
		D2ASSERT(dwBuffSize <= MAX_MSG_SIZE);
		nCopy = ptMsg->nDataSize + sizeof(ptMsg->hSocket);
		if (nCopy > (int)dwBuffSize)
			nCopy = dwBuffSize;
		memcpy(pbBuffer, ptMsg, nCopy);
		nBytes = ptMsg->nDataSize;
		ptServer->pptMsgHeads[nMsgType] = ptMsg->ptNext;
		if (ptServer->pptMsgTails[nMsgType] == ptMsg)
			ptServer->pptMsgTails[nMsgType] = NULL;
		ptFree = ptServer->ptFreeMsgs;
		for (i = 0; i < (int)(sizeof(QSMSG) / sizeof(DWORD)); i++)
			((DWORD *)ptMsg)[i] = 0;
		ptMsg->ptNext = ptFree;
		ptServer->ptFreeMsgs = ptMsg;
	}
	else
		nBytes = -1;
	LeaveCriticalSection(&ptServer->tLock);
	return nBytes;
}

static BOOL QServerChargeFlood(QSCLIENT *ptClient, int nFlood)
{
	DWORD dwTick;

	dwTick = GetTickCount();
	EnterCriticalSection(&ptClient->tLock);
	ptClient->dwFloodCount += nFlood;
	LeaveCriticalSection(&ptClient->tLock);
	if (ptClient->dwFloodCount > 1000)
		return FALSE;
	if (dwTick - ptClient->dwFloodTick > 1000)
	{
		EnterCriticalSection(&ptClient->tLock);
		ptClient->dwFloodTick = dwTick;
		if (ptClient->dwFloodCount >= 200)
			ptClient->dwFloodCount -= 200;
		LeaveCriticalSection(&ptClient->tLock);
	}
	return TRUE;
}

void __fastcall QServerParse(QSERVER *ptServer, QSCLIENT *ptClient)
{
	QSMSG *ptMsg;
	QSCLIENT *ptFound;
	SOCKET hSocket;
	int nPacketSize;
	int nHeaderSize;
	int nMsgType;
	int nFlood;
	int nBytes;
	int nResult;

	while (1)
	{
		nHeaderSize = 0;
		nPacketSize = 0;
		nMsgType = 0;
		nFlood = 0;
		if (!ptServer->pfnParse)
			return;
		nBytes = ptClient->nMsgBuffSize;
		nResult = ptServer->pfnParse(ptClient->abMsgBuff, nBytes, &nPacketSize, &nHeaderSize,
			&nMsgType, &nFlood, ptClient->hSocket, ptClient->dwOwner);
		switch (nResult)
		{
		case 3:
			return;
		case 4:
			QServerDisconnect(ptServer, ptClient->hSocket);
			return;
		case 2:
			if (!QServerChargeFlood(ptClient, nFlood))
			{
				QServerDisconnect(ptServer, ptClient->hSocket);
				return;
			}
			break;
		}
		if (nResult != 5)
		{
			memcpy(ptClient->abLastPacket, ptClient->abMsgBuff, nPacketSize);
			if (ptServer->ptFreeMsgs)
			{
				EnterCriticalSection(&ptServer->tLock);
				ptMsg = ptServer->ptFreeMsgs;
				if (ptMsg)
				{
					ptServer->ptFreeMsgs = ptMsg->ptNext;
					LeaveCriticalSection(&ptServer->tLock);
				}
				else
				{
#line 711
					ptMsg = (QSMSG *)D2ALLOC2(sizeof(QSMSG));
					LeaveCriticalSection(&ptServer->tLock);
				}
			}
			else
			{
#line 720
				ptMsg = (QSMSG *)D2ALLOC2(sizeof(QSMSG));
			}
			nBytes = nPacketSize - nHeaderSize;
			if (nBytes > 0)
			{
				EnterCriticalSection(&ptClient->tLock);
				memcpy(ptMsg->abData, ptClient->abMsgBuff + nHeaderSize, nBytes);
				LeaveCriticalSection(&ptClient->tLock);
			}
			else
				*(BYTE *)&ptMsg->hSocket = 0;
			ptMsg->nDataSize = nBytes;
			ptMsg->dwArrivalTick = GetTickCount();
			ptMsg->ptNext = NULL;
			ptFound = QServerFindClient(ptServer, ptClient->hSocket);
			if (ptFound)
				hSocket = ptFound->hSocket;
			else
				hSocket = 65535;
			ptMsg->hSocket = hSocket;
			if (hSocket == 65535)
			{
#line 748
				D2FREE2(ptMsg);
				QServerDisconnect(ptServer, ptClient->hSocket);
				return;
			}
			else
			{
#line 775
				D2ASSERT(nMsgType < ptServer->nNumMsgLists);
				EnterCriticalSection(&ptServer->tLock);
				if (!ptServer->pptMsgTails[nMsgType])
					ptServer->pptMsgHeads[nMsgType] = ptMsg;
				else
					ptServer->pptMsgTails[nMsgType]->ptNext = ptMsg;
				ptServer->pptMsgTails[nMsgType] = ptMsg;
				LeaveCriticalSection(&ptServer->tLock);
			}
		}
		EnterCriticalSection(&ptClient->tLock);
		ptClient->nMsgBuffSize -= nPacketSize;
#line 786
		D2ASSERT(nPacketSize + ptClient->nMsgBuffSize <= READ_BUFFER_SIZE);
		memmove(ptClient->abMsgBuff, ptClient->abMsgBuff + nPacketSize, ptClient->nMsgBuffSize);
		LeaveCriticalSection(&ptClient->tLock);
	}
}

BOOL __stdcall QServerPump(QSERVER *ptServer, DWORD dwMilliseconds)
{
	if (ptServer->fCompletionPort)
		return QServerNTPump(ptServer, dwMilliseconds);
	return QServerWait(ptServer, dwMilliseconds) == WAIT_OBJECT_0;
}

int __stdcall QServerSend(QSERVER *ptServer, SOCKET hSocket, const BYTE *pbData, int nBytes)
{
	QSCLIENT *ptClient;

	if (QServerIsLocal(ptServer) && ptServer->pfnLocalSend)
		return ptServer->pfnLocalSend(ptServer, pbData, nBytes);
	ptClient = QServerFindClient(ptServer, hSocket);
	if (!ptClient)
		return FALSE;
	if (ptServer->fCompletionPort)
		return QServerNTSend(ptServer, ptClient, pbData, nBytes);
	return QServer98Send(ptServer, hSocket, pbData, nBytes);
}

void __stdcall QServerGetAddressString(QSERVER *ptServer, SOCKET hSocket, char *pszAddress)
{
	QSCLIENT *ptClient;
	char *pszText;

	EnterCriticalSection(&ptServer->tLock);
	ptClient = ptServer->aptClients[hSocket & (QSERVER_CLIENT_HASH - 1)];
	*pszAddress = 0;
	for (; ptClient; ptClient = ptClient->ptNext)
	{
		if (ptClient->hSocket == hSocket)
		{
			pszText = inet_ntoa(*(struct in_addr *)&ptClient->dwAddress);
			StormSStrCopy(pszAddress, pszText, 2147483647);
			LeaveCriticalSection(&ptServer->tLock);
			return;
		}
	}
	LeaveCriticalSection(&ptServer->tLock);
}

DWORD __stdcall QServerGetAddress(QSERVER *ptServer, SOCKET hSocket)
{
	QSCLIENT *ptClient;

	DWORD dwAddress;

	EnterCriticalSection(&ptServer->tLock);
	for (ptClient = ptServer->aptClients[hSocket & (QSERVER_CLIENT_HASH - 1)]; ptClient;
		ptClient = ptClient->ptNext)
	{
		if (ptClient->hSocket == hSocket)
		{
			dwAddress = ptClient->dwAddress;
			LeaveCriticalSection(&ptServer->tLock);
			return dwAddress;
		}
	}
	LeaveCriticalSection(&ptServer->tLock);
	return 0;
}

void __stdcall QServerDisconnect(QSERVER *ptServer, SOCKET hSocket)
{
	QSCLIENT *ptClient;
	struct in_addr tAddress;

	if (!ptServer->fBlockAddresses)
		return;
	EnterCriticalSection(&ptServer->tLock);
	tAddress.s_addr = 0;
	ptClient = ptServer->aptClients[hSocket & (QSERVER_CLIENT_HASH - 1)];
	if (ptClient)
		while (1)
		{
			if (ptClient->hSocket == hSocket)
			{
				tAddress.s_addr = ptClient->dwAddress;
				break;
			}
			if (!(ptClient = ptClient->ptNext))
				break;
		}
	LeaveCriticalSection(&ptServer->tLock);
	QServerRemoveClient(ptServer, hSocket);
	QServerBlockAddress(ptServer, tAddress);
}

void __stdcall QServerDisconnectAddress(QSERVER *ptServer, struct in_addr tAddress)
{
	QSCLIENT *ptClient;
	SOCKET hSocket;
	int i;

	hSocket = 0;
	if (!ptServer->fBlockAddresses)
		return;
	EnterCriticalSection(&ptServer->tLock);
	for (i = 0; i < QSERVER_CLIENT_HASH; i++)
	{
		for (ptClient = ptServer->aptClients[i]; ptClient; ptClient = ptClient->ptNext)
		{
			if (ptClient->dwAddress == tAddress.s_addr)
			{
				hSocket = ptClient->hSocket;
				goto Found;
			}
		}
	}
Found:
	LeaveCriticalSection(&ptServer->tLock);
	if (hSocket)
		QServerRemoveClient(ptServer, hSocket);
	QServerBlockAddress(ptServer, tAddress);
}

void __stdcall QServerClose(QSERVER *ptServer, SOCKET hSocket)
{
	QServerRemoveClient(ptServer, hSocket);
}

QSADDRESSHOOK __stdcall QServerSetBlockHook(QSERVER *ptServer, QSADDRESSHOOK pfnHook)
{
	ptServer->pfnBlockHook = pfnHook;
	return pfnHook;
}

QSADDRESSHOOK __stdcall QServerSetUnusedHook(QSERVER *ptServer, QSADDRESSHOOK pfnHook)
{
	ptServer->field_302c = pfnHook;
	return pfnHook;
}

void __stdcall QServerSetClientValue(QSERVER *ptServer, SOCKET hSocket, DWORD dwValue)
{
	QSCLIENT *ptClient;

	if (!hSocket)
		return;
	ptClient = QServerFindClient(ptServer, hSocket);
	if (ptClient)
		ptClient->dwOwner = dwValue;
}

DWORD __stdcall QServerGetClientValue(QSERVER *ptServer, SOCKET hSocket)
{
	QSCLIENT *ptClient;

	if (!hSocket)
		return 0;
	ptClient = QServerFindClient(ptServer, hSocket);
	if (!ptClient)
		return 0;
	return ptClient->dwOwner;
}

static __inline void QServerFreeBlocks(QSERVER *ptServer)
{
	QSBLOCK *ptBlock;
	QSBLOCK *ptFree;

	ptBlock = ptServer->ptBlocks;
	while (ptBlock)
	{
		ptFree = ptBlock;
		ptBlock = ptBlock->ptNext;
#line 1042
		D2FREE2(ptFree);
	}
}

void __fastcall QServerClientAddRef(QSCLIENT *ptClient)
{
	InterlockedIncrement(&ptClient->lRefCount);
}

void __fastcall QServerClientRelease(QSCLIENT *ptClient)
{
	EnterCriticalSection(&ptClient->tLock);
	if (InterlockedDecrement(&ptClient->lRefCount) <= 0)
	{
		DeleteCriticalSection(&ptClient->tLock);
#line 1073
		D2FREE2(ptClient);
		return;
	}
	LeaveCriticalSection(&ptClient->tLock);
}

static __inline void QServerAddAddress(QSERVER *ptServer, DWORD dwAddress)
{
	QSADDRESS *ptEntry;
	DWORD dwBucket;

	dwBucket = dwAddress % QSERVER_IP_HASH;
	for (ptEntry = ptServer->aptAddresses[dwBucket]; ptEntry; ptEntry = ptEntry->ptNext)
	{
		if (ptEntry->dwAddress == dwAddress)
		{
			ptEntry->nNumConnections++;
			return;
		}
	}
#line 1185
	ptEntry = (QSADDRESS *)D2ALLOC2(sizeof(QSADDRESS));
	ptEntry->dwAddress = dwAddress;
	ptEntry->nNumConnections = 1;
	ptEntry->ptNext = ptServer->aptAddresses[dwBucket];
	ptServer->aptAddresses[dwBucket] = ptEntry;
}

static __inline void QServerReleaseAddress(QSERVER *ptServer, DWORD dwAddress)
{
	QSADDRESS *ptEntry;
	QSADDRESS *ptPrev;
	DWORD dwBucket;

	dwBucket = dwAddress % QSERVER_IP_HASH;
	ptPrev = NULL;
	for (ptEntry = ptServer->aptAddresses[dwBucket]; ptEntry; ptEntry = ptEntry->ptNext)
	{
		if (ptEntry->dwAddress == dwAddress)
		{
			if (--ptEntry->nNumConnections > 0)
				return;
			if (ptPrev)
				ptPrev->ptNext = ptEntry->ptNext;
			else
				ptServer->aptAddresses[dwBucket] = ptEntry->ptNext;
#line 1219
			D2FREE2(ptEntry);
			return;
		}
		ptPrev = ptEntry;
	}
}

static __inline void __fastcall QServerUnlinkClient(QSERVER *ptServer, SOCKET hSocket)
{
	QSCLIENT *ptClient;
	QSCLIENT *ptPrev;
	DWORD dwBucket;

	dwBucket = hSocket & (QSERVER_CLIENT_HASH - 1);
	EnterCriticalSection(&ptServer->tLock);
	ptClient = ptServer->aptClients[dwBucket];
	ptPrev = NULL;
	for (; ptClient; ptClient = ptClient->ptNext)
	{
		if (ptClient->hSocket == hSocket)
		{
			if (ptClient != ptServer->aptClients[dwBucket])
				ptPrev->ptNext = ptClient->ptNext;
			else
				ptServer->aptClients[dwBucket] = ptClient->ptNext;
			QServerReleaseAddress(ptServer, ptClient->dwAddress);
			QServerClientRelease(ptClient);
			ptServer->nNumClients--;
			LeaveCriticalSection(&ptServer->tLock);
			return;
		}
		ptPrev = ptClient;
	}
	LeaveCriticalSection(&ptServer->tLock);
}

static __inline void QServerExpireRates(QSERVER *ptServer, DWORD dwNow)
{
	QSRATE *ptRate;
	QSRATE *ptPrev;

	while (ptServer->ptRateHead && dwNow - ptServer->ptRateHead->dwWindowTick > 15000)
	{
		ptRate = ptServer->ptRateHead;
		ptServer->ptRateHead = ptRate->ptNext;
#line 1245
		D2FREE2(ptRate);
	}
	if (!ptServer->ptRateHead)
		return;
	ptPrev = ptServer->ptRateHead;
	for (ptRate = ptPrev->ptNext; ptRate; ptRate = ptRate->ptNext)
	{
		if (dwNow - ptRate->dwWindowTick > 15000)
		{
			ptPrev->ptNext = ptRate->ptNext;
#line 1261
			D2FREE2(ptRate);
		}
		ptPrev = ptRate;
	}
}

static __inline void QServerFreeRates(QSERVER *ptServer)
{
	QSRATE *ptRate;
	QSRATE *ptFree;

	ptRate = ptServer->ptRateHead;
	while (ptRate)
	{
		ptFree = ptRate;
		ptRate = ptRate->ptNext;
#line 1283
		D2FREE2(ptFree);
	}
	ptServer->ptRateHead = NULL;
}

BOOL __stdcall QServerInject(QSERVER *ptServer, BYTE *pbData, int nBytes, SOCKET hSocket)
{
	QSMSG *ptMsg;
	int nPacketSize;
	int nHeaderSize;
	int nMsgType;
	int nFlood;
	int nResult;

	nHeaderSize = 0;
	nPacketSize = 0;
	nMsgType = 0;
	nFlood = 0;
	nResult = ptServer->pfnParse(pbData, nBytes, &nPacketSize, &nHeaderSize, &nMsgType, &nFlood, 0, 0);
	if (nResult != 2 && nResult != 1)
		return FALSE;
	if (ptServer->ptFreeMsgs)
	{
		EnterCriticalSection(&ptServer->tLock);
		ptMsg = ptServer->ptFreeMsgs;
		if (ptMsg)
		{
			ptServer->ptFreeMsgs = ptMsg->ptNext;
			LeaveCriticalSection(&ptServer->tLock);
		}
		else
		{
#line 1357
			ptMsg = (QSMSG *)D2ALLOC2(sizeof(QSMSG));
			LeaveCriticalSection(&ptServer->tLock);
		}
	}
	else
	{
#line 1366
		ptMsg = (QSMSG *)D2ALLOC2(sizeof(QSMSG));
	}
	memcpy(ptMsg->abData, pbData + nHeaderSize, nBytes - nHeaderSize);
	ptMsg->nDataSize = nBytes - nHeaderSize;
	ptMsg->dwArrivalTick = GetTickCount();
	ptMsg->ptNext = NULL;
	ptMsg->hSocket = hSocket;
	if (nMsgType < ptServer->nNumMsgLists)
	{
		EnterCriticalSection(&ptServer->tLock);
		if (!ptServer->pptMsgTails[nMsgType])
			ptServer->pptMsgHeads[nMsgType] = ptMsg;
		else
			ptServer->pptMsgTails[nMsgType]->ptNext = ptMsg;
		ptServer->pptMsgTails[nMsgType] = ptMsg;
		LeaveCriticalSection(&ptServer->tLock);
	}
	return TRUE;
}

BOOL __stdcall QServerSetBlocking(QSERVER *ptServer, BOOL fBlocking)
{
	ptServer->fBlockAddresses = fBlocking;
	return fBlocking;
}
