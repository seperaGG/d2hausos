#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Stats\\Stats.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "../DATATBLS/DataTbls.h"

struct STATCONTAINER
{
 	BYTE	unobserved_0[56];
 	int	nNum;
};

static void __fastcall StatsListSetValue(PTSTATLIST ptStatsList, int eStat, int nValue, BOOL fAdd,
	BOOL fGlobal);
static int __fastcall StatsListGetValueIndexed(PTSTATLIST ptList, int eStat, int nIndex, DWORD dwMask);
static int __fastcall StatsGetChainValue(STATS_STRUCT *ptStats, int eStat, PTSTATLIST ptList);
static STATS_STRUCT * StatsUnitGetStatsChecked(HUNIT hUnit);
static void __fastcall StatsBuildExportList(STATS_STRUCT *ptStats, BOOL fNoItemSpecific);
static void __fastcall StatsListFreeBlocks(PTSTATLIST ptList);
int __stdcall StatsUnitCalcMaxHp(HUNIT hUnit);
int __stdcall StatsUnitCalcMaxMana(HUNIT hUnit);
int __stdcall StatsUnitCalcMaxStamina(HUNIT hUnit);

PTITEMSTATCOSTTXT __stdcall ItemTblsGetItemStatCost(int nStat);

BOOL __stdcall StatsUnitTestStateKept(HUNIT hUnit, int nShift);

void __stdcall StatsListAddValue(PTSTATLIST ptStatsList, int eStat, int nValue)
{
	StatsListSetValue(ptStatsList, eStat, nValue, TRUE, FALSE);
}

static STATENTRY * __fastcall StatsListFindEntry(PTSTATLIST ptList, int eStat, int nIndex, DWORD dwMask)
{
	STATBLOCK *ptContainer;
	int i;

	if (!dwMask)
	{
		nIndex = eStat / 32;
		dwMask = 1 << (eStat - nIndex * 32);
	}
	if (!(ptList->dwPresenceBits[nIndex] & dwMask))
		return NULL;
	for (ptContainer = &ptList->tBlock0; ptContainer != NULL; ptContainer = ptContainer->ptNext)
	{
		for (i = 0; i < ptContainer->nNum; i++)
		{
			if ((int)ptContainer->atEntries[i].dwStatId == eStat)
				return &ptContainer->atEntries[i];
		}
	}
	return NULL;
}

static STATENTRY *StatsListAppendEntry(PTSTATLIST ptStatsList, int eStat, int nValue)
{
	STATBLOCK *ptContainer;
	STATENTRY *ptEntry;
	unsigned i;

	ptContainer = &ptStatsList->tBlock0;
	while (ptContainer->ptNext != NULL && ptContainer->nNum == 15)
		ptContainer = ptContainer->ptNext;
	D2_OPAQUE(ptContainer);
	if (ptContainer->nNum == 15)
	{
		if (ptContainer->ptNext != NULL)
		{
			ptContainer = ptContainer->ptNext;
#line 120
			D2WARN(! ptContainer->nNum);
		}
		else
		{
#line 122
			ptContainer->ptNext = (STATBLOCK *)D2ALLOCPOOL(ptStatsList->dwMemPool, sizeof(STATBLOCK));
			ptContainer = ptContainer->ptNext;
			for (i = 0; i < sizeof(STATBLOCK) / sizeof(DWORD); i++)
				((DWORD *)ptContainer)[i] = 0;
		}
	}
	ptEntry = &ptContainer->atEntries[ptContainer->nNum];
	ptEntry->dwStatId = eStat;
	ptEntry->dwValue = nValue;
	ptContainer->nNum++;
	return ptEntry;
}

static void __fastcall StatsListSetValue(PTSTATLIST ptStatsList, int eStat, int nValue, BOOL fAdd,
	BOOL fGlobal)
{
	int nIndex;
	DWORD dwMask;
	STATENTRY *ptEntry;

#line 141
	D2WARN(ptStatsList);
#line 142
	D2WARN(eStat != STATS_NONE);
	if (fAdd && !nValue)
		return;

	nIndex = eStat / 32;
	dwMask = 1 << (eStat - nIndex * 32);
	ptEntry = StatsListFindEntry(ptStatsList, eStat, nIndex, dwMask);
	if (ptEntry != NULL)
	{
		if (fAdd)
			ptEntry->dwValue += nValue;
		else
			ptEntry->dwValue = nValue;
	}
	else
	{
		ptStatsList->dwPresenceBits[nIndex] |= dwMask;
		StatsListAppendEntry(ptStatsList, eStat, nValue);
	}

	if (fGlobal)
	{
		ptStatsList->ptStats->adwChangedStats[nIndex] |= dwMask;
		ptStatsList->ptStats->dwChangedStatsFlag = 1;
	}
#line 169
	D2WARN(fGlobal || !ptStatsList->ptStats || (&(ptStatsList->ptStats->tGlobalList) != ptStatsList));
}

