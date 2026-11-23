#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\RoomTile.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "RoomTileLocal.h"

static const int sgcnPresetOldCells[25] =
{
	0, 1, 2, 1, 2, 3, 3, 5, 5, 6, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
};

static const int sgcnPresetOldCellsB[16] =
{
	0, 1, 2, 5, 5, 7, 9, 11, 13, 14, 15, 16, 17, 18, 19, 20,
};

int __fastcall PresetConvertOldCellB(int nValue)
{
	return sgcnPresetOldCellsB[nValue];
}

int __fastcall PresetConvertOldCell(int nValue)
{
	return sgcnPresetOldCells[nValue];
}

static const int sgctDoorLevels[33][3] =
{
	{ 28, 0, 3 }, { 29, 0, 3 }, { 30, 0, 3 }, { 31, 0, 3 }, { 26, 4, 6 }, { 27, 4, 6 }, { 32, 5, 9 },
	{ 33, 5, 9 }, { 34, 10, 11 }, { 35, 10, 11 }, { 36, 10, 11 }, { 37, 10, 12 }, { 51, 13, 14 },
	{ 52, 15, 18 }, { 53, 15, 18 }, { 54, 15, 18 }, { 55, 19, 20 }, { 56, 19, 20 }, { 57, 19, 20 },
	{ 58, 19, 20 }, { 59, 19, 20 }, { 60, 19, 20 }, { 61, 19, 20 }, { 66, 19, 20 }, { 67, 19, 20 },
	{ 68, 19, 20 }, { 69, 19, 20 }, { 70, 19, 20 }, { 71, 19, 20 }, { 72, 19, 20 }, { 62, 21, 22 },
	{ 63, 21, 22 }, { 64, 21, 22 },
};
struct ROOMTILEDOOR
{
 	int			nCode;
 	int			nSub;
 	int			nOrient9;
 	int			nUnitId;
 	int			nUnitType;
 	int			nOffsetX;
 	int			nOffsetY;
};
static const ROOMTILEDOOR sgctDoors[23] =
{
	{ 7, 0, 1, 14, 2, 5, 0 }, { 7, 0, 0, 13, 2, 0, 5 }, { 5, 0, 1, 16, 2, 0, 0 }, { 5, 0, 0, 15, 2, 0, 0 },
	{ 6, 0, 1, 27, 2, 5, -2 }, { 4, 0, 1, 24, 2, 1, 2 }, { 4, 0, 0, 23, 2, 0, 0 }, { 4, 3, 1, 25, 2, 1, 0 },
	{ 1, 2, 0, 62, 2, 0, 3 }, { 1, 2, 1, 63, 2, 3, 0 }, { 0, 0, 1, 16, 2, 0, 0 }, { 0, 0, 0, 64, 2, 0, 0 },
	{ 2, 0, 1, 47, 2, 5, 0 }, { 0, 1, 1, 291, 2, 2, 0 }, { 0, 1, 0, 290, 2, 0, 2 }, { 5, 0, 1, 293, 2, 2, 0 },
	{ 4, 0, 0, 292, 2, 0, 2 }, { 0, 0, 1, 295, 2, 2, 0 }, { 0, 0, 0, 294, 2, 0, 2 }, { 2, 4, 1, 92, 2, 1, 0 },
	{ 2, 1, 0, 91, 2, 0, 2 }, { 0, 1, 1, 229, 2, 0, 0 }, { 0, 1, 0, 230, 2, 0, 0 },
};
static const int sgcanWarpFloor[4][2] =
{
	{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 },
};

static const int sgcnTileReplaceRow[18] =
{
	-1, 0, 1, 2, -1, 3, 4, 5, -2, -2, -1, -1, -1, -2, -1, -1, -1, -1,
};

static const int sgcnTileReplace[43] =
{
	-1, 1, 3, 3, 4, 1, 3,
	1, 1, 2, 3, 4, 3, 2,
	2, 3, 3, 3, 4, 3, 3,
	3, 1, 3, 3, 4, 5, 6,
	1, 3, 2, 3, 4, 3, 6,
	2, 1, 2, 3, 4, 1, 2,
	7,
};

static void __fastcall sSetTileFlags(ROOM *ptRoom, PTDRLGTILE ptTile, DWORD dwCell, int nOrient, int nX,
	int nY);
static void __fastcall sAddDoorUnit(ROOM *ptRoom, PTDRLGTILE ptTile, DWORD dwCell, int nX, int nY,
	int nOrient);
static void __fastcall sAddWarpTiles(ROOM *ptRoom, PTDRLGTILE ptTile, DWORD dwCell,
	int nOrient);
static void __fastcall sAddHiddenWarpTiles(ROOM *ptRoom, int nX, int nY, DWORD dwCell);
static PTDRLGTILE __fastcall sFindNearTile(ROOM *ptRoom, BOOL fFloor, DWORD dwCell, int nX, int nY,
	ROOM **pptOther);
static PTDRLGTILE __fastcall sAddTileToGroup(void *pPool, ROOM *ptRoom, int nOrient, DWORD dwCell, int nX,
	int nY);
static void __fastcall sReplaceTile(void *pPool, ROOM *ptRoom, ROOM *ptOther, PTDRLGTILE ptTile, int nOrient,
	DWORD dwCell, int nX, int nY);
static void __fastcall sAddOrReplaceTile(void *pPool, ROOM *ptRoom, int nOrient, DWORD dwCell, int nX,
	int nY);

void __fastcall RoomTileToScreen(int *pnX, int *pnY)
{
	(*pnY)++;
	DungeonCalcTileToScreen(pnX, pnY);
	*pnY += 40;
}

