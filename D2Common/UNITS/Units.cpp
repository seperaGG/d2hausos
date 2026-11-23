#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\UNITS\\Units.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "StormImports.h"
#include "D2Macros.h"
#include "UnitsLocal.h"
#include "../DATATBLS/DataTbls.h"

BYTE __stdcall UnitGetPathFlagByte(PTUNIT ptUnit)
{
#line 888
	D2ASSERT(ptUnit);
	switch (ptUnit->eType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
		return (BYTE)ptUnit->ptPath->tEndCoord.wX;
	}
	return D2CommonPathGetFacing(ptUnit->ptPath);
}

PTSKILL __stdcall UnitGetFirstSkill(HUNIT hUnit)
{
#line 1662
	D2ASSERT(hUnit);
	return (PTSKILL)D2CommonSkillsHeadGetFirst(hUnit->field_c8);
}

PTSKILL __stdcall UnitGetLeftSkill(HUNIT hUnit)
{
#line 1671
	D2ASSERT(hUnit);
	return SkillsHeadGetLeftSkill(hUnit->field_c8);
}

PTSKILL __stdcall UnitGetRightSkill(HUNIT hUnit)
{
#line 1680
	D2ASSERT(hUnit);
	return SkillsHeadGetRightSkill(hUnit->field_c8);
}

void __stdcall UnitSetSkillThirdCached(HUNIT hUnit, PTSKILL ptSkill)
{
#line 1689
	D2ASSERT(hUnit);
	SkillsHeadSetThirdCached(hUnit->field_c8, ptSkill);
}

PTSKILL __stdcall UnitGetSkillThirdCached(HUNIT hUnit)
{
#line 1698
	D2ASSERT(hUnit);
	return SkillsHeadGetThirdCached(hUnit->field_c8);
}

PTUNIT __fastcall UnitAllocRoomTile(void *pMemPool, int eType)
{
	PTUNIT ptUnit;
	int i;

#line 1707
	D2ASSERT(eType == UNIT_ROOMTILE);
#line 1709
	ptUnit = (PTUNIT)D2ALLOCPOOL(pMemPool, sizeof(UNIT));
	for (i = 0; i < (int)(sizeof(UNIT) / sizeof(DWORD)); i++)
		((DWORD *)ptUnit)[i] = 0;
	ptUnit->dwDropItemCode = 0;
	ptUnit->eType = UNIT_ROOMTILE;
	return ptUnit;
}

void __fastcall UnitSetRoomTilePath(PTUNIT ptUnit, DWORD dwX, DWORD dwY, DWORD dwTarget, DWORD dwKeptTarget)
{
	PTPATH ptPath;

#line 1727
	D2ASSERT(ptUnit);
#line 1729
	D2ASSERT(ptUnit->eType == UNIT_ROOMTILE);
	ptPath = ptUnit->ptPath;
	ptPath->tRoomTileCoord.dwX = dwX;
	ptPath->tRoomTileCoord.dwY = dwY;
	*(DWORD *)&ptPath->tTargetCoord = dwTarget;
	*(DWORD *)&ptPath->wKeptTargetX = dwKeptTarget;
}

DWORD __stdcall UnitGetX(PTUNIT ptUnit)
{
	PTPATH ptPath;

#line 1747
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->tRoomTileCoord.dwX;
	ptPath = ptUnit->ptPath;
	if (!(ptPath == NULL)) {
		return D2CommonPathGetGameX(ptPath); }
	return 0;
}

DWORD __stdcall UnitGetXFixed(PTUNIT ptUnit)
{
#line 1772
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->tRoomTileCoord.dwX << 16;
	return D2CommonPathGetRawX(ptUnit->ptPath);
}

DWORD __stdcall UnitGetYFixed(PTUNIT ptUnit)
{
#line 1794
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->tRoomTileCoord.dwY << 16;
	return D2CommonPathGetRawY(ptUnit->ptPath);
}

void __stdcall UnitSetX(PTUNIT ptUnit, DWORD dwX)
{
#line 1815
	D2ASSERT(ptUnit);
	switch ((int)ptUnit->eType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_ROOMTILE:
		break;
	default:
#line 1826
		D2ASSERT(0);
	}
	ptUnit->ptPath->tRoomTileCoord.dwX = dwX;
}

DWORD __stdcall UnitGetY(PTUNIT ptUnit)
{
	PTPATH ptPath;

#line 1838
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->tRoomTileCoord.dwY;
	ptPath = ptUnit->ptPath;
	if (!(ptPath == NULL)) {
		return D2CommonPathGetGameY(ptPath); }
	return 0;
}

void __stdcall UnitSetY(PTUNIT ptUnit, DWORD dwY)
{
#line 1861
	D2ASSERT(ptUnit);
	switch ((int)ptUnit->eType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
	case UNIT_ROOMTILE:
		break;
	default:
#line 1872
		D2ASSERT(0);
	}
	ptUnit->ptPath->tRoomTileCoord.dwY = dwY;
}

DWORD __stdcall UnitGetSizeX(PTUNIT ptUnit)
{
	PTMONSTATS ptStats;

#line 1884
	D2ASSERT(ptUnit);
	switch (ptUnit->eType)
	{
	case UNIT_PLAYER:
		return 2;
	case UNIT_MONSTER:
		ptStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
		return ptStats->dwSizeX;
	case UNIT_OBJECT:
		return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo->SizeX;
	case UNIT_ITEM:
		return 1;
	case UNIT_MISSILE:
		return ((PTMISSILETBLREC)D2CommonMissileTblsGetMissile(ptUnit->dwClassId))->Size;
	case UNIT_ROOMTILE:
		return 0;
	}
	return 0;
}

DWORD __stdcall UnitGetSizeY(PTUNIT ptUnit)
{
#line 1914
	D2ASSERT(ptUnit);
	switch (ptUnit->eType)
	{
	case UNIT_PLAYER:
		return 2;
	case UNIT_MONSTER:
		return ((PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId))->SizeY;
	case UNIT_OBJECT:
		return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo->SizeY;
	case UNIT_ITEM:
		return 1;
	case UNIT_MISSILE:
		return ((PTMISSILETBLREC)D2CommonMissileTblsGetMissile(ptUnit->dwClassId))->Size;
	case UNIT_ROOMTILE:
		return 0;
	}
	return 0;
}

DWORD __stdcall UnitGetScreenX(PTUNIT ptUnit)
{
#line 1944
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->tGameCoord.dwX;
	return D2CommonPathGetScreenX(ptUnit->ptPath);
}

DWORD __stdcall UnitGetScreenY(PTUNIT ptUnit)
{
#line 1965
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->tGameCoord.dwY;
	return D2CommonPathGetScreenY(ptUnit->ptPath);
}

DWORD __stdcall UnitGetDistanceX(HUNIT hUnitA, HUNIT hUnitB)
{
	PTUNIT ptUnit;
	PTPATH ptPath;
	int nA;
	int nB;
	int nDiff;

	ptUnit = hUnitA;
#line 1747
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		nA = (int)ptUnit->ptPath->tRoomTileCoord.dwX;
	}
	else
	{
		ptPath = ptUnit->ptPath;
		nA = (ptPath != NULL) ? (int)D2CommonPathGetGameX(ptPath) : 0;
	}
	ptUnit = hUnitB;
#line 1747
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		nB = (int)ptUnit->ptPath->tRoomTileCoord.dwX;
	}
	else
	{
		ptPath = ptUnit->ptPath;
		nB = (ptPath != NULL) ? (int)D2CommonPathGetGameX(ptPath) : 0;
	}
	nDiff = nA - nB;
	if (nDiff < 0)
		nDiff = -nDiff;
	return (DWORD)nDiff;
}

DWORD __stdcall UnitGetDistanceY(HUNIT hUnitA, HUNIT hUnitB)
{
	PTUNIT ptUnit;
	PTPATH ptPath;
	int nA;
	int nB;
	int nDiff;

	ptUnit = hUnitA;
#line 1838
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		nA = (int)ptUnit->ptPath->tRoomTileCoord.dwY;
	}
	else
	{
		ptPath = ptUnit->ptPath;
		nA = (ptPath != NULL) ? (int)D2CommonPathGetGameY(ptPath) : 0;
	}
	ptUnit = hUnitB;
#line 1838
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		nB = (int)ptUnit->ptPath->tRoomTileCoord.dwY;
	}
	else
	{
		ptPath = ptUnit->ptPath;
		nB = (ptPath != NULL) ? (int)D2CommonPathGetGameY(ptPath) : 0;
	}
	nDiff = nA - nB;
	if (nDiff < 0)
		nDiff = -nDiff;
	return (DWORD)nDiff;
}

void __fastcall PathSetScreenX(PTPATH ptPath, DWORD dwValue);
void __fastcall PathSetScreenY(PTPATH ptPath, DWORD dwValue);

void __stdcall UnitSetScreenX(PTUNIT ptUnit, DWORD dwValue)
{
#line 2005
	D2ASSERT(ptUnit);
	PathSetScreenX(ptUnit->ptPath, dwValue);
}

void __stdcall UnitSetScreenY(PTUNIT ptUnit, DWORD dwValue)
{
#line 2017
	D2ASSERT(ptUnit);
	PathSetScreenY(ptUnit->ptPath, dwValue);
}

void __stdcall UnitGetPosition(PTUNIT ptUnit, GAMECOORD *ptOut)
{
	PTPATH ptPath;

#line 2029
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		ptPath = ptUnit->ptPath;
		*ptOut = ptPath->tRoomTileCoord;

		return;
	}
	ptPath = ptUnit->ptPath;
	if (ptPath != NULL)
	{
		ptOut->dwX = D2CommonPathGetGameX(ptUnit->ptPath);
		ptOut->dwY = D2CommonPathGetGameY(ptUnit->ptPath);
		return;
	}
	ptOut->dwX = 0;
	ptOut->dwY = 0;
}
#line 2047

void __stdcall UnitGetScreenPosition(PTUNIT ptUnit, GAMECOORD *ptOut)
{
	PTPATH ptPath;

#line 2058
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		ptPath = ptUnit->ptPath;
		*ptOut = ptPath->tGameCoord;
		return;
	}
	ptOut->dwX = D2CommonPathGetScreenX(ptUnit->ptPath);
	ptOut->dwY = D2CommonPathGetScreenY(ptUnit->ptPath);
}

void __fastcall UnitSetRoomTileGameCoord(PTUNIT ptUnit, DWORD dwX, DWORD dwY)
{
	PTPATH ptPath;

#line 2082
	D2ASSERT(ptUnit);
#line 2084
	D2ASSERT(ptUnit->eType == UNIT_ROOMTILE);
	ptPath = ptUnit->ptPath;
	ptPath->tGameCoord.dwX = dwX;
	ptPath->tGameCoord.dwY = dwY;
}