void __stdcall StatsListSetValueA(PTSTATLIST ptStatsList, int eStat, int nValue)
{
	StatsListSetValue(ptStatsList, eStat, nValue, FALSE, FALSE);
}

void __stdcall StatsListSetValueB(PTSTATLIST ptStatsList, int eStat, int nValue)
{
	StatsListSetValue(ptStatsList, eStat, nValue, FALSE, FALSE);
}

void __stdcall StatsUnitSetValue(HUNIT hUnit, int eStat, int nValue)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 198
	D2WARN(ptStats);
	StatsListSetValue(&ptStats->tGlobalList, eStat, nValue, FALSE, TRUE);
}

void __stdcall StatsUnitAddValue(HUNIT hUnit, int eStat, int nValue)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 207
	D2WARN(ptStats);
	StatsListSetValue(&ptStats->tGlobalList, eStat, nValue, TRUE, TRUE);
}

int __stdcall StatsUnitGetBaseValue(HUNIT hUnit, int eStat)
{
	STATS_STRUCT *ptStats;
	STATENTRY *ptEntry;

	ptStats = hUnit->ptStats;
#line 217
	D2WARN(ptStats);
	ptEntry = StatsListFindEntry(&ptStats->tGlobalList, eStat, 0, 0);
	if (ptEntry != NULL)
		return ptEntry->dwValue;
	return 0;
}

int __stdcall StatsUnitGetValue(HUNIT hUnit, int eStat)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList;
	int nIndex;
	DWORD dwMask;
	int nSum;

	ptStats = hUnit->ptStats;
#line 226
	D2WARN(ptStats);
#line 227
	D2WARN(eStat != STATS_NONE);
#line 228
	D2WARN(eStat != STATS_MAXHP);
#line 229
	D2WARN(eStat != STATS_MAXMANA);
#line 230
	D2WARN(eStat != STATS_MAXSTAMINA);
#line 231
	D2WARN(eStat != STATS_MAXDURABILITY);
	nIndex = eStat / 32;
	dwMask = 1 << (eStat - nIndex * 32);
	nSum = 0;
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptList->ptNext)
		nSum += StatsListGetValueIndexed(ptList, eStat, nIndex, dwMask);
	return nSum;
}

DWORD __stdcall StatsContainerGetEntry(STATCONTAINER *ptContainer, int nIndex)
{
#line 242
	D2WARN(ptContainer->nNum);
#line 243
	D2WARN(nIndex < ptContainer->nNum);
	return *(DWORD *)((BYTE *)ptContainer + 64 + nIndex * 8);
}

DWORD __stdcall StatsGetTotalValue(HUNIT hUnit, int nStat)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList;
	int nIndex;
	DWORD dwMask;
	DWORD dwSum;

	ptStats = hUnit->ptStats;
#line 252
	D2WARN(ptStats);
	nIndex = nStat / 32;
	dwMask = 1 << (nStat - nIndex * 32);
	dwSum = 0;
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptList->ptNext)
		dwSum += StatsListGetValueIndexed(ptList, nStat, nIndex, dwMask);
	return dwSum;
}

int __stdcall StatsUnitGetBonusValue(HUNIT hUnit, int eStat)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 261
	D2WARN(ptStats);
	if (!(ptStats->tGlobalList.ptNext == NULL)) {
		return StatsGetChainValue(ptStats, eStat, ptStats->tGlobalList.ptNext); }
	return 0;
}

DWORD __stdcall StatsListGetValueA(PTSTATLIST ptStatsList, int nStat)
{
	STATBLOCK *ptBlock;
	int nWord;
	DWORD dwBit;
	int i;
	struct STATENTRY *ptEntry;

#line 273
	D2WARN(ptStatsList);
	nWord = nStat / 32;
	dwBit = 1u << (nStat - nWord * 32);
	if (!(ptStatsList->dwPresenceBits[nWord] & dwBit))
		return 0;
	for (ptBlock = &ptStatsList->tBlock0; ptBlock != NULL; ptBlock = ptBlock->ptNext)
	{
		for (i = 0; i < (int)ptBlock->nNum; i++)
		{
			if ((int)ptBlock->atEntries[i].dwStatId == nStat)
			{
				ptEntry = &ptBlock->atEntries[i];
				if (ptEntry == NULL)
					return 0;
				return ptEntry->dwValue;
			}
		}
	}
	return 0;
}

