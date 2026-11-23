#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutPlace.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "OutPlaceLocal.h"

static const int sgcanOutlineDefs[48] =
{
	0, 4, 364, 799, 16, 5, 365, 800, 17, 6, 366, 801, 0, 7, 367, 802, 18, 8, 368, 803, 19, 9, 369, 804,
	22, 10, 370, 805, 0, 11, 371, 806, 0, 12, 372, 807, 23, 13, 373, 808, 0, 14, 374, 809, 0, 15, 375, 810,
};

static const int sgcanEdgeRows[9] = { -1, 1, -1, 0, -1, 2, -1, 3, -1 };

static const int sgcanCornerRows[9][9] =
{
	{ 1, 9, 9, -1, -1, 1, 8, -1, -1 },
	{ 12, -1, -1, -1, -1, 12, 4, -1, -1 },
	{ 5, 2, 2, 10, -1, -1, -1, -1, 10 },
	{ 1, 9, 9, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1, -1, -1, -1, -1, -1 },
	{ -1, -1, -1, -1, -1, -1, 11, 11, 3 },
	{ 12, -1, -1, -1, -1, 12, 4, 4, 7 },
	{ -1, -1, 2, 10, -1, -1, -1, -1, 10 },
	{ -1, -1, 6, 3, -1, -1, 11, 11, 3 },
};

static const int sgcanDesertMiddleDefs[5][2] =
{
	{ 373, 372 }, { 372, 375 }, { 0, 0 }, { 373, 374 }, { 374, 375 },
};

int __fastcall OutdoorsGetEdgeDef(int nDX, int nDY, int nColumn)
{
	return sgcanOutlineDefs[sgcanEdgeRows[nDX + nDY * 3 + 4] * 4 + nColumn];
}

int __fastcall OutdoorsGetCornerDef(int nDX, int nDY, int nNextDX, int nNextDY, int nColumn)
{
	int nRow;

	if (nDX < 0)
		nDX -= 2;
	else if (nDX > 0)
		nDX += 2;
	if (nNextDX < 0)
		nNextDX -= 2;
	else if (nNextDX > 0)
		nNextDX += 2;
	nRow = sgcanCornerRows[nNextDX + nNextDY + 4][nDX + nDY + 4];
	if (nRow == -1)
		return 0;
	return sgcanOutlineDefs[(nRow - 1) * 4 + nColumn];
}

void __fastcall OutdoorsMarkOutside(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	int anCorners[4][4];
	int nCornerX;
	int nCornerY;
	int nStepX;
	int nStepY;
	int x;
	int y;
	int i;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	anCorners[0][0] = 0;
	anCorners[0][1] = 0;
	anCorners[0][2] = 1;
	anCorners[0][3] = 1;
	anCorners[1][0] = 1;
	anCorners[1][1] = 0;
	anCorners[1][2] = -1;
	anCorners[1][3] = 1;
	anCorners[2][0] = 0;
	anCorners[2][1] = 1;
	anCorners[2][2] = 1;
	anCorners[2][3] = -1;
	anCorners[3][0] = 1;
	anCorners[3][1] = 1;
	anCorners[3][2] = -1;
	anCorners[3][3] = -1;
	for (i = 0; i < 4; i++)
	{
		nCornerX = anCorners[i][0];
		if (nCornerX)
			nCornerX = ptOutdoor->tCellRect.nSizeTileX - 1;
		nCornerY = anCorners[i][1];
		if (nCornerY)
			nCornerY = ptOutdoor->tCellRect.nSizeTileY - 1;
		nStepX = anCorners[i][2];
		nStepY = anCorners[i][3];
		x = nCornerX;
		y = nCornerY;
		while (!(DrlgGridGet(&ptOutdoor->tCellFlagGrid, x, y) & 1))
		{
			while (!(DrlgGridGet(&ptOutdoor->tCellFlagGrid, x, y) & 1))
			{
				DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 256, 0);
				x += nStepX;
			}
			y += nStepY;
			x = nCornerX;
		}
	}
}

#pragma auto_inline(off)
void __fastcall OutdoorsMarkOutline(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptNext;
	int nExits;
	int nFlags;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	ptVertex = ptOutdoor->ptOutline;
	ptNext = ptVertex->ptNext;
	do
	{
		if (ptVertex->dwFlags & 1)
		{
			nExits = OutdoorsGetEdgeExits(ptLevel, ptVertex);
			nFlags = ptVertex->bConcaveRun ? 3 : 1;
			DrlgGridApplyLine(&ptOutdoor->tExitGrid, ptVertex, nExits, 0, TRUE);
			DrlgGridApplyLine(&ptOutdoor->tCellFlagGrid, ptVertex, nFlags, 0, TRUE);
		}
		ptVertex = ptNext;
		ptNext = ptVertex->ptNext;
	}
	while (ptVertex != ptOutdoor->ptOutline);
}
#pragma auto_inline(on)

int __fastcall OutdoorsGetBorderColumn(DRLGLEVEL *ptLevel, BYTE bConcave)
{
	switch (ptLevel->dwLevelType)
	{
	case 2:
		return bConcave == 0;
	case 16:
		return 2;
	case 27:
		return 3;
	}
	return -1;
}

void __fastcall OutdoorsMarkInterior(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptNext;
	BYTE bAct;
	BYTE bConcave;
	int nDX;
	int nDY;
	int nNextDX;
	int nNextDY;
	int nAbsX;
	int nAbsY;
	int nLength;
	int nT;
	int nMark;
	int nColumn;
	int nDef;
	int nIndex;
	int nNextX;
	int nNextY;
	int x;
	int y;

	bAct = DrlgGetActNum((int)ptLevel->nLevelId);
	ptOutdoor = ptLevel->field_38.pOutdoor;
	ptVertex = ptOutdoor->ptOutline;
	ptNext = ptVertex->ptNext;
	do
	{
		DrlgVerGetStep(ptVertex, &nDX, &nDY);
		DrlgVerGetStep(ptNext, &nNextDX, &nNextDY);
		nAbsX = nDX < 0 ? -nDX : nDX;
		nAbsY = nDY < 0 ? -nDY : nDY;
		x = ptVertex->nX;
		y = ptVertex->nY;
		nNextX = ptNext->nX;
		nNextY = ptNext->nY;
		if (nAbsX)
		{
			nT = x - nNextX;
			nLength = nT < 0 ? -nT : nT;
		}
		else
		{
			nT = y - nNextY;
			nLength = nT < 0 ? -nT : nT;
		}
		bConcave = ptVertex->bConcaveRun;
		nMark = bConcave ? 3 : 1;
		nColumn = OutdoorsGetBorderColumn(ptLevel, bConcave);
		nDef = OutdoorsGetEdgeDef(nDX, nDY, nColumn);
		if (!(ptVertex->dwFlags & 2))
		{
			while (x != nNextX || y != nNextY)
			{
				x += nDX;
				y += nDY;
				OutdoorsStampPreset(ptLevel, x, y, nDef, -1, FALSE);
				DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, nMark, 0);
			}
		}
		if ((ptVertex->dwFlags & 1) && !(ptVertex->dwFlags & 2))
		{
			switch (bAct)
			{
			case 0:
				x = (ptVertex->nX < ptNext->nX ? ptVertex->nX : ptNext->nX) +
					nLength * nAbsX / 2;
				y = (ptVertex->nY < ptNext->nY ? ptVertex->nY : ptNext->nY) +
					nLength * nAbsY / 2;
				if (ptLevel->nLevelId == 17)
				{
					DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 983040, 5);
					DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 263168, 0);
				}
				else
				{
					DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 983040, 5);
					DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 197632, 0);
				}
				break;
			case 1:
				x = (ptVertex->nX < ptNext->nX ? ptVertex->nX : ptNext->nX) +
					nLength * nAbsX / 2;
				y = (ptVertex->nY < ptNext->nY ? ptVertex->nY : ptNext->nY) +
					nLength * nAbsY / 2;
				nIndex = nDX + nDY * 2 + 2;
				OutdoorsStampPreset(ptLevel, x, y, sgcanDesertMiddleDefs[nIndex][0], -1, FALSE);
				OutdoorsStampPreset(ptLevel, x + nAbsX, y + nAbsY, sgcanDesertMiddleDefs[nIndex][1], -1,
					FALSE);
				break;
			case 3:
				x = (ptVertex->nX < ptNext->nX ? ptVertex->nX : ptNext->nX) +
					nLength * nAbsX / 2;
				y = (ptVertex->nY < ptNext->nY ? ptVertex->nY : ptNext->nY) +
					nLength * nAbsY / 2;
				DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 983040, 5);
				DrlgGridApply(&ptOutdoor->tCellFlagGrid, x, y, 197632, 0);
				break;
			}
		}
		if (!bConcave)
			bConcave = ptNext->bConcaveRun;
		if (bConcave)
			nMark |= 2;
		nColumn = OutdoorsGetBorderColumn(ptLevel, bConcave);
		if (ptVertex->dwFlags & 2)
		{
			if (ptNext->dwFlags & 2)
				nDef = OutdoorsGetCornerDef(nDX, nDY, nNextDX, nNextDY, nColumn);
			else
				nDef = OutdoorsGetCornerDef(nDX, nDY, nNextDX * 2, nNextDY * 2, nColumn);
		}
		else
		{
			if (ptNext->dwFlags & 2)
				nDef = OutdoorsGetCornerDef(nDX * 2, nDY * 2, nNextDX, nNextDY, nColumn);
			else
				nDef = OutdoorsGetCornerDef(nDX * 2, nDY * 2, nNextDX * 2, nNextDY * 2, nColumn);
		}
		if (nDef == 19)
		{
			if (ptVertex->bConcaveRun == 1)
				nDef = (ptNext->bConcaveRun != 1) + 19;
			else
				nDef = 21;
		}
		else if (!nDef)
			goto nextCorner;
		OutdoorsStampPreset(ptLevel, nNextX, nNextY, nDef, -1, FALSE);
		DrlgGridApply(&ptOutdoor->tCellFlagGrid, nNextX, nNextY, nMark, 0);
