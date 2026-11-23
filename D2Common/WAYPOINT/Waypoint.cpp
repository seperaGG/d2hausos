#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Waypoint\\Waypoint.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

void __stdcall WaypointSetChatNodeMessageByte(BYTE *pRecord, BYTE bVal)
{
	pRecord[8] = bVal;
}

struct WAYPOINTFIELDENTRY { WORD wWordIndex; WORD wMask; };

static struct WAYPOINTFIELDENTRY sgctWaypointFieldTable[MAX_WAYPOINT_FIELDS] =
{
	{ 1, 1 },
	{ 1, 2 },
	{ 1, 4 },
	{ 1, 8 },
	{ 1, 16 },
	{ 1, 32 },
	{ 1, 64 },
	{ 1, 128 },
	{ 1, 256 },
	{ 1, 512 },
	{ 1, 1024 },
	{ 1, 2048 },
	{ 1, 4096 },
	{ 1, 8192 },
	{ 1, 16384 },
	{ 1, 32768 },
	{ 2, 1 },
	{ 2, 2 },
	{ 2, 4 },
	{ 2, 8 },
	{ 2, 16 },
	{ 2, 32 },
	{ 2, 64 },
	{ 2, 128 },
	{ 2, 256 },
	{ 2, 512 },
	{ 2, 1024 },
	{ 2, 2048 },
	{ 2, 4096 },
	{ 2, 8192 },
	{ 2, 16384 },
	{ 2, 32768 },
	{ 3, 1 },
	{ 3, 2 },
	{ 3, 4 },
	{ 3, 8 },
	{ 3, 16 },
	{ 3, 32 },
	{ 3, 64 },
	{ 3, 128 },
	{ 3, 256 },
	{ 3, 512 },
	{ 3, 1024 },
	{ 3, 2048 },
	{ 3, 4096 },
	{ 3, 8192 },
	{ 3, 16384 },
	{ 3, 32768 },
	{ 4, 1 },
	{ 4, 2 },
	{ 4, 4 },
	{ 4, 8 },
	{ 4, 16 },
	{ 4, 32 },
	{ 4, 64 },
	{ 4, 128 },
	{ 4, 256 },
	{ 4, 512 },
	{ 4, 1024 },
	{ 4, 2048 },
	{ 4, 4096 },
	{ 4, 8192 },
	{ 4, 16384 },
	{ 4, 32768 },
	{ 5, 1 },
	{ 5, 2 },
	{ 5, 4 },
	{ 5, 8 },
	{ 5, 16 },
	{ 5, 32 },
	{ 5, 64 },
	{ 5, 128 },
	{ 5, 256 },
	{ 5, 512 },
	{ 5, 1024 },
	{ 5, 2048 },
	{ 5, 4096 },
	{ 5, 8192 },
	{ 5, 16384 },
	{ 5, 32768 },
	{ 6, 1 },
	{ 6, 2 },
	{ 6, 4 },
	{ 6, 8 },
	{ 6, 16 },
	{ 6, 32 },
	{ 6, 64 },
	{ 6, 128 },
	{ 6, 256 },
	{ 6, 512 },
	{ 6, 1024 },
	{ 6, 2048 },
	{ 6, 4096 },
	{ 6, 8192 },
	{ 6, 16384 },
	{ 6, 32768 },
	{ 7, 1 },
	{ 7, 2 },
	{ 7, 4 },
	{ 7, 8 },
	{ 7, 16 },
	{ 7, 32 },
	{ 7, 64 },
	{ 7, 128 },
	{ 7, 256 },
	{ 7, 512 },
	{ 7, 1024 },
	{ 7, 2048 },
	{ 7, 4096 },
	{ 7, 8192 },
	{ 7, 16384 },
	{ 7, 32768 },
};