void __stdcall StatsSetListEndFrame(PTSTATLIST ptStatsList, DWORD dwValue)
{
#line 282
	D2WARN(ptStatsList);
	ptStatsList->dwExpiry = dwValue;
}

void __stdcall StatsSetListRemoveCallback(PTSTATLIST ptStatsList, DWORD dwValue)
{
#line 292
	D2WARN(ptStatsList);
	ptStatsList->pfnRemoveCallback = (PFNSTATLISTREMOVED)dwValue;
}

void __stdcall StatsListReset(PTSTATLIST ptList)
{
	STATBLOCK *ptBlock;

	if (!ptList)
		return;
#line 306
	D2WARN(!ptList->ptStats || &(ptList->ptStats->tGlobalList) != ptList);
	for (ptBlock = &ptList->tBlock0; ptBlock != NULL; ptBlock = ptBlock->ptNext)
		ptBlock->nNum = 0;
	memset(ptList->dwPresenceBits, 0, sizeof(ptList->dwPresenceBits));
}

PTSTATLIST __stdcall StatsAllocList(DWORD dwPool, DWORD dwFlags, DWORD dwExpiry, DWORD dwSourceType,
	DWORD dwSourceId)
{
	PTSTATLIST ptList;
	unsigned i;

#line 409
	ptList = (PTSTATLIST)D2ALLOCPOOL(dwPool, sizeof(STATLIST));
	for (i = 0; i < sizeof(STATLIST) / sizeof(DWORD); i++)
		((DWORD *)ptList)[i] = 0;
	ptList->dwSourceType = dwSourceType;
	ptList->dwSourceId = dwSourceId;
	ptList->dwFlags = dwFlags;
	ptList->dwExpiry = dwExpiry;
	ptList->dwMemPool = dwPool;
	return ptList;
}

DWORD __stdcall StatsGetListSourceType(PTSTATLIST ptList)
{
#line 424
	D2WARN(ptList);
	return ptList->dwSourceType;
}

DWORD __stdcall StatsGetListSourceId(PTSTATLIST ptList)
{
#line 433
	D2WARN(ptList);
	return ptList->dwSourceId;
}

DWORD __stdcall StatsGetListEndFrame(PTSTATLIST ptList)
{
#line 442
	D2WARN(ptList);
	return ptList->dwExpiry;
}

void __stdcall StatsSetListState(PTSTATLIST ptList, DWORD dwValue)
{
#line 451
	D2WARN(ptList);
	ptList->dwState = dwValue;
}

DWORD __stdcall StatsGetListState(PTSTATLIST ptList)
{
#line 460
	D2WARN(ptList);
	return ptList->dwState;
}

PTSTATLIST __stdcall StatsFindListByState(HUNIT hUnit, DWORD dwKey)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList;

	ptStats = hUnit->ptStats;
#line 469
	D2WARN(ptStats);
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptList->ptNext)
	{
		if (ptList->dwState == dwKey)
			return ptList;
	}
	return NULL;
}

PTSTATLIST __stdcall StatsFindListByMask(HUNIT hUnit, DWORD dwMask)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return NULL;
	ptList = ptStats->ptList;
	while (ptList != NULL)
	{
		if (ptList->dwFlags & dwMask)
			return ptList;
		ptList = ptList->ptNext;
	}
	return NULL;
}

void __stdcall StatsListFree(PTSTATLIST ptList)
{
#line 517
	D2WARN(ptList);
	StatsListFreeBlocks(ptList);
#line 521
	D2FREEPOOL(ptList->dwMemPool, ptList);
}

void __stdcall StatsUnitUnlinkList(HUNIT hUnit, PTSTATLIST ptListVictim)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptListCurr;

#line 553
	D2WARN(ptListVictim);
	ptStats = hUnit->ptStats;
#line 556
	D2WARN(ptStats);
#line 558
	D2WARN(&(ptListVictim->ptStats->tGlobalList) != ptListVictim);
	ptListCurr = ptStats->ptList;
	if (ptListCurr != NULL)
	{
		if (ptListCurr != ptListVictim)
		{
			while (ptListCurr->ptNext != ptListVictim)
				ptListCurr = ptListCurr->ptNext;
#line 568
			D2WARN(ptListCurr);
			ptListCurr->ptNext = ptListVictim->ptNext;
		}
		else
			ptStats->ptList = ptListCurr->ptNext;
	}
	ptListVictim->ptStats = NULL;
	ptListVictim->ptNext = NULL;
	if (ptListVictim->pfnRemoveCallback != NULL)
		ptListVictim->pfnRemoveCallback(hUnit, ptListVictim->dwState);
}

