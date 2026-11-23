#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\DrlgRoom.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"
int *__fastcall DrlgLevelGetVis(DRLGLEVEL *ptLevel);

static void __fastcall sLinkExitRooms(void *pPool, ROOM *ptRoom1, BYTE bExit1);
static void __fastcall sSortRooms(ROOM **pptRooms, int nCount);
static BOOL __fastcall sLevelLinkIsBefore(DRLGLEVELLINK *ptOld, DRLGLEVELLINK *ptNew);
static BOOL __fastcall sLinkLevelRooms(void *pPool, ROOM *ptRoom, BYTE bExit, ROOM *ptFirst, BYTE bVis,
	int nWarp);

PTDRLGLOGICNODE __fastcall DrlgRoomGetLogicNode(ROOM *ptRoom, int nX, int nY)
{
	PTDRLGLOGICINFO ptLogic;
	int nCellX;
	int nCellY;

	ptLogic = ptRoom->ptLogic;
	if (ptLogic->dwFlags & 1)
		return ptLogic->ptNodes;
	nCellX = nX / 5 - ptRoom->tCoords.nTileX;
	nCellY = nY / 5 - ptRoom->tCoords.nTileY;
	return (PTDRLGLOGICNODE)DrlgGridGet(&ptLogic->tNodeGrid, nCellX, nCellY);
}

PTDRLGLOGICNODE __fastcall DrlgRoomGetLogicList(ROOM *ptRoom)
{
	return ptRoom->ptLogic->ptNodes;
}

ROOM *__fastcall DrlgRoomNew(DRLGLEVEL *ptLevel, int nKind)
{
	ROOM *ptRoom;
	DWORD i;

#line 31
	ptRoom = (ROOM *)D2ALLOCPOOL(ptLevel->ptAct->pMemPool, sizeof(ROOM));
	for (i = 0; i < sizeof(ROOM) / sizeof(DWORD); i++)
		((DWORD *)ptRoom)[i] = 0;
	ptRoom->ptLevel = ptLevel;
	ptRoom->dwKind = nKind;
	ptRoom->bRing = 4;
	SeedSet(&ptRoom->tSeed, SeedNext(&ptLevel->tSeed));
	ptRoom->dwInitSeed = SeedNext(&ptRoom->tSeed);
	if ((BYTE)ptLevel->dwFlags & 16)
		ptRoom->dwFlags |= 262144;
	switch (nKind)
	{
	case 1:
		OutRoomNewInfo(ptRoom);
		break;
	case 2:
		PresetRoomNewInfo(ptRoom);
		break;
	}
	return ptRoom;
}

void __fastcall DrlgRoomReleaseGameRoom(ROOM *ptRoom, BOOL fClient, DWORD dwFlags)
{
	ptRoom->ptGameRoom = NULL;
	ptRoom->dwGameRoomFlags = dwFlags & 1;
	if (ptRoom->dwFlags & 1048576)
		RoomTileUnload(ptRoom, fClient == FALSE);
}

void __fastcall DrlgRoomFreeWarps(void *pPool, ROOM *ptRoom)
{
	PTDRLGROOMWARP ptWarp;
	PTDRLGROOMWARP ptNext;

	ptNext = ptRoom->ptWarps;
	while (ptNext)
	{
		ptWarp = ptNext;
		ptNext = ptWarp->ptNext;
#line 87
		D2FREEPOOL(pPool, ptWarp);
	}
	ptRoom->ptWarps = NULL;
}

