#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\DrlgLogic.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "Drlg.h"

void __fastcall DrlgRoomSetTileNode(ROOM *ptRoom, PTDRLGTILE ptTile, int nX, int nY)
{
	PTDRLGLOGICINFO ptLogic;

	ptLogic = ptRoom->ptLogic;
	if (ptLogic->dwFlags & 1)
	{
		ptTile->ptLogicNode = NULL;
		return;
	}
	nX -= ptRoom->tCoords.nTileX;
	nY -= ptRoom->tCoords.nTileY;
	ptTile->ptLogicNode = (PTDRLGLOGICNODE)DrlgGridGet(&ptLogic->tNodeGrid, nX, nY);
}

#pragma auto_inline(off)

void __fastcall DrlgLogicFree(ROOM *ptRoom)
{
	PTDRLGLOGICINFO ptLogic;
	PTDRLGLOGICNODE ptNode;
	PTDRLGLOGICNODE ptNext;
	void *pPool;

	ptLogic = ptRoom->ptLogic;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	if (ptLogic)
	{
		if (ptLogic->dwFlags & 2)
		{
			DrlgGridFree(pPool, &ptLogic->tLogicGrid);
			DrlgGridFree(pPool, &ptLogic->tNodeGrid);
		}
		for (ptNode = ptLogic->ptNodes; ptNode; ptNode = ptNext)
		{
			ptNext = ptNode->ptNext;
#line 251
			D2FREEPOOL(pPool, ptNode);
		}
#line 256
		D2FREEPOOL(pPool, ptLogic);
		ptRoom->ptLogic = NULL;
	}
}

static const int sganLogicSteps[8 + 5 * 5] =
{
	1, 0, 0, 1, -1, 0, 0, -1,
	23, 0, 5, 21, 17,
	15, 3, 0, 9, 7,
	39, 0, 0, 5, 3,
	31, 31, 31, 31, 31,
	31, 31, 31, 31, 31,
};
#define LOGIC_STEP_DX(n)	sganLogicSteps[(n) * 2]
#define LOGIC_STEP_DY(n)	sganLogicSteps[(n) * 2 + 1]
#define LOGIC_MASK(c, n)	sganLogicSteps[8 + (c) * 5 + (n) + 1]

static const int sganLogicWallClass[19] =
{
	-1, 0, 1, 2, 2, 0, 1, 3, 0, 1, 0, 1, 4, -1, 4, 0,
	0, 0, 0,
};

struct DRLGLOGICFILL
{
 	ROOM		*ptRoom;
 	DRLGGRID	*ptIdGrid;
 	DRLGGRID	*ptOrientGrid;
 	DRLGGRID	*ptWallGrid;
 	DRLGGRID	*ptFloorGrid;
 	DRLGGRID	*ptMarkGrid;
 	int			nLastId;
 	DWORD		dwFillId;
};
typedef struct DRLGLOGICFILL DRLGLOGICFILL, *PTDRLGLOGICFILL;

static void __fastcall sLogicFill(PTDRLGLOGICFILL ptFill, int nX, int nY, int nFrom);
static void __fastcall sLogicMergeNear(ROOM *ptRoom);
static void __fastcall sLogicMergeCell(ROOM *ptRoom, ROOM *ptNear, int nX, int nY);
static void __fastcall sLogicRenumber(ROOM *ptRoom, int nOld, int nNew, BOOL fFlag);
static BOOL __fastcall sLogicIsWall(PTDRLGTILE ptTile);
static void __fastcall sLogicSetTileNodes(ROOM *ptRoom);
static void __fastcall sLogicNewNodes(ROOM *ptRoom, PTDRLGLOGICINFO ptLogic, int nFirst);

static __inline PTDRLGLOGICINFO sLogicNew(ROOM *ptRoom, void *pPool)
{
	PTDRLGLOGICINFO ptLogic;
	int i;

#line 217
	ptLogic = (PTDRLGLOGICINFO)D2ALLOCPOOL(pPool, sizeof(DRLGLOGICINFO));
	for (i = 0; i < (int)(sizeof(DRLGLOGICINFO) / 4); i++)
		((DWORD *)ptLogic)[i] = 0;
	ptRoom->ptLogic = ptLogic;
	return ptLogic;
}

static __inline void sLogicNewHead(PTDRLGLOGICINFO ptLogic, void *pPool, int nCount)
{
#line 227
	ptLogic->ptNodes = (PTDRLGLOGICNODE)D2ALLOCPOOL(pPool, nCount * sizeof(DRLGLOGICNODE));
	memset(ptLogic->ptNodes, 0, ptLogic->nNumNodes * sizeof(DRLGLOGICNODE));
}