#pragma auto_inline(off)
DWORD __fastcall UnitGetTypeFlags(PTUNIT ptUnit)
{
	PTOBJECTCLASSINFO ptClassInfo;

#line 2097
	D2ASSERT(ptUnit);
	switch (ptUnit->eType)
	{
	case UNIT_OBJECT:
		ptClassInfo = ((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo;
		if (ptClassInfo->IsDoor)
		{
			if (ptClassInfo->BlocksVis)
				return 2054;
			return ptClassInfo->BlockMissile ? 2052 : 1024;
		}
		if (ptClassInfo->SubClass & 4)
			return 32768;
		return ptClassInfo->BlockMissile ? 1028 : 1024;
	case UNIT_ROOMTILE:
		return 1;
	case UNIT_ITEM:
		return 512;
	}
	return D2CommonPathGetCollisionMask(ptUnit->ptPath);
}
#pragma auto_inline(on)
#line 2115

DWORD __stdcall UnitGetPathRoom(PTUNIT ptUnit);

void __stdcall UnitClearCollision(PTUNIT ptUnit)
{
	DWORD dwSizeY;
	DWORD dwSizeX;
	DWORD dwY;
	DWORD dwX;
	DWORD dwRoom;
	DWORD dwFlags;

	dwSizeY = UnitGetSizeY(ptUnit);
	dwSizeX = UnitGetSizeX(ptUnit);
	dwY = UnitGetY(ptUnit);
	dwX = UnitGetX(ptUnit);
	dwRoom = UnitGetPathRoom(ptUnit);
	dwFlags = UnitGetTypeFlags(ptUnit);
	D2CommonCollisnClearArea(dwRoom, dwX, dwY, dwSizeX, dwSizeY, dwFlags);
}

void __stdcall UnitSetCollision(HUNIT hUnit, GAMEROOM *hRoom, int nX, int nY)
{
	DWORD dwSizeX;
	DWORD dwSizeY;
	DWORD dwFlags;

#line 2164
	D2ASSERT(hUnit);
	dwSizeX = UnitGetSizeX(hUnit);
	dwSizeY = UnitGetSizeY(hUnit);
	if (dwSizeX && dwSizeY)
	{
		dwFlags = UnitGetTypeFlags(hUnit);
		D2CommonCollisnSetArea(hRoom, nX, nY, dwSizeX, dwSizeY, dwFlags);
	}
}

void UNITSTHIS::UnitInitPosition(PTUNIT ptUnit, DWORD dwRoom, DWORD dwTileX, DWORD dwTileY)
{
	PTPATH ptPath;
	DWORD dwA;
	DWORD dwB;

#line 2185
	D2ASSERT(ptUnit);
	ptPath = ptUnit->ptPath;
	*(BYTE *)&ptPath->tEndCoord.wX = 0;
	ptPath->tRoomTileCoord.dwX = dwTileX;
	ptPath->tRoomTileCoord.dwY = dwTileY;
	ptPath->dwRoom = dwRoom;
	dwA = dwTileX;
	dwB = dwTileY;
	D2CommonDungeonCalcSubtileToScreen(&dwA, &dwB);
	ptPath->tGameCoord.dwX = dwA;
	ptPath->tGameCoord.dwY = dwB;
}

void __stdcall UnitClearPathRoom(HUNIT hUnit)
{
	PTPATH ptPath;

#line 2208
	D2ASSERT(hUnit);
	if (hUnit->eType == UNIT_OBJECT || ((int)hUnit->eType > UNIT_MISSILE && (int)hUnit->eType <= UNIT_ROOMTILE))
	{
		hUnit->ptPath->dwRoom = 0;
		return;
	}
	ptPath = hUnit->ptPath;
	if (ptPath == NULL)
		return;
	D2CommonPathSetRoom(ptPath, 0);
}

DWORD __stdcall UnitGetPathRoom(PTUNIT ptUnit)
{
	PTPATH ptPath;

#line 2233
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
		return ptUnit->ptPath->dwRoom;
	ptPath = ptUnit->ptPath;
	if (!(ptPath == NULL)) {
		return D2CommonPathGetRoom(ptPath); }
	return 0;
}

void __stdcall UnitSetTarget(PTUNIT ptUnit, HUNIT hTarget)
{
#line 2255
	D2ASSERT(ptUnit);
#line 2256
	D2ASSERT((ptUnit->eType == UNIT_PLAYER) || (ptUnit->eType == UNIT_MONSTER) ||
		(ptUnit->eType == UNIT_MISSILE));
	D2CommonPathSetTarget(ptUnit->ptPath, hTarget);
}

DWORD UNITSTHIS::UnitGetTargetType(PTUNIT ptUnit)
{
	DWORD dwType;
	DWORD dwId;

#line 2268
	D2ASSERT(ptUnit);
#line 2269
	D2ASSERT((ptUnit->eType == UNIT_PLAYER) || (ptUnit->eType == UNIT_MONSTER) ||
		(ptUnit->eType == UNIT_MISSILE));
	D2CommonPathGetTargetSnapshot(ptUnit->ptPath, &dwType, &dwId);
	return dwType;
}

DWORD UNITSTHIS::UnitGetTargetId(PTUNIT ptUnit)
{
	DWORD dwType;
	DWORD dwId;

#line 2285
	D2ASSERT(ptUnit);
#line 2286
	D2ASSERT((ptUnit->eType == UNIT_PLAYER) || (ptUnit->eType == UNIT_MONSTER) ||
		(ptUnit->eType == UNIT_MISSILE));
	D2CommonPathGetTargetSnapshot(ptUnit->ptPath, &dwType, &dwId);
	return dwId;
}

void __stdcall UnitSetTargetStrict(PTUNIT ptUnit, HUNIT hTarget)
{
#line 2303
	D2ASSERT(ptUnit);
#line 2305
	D2ASSERT((ptUnit->eType == UNIT_PLAYER) || (ptUnit->eType == UNIT_MONSTER));
	D2CommonPathSetTarget(ptUnit->ptPath, hTarget);
}

void __stdcall UnitUpdateMonsterSpeed(PTUNIT ptUnit)
{
	PTMONSTATS ptMonStats;
	int nSpeed;

#line 2316
	D2ASSERT(ptUnit);
	switch (ptUnit->eType)
	{
	case UNIT_MONSTER:
		break;
	default:
		return;
	}
	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
	if (ptUnit->ptStats == NULL)
	{
		ptUnit->nAnimRate = ptMonStats->nWalkAnimRate;
		return;
	}
	nSpeed = D2CommonStatsUnitGetValue(ptUnit, 67);
	ptUnit->nAnimRate = ptMonStats->nWalkAnimRate * nSpeed / 100;
}

PTUNITLOOKUPREC __stdcall UnitGetLookupBytes(DWORD dwArg1Unused, DWORD dwIndex, DWORD *pdwWalkVelocity,
	DWORD *pdwRunVelocity)
{
	PTUNITLOOKUPREC ptRec;

	ptRec = (PTUNITLOOKUPREC)D2CommonDataTblsGetCharStats(dwIndex);
	*pdwWalkVelocity = ptRec->WalkVelocity;
	*pdwRunVelocity = ptRec->RunVelocity;
	return ptRec;
}

PTANIMDATAREC __stdcall UnitSetAnimRecord(PTUNIT ptUnit, DWORD eType, DWORD nClass, DWORD nMode)
{
	PTANIMDATAREC ptRec;

#line 2372
	D2ASSERT(ptUnit);
	ptRec = AnimTblsFindSequence((DWORD)ptUnit, nClass, nMode, eType, ptUnit->ptInventory);
	ptUnit->ptAnimRecord = (BYTE *)ptRec;
	return ptRec;
}

static __inline void UnitMakeRunSpeedList(PTUNIT ptUnit, DWORD dwClass)
{
	DWORD dwStatList;
	PTUNITLOOKUPREC ptRec;
	int nWalk;

	dwStatList = D2CommonStatsAllocList(ptUnit->dwMemPool, 4, 0, ptUnit->eType, ptUnit->dwId);
	ptRec = (PTUNITLOOKUPREC)D2CommonDataTblsGetCharStats(dwClass);
	nWalk = ptRec->WalkVelocity;
	if (nWalk == 0)
		return;
	D2CommonStatsListSetValueA(dwStatList, 67, ptRec->RunVelocity * 100 / nWalk - 100);
	D2CommonStatsUnitLinkList(ptUnit, dwStatList);
}

static __inline DWORD UnitRollBelow(DWORD *ptSeed, DWORD dwRange)
{
	return dwRange ? D2CommonSeedRand(ptSeed, dwRange) : 0;
}

static __inline void UnitInitObjectAnim(PTUNIT ptUnit, int nNewMode)
{
	PTOBJECTCLASSINFO ptObjInfo;
	DWORD dwDelta;
	DWORD dwRand;

#line 2454
	D2ASSERT(nNewMode < NUM_OBJECTMODES);
	ptObjInfo = (PTOBJECTCLASSINFO)D2CommonObjTblsGetObject(ptUnit->dwClassId);
	ptUnit->nAnimLength = (&ptObjInfo->FrameCnt0)[nNewMode];
	dwDelta = ptObjInfo->FrameDelta[nNewMode];
	ptUnit->nAnimPos = (DWORD)ptObjInfo->Start[nNewMode] << 8;
	if (ptObjInfo->Sync)
	{
		ptUnit->nAnimRate = dwDelta;
		return;
	}
	dwRand = UnitRollBelow(&ptUnit->tSeed, dwDelta >> 3);
	ptUnit->nAnimRate = dwRand - (dwDelta >> 4) + dwDelta;
}

static __inline void UnitStartAnimSequence(PTUNIT ptUnit)
{
	DWORD dwOut1;
	DWORD dwOut2;
	DWORD dwOut3;
	DWORD dwOut4;

	ptUnit->hAnimSequence = (void *)D2CommonOverlayTblsGetAnimSequence(ptUnit);
#line 2753
	D2ASSERT(ptUnit->hAnimSequence);
	ptUnit->nSeqLength = D2CommonSequenceGetLengthFixed(ptUnit);
	ptUnit->nSeqPos = 0;
	ptUnit->nSeqRate = 256;
	ptUnit->nAnimLength = D2CommonSequenceGetFrameCount(ptUnit) << 8;
	D2CommonTokTblsGetFrameData(ptUnit->hAnimSequence, ptUnit->nSeqPos, &dwOut1, &dwOut2,
		&dwOut3, &dwOut4);
	ptUnit->dwSeqMode = dwOut1;
	ptUnit->nAnimPos = dwOut2 << 8;
	ptUnit->bAnimEvent = (BYTE)dwOut4;
	ptUnit->dwFlags0 |= 16384;
}

BOOL __stdcall UnitInitModeAnim(PTUNIT ptUnit)
{
	DWORD dwType;
	DWORD dwClass;
	int nNewMode;
	PTMISSILETBLREC ptMissileRec;

#line 2399
	D2ASSERT(ptUnit);
	ptUnit->bAnimEvent = 0;
	ptUnit->nAnimPos = D2CommonUnitGetWClassModeValue(ptUnit) << 8;
	dwType = ptUnit->eType;
	nNewMode = (int)ptUnit->dwMode;
	dwClass = ptUnit->dwClassId;
	D2CommonSkillsClearFlagIfMatched(ptUnit, &dwType, &dwClass, &nNewMode);
	switch (dwType)
	{
	case UNIT_PLAYER:
		if (nNewMode == 3 || nNewMode == 19)
			UnitMakeRunSpeedList(ptUnit, dwClass);
		if (nNewMode == 18)
		{
			UnitStartAnimSequence(ptUnit);
			D2CommonUnitUpdateAnimRate(ptUnit);
			break;
		}
		ptUnit->hAnimSequence = NULL;
		D2CommonUnitUpdateAnimRate(ptUnit);
		ptUnit->nAnimLength = ((PTUNITSEQREC)ptUnit->ptAnimRecord)->dwFrames << 8;
		break;
	case UNIT_MONSTER:
		if (nNewMode == 14)
		{
			UnitStartAnimSequence(ptUnit);
			D2CommonUnitUpdateAnimRate(ptUnit);
			break;
		}
		D2CommonUnitUpdateAnimRate(ptUnit);
		ptUnit->hAnimSequence = NULL;
		ptUnit->nAnimLength = ((PTUNITSEQREC)ptUnit->ptAnimRecord)->dwFrames << 8;
		break;
	case UNIT_OBJECT:
		UnitInitObjectAnim(ptUnit, nNewMode);
		break;
	case UNIT_ITEM:
		switch (nNewMode)
		{
		case 3:
			ptUnit->nAnimRate = 0;
			ptUnit->nAnimLength = 4352;
			ptUnit->nAnimPos = 4096;
			break;
		case 5:
			ptUnit->nAnimRate = 256;
			ptUnit->nAnimLength = 4352;
			break;
		}
		break;
	case UNIT_MISSILE:
		ptMissileRec = (PTMISSILETBLREC)D2CommonMissileTblsGetMissile(ptUnit->dwClassId);
		ptUnit->nAnimRate = 256;
		ptUnit->nAnimLength = (DWORD)ptMissileRec->AnimLen << 8;
		break;
	}
	return TRUE;
}

BOOL __stdcall UnitSetMode(PTUNIT ptUnit, DWORD dwNewMode)
{
#line 2538
	D2ASSERT(ptUnit);
#line 2539
	D2ASSERT(ptUnit->eType != UNIT_ROOMTILE);
	D2CommonUnitRoomAddToChangedList(ptUnit);
	ptUnit->dwFlags0 |= 1;
	if (!(ptUnit->dwMode == dwNewMode)) {
		ptUnit->dwMode = dwNewMode;
		D2CommonStatsUnitFreeStateLists(ptUnit);
		D2CommonUnitInitModeAnim(ptUnit);
		return TRUE; }
	return FALSE;
}

DWORD __stdcall UnitGetRoomChanged(PTUNIT ptUnit)
{
#line 2562
	D2ASSERT(ptUnit);
	switch (ptUnit->eType)
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
		return ptUnit->ptPath->field_20;
	}
	return D2CommonPathTestLeftRoom(ptUnit->ptPath);
}

void __stdcall UnitSetRoomChanged(PTUNIT ptUnit, DWORD dwValue)
{
#line 2580
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		ptUnit->ptPath->field_20 = dwValue;
		return;
	}
	D2CommonPathSetLeftRoom(ptUnit->ptPath, dwValue);
}

