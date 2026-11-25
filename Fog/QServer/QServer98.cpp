#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\QServer\\QServer98.cpp"
#include "FogLocal.h"
#include <winsock.h>
#include <stdio.h>
#include "QServer.h"
#include "D2Macros.h"

static DWORD __stdcall QServer98Thread(void *pContext);

BOOL __fastcall QServer98Start(QSERVER *ptServer, u_short nPort)
{
	SOCKET hSocket;
	SOCKADDR_IN tAddress;
	BOOL fNoDelay;
	BOOL fKeepAlive;
	u_long dwNonBlocking;

	ptServer->nAddressLimit = 1;
	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		return FALSE;
	ptServer->hListenSocket = hSocket;
	fNoDelay = TRUE;
	setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&fNoDelay, sizeof(fNoDelay));
	fKeepAlive = TRUE;
	setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&fKeepAlive, sizeof(fKeepAlive));
	dwNonBlocking = 1;
	ioctlsocket(hSocket, FIONBIO, &dwNonBlocking);
	memset(&tAddress, 0, sizeof(tAddress));
	tAddress.sin_family = AF_INET;
	tAddress.sin_port = htons(nPort);
	if (bind(ptServer->hListenSocket, (SOCKADDR *)&tAddress, sizeof(tAddress)) == SOCKET_ERROR)
	{
		closesocket(hSocket);
		return FALSE;
	}
	if (listen(hSocket, 100) == SOCKET_ERROR)
		closesocket(hSocket);
	ptServer->hMsgEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	ptServer->hThread = CreateThread(NULL, 0, QServer98Thread, ptServer, 0, &ptServer->dwThreadId);
	return TRUE;
}

void __fastcall QServer98Stop(QSERVER *ptServer)
{
	EnterCriticalSection(&ptServer->tLock);
	ptServer->fStopThread = TRUE;
	LeaveCriticalSection(&ptServer->tLock);
	WaitForSingleObject(ptServer->hThread, 200);
}

BOOL __fastcall QServer98Send(QSERVER *ptServer, SOCKET hSocket, const BYTE *pbData, int nBytes)
{
	if (send(hSocket, (const char *)pbData, nBytes, 0) == SOCKET_ERROR)
	{
		WSAGetLastError();
		return FALSE;
	}
	return TRUE;
}

#define QSERVER98_FD_SET(hSocket, ptSet)	\
	do { \
		u_int i; \
		for (i = 0; i < (ptSet)->fd_count; i++) \
		{ \
			if ((ptSet)->fd_array[i] == (hSocket)) \
				break; \
		} \
		if (i == (ptSet)->fd_count) \
		{ \
			if ((ptSet)->fd_count < FD_SETSIZE) \
			{ \
				(ptSet)->fd_array[i] = (hSocket); \
				(ptSet)->fd_count++; \
			} \
		} \
	} while (0)

