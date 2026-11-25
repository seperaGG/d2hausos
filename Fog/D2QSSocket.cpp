#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\D2QSSocket.cpp"
#include "FogLocal.h"
#include <winsock.h>
#include "FogLocalAsync.h"
#include "StormImports.h"
#include "Safesock.h"
#include "D2QSQueue.h"
#include "D2Macros.h"

#define MAXBUFFERSIZE	8192

typedef BOOL (__fastcall *D2QSDISPATCH)(BYTE *pbPacket, DWORD dwBytes, void *pContext);

struct D2QSSOCKET
{
 	HANDLE		hConnectThread;
 	DWORD		dwConnectThreadId;
 	char		szIpAddr[260];
 	DWORD		dwPort;
 	SAFESOCK	*ptConnection;
 	HANDLE		hMsgEvent;
 	int			nHeaderSize;
 	int			nSizeFieldOffset;
 	BOOL		fStop;
 	char		szName[64];
 	D2QSDISPATCH	fpDispatcher;
 	void		*pContext;
 	D2QSQUEUE	tQueue;
};

struct D2QSMSGHEADER
{
 	WORD		wBytes;
};

void __stdcall D2QSSocketSetDispatcher(D2QSSOCKET *pSocket, D2QSDISPATCH fpDispatcher, void *pContext)
{
	if (pSocket)
	{
		pSocket->fpDispatcher = fpDispatcher;
		pSocket->pContext = pContext;
	}
}

BOOL __stdcall D2QSSocketSend(D2QSSOCKET *pSocket, const BYTE *pbData, WORD wBytes)
{
	D2QSMSGHEADER tMsg;
	BYTE abBuffer[MAXBUFFERSIZE];
	DWORD nTotal;

	if (!pSocket)
		return FALSE;
	tMsg.wBytes = wBytes + sizeof(tMsg.wBytes);
#line 48
	D2ASSERT(tMsg.wBytes < MAXBUFFERSIZE);
	if (!pSocket->ptConnection)
		return FALSE;
	if (!pbData)
		wBytes = 0;
	nTotal = wBytes + sizeof(tMsg.wBytes);
	StormSMemCopy(abBuffer, &tMsg.wBytes, sizeof(tMsg.wBytes));
	if (pbData && nTotal < MAXBUFFERSIZE)
		StormSMemCopy(abBuffer + sizeof(tMsg.wBytes), pbData, wBytes);
	SafesockSend(pSocket->ptConnection, (const char *)abBuffer, nTotal);
	return TRUE;
}

BOOL __stdcall D2QSSocketSendRaw(D2QSSOCKET *pSocket, const BYTE *pbData, WORD wBytes)
{
	if (!pSocket)
		return FALSE;
	if (!pSocket->ptConnection)
		return FALSE;
	SafesockLock(pSocket->ptConnection);
	if (pbData && wBytes)
		SafesockSendUnlocked(pSocket->ptConnection, (const char *)pbData, wBytes);
	SafesockUnlock(pSocket->ptConnection);
	return TRUE;
}

DWORD __stdcall D2QSSocketReceive(D2QSSOCKET *pSocket, BYTE *pbBuffer, WORD wMax)
{
	if (!pSocket)
		return 0;
	return D2QSQueuePop(&pSocket->tQueue, pbBuffer, wMax);
}