void __stdcall UnitSetChanged(HUNIT hUnit, DWORD fEnable)
{
	DWORD dwFlags;
	BOOL fPlayer = FALSE;

	if (fEnable)
	{
		D2CommonUnitRoomAddToChangedList(hUnit);
		dwFlags = hUnit->dwFlags | 1;
		hUnit->dwFlags = dwFlags;
		if (hUnit->eType == UNIT_PLAYER)
			fPlayer = TRUE;
	}
	else
	{
		hUnit->dwFlags &= 0xFFFFFFFEu;
	}
	if (fPlayer)
		hUnit->dwFlags = dwFlags | 2;
}

static struct { DWORD dwClass; int nRecord; } sgtPlayerInvRecordTable[5] =
{
	{ 0, 0 },
	{ 1, 1 },
	{ 2, 2 },
	{ 3, 3 },
	{ 4, 4 },
};

static int sgnPlayerInvRecordCount = 5;

int __stdcall UnitGetInvRecordIndex(HUNIT hUnit, BYTE bPage)
{
	DWORD i;
	DWORD nCount;
	DWORD dwClass;
	int nRecord;

#line 2629
	D2ASSERT(hUnit);
	nRecord = -1;
	switch (hUnit->eType)
	{
	case UNIT_PLAYER:
		switch (bPage)
		{
		case 1:
			return 6;
		case 2:
			return 7;
		case 4:
			return 8;
		case 3:
			return 9;
		}
		nCount = sgnPlayerInvRecordCount;
		dwClass = hUnit->dwClassId;
		for (i = 0; i < nCount; i++)
		{
			if (dwClass == sgtPlayerInvRecordTable[i].dwClass)
			{
				nRecord = sgtPlayerInvRecordTable[i].nRecord;
				break;
			}
		}
		break;
	case UNIT_MONSTER:
		return 5;
	case UNIT_OBJECT:
		switch (hUnit->dwClassId)
		{
		case 338:
			return 10;
		case 339:
			return 11;
		}
		return -1;
	}
	return nRecord;
}
#line 2664

DWORD __stdcall UnitTakeLight(PTUNIT ptUnit)
{
	DWORD dwOld;

#line 2688
	D2ASSERT(ptUnit);
	dwOld = ptUnit->field_84;
	ptUnit->field_84 = 0;
	return dwOld;
}

DWORD __stdcall UnitGetAnimMode(PTUNIT ptUnit)
{
#line 2700
	D2ASSERT(ptUnit);
	if ((ptUnit->eType == UNIT_PLAYER || ptUnit->eType == UNIT_MONSTER) && ptUnit->hAnimSequence != NULL)
		return ptUnit->dwSeqMode;
	return ptUnit->dwMode;
}

void __stdcall UnitSetAnimMode(PTUNIT ptUnit, DWORD dwValue)
{
#line 2716
	D2ASSERT(ptUnit);
	if ((ptUnit->eType == UNIT_PLAYER || ptUnit->eType == UNIT_MONSTER) && ptUnit->hAnimSequence != NULL)
	{
		ptUnit->dwSeqMode = dwValue;
		return;
	}
	ptUnit->dwMode = dwValue;
}

void __stdcall UnitInitAnimState(PTUNIT ptUnit)
{
#line 2750
	D2ASSERT(ptUnit);
	UnitStartAnimSequence(ptUnit);
}

void __stdcall UnitSetAnimSequencePos(PTUNIT ptUnit, DWORD dwFrame)
{
#line 2768
	D2ASSERT(ptUnit);
	ptUnit->nSeqPos = (ptUnit->nSeqPos & 255) + (dwFrame << 8);
}

void __stdcall UnitSetAnimEventFlagByte(PTUNIT ptUnit, DWORD dwIndex);

void __stdcall UnitStepAnimState(PTUNIT ptUnit)
{
	void *hAnimSequence;

	hAnimSequence = ptUnit->hAnimSequence;
	ptUnit->bAnimEvent = 0;
	if (hAnimSequence != NULL)
	{
		DWORD dwRate;
		DWORD dwOldFrame;
		DWORD dwOut1;
		DWORD dwOut2;
		DWORD dwOut3;
		DWORD dwOut4;

		ptUnit->dwFlags0 &= 0xFFFFBFFF;
		dwRate = ptUnit->nSeqRate;
		ptUnit->nSeqPos += dwRate;
		if ((int)ptUnit->nSeqPos >= (int)ptUnit->nSeqLength)
			ptUnit->nSeqPos -= ptUnit->nSeqLength;
		dwOldFrame = ptUnit->dwSeqMode;
		ptUnit->nAnimLength -= dwRate;
		D2CommonTokTblsGetFrameData(hAnimSequence, ptUnit->nSeqPos, &dwOut1, &dwOut2,
			&dwOut3, &dwOut4);
		ptUnit->dwSeqMode = dwOut1;
		ptUnit->nAnimPos = dwOut2 << 8;
		ptUnit->bAnimEvent = (BYTE)dwOut4;
		if (dwOut1 != dwOldFrame)
			ptUnit->dwFlags0 |= 16384;
		return;
	}
	{
		int nStart;
		DWORD dwAccum;
		DWORD dwLimit;
		int nUpper;
		int i;

		if ((int)ptUnit->nAnimRate >= 256)
			nStart = ((int)ptUnit->nAnimPos >> 8) + 1;
		else
			nStart = (int)ptUnit->nAnimPos >> 8;
		dwLimit = ptUnit->nAnimLength;
		dwAccum = ptUnit->nAnimPos + ptUnit->nAnimRate;
		ptUnit->nAnimPos = dwAccum;
		while ((int)dwAccum >= (int)dwLimit)
		{
			nUpper = (int)dwLimit >> 8;
			for (i = nStart; i < nUpper; i++)
				UnitSetAnimEventFlagByte(ptUnit, i);
			ptUnit->nAnimPos -= dwLimit;
			nStart = 0;
			ptUnit->nAnimPos += D2CommonUnitGetWClassModeValue(ptUnit) << 8;
			dwAccum = ptUnit->nAnimPos;
			dwLimit = ptUnit->nAnimLength;
		}
		nUpper = (int)ptUnit->nAnimPos >> 8;
		for (i = nStart; i <= nUpper; i++)
			UnitSetAnimEventFlagByte(ptUnit, i);
	}
}

void __stdcall UnitStepAnimPos(PTUNIT ptUnit)
{
	ptUnit->nAnimPos += ptUnit->nAnimRate;
	if ((int)ptUnit->nAnimPos >= (int)ptUnit->nAnimLength)
		ptUnit->nAnimPos -= ptUnit->nAnimLength;
}

void __stdcall UnitStepAnimPosTo(PTUNIT ptUnit, DWORD dwLoopFrame, DWORD dwFrameCount)
{
	DWORD dwNew;

	dwFrameCount <<= 8;
	ptUnit->nAnimPos += ptUnit->nAnimRate;
	dwNew = ptUnit->nAnimPos;
	if ((int)dwNew < (int)dwFrameCount)
		return;
	ptUnit->nAnimPos = dwNew + (dwLoopFrame << 8) - dwFrameCount;
}

struct UNITMODEANIMREC
{
	DWORD	field_00;
	DWORD	field_04;
	DWORD	field_08;
	DWORD	field_0c;
	DWORD	field_10;
};
typedef struct UNITMODEANIMREC UNITMODEANIMREC;

struct UNITMODEANIMTABLES
{
	UNITMODEANIMREC	atPlayer[20];
	UNITMODEANIMREC	atMonster[16];
	DWORD	adwStats5C[3];
	DWORD	adwStats62[3];
	DWORD	adwStats68[3];
	DWORD	adwStats65[3];
	DWORD	adwStats5F[3];
};
typedef struct UNITMODEANIMTABLES UNITMODEANIMTABLES;

static const UNITMODEANIMTABLES sgtModeAnimTables =
{
	{
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 1, 0, 1, 0, 0 },
		{ 1, 0, 1, 0, 0 },
		{ 1, 0, 1, 0, 0 },
		{ 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 0 },
		{ 0, 0, 0, 0, 0 },
	},
	{
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 1, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 1, 1, 0, 0, 0 },
		{ 1, 1, 0, 0, 0 },
		{ 1, 1, 0, 0, 0 },
		{ 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 1 },
		{ 0, 0, 0, 0, 0 },
		{ 1, 0, 1, 0, 0 },
		{ 0, 1, 0, 0, 0 },
	},
	{ 92, 93, 94 },
	{ 98, 99, 100 },
	{ 104, 105, 106 },
	{ 101, 102, 103 },
	{ 95, 96, 97 },
};

