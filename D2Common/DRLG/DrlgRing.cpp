#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\DrlgRing.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "FogImports.h"

static DWORD sgdwClientCount = 0;
static DWORD sgdwClientBuilt = 0;
static DWORD sgdwServerCount = 0;
static DWORD sgdwServerBuilt = 0;

typedef void (__fastcall *PFNROOMSTATE)(ROOM *ptRoom);

static __inline void sRoomUnlink(ROOM *ptRoom)
{
	if (ptRoom->ptRingPrev && ptRoom->ptRingNext)
	{
		ptRoom->ptRingPrev->ptRingNext = ptRoom->ptRingNext;
		ptRoom->ptRingNext->ptRingPrev = ptRoom->ptRingPrev;
		ptRoom->ptRingPrev = NULL;
		ptRoom->ptRingNext = NULL;
		ptRoom->bRing = 4;
	}
}

static __inline void sRoomLink(ROOM *ptRoom, BYTE bRing)
{
	ROOM *ptHead;

	ptHead = &ptRoom->ptLevel->ptAct->atRings[bRing];
	ptRoom->ptRingNext = ptHead;
	ptRoom->ptRingPrev = ptHead->ptRingPrev;
	ptHead->ptRingPrev->ptRingNext = ptRoom;
	ptHead->ptRingPrev = ptRoom;
	ptRoom->bRing = bRing;
}

static __inline BOOL sRoomIsFree(ROOM *ptRoom, BYTE bLast)
{
	DWORD i;

	for (i = 0; i <= bLast; i++)
	{
		if (ptRoom->awHoldCounts[i])
			return FALSE;
	}
	return TRUE;
}

static __inline void sRoomBuildGameRoom(void *pPool, ROOM *ptRoom)
{
	DRLGACTCTX *ptAct;

	ptAct = ptRoom->ptLevel->ptAct;
	if (!ptRoom->nNumNear)
		DrlgRoomBuildNear(pPool, ptRoom);
	RoomTileBuildA(ptRoom);
	RoomTileBuildB(ptRoom);
	DrlgRoomNewGameRoom(ptAct, ptRoom);
	ptAct->bNumTickBuilds++;
	ptAct->dwNumRoomsBuilt++;
}

static void __fastcall sRoomEnterRelinkOnly(ROOM *ptRoom)
{
	if (ptRoom->bRing)
	{
		sRoomUnlink(ptRoom);
		sRoomLink(ptRoom, 0);
	}
}

static void __fastcall sRoomEnterBuildGameRoom(ROOM *ptRoom)
{
	DRLGACTCTX *ptAct;

	if (!(ptRoom->dwFlags & 1048576) && !ptRoom->ptGameRoom)
	{
		ptAct = ptRoom->ptLevel->ptAct;
		sRoomBuildGameRoom(ptAct->pMemPool, ptRoom);
		ptAct->bTicksToUpdate = (BYTE)(((~(BYTE)ptAct->dwFlags & 1) << 1) | 5);
	}
	if (ptRoom->bRing > 1)
	{
		sRoomUnlink(ptRoom);
		sRoomLink(ptRoom, 1);
	}
}

static void __fastcall sRoomEnterIfTilesSet(ROOM *ptRoom)
{
	if (!(ptRoom->dwFlags & 16777216))
		return;
	if (!(ptRoom->dwFlags & 33554432) && ptRoom->dwKind == 2)
		return;
	if (ptRoom->bRing <= 2)
		return;
	sRoomUnlink(ptRoom);
	sRoomLink(ptRoom, 2);
	if (ptRoom->awHoldCounts[1])
		sRoomEnterBuildGameRoom(ptRoom);
}

static void __fastcall sRoomEnterInitTiles(ROOM *ptRoom)
{
	if (!(ptRoom->dwFlags & 16777216))
		RoomTileInit(ptRoom);
	if (!(ptRoom->dwFlags & 33554432) && ptRoom->dwKind == 2)
		PresetRoomLoadUnits(ptRoom);
	if (ptRoom->bRing > 3)
	{
		sRoomUnlink(ptRoom);
		sRoomLink(ptRoom, 3);
	}
}

static void __fastcall sRoomLeaveNothingA(ROOM *ptRoom)
{
}

static void __fastcall sRoomLeaveNothingB(ROOM *ptRoom)
{
}

static void __fastcall sRoomLeaveNothingC(ROOM *ptRoom)
{
}

static void __fastcall sRoomLeaveUnloadIfFree(ROOM *ptRoom)
{
	if (!sRoomIsFree(ptRoom, 4))
		return;
	sRoomUnlink(ptRoom);
	if ((BYTE)ptRoom->ptLevel->ptAct->dwFlags & 1)
		RoomTileUnload(ptRoom, FALSE);
}

