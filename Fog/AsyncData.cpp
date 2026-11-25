#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\AsyncData.cpp"
#include "FogLocal.h"
#include <time.h>
#include "FogLocalAsync.h"
#include <string.h>
#include "ErrorManager.h"
#include "mem.h"
#include "D2Macros.h"

struct ASYNCEVENT
{
 	HANDLE		hEvent;
 	BOOL		fFree;
 	struct ASYNCDATA	*ptAsyncData;
};

struct ASYNCDATA
{
 	DWORD		dwStormFile;
 	DWORD		dwMemPool;
 	OVERLAPPED	tOverlapped;
 	DWORD		adwCallback[7];
 	ASYNCEVENT	*ptEvent;
 	BYTE		*ptBuffer;
 	DWORD		dwSize;
 	BOOL		fLoaded;
 	BOOL		fOwnsBuffer;
};

static BOOL sgfAsyncDataEnabled = 0;
static ASYNCEVENT *sgptAsyncEventArray = 0;

void __fastcall AsyncDataInit(BOOL fEnable)
{
	int i;
	ASYNCEVENT *ptEvent;

#line 87
	D2ASSERT(!sgfAsyncDataEnabled);
	StormSFileSetAsyncRate(262144);
	StormSFileSetDataChunkSize(16384);
	if (!fEnable)
		return;
#line 100
	sgptAsyncEventArray = (ASYNCEVENT *)D2ALLOC2(128 * sizeof(ASYNCEVENT));
#line 101
	D2ASSERT(sgptAsyncEventArray);
	for (i = 0; i < 128; i++)
	{
		ptEvent = &sgptAsyncEventArray[i];
		ptEvent->hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
		ptEvent->fFree = TRUE;
	}
	sgfAsyncDataEnabled = TRUE;
}

void __fastcall AsyncDataShutdown(void)
{
	ASYNCEVENT *ptEvent;
	int i;

	if (sgptAsyncEventArray)
	{
		for (i = 0; i < 128; i++)
		{
			ptEvent = &sgptAsyncEventArray[i];
			if (ptEvent && ptEvent->hEvent)
			{
				CloseHandle(ptEvent->hEvent);
				ptEvent->hEvent = NULL;
			}
		}
#line 133
		D2FREE2(sgptAsyncEventArray);
		sgptAsyncEventArray = NULL;
	}
	sgfAsyncDataEnabled = FALSE;
}

void __fastcall AsyncDataComplete(ASYNCDATA *ptAsyncData)
{
	ASYNCEVENT *ptEvent;

	if (ptAsyncData->fLoaded)
		return;
	ptEvent = ptAsyncData->ptEvent;
	if (ptEvent)
	{
		ResetEvent(ptEvent->hEvent);
		ptEvent->fFree = TRUE;
		ptEvent->ptAsyncData = NULL;
		ptAsyncData->ptEvent = NULL;
	}
	if (ptAsyncData->dwStormFile)
	{
		FogSafesockCloseStormFile(ptAsyncData->dwStormFile);
		ptAsyncData->dwStormFile = 0;
	}
	ptAsyncData->fLoaded = TRUE;
	if (ptAsyncData->adwCallback[5])
	{
#line 171
		D2FREEPOOL(ptAsyncData->dwMemPool, ptAsyncData->ptBuffer);
		ptAsyncData->ptBuffer = (BYTE *)ptAsyncData->adwCallback[5];
		ptAsyncData->dwSize = ptAsyncData->adwCallback[6];
		memset(ptAsyncData->adwCallback, 0, sizeof(ptAsyncData->adwCallback));
	}
}

ASYNCEVENT * __fastcall AsyncDataGetFreeEvent(void)
{
	ASYNCEVENT *ptEvent;
	int i;

	if (!sgfAsyncDataEnabled)
		return NULL;
	ptEvent = sgptAsyncEventArray;
	for (i = 0; i < 128; i++, ptEvent++)
	{
		if (ptEvent->fFree)
		{
			ptEvent->fFree = FALSE;
			return ptEvent;
		}
	}
	for (i = 0; i < 128; i++)
	{
		ptEvent = &sgptAsyncEventArray[i];
		if (WaitForSingleObject(ptEvent->hEvent, 0) == WAIT_OBJECT_0)
		{
			AsyncDataComplete(ptEvent->ptAsyncData);
			ptEvent->fFree = FALSE;
			return ptEvent;
		}
	}
	return NULL;
}

