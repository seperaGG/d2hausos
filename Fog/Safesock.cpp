#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\Safesock.cpp"
#include "FogLocal.h"
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StormImports.h"
#include "FogLocalSafesock.h"
#include "Safesock.h"
#include "D2Macros.h"

static DWORD sgdwFileOption = 0;

void __fastcall SafesockSetFileOptions(DWORD dwOption, DWORD dwOption2)
{
	DWORD dwValue2;
	DWORD dwValue;

	StormSFileSetBasePath(".\\");
	StormSFileEnableDirectAccess(dwOption);
	StormSFileSetIoErrorMode(0, 0);
	StormSFileEnableSeekOptimization(dwOption2);
	sgdwFileOption = dwOption;
	dwValue2 = 32768;
	StormSetOption(2, &dwValue2, sizeof(dwValue2));
	dwValue = 1;
	StormSetOption(1, &dwValue, sizeof(dwValue));
	StormSetOption(3, &dwValue, sizeof(dwValue));
}

DWORD __fastcall SafesockOpenStormFile(DWORD dwFileName, DWORD dwFileHandleOut)
{
	return StormSFileOpenFile(dwFileName, dwFileHandleOut);
}

DWORD __fastcall SafesockCloseStormFile(DWORD dwArg)
{
	return StormOrdinal253(dwArg);
}

DWORD __fastcall SafesockReadStormFile(DWORD dwFile, DWORD dwBuffer, DWORD dwBytes, DWORD dwBytesReadOut,
	DWORD dwOverlapped,
	DWORD dwCallback)
{
	return StormSFileReadFileEx(dwFile, dwBuffer, dwBytes, dwBytesReadOut, dwOverlapped, dwCallback);
}
DWORD __fastcall SafesockGetStormFileSize(DWORD dwFile, DWORD dwFileSizeHighOut)
{
	return StormSFileGetFileSize(dwFile, dwFileSizeHighOut);
}

DWORD __fastcall SafesockSetStormFilePointer(DWORD dwFile, DWORD dwDistance, DWORD dwDistanceHighOut,
	DWORD dwMethod)
{
	return StormSFileSetFilePointer(dwFile, dwDistance, dwDistanceHighOut, dwMethod);
}
HANDLE __fastcall SafesockCreateFile(LPCSTR pszFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	return CreateFileA(pszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition,
		dwFlagsAndAttributes, hTemplateFile);
}

BOOL __fastcall SafesockDeleteFile(LPCSTR pszFileName)
{
	return DeleteFileA(pszFileName);
}

BOOL __fastcall SafesockCloseHandle(HANDLE hObject)
{
	return CloseHandle(hObject);
}

BOOL __fastcall SafesockReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD dwToRead, LPDWORD lpdwRead,
	LPOVERLAPPED lpOverlapped)
{
	return ReadFile(hFile, lpBuffer, dwToRead, lpdwRead, lpOverlapped);
}

BOOL __fastcall SafesockWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD dwToWrite, LPDWORD lpdwWritten,
	LPOVERLAPPED lpOverlapped)
{
	return WriteFile(hFile, lpBuffer, dwToWrite, lpdwWritten, lpOverlapped);
}

DWORD __fastcall SafesockSetFilePointer(HANDLE hFile, LONG lDistance, PLONG plDistanceHigh,
	DWORD dwMoveMethod)
{
	return SetFilePointer(hFile, lDistance, plDistanceHigh, dwMoveMethod);
}

