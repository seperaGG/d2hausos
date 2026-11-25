#ifndef D2RE_FOG_ERRORMANAGER_H
#define D2RE_FOG_ERRORMANAGER_H

#include <windows.h>

struct ERRORMANAGERSYSINFO
{
 	BOOL		fValid;
 	DWORD		dwOsType;
 	float		flVersion;
 	DWORD		dwTotalPhysMemory;
 	DWORD		unobserved_10;
 	SYSTEM_INFO	tSystemInfo;
};

extern ERRORMANAGERSYSINFO gtErrorManagerSysInfo;

#endif