#define sgtPlayerModeAnim	(sgtModeAnimTables.atPlayer)
#define sgtMonsterModeAnim	(sgtModeAnimTables.atMonster)
#define sgcadwAnimStats5C	(sgtModeAnimTables.adwStats5C)
#define sgcadwAnimStats62	(sgtModeAnimTables.adwStats62)
#define sgcadwAnimStats68	(sgtModeAnimTables.adwStats68)
#define sgcadwAnimStats65	(sgtModeAnimTables.adwStats65)
#define sgcadwAnimStats5F	(sgtModeAnimTables.adwStats5F)

static __inline int UnitSumStats(PTUNIT ptUnit, const DWORD *pdwStats)
{
	int nSum;
	int i;

	nSum = 0;
	for (i = 0; i < 3; i++)
		nSum += (int)D2CommonStatsGetTotalValue(ptUnit, pdwStats[i]);
	return nSum;
}

static __inline int UnitGetModeSpeedBase(DWORD eTOU, DWORD nClass, DWORD nMode)
{
	int nSpeed;

	switch (eTOU)
	{
	case UNIT_PLAYER:
		return nMode == 3 ? 101 : 213;
	case UNIT_MONSTER:
		nSpeed = ((PTMONSTATS)D2CommonMonsterTblsGetStats(nClass))->nWalkAnimRate;
		if (nMode == 15)
			nSpeed /= 2;
		return nSpeed;
	}
	return 0;
}

static __inline int UnitGetClassVelocity(DWORD eTOU, DWORD nClass)
{
	switch (eTOU)
	{
	case UNIT_PLAYER:
		return ((PTUNITLOOKUPREC)D2CommonDataTblsGetCharStats(nClass))->WalkVelocity << 8;
	case UNIT_MONSTER:
		return ((PTMONSTATS)D2CommonMonsterTblsGetStats(nClass))->nVelocity << 8;
	}
	return 0;
}

static __inline BOOL UnitTestModeMoving(PTUNIT ptUnit, DWORD eTOU, DWORD nMode)
{
	const UNITMODEANIMREC *ptRec;
	PTSKILL ptSkill;
	DWORD dwFlags;

	switch (eTOU)
	{
	case UNIT_PLAYER:
		ptRec = &sgtPlayerModeAnim[nMode];
		break;
	case UNIT_MONSTER:
		if (((PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId))->bIsNPC)
		{
			switch (nMode)
			{
			case 2:
			case 15:
				return TRUE;
			}
			return FALSE;
		}
		ptRec = &sgtMonsterModeAnim[nMode];
		break;
	default:
#line 3023
		D2ASSERT((eTOU == UNIT_PLAYER) || (eTOU == UNIT_MONSTER));
		return FALSE;
	}
	if (ptRec->field_04)
		return TRUE;
	if (!ptRec->field_00)
		return FALSE;
	ptSkill = SkillsHeadGetThirdCached(ptUnit->field_c8);
	if (ptSkill == NULL)
		return FALSE;
	dwFlags = D2CommonSkillsGetNodeFlags(ptSkill);
	if (!(dwFlags & 1))
		return FALSE;
	if (!(dwFlags & 4096)) {
		return TRUE; }
	return FALSE;
}

static __inline BOOL UnitTestModeAttacking(PTUNIT ptUnit, DWORD eTOU, DWORD nMode)
{
	const UNITMODEANIMREC *ptRec;
	PTSKILL ptSkill;

	switch (eTOU)
	{
	case UNIT_PLAYER:
		ptRec = &sgtPlayerModeAnim[nMode];
		break;
	case UNIT_MONSTER:
		ptRec = &sgtMonsterModeAnim[nMode];
		break;
	default:
#line 3051
		D2ASSERT((eTOU == UNIT_PLAYER) || (eTOU == UNIT_MONSTER));
		return FALSE;
	}
	if (ptRec->field_0c)
		return TRUE;
	if (!ptRec->field_08)
		return FALSE;
	ptSkill = SkillsHeadGetThirdCached(ptUnit->field_c8);
	if (ptSkill == NULL)
		return FALSE;
	if (!((PTSKILLSTXT)D2CommonDataTblsGetSkill(SkillsGetNodeType(ptSkill)))->UseAttackRate)
		return FALSE;
	return TRUE;
}

static __inline BOOL UnitTestModePair(DWORD eTOU, DWORD nMode, DWORD nPlayerMode, DWORD nMonsterMode)
{
	switch (eTOU)
	{
	case UNIT_PLAYER:
		if (nMode == nPlayerMode)
			return TRUE;
		break;
	case UNIT_MONSTER:
		if (nMode == nMonsterMode)
			return TRUE;
		break;
	}
	return FALSE;
}

static __inline DWORD UnitGetModeFixedRate(DWORD eTOU, DWORD nMode)
{
	switch (eTOU)
	{
	case UNIT_PLAYER:
		return sgtPlayerModeAnim[nMode].field_10;
	case UNIT_MONSTER:
		return sgtMonsterModeAnim[nMode].field_10;
	}
	return 0;
}

void __stdcall UnitUpdateAnimRate(PTUNIT ptUnit)
{
	DWORD eTOU;
	DWORD nClass;
	DWORD nMode;
	PTANIMDATAREC ptAnim;
	int nSum;
	int nRate;
	DWORD dwInventory;
	HUNIT hRight;
	HUNIT hLeft;

	eTOU = ptUnit->eType;
	nClass = ptUnit->dwClassId;
	nMode = ptUnit->dwMode;
	if (eTOU == UNIT_ITEM)
		return;
	D2CommonSkillsClearFlagIfMatched(ptUnit, &eTOU, &nClass, &nMode);
	ptAnim = UnitSetAnimRecord(ptUnit, eTOU, nClass, nMode);

	if (UnitTestModePair(eTOU, nMode, 10, 7))
	{
		nSum = UnitSumStats(ptUnit, sgcadwAnimStats68);
		ptUnit->nAnimRate = (DWORD)((nSum + 100) * ptAnim->dwSpeed) / 100;
		return;
	}
	if (UnitTestModePair(eTOU, nMode, 9, 6))
	{
		nSum = UnitSumStats(ptUnit, sgcadwAnimStats65);
		nRate = 50;
		if (D2CommonStatsGetState(ptUnit, 101))
			nRate = 100;
		nSum += nRate;
		nRate = (int)((DWORD)(nSum * ptAnim->dwSpeed) / 100);
		if (nRate < 1)
			nRate = 1;
		ptUnit->nAnimRate = (DWORD)nRate;
		return;
	}
	if (UnitTestModePair(eTOU, nMode, 4, 3))
	{
		nSum = UnitSumStats(ptUnit, sgcadwAnimStats62);
		ptUnit->nAnimRate = (DWORD)((nSum + 50) * ptAnim->dwSpeed) / 100;
		return;
	}
	if (UnitTestModePair(eTOU, nMode, 19, 13))
	{
		ptUnit->nAnimRate = (DWORD)UnitGetModeSpeedBase(eTOU, nClass, nMode);
		D2CommonPathSetVelocity(ptUnit->ptPath, 4096);
		return;
	}
	if (UnitTestModeMoving(ptUnit, eTOU, nMode))
	{
		nSum = UnitSumStats(ptUnit, sgcadwAnimStats5F) + D2CommonStatsUnitGetValue(ptUnit, 67);
		if (nSum < 25)
			nSum = 25;
		ptUnit->nAnimRate = (DWORD)(UnitGetModeSpeedBase(eTOU, nClass, nMode) * nSum / 100);
		D2CommonPathSetVelocity(ptUnit->ptPath, UnitGetClassVelocity(eTOU, nClass) * nSum / 100);
		return;
	}
	if (UnitTestModeAttacking(ptUnit, eTOU, nMode))
	{
		D2CommonUnitApplyPrimaryWeaponSwap(ptUnit);
		nSum = UnitSumStats(ptUnit, sgcadwAnimStats5C) + D2CommonStatsUnitGetValue(ptUnit, 68);
		if (eTOU == UNIT_PLAYER && ptUnit->dwClassId == 4)
		{
			dwInventory = ptUnit->ptInventory;
			hRight = (HUNIT)D2CommonINVENTORYGetLocSlot(dwInventory, 4);
			if (!D2CommonITEMSTestEffectsActive(hRight))
				hRight = NULL;
			hLeft = (HUNIT)D2CommonINVENTORYGetLocSlot(dwInventory, 5);
			if (!D2CommonITEMSTestEffectsActive(hLeft))
				hLeft = NULL;
			if (hRight != NULL && hLeft != NULL && D2CommonITEMSIsWeapon(hRight)
				&& D2CommonITEMSIsWeapon(hLeft))
				nSum += 10;
		}
		if (nSum < 15)
			nSum = 15;
		ptUnit->nAnimRate = (DWORD)(ptAnim->dwSpeed * nSum) / 100;
		D2CommonUnitApplyEquipStatSwap(ptUnit);
		return;
	}
	if (UnitGetModeFixedRate(eTOU, nMode))
	{
		ptUnit->nAnimRate = ptAnim->dwSpeed;
		return;
	}
	nRate = D2CommonStatsUnitGetValue(ptUnit, 69);
	if (nRate < 0)
		nRate = 0;
	ptUnit->nAnimRate = (DWORD)(ptAnim->dwSpeed * nRate) / 100;
}

void __stdcall UnitSetMissileAnimRate(HUNIT hUnit, DWORD dwValue)
{
#line 3285
	D2ASSERT(UnitGetType (hUnit) == UNIT_MISSILE);
	hUnit->nAnimRate = dwValue;
}

BOOL __stdcall UnitTestAnimComplete(PTUNIT ptUnit)
{
#line 3303
	D2ASSERT(ptUnit);
	if (ptUnit->hAnimSequence != NULL)
		return ptUnit->nAnimLength == 0;
	return (int)(ptUnit->nAnimRate + ptUnit->nAnimPos) >= (int)ptUnit->nAnimLength;
}