void *__fastcall RoomTilePick(ROOM *ptRoom, DWORD dwOrient, DWORD dwCell)
{
	void *phTileArray[40];
	DWORD nSize;
	DWORD ii;
	DWORD dwTotal;
	int nLeft;
	BYTE bCode;
	BYTE bSub;

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
	nSize = D2CMPTileLibraryFindTiles(ptRoom->apTileProjects, dwOrient, bCode, bSub, phTileArray, 40);
	if (!nSize)
	{
		nSize = D2CMPTileLibraryFindTiles(ptRoom->apTileProjects, 10, 0, 0, phTileArray, 40);
#line 113
		D2WARN(nSize);
		return phTileArray[0];
	}
	dwTotal = 0;
	for (ii = 0; ii < nSize; ii++)
	{
#line 122
		D2WARN(phTileArray[ii]);
		dwTotal += D2CMPTileGetRarity(phTileArray[ii]);
	}
	nLeft = (int)(dwTotal ? SeedRand(&ptRoom->tSeed, dwTotal) : 0) + 1;
	ii = 0;
	if (dwTotal)
	{
		while (1)
		{
			if (nSize <= 1 || nLeft <= 0)
				break;
			nLeft -= (int)D2CMPTileGetRarity(phTileArray[ii]);
			ii++;
		}
		if (ii)
			ii--;
	}
	return phTileArray[ii];
}

PTDRLGTILE __fastcall RoomTileAddWall(ROOM *ptRoom, PTDRLGTILE *ppChain, int nX, int nY, DWORD dwCell,
	void *hTile, int nOrient)
{
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILE ptTile;
	PTDRLGTILE ptRoof;
	int nWorldX;

	ptTiles = ptRoom->ptDrlgTiles;
	ptTile = &ptTiles->tTileArrays.ptWalls[ptTiles->nWallCount];
	if (ppChain)
	{
		ptTile->ptNext = *ppChain;
		*ppChain = ptTile;
	}
	else
		ptTile->ptNext = NULL;
	ptTiles->nWallCount++;
	ptTile->nX = nX - ptRoom->tCoords.nTileX;
	ptTile->nY = nY - ptRoom->tCoords.nTileY;
	nWorldX = nX;
	{
		int nWorldY;

		nWorldY = nY;
		RoomTileToScreen(&nWorldX, &nWorldY);
		ptTile->nScreenX = nWorldX;
		ptTile->nScreenY = nWorldY;
	}
	ptTile->nOrientation = nOrient;
	ptTile->dwFlags = 0;
	ptTile->dwFadeFlags = 0;
	ptTile->pTile = hTile;
	ptTile->bFadeFromAlpha = 255;
	ptTile->bFadeToAlpha = 255;
	ptTile->bAlpha = 255;
	sSetTileFlags(ptRoom, ptTile, dwCell, nOrient, nX, nY);
	if (nOrient == 3)
	{
		hTile = RoomTilePick(ptRoom, 4, dwCell);
		ptRoof = RoomTileAddWall(ptRoom, ppChain, nX, nY, dwCell, hTile, 4);
		sSetTileFlags(ptRoom, ptRoof, dwCell, 4, nX, nY);
	}
	return ptTile;
}

static void __fastcall sSetTileFlags(ROOM *ptRoom, PTDRLGTILE ptTile, DWORD dwCell, int nOrient, int nX,
	int nY)
{
	DWORD dwTileFlags;

	if (nOrient == 9 || nOrient == 8)
		sAddDoorUnit(ptRoom, ptTile, dwCell, nX, nY, 0);
	if (nOrient != 13)
		ptTile->dwFlags |= (((dwCell >> 18) & 3) + 1) * 16384;
	if (nOrient == 14)
		ptTile->dwFlags |= 4;
	else if (nOrient == 11 || nOrient == 10 || nOrient == 9 || nOrient == 8)
		ptTile->dwFlags |= 2;
	if (dwCell & 128)
		ptTile->dwFlags |= 1;
	if (dwCell & 268435456)
		ptTile->dwFlags |= 258;
	if (dwCell & 131072)
		ptTile->dwFlags |= 64;
	if (dwCell & 65536)
		ptTile->dwFlags |= 128;
	if (dwCell & 8)
		ptTile->dwFlags |= 4;
	if (dwCell & 0x80000000)
		ptTile->dwFlags |= 8;
	else
		ptTile->dwFlags &= ~8;
	if (dwCell & 67108864)
		ptTile->dwFlags |= 524;
	if (dwCell & 536870912)
		ptTile->dwFlags |= 2048;
	if (dwCell & 4)
		ptTile->dwFlags |= 8192;
	dwTileFlags = D2CMPTileGetFlags(ptTile->pTile);
	if (dwTileFlags & 1)
		ptTile->dwFlags |= 4;
	if (dwTileFlags & 4)
		ptTile->dwFlags |= 2048;
}

static int __fastcall sFindDoorRow(int nLevelId, DWORD dwCell, BOOL fOrient9)
{
	int nLevel;
	int nLast;
	int nIndex;
	int nCode;
	int nSub;

	for (nLevel = 0; nLevel < 33; nLevel++)
	{
		if (sgctDoorLevels[nLevel][0] != nLevelId)
			continue;
		nLast = sgctDoorLevels[nLevel][2];
		nIndex = sgctDoorLevels[nLevel][1];
		nCode = (dwCell >> 20) & 63;
		nSub = (dwCell >> 8) & 255;
		for (; nIndex <= nLast; nIndex++)
		{
			if (nCode == sgctDoors[nIndex].nCode && nSub == sgctDoors[nIndex].nSub &&
				fOrient9 == sgctDoors[nIndex].nOrient9)
				return nIndex;
		}
	}
	return -1;
}

