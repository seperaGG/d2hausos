#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DUNGEON\\Dungeon.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "DungeonLocal.h"
#include "../DRLG/Drlg.h"

int * __fastcall LvlTblsGetPortalLevels(int *pnCount);

void __fastcall DungeonRoomFree(void *pPool, GAMEROOM *ptRoom);

void __stdcall DungeonCalcTileToScreenOffset(int nA, int nB, int *pnOut1, int *pnOut2);

PTACT __stdcall DungeonNewAct(BYTE bAct, DWORD dwMapSeed, DWORD dwClient, DWORD dwGame, void *pPool,
	int nInitLevel, PFNDRLGROOMBUILT pfnRoomBuilt, PFNDRLGTOWNPLACED pfnTownPlaced)
{
	PTACT ptAct;
	DWORD i;

#line 15
	ptAct = (PTACT)D2ALLOCPOOL(pPool, sizeof(ACT));
	for (i = 0; i < sizeof(ACT) / sizeof(DWORD); i++)
		((DWORD *)ptAct)[i] = 0;
	ptAct->dwMapSeed = dwMapSeed;
	ptAct->dwIsClientAct = dwClient;
	ptAct->bActIndex = bAct;
	ptAct->pMemPool = pPool;
	if (dwClient)
		ptAct->ptDrlgAct = DrlgNewAct(ptAct, bAct, NULL, dwMapSeed, 0, 1, dwGame, pfnRoomBuilt,
			pfnTownPlaced);
	else
	{
		ptAct->dwInitLevelId = nInitLevel;
		ptAct->ptDrlgAct = DrlgNewAct(ptAct, bAct, NULL, dwMapSeed, nInitLevel, 0, dwGame, pfnRoomBuilt,
			pfnTownPlaced);
	}
	ptAct->ptEnv = EnvNew(pPool);
	return ptAct;
}

DWORD __stdcall DungeonFreeAct(PTACT ptAct)
{
	GAMEROOM *ptRoom;
	GAMEROOM *ptNext;

#line 42
	D2ASSERT(ptAct);
	if (ptAct->ptDrlgAct)
	{
		DrlgFreeAct(ptAct->ptDrlgAct);
		ptAct->ptDrlgAct = NULL;
	}
	for (ptRoom = ptAct->ptFirstRoom; ptRoom; ptRoom = ptNext)
	{
		ptNext = ptRoom->ptActNext;
		DungeonRoomFree(ptAct->pMemPool, ptRoom);
	}
	if (ptAct->ptEnv)
	{
		EnvFree(ptAct->pMemPool, ptAct->ptEnv);
		ptAct->ptEnv = NULL;
	}
#line 66
	return D2FREEPOOL(ptAct->pMemPool, ptAct);
}

void *__fastcall DungeonActGetPool(PTACT ptAct)
{
	return ptAct->pMemPool;
}

void __stdcall DungeonRoomSetWarpsDefAllowed(PTDUNGEONROOM ptRoom, int nValue)
{
#line 82
	D2ASSERT(ptRoom);
	DrlgRoomSetWarpsDefAllowed(ptRoom->ptDrlgRoom, nValue);
}

UNIT *__stdcall DungeonRoomGetWarpUnit(PTDUNGEONROOM ptRoom, int bExit, PTLVLWARPTXT *pptWarpDef)
{
	DUNGEONROOM *hRoomDest;
	UNIT *ptUnit;
	BYTE bExitBack;
	PTLVLWARPTXT ptWarpDef;

#line 90
	D2ASSERT(ptRoom);
	hRoomDest = DrlgRoomGetWarpDest(ptRoom->ptDrlgRoom, bExit, &bExitBack, &ptWarpDef);
#line 97
	D2ASSERT(hRoomDest);
	for (ptUnit = hRoomDest->ptUnitFirst; ptUnit; ptUnit = ptUnit->ptRoomNext)
	{
		if (ptUnit->eType == UNIT_ROOMTILE && ptUnit->dwClassId == bExitBack)
			break;
	}
	*pptWarpDef = ptWarpDef;
	return ptUnit;
}

PTLVLWARPTXT __stdcall DungeonRoomGetUnitWarpDef(PTDUNGEONROOM ptRoom, UNIT *ptUnit)
{
#line 122
	D2ASSERT(ptRoom);
	return DrlgRoomGetUnitWarpDef(ptRoom->ptDrlgRoom, ptUnit);
}

DWORD __stdcall DungeonRoomGetSecondTileArray(GAMEROOM *ptRoom, DWORD *pdwOut)
{
#line 130
	D2ASSERT(ptRoom);
#line 131
	D2ASSERT(ptRoom->ptTiles);
	*pdwOut = ptRoom->ptTiles->dwNumFloors;
	return ptRoom->ptTiles->dwFloors;
}

DWORD __stdcall DungeonRoomGetFirstTileArray(GAMEROOM *ptRoom, DWORD *pdwOut)
{
#line 144
	D2ASSERT(ptRoom);
	*pdwOut = ptRoom->ptTiles->dwNumWalls;
	return ptRoom->ptTiles->dwWalls;
}

DWORD __stdcall DungeonRoomGetThirdTileArray(GAMEROOM *ptRoom, DWORD *pdwOut)
{
#line 157
	D2ASSERT(ptRoom);
	*pdwOut = ptRoom->ptTiles->dwNumShadows;
	return ptRoom->ptTiles->dwShadows;
}

void __stdcall DungeonRoomCopyRects(GAMEROOM *ptRoom, void *ptCoords)
{
#line 169
	D2ASSERT(ptCoords);
#line 172
	D2ASSERT(ptRoom != NULL);
	memcpy(ptCoords, &ptRoom->tRect, sizeof(GAMERECT) * 2);
}