void __stdcall UnitGetAnimFrameAndCount(PTUNIT ptUnit, DWORD *pdwFrame, DWORD *pdwCount)
{
#line 3317
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT) {
		*pdwFrame = (DWORD)((int)ptUnit->nAnimPos >> 8);
		*pdwCount = (&((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo->FrameCnt0)[ptUnit->dwMode];
	} else {
		*pdwFrame = (DWORD)((int)ptUnit->nAnimPos >> 8);
		*pdwCount = (DWORD)((int)ptUnit->nAnimLength >> 8);
	}
}

void __stdcall UnitGetAnimPosAndLength(HUNIT hUnit, DWORD *pdwPos, DWORD *pdwLength)
{
	if (hUnit == NULL)
	{
		*pdwPos = 0;
		*pdwLength = 0;
		return;
	}
	if (hUnit->eType == UNIT_OBJECT)
	{
		*pdwPos = hUnit->nAnimPos;
		*pdwLength = (&((PTOBJECTUNITDATA)hUnit->uTypeData)->ptClassInfo->FrameCnt0)[hUnit->dwMode];
	}
	else
	{
		*pdwPos = hUnit->nAnimPos;
		*pdwLength = hUnit->nAnimLength;
	}
}

void __stdcall UnitSetAnimEventFlagByte(PTUNIT ptUnit, DWORD dwIndex)
{
	PBYTE pRec;

#line 3355
	D2ASSERT(ptUnit);
	pRec = ptUnit->ptAnimRecord;
	if (pRec[16 + dwIndex] == 1)
		ptUnit->bAnimEvent = 1;
	if (pRec[16 + dwIndex] == 2)
		ptUnit->bAnimEvent = 2;
	if (pRec[16 + dwIndex] == 3)
		ptUnit->bAnimEvent = 3;
	if (pRec[16 + dwIndex] == 4)
		ptUnit->bAnimEvent = 4;
}

DWORD __stdcall UnitGetAnimFrameByte(PTUNIT ptUnit, int nFrame)
{
	void *hAnimSequence;
	DWORD dwOut1;
	DWORD dwOut2;
	DWORD dwOut3;
	DWORD dwOut4;
	BOOL fAttack;

#line 3369
	D2ASSERT(ptUnit);
#line 3370
	D2ASSERT(nFrame < MAX_FRAMES);
	fAttack = FALSE;
	switch (ptUnit->eType)
	{
	case UNIT_PLAYER:
		if (ptUnit->dwMode == 18)
			fAttack = TRUE;
	case UNIT_MONSTER:
		if (ptUnit->dwMode == 14)
			fAttack = TRUE;
	}
	if (fAttack)
	{
		hAnimSequence = (void *)D2CommonOverlayTblsGetAnimSequence(ptUnit);
		D2CommonTokTblsGetFrameData(hAnimSequence, nFrame << 8, &dwOut1, &dwOut2, &dwOut3, &dwOut4);
		return dwOut4;
	}
	return ptUnit->ptAnimRecord[16 + nFrame];
}

DWORD __stdcall UnitTestObjectHasCollision(HUNIT hUnit)
{
	if (!(hUnit->eType != UNIT_OBJECT)) {
		return (&((PTOBJECTUNITDATA)hUnit->uTypeData)->ptClassInfo->HasCollision0)[hUnit->dwMode]; }
	return 1;
}

PTSKILL __stdcall UnitFindSkillByType(HUNIT hUnit, DWORD nSkillId)
{
	PTSKILLTYPEINFO ptTable;
	PTSKILL ptSkill;

	ptTable = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
#line 3414
	D2ASSERT(ptTable);
	for (ptSkill = UnitGetFirstSkill(hUnit); ptSkill != NULL; ptSkill = ptSkill->ptNext)
	{
		if (ptSkill->ptType == ptTable)
			return ptSkill;
	}
	return NULL;
}

int __stdcall UnitIsDoor(PTUNIT ptUnit)
{
#line 3435
	D2ASSERT(ptUnit);
	if (!(ptUnit->eType != UNIT_OBJECT)) {
		return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo->IsDoor; }
	return 0;
}

BOOL __fastcall UnitDoorOpensAlongY(HUNIT ptUnit)
{
#line 3449
	D2ASSERT(ptUnit->eType == UNIT_OBJECT);
	return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo->Orientation == 1;
}

DWORD __stdcall UnitTestObjectSubclassBitZero(HUNIT hUnit)
{
	if (!(hUnit->eType != UNIT_OBJECT)) {
		if (((PTOBJECTUNITDATA)hUnit->uTypeData)->ptClassInfo->SubClass & 1)
			return 1; }
	return 0;
}

DWORD __stdcall UnitGetObjectClassInfoPtr(PTUNIT ptUnit)
{
#line 3479
	D2ASSERT(ptUnit);
	if (!(ptUnit->eType != UNIT_OBJECT))
		return (DWORD)((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo;
#line 3486
	D2FATAL(NULL);

	return (DWORD)((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo;
}

DWORD __stdcall UnitGetObjectShrine(PTUNIT ptUnit)
{
#line 3497
	D2ASSERT(ptUnit);
	if (!(ptUnit->eType != UNIT_OBJECT))
		return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->dwShrineData;
#line 3504
	D2FATAL(NULL);

	return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->dwShrineData;
}

void __stdcall UnitSetObjectShrine(PTUNIT ptUnit, DWORD dwValue)
{
#line 3515
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT)
	{
		((PTOBJECTUNITDATA)ptUnit->uTypeData)->dwShrineData = dwValue;
		return;
	}
#line 3522
	D2FATAL(NULL);
}

void __stdcall UnitPathSetExtra(HUNIT hUnit, DWORD dwX, DWORD dwY)
{
	PTPATH hPath;

	hPath = hUnit->ptPath;
#line 3532
	D2ASSERT(hPath);
	D2CommonPathTurnToSquare(hPath, dwX, dwY, 0);
}

DWORD __stdcall UnitSyncPosition(HUNIT hUnit)
{
	PTPATH ptPath;
	DWORD dwX1;
	DWORD dwY1;
	DWORD dwCachedX;
	DWORD dwCachedY;
	DWORD dwX2;
	DWORD dwY2;

#line 3542
	D2ASSERT(hUnit);

	if (hUnit->eType == UNIT_OBJECT || ((int)hUnit->eType > UNIT_MISSILE && (int)hUnit->eType <= UNIT_ROOMTILE))
		dwX1 = hUnit->ptPath->tRoomTileCoord.dwX;
	else
	{
		ptPath = hUnit->ptPath;
		dwX1 = (ptPath != NULL) ? D2CommonPathGetGameX(ptPath) : 0;
	}
	if (D2CommonPathGetTargetXRaw((DWORD)hUnit->ptPath) == dwX1)
	{
		if (hUnit->eType == UNIT_OBJECT || ((int)hUnit->eType > UNIT_MISSILE && (int)hUnit->eType <= UNIT_ROOMTILE))
			dwY1 = hUnit->ptPath->tRoomTileCoord.dwY;
		else
		{
			ptPath = hUnit->ptPath;
			dwY1 = (ptPath != NULL) ? D2CommonPathGetGameY(ptPath) : 0;
		}
		if (D2CommonPathGetTargetYRaw((DWORD)hUnit->ptPath) == dwY1)
			return D2CommonPathGetTargetFacingRaw((DWORD)hUnit->ptPath);
	}

	dwCachedY = D2CommonPathGetTargetYRaw((DWORD)hUnit->ptPath);
	dwCachedX = D2CommonPathGetTargetXRaw((DWORD)hUnit->ptPath);

	if (hUnit->eType == UNIT_OBJECT || ((int)hUnit->eType > UNIT_MISSILE && (int)hUnit->eType <= UNIT_ROOMTILE))
		dwX2 = hUnit->ptPath->tRoomTileCoord.dwX;
	else
	{
		ptPath = hUnit->ptPath;
		dwX2 = (ptPath != NULL) ? D2CommonPathGetGameX(ptPath) : 0;
	}

	if (hUnit->eType == UNIT_OBJECT || ((int)hUnit->eType > UNIT_MISSILE && (int)hUnit->eType <= UNIT_ROOMTILE))
		dwY2 = hUnit->ptPath->tRoomTileCoord.dwY;
	else
	{
		ptPath = hUnit->ptPath;
		dwY2 = (ptPath != NULL) ? D2CommonPathGetGameY(ptPath) : 0;
	}

	return D2CommonStepGetSquareDirection(dwX2, dwY2, dwCachedX, dwCachedY);
}

void __stdcall UnitSetLinkedUnitIds(PTUNIT ptUnit, DWORD dwB4, DWORD dwB8)
{
#line 3555
	D2ASSERT(ptUnit);
	ptUnit->dwLinkedUnitType = dwB4;
	ptUnit->dwLinkedUnitId = dwB8;
	if (ptUnit->ptStats != NULL)
		D2CommonStatsUnitSetActiveStateBit(ptUnit, 98, 1);
	ptUnit->dwFlags |= 1024;
}

void __stdcall UnitSetLinkedUnit(PTUNIT ptUnit, DWORD *pdwPair)
{
#line 3570
	D2ASSERT(ptUnit);
	if (pdwPair != NULL)
	{
		ptUnit->dwLinkedUnitType = pdwPair[0];
		ptUnit->dwLinkedUnitId = pdwPair[2];
		if (ptUnit->ptStats != NULL)
			D2CommonStatsUnitSetActiveStateBit(ptUnit, 98, 1);
		ptUnit->dwFlags |= 1024;
	}
	else
	{
		ptUnit->dwLinkedUnitType = 0;
		ptUnit->dwLinkedUnitId = 0;
		if (ptUnit->ptStats != NULL)
			D2CommonStatsUnitSetActiveStateBit(ptUnit, 98, 0);
		ptUnit->dwFlags &= 0xFFFFFBFF;
	}
}

void __stdcall UnitSetSecondLinkedUnit(PTUNIT ptUnit, DWORD *pdwPair)
{
#line 3593
	D2ASSERT(ptUnit);
	if (pdwPair != NULL)
	{
		ptUnit->dwLastAttackerType = pdwPair[0];
		ptUnit->dwLastAttackerId = pdwPair[2];
		ptUnit->dwFlags |= 131072;
	}
	else
	{
		ptUnit->dwLastAttackerType = 0;
		ptUnit->dwLastAttackerId = 0;
		ptUnit->dwFlags &= 0xFFFDFFFF;
	}
}

DWORD __stdcall UnitSetPositionExtra(PTUNIT ptUnit, DWORD dwTargetX, DWORD dwTargetY)
{
	PTPATH ptPath;
	DWORD dwX;
	DWORD dwY;

#line 1747
	D2ASSERT(ptUnit);
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		dwX = ptUnit->ptPath->tRoomTileCoord.dwX;
	}
	else
	{
		ptPath = ptUnit->ptPath;
		dwX = (ptPath != NULL) ? D2CommonPathGetGameX(ptPath) : 0;
	}
	if (ptUnit->eType == UNIT_OBJECT || ((int)ptUnit->eType > UNIT_MISSILE && (int)ptUnit->eType <= UNIT_ROOMTILE))
	{
		dwY = ptUnit->ptPath->tRoomTileCoord.dwY;
	}
	else
	{
		ptPath = ptUnit->ptPath;
		dwY = (ptPath != NULL) ? D2CommonPathGetGameY(ptPath) : 0;
	}
	return D2CommonStepGetSquareDirection(dwX, dwY, dwTargetX, dwTargetY);
}

void __stdcall UnitAddAnimOverlay(PTUNIT ptUnit, DWORD eOverlay, int nFrames)
{
	void *pRecord;

	pRecord = (void *)D2CommonStatsFindListByMask(ptUnit, 128);
	if (pRecord == NULL)
	{
		pRecord = (void *)D2CommonStatsAllocList(ptUnit->dwMemPool, 128, pRecord, ptUnit->eType,
			ptUnit->dwId);
		D2CommonStatsUnitLinkList(ptUnit, pRecord);
	}
#line 3630
	D2ASSERT(((int)eOverlay) < 256);
#line 3631
	D2ASSERT(nFrames < 256);
	D2CommonStatsUnitSetHasOverlay(ptUnit);
	D2CommonStatsListSetValuePacked(pRecord, 176, eOverlay & 255, nFrames);
	D2CommonUnitRoomAddToChangedList(ptUnit);
}

static struct { DWORD dwCode; DWORD dwValue; } sgtItemCodeTable[10] =
{
	{ 543973996, 1 },
	{ 543973997, 0 },
	{ 543973992, 3 },
	{ 543974006, 4 },
	{ 543974004, 5 },
	{ 543321210, 6 },
	{ 543323770, 6 },
	{ 543321466, 6 },
	{ 543323258, 6 },
	{ 543320186, 6 },
};
static DWORD sgnItemCodeTable = arrsize(sgtItemCodeTable);

DWORD __stdcall UnitGetItemCodeValue(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;
	DWORD i;

#line 3645
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 3662
	D2ASSERT(ptStats);
	for (i = 0; i < sgnItemCodeTable; i++)
	{
		if (ptStats->code == sgtItemCodeTable[i].dwCode)
			return sgtItemCodeTable[i].dwValue;
	}
	return 2;
}

int __stdcall UnitCalcLifePercent(HUNIT hUnit)
{
	DWORD dwA;
	DWORD dwB;

	dwA = D2CommonStatsUnitGetValue(hUnit, 6);
	dwB = D2CommonStatsUnitCalcMaxHp(hUnit);
	if (dwB)
		return (int)((int)dwA * 100 / (int)dwB);
	return 0;
}

BOOL __stdcall UnitTestMonsterSoft(HUNIT hUnit)
{
	PTMONSTERTBLREC ptStats;

#line 3688
	D2ASSERT(UnitGetType (hUnit) == UNIT_MONSTER);
	ptStats = (PTMONSTERTBLREC)D2CommonMonsterTblsGetStats(hUnit->dwClassId);
#line 3692
	D2ASSERT(ptStats);
	return ptStats->bSoft != 0;
}

DWORD __stdcall DungeonGetPortalLevelBit(int nLevelId);

void __stdcall UnitCreatePlrInfo(PTUNIT ptUnit)
{
	PTPLRINFO ptPlrInfo;
	int i;

#line 3706
	D2ASSERT(ptUnit);
#line 3708
	D2ASSERTMSG(ptUnit->uTypeData == 0, "ptUnit->uTypeData.ptPlrInfo == NULL");
#line 3709
	ptPlrInfo = (PTPLRINFO)D2ALLOCPOOL(ptUnit->dwMemPool, sizeof(PLRINFO));
	ptUnit->uTypeData = (DWORD)ptPlrInfo;
	for (i = 0; i < (int)(sizeof(PLRINFO) / sizeof(DWORD)); i++)
		((DWORD *)ptPlrInfo)[i] = 0;
	((PTPLRINFO)ptUnit->uTypeData)->szName[0] = 0;
	for (i = 0; i < 3; i++)
	{
		((PTPLRINFO)ptUnit->uTypeData)->apQuestRecords[i] = D2CommonQuestRecordNew(ptUnit->dwMemPool);
		((PTPLRINFO)ptUnit->uTypeData)->apwWaypoints[i]
			= (DWORD)D2CommonWaypointNewHistory(ptUnit->dwMemPool);
	}
	((PTPLRINFO)ptUnit->uTypeData)->dwPortalLevelBits = DungeonGetPortalLevelBit(LEVEL_ACT_1_TOWN);
	((PTPLRINFO)ptUnit->uTypeData)->bBusy = 0;
	D2CommonMonsterKillsLogClearRecord(ptUnit);
}

void __stdcall UnitFreePlrInfo(DWORD dwPool, PTUNIT ptUnit)
{
	int i;

#line 3729
	D2ASSERT(ptUnit);
#line 3731
	D2ASSERTMSG(ptUnit->uTypeData, "ptUnit->uTypeData.ptPlrInfo");
	for (i = 0; i < 3; i++)
	{
		D2CommonQuestRecordFree(dwPool, ((PTPLRINFO)ptUnit->uTypeData)->apQuestRecords[i]);
		D2CommonWaypointFreeHistory(dwPool, ((PTPLRINFO)ptUnit->uTypeData)->apwWaypoints[i]);
	}
#line 3738
	D2FREEPOOL(dwPool, ptUnit->uTypeData);
	ptUnit->uTypeData = 0;
}

void __stdcall UnitSetPlayerName(PTUNIT ptUnit, const char *pszName)
{
#line 3771
	D2ASSERT(ptUnit);
#line 3773
	D2ASSERTMSG(ptUnit->uTypeData, "ptUnit->uTypeData.ptPlrInfo");
#line 3774
	D2ASSERT(ptUnit->eType == UNIT_PLAYER);
	StormSStrCopy(((PTPLRINFO)ptUnit->uTypeData)->szName, pszName, 2147483647);
}

char * __stdcall UnitGetPlayerName(PTUNIT ptUnit)
{
#line 3785
	D2ASSERT(ptUnit);
#line 3787
	D2ASSERTMSG(ptUnit->uTypeData, "ptUnit->uTypeData.ptPlrInfo");
#line 3788
	D2ASSERT(ptUnit->eType == UNIT_PLAYER);
	return ((PTPLRINFO)ptUnit->uTypeData)->szName;
}

PTPLRINFO __stdcall UnitGetPlrInfo(PTUNIT ptUnit)
{
#line 3800
	D2ASSERT(ptUnit);
#line 3802
	D2ASSERT(ptUnit->eType == UNIT_PLAYER);
	return (PTPLRINFO)ptUnit->uTypeData;
}

void __stdcall UnitSetPlayerLevelBits(PTUNIT ptUnit, DWORD dwValue)
{
#line 3811
	D2ASSERT(ptUnit);
#line 3812
	D2ASSERT(ptUnit->eType == UNIT_PLAYER);
	((PTPLRINFO)ptUnit->uTypeData)->dwPortalLevelBits = dwValue;
}

DWORD __stdcall UnitGetPlayerLevelBits(PTUNIT ptUnit)
{
#line 3821
	D2ASSERT(ptUnit);
#line 3822
	D2ASSERT(ptUnit->eType == UNIT_PLAYER);
	return ((PTPLRINFO)ptUnit->uTypeData)->dwPortalLevelBits;
}

DWORD __stdcall UnitGetObjectNameOffset(PTUNIT ptUnit)
{
#line 3831
	D2ASSERT(ptUnit);
	if (!(ptUnit->eType != UNIT_OBJECT)) {
		return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->ptClassInfo->NameOffset; }
	return 0;
}

BYTE __stdcall UnitGetObjectFlags(PTUNIT ptUnit)
{
#line 3848
	D2ASSERT(ptUnit);
#line 3849
	D2ASSERT(ptUnit->eType == UNIT_OBJECT);
	return ((PTOBJECTUNITDATA)ptUnit->uTypeData)->bFlags;
}

void __stdcall UnitSetObjectFlags(PTUNIT ptUnit, BYTE bValue)
{
#line 3859
	D2ASSERT(ptUnit);
#line 3860
	D2ASSERT(ptUnit->eType == UNIT_OBJECT);
	((PTOBJECTUNITDATA)ptUnit->uTypeData)->bFlags = bValue;
}

BOOL __stdcall UnitTestObjectFlags(PTUNIT ptUnit, BYTE bMask)
{
#line 3870
	D2ASSERT(ptUnit);
#line 3871
	D2ASSERT(ptUnit->eType == UNIT_OBJECT);
	return (((PTOBJECTUNITDATA)ptUnit->uTypeData)->bFlags & bMask) != 0;
}

DWORD __stdcall UnitGetOverlayHeightClass(HUNIT hUnit)
{
	PTMONSTERTBLREC ptRec;

	switch (hUnit->eType)
	{
	case UNIT_PLAYER:
		return 1;
	case UNIT_MONSTER:
		ptRec = (PTMONSTERTBLREC)D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		return (DWORD)ptRec->OverlayHeight - 1;
	}
	return 0;
}

PTSKILL __fastcall SkillsFindByKey(HUNIT hUnit, DWORD dwKey);

int __stdcall UnitCalcArmorClassWithBonus(HUNIT hUnit)
{
	int nValue;
	int nPercent;
	DWORD dwInventory;
	PTSKILL ptSkill;
	HUNIT hShield;

	nValue = D2CommonStatsUnitGetValue(hUnit, 31);
	nValue += D2CommonStatsUnitGetValue(hUnit, 2) / 4;
	if (D2CommonStatsGetState(hUnit, 101))
	{
		dwInventory = hUnit->ptInventory;
		hShield = NULL;
		ptSkill = SkillsFindByKey(hUnit, 117);
		if (ptSkill != NULL && D2CommonINVENTORYFindShieldInHands(dwInventory, &hShield))
		{
#line 3912
			D2ASSERT(hShield);
			nValue += D2CommonSkillsCalcParamThreeByLevel(SKILL_HOLY_SHIELD,
				D2CommonSkillsCalcNodeLevel(hUnit, ptSkill,
				1)) * D2CommonStatsUnitGetValue(hShield, 31) / 100;
		}
	}
	nPercent = (int)D2CommonStatsGetTotalValue(hUnit, 171);
	if (nValue > 0)
		nValue += nPercent * nValue / 100;
	else
		nValue += nPercent * nValue / -100;
	return nValue;
}

DWORD __stdcall UnitGetAttackerIndexedValue(HUNIT hAttacker)
{
	DWORD dwBase;
	DWORD dwIndex;

#line 3933
	D2ASSERT(UnitGetType ( hAttacker ) == UNIT_PLAYER);
	dwBase = D2CommonStatsUnitGetValue(hAttacker, 19);
	dwIndex = D2CommonStatsUnitGetValue(hAttacker, 2);
	return dwBase + dwIndex * 4 - 28;
}

HUNIT __stdcall UnitGetActiveWeapon(HUNIT hUnit)
{
	PTINVENTORY ptInventory;
	DWORD dwClass;
	HUNIT hItem;
	DWORD dwOut3;
	DWORD dwOut4;
	PTSKILL ptSkill;

	if (hUnit->eType == UNIT_PLAYER)
	{
		ptInventory = (PTINVENTORY)hUnit->ptInventory;
		dwClass = hUnit->dwClassId;
		if (D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &dwOut3, &dwOut4))
		{
			if (dwClass == 4)
			{
				ptSkill = SkillsHeadGetThirdCached(hUnit->field_c8);
				if (!ptSkill)
					return hItem;
				switch (SkillsGetNodeType(ptSkill))
				{
				case 4:
				case 5:
					hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 6);
					break;
				case 133:
				case 147:
					if ((int)(hUnit->nSeqPos & 0xFFFFFF00) > 512)
						hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 6);
					else
						hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 5);
					break;
				case 140:
					if ((int)(hUnit->nSeqPos & 0xFFFFFF00) > 1536)
						hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 6);
					else
						hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 5);
					break;
				default:
					hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 5);
					if (!hItem || !D2CommonITEMSIsWeapon(hItem))
						hItem = (HUNIT)D2CommonINVENTORYGetBodyLocItem(ptInventory, 6);
					break;
				}
				if (!hItem || !D2CommonITEMSIsWeapon(hItem) || !D2CommonITEMSTestEffectsActive(hItem))
					return NULL;
			}
			return hItem;
		}
	}
	return NULL;
}

