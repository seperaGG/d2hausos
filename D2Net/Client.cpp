#define D2_THIS_FILE "C:\\D2\\Source\\D2Net\\SRC\\Client.cpp"
#include <string.h>
#include "D2Net.h"
#include "FogImports.h"
#include "StormImports.h"
#include "D2Macros.h"

struct D2NETCLIENT
{
 	DWORD		unobserved_00[5];
 	BYTE		abBuffer[1976];
 	DWORD		dwBufferedBytes;
 	DWORD		unobserved_7d0;
};

static CRITICAL_SECTION sgtClientLock;
static D2NETMSG *sgptQueue0;
static SOCKADDR_IN sgtServerAddr;
static D2NETMSG *sgptQueue1;
static D2NETMSG *sgptQueue2;
static D2NETCLIENT *sgptClient;
static HANDLE sghReceiveThread;
static SOCKET sghSocket;
static HANDLE sghConnectThread;
static WSADATA sgtWsaData;
static DWORD sgdwThreadId;
static BOOL sgfClientStop;
static DWORD sgdwClientState;

DWORD WINAPI ClientReceiveThread(void *pParam);

void __fastcall ClientSetState(DWORD dwState)
{
	if (!D2NetIsLocal())
	{
		EnterCriticalSection(&sgtClientLock);
		sgdwClientState = dwState;
		LeaveCriticalSection(&sgtClientLock);
	}
}

DWORD __cdecl D2NetGetRemoteConnection(void)
{
	DWORD dwState;

	if (D2NetIsLocal())
		return 1;
	if (!sgptClient)
		return 0;
	EnterCriticalSection(&sgtClientLock);
	dwState = sgdwClientState;
	LeaveCriticalSection(&sgtClientLock);
	return dwState;
}

DWORD WINAPI ClientConnectThread(void *pParam)
{
	sgfClientStop = FALSE;
	if (WSAStartup(257, &sgtWsaData))
	{
#line 104
		D2FATAL("WSAStartup");
	}
	sghSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (sghSocket == INVALID_SOCKET)
	{
		WSAGetLastError();
#line 110
		D2FATAL("Socket");
	}
	memset(&sgtServerAddr, 0, sizeof(sgtServerAddr));
	sgtServerAddr.sin_family = AF_INET;
	sgtServerAddr.sin_port = htons(4000);
	sgtServerAddr.sin_addr.s_addr = inet_addr(FogSafesockResolve(pParam));
	if (connect(sghSocket, (SOCKADDR *)&sgtServerAddr, sizeof(sgtServerAddr)) == SOCKET_ERROR)
	{
		WSAGetLastError();
		sghSocket = 0;
	}
	return 1;
}

int __cdecl ClientPollConnect(void)
{
	if (D2NetIsLocal())
		return 2;
	if (!WaitForSingleObject(sghConnectThread, 100))
	{
		if (sghSocket)
		{
			ClientSetState(1);
			sghReceiveThread = CreateThread(NULL, 0, ClientReceiveThread, NULL, 0, &sgdwThreadId);
			SetThreadPriority(sghReceiveThread, THREAD_PRIORITY_ABOVE_NORMAL);
			return 2;
		}
		return 0;
	}
	return 1;
}

DWORD WINAPI D2NetClientInit(int nMode, const char *pszHost)
{
	sgptQueue1 = NULL;
	sgptQueue0 = NULL;
	sgptQueue2 = NULL;
	InitializeCriticalSection(&sgtClientLock);
#line 190
	sgptClient = (D2NETCLIENT *)D2ALLOC2(sizeof(D2NETCLIENT));
	memset(sgptClient, 0, sizeof(D2NETCLIENT));
	D2NetSetMode(nMode);
	if (D2NetIsLocal())
	{
		D2NetLocalAck();
		return TRUE;
	}
	sghConnectThread = CreateThread(NULL, 0, ClientConnectThread, (void *)pszHost, 0, &sgdwThreadId);
	return (DWORD)(INT_PTR)sghConnectThread;
}

int __cdecl D2NetClientClose(void)
{
	int nResult;

	EnterCriticalSection(&sgtClientLock);
	if (sgptClient)
	{
#line 211
		D2FREE2(sgptClient);
	}
	sgptClient = NULL;
	sgfClientStop = TRUE;
	LeaveCriticalSection(&sgtClientLock);
	WaitForSingleObject(sghReceiveThread, 6000);
	D2NetFlush(&sgtClientLock, &sgptQueue0);
	D2NetFlush(&sgtClientLock, &sgptQueue1);
	EnterCriticalSection(&sgtClientLock);
	DeleteCriticalSection(&sgtClientLock);
	nResult = D2NetIsLocal();
	if (!nResult)
	{
		nResult = closesocket(sghSocket);
		sghSocket = 0;
		if (nResult == SOCKET_ERROR)
			return WSAGetLastError();
	}
	return nResult;
}

