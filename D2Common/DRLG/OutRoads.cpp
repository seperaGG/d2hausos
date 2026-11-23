#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutRoads.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

static const BYTE scabRoadPieces[256] =
{
	 0,  0, 16, 16,  0,  0, 16, 16, 14, 14,  6, 19, 14, 14,  6, 19,
	15, 15,  5,  5, 15, 15, 21, 21,  8,  8, 10, 38,  8,  8, 40, 20,
	 0,  0, 16, 16,  0,  0, 16, 16, 14, 14,  6, 19, 14, 14,  6, 19,
	15, 15,  5,  5, 15, 15, 21, 21,  8,  8, 10, 38,  8,  8, 40, 20,
	13, 13,  7,  7, 13, 13, 13,  7,  4,  4, 11, 37,  4,  4, 11, 43,
	 3,  3, 12, 12,  3,  3, 39, 39,  9,  9,  2, 43,  9,  9, 44, 26,
	13, 13,  7,  7, 13, 13, 13,  7, 23, 23, 41, 17, 23, 23, 41, 17,
	 3,  3, 12, 12,  3,  3, 39, 39, 42, 42, 46, 42, 42, 42, 33, 31,
	 0,  0, 16, 16,  0,  0, 16, 16, 14, 14,  6, 19, 14, 14,  6, 19,
	15, 15,  5,  5, 15, 15, 21, 21,  8,  8, 10, 38,  8,  8, 35, 20,
	 0,  0, 16, 16,  0,  0, 16, 16, 14, 14,  6, 19, 14, 14,  6, 19,
	15, 15,  5,  5, 15, 15, 21, 21,  8,  8, 10, 38,  8,  8, 40, 20,
	13, 13,  7,  7, 13, 13, 13,  7,  4,  4, 11, 37,  4,  4, 11, 37,
	18, 18, 35, 35, 18, 18, 22, 22, 36, 36, 45, 34, 36, 36, 28, 29,
	13, 13,  7,  7, 13, 13, 13,  7, 23, 23, 41, 17, 23, 23, 41, 17,
	18, 18, 35, 35, 18, 18, 22, 22, 24, 24, 25, 32, 24, 24, 30,  1,
};

void __fastcall OutdoorsMarkRoomPaths(DRLGLEVEL *ptLevel, ROOM *ptRoom)
{
	PTOUTDOORINFO ptOutdoor;
	PTOUTROOMINFO ptInfo;
	DRLGVERTEX *ptVertex;
	DRLGCOORDS tRect;
	int anWindow[9];
	int nCode;
	BYTE bPiece;
	int nX;
	int nY;
	int i;
	int nTop;
	DRLGCOORDS *ptCoords;

	DrlgGridAlloc(ptLevel->ptAct->pMemPool, &ptRoom->field_20.ptOutdoor->tPaths,
		ptRoom->tCoords.nSizeTileX + 3, ptRoom->tCoords.nSizeTileY + 3);
	ptOutdoor = ptLevel->field_38.pOutdoor;
	ptCoords = &ptRoom->tCoords;
	ptInfo = ptRoom->field_20.ptOutdoor;
	tRect = *ptCoords;
	tRect.nTileX--;
	tRect.nTileY--;
	tRect.nSizeTileX += 3;
	tRect.nSizeTileY += 3;
	for (i = 0; i < ptOutdoor->nEntries; i++)
	{
		for (ptVertex = ptOutdoor->aptEntryPaths[i]; ptVertex; ptVertex = ptVertex->ptNext)
		{
			if (ptVertex->ptNext)
				DrlgGridApplyThickLine(&ptInfo->tPaths, ptVertex, &tRect, 1, 0, 2);
		}
	}
	ptInfo = ptRoom->field_20.ptOutdoor;
	for (nX = 1; nX <= ptCoords->nSizeTileX + 1; nX++)
	{
		nY = ptCoords->nSizeTileY + 1;
		anWindow[2] = DrlgGridGet(&ptInfo->tPaths, nX - 1, nY - 1);
		anWindow[1] = DrlgGridGet(&ptInfo->tPaths, nX - 1, nY);
		anWindow[0] = DrlgGridGet(&ptInfo->tPaths, nX - 1, nY + 1);
		anWindow[5] = DrlgGridGet(&ptInfo->tPaths, nX, nY - 1);
		anWindow[8] = DrlgGridGet(&ptInfo->tPaths, nX + 1, nY - 1);
		anWindow[4] = DrlgGridGet(&ptInfo->tPaths, nX, nY);
		anWindow[7] = DrlgGridGet(&ptInfo->tPaths, nX + 1, nY);
		anWindow[3] = DrlgGridGet(&ptInfo->tPaths, nX, nY + 1);
		anWindow[6] = DrlgGridGet(&ptInfo->tPaths, nX + 1, nY + 1);
		for (; nY >= 1; nY--)
		{
			if (anWindow[4])
			{
				nCode = 0;
				for (i = 8; i >= 0; i--)
				{
					if (i == 4)
						continue;
					nCode *= 2;
					if (anWindow[i])
						nCode++;
				}
				if (nCode)
				{
					bPiece = scabRoadPieces[nCode];
					if (bPiece)
						DrlgGridApply(&ptInfo->tFloors, nX - 1, nY - 1, (bPiece << 8) | 130, 3);
				}
			}
			if (nY > 1)
			{
				nTop = nY - 1;
				for (i = 0; i < 8; i++)
					anWindow[i] = anWindow[i + 1];
				anWindow[2] = DrlgGridGet(&ptInfo->tPaths, nX - 1, nTop - 1);
				anWindow[5] = DrlgGridGet(&ptInfo->tPaths, nX, nTop - 1);
				anWindow[8] = DrlgGridGet(&ptInfo->tPaths, nX + 1, nTop - 1);
			}
		}
	}
}