void __fastcall DrlgRoomFree(ROOM *ptRoom)
{
	void *pPool;
	PTPRESETUNIT ptUnit;
	PTPRESETUNIT ptNextUnit;
	PTDRLGROOMLINK ptLink;
	PTDRLGROOMLINK ptNextLink;
	DRLGLEVEL *ptLevel;
	ROOM *ptPrev;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	DrlgRoomFreeWarps(pPool, ptRoom);
	if (ptRoom->pptNear)
	{
#line 104
		D2FREEPOOL(pPool, ptRoom->pptNear);
		ptRoom->nNumNear = 0;
		ptRoom->pptNear = NULL;
	}
	switch (ptRoom->dwKind)
	{
	case 1:
		OutRoomFreeInfo(ptRoom);
		break;
	case 2:
		PresetRoomFreeInfo(ptRoom);
		break;
	}
	ptNextUnit = ptRoom->ptPresetUnits;
	while (ptNextUnit)
	{
		ptUnit = ptNextUnit;
		ptNextUnit = ptUnit->ptNext;
		PresetUnitFree(pPool, ptUnit);
	}
	ptNextLink = ptRoom->ptLinks;
	while (ptNextLink)
	{
		ptLink = ptNextLink;
		ptNextLink = ptLink->ptNext;
		if (ptLink->nKind == 1)
			DrlgRoomUnlink(pPool, ptRoom, ptLink->ptOtherRoom);
	}
	DrlgRoomFreeLinks(pPool, ptRoom->ptLinks);
	ptLevel = ptRoom->ptLevel;
	ptPrev = ptLevel->ptRoomFirst;
	if (ptPrev == ptRoom)
	{
		ptLevel->ptRoomFirst = ptRoom->ptLevelNext;
		ptLevel->nNumRooms--;
	}
	else
	{
		for (; ptPrev->ptLevelNext; ptPrev = ptPrev->ptLevelNext)
		{
			if (ptPrev->ptLevelNext == ptRoom)
			{
				ptPrev->ptLevelNext = ptRoom->ptLevelNext;
				ptLevel->nNumRooms--;
				break;
			}
		}
	}
	RoomTileFree(ptRoom);
	DrlgLogicFree(ptRoom);
#line 173
	D2FREEPOOL(pPool, ptRoom);
}

void __fastcall DrlgRoomFreeLinks(void *pPool, PTDRLGROOMLINK ptLink)
{
	PTDRLGROOMLINK ptNext;

	ptNext = ptLink;
	if (!ptNext)
		return;
	do
	{
		ptLink = ptNext;
		ptNext = ptLink->ptNext;
#line 188
		D2FREEPOOL(pPool, ptLink);
	} while (ptNext);
}

void __fastcall DrlgRoomAddLink(ROOM *ptRoom, ROOM *ptOther, int nDirection)
{
	PTDRLGROOMLINK ptLink;
	DWORD i;

	for (ptLink = ptRoom->ptLinks; ptLink; ptLink = ptLink->ptNext)
	{
		if (ptLink->ptOtherRoom == ptOther)
			return;
	}
#line 211
	ptLink = (PTDRLGROOMLINK)D2ALLOCPOOL(ptRoom->ptLevel->ptAct->pMemPool, sizeof(DRLGROOMLINK));
	for (i = 0; i < sizeof(DRLGROOMLINK) / sizeof(DWORD); i++)
		((DWORD *)ptLink)[i] = 0;
	ptLink->ptNext = ptRoom->ptLinks;
	ptRoom->ptLinks = ptLink;
	ptLink->nDirection = nDirection;
	ptLink->nKind = 1;
	ptLink->ptOtherRoom = ptOther;
	ptLink->ptCoords = &ptOther->tCoords;
}

void __fastcall DrlgRoomLinkRooms(ROOM *ptRoomA, ROOM *ptRoomB, int nDirection)
{
	int nBack;

	nBack = (nDirection - 2) & 3;
	DrlgRoomAddLink(ptRoomA, ptRoomB, nDirection);
	DrlgRoomAddLink(ptRoomB, ptRoomA, nBack);
}

void __fastcall DrlgRoomRemoveLink(void *pPool, ROOM *ptRoom, ROOM *ptOther)
{
	PTDRLGROOMLINK ptPrev;
	PTDRLGROOMLINK ptLink;

	ptPrev = ptRoom->ptLinks;
	ptLink = ptPrev->ptNext;
	if (ptPrev->nKind == 1 && ptPrev->ptOtherRoom == ptOther)
	{
		ptRoom->ptLinks = ptLink;
		ptLink = ptPrev;
	}
	else
	{
		for (; ptLink; ptLink = ptLink->ptNext)
		{
			if (ptLink->nKind == 1 && ptLink->ptOtherRoom == ptOther)
				break;
			ptPrev = ptLink;
		}
		if (!ptLink)
			return;
		ptPrev->ptNext = ptLink->ptNext;
	}
#line 276
	D2FREEPOOL(pPool, ptLink);
}

