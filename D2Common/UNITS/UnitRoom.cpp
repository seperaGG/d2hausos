#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\UNITS\\UnitRoom.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

void __stdcall UnitRoomRemoveFromChangedList(PTUNIT hUnit);
void __stdcall UnitRoomRemoveUnit(PTUNIT hUnit);

#define DungeonTestRoomGame(hRoom, nX, nY) \
	((nX) >= (hRoom)->tRect.nX && (nX) < (hRoom)->tRect.nX + (hRoom)->tRect.nWidth && \
	(nY) >= (hRoom)->tRect.nY && (nY) < (hRoom)->tRect.nY + (hRoom)->tRect.nHeight)

static void UnitRoomAssertInRoom(PTDUNGEONROOM hRoom, int nX, int nY)
{
#line 23
	D2ASSERT(DungeonTestRoomGame ( hRoom, nX, nY));
}

static void UnitRoomAssertNotAlreadyInRoom(PTUNIT ptWalk, PTUNIT ptUnit)
{
#line 28
	D2ASSERTMSG(ptUnit != ptWalk, "Unit being added to a room it is already in");
}

static void UnitRoomAssertChangedListUnit(PTUNIT hUnit)
{
#line 74
	D2ASSERT(hUnit);
}

void __stdcall AddUnitToRoom(PTUNIT ptUnit, PTDUNGEONROOM hRoom)
{
	GAMERECT atCopy[2];
	PTUNIT *pptUnitFirst;
	PTUNIT ptWalk;
	PTDUNGEONROOM hPathRoom;
	DWORD dwFlags;
	int nX, nY;

#line 40
	D2ASSERT(ptUnit);
#line 41
	D2ASSERT(hRoom);

	D2CommonDungeonRoomCopyRects(hRoom, atCopy);
	nX = (int)D2CommonUnitGetX(ptUnit);
	nY = (int)D2CommonUnitGetY(ptUnit);
	UnitRoomAssertInRoom(hRoom, nX, nY);

	for (ptWalk = hRoom->ptUnitFirst; ptWalk != NULL; ptWalk = ptWalk->ptRoomNext)
	{
		UnitRoomAssertNotAlreadyInRoom(ptWalk, ptUnit);
	}

	pptUnitFirst = DungeonRoomGetUnitListHead(hRoom);
#line 47
	D2ASSERT(pptUnitFirst);
	ptUnit->ptRoomNext = *pptUnitFirst;
	*pptUnitFirst = ptUnit;

	hPathRoom = (PTDUNGEONROOM)D2CommonUnitGetPathRoom(ptUnit);
	if (hPathRoom != NULL && !D2CommonDungeonRoomTestSkipsChangedList(hPathRoom))
	{
		dwFlags = ptUnit->dwFlags0;
		if (!(dwFlags & 8192))
		{
			ptUnit->dwFlags0 = dwFlags | 8192;
			pptUnitFirst = DungeonRoomGetChangedListHead(hPathRoom, TRUE);
#line 93
			D2ASSERT(pptUnitFirst);
			ptUnit->ptChangedNext = *pptUnitFirst;
			*pptUnitFirst = ptUnit;
		}
	}

	switch (ptUnit->eType)
	{
	case UNIT_PLAYER:
		D2CommonDungeonRoomIncPlayerUnits(hRoom);
		break;
	case UNIT_MONSTER:
		if (D2CommonStatsUnitGetAlignment(ptUnit) == 0)
			D2CommonDungeonRoomIncPlayerUnits(hRoom);
		break;
	}
}

void __stdcall UnitRoomAddToChangedList(PTUNIT hUnit)
{
	PTDUNGEONROOM hRoom;
	PTUNIT *pptUnitFirst;
	DWORD dwFlags;
	DWORD dwResult;

	UnitRoomAssertChangedListUnit(hUnit);
	hRoom = (PTDUNGEONROOM)D2CommonUnitGetPathRoom(hUnit);
	if (hRoom == NULL)
		return;
	dwResult = D2CommonDungeonRoomTestSkipsChangedList(hRoom);
	if (dwResult)
		return;
	dwFlags = hUnit->dwFlags0;
	if (dwFlags & 8192)
		return;
	hUnit->dwFlags0 = dwFlags | 8192;
	pptUnitFirst = DungeonRoomGetChangedListHead(hRoom, TRUE);
#line 93
	D2ASSERT(pptUnitFirst);
	hUnit->ptChangedNext = *pptUnitFirst;
	*pptUnitFirst = hUnit;
}
#line 98

