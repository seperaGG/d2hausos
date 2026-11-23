#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Drlg\\DrlgAnim.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "FogImports.h"

void __fastcall DrlgRoomAnimateNear(ROOM *ptRoom)
{
	ROOM *ptNear;
	PTDRLGTILEANIM ptAnim;
	int i;

	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (!(ptNear->dwFlags & 134217728))
			continue;
		if (!ptNear->ptDrlgTiles)
			continue;
		for (ptAnim = ptNear->ptDrlgTiles->ptFirstAnim; ptAnim; ptAnim = ptAnim->ptNext)
		{
			ptAnim->pptFrames[ptAnim->nAnimPos >> 8]->dwFlags |= 8;
			ptAnim->nAnimPos = (ptAnim->nAnimRate + ptAnim->nAnimPos) % (ptAnim->nNumFrames << 8);
			ptAnim->pptFrames[ptAnim->nAnimPos >> 8]->dwFlags &= ~8;
		}
	}
}

static void __fastcall sAnimateTiles(ROOM *ptRoom, int nStep, PTDRLGTILE ptTiles, int nCount,
	DRLGGRID *ptGrids, int nGrids);

void __fastcall DrlgAnimTiles(ROOM *ptRoom, int nStep, DRLGGRID *ptWalls, int nPairs, DRLGGRID *ptFloors,
	int nFloors, DRLGGRID *ptShadows)
{
	PTDRLGTILEBLOCK ptBlock;

	ptBlock = ptRoom->ptDrlgTiles;
	sAnimateTiles(ptRoom, nStep, ptBlock->tTileArrays.ptWalls, ptBlock->nWallCount, ptWalls, nPairs);
	sAnimateTiles(ptRoom, nStep, ptBlock->tTileArrays.ptFloors, ptBlock->nFloorCount, ptFloors, nFloors);
	sAnimateTiles(ptRoom, nStep, ptBlock->tTileArrays.ptShadows, (int)ptBlock->dwShadowCount, ptShadows, 1);
}

static void __fastcall sAnimateTiles(ROOM *ptRoom, int nStep, PTDRLGTILE ptTiles, int nCount,
	DRLGGRID *ptGrids, int nGrids)
{
	void *pPool;
	void *ahTiles[40];
	PTDRLGTILE ptTile;
	PTDRLGTILE ptFrame;
	PTDRLGTILEANIM ptAnim;
	DWORD dwCell;
	DWORD dwFound;
	DWORD dwFrame;
	DWORD i;
	BYTE bCode;
	BYTE bSub;
	int nX;
	int nY;
	int n;
	int nGrid;

	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	if (!ptTiles)
		return;
	if (!nStep)
		nStep = 80;
	for (n = 0; n < nCount; n++)
	{
		ptTile = &ptTiles[n];
		if (!ptTile->pTile || !(D2CMPTileGetFlags(ptTile->pTile) & 256))
			continue;
		nGrid = (int)((ptTile->dwFlags >> 14) & 7);
		nGrid--;
		nX = ptTile->nX;
		dwCell = (DWORD)DrlgGridGet(&ptGrids[nGrid], nX, ptTile->nY);
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
		dwFound = D2CMPTileLibraryFindTiles(ptRoom->apTileProjects, ptTile->nOrientation, bCode, bSub,
			ahTiles,
			40);
#line 153
		ptAnim = (PTDRLGTILEANIM)D2ALLOCPOOL(pPool, sizeof(DRLGTILEANIM));
		memset(ptAnim, 0, sizeof(DRLGTILEANIM));
		ptAnim->nAnimPos = 0;
		ptAnim->nNumFrames = (int)dwFound;
#line 159
		ptAnim->pptFrames = (PTDRLGTILE *)D2ALLOCPOOL(pPool, dwFound * sizeof(PTDRLGTILE));
		memset(ptAnim->pptFrames, 0, dwFound * sizeof(PTDRLGTILE));
		ptAnim->nAnimRate = nStep;
		ptAnim->ptNext = ptRoom->ptDrlgTiles->ptFirstAnim;
		ptRoom->ptDrlgTiles->ptFirstAnim = ptAnim;
		for (i = 0; i < dwFound; i++)
		{
			if (!D2CMPTileGetRarity(ahTiles[i]))
				break;
		}
		{
			char szMessage[100] = "";

			if (i >= dwFound)
			{
#line 179
				D2WARNMSG((const char *)FogDataArraysSprintf(szMessage, "Animating tiles missing Rarity 0 Tile - Style %d Seq %d", bCode, bSub));
				i = 0;
			}
			ptTile->pTile = ahTiles[i];
			ptAnim->pptFrames[0] = ptTile;
			nX = ptTile->nX + ptRoom->tCoords.nTileX;
			nY = ptTile->nY + ptRoom->tCoords.nTileY;
			for (dwFrame = 1; dwFrame < dwFound; dwFrame++)
			{
				for (i = 0; i < dwFound; i++)
				{
					if (D2CMPTileGetRarity(ahTiles[i]) == dwFrame)
						break;
				}
				if (i >= dwFound)
				{
#line 200
					D2WARNMSG((const char *)FogDataArraysSprintf(szMessage, "Animating tiles missing Rarity %d Tile - Style %d Seq %d", dwFrame, bCode, bSub));
				}
				if (i >= dwFound)
					i = 0;
				if (ptTile->nOrientation != 0)
				{
					if (ptTile->nOrientation != 13)
						ptFrame = RoomTileAddWall(ptRoom, NULL, nX, nY, dwCell, ahTiles[i], ptTile->nOrientation);
					else
						ptFrame = RoomTileAddShadow(ptRoom, NULL, nX, nY, dwCell, ahTiles[i]);
				}
				else
					ptFrame = RoomTileAddFloor(ptRoom, NULL, nX, nY, dwCell, ahTiles[i]);
				ptAnim->pptFrames[dwFrame] = ptFrame;
				ptFrame->dwFlags |= 8;
			}
		}
	}
}

void __fastcall DrlgRoomSyncAnimFrames(ROOM *ptFrom, ROOM *ptTo)
{
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILEANIM ptAnim;
	ROOM **pptNear;
	int nFrame;
	int nCount;
	int i;

	if (!ptTo)
		return;
	nFrame = 0;
	if (ptFrom)
	{
		nCount = ptFrom->nNumNear;
		pptNear = ptFrom->pptNear;
		for (i = 0; i < nCount; i++)
		{
			ptTiles = ptFrom->pptNear[i]->ptDrlgTiles;
			if (ptTiles && ptTiles->ptFirstAnim)
			{
				nFrame = ptFrom->pptNear[i]->ptDrlgTiles->ptFirstAnim->nAnimPos;
				break;
			}
		}
	}
	for (i = 0; i < ptTo->nNumNear; i++)
	{
		ptTiles = ptTo->pptNear[i]->ptDrlgTiles;
		if (!ptTiles)
			continue;
		for (ptAnim = ptTiles->ptFirstAnim; ptAnim; ptAnim = ptAnim->ptNext)
			ptAnim->nAnimPos = nFrame;
	}
}