void __fastcall DrlgRoomUnlink(void *pPool, ROOM *ptRoomA, ROOM *ptRoomB)
{
	DrlgRoomRemoveLink(pPool, ptRoomA, ptRoomB);
	DrlgRoomRemoveLink(pPool, ptRoomB, ptRoomA);
}

void __fastcall DrlgLevelLinkNew(DRLGLEVELLINK **pptList, DRLGLEVEL *ptLevel, int nDirection, int nArg)
{
	DRLGLEVELLINK *ptLink;
	DRLGLEVELLINK *ptPrev;
	DRLGLEVELLINK *ptNext;
	DWORD i;

#line 350
	ptLink = (DRLGLEVELLINK *)D2ALLOCPOOL(ptLevel->ptAct->pMemPool, sizeof(DRLGLEVELLINK));
	for (i = 0; i < sizeof(DRLGLEVELLINK) / sizeof(DWORD); i++)
		((DWORD *)ptLink)[i] = 0;
	ptLink->ptLevel = ptLevel;
	ptLink->nDirection = nDirection;
	ptLink->nPresetNeighbour = nArg;
	ptLink->field_0c = 0;
	ptLink->ptCoords = &ptLevel->tCoords;
	if (!*pptList)
	{
		*pptList = ptLink;
		return;
	}
	ptNext = (*pptList)->ptNext;
	ptPrev = *pptList;
	if (!ptNext)
	{
		if (sLevelLinkIsBefore(ptPrev, ptLink))
		{
			ptLink->ptNext = *pptList;
			*pptList = ptLink;
			return;
		}
	}
	else
	{
		for (; ptNext; ptNext = ptNext->ptNext)
		{
			if (sLevelLinkIsBefore(ptNext, ptLink))
				break;
			ptPrev = ptNext;
		}
	}
	ptLink->ptNext = ptNext;
	ptPrev->ptNext = ptLink;
}

static BOOL __fastcall sLevelLinkIsBefore(DRLGLEVELLINK *ptOld, DRLGLEVELLINK *ptNew)
{
	if (ptOld->nDirection > ptNew->nDirection)
		return TRUE;
	if (ptOld->nDirection != ptNew->nDirection)
		return FALSE;
	switch (ptNew->nDirection)
	{
	case 0:
		if (ptOld->ptCoords->nTileY > ptNew->ptCoords->nTileY)
			return TRUE;
		break;
	case 1:
		if (ptOld->ptCoords->nTileX < ptNew->ptCoords->nTileX)
			return TRUE;
		break;
	case 2:
		if (ptOld->ptCoords->nTileY < ptNew->ptCoords->nTileY)
			return TRUE;
		break;
	case 3:
		if (ptOld->ptCoords->nTileX > ptNew->ptCoords->nTileX)
			return TRUE;
		break;
	}
	return FALSE;
}

BOOL __fastcall DrlgRectTestApartAndGaps(DRLGCOORDS *ptA, DRLGCOORDS *ptB, int nThreshold, int *pnGapX,
	int *pnGapY)
{
	if (ptA->nTileX < ptB->nTileX)
		*pnGapX = ptB->nTileX - ptA->nSizeTileX - ptA->nTileX;
	else
		*pnGapX = ptA->nTileX - ptB->nSizeTileX - ptB->nTileX;
	if (ptA->nTileY < ptB->nTileY)
		*pnGapY = ptB->nTileY - ptA->nSizeTileY - ptA->nTileY;
	else
		*pnGapY = ptA->nTileY - ptB->nSizeTileY - ptB->nTileY;
	if (*pnGapX < nThreshold && *pnGapY < nThreshold)
		return FALSE;
	return TRUE;
}

