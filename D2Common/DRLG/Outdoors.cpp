#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\Outdoors.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

static __inline int sDrawBelow(D2SEED *ptSeed, int nRange)
{
	return nRange ? (int)SeedRand(ptSeed, nRange) : 0;
}

static const int sgcanEdgeProbe[4][2] =
{
	{ -4, 4 }, { 4, -4 }, { 12, 4 }, { 4, 12 },
};

int __fastcall OutdoorsGetEdgeExits(DRLGLEVEL *ptLevel, DRLGVERTEX *ptVertex)
{
	OUTDOORINFO *ptOutdoor;
	DRLGLEVELLINK *ptLink;
	DRLGLEVEL *ptLinkLevel;
	int *pnVis;
	int nEdge;
	int nX;
	int nY;
	int i;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (ptVertex->nX == 0)
		nEdge = ptVertex->nY == 0;
	else if (ptVertex->nY == 0)
		nEdge = (ptVertex->nX == ptOutdoor->tCellRect.nSizeTileX - 1) + 1;
	else if (ptVertex->nX == ptOutdoor->tCellRect.nSizeTileX - 1)
		nEdge = (ptVertex->nY == ptOutdoor->tCellRect.nSizeTileY - 1) + 2;
	else if (ptVertex->nY == ptOutdoor->tCellRect.nSizeTileY - 1)
		nEdge = 3;
	else
		return 0;
	nX = sgcanEdgeProbe[nEdge][0] + ptVertex->nX * 8 + ptLevel->tCoords.nTileX;
	nY = sgcanEdgeProbe[nEdge][1] + ptVertex->nY * 8 + ptLevel->tCoords.nTileY;
	for (ptLink = ptOutdoor->ptLevelLinks; ptLink; ptLink = ptLink->ptNext)
	{
		if (nEdge == ptLink->nDirection && DrlgRectContains(ptLink->ptCoords, nX, nY))
			break;
	}
	if (!ptLink || ptLink->field_0c)
		return 0;
	ptLinkLevel = ptLink->ptLevel;
	pnVis = DrlgRoomGetVis(ptLevel->ptAct, (int)ptLevel->nLevelId);
	for (i = 0; i < 8; i++)
	{
		if (pnVis[i] == (int)ptLinkLevel->nLevelId)
			break;
	}
	if (i == 8)
		return 0;
	return 1 << (i + 4);
}

int __fastcall OutdoorsGetMarkedDef(DRLGLEVEL *ptLevel, int nX, int nY)
{
	OUTDOORINFO *ptOutdoor;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (!(DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, nY) & 512))
		return 0;
	return DrlgGridGet(&ptOutdoor->tDefGrid, nX, nY);
}

void __fastcall OutdoorsCellClear(DRLGLEVEL *ptLevel, int nX, int nY)
{
	OUTDOORINFO *ptOutdoor;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	DrlgGridApply(&ptOutdoor->tDefGrid, nX, nY, 0, 3);
	DrlgGridApply(&ptOutdoor->tCellFlagGrid, nX, nY, 0, 3);
}

void __fastcall OutdoorsCellBlock(DRLGLEVEL *ptLevel, int nX, int nY)
{
	OUTDOORINFO *ptOutdoor;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	DrlgGridApply(&ptOutdoor->tDefGrid, nX, nY, 0, 3);
	DrlgGridApply(&ptOutdoor->tCellFlagGrid, nX, nY, 256, 3);
}

BOOL __fastcall OutdoorsCellTestOpen(DRLGLEVEL *ptLevel, int nX, int nY)
{
	OUTDOORINFO *ptOutdoor = ptLevel->field_38.pOutdoor;
	return (~(DWORD)DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, nY) >> 10) & 1;
}

BOOL __fastcall OutdoorsCellIsFree(DRLGLEVEL *ptLevel, int nX, int nY)
{
	OUTDOORINFO *ptOutdoor = ptLevel->field_38.pOutdoor;
	return !(DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, nY) & 7041);
}