void __stdcall StatsUnitLinkList(HUNIT hUnit, PTSTATLIST ptList)
{
	STATS_STRUCT *ptStats;

#line 589
	D2WARN(ptList);
	ptStats = hUnit->ptStats;
#line 592
	D2WARN(ptStats);
#line 594
	D2WARN(ptList);
#line 595
	D2WARN(ptList->ptNext == NULL);
	ptList->ptStats = ptStats;
	if ((BYTE)ptList->dwFlags & 4)
		ptStats->dwFlags |= 2;
	if (ptStats->ptList != NULL)
	{
		ptList->ptNext = ptStats->ptList->ptNext;
		ptStats->ptList->ptNext = ptList;
	}
	else
	{
		ptList->ptNext = NULL;
		ptStats->ptList = ptList;
	}
}

void __stdcall StatsUnitFreeLists(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList, ptNext;

	ptStats = hUnit->ptStats;
#line 616
	D2WARN(ptStats);
#line 617
	D2WARN(! (ptStats->dwFlags & STATS_MODIFYING_MASK));
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptNext)
	{
		ptNext = ptList->ptNext;
		if (ptList->dwSourceType != UNIT_ITEM && !(ptList->dwFlags & 385)
			&& !StatsUnitTestStateKept(hUnit, (int)ptList->dwState) && ptList != &ptStats->tGlobalList)
		{
			StatsUnitUnlinkList(hUnit, ptList);
			StatsListFree(ptList);
		}
	}
}

void __stdcall StatsUnitClearFieldEc(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return;
	if (!ptStats->dwChangedStatsFlag)
		return;
	memset(ptStats->adwChangedStats, 0, sizeof(ptStats->adwChangedStats));
	ptStats->dwChangedStatsFlag = 0;
}

typedef void (__fastcall *PFNSTATCHANGE)(HUNIT hUnit, int eStat, int nValue, DWORD dwArg);

static void StatsCallChangeFunction(HUNIT hUnit, int eStat, int nValue, DWORD dwArg,
	PFNSTATCHANGE pfnChangeFunction)
{
#line 662
	D2WARN(pfnChangeFunction);
	pfnChangeFunction(hUnit, eStat, nValue, dwArg);
}

void __stdcall StatsUnitCallChanged(HUNIT hUnit, DWORD dwArg, int eStat, PFNSTATCHANGE pfnChangeFunction)
{
	STATS_STRUCT *ptStats;
	int nIndex;
	DWORD dwMask;

	ptStats = hUnit->ptStats;
#line 671
	D2WARN(ptStats);
	if (!ptStats->dwChangedStatsFlag)
		return;
	nIndex = eStat / 32;
	dwMask = 1 << (eStat - nIndex * 32);
	if (!(ptStats->adwChangedStats[nIndex] & dwMask))
		return;
	StatsCallChangeFunction(hUnit, eStat,
		StatsListGetValueIndexed(&ptStats->tGlobalList, eStat, nIndex, dwMask), dwArg, pfnChangeFunction);
}

void __stdcall StatsUnitCallChangedAll(HUNIT hUnit, DWORD dwArg, PFNSTATCHANGE pfnChangeFunction)
{
	STATS_STRUCT *ptStats;
	int eStat;
	int nIndex;
	DWORD dwMask;

	ptStats = hUnit->ptStats;
	for (eStat = 0; eStat < 16; eStat++)
	{
		nIndex = eStat / 32;
		dwMask = 1 << (eStat - nIndex * 32);
		if (ptStats->adwChangedStats[nIndex] & dwMask)
			StatsCallChangeFunction(hUnit, eStat,
				StatsListGetValueIndexed(&ptStats->tGlobalList, eStat, nIndex, dwMask), dwArg,
				pfnChangeFunction);
	}
}

void __stdcall StatsUnitClampToMax(HUNIT hUnit)
{
	int nValue, nMax;

	if (!hUnit->ptStats)
		return;

	nValue = StatsUnitGetValue(hUnit, 10);
	nMax = StatsUnitCalcMaxStamina(hUnit);
	if (nValue > nMax)
		StatsUnitAddValue(hUnit, 10, nMax - nValue);

	nValue = StatsUnitGetValue(hUnit, 8);
	nMax = StatsUnitCalcMaxMana(hUnit);
	if (nValue > nMax)
		StatsUnitAddValue(hUnit, 8, nMax - nValue);

	nValue = StatsUnitGetValue(hUnit, 6);
	nMax = StatsUnitCalcMaxHp(hUnit);
	if (nValue > nMax)
		StatsUnitAddValue(hUnit, 6, nMax - nValue);
}

static int __fastcall StatsListGetValueIndexed(PTSTATLIST ptList, int eStat, int nIndex, DWORD dwMask)
{
	STATENTRY *ptEntry;

	ptEntry = StatsListFindEntry(ptList, eStat, nIndex, dwMask);
	if (ptEntry != NULL)
		return ptEntry->dwValue;
	return 0;
}