BOOL __fastcall SafesockCreateDirectories(const char *pszPath)
{
	BOOL fResult;
	char szDirectory[MAX_PATH];
	const char *pszSlash;
	DWORD dwLength;
	DWORD dwError;

	fResult = TRUE;
	if (!pszPath || !*pszPath)
		return FALSE;
	pszSlash = StormSStrChr(pszPath + 1, '\\');
	while (pszSlash)
	{
		dwLength = pszSlash - pszPath;
		if (dwLength >= MAX_PATH)
		{
			fResult = FALSE;
			break;
		}
		memcpy(szDirectory, pszPath, dwLength);
		szDirectory[dwLength] = 0;
		if (!CreateDirectoryA(szDirectory, NULL))
		{
			dwError = GetLastError();
			if ((dwError != ERROR_ALREADY_EXISTS && dwError != ERROR_ACCESS_DENIED)
				|| !(GetFileAttributesA(szDirectory) & FILE_ATTRIBUTE_DIRECTORY))
			{
				fResult = FALSE;
				break;
			}
		}
		pszSlash = StormSStrChr(pszSlash + 1, '\\');
	}
	return fResult;
}

void __fastcall SafesockAddBackslash(char *pszPath, DWORD dwSize)
{
	DWORD dwLength;

	if (!pszPath || !dwSize)
		return;
	dwLength = StormSStrLen(pszPath);
	if (dwLength > 0 && pszPath[dwLength - 1] == '\\')
		return;
	if (dwLength + 1 < dwSize)
	{
		pszPath[dwLength] = '\\';
		pszPath[dwLength + 1] = 0;
	}
}

void __fastcall SafesockGetModuleDirectory(char *pszPath, DWORD dwSize)
{
	char *pszSlash;

	GetModuleFileNameA(NULL, pszPath, dwSize);
	pszPath[dwSize - 1] = 0;
	pszSlash = StormSStrChrR(pszPath, '\\');
	if (pszSlash)
		pszSlash[1] = 0;
}

void __fastcall SafesockGetInstallPath(char *pszPath, DWORD dwSize)
{
	if (!StormSRegLoadString("Diablo II", "InstallPath", 0, pszPath, dwSize))
		*pszPath = 0;
	if (GetFileAttributesA(pszPath) == (DWORD)-1)
		*pszPath = 0;
	if (!*pszPath)
	{
		SafesockGetModuleDirectory(pszPath, dwSize);
		StormSRegSaveString("Diablo II", "InstallPath", 0, pszPath);
	}
	SafesockAddBackslash(pszPath, dwSize);
}

BOOL __fastcall SafesockGetSavePath(char *pszPath, DWORD dwSize)
{
	if (!StormSRegLoadString("Diablo II", "Save Path", 0, pszPath, dwSize))
		*pszPath = 0;
	if (GetFileAttributesA(pszPath) == (DWORD)-1)
		*pszPath = 0;
	if (!*pszPath)
	{
		SafesockGetInstallPath(pszPath, dwSize);
		StormSStrAppend(pszPath, "Save\\", dwSize);
		StormSRegSaveString("Diablo II", "Save Path", 0, pszPath);
	}
	SafesockAddBackslash(pszPath, dwSize);
	return SafesockCreateDirectories(pszPath);
}

DWORD __fastcall SafesockGetFileOption(void)
{
	return sgdwFileOption;
}

static char sgszLocalAddress[256] = { 0 };
static char sgszAddress[32] = { 0 };

BOOL __fastcall SafesockStartup(void)
{
	WSADATA tWsaData;

	return WSAStartup(257, &tWsaData) == 0;
}

int __fastcall SafesockCleanup(void)
{
	return WSACleanup();
}

SAFESOCK * __fastcall SafesockConnect(const char *pszHost, DWORD nPort, LPTHREAD_START_ROUTINE pfnThread,
	LPVOID pThreadArg, const char *pszName)
{
	return SafesockConnectWithFlags(pszHost, nPort, pfnThread, pThreadArg, pszName, 0);
}