void __stdcall DungeonRoomGetNear(GAMEROOM *ptRoom, GAMEROOM ***pppNear, DWORD *pdwNumNear)
{
#line 183
	D2ASSERT(ptRoom);
	*pdwNumNear = ptRoom->dwNumNear;
	*pppNear = ptRoom->pptNear;
}

static __inline GAMEROOM *sAllocRoom(ACT *ptAct, ROOM *ptDrlgRoom, GAMERECT *ptRects,
	PTDRLGTILEARRAYS ptTiles, DWORD dwSeed, DWORD dwFlags)
{
	GAMEROOM *ptRoom;
	GAMEROOM *ptRoomAdj;
	DWORD i;
	void *pMemPool;
	DWORD nNear;

#line 192
	ptRoom = (GAMEROOM *)D2ALLOCPOOL(ptAct->pMemPool, 128);
	for (i = 0; i < 128 / 4; i++)
		((DWORD *)ptRoom)[i] = 0;
	ptRoom->dwFlags = dwFlags;
	SeedSet(&ptRoom->tSeed, dwSeed);
	ptRoom->ptDrlgRoom = ptDrlgRoom;
	memcpy(&ptRoom->tRect, ptRects, 2 * sizeof(GAMERECT));
	ptRoom->ptTiles = (PTDUNGEONTILEINFO)ptTiles;
	ptRoom->ptActNext = ptAct->ptFirstRoom;
	ptAct->ptFirstRoom = ptRoom;
	ptAct->dwRoomsAdded = 1;
	ptRoom->ptAct = ptAct;
	DrlgRoomSetGameRoom(ptDrlgRoom, (DUNGEONROOM *)ptRoom);
	pMemPool = ptAct->pMemPool;
	nNear = DrlgRoomGetNearCount(ptRoom->ptDrlgRoom);
#line 203
	ptRoom->pptNear = (GAMEROOM **)D2ALLOCPOOL(pMemPool, nNear * sizeof(GAMEROOM *));

	ptRoom->dwNumNear = DrlgRoomGetNearGameRooms(ptRoom->ptDrlgRoom, (DUNGEONROOM **)ptRoom->pptNear);
	for (i = 0; i < ptRoom->dwNumNear; i++)
	{
		ptRoomAdj = ptRoom->pptNear[i];
#line 211
		D2ASSERT(ptRoomAdj);
		if (ptRoomAdj != ptRoom)
			ptRoomAdj->dwNumNear = DrlgRoomGetNearGameRooms(ptRoomAdj->ptDrlgRoom,
				(DUNGEONROOM **)ptRoomAdj->pptNear);
	}
	return ptRoom;
}

typedef void (__fastcall *PFNDUNGEONROOMMADE)(GAMEROOM *ptRoom);

DUNGEONROOM *__fastcall DungeonNewRoom(ACT *ptAct, ROOM *ptDrlgRoom, GAMERECT *ptRects,
	PTDRLGTILEARRAYS ptTiles, DWORD dwSeed, DWORD dwFlags)
{
	GAMEROOM *ptRoom;
	PFNDUNGEONROOMMADE pfnRoomMade;

#line 228
	D2ASSERT(ptAct);
	ptRoom = sAllocRoom(ptAct, ptDrlgRoom, ptRects, ptTiles, dwSeed, dwFlags);
	CollisnNew(ptAct->pMemPool, ptRoom);
	pfnRoomMade = (PFNDUNGEONROOMMADE)ptAct->dwRoomMadeCallback;
	if (pfnRoomMade)
		pfnRoomMade(ptRoom);
	return (DUNGEONROOM *)ptRoom;
}

DWORD __stdcall DungeonRoomsOverlap(GAMEROOM *ptFirst, GAMEROOM *ptSecond)
{
#line 265
	D2ASSERT(ptFirst);
#line 268
	D2ASSERT(ptSecond);
	if (ptFirst->tRect2.nX > ptSecond->tRect2.nX + ptSecond->tRect2.nWidth)
		return 0;
	if (ptFirst->tRect2.nX + ptFirst->tRect2.nWidth < ptSecond->tRect2.nX)
		return 0;
	if (ptFirst->tRect2.nY > ptSecond->tRect2.nY + ptSecond->tRect2.nHeight)
		return 0;
	return (DWORD)(ptFirst->tRect2.nY + ptFirst->tRect2.nHeight >= ptSecond->tRect2.nY);
}

DWORD __stdcall DungeonRoomTestPointInSecondRect(GAMEROOM *hRoom, int nX, int nY)
{
#line 307
	D2ASSERT(hRoom);
	if (nX < hRoom->tRect2.nX || nX >= hRoom->tRect2.nX + hRoom->tRect2.nWidth)
		return 0;
	if (nY < hRoom->tRect2.nY || nY >= hRoom->tRect2.nY + hRoom->tRect2.nHeight)
		return 0;
	return 1;
}

DWORD __stdcall DungeonRoomGetSecondRectShape(GAMEROOM *pPrimary, DWORD dwUnused)
{
	(void)dwUnused;
#line 326
	D2ASSERT(pPrimary);
	if (pPrimary->tRect2.nX > pPrimary->tRect2.nX + pPrimary->tRect2.nWidth)
		return 0;
	if (pPrimary->tRect2.nX + pPrimary->tRect2.nWidth < pPrimary->tRect2.nX)
		return 0;
	if (pPrimary->tRect2.nY > pPrimary->tRect2.nY + pPrimary->tRect2.nHeight)
		return 0;
	if (pPrimary->tRect2.nY + pPrimary->tRect2.nHeight < pPrimary->tRect2.nY)
		return 0;
	if (pPrimary->tRect2.nX == pPrimary->tRect2.nX + pPrimary->tRect2.nWidth)
		return 1;
	if (pPrimary->tRect2.nY == pPrimary->tRect2.nY + pPrimary->tRect2.nHeight)
		return 3;
	return 4;
}