static __inline BOOL sLogicWallMarks(PTDRLGTILE ptTile)
{
	if ((ptTile->dwFlags & 114688) != 16384)
		return FALSE;
	if (ptTile->nOrientation == 15)
		return FALSE;
	return (~ptTile->dwFlags >> 11) & 1;
}

void __fastcall DrlgLogicBuild(ROOM *ptRoom, DRLGGRID *ptOrient, DRLGGRID *ptFloors, DRLGGRID *ptWalls)
{
	DRLGLOGICFILL tFill;
	DRLGGRID tMark;
	int anRowOffsets[256];
	int anCells[1024];
	DRLGLEVEL *ptLevel;
	void *pPool;
	PTDRLGLOGICINFO ptLogic;
	PTDRLGTILEARRAYS ptArrays;
	PTDRLGTILEARRAYS ptWallArrays;
	PTDRLGTILE ptWall;
	PTDRLGTILE ptTile;
	PTDRLGTILEGROUP ptGroup;
	ROOM *ptNear;
	int nFirst;
	int nX;
	int nY;
	int nSizeX;
	int nSizeY;
	DWORD dwFloor;
	DWORD i;

	ptLevel = ptRoom->ptLevel;
	pPool = ptLevel->ptAct->pMemPool;
	sLogicNew(ptRoom, pPool);
	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	ptLogic = ptRoom->ptLogic;
	for (i = 0; i < (DWORD)ptArrays->nNumWalls; i++)
	{
		if ((ptArrays->ptWalls[i].dwFlags & 4)
			&& !(ptArrays->ptWalls[i].dwFlags & 114688))
			DrlgGridApply(ptWalls, ptArrays->ptWalls[i].nX, ptArrays->ptWalls[i].nY, 8, 0);
	}
	ptLogic->dwFlags |= 2;
	nSizeX = ptRoom->tCoords.nSizeTileX;
	nSizeY = ptRoom->tCoords.nSizeTileY;
	DrlgGridAlloc(pPool, &ptLogic->tLogicGrid, nSizeX + 1, nSizeY + 1);
	if (ptLevel->nNextLogic == 0)
		ptLevel->nNextLogic = 1;
	nFirst = ptLevel->nNextLogic;
	DrlgGridInit(&tMark, ptRoom->tCoords.nSizeTileX + 1, ptRoom->tCoords.nSizeTileY + 1, anCells,
		anRowOffsets);
	ptWallArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	for (i = 0; i < (DWORD)ptWallArrays->nNumWalls; i++)
	{
		ptWall = &ptWallArrays->ptWalls[i];
		if (sLogicWallMarks(ptWall))
			DrlgGridApply(&tMark, ptWall->nX, ptWall->nY, 1, 0);
	}
	for (i = 0; i < (DWORD)ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear == ptRoom || !ptNear->ptDrlgTiles)
			continue;
		for (ptGroup = ptNear->ptDrlgTiles->ptFirstGroup; ptGroup; ptGroup = ptGroup->ptNext)
		{
			if (ptGroup->nFloorGroup)
				continue;
			for (ptTile = ptGroup->ptTiles; ptTile; ptTile = ptTile->ptNext)
			{
				if (!sLogicIsWall(ptTile))
					continue;
				nX = ptTile->nX + ptNear->tCoords.nTileX;
				nY = ptTile->nY + ptNear->tCoords.nTileY;
				if (DrlgRectContainsInclusive(&ptRoom->tCoords, nX, nY))
					DrlgGridApply(&tMark, nX - ptRoom->tCoords.nTileX, nY - ptRoom->tCoords.nTileY, 1,
						0);
			}
		}
	}
	memset(&tFill, 0, sizeof(tFill));
	tFill.ptIdGrid = &ptLogic->tLogicGrid;
	tFill.ptOrientGrid = ptOrient;
	tFill.ptRoom = ptRoom;
	tFill.ptWallGrid = ptWalls;
	tFill.ptFloorGrid = ptFloors;
	tFill.ptMarkGrid = &tMark;
	tFill.nLastId = nFirst;
	nSizeX = tFill.ptRoom->tCoords.nSizeTileX;
	nSizeY = tFill.ptRoom->tCoords.nSizeTileY;
	for (nY = 0; nY <= nSizeY; nY++)
	{
		for (nX = 0; nX <= nSizeX; nX++)
		{
			if (DrlgGridGet(tFill.ptIdGrid, nX, nY) & 268435456)
				continue;
			tFill.nLastId++;
			tFill.dwFillId = (tFill.nLastId & 268435455) | 268435456;
			dwFloor = (DWORD)DrlgGridGet(tFill.ptFloorGrid, nX, nY);
			if ((dwFloor & 31522560) == 31457280 || (dwFloor & 0x80000000))
				tFill.dwFillId |= 536870912;
			sLogicFill(&tFill, nX, nY, -1);
		}
	}
	DrlgGridClear(&tMark);
	ptLogic->nNumNodes = tFill.nLastId - nFirst + 1;
	sLogicNewHead(ptLogic, pPool, ptLogic->nNumNodes);
	ptLevel->nNextLogic += ptLogic->nNumNodes;
	sLogicNewNodes(ptRoom, ptLogic, nFirst);
	sLogicSetTileNodes(ptRoom);
	sLogicMergeNear(ptRoom);
}

