#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\Drlg.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "FogImports.h"

static void __fastcall sFreeLevelContents(void *pPool, DRLGLEVEL *ptLevel, BOOL fKeep);
static __inline void sLevelGenerate(DRLGLEVEL *ptLevel);
DWORD __fastcall DrlgActGetLevelAt(DRLGACTCTX *ptAct, int nX, int nY);
void __fastcall DrlgActInitRooms(DRLGACTCTX *ptAct);

DRLGACTCTX *__fastcall DrlgNewAct(ACT *ptOwner, BYTE bAct, struct HD2ARCHIVE__ *hArchive, DWORD dwMapSeed,
	int nInitLevel, DWORD dwFlags, DWORD dwGame, PFNDRLGROOMBUILT pfnRoomBuilt,
	PFNDRLGTOWNPLACED pfnTownPlaced)
{
	void *pPool;
	DRLGACTCTX *ptAct;
	int nLevelA;
	int nLevelB;

	pPool = DungeonActGetPool(ptOwner);
#line 19
	ptAct = (DRLGACTCTX *)D2ALLOCPOOL(pPool, sizeof(DRLGACTCTX));
	memset(ptAct, 0, sizeof(DRLGACTCTX));
	ptAct->ptOwnerAct = ptOwner;
	ptAct->pMemPool = pPool;
	ptAct->bAct = bAct;
	ptAct->hArchive = hArchive;
	SeedSet(&ptAct->tSeed, dwMapSeed);
	ptAct->dwDungeonSeed = SeedNext(&ptAct->tSeed);
	ptAct->dwFlags = dwFlags;
	ptAct->dwMapSeed = dwMapSeed;
	ptAct->field_3e8 = dwGame;
	ptAct->pfnRoomBuilt = pfnRoomBuilt;
	ptAct->pfnTownPlaced = pfnTownPlaced;
	if (bAct == 1)
	{
		do
		{
			nLevelA = (int)SeedRand(&ptAct->tSeed, 7);
			nLevelB = (int)SeedRand(&ptAct->tSeed, 7);
		} while (nLevelA == nLevelB);
		ptAct->nTombLevelIdA = nLevelA + 66;
		ptAct->nTombLevelIdB = nLevelB + 66;
	}
	else if (bAct == 2)
		ptAct->dwKurastBorderSide = SeedNext(&ptAct->tSeed) & 1;
	DrlgActInitRooms(ptAct);
	OutdoorsLayoutAct(ptAct, bAct);
	if (nInitLevel)
		sLevelGenerate(DrlgGetLevel(ptAct, nInitLevel));
	return ptAct;
}

void __fastcall DrlgFreeAct(DRLGACTCTX *ptAct)
{
	DRLGLEVEL *ptLevel;
	DRLGLEVEL *ptNextLevel;
	PTVISINFO ptVis;
	PTVISINFO ptNextVis;

	if (ptAct->ptFirstLevel)
	{
		ptNextLevel = ptAct->ptFirstLevel;
		while (ptNextLevel)
		{
			ptLevel = ptNextLevel;
			ptNextLevel = ptLevel->ptNext;
			sFreeLevelContents(ptAct->pMemPool, ptLevel, FALSE);
#line 177
			D2FREEPOOL(ptAct->pMemPool, ptLevel);
		}
		ptAct->ptFirstLevel = NULL;
	}
	if (ptAct->ptVisInfo)
	{
		ptNextVis = ptAct->ptVisInfo;
		while (ptNextVis)
		{
			ptVis = ptNextVis;
			ptNextVis = ptVis->ptNext;
#line 192
			D2FREEPOOL(ptAct->pMemPool, ptVis);
		}
		ptAct->ptVisInfo = NULL;
	}
#line 198
	D2FREEPOOL(ptAct->pMemPool, ptAct);
}