nextCorner:
		ptVertex = ptNext;
		ptNext = ptNext->ptNext;
	}
	while (ptVertex != ptOutdoor->ptOutline);
	OutdoorsMarkOutside(ptLevel);
}

struct DRLGCHAINROW;
struct DRLGCHAINSTATE
{
 	D2SEED		tSeed;
 	DRLGCOORDS	atCoords[15];
 	const struct DRLGCHAINROW	*ptTable;
 	int			anSides[15];
 	int			anFirstSides[15];
 	int			anAligns[15];
 	int			anFirstAligns[15];
 	int			nRow;
 	int			nLevelId;
};
typedef struct DRLGCHAINSTATE DRLGCHAINSTATE, *PTDRLGCHAINSTATE;

typedef BOOL (__fastcall *PFNCHAINPLACE)(DRLGCHAINSTATE *ptState);
typedef BOOL (__fastcall *PFNCHAINCHECK)(DRLGCHAINSTATE *ptState, int nIndex);
typedef void (__fastcall *PFNCHAINDONE)(DRLGLEVEL *ptLevel, int nIndex, int *pnSides);

struct DRLGCHAINROW
{
 	PFNCHAINPLACE	pfnPlace;
 	int			nLevelId;
 	int			nParentRow;
 	int			nLinkRow;
};
typedef struct DRLGCHAINROW DRLGCHAINROW;

static BOOL __fastcall sPlaceRoot(DRLGCHAINSTATE *ptState);
static BOOL __fastcall sPlaceFourSides(DRLGCHAINSTATE *ptState);
static void __fastcall sAttachA(DRLGCOORDS *ptParent, DRLGCOORDS *ptChild, int nSide,
	int nGap);
static BOOL __fastcall sPlaceEightPlaces(DRLGCHAINSTATE *ptState);
static void __fastcall sAttachEightPlaces(DRLGCOORDS *ptParent, DRLGCOORDS *ptChild, int nSide,
	int nGap);
static BOOL __fastcall sPlaceTownFirstAct(DRLGCHAINSTATE *ptState);
static void __fastcall sAttachB(DRLGCOORDS *ptParent, DRLGCOORDS *ptChild, int nSide,
	int nGap);
static BOOL __fastcall sPlaceBloodMoor(DRLGCHAINSTATE *ptState);
static BOOL __fastcall sPlaceBelowParent(DRLGCHAINSTATE *ptState);
static BOOL __fastcall sPlaceRockyWaste(DRLGCHAINSTATE *ptState);
static BOOL __fastcall sPlaceEightPlacesNoGap(DRLGCHAINSTATE *ptState);
static BOOL __fastcall sPlaceOuterSteppes(DRLGCHAINSTATE *ptState);
static BOOL __fastcall sCheckChainTownGraveyard(DRLGCHAINSTATE *ptState, int nIndex);
static BOOL __fastcall sCheckChainFarmMonastery(DRLGCHAINSTATE *ptState, int nIndex);
static BOOL __fastcall sCheckChainDesert(DRLGCHAINSTATE *ptState, int nIndex);
static BOOL __fastcall sCheckChainValleyOfKings(DRLGCHAINSTATE *ptState, int nIndex);
static BOOL __fastcall sCheckChainMesa(DRLGCHAINSTATE *ptState, int nIndex);
static BOOL __fastcall sCheckChainDiablo(DRLGCHAINSTATE *ptState, int nIndex);
static void __fastcall sSetOutdoorFlags(DRLGLEVEL *ptLevel, int nIndex, int *pnSides);
static void __fastcall sChainLevels(DRLGACTCTX *ptAct, const DRLGCHAINROW *ptTable, PFNCHAINCHECK pfnCheck,
	PFNCHAINDONE pfnDone);
static void __fastcall sLinkOutdoorLevels(DRLGACTCTX *ptAct, int nFirst, int nLast);

static DRLGLEVEL * __fastcall sCarveKurast(DRLGLEVEL *ptLevel);

static DWORD sgdwOuterSteppesFlags;

static BOOL __fastcall sPlaceRoot(DRLGCHAINSTATE *ptState)
{
	PTLEVELSREC ptLevelDef;

	if (ptState->anFirstSides[ptState->nRow] == -1)
		ptState->anSides[ptState->nRow] = -1;
	ptLevelDef = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptState->nLevelId);
	ptState->atCoords[ptState->nRow].nTileX = ptLevelDef->OffsetX;
	ptState->atCoords[ptState->nRow].nTileY = ptLevelDef->OffsetY;
	return TRUE;
}

static BOOL __fastcall sPlaceFourSides(DRLGCHAINSTATE *ptState)
{
	int nFirst;
	int nSide;

	nFirst = ptState->anFirstSides[ptState->nRow];
	if (nFirst == -1)
	{
		ptState->anFirstSides[ptState->nRow] = SeedNext(&ptState->tSeed) & 3;
		ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
	}
	else
	{
		nSide = (ptState->anSides[ptState->nRow] + 1) % 4;
		if (nSide == nFirst)
			return FALSE;
		ptState->anSides[ptState->nRow] = nSide;
	}
	sAttachA(&ptState->atCoords[ptState->ptTable[ptState->nRow].nParentRow],
		&ptState->atCoords[ptState->nRow], ptState->anSides[ptState->nRow], 1);
	return TRUE;
}

static void __fastcall sAttachA(DRLGCOORDS *ptParent, DRLGCOORDS *ptChild, int nSide, int nGap)
{
	switch (nSide)
	{
	case 0:
		ptChild->nTileX = ptParent->nTileX;
		ptChild->nTileY = ptParent->nTileY + ptParent->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX -= 16;
		break;
	case 1:
		ptChild->nTileX = ptParent->nTileX - ptChild->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY;
		if (nGap == 1)
			ptChild->nTileY -= 16;
		else if (nGap == 2)
			ptChild->nTileY += 8;
		break;
	case 2:
		ptChild->nTileX = ptParent->nTileX + ptParent->nSizeTileX - ptChild->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY - ptChild->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX += 16;
		break;
	case 3:
		ptChild->nTileX = ptParent->nTileX + ptParent->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY + ptParent->nSizeTileY - ptChild->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileY += 16;
		else if (nGap == 2)
			ptChild->nTileY -= 8;
		else if (nGap == 3)
			ptChild->nTileY += 8;
		break;
	}
}