BOOL __fastcall OutdoorsTryPlacePreset(DRLGLEVEL *ptLevel, int nX, int nY, int nDef, int nGrow, int bSides)
{
	OUTDOORINFO *ptOutdoor;
	PTLVLPRESTTXT ptRecord;
	int nSizeX;
	int nSizeY;
	int x;
	int y;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (nDef == 0)
	{
		nSizeX = 1;
		nSizeY = 1;
	}
	else
	{
		ptRecord = (PTLVLPRESTTXT)D2CommonLvlTblsGetPrest(nDef);
		nSizeX = ptRecord->SizeX / 8;
		nSizeY = ptRecord->SizeY / 8;
	}
	if (nGrow)
	{
		if (bSides & 1)
		{
			nY -= nGrow;
			nSizeY += nGrow;
		}
		if (bSides & 2)
			nSizeX += nGrow;
		if (bSides & 4)
			nSizeY += nGrow;
		if (bSides & 8)
		{
			nX -= nGrow;
			nSizeX += nGrow;
		}
	}
	for (y = nY; y < nY + nSizeY; y++)
	{
		for (x = nX; x < nX + nSizeX; x++)
		{
			if (!DrlgGridInBounds(&ptOutdoor->tCellFlagGrid, x, y))
				return FALSE;
			if (!OutdoorsCellIsFree(ptLevel, x, y))
				return FALSE;
		}
	}
	return TRUE;
}

void __fastcall OutdoorsStampPreset(DRLGLEVEL *ptLevel, int nX, int nY, int nDef, int nFile, BOOL fBorder)
{
	OUTDOORINFO *ptOutdoor;
	PTDRLGFILECYCLE ptCycle;
	PTLVLPRESTTXT ptRecord;
	void *pPool;
	int nSizeX;
	int nSizeY;
	int x;
	int y;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	nSizeX = PresetGetSizeX(nDef) / 8;
	nSizeY = PresetGetSizeY(nDef) / 8;
	if (nFile == -1)
	{
		pPool = ptLevel->ptAct->pMemPool;
		for (ptCycle = ptLevel->ptFileCycleFirst; ptCycle; ptCycle = ptCycle->ptNext)
		{
			if (ptCycle->nDef == nDef)
				break;
		}
		if (!ptCycle)
		{
			ptRecord = (PTLVLPRESTTXT)D2CommonLvlTblsGetPrest(nDef);
#line 240
			ptCycle = (PTDRLGFILECYCLE)D2ALLOCPOOL(pPool, sizeof(DRLGFILECYCLE));
			memset(ptCycle, 0, sizeof(DRLGFILECYCLE));
			ptCycle->nDef = ptRecord->Def;
			ptCycle->nFiles = ptRecord->Files;
			ptCycle->nLastFile = sDrawBelow(&ptLevel->tSeed, ptRecord->Files);
			ptCycle->ptNext = ptLevel->ptFileCycleFirst;
			ptLevel->ptFileCycleFirst = ptCycle;
		}
		ptCycle->nLastFile = (ptCycle->nLastFile + 1) % ptCycle->nFiles;
		nFile = ptCycle->nLastFile;
	}
	for (y = nY; y < nY + nSizeY; y++)
	{
		for (x = nX; x < nX + nSizeX; x++)
		{
			DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 983040, 5);
			DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, (nFile << 16) | 512, 0);
			if (fBorder && ((nDef >= 4 && nDef <= 15) || (nDef >= 364 && nDef <= 375)))
				DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 1, 0);
			DrlgGridApply(&ptOutdoor->tDefGrid, x, y, 0, 3);
		}
	}
	DrlgGridApply(&ptOutdoor->tDefGrid, nX, nY, nDef, 3);
}

BOOL __fastcall OutdoorsPlaceFarthest(DRLGLEVEL *ptLevel, DRLGCOORDS *ptFrom, int nDef, int nFile, int nGrow,
	int bSides)
{
	OUTDOORINFO *ptOutdoor;
	int nWidth;
	int nHeight;
	int nRandX;
	int nRandY;
	int nCentreX;
	int nCentreY;
	int nBest;
	int nBestX;
	int nBestY;
	int nDistX;
	int nDistY;
	int nDist;
	int i;
	int j;
	int x;
	int y;
	int nTileX;
	int nTileY;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	nTileX = ptLevel->tCoords.nTileX;
	nTileY = ptLevel->tCoords.nTileY;
	nWidth = ptOutdoor->tCellRect.nSizeTileX - 2;
	nHeight = ptOutdoor->tCellRect.nSizeTileY - 2;
	nRandX = sDrawBelow(&ptLevel->tSeed, nWidth);
	nRandY = sDrawBelow(&ptLevel->tSeed, nHeight);
	nBest = 0;
	nCentreX = ptFrom->nSizeTileX / 2 + ptFrom->nTileX;
	nCentreY = ptFrom->nSizeTileY / 2 + ptFrom->nTileY;
	nBestX = -1;
	nBestY = -1;
	for (j = 0; j <= nHeight; j++)
	{
		for (i = 0; i <= nWidth; i++)
		{
			y = (j + nRandY) % nHeight + 1;
			x = (i + nRandX) % nWidth + 1;
			if (OutdoorsTryPlacePreset(ptLevel, x, y, nDef, nGrow, bSides))
			{
				nDist = x * 8 - nCentreX + nTileX + 4;
				nDistX = nDist < 0 ? -nDist : nDist;
				nDistY = y * 8 - nCentreY + nTileY + 4;
				if (nDistY < 0)
					nDistY = -nDistY;
				if (nDistX > nDistY)
					nDist = nDistY + nDistX * 2;
				else
					nDist = nDistX + nDistY * 2;
				nDist /= 2;
				if (nBest < nDist)
				{
					nBest = nDist;
					nBestX = x;
					nBestY = y;
				}
			}
		}
	}
	if (nBestX == -1 || nBestY == -1)
		return FALSE;
	OutdoorsStampPreset(ptLevel, nBestX, nBestY, nDef, nFile, FALSE);
	return TRUE;
}