static void __fastcall sFreeLevelContents(void *pPool, DRLGLEVEL *ptLevel, BOOL fKeep)
{
	ROOM *ptRoom;
	ROOM *ptNextRoom;
	PTDRLGFILECYCLE ptCycle;
	PTDRLGFILECYCLE ptNextCycle;
	int nRoom;

	if (fKeep)
	{
		if (!ptLevel->pdwSavedRoomFlags)
		{
			if (ptLevel->nNumRooms)
			{
#line 80
				ptLevel->pdwSavedRoomFlags = (DWORD *)D2ALLOCPOOL(pPool, ptLevel->nNumRooms * sizeof(DWORD));
			}
			else
				ptLevel->nNumRooms = 0;
			memset(ptLevel->pdwSavedRoomFlags, 0, ptLevel->nNumRooms * sizeof(DWORD));
		}
	}
	else if (ptLevel->pdwSavedRoomFlags)
	{
#line 88
		D2FREEPOOL(pPool, ptLevel->pdwSavedRoomFlags);
		ptLevel->pdwSavedRoomFlags = NULL;
	}
	if (ptLevel->ptRoomFirst)
	{
		ptNextRoom = ptLevel->ptRoomFirst;
		nRoom = 0;
		while (ptNextRoom)
		{
			ptRoom = ptNextRoom;
			ptNextRoom = ptRoom->ptLevelNext;
			if (ptLevel->pdwSavedRoomFlags)
			{
				ptLevel->pdwSavedRoomFlags[nRoom] = ptRoom->dwGameRoomFlags & 1;
				nRoom++;
			}
			DrlgRoomFree(ptRoom);
		}
		ptLevel->ptRoomFirst = NULL;
		ptLevel->nNumRooms = 0;
	}
	if (ptLevel->ptRegionFirst)
	{
		PresetFreeRegions(pPool, ptLevel->ptRegionFirst);
		ptLevel->ptRegionFirst = NULL;
	}
	switch (ptLevel->dwDrlgType)
	{
	case 1:
		MazeLevelFree(ptLevel, fKeep);
		break;
	case 2:
		PresetLevelFree(ptLevel, fKeep);
		break;
	case 3:
		OutdoorsLevelFree(ptLevel, fKeep);
		break;
	}
	memset(ptLevel->atPresetMarks, 0, sizeof(ptLevel->atPresetMarks));
	ptLevel->nNumPresetMarks = 0;
	memset(ptLevel->anWarpRoomX, 0, sizeof(ptLevel->anWarpRoomX) + sizeof(ptLevel->anWarpRoomY) +
		sizeof(ptLevel->nNumWarpRooms));
	if (!fKeep && ptLevel->pnJungleDefs)
	{
#line 139
		D2FREEPOOL(pPool, ptLevel->pnJungleDefs);
		ptLevel->pnJungleDefs = NULL;
	}
	if (ptLevel->ptFileCycleFirst)
	{
		ptNextCycle = ptLevel->ptFileCycleFirst;
		while (ptNextCycle)
		{
			ptCycle = ptNextCycle;
			ptNextCycle = ptCycle->ptNext;
#line 153
			D2FREEPOOL(pPool, ptCycle);
		}
		ptLevel->ptFileCycleFirst = NULL;
	}
}

void __fastcall DrlgLevelAddUnits(DRLGLEVEL *ptLevel, int nDelta)
{
	DRLGLEVEL *ptVisLevel;
	int *pnVis;
	int i;

	ptLevel->nNumClients += nDelta;
	ptLevel->nFreeCountdown = 10;
	pnVis = DrlgRoomGetVis(ptLevel->ptAct, (int)ptLevel->nLevelId);
	for (i = 0; i < 8; i++)
	{
		if (pnVis[i])
		{
			ptVisLevel = DrlgGetLevel(ptLevel->ptAct, (DWORD)pnVis[i]);
			ptVisLevel->nFreeCountdown = 10;
			ptVisLevel->nNumClients += nDelta;
		}
	}
}

void __fastcall DrlgRoomChangeLevel(ROOM *ptFrom, ROOM *ptTo)
{
	DRLGLEVEL *ptLevelFrom;
	DRLGLEVEL *ptLevelTo;
	DRLGLEVEL *ptLevel;
	int *pnVis;
	int i;

	ptLevelFrom = ptFrom ? ptFrom->ptLevel : NULL;
	ptLevelTo = ptTo ? ptTo->ptLevel : NULL;
	if (ptLevelFrom == ptLevelTo)
		return;
	if (ptLevelFrom)
	{
		ptLevelFrom->nNumClients--;
		ptLevelFrom->nFreeCountdown = 10;
		pnVis = DrlgRoomGetVis(ptLevelFrom->ptAct, (int)ptLevelFrom->nLevelId);
		for (i = 0; i < 8; i++)
		{
			if (pnVis[i])
			{
				ptLevel = DrlgGetLevel(ptLevelFrom->ptAct, (DWORD)pnVis[i]);
				ptLevel->nFreeCountdown = 10;
				ptLevel->nNumClients--;
			}
		}
	}
	if (ptLevelTo)
	{
		ptLevelTo->nNumClients++;
		ptLevelTo->nFreeCountdown = 10;
		pnVis = DrlgRoomGetVis(ptLevelTo->ptAct, (int)ptLevelTo->nLevelId);
		for (i = 0; i < 8; i++)
		{
			if (pnVis[i])
			{
				ptLevel = DrlgGetLevel(ptLevelTo->ptAct, (DWORD)pnVis[i]);
				ptLevel->nFreeCountdown = 10;
				ptLevel->nNumClients++;
			}
		}
	}
}

