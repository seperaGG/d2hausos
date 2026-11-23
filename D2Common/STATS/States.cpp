#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Stats\\States.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "FogImports.h"

void __stdcall StatsUnitSetActiveStateBit(HUNIT hUnit, int nShift, BOOL fSet)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return;

#line 39
	D2ASSERT(nShift < UNIT_STATE_NUM_STATES);

	(&ptStats->dwChangedStates0)[nShift >> 5] |= FoggdwBitMasks[nShift & 31];

	if (fSet)
		(&ptStats->dwStates0)[nShift >> 5] |= FoggdwBitMasks[nShift & 31];
	else
		(&ptStats->dwStates0)[nShift >> 5] &= FoggdwInvBitMasks[nShift & 31];

	D2CommonUnitRoomAddToChangedList(hUnit);
}

DWORD __stdcall StatsGetState(HUNIT hUnit, DWORD dwShift)
{
	STATS_STRUCT *ptStats;
	DWORD *pdwPending;
	int nShift = (int)dwShift;

	if (!((int)hUnit->eType < 0)) {
		if ((int)hUnit->eType > 1 && hUnit->eType != UNIT_MISSILE)
			return 0;

		ptStats = hUnit->ptStats;
#line 74
		D2ASSERT(ptStats);

#line 75
		D2ASSERT(nShift < UNIT_STATE_NUM_STATES);

		pdwPending = &ptStats->dwStates0 + (nShift >> 5);
		return *pdwPending & FoggdwBitMasks[nShift & 31]; }
	return 0;
}

void __stdcall StatsUnitSetActiveStateValue(HUNIT hUnit, int nShift, BOOL fSet)
{
	STATS_STRUCT *ptStats;
	DWORD *pdwActive;

#line 95
	D2ASSERT((STATS_STRUCT *)UnitGetStats (hUnit));
	ptStats = hUnit->ptStats;

#line 96
	D2ASSERT(nShift < UNIT_STATE_NUM_STATES);

	if (fSet)
	{ pdwActive = &ptStats->dwChangedStates0 + (nShift >> 5); *pdwActive |= FoggdwBitMasks[nShift & 31]; }
	else
	{ pdwActive = &ptStats->dwChangedStates0 + (nShift >> 5); *pdwActive &= FoggdwInvBitMasks[nShift & 31]; }

	D2CommonUnitRoomAddToChangedList(hUnit);
}

DWORD __stdcall StatsUnitTestActiveStateBit(HUNIT hUnit, int nShift)
{
	STATS_STRUCT *ptStats;
	DWORD *pdwActive;

#line 113
	D2ASSERT(UnitGetStats(hUnit));
	ptStats = hUnit->ptStats;

#line 114
	D2ASSERT(nShift < UNIT_STATE_NUM_STATES);

	pdwActive = &ptStats->dwChangedStates0 + (nShift >> 5);
	return *pdwActive & FoggdwBitMasks[nShift & 31];
}

void __stdcall StatsUnitClearActiveStates(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return;
	memset(&ptStats->dwChangedStates0, 0, UNIT_STATE_BLOCKS * sizeof(DWORD));
}

BOOL __stdcall StatsUnitTestAnyActiveState(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	DWORD *pdwActive;
	int i;

	ptStats = hUnit->ptStats;
#line 143
	D2ASSERT(ptStats);

	i = 0;
	pdwActive = &ptStats->dwChangedStates0;
	for (; i < UNIT_STATE_BLOCKS; i++)
	{
		if (pdwActive[i] != 0)
			return TRUE;
	}
	return FALSE;
}

void __stdcall StatsUnitCommitPendingFlags(HUNIT hUnit, DWORD dwBoss)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 164
	D2ASSERT(ptStats);

	if (dwBoss != 0)
	{
		ptStats->dwChangedStates0 |= ptStats->dwStates0;
		ptStats->dwChangedStates1 |= ptStats->dwStates1 & 2147483647;
		ptStats->dwChangedStates2 |= ptStats->dwStates2;
		ptStats->dwChangedStates3 |= ptStats->dwStates3 & 0xFFFFFDF2;
		ptStats->dwChangedStates4 |= ptStats->dwStates4;

		ptStats->dwStates0 = 0;
		ptStats->dwStates1 &= 0x80000000;
		ptStats->dwStates2 = 0;
		ptStats->dwStates3 &= 525;
		ptStats->dwStates4 = 0;
	}
	else if (hUnit->eType == UNIT_PLAYER)
	{
		ptStats->dwChangedStates0 |= ptStats->dwStates0;
		ptStats->dwChangedStates1 |= ptStats->dwStates1;
		ptStats->dwChangedStates2 |= ptStats->dwStates2;
		ptStats->dwChangedStates3 |= ptStats->dwStates3 & 0xFFFFEDFF;
		ptStats->dwChangedStates4 |= ptStats->dwStates4;
		ptStats->dwStates0 = 0;
		ptStats->dwStates1 = 0;
		ptStats->dwStates2 = 0;
		ptStats->dwStates3 &= 4608;
		ptStats->dwStates4 = 0;
	}
	else
	{
		ptStats->dwChangedStates0 |= ptStats->dwStates0 & 0xFFFFFFFD;
		ptStats->dwChangedStates1 |= ptStats->dwStates1 & 2147483647;
		ptStats->dwChangedStates2 |= ptStats->dwStates2;
		ptStats->dwChangedStates3 |= ptStats->dwStates3 & 0xFFFFF5F2;
		ptStats->dwChangedStates4 |= ptStats->dwStates4;

		ptStats->dwStates0 &= 2;
		ptStats->dwStates1 &= 0x80000000;
		ptStats->dwStates2 = 0;
		ptStats->dwStates3 &= 2573;
		ptStats->dwStates4 = 0;
	}

	D2CommonUnitRoomAddToChangedList(hUnit);
}