SAFESOCK * __fastcall SafesockConnectWithFlags(const char *pszHost, u_short nPort,
	LPTHREAD_START_ROUTINE pfnThread,
	LPVOID pThreadArg, const char *pszName, DWORD dwFlags)
{
	SAFESOCK *ptSock;
	WSADATA tWsaData;
	SOCKADDR_IN tAddress;
	SOCKADDR_IN tLocal;

	BOOL fNoDelay;
	BOOL fKeepAlive;
	u_long dwNonBlocking;
	DWORD dwThreadId;
	DWORD dwError;
	BOOL fAsync;
	int i;
	SOCKET hSocket;

#line 175
	ptSock = (SAFESOCK *)D2ALLOC2(sizeof(SAFESOCK));
	for (i = 0; i < (int)(sizeof(SAFESOCK) / sizeof(DWORD)); i++)
		((DWORD *)ptSock)[i] = 0;
	if (pszName)
		StormSStrCopy(ptSock->szName, pszName, sizeof(ptSock->szName));
	else
		StormSStrCopy(ptSock->szName, "UNKNOWN", sizeof(ptSock->szName));
	InitializeCriticalSection(&ptSock->tLock);
	dwError = WSAStartup(257, &tWsaData);
	if (dwError)
	{
		ptSock->dwError = dwError;
		ptSock->dwFailedStep = 1;
		DeleteCriticalSection(&ptSock->tLock);
#line 193
		D2FREE2(ptSock);
		return NULL;
	}
	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	ptSock->hSocket = hSocket;
	if (hSocket == INVALID_SOCKET)
	{
		ptSock->dwError = WSAGetLastError();
		ptSock->dwFailedStep = 2;
		DeleteCriticalSection(&ptSock->tLock);
#line 206
		D2FREE2(ptSock);
		return NULL;
	}
	fNoDelay = TRUE;
	setsockopt(hSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&fNoDelay, sizeof(fNoDelay));
	fKeepAlive = TRUE;
	setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&fKeepAlive, sizeof(fKeepAlive));
	fAsync = dwFlags & 1;
	if (fAsync)
	{
		dwNonBlocking = 1;
		ioctlsocket(hSocket, FIONBIO, &dwNonBlocking);
	}
	memset(&tAddress, 0, sizeof(tAddress));
	tAddress.sin_family = AF_INET;
	tAddress.sin_port = htons(nPort);
	tAddress.sin_addr.s_addr = inet_addr(pszHost);
	if (connect(hSocket, (SOCKADDR *)&tAddress, sizeof(tAddress)) == SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		if (!(dwFlags & 1) || dwError != WSAEWOULDBLOCK)
		{
			ptSock->dwError = dwError;
			ptSock->dwFailedStep = 3;
			DeleteCriticalSection(&ptSock->tLock);
#line 242
			D2FREE2(ptSock);
			return NULL;
		}
	}
	if (!sgszLocalAddress[0] || !inet_addr(sgszLocalAddress))
	{ int nLength;
		nLength = sizeof(tLocal);
		if (getsockname(hSocket, (SOCKADDR *)&tLocal, &nLength) != SOCKET_ERROR)
			strcpy(sgszLocalAddress, inet_ntoa(tLocal.sin_addr));
	}
	if (pfnThread)
		ptSock->hThread = CreateThread(NULL, 0, pfnThread, pThreadArg, 0, &dwThreadId);
	return ptSock;
}

void __fastcall SafesockClose(SAFESOCK *ptSock)
{
	EnterCriticalSection(&ptSock->tLock);
	ptSock->fClosing = TRUE;
	LeaveCriticalSection(&ptSock->tLock);
	WaitForSingleObject(ptSock->hThread, 500);
	EnterCriticalSection(&ptSock->tLock);
	if (closesocket(ptSock->hSocket) == SOCKET_ERROR)
	{
		ptSock->dwError = WSAGetLastError();
		ptSock->dwFailedStep = 5;
	}
	DeleteCriticalSection(&ptSock->tLock);
#line 311
	D2FREE2(ptSock);
}