static __inline BOOL sLevelReleaseVisRooms(DRLGLEVEL *ptLevel)
{
	void *pLevelPool;
	DRLGLEVEL *ptVisLevel;
	ROOM *ptRoom;
	int *pnVis;
	int *pnVisVis;
	DWORD dwMask;
	int i;
	int j;

	pLevelPool = ptLevel->ptAct->pMemPool;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptRoom->bRing < 4 || (ptRoom->dwFlags & 1048576))
			return FALSE;
	}
	pnVis = DrlgRoomGetVis(ptLevel->ptAct, (int)ptLevel->nLevelId);
	for (i = 0; i < 8; i++)
	{
		if (!pnVis[i])
			continue;
		ptVisLevel = DrlgGetLevel(ptLevel->ptAct, (DWORD)pnVis[i]);
		ptRoom = ptVisLevel->ptRoomFirst;
		if (!ptRoom)
			continue;
		pnVisVis = DrlgRoomGetVis(ptVisLevel->ptAct, (int)ptVisLevel->nLevelId);
		dwMask = 0;
		for (j = 0; j < 8; j++)
		{
			if (pnVisVis[j] == (int)ptLevel->nLevelId)
				dwMask |= 1 << (j + 4);
		}
		for (; ptRoom; ptRoom = ptRoom->ptLevelNext)
		{
			if ((dwMask & ptRoom->dwFlags) &&
				(ptRoom->bRing < 4 || (ptRoom->dwFlags & 1048576)))
				return FALSE;
		}
		for (ptRoom = ptVisLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
		{
			if (ptRoom->dwFlags & dwMask)
			{
				DrlgRoomFreeWarps(pLevelPool, ptRoom);
				if (ptRoom->pptNear)
				{
#line 316
					D2FREEPOOL(pLevelPool, ptRoom->pptNear);
					ptRoom->pptNear = NULL;
					ptRoom->nNumNear = 0;
				}
			}
		}
	}
	return TRUE;
}

void __fastcall DrlgActFreeIdleLevels(DRLGACTCTX *ptAct)
{
	void *pPool;
	DRLGLEVEL *ptLevel;

	pPool = ptAct->pMemPool;
	for (ptLevel = ptAct->ptFirstLevel; ptLevel; ptLevel = ptLevel->ptNext)
	{
		if (ptLevel->nNumClients || !ptLevel->ptRoomFirst)
			continue;
		if (ptLevel->nFreeCountdown)
			ptLevel->nFreeCountdown--;
		else if (sLevelReleaseVisRooms(ptLevel))
			sFreeLevelContents(pPool, ptLevel, TRUE);
		else
			ptLevel->nFreeCountdown = 10;
	}
}

DRLGLEVEL *__fastcall DrlgNewLevel(DRLGACTCTX *ptAct, int nLevelId)
{
	DRLGLEVEL *ptLevel;
	DWORD i;

#line 372
	ptLevel = (DRLGLEVEL *)D2ALLOCPOOL(ptAct->pMemPool, sizeof(DRLGLEVEL));
	for (i = 0; i < sizeof(DRLGLEVEL) / sizeof(DWORD); i++)
		((DWORD *)ptLevel)[i] = 0;
	ptLevel->ptAct = ptAct;
	ptLevel->nLevelId = nLevelId;
	ptLevel->dwLevelType = ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->LevelType;
	ptLevel->dwDrlgType = ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->DrlgType;
	if ((BYTE)ptAct->dwFlags & 1)
		ptLevel->dwFlags |= 16;
	SeedSet(&ptLevel->tSeed, ptLevel->ptAct->dwDungeonSeed + ptLevel->nLevelId);
	switch (ptLevel->dwDrlgType)
	{
	case 1:
		MazeLevelInit(ptLevel);
		break;
	case 2:
		PresetLevelInit(ptLevel);
		break;
	case 3:
		OutdoorsLevelInit(ptLevel);
		break;
	}
	ptLevel->ptNext = ptAct->ptFirstLevel;
	ptAct->ptFirstLevel = ptLevel;
	return ptLevel;
}