static void __fastcall sEntryToEdge(DRLGLEVEL *ptLevel, OUTDOORENTRY *ptFrom,
	OUTDOORENTRY *ptTo);
static void __fastcall sPlaceMeetingPoints(DRLGLEVEL *ptLevel);
static void __fastcall sBuildEntryPath(DRLGLEVEL *ptLevel, int nEntry);

void __fastcall OutdoorsBuildEntries(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	DRLGLEVELLINK *ptLink;
	DRLGLEVEL *ptOther;
	OUTDOORENTRY *ptEntry;
	int nDef;
	int nFile;
	int x;
	int y;
	int i;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	ptOutdoor->nEntries = 0;
	for (ptLink = ptOutdoor->ptLevelLinks; ptLink; ptLink = ptLink->ptNext)
	{
		ptOther = ptLink->ptLevel;
		switch (ptOther->nLevelId)
		{
		case 1:
			ptEntry = &ptOutdoor->atEntries[ptOutdoor->nEntries];
			ptEntry->bDirection = (BYTE)ptLink->nDirection;
			switch (ptLink->nDirection)
			{
			case 0:
				ptEntry->nX = ptOther->tCoords.nTileX + 59;
				ptEntry->nY = ptOther->tCoords.nTileY + 19;
				break;
			case 1:
				ptEntry->nX = ptOther->tCoords.nTileX + 29;
				ptEntry->nY = ptOther->tCoords.nTileY + 35;
				break;
			case 2:
				ptEntry->nX = ptOther->tCoords.nTileX + 4;
				ptEntry->nY = ptOther->tCoords.nTileY + 22;
				break;
			case 3:
				ptEntry->nX = ptOther->tCoords.nTileX + 29;
				ptEntry->nY = ptOther->tCoords.nTileY + 3;
				break;
			}
			ptOutdoor->nEntries++;
			break;
		case 26:
			ptEntry = &ptOutdoor->atEntries[ptOutdoor->nEntries];
			ptEntry->nX = ptOther->tCoords.nTileX + 27;
			ptEntry->nY = ptOther->tCoords.nTileY + 13;
			ptEntry->bDirection = 1;
			ptOutdoor->nEntries++;
			break;
		}
	}
	for (x = 0; x < ptOutdoor->tCellRect.nSizeTileX; x++)
	{
		for (y = 0; y < ptOutdoor->tCellRect.nSizeTileY; y++)
		{
			nDef = DrlgGridGet(&ptOutdoor->tDefGrid, x, y);
			nFile = ((DWORD)DrlgGridGet(&ptOutdoor->tCellFlagGrid, x, y) >> 16) & 15;
			ptEntry = &ptOutdoor->atEntries[ptOutdoor->nEntries];
			ptEntry->nX = ptLevel->tCoords.nTileX + x * 8 + 3;
			ptEntry->nY = ptLevel->tCoords.nTileY + y * 8 + 3;
			ptEntry->bDirection = 4;
			switch (nDef)
			{
			case 4:
				if (nFile == 3)
					ptEntry->bDirection = 3;
				break;
			case 5:
				if (nFile == 3)
					ptEntry->bDirection = 0;
				break;
			case 6:
				if (nFile == 3)
					ptEntry->bDirection = 1;
				break;
			case 7:
				if (nFile == 3)
					ptEntry->bDirection = 2;
				break;
			case 24:
				ptEntry->bDirection = 1;
				break;
			case 25:
				ptEntry->bDirection = 0;
				break;
			case 51:
				ptEntry->bDirection = nFile != 0;
				break;
			case 52:
				ptEntry->bDirection = nFile != 0;
				break;
			case 28:
				if (nFile == 1 && x == ptOutdoor->tCellRect.nSizeTileX - 2)
					ptEntry->bDirection = 2;
				break;
			}
			if (ptEntry->bDirection != 4)
				ptOutdoor->nEntries++;
		}
	}
	for (i = 0; i < ptOutdoor->nEntries; i++)
		sEntryToEdge(ptLevel, &ptOutdoor->atEntries[i], &ptOutdoor->atEntryEdges[i]);
	sPlaceMeetingPoints(ptLevel);
	if (0)
		sPlaceMeetingPoints(ptLevel);
	for (i = 0; i < ptOutdoor->nEntries; i++)
	{
		if (OutdoorsFindEntryPath(ptLevel, i))
		{
			DrlgGridOrPoints(&ptOutdoor->tCellFlagGrid, ptOutdoor->aptEntryPaths[i], 128);
			sBuildEntryPath(ptLevel, i);
			if (0)
				sBuildEntryPath(ptLevel, i);
		}
	}
}