int __fastcall SafesockRecv(SAFESOCK *ptSock, char *pBuffer, int nLength)
{
	int nResult;

	if (!ptSock || ptSock->fClosing)
		return 0;
	EnterCriticalSection(&ptSock->tLock);
	nResult = recv(ptSock->hSocket, pBuffer, nLength, 0);
	LeaveCriticalSection(&ptSock->tLock);
	if (nResult <= 0)
	{
		ptSock->dwError = WSAGetLastError();
		ptSock->dwFailedStep = 4;
	}
	return nResult;
}

int __fastcall SafesockSend(SAFESOCK *ptSock, const char *pBuffer, int nLength)
{
	int nResult;

	if (!ptSock || ptSock->fClosing)
		return 0;
	EnterCriticalSection(&ptSock->tLock);
	nResult = send(ptSock->hSocket, pBuffer, nLength, 0);
	if (nResult != nLength)
	{
		ptSock->dwError = WSAGetLastError();
		ptSock->dwFailedStep = 4;
		nResult = 0;
	}
	LeaveCriticalSection(&ptSock->tLock);
	return nResult;
}

int __fastcall SafesockSendUnlocked(SAFESOCK *ptSock, const char *pBuffer, int nLength)
{
	return send(ptSock->hSocket, pBuffer, nLength, 0);
}

void __fastcall SafesockLock(SAFESOCK *ptSock)
{
	EnterCriticalSection(&ptSock->tLock);
}

void __fastcall SafesockUnlock(SAFESOCK *ptSock)
{
	LeaveCriticalSection(&ptSock->tLock);
}

BOOL __fastcall SafesockCanRead(SAFESOCK *ptSock)
{
	fd_set tExcept;
	fd_set tRead;
	struct timeval tTimeout;
	int nResult;

	if (!ptSock || ptSock->fClosing)
		return FALSE;
	tRead.fd_count = 1;
	tRead.fd_array[0] = ptSock->hSocket;
	tExcept.fd_count = 1;
	tExcept.fd_array[0] = ptSock->hSocket;
	tTimeout.tv_sec = 0;
	tTimeout.tv_usec = 100000;
	nResult = select(0, &tRead, NULL, &tExcept, &tTimeout);
	if (ptSock->fClosing || !nResult)
		return FALSE;
	if (nResult != SOCKET_ERROR)
	{
		if (!FD_ISSET(ptSock->hSocket, &tExcept))
		{
			if (FD_ISSET(ptSock->hSocket, &tRead))
				return TRUE;
		}
		return FALSE;
	}
	ptSock->dwError = WSAGetLastError();
	ptSock->dwFailedStep = 4;
	return TRUE;
}

BOOL __fastcall SafesockGetPeerName(SAFESOCK *ptSock, SOCKADDR *ptAddress, int *pnLength)
{
	int nResult;

	EnterCriticalSection(&ptSock->tLock);
	nResult = getpeername(ptSock->hSocket, ptAddress, pnLength);
	LeaveCriticalSection(&ptSock->tLock);
	return nResult != SOCKET_ERROR;
}

const char * __fastcall SafesockGetLocalAddress(void)
{
	SOCKADDR_IN tLocal;
	SOCKADDR_IN tAddress;
	WSADATA tWsaData;
	char szHostName[256];
	HOSTENT *ptHost;
	SOCKET hSocket;
	int nLength;

	tLocal.sin_addr.s_addr = 0;
	WSAStartup(257, &tWsaData);
	if (gethostname(szHostName, sizeof(szHostName)) != SOCKET_ERROR)
	{
		ptHost = gethostbyname(szHostName);
		if (ptHost)
			tLocal.sin_addr.s_addr = *(u_long *)ptHost->h_addr_list[0];
	}
	if (!tLocal.sin_addr.s_addr)
	{
		hSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (hSocket != INVALID_SOCKET)
		{
			tAddress.sin_family = AF_INET;
			tAddress.sin_port = htons(7);
			tAddress.sin_addr.s_addr = inet_addr("209.67.136.168");
			if (connect(hSocket, (SOCKADDR *)&tAddress, sizeof(tAddress)) != SOCKET_ERROR)
			{
				nLength = sizeof(tLocal);
				getsockname(hSocket, (SOCKADDR *)&tLocal, &nLength);
			}
			closesocket(hSocket);
		}
	}
	sprintf(sgszLocalAddress, "%d.%d.%d.%d", tLocal.sin_addr.S_un.S_un_b.s_b1,
		tLocal.sin_addr.S_un.S_un_b.s_b2, tLocal.sin_addr.S_un.S_un_b.s_b3, tLocal.sin_addr.S_un.S_un_b.s_b4);
	WSACleanup();
	return sgszLocalAddress;
}