static void __fastcall sAddDoorUnit(ROOM *ptRoom, PTDRLGTILE ptTile, DWORD dwCell, int nX, int nY,
	int nOrient)
{
	int nIndex;
	BOOL fOrient9;
	int nUnitX;
	int nUnitY;
	int nUnitType;
	int nUnitId;
	int nMode;
	BOOL fAdd;

	if (ptTile)
	{
		if (ptTile->dwFlags & 32)
			return;
		fOrient9 = ptTile->nOrientation == 9;
	}
	else
		fOrient9 = nOrient == 9;
	nIndex = sFindDoorRow((int)ptRoom->ptLevel->nLevelId, dwCell, fOrient9);
	if (nIndex == -1)
		return;
	nUnitX = nX - ptRoom->tCoords.nTileX;
	nUnitY = nY - ptRoom->tCoords.nTileY;
	EnvTileToSubtile(&nUnitX, &nUnitY);
	nUnitX += sgctDoors[nIndex].nOffsetX;
	nUnitY += sgctDoors[nIndex].nOffsetY;
	if (nUnitX < 0 || nUnitY < 0 || nUnitX >= ptRoom->tCoords.nSizeTileX * 5 ||
		nUnitY >= ptRoom->tCoords.nSizeTileY * 5)
		return;
	nUnitId = sgctDoors[nIndex].nUnitId;
	nUnitType = sgctDoors[nIndex].nUnitType;
	nMode = 0;
	fAdd = TRUE;
	switch (nUnitType)
	{
	case 1:
		nMode = 1;
		break;
	case 2:
		nMode = 0;
		if (nUnitId >= 91 && nUnitId <= 92)
			fAdd = SeedRand(&ptRoom->tSeed, 3);
		break;
	}
	if (fAdd)
		DrlgRoomNewPresetUnit(ptRoom, ptRoom->ptLevel->ptAct->pMemPool, nUnitType, nUnitId, nMode, nUnitX,
			nUnitY);
	if (ptTile)
		ptTile->dwFlags |= 32;
}

PTDRLGTILE __fastcall RoomTileAddFloor(ROOM *ptRoom, PTDRLGTILE *ppChain, int nX, int nY, DWORD dwCell,
	void *hTile)
{
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILE ptTile;
	int nWorldX;

	ptTiles = ptRoom->ptDrlgTiles;
	ptTile = &ptTiles->tTileArrays.ptFloors[ptTiles->nFloorCount];
	if (ppChain)
	{
		ptTile->ptNext = *ppChain;
		*ppChain = ptTile;
	}
	else
		ptTile->ptNext = NULL;
	ptTiles->nFloorCount++;
	ptTile->nX = nX - ptRoom->tCoords.nTileX;
	ptTile->nY = nY - ptRoom->tCoords.nTileY;
	nWorldX = nX;
	{
		int nWorldY;

		nWorldY = nY;
		RoomTileToScreen(&nWorldX, &nWorldY);
		ptTile->nScreenX = nWorldX;
		ptTile->nScreenY = nWorldY;
	}
	ptTile->nOrientation = 0;
	ptTile->dwFlags = 0;
	ptTile->dwFadeFlags = 0;
	ptTile->pTile = hTile;
	ptTile->bFadeFromAlpha = 255;
	ptTile->bFadeToAlpha = 255;
	ptTile->bAlpha = 255;
	sSetTileFlags(ptRoom, ptTile, dwCell, 0, nX, nY);
	return ptTile;
}

PTDRLGTILE __fastcall RoomTileAddShadow(ROOM *ptRoom, PTDRLGTILE *ppChain, int nX, int nY, DWORD dwCell,
	void *hTile)
{
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILE ptTile;
	int nWorldX;

	ptTiles = ptRoom->ptDrlgTiles;
	ptTile = &ptTiles->tTileArrays.ptShadows[ptTiles->dwShadowCount];
	if (ppChain)
	{
		ptTile->ptNext = *ppChain;
		*ppChain = ptTile;
	}
	else
		ptTile->ptNext = NULL;
	ptTiles->dwShadowCount++;
	ptTile->nX = nX - ptRoom->tCoords.nTileX;
	ptTile->nY = nY - ptRoom->tCoords.nTileY;
	nWorldX = nX;
	{
		int nWorldY;

		nWorldY = nY;
		RoomTileToScreen(&nWorldX, &nWorldY);
		ptTile->nOrientation = 13;
		ptTile->nScreenX = nWorldX;
		ptTile->nScreenY = nWorldY;
	}
	ptTile->dwFlags = 0;
	ptTile->dwFadeFlags = 0;
	ptTile->pTile = hTile;
	ptTile->bFadeFromAlpha = 255;
	ptTile->bFadeToAlpha = 255;
	ptTile->bAlpha = 255;
	sSetTileFlags(ptRoom, ptTile, dwCell, 13, nX, nY);
	return ptTile;
}

PTDRLGTILE __fastcall RoomTileAddShadowAt(ROOM *ptRoom, int nX, int nY, DWORD dwCell)
{
	return RoomTileAddShadow(ptRoom, NULL, nX, nY, dwCell, RoomTilePick(ptRoom, 13, dwCell));
}

void __fastcall RoomTileTrimShadows(ROOM *ptRoom)
{
	PTDRLGTILEBLOCK ptBlock;
	void *pPool;
	DRLGTILEARRAYS *ptArrays;
	int i;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	ptBlock = ptRoom->ptDrlgTiles;
	ptArrays = &ptBlock->tTileArrays;
	if (!ptBlock->dwShadowCount)
	{
		if (ptArrays->ptShadows)
		{
#line 574
			D2FREEPOOL(pPool, ptArrays->ptShadows);
		}
		ptArrays->ptShadows = NULL;
		return;
	}
	if (ptArrays->nNumShadows <= (int)ptBlock->dwShadowCount)
		return;
#line 582
	ptArrays->ptShadows = (PTDRLGTILE)D2REALLOCPOOL(pPool, ptArrays->ptShadows, ptBlock->dwShadowCount * sizeof(DRLGTILE));
	for (i = 0; i < (int)ptBlock->dwShadowCount - 1; i++)
		ptArrays->ptShadows[i].ptNext = &ptArrays->ptShadows[i + 1];
	ptArrays->ptShadows[i].ptNext = NULL;
	ptArrays->nNumShadows = (int)ptBlock->dwShadowCount;
}