static PFNROOMSTATE sgpfnRoomStates[8] =
{
	sRoomEnterRelinkOnly, sRoomEnterBuildGameRoom, sRoomEnterIfTilesSet, sRoomEnterInitTiles,
	sRoomLeaveNothingA, sRoomLeaveNothingB, sRoomLeaveNothingC, sRoomLeaveUnloadIfFree,
};

static void __fastcall sRoomHoldNear(void *pPool, ROOM *ptRoom, BYTE bDistance);
static void __fastcall sRoomReleaseNear(ROOM *ptRoom, BYTE bDistance);

void __fastcall DrlgRoomEnterClient(DRLGACTCTX *ptAct, DWORD nLevelId, int nX, int nY, ROOM *ptHint)
{
	DRLGLEVEL *ptLevel;
	ROOM *ptRoom;

	ptLevel = DrlgGetLevel(ptAct, nLevelId);
	if (ptHint && ptHint->ptLevel->nLevelId != nLevelId)
		ptHint = NULL;
	ptRoom = DrlgActFindRoom(nX, nY, ptAct, ptHint, ptLevel);
	if (ptRoom->awHoldCounts[1])
		return;
	sRoomHoldNear(ptRoom->ptLevel->ptAct->pMemPool, ptRoom, 2);
	if (ptRoom->bRing >= 1 && sRoomIsFree(ptRoom, 1))
		sgpfnRoomStates[1](ptRoom);
	ptRoom->awHoldCounts[1]++;
}

static void __fastcall sRoomHoldNear(void *pPool, ROOM *ptRoom, BYTE bDistance)
{
	ROOM *ptNear;
	int i;

	if (!ptRoom->nNumNear)
		DrlgRoomBuildNear(pPool, ptRoom);
	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (bDistance < 3)
			sRoomHoldNear(pPool, ptNear, bDistance + 1);
		if (ptNear->bRing >= bDistance && sRoomIsFree(ptNear, bDistance))
			sgpfnRoomStates[bDistance](ptNear);
		ptNear->awHoldCounts[bDistance]++;
	}
}

void __fastcall DrlgRoomLeaveClient(DRLGACTCTX *ptAct, DWORD nLevelId, int nX, int nY, ROOM *ptHint)
{
	DRLGLEVEL *ptLevel;
	ROOM *ptRoom;

	ptLevel = DrlgGetLevel(ptAct, nLevelId);
	if (ptHint && ptHint->ptLevel->nLevelId != nLevelId)
		ptHint = NULL;
	ptRoom = DrlgActFindRoom(nX, nY, ptAct, ptHint, ptLevel);
	if (!ptRoom || !ptRoom->awHoldCounts[1])
		return;
	ptRoom->awHoldCounts[1]--;
	sgpfnRoomStates[5](ptRoom);
	sRoomReleaseNear(ptRoom, 2);
}

static void __fastcall sRoomReleaseNear(ROOM *ptRoom, BYTE bDistance)
{
	ROOM *ptNear;
	int i;

	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear)
		{
			ptNear->awHoldCounts[bDistance]--;
			sgpfnRoomStates[4 + bDistance](ptNear);
			if (bDistance < 3)
				sRoomReleaseNear(ptNear, bDistance + 1);
		}
	}
}

void __fastcall DrlgRoomSwitch(ROOM *ptFrom, ROOM *ptTo)
{
	if (ptFrom == ptTo)
		return;
	if (ptFrom && ptFrom->awHoldCounts[0])
	{
		ptFrom->awHoldCounts[0]--;
		sgpfnRoomStates[4](ptFrom);
		sRoomReleaseNear(ptFrom, 1);
	}
	if (!ptTo)
		return;
	sRoomHoldNear(ptTo->ptLevel->ptAct->pMemPool, ptTo, 1);
	if (sRoomIsFree(ptTo, 0))
		sgpfnRoomStates[0](ptTo);
	ptTo->awHoldCounts[0]++;
}

DUNGEONROOM *__fastcall DrlgRoomBuild(ROOM *ptRoom)
{
	if (!(ptRoom->dwFlags & 16777216))
		RoomTileInit(ptRoom);
	if (!(ptRoom->dwFlags & 33554432) && ptRoom->dwKind == 2)
		PresetRoomLoadUnits(ptRoom);
	if (!(ptRoom->dwFlags & 1048576))
		sRoomBuildGameRoom(ptRoom->ptLevel->ptAct->pMemPool, ptRoom);
	return ptRoom->ptGameRoom;
}

DUNGEONROOM *__fastcall DrlgLevelGetRoom(DRLGACTCTX *ptAct, int nX, int nY)
{
	return DrlgRoomBuild(DrlgActFindRoom(nX, nY, ptAct, NULL, NULL));
}

