#ifndef D2RE_SAFESOCK_H
#define D2RE_SAFESOCK_H

#include <windows.h>
#include <winsock.h>

struct SAFESOCK
{
 	SOCKET		hSocket;
 	CRITICAL_SECTION	tLock;
 	HANDLE		hThread;
 	BOOL		fClosing;
 	DWORD		dwFailedStep;
 	DWORD		dwError;
 	char		szName[64];
};

SAFESOCK * __fastcall SafesockConnect(const char *pszHost, DWORD nPort, LPTHREAD_START_ROUTINE pfnThread,
	LPVOID pThreadArg, const char *pszName);
SAFESOCK * __fastcall SafesockConnectWithFlags(const char *pszHost, u_short nPort,
	LPTHREAD_START_ROUTINE pfnThread,
	LPVOID pThreadArg, const char *pszName, DWORD dwFlags);
void __fastcall SafesockClose(SAFESOCK *ptSock);
int __fastcall SafesockRecv(SAFESOCK *ptSock, char *pBuffer, int nLength);
int __fastcall SafesockSend(SAFESOCK *ptSock, const char *pBuffer, int nLength);
int __fastcall SafesockSendUnlocked(SAFESOCK *ptSock, const char *pBuffer, int nLength);
void __fastcall SafesockLock(SAFESOCK *ptSock);
void __fastcall SafesockUnlock(SAFESOCK *ptSock);
BOOL __fastcall SafesockCanRead(SAFESOCK *ptSock);
BOOL __fastcall SafesockGetError(SAFESOCK *ptSock, DWORD *pdwStep, DWORD *pdwError);

BOOL __fastcall SafesockGetSavePath(char *pszPath, DWORD dwSize);

#endif