void __fastcall RoomTileFill(ROOM *ptRoom, DRLGGRID *ptWalls, DRLGGRID *ptOrient, BOOL fFill, BOOL fRight,
	BOOL fBottom)
{
	PTLVLWARPTXT ptWarpDef;
	PTDRLGTILE ptTile;
	int nUnitX;
	int nUnitY;
	void *pTileDef;
	void *pPool;
	DWORD dwCell;
	DWORD dwHidden;
	int nWidth;
	int nHeight;
	int nRow;
	int nCol;
	int nX;
	int nY;
	int nOrient;
	int nTileX;
	int nTileY;
	BYTE bCode;
	BYTE bSub;
	BYTE bExit;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	nWidth = ptRoom->tCoords.nSizeTileX + (fRight == 0);
	nTileX = ptRoom->tCoords.nTileX;
	nHeight = ptRoom->tCoords.nSizeTileY + (fBottom == 0);
	nTileY = ptRoom->tCoords.nTileY;
	for (nRow = 0; nRow < nHeight; nRow++)
	{
		for (nCol = 0; nCol < nWidth; nCol++)
		{
			nY = nRow + nTileY;
			nX = nCol + nTileX;
			dwCell = (DWORD)DrlgGridGet(ptWalls, nCol, nRow);
			nOrient = ptOrient ? DrlgGridGet(ptOrient, nCol, nRow) : 0;
			bCode = (BYTE)((dwCell >> 20) & 63);
			bSub = (BYTE)(dwCell >> 8);
			if ((nOrient == 11 || nOrient == 10) && bCode >= 8)
				continue;
			if (!nOrient && bCode == 30 && (bSub == 0 || bSub == 1))
				dwCell |= 0x80000000;
			dwHidden = dwCell & 0x80000000;
			if (dwHidden)
			{
				switch (nOrient)
				{
				case 10:
				case 11:
					if (bCode >= 8)
						continue;
					bExit = (BYTE)((dwCell >> 20) & 63);
					ptWarpDef = DrlgWarpGetWarpDef(ptRoom->ptLevel, bExit);
					if (ptWarpDef)
					{
						nUnitX = nX - ptRoom->tCoords.nTileX;
						nUnitY = nY - ptRoom->tCoords.nTileY;
						if (nUnitX != ptRoom->tCoords.nSizeTileX && nUnitY != ptRoom->tCoords.nSizeTileY)
						{
							EnvTileToSubtile(&nUnitX, &nUnitY);
							nUnitX += ptWarpDef->OffsetX;
							nUnitY += ptWarpDef->OffsetY;
							DrlgRoomNewPresetUnit(ptRoom, ptRoom->ptLevel->ptAct->pMemPool, 5, bExit, 0, nUnitX, nUnitY);
						}
					}
					sAddHiddenWarpTiles(ptRoom, nX, nY, dwCell);
					if (0)
						sAddHiddenWarpTiles(ptRoom, nX, nY, dwCell);
					continue;
				case 8:
				case 9:
					sAddDoorUnit(ptRoom, NULL, dwCell, nX, nY, nOrient);
					continue;
				}
			}
			if (dwCell & 4)
			{
				if (dwCell & 2)
				{
					ROOM *ptOther;

					if (bCode == 30 && (bSub == 0 || bSub == 1))
						dwCell &= ~128;
					ptOther = NULL;
					ptTile = sFindNearTile(ptRoom, TRUE, dwCell, nX, nY, &ptOther);
					if (ptTile)
						sReplaceTile(pPool, ptRoom, ptOther, ptTile, 0, dwCell, nX, nY);
					else
						sAddTileToGroup(pPool, ptRoom, 0, dwCell, nX, nY);
					continue;
				}
				if (dwCell & 1)
				{
					sAddOrReplaceTile(pPool, ptRoom, nOrient, dwCell, nX, nY);
					continue;
				}
				if ((dwCell & 134217728) && !dwHidden)
				{
					sAddOrReplaceTile(pPool, ptRoom, 13, dwCell, nX, nY);
					continue;
				}
			}
			if (dwCell & 2)
				RoomTileAddFloor(ptRoom, NULL, nX, nY, dwCell, RoomTilePick(ptRoom, 0, dwCell));
			else if (fFill && DrlgRectContains(&ptRoom->tCoords, nX, nY))
			{
				DWORD dwFloor;

				dwFloor = (dwCell & ~128) | 0x80000000;
				if (ptRoom->ptLevel->nLevelId != 74)
					pTileDef = RoomTilePick(ptRoom, 0, 31457280);
				else
					pTileDef = RoomTilePick(ptRoom, 0, 31457536);
				RoomTileAddFloor(ptRoom, NULL, nX, nY, dwFloor, pTileDef);
			}
			if (dwCell & 1)
			{
				pTileDef = RoomTilePick(ptRoom, nOrient, dwCell);
				ptTile = RoomTileAddWall(ptRoom, NULL, nX, nY, dwCell, pTileDef, nOrient);
				if (nOrient == 11 || nOrient == 10)
					sAddWarpTiles(ptRoom, ptTile, dwCell, nOrient);
			}
			if (dwCell & 134217728)
				RoomTileAddShadow(ptRoom, NULL, nX, nY, dwCell, RoomTilePick(ptRoom, 13, dwCell));
		}
	}
}

