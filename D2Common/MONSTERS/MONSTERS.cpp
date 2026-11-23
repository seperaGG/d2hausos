#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Monsters\\MONSTERS.CPP"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

DWORD __stdcall MonsterUnitGetTypeLocalIndex(HUNIT ptUnit);
int __stdcall MonsterResolvePopulationIndex(DWORD dwAreaRef, int nClass);

#include "MonsterRegionLocal.h"

DWORD __stdcall MonsterRegionGetMissileField(HUNIT ptUnit, DWORD dwPosition)
{
	DWORD dwArgCopy;
	DWORD dwOffset;
	int nIndex, nIndex0;
	PTMONSTATS ptMonStats;

	if (dwPosition != 0)
	{
		if (ptUnit->bAnimEvent == 0)
			return 65535;

		D2CommonUnitGetAnimPosAndLength(ptUnit, &dwPosition, &dwArgCopy);
		nIndex0 = ((int)dwPosition - (int)ptUnit->nAnimRate) >> 8;
		dwPosition = (DWORD)((int)dwPosition >> 8);
		if (nIndex0 >= (int)dwPosition)
			return 65535;

		nIndex = nIndex0;
		while (1)
		{
			if (D2CommonUnitGetAnimFrameByte(ptUnit, nIndex) == 2)
				break;
			nIndex++;
			if (nIndex > (int)dwPosition)
				return 65535;
		}
		if (nIndex > (int)dwPosition)
			return 65535;
		if (nIndex <= nIndex0)
			return 65535;
	}

	DWORD scadwFieldOffset[16] =
	{
		0, 0, 0, 0, 504, 508, 0, 528,
		512, 516, 520, 524, 0, 0, 532, 0,
	};

	dwOffset = scadwFieldOffset[ptUnit->dwMode];
	if (dwOffset == 0)
		return 65535;
	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
	ptMonStats = (PTMONSTATS)((BYTE *)ptMonStats + dwOffset);
	return *(DWORD *)ptMonStats;
}

BYTE __stdcall MonsterRegionLookupTableA(BYTE bIndex)
{
	static BYTE scabTableA[64] =
	{
		0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4,
		4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8,
		8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12,
		12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 0, 0,
	};

	return scabTableA[bIndex];
}

BYTE __stdcall MonsterRegionLookupTableB(BYTE bIndex)
{
	static BYTE scabTableB[64] =
	{
		0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
		2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
		4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6,
		6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0,
	};

	return scabTableB[bIndex];
}

BYTE __stdcall MonsterRegionLookupTableC(BYTE bIndex)
{
	static BYTE scabTableC[8] =
	{
		4, 12, 20, 28, 36, 44, 52, 60,
	};

	return scabTableC[bIndex];
}

void __stdcall MonsterRegionGetOffsetPair(BYTE bIndex, int *pnOffsetA, int *pnOffsetB)
{
	signed char acOffsetA[32] =
	{
		0, -1, -1, -1, 0, 1, 1, 1, 0, -1, -2, -2, -2, -2, -2, -1,
		0, 1, 2, 2, 2, 2, 2, 1, 0, -3, -3, -3, 0, 3, 3, 3,
	};
	signed char acOffsetB[32] =
	{
		-1, -1, 0, 1, 1, 1, 0, -1, -2, -2, -2, -1, 0, 1, 2, 2,
		2, 2, 2, 1, 0, -1, -2, -2, -3, -3, 0, 3, 3, 3, 0, -3,
	};
	*pnOffsetA = acOffsetA[bIndex];
	*pnOffsetB = acOffsetB[bIndex];
}

BOOL __fastcall MonsterRegionMayEnterTown(HUNIT hUnit)
{
	PTMONSTATS ptMonStats;

	if (hUnit->eType != UNIT_MONSTER)
		return FALSE;
	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(hUnit->dwClassId);
	if (ptMonStats->bIsNPC)
		return TRUE;
	if (D2CommonStatsUnitGetAlignment(hUnit) == 0)
		return TRUE;
	return ptMonStats->bInTown;
}

