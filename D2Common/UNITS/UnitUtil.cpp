#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\UNITS\\UnitUtil.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

DWORD __stdcall UnitCalcEdgeDistance(HUNIT hUnitA, HUNIT hUnitB);
DWORD __stdcall UnitGetProximityMetric(HUNIT hUnitA, HUNIT hUnitB);
int __stdcall UnitUtilGetDistance(void *hUnit, int nX, int nY);
BOOL __stdcall UnitUtilIsWithinRange(void *hUnit, GAMECOORD *ptCoord, DWORD dwRange);

static const int sgnUnitOverlapTable[8][8] =
{
	{ -1, -1, -1,  0,  2,  4,  6,  8 },
	{ -1, -1,  0,  1,  2,  4,  6,  8 },
	{ -1,  0,  0,  2,  3,  5,  7,  8 },
	{  0,  1,  2,  2,  4,  5,  7,  8 },
	{  2,  2,  3,  4,  5,  6,  7,  9 },
	{  4,  4,  5,  5,  6,  7,  8,  9 },
	{  6,  6,  7,  7,  7,  8, 10, 10 },
	{  8,  8,  8,  8,  9,  9, 10, 11 },
};

DWORD __stdcall UnitCalcEdgeDistance(HUNIT hUnitA, HUNIT hUnitB)
{
	int nDX;
	int nDY;
	int nSizeA;
	int nSizeB;
	DWORD dwDist;
	int nHalfSum, nT;

	nT = (int)D2CommonUnitGetX(hUnitB) - (int)D2CommonUnitGetX(hUnitA);
	nDX = nT < 0 ? -nT : nT;
	nT = (int)D2CommonUnitGetY(hUnitB) - (int)D2CommonUnitGetY(hUnitA);
	nDY = nT < 0 ? -nT : nT;
	nSizeA = (int)D2CommonUnitGetSizeX(hUnitA);
	nSizeB = (int)D2CommonUnitGetSizeX(hUnitB);

	if (nDX < 8 && nDY < 8 && nSizeA < 4 && nSizeB < 4)
	{
		dwDist = (DWORD)sgnUnitOverlapTable[nDY][nDX];
		if ((int)dwDist < 0)
			return 0;
		if (nSizeA == 3 || nSizeB == 3)
			dwDist--;
		if ((int)dwDist < 0)
			dwDist = 0;
		if (nSizeA <= 1 || nSizeB <= 1)
			return dwDist + 1;
	}
	else
	{
		nHalfSum = (int)(D2CommonUnitGetSizeX(hUnitA) >> 1) +
			(int)(D2CommonUnitGetSizeX(hUnitB) >> 1);
		if (nHalfSum < 0)
			nHalfSum = -nHalfSum;
		nDX -= nHalfSum;

		nHalfSum = (int)(D2CommonUnitGetSizeX(hUnitA) >> 1) +
			(int)(D2CommonUnitGetSizeX(hUnitB) >> 1);
		if (nHalfSum < 0)
			nHalfSum = -nHalfSum;
		nDY -= nHalfSum;

		if (nDX < 0)
			nDX = 0;
		if (nDY < 0)
			nDY = 0;
		dwDist = (nDX > nDY) ? (DWORD)(nDY + nDX * 2) : (DWORD)(nDX + nDY * 2);
	}
	return dwDist;
}

DWORD __stdcall UnitGetProximityMetric(HUNIT hUnitA, HUNIT hUnitB)
{
	int nDX;
	int nDY;
	int nSizeA;
	int nSizeB;
	int nDist;

	nDist = (int)D2CommonUnitGetX(hUnitB) - (int)D2CommonUnitGetX(hUnitA);
	nDX = nDist < 0 ? -nDist : nDist;
	nDist = (int)D2CommonUnitGetY(hUnitB) - (int)D2CommonUnitGetY(hUnitA);
	nDY = nDist < 0 ? -nDist : nDist;
	nSizeA = (int)D2CommonUnitGetSizeX(hUnitA);
	nSizeB = (int)D2CommonUnitGetSizeX(hUnitB);

	nDX = nDX - nSizeA / 2 - nSizeB / 2;
	nDY = nDY - nSizeA / 2 - nSizeB / 2;
	if (nDX < 0)
		nDX = 0;
	if (nDY < 0)
		nDY = 0;
	if (nDX > nDY)
		return (DWORD)((nDY + nDX * 2) / 2);
	return (DWORD)((nDX + nDY * 2) / 2);
}

