#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\TileSub.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "Drlg.h"

static __inline int sDrawBelow(D2SEED *ptSeed, int nRange)
{
	return nRange ? (int)SeedRand(ptSeed, nRange) : 0;
}

static BOOL __fastcall sLvlSubTestClusterFits(int nX, int nY, PTTILESUBCTX ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub);
static void __fastcall sLvlSubPlaceCluster(int nX, int nY, PTTILESUBCTX ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub, int nOffset);

static __inline int sLvlSubGetLayerCell(int nLayers, DRLGGRID *ptGrid, int nX, int nY)
{
	if (nLayers)
		return DrlgGridGet(ptGrid, nX, nY);
	return 0;
}

void __fastcall TileSubRun(PTTILESUBCTX ptCtx)
{
	PTLVLSUBTXT ptTileSub;
	PTPRESETDS1 ptFile;
	PTPRESETCLUSTER ptCluster;
	DRLGLEVEL *ptLevel;
	DRLGCOORDS *ptRect;
	struct HD2ARCHIVE__ *hArchive;
	struct { int x; int y; } anCells[256], tSwap;
	int nClusters;
	int nStart;
	int nAdjust;
	int nWidth;
	int nHeight;
	int nCells;
	int nA;
	int nB;
	int nAlternate;
	BOOL fBorder;
	BOOL fWild;
	BOOL fSkipCentre;
	DWORD dwFlags;
	int i;
	int j;
	int nX;
	int nY;
	BOOL fPlaced;

	ptTileSub = LvlTblsGetTileSubs(ptCtx->nType);
	hArchive = ptCtx->ptLevel->ptAct->hArchive;
	for (; ptTileSub->Type == ptCtx->nType; ptTileSub++)
	{
		TileSubLoadFile(hArchive, ptTileSub);
		ptFile = ptTileSub->ptFile;
		ptLevel = ptCtx->ptLevel;
		nStart = 0;
		nClusters = ptFile->nClusters;
		if (!nClusters)
			continue;
		if (ptCtx->nWildcardIndex == -1)
			ptCtx->nWildcardIndex = 62;
		if (!ptTileSub->BordType)
			nStart = sDrawBelow(&ptLevel->tSeed, nClusters);
		for (i = 0; i < nClusters; i++)
		{
			ptCluster = &ptFile->ptClusters[(i + nStart) % nClusters];
			ptRect = ptCtx->ptCellRect;
			ptLevel = ptCtx->ptLevel;
			fBorder = ptCtx->nType == 1;
			dwFlags = ptLevel->field_38.pOutdoor->dwFlags & 12;
			fWild = (int)ptLevel->nLevelId >= 2 && (int)ptLevel->nLevelId <= 7;
			if (fBorder && dwFlags)
				nAdjust = -1;
			else
				nAdjust = 1;
			nWidth = ptRect->nSizeTileX - ptCluster->nWidth * ptTileSub->GridSize + nAdjust;
			nHeight = ptRect->nSizeTileY - ptCluster->nHeight * ptTileSub->GridSize + 1;
			nCells = nHeight * nWidth;
			if (!nCells)
				continue;
			fSkipCentre = fBorder && fWild && nWidth < 6 && nHeight < 6;
			fPlaced = FALSE;
			for (j = 0; j < nCells; j++)
			{
				anCells[j].x = j % nWidth;
				anCells[j].y = j / nWidth;
			}
			for (j = 0; j < nCells; j++)
			{
				nA = sDrawBelow(&ptLevel->tSeed, nCells);
				nB = sDrawBelow(&ptLevel->tSeed, nCells);
				tSwap = anCells[nA];
				anCells[nA] = anCells[nB];
				anCells[nB] = tSwap;
			}
			for (j = 0; j < nCells; j++)
			{
				nX = anCells[j].x;
				nY = anCells[j].y;
				if (fSkipCentre && nX == 2 && nY == 2)
					continue;
				if (0)
				{
					sLvlSubTestClusterFits(nX, nY, ptCtx, ptCluster, ptTileSub);
					sLvlSubPlaceCluster(nX, nY, ptCtx, ptCluster, ptTileSub, 0);
				}
				if (!sLvlSubTestClusterFits(nX, nY, ptCtx, ptCluster, ptTileSub))
					continue;
				nAlternate = sDrawBelow(&ptLevel->tSeed, ptCluster->nAlternates);
				sLvlSubPlaceCluster(nX, nY, ptCtx, ptCluster, ptTileSub,
					(ptCluster->nWidth + 1) * (nAlternate + 1));
				if (ptTileSub->BordType == 0 || ptTileSub->BordType == 1)
				{
					fPlaced = TRUE;
					break;
				}
			}
			if (fPlaced && ptTileSub->BordType == 0)
				break;
		}
	}
}