void __cdecl operator delete(void *pMemory)
{
	if (pMemory)
		StormSMemFree(pMemory, "delete", -1, 0);
}

void * __cdecl operator new(unsigned int nSize)
{
	return StormSMemAlloc(nSize, "new", -1, 0);
}

int WINAPI ClientReceiveGameMessage(BYTE *pbBuffer, DWORD dwSize)
{
	return D2NetDequeue(&sgtClientLock, &sgptQueue1, pbBuffer, dwSize);
}

int WINAPI D2NetGetCltSysMsg(BYTE *pbBuffer, DWORD dwSize)
{
	return D2NetDequeue(&sgtClientLock, &sgptQueue0, pbBuffer, dwSize);
}

int WINAPI ClientReceiveOtherQueue(BYTE *pbBuffer, DWORD dwSize)
{
	return D2NetDequeue(&sgtClientLock, &sgptQueue2, pbBuffer, dwSize);
}

DWORD WINAPI ClientReceiveThread(void *pParam)
{
	fd_set tRead;
	fd_set tExcept;
	struct timeval tTimeout;
	D2NETHDR *ptHdr;
	D2NETMSG *ptMsg;
	D2NETMSG *ptTail;
	BOOL fStop;
	int nResult;
	WORD msgSize;
	int nLen;
	BYTE *pbCmd;
	DWORD dwOffset;

	while (1)
	{
		EnterCriticalSection(&sgtClientLock);
		fStop = sgfClientStop;
		LeaveCriticalSection(&sgtClientLock);
		if (fStop)
		{
			OutputDebugStringA("Client thread close #2\n");
			ExitThread(0);
		}
		FD_ZERO(&tRead);
		FD_SET(sghSocket, &tRead);
		FD_ZERO(&tExcept);
		FD_SET(sghSocket, &tExcept);
		tTimeout.tv_sec = 0;
		tTimeout.tv_usec = 100000;
		nResult = select(0, &tRead, NULL, &tExcept, &tTimeout);
		if (nResult == 0)
		{
			EnterCriticalSection(&sgtClientLock);
			if (sgfClientStop)
			{
				sgfClientStop = FALSE;
				LeaveCriticalSection(&sgtClientLock);
				OutputDebugStringA("Client thread close #3\n");
				ExitThread(0);
			}
			LeaveCriticalSection(&sgtClientLock);
		}
		else if (nResult != SOCKET_ERROR)
		{
			if (!FD_ISSET(sghSocket, &tExcept) && FD_ISSET(sghSocket, &tRead))
			{
				EnterCriticalSection(&sgtClientLock);
				if (sgfClientStop)
				{
					sgfClientStop = FALSE;
					LeaveCriticalSection(&sgtClientLock);
					OutputDebugStringA("Client thread close #4\n");
					ExitThread(0);
				}
				dwOffset = sgptClient->dwBufferedBytes;
				nResult = recv(sghSocket, (char *)sgptClient->abBuffer + dwOffset, 1460, 0);
				if (nResult == SOCKET_ERROR)
				{
					LeaveCriticalSection(&sgtClientLock);
					WSAGetLastError();
					ClientSetState(0);
					OutputDebugStringA("Client thread close #5\n");
					ExitThread(0);
				}
				if (nResult == 0)
				{
					LeaveCriticalSection(&sgtClientLock);
					ClientSetState(0);
					OutputDebugStringA("Client thread close #6\n");
					ExitThread(0);
				}
				sgptClient->dwBufferedBytes += nResult;
				for (;;)
				{
					if (sgfClientStop)
					{
						OutputDebugStringA("Client thread close #1\n");
						ExitThread(0);
					}
					if (sgptClient->dwBufferedBytes < 4)
						break;
					ptHdr = (D2NETHDR *)sgptClient->abBuffer;
					msgSize = ptHdr->wSize;
		#line 307
					D2ASSERT(msgSize <= MAX_MSG_SIZE);
					if (msgSize > (int)sgptClient->dwBufferedBytes)
						break;
		#line 312
					ptMsg = (D2NETMSG *)D2ALLOC2(sizeof(D2NETMSG));
					memcpy(ptMsg, sgptClient->abBuffer + 4, msgSize - 4);
					ptMsg->nSize = msgSize - 4;
					ptMsg->dwQueuedTick = 0;
					ptMsg->ptNext = NULL;
					pbCmd = &ptMsg->abData[2];
					if (*pbCmd == 130)
						*(DWORD *)&ptMsg->abData[15] = GetTickCount();
					if (ptHdr->bQueue == 1)
						ptTail = sgptQueue1;
					else if (ptHdr->bQueue == 0)
						ptTail = sgptQueue0;
					else
						ptTail = sgptQueue2;
					if (!ptTail)
					{
						if (ptHdr->bQueue == 1)
							sgptQueue1 = ptMsg;
						else if (ptHdr->bQueue == 0)
							sgptQueue0 = ptMsg;
						else
							sgptQueue2 = ptMsg;
					}
					else
					{
						while (ptTail->ptNext)
							ptTail = ptTail->ptNext;
						ptTail->ptNext = ptMsg;
					}
					nLen = msgSize;
					memcpy(sgptClient->abBuffer, sgptClient->abBuffer + nLen,
						sizeof(sgptClient->abBuffer) - nLen);
					sgptClient->dwBufferedBytes -= nLen;
				}
				LeaveCriticalSection(&sgtClientLock);
			}
		}
		else
		{
			WSAGetLastError();
			ClientSetState(0);
		}
	}
}