int __stdcall UnitUtilGetDistance(void *hUnit, int nX, int nY)
{
	int nDx, nDy, nT, nCombined;

	nT = nX - (int)D2CommonUnitGetX(hUnit);
	nDx = nT < 0 ? -nT : nT;
	nT = nY - (int)D2CommonUnitGetY(hUnit);
	nDy = nT < 0 ? -nT : nT;
	if (nDx < 0)
		nDx = 0;
	if (nDy < 0)
		nDy = 0;
	if (nDx > nDy)
		nCombined = nDy + nDx * 2;
	else
		nCombined = nDx + nDy * 2;
	return nCombined / 2;
}

BOOL __stdcall UnitUtilIsWithinRange(void *hUnit, GAMECOORD *ptCoord, DWORD dwRange)
{
	int nDx, nDy, nT;
	DWORD dwCombined, dwDist;

	nT = (int)ptCoord->dwX - (int)D2CommonUnitGetX(hUnit);
	nDx = nT < 0 ? -nT : nT;
	nT = (int)ptCoord->dwY - (int)D2CommonUnitGetY(hUnit);
	nDy = nT < 0 ? -nT : nT;
	nDx -= (int)(D2CommonUnitGetSizeX(hUnit) >> 1);
	nDy -= (int)(D2CommonUnitGetSizeY(hUnit) >> 1);
	if (nDx > nDy)
		dwCombined = (DWORD)(nDy + nDx * 2);
	else
		dwCombined = (DWORD)(nDx + nDy * 2);
	dwDist = dwCombined >> 1;
	return dwRange >= dwDist;
}

static int sgnFootprintCells[9][2] =
{
	{ 0, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 }, { -1, -1 }, { 1, -1 }, { 1, 1 }, { -1, 1 },
};
static int sgnFootprintCellCount[4] = { 0, 1, 5, 9 };

typedef BOOL (__fastcall *PFNUNITUTILTEST)(PTUNIT);

static PTUNIT __fastcall UnitUtilFindUnitInFootprint(PTUNIT hUnit, PTDUNGEONROOM hRoom, int nX, int nY,
	int nSrcSize, PFNUNITUTILTEST pfnTest);

PTUNIT __stdcall UnitUtilFindOverlappingUnit(PTUNIT hUnit, PFNUNITUTILTEST pfnTest)
{
	int nSrcSize;

	nSrcSize = (int)D2CommonUnitGetSizeX(hUnit);
	if (nSrcSize == 0)
		return NULL;
#line 233
	D2ASSERT(nSrcSize < 4);
	return UnitUtilFindUnitInFootprint(hUnit, (PTDUNGEONROOM)D2CommonUnitGetPathRoom(hUnit),
		(int)D2CommonUnitGetX(hUnit), (int)D2CommonUnitGetY(hUnit), nSrcSize, pfnTest);
}

static PTUNIT __fastcall UnitUtilFindUnitInFootprint(PTUNIT hUnit, PTDUNGEONROOM hRoom, int nX, int nY,
	int nSrcSize, PFNUNITUTILTEST pfnTest)
{
	int nSrcCells;
	PTDUNGEONROOM *pppNear;
	DWORD dwNumNear;
	int i;
	int nCellX;
	int nCellY;
	DWORD j;
	GAMERECT atCoords[2];
	PTUNIT ptUnit;
	int nTestSize;
	int nTestCells;
	int nUnitX;
	int nUnitY;
	int k;
	int *pnCell;
	int *pnTest;

#line 179
	D2ASSERT(nSrcSize < 4);
	nSrcCells = sgnFootprintCellCount[nSrcSize];
	pnCell = sgnFootprintCells[0];
	D2CommonDungeonRoomGetNear(hRoom, &pppNear, &dwNumNear);
#line 188
	D2ASSERTMSG(!IsBadCodePtr((FARPROC)pfnTest), "pfnTest");
	for (i = 0; i < nSrcCells; i++)
	{
		nCellX = nX + *pnCell++;
		nCellY = nY + *pnCell++;
		for (j = 0; j < dwNumNear; j++)
		{
			D2CommonDungeonRoomCopyRects(pppNear[j], atCoords);
			if (nCellX + 2 >= atCoords[0].nX)
			{
				if (nCellY + 2 < atCoords[0].nY && nCellY - 2 > atCoords[0].nY + atCoords[0].nHeight)
					continue;
			}
			else
			{
				if (nCellX - 2 > atCoords[0].nX + atCoords[0].nWidth)
					continue;
				if (nCellY + 2 < atCoords[0].nY && nCellY - 2 > atCoords[0].nY + atCoords[0].nHeight)
					continue;
			}
			for (ptUnit = pppNear[j]->ptUnitFirst; ptUnit != NULL; ptUnit = ptUnit->ptRoomNext)
			{
				if (ptUnit == hUnit)
					continue;
				if (!pfnTest(ptUnit))
					continue;
				nTestSize = (int)D2CommonUnitGetSizeX(ptUnit);
				if (nTestSize == 0)
					continue;
#line 204
				D2ASSERT(nTestSize < 4);
				nTestCells = sgnFootprintCellCount[nTestSize];
				pnTest = sgnFootprintCells[0];
				nUnitX = (int)D2CommonUnitGetX(ptUnit);
				nUnitY = (int)D2CommonUnitGetY(ptUnit);
				for (k = 0; k < nTestCells; pnTest++, k++)
				{
					if (nCellX == nUnitX + *pnTest++ &&
						nCellY == nUnitY + *pnTest)
						return ptUnit;
				}
			}
		}
	}
	return NULL;
}