static BOOL __fastcall sLvlSubTestClusterFits(int nX, int nY, PTTILESUBCTX ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub)
{
	int nMask;
	int nCell;
	int nDest;
	int nIndex;
	int x;
	int y;
	int i;
	int j;

	nX -= nX % ptTileSub->GridSize;
	nY -= nY % ptTileSub->GridSize;
	for (j = 0; j < ptCluster->nHeight; j++)
	{
		for (i = 0; i < ptCluster->nWidth; i++)
		{
			nMask = sLvlSubGetLayerCell(ptTileSub->ptFile->nFloorLayers, &ptTileSub->tFloor,
				ptCluster->nX + i, ptCluster->nY + j);
			nCell = sLvlSubGetLayerCell(ptTileSub->ptFile->nWallLayers, &ptTileSub->atWalls[0],
				ptCluster->nX + i, ptCluster->nY + j);
			x = ptTileSub->GridSize * i + nX;
			y = ptTileSub->GridSize * j + nY;
			nDest = DrlgGridGet(ptCtx->ptDefGrid, x, y);
			if (ptCtx->pfnCheckCell)
			{
				if (!ptCtx->pfnCheckCell(ptCtx->ptLevel, x, y, nDest, nMask, nCell))
					return FALSE;
			}
			else if (nCell & 1)
			{
				nIndex = ((nCell >> 8) & 255) - 1;
				if (nIndex != ptCtx->nWildcardIndex && ptCtx->nDefBase + nIndex != nDest)
					return FALSE;
				if (!ptCtx->pfnCellTestOpen(ptCtx->ptLevel, x, y))
					return FALSE;
			}
			else if (nMask & 2)
			{
				if (!ptCtx->pfnTryPlacePreset(ptCtx->ptLevel, x, y, 0, 0, 0))
					return FALSE;
			}
		}
	}
	return TRUE;
}

static void __fastcall sLvlSubPlaceCluster(int nX, int nY, PTTILESUBCTX ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub, int nOffset)
{
	DRLGLEVEL *ptLevel;
	int nFileX;
	int nFileY;
	DWORD nCell;
	int nMask;
	int nSub;
	int nIndex;
	int nDef;
	int x;
	int y;
	int i;
	int j;

	ptLevel = ptCtx->ptLevel;
	nFileX = ptCluster->nX + nOffset;
	nFileY = ptCluster->nY;
	nX -= nX % ptTileSub->GridSize;
	nY -= nY % ptTileSub->GridSize;
	for (j = 0; j < ptCluster->nHeight; j++)
	{
		for (i = 0; i < ptCluster->nWidth; i++)
		{
			nCell = sLvlSubGetLayerCell(ptTileSub->ptFile->nWallLayers, &ptTileSub->atWalls[0], i + nFileX,
				j + nFileY);
			nMask = sLvlSubGetLayerCell(ptTileSub->ptFile->nFloorLayers, &ptTileSub->tFloor, i + nFileX, j + nFileY);
			x = ptTileSub->GridSize * i + nX;
			y = ptTileSub->GridSize * j + nY;
			if (nCell & 1)
			{
				nSub = (nCell >> 8) & 255;
				nIndex = nSub - 1;
				if (ptCtx->pfnGetDef)
					nDef = ptCtx->pfnGetDef((nCell >> 20) & 63, nSub);
				else
					nDef = ptCtx->nDefBase + nIndex;
				if (nIndex != ptCtx->nWildcardIndex)
					ptCtx->pfnStampPreset(ptLevel, x, y, nDef, 0, TRUE);
			}
			else if (nMask & 2)
				ptCtx->pfnCellClear(ptLevel, x, y);
			else
				ptCtx->pfnCellBlock(ptLevel, x, y);
		}
	}
}

