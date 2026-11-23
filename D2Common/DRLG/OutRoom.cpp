#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutRoom.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

void __fastcall OutRoomBuildTilesA(ROOM *ptRoom)
{
	TILESUBROOM tCtx;
	DRLGLEVEL *ptLevel;
	PTLEVELSREC ptRecord;
	PTOUTROOMINFO ptInfo;
	void *pPool;
	BYTE bAct;
	DWORD dwFloor;
	int nWaypoint;
	int nShrine;
	int nSizeX;
	int nSizeY;
	int nX;
	int nY;
	int i; int nLevelId;

	ptLevel = ptRoom->ptLevel;
	pPool = ptLevel->ptAct->pMemPool;
	nLevelId = ptLevel->nLevelId;
	bAct = DrlgGetActNum(nLevelId);
	ptInfo = ptRoom->field_20.ptOutdoor;
	ptRecord = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId);
	nSizeX = ptRoom->tCoords.nSizeTileX + 1;
	nSizeY = ptRoom->tCoords.nSizeTileY + 1;
	nWaypoint = (ptRoom->dwFlags >> 16) & 3;
	nShrine = (ptRoom->dwFlags >> 12) & 15;
	dwFloor = 0;
	DrlgGridAlloc(pPool, &ptInfo->tOrientations, nSizeX, nSizeY);
	DrlgGridAlloc(pPool, &ptInfo->tWalls, nSizeX, nSizeY);
	DrlgGridAlloc(pPool, &ptInfo->tFloors, nSizeX, nSizeY);
	for (nY = 0; nY < 8; nY++)
	{
		for (nX = 0; nX < 8; nX++)
			DrlgGridApply(&ptInfo->tFloors, nX, nY, 262146, 3);
	}
	if (bAct == 0)
		OutdoorsMarkRoomPaths(ptLevel, ptRoom);
	RoomTileAllocBlock(ptRoom);
	for (i = 0; i < (int)(sizeof(tCtx) / 4); i++)
		((DWORD *)&tCtx)[i] = 0;
	tCtx.ptRoom = ptRoom;
	tCtx.aptOrientationGrids[0] = &ptInfo->tOrientations;
	tCtx.aptWallGrids[0] = &ptInfo->tWalls;
	tCtx.ptFloorGrid = &ptInfo->tFloors;
	tCtx.field_28 = 0;
	tCtx.nGrids = 1;
	if (nWaypoint)
	{
		tCtx.nType = ptRecord->SubWaypoint;
		tCtx.nSubTheme = 0;
		tCtx.dwPicks = (DWORD)nWaypoint;
		TileSubRoomRun(&tCtx);
	}
	if (nShrine)
	{
		tCtx.nType = ptRecord->SubShrine;
		tCtx.nSubTheme = 0;
		tCtx.dwPicks = (DWORD)nShrine;
		TileSubRoomRun(&tCtx);
	}
	tCtx.nType = ptInfo->nSubType;
	tCtx.nSubTheme = ptInfo->nSubTheme;
	tCtx.dwPicks = ptInfo->dwSubPicks;
	TileSubRoomRun(&tCtx);
	switch (ptLevel->dwLevelType)
	{
	case 16:
		dwFloor = 256;
		break;
	case 22:
		dwFloor = 1048576;
		break;
	case 27:
		dwFloor = 10485760;
		break;
	case 28:
		dwFloor = 23068672;
		break;
	}
	for (nY = 0; nY < nSizeY; nY++)
	{
		for (nX = 0; nX < nSizeX; nX++)
		{
			if (!(DrlgGridGet(&ptInfo->tFloors, nX, nY) & 66125568))
				DrlgGridApply(&ptInfo->tFloors, nX, nY, (int)dwFloor, 0);
		}
	}
	DrlgGridApplyBorder(&ptInfo->tWalls, 4, 0);
	DrlgGridApplyBorder(&ptInfo->tFloors, 4, 0);
}

ROOM *__fastcall OutRoomNew(DRLGLEVEL *ptLevel, int nX, int nY, int nSizeX, int nSizeY, DWORD dwExits,
	DWORD dwCellFlags, int nCellValue, DWORD dwFlags)
{
	ROOM *ptRoom;
	OUTROOMINFO *ptInfo;
	PTLEVELSREC ptRecord;

	ptRoom = DrlgRoomNew(ptLevel, 1);
	ptRoom->tCoords.nTileX = nX;
	ptRoom->tCoords.nTileY = nY;
	ptRoom->tCoords.nSizeTileX = nSizeX;
	ptRoom->tCoords.nSizeTileY = nSizeY;
	DrlgRoomLink(ptLevel, ptRoom);
	ptRoom->dwFlags |= dwExits | 524288;
	ptRoom->dwDt1Mask = dwFlags;
	ptInfo = ptRoom->field_20.ptOutdoor;
	ptInfo->nCellFlags = (int)dwCellFlags;
	ptInfo->field_58 = nCellValue;
	ptRecord = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(ptLevel->nLevelId);
	ptInfo->nSubType = ptRecord->SubType;
	ptInfo->nSubTheme = ptRecord->SubTheme;
	ptInfo->dwSubPicks = TileSubRoomPick(ptRoom, ptRecord->SubType, ptRecord->SubTheme);
	return ptRoom;
}