static DWORD __stdcall QServer98Thread(void *pContext)
{
	QSERVER *ptServer;
	QSCLIENT *ptClient;
	QSCLIENT *ptNext;
	fd_set tRead;
	fd_set tExcept;
	struct timeval tTimeout;
	SOCKADDR_IN tAddress;
	char szMessage[100];
	BYTE abDiscard[READ_BUFFER_SIZE];
	SOCKET hSocket;
	SOCKET hAccepted;
	int nBucket;
	int nSkip;
	int nSockets;
	int nLength;
	int nResult;
	int nSize;
	DWORD nTries;
	int nError;

	ptServer = (QSERVER *)pContext;
	nBucket = 0;
	nSkip = 0;
	while (1)
	{
		QServerAgeBlocks(ptServer);
		tRead.fd_count = 0;
		tExcept.fd_count = 0;
		EnterCriticalSection(&ptServer->tLock);
		QSERVER98_FD_SET(ptServer->hListenSocket, &tRead);
		QSERVER98_FD_SET(ptServer->hListenSocket, &tExcept);
		nSockets = 1;
		ptClient = ptServer->aptClients[nBucket];
		for (; ptClient && nSkip--; ptClient = ptClient->ptNext)
			;
		while (nSockets < FD_SETSIZE)
		{
			if (nSockets > ptServer->nNumClients)
				break;
			if (!ptClient)
			{
				nSkip = 0;
				do
				{
					ptClient = ptServer->aptClients[nBucket];
					nBucket = (nBucket + 1) % QSERVER_CLIENT_HASH;
				} while (!ptClient);
			}
			EnterCriticalSection(&ptClient->tLock);
			QSERVER98_FD_SET(ptClient->hSocket, &tRead);
			QSERVER98_FD_SET(ptClient->hSocket, &tExcept);
			ptNext = ptClient->ptNext;
			LeaveCriticalSection(&ptClient->tLock);
			ptClient = ptNext;
			nSockets++;
			nSkip++;
		}
		tTimeout.tv_sec = 0;
		tTimeout.tv_usec = 100000 / (ptServer->nNumClients / FD_SETSIZE + 1);
		LeaveCriticalSection(&ptServer->tLock);
		nResult = select(0, &tRead, NULL, &tExcept, &tTimeout);
		if (!nResult)
		{
			EnterCriticalSection(&ptServer->tLock);
			if (ptServer->fStopThread)
			{
				ptServer->fStopThread = FALSE;
				LeaveCriticalSection(&ptServer->tLock);
				OutputDebugStringA("Server thread close #1\n");
				ExitThread(0);
			}
			LeaveCriticalSection(&ptServer->tLock);
			continue;
		}
		if (nResult == SOCKET_ERROR)
			continue;
		while (tExcept.fd_count > 0)
			tExcept.fd_count--;
		while (tRead.fd_count > 0)
		{
			hSocket = tRead.fd_array[--tRead.fd_count];
			if (hSocket == ptServer->hListenSocket)
			{
				nTries = 0;
				memset(&tAddress, 0, sizeof(tAddress));
				nLength = sizeof(tAddress);
				while (1)
				{
					WSASetLastError(0);
					EnterCriticalSection(&ptServer->tLock);
					hAccepted = accept(ptServer->hListenSocket, (SOCKADDR *)&tAddress, &nLength);
					if (hAccepted != INVALID_SOCKET)
						break;
					nError = WSAGetLastError();
					if (nError == WSAEWOULDBLOCK)
					{
						if (++nTries > 100)
						{
#line 242
							FogErrorManagerHalt("EWOULDBLOCK 100 times", D2_THIS_FILE, __LINE__);
							exit(-1);
						}
					}
					else
					{
						sprintf(szMessage, "Accept failed.  Error %d", nError);
#line 253
						FogErrorManagerHalt(szMessage, D2_THIS_FILE, __LINE__);
						exit(-1);
					}
					LeaveCriticalSection(&ptServer->tLock);
				}
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
					QServerAddClient(ptServer, hAccepted, tAddress.sin_addr.s_addr);
					if (ptServer->pfnAccept)
						ptServer->pfnAccept(ptServer, hAccepted);
				}
				LeaveCriticalSection(&ptServer->tLock);
				continue;
			}
			ptClient = QServerFindClient(ptServer, hSocket);
			EnterCriticalSection(&ptServer->tLock);
			if (ptClient)
			{
				EnterCriticalSection(&ptClient->tLock);
				nSize = ptClient->nMsgBuffSize;
#line 272
				D2ASSERT(nSize >= 0 && nSize < READ_BUFFER_SIZE);
				nResult = recv(hSocket, (char *)ptClient->abMsgBuff + nSize, READ_BUFFER_SIZE - nSize, 0);
				LeaveCriticalSection(&ptClient->tLock);
				if (nResult == SOCKET_ERROR || !nResult)
				{
					WSAGetLastError();
					QServerRemoveClient(ptServer, hSocket);
				}
				else
				{
					EnterCriticalSection(&ptClient->tLock);
					ptClient->nMsgBuffSize += nResult;
					LeaveCriticalSection(&ptClient->tLock);
					QServerParse(ptServer, ptClient);
					SetEvent(ptServer->hMsgEvent);
				}
			}
			else
			{
				nResult = recv(hSocket, (char *)abDiscard, READ_BUFFER_SIZE, 0);
				if (nResult == SOCKET_ERROR || !nResult)
					QServerRemoveClient(ptServer, hSocket);
				else
					QServerDisconnect(ptServer, hSocket);
			}
			LeaveCriticalSection(&ptServer->tLock);
		}
	}
}