static const DWORD sgadwWClassModeTbl[60] =
{
	1, 1, 0, 0, 0,
	0, 0, 0, 0, 0,
	2, 2, 0, 0, 0,
	2, 2, 0, 0, 0,
	2, 2, 0, 0, 0,
	2, 2, 0, 0, 0,
	2, 2, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
};

DWORD __stdcall UnitGetWClassModeValue(PTUNIT ptUnit)
{
	DWORD eType;
	DWORD nClass;
	DWORD nMode;
	HUNIT hWeapon;
	WORD wIndex;

	eType = ptUnit->eType;
	nClass = ptUnit->dwClassId;
	nMode = ptUnit->dwMode;
	D2CommonSkillsClearFlagIfMatched(ptUnit, &eType, &nClass, &nMode);
	if (!(eType != UNIT_PLAYER)) {
		if ((int)nMode < 7)
			return 0;
		if ((int)nMode > 8)
		{
			if (nMode != 15 || nClass != 4)
				return 0;
			hWeapon = UnitGetActiveWeapon(ptUnit);
			wIndex = 0;
			if (hWeapon != NULL) wIndex = D2CommonITEMSGetWClassIndexRaw((DWORD)hWeapon);
			return sgadwWClassModeTbl[nClass + wIndex * 5];
		}
		hWeapon = UnitGetActiveWeapon(ptUnit);
		wIndex = 0;
		if (hWeapon != NULL) wIndex = D2CommonITEMSGetWClassIndexRaw((DWORD)hWeapon);
		return sgadwWClassModeTbl[nClass + wIndex * 5]; }
	return 0;
}