static BOOL __fastcall sLvlSubTestFitsOpen(int nX, int nY, PTTILESUBROOM ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub);
static void __fastcall sLvlSubPlaceClusterInRoom(void *pPool, int nX, int nY, PTTILESUBROOM ptCtx,
	PTPRESETCLUSTER ptCluster, PTLVLSUBTXT ptTileSub, int nOffset);
static BOOL __fastcall sLvlSubTestFitsSame(int nX, int nY, PTTILESUBROOM ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub);
static void __fastcall sLvlSubPlaceRandomTrials(PTTILESUBROOM ptCtx,
	PTLVLSUBTXT ptTileSub);

void __fastcall TileSubRoomRun(PTTILESUBROOM ptCtx)
{
	PTLVLSUBTXT ptTileSub;
	PTPRESETDS1 ptFile;
	PTPRESETCLUSTER ptCluster;
	ROOM *ptRoom;
	struct HD2ARCHIVE__ *hArchive;
	void *pPool;
	DWORD dwMask;
	int nWidth;
	int nHeight;
	int nAlternate;
	int x;
	int y;
	int i;
	int nX;
	int nY;

	if (ptCtx->nType == -1)
		return;
	ptTileSub = LvlTblsGetTileSubs(ptCtx->nType);
	dwMask = ptCtx->dwPicks;
	hArchive = ptCtx->ptRoom->ptLevel->ptAct->hArchive;
	for (; dwMask; dwMask >>= 1, ptTileSub++)
	{
		if (!(dwMask & 1))
			continue;
		TileSubLoadFile(hArchive, ptTileSub);
		if (ptTileSub->CheckAll)
		{
			ptRoom = ptCtx->ptRoom;
			ptFile = ptTileSub->ptFile;
			pPool = ptRoom->ptLevel->ptAct->pMemPool;
			for (i = 0; i < ptFile->nClusters; i++)
			{
				ptCluster = &ptFile->ptClusters[i];
				nWidth = ptRoom->tCoords.nSizeTileX - ptCluster->nWidth + 1;
				nHeight = ptRoom->tCoords.nSizeTileY - ptCluster->nHeight + 1;
				if (nWidth <= 0 || nHeight <= 0)
					continue;
				switch (ptFile->field_00)
				{
				case 1:
					for (y = 1; y < nHeight; y++)
					{
						for (x = 1; x < nWidth; x++)
						{
							if (sLvlSubTestFitsOpen(x, y, ptCtx, ptCluster, ptTileSub))
								sLvlSubPlaceClusterInRoom(pPool, x, y, ptCtx, ptCluster, ptTileSub, 0);
						}
					}
					break;
				case 2:
					for (nY = 0; nY < nHeight; nY++)
					{
						for (nX = 0; nX < nWidth; nX++)
						{
							if (!sLvlSubTestFitsSame(nX, nY, ptCtx, ptCluster, ptTileSub))
								continue;
							if (ptTileSub->Prob[ptCtx->nSubTheme] >= (int)SeedRand(&ptRoom->tSeed, 100))
								continue;
							if (ptCluster->nAlternates)
								nAlternate = (int)SeedRand(&ptRoom->tSeed, ptCluster->nAlternates);
							else
								nAlternate = 0;
							sLvlSubPlaceClusterInRoom(pPool, nX, nY, ptCtx, ptCluster, ptTileSub,
								(ptCluster->nWidth + 1) * (nAlternate + 1));
						}
					}
					break;
				}
			}
		}
		else
			sLvlSubPlaceRandomTrials(ptCtx, ptTileSub);
			if (0)
			{
				sLvlSubPlaceRandomTrials(ptCtx, ptTileSub);
				sLvlSubTestFitsSame(0, 0, ptCtx, 0, ptTileSub);
			}
	}
}