PTDRLGLEVEL __stdcall DrlgGetLevel(struct DRLGACTCTX *pDrlgAct, DWORD nLevelId)
{
	PTDRLGLEVEL ptLevel;

	for (ptLevel = pDrlgAct->ptFirstLevel; ptLevel; ptLevel = ptLevel->ptNext)
	{
		if (ptLevel->nLevelId == nLevelId)
			return ptLevel;
	}
	return DrlgNewLevel(pDrlgAct, (int)nLevelId);
}

int __fastcall DrlgActGetDrawnLevel(DRLGACTCTX *ptAct)
{
	if (ptAct)
		return ptAct->nTombLevelIdA;
	return 0;
}

int __fastcall DrlgRectEdgeDir(DRLGCOORDS *ptA, DRLGCOORDS *ptB)
{
	if (ptA->nTileX > ptB->nTileX)
	{
		if (ptA->nTileX == ptB->nTileX + ptB->nSizeTileX)
			return 0;
	}
	else if (ptB->nTileX == ptA->nTileX + ptA->nSizeTileX)
		return 2;
	if (ptA->nTileY > ptB->nTileY)
	{
		if (ptA->nTileY == ptB->nTileY + ptB->nSizeTileY)
			return 1;
	}
	else if (ptB->nTileY == ptA->nTileY + ptA->nSizeTileY)
		return 3;
	return -1;
}

void __fastcall DrlgRoomNewGameRoom(DRLGACTCTX *ptAct, ROOM *ptRoom)
{
	GAMERECT atRects[2] = { 0 };
	PTDRLGTILEARRAYS ptArrays;
	DWORD dwFlags;

	atRects[1].nX = ptRoom->tCoords.nTileX;
	atRects[1].nY = ptRoom->tCoords.nTileY;
	atRects[1].nWidth = ptRoom->tCoords.nSizeTileX;
	atRects[1].nHeight = ptRoom->tCoords.nSizeTileY;
	atRects[0].nX = ptRoom->tCoords.nTileX;
	atRects[0].nY = ptRoom->tCoords.nTileY;
	atRects[0].nWidth = ptRoom->tCoords.nSizeTileX;
	atRects[0].nHeight = ptRoom->tCoords.nSizeTileY;
	EnvTileToSubtile(&atRects[0].nX, &atRects[0].nY);
	EnvTileToSubtile(&atRects[0].nWidth, &atRects[0].nHeight);
	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	if (ptArrays->nNumWalls || ptArrays->nNumFloors)
	{
		dwFlags = 0;
		if (ptRoom->dwFlags & 262144)
			dwFlags = 4;
		else if (ptRoom->dwGameRoomFlags & 1)
			dwFlags = 1;
		ptRoom->ptGameRoom = DungeonNewRoom(ptAct->ptOwnerAct, ptRoom, atRects, ptArrays,
			SeedNext(&ptRoom->tSeed), dwFlags);
	}
}

int *__fastcall DrlgRoomGetLevelWarpX(ROOM *ptRoom)
{
	return ptRoom->ptLevel->anWarpRoomX;
}

