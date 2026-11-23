#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutJung.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

static __inline int sDrawBelow(D2SEED *ptSeed, int nRange)
{
	return nRange ? (int)SeedRand(ptSeed, nRange) : 0;
}

void __fastcall sPlaceJungleRows(DRLGLEVEL *ptDrlgLevel);
void __fastcall sBorderLowerKurast(DRLGLEVEL *ptLevel);
void __fastcall sBorderKurastBazaar(DRLGLEVEL *ptLevel);
void __fastcall sBorderUpperKurast(DRLGLEVEL *ptLevel);
void __fastcall sPlaceDefRange(DRLGLEVEL *ptLevel, int nFirst, int nLast,
	int nMax);
void __fastcall sLayoutKurast(DRLGLEVEL *ptLevel);
void __fastcall sStampTravincal(DRLGLEVEL *ptLevel);

static const int sgcnJungleDefOffsets[3] =
{
	0, 10, 20,
};
static const int sgcnJungleFiles[6][3] =
{
	{ 0, 1, 2 }, { 1, 0, 2 }, { 0, 2, 1 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 },
};

void __fastcall sPlaceJungleRows(DRLGLEVEL *ptDrlgLevel)
{
	PTLEVELSREC ptRecord;
	int nFileIndex;
	int nChoice;
	int nWidth;
	int nHeight;
	int nDef;
	int nFile;
	int nIndex;
	int x;
	int y;

	if ((int)ptDrlgLevel->nLevelId < 76 || (int)ptDrlgLevel->nLevelId > 78)
		return;
	ptRecord = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(76);
	nWidth = ptRecord->SizeX / 32;
	nHeight = ptRecord->SizeY / 32;
	nIndex = 0;
	nChoice = (int)SeedRand(&ptDrlgLevel->tSeed, ptDrlgLevel->nNumClearings != 3 ? 2 : 6);
	nFileIndex = 0;
#line 35
	D2WARN(ptDrlgLevel->pnJungleDefs);
	for (y = 0; y < nHeight; y++)
	{
		if (ptDrlgLevel->nLevelId == 76 && y == nHeight - 1)
		{
			OutdoorsStampPreset(ptDrlgLevel, 0, y * 4, 573,
				ptDrlgLevel->pnJungleDefs[nHeight * nWidth - 1] == 0, FALSE);
			nIndex += 2;
		}
		else if (ptDrlgLevel->nLevelId == 78 && y == 0)
		{
			OutdoorsStampPreset(ptDrlgLevel, 0, 0, 574, ptDrlgLevel->pnJungleDefs[1] == 0, FALSE);
			nIndex += 2;
		}
		else
		{
			for (x = 0; x < nWidth; x++)
			{
				nDef = ptDrlgLevel->pnJungleDefs[nIndex++];
				nFile = -1;
				if (nDef > 574)
				{
#line 67
					D2WARN(nFileIndex < 3);
					nDef += sgcnJungleDefOffsets[ptDrlgLevel->nLevelId - 76];
					nFile = sgcnJungleFiles[nChoice][nFileIndex++];
				}
				if (nDef)
					OutdoorsStampPreset(ptDrlgLevel, x * 4, y * 4, nDef, nFile, FALSE);
			}
		}
	}
}

void __fastcall sBorderLowerKurast(DRLGLEVEL *ptLevel)
{
	int nWidth;
	int nHeight;
	int nMiddle;
	int nGate;
	int i;

	nWidth = ptLevel->tCoords.nSizeTileX / 8 - 1;
	nHeight = ptLevel->tCoords.nSizeTileY / 8 - 1;
	nMiddle = nWidth / 2;
	if (ptLevel->ptAct->dwKurastBorderSide)
		nGate = 1;
	else
		nGate = nWidth - 1;
	for (i = 1; i < nWidth; i++)
		OutdoorsStampPreset(ptLevel, i, 0, i != nGate ? 605 : 613, -1, FALSE);
	for (i = 1; i < nWidth; i += (i == nMiddle) + 1)
		OutdoorsStampPreset(ptLevel, i, nHeight, i != nMiddle ? 606 : 614, -1, FALSE);
	for (i = 1; i < nHeight; i++)
	{
		OutdoorsStampPreset(ptLevel, nWidth, i, 607, -1, FALSE);
		OutdoorsStampPreset(ptLevel, 0, i, 608, -1, FALSE);
	}
	OutdoorsStampPreset(ptLevel, 0, 0, 610, -1, FALSE);
	OutdoorsStampPreset(ptLevel, nWidth, 0, 609, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 0, nHeight, 612, -1, FALSE);
	OutdoorsStampPreset(ptLevel, nWidth, nHeight, 611, -1, FALSE);
}

