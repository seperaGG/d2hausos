#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutWild.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "OutWildLocal.h"

#pragma auto_inline(off)

static BOOL __fastcall sTestColumn(DRLGLEVEL *ptLevel, int nX);
static void __fastcall sPlaceColumn(DRLGLEVEL *ptLevel, int nX);
static BOOL __fastcall sIsConcave(DRLGVERTEX *ptVertex);
static BOOL __fastcall sEndsRun(DRLGVERTEX *ptVertex);
static BOOL __fastcall sTryTower(DRLGLEVEL *ptLevel, int nX, int nY);
static void __fastcall sPlaceTownEdges(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceFills(DRLGLEVEL *ptLevel);

static __inline int sCaveEntrancePreset(DRLGLEVEL *ptLevel)
{
	if (ptLevel->nLevelId == 2)
		return 52;
	return 51;
}

void __fastcall OutWildGenerate(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptPrev;
	DRLGVERTEX *ptNext;
	DRLGVERTEX *ptStart;
	DRLGVERTEX *ptLast;
	BOOL fWrapped;
	BOOL fAdded;
	int nRand;
	int nX;
	int nY;
	int nFarX;
	int nFarY;

	if (ptLevel->nLevelId != 2 && ptLevel->nLevelId != 3 && ptLevel->nLevelId != 17)
	{
		ptOutdoor = ptLevel->field_38.pOutdoor;
		fWrapped = FALSE;
		ptVertex = ptOutdoor->ptOutline;
		ptPrev = ptVertex;
		while (ptPrev->ptNext != ptVertex)
			ptPrev = ptPrev->ptNext;
		for (;;)
		{
			ptNext = ptVertex->ptNext;
			if ((ptVertex->nX < ptNext->nX && ptPrev->nY > ptVertex->nY &&
					!(ptVertex->dwFlags & 1) && !(ptPrev->dwFlags & 1)) ||
				(ptVertex->nY > ptNext->nY && ptPrev->nX > ptVertex->nX &&
					!(ptVertex->dwFlags & 1) && !(ptPrev->dwFlags & 1)))
			{
				ptStart = ptVertex;
				ptLast = NULL;
				do
				{
					if (ptVertex == ptOutdoor->ptOutline)
						fWrapped = TRUE;
					if (sEndsRun(ptVertex))
						break;
					if (sIsConcave(ptVertex))
						ptLast = ptVertex;
					ptVertex = ptVertex->ptNext;
				}
				while (ptVertex != ptStart);
				if (ptLast)
				{
					for (ptNext = ptStart; ptNext != ptLast; ptNext = ptNext->ptNext)
						ptNext->bConcaveRun = 1;
					ptNext->bConcaveRun = 1;
					ptOutdoor->dwFlags |= 32;
				}
			}
			ptPrev = ptVertex;
			ptVertex = ptVertex->ptNext;
			if (fWrapped || ptVertex == ptOutdoor->ptOutline)
				break;
		}
	}
	OutdoorsMarkOutline(ptLevel);
	OutdoorsMarkInterior(ptLevel);
	if ((int)ptLevel->nLevelId >= 2 && (int)ptLevel->nLevelId <= 7)
	{
		OutdoorsRunLvlSubPass(ptLevel, 0, 4);
		if (ptLevel->nLevelId != 39)
		{
			ptOutdoor = ptLevel->field_38.pOutdoor;
			if (ptOutdoor->dwFlags & 12)
			{
				nX = ptOutdoor->tCellRect.nSizeTileX - 2;
				if (sTestColumn(ptLevel, nX))
					sPlaceColumn(ptLevel, nX);
			}
			if ((ptOutdoor->dwFlags & 32) && !(ptOutdoor->dwFlags & 64))
			{
				fAdded = FALSE;
				if (SeedNext(&ptLevel->tSeed) & 1)
				{
					for (nX = 0; nX < ptOutdoor->tCellRect.nSizeTileY && !fAdded; nX++)
					{
						for (nY = 0; nY < ptOutdoor->tCellRect.nSizeTileX && !fAdded; nY++)
							fAdded = sTryTower(ptLevel, nX, nY);
					}
				}
				else
				{
					for (nY = 0; nY < ptOutdoor->tCellRect.nSizeTileY && !fAdded; nY++)
					{
						for (nX = 0; nX < ptOutdoor->tCellRect.nSizeTileX && !fAdded; nX++)
							fAdded = sTryTower(ptLevel, nX, nY);
					}
				}
#line 403
				D2WARN(fAdded);
			}
			if ((ptOutdoor->dwFlags & 28) && !(ptOutdoor->dwFlags & 64))
			{
				nFarX = ptOutdoor->tCellRect.nSizeTileX - (ptOutdoor->dwFlags & 16 ? 4 : 5);
				nFarY = ptOutdoor->tCellRect.nSizeTileY - 4;
				nRand = SeedNext(&ptLevel->tSeed) & 3;
				nX = nRand % 2 ? 3 : nFarX;
				nY = nRand / 2 ? 3 : nFarY;
				OutdoorsStampPreset(ptLevel, nX, nY, sCaveEntrancePreset(ptLevel), -1, FALSE);
				ptOutdoor->dwFlags |= 64;
			}
		}
		OutdoorsRunLvlSubPass(ptLevel, 1, 4);
		OutdoorsRunLvlSubPass(ptLevel, 2, 4);
		sPlaceTownEdges(ptLevel);
		OutdoorsRunLvlSubPass(ptLevel, 3, 4);
		OutdoorsBuildEntries(ptLevel);
	}
	if (ptLevel->nLevelId == 39)
	{
		OutdoorsRunLvlSubPass(ptLevel, 0, 4);
		OutdoorsRunLvlSubPass(ptLevel, 1, 4);
		OutdoorsRunLvlSubPass(ptLevel, 2, 4);
		OutdoorsRunLvlSubPass(ptLevel, 3, 4);
	}
	if ((int)ptLevel->nLevelId >= 3 && (int)ptLevel->nLevelId <= 6)
		OutdoorsPlaceWaypoint(ptLevel);
	if ((int)ptLevel->nLevelId >= 2 && (int)ptLevel->nLevelId <= 7)
		OutdoorsPlaceShrines(ptLevel, 5);
	sPlaceFills(ptLevel);
}

static BOOL __fastcall sTestColumn(DRLGLEVEL *ptLevel, int nX)
{
	OUTDOORINFO *ptOutdoor;
	int y;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	for (y = 0; y < ptOutdoor->tCellRect.nSizeTileY; y++)
	{
		if (DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, y) & 2)
			return FALSE;
		if (DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX + 1, y) & 2)
			return FALSE;
	}
	return TRUE;
}