static int __fastcall StatsGetChainValue(STATS_STRUCT *ptStats, int eStat, PTSTATLIST ptList)
{
	int nIndex;
	DWORD dwMask;
	int nSum;

	nIndex = eStat / 32;
	dwMask = 1 << (eStat - nIndex * 32);
	nSum = 0;
	if (ptList == NULL)
		ptList = ptStats->ptList;
	for ( ; ptList != NULL; ptList = ptList->ptNext)
		nSum += StatsListGetValueIndexed(ptList, eStat, nIndex, dwMask);
	return nSum;
}

void __stdcall StatsUnitFreeStateLists(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList, ptNext;

	ptStats = hUnit->ptStats;
	if (!ptStats || !(ptStats->dwFlags & 2))
		return;
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptNext)
	{
		ptNext = ptList->ptNext;
		if ((BYTE)ptList->dwFlags & 4)
		{
			if (ptList->dwState)
				StatsUnitSetActiveStateBit(hUnit, ptList->dwState, FALSE);
			StatsUnitUnlinkList(hUnit, ptList);
			StatsListFree(ptList);
		}
	}
	ptStats->dwFlags &= ~2;
}

void __stdcall StatsUnitExpireLists(HUNIT hUnit, int nFrame)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList, ptNext;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return;
	if (nFrame)
	{
		for (ptList = ptStats->ptList; ptList != NULL; ptList = ptNext)
		{
			ptNext = ptList->ptNext;
			if (((BYTE)ptList->dwFlags & 2) && (int)ptList->dwExpiry <= nFrame)
			{
				StatsUnitUnlinkList(hUnit, ptList);
				StatsListFree(ptList);
			}
		}
	}
	else
	{
		for (ptList = ptStats->ptList; ptList != NULL; ptList = ptNext)
		{
			ptNext = ptList->ptNext;
			if ((BYTE)ptList->dwFlags & 2)
			{
				ptList->dwExpiry--;
				if ((int)ptList->dwExpiry <= 0)
				{
					StatsUnitUnlinkList(hUnit, ptList);
					StatsListFree(ptList);
				}
			}
		}
	}
}

void __stdcall StatsUnitApplyDye(HUNIT hCloth, HUNIT hDye, int nMode)
{
	STATS_STRUCT *ptDyeStats;

	ptDyeStats = hDye->ptStats;
#line 784
	D2WARN(ptDyeStats);
#line 786
	D2WARN(hCloth);
#line 788
	D2WARN(! (ptDyeStats->dwFlags & STATS_MODIFYING_MASK));
	if (!(ptDyeStats->dwFlags & STATS_EXPORT_MASK))
	{
		ptDyeStats->dwFlags |= STATS_EXPORT_MASK;
		if (ptDyeStats->ptExportList == NULL)
			ptDyeStats->ptExportList = StatsAllocList(hDye->dwMemPool, 0, 0, hDye->eType, hDye->dwId);
		ptDyeStats->ptExportList->dwFlags |= 256;
	}
	if (nMode == 0)
		ptDyeStats->ptExportList->dwFlags |= 1024;
	else
		ptDyeStats->ptExportList->dwFlags &= ~1024;
	StatsBuildExportList(ptDyeStats, nMode == 0);
	StatsUnitLinkList(hCloth, ptDyeStats->ptExportList);
	ptDyeStats->hCloth = hCloth;
	ptDyeStats->dwFlags |= STATS_MODIFYING_MASK;
}

static int StatsGetCombinedValue(STATS_STRUCT *ptStats, int eStatA, int eStatB)
{
	int nSumA, nValueB, nSumB;

#line 321
	D2ASSERT(ptStats);
	nSumA = StatsGetChainValue(ptStats, eStatA, NULL);
	nValueB = StatsListGetValueIndexed(&ptStats->tGlobalList, eStatB, 0, 0);
	nSumB = 0;
	if (ptStats->tGlobalList.ptNext != NULL)
		nSumB = StatsGetChainValue(ptStats, eStatB, ptStats->tGlobalList.ptNext);
	return (nSumA * nValueB) / 100 + nSumB + nValueB;
}

struct STATSCOMBINE
{
	int		field_00;
	int		field_04;
};

static const struct STATSCOMBINE sgtStatsCombine[3] =
{
	{ 31, -1 },
	{ 22, -1 },
	{ 21, -1 },
};