BOOL MONSTERREGIONTHIS::MonsterUtilTestModeMove(DWORD dwParam1, DWORD dwParam2)
{
	DWORD dwParam0 = (DWORD)this;
	DWORD dwParam0Copy;
	DWORD dwDelta;

	dwDelta = *(DWORD *)(dwParam1 + 4);
	D2CommonMonsterTblsGetRun(dwDelta, &dwParam0Copy, &dwParam1);
	if (!(dwDelta - dwParam1 != 78)) {
		if (dwParam2 == 0)
			return TRUE; }
	return FALSE;
}

DWORD MONSTERREGIONTHIS::MonsterRegionUnitIsDemon(HUNIT ptUnit)
{
	PTMONSTATS ptMonStats;

	if (ptUnit->eType != UNIT_MONSTER)
		return 0;
	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
	return ptMonStats->eDemon;
}

BOOL __stdcall MonsterRegionUnitIsUndead(HUNIT ptUnit)
{
	PTMONSTATS ptMonStats;

	if (ptUnit->eType != UNIT_MONSTER)
		return FALSE;
	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
	if (ptMonStats->bLUndead != 0 || ptMonStats->bHUndead != 0)
		return TRUE;
	return FALSE;
}

DWORD __stdcall MonsterRegionUnitIsBoss(DWORD dwOverride, HUNIT ptUnit)
{
	PTMONSTATS ptMonStats;

	if (ptUnit == NULL)
		return 0;
	if (ptUnit->eType != UNIT_MONSTER)
		return 0;
	ptMonStats = (PTMONSTATS)dwOverride;
	if (ptMonStats == NULL) {
		ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
		if (ptMonStats == NULL)
			return 0; }
	return ptMonStats->eBoss;
}

BOOL __stdcall MonsterUnitTestFieldC(HUNIT ptUnit)
{
	if (ptUnit->eType != UNIT_MONSTER)
		return FALSE;
	if (ptUnit->dwMode == 0 || ptUnit->dwMode == 12)
		return TRUE;
	return FALSE;
}

void __stdcall MonsterGetSpecialPosition(HUNIT hUnit, int *pnClass, int *pnX, int *pnY, int *pnMode,
	DWORD dwUnused)
{
	DWORD dwFirst;
	DWORD dwBase;
	DWORD dwOffset;
	PTPATH ptPath;

	dwOffset = MonsterUnitGetTypeLocalIndex(hUnit);
	if (dwOffset == 206)
	{
		D2CommonMonsterTblsGetRun(hUnit->dwClassId, &dwFirst, &dwBase);
		*pnClass = dwBase + 15;
		*pnX = D2CommonUnitGetX(hUnit);
		*pnY = D2CommonUnitGetY(hUnit) + 3;
		*pnMode = 1;
		return;
	}
	if (dwOffset == 228)
	{
		*pnClass = MonsterResolvePopulationIndex(D2CommonUnitGetPathRoom(hUnit), 96);
		*pnX = D2CommonUnitGetX(hUnit);
		*pnY = D2CommonUnitGetY(hUnit) + 2;
		*pnMode = 1;
		return;
	}
	if (dwOffset == 267)
	{
		*pnClass = 6;
		ptPath = hUnit->ptPath;
		*pnX = D2CommonPathGetTargetXRaw((DWORD)ptPath);
		*pnY = D2CommonPathGetTargetYRaw((DWORD)ptPath);
		*pnMode = 8;
		return;
	}
	if (dwOffset == 284)
	{
		D2CommonMonsterTblsGetRun(hUnit->dwClassId, &dwFirst, &dwBase);
		*pnClass = dwBase + 68;
		*pnX = D2CommonUnitGetX(hUnit) + 8;
		*pnY = D2CommonUnitGetY(hUnit);
		*pnMode = 8;
		return;
	}
	if (dwOffset == 321)
	{
		*pnClass = 19;
		*pnX = D2CommonUnitGetX(hUnit);
		*pnY = D2CommonUnitGetY(hUnit);
		*pnMode = 1;
		return;
	}
	if (dwOffset == 334)
	{
		D2CommonMonsterTblsGetRun(hUnit->dwClassId, &dwFirst, &dwBase);
		*pnClass = dwBase + 114;
		*pnX = D2CommonUnitGetX(hUnit) - 2;
		*pnY = D2CommonUnitGetY(hUnit) - 2;
		*pnMode = 1;
		return;
	}
	if (dwOffset == 298)
	{
		D2CommonMonsterTblsGetRun(hUnit->dwClassId, &dwFirst, &dwBase);
		*pnClass = dwBase + 301;
		ptPath = hUnit->ptPath;
		*pnX = D2CommonPathGetTargetXRaw((DWORD)ptPath);
		*pnY = D2CommonPathGetTargetYRaw((DWORD)ptPath);
		*pnMode = 1;
		return;
	}
	*pnClass = 0;
	*pnX = 0;
	*pnY = 0;
	*pnMode = 1;
}