static void __fastcall sLogicFill(PTDRLGLOGICFILL ptFill, int nX, int nY, int nFrom)
{
	DWORD dwMask;
	int nOrient;
	int i;

	while (DrlgRectContainsInclusive(&ptFill->ptRoom->tCoords, ptFill->ptRoom->tCoords.nTileX + nX,
		ptFill->ptRoom->tCoords.nTileY + nY))
	{
		if (DrlgGridGet(ptFill->ptIdGrid, nX, nY) & 268435456)
			return;
		if (!DrlgGridGet(ptFill->ptMarkGrid, nX, nY))
		{
			DrlgGridApply(ptFill->ptIdGrid, nX, nY, (int)ptFill->dwFillId, 0);
			for (i = 0; i < 4; i++)
				sLogicFill(ptFill, nX + LOGIC_STEP_DX(i), nY + LOGIC_STEP_DY(i), i);
			return;
		}
		nOrient = 0;
		if (DrlgGridIsSet(ptFill->ptOrientGrid))
			nOrient = DrlgGridGet(ptFill->ptOrientGrid, nX, nY);
		dwMask = (DWORD)LOGIC_MASK(sganLogicWallClass[nOrient], nFrom);
		if (dwMask & 1)
			DrlgGridApply(ptFill->ptIdGrid, nX, nY, (int)ptFill->dwFillId, 0);
		if ((dwMask & 2) && nFrom != 2)
			sLogicFill(ptFill, nX + LOGIC_STEP_DX(0), nY + LOGIC_STEP_DY(0), 0);
		if ((dwMask & 4) && nFrom != 3)
			sLogicFill(ptFill, nX + LOGIC_STEP_DX(1), nY + LOGIC_STEP_DY(1), 1);
		if ((dwMask & 8) && nFrom != 0)
			sLogicFill(ptFill, nX + LOGIC_STEP_DX(2), nY + LOGIC_STEP_DY(2), 2);
		if ((dwMask & 16) && nFrom != 1)
			sLogicFill(ptFill, nX + LOGIC_STEP_DX(3), nY + LOGIC_STEP_DY(3), 3);
		if (!(dwMask & 32))
			return;
		nY += LOGIC_STEP_DY(1);
		nX += LOGIC_STEP_DX(0);
		nFrom = -1;
	}
}

static void __fastcall sLogicMergeNear(ROOM *ptRoom)
{
	ROOM *ptNear;
	int nX;
	int nY;
	int i;

	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear == ptRoom || !ptNear->ptLogic)
			continue;
		if (DrlgRectGapTest(&ptRoom->tCoords, &ptNear->tCoords, 1))
			continue;
		for (nX = ptRoom->tCoords.nTileX; nX <= ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX;
			nX++)
		{
			sLogicMergeCell(ptRoom, ptNear, nX, ptRoom->tCoords.nTileY);
			sLogicMergeCell(ptRoom, ptNear, nX, ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY);
		}
		for (nY = ptRoom->tCoords.nTileY; nY <= ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY;
			nY++)
		{
			sLogicMergeCell(ptRoom, ptNear, ptRoom->tCoords.nTileX, nY);
			sLogicMergeCell(ptRoom, ptNear, ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX, nY);
		}
	}
}