static const int sgcanColumnFiles[13][2] =
{
	{ 0, 0 }, { 2, 2 }, { 0, 3 }, { 1, 1 }, { 3, 0 }, { 0, 2 }, { 0, 1 },
	{ 1, 0 }, { 2, 0 }, { 2, 3 }, { 1, 3 }, { 3, 1 }, { 3, 2 },
};

static void __fastcall sPlaceColumn(DRLGLEVEL *ptLevel, int nX)
{
	OUTDOORINFO *ptOutdoor;
	OUTDOORINFO *ptBridgeOutdoor;
	int nFlags;
	int nDef;
	int nFile;
	int nBridge;
	int nRows;
	int nRand;
	int y;
	int i;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	for (y = 0; y < ptOutdoor->tCellRect.nSizeTileY; y++)
	{
		nFlags = DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, y);
		nDef = DrlgGridGet(&ptOutdoor->tDefGrid, nX, y);
		if (nDef)
		{
			if (nDef == 7 && (nFlags & 983040) == 196608)
				nFile = 3;
			else
				nFile = sgcanColumnFiles[nDef - 3][0];
		}
		else
			nFile = nFlags & 256 ? 0 : 3;
		OutdoorsStampPreset(ptLevel, nX, y, 26, nFile, FALSE);
		nFlags = DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX + 1, y);
		nDef = DrlgGridGet(&ptOutdoor->tDefGrid, nX + 1, y);
		if (nDef)
		{
			if (nDef == 7 && (nFlags & 983040) == 196608)
				nFile = 3;
			else
				nFile = sgcanColumnFiles[nDef - 3][1];
		}
		else
			nFile = nFlags & 256 ? 0 : 3;
		OutdoorsStampPreset(ptLevel, nX + 1, y, 27, nFile, FALSE);
	}
	if (!(ptOutdoor->dwFlags & 20))
		return;
	ptBridgeOutdoor = ptLevel->field_38.pOutdoor;
	nBridge = ptBridgeOutdoor->dwFlags & 4;
	nRows = ptBridgeOutdoor->tCellRect.nSizeTileY - 2;
	nRand = nRows ? (int)SeedRand(&ptLevel->tSeed, nRows) : 0;
	for (i = 0; i < nRows; i++)
	{
		y = (i + nRand) % nRows + 1;
		if (nBridge)
		{
			if (!OutdoorsCellIsFree(ptLevel, nX - 1, y))
				continue;
		}
		else
		{
			if (!OutdoorsCellIsFree(ptLevel, nX - 1, y))
				continue;
			if (!OutdoorsCellIsFree(ptLevel, nX + 2, y))
				continue;
		}
		if ((DrlgGridGet(&ptBridgeOutdoor->tCellFlagGrid, nX, y) & 983040) == 196608 &&
			(DrlgGridGet(&ptBridgeOutdoor->tCellFlagGrid, nX + 1, y) & 983040) == 196608)
		{
			OutdoorsStampPreset(ptLevel, nX, y, 28, 1, FALSE);
			OutdoorsStampPreset(ptLevel, nX + 1, y, 28, nBridge ? 3 : 2, FALSE);
			return;
		}
	}
}