static __inline void sLevelGenerate(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	int i;
	BOOL fWarp;
	DWORD dwFlags;
	DWORD dwBit;
	BYTE bWarp;
	int *pnX;
	int *pnY;

	SeedSet(&ptLevel->tSeed, ptLevel->ptAct->dwDungeonSeed + ptLevel->nLevelId);
	switch (ptLevel->dwDrlgType)
	{
	case 1:
		MazeLevelGenerate(ptLevel);
		break;
	case 2:
		PresetLevelGenerate(ptLevel);
		break;
	case 3:
		OutdoorsLevelGenerate(ptLevel);
		break;
	}
	if (ptLevel->nNumRooms && ptLevel->pdwSavedRoomFlags && ptLevel->ptRoomFirst)
	{
		i = 0;
		for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
		{
			if (ptLevel->pdwSavedRoomFlags[i++])
				ptRoom->dwGameRoomFlags |= 1;
		}
	}
	fWarp = FALSE;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		dwFlags = ptRoom->dwFlags;
		if (dwFlags & 196608)
			fWarp = TRUE;
		if ((dwFlags & 4080) && !fWarp)
		{
			for (dwBit = 16, bWarp = 0; dwBit & 4080; dwBit <<= 1, bWarp++)
			{
				if ((ptRoom->dwFlags & dwBit) && DrlgWarpGetWarp(ptLevel, bWarp) != -1)
				{
					fWarp = TRUE;
					break;
				}
			}
		}
		if (fWarp)
		{
			pnX = &ptLevel->anWarpRoomX[ptLevel->nNumWarpRooms];
			pnY = &ptLevel->anWarpRoomY[ptLevel->nNumWarpRooms];
			fWarp = FALSE;
			*pnX = ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX / 2;
			*pnY = ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY / 2;
			EnvTileToSubtile(pnX, pnY);
			ptLevel->nNumWarpRooms++;
		}
	}
}

void __stdcall DrlgGenerateLevel(DRLGLEVEL *ptLevel)
{
	sLevelGenerate(ptLevel);
}

int __fastcall DrlgActCountLevelRooms(DRLGACTCTX *ptAct, int nLevelId)
{
	DRLGLEVEL *ptLevel;
	ROOM *ptRoom;
	int nCount;

	nCount = 0;
	ptLevel = DrlgGetLevel(ptAct, (DWORD)nLevelId);
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (!(ptRoom->dwFlags & 8388608))
			nCount++;
	}
	return nCount;
}

void __fastcall DrlgLevelGetRoomBounds(DRLGLEVEL *ptLevel, int *pnMinX, int *pnMinY, int *pnMaxX, int *pnMaxY)
{
	ROOM *ptRoom;

	ptRoom = ptLevel->ptRoomFirst;
	*pnMinX = ptRoom->tCoords.nTileX;
	*pnMinY = ptRoom->tCoords.nTileY;
	*pnMaxX = ptRoom->tCoords.nSizeTileX + ptRoom->tCoords.nTileX;
	*pnMaxY = ptRoom->tCoords.nSizeTileY + ptRoom->tCoords.nTileY;
	for (; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptRoom->tCoords.nSizeTileX + ptRoom->tCoords.nTileX > *pnMaxX)
			*pnMaxX = ptRoom->tCoords.nSizeTileX + ptRoom->tCoords.nTileX;
		if (ptRoom->tCoords.nTileX < *pnMinX)
			*pnMinX = ptRoom->tCoords.nTileX;
		if (ptRoom->tCoords.nSizeTileY + ptRoom->tCoords.nTileY > *pnMaxY)
			*pnMaxY = ptRoom->tCoords.nSizeTileY + ptRoom->tCoords.nTileY;
		if (ptRoom->tCoords.nTileY < *pnMinY)
			*pnMinY = ptRoom->tCoords.nTileY;
	}
}

void __fastcall DrlgLevelFitRooms(DRLGLEVEL *ptDrlgLevel)
{
	DRLGCOORDS *ptCoordsLevel;
	ROOM *ptRoom;
	int nTileMinX;
	int nTileMinY;
	int nTileMaxX;
	int nTileMaxY;

	ptCoordsLevel = &ptDrlgLevel->tCoords;
#line 684
	D2WARN(ptDrlgLevel);
#line 685
	D2WARN(ptDrlgLevel->ptRoomFirst);
	DrlgLevelGetRoomBounds(ptDrlgLevel, &nTileMinX, &nTileMinY, &nTileMaxX, &nTileMaxY);
	ptRoom = ptDrlgLevel->ptRoomFirst;
#line 692
	D2WARN(ptCoordsLevel->nSizeTileX >= nTileMaxX - nTileMinX);
#line 693
	D2WARN(ptCoordsLevel->nSizeTileY >= nTileMaxY - nTileMinY);
	for (; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		ptRoom->tCoords.nTileX += ptCoordsLevel->nTileX - nTileMinX;
		ptRoom->tCoords.nTileY += ptCoordsLevel->nTileY - nTileMinY;
	}
}