static BOOL __fastcall sPlaceEightPlaces(DRLGCHAINSTATE *ptState)
{
	int nFirst;
	int nSide;

	nFirst = ptState->anFirstSides[ptState->nRow];
	if (nFirst == -1)
	{
		ptState->anFirstSides[ptState->nRow] = SeedNext(&ptState->tSeed) & 7;
		ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
	}
	else
	{
		nSide = (ptState->anSides[ptState->nRow] + 1) % 8;
		if (nSide == nFirst)
			return FALSE;
		ptState->anSides[ptState->nRow] = nSide;
	}
	sAttachEightPlaces(&ptState->atCoords[ptState->ptTable[ptState->nRow].nParentRow],
		&ptState->atCoords[ptState->nRow], ptState->anSides[ptState->nRow], 1);
	return TRUE;
}

static void __fastcall sAttachEightPlaces(DRLGCOORDS *ptParent, DRLGCOORDS *ptChild, int nSide, int nGap)
{
	switch (nSide)
	{
	case 0:
		ptChild->nTileX = ptParent->nTileX;
		ptChild->nTileY = ptParent->nTileY + ptParent->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX -= ptChild->nSizeTileX / 2 + 8;
		break;
	case 1:
		ptChild->nTileX = ptParent->nTileX;
		ptChild->nTileY = ptParent->nTileY + ptParent->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX += ptChild->nSizeTileX / 2 + 8;
		break;
	case 2:
		ptChild->nTileX = ptParent->nTileX - ptChild->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY;
		if (nGap == 1)
			ptChild->nTileY -= ptChild->nSizeTileY / 2 + 8;
		break;
	case 3:
		ptChild->nTileX = ptParent->nTileX - ptChild->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY;
		if (nGap == 1)
			ptChild->nTileY += ptChild->nSizeTileY / 2 + 8;
		break;
	case 4:
		ptChild->nTileX = ptParent->nTileX;
		ptChild->nTileY = ptParent->nTileY - ptChild->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX -= ptChild->nSizeTileX / 2 + 8;
		break;
	case 5:
		ptChild->nTileX = ptParent->nTileX;
		ptChild->nTileY = ptParent->nTileY - ptChild->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX += ptChild->nSizeTileX / 2 + 8;
		break;
	case 6:
		ptChild->nTileX = ptParent->nTileX + ptParent->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY;
		if (nGap == 1)
			ptChild->nTileY -= ptChild->nSizeTileY / 2 + 8;
		break;
	case 7:
		ptChild->nTileX = ptParent->nTileX + ptParent->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY;
		if (nGap == 1)
			ptChild->nTileY += ptChild->nSizeTileY / 2 + 8;
		break;
	}
}

static BOOL __fastcall sPlaceTownFirstAct(DRLGCHAINSTATE *ptState)
{
	int nFirst;
	int nAlign;
	int nSide;
	int nParent;

	nFirst = ptState->anFirstSides[ptState->nRow];
	if (nFirst == -1)
	{
		ptState->anFirstSides[ptState->nRow] = SeedNext(&ptState->tSeed) & 3;
		ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
		ptState->anFirstAligns[ptState->nRow] = SeedNext(&ptState->tSeed) & 1;
		ptState->anAligns[ptState->nRow] = ptState->anFirstAligns[ptState->nRow];
	}
	else
	{
		nAlign = ptState->anAligns[ptState->nRow];
		nSide = (nAlign + ptState->anSides[ptState->nRow]) % 4;
		nAlign = (nAlign + 1) % 2;
		if (nSide == nFirst && nAlign == ptState->anFirstAligns[ptState->nRow])
			return FALSE;
		ptState->anSides[ptState->nRow] = nSide;
		ptState->anAligns[ptState->nRow] = nAlign;
	}
	nParent = ptState->ptTable[ptState->nRow].nParentRow;
	if (ptState->anAligns[ptState->nRow] == 1)
		sAttachA(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 2);
	else
		sAttachB(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 2);
	return TRUE;
}

static void __fastcall sAttachB(DRLGCOORDS *ptParent, DRLGCOORDS *ptChild, int nSide, int nGap)
{
	switch (nSide)
	{
	case 0:
		ptChild->nTileX = ptParent->nTileX + ptParent->nSizeTileX - ptChild->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY + ptParent->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX += 16;
		break;
	case 1:
		ptChild->nTileX = ptParent->nTileX - ptChild->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY + ptParent->nSizeTileY - ptChild->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileY += 16;
		else if (nGap == 2)
			ptChild->nTileY -= 8;
		break;
	case 2:
		ptChild->nTileX = ptParent->nTileX;
		ptChild->nTileY = ptParent->nTileY - ptChild->nSizeTileY;
		if (nGap == 1)
			ptChild->nTileX -= 16;
		break;
	case 3:
		ptChild->nTileX = ptParent->nTileX + ptParent->nSizeTileX;
		ptChild->nTileY = ptParent->nTileY;
		if (nGap == 1)
			ptChild->nTileY -= 16;
		else if (nGap == 2)
			ptChild->nTileY += 8;
		else if (nGap == 3)
			ptChild->nTileY -= 8;
		break;
	}
}

static BOOL __fastcall sPlaceBloodMoor(DRLGCHAINSTATE *ptState)
{
	int nFirst;
	int nAlign;
	int nSide;
	int nParent;

	nFirst = ptState->anFirstSides[ptState->nRow];
	if (nFirst == -1)
	{
		ptState->anFirstSides[ptState->nRow] = SeedNext(&ptState->tSeed) & 3;
		ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
		ptState->anFirstAligns[ptState->nRow] = SeedNext(&ptState->tSeed) & 1;
		ptState->anAligns[ptState->nRow] = ptState->anFirstAligns[ptState->nRow];
	}
	else
	{
		nAlign = ptState->anAligns[ptState->nRow];
		nSide = (nAlign + ptState->anSides[ptState->nRow]) % 4;
		nAlign = (nAlign + 1) % 2;
		if (nSide == nFirst && nAlign == ptState->anFirstAligns[ptState->nRow])
			return FALSE;
		ptState->anSides[ptState->nRow] = nSide;
		ptState->anAligns[ptState->nRow] = nAlign;
	}
	ptState->atCoords[ptState->nRow].nSizeTileX = ptState->anSides[ptState->nRow] % 2 ? 96 : 56;
	ptState->atCoords[ptState->nRow].nSizeTileY = ptState->anSides[ptState->nRow] % 2 ? 56 : 96;
	nParent = ptState->ptTable[ptState->nRow].nParentRow;
	if (ptState->anAligns[ptState->nRow] == 1)
		sAttachA(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 1);
	else
		sAttachB(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 1);
	return TRUE;
}

static BOOL __fastcall sPlaceBelowParent(DRLGCHAINSTATE *ptState)
{
	ptState->anFirstSides[ptState->nRow] = 0;
	ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
	sAttachA(&ptState->atCoords[ptState->ptTable[ptState->nRow].nParentRow],
		&ptState->atCoords[ptState->nRow], ptState->anSides[ptState->nRow], 0);
	return TRUE;
}

static BOOL __fastcall sPlaceRockyWaste(DRLGCHAINSTATE *ptState)
{
	int nFirst;
	int nSide;
	int nParent;

	nFirst = ptState->anFirstSides[ptState->nRow];
	if (nFirst == -1)
	{
		ptState->anFirstSides[ptState->nRow] = (SeedNext(&ptState->tSeed) & 1) + 1;
		ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
	}
	else
	{
		nSide = ptState->anSides[ptState->nRow] == 1 ? 2 : 1;
		if (nSide == nFirst)
			return FALSE;
		ptState->anSides[ptState->nRow] = nSide;
	}
	nParent = ptState->ptTable[ptState->nRow].nParentRow;
	if (ptState->anSides[ptState->nRow] == 1)
		sAttachA(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 0);
	else
		sAttachB(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 0);
	return TRUE;
}

static BOOL __fastcall sPlaceEightPlacesNoGap(DRLGCHAINSTATE *ptState)
{
	int nFirst;
	int nSide;

	nFirst = ptState->anFirstSides[ptState->nRow];
	if (nFirst == -1)
	{
		ptState->anFirstSides[ptState->nRow] = SeedNext(&ptState->tSeed) & 7;
		ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
	}
	else
	{
		nSide = (ptState->anSides[ptState->nRow] + 1) % 8;
		if (nSide == nFirst)
			return FALSE;
		ptState->anSides[ptState->nRow] = nSide;
	}
	sAttachEightPlaces(&ptState->atCoords[ptState->ptTable[ptState->nRow].nParentRow],
		&ptState->atCoords[ptState->nRow], ptState->anSides[ptState->nRow], 0);
	return TRUE;
}