BOOL __fastcall OutdoorsPlacePreset(DRLGLEVEL *ptLevel, int nDef, int nFile, int nGrow, int bSides)
{
	OUTDOORINFO *ptOutdoor;
	struct { int x; int y; } anCells[256], tSwap;
	int nWidth;
	int nCells;
	int nA;
	int nB;
	int i;
	int x;
	int y;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	nWidth = ptOutdoor->tCellRect.nSizeTileX - 2;
	nCells = (ptOutdoor->tCellRect.nSizeTileY - 2) * nWidth;
	if (!nCells)
		return FALSE;
	for (i = 0; i < nCells; i++)
	{
		anCells[i].x = i % nWidth;
		anCells[i].y = i / nWidth;
	}
	for (i = 0; i < nCells; i++)
	{
		nA = sDrawBelow(&ptLevel->tSeed, nCells);
		nB = sDrawBelow(&ptLevel->tSeed, nCells);
		tSwap = anCells[nA];
		anCells[nA] = anCells[nB];
		anCells[nB] = tSwap;
	}
	for (i = 0; i < nCells; i++)
	{
		x = anCells[i].x + 1;
		y = anCells[i].y + 1;
		if (OutdoorsTryPlacePreset(ptLevel, x, y, nDef, nGrow, bSides))
		{
			OutdoorsStampPreset(ptLevel, x, y, nDef, nFile, FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

static const char sgcanNeighbourX[8] = { -1, 0, 0, 1, -1, 1, 1, -1 };
static const char sgcanNeighbourY[8] = { 0, -1, 1, 0, -1, 1, -1, 1 };

BOOL __fastcall OutdoorsPlacePresetNear(DRLGLEVEL *ptLevel, int nDef, int nFile)
{
	OUTDOORINFO *ptOutdoor;
	struct { int x; int y; } anCells[256], tSwap;
	int nWidth;
	int nCells;
	int nA;
	int nB;
	int i;
	int k;
	int x;
	int y;
	int nX;
	int nY;

	D2CommonLvlTblsGetPrest(nDef);
	ptOutdoor = ptLevel->field_38.pOutdoor;
	nWidth = ptOutdoor->tCellRect.nSizeTileX - 2;
	nCells = (ptOutdoor->tCellRect.nSizeTileY - 2) * nWidth;
	if (!nCells)
		return FALSE;
	for (i = 0; i < nCells; i++)
	{
		anCells[i].x = i % nWidth;
		anCells[i].y = i / nWidth;
	}
	for (i = 0; i < nCells; i++)
	{
		nA = sDrawBelow(&ptLevel->tSeed, nCells);
		nB = sDrawBelow(&ptLevel->tSeed, nCells);
		tSwap = anCells[nA];
		anCells[nA] = anCells[nB];
		anCells[nB] = tSwap;
	}
	for (i = 0; i < nCells; i++)
	{
		x = anCells[i].x + 1;
		y = anCells[i].y + 1;
		if (DrlgGridGet(&ptOutdoor->tCellFlagGrid, x, y) & 128)
		{
			for (k = 0; k < 8; k++)
			{
				nX = x + sgcanNeighbourX[k];
				nY = y + sgcanNeighbourY[k];
				if (OutdoorsTryPlacePreset(ptLevel, nX, nY, nDef, 0, 15))
				{
					OutdoorsStampPreset(ptLevel, nX, nY, nDef, nFile, FALSE);
					return TRUE;
				}
			}
		}
	}
	return OutdoorsPlacePreset(ptLevel, nDef, nFile, 0, 15);
}

static __inline OUTDOORINFO *sLevelOutdoor(DRLGLEVEL *ptLevel)
{
	return ptLevel->field_38.pOutdoor;
}

void __fastcall OutdoorsPlaceWaypoint(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	struct { int x; int y; } anCells[256], tSwap;
	int *pnVis;
	int nLevelId;
	int nExit;
	int nWidth;
	int nHeight;
	int nCells;
	int nA;
	int nB;
	int i;
	int x;
	int y;

	nLevelId = (int)ptLevel->nLevelId;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (nLevelId == 3)
	{
		pnVis = DrlgRoomGetVis(ptLevel->ptAct, nLevelId);
		for (i = 0; i < 8; i++)
		{
			if (pnVis[i] == 2)
				break;
		}
		nExit = 1 << (i + 4);
		nWidth = ptOutdoor->tCellRect.nSizeTileX;
		nHeight = ptOutdoor->tCellRect.nSizeTileY;
		for (y = 0; y < nHeight; y++)
		{
			for (x = 0; x < nWidth; x++)
			{
				nA = DrlgGridGet(&ptOutdoor->tExitGrid, x, y);
				nB = DrlgGridGet(&ptOutdoor->tCellFlagGrid, x, y);
				if ((nExit & nA) && (nB & 1024))
				{
					if (x == 0)
						x = 1;
					if (y == 0)
						y = 1;
					if (x == nWidth - 1)
						x--;
					if (y == nHeight - 1)
						y--;
					DrlgGridApply(&ptOutdoor->tExitGrid, x, y, 131072, 0);
					DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 2048, 0);
					return;
				}
			}
		}
	}
	nWidth = ptOutdoor->tCellRect.nSizeTileX - 2;
	nCells = (ptOutdoor->tCellRect.nSizeTileY - 2) * nWidth;
	if (!nCells)
		return;
	for (i = 0; i < nCells; i++)
	{
		anCells[i].x = i % nWidth;
		anCells[i].y = i / nWidth;
	}
	for (i = 0; i < nCells; i++)
	{
		nA = sDrawBelow(&ptLevel->tSeed, nCells);
		nB = sDrawBelow(&ptLevel->tSeed, nCells);
		tSwap = anCells[nA];
		anCells[nA] = anCells[nB];
		anCells[nB] = tSwap;
	}
	for (i = 0; i < nCells; i++)
	{
		x = anCells[i].x + 1;
		y = anCells[i].y + 1;
		if (!(DrlgGridGet(&sLevelOutdoor(ptLevel)->tCellFlagGrid, x, y) & 7041))
		{
			DrlgGridApply(&ptOutdoor->tExitGrid, x, y, 65536, 0);
			DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 2048, 0);
			return;
		}
	}
}

static const int sgcanShrineExits[4] = { 4096, 8192, 16384, 32768 };

void __fastcall OutdoorsPlaceShrines(DRLGLEVEL *ptLevel, int nCount)
{
	OUTDOORINFO *ptOutdoor;
	struct { int x; int y; } anCells[256], tSwap;
	int nIndex;
	int nWidth;
	int nCells;
	int nA;
	int nB;
	int i;
	int x;
	int y;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	nIndex = SeedNext(&ptLevel->tSeed) & 3;
	nWidth = ptOutdoor->tCellRect.nSizeTileX - 2;
	nCells = (ptOutdoor->tCellRect.nSizeTileY - 2) * nWidth;
	if (!nCells)
		return;
	for (i = 0; i < nCells; i++)
	{
		anCells[i].x = i % nWidth;
		anCells[i].y = i / nWidth;
	}
	for (i = 0; i < nCells; i++)
	{
		nA = sDrawBelow(&ptLevel->tSeed, nCells);
		nB = sDrawBelow(&ptLevel->tSeed, nCells);
		tSwap = anCells[nA];
		anCells[nA] = anCells[nB];
		anCells[nB] = tSwap;
	}
	for (i = 0; i < nCells; i++)
	{
		if (nCount <= 0)
			return;
		x = anCells[i].x + 1;
		y = anCells[i].y + 1;
		if (!(DrlgGridGet(&sLevelOutdoor(ptLevel)->tCellFlagGrid, x, y) & 7041))
		{
			DrlgGridApply(&ptOutdoor->tExitGrid, x, y, sgcanShrineExits[nIndex], 0);
			DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 4096, 0);
			nIndex = (nIndex + 1) % 4;
			nCount--;
		}
	}
}

DRLGVERTEX *__fastcall OutdoorsGetEntryPath(DRLGLEVEL *ptLevel, int nEntry)
{
	return ptLevel->field_38.pOutdoor->aptEntryPaths[nEntry];
}

void __fastcall OutdoorsRunLvlSubPass(DRLGLEVEL *ptLevel, int nType, int nDefBase)
{
	TILESUBCTX tCtx;
	OUTDOORINFO *ptOutdoor;
	DWORD i;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	for (i = 0; i < sizeof(TILESUBCTX) / sizeof(DWORD); i++)
		((DWORD *)&tCtx)[i] = 0;
	tCtx.ptCellRect = &ptOutdoor->tCellRect;
	tCtx.ptDefGrid = &ptOutdoor->tDefGrid;
	tCtx.ptLevel = ptLevel;
	tCtx.ptCellFlagGrid = &ptOutdoor->tCellFlagGrid;
	tCtx.nDefBase = nDefBase;
	tCtx.nWildcardIndex = -1;
	tCtx.nType = nType;
	tCtx.pfnCellTestOpen = OutdoorsCellTestOpen;
	tCtx.pfnTryPlacePreset = OutdoorsTryPlacePreset;
	tCtx.pfnCellClear = OutdoorsCellClear;
	tCtx.pfnCellBlock = OutdoorsCellBlock;
	tCtx.pfnStampPreset = OutdoorsStampPreset;
	TileSubRun(&tCtx);
}

void __fastcall OutdoorsLevelInit(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	DWORD i;

#line 829
	ptOutdoor = (OUTDOORINFO *)D2ALLOCPOOL(ptLevel->ptAct->pMemPool, sizeof(OUTDOORINFO));
	ptLevel->field_38.pOutdoor = ptOutdoor;
	for (i = 0; i < sizeof(OUTDOORINFO) / sizeof(DWORD); i++)
		((DWORD *)ptOutdoor)[i] = 0;
}

void __fastcall OutdoorsLevelGenerate(DRLGLEVEL *ptLevel)
{
	void *pPool;
	OUTDOORINFO *ptOutdoor;
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptNext;
	BYTE bAct;
	DWORD dwRoomFlags;
	int nX;
	int nY;
	int nTileX;
	int nTileY;
	DWORD dwExits;
	DWORD dwCellFlags;
	DWORD dwGridFlags;
	int nDef;
	int nSizeX;
	int nSizeY;
	DRLGCOORDS tCoords;
	PTPRESETSUBINFO ptRegion;

	pPool = ptLevel->ptAct->pMemPool;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	ptOutdoor->tCellRect.nTileX = 0;
	ptOutdoor->tCellRect.nTileY = 0;
	ptOutdoor->tCellRect.nSizeTileX = ptLevel->tCoords.nSizeTileX / 8;
	nSizeY = ptLevel->tCoords.nSizeTileY / 8;
	ptOutdoor->tCellRect.nSizeTileY = nSizeY;
	nSizeX = ptOutdoor->tCellRect.nSizeTileX;
	DrlgGridAlloc(pPool, &ptOutdoor->tDefGrid, nSizeX,
		nSizeY);
	DrlgGridAlloc(pPool, &ptOutdoor->tExitGrid, nSizeX,
		nSizeY);
	DrlgGridAlloc(pPool, &ptOutdoor->tCellFlagGrid, nSizeX,
		nSizeY);
	DrlgGridAlloc(pPool, &ptOutdoor->field_40, nSizeX,
		nSizeY);
	bAct = DrlgGetActNum((int)ptLevel->nLevelId);
	DrlgVerBuildOutline(pPool, &ptOutdoor->ptOutline, &ptLevel->tCoords, 0, ptOutdoor->ptLevelLinks);
	ptVertex = ptOutdoor->ptOutline;
	do
	{
		ptVertex->nX /= 8;
		ptVertex->nY /= 8;
		ptVertex = ptVertex->ptNext;
	}
	while (ptVertex != ptOutdoor->ptOutline);
	do
	{
		ptNext = ptVertex->ptNext;
		if (ptVertex->nX == ptNext->nX && ptVertex->nY == ptNext->nY)
		{
			if (ptNext == ptOutdoor->ptOutline)
				ptOutdoor->ptOutline = ptVertex;
			ptVertex->ptNext = ptNext->ptNext;
			ptVertex->dwFlags |= ptNext->dwFlags;
			ptVertex->bConcaveRun = ptNext->bConcaveRun;
#line 702
			D2FREEPOOL(pPool, ptNext);
		}
		ptVertex = ptVertex->ptNext;
	}
	while (ptVertex != ptOutdoor->ptOutline);
	switch (bAct)
	{
	case 0:
		OutWildGenerate(ptLevel);
		break;
	case 1:
		OutDesrGenerate(ptLevel);
		break;
	case 2:
		OutJungGenerate(ptLevel);
		break;
	case 3:
		OutJungGenerateMesa(ptLevel);
		break;
	}
	ptOutdoor = ptLevel->field_38.pOutdoor;
	dwRoomFlags = 0;
	switch (ptLevel->dwLevelType)
	{
	case 2:
		dwRoomFlags = 278787;
		break;
	case 21:
		dwRoomFlags = 4;
		break;
	case 16:
		dwRoomFlags = 1;
		break;
	case 22:
		dwRoomFlags = 1;
		break;
	case 27:
		dwRoomFlags = 1;
		break;
	case 28:
		dwRoomFlags = 1;
		break;
	}
	nTileY = ptLevel->tCoords.nTileY;
	for (nY = 0; nY < ptOutdoor->tCellRect.nSizeTileY; nY++)
	{
		nTileX = ptLevel->tCoords.nTileX;
		for (nX = 0; nX < ptOutdoor->tCellRect.nSizeTileX; nX++)
		{
			dwExits = (DWORD)DrlgGridGet(&ptOutdoor->tExitGrid, nX, nY);
			dwCellFlags = (DWORD)DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, nY);
			if (dwCellFlags & 512)
			{
				nDef = DrlgGridGet(&ptOutdoor->tDefGrid, nX, nY);
				if (nDef)
				{
					tCoords.nTileX = nTileX;
					tCoords.nTileY = nTileY;
					tCoords.nSizeTileX = 0;
					tCoords.nSizeTileY = 0;
					ptRegion = PresetRegionNew(ptLevel, nDef, &tCoords, &ptLevel->tSeed);
					PresetRegionSetFile(ptRegion, (dwCellFlags >> 16) & 15);
					PresetRegionToRooms(ptLevel, ptRegion, dwExits, FALSE);
				}
			}
			else if (!(dwCellFlags & 256))
			{
				dwGridFlags = DrlgGridGet(&ptOutdoor->field_40, nX, nY);
				OutRoomNew(ptLevel, nTileX, nTileY, 8, 8, dwExits, dwCellFlags, dwGridFlags, dwRoomFlags);
			}
			nTileX += 8;
		}
		nTileY += 8;
	}
}

