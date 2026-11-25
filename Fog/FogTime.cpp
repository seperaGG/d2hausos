#include "FogLocal.h"
#include <time.h>
#include "FogLocalAsync.h"
#include <string.h>
#include "ErrorManager.h"
#include "mem.h"

static CRITICAL_SECTION sgtTimeLock = { 0 };
static BYTE sgbTimeInit = 0;
static DWORD sgdwTimeTick = 0;
static DWORD sgdwTime = 0;

void __fastcall AsyncDataTimeInit(void)
{
	InitializeCriticalSection(&sgtTimeLock);
}

void __fastcall AsyncDataTimeShutdown(void)
{
	EnterCriticalSection(&sgtTimeLock);
	DeleteCriticalSection(&sgtTimeLock);
}

DWORD __fastcall AsyncDataGetTime(void)
{
	DWORD dwTick;

	if (!(sgbTimeInit & 1))
	{
		sgbTimeInit |= 1;
		sgdwTimeTick = GetTickCount();
	}
	if (!(sgbTimeInit & 2))
	{
		sgbTimeInit |= 2;
		sgdwTime = (DWORD)time(NULL);
	}
	if (GetTickCount() >= sgdwTimeTick)
	{
		EnterCriticalSection(&sgtTimeLock);
		dwTick = GetTickCount();
		if (dwTick - sgdwTimeTick >= 500)
		{
			sgdwTimeTick = dwTick;
			sgdwTime = (DWORD)time(NULL);
		}
		LeaveCriticalSection(&sgtTimeLock);
	}
	return sgdwTime;
}
