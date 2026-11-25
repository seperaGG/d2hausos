#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\BnConnect.cpp"
#include "Bnclient.h"
#include "D2Macros.h"
#include "FogImports.h"
#include "StormImports.h"

#include "BnConnect.h"

BNHOSTNAME sgszGateway = { 0 };
u_long sgdwHostAddress = 0;
int sgnStage = 0;
HOSTENT sgtHostEnt = { 0 };
BNADDRLIST sgapszHostAddrs = { 0 };
BNRESOLVE sgtResolve = { 0 };
BNSERVERLIST sgszServers = { 0 };
SAFESOCK *sgptConnection = 0;
LONG sglPumps = 0;
BOOL sgfConnected = 0;
BOOL sgfAbort = 0;
LONG sglConnectThreads = 0;

static BOOL BnConnectIsLost(void)
{
	BNERRINFO tReason;

	BnMessQueuePeekReason(&tReason);
	return BnConnectIsCancelled() || tReason.dwReason || !sgptConnection;
}

BOOL __stdcall sFastestConnect(char *pszServers)
{
	BNSERVER atServers[MAX_SIMULTANEOUS_CONNECTS];
	fd_set tWrite;
	fd_set tExcept;
	struct timeval tTimeout;
	SOCKADDR_IN tAddress;
	int nAddressLength;
	BNERRINFO tVersionReason;
	char *pszToken;
	SOCKET hSocket;
	int nServerCount;
	int nActive;
	int nTimeouts;
	int nResult;
	int i;
	BYTE bSelector;

	nServerCount = 0;
	BnConnectDebugPrint("sFastestConnect()");
	if (pszServers)
	{
		sgfConnected = FALSE;
		BnConnectSetStage(2);
		BnclientLogPrintf("searching for the fastest server");
		for (pszToken = strtok(pszServers, " ,;"); pszToken && *pszToken; pszToken = strtok(NULL, " ,;"))
		{
#line 85
			D2ASSERT(nServerCount < MAX_SIMULTANEOUS_CONNECTS);
			BnConnectDebugPrint("  Try to connect to %s", pszToken);
			strcpy(atServers[nServerCount].szHost, pszToken);
			atServers[nServerCount].ptSock = (SAFESOCK *)FogSafesockConnectWithFlags(pszToken, BNET_PORT,
				BnConnectPump, 0, "Client->BNet", 1);
			nServerCount++;
		}
		tTimeout.tv_sec = 1;
		tTimeout.tv_usec = 0;
		nTimeouts = 0;
		while (!sgptConnection && !sgfAbort)
		{
			FD_ZERO(&tWrite);
			FD_ZERO(&tExcept);
			nActive = 0;
			for (i = 0; i < nServerCount; i++)
			{
				if (atServers[i].ptSock)
				{
					hSocket = atServers[i].ptSock->hSocket;
					FD_SET(hSocket, &tWrite);
					FD_SET(hSocket, &tExcept);
					nActive++;
				}
			}
			if (!nActive)
				break;
			nResult = select(0, NULL, &tWrite, &tExcept, &tTimeout);
			if (!nResult)
			{
				if (nTimeouts++ > 60)
					break;
			}
			else if (nResult != SOCKET_ERROR)
			{
				while (tExcept.fd_count > 0)
				{
					hSocket = tExcept.fd_array[--tExcept.fd_count];
					for (i = 0; i < nServerCount; i++)
					{
						if (atServers[i].ptSock && atServers[i].ptSock->hSocket == hSocket)
						{
							FogSafesockClose(atServers[i].ptSock);
							atServers[i].ptSock = NULL;
							break;
						}
					}
				}
				while (tWrite.fd_count > 0)
				{
					hSocket = tWrite.fd_array[--tWrite.fd_count];
					for (i = 0; i < nServerCount; i++)
					{
						if (atServers[i].ptSock && atServers[i].ptSock->hSocket == hSocket)
						{
							sgptConnection = atServers[i].ptSock;
							break;
						}
					}
				}
			}
			else
				WSAGetLastError();
		}
		for (i = 0; i < nServerCount; i++)
		{
			if (atServers[i].ptSock && atServers[i].ptSock != sgptConnection)
				FogSafesockClose(atServers[i].ptSock);
		}
		if (!sgptConnection)
		{
			BnclientLogPrintf("no servers found");
			return FALSE;
		}
		sgfConnected = TRUE;
		if (BnConnectIsLost())
		{
			BnConnectDebugPrint("  Error, not connected.");
			return FALSE;
		}
		nAddressLength = sizeof(tAddress);
		if (FogSafesockGetPeerName(sgptConnection, &tAddress, &nAddressLength))
			BnclientLogPrintf("Connected to server %d.%d.%d.%d", tAddress.sin_addr.S_un.S_un_b.s_b1,
				tAddress.sin_addr.S_un.S_un_b.s_b2, tAddress.sin_addr.S_un.S_un_b.s_b3,
				tAddress.sin_addr.S_un.S_un_b.s_b4);
		BnConnectSetStage(3);
		BnclientLogPrintf("accessing accounts");
		FogSafesockSetNonBlocking(sgptConnection, 0);
		bSelector = 1;
		if (FogSafesockSend(sgptConnection, &bSelector, sizeof(bSelector)) <= 0)
		{
			BnConnectDebugPrint("  Failed send.");
			return FALSE;
		}
		BnConnectClearMessage(29);
		BnMessQueueSendRegistration();
		if (!BnConnectWaitMessage(29))
		{
			if (BnConnectIsLost())
			{
				FogSafesockClose(sgptConnection);
				sgptConnection = NULL;
				return FALSE;
			}
		}
		if (BnConnectIsCancelled())
		{
			BnMessQueueForceReason(7);
			return TRUE; }
		BnSendLocaleAndTime();
		BnConnectSetStage(4);
		BnclientLogPrintf("Checking for the latest version");
		BnSendVersion();
		BnMessQueuePeekReason(&tVersionReason);
		if (tVersionReason.dwReason)
			return TRUE;
		if (!BnConnectGetResult(7))
			return TRUE;
		BnConnectSetStage(5);
		BnclientLogPrintf("Verifying CD key");
		if (!BnMessQueueSendCDKey())
			return FALSE;
		if (!BnConnectWaitMessage(54))
			return FALSE;
		if (!BnConnectGetResult(54))
			return FALSE;
		BnDownloadRequestFile("bnserver.ini", BNDL_SERVERINI);
		BnclientLogPrintf("Connected to Battle.net");
		return TRUE; }
	return TRUE;
}