DWORD __stdcall UnitGetWeaponNumber(HUNIT hUnit)
{
	HUNIT hItem;
	DWORD dwOut3;
	DWORD dwOut4;
	PTMONSTATS ptStats;
	DWORD dwValue;
	DWORD dwResult;

#line 4075
	D2ASSERT(hUnit);
	switch (hUnit->eType)
	{
	case UNIT_PLAYER:
		dwResult = D2CommonINVENTORYGetWeapon(hUnit->ptInventory, &hItem, &dwOut3, &dwOut4);
		if (!dwResult)
			return dwResult;
		return D2CommonITEMSGetTypeRangeAdderRaw((DWORD)hItem) & 255;
	case UNIT_MONSTER:
		ptStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(hUnit->dwClassId);
#line 4095
		D2ASSERT(ptStats);
		dwValue = ptStats->bMeleeRng;
		if (dwValue != 255)
			return dwValue;
		COMPOSITGetUnitWeaponClass(hUnit, hUnit->ptInventory, (DWORD)&hItem, (DWORD)-1, 1);
		return (DWORD)hItem == 6 ? 2 : 0;
	}
	return 0;
}

static BOOL __fastcall UnitTestLineCollision(DWORD dwX1, DWORD dwY1, int nSize1, DWORD dwX2, DWORD dwY2, int nSize2,
	GAMEROOM *hRoom, DWORD dwMask);

BOOL __stdcall UnitTestLineBlockedToPoint(HUNIT hUnit, DWORD dwX, DWORD dwY, DWORD dwMask)
{
	DWORD dwRoom;
	DWORD dwUnitX;
	DWORD dwUnitY;
	DWORD dwSize;

	if (!hUnit)
		return FALSE;
	dwRoom = UnitGetPathRoom(hUnit);
	if (!dwRoom)
		return FALSE;
	dwUnitX = UnitGetX(hUnit);
	dwUnitY = UnitGetY(hUnit);
	dwSize = UnitGetSizeX(hUnit);
	return UnitTestLineCollision(dwUnitX, dwUnitY, (int)dwSize, dwX, dwY, 2, (GAMEROOM *)dwRoom, dwMask);
}

static BOOL __fastcall UnitTestLineCollision(DWORD dwX1, DWORD dwY1, int nSize1, DWORD dwX2, DWORD dwY2, int nSize2,
	GAMEROOM *hRoom, DWORD dwMask)
{
	DWORD dwMinX1;
	DWORD dwMaxX1;
	DWORD dwMinY1;
	DWORD dwMaxY1;
	DWORD dwMinX2;
	DWORD dwMaxX2;
	DWORD dwMinY2;
	DWORD dwMaxY2;
	int nHalf;
	int nDX;
	int nDY;
	int nAX;
	int nAY;
	int nSX;
	int nSY;
	int nErr;
	DWORD dwX;
	DWORD dwY;

	dwX = dwX1;
	dwY = dwY1;
	nHalf = nSize1 >> 1;
	dwMinX1 = dwX1 - nHalf;
	dwMaxX1 = dwX1 + nHalf;
	dwMinY1 = dwY1 - nHalf;
	dwMaxY1 = dwY1 + nHalf;
	nHalf = nSize2 >> 1;
	dwMinX2 = dwX2 - nHalf;
	dwMaxX2 = dwX2 + nHalf;
	dwMinY2 = dwY2 - nHalf;
	dwMaxY2 = dwY2 + nHalf;
	hRoom = DungeonFindRoomGame(hRoom, (int)dwX1, (int)dwY1);
	nDX = (int)(dwX2 - dwX1);
	nDY = (int)(dwY2 - dwY1);
	nAX = (nDX < 0 ? -nDX : nDX) * 2;
	nAY = (nDY < 0 ? -nDY : nDY) * 2;
	nSX = (nDX < 0) ? -1 : 1;
	nSY = (nDY < 0) ? -1 : 1;
	if (nAX > nAY)
	{
		nErr = nAY - nAX / 2;
		for (;;)
		{
			if (dwX < dwMinX1 || dwX > dwMaxX1 || dwY < dwMinY1 || dwY > dwMaxY1)
			{
				if (D2CommonCollisnTestPoint(hRoom, dwX, dwY, dwMask))
					return TRUE;
			}
			if (dwX >= dwMinX2 && dwX <= dwMaxX2 && dwY >= dwMinY2 && dwY <= dwMaxY2)
				return FALSE;
			if (nErr >= 0)
			{
				dwY += nSY;
				nErr -= nAX;
			}
			dwX += nSX;
			nErr += nAY;
		}
	}
	nErr = nAX - nAY / 2;
	for (;;)
	{
		if (dwX < dwMinX1 || dwX > dwMaxX1 || dwY < dwMinY1 || dwY > dwMaxY1)
		{
			if (D2CommonCollisnTestPoint(hRoom, dwX, dwY, dwMask))
				return TRUE;
		}
		if (dwX >= dwMinX2 && dwX <= dwMaxX2 && dwY >= dwMinY2 && dwY <= dwMaxY2)
			return FALSE;
		if (nErr >= 0)
		{
			dwX += nSX;
			nErr -= nAY;
		}
		dwY += nSY;
		nErr += nAX;
	}
}

BOOL __stdcall UnitTestLineBlocked(HUNIT hUnit1, HUNIT hUnit2, DWORD dwMask)
{
	DWORD dwRoom;
	DWORD dwX1;
	DWORD dwY1;
	DWORD dwSize1;
	DWORD dwX2;
	DWORD dwY2;
	DWORD dwSize2;

#line 4222
	D2ASSERT(hUnit1);
#line 4223
	D2ASSERT(hUnit2);
	dwRoom = UnitGetPathRoom(hUnit1);
	if (!dwRoom)
		return FALSE;
	dwX1 = UnitGetX(hUnit1);
	dwY1 = UnitGetY(hUnit1);
	dwSize1 = UnitGetSizeX(hUnit1);
	dwX2 = UnitGetX(hUnit2);
	dwY2 = UnitGetY(hUnit2);
	dwSize2 = UnitGetSizeX(hUnit2);
	return UnitTestLineCollision(dwX1, dwY1, (int)dwSize1, dwX2, dwY2, (int)dwSize2, (GAMEROOM *)dwRoom,
		dwMask);
}

BOOL __stdcall UnitTestLineBlockedIgnoring(HUNIT hUnit1, HUNIT hUnit2, DWORD dwMask)
{
	DWORD dwRoom;
	DWORD dwX1;
	DWORD dwY1;
	DWORD dwX2;
	DWORD dwY2;
	BOOL fCleared1;
	BOOL fCleared2;
	BOOL fBlocked;

	if (!hUnit1 || !hUnit2)
		return FALSE;
	dwRoom = UnitGetPathRoom(hUnit1);
	if (dwRoom) {
		dwX1 = UnitGetX(hUnit1);
		dwY1 = UnitGetY(hUnit1);
		dwX2 = UnitGetX(hUnit2);
		dwY2 = UnitGetY(hUnit2);
		fCleared1 = FALSE;
		fCleared2 = FALSE;
		if (dwMask & UnitGetTypeFlags(hUnit1))
			fCleared1 = D2CommonPathClearCollisionByType(hUnit1, 0);
		if (dwMask & UnitGetTypeFlags(hUnit2))
			fCleared2 = D2CommonPathClearCollisionByType(hUnit2, 0);
		fBlocked = UnitTestLineCollision(dwX1, dwY1, 0, dwX2, dwY2, 2, (GAMEROOM *)dwRoom, dwMask);
		if (fCleared1)
			D2CommonPathSetCollisionByType(hUnit1);
		if (fCleared2)
			D2CommonPathSetCollisionByType(hUnit2);
		return fBlocked; }
	return FALSE;
}

DWORD __stdcall UnitCalcEdgeDistance(HUNIT hUnitA, HUNIT hUnitB);

BOOL __stdcall UnitTestPositionClear(HUNIT hUnitA, HUNIT hUnitB, DWORD dwDistanceAdjust)
{
	DWORD dwSize;
	int nDist;

	if (!hUnitB || !hUnitA)
		return 0;
	if (hUnitB->eType == UNIT_MONSTER)
	{
		DWORD dwFlags = D2CommonMonsterUnitGetTypeLocalIndex(hUnitB);

		if (dwFlags == 261 || dwFlags == 258)
		{
			nDist = (int)UnitCalcEdgeDistance(hUnitA, hUnitB);
			dwSize = D2CommonUnitGetWeaponNumber(hUnitA);
			if ((int)(dwSize + 8) > nDist)
				return 1;
		}
	}
	dwSize = D2CommonUnitGetWeaponNumber(hUnitA) + 1;
	nDist = (int)UnitCalcEdgeDistance(hUnitA, hUnitB);
	if (nDist > 0)
	{
		nDist += (int)dwDistanceAdjust;
		if ((int)dwSize < nDist)
			return 0;
	}
	return D2CommonUnitTestLineBlocked(hUnitA, hUnitB, 2052) == 0;
}