GAMEROOM * __stdcall DungeonTestRoomGame(ACT *hRoom, int nX, int nY)
{
	GAMEROOM *ptRoom;

	for (ptRoom = hRoom->ptFirstRoom; ptRoom; ptRoom = ptRoom->ptActNext)
	{
		if (nX >= ptRoom->tRect2.nX && nX < ptRoom->tRect2.nX + ptRoom->tRect2.nWidth &&
			nY >= ptRoom->tRect2.nY && nY < ptRoom->tRect2.nY + ptRoom->tRect2.nHeight)
			return ptRoom;
	}
	return NULL;
}

GAMEROOM * __stdcall DungeonRoomFindNearByPoint(GAMEROOM *ptRoom, int nX, int nY)
{
	DWORD i;
	GAMEROOM *pTemp;

#line 378
	D2ASSERT(ptRoom);
	i = 0;
	if (i < ptRoom->dwNumNear)
	while (1)
	{
		pTemp = ptRoom->pptNear[i];
#line 382
		D2ASSERT(pTemp);
		if (nX >= pTemp->tRect2.nX && nX < pTemp->tRect2.nX + pTemp->tRect2.nWidth &&
			nY >= pTemp->tRect2.nY && nY < pTemp->tRect2.nY + pTemp->tRect2.nHeight)
			return pTemp;
		if (++i >= ptRoom->dwNumNear)
			break;
	}
	return NULL;
}

typedef DWORD (__stdcall *PFNROOMCALLBACK)(GAMEROOM *ptNear, void *pContext);

void __stdcall DungeonRoomForEachNear(GAMEROOM *ptRoom, PFNROOMCALLBACK pfnRoomCallback, void *pContext)
{
	DWORD i;

#line 395
	D2ASSERT(ptRoom);
#line 397
	D2ASSERTMSG(!IsBadCodePtr((FARPROC)pfnRoomCallback), "pfnRoomCallback");
	for (i = 0; i < ptRoom->dwNumNear; i++)
	{
		if (!pfnRoomCallback(ptRoom->pptNear[i], pContext))
			return;
	}
}

void __stdcall DungeonRoomGetScaledBounds(GAMEROOM *hRoom, GAMERECTLTRB *prc)
{
	GAMEROOM *pRoom = hRoom;
	int nScratch;
	int *pnScratch = &nScratch;

#line 411
	D2ASSERT(pRoom);
	DungeonCalcTileToScreenOffset(pRoom->tRect2.nX, pRoom->tRect2.nY, pnScratch, &prc->top);
	DungeonCalcTileToScreenOffset(pRoom->tRect2.nX, pRoom->tRect2.nHeight + pRoom->tRect2.nY,
		&prc->left, pnScratch);
	DungeonCalcTileToScreenOffset(pRoom->tRect2.nX + pRoom->tRect2.nWidth, pRoom->tRect2.nY,
		&prc->right, pnScratch);
	DungeonCalcTileToScreenOffset(pRoom->tRect2.nX + pRoom->tRect2.nWidth,
		pRoom->tRect2.nY + pRoom->tRect2.nHeight, pnScratch, &prc->bottom);
#line 421
	D2ASSERT(prc->top <= prc->bottom);
#line 422
	D2ASSERT(prc->left <= prc->right);
}

void __stdcall DungeonRoomGetRectLTRB(GAMEROOM *pRoom, GAMERECTLTRB *prc)
{
#line 430
	D2ASSERT(pRoom);
	prc->top = pRoom->tRect.nY;
	prc->bottom = pRoom->tRect.nY + pRoom->tRect.nHeight;
	prc->left = pRoom->tRect.nX;
	prc->right = pRoom->tRect.nX + pRoom->tRect.nWidth;
#line 438
	D2ASSERT(prc->top <= prc->bottom);
#line 439
	D2ASSERT(prc->left <= prc->right);
}

void __stdcall DungeonRoomGetLight(PTDUNGEONROOM ptRoom, BYTE *pbIntensity, BYTE *pbRed, BYTE *pbGreen,
	BYTE *pbBlue)
{
	DrlgRoomGetLight(ptRoom->ptDrlgRoom, pbIntensity, pbRed, pbGreen, pbBlue);
}

GAMEROOM * __stdcall DungeonFindRoomGame(GAMEROOM *hRoomDest, int nX, int nY)
{
	DWORD i;
	DWORD dwNumNear;
	GAMEROOM **pptNear;

	if (!hRoomDest)
		return NULL;
	if (nX >= hRoomDest->tRect.nX && nX < hRoomDest->tRect.nX + hRoomDest->tRect.nWidth &&
		nY >= hRoomDest->tRect.nY && nY < hRoomDest->tRect.nY + hRoomDest->tRect.nHeight)
		return hRoomDest;
	dwNumNear = hRoomDest->dwNumNear;
	pptNear = hRoomDest->pptNear;
	for (i = 0; i < dwNumNear; i++)
	{
		GAMEROOM *ptNear = pptNear[i];

		if (!ptNear)
			continue;
		if (nX >= ptNear->tRect.nX && nX < ptNear->tRect.nX + ptNear->tRect.nWidth &&
			nY >= ptNear->tRect.nY && nY < ptNear->tRect.nY + ptNear->tRect.nHeight)
			return pptNear[i];
	}
	return NULL;
}

GAMEROOM * __stdcall DungeonActFindRoomByPoint(PTACT hAct, int nX, int nY)
{
	GAMEROOM *ptRoom;

#line 481
	D2ASSERT(hAct);
	for (ptRoom = hAct->ptFirstRoom; ptRoom; ptRoom = ptRoom->ptActNext)
	{
		if (ptRoom && nX >= ptRoom->tRect.nX && nX < ptRoom->tRect.nX + ptRoom->tRect.nWidth &&
			nY >= ptRoom->tRect.nY && nY < ptRoom->tRect.nY + ptRoom->tRect.nHeight)
			return ptRoom;
	}
	return NULL;
}