DWORD *__stdcall StatsUnitGetPendingFlags(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 254
	D2ASSERT(ptStats);

	return &ptStats->dwStates0;
}

DWORD __fastcall StatsGetFive(void)
{
	return 5;
}

BOOL __stdcall StatsTestStateInOverlayGroup(int nShift)
{
	return StatsTestStateInSet(nShift, 872415544, 1048383, 0, 0, 0);
}

#pragma auto_inline(off)
static BOOL __fastcall StatsTestStateInSet(int nShift, DWORD dwFlagBlock0, DWORD dwFlagBlock1,
	DWORD dwFlagBlock2, DWORD dwFlagBlock3, DWORD dwFlagBlock4)
{
	DWORD dwFlags;
	int nFlagBlock;
	DWORD dwMask;

	dwMask = 1;
	dwFlags = 0;
#line 281
	D2ASSERT(nShift < UNIT_STATE_NUM_STATES);

	nFlagBlock = nShift / 32;
	dwMask <<= nShift % 32;
	D2_OPAQUE(nFlagBlock);
#line 286
	D2ASSERT(nFlagBlock < UNIT_STATE_BLOCKS);

	switch (nFlagBlock)
	{
	case 0:
		dwFlags = dwFlagBlock0;
		break;
	case 1:
		dwFlags = dwFlagBlock1;
		break;
	case 2:
		dwFlags = dwFlagBlock2;
		break;
	case 3:
		dwFlags = dwFlagBlock3;
		break;
	case 4:
		dwFlags = dwFlagBlock4;
		break;
	}

	return (dwFlags & dwMask) != 0;
}
#pragma auto_inline(on)

BOOL __stdcall StatsTestStateSharedList(int nShift)
{
	return StatsTestStateInSet(nShift, 143131136, 1065353216, 33554432, 0, 1023);
}

BOOL __stdcall StatsTestStateSharedListBase(int nShift)
{
	return StatsTestStateInSet(nShift, 143131136, 1065353216, 0, 0, 0);
}

BOOL __stdcall StatsUnitTestHasAction(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates1 & 1073774592)
		return TRUE;
	return (ptStats->dwStates2 & 17825792) != 0;
}

BOOL __stdcall StatsUnitTestOverlayGroup(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 872415544)
		return TRUE;
	return (ptStats->dwStates1 & 1048383) != 0;
}

BOOL __stdcall StatsTestStatePassesOn(int nShift)
{
	return StatsTestStateInSet(nShift, 312, 1247010, 0, 0, 0);
}

BOOL __stdcall StatsTestStateStoresStat(int nShift)
{
	return StatsTestStateInSet(nShift, 0xE71B4F38, 1955796787, 0xC3840000, 674, 511);
}

BOOL __stdcall StatsTestStateSendsValue(int nShift)
{
	return StatsTestStateInSet(nShift, 625716536, 805344547, 1099169792, 674, 509);
}

BOOL __stdcall StatsTestStateSendsTwoValues(int nShift)
{
	return StatsTestStateInSet(nShift, 1064960, 131584, 0x82000000, 4, 2);
}

BOOL __stdcall StatsTestStateSetAll(int nShift)
{
	return StatsTestStateInSet(nShift, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
}

BOOL __stdcall StatsTestStateSetNoneA(int nShift)
{
	return StatsTestStateInSet(nShift, 0, 0, 0, 0, 0);
}

BOOL __stdcall StatsTestStateSetNoneB(int nShift)
{
	return StatsTestStateInSet(nShift, 0, 0, 0, 0, 0);
}

DWORD __stdcall StatsUnitGetZero(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 467
	D2ASSERT(ptStats);

	return 0;
}

BOOL __fastcall StatsUnitTestFalse(HUNIT hUnit)
{
	return FALSE;
}

BOOL __stdcall StatsUnitTestMatchState(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates1 & 0x80000000)
		return TRUE;
	return (ptStats->dwStates2 >> 29) & 1;
}