BOOL __fastcall DrlgRectGapTest(DRLGCOORDS *ptA, DRLGCOORDS *ptB, int nThreshold)
{
	int nGapX;
	int nGapY;

	if (ptA->nTileX < ptB->nTileX)
		nGapX = ptB->nTileX - ptA->nSizeTileX - ptA->nTileX;
	else
		nGapX = ptA->nTileX - ptB->nSizeTileX - ptB->nTileX;
	if (ptA->nTileY < ptB->nTileY)
		nGapY = ptB->nTileY - ptA->nSizeTileY - ptA->nTileY;
	else
		nGapY = ptA->nTileY - ptB->nSizeTileY - ptB->nTileY;
	if (nGapX < nThreshold && nGapY < nThreshold)
		return FALSE;
	return TRUE;
}

BOOL __fastcall DrlgRectIsAdjacent(DRLGCOORDS *ptA, DRLGCOORDS *ptB, int nThreshold)
{
	int nGapX;
	int nGapY;

	nGapX = 0;
	nGapY = 0;
	if (ptA->nTileX <= ptB->nTileX)
		nGapX = ptB->nTileX - ptA->nSizeTileX - ptA->nTileX;
	if (ptA->nTileX > ptB->nTileX)
		nGapX = ptA->nTileX - ptB->nSizeTileX - ptB->nTileX;
	if (ptA->nTileY <= ptB->nTileY)
		nGapY = ptB->nTileY - ptA->nSizeTileY - ptA->nTileY;
	if (ptA->nTileY > ptB->nTileY)
		nGapY = ptA->nTileY - ptB->nSizeTileY - ptB->nTileY;
	if (!nThreshold)
	{
		if (nGapX > 0 || nGapY > 0)
			return FALSE;
		return TRUE;
	}
	if (!nGapX && nGapY <= nThreshold)
		return TRUE;
	if (!nGapY && nGapX <= nThreshold)
		return TRUE;
	return FALSE;
}

BOOL __fastcall DrlgRoomFitsInLevel(DRLGLEVEL *ptLevel, ROOM *ptRoom, ROOM *ptExclude, int nThreshold)
{
	ROOM *ptOther;

	for (ptOther = ptLevel->ptRoomFirst; ptOther; ptOther = ptOther->ptLevelNext)
	{
		if (ptOther == ptRoom || ptOther == ptExclude)
			continue;
		if (!DrlgRectGapTest(&ptRoom->tCoords, &ptOther->tCoords, nThreshold))
			return FALSE;
	}
	return TRUE;
}

void __fastcall DrlgRoomLink(DRLGLEVEL *ptLevel, ROOM *ptRoom)
{
	ptRoom->ptLevelNext = ptLevel->ptRoomFirst;
	ptLevel->ptRoomFirst = ptRoom;
	ptLevel->nNumRooms++;
}

BOOL __fastcall DrlgRectContains(DRLGCOORDS *ptCoords, int nX, int nY)
{
	if (nX < ptCoords->nTileX)
		return FALSE;
	if (nY < ptCoords->nTileY)
		return FALSE;
	if (nX >= ptCoords->nTileX + ptCoords->nSizeTileX)
		return FALSE;
	return nY < ptCoords->nTileY + ptCoords->nSizeTileY;
}

BOOL __fastcall DrlgRectContainsInclusive(DRLGCOORDS *ptRect, int nX, int nY)
{
	if (nX < ptRect->nTileX)
		return FALSE;
	if (nY < ptRect->nTileY)
		return FALSE;
	if (nX > ptRect->nTileX + ptRect->nSizeTileX)
		return FALSE;
	return nY <= ptRect->nTileY + ptRect->nSizeTileY;
}

BOOL __fastcall DrlgRectContainsSubtile(DRLGCOORDS *ptRect, int nX, int nY)
{
	int nLeft = ptRect->nTileX * 5;
	int nTop;

	if (nX < nLeft)
		return FALSE;
	if (nX >= nLeft + ptRect->nSizeTileX * 5)
		return FALSE;
	nTop = ptRect->nTileY * 5;
	if (nY < nTop)
		return FALSE;
	return nY < nTop + ptRect->nSizeTileY * 5;
}

DWORD __fastcall DrlgRoomGetKindFlag(ROOM *ptRoom)
{
	switch (ptRoom->dwKind)
	{
	case 1:
		return 1;
	case 2:
		return ptRoom->dwFlags & 524288;
	}
	return 0;
}