BOOL __fastcall DungeonCoordsContain(PTCOLCOORDS ptCoords, int nX, int nY)
{
#line 501
	D2ASSERT(ptCoords);
	if (nX >= ptCoords->nMinX && nX < ptCoords->nMinX + ptCoords->nSizeGameX &&
		nY >= ptCoords->nMinY && nY < ptCoords->nMinY + ptCoords->nSizeGameY)
		return TRUE;
	return FALSE;
}

GAMEROOM *__stdcall DungeonGetWarpRoom(PTACT ptAct, int nLevelId, DWORD dwArg, int *pnX, int *pnY)
{
#line 516
	D2ASSERT(ptAct);
	return (GAMEROOM *)DrlgWarpGetRoom(ptAct->ptDrlgAct, nLevelId, (int)dwArg, pnX, pnY);
}

GAMEROOM *__stdcall DungeonGetActStartRoom(PTACT ptAct, int nLevelId, DWORD dwArg, int *pnX, int *pnY,
	DWORD dwArg6)
{
	GAMEROOM *ptRoom;
	int nX;
	int nY;
	int nSpot[2];

#line 527
	D2ASSERT(ptAct);
	nX = 0;
	nY = 0;
	ptRoom = (GAMEROOM *)DrlgWarpGetRoom(ptAct->ptDrlgAct, nLevelId, (int)dwArg, &nX, &nY);
	if (ptRoom) {
		EnvTileToSubtile(&nX, &nY);
		nSpot[0] = nX + 3;
		nSpot[1] = nY + 3;
#line 539
		D2ASSERTMSG((DWORD)D2CommonCollisnFindSpawnPointNear(ptRoom, nSpot, dwArg6, 7177, 0), "Unable to find spawn location for act start");
		*pnX = nSpot[0];
		*pnY = nSpot[1];
		return DungeonFindRoomGame(ptRoom, *pnX, nSpot[1]); }
	return NULL;
}

DWORD __stdcall DungeonRoomGetUnitList(PTDUNGEONROOM ptRoom)
{
#line 556
	D2ASSERT(ptRoom);
	D2CommonUnitRoomSortByTileY(ptRoom);
	return (DWORD)ptRoom->ptUnitFirst;
}

void __stdcall DungeonRoomIncPlayerUnits(PTDUNGEONROOM ptRoom)
{
	if (ptRoom == NULL)
		return;
	ptRoom->nPlayerUnits++;
}

DWORD __stdcall DungeonRoomGetPlayerUnits(PTDUNGEONROOM ptRoom)
{
	if (ptRoom == NULL)
		return 0;
	return (DWORD)ptRoom->nPlayerUnits;
}

void __stdcall DungeonRoomDecPlayerUnits(PTDUNGEONROOM ptRoom)
{
	if (ptRoom == NULL)
		return;
#line 607
	D2ASSERT(ptRoom->nPlayerUnits > 0);
	ptRoom->nPlayerUnits--;
}

PTUNIT * __fastcall DungeonRoomGetUnitListHead(PTDUNGEONROOM ptRoom)
{
#line 617
	D2ASSERT(ptRoom);
	return (PTUNIT *)&ptRoom->ptUnitFirst;
}

PTUNIT * __fastcall DungeonRoomGetChangedListHead(PTDUNGEONROOM ptRoom, DWORD fMarkDirty)
{
#line 656
	D2ASSERT(ptRoom);
	if (fMarkDirty)
		*(DWORD *)((BYTE *)ptRoom->pAct + 40) = 1;
	return (PTUNIT *)&ptRoom->ptUnitChanged;
}

PTPRESETUNIT __stdcall DungeonRoomGetPresetUnits(PTDUNGEONROOM ptRoom)
{
#line 670
	D2ASSERT(ptRoom);
	return DrlgRoomGetPresetUnits(ptRoom->ptDrlgRoom);
}

struct COL * __fastcall DungeonRoomGetCollision(GAMEROOM *ptRoom)
{
#line 680
	D2ASSERT(ptRoom);
	return ptRoom->ptCol;
}

void __fastcall DungeonRoomSetCollision(GAMEROOM *ptRoom, struct COL *ptCol)
{
#line 689
	D2ASSERT(ptRoom);
	ptRoom->ptCol = ptCol;
}

void __stdcall DungeonActFindRoomA(PTACT ptAct, DWORD dwLevelId, DWORD dwX, DWORD dwY,
	PTDUNGEONROOM ptRoom)
{
#line 698
	D2ASSERT(ptAct);
	DrlgRoomEnterClient(ptAct->ptDrlgAct, dwLevelId, (int)dwX, (int)dwY,
		ptRoom ? ptRoom->ptDrlgRoom : NULL);
}

void __stdcall DungeonActFindRoomB(PTACT ptAct, DWORD dwLevelId, DWORD dwX, DWORD dwY,
	PTDUNGEONROOM ptRoom)
{
#line 716
	D2ASSERT(ptAct);
	DrlgRoomLeaveClient(ptAct->ptDrlgAct, dwLevelId, (int)dwX, (int)dwY,
		ptRoom ? ptRoom->ptDrlgRoom : NULL);
}

void __stdcall DungeonRoomSwitch(PTDUNGEONROOM ptFrom, PTDUNGEONROOM ptTo)
{
	ROOM *ptA = NULL;
	ROOM *ptB = NULL;

	if (ptFrom)
		ptA = ptFrom->ptDrlgRoom;
	if (ptTo)
		ptB = ptTo->ptDrlgRoom;
	DrlgRoomSwitch(ptA, ptB);
}