static BOOL sAddWarpUnit(ROOM *ptRoom, DWORD dwCell, int nX, int nY)
{
	PTLVLWARPTXT pctWarpDef;
	BYTE bCode;
	int nUnitX;
	int nUnitY;

	bCode = (BYTE)((dwCell >> 20) & 63);
	pctWarpDef = DrlgWarpGetWarpDef(ptRoom->ptLevel, bCode);
	if (!pctWarpDef)
		return FALSE;
	nUnitX = nX - ptRoom->tCoords.nTileX;
	nUnitY = nY - ptRoom->tCoords.nTileY;
	if (nUnitX == ptRoom->tCoords.nSizeTileX || nUnitY == ptRoom->tCoords.nSizeTileY)
		return FALSE;
	EnvTileToSubtile(&nUnitX, &nUnitY);
	nUnitX += pctWarpDef->OffsetX;
	nUnitY += pctWarpDef->OffsetY;
	DrlgRoomNewPresetUnit(ptRoom, ptRoom->ptLevel->ptAct->pMemPool, 5, bCode, 0, nUnitX, nUnitY);
	return TRUE;
}

static void __fastcall sAddWarpTiles(ROOM *ptRoom, PTDRLGTILE ptTile, DWORD dwCell, int nOrient)
{
	PTDRLGROOMWARP ptWarp;
	PTLVLWARPTXT pctWarpDef;
	PTDRLGTILE ptCopy;
	DRLGLEVEL *ptLevel;
	DRLGCOORDS *ptCoords;
	void *hTile;
	BYTE bCode;
	BYTE bSub;

	bCode = (BYTE)((dwCell >> 20) & 63);
	bSub = (BYTE)(dwCell >> 8);
	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		if (ptWarp->bExit == bCode)
			break;
	}
	if (!ptWarp)
	{
#line 640
		D2WARN(ptWarp);
		return;
	}
	ptCoords = &ptRoom->tCoords;
	if ((bSub == 0 || bSub == 4) && !sAddWarpUnit(ptRoom, dwCell,
		ptCoords->nTileX + ptTile->nX, ptCoords->nTileY + ptTile->nY))
		return;
	ptTile->ptNext = ptWarp->ptRoomTiles;
	ptWarp->ptRoomTiles = ptTile;
	ptLevel = ptRoom->ptLevel;
	pctWarpDef = DrlgWarpGetWarpDef(ptLevel, bCode);
#line 663
	D2WARN(pctWarpDef);
	if (pctWarpDef->LitVersion)
	{
		dwCell |= 512;
		hTile = RoomTilePick(ptRoom, nOrient, dwCell);
		ptCopy = RoomTileAddWall(ptRoom, &ptWarp->ptLitTiles, ptCoords->nTileX + ptTile->nX,
			ptCoords->nTileY + ptTile->nY, dwCell, hTile, nOrient);
		ptCopy->dwFlags |= 8;
	}
}

static void __fastcall sAddHiddenWarpTiles(ROOM *ptRoom, int nX, int nY, DWORD dwCell)
{
	PTDRLGROOMWARP ptWarp;
	PTDRLGTILE ptTile;
	DWORD dwFloor;
	int i;
	BYTE bCode;
	BYTE bSub;
	const int *pnWarpFloor;

	nX--;
	nY--;
	ptRoom->dwFlags |= 8388608;
	bCode = (BYTE)((dwCell >> 20) & 63);
	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		if (ptWarp->bExit == bCode)
			break;
	}
	if (!ptWarp)
		return;
	if (!DrlgWarpGetWarpDef(ptRoom->ptLevel, bCode)->LitVersion)
		return;
	bSub = (BYTE)(dwCell >> 8);
	for (i = 0; i < 4; i++)
	{
		dwFloor = ((bSub << 12) | i | 4) << 8;
		pnWarpFloor = sgcanWarpFloor[i];
		ptTile = RoomTileAddFloor(ptRoom, &ptWarp->ptLitTiles, nX + pnWarpFloor[0],
			nY + pnWarpFloor[1], dwFloor, RoomTilePick(ptRoom, 0, dwFloor));
		ptTile->dwFlags |= 8;
	}
	ptTile = ptRoom->ptDrlgTiles->tTileArrays.ptFloors;
	for (i = 0; i < ptRoom->ptDrlgTiles->nFloorCount; ptTile++, i++)
	{
		if (D2CMPTileGetStyle(ptTile->pTile) == bSub &&
			D2CMPTileGetSequence(ptTile->pTile) < 4)
		{
			ptTile->ptNext = ptWarp->ptRoomTiles;
			ptWarp->ptRoomTiles = ptTile;
		}
	}
}

static __inline PTDRLGTILE __fastcall sFindTileInRoom(ROOM *ptNear, BOOL fFloor, DWORD dwCell, int nX, int nY)
{
	PTDRLGTILEGROUP ptGroup;
	PTDRLGTILE ptTile;

	for (ptGroup = ptNear->ptDrlgTiles->ptFirstGroup; ptGroup; ptGroup = ptGroup->ptNext)
	{
		ptTile = ptGroup->ptTiles;
		if ((ptGroup->nFloorGroup && !fFloor) || (!ptGroup->nFloorGroup && fFloor))
			continue;
		for (; ptTile; ptTile = ptTile->ptNext)
		{
			if (ptNear->tCoords.nTileX + ptTile->nX != nX ||
				ptNear->tCoords.nTileY + ptTile->nY != nY)
				continue;
			if (ptTile->nOrientation == 4)
				continue;
			if (ptTile->nOrientation != 13 && (dwCell & 134217728))
				continue;
			if (!(ptTile->dwFlags & 114688) ||
				(int)((ptTile->dwFlags >> 14) & 7) - 1 == (int)((dwCell >> 18) & 3))
				return ptTile;
		}
	}
	return NULL;
}