void __fastcall OutdoorsLevelFree(DRLGLEVEL *ptLevel, BOOL fKeep)
{
	void *pPool;
	OUTDOORINFO *ptOutdoor;
	int i;

	pPool = ptLevel->ptAct->pMemPool;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (ptOutdoor->dwFlags & 32)
		ptOutdoor->dwFlags ^= 32;
	if (ptOutdoor->dwFlags & 64)
		ptOutdoor->dwFlags ^= 64;
	DrlgGridFree(pPool, &ptOutdoor->tDefGrid);
	DrlgGridFree(pPool, &ptOutdoor->tExitGrid);
	DrlgGridFree(pPool, &ptOutdoor->tCellFlagGrid);
	DrlgGridFree(pPool, &ptOutdoor->field_40);
	DrlgVerFreeRing(pPool, &ptOutdoor->ptOutline);
	for (i = 0; i < 6; i++)
		DrlgVerFreeRing(pPool, &ptOutdoor->aptEntryPaths[i]);
	memset(ptOutdoor->atEntries, 0, sizeof(ptOutdoor->atEntries));
	memset(ptOutdoor->atEntryEdges, 0, sizeof(ptOutdoor->atEntryEdges));
	memset(ptOutdoor->atMeetingEdges, 0, sizeof(ptOutdoor->atMeetingEdges));
	memset(ptOutdoor->atMeetingPoints, 0, sizeof(ptOutdoor->atMeetingPoints));
	ptOutdoor->nEntries = 0;
	if (!fKeep)
	{
		DrlgRoomFreeLinks(pPool, (PTDRLGROOMLINK)ptOutdoor->ptLevelLinks);
#line 922
		D2FREEPOOL(pPool, ptLevel->field_38.pOutdoor);
		ptLevel->field_38.pOutdoor = NULL;
	}
}