static BOOL __fastcall sPlaceOuterSteppes(DRLGCHAINSTATE *ptState)
{
	int nParent;

	ptState->anFirstSides[ptState->nRow] = 3;
	ptState->anSides[ptState->nRow] = ptState->anFirstSides[ptState->nRow];
	nParent = ptState->ptTable[ptState->nRow].nParentRow;
	if (SeedNext(&ptState->tSeed) & 1)
	{
		sAttachA(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 3);
		sgdwOuterSteppesFlags = 8388608;
	}
	else
	{
		sAttachB(&ptState->atCoords[nParent], &ptState->atCoords[ptState->nRow],
			ptState->anSides[ptState->nRow], 3);
		sgdwOuterSteppesFlags = 4194304;
	}
	return TRUE;
}

static const DRLGCHAINROW sgctChainAct1A[15] =
{
	{ sPlaceRoot, 4, -1, -1 }, { sPlaceFourSides, 3, 0, -1 }, { sPlaceBloodMoor, 2, 1, -1 },
	{ sPlaceTownFirstAct, 1, 2, -1 }, { sPlaceFourSides, 17, 1, -1 }, { NULL, 0, -1, -1 },
};
static const DRLGCHAINROW sgctChainAct1B[15] =
{
	{ sPlaceRoot, 39, -1, -1 }, { sPlaceRoot, 26, -1, -1 }, { sPlaceBelowParent, 7, 1, -1 },
	{ sPlaceFourSides, 6, 2, -1 }, { sPlaceFourSides, 5, 3, -1 }, { NULL, 0, -1, -1 },
};
static const DRLGCHAINROW sgctChainAct2A[15] =
{
	{ sPlaceRoot, 40, -1, -1 }, { sPlaceRockyWaste, 41, 0, -1 }, { sPlaceEightPlaces, 42, 1, -1 },
	{ sPlaceEightPlaces, 43, 2, -1 }, { sPlaceEightPlaces, 44, 3, -1 }, { sPlaceEightPlacesNoGap, 45, 4, -1 },
	{ NULL, 0, -1, -1 },
};
static const DRLGCHAINROW sgctChainAct2B[15] =
{
	{ sPlaceRoot, 46, -1, -1 }, { NULL, 0, -1, -1 },
};
static const DRLGCHAINROW sgctChainAct4A[15] =
{
	{ sPlaceRoot, 103, -1, -1 }, { sPlaceOuterSteppes, 104, 0, -1 }, { sPlaceFourSides, 105, 1, -1 },
	{ sPlaceFourSides, 106, 2, -1 }, { NULL, 0, -1, -1 },
};
static const DRLGCHAINROW sgctChainAct4B[15] =
{
	{ sPlaceRoot, 108, -1, -1 }, { NULL, 0, -1, -1 },
};

static const int sgcanTownAccept[64] =
{
	1, 1, 0, 0, 0, 0, 0, 0,  0, 1, 0, 0, 0, 0, 0, 0,  0, 0, 0, 1, 0, 1, 0, 0,  1, 0, 0, 0, 0, 0, 1, 1,
	0, 1, 0, 0, 0, 0, 0, 1,  0, 1, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 1, 1, 0,  1, 0, 0, 0, 0, 0, 1, 1,
};

static const int sgcanOutdoorFlags[15][6] =
{
	{ 0, 2, 3, 1, 0, 4 }, { 0, 2, 3, 2, 3, 4 }, { 0, 3, 17, 2, 1, 8 }, { 0, 3, 17, 3, 0, 8 },
	{ 0, 3, 17, 1, 1, 16 }, { 0, 3, 17, 3, 3, 16 }, { 2, 0, 0, 0, 0, 8 }, { 2, 0, 0, 2, 2, 8 },
	{ 2, 0, 0, 3, 0, 8 }, { 2, 0, 0, 3, 2, 8 }, { 2, 0, 0, 0, 1, 1024 }, { 2, 0, 0, 1, 1, 1024 },
	{ 2, 0, 0, 2, 1, 512 }, { 2, 0, 0, 2, 2, 128 }, { 2, 0, 0, 3, 2, 256 },
};

static __inline void sStackKurast(DRLGLEVEL *ptBase)
{
	DRLGLEVEL *ptOther;
	PTLEVELSREC ptLevelDef;
	DRLGACTCTX *ptBaseAct;
	int nOffsetY;
	int nHalf;
	int nSizeX;
	int nSizeY;
	int i;

	nOffsetY = 0;
	ptBaseAct = ptBase->ptAct;
	nHalf = ptBase->tCoords.nSizeTileX / 2;
	for (i = 0; i < 5; i++)
	{
		ptLevelDef = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(79 + i);
		nSizeX = ptLevelDef->SizeX;
		nSizeY = ptLevelDef->SizeY;
		nOffsetY -= nSizeY;
		ptOther = DrlgGetLevel(ptBaseAct, 79 + i);
		ptOther->tCoords.nTileX = ptBase->tCoords.nTileX - nSizeX / 2 + nHalf;
		ptOther->tCoords.nTileY = nOffsetY + ptBase->tCoords.nTileY;
		ptOther->tCoords.nSizeTileX = nSizeX;
		ptOther->tCoords.nSizeTileY = nSizeY;
	}
}

void __fastcall OutdoorsLayoutAct(DRLGACTCTX *ptAct, BYTE bAct)
{
	DRLGLEVEL *ptLevel;
	DRLGLEVEL *ptOther;
	PTLEVELSREC ptLevelDef;
	PTVISINFO ptVisInfo;
	int nLevelId;
	int nOther;

	switch (bAct)
	{
	case 0:
		sChainLevels(ptAct, sgctChainAct1A, sCheckChainTownGraveyard, sSetOutdoorFlags);
		sChainLevels(ptAct, sgctChainAct1B, sCheckChainFarmMonastery, sSetOutdoorFlags);
		sLinkOutdoorLevels(ptAct, 1, 17);
		break;
	case 1:
		sChainLevels(ptAct, sgctChainAct2A, sCheckChainDesert, NULL);
		sChainLevels(ptAct, sgctChainAct2B, sCheckChainValleyOfKings, NULL);
		sLinkOutdoorLevels(ptAct, 40, 46);
		break;
	case 2:
		ptLevel = DrlgGetLevel(ptAct, LEVEL_ACT_3_TOWN);
		ptLevelDef = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(75);
		ptLevel->tCoords.nTileX = ptLevelDef->OffsetX;
		ptLevel->tCoords.nTileY = ptLevelDef->OffsetY;
		ptLevel->tCoords.nSizeTileX = ptLevelDef->SizeX;
		ptLevel->tCoords.nSizeTileY = ptLevelDef->SizeY;
		ptLevel = sCarveKurast(ptLevel);
		sStackKurast(ptLevel);
		for (nLevelId = 75; nLevelId <= 83; nLevelId++)
		{
			ptLevel = DrlgGetLevel(ptAct, nLevelId);
			ptVisInfo = DrlgActGetVisInfo(ptAct, nLevelId);
			for (nOther = 75; nOther <= 83; nOther++)
			{
				if (nLevelId == nOther)
					continue;
				ptOther = DrlgGetLevel(ptAct, nOther);
				if (DrlgRectIsAdjacent(&ptLevel->tCoords, &ptOther->tCoords, -1))
					DrlgVisInfoSetLevel(ptVisInfo, nOther, -1, -1);
			}
		}
		sLinkOutdoorLevels(ptAct, 75, 83);
		break;
	case 3:
		sChainLevels(ptAct, sgctChainAct4A, sCheckChainMesa, NULL);
		sChainLevels(ptAct, sgctChainAct4B, sCheckChainDiablo, NULL);
		DrlgGetLevel(ptAct, LEVEL_ACT_4_MESA_1)->field_38.pOutdoor->dwFlags |= sgdwOuterSteppesFlags;
		sLinkOutdoorLevels(ptAct, 103, 106);
		break;
	}
}