void __fastcall BnConnectSetStage(int nStage)
{
	sgnStage = nStage;
	Sleep(10);
}

DWORD WINAPI GetServersList(LPVOID pArg)
{
	BNRESOLVE *ptArgs = (BNRESOLVE *)pArg;
	char *pszResult;
	char *pszHost;
	char *pszOut;
	char **ppszAddress;
	HOSTENT *ptHost;
	u_long dwAddress;
	u_long dwSwap;
	int nCount;
	int nLast;
	int swapA;
	int swapB;

	pszResult = ptArgs->pszResult;
	pszHost = ptArgs->pszHost;
	if (isdigit(*pszHost))
	{
		dwAddress = inet_addr(pszHost);
		if (dwAddress == INADDR_NONE)
		{
			BnclientLogPrintf("ERROR GetServersList: inet_addr failed to convert IP (%d)", WSAGetLastError());
			return 0;
		}
		sgdwHostAddress = dwAddress;
		sgtHostEnt.h_addr_list = sgapszHostAddrs;
		sgapszHostAddrs[0] = (char *)&sgdwHostAddress;
		sgapszHostAddrs[1] = NULL;
		ptHost = &sgtHostEnt;
		if (!ptHost)
			return 0;
	}
	else
	{
		ptHost = gethostbyname(pszHost);
		if (!ptHost)
		{
			BnclientLogPrintf("ERROR GetServersList: gethostbyname failed to get server list (%d)",
				WSAGetLastError());
			return 0;
		}
	}
	ppszAddress = ptHost->h_addr_list;
	nCount = 0;
	BnConnectDebugPrint("");
	BnConnectDebugPrint("Original servlist:");
	for (; ppszAddress[nCount]; nCount++)
		BnConnectDebugPrint("%d --> %d.%d.%d.%d", nCount, (BYTE)ppszAddress[nCount][0],
			(BYTE)ppszAddress[nCount][1], (BYTE)ppszAddress[nCount][2], (BYTE)ppszAddress[nCount][3]);
	BnConnectDebugPrint("");
	BnConnectDebugPrint("Scrambled servlist:");
	srand(GetTickCount());
	nLast = nCount - 1;
	for (swapA = 0; swapA < nLast; swapA++)
	{
		swapB = rand() * (nLast - swapA) / 32768 + swapA;
#line 377
		D2ASSERT(swapA <= swapB);
		BnConnectDebugPrint("%d <-> %d SWAP", swapA, swapB);
		dwSwap = *(u_long *)ppszAddress[swapA];
		*(u_long *)ppszAddress[swapA] = *(u_long *)ppszAddress[swapB];
		*(u_long *)ppszAddress[swapB] = dwSwap;
	}
	BnConnectDebugPrint("");
	pszOut = pszResult;
	for (ppszAddress = ptHost->h_addr_list; *ppszAddress; ppszAddress++)
	{
		pszOut += StormSStrCopy(pszOut, inet_ntoa(*(struct in_addr *)*ppszAddress),
			2147483647);
		pszOut += StormSStrCopy(pszOut, ";", 2147483647);
	}
	if (pszOut > pszResult)
		pszOut[-1] = 0;
	return 1;
}