int __fastcall D2QSSocketDispatch(D2QSSOCKET *pSocket, BYTE *pbBuffer, int nBytes)
{
	BYTE *pbPacket;
	BYTE *pbData;
	int nPacketSize;
	int nLeft;
	int nData;
	BOOL fTaken;

	pbPacket = pbBuffer;
	while (1)
	{
		nLeft = pbBuffer + nBytes - pbPacket;
		nPacketSize = *(WORD *)(pbPacket + pSocket->nSizeFieldOffset);
		if (nLeft < pSocket->nHeaderSize || nLeft < nPacketSize)
			break;
#line 134
		D2ASSERT(nPacketSize < MAXBUFFERSIZE);
		pbData = pbPacket + pSocket->nHeaderSize;
		nData = nPacketSize - pSocket->nHeaderSize;
		if (pSocket->fpDispatcher)
		{
#line 140
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)pSocket->fpDispatcher), "pSocket->fpDispatcher");
			if (pSocket->nSizeFieldOffset > 0)
				fTaken = pSocket->fpDispatcher(pbPacket, nPacketSize, pSocket->pContext);
			else
				fTaken = pSocket->fpDispatcher(pbData, (WORD)nData, pSocket->pContext);
			if (!fTaken)
			{
				if (pSocket->nSizeFieldOffset > 0)
					D2QSQueuePush(&pSocket->tQueue, pbPacket, (WORD)nPacketSize);
				else
					D2QSQueuePush(&pSocket->tQueue, pbData, (WORD)nData);
				SetEvent(pSocket->hMsgEvent);
			}
		}
		else
		{
			D2QSQueuePush(&pSocket->tQueue, pbData, (WORD)nData);
			SetEvent(pSocket->hMsgEvent);
		}
		pbPacket += nPacketSize;
	}
	nLeft = pbBuffer + nBytes - pbPacket;
	memcpy(pbBuffer, pbPacket, nLeft);
	return nLeft;
}

void __stdcall D2QSSocketSetHeader(D2QSSOCKET *pSocket, int nHeaderSize, int nSizeOffset)
{
	if (pSocket)
	{
		pSocket->nHeaderSize = nHeaderSize;
		pSocket->nSizeFieldOffset = nSizeOffset;
	}
}

void __stdcall D2QSSocketStopUnused(D2QSSOCKET *pSocket)
{
	if (pSocket && pSocket->ptConnection && pSocket->ptConnection->hThread)
		pSocket->fStop = TRUE;
}

DWORD __stdcall D2QSSocketReceiveThread(void *pContext)
{
	D2QSSOCKET *pSocket;
	DWORD dwStep;
	DWORD dwError;
	BYTE abBuffer[8196];
	DWORD dwGot;
	DWORD dwRoom;
	int nHave;
	BOOL fReadable;

	pSocket = (D2QSSOCKET *)pContext;
	nHave = 0;
	while (!pSocket->ptConnection)
		Sleep(0);
	while (!pSocket->fStop)
	{
		if (!pSocket->ptConnection || pSocket->ptConnection->fClosing
			|| pSocket->ptConnection->dwFailedStep)
			break;
		fReadable = SafesockCanRead(pSocket->ptConnection);
		if (!pSocket->ptConnection || pSocket->ptConnection->fClosing)
			break;
		if (fReadable)
		{
			while (!pSocket->fStop)
			{
				dwGot = SafesockRecv(pSocket->ptConnection, (char *)abBuffer + nHave, MAXBUFFERSIZE - nHave);
				SafesockGetError(pSocket->ptConnection, &dwStep, &dwError);
				if (dwStep || dwGot <= 0)
				{
					if (pSocket && pSocket->ptConnection && pSocket->ptConnection->hThread)
					{
						SafesockClose(pSocket->ptConnection);
						pSocket->ptConnection = NULL;
					}
					return 0;
				}
				dwRoom = MAXBUFFERSIZE - nHave;
				nHave = D2QSSocketDispatch(pSocket, abBuffer, dwGot + nHave);
				if (dwGot < dwRoom)
					break;
			}
		}
	}
	if (pSocket && pSocket->ptConnection && pSocket->ptConnection->hThread)
	{
		SafesockClose(pSocket->ptConnection);
		pSocket->ptConnection = NULL;
	}
	return 0;
}
DWORD __stdcall D2QSSocketConnectThread(void *pContext)
{
	D2QSSOCKET *pSocket;

	pSocket = (D2QSSOCKET *)pContext;
	pSocket->ptConnection = SafesockConnect(pSocket->szIpAddr, pSocket->dwPort,
		D2QSSocketReceiveThread, pSocket, pSocket->szName);
	return TRUE;
}