PTDUNGEONROOM __stdcall DungeonActGetLevelRoom(PTACT ptAct, DWORD dwX, DWORD dwY)
{
#line 758
	D2ASSERT(ptAct);
	return DrlgLevelGetRoom(ptAct->ptDrlgAct, (int)dwX, (int)dwY);
}

GAMEROOM * __stdcall DungeonActGetFirstRoom(PTACT ptAct)
{
#line 769
	D2ASSERT(ptAct);
	return ptAct->ptFirstRoom;
}

DWORD __stdcall DungeonRoomGetLevelId(PTDUNGEONROOM ptRoom)
{
	if (!ptRoom)
		return 0;
	return DrlgRoomGetLevelId(ptRoom->ptDrlgRoom);
}

int __stdcall DungeonRoomGetVisLevel(PTDUNGEONROOM ptRoom, int nIndex)
{
	if (!ptRoom)
		return 0;
	return DrlgRoomGetVisLevel(ptRoom->ptDrlgRoom, nIndex);
}

DWORD __stdcall DungeonRoomGetLevelIdChecked(PTDUNGEONROOM ptRoom)
{
	if (!ptRoom)
		return 0;
	return DrlgRoomGetLevelIdChecked(ptRoom->ptDrlgRoom);
}

BOOL __stdcall DungeonRoomHasExitFlags(PTDUNGEONROOM ptRoom)
{
#line 838
	D2ASSERT(ptRoom);
	return DrlgRoomHasExitFlags(ptRoom->ptDrlgRoom);
}

const char *__stdcall DungeonRoomGetFileName(PTDUNGEONROOM ptRoom)
{
	if (!ptRoom)
		return NULL;
	return DrlgRoomGetFileName(ptRoom->ptDrlgRoom);
}

void __stdcall DungeonRoomAddNode(PTGAMEROOM ptRoom, DWORD dwType, DWORD dwUnitId)
{
	PTDUNGEONROOMNODE ptNode;

#line 863
	D2ASSERT(ptRoom);
#line 865
	ptNode = (PTDUNGEONROOMNODE)D2ALLOCPOOL(ptRoom->ptAct->pMemPool, sizeof(DUNGEONROOMNODE));
	ptNode->dwType = dwType;
	ptNode->dwUnitId = dwUnitId;
	ptNode->pNext = ptRoom->ptUnitNodes;
	ptRoom->ptUnitNodes = ptNode;
	ptRoom->ptAct->dwRoomNodesAdded = 1;
}

void __stdcall DungeonRoomFreeNodes(PTGAMEROOM ptRoom)
{
	PTACT ptAct;
	PTDUNGEONROOMNODE ptNode;
	PTDUNGEONROOMNODE ptNext;
	void *pPool;

#line 877
	D2ASSERT(ptRoom);
	ptAct = ptRoom->ptAct;
	pPool = ptAct->pMemPool;
	ptNext = ptRoom->ptUnitNodes;
	if (ptNext)
	{
		do
		{
			ptNode = ptNext;
			ptNext = ptNode->pNext;
#line 889
			D2FREEPOOL(pPool, ptNode);
		} while (ptNext);
		ptRoom->ptUnitNodes = ptNext;
	}
}

PTDUNGEONROOMNODE __stdcall DungeonRoomGetNodes(PTGAMEROOM ptRoom)
{
#line 899
	D2ASSERT(ptRoom);
	return ptRoom->ptUnitNodes;
}

DUNGEONROOM *__stdcall DungeonActGetFirstActiveRoom(PTACT ptAct)
{
#line 930
	D2ASSERT(ptAct);
	return DrlgActGetFirstActiveRoom(ptAct->ptDrlgAct);
}

DUNGEONROOM *__stdcall DungeonActGetNextActiveRoom(PTACT ptAct, PTDUNGEONROOM ptRoom)
{
#line 939
	D2ASSERT(ptAct);
#line 942
	D2ASSERT(ptRoom);
	return DrlgActGetNextActiveRoom(ptAct->ptDrlgAct, ptRoom->ptDrlgRoom);
}

BOOL __stdcall DungeonRoomCanRelease(PTACT ptAct, PTDUNGEONROOM ptRoom)
{
#line 954
	D2ASSERT(ptAct);
#line 955
	D2ASSERT(ptRoom);
	return DrlgRoomCanRelease(ptRoom->ptDrlgRoom);
}

BOOL __stdcall DungeonRoomIsIdle(PTDUNGEONROOM ptRoom)
{
	return DrlgRoomGetState(ptRoom->ptDrlgRoom) >= 3;
}

BOOL __stdcall DungeonRoomNearAllFlagged(PTGAMEROOM ptRoom)
{
	DWORD i;

#line 971
	D2ASSERT(ptRoom);
	if (ptRoom->dwNumNear != (DWORD)DrlgRoomGetNearCount(ptRoom->ptDrlgRoom))
		return FALSE;
	for (i = 0; i < ptRoom->dwNumNear; i++)
	{
		if (!(ptRoom->pptNear[i]->dwFlags & 1))
			return FALSE;
	}
	return TRUE;
}

DWORD __stdcall DungeonRoomTestPopulated(PTGAMEROOM ptRoom)
{
#line 1035
	D2ASSERT(ptRoom);
	return ptRoom->dwFlags & 2;
}

void __stdcall DungeonRoomSetPopulated(PTGAMEROOM ptRoom, BOOL fSet)
{
#line 1044
	D2ASSERT(ptRoom);
	if (fSet)
		ptRoom->dwFlags |= 2;
	else
		ptRoom->dwFlags &= ~2;
}

static inline void DungeonRoomValidateClients(PTGAMEROOM ptRoom)
{
	DWORD i;

	for (i = 0; i < ptRoom->nNumClients; i++)
	{
#line 1071
		D2ASSERT(ptRoom->phClients[i]);
	}
}