BOOL __stdcall UnitTestModeGroup(HUNIT hUnit)
{
	DWORD dwMode;

	if (!hUnit)
		return FALSE;
	switch (hUnit->eType)
	{
	case UNIT_MISSILE:
#line 4358
		D2ASSERT(UnitGetType (hUnit) != UNIT_MISSILE);
	case UNIT_MONSTER:
		switch (hUnit->dwMode)
		{
		case 2:
		case 15:
			return TRUE;
		case 8:
		case 9:
			return D2CommonMonsterUnitGetTypeLocalIndex(hUnit) == 110;
		}
		return FALSE;
	case UNIT_PLAYER:
		dwMode = hUnit->hAnimSequence ? hUnit->dwSeqMode : hUnit->dwMode;
		switch (dwMode)
		{
		case 2:
		case 3:
		case 6:
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

DWORD __stdcall UnitGetThreeIfInModeGroup(HUNIT hUnit)
{
	return D2CommonUnitTestModeGroup(hUnit) ? 3 : 0;
}

DWORD __fastcall UnitGetStateByte(HUNIT hUnit)
{
	PTMONSTERTBLREC ptRec;
	DWORD dwHandle;
	DWORD dwClassId;

	if (hUnit->eType == UNIT_PLAYER)
	{
		dwHandle = D2CommonUnitGetActiveWeapon(hUnit);
		if (dwHandle != 0)
			return D2CommonITEMSGetTypeHitClassRaw(dwHandle) & 255;
		return 1;
	}
	if (hUnit->eType == UNIT_MONSTER)
	{
		dwClassId = hUnit->dwClassId;
		return ((PTMONSTERTBLREC)D2CommonMonsterTblsGetStats(dwClassId))->HitClass;
	}
	return 0;

}

DWORD __fastcall UnitGetItemFlagsWord(HUNIT hUnit)
{
	DWORD dwHandle;

	dwHandle = D2CommonUnitGetActiveWeapon(hUnit);
	if (dwHandle)
		return D2CommonITEMSGetWClassIndexRaw(dwHandle) & 65535;
	return 0;
}

DWORD __stdcall UnitGetCombinedStatDelta(DWORD dwArg)
{
	DWORD dwA;
	DWORD dwB;
	DWORD dwC;
	DWORD dwD;
	DWORD dwDeltaA;
	DWORD dwDeltaB;

	dwA = D2CommonStatsUnitCalcMaxHp(dwArg);
	dwB = D2CommonStatsUnitCalcMaxMana(dwArg);
	dwC = D2CommonStatsUnitGetValue(dwArg, 6);
	dwD = D2CommonStatsUnitGetValue(dwArg, 8);

	dwDeltaA = 0;
	dwDeltaB = 0;
	if (dwA > dwC)
		dwDeltaA = (dwA - dwC) >> 8;
	if (dwB > dwD)
		dwDeltaB = (dwB - dwD) >> 8;
	if (dwDeltaA == 0 && dwDeltaB == 0)
		return 0;
	return ((DWORD)D2CommonStatsUnitGetValue(dwArg, 12) * (dwDeltaA + dwDeltaB)) >> 2;
}

DWORD __stdcall UnitCalcGoldLimit(DWORD dwArg)
{
	return (DWORD)D2CommonStatsUnitGetValue(dwArg, 12) * 10000;
}

void __stdcall UnitApplyPrimaryWeaponSwap(HUNIT hUnit)
{
	DWORD hOtherWeapon;
	DWORD hPrimaryWeapon;

	if (hUnit->eType != UNIT_PLAYER)
		return;
	if (hUnit->dwClassId != 4)
		return;
	hOtherWeapon = D2CommonUnitGetActiveWeapon(hUnit);
	hPrimaryWeapon = D2CommonINVENTORYFindEquippedMatch(hUnit->ptInventory);
	if (hOtherWeapon == 0)
		return;
	if (hOtherWeapon == hPrimaryWeapon)
		return;
#line 4468
	D2ASSERT(hPrimaryWeapon);
	if (D2CommonStatsTestModifying(hOtherWeapon))
	{
		D2CommonStatsUnitRemoveDye(hUnit, hOtherWeapon);
		D2CommonStatsUnitApplyDye(hUnit, hOtherWeapon, 1);
	}
	if (D2CommonStatsTestModifying(hPrimaryWeapon))
	{
		D2CommonStatsUnitRemoveDye(hUnit, hPrimaryWeapon);
		D2CommonStatsUnitApplyDye(hUnit, hPrimaryWeapon, 0);
	}
}

void __stdcall UnitApplyEquipStatSwap(HUNIT hUnit)
{
	DWORD dwNewItem;
	DWORD dwOldItem;

	if (hUnit->eType != UNIT_PLAYER || hUnit->dwClassId != 4)
		return;
	dwOldItem = D2CommonUnitGetActiveWeapon(hUnit);
	dwNewItem = D2CommonINVENTORYFindEquippedMatch(hUnit->ptInventory);
	if (dwNewItem != 0 && D2CommonStatsTestModifying(dwNewItem))
	{
		D2CommonStatsUnitRemoveDye(hUnit, dwNewItem);
		D2CommonStatsUnitApplyDye(hUnit, dwNewItem, 1);
	}
	if (dwOldItem != 0 && dwOldItem != dwNewItem && D2CommonStatsTestModifying(dwOldItem))
	{
		D2CommonStatsUnitRemoveDye(hUnit, dwOldItem);
		D2CommonStatsUnitApplyDye(hUnit, dwOldItem, 0);
	}
}

BYTE __stdcall UnitTestEquipSlotConflict(HUNIT hUnit, DWORD dwItemArg)
{
	DWORD dwKind;
	DWORD dwContainer;
	DWORD dwSlotA;
	DWORD dwSlotB;

	dwKind = D2CommonITEMSGetCategoryRaw(dwItemArg) & 255;
	if (dwKind == 5 || dwKind == 4)
	{
		dwContainer = hUnit->ptInventory;
		dwSlotA = D2CommonINVENTORYGetLocSlot(dwContainer, 4);
		if (!D2CommonITEMSTestEffectsActive(dwSlotA))
			dwSlotA = 0;
		dwSlotB = D2CommonINVENTORYGetLocSlot(dwContainer, 5);
		if (!D2CommonITEMSTestEffectsActive(dwSlotB))
			dwSlotB = 0;
		if (dwSlotA != 0 && dwSlotB != 0 &&
			!D2CommonITEMSGetTypeTwoHanded(dwSlotA) && !D2CommonITEMSGetTypeTwoHanded(dwSlotB) &&
			hUnit->eType == UNIT_PLAYER && hUnit->dwClassId == 4 &&
			dwItemArg != D2CommonINVENTORYFindEquippedMatch(dwContainer))
		{
			return 6;
		}
	}
	return D2CommonITEMSGetTypeComponent(dwItemArg);
}

void __stdcall UnitSyncPlayerRightSkill(PTUNIT ptUnit)
{
	PTPLRINFO ptInfo;

	if (ptUnit->eType != 0)
		return;
#line 3800
	D2ASSERT(ptUnit);
	ptInfo = (PTPLRINFO)ptUnit->uTypeData;
#line 4559
	D2ASSERT(ptInfo);
	ptInfo->dwRightSkillId = SkillsGetNodeType(SkillsHeadGetRightSkill(ptUnit->field_c8));
}

void __stdcall UnitSyncPlayerLeftSkill(PTUNIT ptUnit)
{
	PTPLRINFO ptInfo;

	if (ptUnit->eType != 0)
		return;
#line 3800
	D2ASSERT(ptUnit);
	ptInfo = (PTPLRINFO)ptUnit->uTypeData;
#line 4572
	D2ASSERT(ptInfo);
	ptInfo->dwLeftSkillId = SkillsGetNodeType(SkillsHeadGetLeftSkill(ptUnit->field_c8));
}

DWORD __stdcall UnitGetPlayerRightSkill(PTUNIT ptUnit)
{
	PTPLRINFO ptInfo;

	if (ptUnit->eType != 0)
		return 0;
#line 3800
	D2ASSERT(ptUnit);
	ptInfo = (PTPLRINFO)ptUnit->uTypeData;
#line 4585
	D2ASSERT(ptInfo);
	return ptInfo->dwRightSkillId;
}

DWORD __stdcall UnitGetPlayerLeftSkill(PTUNIT ptUnit)
{
	PTPLRINFO ptInfo;

	if (ptUnit->eType != 0)
		return 0;
#line 3800
	D2ASSERT(ptUnit);
	ptInfo = (PTPLRINFO)ptUnit->uTypeData;
#line 4596
	D2ASSERT(ptInfo);
	return ptInfo->dwLeftSkillId;
}

DWORD __stdcall UnitSetPlayerWeaponId(PTUNIT ptUnit, PTUNIT hWeapon)
{
	PTPLRINFO ptInfo;

	if (ptUnit->eType != 0)
		return (DWORD)ptUnit;
#line 3800
	D2ASSERT(ptUnit);
	ptInfo = (PTPLRINFO)ptUnit->uTypeData;
#line 4607
	D2ASSERT(ptInfo);
	if (hWeapon != NULL)
	{
#line 4611
		D2ASSERT(UnitGetType (hWeapon) == UNIT_ITEM);
		ptInfo->dwWeaponId = hWeapon->dwId;
	}
	else
	{
		ptInfo->dwWeaponId = 0;
	}
	return (DWORD)ptInfo;
}

DWORD __stdcall UnitGetPlayerWeaponId(PTUNIT ptUnit)
{
	PTPLRINFO ptInfo;

#line 3800
	D2ASSERT(ptUnit);
#line 3802
	D2ASSERT(ptUnit->eType == UNIT_PLAYER);
	ptInfo = (PTPLRINFO)ptUnit->uTypeData;
#line 4625
	D2ASSERT(ptInfo);
	return ptInfo->dwWeaponId;
}

DWORD __stdcall UnitCalcGoldBankLimit(DWORD dwArg)
{
	int nStat;

	nStat = D2CommonStatsUnitGetValue(dwArg, 12);
	return (DWORD)(nStat / 10 + 1) * 50000;
}

DWORD __fastcall UnitTestMonsterCombinedFlag(HUNIT hUnit)
{
	PTMONSTERTBLREC ptRec;

	ptRec = (PTMONSTERTBLREC)D2CommonMonsterTblsGetStats(hUnit);
	if (ptRec == NULL)
		return 0;
	if (ptRec->bModeW == 0)
		return 0;
	if (ptRec->eBoss != 0)
		return 0;
	return ptRec->bSwitch != 0;
}

void __fastcall UnitSetObjectLinks(HUNIT hUnit, DWORD dwValue)
{
#line 4662
	D2ASSERT(hUnit);
	hUnit->field_9c = dwValue;
}

DWORD __fastcall UnitGetObjectLinks(HUNIT hUnit)
{
#line 4670
	D2ASSERT(hUnit);
	return hUnit->field_9c;
}

void __stdcall UnitAllocAuxBlock(HUNIT hUnit)
{
	PTUNITAUXBLOCK ptAux;
	DWORD i;

	if (hUnit->ptPath != NULL)
		return;
#line 4681
	ptAux = (PTUNITAUXBLOCK)D2ALLOCPOOL((void *)hUnit->dwMemPool, sizeof(UNITAUXBLOCK));
	hUnit->ptPath = (struct PATH *)ptAux;
	for (i = 0; i < sizeof(UNITAUXBLOCK) / sizeof(DWORD); i++)
		((DWORD *)ptAux)[i] = 0;
}

void __stdcall UnitFreeAuxBlock(HUNIT hUnit)
{
	if (hUnit->ptPath == NULL)
		return;
#line 4694
	D2FREEPOOL((void *)hUnit->dwMemPool, (void *)hUnit->ptPath);
}