ROOM *__fastcall DrlgLevelFindRoom(DRLGLEVEL *ptDrlgLevel, int nX, int nY)
{
	ROOM *ptRoom;

#line 710
	D2WARN(ptDrlgLevel);
#line 711
	D2WARN(ptDrlgLevel->ptRoomFirst);
	for (ptRoom = ptDrlgLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (DrlgRectContains(&ptRoom->tCoords, nX, nY))
			return ptRoom;
	}
	return NULL;
}

ROOM *__fastcall DrlgActFindRoom(int nX, int nY, DRLGACTCTX *ptAct, ROOM *ptHint, DRLGLEVEL *ptDrlgLevel)
{
	ROOM *ptNear;
	int i;

	if (ptHint)
	{
		if (DrlgRectContains(&ptHint->tCoords, nX, nY))
			return ptHint;
		for (i = 0; i < ptHint->nNumNear; i++)
		{
			ptNear = ptHint->pptNear[i];
			if (ptHint != ptNear && DrlgRectContains(&ptNear->tCoords, nX, nY))
				return ptNear;
		}
	}
	if (!ptDrlgLevel)
		ptDrlgLevel = DrlgGetLevel(ptAct, DrlgActGetLevelAt(ptAct, nX, nY));
	if (!ptDrlgLevel->ptRoomFirst)
		sLevelGenerate(ptDrlgLevel);
	return DrlgLevelFindRoom(ptDrlgLevel, nX, nY);
}

BOOL __fastcall DrlgLevelIsTown(int nLevelId)
{
	switch (nLevelId)
	{
	case 1:
	case 40:
	case 75:
	case 103:
		return TRUE;
	}
	return FALSE;
}

BOOL __fastcall DrlgLevelIsNone(int nLevelId)
{
	return FALSE;
}

DWORD __stdcall DrlgGetLevelType(DWORD nLevelId)
{
	PTLEVELSREC ptLevel;

	ptLevel = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId);
	return ptLevel->LevelType;
}

int __fastcall DrlgGetDrlgType(int nLevelId)
{
	return ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->DrlgType;
}

void __fastcall DrlgLevelSetRect(DRLGACTCTX *ptAct, DRLGLEVEL *ptLevel)
{
	PTLEVELSREC ptRecord;
	DRLGLEVEL *ptDepend;
	int nX;
	int nY;
	int nDepend;

	ptRecord = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptLevel->nLevelId);
	nX = 0;
	nY = 0;
	nDepend = ptRecord->Depend;
	ptLevel->tCoords.nSizeTileX = ptRecord->SizeX;
	ptLevel->tCoords.nSizeTileY = ptRecord->SizeY;
	if (nDepend)
	{
		ptDepend = DrlgGetLevel(ptAct, nDepend);
		nX = ptDepend->tCoords.nTileX;
		nY = ptDepend->tCoords.nTileY;
	}
	ptLevel->tCoords.nTileX = ptRecord->OffsetX + nX;
	ptLevel->tCoords.nTileY = ptRecord->OffsetY + nY;
	for (ptDepend = ptAct->ptFirstLevel; ptDepend; ptDepend = ptDepend->ptNext)
		;
}

DWORD __fastcall DrlgActGetLevelAt(DRLGACTCTX *ptAct, int nX, int nY)
{
	DRLGLEVEL *ptLevel;

	for (ptLevel = ptAct->ptFirstLevel; ptLevel; ptLevel = ptLevel->ptNext)
	{
		if (DrlgRectContains(&ptLevel->tCoords, nX, nY))
			return ptLevel->nLevelId;
	}
	return 0;
}

BYTE __stdcall DrlgGetActNum(int nLevelId)
{
	static const BYTE scabActLevelBound[5] =
	{
		1, 40, 75, 103, 109,
	};
	BYTE a;

	for (a = 1; a <= 4; a++)
	{
		if (nLevelId < scabActLevelBound[a])
			return a - 1;
	}
	return 0;
}

DWORD __stdcall DrlgGetLevelSaveMonsters(DWORD nLevelId)
{
	PTLEVELSREC ptLevel;

	ptLevel = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId);
	return ptLevel->SaveMonsters;
}

DWORD __stdcall DrlgGetLevelLOSDraw(DWORD nLevelId)
{
	PTLEVELSREC ptLevel;

	ptLevel = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId);
	return ptLevel->LOSDraw;
}