void __fastcall sBorderKurastBazaar(DRLGLEVEL *ptLevel)
{
	int nWidth;
	int nHeight;
	int nTop;
	int nBottom;
	int i;

	nWidth = ptLevel->tCoords.nSizeTileX / 8 - 1;
	nHeight = ptLevel->tCoords.nSizeTileY / 8 - 1;
	if (ptLevel->ptAct->dwKurastBorderSide)
		nBottom = 1;
	else
		nBottom = nWidth - 1;
	if (ptLevel->ptAct->dwKurastBorderSide)
		nTop = nWidth - 1;
	else
		nTop = 1;
	for (i = 1; i < nWidth; i++)
	{
		OutdoorsStampPreset(ptLevel, i, 0, i != nTop ? 619 : 627, -1, FALSE);
		OutdoorsStampPreset(ptLevel, i, nHeight, i != nBottom ? 620 : 628, -1, FALSE);
	}
	for (i = 1; i < nHeight; i++)
	{
		OutdoorsStampPreset(ptLevel, nWidth, i, 621, -1, FALSE);
		OutdoorsStampPreset(ptLevel, 0, i, 622, -1, FALSE);
	}
	OutdoorsStampPreset(ptLevel, 0, 0, 624, -1, FALSE);
	OutdoorsStampPreset(ptLevel, nWidth, 0, 623, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 0, nHeight, 626, -1, FALSE);
	OutdoorsStampPreset(ptLevel, nWidth, nHeight, 625, -1, FALSE);
}

void __fastcall sBorderUpperKurast(DRLGLEVEL *ptLevel)
{
	int nWidth;
	int nHeight;
	int nMiddle;
	int nGate;
	int i;

	nWidth = ptLevel->tCoords.nSizeTileX / 8 - 1;
	nHeight = ptLevel->tCoords.nSizeTileY / 8 - 1;
	if (ptLevel->ptAct->dwKurastBorderSide)
		nGate = nWidth - 1;
	else
		nGate = 1;
	nMiddle = nWidth / 2;
	for (i = 1; i < nWidth; i += (i == nMiddle) + 1)
		OutdoorsStampPreset(ptLevel, i, 0, i != nMiddle ? 636 : 644, -1, FALSE);
	for (i = 1; i < nWidth; i++)
		OutdoorsStampPreset(ptLevel, i, nHeight, i != nGate ? 637 : 645, -1, FALSE);
	for (i = 1; i < nHeight; i++)
	{
		OutdoorsStampPreset(ptLevel, nWidth, i, 638, -1, FALSE);
		OutdoorsStampPreset(ptLevel, 0, i, 639, -1, FALSE);
	}
	OutdoorsStampPreset(ptLevel, 0, 0, 641, -1, FALSE);
	OutdoorsStampPreset(ptLevel, nWidth, 0, 640, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 0, nHeight, 643, -1, FALSE);
	OutdoorsStampPreset(ptLevel, nWidth, nHeight, 642, -1, FALSE);
}