int WINAPI D2NetSendToServer(BYTE bQueue, const BYTE *pbData, int nSize)
{
	D2NETPKT tPkt;
	int nResult;

	if (!sgptClient)
		return 0;
	if (D2NetIsLocal())
	{
		tPkt.tHdr.bQueue = bQueue;
		tPkt.tHdr.bMagic = D2NET_MAGIC;
		tPkt.tHdr.wSize = (WORD)(nSize + 4);
#line 472
		D2ASSERT(tPkt.tHdr.wSize <= MAX_MSG_SIZE);
		memcpy(tPkt.abData, pbData, nSize);
		return ServerLocalSend(&tPkt);
	}
	tPkt.tHdr.bMagic = D2NET_MAGIC;
	tPkt.tHdr.bQueue = bQueue;
	tPkt.tHdr.wSize = (WORD)(nSize + 4);
#line 493
	D2ASSERT(tPkt.tHdr.wSize <= MAX_MSG_SIZE);
	memcpy(tPkt.abData, pbData, nSize);
	nResult = send(sghSocket, (const char *)&tPkt, nSize + 4, 0);
	if (nResult == SOCKET_ERROR)
	{
		WSAGetLastError();
		ClientSetState(0);
		return 0;
	}
	return nResult;
}

void __fastcall ClientLocalReceive(const D2NETPKT *ptPkt)
{
	D2NETMSG *ptMsg;
	D2NETMSG *ptTail;
	WORD packetSize;
	int nSize;

#line 533
	ptMsg = (D2NETMSG *)D2ALLOC2(sizeof(D2NETMSG));
	packetSize = ptPkt->tHdr.wSize;
#line 535
	D2ASSERT(packetSize <= MAX_MSG_SIZE);
	nSize = packetSize - 4;
#line 537
	D2ASSERT(nSize > 0);
	memcpy(ptMsg, ptPkt->abData, nSize);
	ptMsg->nSize = nSize;
	ptMsg->dwQueuedTick = GetTickCount();
	ptMsg->ptNext = NULL;
	ptTail = ptPkt->tHdr.bQueue == 1 ? sgptQueue1 : sgptQueue0;
	if (!ptTail)
	{
		if (ptPkt->tHdr.bQueue == 1)
			sgptQueue1 = ptMsg;
		else
			sgptQueue0 = ptMsg;
		return;
	}
	while (ptTail->ptNext)
		ptTail = ptTail->ptNext;
	ptTail->ptNext = ptMsg;
}

void WINAPI ClientGetLocalAddress(char *pszAddress)
{
	SOCKADDR_IN tAddr;
	int nLength;
	char *pszText;

	memset(&tAddr, 0, sizeof(tAddr));
	nLength = sizeof(tAddr);
	getsockname(sghSocket, (SOCKADDR *)&tAddr, &nLength);
	pszText = inet_ntoa(tAddr.sin_addr);
	StormSStrCopy(pszAddress, pszText, 2147483647);
}