DWORD __fastcall DrlgRoomGetOutdoorFlag(ROOM *ptRoom)
{
	switch (ptRoom->dwKind)
	{
	case 1:
		return OutRoomGetCellFlagBit(ptRoom);
	}
	return 0;
}

DWORD __fastcall DrlgRoomGetExitFlags(ROOM *ptRoom)
{
	return ptRoom->dwFlags & 196608;
}

DWORD __fastcall DrlgRoomGetLevelId(ROOM *ptRoom)
{
	return ptRoom->ptLevel->nLevelId;
}

int __fastcall DrlgRoomGetVisLevel(ROOM *ptRoom, int nIndex)
{
	return ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptRoom->ptLevel->nLevelId))->Vis[nIndex];
}

DWORD __fastcall DrlgRoomGetLevelIdChecked(ROOM *ptRoom)
{
#line 610
	D2WARN(ptRoom);
	if (ptRoom->dwFlags & 8388608)
		return 0;
	return ptRoom->ptLevel->nLevelId;
}

BOOL __fastcall DrlgRoomHasExitFlags(ROOM *ptRoom)
{
#line 621
	D2WARN(ptRoom);
	return (ptRoom->dwFlags & 196608) != 0;
}

const char *__fastcall DrlgRoomGetFileName(ROOM *ptRoom)
{
	if (ptRoom->dwKind == 2)
		return (const char *)PresetRoomGetFile((PTPRESETROOM)ptRoom);
	return "None";
}

int __fastcall DrlgRoomGetNearCount(ROOM *ptRoom)
{
	return ptRoom->nNumNear;
}

int __fastcall DrlgRoomGetNearGameRooms(ROOM *ptRoom, DUNGEONROOM **pptOut)
{
	DUNGEONROOM *ptGameRoom;
	int nCount;
	int i;

	nCount = 0;
	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptGameRoom = ptRoom->pptNear[i]->ptGameRoom;
		if (ptGameRoom)
		{
			pptOut[nCount] = ptGameRoom;
			nCount++;
		}
	}
	for (i = nCount; i < ptRoom->nNumNear; i++)
		pptOut[i] = NULL;
	return nCount;
}

static void __fastcall sCollectNearRooms(void *pPool, ROOM *ptRoom)
{
	ROOM *aptNear[30];
	ROOM *ptOther;
	int i;

	ptOther = ptRoom->ptLevel->ptRoomFirst;
	ptRoom->nNumNear = 0;
	while (ptOther)
	{
		if (DrlgRectGapTest(&ptRoom->tCoords, &ptOther->tCoords, 6))
			ptOther = ptOther->ptLevelNext;
		else
		{
			aptNear[ptRoom->nNumNear] = ptOther;
			ptOther = ptOther->ptLevelNext;
			ptRoom->nNumNear++;
		}
	}
	sSortRooms(aptNear, ptRoom->nNumNear);
#line 690
	ptRoom->pptNear = (ROOM **)D2ALLOCPOOL(pPool, ptRoom->nNumNear * sizeof(ROOM *));
	for (i = 0; i < ptRoom->nNumNear; i++)
		ptRoom->pptNear[i] = aptNear[i];
}

static void __fastcall sFlagNearTown(ROOM *ptRoom)
{
	int i;

	if (!DrlgLevelIsTown((int)ptRoom->ptLevel->nLevelId))
	{
		for (i = 0; i < ptRoom->nNumNear; i++)
		{
			if (DrlgLevelIsTown((int)ptRoom->pptNear[i]->ptLevel->nLevelId))
			{
				ptRoom->dwFlags |= 8388608;
				return;
			}
		}
	}
}

void __fastcall DrlgRoomBuildNear(void *pPool, ROOM *ptRoom1)
{
	DWORD dwBit;
	BYTE bExit1;

	sCollectNearRooms(pPool, ptRoom1);
	if (ptRoom1->dwFlags & 4080)
	{
		for (bExit1 = 0, dwBit = 16; dwBit & 4080; dwBit <<= 1, bExit1++)
		{
			if (!(ptRoom1->dwFlags & dwBit))
				continue;
			sLinkExitRooms(pPool, ptRoom1, bExit1);
		}
	}
	sFlagNearTown(ptRoom1);
}