static void __fastcall sLogicMergeCell(ROOM *ptRoom, ROOM *ptNear, int nX, int nY)
{
	PTDRLGLOGICNODE ptNodeNear;
	PTDRLGLOGICNODE ptNodeRoom;
	int nIdNear;
	int nIdRoom;
	int nCellX;
	int nCellY;

	if (!DrlgRectContainsInclusive(&ptNear->tCoords, nX, nY))
		return;
	if (ptNear->ptLogic->dwFlags & 1)
		ptNodeNear = ptNear->ptLogic->ptNodes;
	else
	{
		nCellX = nX - ptNear->tCoords.nTileX;
		nCellY = nY - ptNear->tCoords.nTileY;
		ptNodeNear = (PTDRLGLOGICNODE)DrlgGridGet(&ptNear->ptLogic->tNodeGrid, nCellX, nCellY);
	}
	if (ptRoom->ptLogic->dwFlags & 1)
		ptNodeRoom = ptRoom->ptLogic->ptNodes;
	else
	{
		nCellX = nX - ptRoom->tCoords.nTileX;
		nCellY = nY - ptRoom->tCoords.nTileY;
		ptNodeRoom = (PTDRLGLOGICNODE)DrlgGridGet(&ptRoom->ptLogic->tNodeGrid, nCellX, nCellY);
	}
	nIdNear = ptNodeNear->nLogic;
	if (!nIdNear)
		return;
	nIdRoom = ptNodeRoom->nLogic;
	if (!nIdRoom)
		return;
	if (ptNear->ptLevel->nLevelId != ptRoom->ptLevel->nLevelId)
		return;
	if (!ptNodeNear || !ptNodeRoom || nIdNear == nIdRoom)
		return;
	if (ptNodeNear->field_20 != ptNodeRoom->field_20)
		return;
	sLogicRenumber(ptRoom, nIdRoom, nIdNear, ptNodeNear->field_20);
}

static void __fastcall sLogicRenumber(ROOM *ptRoom, int nOld, int nNew, BOOL fFlag)
{
	PTDRLGLOGICNODE ptNode;
	ROOM *ptNear;
	BOOL fFound;
	int i;

	if (!ptRoom->ptLogic || (ptRoom->ptLogic->dwFlags & 1))
		return;
	fFound = FALSE;
	for (ptNode = ptRoom->ptLogic->ptNodes; ptNode; ptNode = ptNode->ptNext)
	{
		if (ptNode->nLogic == nOld)
		{
			fFound = TRUE;
			ptNode->nLogic = nNew;
		}
	}
	if (!fFound)
		return;
	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear != ptRoom && ptNear->ptLevel->nLevelId == ptRoom->ptLevel->nLevelId)
			sLogicRenumber(ptNear, nOld, nNew, fFlag);
	}
}

static BOOL __fastcall sLogicIsWall(PTDRLGTILE ptTile)
{
	if ((ptTile->dwFlags & 114688) != 16384)
		return FALSE;
	if (ptTile->nOrientation == 15)
		return FALSE;
	return (~ptTile->dwFlags >> 11) & 1;
}

static void __fastcall sLogicSetTileNodes(ROOM *ptRoom)
{
	PTDRLGTILEARRAYS ptArrays;
	PTDRLGTILE ptTile;
	int nX;
	int nY;
	DWORD i;

	ptArrays = &ptRoom->ptDrlgTiles->tTileArrays;
	for (i = 0; i < (DWORD)ptArrays->nNumWalls; i++)
	{
		ptTile = &ptArrays->ptWalls[i];
		nY = ptTile->nY + ptRoom->tCoords.nTileY;
		nX = ptTile->nX + ptRoom->tCoords.nTileX;
		if (ptRoom->ptLogic->dwFlags & 1)
			ptTile->ptLogicNode = NULL;
		else
			ptTile->ptLogicNode = (PTDRLGLOGICNODE)DrlgGridGet(&ptRoom->ptLogic->tNodeGrid,
				nX - ptRoom->tCoords.nTileX, nY - ptRoom->tCoords.nTileY);
	}
}