const char * __fastcall SafesockAddressToString(u_long dwAddress)
{
	struct in_addr tAddress;

	tAddress.s_addr = dwAddress;
	strcpy(sgszAddress, inet_ntoa(tAddress));
	return sgszAddress;
}

BOOL __fastcall SafesockGetError(SAFESOCK *ptSock, DWORD *pdwStep, DWORD *pdwError)
{
	*pdwStep = ptSock->dwFailedStep;
	*pdwError = ptSock->dwError;
	return TRUE;
}

const char * __fastcall SafesockResolve(const char *pszHost)
{
	WSADATA tWsaData;
	HOSTENT *ptHost;
	int nField1, nField2, nField3, nField4;

	if (sscanf(pszHost, "%d.%d.%d.%d", &nField1, &nField2, &nField3, &nField4) == 4)
		return pszHost;
	ptHost = gethostbyname(pszHost);
	if (!ptHost)
	{
		if (WSAGetLastError() == WSANOTINITIALISED)
		{
			if (WSAStartup(257, &tWsaData))
				return NULL;
		}
		else
			return NULL;
		ptHost = gethostbyname(pszHost);
		if (!ptHost)
		{
			WSACleanup();
			return NULL;
		}
		WSACleanup();
	}
	strcpy(sgszLocalAddress, inet_ntoa(*(struct in_addr *)ptHost->h_addr_list[0]));
	return sgszLocalAddress;
}

BOOL __fastcall SafesockSetNonBlocking(SAFESOCK *ptSock, u_long dwNonBlocking)
{
	ioctlsocket(ptSock->hSocket, FIONBIO, &dwNonBlocking);
	return TRUE;
}

#define SAFESOCK_ROTL(x, n)	(((x) << ((n) & 31)) | ((x) >> (32 - ((n) & 31))))

typedef struct
{
	DWORD adwState[5];
	BYTE abBlock[64];
} SAFESOCKDIGESTCTX;

