#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\DrlgWarp.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

struct DRLGWARPPAIR
{
	DWORD		field_00;
	int			field_04;
};

static const struct DRLGWARPPAIR sgtWarpPairs[14] =
{
	{ 1, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 1, 1 }, { 0, 1 }, { 0, 1 }, { 0, 1 }, { 0, 1 },
	{ 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 },
};

void __fastcall DrlgRoomSetWarpsDefAllowed(ROOM *ptRoom, int nValue)
{
	PTDRLGROOMWARP ptWarp;

	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
		ptWarp->nDefAllowed = nValue;
}

void __fastcall DrlgRoomShowWarpTiles(ROOM *ptRoom, int nExit)
{
	PTDRLGROOMWARP ptWarp;
	PTDRLGTILE ptTile;

	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		if (ptWarp->bExit == nExit)
			break;
	}
	if (!ptWarp)
		return;
	if (!ptWarp->ptLitTiles)
		return;
	for (ptTile = ptWarp->ptLitTiles; ptTile; ptTile = ptTile->ptNext)
		ptTile->dwFlags &= ~8;
	if (!ptWarp->ptRoomTiles)
		return;
	for (ptTile = ptWarp->ptRoomTiles; ptTile; ptTile = ptTile->ptNext)
		ptTile->dwFlags |= 8;
}

void __fastcall DrlgRoomHideWarpTiles(ROOM *ptRoom, int nExit)
{
	PTDRLGROOMWARP ptWarp;
	PTDRLGTILE ptTile;

	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		if (ptWarp->bExit == nExit)
			break;
	}
	if (!ptWarp)
		return;
	if (!ptWarp->ptRoomTiles)
		return;
	for (ptTile = ptWarp->ptRoomTiles; ptTile; ptTile = ptTile->ptNext)
		ptTile->dwFlags &= ~8;
	if (!ptWarp->ptLitTiles)
		return;
	for (ptTile = ptWarp->ptLitTiles; ptTile; ptTile = ptTile->ptNext)
		ptTile->dwFlags |= 8;
}

static ROOM *__fastcall sFindTownStart(DRLGLEVEL *ptLevel, int *pnX, int *pnY)
{
	ROOM *ptRoom;
	PTPRESETUNIT ptPresetUnits;

	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (DrlgRoomGetExitFlags(ptRoom))
		{
			DrlgRoomBuild(ptRoom);
			for (ptPresetUnits = ptRoom->ptPresetUnits; ptPresetUnits; ptPresetUnits = ptPresetUnits->ptNext)
			{
				if (ptPresetUnits->nUnitType == 2 && (DWORD)ptPresetUnits->nClassId < 410 &&
					(((PTOBJECTSTXT)D2CommonObjTblsGetObject(ptPresetUnits->nClassId))->SubClass & 64))
				{
					*pnX = ptPresetUnits->nX / 5 + ptRoom->tCoords.nTileX;
					*pnY = ptPresetUnits->nY / 5 + ptRoom->tCoords.nTileY;
					return ptRoom;
				}
			}
#line 257
			D2WARN(ptPresetUnits);
			return ptRoom;
		}
	}
	return NULL;
}

static __inline ROOM *sFindWarpRoom(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	DWORD dwBit;
	BYTE bWarp;

	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (!(ptRoom->dwFlags & 4080))
			continue;
		for (dwBit = 16, bWarp = 0; dwBit & 4080; dwBit <<= 1, bWarp++)
		{
			if ((ptRoom->dwFlags & dwBit) && DrlgWarpGetList(ptLevel->ptAct, (int)ptLevel->nLevelId)[bWarp] != -1)
				return ptRoom;
		}
	}
	return NULL;
}