static void __fastcall sLogicNewNodes(ROOM *ptRoom, PTDRLGLOGICINFO ptLogic, int nFirst)
{
	void *pPool;
	PTDRLGLOGICNODE ptNode;
	DWORD dwCell;
	int nId;
	int nSizeX;
	int nSizeY;
	int nX;
	int nY;
	int x;
	int y;
	BOOL fStop;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	DrlgGridAlloc(pPool, &ptLogic->tNodeGrid, ptRoom->tCoords.nSizeTileX + 1,
		ptRoom->tCoords.nSizeTileY + 1);
	nSizeX = ptRoom->tCoords.nSizeTileX + 1;
	nSizeY = ptRoom->tCoords.nSizeTileY + 1;
	for (nY = 0; nY < nSizeY; nY++)
	{
		for (nX = 0; nX < nSizeX; nX++)
		{
			dwCell = (DWORD)DrlgGridGet(&ptLogic->tLogicGrid, nX, nY);
			nId = (int)(dwCell & 268435455);
			if (DrlgGridGet(&ptLogic->tNodeGrid, nX, nY))
				continue;
#line 507
			ptNode = (PTDRLGLOGICNODE)D2ALLOCPOOL(pPool, sizeof(DRLGLOGICNODE));
			memset(ptNode, 0, sizeof(DRLGLOGICNODE));
			ptNode->nLogic = nId;
			ptNode->field_20 = (dwCell & 536870912) == 536870912;
			ptNode->ptNext = ptLogic->ptNodes;
			ptLogic->ptNodes = ptNode;
			ptNode->nLeft = nX;
			ptNode->nTop = nY;
			for (x = nX; x < nSizeX; x++)
			{
				if ((DWORD)nId != ((DWORD)DrlgGridGet(&ptLogic->tLogicGrid, x, nY) & 268435455))
					break;
				if (DrlgGridGet(&ptLogic->tNodeGrid, x, nY))
					break;
			}
			ptNode->nRight = x;
			fStop = FALSE;
			for (y = nY; !fStop && y < nSizeY; y++)
			{
				for (x = nX; x < ptNode->nRight; x++)
				{
					if ((DWORD)nId != ((DWORD)DrlgGridGet(&ptLogic->tLogicGrid, x, y) & 268435455) ||
						DrlgGridGet(&ptLogic->tNodeGrid, x, y))
					{
						fStop = TRUE;
						y--;
						break;
					}
				}
			}
			ptNode->nBottom = y;
			for (y = ptNode->nTop; y < ptNode->nBottom; y++)
			{
				for (x = ptNode->nLeft; x < ptNode->nRight; x++)
					DrlgGridApply(&ptLogic->tNodeGrid, x, y, (int)ptNode, 3);
			}
			ptNode->nTop += ptRoom->tCoords.nTileY;
			ptNode->nBottom += ptRoom->tCoords.nTileY;
			ptNode->nLeft += ptRoom->tCoords.nTileX;
			ptNode->nRight += ptRoom->tCoords.nTileX;
			memcpy(&ptNode->tRect, &ptNode->nLeft, 4 * sizeof(int));
			if (ptNode->nRectRight >= ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX)
				ptNode->nRectRight = ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX;
			if (ptNode->nRectBottom >= ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY)
				ptNode->nRectBottom = ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY;
			if (ptNode->tRect >= ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX ||
				ptNode->nRectTop >= ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY)
				memset(&ptNode->tRect, 0, 4 * sizeof(int));
		}
	}
}

void __fastcall DrlgLogicBuildEmpty(ROOM *ptRoom)
{
	DRLGLEVEL *ptLevel;
	void *pPool;
	PTDRLGLOGICINFO ptLogic;

	ptLevel = ptRoom->ptLevel;
	pPool = ptLevel->ptAct->pMemPool;
	ptLogic = sLogicNew(ptRoom, pPool);
	ptLogic->dwFlags |= 1;
	ptLevel->nNextLogic = 1;
	ptLogic->nNumNodes = 1;
	sLogicNewHead(ptLogic, pPool, 1);
	ptLogic->ptNodes->nLogic = ptLevel->nNextLogic;
	ptLogic->ptNodes->nTop = ptRoom->tCoords.nTileY;
	ptLogic->ptNodes->nBottom = ptRoom->tCoords.nSizeTileY + ptRoom->tCoords.nTileY;
	ptLogic->ptNodes->nLeft = ptRoom->tCoords.nTileX;
	ptLogic->ptNodes->nRight = ptRoom->tCoords.nSizeTileX + ptRoom->tCoords.nTileX;
	ptLogic->ptNodes->nRectTop = ptRoom->tCoords.nTileY;
	ptLogic->ptNodes->nRectBottom = ptRoom->tCoords.nSizeTileY + ptRoom->tCoords.nTileY;
	ptLogic->ptNodes->tRect = ptRoom->tCoords.nTileX;
	ptLogic->ptNodes->nRectRight = ptRoom->tCoords.nSizeTileX + ptRoom->tCoords.nTileX;
}

int __fastcall DrlgRoomGetLogicValue(ROOM *ptRoom, int nX, int nY)
{
	PTDRLGLOGICINFO ptLogic;
	PTDRLGLOGICNODE ptNode;
	int nCellX;
	int nCellY;

	ptLogic = ptRoom->ptLogic;
	if (ptLogic->dwFlags & 1)
		ptNode = ptLogic->ptNodes;
	else
	{
		nCellX = nX / 5 - ptRoom->tCoords.nTileX;
		nCellY = nY / 5 - ptRoom->tCoords.nTileY;
		ptNode = (PTDRLGLOGICNODE)DrlgGridGet(&ptLogic->tNodeGrid, nCellX, nCellY);
	}
	if (ptNode)
		return ptNode->nLogic;
	return -1;
}

#pragma auto_inline(on)