#pragma auto_inline(off)
static PTDRLGTILE __fastcall sFindNearTile(ROOM *ptRoom, BOOL fFloor, DWORD dwCell, int nX, int nY,
	ROOM **pptOther)
{
	ROOM *ptNear;
	PTDRLGTILE ptTile;
	int i;

	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear == ptRoom || !ptNear->ptDrlgTiles || !DrlgRectContainsInclusive(&ptNear->tCoords, nX, nY))
			continue;
		ptTile = sFindTileInRoom(ptNear, fFloor, dwCell, nX, nY);
		if (ptTile)
		{
			*pptOther = ptRoom->pptNear[i];
			return ptTile;
		}
	}
	*pptOther = NULL;
	return NULL;
}
#pragma auto_inline(on)

static __inline PTDRLGTILEGROUP sNewTileGroup(void *pPool, ROOM *ptRoom, BOOL fFloor)
{
	PTDRLGTILEGROUP ptGroup;

#line 833
	ptGroup = (PTDRLGTILEGROUP)D2ALLOCPOOL(pPool, sizeof(DRLGTILEGROUP));
	ptGroup->nFloorGroup = fFloor;
	ptGroup->ptTiles = NULL;
	ptGroup->ptNext = ptRoom->ptDrlgTiles->ptFirstGroup;
	ptRoom->ptDrlgTiles->ptFirstGroup = ptGroup;
	return ptGroup;
}

static PTDRLGTILE __fastcall sAddTileToGroup(void *pPool, ROOM *ptRoom, int nOrient, DWORD dwCell, int nX,
	int nY)
{
	PTDRLGTILEGROUP ptGroup;
	PTDRLGTILE ptTile;
	void *hTile;
	BOOL fFloor;

	if ((nOrient == 11 || nOrient == 10) && !DrlgRectContains(&ptRoom->tCoords, nX, nY))
		return NULL;
	ptGroup = ptRoom->ptDrlgTiles->ptFirstGroup;
	fFloor = nOrient == 0;
	for (; ptGroup; ptGroup = ptGroup->ptNext)
	{
		if ((!ptGroup->nFloorGroup || fFloor) && (ptGroup->nFloorGroup || !fFloor))
			break;
	}
	if (!ptGroup)
		ptGroup = sNewTileGroup(pPool, ptRoom, fFloor);
	hTile = RoomTilePick(ptRoom, nOrient, dwCell);
	if (!nOrient)
		ptTile = RoomTileAddFloor(ptRoom, &ptGroup->ptTiles, nX, nY, dwCell, hTile);
	else if (nOrient == 13)
		ptTile = RoomTileAddShadow(ptRoom, &ptGroup->ptTiles, nX, nY, dwCell, hTile);
	else
	{
		RoomTileAddWall(ptRoom, &ptGroup->ptTiles, nX, nY, dwCell, hTile, nOrient);
		ptTile = ptGroup->ptTiles;
		if (nOrient == 11 || nOrient == 10)
			sAddWarpTiles(ptRoom, ptTile, dwCell, nOrient);
	}
	return ptTile;
}

static void __fastcall sReplaceTile(void *pPool, ROOM *ptRoom, ROOM *ptOther, PTDRLGTILE ptTile, int nOrient,
	DWORD dwCell, int nX, int nY)
{
	void *hTile;
	DWORD dwFlags;
	BOOL fBlank;
	BOOL fChange;
	int nRow;
	int nPickOrient;

	dwFlags = ptTile->dwFlags;
	nRow = sgcnTileReplaceRow[nOrient];
	if (dwFlags & 1)
	{
		if (ptTile->nOrientation == 9 || ptTile->nOrientation == 8)
			sSetTileFlags(ptRoom, ptTile, dwCell, ptTile->nOrientation, nX, nY);
		return;
	}
	if (!(dwCell & 128))
	{
		fChange = TRUE;
		if (nOrient == 9 || nOrient == 8)
		{
			if (!(nX - ptRoom->tCoords.nTileX) || !(nY - ptRoom->tCoords.nTileY))
				fChange = FALSE;
		}
		else if (ptTile->nOrientation == 9 || ptTile->nOrientation == 8)
		{
			if (!(nX - ptOther->tCoords.nTileX) || !(nY - ptOther->tCoords.nTileY))
				return;
		}
		if (fChange)
		{
			if (nRow >= 0 && ptTile->nOrientation <= 7)
			{
				nPickOrient = sgcnTileReplace[nRow * 7 + ptTile->nOrientation];
				goto pick;
			}
			else if (nRow != -1)
				return;
		}
	}
	nPickOrient = nOrient;
pick:
	if (ptTile->nOrientation != 3)
	{
		if (nPickOrient == 3)
		{
			ptTile->dwFlags = dwFlags | 49160;
			CollisnReplaceTile(ptOther->ptGameRoom, ptTile, NULL);
			sAddTileToGroup(pPool, ptRoom, 3, dwCell, nX, nY);
		}
	}
	else if (nPickOrient != 3)
	{
		ptTile->ptNext->dwFlags |= 8;
		CollisnReplaceTile(ptOther->ptGameRoom, ptTile->ptNext, NULL);
	}
	fBlank = !ptTile->nOrientation && D2CMPTileGetStyle(ptTile->pTile) == 30 &&
		!D2CMPTileGetSequence(ptTile->pTile);
	if (nPickOrient != ptTile->nOrientation || fBlank)
	{
		hTile = RoomTilePick(ptOther, nPickOrient, dwCell);
		ptTile->nOrientation = nPickOrient;
		if (hTile != ptTile->pTile && ptOther->ptGameRoom)
			CollisnReplaceTile(ptOther->ptGameRoom, ptTile, hTile);
		ptTile->pTile = hTile;
	}
	sSetTileFlags(ptRoom, ptTile, dwCell, ptTile->nOrientation, nX, nY);
}