ASYNCDATA * __fastcall AsyncDataLoad(void *pPool, const char *pszFile, BOOL fAsync, DWORD dwOffset, int nSize,
	BYTE *pbBuffer, DWORD *pCallback, const char *pszSrcFile, int nSrcLine)
{
	ASYNCDATA *ptAsyncData;
	int i;

#line 237
	ptAsyncData = (ASYNCDATA *)D2ALLOCPOOL(pPool, sizeof(ASYNCDATA));
	for (i = 0; i < 19; i++)
		((DWORD *)ptAsyncData)[i] = 0;
	if (!FogSafesockOpenStormFile(pszFile, &ptAsyncData->dwStormFile))
	{
#line 243
		FogErrorManagerError(3, pszFile, D2_THIS_FILE, __LINE__);
		exit(-1);
	}
	ptAsyncData->dwMemPool = (DWORD)(INT_PTR)pPool;
	if (dwOffset && FogSafesockSetStormFilePointer(ptAsyncData->dwStormFile, dwOffset, 0, 0) == (DWORD)-1)
	{
#line 256
		FogErrorManagerError(3, pszFile, D2_THIS_FILE, __LINE__);
		exit(-1);
	}
	if (nSize > 0)
		ptAsyncData->dwSize = nSize;
	else
		ptAsyncData->dwSize = FogSafesockGetStormFileSize(ptAsyncData->dwStormFile, 0);
	if (pbBuffer)
	{
		ptAsyncData->ptBuffer = pbBuffer;
		ptAsyncData->fOwnsBuffer = FALSE;
	}
	else
	{
		ptAsyncData->ptBuffer = (BYTE *)MemPoolAlloc((D2POOLSYSTEM *)pPool, ptAsyncData->dwSize, pszSrcFile,
			nSrcLine);
		ptAsyncData->fOwnsBuffer = TRUE;
	}
	if (pCallback)
	{
		memcpy(ptAsyncData->adwCallback, pCallback, sizeof(ptAsyncData->adwCallback));

		pCallback = ptAsyncData->adwCallback;
	}
	if (fAsync && sgfAsyncDataEnabled)
	{
		ptAsyncData->ptEvent = AsyncDataGetFreeEvent();
		if (!ptAsyncData->ptEvent)
		{
			if (ptAsyncData->dwStormFile)
			{
				FogSafesockCloseStormFile(ptAsyncData->dwStormFile);
				ptAsyncData->dwStormFile = 0;
			}
			if (ptAsyncData->fOwnsBuffer)
			{
#line 319
				D2FREEPOOL(pPool, ptAsyncData->ptBuffer);
			}
#line 321
			D2FREEPOOL(pPool, ptAsyncData);
			return NULL;
		}
		ptAsyncData->ptEvent->ptAsyncData = ptAsyncData;
		ptAsyncData->tOverlapped.hEvent = ptAsyncData->ptEvent->hEvent;
		ptAsyncData->tOverlapped.Offset = dwOffset;
		FogSafesockReadStormFile(ptAsyncData->dwStormFile, ptAsyncData->ptBuffer, ptAsyncData->dwSize,
			0,
			&ptAsyncData->tOverlapped, pCallback);
		return ptAsyncData;
	}
	FogSafesockReadStormFile(ptAsyncData->dwStormFile, ptAsyncData->ptBuffer, ptAsyncData->dwSize, 0,
		0, 0);
	if (pCallback && pCallback[0])
		((void (__stdcall *)(BYTE *, DWORD, DWORD *))(INT_PTR)pCallback[0])(ptAsyncData->ptBuffer,
			ptAsyncData->dwSize, pCallback);
	AsyncDataComplete(ptAsyncData);
	return ptAsyncData;
}

BOOL __fastcall AsyncDataIsLoaded(ASYNCDATA *ptAsyncData)
{
#line 377
	D2ASSERT(ptAsyncData);
	if (!ptAsyncData->fLoaded && WaitForSingleObject(ptAsyncData->ptEvent->hEvent, 0) == WAIT_OBJECT_0)
		AsyncDataComplete(ptAsyncData);
	return ptAsyncData->fLoaded;
}

void __fastcall AsyncDataWait(ASYNCDATA *ptAsyncData)
{
	DWORD dwRet;

#line 398
	D2ASSERT(ptAsyncData);
	if (!ptAsyncData->fLoaded && WaitForSingleObject(ptAsyncData->ptEvent->hEvent, 0) == WAIT_OBJECT_0)
		AsyncDataComplete(ptAsyncData);
	if (ptAsyncData->fLoaded)
		return;
	StormSFilePrioritizeRequest(ptAsyncData->ptBuffer, 1);
	dwRet = WaitForSingleObject(ptAsyncData->ptEvent->hEvent, INFINITE);
#line 418
	D2ASSERT(dwRet == WAIT_OBJECT_0);
	AsyncDataComplete(ptAsyncData);
}

BYTE * __fastcall AsyncDataGetBuffer(ASYNCDATA *ptAsyncData)
{
#line 441
	D2ASSERT(ptAsyncData);
	if (!ptAsyncData->fLoaded)
	{
		AsyncDataWait(ptAsyncData);
#line 446
		D2ASSERT(ptAsyncData->fLoaded);
	}
	return ptAsyncData->ptBuffer;
}

DWORD __fastcall AsyncDataGetSize(ASYNCDATA *ptAsyncData)
{
#line 457
	D2ASSERT(ptAsyncData);
	return ptAsyncData->dwSize;
}

void __fastcall AsyncDataSetBuffer(ASYNCDATA *ptAsyncData, int nUnused, BYTE *pbBuffer, DWORD dwSize)
{
#line 469
	D2ASSERT(ptAsyncData && ptAsyncData->fLoaded);
#line 472
	D2FREEPOOL(ptAsyncData->dwMemPool, ptAsyncData->ptBuffer);
	ptAsyncData->ptBuffer = pbBuffer;
	ptAsyncData->dwSize = dwSize;
}

void __fastcall AsyncDataFree(ASYNCDATA *ptAsyncData)
{
#line 484
	D2ASSERT(ptAsyncData);
	if (!ptAsyncData->fLoaded && WaitForSingleObject(ptAsyncData->ptEvent->hEvent, 0) == WAIT_OBJECT_0)
		AsyncDataComplete(ptAsyncData);
	if (!ptAsyncData->fLoaded)
	{
		StormSFileCancelRequest(ptAsyncData->ptBuffer);
		WaitForSingleObject(ptAsyncData->ptEvent->hEvent, INFINITE);
		AsyncDataComplete(ptAsyncData);
	}
	if (ptAsyncData->fOwnsBuffer)
	{
#line 518
		D2ASSERT(ptAsyncData->ptBuffer);
#line 521
		D2FREEPOOL(ptAsyncData->dwMemPool, ptAsyncData->ptBuffer);
		ptAsyncData->ptBuffer = NULL;
	}
#line 525
	D2FREEPOOL(ptAsyncData->dwMemPool, ptAsyncData);
}

BOOL __fastcall AsyncDataNothing(void)
{
	return FALSE;
}