static void __fastcall StatsBuildExportList(STATS_STRUCT *ptStats, BOOL fNoItemSpecific)
{
	PTSTATLIST ptList;
	STATBLOCK *ptBlock;
	STATENTRY *ptEntry;
	int i;
	int eStat, nValue;
	int nCombined;
	BOOL fCombined;

#line 338
	D2WARN(ptStats);
#line 339
	D2WARN(ptStats->dwFlags & STATS_EXPORT_MASK);
#line 340
	D2WARN(ptStats->ptExportList);
	StatsListReset(ptStats->ptExportList);
	fCombined = FALSE;
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptList->ptNext)
	{
		for (ptBlock = &ptList->tBlock0; ptBlock != NULL; ptBlock = ptBlock->ptNext)
		{
			for (i = 0; i < ptBlock->nNum; i++)
			{
				eStat = (int)ptBlock->atEntries[i].dwStatId;
				nValue = (int)ptBlock->atEntries[i].dwValue;
				if (fNoItemSpecific && ItemTblsGetItemStatCost(eStat)->ItemSpecific)
					continue;
				if (eStat >= 16 && eStat <= 18)
					fCombined = TRUE;
				else
					StatsListSetValue(ptStats->ptExportList, eStat, nValue, TRUE, FALSE);
			}
		}
	}
	if (!fCombined)
		return;
	for (eStat = 16; eStat - 16 < 3; eStat++)
	{
		if (fNoItemSpecific && ItemTblsGetItemStatCost(eStat)->ItemSpecific)
			continue;
		nCombined = sgtStatsCombine[eStat - 16].field_00;
		ptEntry = StatsListFindEntry(ptStats->ptExportList, nCombined, 0, 0);
		if (ptEntry == NULL)
			continue;
		ptEntry->dwValue = StatsGetCombinedValue(ptStats, eStat, nCombined);
		ptStats->dwChangedStatsFlag = 1;
	}
}

void __stdcall StatsUnitRemoveDye(HUNIT hCloth, HUNIT hDye)
{
	STATS_STRUCT *ptDyeStats;

	ptDyeStats = hDye->ptStats;
#line 820
	D2WARN(ptDyeStats);
	if (ptDyeStats->dwFlags & STATS_MODIFYING_MASK)
	{
		if (!(ptDyeStats->ptExportList->dwFlags & 512))
			StatsUnitUnlinkList(hCloth, ptDyeStats->ptExportList);
	}
	ptDyeStats->dwFlags &= ~(STATS_MODIFYING_MASK | 512);
}

void __stdcall StatsUnitRebuildExport(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (ptStats->dwFlags & STATS_EXPORT_MASK)
		StatsBuildExportList(ptStats, (ptStats->dwFlags >> 10) & 1);
}

void __stdcall StatsUnitAllocStats(HUNIT hUnit, DWORD dwFlags)
{
	STATS_STRUCT *ptStats;
	unsigned i;

#line 842
	D2ASSERT(! UnitGetStats (hUnit));
#line 843
	ptStats = (STATS_STRUCT *)D2ALLOCPOOL(hUnit->dwMemPool, sizeof(STATS_STRUCT));
	for (i = 0; i < sizeof(STATS_STRUCT) / sizeof(DWORD); i++)
		((DWORD *)ptStats)[i] = 0;
	hUnit->ptStats = ptStats;
	ptStats->dwFlags = dwFlags & STATS_EXPORT_MASK;
	if (ptStats->dwFlags & STATS_EXPORT_MASK)
		ptStats->ptExportList = StatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
	ptStats->tGlobalList.dwSourceType = hUnit->eType;
	ptStats->tGlobalList.dwSourceId = hUnit->dwId;
	StatsUnitLinkList(hUnit, &ptStats->tGlobalList);
}

static void StatsListFreeOrDetach(PTSTATLIST ptList)
{
	if (ptList->dwFlags & 256)
	{
		ptList->dwFlags |= 512;
		ptList->ptNext = NULL;
	}
	else
		StatsListFree(ptList);
}

static void StatsUnitFreeChainLists(STATS_STRUCT *ptStats)
{
	PTSTATLIST ptList, ptNext;

	for (ptList = ptStats->tGlobalList.ptNext; ptList != NULL; ptList = ptNext)
	{
		ptNext = ptList->ptNext;
		StatsListFreeOrDetach(ptList);
	}
	StatsListFreeBlocks(&ptStats->tGlobalList);
}

void __stdcall StatsUnitFreeStats(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList, ptNext;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return;
#line 867
	D2ASSERT(ptStats->ptList == &(ptStats->tGlobalList));
	StatsUnitFreeChainLists(ptStats);
	if (ptStats->ptExportList != NULL)
	{
		if (ptStats->dwFlags & STATS_MODIFYING_MASK)
			StatsUnitRemoveDye(ptStats->hCloth, hUnit);
		for (ptList = ptStats->ptExportList; ptList != NULL; ptList = ptNext)
		{
			ptNext = ptList->ptNext;
			StatsListFree(ptList);
		}
	}
#line 880
	D2FREEPOOL(hUnit->dwMemPool, ptStats);
	hUnit->ptStats = NULL;
}