void __fastcall BnConnectResolve(char *pszResult, char *pszHost)
{
	DWORD dwThreadId;
	HANDLE hThread;

	BnConnectSetStage(1);
	*pszResult = 0;
	dwThreadId = 0;
	sgtResolve.pszResult = pszResult;
	sgtResolve.pszHost = pszHost;
	hThread = CreateThread(NULL, 0, GetServersList, &sgtResolve, 0, &dwThreadId);
	while (!sgfAbort && WaitForSingleObject(hThread, 0))
		Sleep(10);
}

BOOL __fastcall BnConnectRequestServerIni(void)
{
	return BnDownloadRequestFile("bnserver.ini", BNDL_SERVERINI);
}

void __fastcall BnConnectGetGateway(char *pszHost, int nSize)
{
	char szValue[256];
	int nGateway;

	*pszHost = 0;
	gtGateways.Load();
	nGateway = gtGateways.nGateway;
	StormSStrCopy(pszHost, gtGateways.GetHost(nGateway), nSize);
	if (StormSRegLoadString("Diablo II", "BNETIP", 0, szValue, sizeof(szValue)))
		StormSStrCopy(pszHost, szValue, nSize);
}

DWORD __cdecl BattleConnectFastest(void)
{
	BnConnectDebugPrint("BattleConnectFastest()");
	if (InterlockedIncrement(&sglConnectThreads) > 1)
	{
		InterlockedDecrement(&sglConnectThreads);
		return 0;
	}
	BnConnectGetGateway(sgszGateway, sizeof(sgszGateway));
	BnConnectDebugPrint("   connect to: %s", sgszGateway);
	BnclientLogControl(2, 4);
	BnclientLogPrintf("querying gateway %s", sgszGateway);
	sgszServers[0] = 0;
	if (FogSafesockStartup())
		BnConnectResolve(sgszServers, sgszGateway);
	if (!sgszServers[0])
	{
		InterlockedDecrement(&sglConnectThreads);
		BnMessQueueForceReason(8);
		BnConnectDebugPrint("   failed to get server list");
		BnclientLogControl(2, -4);
		return 0;
	}
	BnMessQueueForceReason(0);
	sFastestConnect(sgszServers);
	BnclientLogControl(2, -4);
	if (!sgptConnection && !sgfAbort)
		BnMessQueueForceReason(8);
	InterlockedDecrement(&sglConnectThreads);
	return 1;
}

HANDLE __fastcall BnConnectConnect(void)
{
	DWORD dwThreadId;
	int nWait;

	BnclientLogPrintf("Connecting to Battle.net...");
	if (sgptConnection)
	{
		BnConnectDebugPrint("   already connected");
		return NULL;
	}
	BnSendFreeChannelList();
	BnSendFreeGameList();
	CacheClose();
	BnMessQueueDrain();
	sgfAbort = TRUE;
	while (sglPumps)
		Sleep(10);
	if (sgptConnection)
	{
		BnConnectSetCancelled();
		while (sglConnectThreads)
			Sleep(10);
		if (sgptConnection)
		{
			FogSafesockClose(sgptConnection);
			sgptConnection = NULL;
		}
	}
	if (sglConnectThreads) { nWait = 0;
	while (sglConnectThreads)
	{
		nWait += 100;
		Sleep(10);
		if (nWait > 60)
			return NULL;
	} }
	sgfAbort = FALSE;
	BnConnectSetStage(0);
	CacheOpen();
	BnConnectReset();
	return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BattleConnectFastest, NULL, 0, &dwThreadId);
}

