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

#endif