void __fastcall DrlgActInitRooms(DRLGACTCTX *ptAct)
{
	DWORD i;

	for (i = 0; i < 4; i++)
	{
		ptAct->atRings[i].ptRingNext = &ptAct->atRings[i];
		ptAct->atRings[i].ptRingPrev = &ptAct->atRings[i];
		ptAct->atRings[i].bRing = (BYTE)i;
	}
}

DUNGEONROOM *__fastcall DrlgActGetFirstActiveRoom(DRLGACTCTX *ptAct)
{
	ROOM *ptRoom;

	ptRoom = ptAct->atRings[0].ptRingNext;
	if (ptRoom == &ptAct->atRings[0] || !ptRoom)
	{
		ptRoom = ptAct->atRings[1].ptRingNext;
		if (ptRoom == &ptAct->atRings[1] || !ptRoom)
			return NULL;
	}
	return ptRoom->ptGameRoom;
}

static __inline BOOL sRoomIsInRing(ROOM *ptRoom, int nRing)
{
	if (ptRoom->bRing == nRing)
		return TRUE;
	return FALSE;
}

DUNGEONROOM *__fastcall DrlgActGetNextActiveRoom(DRLGACTCTX *ptAct, ROOM *ptRoom)
{
	BYTE bRing;
	ROOM *ptNext;

	bRing = ptRoom->bRing;
	ptNext = ptRoom->ptRingNext;
	if (sRoomIsInRing(ptRoom, 0) && ptNext == &ptAct->atRings[bRing])
	{
		ptNext = ptAct->atRings[1].ptRingNext;
		if (ptNext == &ptAct->atRings[1])
			return NULL;
	}
	if (ptNext)
		return ptNext->ptGameRoom;
	return NULL;
}

void __fastcall DrlgGetBuiltCounts(DWORD *pdwClientBuilt, DWORD *pdwClientCount, DWORD *pdwServerBuilt,
	DWORD *pdwServerCount)
{
	*pdwClientBuilt = sgdwClientBuilt;
	*pdwClientCount = sgdwClientCount;
	*pdwServerBuilt = sgdwServerBuilt;
	*pdwServerCount = sgdwServerCount;
}

void __stdcall DrlgActUpdateRooms(DRLGACTCTX *ptAct)
{
	ROOM *ptStart;
	ROOM *ptRoom;

	if ((BYTE)ptAct->dwFlags & 1)
	{
		sgdwClientBuilt = ptAct->dwNumRoomsBuilt;
		sgdwClientCount = ptAct->dwNumRoomsUnloaded;
	}
	else
	{
		sgdwServerBuilt = ptAct->dwNumRoomsBuilt;
		sgdwServerCount = ptAct->dwNumRoomsUnloaded;
	}
	if (ptAct->bNumTickBuilds > 1)
	{
		ptAct->bNumTickBuilds = 0;
		return;
	}
	if (--ptAct->bTicksToUpdate)
		return;
	ptAct->bTicksToUpdate = (BYTE)(((~(BYTE)ptAct->dwFlags & 1) << 1) | 5);
	if (!ptAct->ptRingCursor || ptAct->ptRingCursor->bRing != 2)
		ptAct->ptRingCursor = ptAct->atRings[2].ptRingNext;
	ptStart = ptAct->ptRingCursor;
	if (!ptStart)
		return;
	ptRoom = ptStart;
	do
	{
		if (!(ptRoom->dwFlags & 1048576) && ptRoom != &ptAct->atRings[2] && !ptRoom->ptGameRoom)
			sRoomBuildGameRoom(ptAct->pMemPool, ptRoom);
		ptRoom = ptRoom->ptRingNext;
	} while (ptRoom != ptStart && ptAct->bNumTickBuilds < 1);
	ptAct->ptRingCursor = ptRoom;
	ptAct->bNumTickBuilds = 0;
}

BOOL __fastcall DrlgRoomCanRelease(ROOM *ptRoom)
{
	ROOM *ptOther;

	if (ptRoom->dwFlags & 4194304)
		return FALSE;
	if (ptRoom->bRing < 4)
		return FALSE;
	if (!(ptRoom->dwFlags & 262144) && DrlgLevelIsTown((int)ptRoom->ptLevel->nLevelId))
	{
		for (ptOther = ptRoom->ptLevel->ptRoomFirst; ptOther; ptOther = ptOther->ptLevelNext)
		{
			if (ptOther->bRing < 4)
				return FALSE;
		}
	}
	return TRUE;
}

void __fastcall DrlgRoomSetHold(ROOM *ptRoom, BOOL fClear)
{
	if (fClear)
		ptRoom->dwFlags &= ~4194304;
	else
		ptRoom->dwFlags |= 4194304;
}

BYTE __fastcall DrlgRoomGetState(ROOM *ptRoom)
{
	return ptRoom->bRing;
}