static void __fastcall sLvlSubPlaceClusterInRoom(void *pPool, int nX, int nY, PTTILESUBROOM ptCtx,
	PTPRESETCLUSTER ptCluster, PTLVLSUBTXT ptTileSub, int nOffset)
{
	ROOM *ptRoom;
	PTDRLGTILEBLOCK ptBlock;
	PTPRESETUNIT ptUnit;
	int nFileX;
	int nFileY;
	int nPairs;
	int nShadows;
	int nCell;
	int i;
	int j;
	int k;

	nPairs = ptTileSub->ptFile->nWallLayers;
	ptRoom = ptCtx->ptRoom;
	ptBlock = ptRoom->ptDrlgTiles;
	nFileX = ptCluster->nX + nOffset;
	nFileY = ptCluster->nY;
	nShadows = 0;
	for (j = 0; j < ptCluster->nHeight; j++)
	{
		for (i = 0; i < ptCluster->nWidth; i++)
		{
			if (DrlgGridGet(&ptTileSub->tShadows, nFileX + i, nFileY + j) & 134217728)
				nShadows++;
		}
	}
	RoomTileGrowShadows(pPool, ptBlock, nShadows);
	for (j = 0; j < ptCluster->nHeight; j++)
	{
		for (i = 0; i < ptCluster->nWidth; i++)
		{
			nCell = sLvlSubGetLayerCell(ptTileSub->ptFile->nFloorLayers, &ptTileSub->tFloor, nFileX + i, nFileY + j);
			if (nCell & 2)
				DrlgGridApply(ptCtx->ptFloorGrid, nX + i, nY + j, nCell, 3);
			for (k = 0; k < nPairs; k++)
			{
				nCell = DrlgGridGet(&ptTileSub->atWalls[k], nFileX + i, nFileY + j);
				if (nCell & 1)
					DrlgGridApply(ptCtx->aptWallGrids[k], nX + i, nY + j, nCell, 3);
				nCell = DrlgGridGet(&ptTileSub->atOrientations[k], nFileX + i, nFileY + j);
				if (nCell)
					DrlgGridApply(ptCtx->aptOrientationGrids[k], nX + i, nY + j, nCell, 3);
			}
			nCell = DrlgGridGet(&ptTileSub->tShadows, nFileX + i, nFileY + j);
			if (nCell & 134217728)
				RoomTileAddShadowAt(ptRoom, ptRoom->tCoords.nTileX + nX + i,
					ptRoom->tCoords.nTileY + nY + j,
					(DWORD)nCell);
		}
	}
	{
		int nSubX;
		int nSubY;
		int nClusterX;
		int nClusterY;
		int nClusterW;
		int nClusterH;

		nSubX = nX;
		nSubY = nY;
		nClusterX = ptCluster->nX;
		nClusterY = ptCluster->nY;
		nClusterW = ptCluster->nWidth;
		nClusterH = ptCluster->nHeight;
		EnvTileToSubtile(&nSubX, &nSubY);
		EnvTileToSubtile(&nClusterX, &nClusterY);
		EnvTileToSubtile(&nClusterW, &nClusterH);
		for (ptUnit = ptTileSub->ptFile->ptUnits; ptUnit; ptUnit = ptUnit->ptNext)
		{
			if (ptUnit->nX > nClusterX && ptUnit->nY > nClusterY &&
				ptUnit->nX < nClusterX + nClusterW && ptUnit->nY < nClusterY + nClusterH)
				DrlgRoomNewPresetUnit(ptCtx->ptRoom, pPool, ptUnit->nUnitType, ptUnit->nClassId,
					ptUnit->nMode,
					ptUnit->nX - nClusterX + nSubX, ptUnit->nY - nClusterY + nSubY);
		}
	}
}

static BOOL __fastcall sLvlSubTestFitsOpen(int nX, int nY, PTTILESUBROOM ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub)
{
	int nCell;
	int nGrids;
	int i;
	int j;
	int k;

	nGrids = ptCtx->nGrids;
	for (j = 0; j < ptCluster->nHeight; j++)
	{
		for (i = 0; i < ptCluster->nWidth; i++)
		{
			if (!(DrlgGridGet(&ptTileSub->tFloor, ptCluster->nX + i, ptCluster->nY + j) & 2))
			{
				if (!ptTileSub->atWalls[0].nWidth)
					continue;
				if (!(DrlgGridGet(&ptTileSub->atWalls[0], ptCluster->nX + i,
					ptCluster->nY + j) & 1))
					continue;
			}
			nCell = DrlgGridGet(ptCtx->ptFloorGrid, nX + i, nY + j);
			if (nCell & 66125568)
				return FALSE;
			if (!(nCell & 2))
				return FALSE;
			for (k = 0; k < nGrids; k++)
			{
				if (DrlgGridGet(ptCtx->aptWallGrids[k], nX + i, nY + j) & 1)
					return FALSE;
			}
		}
	}
	return TRUE;
}