static void __fastcall sEntryToEdge(DRLGLEVEL *ptLevel, OUTDOORENTRY *ptFrom, OUTDOORENTRY *ptTo)
{
	ptTo->nX = ptFrom->nX - ptLevel->tCoords.nTileX;
	ptTo->nY = ptFrom->nY - ptLevel->tCoords.nTileY;
	switch (ptFrom->bDirection)
	{
	case 0:
		ptTo->nX = ptTo->nX / 8 * 8 + 11;
		break;
	case 1:
		ptTo->nY = ptTo->nY / 8 * 8 + 11;
		break;
	case 2:
		ptTo->nX = ptTo->nX / 8 * 8 - 5;
		break;
	case 3:
		ptTo->nY = ptTo->nY / 8 * 8 - 5;
		break;
	}
	ptTo->nX += ptLevel->tCoords.nTileX;
	ptTo->nY += ptLevel->tCoords.nTileY;
}

static const int sgcanMeetX[4] = { -1, 0, 0, 1 };
static const int sgcanMeetY[4] = { 0, 1, -1, 0 };

static void __fastcall sPlaceMeetingPoints(DRLGLEVEL *ptLevel)
{
	OUTDOORINFO *ptOutdoor;
	BOOL fFound;
	int nX;
	int nY;
	int nCentreX;
	int nCentreY;
	int nSumX;
	int nSumY;
	int nTileX;
	int nTileY;
	int nRadius;
	int i;
	int k;

	nY = -1;
	nX = -1;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	if (ptOutdoor->dwFlags & 16)
		OutRoomFindTownEdge(ptLevel, &nX, &nY);
	if (nX != -1)
	{
		nTileX = ptLevel->tCoords.nTileX + nX * 8 + 3;
		nTileY = ptLevel->tCoords.nTileY + nY * 8 + 3;
		for (i = 0; i < ptOutdoor->nEntries; i++)
		{
			if (ptOutdoor->atEntries[i].nX > nTileX)
			{
				ptOutdoor->atMeetingPoints[i].nX = nTileX + 8;
				ptOutdoor->atMeetingPoints[i].nY = nTileY;
				ptOutdoor->atMeetingPoints[i].bDirection = 0;
			}
			else
			{
				ptOutdoor->atMeetingPoints[i].nX = nTileX;
				ptOutdoor->atMeetingPoints[i].nY = nTileY;
				ptOutdoor->atMeetingPoints[i].bDirection = 2;
			}
		}
	}
	else
	{
		for (i = 0; i < ptOutdoor->nEntries; i++)
		{
			if (i == 0)
			{
				nSumX = 0;
				nSumY = 0;
				nRadius = 0;
				if (ptOutdoor->nEntries == 1)
				{
					nCentreX = ptOutdoor->tCellRect.nSizeTileX / 2;
					nCentreY = ptOutdoor->tCellRect.nSizeTileY / 2;
				}
				else
				{
					for (k = 0; k < ptOutdoor->nEntries; k++)
					{
						nSumX += ptOutdoor->atEntries[k].nX - ptLevel->tCoords.nTileX;
						nSumY += ptOutdoor->atEntries[k].nY - ptLevel->tCoords.nTileY;
					}
					nCentreX = nSumX / (ptOutdoor->nEntries * 8);
					nCentreY = nSumY / (ptOutdoor->nEntries * 8);
				}
				fFound = FALSE;
				for (; nRadius < 8 && !fFound; nRadius++)
				{
					for (k = 0; k < 4; k++)
					{
						nX = sgcanMeetX[k] * nRadius + nCentreX;
						nY = sgcanMeetY[k] * nRadius + nCentreY;
						if (nX >= 0 && nX < ptOutdoor->tCellRect.nSizeTileX && nY >= 0 &&
							nY < ptOutdoor->tCellRect.nSizeTileY && OutdoorsCellIsFree(ptLevel, nX, nY))
						{
							fFound = TRUE;
							break;
						}
					}
				}
				ptOutdoor->atMeetingPoints[i].nX = ptLevel->tCoords.nTileX + nX * 8 + 3;
				ptOutdoor->atMeetingPoints[i].nY = ptLevel->tCoords.nTileY + nY * 8 + 3;
				ptOutdoor->atMeetingPoints[i].bDirection = 4;
			}
			else
			{
				ptOutdoor->atMeetingPoints[i].nX = ptOutdoor->atMeetingPoints[0].nX;
				ptOutdoor->atMeetingPoints[i].nY = ptOutdoor->atMeetingPoints[0].nY;
				ptOutdoor->atMeetingPoints[i].bDirection = 4;
			}
		}
	}
	for (i = 0; i < ptOutdoor->nEntries; i++)
		sEntryToEdge(ptLevel, &ptOutdoor->atMeetingPoints[i], &ptOutdoor->atMeetingEdges[i]);
}