#pragma auto_inline(off)
static void __fastcall sSortRooms(ROOM **pptRooms, int nCount)
{
	int nPass;
	int i;
	ROOM *ptA;
	ROOM *ptB;

	for (nPass = nCount - 1; nPass > 0; nPass--)
	{
		for (i = 0; i < nCount - 1; i++)
		{
			ptA = pptRooms[i];
			ptB = pptRooms[i + 1];
			if (ptA->tCoords.nTileX >= ptB->tCoords.nSizeTileX + ptB->tCoords.nTileX ||
				ptA->tCoords.nTileY >= ptB->tCoords.nSizeTileY + ptB->tCoords.nTileY)
			{
				pptRooms[i] = ptB;
				pptRooms[i + 1] = ptA;
			}
		}
	}
}
#pragma auto_inline(on)

static __inline void sAddNearRoom(void *pPool, ROOM *ptRoom, ROOM *ptOther)
{
	ROOM **pptNear;

	ptRoom->nNumNear++;
#line 757
	pptNear = (ROOM **)D2ALLOCPOOL(pPool, ptRoom->nNumNear * sizeof(ROOM *));
	if (ptRoom->nNumNear - 1 > 0)
		memcpy(pptNear, ptRoom->pptNear, (ptRoom->nNumNear - 1) * sizeof(ROOM *));
#line 760
	D2FREEPOOL(pPool, ptRoom->pptNear);
	ptRoom->pptNear = pptNear;
	pptNear[ptRoom->nNumNear - 1] = ptOther;
	sSortRooms(ptRoom->pptNear, ptRoom->nNumNear);
}

static BOOL __fastcall sLinkLevelRooms(void *pPool, ROOM *ptRoom, BYTE bExit, ROOM *ptOther, BYTE bVis,
	int nWarp)
{
	DWORD dwBit;
	BOOL fLinked;
	PTDRLGROOMWARP ptWarp;
	DWORD i;

	dwBit = 1 << (bVis + 4);
	fLinked = FALSE;
	for (; ptOther; ptOther = ptOther->ptLevelNext)
	{
		if (!(ptOther->dwFlags & dwBit))
			continue;
		if (nWarp != -1)
		{
			sAddNearRoom(pPool, ptRoom, ptOther);
#line 777
			ptWarp = (PTDRLGROOMWARP)D2ALLOCPOOL(ptRoom->ptLevel->ptAct->pMemPool, sizeof(DRLGROOMWARP));
			for (i = 0; i < sizeof(DRLGROOMWARP) / sizeof(DWORD); i++)
				((DWORD *)ptWarp)[i] = 0;
			ptWarp->bExit = bExit;
			ptWarp->ptDestRoom = ptOther;
			ptWarp->nWarpId = nWarp;
			ptWarp->nDefAllowed = 1;
			ptWarp->ptNext = ptRoom->ptWarps;
			ptRoom->ptWarps = ptWarp;
			return TRUE;
		}
		if (!DrlgRectGapTest(&ptRoom->tCoords, &ptOther->tCoords, 6))
		{
			sAddNearRoom(pPool, ptRoom, ptOther);
			fLinked = TRUE;
		}
	}
	return fLinked;
}