DUNGEONROOM *__fastcall DrlgWarpGetRoom(DRLGACTCTX *ptAct, int nLevelId, int nEntry, int *pnX, int *pnY)
{
	PTLEVELSREC ptRecord;
	DRLGLEVEL *ptDrlgLevel;
	ROOM *ptRoom;
	int nX, nY;
	int nKind;
	int nCount;
	int nIndex;
	int i;

	ptRecord = (PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId);
	*pnX = -1;
	*pnY = -1;
	ptDrlgLevel = DrlgGetLevel(ptAct, nLevelId);
	if (!ptDrlgLevel->ptRoomFirst)
		DrlgGenerateLevel(ptDrlgLevel);
	if (ptRecord->Position)
	{
		if (nEntry == 13)
		{
			ptRoom = sFindTownStart(ptDrlgLevel, pnX, pnY);
			DrlgRoomBuild(ptRoom);
			return ptRoom->ptGameRoom;
		}
		nCount = 0;
		for (i = 0; i < ptDrlgLevel->nNumPresetMarks; i++)
		{
			nKind = ptDrlgLevel->atPresetMarks[i].nKind;
			if (nEntry == nKind)
				nCount++;
			else if (sgtWarpPairs[nEntry].field_00 &&
				sgtWarpPairs[nEntry].field_04 == sgtWarpPairs[nKind].field_04)
				nCount++;
		}
		nIndex = nCount ? (int)SeedRand(&ptDrlgLevel->tSeed, (DWORD)nCount) + 1 : 0;
		for (i = 0; i < ptDrlgLevel->nNumPresetMarks; i++)
		{
			nKind = ptDrlgLevel->atPresetMarks[i].nKind;
			if (nEntry == nKind)
				nIndex--;
			else if (sgtWarpPairs[nEntry].field_00 &&
				sgtWarpPairs[nEntry].field_04 == sgtWarpPairs[nKind].field_04)
				nIndex--;
			if (nIndex <= 0)
				break;
		}
		*pnX = ptDrlgLevel->atPresetMarks[i].nX;
		*pnY = ptDrlgLevel->atPresetMarks[i].nY;
		ptRoom = DrlgActFindRoom(*pnX, *pnY, ptDrlgLevel->ptAct, NULL, ptDrlgLevel);
		DrlgRoomBuild(ptRoom);
		return ptRoom->ptGameRoom;
	}
	ptRoom = sFindTownStart(ptDrlgLevel, pnX, pnY);
	if (!ptRoom)
		ptRoom = sFindWarpRoom(ptDrlgLevel);
	if (!ptRoom)
	{
		nX = ptDrlgLevel->tCoords.nTileX + ptDrlgLevel->tCoords.nSizeTileX / 2;
		nY = ptDrlgLevel->tCoords.nTileY + ptDrlgLevel->tCoords.nSizeTileY / 2;
		ptRoom = DrlgLevelFindRoom(ptDrlgLevel, nX, nY);
	}
	if (!ptRoom)
	{
#line 313
		D2WARN(ptDrlgLevel);
#line 314
		D2WARN(ptDrlgLevel->ptRoomFirst);
		nIndex = ptDrlgLevel->nNumRooms ? (int)SeedRand(&ptDrlgLevel->tSeed, (DWORD)ptDrlgLevel->nNumRooms) : 0;
		ptRoom = ptDrlgLevel->ptRoomFirst;
		while (nIndex--)
			ptRoom = ptRoom->ptLevelNext;
	}
	if (ptRoom)
	{
		if (*pnX == -1 || *pnY == -1)
		{
			*pnX = ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX / 2;
			*pnY = ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY / 2;
		}
		DrlgRoomBuild(ptRoom);
		return ptRoom->ptGameRoom;
	}
	return NULL;
}

int *__fastcall DrlgWarpGetList(DRLGACTCTX *ptAct, int nLevelId)
{
	PTVISINFO ptVisInfo;

	for (ptVisInfo = ptAct->ptVisInfo; ptVisInfo; ptVisInfo = ptVisInfo->ptNext)
	{
#line 419
		D2WARN(ptVisInfo->eLevelId != LEVEL_ID_NONE);
		if (nLevelId == ptVisInfo->eLevelId)
			return ptVisInfo->anWarp;
	}
	return ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->Warp;
}

int *__fastcall DrlgLevelGetWarpList(DRLGLEVEL *ptLevel)
{
	PTVISINFO ptVisInfo;
	int nLevelId;
	int *pnWarp;

	ptVisInfo = ptLevel->ptAct->ptVisInfo;
	nLevelId = (int)ptLevel->nLevelId;
	for (; ptVisInfo; ptVisInfo = ptVisInfo->ptNext)
	{
#line 419
		D2WARN(ptVisInfo->eLevelId != LEVEL_ID_NONE);
		if (nLevelId == ptVisInfo->eLevelId)
			return ptVisInfo->anWarp;
	}
	return ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->Warp;
}

int __fastcall DrlgWarpGetWarp(DRLGLEVEL *ptLevel, BYTE bIndex)
{
	PTVISINFO ptVisInfo;
	int nLevelId;
	int *pnWarp;

	ptVisInfo = ptLevel->ptAct->ptVisInfo;
	nLevelId = (int)ptLevel->nLevelId;
	for (;; ptVisInfo = ptVisInfo->ptNext)
	{
		if (!ptVisInfo) {
			pnWarp = ((PTLEVELSREC)D2CommonLvlTblsGetLevelDef(nLevelId))->Warp;
			break; }
#line 419
		D2WARN(ptVisInfo->eLevelId != LEVEL_ID_NONE);
		if (nLevelId == ptVisInfo->eLevelId) {
			pnWarp = ptVisInfo->anWarp;
			break; }
	}
	return pnWarp[bIndex];
}