static BOOL __fastcall sCheckChainTownGraveyard(DRLGCHAINSTATE *ptState, int nIndex)
{
	int nParent;
	int i;

	nParent = sgctChainAct1A[nIndex].nParentRow;
	for (i = 0; i < nIndex; i++)
	{
		if (i == nParent)
			continue;
		if (!DrlgRectGapTest(&ptState->atCoords[nIndex], &ptState->atCoords[i], 0))
			return FALSE;
	}
	switch (sgctChainAct1A[nIndex].nLevelId)
	{
	case 1:
		return sgcanTownAccept[ptState->anSides[nIndex] + ptState->anAligns[nIndex] * 4 +
			ptState->anSides[nParent] * 8 + ptState->anAligns[nParent] * 32];
	case 17:
		for (i = 0; i < 15; i++)
		{
			if (i != nIndex && sgctChainAct1A[i].nParentRow == nParent &&
				ptState->anSides[nIndex] == ptState->anSides[i])
				return FALSE;
		}
		break;
	}
	return TRUE;
}

static BOOL __fastcall sCheckChainFarmMonastery(DRLGCHAINSTATE *ptState, int nIndex)
{
	int nParent;
	int i;
	BOOL fResult;

	nParent = sgctChainAct1B[nIndex].nParentRow;
	for (i = 0; i < nIndex; i++)
	{
		if (i == nParent)
			continue;
		if (!DrlgRectGapTest(&ptState->atCoords[nIndex], &ptState->atCoords[i], 0))
			return FALSE;
	}
	fResult = TRUE;
	if (nIndex)
	{
		ptState->atCoords[0].nSizeTileY += 200;
		ptState->atCoords[0].nTileY -= 200;
		fResult = DrlgRectGapTest(&ptState->atCoords[0], &ptState->atCoords[i], 0);
		ptState->atCoords[0].nSizeTileY -= 200;
		ptState->atCoords[0].nTileY += 200;
	}
	return fResult;
}

static BOOL __fastcall sCheckChainDesert(DRLGCHAINSTATE *ptState, int nIndex)
{
	int nParent;
	int i;

	nParent = sgctChainAct2A[nIndex].nParentRow;
	for (i = 0; i < nIndex; i++)
	{
		if (i == nParent)
			continue;
		if (!DrlgRectGapTest(&ptState->atCoords[nIndex], &ptState->atCoords[i], 0))
			return FALSE;
	}
	return TRUE;
}

static BOOL __fastcall sCheckChainValleyOfKings(DRLGCHAINSTATE *ptState, int nIndex)
{
	int nParent;
	int i;

	nParent = sgctChainAct2B[nIndex].nParentRow;
	for (i = 0; i < nIndex; i++)
	{
		if (i == nParent)
			continue;
		if (!DrlgRectGapTest(&ptState->atCoords[nIndex], &ptState->atCoords[i], 0))
			return FALSE;
	}
	return TRUE;
}

static BOOL __fastcall sCheckChainMesa(DRLGCHAINSTATE *ptState, int nIndex)
{
	int nParent;
	int i;

	nParent = sgctChainAct4A[nIndex].nParentRow;
	for (i = 0; i < nIndex; i++)
	{
		if (i == nParent)
			continue;
		if (!DrlgRectGapTest(&ptState->atCoords[nIndex], &ptState->atCoords[i], 0))
			return FALSE;
	}
	return TRUE;
}

static BOOL __fastcall sCheckChainDiablo(DRLGCHAINSTATE *ptState, int nIndex)
{
	int nParent;
	int i;

	nParent = sgctChainAct4B[nIndex].nParentRow;
	for (i = 0; i < nIndex; i++)
	{
		if (i == nParent)
			continue;
		if (!DrlgRectGapTest(&ptState->atCoords[nIndex], &ptState->atCoords[i], 0))
			return FALSE;
	}
	return TRUE;
}

static void __fastcall sSetOutdoorFlags(DRLGLEVEL *ptLevel, int nIndex, int *pnSides)
{
	OUTDOORINFO *ptOutdoor;
	int i;

	if (ptLevel->dwDrlgType != 3)
		return;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	for (i = 0; i < 15; i++)
	{
		if ((int)ptLevel->nLevelId != sgcanOutdoorFlags[i][0] && sgcanOutdoorFlags[i][0])
			continue;
		if ((int)ptLevel->nLevelId == sgcanOutdoorFlags[i][1] ||
			(int)ptLevel->nLevelId == sgcanOutdoorFlags[i][2])
			continue;
		if (pnSides[nIndex] == sgcanOutdoorFlags[i][3] && pnSides[nIndex + 1] == sgcanOutdoorFlags[i][4])
			ptOutdoor->dwFlags |= sgcanOutdoorFlags[i][5];
	}
}

static void __fastcall sChainLevels(DRLGACTCTX *ptAct, const DRLGCHAINROW *ptTable, PFNCHAINCHECK pfnCheck,
	PFNCHAINDONE pfnDone)
{
	DRLGCHAINSTATE tState;
	const DRLGCHAINROW *ptRow;
	DRLGLEVEL *ptLevel;
	PRESETLEVEL *ptPreset;
	PTLEVELSREC ptLevelDef;
	PTVISINFO ptVisInfo;
	PTVISINFO ptOtherVis;
	int nLevelId;
	int nParent;
	int nSecond;
	int i;

	memset(&tState, 0, sizeof(tState));
	for (i = 0; i < 15; i++)
	{
		tState.anAligns[i] = -1;
		tState.anFirstAligns[i] = -1;
		tState.anSides[i] = -1;
		tState.anFirstSides[i] = -1;
	}
	tState.ptTable = ptTable;
	tState.tSeed = ptAct->tSeed;
	for (i = 0; ptTable[i].nLevelId; i++)
	{
		ptLevelDef = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptTable[i].nLevelId);
		tState.atCoords[i].nSizeTileX = ptLevelDef->SizeX;
		tState.atCoords[i].nSizeTileY = ptLevelDef->SizeY;
	}
	i = 0;
	ptRow = ptTable;
	while (ptRow->nLevelId)
	{
		tState.nRow = i;
		tState.nLevelId = ptRow->nLevelId;
		if (!ptRow->pfnPlace(&tState))
		{
			tState.anAligns[i] = -1;
			tState.anFirstAligns[i] = -1;
			tState.anSides[i] = -1;
			tState.anFirstSides[i] = -1;
			i--;
		}
		else if (!pfnCheck || pfnCheck(&tState, i))
			i++;
		ptRow = &ptTable[i];
	}
	i = 0;
	ptRow = ptTable;
	while (ptRow->nLevelId)
	{
		nLevelId = ptRow->nLevelId;
		nParent = ptRow->nParentRow == -1 ? 0 : ptTable[ptRow->nParentRow].nLevelId;
		nSecond = ptRow->nLinkRow == -1 ? 0 : ptTable[ptRow->nLinkRow].nLevelId;
		ptLevel = DrlgGetLevel(ptAct, nLevelId);
		ptLevel->tCoords = tState.atCoords[i];
		if (ptLevel->dwDrlgType == 2)
		{
			ptPreset = ptLevel->field_38.ptPreset;
			switch (ptLevel->nLevelId)
			{
			case 1:
				ptPreset->nFile = tState.anSides[i];
				break;
			case 40:
				ptPreset->nFile = tState.anSides[i + 1];
				break;
			}
		}
		if (ptLevel->nLevelId == 6)
		{
			ptPreset = DrlgGetLevel(ptAct, LEVEL_ACT_1_COURTYARD_1)->field_38.ptPreset;
			switch (tState.anSides[i])
			{
			case 1:
				ptPreset->nFile = (SeedNext(&ptAct->tSeed) & 1) ? 1 : 2;
				break;
			case 3:
				ptPreset->nFile = ~SeedNext(&ptAct->tSeed) & 1;
				break;
			}
		}
		if (pfnDone)
			pfnDone(ptLevel, i, tState.anSides);
		if (nParent)
		{
			ptVisInfo = DrlgActGetVisInfo(ptAct, nLevelId);
			ptOtherVis = DrlgActGetVisInfo(ptAct, nParent);
			DrlgVisInfoSetLevel(ptVisInfo, nParent, -1, -1);
			DrlgVisInfoSetLevel(ptOtherVis, nLevelId, -1, -1);
		}
		if (nSecond)
		{
			ptVisInfo = DrlgActGetVisInfo(ptAct, nLevelId);
			ptOtherVis = DrlgActGetVisInfo(ptAct, nSecond);
			DrlgVisInfoSetLevel(ptVisInfo, nSecond, -1, -1);
			DrlgVisInfoSetLevel(ptOtherVis, nLevelId, -1, -1);
		}
		i++;
		ptRow = &ptTable[i];
	}
}