void __fastcall OutRoomFreeInfo(ROOM *ptRoom)
{
	OUTROOMINFO *ptInfo;
	void *pPool;

	ptInfo = ptRoom->field_20.ptOutdoor;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	if (ptInfo)
	{
		DrlgGridFree(pPool, &ptInfo->tOrientations);
		DrlgGridFree(pPool, &ptInfo->tWalls);
		DrlgGridFree(pPool, &ptInfo->tFloors);
		DrlgGridFree(pPool, &ptInfo->tPaths);
		DrlgVerFreeRing(pPool, &ptInfo->field_50);
#line 171
		D2FREEPOOL(pPool, ptInfo);
		ptRoom->field_20.ptOutdoor = NULL;
	}
}

void __fastcall OutRoomFreeGrids(ROOM *ptRoom)
{
	OUTROOMINFO *ptInfo;
	void *pPool;

	ptInfo = ptRoom->field_20.ptOutdoor;
	if (ptInfo)
	{
		pPool = ptRoom->ptLevel->ptAct->pMemPool;
		DrlgGridFree(pPool, &ptInfo->tOrientations);
		DrlgGridFree(pPool, &ptInfo->tWalls);
		DrlgGridFree(pPool, &ptInfo->tFloors);
		DrlgGridFree(pPool, &ptInfo->tPaths);
		DrlgVerFreeRing(pPool, &ptInfo->field_50);
	}
}

void __fastcall OutRoomNewInfo(ROOM *ptRoom)
{
	OUTROOMINFO *ptInfo;
	DWORD i;

#line 195
	ptInfo = (OUTROOMINFO *)D2ALLOCPOOL(ptRoom->ptLevel->ptAct->pMemPool, sizeof(OUTROOMINFO));
	for (i = 0; i < sizeof(OUTROOMINFO) / sizeof(DWORD); i++)
		((DWORD *)ptInfo)[i] = 0;
	ptRoom->field_20.ptOutdoor = ptInfo;
}

void __fastcall OutRoomBuildTilesB(ROOM *ptRoom)
{
	PTOUTROOMINFO ptInfo;
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILEARRAYS ptArrays;

	RoomTileAllocBlock(ptRoom);
	ptInfo = ptRoom->field_20.ptOutdoor;
	RoomTileCountWalls(ptRoom, &ptInfo->tWalls, &ptInfo->tOrientations, FALSE, FALSE);
	RoomTileCountTiles(ptRoom, &ptInfo->tWalls, FALSE, FALSE, FALSE);
	RoomTileCountTiles(ptRoom, &ptInfo->tFloors, FALSE, FALSE, FALSE);
	RoomTileAllocArrays(ptRoom);
	RoomTileFill(ptRoom, &ptInfo->tWalls, &ptInfo->tOrientations, FALSE, FALSE, FALSE);
	RoomTileFill(ptRoom, &ptInfo->tFloors, NULL, FALSE, FALSE, FALSE);
	ptTiles = ptRoom->ptDrlgTiles;
	ptArrays = &ptTiles->tTileArrays;
	ptArrays->nNumWalls = ptTiles->nWallCount;
	ptArrays->nNumFloors = ptTiles->nFloorCount;
	ptArrays->nNumShadows = ptTiles->dwShadowCount;
	DrlgLogicBuildEmpty(ptRoom);
}

DWORD __fastcall OutRoomGetCellFlagBit(ROOM *ptRoom)
{
	return ptRoom->field_20.ptOutdoor->nCellFlags & 128;
}

static __inline void sFindTownEdge(DRLGLEVEL *ptLevel, int nX, int *pnX, int *pnY)
{
	OUTDOORINFO *pOutdoor;
	int nY;
	int nDef;
	DWORD nFile;

	pOutdoor = ptLevel->field_38.pOutdoor;
	for (nY = 1; nY < pOutdoor->tCellRect.nSizeTileX - 1; nY++)
	{
		nDef = DrlgGridGet(&pOutdoor->tDefGrid, nX, nY);
		nFile = ((DWORD)DrlgGridGet(&pOutdoor->tCellFlagGrid, nX, nY) >> 16) & 15;
		if (nDef == 28 && nFile == 1)
		{
			*pnX = nX;
			*pnY = nY;
			return;
		}
	}
	*pnX = -1;
	*pnY = -1;
}

void __fastcall OutRoomFindTownEdge(DRLGLEVEL *ptLevel, int *pnX, int *pnY)
{
	sFindTownEdge(ptLevel, ptLevel->field_38.pOutdoor->tCellRect.nSizeTileX / 2 - 1, pnX, pnY);
}

void __fastcall OutRoomFindTownEdgeRight(DRLGLEVEL *ptLevel, int *pnX, int *pnY)
{
	sFindTownEdge(ptLevel, ptLevel->field_38.pOutdoor->tCellRect.nSizeTileX - 2, pnX, pnY);
}