static void __fastcall StatsListFreeBlocks(PTSTATLIST ptList)
{
	STATBLOCK *ptBlock, *ptNext;

	ptBlock = ptList->tBlock0.ptNext;
	while (ptBlock != NULL)
	{
		ptNext = ptBlock->ptNext;
#line 507
		D2FREEPOOL(ptList->dwMemPool, ptBlock);
		ptBlock = ptNext;
	}
}

void __stdcall StatsSetListEndFrameSecond(PTSTATLIST ptStatsList, DWORD dwValue)
{
#line 889
	D2WARN(ptStatsList);
	ptStatsList->dwExpiry = dwValue;
}

DWORD __stdcall StatsGetGlobalListEndFrame(PTSTATS ptStatsList)
{
#line 898
	D2WARN(ptStatsList);
	return ptStatsList->tGlobalList.dwSourceType;
}

void __stdcall StatsListSetValuePacked(PTSTATLIST ptStatsList, int eStat, int nLow, int nHigh)
{
	StatsListSetValue(ptStatsList, eStat, (nHigh << 8) + nLow, FALSE, FALSE);
}

void __stdcall StatsUnitSetHasOverlay(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
	{
#line 918
		D2WARN(ptStats);
		return;
	}
	ptStats->dwFlags |= 256;
}

BOOL __stdcall StatsTestHasOverlay(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
	if (!ptStats)
		return FALSE;
	return (ptStats->dwFlags >> 8) & 1;
}

DWORD __stdcall StatsListGetValueB(PTSTATLIST ptStatsList, int nStat)
{
	STATBLOCK *ptBlock;
	int nWord;
	DWORD dwBit;
	int i;
	struct STATENTRY *ptEntry;

#line 273
	D2WARN(ptStatsList);
	nWord = nStat / 32;
	dwBit = 1u << (nStat - nWord * 32);
	if (!(ptStatsList->dwPresenceBits[nWord] & dwBit))
		return 0;
	for (ptBlock = &ptStatsList->tBlock0; ptBlock != NULL; ptBlock = ptBlock->ptNext)
	{
		for (i = 0; i < (int)ptBlock->nNum; i++)
		{
			if ((int)ptBlock->atEntries[i].dwStatId == nStat)
			{
				ptEntry = &ptBlock->atEntries[i];
				if (ptEntry == NULL)
					return 0;
				return ptEntry->dwValue;
			}
		}
	}
	return 0;
}

void __stdcall StatsUnitClearHasOverlay(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList;

	ptStats = hUnit->ptStats;
	if (!ptStats)
	{
#line 956
		D2WARN(ptStats);
		return;
	}
	ptStats->dwFlags &= ~256;
	ptList = NULL;
	if (hUnit->ptStats)
		ptList = hUnit->ptStats->ptList;
	while (ptList != NULL && !(ptList->dwFlags & 128))
		ptList = ptList->ptNext;
	StatsListReset(ptList);
}

BOOL __stdcall StatsTestModifying(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 975
	D2WARN(ptStats);
	if (ptStats->dwFlags & 64)
		return TRUE;
	return FALSE;
}

static __inline DWORD StatsListGetAlignmentValue(PTSTATLIST ptList)
{
	STATBLOCK *ptBlock;
	struct STATENTRY *ptEnt;
	int i;

	if (!(!(ptList->dwPresenceBits[172 / 32] & (1u << (172 % 32)))))
	{
		for (ptBlock = &ptList->tBlock0; ptBlock != NULL; ptBlock = ptBlock->ptNext)
		{
			for (i = 0; i < (int)ptBlock->nNum; i++)
			{
				if (ptBlock->atEntries[i].dwStatId == 172)
				{
					ptEnt = &ptBlock->atEntries[i];
					if (ptEnt)
						return ptEnt->dwValue;
					return 0; }
			} }
		return 0; }
	return 0;
}

static __inline DWORD StatsUnitGetAlignmentInline(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	PTSTATLIST ptList;

	if (!hUnit)
		return 2;
	if ((int)hUnit->eType < 0 || (int)hUnit->eType > 1)
		return 2;
	ptStats = hUnit->ptStats;
#line 469
	D2WARN(ptStats);
	for (ptList = ptStats->ptList; ptList != NULL; ptList = ptList->ptNext)
	{
		if (ptList->dwState == 105)
			break;
	}
	if (!ptList)
		return 2;
	return StatsListGetAlignmentValue(ptList);
}

DWORD __stdcall StatsUnitGetAlignment(HUNIT hUnit)
{
	return StatsUnitGetAlignmentInline(hUnit);
}