void __fastcall BnConnectGetStage(int *pState)
{
#line 571
	D2ASSERT(pState != NULL);
	*pState = sgnStage;
}

BOOL __fastcall BnConnectLogin(const char *pszFirst, const char *pszSecond)
{
	BnConnectClearMessage(58);
	BnSendLogin(pszFirst, pszSecond);
	return TRUE;
}

BOOL __fastcall BnConnectLoginResult(void)
{
	if (BnConnectIsCancelled())
	{
		BnMessQueueSetReason(14);
		return FALSE;
	}
	return BnConnectIsHandled(58);
}

BOOL __fastcall BnConnectCreateAccount(const char *pszFirst, const char *pszSecond)
{
	BnConnectClearMessage(61);
	BnSendCreateAccount(pszFirst, pszSecond);
	return TRUE;
}

BOOL __fastcall BnConnectCreateAccountResult(DWORD *pdwResult)
{
	*pdwResult = BnConnectGetCreateAccountResult();
	return BnConnectIsHandled(61);
}

BOOL __fastcall BnConnectEnterChat(const char *pszFirst, const char *pszSecond, DWORD dwThird,
	const char *pszFourth)
{
	BnConnectClearMessage(10);
	BnSendEnterChat(pszFirst, pszSecond, dwThird);
	BnSendJoinChannel(pszFourth);
	BnConnectWaitMessage(10);
	if (BnConnectIsCancelled())
	{
		BnMessQueueSetReason(10);
		return TRUE;
	}
	BnMessQueueForceReason(0);
	return TRUE;
}

BOOL __fastcall BnConnectLeaveChannel(void)
{
	BnSendLeaveChannel();
	return TRUE;
}

BOOL __fastcall BnConnectDisconnect(void)
{
	int nWait;

	BnSendFreeChannelList();
	BnSendFreeGameList();
	CacheClose();
	BnMessQueueDrain();
	sgfAbort = TRUE;
	while (sglPumps)
		Sleep(10);
	if (sgptConnection)
	{
		BnConnectSetCancelled();
		while (sglConnectThreads)
			Sleep(10);
		if (sgptConnection)
		{
			FogSafesockClose(sgptConnection);
			sgptConnection = NULL;
		}
	}
	if (sglConnectThreads) { nWait = 0;
	while (sglConnectThreads)
	{
		nWait += 100;
		Sleep(10);
		if (nWait > 60)
			return FALSE;
	} }
	return TRUE;
}

BOOL __fastcall BnConnectSendLine(const char *pszLine)
{
	FogSafesockSend(sgptConnection, pszLine, strlen(pszLine));
	FogSafesockSend(sgptConnection, "\n", 1);
	return TRUE;
}

int __fastcall BnConnectSend(BYTE bMessage, const void *pData, DWORD dwLength)
{
	BNHEADER tHeader;
	BYTE abBuffer[8192];
	DWORD dwSize;

	tHeader.bSignature = 255;
	tHeader.bMessage = bMessage;
	tHeader.wLength = (WORD)(dwLength + sizeof(tHeader));
	if (!sgptConnection || sgfAbort)
	{
		BnConnectSetCancelled();
		return 0;
	}
	if (!pData)
		dwLength = 0;
	dwSize = dwLength + sizeof(tHeader);
	StormSMemCopy(abBuffer, &tHeader, sizeof(tHeader));
	if (pData && dwSize < sizeof(abBuffer))
		StormSMemCopy(abBuffer + sizeof(tHeader), pData, dwLength);
	return FogSafesockSend(sgptConnection, abBuffer, dwSize);
}

BOOL __fastcall BnConnectQueueText(const char *pszText)
{
	return BnMessQueueAddText(1, pszText);
}

void __fastcall BnConnectQueueLines(char *pszText)
{
	char *pszLine;

	for (pszLine = strtok(pszText, "\r"); pszLine; pszLine = strtok(NULL, "\r"))
	{
		if (*pszLine == '\n')
			pszLine++;
		BnMessQueueAddText(1, pszLine);
	}
}