static BOOL __fastcall sLvlSubTestFitsSame(int nX, int nY, PTTILESUBROOM ptCtx, PTPRESETCLUSTER ptCluster,
	PTLVLSUBTXT ptTileSub)
{
	int nCell;
	int nRoom;
	int i;
	int j;

	for (j = 0; j < ptCluster->nHeight; j++)
	{
		for (i = 0; i < ptCluster->nWidth; i++)
		{
			nCell = DrlgGridGet(&ptTileSub->atOrientations[0], ptCluster->nX + i, ptCluster->nY + j);
			if (nCell != DrlgGridGet(ptCtx->aptOrientationGrids[0], nX + i, nY + j))
				return FALSE;
			nCell = sLvlSubGetLayerCell(ptTileSub->ptFile->nFloorLayers, &ptTileSub->tFloor, ptCluster->nX + i,
				ptCluster->nY + j);
			if (nCell & 2)
			{
				nRoom = DrlgGridGet(ptCtx->ptFloorGrid, nX + i, nY + j);
				if (!(nRoom & 2))
					return FALSE;
				if ((nRoom ^ nCell) & 66125568)
					return FALSE;
			}
			nCell = sLvlSubGetLayerCell(ptTileSub->ptFile->nWallLayers, &ptTileSub->atWalls[0], ptCluster->nX + i,
				ptCluster->nY + j);
			if (nCell & 1)
			{
				nRoom = DrlgGridGet(ptCtx->aptWallGrids[0], nX + i, nY + j);
				if (!(nRoom & 1))
					return FALSE;
				if ((nRoom ^ nCell) & 66125568)
					return FALSE;
			}
		}
	}
	return TRUE;
}

static void __fastcall sLvlSubPlaceRandomTrials(PTTILESUBROOM ptCtx, PTLVLSUBTXT ptTileSub)
{
	struct { int x; int y; } anCells[256], tSwap;
	ROOM *ptRoom;
	PTPRESETDS1 ptFile;
	PTPRESETCLUSTER ptCluster;
	void *pPool;
	int nMax;
	int nTrials;
	int nWidth;
	int nHeight;
	int nCount;
	int nA;
	int nB;
	int nX;
	int nY;
	int n;

	ptRoom = ptCtx->ptRoom;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	ptFile = ptTileSub->ptFile;
	nMax = ptTileSub->Max[ptCtx->nSubTheme];
	if (!ptFile->nClusters)
		return;
	for (n = 0; n < nMax; n++)
	{
		nA = sDrawBelow(&ptRoom->tSeed, ptFile->nClusters);
		ptCluster = &ptFile->ptClusters[nA];
		nWidth = ptRoom->tCoords.nSizeTileX - ptCluster->nWidth + 1;
		nHeight = ptRoom->tCoords.nSizeTileY - ptCluster->nHeight + 1;
		if (nWidth <= 1 || nHeight <= 1)
			continue;
		nTrials = ptTileSub->Trials[ptCtx->nSubTheme];
		if (nTrials == -1)
		{
			int i;

			nCount = (nHeight - 1) * (nWidth - 1);
			if (!nCount)
				continue;
			for (i = 0; i < nCount; i++)
			{
				anCells[i].x = i % (nWidth - 1);
				anCells[i].y = i / (nWidth - 1);
			}
			for (i = 0; i < nCount; i++)
			{
				nA = sDrawBelow(&ptRoom->tSeed, nCount);
				nB = sDrawBelow(&ptRoom->tSeed, nCount);
				tSwap = anCells[nA];
				anCells[nA] = anCells[nB];
				anCells[nB] = tSwap;
			}
			for (i = 0; i < nCount; i++)
			{
				nX = anCells[i].x + 1;
				nY = anCells[i].y + 1;
				if (sLvlSubTestFitsOpen(nX, nY, ptCtx, ptCluster, ptTileSub))
				{
					sLvlSubPlaceClusterInRoom(pPool, nX, nY, ptCtx, ptCluster, ptTileSub, 0);
					return;
				}
			}
		}
		else
		{
			int i;

			for (i = 0; i < nTrials; i++)
			{
				nX = sDrawBelow(&ptRoom->tSeed, nWidth - 1) + 1;
				nY = sDrawBelow(&ptRoom->tSeed, nHeight - 1) + 1;
				if (sLvlSubTestFitsOpen(nX, nY, ptCtx, ptCluster, ptTileSub))
				{
					sLvlSubPlaceClusterInRoom(pPool, nX, nY, ptCtx, ptCluster, ptTileSub, 0);
					return;
				}
			}
		}
	}
}