void __stdcall D2QSSocketStart(D2QSSOCKET *pSocket)
{
	if (!pSocket || pSocket->hConnectThread)
		return;
	pSocket->fStop = FALSE;
	pSocket->hMsgEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	pSocket->hConnectThread = CreateThread(NULL, 0, D2QSSocketConnectThread, pSocket, 0,
		&pSocket->dwConnectThreadId);
	while (WaitForSingleObject(pSocket->hConnectThread, 0))
		Sleep(10);
}
HANDLE __stdcall D2QSSocketStartAsync(D2QSSOCKET *pSocket)
{
	if (!pSocket)
		return NULL;
	if (pSocket->hConnectThread)
		return NULL;
	pSocket->fStop = FALSE;
	pSocket->hMsgEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	pSocket->hConnectThread = CreateThread(NULL, 0, D2QSSocketConnectThread, pSocket, 0,
		&pSocket->dwConnectThreadId);
	return pSocket->hConnectThread;
}
void __stdcall D2QSSocketStop(D2QSSOCKET *pSocket)
{
	if (pSocket && pSocket->ptConnection && pSocket->ptConnection->hThread)
		pSocket->fStop = TRUE;
}

D2QSSOCKET * __stdcall D2QSSocketCreate(const char *szIpAddr, DWORD nPort, const char *pszName)
{
	D2QSSOCKET *pSocket;
	int i;

#line 425
	D2ASSERT(szIpAddr);
#line 426
	D2ASSERT(strlen(szIpAddr) < MAX_PATH-1);
#line 428
	pSocket = (D2QSSOCKET *)D2ALLOC2(sizeof(D2QSSOCKET));
	if (pSocket)
	{
		for (i = 0; i < (int)(sizeof(D2QSSOCKET) / sizeof(DWORD)); i++)
			((DWORD *)pSocket)[i] = 0;
		StormSStrCopy(pSocket->szIpAddr, szIpAddr, sizeof(pSocket->szIpAddr));
		if (pszName)
			StormSStrCopy(pSocket->szName, pszName, sizeof(pszName));
		else
			StormSStrCopy(pSocket->szName, "UNKNOWN", sizeof(pSocket->szName));
		pSocket->dwPort = nPort;
		D2QSQueueInit(&pSocket->tQueue);
		pSocket->nHeaderSize = 2;
		pSocket->nSizeFieldOffset = 0;
	}
	return pSocket;
}

void __stdcall D2QSSocketDestroy(D2QSSOCKET *pSocket)
{
	if (!pSocket)
		return;
	if (pSocket->ptConnection && pSocket->ptConnection->hThread)
		pSocket->fStop = TRUE;
	if (pSocket->ptConnection)
	{
		ResetEvent(pSocket->ptConnection->hThread);
		pSocket->fStop = TRUE;
		while (1)
			if (!pSocket->ptConnection || !WaitForSingleObject(pSocket->ptConnection->hThread, 10))
				break;
	}

	D2QSQueueDestroy(&pSocket->tQueue);
	if (pSocket->ptConnection && pSocket->ptConnection->hThread)
	{
		SafesockClose(pSocket->ptConnection);
		pSocket->ptConnection = NULL;
	}
#line 475
	D2FREE2(pSocket);
}

DWORD __stdcall D2QSSocketWait(D2QSSOCKET *pSocket, DWORD dwMilliseconds)
{
	if (!pSocket)
		return 0;
	return WaitForSingleObject(pSocket->hMsgEvent, dwMilliseconds);
}

SAFESOCK * __stdcall D2QSSocketGetConnection(D2QSSOCKET *pSocket)
{
	if (!pSocket)
		return NULL;
	return pSocket->ptConnection;
}

void __stdcall D2QSSocketSignal(D2QSSOCKET *pSocket)
{
	if (pSocket)
		SetEvent(pSocket->hMsgEvent);
}

int __stdcall D2QSSocketReadSevenBitPair(const BYTE *pbData)
{
	return ((pbData[1] & 127) << 7) + (pbData[0] & 127);
}

void __stdcall D2QSSocketWriteSevenBitPair(BYTE *pbData, DWORD dwValue)
{
	pbData[0] = (BYTE)dwValue | 128;
	pbData[1] = (BYTE)(dwValue >> 7) | 128;
}

void __fastcall D2QSSocketNop(void)
{
}