static const int sgcanBendX[4] = { 1, 0, -1, 0 };
static const int sgcanBendY[4] = { 0, 1, 0, -1 };

static void __fastcall sBuildEntryPath(DRLGLEVEL *ptLevel, int nEntry)
{
	OUTDOORINFO *ptOutdoor;
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptNew;
	void *pPool;
	int nTurn;
	int nBendX;
	int nBendY;
	int nX;
	int nY;

	pPool = ptLevel->ptAct->pMemPool;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	ptVertex = ptOutdoor->aptEntryPaths[nEntry];
	nTurn = SeedNext(&ptLevel->tSeed) & 3;
	if (!ptVertex)
		return;
	if (ptOutdoor->atMeetingPoints[nEntry].bDirection != 4)
	{
		ptNew = DrlgVerNew(pPool, 0);
		ptNew->nX = ptOutdoor->atMeetingPoints[nEntry].nX;
		ptNew->nY = ptOutdoor->atMeetingPoints[nEntry].nY;
		ptNew->ptNext = ptVertex;
		ptOutdoor->aptEntryPaths[nEntry] = ptNew;
	}
	ptVertex->nX = ptOutdoor->atMeetingEdges[nEntry].nX;
	ptVertex->nY = ptOutdoor->atMeetingEdges[nEntry].nY;
	for (ptVertex = ptVertex->ptNext; ptVertex; ptVertex = ptVertex->ptNext)
	{
		if (!ptVertex->ptNext)
		{
			ptVertex->nX = ptOutdoor->atEntryEdges[nEntry].nX;
			ptVertex->nY = ptOutdoor->atEntryEdges[nEntry].nY;
			ptNew = DrlgVerNew(pPool, 0);
			ptNew->nX = ptOutdoor->atEntries[nEntry].nX;
			ptNew->nY = ptOutdoor->atEntries[nEntry].nY;
			ptVertex->ptNext = ptNew;
			return;
		}
		nBendX = ((SeedNext(&ptLevel->tSeed) & 1) + 2) * sgcanBendX[nTurn];
		nBendY = ((SeedNext(&ptLevel->tSeed) & 1) + 2) * sgcanBendY[nTurn];
		nTurn = (nTurn + 1) % 4;
		nX = ptVertex->nX << 3;
		nY = ptVertex->nY << 3;
		ptVertex->nX = nX;
		ptVertex->nY = nY;
		ptVertex->nX = nBendX + ptLevel->tCoords.nTileX + nX + 3;
		ptVertex->nY = ptLevel->tCoords.nTileY + nY + nBendY + 3;
	}
}