void __stdcall UnitRoomSortByTileY(PTDUNGEONROOM ptRoom)
{
	PTUNIT *pptUnitFirst;
	PTUNIT ptPrev, ptCur, ptNext;
	BOOL fMoved;
	int nY;

	pptUnitFirst = DungeonRoomGetUnitListHead(ptRoom);
	ptPrev = NULL;
	ptCur = *pptUnitFirst;
	if (ptCur == NULL)
		return;
	ptNext = ptCur->ptRoomNext;
	do
	{
		fMoved = FALSE;
		while (ptNext != NULL)
		{
			nY = D2CommonUnitGetScreenY(ptCur);
			if (nY > (int)D2CommonUnitGetScreenY(ptNext))
			{
				if (ptPrev != NULL)
					ptPrev->ptRoomNext = ptNext;
				else
					*pptUnitFirst = ptNext;
				ptCur->ptRoomNext = ptNext->ptRoomNext;
				ptPrev = ptNext;
				ptNext->ptRoomNext = ptCur;
				ptNext = ptCur->ptRoomNext;
				fMoved = TRUE;
			}
			else
			{
				ptPrev = ptCur;
				ptCur = ptNext;
				ptNext = ptNext->ptRoomNext;
			}
		}
	} while (fMoved);
}

void __stdcall UnitRoomLeave(PTUNIT ptUnit)
{
	PTPATH ptPath;

#line 161
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT
		|| ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		ptPath = ptUnit->ptPath;
		if (ptPath->dwRoom != 0)
			UnitRoomRemoveUnit(ptUnit);
		ptPath->dwRoom = 0;
		return;
	}
	StepLeaveRoom(ptUnit->ptPath);
}

void __stdcall UnitRoomClearChangedList(PTDUNGEONROOM ptRoom)
{
	PTUNIT *pptChangedFirst;
	PTUNIT ptCur, ptNext;

	pptChangedFirst = DungeonRoomGetChangedListHead(ptRoom, FALSE);
	ptCur = *pptChangedFirst;
	while (ptCur != NULL)
	{
		ptNext = ptCur->ptChangedNext;
		ptCur->dwFlags0 &= ~8192;
		ptCur->ptChangedNext = NULL;
		ptCur = ptNext;
	}
	*pptChangedFirst = NULL;
}

void __stdcall UnitRoomRemoveUnit(PTUNIT hUnit)
{
	PTUNIT ptUnit = hUnit;
	PTDUNGEONROOM hRoom;
	PTUNIT *pptUnitFirst;
	PTUNIT ptPrev, ptCur;
	BOOL fFound = FALSE;

#line 196
	D2ASSERT(hUnit);
	hRoom = (PTDUNGEONROOM)D2CommonUnitGetPathRoom(hUnit);
	if (hRoom == NULL)
	{
#line 204
		D2ASSERT(! ptUnit->ptRoomNext);
		return;
	}

	pptUnitFirst = DungeonRoomGetUnitListHead(hRoom);
	ptPrev = NULL;
	ptCur = *pptUnitFirst;
	while (ptCur != NULL && ptCur != hUnit)
	{
		ptPrev = ptCur;
		ptCur = ptCur->ptRoomNext;
	}
	if (ptCur != NULL)
	{
		fFound = TRUE;
		if (ptPrev == NULL)
			*pptUnitFirst = ptCur->ptRoomNext;
		else
			ptPrev->ptRoomNext = ptCur->ptRoomNext;
		ptCur->ptRoomNext = NULL;

		switch (hUnit->eType) {
		case UNIT_MONSTER:
			if (D2CommonStatsUnitGetAlignment(hUnit) != 0)
				break;
		case UNIT_PLAYER:
			D2CommonDungeonRoomDecPlayerUnits(hRoom);
		}
	}

	UnitRoomRemoveFromChangedList(hUnit);
	hUnit->dwFlags0 &= ~8192;
	if (fFound)
		D2CommonUnitClearPathRoom(hUnit);
}

void __stdcall UnitRoomRemoveFromChangedList(PTUNIT hUnit)
{
	PTUNIT ptUnit = hUnit;
	PTDUNGEONROOM hRoom;
	PTUNIT *pptChangedFirst;
	PTUNIT ptPrev, ptCur;

#line 252
	D2ASSERT(hUnit);
	hRoom = (PTDUNGEONROOM)D2CommonUnitGetPathRoom(hUnit);
	if (hRoom == NULL)
	{
#line 263
		D2ASSERT(! ptUnit->ptChangedNext);
		return;
	}

	pptChangedFirst = DungeonRoomGetChangedListHead(hRoom, FALSE);
	ptPrev = NULL;
	ptCur = *pptChangedFirst;
	while (ptCur != NULL)
	{
		if (ptCur == hUnit)
			break;
		ptPrev = ptCur;
		ptCur = ptCur->ptChangedNext;
	}
	if (ptCur != NULL)
	{
		if (ptPrev == NULL)
			*pptChangedFirst = ptCur->ptChangedNext;
		else
			ptPrev->ptChangedNext = ptCur->ptChangedNext;
		ptCur->ptChangedNext = NULL;
	}
}

BOOL __stdcall UnitRoomTestUnitInRoom(PTDUNGEONROOM ptRoom, PTUNIT hUnit)
{
	PTUNIT *pptUnitFirst;
	PTUNIT ptCur;

#line 291
	D2ASSERT(hUnit);
	if (ptRoom == NULL)
		return FALSE;
	pptUnitFirst = DungeonRoomGetUnitListHead(ptRoom);
	ptCur = *pptUnitFirst;
	while (ptCur != hUnit) {
		ptCur = ptCur->ptRoomNext;
		if (ptCur == NULL)
			return FALSE;
	}
	return TRUE;
}