void __fastcall SafesockDigestFirstBlock(DWORD *pdwDigest, const BYTE *pbData, DWORD dwLength)
{
	SAFESOCKDIGESTCTX tCtx;
	DWORD adwW[80];
	DWORD dwA, dwB, dwC, dwD, dwE, dwTemp;
	DWORD dwDone, dwChunk;
	DWORD i;

	tCtx.adwState[0] = 1732584193;
	tCtx.adwState[1] = 0xEFCDAB89;
	tCtx.adwState[2] = 0x98BADCFE;
	tCtx.adwState[3] = 271733878;
	tCtx.adwState[4] = 0xC3D2E1F0;
	for (dwDone = 0; dwDone < dwLength; dwDone += 64)
	{
		dwChunk = dwLength - dwDone > 64 ? 64 : dwLength - dwDone;
		memcpy(tCtx.abBlock, pbData, dwChunk);
		if (dwChunk < 64)
			memset(tCtx.abBlock + dwChunk, 0, 64 - dwChunk);
		memcpy(adwW, tCtx.abBlock, 64);
		for (i = 16; 80 > i; i++)
			adwW[i] = SAFESOCK_ROTL(1,
				adwW[i - 3] ^ adwW[i - 8] ^ adwW[i - 14] ^ adwW[i - 16]);
		dwA = tCtx.adwState[0];
		dwB = tCtx.adwState[1];
		dwC = tCtx.adwState[2];
		dwD = tCtx.adwState[3];
		dwE = tCtx.adwState[4];
		for (i = 0; i < 20; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + ((dwB & dwC) | (~dwB & dwD)) + adwW[i] + dwE + 1518500249;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		for (; i < 40; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + (dwB ^ dwC ^ dwD) + adwW[i] + dwE + 1859775393;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		for (; i < 60; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + (((dwB | dwC) & dwD) | (dwB & dwC)) + adwW[i] + dwE + 0x8F1BBCDC;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		for (; i < 80; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + (dwB ^ dwC ^ dwD) + adwW[i] + dwE + 0xCA62C1D6;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		tCtx.adwState[0] += dwA;
		tCtx.adwState[1] += dwB;
		tCtx.adwState[2] += dwC;
		tCtx.adwState[3] += dwD;
		tCtx.adwState[4] += dwE;
		for (i = 0; i < 80; i++)
			adwW[i] = 0;
	}
	memcpy(pdwDigest, tCtx.adwState, sizeof(tCtx.adwState));
}

void __fastcall SafesockDigest(DWORD *pdwDigest, const BYTE *pbData, DWORD dwLength)
{
	SAFESOCKDIGESTCTX tCtx;
	DWORD adwW[80];
	DWORD dwA, dwB, dwC, dwD, dwE, dwTemp;
	DWORD dwDone, dwChunk;
	DWORD i;
	const BYTE *pbBlock;

	pbBlock = pbData;
	tCtx.adwState[0] = 1732584193;
	tCtx.adwState[1] = 0xEFCDAB89;
	tCtx.adwState[2] = 0x98BADCFE;
	tCtx.adwState[3] = 271733878;
	tCtx.adwState[4] = 0xC3D2E1F0;
	for (dwDone = 0; dwDone < dwLength; dwDone += 64)
	{
		dwChunk = dwLength - dwDone > 64 ? 64 : dwLength - dwDone;
		memcpy(tCtx.abBlock, pbBlock, dwChunk);
		pbBlock += 64;
		if (dwChunk < 64)
			memset(tCtx.abBlock + dwChunk, 0, 64 - dwChunk);
		memcpy(adwW, tCtx.abBlock, 64);
		for (i = 16; i < 80; i++)
		{
			dwTemp = adwW[i - 3] ^ adwW[i - 8] ^ adwW[i - 14] ^ adwW[i - 16];
			adwW[i] = SAFESOCK_ROTL(1, dwTemp);
		}
		dwA = tCtx.adwState[0];
		dwB = tCtx.adwState[1];
		dwC = tCtx.adwState[2];
		dwD = tCtx.adwState[3];
		dwE = tCtx.adwState[4];
		for (i = 0; 20 > i; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + ((dwB & dwC) | (~dwB & dwD)) + adwW[i] + dwE + 1518500249;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		for (; 40 > i; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + (dwB ^ dwC ^ dwD) + adwW[i] + dwE + 1859775393;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		for (; 60 > i; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + (((dwB | dwC) & dwD) | (dwB & dwC)) + adwW[i] + dwE + 0x8F1BBCDC;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		for (; i < 80; i++)
		{
			dwTemp = SAFESOCK_ROTL(dwA, 5) + (dwB ^ dwC ^ dwD) + adwW[i] + dwE + 0xCA62C1D6;
			dwE = dwD;
			dwD = dwC;
			dwC = SAFESOCK_ROTL(dwB, 30);
			dwB = dwA;
			dwA = dwTemp;
		}
		tCtx.adwState[0] += dwA;
		tCtx.adwState[1] += dwB;
		tCtx.adwState[2] += dwC;
		tCtx.adwState[3] += dwD;
		tCtx.adwState[4] += dwE;
		for (i = 0; i < 80; i++)
			adwW[i] = 0;
	}
	memcpy(pdwDigest, tCtx.adwState, sizeof(tCtx.adwState));
}