PTLVLWARPTXT __fastcall DrlgWarpGetDefByWarp(int nWarp)
{
	if (nWarp == -1)
		return NULL;
	return LvlTblsGetWarpDef(nWarp);
}

PTLVLWARPTXT __fastcall DrlgWarpGetWarpDef(DRLGLEVEL *ptLevel, BYTE bExit)
{
	int nWarp;

	nWarp = DrlgWarpGetList(ptLevel->ptAct, (int)ptLevel->nLevelId)[bExit];
	if (nWarp == -1)
		return NULL;
	return LvlTblsGetWarpDef(nWarp);
}

PTLVLWARPTXT __fastcall DrlgRoomGetUnitWarpDef(ROOM *ptRoom, UNIT *ptUnit)
{
	PTDRLGROOMWARP ptWarp;
	BYTE bExit;
	int nWarp;

	bExit = (BYTE)ptUnit->dwClassId;
	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		if (ptWarp->bExit == bExit && !ptWarp->nDefAllowed)
			return NULL;
	}
	nWarp = DrlgWarpGetList(ptRoom->ptLevel->ptAct, (int)ptRoom->ptLevel->nLevelId)[bExit];
	if (nWarp == -1)
		return NULL;
	return LvlTblsGetWarpDef(nWarp);
}

static __inline int sVisExitIndex(int *pnVis, int nLevel, int n)
{
	int nSame;
	BYTE b;

	nSame = 0;
	for (b = 0; b < 8; b++)
	{
		if (pnVis[b] == nLevel)
		{
			if (nSame == n)
				return b;
			nSame++;
		}
	}
	return -1;
}

static __inline ROOM *sFindExitRoom(DRLGLEVEL *ptLevel, int nExit)
{
	DWORD dwMask;
	ROOM *ptRoom;

	dwMask = 1 << (nExit + 4);
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptRoom->dwFlags & dwMask)
			return ptRoom;
	}
	return NULL;
}

void __fastcall DrlgWarpBuildLinkedRooms(DRLGACTCTX *ptAct, BYTE bAct, int nLevelA, int nLevelB)
{
	DRLGLEVEL *ptLevelA;
	DRLGLEVEL *ptLevelB;
	ROOM *ptRoomA;
	ROOM *ptRoomB;
	PTPRESETUNIT ptUnit;
	int *pnVis;
	int nExitA;
	int nExitB;
	int n;

	OutdoorsLayoutAct(ptAct, bAct);
	ptLevelA = DrlgGetLevel(ptAct, (DWORD)nLevelA);
	if (!ptLevelA->ptRoomFirst)
		DrlgGenerateLevel(ptLevelA);
	ptLevelB = DrlgGetLevel(ptAct, (DWORD)nLevelB);
	if (!ptLevelB->ptRoomFirst)
		DrlgGenerateLevel(ptLevelB);
	for (n = 0; n < 8; n++)
	{
		pnVis = DrlgRoomGetVis(ptAct, nLevelA);
		nExitA = sVisExitIndex(pnVis, nLevelB, n);
		pnVis = DrlgRoomGetVis(ptAct, nLevelB);
		nExitB = sVisExitIndex(pnVis, nLevelA, n);
		if (nExitA == -1 || nExitB == -1)
			return;
		DrlgLevelGetWarpList(ptLevelA);
		DrlgLevelGetWarpList(ptLevelB);
		ptRoomA = sFindExitRoom(ptLevelA, nExitA);
		ptRoomB = sFindExitRoom(ptLevelB, nExitB);
		DrlgRoomBuild(ptRoomA);
		DrlgRoomBuild(ptRoomB);
		for (ptUnit = DrlgRoomGetPresetUnits(ptRoomA); ptUnit; ptUnit = ptUnit->ptNext)
		{
			if (ptUnit->nUnitType == 5 && ptUnit->nClassId == nExitA)
				break;
		}
		for (ptUnit = DrlgRoomGetPresetUnits(ptRoomB); ptUnit; ptUnit = ptUnit->ptNext)
		{
			if (ptUnit->nUnitType == 5 && ptUnit->nClassId == nExitB)
				break;
		}
	}
}