static void __fastcall sLinkOutdoorLevels(DRLGACTCTX *ptAct, int nFirst, int nLast)
{
	DRLGLEVEL *ptLevel;
	DRLGLEVEL *ptOther;
	int *pnVis;
	int *pnWarp;
	OUTDOORINFO *ptOutdoor;
	int nDirection;
	int nLevelId;
	int i;

	for (nLevelId = nFirst; nLevelId <= nLast; nLevelId++)
	{
		ptLevel = DrlgGetLevel(ptAct, nLevelId);
		if (ptLevel->dwDrlgType != 3)
			continue;
		pnVis = DrlgRoomGetVis(ptAct, nLevelId);
		pnWarp = DrlgWarpGetList(ptAct, nLevelId);
		for (i = 0; i < LEVEL_VIS_MAX; i++)
		{
			if (pnVis[i] && pnWarp[i] == -1)
			{
				ptOutdoor = ptLevel->field_38.pOutdoor;
				ptOther = DrlgGetLevel(ptLevel->ptAct, pnVis[i]);
				nDirection = DrlgRectEdgeDir(&ptLevel->tCoords, &ptOther->tCoords);
				DrlgLevelLinkNew(&ptOutdoor->ptLevelLinks, ptOther, nDirection, ptOther->dwDrlgType == 2);
			}
		}
	}
}

#define JUNGLE_MAX_ATTACH	3
#define PRESET_DEF_NONE		0

struct JUNGLEINFO
{
 	DRLGCOORDS	tCoords;
 	int			nSide;
 	int			nBranch;
 	struct JUNGLEINFO	*ptBaseOn;
 	struct JUNGLEINFO	*aptBranches[JUNGLE_MAX_ATTACH];
 	int			nCellX;
 	int			nCellY;
 	int			*pnJungleDefs;
 	int			nNumClearings;
};
typedef struct JUNGLEINFO JUNGLEINFO;

static const int sgcanJungleSideDefs[16][4] =
{
	{ 0, 545, 546, 547 }, { 548, 0, 549, 550 }, { 0, 0, 551, 552 }, { 553, 554, 0, 555 },
	{ 0, 556, 0, 557 }, { 558, 0, 0, 559 }, { 0, 0, 0, 560 }, { 561, 562, 563, 0 },
	{ 0, 564, 565, 0 }, { 566, 0, 567, 0 }, { 0, 0, 568, 0 }, { 569, 570, 0, 0 },
	{ 0, 571, 0, 0 }, { 572, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 },
};

static const int sgcanJungleClearingDefs[15] =
{
	575, 576, 577, 578, 579, 580, 0, 581, 582, 583, 0, 584, 0, 0, 0,
};

static void __fastcall sPlaceJungle(DRLGCOORDS *ptFrom, JUNGLEINFO *ptJungle, int nSide, int nWidth,
	int nHeight);

static __inline int sJungleAbs(int nValue)
{
	return nValue < 0 ? -nValue : nValue;
}

static __inline BOOL sJungleSortsBefore(JUNGLEINFO *ptA, JUNGLEINFO *ptB)
{
	if (ptA->tCoords.nTileY > ptB->tCoords.nTileY)
		return TRUE;
	if (ptA->tCoords.nTileY == 0)
		return FALSE;
	return FALSE;
}

static __inline void sSwapJungles(JUNGLEINFO *ptA, JUNGLEINFO *ptB)
{
	JUNGLEINFO tSwap;

	tSwap = *ptA;
	*ptA = *ptB;
	*ptB = tSwap;
}

static __inline int sGridIndex(int nGridW, int x, int y)
{
	return y * nGridW + x;
}

static __inline int *sGridCell(int *pnGrid, int nGridW, int x, int y)
{
	return &pnGrid[sGridIndex(nGridW, x, y)];
}