static BOOL __fastcall sIsConcave(DRLGVERTEX *ptVertex)
{
	DRLGVERTEX *ptNext;

	ptNext = ptVertex->ptNext;
	if ((ptVertex->nX < ptVertex->ptNext->nX && ptVertex->ptNext->nY < ptVertex->ptNext->ptNext->nY &&
			!(ptVertex->dwFlags & 1) && !(ptVertex->ptNext->dwFlags & 1)) ||
		(ptVertex->nY > ptNext->nY && ptNext->nX < ptNext->ptNext->nX &&
			!(ptVertex->dwFlags & 1) && !(ptNext->dwFlags & 1)))
		return TRUE;
	return FALSE;
}

static BOOL __fastcall sEndsRun(DRLGVERTEX *ptVertex)
{
	DRLGVERTEX *ptNext;

	ptNext = ptVertex->ptNext;
	if (ptVertex->nY < ptNext->nY)
		return TRUE;
	if (ptVertex->nX > ptNext->nX)
		return TRUE;
	if (ptVertex->dwFlags & 1)
		return TRUE;
	if (ptNext->dwFlags & 1)
		return TRUE;
	return FALSE;
}

static BOOL __fastcall sTryTower(DRLGLEVEL *ptLevel, int nX, int nY)
{
	OUTDOORINFO *ptOutdoor;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	switch (DrlgGridGet(&ptOutdoor->tDefGrid, nX, nY))
	{
	case 16:
		OutdoorsStampPreset(ptLevel, nX, nY, 25, -1, FALSE);
		ptOutdoor->dwFlags |= 64;
		return TRUE;
	case 17:
		OutdoorsStampPreset(ptLevel, nX, nY, 24, -1, FALSE);
		ptOutdoor->dwFlags |= 64;
		return TRUE;
	}
	return FALSE;
}

static void __fastcall sPlaceTownEdges(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	BOOL fAdded;
	int nX;
	int y;
	BOOL fFree;
	DRLGCOORDS *ptTownCoords;

	if (ptLevel->nLevelId == 39)
		return;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (ptOutdoor->dwFlags & 16)
	{
		nX = ptOutdoor->tCellRect.nSizeTileX / 2 - 1;
		fFree = TRUE;
		for (y = 0; y < ptOutdoor->tCellRect.nSizeTileY; y++)
		{
			if (DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, y) & 2)
			{
				fFree = FALSE;
				break;
			}
			if (DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX + 1, y) & 2)
			{
				fFree = FALSE;
				break;
			}
		}
		if (fFree)
			sPlaceColumn(ptLevel, nX);
	}
	if (ptOutdoor->dwFlags & 128)
		OutdoorsStampPreset(ptLevel, 0, 0, 3, 1, FALSE);
	if (ptOutdoor->dwFlags & 256)
		OutdoorsStampPreset(ptLevel, ptOutdoor->tCellRect.nSizeTileX - 7, 0, 3, 2, FALSE);
	if (ptOutdoor->dwFlags & 512)
		OutdoorsStampPreset(ptLevel, 0, 1, 2, 1, FALSE);
	if (ptOutdoor->dwFlags & 1024)
		OutdoorsStampPreset(ptLevel, 0, ptOutdoor->tCellRect.nSizeTileY - 6, 2, 1, FALSE);
	if (ptOutdoor->dwFlags & 64)
		return;
	if (ptLevel->nLevelId == 2)
	{
		ptTownCoords = &DrlgGetLevel(ptLevel->ptAct, LEVEL_ACT_1_TOWN)->tCoords;
		fAdded = OutdoorsPlaceFarthest(ptLevel, ptTownCoords, 52, -1, 1, 15);
	}
	else
		fAdded = OutdoorsPlacePreset(ptLevel, 51, -1, 1, 15);
#line 484
	D2WARN(fAdded);
	ptOutdoor->dwFlags |= 64;
}