void __fastcall TileSubLoadFile(struct HD2ARCHIVE__ *hArchive, PTLVLSUBTXT ptTileSub)
{
	DRLGCOORDS tCoords;
	int nStride;
	int nHeight;
	int i;

	if (ptTileSub->ptFile)
		return;
	PresetLoadFile(&ptTileSub->ptFile, hArchive, ptTileSub->File);
	nStride = ptTileSub->ptFile->nSizeX + 1;
	nHeight = ptTileSub->ptFile->nSizeY + 1;
	tCoords.nTileX = 0;
	tCoords.nTileY = 0;
	tCoords.nSizeTileX = nStride;
	tCoords.nSizeTileY = nHeight;
#line 687
	D2WARN(ptTileSub->ptFile->nClusters);
	for (i = 0; i < ptTileSub->ptFile->nWallLayers; i++)
	{
		DrlgGridAllocView(NULL, &ptTileSub->atWalls[i], ptTileSub->ptFile->apnWalls[i], &tCoords, nStride);
		DrlgGridAllocView(NULL, &ptTileSub->atOrientations[i], ptTileSub->ptFile->apnOrientations[i],
			&tCoords,
			nStride);
	}
	for (i = 1; i < ptTileSub->ptFile->nWallLayers; i++)
		DrlgGridApplyAll(&ptTileSub->atWalls[i], i << 18, 0);
	if (ptTileSub->ptFile->nFloorLayers)
		DrlgGridAllocView(NULL, &ptTileSub->tFloor, ptTileSub->ptFile->apnFloors[0], &tCoords, nStride);
	if (ptTileSub->ptFile->pnShadows)
		DrlgGridAllocView(NULL, &ptTileSub->tShadows, ptTileSub->ptFile->pnShadows, &tCoords, nStride);
}

void __fastcall TileSubFreeRow(PTLVLSUBTXT ptTileSub)
{
	int i;

	if (!ptTileSub->ptFile)
		return;
	for (i = 0; i < ptTileSub->ptFile->nWallLayers; i++)
	{
		DrlgGridFree(NULL, &ptTileSub->atWalls[i]);
		DrlgGridFree(NULL, &ptTileSub->atOrientations[i]);
	}
	DrlgGridFree(NULL, &ptTileSub->tFloor);
	DrlgGridFree(NULL, &ptTileSub->tShadows);
	PresetFreeFile(&ptTileSub->ptFile);
}

DWORD __fastcall TileSubRoomPick(ROOM *ptRoom, int nType, int nTheme)
{
	PTLVLSUBTXT ptTileSub;
	DWORD dwPicked;
	int i;

	if (nType == -1 || nTheme == -1)
		return 0;
	ptTileSub = LvlTblsGetTileSubs(nType);
	dwPicked = 0;
	for (i = 0; ptTileSub->Type == nType; i++)
	{
		if ((int)SeedRand(&ptRoom->tSeed, 100) < ptTileSub->Prob[nTheme])
		{
			dwPicked |= 1 << i;
			ptRoom->dwDt1Mask |= ptTileSub->Dt1Mask;
		}
		ptTileSub++;
	}
	return dwPicked;
}
