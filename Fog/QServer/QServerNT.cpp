#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\QServer\\QServerNT.cpp"
#include "FogLocal.h"
#include <winsock.h>
#include "QServer.h"
#include "D2Macros.h"

static BOOL __fastcall QServerNTRead(QSERVER *ptServer, QSCLIENT *ptClient);
static __inline void QServerNTQueue(QSCLIENT *ptClient, const BYTE *pbData, DWORD dwBytes);
static __inline void QServerNTWriteDone(QSCLIENT *ptClient);
static __inline void QServerNTWriteNext(QSERVER *ptServer, QSCLIENT *ptClient);

SOCKET __fastcall QServerNTStart(QSERVER *ptServer, u_short nPort)
{
	WSADATA tWsaData;
	SOCKADDR_IN tAddress;
	BOOL fNoDelay;
	BOOL fKeepAlive;
	u_long dwNonBlocking;

	ptServer->nAddressLimit = 1;
	if (WSAStartup(257, &tWsaData))
		return 0;
	ptServer->hPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
#line 47
	D2ASSERT(ptServer->hPort);
	ptServer->hListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ptServer->hListenSocket == INVALID_SOCKET)
		return 0;
	fNoDelay = TRUE;
	setsockopt(ptServer->hListenSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&fNoDelay, sizeof(fNoDelay));
	fKeepAlive = TRUE;
	setsockopt(ptServer->hListenSocket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&fKeepAlive,
		sizeof(fKeepAlive));
	dwNonBlocking = 1;
	ioctlsocket(ptServer->hListenSocket, FIONBIO, &dwNonBlocking);
	memset(&tAddress, 0, sizeof(tAddress));
	tAddress.sin_family = AF_INET;
	tAddress.sin_port = htons(nPort);
	if (bind(ptServer->hListenSocket, (SOCKADDR *)&tAddress, sizeof(tAddress)) == SOCKET_ERROR)
	{
		closesocket(ptServer->hListenSocket);
		return 0;
	}
	if (listen(ptServer->hListenSocket, 100) == SOCKET_ERROR)
	{
		closesocket(ptServer->hListenSocket);
		return 0;
	}
	return ptServer->hListenSocket;
}
void __fastcall QServerNTStop(QSERVER *ptServer)
{
}

BOOL __fastcall QServerNTSend(QSERVER *ptServer, QSCLIENT *ptClient, const BYTE *pbData, DWORD dwBytes)
{
	SOCKET hSocket;
	DWORD dwSent;

	int nSent;

	EnterCriticalSection(&ptClient->tLock);
	if (ptClient->ptSendQueue)
	{
		QServerNTQueue(ptClient, pbData, dwBytes);
	}
	else
	{
		nSent = send(ptClient->hSocket, (const char *)pbData, dwBytes, 0);
		if (nSent == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				hSocket = ptClient->hSocket;
				LeaveCriticalSection(&ptClient->tLock);
				QServerRemoveClient(ptServer, hSocket);
				return FALSE;
			}
			nSent = 0;
		}
		dwSent = nSent;
		if (dwSent < dwBytes)
		{
			QServerNTQueue(ptClient, pbData + dwSent, dwBytes - dwSent);
			QServerNTWriteNext(ptServer, ptClient);
		}
	}
	LeaveCriticalSection(&ptClient->tLock);
	return TRUE;
}