PTUNIT __stdcall UnitUtilFindUnitAtPoint(PTDUNGEONROOM hRoom, int nX, int nY, PFNUNITUTILTEST pfnTest,
	int nSize)
{
	PTDUNGEONROOM *pppNear;
	DWORD dwNumNear;
	DWORD j;
	GAMERECT atCoords[2];
	PTUNIT ptUnit;
	int nTestSize;
	int nTestCells;
	int nUnitX;
	int nUnitY;
	int k;
	int *pnTest;

#line 264
	D2ASSERTMSG(!IsBadCodePtr((FARPROC)pfnTest), "pfnTest");
	if (nSize != 1)
		return UnitUtilFindUnitInFootprint(NULL, hRoom, nX, nY, nSize, pfnTest);

	D2CommonDungeonRoomGetNear(hRoom, &pppNear, &dwNumNear);
	for (j = 0; j < dwNumNear; j++)
	{
		D2CommonDungeonRoomCopyRects(pppNear[j], atCoords);
		if (nX + 2 >= atCoords[0].nX)
		{
			if (nY + 2 < atCoords[0].nY && nY - 2 > atCoords[0].nY + atCoords[0].nHeight)
				continue;
		}
		else
		{
			if (nX - 2 > atCoords[0].nX + atCoords[0].nWidth)
				continue;
			if (nY + 2 < atCoords[0].nY && nY - 2 > atCoords[0].nY + atCoords[0].nHeight)
				continue;
		}
		for (ptUnit = pppNear[j]->ptUnitFirst; ptUnit != NULL; ptUnit = ptUnit->ptRoomNext)
		{
			switch (ptUnit->eType)
			{
			case UNIT_PLAYER:
				if (ptUnit->dwMode == 0 || ptUnit->dwMode == 17)
					continue;
				break;
			case UNIT_MONSTER:
				if (ptUnit->dwMode == 0 || ptUnit->dwMode == 12)
					continue;
				break;
			}
			if (!pfnTest(ptUnit))
				continue;
			nTestSize = (int)D2CommonUnitGetSizeX(ptUnit);
			if (nTestSize == 0)
				continue;
#line 284
			D2ASSERT(nTestSize < 4);
			nTestCells = sgnFootprintCellCount[nTestSize];
			pnTest = sgnFootprintCells[0];
			nUnitX = (int)D2CommonUnitGetX(ptUnit);
			nUnitY = (int)D2CommonUnitGetY(ptUnit);
			for (k = 0; k < nTestCells; pnTest++, k++)
			{
				if (nX == nUnitX + *pnTest++ && nY == nUnitY + *pnTest)
					return ptUnit;
			}
		}
	}
	return NULL;
}

void __fastcall UnitUtilSetTestCallback(UNIT *ptUnit, void *pfnTest, DWORD dwKeptTargetType,
	DWORD dwKeptTargetId)
{
#line 311
	D2ASSERT(ptUnit);
	ptUnit->nKeptCmd = (DWORD)pfnTest;
	ptUnit->dwKeptCmdPending = 1;
	ptUnit->dwKeptTargetType = dwKeptTargetType;
	ptUnit->dwKeptTargetId = dwKeptTargetId;
}