static void __fastcall sAddOrReplaceTile(void *pPool, ROOM *ptRoom, int nOrient, DWORD dwCell, int nX,
	int nY)
{
	ROOM *ptNear;
	ROOM *ptOther;
	PTDRLGTILE ptTile;
	BOOL fFloor;
	int i;

	fFloor = nOrient == 0;
	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear == ptRoom || !ptNear->ptDrlgTiles || !DrlgRectContainsInclusive(&ptNear->tCoords, nX, nY))
			continue;
		ptTile = sFindTileInRoom(ptNear, fFloor, dwCell, nX, nY);
		if (ptTile)
		{
			ptOther = ptRoom->pptNear[i];
			goto found;
		}
	}
	ptTile = NULL;
found:
	if (ptTile)
		sReplaceTile(pPool, ptRoom, ptOther, ptTile, nOrient, dwCell, nX, nY);
	else
		sAddTileToGroup(pPool, ptRoom, nOrient, dwCell, nX, nY);
}

void __fastcall RoomTileCountTiles(ROOM *ptRoom, DRLGGRID *ptGrid, BOOL fFill, BOOL fRight, BOOL fBottom)
{
	PTDRLGTILEARRAYS ptArrays;
	DWORD *pdwCell;
	int nWidth;
	int nHeight;
	int nRow;
	int nCol;
	int nTileX;
	int nTileY;

	nWidth = ptRoom->tCoords.nSizeTileX + (fRight == 0);
	nHeight = ptRoom->tCoords.nSizeTileY + (fBottom == 0);
	nTileX = ptRoom->tCoords.nTileX;
	nTileY = ptRoom->tCoords.nTileY;
	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	for (nRow = 0; nRow < nHeight; nRow++)
	{
		for (nCol = 0; nCol < nWidth; nCol++)
		{
			pdwCell = (DWORD *)DrlgGridGetCell(ptGrid, nCol, nRow);
			if (*pdwCell & 1)
				ptArrays->nNumWalls++;
			if ((*pdwCell & 2) || (fFill && DrlgRectContains(&ptRoom->tCoords,
				nTileX + nCol, nTileY + nRow)))
				ptArrays->nNumFloors++;
			if (*pdwCell & 134217728)
				ptArrays->nNumShadows++;
		}
	}
}

void __fastcall RoomTileCountWalls(ROOM *ptRoom, DRLGGRID *ptWalls, DRLGGRID *ptOrient, BOOL fRight,
	BOOL fBottom)
{
	PTDRLGTILEARRAYS ptArrays;
	int nWidth;
	int nHeight;
	int nRow;
	int nCol;
	int nOrient;

	nWidth = ptRoom->tCoords.nSizeTileX + (fRight == 0);
	nHeight = ptRoom->tCoords.nSizeTileY + (fBottom == 0);
	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	for (nRow = 0; nRow < nHeight; nRow++)
	{
		for (nCol = 0; nCol < nWidth; nCol++)
		{
			nOrient = DrlgGridGet(ptOrient, nCol, nRow);
			if (nOrient == 3)
				ptArrays->nNumWalls++;
			else if (nOrient > 9 && nOrient <= 11)
			{
				if ((DWORD)DrlgGridGet(ptWalls, nCol, nRow) & 0x80000000)
					ptArrays->nNumFloors += 6;
				else
					ptArrays->nNumWalls++;
			}
		}
	}
}

void __fastcall RoomTileBuildA(ROOM *ptRoom)
{
	SeedSet(&ptRoom->tSeed, ptRoom->dwInitSeed);
	switch (ptRoom->dwKind)
	{
	case 1:
		OutRoomBuildTilesA(ptRoom);
		break;
	case 2:
		PresetRoomBuildTilesA(ptRoom);
		break;
	}
}

void __fastcall RoomTileBuildB(ROOM *ptRoom)
{
	switch (ptRoom->dwKind)
	{
	case 1:
		OutRoomBuildTilesB(ptRoom);
		break;
	case 2:
		PresetRoomBuildTilesB(ptRoom);
		break;
	}
	ptRoom->dwFlags |= 1048576;
}

void __fastcall RoomTileAllocBlock(ROOM *ptRoom)
{
	PTDRLGTILEBLOCK ptTiles;
	DWORD i;

	if (ptRoom->ptDrlgTiles)
		return;
#line 1256
	ptTiles = (PTDRLGTILEBLOCK)D2ALLOCPOOL(ptRoom->ptLevel->ptAct->pMemPool, sizeof(DRLGTILEBLOCK));
	ptRoom->ptDrlgTiles = ptTiles;
	for (i = 0; i < sizeof(DRLGTILEBLOCK) / sizeof(DWORD); i++)
		((DWORD *)ptTiles)[i] = 0;
}

void __fastcall RoomTileAllocArrays(ROOM *ptRoom)
{
	PTDRLGTILEARRAYS ptArrays;
	void *pPool;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	if (ptArrays->nNumFloors)
	{
#line 1274
		ptArrays->ptFloors = (PTDRLGTILE)D2ALLOCPOOL(pPool, ptArrays->nNumFloors * sizeof(DRLGTILE));
		memset(ptArrays->ptFloors, 0, ptArrays->nNumFloors * sizeof(DRLGTILE));
	}
	if (ptArrays->nNumWalls)
	{
#line 1280
		ptArrays->ptWalls = (PTDRLGTILE)D2ALLOCPOOL(pPool, ptArrays->nNumWalls * sizeof(DRLGTILE));
		memset(ptArrays->ptWalls, 0, ptArrays->nNumWalls * sizeof(DRLGTILE));
	}
	if (ptArrays->nNumShadows && !ptArrays->ptShadows)
	{
#line 1286
		ptArrays->ptShadows = (PTDRLGTILE)D2ALLOCPOOL(pPool, ptArrays->nNumShadows * sizeof(DRLGTILE));
		memset(ptArrays->ptShadows, 0, ptArrays->nNumShadows * sizeof(DRLGTILE));
	}
}