static void __fastcall sPlaceFills(DRLGLEVEL *ptLevel)
{
	switch (ptLevel->nLevelId)
	{
	case 2:
		OutdoorsPlacePresetNear(ptLevel, 46, -1);
		if (!(SeedNext(&ptLevel->tSeed) & 3))
		{
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
		}
		else
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
		OutdoorsPlacePreset(ptLevel, 29, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 30, -1, 0, 15);
		return;
	case 3:
		if (SeedNext(&ptLevel->tSeed) & 3)
		{
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
			if (SeedNext(&ptLevel->tSeed) & 1)
				OutdoorsPlacePresetNear(ptLevel, 49, -1);
		}
		else
		{
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
		}
		OutdoorsPlacePreset(ptLevel, 44, -1, 0, 15);
		break;
	case 4:
		OutdoorsPlacePresetNear(ptLevel, 160, -1);
		OutdoorsPlacePresetNear(ptLevel, 45, -1);
		OutdoorsPlacePreset(ptLevel, 162, -1, 0, 15);
		if (SeedNext(&ptLevel->tSeed) & 3)
		{
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
			if (SeedNext(&ptLevel->tSeed) & 1)
				OutdoorsPlacePresetNear(ptLevel, 49, -1);
		}
		else
		{
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
		}
		if (!(SeedNext(&ptLevel->tSeed) & 3))
		{
			OutdoorsPlacePresetNear(ptLevel, 42, -1);
			OutdoorsPlacePresetNear(ptLevel, 42, -1);
		}
		else
			OutdoorsPlacePresetNear(ptLevel, 42, -1);
		OutdoorsPlacePreset(ptLevel, 31, -1, 0, 15);
		return;
	case 5:
		OutdoorsPlacePreset(ptLevel, 161, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 41, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 40, -1, 0, 15);
		if (SeedNext(&ptLevel->tSeed) & 3)
		{
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
			if (SeedNext(&ptLevel->tSeed) & 1)
				OutdoorsPlacePresetNear(ptLevel, 49, -1);
		}
		else
		{
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
		}
		if (!(SeedNext(&ptLevel->tSeed) & 3))
		{
			OutdoorsPlacePresetNear(ptLevel, 43, -1);
			OutdoorsPlacePresetNear(ptLevel, 43, -1);
		}
		else
			OutdoorsPlacePresetNear(ptLevel, 43, -1);
		OutdoorsPlacePreset(ptLevel, 29, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 30, -1, 0, 15);
		return;
	case 6:
		OutdoorsPlacePreset(ptLevel, 163, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 38, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 39, -1, 0, 15);
		if (SeedNext(&ptLevel->tSeed) & 3)
		{
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
			if (SeedNext(&ptLevel->tSeed) & 1)
				OutdoorsPlacePresetNear(ptLevel, 49, -1);
		}
		else
		{
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
			OutdoorsPlacePresetNear(ptLevel, 47, -1);
		}
		if (!(SeedNext(&ptLevel->tSeed) & 3))
		{
			OutdoorsPlacePresetNear(ptLevel, 42, -1);
			OutdoorsPlacePresetNear(ptLevel, 42, -1);
		}
		else
			OutdoorsPlacePresetNear(ptLevel, 42, -1);
		OutdoorsPlacePreset(ptLevel, 29, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 30, -1, 0, 15);
		return;
	case 7:
		if (SeedNext(&ptLevel->tSeed) & 3)
		{
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
			if (SeedNext(&ptLevel->tSeed) & 1)
				OutdoorsPlacePresetNear(ptLevel, 49, -1);
		}
		else
		{
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
			OutdoorsPlacePresetNear(ptLevel, 48, -1);
		}
		if (!(SeedNext(&ptLevel->tSeed) & 3))
		{
			OutdoorsPlacePresetNear(ptLevel, 43, -1);
			OutdoorsPlacePresetNear(ptLevel, 43, -1);
		}
		else
			OutdoorsPlacePresetNear(ptLevel, 43, -1);
		OutdoorsPlacePreset(ptLevel, 31, -1, 0, 15);
		return;
	case 17:
		OutdoorsStampPreset(ptLevel, 1, 1, 108, -1, FALSE);
		return;
	case 39:
		OutdoorsPlacePreset(ptLevel, 50, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 46, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 31, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 38, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 39, -1, 0, 15);
		break;
	default:
		return;
	}
	OutdoorsPlacePreset(ptLevel, 29, -1, 0, 15);
	OutdoorsPlacePreset(ptLevel, 30, -1, 0, 15);
}

#pragma auto_inline(on)