static void __fastcall DungeonRoomSortClients(PTGAMEROOM ptRoom)
{
	int i;
	int nPass;

	DungeonRoomValidateClients(ptRoom);
	nPass = (int)ptRoom->nNumClients - 1;
	if (nPass > 0)
	{
		for (;;)
		{
			BOOL fSwapped = FALSE;

			for (i = 0; i < nPass; i++)
			{
				if (ptRoom->phClients[i] > ptRoom->phClients[i + 1])
				{
					GAMEROOM *ptTemp = ptRoom->phClients[i];

					ptRoom->phClients[i] = ptRoom->phClients[i + 1];
					ptRoom->phClients[i + 1] = ptTemp;
					fSwapped = TRUE;
				}
			}
			if (!fSwapped)
				return;
			nPass--;
			if (nPass <= 0)
				break;
		}
	}
	DungeonRoomValidateClients(ptRoom);
}

void __stdcall DungeonRoomAddClient(PTGAMEROOM ptRoom, GAMEROOM *hClient)
{
#line 1103
	D2ASSERT(ptRoom);
#line 1104
	D2ASSERT(hClient);
	DungeonRoomValidateClients(ptRoom);
#line 1107
	D2ASSERT(ptRoom->nNumClients <= ptRoom->nMaxClients);
	if (ptRoom->nNumClients == ptRoom->nMaxClients)
	{
		DWORD nNewMax = ptRoom->nMaxClients + 4;

		ptRoom->nMaxClients = nNewMax;
#line 1110
		ptRoom->phClients = (GAMEROOM **)D2REALLOCPOOL(ptRoom->ptAct->pMemPool, ptRoom->phClients,
			nNewMax * sizeof(GAMEROOM *));
	}
	ptRoom->phClients[ptRoom->nNumClients] = hClient;
	ptRoom->nNumClients++;
	DungeonRoomSortClients(ptRoom);
}

void __stdcall DungeonRoomRemoveClient(PTGAMEROOM ptRoom, GAMEROOM *hClient)
{
	DWORD i;

#line 1123
	D2ASSERT(ptRoom);
#line 1124
	D2ASSERT(hClient);
	DungeonRoomValidateClients(ptRoom);
	for (i = 0; i < ptRoom->nNumClients; i++)
	{
		if (ptRoom->phClients[i] == hClient)
			break;
	}
#line 1132
	D2ASSERT(i < ptRoom->nNumClients);
	ptRoom->phClients[i] = ptRoom->phClients[ptRoom->nNumClients - 1];
	ptRoom->nNumClients--;
	DungeonRoomSortClients(ptRoom);
}

DWORD __stdcall DungeonRoomTickEmptyCounter(PTGAMEROOM ptRoom)
{
#line 1162
	D2ASSERT(ptRoom);
	if (ptRoom->nNumClients != 0)
		ptRoom->dwEmptyCount = 0;
	else
		ptRoom->dwEmptyCount++;
	return ptRoom->dwEmptyCount;
}

BOOL __fastcall DungeonRoomHasOtherClients(PTGAMEROOM ptRoom, PTGAMEROOM ptOther)
{
	int i;
	int j;

	for (i = 0; i < (int)ptOther->nNumClients; i++)
	{
		for (j = 0; j < (int)ptRoom->nNumClients; j++)
		{
			if (ptRoom->phClients[j] == ptOther->phClients[i])
				break;
		}
		if (j >= (int)ptRoom->nNumClients)
			return TRUE;
	}
	return FALSE;
}

void __fastcall DungeonRoomFree(void *pPool, GAMEROOM *ptRoom)
{
	UNIT *ptUnit;

	for (ptUnit = ptRoom->ptUnitFirst; ptUnit; ptUnit = ptRoom->ptUnitFirst)
	{
		if (!(ptUnit->dwFlags0 & 4194304))
			ptUnit->dwFlags |= 32;
		ptUnit->dwFlags0 |= 8388608;
		D2CommonUnitRoomLeave(ptUnit);
	}
	CollisnFree(pPool, ptRoom);
	DungeonRoomFreeNodes(ptRoom);
	if (ptRoom->phClients)
	{
#line 1213
		D2FREEPOOL(ptRoom->ptAct->pMemPool, ptRoom->phClients);
	}
#line 1215
	D2FREEPOOL(pPool, ptRoom->pptNear);
#line 1216
	D2FREEPOOL(pPool, ptRoom);
}

void __fastcall DungeonRoomClearTiles(PTGAMEROOM ptRoom)
{
#line 1223
	D2ASSERT(ptRoom);
	ptRoom->ptTiles = NULL;
}

void __stdcall DungeonActRemoveRoom(PTACT ptAct, PTGAMEROOM ptRoom)
{
	GAMEROOM *ptRoomCurr;
	GAMEROOM *ptRoomPrev;
	GAMEROOM *ptNear;
	int i;
	int j;

#line 1234
	D2ASSERT(ptRoom);
	ptRoomPrev = NULL;
	for (ptRoomCurr = ptAct->ptFirstRoom; ptRoomCurr; ptRoomCurr = ptRoomCurr->ptActNext)
	{
		if (ptRoomCurr == ptRoom)
			break;
		ptRoomPrev = ptRoomCurr;
	}
#line 1245
	D2ASSERT(ptRoomCurr);
	if (ptRoomPrev)
		ptRoomPrev->ptActNext = ptRoomCurr->ptActNext;
	else
		ptAct->ptFirstRoom = ptRoomCurr->ptActNext;
	for (i = 0; i < (int)ptRoom->dwNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear == ptRoom)
			continue;
		for (j = 0; j < (int)ptNear->dwNumNear; j++)
		{
			if (ptNear->pptNear[j] == ptRoom)
			{
				ptNear->pptNear[j] = ptNear->pptNear[ptNear->dwNumNear - 1];
				ptNear->dwNumNear--;
				break;
			}
		}
	}
	DrlgRoomReleaseGameRoom(ptRoom->ptDrlgRoom, (BOOL)ptAct->dwIsClientAct, ptRoom->dwFlags);
	DungeonRoomFree(ptAct->pMemPool, ptRoom);
}