static void __fastcall sLinkExitRooms(void *pPool, ROOM *ptRoom1, BYTE bExit1)
{
	DRLGLEVEL *ptLevel1;
	int nLevelId;
	int *pnVis1;
	int nVisLevelId;
	DRLGLEVEL *ptVisLevel;
	int *pnVis2;
	int nWarp;
	int nSame;
	int n;
	BYTE b;

#line 825
	D2WARN(ptRoom1 && bExit1 < LEVEL_VIS_MAX);
	ptLevel1 = ptRoom1->ptLevel;
	nLevelId = (int)ptLevel1->nLevelId;
	pnVis1 = DrlgRoomGetVis(ptLevel1->ptAct, nLevelId);
	nVisLevelId = pnVis1[bExit1];
	ptVisLevel = DrlgGetLevel(ptLevel1->ptAct, (DWORD)nVisLevelId);
	pnVis2 = DrlgRoomGetVis(ptLevel1->ptAct, nVisLevelId);
	nWarp = DrlgWarpGetWarp(ptLevel1, bExit1);
	if (!ptVisLevel->ptRoomFirst)
		DrlgGenerateLevel(ptVisLevel);
	if (nWarp != -1)
	{
		nSame = 0;
		n = 0;
		for (b = 0; b < bExit1; b++)
		{
			if (pnVis1[b] == nVisLevelId)
				nSame++;
		}
		for (b = 0; b < LEVEL_VIS_MAX; b++)
		{
			if (pnVis2[b] == nLevelId)
			{
				if (nSame == n)
				{
					if (sLinkLevelRooms(pPool, ptRoom1, bExit1, ptVisLevel->ptRoomFirst, b, nWarp))
						return;
					break;
				}
				n++;
			}
		}
	}
	for (b = 0; b < LEVEL_VIS_MAX; b++)
	{
		if (pnVis2[b] == nLevelId &&
			sLinkLevelRooms(pPool, ptRoom1, bExit1, ptVisLevel->ptRoomFirst, b, nWarp))
			break;
	}
}

PTPRESETUNIT __fastcall DrlgRoomNewPresetUnit(ROOM *ptRoom, void *pPool, int nType, int nId, int nMode,
	int nX, int nY)
{
	PTPRESETUNIT ptUnit;
	DWORD i;

#line 918
	ptUnit = (PTPRESETUNIT)D2ALLOCPOOL(pPool, sizeof(PRESETUNIT));
	for (i = 0; i < sizeof(PRESETUNIT) / sizeof(DWORD); i++)
		((DWORD *)ptUnit)[i] = 0;
	ptUnit->nUnitType = nType;
	ptUnit->nClassId = nId;
	ptUnit->nMode = nMode;
	ptUnit->nX = nX;
	ptUnit->nY = nY;
	if (ptRoom)
	{
		ptUnit->ptNext = ptRoom->ptPresetUnits;
		ptRoom->ptPresetUnits = ptUnit;
	}
	else
		ptUnit->ptNext = NULL;
	return ptUnit;
}

PTPRESETUNIT __fastcall DrlgRoomGetPresetUnits(ROOM *ptRoom)
{
	if (!(ptRoom->dwFlags & 262144))
	{
		if (ptRoom->dwFlags & 67108864)
			return NULL;
		ptRoom->dwFlags |= 67108864;
	}
	return ptRoom->ptPresetUnits;
}

void __fastcall DrlgRoomSetGameRoom(ROOM *ptRoom, DUNGEONROOM *ptGameRoom)
{
	ptRoom->ptGameRoom = ptGameRoom;
}

void __fastcall DrlgRoomGetLight(ROOM *ptRoom, BYTE *pbIntensity, BYTE *pbRed, BYTE *pbGreen, BYTE *pbBlue)
{
	PTLEVELSREC ptRecord;

	ptRecord = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptRoom->ptLevel->nLevelId);
	*pbIntensity = ptRecord->Intensity;
	*pbRed = ptRecord->Red;
	*pbGreen = ptRecord->Green;
	*pbBlue = ptRecord->Blue;
}

int __fastcall DrlgRoomGetDrlgType(ROOM *ptRoom)
{
	return ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptRoom->ptLevel->nLevelId))->DrlgType;
}

int *__fastcall DrlgRoomGetVis(DRLGACTCTX *ptAct, int nLevelId)
{
	PTVISINFO ptVisInfo;

	for (ptVisInfo = ptAct->ptVisInfo; ptVisInfo; ptVisInfo = ptVisInfo->ptNext)
	{
#line 1006
		D2WARN(ptVisInfo->eLevelId != LEVEL_ID_NONE);
		if (nLevelId == ptVisInfo->eLevelId)
			return ptVisInfo->anVis;
	}
	return ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->Vis;
}

int *__fastcall DrlgLevelGetVis(DRLGLEVEL *ptLevel)
{
	return DrlgRoomGetVis(ptLevel->ptAct, (int)ptLevel->nLevelId);
}