BOOL __fastcall QServerNTPump(QSERVER *ptServer, DWORD dwMilliseconds)
{
	DWORD dwNow;
	BOOL fReceived;

	fReceived = FALSE;
	dwNow = GetTickCount();
	if (dwNow - ptServer->dwLastAcceptTick > 100)
	{
		fd_set tListen;
		struct timeval tTimeout;
		SOCKADDR_IN tAddress;
		SOCKET hAccepted;
		u_long dwNonBlocking;
		QSCLIENT *ptNew;
		int nLength;

		QServerAgeBlocks(ptServer);
		ptServer->dwLastAcceptTick = dwNow;
		tListen.fd_array[0] = ptServer->hListenSocket;
		tListen.fd_count = 1;
		tTimeout.tv_sec = 0;
		tTimeout.tv_usec = 0;
		if (select(0, &tListen, NULL, NULL, &tTimeout))
		{
			memset(&tAddress, 0, sizeof(tAddress));
			nLength = sizeof(tAddress);
			hAccepted = accept(ptServer->hListenSocket, (SOCKADDR *)&tAddress, &nLength);
			if (hAccepted != INVALID_SOCKET)
			{
				if (QServerIsBlocked(ptServer, tAddress.sin_addr))
					closesocket(hAccepted);
				else if (QServerAddressFull(ptServer, &tAddress.sin_addr.s_addr))
				{
					QServerDropAddress(ptServer, &tAddress.sin_addr.s_addr);
					closesocket(hAccepted);
				}
				else if (!QServerAllowConnect(ptServer, &tAddress.sin_addr.s_addr))
				{
					QServerBlockAddress(ptServer, tAddress.sin_addr);
					closesocket(hAccepted);
				}
				else
				{
					ptNew = QServerAddClient(ptServer, hAccepted, tAddress.sin_addr.s_addr);
					dwNonBlocking = 1;
					ioctlsocket(hAccepted, FIONBIO, &dwNonBlocking);
					CreateIoCompletionPort((HANDLE)hAccepted, ptServer->hPort, (DWORD)ptNew, 0);
					QServerNTRead(ptServer, ptNew);
					if (ptServer->pfnAccept)
						ptServer->pfnAccept(ptServer, hAccepted);
				}
			}
		}
	}
	for (;;)
	{
		QSCLIENT *ptClient;
		OVERLAPPED *ptOverlapped;
		DWORD dwBytes;
		DWORD dwError;
		BOOL fDone;

		ptClient = NULL;
		fDone = GetQueuedCompletionStatus(ptServer->hPort, &dwBytes, (DWORD *)&ptClient, &ptOverlapped,
			dwMilliseconds);
		dwError = GetLastError();
		if (fDone)
		{
			if (!ptClient)
				break;
			if (!dwBytes)
			{
				QServerRemoveClient(ptServer, ptClient->hSocket);
				dwMilliseconds = 0;
			}
			else
			{
				if (ptOverlapped == &ptClient->tWriteOverlapped)
				{
					QServerNTWriteDone(ptClient);
					QServerNTWriteNext(ptServer, ptClient);
				}
				else if (ptOverlapped == &ptClient->tReadOverlapped)
				{
					ptClient->nMsgBuffSize += dwBytes;
					QServerParse(ptServer, ptClient);
					fReceived = TRUE;
					QServerNTRead(ptServer, ptClient);
				}
				else
				{
#line 242
					D2ASSERT(0);
				}
				QServerClientRelease(ptClient);
				dwMilliseconds = 0;
			}
		}
		else if (ptOverlapped && ptClient)
		{
			QServerRemoveClient(ptServer, ptClient->hSocket);
			dwMilliseconds = 0;
		}
		else
		{
			if (dwError == WAIT_TIMEOUT)
				break;
#line 259
			D2ASSERT(0);
		}
	}
	return fReceived;
}

static BOOL __fastcall QServerNTRead(QSERVER *ptServer, QSCLIENT *ptClient)
{
	BYTE *pbTail;
	DWORD dwRoom;
	BOOL fFull;
	BOOL fResult;

	dwRoom = READ_BUFFER_SIZE - ptClient->nMsgBuffSize;
	pbTail = ptClient->abMsgBuff + ptClient->nMsgBuffSize;
	fFull = FALSE;
	if (!dwRoom)
	{
		QServerRemoveClient(ptServer, ptClient->hSocket);
		fFull = TRUE;
	}
	if (ptClient->tReadOverlapped.hEvent)
	{
#line 289
		D2ASSERT(0);
	}
	fResult = ReadFile((HANDLE)ptClient->hSocket, pbTail, dwRoom, &ptClient->dwBytesRead,
		&ptClient->tReadOverlapped);
	if (!fResult && GetLastError() != ERROR_IO_PENDING)
	{
		QServerRemoveClient(ptServer, ptClient->hSocket);
	}
	else if (!fFull)
		QServerClientAddRef(ptClient);

	return fResult;
}

static __inline void QServerNTQueue(QSCLIENT *ptClient, const BYTE *pbData, DWORD dwBytes)
{
	QSSENDBUF *ptBuffer;
	QSSENDBUF *ptLast;

#line 320
	ptBuffer = (QSSENDBUF *)D2ALLOC2(sizeof(QSSENDBUF));
	memcpy(ptBuffer->abData, pbData, dwBytes);
	ptBuffer->dwDataSize = dwBytes;
	ptBuffer->ptNext = NULL;
	EnterCriticalSection(&ptClient->tLock);
	if (!ptClient->ptSendQueue)
		ptClient->ptSendQueue = ptBuffer;
	else
	{
		for (ptLast = ptClient->ptSendQueue; ptLast->ptNext; ptLast = ptLast->ptNext)
			;
		ptLast->ptNext = ptBuffer;
	}
	LeaveCriticalSection(&ptClient->tLock);
}

static __inline void QServerNTWriteDone(QSCLIENT *ptClient)
{
	QSSENDBUF *ptBuffer;

	EnterCriticalSection(&ptClient->tLock);
	ptBuffer = ptClient->ptSendQueue;
	ptClient->ptSendQueue = ptBuffer->ptNext;
	LeaveCriticalSection(&ptClient->tLock);
#line 436
	D2FREE2(ptBuffer);
}

static __inline void QServerNTWriteNext(QSERVER *ptServer, QSCLIENT *ptClient)
{
	DWORD dwWritten;

	if (!ptClient->ptSendQueue)
		return;
	if (!WriteFile((HANDLE)ptClient->hSocket, ptClient->ptSendQueue->abData,
		ptClient->ptSendQueue->dwDataSize,
		&dwWritten, &ptClient->tWriteOverlapped) && GetLastError() != ERROR_IO_PENDING)
		QServerRemoveClient(ptServer, ptClient->hSocket);
	else
		QServerClientAddRef(ptClient);
}