void __stdcall MonsterUnitApplyChartEffects(HUNIT ptUnit)
{
	DWORD dwRunFirst;
	DWORD dwRaw;

	dwRunFirst = MonsterUnitGetTypeLocalIndex(ptUnit);
	dwRaw = ptUnit->dwClassId;

	if (dwRunFirst == 242)
	{
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 186), 5);
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 183), 5);
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 192), 5);
	}
	else if (dwRunFirst == 243)
	{
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 188), 10);
	}
	else if (dwRunFirst == 131)
	{
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 183), 7);
	}

	if (dwRaw == 45)
	{
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 170), 4);
	}
	else if (dwRaw == 312)
	{
		D2CommonSkillsSetNodeMode(D2CommonSkillsFindByKey(ptUnit, 183), 9);
	}
}

BYTE __stdcall MonsterUnitGetBestSlotRank(HUNIT ptUnit)
{
	int i;
	BYTE bBest;
	BYTE bSlot;
	DWORD dwCode;
	DWORD dwOut2;
	BYTE *pRecord;

	if (ptUnit->eType != UNIT_MONSTER)
		return 0;

	bBest = 0;
	for (i = 0; i < 16; i++)
	{
		bSlot = ptUnit->abComponentVariations[(BYTE)i];
		dwCode = D2CommonMonsterRandGetComponentCode(ptUnit, i, bSlot);
		if (dwCode == 0)
			continue;
		if (dwCode == 544500076 || dwCode == 543450477 || dwCode == 544831080)
			continue;
		pRecord = (BYTE *)D2CommonItemTblsFindItemByCode(dwCode, &dwOut2);
		if (pRecord == NULL)
			continue;
		if (pRecord[51] > bBest)
			bBest = pRecord[51];
	}
	return bBest;
}

DWORD __stdcall MonsterUnitGetTypeLocalIndex(HUNIT ptUnit)
{
	DWORD dwValue;
	DWORD dwOut1;
	DWORD dwBase;

	dwValue = ptUnit->dwClassId;
	D2CommonMonsterTblsGetRun(dwValue, &dwOut1, &dwBase);
	return dwValue - dwBase;
}

int __stdcall MonsterResolvePopulationIndex(DWORD dwAreaRef, int nClass)
{
	PTLEVELDATAREC ptLvlEntry;
	int nLevelId;
	int nRunLength;
	int nBase;
	int nFirst;
	int i;
	PTMONSTATS ptMonStats;
	int nMember;

	nLevelId = (int)D2CommonDungeonRoomGetLevelId(dwAreaRef);
	ptLvlEntry = (PTLEVELDATAREC)D2CommonLvlTblsGetLevelData(nLevelId);
	if (ptLvlEntry->Mtot == 0)
		return nClass;

	nMember = nClass;
	D2CommonMonsterTblsGetRun(nMember, &nRunLength, &nBase);
	nFirst = nMember - nBase;
	for (i = 0; i < ptLvlEntry->Mtot; i++)
	{
		nMember = ptLvlEntry->M[i];
		if (nMember >= nFirst && nMember <= nFirst + nRunLength - 1)
			break;
	}

	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(nFirst);
	if (i >= ptLvlEntry->Mtot)
	{
		nMember = nFirst;
		for (i = 0; i < nRunLength; i++)
		{
			if (ptMonStats->Level[0] > ptLvlEntry->MonLvl1 + 1)
				break;
			nMember++;
			ptMonStats++;
		}
		if (i >= nRunLength)
			nMember--;
		if (nMember != nFirst)
			nMember--;
	}
	return nMember;
}

BOOL __stdcall MonsterUnitTestBoss(HUNIT ptUnit)
{
	PTMONSTATS ptStats;

	if (ptUnit->eType == UNIT_MONSTER)
	{
		ptStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(ptUnit->dwClassId);
#line 497
		D2ASSERT(ptStats);
		if (ptStats->eBoss)
			return TRUE;
	}
	return FALSE;
}