PTVISINFO __fastcall DrlgActGetVisInfo(DRLGACTCTX *ptAct, int nLevelId)
{
	PTVISINFO ptVisInfo;
	PTLEVELSREC ptLevelDef;
	int i;

	for (ptVisInfo = ptAct->ptVisInfo; ptVisInfo; ptVisInfo = ptVisInfo->ptNext)
	{
		if (ptVisInfo->eLevelId == nLevelId)
			return ptVisInfo;
	}
	ptLevelDef = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId);
#line 951
	ptVisInfo = (PTVISINFO)D2ALLOCPOOL(ptAct->pMemPool, sizeof(VISINFO));
	ptVisInfo->eLevelId = nLevelId;
	for (i = 0; i < LEVEL_VIS_MAX; i++)
	{
		ptVisInfo->anVis[i] = ptLevelDef->Vis[i];
		ptVisInfo->anWarp[i] = ptLevelDef->Warp[i];
	}
	ptVisInfo->ptNext = ptAct->ptVisInfo;
	ptAct->ptVisInfo = ptVisInfo;
	return ptVisInfo;
}

PTVISINFO __fastcall DrlgVisInfoSetLevel(PTVISINFO ptVisInfo, int nLevelId, int nWarp, int nSlot)
{
	int i;

	for (i = 0; i < LEVEL_VIS_MAX; i++)
	{
		if (ptVisInfo->anVis[i] == nLevelId)
		{
			ptVisInfo->anWarp[i] = nWarp;
			return ptVisInfo;
		}
	}
	if (nSlot == -1)
	{
		for (i = 0; i < LEVEL_VIS_MAX && nSlot == -1; i++)
		{
			if (!ptVisInfo->anVis[i] && ptVisInfo->anWarp[i] == -1)
				nSlot = i;
		}
	}
	ptVisInfo->anVis[nSlot] = nLevelId;
	ptVisInfo->anWarp[nSlot] = nWarp;
	return ptVisInfo;
}

void __cdecl DrlgAssertReport(void)
{
	OutputDebugStringA("\nThis was a DRLG DrlgAssert!\n");
	FogErrorManagerLog("\nThis was a DRLG DrlgAssert!\n");
}

BYTE __stdcall DrlgGetLevelTypeAct(int nType)
{
	return ((PTLVLTYPESTXT)D2CommonLvlTblsGetLevelTypeDef(nType))->Act;
}

void __fastcall DrlgCountAnimTiles(ROOM *ptRoom, DRLGGRID *ptGrid, DRLGGRID *ptOrient, int nOrient,
	BOOL fRight, BOOL fBottom)
{
	void *phTiles[40];
	PTDRLGTILEARRAYS ptArrays;
	DWORD dwCell;
	DWORD dwCount;
	int nWidth;
	int nHeight;
	int nRow;
	int nCol;
	int nTileOrient;
	BYTE bCode;
	BYTE bSub;

	nWidth = ptRoom->tCoords.nSizeTileX + (fRight == 0);
	nHeight = ptRoom->tCoords.nSizeTileY + (fBottom == 0);
	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	for (nRow = 0; nRow < nHeight; nRow++)
	{
		for (nCol = 0; nCol < nWidth; nCol++)
		{
			dwCell = (DWORD)DrlgGridGet(ptGrid, nCol, nRow);
			if (!(dwCell & 134217731))
				continue;
			nTileOrient = ptOrient ? DrlgGridGet(ptOrient, nCol, nRow) : nOrient;
			if (dwCell)
			{
				bCode = (BYTE)((dwCell >> 20) & 63);
				bSub = (BYTE)(dwCell >> 8);
			}
			else
			{
				bCode = 0;
				bSub = 0;
			}
			dwCount = D2CMPTileLibraryFindTiles(ptRoom->apTileProjects, nTileOrient, bCode, bSub, phTiles,
				40);
			if (!dwCount || !(D2CMPTileGetFlags(phTiles[0]) & 256))
				continue;
			if (dwCell & 2)
				ptArrays->nNumFloors += dwCount - 1;
			else if (dwCell & 1)
				ptArrays->nNumWalls += dwCount - 1;
			else
				ptArrays->nNumShadows += dwCount - 1;
			ptRoom->dwFlags |= 134217728;
		}
	}
}