void __stdcall DungeonRoomChangeLevel(PTDUNGEONROOM ptFrom, PTDUNGEONROOM ptTo)
{
	ROOM *ptDrlgFrom = ptFrom ? ptFrom->ptDrlgRoom : NULL;
	ROOM *ptDrlgTo = ptTo ? ptTo->ptDrlgRoom : NULL;

	DrlgRoomChangeLevel(ptDrlgFrom, ptDrlgTo);
}

void __stdcall DungeonActFreeIdleLevels(PTACT ptAct)
{
	DrlgActFreeIdleLevels(ptAct->ptDrlgAct);
}

DWORD __stdcall DungeonRoomGetKindFlag(PTDUNGEONROOM ptRoom)
{
#line 1304
	D2ASSERT(ptRoom);
	return DrlgRoomGetKindFlag(ptRoom->ptDrlgRoom);
}

PTENV __fastcall DungeonActGetEnv(PTACT ptAct)
{
#line 1313
	D2ASSERT(ptAct);
	return ptAct->ptEnv;
}

DWORD __stdcall DungeonActGetDrlgAct(PTACT ptAct)
{
#line 1322
	D2ASSERT(ptAct);
	return (DWORD)ptAct->ptDrlgAct;
}

DWORD __stdcall DungeonActGetMapSeed(PTACT ptAct)
{
	if (ptAct == NULL)
		return 0;
	return ptAct->dwMapSeed;
}

struct ROOM * __fastcall DungeonRoomGetDrlgRoom(PTDUNGEONROOM ptRoom)
{
	return ptRoom->ptDrlgRoom;
}

BOOL __stdcall DungeonLevelIsTown(int nLevelId)
{
	return DrlgLevelIsTown(nLevelId);
}

BOOL __stdcall DungeonRoomIsTown(PTDUNGEONROOM ptRoom)
{
	int nLevelId;

	nLevelId = !ptRoom ? LEVEL_ID_NONE : (int)DrlgRoomGetLevelId(ptRoom->ptDrlgRoom);
	return DrlgLevelIsTown(nLevelId);
}

BOOL __stdcall DungeonRoomIsNone(PTDUNGEONROOM ptRoom)
{
	int nLevelId;

	nLevelId = !ptRoom ? LEVEL_ID_NONE : (int)DrlgRoomGetLevelId(ptRoom->ptDrlgRoom);
	return DrlgLevelIsNone(nLevelId);
}

DWORD __stdcall DungeonRoomTestSkipsChangedList(PTDUNGEONROOM ptRoom)
{
	return ptRoom->dwFlags & 4;
}

static const DWORD sgcadwActTownLevelId[NUM_ACTS] =
{
	1, 40, 75, 103,
};

DWORD __stdcall DungeonActGetTownLevelId(BYTE bAct)
{
#line 1393
	D2ASSERT(bAct < NUM_ACTS);
	return sgcadwActTownLevelId[bAct];
}

DWORD __stdcall DungeonRoomGetOutdoorFlag(PTDUNGEONROOM ptRoom)
{
#line 1404
	D2ASSERT(ptRoom);
	return DrlgRoomGetOutdoorFlag(ptRoom->ptDrlgRoom);
}

int __stdcall DungeonActCountLevelRooms(PTACT ptAct, int nLevelId)
{
	return DrlgActCountLevelRooms(ptAct->ptDrlgAct, nLevelId);
}

int *__stdcall DungeonRoomGetLevelWarpX(PTDUNGEONROOM ptRoom)
{
#line 1424
	D2ASSERT(ptRoom);
	return DrlgRoomGetLevelWarpX(ptRoom->ptDrlgRoom);
}

void __stdcall DungeonRoomShowWarpTiles(PTDUNGEONROOM ptRoom, int nExit)
{
#line 1435
	D2ASSERT(ptRoom);
	DrlgRoomShowWarpTiles(ptRoom->ptDrlgRoom, nExit);
}

void __stdcall DungeonRoomHideWarpTiles(PTDUNGEONROOM ptRoom, int nExit)
{
#line 1446
	D2ASSERT(ptRoom);
	DrlgRoomHideWarpTiles(ptRoom->ptDrlgRoom, nExit);
}

void __stdcall DungeonRoomUpdateDoors(PTDUNGEONROOM ptRoom, int nX, int nY, BOOL fReopen)
{
#line 1457
	D2ASSERT(ptRoom);
	PresetRoomUpdateDoors(ptRoom->ptDrlgRoom, nX, nY, fReopen);
}

void __stdcall DungeonRoomGetPoints(PTDUNGEONROOM ptRoom, struct GAMECOORD **pptPoints, int *pnCount)
{
	PresetRoomGetPoints(ptRoom->ptDrlgRoom, pptPoints, pnCount);
}

int __stdcall DungeonGetLogicValue(PTGAMEROOM ptRoom, int nX, int nY)
{
	GAMEROOM *ptFound;

	ptFound = DungeonFindRoomGame(ptRoom, nX, nY);
	if (!ptFound)
		return 0;
	return DrlgRoomGetLogicValue(ptFound->ptDrlgRoom, nX, nY);
}

PTDRLGLOGICNODE __stdcall DungeonRoomGetLogicNode(PTDUNGEONROOM ptRoom, int nX, int nY)
{
	return DrlgRoomGetLogicNode(ptRoom->ptDrlgRoom, nX, nY);
}