static STATS_STRUCT * StatsUnitGetStatsChecked(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;

	ptStats = hUnit->ptStats;
#line 252
	D2WARN(ptStats);
	return ptStats;
}

BOOL __stdcall StatsUnitsTestSameAlignment(HUNIT hUnitA, HUNIT hUnitB)
{
	DWORD dwValueA, dwValueB;

	if (!hUnitA || !hUnitB)
		return FALSE;
	if (hUnitA == hUnitB)
		return TRUE;
	dwValueA = StatsUnitGetAlignmentInline(hUnitA);
	dwValueB = StatsUnitGetAlignmentInline(hUnitB);
	switch (dwValueA)
	{
	case 0:
		if (dwValueB == 0)
			return TRUE;
		break;
	case 1:
		return FALSE;
	case 2:
		if (dwValueB == 0 || dwValueB == 1)
			return FALSE;
		if (dwValueB == 2)
			return TRUE;
		break;
	}
	return FALSE;
}

static int StatsUnitCalcRaisedMax(HUNIT hUnit, int eStat, int ePercentStat)
{
	STATS_STRUCT *ptStats;
	int nBase, nPercent;

	ptStats = hUnit->ptStats;
#line 1052
	D2WARN(ptStats);
	nBase = StatsGetChainValue(ptStats, eStat, NULL);
	nPercent = (int)StatsGetTotalValue(hUnit, ePercentStat);
	return (nPercent * nBase) / 100 + nBase;
}

int __stdcall StatsUnitCalcMaxHp(HUNIT hUnit)
{
	return StatsUnitCalcRaisedMax(hUnit, 7, 76);
}

int __stdcall StatsUnitCalcMaxMana(HUNIT hUnit)
{
	return StatsUnitCalcRaisedMax(hUnit, 9, 77);
}

int __stdcall StatsUnitCalcMaxStamina(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	int nBase, nPercent;

	ptStats = hUnit->ptStats;
#line 1075
	D2WARN(ptStats);
	nBase = StatsGetChainValue(ptStats, STATS_MAXSTAMINA, NULL);
	ptStats = StatsUnitGetStatsChecked(hUnit);
	nPercent = StatsGetChainValue(ptStats, 162, NULL) + (int)StatsGetTotalValue(hUnit, 163);
	return (nPercent * nBase) / 100 + nBase;
}

int __stdcall StatsUnitCalcMaxDurability(HUNIT hUnit)
{
	return StatsUnitCalcRaisedMax(hUnit, 73, 75);
}

int __stdcall StatsUnitCalcMaxDamage(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	int nSumP, nVal, nSumB;

	ptStats = hUnit->ptStats;
#line 321
	D2ASSERT(ptStats);
	nSumP = StatsGetChainValue(ptStats, 17, NULL);
	nVal = StatsListGetValueIndexed(&ptStats->tGlobalList, 22, 22 / 32, 1u << (22 % 32));
	nSumB = 0;
	if (ptStats->tGlobalList.ptNext != NULL)
		nSumB = StatsGetChainValue(ptStats, 22, ptStats->tGlobalList.ptNext);
	return (nSumP * nVal) / 100 + nSumB + nVal;
}

int __stdcall StatsUnitCalcMinDamage(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	int nSumP, nVal, nSumB;

	ptStats = hUnit->ptStats;
#line 321
	D2ASSERT(ptStats);
	nSumP = StatsGetChainValue(ptStats, 18, NULL);
	nVal = StatsListGetValueIndexed(&ptStats->tGlobalList, 21, 21 / 32, 1u << (21 % 32));
	nSumB = 0;
	if (ptStats->tGlobalList.ptNext != NULL)
		nSumB = StatsGetChainValue(ptStats, 21, ptStats->tGlobalList.ptNext);
	return (nSumP * nVal) / 100 + nSumB + nVal;
}

int __stdcall StatsUnitCalcArmorClass(HUNIT hUnit)
{
	STATS_STRUCT *ptStats;
	int nSumP, nVal, nSumB;

	ptStats = hUnit->ptStats;
#line 321
	D2ASSERT(ptStats);
	nSumP = StatsGetChainValue(ptStats, 16, NULL);
	nVal = StatsListGetValueIndexed(&ptStats->tGlobalList, 31, 31 / 32, 1u << (31 % 32));
	nSumB = 0;
	if (ptStats->tGlobalList.ptNext != NULL)
		nSumB = StatsGetChainValue(ptStats, 31, ptStats->tGlobalList.ptNext);
	return (nSumP * nVal) / 100 + nSumB + nVal;
}

int ganStatsUnread[4] = { 67, 68, 12, -1 };