BOOL __stdcall WaypointFindLevelByIndex(WORD wIndex, DWORD *pdwOutLevelId)
{
	PTLEVELDATAREC ptLevel;
	int nLevelId;

	*pdwOutLevelId = 0;
	if (wIndex >= 255)
		return FALSE;

	for (nLevelId = 1; nLevelId < 109; nLevelId++)
	{
		ptLevel = (PTLEVELDATAREC)D2CommonLvlTblsGetLevelData(nLevelId);
		if ((WORD)ptLevel->Waypoint == wIndex)
		{
			*pdwOutLevelId = (DWORD)nLevelId;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL __stdcall WaypointGetLevelIndex(int nLevelId, WORD *pwOut)
{
	PTLEVELDATAREC ptLevel;
	WORD wVal;

	*pwOut = 255;
	if (!(nLevelId >= 109)) {

		ptLevel = (PTLEVELDATAREC)D2CommonLvlTblsGetLevelData(nLevelId);
		if (ptLevel == NULL)
			return FALSE;

		wVal = ptLevel->Waypoint;
		*pwOut = wVal;
		if (wVal != 255)
			return TRUE; }
	return FALSE;
}

static DWORD sgNumAddress = MAX_WAYPOINT_FIELDS;

DWORD __stdcall WaypointTestField(WORD *pHistory, WORD wField)
{
#line 247
	D2ASSERT(pHistory != NULL);
D2_OPAQUE(sgNumAddress);
#line 248
	D2ASSERT(MAX_WAYPOINT_FIELDS == sgNumAddress);
#line 251
	D2ASSERT(wField < MAX_WAYPOINT_FIELDS);
	return pHistory[sgctWaypointFieldTable[wField].wWordIndex] & sgctWaypointFieldTable[wField].wMask;
}

void __stdcall WaypointSetField(WORD *pHistory, WORD wField)
{
#line 263
	D2ASSERT(pHistory != NULL);
D2_OPAQUE(sgNumAddress);
#line 264
	D2ASSERT(MAX_WAYPOINT_FIELDS == sgNumAddress);
#line 267
	D2ASSERT(wField < MAX_WAYPOINT_FIELDS);
	pHistory[sgctWaypointFieldTable[wField].wWordIndex] |= sgctWaypointFieldTable[wField].wMask;
}

WORD * __stdcall WaypointNewHistory(void *pPool)
{
	WORD *pHistory;

#line 278
	pHistory = (WORD *)D2ALLOCPOOL(pPool, 16);
	memset(pHistory, 0, 16);
	pHistory[0] = 258;
	pHistory[sgctWaypointFieldTable[0].wWordIndex] |= sgctWaypointFieldTable[0].wMask;
	return pHistory;
}

void __stdcall WaypointFreeHistory(void *pPool, WORD *pHistory)
{
	if (pHistory[0] != 258)
	{
		if (pHistory[0] != 0 && pHistory[0] != 257)
		{
#line 301
			D2ASSERT(pHistory[0] == 0 || pHistory[0] == 0x101);
		}
		pHistory[0] = 258;
	}
#line 315
	D2FREEPOOL(pPool, pHistory);
}

WORD * __stdcall WaypointCopyAndResetHistory(WORD *pHistory, WORD *pSrc)
{
	memcpy(pHistory, pSrc, 16);
	if (pHistory[0] != 258)
	{
		if (pHistory[0] != 0 && pHistory[0] != 257)
		{
#line 301
			D2ASSERT(pHistory[0] == 0 || pHistory[0] == 0x101);
		}
		pHistory[0] = 258;
		memset(pHistory, 0, 16);
	}
	pHistory += sgctWaypointFieldTable[0].wWordIndex;
	*pHistory |= sgctWaypointFieldTable[0].wMask;
	return pHistory;
}

void __stdcall WaypointExportHistory(WORD *pHistory, void *pOut)
{
	if (pHistory[0] != 258)
	{
		if (pHistory[0] != 0 && pHistory[0] != 257)
		{
#line 301
			D2ASSERT(pHistory[0] == 0 || pHistory[0] == 0x101);
		}
		pHistory[0] = 258;
	}
	memcpy(pOut, pHistory, 16);
}