void __fastcall RoomTileGrowShadows(void *pPool, PTDRLGTILEBLOCK ptBlock, int nExtra)
{
	DRLGTILEARRAYS *ptArrays;
	int i;

	if (!nExtra)
		return;
	ptArrays = &ptBlock->tTileArrays;
#line 1300
	ptArrays->ptShadows = (PTDRLGTILE)D2REALLOCPOOL(pPool, ptArrays->ptShadows,
		(ptArrays->nNumShadows + nExtra) * sizeof(DRLGTILE));
	for (i = 0; i < (int)ptBlock->dwShadowCount - 1; i++)
		ptArrays->ptShadows[i].ptNext = &ptArrays->ptShadows[i + 1];
	ptArrays->ptShadows[i].ptNext = NULL;
	ptArrays->nNumShadows += nExtra;
}

DWORD __fastcall RoomTileGetThirdArrayCount(PTDUNGEONROOM ptRoom)
{
	struct ROOM *ptDrlgRoom;

	if (ptRoom == NULL)
		return 0;
	ptDrlgRoom = (struct ROOM *)D2CommonDungeonRoomGetDrlgRoom(ptRoom);
	return ptDrlgRoom->ptDrlgTiles->dwShadowCount;
}

static __inline void sFreeTileArrays(void *pPool, PTDRLGTILEARRAYS ptArrays)
{
	if (!ptArrays)
		return;
	if (ptArrays->ptWalls)
	{
#line 1339
		D2FREEPOOL(pPool, ptArrays->ptWalls);
	}
	if (ptArrays->ptFloors)
	{
#line 1340
		D2FREEPOOL(pPool, ptArrays->ptFloors);
	}
	if (ptArrays->ptShadows)
	{
#line 1341
		D2FREEPOOL(pPool, ptArrays->ptShadows);
	}
}

void __fastcall RoomTileFree(ROOM *ptRoom)
{
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILEGROUP ptGroup;
	PTDRLGTILEGROUP ptNextGroup;
	PTDRLGTILEANIM ptAnim;
	PTDRLGTILEANIM ptNextAnim;
	void *pPool;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	if (ptRoom->ptDrlgTiles)
		sFreeTileArrays(pPool, &ptRoom->ptDrlgTiles->tTileArrays);
	ptTiles = ptRoom->ptDrlgTiles;
	if (ptTiles)
	{
		ptNextGroup = ptTiles->ptFirstGroup;
		while (ptNextGroup)
		{
			ptGroup = ptNextGroup;
			ptNextGroup = ptGroup->ptNext;
#line 1366
			D2FREEPOOL(pPool, ptGroup);
		}
		ptTiles->ptFirstGroup = NULL;
		ptNextAnim = ptTiles->ptFirstAnim;
		while (ptNextAnim)
		{
			ptAnim = ptNextAnim;
			ptNextAnim = ptAnim->ptNext;
#line 1378
			D2FREEPOOL(pPool, ptAnim->pptFrames);
#line 1379
			D2FREEPOOL(pPool, ptAnim);
		}
#line 1382
		D2FREEPOOL(pPool, ptTiles);
		ptRoom->ptDrlgTiles = NULL;
	}
}

void __fastcall RoomTileUnload(ROOM *ptRoom, BOOL fKeep)
{
	DRLGACTCTX *ptAct;
	PTDRLGROOMWARP ptWarp;

	ptAct = ptRoom->ptLevel->ptAct;
	if (!fKeep && ptRoom->ptGameRoom)
		D2CommonDungeonActRemoveRoom(ptAct->ptOwnerAct, ptRoom->ptGameRoom);
	ptRoom->ptGameRoom = NULL;
	DrlgLogicFree(ptRoom);
	if (!(ptRoom->dwFlags & 1048576))
		return;
	ptRoom->dwFlags ^= 1048576;
	ptAct->dwNumRoomsUnloaded++;
	RoomTileFree(ptRoom);
	switch (ptRoom->dwKind)
	{
	case 1:
		OutRoomFreeGrids(ptRoom);
		break;
	case 2:
		PresetRoomFreeGrids(ptRoom);
		break;
	}
	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		ptWarp->ptLitTiles = NULL;
		ptWarp->ptRoomTiles = NULL;
	}
	if (fKeep)
		ptRoom->dwFlags |= 2097152;
}

void __fastcall RoomTileInit(ROOM *ptRoom)
{
	char (*pszFiles)[60];
	DWORD dwMask;
	DWORD i;

	pszFiles = (char (*)[60])D2CommonLvlTblsGetLevelTypeDef(ptRoom->ptLevel->dwLevelType);
	dwMask = ptRoom->dwDt1Mask;
	for (i = 0; dwMask && i < 32; i++)
	{
		if (dwMask & 1)
			D2CMPTileLibraryAddProject(ptRoom->apTileProjects, pszFiles[i]);
		dwMask >>= 1;
	}
	char szPath[260] = "";
	wsprintfA(szPath, "%s\\Tiles\\Act1\\Outdoors\\Blank.dt1", "DATA\\GLOBAL");
	D2CMPTileLibraryAddProject(ptRoom->apTileProjects, szPath);
	wsprintfA(szPath, "%s\\Tiles\\Act1\\Barracks\\InvisWal.dt1", "DATA\\GLOBAL");
	D2CMPTileLibraryAddProject(ptRoom->apTileProjects, szPath);
	wsprintfA(szPath, "%s\\Tiles\\Act1\\Barracks\\Warp.dt1", "DATA\\GLOBAL");
	D2CMPTileLibraryAddProject(ptRoom->apTileProjects, szPath);
	ptRoom->dwFlags |= 16777216;
}