BOOL __stdcall StatsUnitTestLightColor(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 0x80000802)
		return TRUE;
	if (ptStats->dwStates1 & 4096)
		return TRUE;
	return (ptStats->dwStates2 >> 26) & 1;
}

BOOL __stdcall StatsUnitTestToHitRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 134217728)
		return TRUE;
	if (ptStats->dwStates1 & 131328)
		return TRUE;
	if (ptStats->dwStates4 & 2)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestDamageRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 134807552)
		return TRUE;
	if (ptStats->dwStates1 & 1026)
		return TRUE;
	if (ptStats->dwStates2 & 33554432)
		return TRUE;
	if (ptStats->dwStates4 & 2)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestArmorClassRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 68191232)
		return TRUE;
	if (ptStats->dwStates1 & 32)
		return TRUE;
	if (ptStats->dwStates2 & 0x83000000)
		return TRUE;
	if (ptStats->dwStates4 & 1)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestFireResistRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 264)
		return TRUE;
	if (ptStats->dwStates4 & 8)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestColdResistRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 272)
		return TRUE;
	if (ptStats->dwStates4 & 16)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestLightResistRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 288)
		return TRUE;
	if (ptStats->dwStates4 & 4)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestPoisonResistRaised(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates4 & 32)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestToHitLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	return (ptStats->dwStates0 >> 19) & 1;
}

BOOL __stdcall StatsUnitTestDamageLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	return (ptStats->dwStates2 >> 25) & 1;
}

BOOL __stdcall StatsUnitTestArmorClassLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats == NULL)
		return FALSE;
	if (ptStats->dwStates0 & 537001984)
		return TRUE;
	return (ptStats->dwStates2 >> 25) & 1;
}

BOOL __stdcall StatsUnitTestFireResistLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if (ptStats->dwStates0 & 536870912)
		return TRUE;
	return (ptStats->dwStates1 >> 29) & 1;
}

BOOL __stdcall StatsUnitTestColdResistLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if (ptStats->dwStates0 & 536870912)
		return TRUE;
	return (ptStats->dwStates1 >> 29) & 1;
}

BOOL __stdcall StatsUnitTestLightResistLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if (ptStats->dwStates0 & 536870912)
		return TRUE;
	return (ptStats->dwStates1 >> 29) & 1;
}

BOOL __stdcall StatsUnitTestPoisonResistLowered(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	return (ptStats->dwStates1 >> 29) & 1;
}

BOOL __stdcall StatsUnitTestOwnedState(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	return (ptStats->dwStates1 & 169869312) != 0;
}

BOOL __stdcall StatsTestStateOwned(int nShift)
{
	return StatsTestStateInSet(nShift, 0, 169869312, 0, 0, 0);
}

BOOL __stdcall StatsUnitTestStateKept(HUNIT hUnit, int nShift)
{
	if (hUnit->eType == UNIT_MONSTER)
		return StatsTestStateInSet(nShift, 2, 0x80000000, 0, 2573, 0);
	return StatsTestStateInSet(nShift, 0, 0, 0, 4608, 0);
}

BOOL __stdcall StatsTestStateGroupA(int nShift)
{
	return StatsTestStateInSet(nShift, 2, 0, 0, 2048, 0);
}

BOOL __stdcall StatsUnitTestStateGroupA(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if ((BYTE)ptStats->dwStates0 & 2)
		return TRUE;
	return (ptStats->dwStates3 >> 11) & 1;
}

BOOL __stdcall StatsTestStateGroupB(int nShift)
{
	return StatsTestStateInSet(nShift, 2, 0x80000000, 0, 2056, 0);
}

BOOL __stdcall StatsUnitTestStateGroupB(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if ((BYTE)ptStats->dwStates0 & 2)
		return TRUE;
	if (ptStats->dwStates1 & 0x80000000)
		return TRUE;
	return (ptStats->dwStates3 & 2056) != 0;
}

BOOL __stdcall StatsUnitTestRegenBaseOnly(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if (ptStats->dwStates0 & 4)
		return TRUE;
	if (ptStats->dwStates1 & 1074790400)
		return TRUE;
	if (ptStats->dwStates3 & 2)
		return TRUE;
	return FALSE;
}

BOOL __stdcall StatsUnitTestStateGroupC(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if ((BYTE)ptStats->dwStates0 & 2)
		return TRUE;
	if (ptStats->dwStates1 & 0x80000000)
		return TRUE;
	return (ptStats->dwStates3 & 2312) != 0;
}

BOOL __stdcall StatsUnitTestNoColors(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	if (ptStats->dwStates0 & 4)
		return TRUE;
	return (ptStats->dwStates1 >> 30) & 1;
}