static DRLGLEVEL * __fastcall sCarveKurast(DRLGLEVEL *ptLevelArg)
{
	DRLGLEVEL *ptLevel = ptLevelArg;
	DRLGACTCTX *ptAct;
	void *pPool;
	PTLEVELSREC ptLevelDef;
	DRLGLEVEL *ptJungleLevel;
	JUNGLEINFO ptJungle[3];
	JUNGLEINFO *ptNew;
	int nPreset;
	int nBaseOn;
	int nPDef;
	int nMinX, nMaxX, nMinY, nMaxY;
	int nSizeX, nSizeY;
	int nCellsX, nCellsY;
	int nCellX, nCellY;
	int nGridW, nGridH, nGridSize;
	int *pnOwner;
	int *pnOrder;
	int *pnMark;
	int *pnDirs;
	int nSide, nFlip, nRow, nCol;
	int nRun, nRuns, nOrder, nPick, nSeen;
	int nOwner, nBest, nRand, nCode;
	DWORD dwDirs;
	BOOL fDone, fSearch, fAgain, fWasZero;
	BOOL fSwapped;
	int nCell;
	int i, j, k;
	int x = 0, y = 0;

	nPreset = 32;
	ptLevelDef = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(76);
	i = 1;
	ptAct = ptLevel->ptAct;
	nSizeX = ptLevelDef->SizeX;
	pPool = ptAct->pMemPool;
	nSizeY = ptLevelDef->SizeY;
	nCellsX = nSizeX / nPreset;
	nCellsY = nSizeY / nPreset;
	nMinX = ptLevel->tCoords.nTileX;
	nMaxY = ptLevel->tCoords.nTileY;
	nMinY = nMaxY - nSizeY;
	nMaxX = nMinX + nSizeX;
	memset(ptJungle, 0, sizeof(ptJungle));
	sPlaceJungle(&ptLevel->tCoords, &ptJungle[0], 0, nSizeX, nSizeY);

	while (i < 3)
	{
		nBaseOn = i ? (int)SeedRand(&ptAct->tSeed, i) : 0;
		nSide = (int)SeedRand(&ptAct->tSeed, 5);
		ptNew = &ptJungle[i];
		sPlaceJungle(&ptJungle[nBaseOn].tCoords, ptNew, nSide, nSizeX, nSizeY);
		for (j = 0; j < i; j++)
		{
			if (!DrlgRectGapTest(&ptJungle[j].tCoords, &ptNew->tCoords, 0))
				break;
		}
		if (j < i)
			continue;
		ptJungle[nBaseOn].aptBranches[ptJungle[nBaseOn].nBranch] = ptNew;
		ptJungle[i].ptBaseOn = &ptJungle[nBaseOn];
		ptJungle[nBaseOn].nBranch++;
#line 1562
		D2WARN(ptJungle[nBaseOn].nBranch <= JUNGLE_MAX_ATTACH);
		if (nMinX > ptNew->tCoords.nTileX)
			nMinX = ptNew->tCoords.nTileX;
		if (nMinY > ptNew->tCoords.nTileY)
			nMinY = ptNew->tCoords.nTileY;
		if (nMaxX < ptNew->tCoords.nTileX + ptNew->tCoords.nSizeTileX)
			nMaxX = ptNew->tCoords.nTileX + ptNew->tCoords.nSizeTileX;
		i++;
	}
#line 1577
	D2WARN((nMaxX - nMinX) % nPreset == 0);
#line 1578
	D2WARN((nMaxY - nMinY) % nPreset == 0);

	nGridW = (nMaxX - nMinX) / nPreset + 2;
	nGridH = (nMaxY - nMinY) / nPreset + 2;
	nGridSize = nGridH * nGridW * (int)sizeof(int);
#line 1588
	pnOwner = (int *)D2ALLOCPOOL(pPool, nGridSize);
#line 1589
	pnOrder = (int *)D2ALLOCPOOL(pPool, nGridSize);
#line 1590
	pnMark = (int *)D2ALLOCPOOL(pPool, nGridSize);
#line 1591
	pnDirs = (int *)D2ALLOCPOOL(pPool, nGridSize);

restart:
	memset(pnOwner, 0, nGridSize);
	memset(pnOrder, 0, nGridSize);
	memset(pnMark, 0, nGridSize);
	memset(pnDirs, 0, nGridSize);

	for (j = 0; j < 3; j++)
	{
		nCellX = (ptJungle[j].tCoords.nTileX - nMinX) / nPreset + 1;
		nCellY = (ptJungle[j].tCoords.nTileY - nMinY) / nPreset + 1;
		ptJungle[j].nCellX = nCellX;
		ptJungle[j].nCellY = nCellY;
#line 1611
		ptJungle[j].pnJungleDefs = (int *)D2ALLOCPOOL(pPool, nCellsY * nCellsX * (int)sizeof(int));
		if (j == 0)
			nFlip = (int)(SeedNext(&ptAct->tSeed) & 1);
		else
			nFlip = ptJungle[j].nSide % 2 == 1;
		do
		{
			for (y = 0; y < nCellsY; y++)
			{
				for (x = 0; x < nCellsX; x++)
				{
					*sGridCell(pnOwner, nGridW, nCellX + x, nCellY + y) = j + 1;
					*sGridCell(pnOrder, nGridW, nCellX + x, nCellY + y) = 0;
					*sGridCell(pnMark, nGridW, nCellX + x, nCellY + y) = 0;
					*sGridCell(pnDirs, nGridW, nCellX + x, nCellY + y) = 0;
				}
			}
			x = nFlip + nCellX;
			y = nCellY + nCellsY - 1;
			if (j != 0)
			{
				*sGridCell(pnMark, nGridW, x, y) = 1;
				*sGridCell(pnMark, nGridW, x + (nFlip ? -1 : 1), y) = 2;
			}
			for (k = 0; k < ptJungle[j].nBranch; k++)
			{
				switch (ptJungle[j].aptBranches[k]->nSide)
				{
				case 0:
					*sGridCell(pnMark, nGridW, nCellX, nCellY) = 1;
					break;
				case 1:
					*sGridCell(pnMark, nGridW, nCellX, nCellY + 3) = 1;
					break;
				case 2:
					*sGridCell(pnMark, nGridW, nCellX + 1, nCellY + 3) = 1;
					break;
				case 3:
					*sGridCell(pnMark, nGridW, nCellX, nCellY + 1) = 1;
					break;
				case 4:
					*sGridCell(pnMark, nGridW, nCellX + 1, nCellY + 1) = 1;
					break;
				}
			}
			nRuns = j != 0;
			nOrder = 0;
			nRun = 0;
			while (y >= nCellY)
			{
				*sGridCell(pnOrder, nGridW, x, y) = (j + 1) * 100 + nOrder;
				nOrder++;
				if (nRun != 0 && y != nCellY && SeedRand(&ptAct->tSeed, 3) == 0)
				{
					nRun = 0;
					if (nFlip)
					{
						nFlip = 0;
						x--;
					}
					else
					{
						nFlip = 1;
						x++;
					}
				}
				else
				{
					nRun++;
					if (nRun >= 2 && y > 1)
					{
						if (*sGridCell(pnMark, nGridW, x + (nFlip ? -1 : 1), y) == 0)
						{
							*sGridCell(pnMark, nGridW, x + (nFlip ? -1 : 1), y) = 2;
							nRuns++;
						}
					}
					y--;
				}
			}
		} while (nRuns < 2);

		while (nRuns > 3)
		{
			nPick = nRuns ? (int)SeedRand(&ptAct->tSeed, nRuns) : 0;
			fDone = FALSE;
			nSeen = 0;
			for (y = 0; y < nCellsY && !fDone; y++)
			{
				for (x = 0; x < nCellsX && !fDone; x++)
				{
					if (*sGridCell(pnMark, nGridW, nCellX + x, nCellY + y) == 2)
					{
						if (nPick == nSeen)
						{
							*sGridCell(pnMark, nGridW, nCellX + x, nCellY + y) = 0;
							fDone = TRUE;
							nRuns--;
						}
						nSeen++;
					}
				}
			}
		}
	}

	for (y = 0; y < nGridH; y++)
	{
		for (x = 0; x < nGridW; x++)
		{
			nOwner = *sGridCell(pnOwner, nGridW, x, y);
			nBest = 2147483647;
			dwDirs = 0;
			if (*sGridCell(pnMark, nGridW, x, y) == 1)
			{
				if (*sGridCell(pnOrder, nGridW, x, y - 1) && *sGridCell(pnOrder, nGridW, x, y - 1) < nBest &&
					*sGridCell(pnOwner, nGridW, x, y - 1) == nOwner)
				{
					dwDirs = 8;
					nBest = *sGridCell(pnOrder, nGridW, x, y - 1);
				}
				if (*sGridCell(pnOrder, nGridW, x, y + 1) && *sGridCell(pnOrder, nGridW, x, y + 1) < nBest &&
					*sGridCell(pnOwner, nGridW, x, y + 1) == nOwner)
				{
					dwDirs = 4;
					nBest = *sGridCell(pnOrder, nGridW, x, y + 1);
				}
				if (*sGridCell(pnOrder, nGridW, x + 1, y) && *sGridCell(pnOrder, nGridW, x + 1, y) < nBest &&
					*sGridCell(pnOwner, nGridW, x + 1, y) == nOwner)
				{
					dwDirs = 2;
					nBest = *sGridCell(pnOrder, nGridW, x + 1, y);
				}
				if (*sGridCell(pnOrder, nGridW, x - 1, y) && *sGridCell(pnOrder, nGridW, x - 1, y) < nBest &&
					*sGridCell(pnOwner, nGridW, x - 1, y) == nOwner)
				{
					dwDirs = 1;
				}
				if (dwDirs & 8)
					*sGridCell(pnDirs, nGridW, x, y - 1) |= 4;
				if (dwDirs & 4)
					*sGridCell(pnDirs, nGridW, x, y + 1) |= 8;
				if (dwDirs & 2)
					*sGridCell(pnDirs, nGridW, x + 1, y) |= 1;
				if (dwDirs & 1)
					*sGridCell(pnDirs, nGridW, x - 1, y) |= 2;
				if (*sGridCell(pnMark, nGridW, x, y - 1) == 1 && *sGridCell(pnOwner, nGridW, x, y - 1) != nOwner)
					dwDirs |= 8;
				if (*sGridCell(pnMark, nGridW, x, y + 1) == 1 && *sGridCell(pnOwner, nGridW, x, y + 1) != nOwner)
					dwDirs |= 4;
				if (*sGridCell(pnMark, nGridW, x + 1, y) == 1 && *sGridCell(pnOwner, nGridW, x + 1, y) != nOwner)
					dwDirs |= 2;
				if (*sGridCell(pnMark, nGridW, x - 1, y) == 1 && *sGridCell(pnOwner, nGridW, x - 1, y) != nOwner)
					dwDirs |= 1;
			}
			nCell = *sGridCell(pnOrder, nGridW, x, y);
			if (nCell)
			{
				if (sJungleAbs(nCell - *sGridCell(pnOrder, nGridW, x, y - 1)) == 1)
					dwDirs |= 8;
				if (sJungleAbs(nCell - *sGridCell(pnOrder, nGridW, x, y + 1)) == 1)
					dwDirs |= 4;
				if (sJungleAbs(nCell - *sGridCell(pnOrder, nGridW, x + 1, y)) == 1)
					dwDirs |= 2;
				if (sJungleAbs(nCell - *sGridCell(pnOrder, nGridW, x - 1, y)) == 1)
					dwDirs |= 1;
			}
			*sGridCell(pnDirs, nGridW, x, y) |= dwDirs;
		}
	}

	for (y = 0; y < nGridH; y++)
	{
		for (x = 0; x < nGridW; x++)
		{
			dwDirs = (DWORD)*sGridCell(pnDirs, nGridW, x, y);
			nOwner = *sGridCell(pnOwner, nGridW, x, y);
			nRand = (int)(SeedNext(&ptAct->tSeed) & 3);
			fWasZero = dwDirs == 0;
			if (*sGridCell(pnMark, nGridW, x, y) == 2)
			{
				k = 0;
				fSearch = TRUE;
				for (; k < 4 && fSearch; k++)
				{
					switch ((nRand + k) % 4)
					{
					case 0:
						if (*sGridCell(pnOwner, nGridW, x, y - 1) == nOwner && *sGridCell(pnDirs, nGridW, x, y - 1) &&
							*sGridCell(pnDirs, nGridW, x, y - 1) < 15)
						{
							dwDirs |= 128;
							fSearch = FALSE;
						}
						break;
					case 1:
						if (*sGridCell(pnOwner, nGridW, x, y + 1) == nOwner && *sGridCell(pnDirs, nGridW, x, y + 1) &&
							*sGridCell(pnDirs, nGridW, x, y + 1) < 15)
						{
							dwDirs |= 64;
							fSearch = FALSE;
						}
						break;
					case 2:
						if (*sGridCell(pnOwner, nGridW, x + 1, y) == nOwner &&
							*sGridCell(pnDirs, nGridW, x + 1, y) && *sGridCell(pnDirs, nGridW, x + 1, y) < 15)
						{
							dwDirs |= 32;
							fSearch = FALSE;
						}
						break;
					case 3:
						if (*sGridCell(pnOwner, nGridW, x - 1, y) == nOwner &&
							*sGridCell(pnDirs, nGridW, x - 1, y) && *sGridCell(pnDirs, nGridW, x - 1, y) < 15)
						{
							dwDirs |= 16;
							fSearch = FALSE;
						}
						break;
					}
				}
				if (!dwDirs)
					goto restart;
				if (fWasZero)
				{
					k = 0;
					fSearch = TRUE;
					for (; k < 4 && fSearch; k++)
					{
						switch ((nRand + k) % 4)
						{
						case 0:
							if (*sGridCell(pnOwner, nGridW, x, y - 1) != nOwner && *sGridCell(pnMark, nGridW, x, y - 1) == 2)
							{
								dwDirs |= 128;
								fSearch = FALSE;
							}
							break;
						case 1:
							if (*sGridCell(pnOwner, nGridW, x, y + 1) != nOwner && *sGridCell(pnMark, nGridW, x, y + 1) == 2)
							{
								dwDirs |= 64;
								fSearch = FALSE;
							}
							break;
						case 2:
							if (*sGridCell(pnOwner, nGridW, x + 1, y) != nOwner && *sGridCell(pnMark, nGridW, x + 1, y) == 2)
							{
								dwDirs |= 32;
								fSearch = FALSE;
							}
							break;
						case 3:
							if (*sGridCell(pnOwner, nGridW, x - 1, y) != nOwner && *sGridCell(pnMark, nGridW, x - 1, y) == 2)
							{
								dwDirs |= 16;
								fSearch = FALSE;
							}
							break;
						}
					}
					fAgain = (SeedNext(&ptAct->tSeed) & 1) && fSearch;
					nRand = (int)(SeedNext(&ptAct->tSeed) & 3);
					for (k = 0; k < 4 && fAgain; k++)
					{
						switch ((nRand + k) % 4)
						{
						case 0:
							if (*sGridCell(pnDirs, nGridW, x, y - 1) && *sGridCell(pnDirs, nGridW, x, y - 1) < 15 && !(dwDirs & 128))
							{
								dwDirs |= 128;
								fAgain = FALSE;
							}
							break;
						case 1:
							if (*sGridCell(pnDirs, nGridW, x, y + 1) && *sGridCell(pnDirs, nGridW, x, y + 1) < 15 && !(dwDirs & 64))
							{
								dwDirs |= 64;
								fAgain = FALSE;
							}
							break;
						case 2:
							if (*sGridCell(pnDirs, nGridW, x + 1, y) && *sGridCell(pnDirs, nGridW, x + 1, y) < 15 && !(dwDirs & 32))
							{
								dwDirs |= 32;
								fAgain = FALSE;
							}
							break;
						case 3:
							if (*sGridCell(pnDirs, nGridW, x - 1, y) && *sGridCell(pnDirs, nGridW, x - 1, y) < 15 && !(dwDirs & 16))
							{
								dwDirs |= 16;
								fAgain = FALSE;
							}
							break;
						}
					}
				}
				if (dwDirs & 128)
					*sGridCell(pnDirs, nGridW, x, y - 1) |= 64;
				if (dwDirs & 64)
					*sGridCell(pnDirs, nGridW, x, y + 1) |= 128;
				if (dwDirs & 32)
					*sGridCell(pnDirs, nGridW, x + 1, y) |= 16;
				if (dwDirs & 16)
					*sGridCell(pnDirs, nGridW, x - 1, y) |= 32;
				*sGridCell(pnMark, nGridW, x, y) = 0;
			}
			*sGridCell(pnDirs, nGridW, x, y) |= dwDirs;
		}
	}

	for (y = 0; y < nGridH; y++)
	{
		for (x = 0; x < nGridW; x++)
		{
			nCode = *sGridCell(pnDirs, nGridW, x, y);
			nPDef = nCode % 16;
			if (nPDef)
			{
				if (nCode >= 16)
				{
					if (nCode & 16)
						nPDef = sgcanJungleSideDefs[nPDef - 1][0];
					if (nCode & 32)
						nPDef = sgcanJungleSideDefs[nPDef - 1][1];
					if (nCode & 64)
						nPDef = sgcanJungleSideDefs[nPDef - 1][2];
					if (nCode & 128)
						nPDef = sgcanJungleSideDefs[nPDef - 1][3];
#line 1896
					D2WARN(nPDef != PRESET_DEF_NONE);
				}
				else
					nPDef += 529;
				*sGridCell(pnDirs, nGridW, x, y) = nPDef;
			}
			else
			{
				if (nCode >= 16)
				{
					nPDef = sgcanJungleClearingDefs[(nCode >> 4) - 1];
#line 1909
					D2WARN(nPDef != PRESET_DEF_NONE);
				}
				*sGridCell(pnDirs, nGridW, x, y) = nPDef;
			}
		}
	}

	for (j = 0; j < 3; j++)
	{
		k = 0;
		for (y = 0; y < nCellsY; y++)
		{
			for (x = 0; x < nCellsX; x++)
			{
				ptJungle[j].pnJungleDefs[k] = *sGridCell(pnDirs, nGridW, ptJungle[j].nCellX + x, ptJungle[j].nCellY + y);
				if (ptJungle[j].pnJungleDefs[k] > 574)
					ptJungle[j].nNumClearings++;
				k++;
			}
		}
	}

	do
	{
		fSwapped = FALSE;
		for (j = 0; j < 2; j++)
		{
			if (sJungleSortsBefore(&ptJungle[j + 1], &ptJungle[j]))
			{
				sSwapJungles(&ptJungle[j + 1], &ptJungle[j]);
				fSwapped = TRUE;
			}
		}
	} while (fSwapped);

	ptJungleLevel = NULL;
	for (j = 0; j < 3; j++)
	{
		ptJungleLevel = DrlgGetLevel(ptAct, 76 + j);
		ptJungleLevel->pnJungleDefs = ptJungle[j].pnJungleDefs;
		ptJungleLevel->nNumClearings = ptJungle[j].nNumClearings;
		ptJungleLevel->tCoords = ptJungle[j].tCoords;
	}
#line 1966
	D2FREEPOOL(pPool, pnOwner);
#line 1967
	D2FREEPOOL(pPool, pnOrder);
#line 1968
	D2FREEPOOL(pPool, pnMark);
#line 1969
	D2FREEPOOL(pPool, pnDirs);
	return ptJungleLevel;
}

static void __fastcall sPlaceJungle(DRLGCOORDS *ptFrom, JUNGLEINFO *ptJungle, int nSide, int nWidth,
	int nHeight)
{
	int nX;
	int nY;

	nX = 0;
	nY = 0;
	switch (nSide)
	{
	case 0:
		nX = 0;
		nY = -nHeight;
		break;
	case 1:
		nX = -nWidth;
		nY = nHeight / -3;
		break;
	case 2:
		nX = nWidth;
		nY = nHeight / -3;
		break;
	case 3:
		nX = -nWidth;
		nY = -nHeight * 2 / 3;
		break;
	case 4:
		nX = nWidth;
		nY = -nHeight * 2 / 3;
		break;
	}
	ptJungle->tCoords.nTileX = ptFrom->nTileX + nX;
	ptJungle->tCoords.nTileY = ptFrom->nTileY + nY;
	ptJungle->tCoords.nSizeTileX = nWidth;
	ptJungle->tCoords.nSizeTileY = nHeight;
	ptJungle->nSide = nSide;
}