void __fastcall sPlaceDefRange(DRLGLEVEL *ptLevel, int nFirst, int nLast, int nMax)
{
	struct DEFCELL { int x; int y; } *ptCells, tSwap;
	void *pPool;
	int nDefs;
	int nWidth;
	int nCells;
	int nPlaced;
	int nDef;
	int nA;
	int nB;
	int nX;
	int nY;
	int i;

	nDefs = nLast - nFirst + 1;
	pPool = ptLevel->ptAct->pMemPool;
	nPlaced = 0;
	nWidth = ptLevel->field_38.pOutdoor->tCellRect.nSizeTileX;
	nCells = ptLevel->field_38.pOutdoor->tCellRect.nSizeTileY * nWidth;
	if (!nCells)
		return;
#line 199
	ptCells = (struct DEFCELL *)D2ALLOCPOOL(pPool, nCells * 8);
	for (i = 0; i < nCells; i++)
	{
		ptCells[i].x = i % nWidth;
		ptCells[i].y = i / nWidth;
	}
	for (i = nCells; i > 0; i--)
	{
		nA = sDrawBelow(&ptLevel->tSeed, nCells);
		nB = sDrawBelow(&ptLevel->tSeed, nCells);
		tSwap = ptCells[nA];
		ptCells[nA] = ptCells[nB];
		ptCells[nB] = tSwap;
	}
	for (i = 0; i < nCells; i++)
	{
		nX = ptCells[i].x;
		nY = ptCells[i].y;
		nDef = sDrawBelow(&ptLevel->tSeed, nDefs) + nFirst;
		if (!OutdoorsTryPlacePreset(ptLevel, nX, nY, nDef, 0, 15))
			continue;
		OutdoorsStampPreset(ptLevel, nX, nY, nDef, -1, FALSE);
		nPlaced++;
		if (nMax > 0 && nPlaced >= nMax)
		{
#line 231
			D2FREEPOOL(pPool, ptCells);
			return;
		}
	}
#line 238
	D2FREEPOOL(pPool, ptCells);
}

void __fastcall sLayoutKurast(DRLGLEVEL *ptLevel)
{
	int nX;
	int nY;

	switch (ptLevel->nLevelId)
	{
	case 79:
		sBorderLowerKurast(ptLevel);
		break;
	case 80:
		sBorderKurastBazaar(ptLevel);
		break;
	case 81:
		sBorderUpperKurast(ptLevel);
		break;
	}
	nX = ptLevel->tCoords.nSizeTileX / 8 - 4;
	nY = ptLevel->tCoords.nSizeTileY / 8 - 4;
	switch (ptLevel->nLevelId)
	{
	case 79:
		OutdoorsPlacePreset(ptLevel, 631, 0, 0, 15);
		sPlaceDefRange(ptLevel, 618, 618, 4);
		sPlaceDefRange(ptLevel, 616, 617, 0);
		sPlaceDefRange(ptLevel, 615, 615, 0);
		break;
	case 80:
		OutdoorsStampPreset(ptLevel, 3, 3, 629, 0, FALSE);
		OutdoorsStampPreset(ptLevel, nX, 3, 629, 1, FALSE);
		OutdoorsPlacePreset(ptLevel, 630, 0, 0, 15);
		OutdoorsPlacePreset(ptLevel, 630, 1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 631, 0, 0, 15);
		sPlaceDefRange(ptLevel, 635, 635, 4);
		sPlaceDefRange(ptLevel, 633, 634, 0);
		sPlaceDefRange(ptLevel, 632, 632, 0);
		break;
	case 81:
		OutdoorsStampPreset(ptLevel, 3, nY, 646, 0, FALSE);
		OutdoorsStampPreset(ptLevel, nX, nY, 646, 1, FALSE);
		OutdoorsPlacePreset(ptLevel, 647, 0, 0, 15);
		OutdoorsPlacePreset(ptLevel, 647, 1, 0, 15);
		OutdoorsPlacePreset(ptLevel, 631, 0, 0, 15);
		sPlaceDefRange(ptLevel, 651, 651, 4);
		sPlaceDefRange(ptLevel, 649, 650, 0);
		sPlaceDefRange(ptLevel, 648, 648, 0);
		break;
	case 82:
		OutdoorsStampPreset(ptLevel, 0, 0, 652, 0, FALSE);
		break;
	}
}

void __fastcall sStampTravincal(DRLGLEVEL *ptLevel)
{
	if (ptLevel->nLevelId != 83)
		return;
	OutdoorsStampPreset(ptLevel, 0, 0, 653, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 2, 0, 654, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 6, 0, 655, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 0, 4, 656, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 2, 4, 657, -1, FALSE);
	OutdoorsStampPreset(ptLevel, 6, 4, 658, -1, FALSE);
}

void __fastcall OutJungGenerate(DRLGLEVEL *ptLevel)
{
	OutdoorsMarkOutline(ptLevel);
	sPlaceJungleRows(ptLevel);
	sLayoutKurast(ptLevel);
	sStampTravincal(ptLevel);
}