PTDRLGLOGICNODE __stdcall DungeonRoomGetLogicList(PTDUNGEONROOM ptRoom)
{
	return DrlgRoomGetLogicList(ptRoom->ptDrlgRoom);
}

void __stdcall DungeonGetPortalLevels(void *pPool, DWORD dwMask, int **ppnLevels, int *pnCount)
{
	int anLevels[32];
	int *pnPortals;
	int *pnOut;
	int nMax;
	int nCount;
	DWORD dwBit;
	int i;

	dwBit = 1;
	nCount = 0;
	pnPortals = LvlTblsGetPortalLevels(&nMax);
#line 1505
	D2ASSERT(nMax <= 32);
	if (nMax > 0)
	{
		for (i = nMax; i; i--)
		{
			if (dwMask & dwBit)
				anLevels[nCount++] = *pnPortals;
			dwBit <<= 1;
			pnPortals++;
		}
	}
	if (nCount)
	{
#line 1515
		pnOut = (int *)D2ALLOCPOOL(pPool, nCount * sizeof(int));
		*ppnLevels = pnOut;
		memcpy(pnOut, anLevels, nCount * sizeof(int));
	}
	else
		*ppnLevels = NULL;
	*pnCount = nCount;
}

DWORD __stdcall DungeonGetPortalLevelBit(int nLevelId)
{
	int *pnPortals;
	int nMax;
	int i;

	pnPortals = LvlTblsGetPortalLevels(&nMax);
	for (i = 0; i < nMax; pnPortals++, i++)
	{
		if (nLevelId == *pnPortals)
			return 1 << i;
	}
	return 0;
}

DWORD __stdcall DungeonActGetInitialLevelId(PTACT ptAct)
{
#line 1547
	D2ASSERT(ptAct);
	return ptAct->dwInitLevelId;
}

int __stdcall DungeonActGetDrawnLevel(PTACT ptAct)
{
	if (ptAct)
		return DrlgActGetDrawnLevel(ptAct->ptDrlgAct);
	return 0;
}

void __stdcall DungeonRoomSetHold(PTDUNGEONROOM ptRoom, BOOL fClear)
{
	if (ptRoom)
		DrlgRoomSetHold(ptRoom->ptDrlgRoom, fClear);
}

void __stdcall DungeonRoomAnimateNear(PTDUNGEONROOM ptRoom)
{
	if (ptRoom)
		DrlgRoomAnimateNear(ptRoom->ptDrlgRoom);
}

void __stdcall DungeonActSyncAnimFrames(PTACT ptAct, PTGAMEROOM ptFrom, PTGAMEROOM ptTo)
{
	GAMEROOM *ptRoom;
	ROOM *ptDrlgFrom;
	ROOM *ptDrlgTo;

	ptDrlgFrom = NULL;
	ptDrlgTo = NULL;
	if (ptFrom)
	{
		for (ptRoom = ptAct->ptFirstRoom; ptRoom; ptRoom = ptRoom->ptActNext)
		{
			if (ptRoom == ptFrom)
			{
				ptDrlgFrom = ptFrom->ptDrlgRoom;
				break;
			}
		}
	}
	if (ptTo)
		ptDrlgTo = ptTo->ptDrlgRoom;
	DrlgRoomSyncAnimFrames(ptDrlgFrom, ptDrlgTo);
}

void __stdcall DungeonActSetRoomMadeCallback(PTACT ptAct, DWORD dwValue)
{
#line 1622
	D2WARN(ptAct);
	ptAct->dwRoomMadeCallback = dwValue;
}

void __stdcall DungeonRoomRingBufferPush(PTGAMEROOM hRoom, DWORD dwValue)
{
#line 1629
	D2ASSERT(hRoom);
	hRoom->adwRing[hRoom->bRingIndex] = dwValue;
	hRoom->bRingIndex = (hRoom->bRingIndex + 1) & 3;
}

void __stdcall DungeonCalcSubtileToScreen(int *pnA, int *pnB)
{
	int nA, nB;

	nA = *pnA;
	nB = *pnB;
	*pnA = (nA - nB) * 16;
	*pnB = (nA + nB) * 8;
}

void __stdcall DungeonCalcScaledSubtileToScreen(int *pnA, int *pnB)
{
	int nA, nB;

	nA = *pnA;
	nB = *pnB;
	*pnA = (nA - nB) >> 1;
	*pnB = (nA + nB) >> 2;
}

void __stdcall DungeonCalcScreenToTile(int *pnA, int *pnB)
{
	int nA, nB;

	nA = *pnA;
	nB = *pnB;
	*pnA = (2 * nB + nA) / 160;
	*pnB = (2 * nB - nA) / 160;
}

void __stdcall DungeonCalcTileToScreen(int *pnA, int *pnB)
{
	int nA, nB;

	nA = *pnA;
	nB = *pnB;
	*pnA = (nA - nB) * 80;
	*pnB = ((nA + nB) * 80) >> 1;
}

void __stdcall DungeonCalcScreenToTileFloor(int nA, int nB, int *pnOut1, int *pnOut2)
{
	*pnOut1 = 2 * nB + nA;
	*pnOut2 = 2 * nB - nA;

	if (*pnOut1 < 0) *pnOut1 = *pnOut1 / 160 - 1;
	else *pnOut1 = *pnOut1 / 160;

	if (*pnOut2 < 0) *pnOut2 = *pnOut2 / 160 - 1;
	else *pnOut2 = *pnOut2 / 160;
}

#pragma auto_inline(off)
void __stdcall DungeonCalcTileToScreenOffset(int nA, int nB, int *pnOut1, int *pnOut2)
{
	*pnOut1 = (nA - nB) * 80;
	*pnOut2 = (nA + nB) * 40;
	*pnOut1 -= 80;
	*pnOut2 += 80;
}
#pragma auto_inline(on)
