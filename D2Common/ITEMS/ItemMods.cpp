#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\ITEMS\\ItemMods.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "../DATATBLS/DataTbls.h"

typedef int (__fastcall *PFNITEMMOD)(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods, int nIndex,
	DWORD nType, int nStatId, BOOL fRemove);

struct ITEMMODCALL
{
 	PFNITEMMOD	pfnFunction;
 	int			nStatId;
};
typedef struct ITEMMODCALL ITEMMODCALL;

static int __fastcall ITEMSModsRollSigned(HUNIT hItem, int nMin, int nMax);
static int __fastcall ITEMSModsRollAbs(HUNIT hItem, int nMin, int nMax);

static void __fastcall ITEMSModsScaleValue(int nStatId, int *pnValue)
{
	switch (nStatId)
	{
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		*pnValue <<= 8;
	case 57:
	case 58:
		return;
	}
}

static int __fastcall ITEMSModsApplyStat(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods, int nIndex,
	DWORD nType, int nStatId, BOOL fRemove)
{
	PTITEMMODTXT ptProperty;
	PTITEMMODTXT aptGemProperties[3];
	DWORD nNumGemProperties;
	int nValue;
	PTITEMSTXT ptItemStats;
	int nItemIndex;
	PTSTATLIST hStatsList;

	switch (eMods)
	{
	case 0:
		ptProperty = &((PTMAGICAFFIXTXT)pMods)->mod[nIndex];
#line 107
		D2ASSERT(ptProperty);
		nValue = ITEMSModsRollSigned(hItem, ptProperty->min, ptProperty->max);
		break;
	case 1:
		ptProperty = &((PTQUALITYITEMSTXT)pMods)->mod[nIndex];
#line 118
		D2ASSERT(ptProperty);
		nValue = ITEMSModsRollSigned(hItem, ptProperty->min, ptProperty->max);
		ptItemStats = (PTITEMSTXT)D2CommonItemTblsFindItemByCode(D2CommonITEMSGetTypeCode(hItem),
			&nItemIndex);
		switch (nStatId)
		{
		case 16:
		case 31:
			D2CommonStatsUnitSetValue(hItem, 31, ptItemStats->maxac + 1);
			break;
		case 17:
		case 22:
			D2CommonStatsUnitSetValue(hItem, 22, ptItemStats->maxdam + 1);
			break;
		case 18:
		case 21:
			D2CommonStatsUnitSetValue(hItem, 21, ptItemStats->mindam + 1);
			break;
		}
		break;
	case 2:
		aptGemProperties[0] = ((PTGEMSTXT)pMods)->weaponmod;
		aptGemProperties[1] = ((PTGEMSTXT)pMods)->helmmod;
		aptGemProperties[2] = ((PTGEMSTXT)pMods)->shieldmod;
		nNumGemProperties = arrsize(aptGemProperties);
#line 135
		D2ASSERT(nType < nNumGemProperties);
		ptProperty = &aptGemProperties[nType][nIndex];
#line 138
		D2ASSERT(ptProperty);
		nValue = ITEMSModsRollSigned(hItem, ptProperty->min, ptProperty->max);
		break;
	case 3:
		ptProperty = &((PTUNIQUEITEMSTXT)pMods)->itemmod[nIndex];
#line 96
		D2ASSERT(ptProperty);
		nValue = ITEMSModsRollSigned(hItem, ptProperty->min, ptProperty->max);
		break;
	case 4:
		ptProperty = &((PTSETITEMSTXT)pMods)->propertymod[nIndex];
#line 85
		D2ASSERT(ptProperty);
		nValue = ITEMSModsRollSigned(hItem, ptProperty->min, ptProperty->max);
		break;
	default:
#line 216
		D2ASSERT(0);
		return 0;
	}
	if (nValue == 0)
		return 0;
	{
		ITEMSModsScaleValue(nStatId, &nValue);
		if (hOwner != NULL)
			hStatsList = (PTSTATLIST)D2CommonStatsFindListByMask(hOwner, 4096);
		else
			hStatsList = (PTSTATLIST)D2CommonStatsFindListByMask(hItem, 64);
#line 233
		D2ASSERT(hStatsList);
		if (fRemove)
			nValue = -(int)D2CommonStatsListGetValueA(hStatsList, nStatId);
		D2CommonStatsListAddValue(hStatsList, nStatId, nValue);
		switch (nStatId)
		{
		case 1:
			D2CommonStatsListAddValue(hStatsList, 9, nValue << 9);
			break;
		case 3:
			D2CommonStatsListAddValue(hStatsList, 7, nValue << 9);
			break; }
		return 1; }

}

static int __fastcall ITEMSModsRollSigned(HUNIT hItem, int nMin, int nMax)
{
	BOOL fNegative;
	int nRange;
	DWORD dwSeed;
	int nValue;

	fNegative = FALSE;
	if (nMin < 0)
	{
		if (nMax < 0)
			fNegative = TRUE;
		nMin = -nMin;
	}
	if (nMax < 0)
		nMax = -nMax;
	if (nMin == 0 && nMax == 0)
		return 0;
#line 52
	D2ASSERT(nMin <= nMax);
	nRange = nMax - nMin;
	dwSeed = (DWORD)D2CommonITEMSGetItemSeed(hItem);
	if (nRange != 0)
		nValue = (int)D2CommonSeedRand(dwSeed, nRange);
	else
		nValue = 0;
	nValue += nMin;
#line 55
	D2ASSERT(nValue <= nMax);
	if (fNegative)
		nValue = -nValue;
	return nValue;
}

static int __fastcall ITEMSModsApplyStatAbs(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods, int nIndex,
	DWORD nType, int nStatId, BOOL fRemove)
{
	PTITEMMODTXT ptProperty;
	PTITEMMODTXT aptGemProperties[3];
	DWORD nNumGemProperties;
	int nStatValue;
	PTITEMSTXT ptItemStats;
	int nItemIndex;
	PTSTATLIST hStatsList;
	int nMin, nMax, nRange, nValue;
	int nRoll;
	DWORD dwSeed;

	switch (eMods)
	{
	case 0:
		ptProperty = &((PTMAGICAFFIXTXT)pMods)->mod[nIndex];
#line 285
		D2ASSERT(ptProperty);
		nStatValue = ITEMSModsRollAbs(hItem, ptProperty->min, ptProperty->max);
		break;
	case 1:
		ptProperty = &((PTQUALITYITEMSTXT)pMods)->mod[nIndex];
#line 296
		D2ASSERT(ptProperty);
		nStatValue = ITEMSModsRollAbs(hItem, ptProperty->min, ptProperty->max);
		break;
	case 2:
		aptGemProperties[0] = ((PTGEMSTXT)pMods)->weaponmod;
		aptGemProperties[1] = ((PTGEMSTXT)pMods)->helmmod;
		aptGemProperties[2] = ((PTGEMSTXT)pMods)->shieldmod;
		nNumGemProperties = arrsize(aptGemProperties);
#line 313
		D2ASSERT(nType < nNumGemProperties);
		ptProperty = &aptGemProperties[nType][nIndex];
#line 316
		D2ASSERT(ptProperty);
		nStatValue = ITEMSModsRollAbs(hItem, ptProperty->min, ptProperty->max);
		break;
	case 3:
		ptProperty = &((PTUNIQUEITEMSTXT)pMods)->itemmod[nIndex];
#line 274
		D2ASSERT(ptProperty);
		nMin = ptProperty->min;
		nMax = ptProperty->max;
		if (nMin < 0)
			nMin = -nMin;
		if (nMax < 0)
			nMax = -nMax;
		if (nMin == 0 && nMax == 0)
			nRoll = 0;
		else
		{
#line 72
			D2ASSERT(nMin <= nMax);
			nRange = nMax - nMin;
			dwSeed = (DWORD)D2CommonITEMSGetItemSeed(hItem);
			nValue = (nRange != 0 ? (int)D2CommonSeedRand(dwSeed, nRange) : 0) + nMin;
#line 75
			D2ASSERT(nValue <= nMax);
			nRoll = nValue;
		}
		nStatValue = nRoll;
		break;
	case 4:
		ptProperty = &((PTSETITEMSTXT)pMods)->propertymod[nIndex];
#line 263
		D2ASSERT(ptProperty);
		nStatValue = ITEMSModsRollAbs(hItem, ptProperty->min, ptProperty->max);
		break;
	default:
#line 345
		D2ASSERT(0);
		return 0;
	}
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsFindItemByCode(D2CommonITEMSGetTypeCode(hItem), &nItemIndex);
	switch (nStatId)
	{
	case 16:
	case 31:
		D2CommonStatsUnitSetValue(hItem, 31, ptItemStats->maxac + 1);
		break;
	case 17:
	case 22:
		D2CommonStatsUnitSetValue(hItem, 22, ptItemStats->maxdam + 1);
		break;
	case 18:
	case 21:
		D2CommonStatsUnitSetValue(hItem, 21, ptItemStats->mindam + 1);
		break;
	}
	if (nStatValue == 0)
		return 0;
	ITEMSModsScaleValue(nStatId, &nStatValue);
	if (hOwner != NULL)
		hStatsList = (PTSTATLIST)D2CommonStatsFindListByMask(hOwner, 4096);
	else
		hStatsList = (PTSTATLIST)D2CommonStatsFindListByMask(hItem, 64);
#line 363
	D2ASSERT(hStatsList);
	if (fRemove)
		nStatValue = -(int)D2CommonStatsListGetValueA(hStatsList, nStatId);
	D2CommonStatsListAddValue(hStatsList, nStatId, nStatValue);
	return 1;
}

static int __fastcall ITEMSModsRollAbs(HUNIT hItem, int nMin, int nMax)
{
	int nRange;
	DWORD dwSeed;
	int nValue;

	if (nMin < 0)
		nMin = -nMin;
	if (nMax < 0)
		nMax = -nMax;
	if (nMin == 0 && nMax == 0)
		return 0;
#line 72
	D2ASSERT(nMin <= nMax);
	nRange = nMax - nMin;
	dwSeed = (DWORD)D2CommonITEMSGetItemSeed(hItem);
	nValue = (nRange != 0 ? (int)D2CommonSeedRand(dwSeed, nRange) : 0) + nMin;
#line 75
	D2ASSERT(nValue <= nMax);
	return nValue;
}

static int __fastcall ITEMSModsApplyResists(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods, int nIndex,
	DWORD nType, int nStatId, BOOL fRemove)
{
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 39, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 41, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 43, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 45, fRemove);
	return nStatId;
}

static int __fastcall ITEMSModsApplyMaxResists(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods, int nIndex,
	DWORD nType, int nStatId, BOOL fRemove)
{
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 40, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 42, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 44, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 46, fRemove);
	return nStatId;
}

static int __fastcall ITEMSModsApplyDamagePercents(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods,
	int nIndex,
	DWORD nType, int nStatId, BOOL fRemove)
{
	ITEMSModsApplyStatAbs(eMods, hOwner, hItem, pMods, nIndex, nType, 17, fRemove);
	ITEMSModsApplyStatAbs(eMods, hOwner, hItem, pMods, nIndex, nType, 18, fRemove);
	return nStatId;
}

static int __fastcall ITEMSModsApplyClassSkillPoints(int eMods, HUNIT hOwner, HUNIT hItem, void *pMods,
	int nIndex,
	DWORD nType, int nStatId, BOOL fRemove)
{
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 83, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 84, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 85, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 86, fRemove);
	ITEMSModsApplyStat(eMods, hOwner, hItem, pMods, nIndex, nType, 87, fRemove);
	return nStatId;
}

static ITEMMODCALL sgtItemModCall[] =
{
	{ ITEMSModsApplyStat, 31 }, { ITEMSModsApplyStat, 32 }, { ITEMSModsApplyStat, 33 },
	{ ITEMSModsApplyStat, 34 }, { ITEMSModsApplyStat, 36 }, { ITEMSModsApplyStatAbs, 16 },
	{ ITEMSModsApplyStat, 35 }, { ITEMSModsApplyStat, 0 }, { ITEMSModsApplyStat, 2 },
	{ ITEMSModsApplyStat, 3 }, { ITEMSModsApplyStat, 1 }, { ITEMSModsApplyStat, 9 },
	{ ITEMSModsApplyStatAbs, 77 }, { ITEMSModsApplyStat, 7 }, { ITEMSModsApplyStatAbs, 76 },
	{ ITEMSModsApplyStat, 19 }, { ITEMSModsApplyStat, 20 }, { ITEMSModsApplyStat, 54 },
	{ ITEMSModsApplyStat, 55 }, { ITEMSModsApplyStat, 56 }, { ITEMSModsApplyStat, 48 },
	{ ITEMSModsApplyStat, 49 }, { ITEMSModsApplyStat, 50 }, { ITEMSModsApplyStat, 51 },
	{ ITEMSModsApplyStat, 57 }, { ITEMSModsApplyStat, 58 }, { ITEMSModsApplyStat, 59 },
	{ ITEMSModsApplyStat, 21 }, { ITEMSModsApplyStat, 22 }, { ITEMSModsApplyDamagePercents, -1 },
	{ ITEMSModsApplyStatAbs, 114 }, { ITEMSModsApplyStat, 39 }, { ITEMSModsApplyStat, 40 },
	{ ITEMSModsApplyStat, 41 }, { ITEMSModsApplyStat, 42 }, { ITEMSModsApplyStat, 43 },
	{ ITEMSModsApplyStat, 44 }, { ITEMSModsApplyStat, 37 }, { ITEMSModsApplyStat, 38 },
	{ ITEMSModsApplyStat, 45 }, { ITEMSModsApplyStat, 46 }, { ITEMSModsApplyResists, -1 },
	{ ITEMSModsApplyMaxResists, -1 }, { ITEMSModsApplyStat, 142 }, { ITEMSModsApplyStat, 143 },
	{ ITEMSModsApplyStat, 144 }, { ITEMSModsApplyStat, 145 }, { ITEMSModsApplyStat, 146 },
	{ ITEMSModsApplyStat, 147 }, { ITEMSModsApplyStat, 148 }, { ITEMSModsApplyStat, 149 },
	{ ITEMSModsApplyStat, 73 }, { ITEMSModsApplyStatAbs, 75 }, { ITEMSModsApplyStat, 74 },
	{ ITEMSModsApplyStat, 78 }, { ITEMSModsApplyStat, 92 }, { ITEMSModsApplyStat, 93 },
	{ ITEMSModsApplyStat, 94 }, { ITEMSModsApplyStat, 79 }, { ITEMSModsApplyStat, 80 },
	{ ITEMSModsApplyStat, 81 }, { ITEMSModsApplyStatAbs, 28 }, { ITEMSModsApplyStatAbs, 27 },
	{ ITEMSModsApplyStat, 11 }, { ITEMSModsApplyStat, 82 }, { ITEMSModsApplyStat, 62 },
	{ ITEMSModsApplyStat, 60 }, { ITEMSModsApplyStat, 83 }, { ITEMSModsApplyStat, 84 },
	{ ITEMSModsApplyStat, 85 }, { ITEMSModsApplyStat, 86 }, { ITEMSModsApplyStat, 87 },
	{ ITEMSModsApplyStat, 88 }, { ITEMSModsApplyStat, 89 }, { ITEMSModsApplyStat, 90 },
	{ ITEMSModsApplyStat, 91 }, { ITEMSModsApplyStat, 95 }, { ITEMSModsApplyStat, 96 },
	{ ITEMSModsApplyStat, 97 }, { ITEMSModsApplyStat, 98 }, { ITEMSModsApplyStat, 99 },
	{ ITEMSModsApplyStat, 100 }, { ITEMSModsApplyStat, 101 }, { ITEMSModsApplyStat, 102 },
	{ ITEMSModsApplyStat, 103 }, { ITEMSModsApplyStat, 104 }, { ITEMSModsApplyStat, 105 },
	{ ITEMSModsApplyStat, 106 }, { ITEMSModsApplyStat, 110 }, { ITEMSModsApplyStat, 111 },
	{ ITEMSModsApplyStat, 112 }, { ITEMSModsApplyStat, 113 }, { ITEMSModsApplyStat, 115 },
	{ ITEMSModsApplyStat, 116 }, { ITEMSModsApplyStat, 117 }, { ITEMSModsApplyStat, 118 },
	{ ITEMSModsApplyStatAbs, 119 }, { ITEMSModsApplyStat, 120 }, { ITEMSModsApplyStatAbs, 121 },
	{ ITEMSModsApplyStatAbs, 122 }, { ITEMSModsApplyStat, 123 }, { ITEMSModsApplyStat, 124 },
	{ ITEMSModsApplyStat, 125 }, { ITEMSModsApplyStat, 126 }, { ITEMSModsApplyClassSkillPoints, 127 },
	{ ITEMSModsApplyStat, 128 }, { ITEMSModsApplyStat, 134 }, { ITEMSModsApplyStat, 135 },
	{ ITEMSModsApplyStat, 136 }, { ITEMSModsApplyStat, 137 }, { ITEMSModsApplyStat, 138 },
	{ ITEMSModsApplyStat, 139 }, { ITEMSModsApplyStat, 140 }, { ITEMSModsApplyStat, 141 },
	{ ITEMSModsApplyStat, 150 }, { ITEMSModsApplyStat, 153 }, { ITEMSModsApplyStat, 154 },
	{ ITEMSModsApplyStat, 155 }, { ITEMSModsApplyStat, 156 }, { ITEMSModsApplyStat, 157 },
	{ ITEMSModsApplyStat, 158 },
};
static int sgnNumItemMods = arrsize(sgtItemModCall);

void __stdcall ITEMSModsApply(int eMods, HUNIT hItem, void *pMods, DWORD nType, BOOL fRemove)
{
	PTMAGICAFFIXTXT ptMagicMods;
	PTQUALITYITEMSTXT ptHiqualityMods;
	PTGEMSTXT ptGemMods;
	PTUNIQUEITEMSTXT ptUniqueMods;
	PTSETITEMSTXT ptSetMods;
	PTITEMMODTXT aptGemProperties[3];
	PTITEMMODTXT ptProperty;
	DWORD i;
	DWORD nFirst;
	DWORD nLast;

	switch (eMods)
	{
	case 0:
		ptMagicMods = (PTMAGICAFFIXTXT)pMods;
#line 567
		D2ASSERT(ptMagicMods);
		for (i = 0; i < ptMagicMods->nummods; i++)
		{
			ptProperty = &ptMagicMods->mod[i];
#line 570
			D2ASSERT(ptProperty);
			if (ptProperty->nModOffset == -1)
				continue;
#line 571
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtItemModCall[ptProperty->nModOffset].pfnFunction),
				"sgtItemModCall[ptProperty->nModOffset].pfnFunction");
			sgtItemModCall[ptProperty->nModOffset].pfnFunction(eMods, NULL, hItem, ptMagicMods, (int)i, 0,
				sgtItemModCall[ptProperty->nModOffset].nStatId, fRemove);
		}
		break;
	case 1:
		ptHiqualityMods = (PTQUALITYITEMSTXT)pMods;
#line 580
		D2ASSERT(ptHiqualityMods);
		for (i = 0; i < ptHiqualityMods->nummods; i++)
		{
			ptProperty = &ptHiqualityMods->mod[i];
#line 583
			D2ASSERT(ptProperty);
			if (ptProperty->nModOffset == -1)
				continue;
#line 584
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtItemModCall[ptProperty->nModOffset].pfnFunction),
				"sgtItemModCall[ptProperty->nModOffset].pfnFunction");
			sgtItemModCall[ptProperty->nModOffset].pfnFunction(eMods, NULL, hItem, ptHiqualityMods, (int)i, 0,
				sgtItemModCall[ptProperty->nModOffset].nStatId, fRemove);
		}
		break;
	case 2:
		ptGemMods = (PTGEMSTXT)pMods;
#line 593
		D2ASSERT(ptGemMods);
		aptGemProperties[0] = ptGemMods->weaponmod;
		aptGemProperties[1] = ptGemMods->helmmod;
		aptGemProperties[2] = ptGemMods->shieldmod;
		for (i = 0; i < ptGemMods->nummods; i++)
		{
			ptProperty = &aptGemProperties[nType][i];
#line 601
			D2ASSERT(ptProperty);
			if (ptProperty->nModOffset == -1)
				continue;
#line 602
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtItemModCall[ptProperty->nModOffset].pfnFunction),
				"sgtItemModCall[ptProperty->nModOffset].pfnFunction");
			sgtItemModCall[ptProperty->nModOffset].pfnFunction(eMods, NULL, hItem, ptGemMods, (int)i, nType,
				sgtItemModCall[ptProperty->nModOffset].nStatId, fRemove);
		}
		break;
	case 3:
		ptUniqueMods = (PTUNIQUEITEMSTXT)pMods;
#line 611
		D2ASSERT(ptUniqueMods);
		for (i = 0; i < ptUniqueMods->nummods; i++)
		{
			ptProperty = &ptUniqueMods->itemmod[i];
#line 614
			D2ASSERT(ptProperty);
			if (ptProperty->nModOffset == -1)
				continue;
#line 615
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtItemModCall[ptProperty->nModOffset].pfnFunction),
				"sgtItemModCall[ptProperty->nModOffset].pfnFunction");
			sgtItemModCall[ptProperty->nModOffset].pfnFunction(eMods, NULL, hItem, ptUniqueMods, (int)i, 0,
				sgtItemModCall[ptProperty->nModOffset].nStatId, fRemove);
		}
		break;
	case 4:
		ptSetMods = (PTSETITEMSTXT)pMods;
#line 624
		D2ASSERT(ptSetMods);
		nFirst = nType * 2;
		nLast = nFirst + 1;
		for (i = nFirst; i <= nLast; i++)
		{
			ptProperty = &ptSetMods->propertymod[i];
#line 631
			D2ASSERT(ptProperty);
			if (ptProperty->nModOffset == -1)
				continue;
#line 632
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtItemModCall[ptProperty->nModOffset].pfnFunction),
				"sgtItemModCall[ptProperty->nModOffset].pfnFunction");
			sgtItemModCall[ptProperty->nModOffset].pfnFunction(eMods, NULL, hItem, ptSetMods, (int)i, 0,
				sgtItemModCall[ptProperty->nModOffset].nStatId, fRemove);
		}
		break;
	default:
#line 640
		D2ASSERT(0);
	}
}

void __stdcall ITEMSModsApplySetProperties(HUNIT hOwner, HUNIT hItem, PTSETITEMSTXT ptSetMods)
{
	PTITEMMODTXT ptProperty;
	DWORD i;
	DWORD nCount;

	if (D2CommonITEMSDataGetModType(hItem) != 5)
		return;
#line 651
	D2ASSERT(ptSetMods);
	nCount = ptSetMods->numproperties;
	i = ptSetMods->numitems * 2;
	for (; i < nCount; i++)
	{
		ptProperty = &ptSetMods->propertymod[i];
#line 658
		D2ASSERT(ptProperty);
		if (ptProperty->nModOffset == -1)
			continue;
#line 659
		D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtItemModCall[ptProperty->nModOffset].pfnFunction),
			"sgtItemModCall[ptProperty->nModOffset].pfnFunction");
		sgtItemModCall[ptProperty->nModOffset].pfnFunction(4, hOwner, hItem, ptSetMods, (int)i, 0,
			sgtItemModCall[ptProperty->nModOffset].nStatId, FALSE);
	}
}

int __stdcall ITEMSModsGetStatId(int nOffset)
{
#line 672
	D2ASSERT(nOffset < sgnNumItemMods);
#line 673
	D2ASSERT(nOffset != -1);
	return sgtItemModCall[nOffset].nStatId;
}

struct ITEMMODSREC1
{
	BYTE unobserved_0[64];
	BYTE ab[12];
};
typedef struct ITEMMODSREC1 ITEMMODSREC1, *PTITEMMODSREC1;

struct ITEMMODSREC2
{
	BYTE ab[16];
};
typedef struct ITEMMODSREC2 ITEMMODSREC2, *PTITEMMODSREC2;

struct ITEMMODSREC3
{
	BYTE ab[10];
};
typedef struct ITEMMODSREC3 ITEMMODSREC3, *PTITEMMODSREC3;

BOOL __stdcall ITEMSModsTestMagicFits(DWORD dwItem, PTITEMMODSREC1 ptRec)
{
	struct ITEMMODSENTRY atTable[11];
	DWORD dwId;
	DWORD i;

	atTable[0].bFlag = ptRec->ab[65 - 64]; atTable[0].dwId = 2;
	atTable[1].bFlag = ptRec->ab[67 - 64]; atTable[1].dwId = 24;
	atTable[2].bFlag = ptRec->ab[68 - 64]; atTable[2].dwId = 25;
	atTable[3].bFlag = ptRec->ab[69 - 64]; atTable[3].dwId = 26;
	atTable[4].bFlag = ptRec->ab[70 - 64]; atTable[4].dwId = 35;
	atTable[5].bFlag = ptRec->ab[70 - 64]; atTable[5].dwId = 27;
	atTable[6].bFlag = ptRec->ab[71 - 64]; atTable[6].dwId = 15;
	atTable[7].bFlag = ptRec->ab[72 - 64]; atTable[7].dwId = 16;
	atTable[8].bFlag = ptRec->ab[73 - 64]; atTable[8].dwId = 19;
	atTable[9].bFlag = ptRec->ab[74 - 64]; atTable[9].dwId = 10;
	atTable[10].bFlag = ptRec->ab[75 - 64]; atTable[10].dwId = 12;

	if (D2CommonITEMSGetTypeStackable(dwItem))
		return FALSE;
	if (D2CommonITEMSGetTypeThrowable(dwItem))
		return FALSE;
	dwId = D2CommonITEMSGetType(dwItem);
	if (ptRec->ab[66 - 64] != 0 && D2CommonITEMSIsWeapon(dwItem) &&
		dwId != 26 && dwId != 27 && dwId != 35 && dwId != 24 && dwId != 25)
		return TRUE;
	if (ptRec->ab[64 - 64] != 0 && D2CommonITEMSIsArmor(dwItem) &&
		dwId != 2 && dwId != 15 && dwId != 16 && dwId != 19)
		return TRUE;
	for (i = 0; i < 11; i++)
		if (dwId == atTable[i].dwId && atTable[i].bFlag != 0)
			return TRUE;
	return FALSE;
}

BOOL __stdcall ITEMSModsTestRareFits(DWORD dwItem, PTITEMMODSREC2 ptRec)
{
	struct ITEMMODSENTRY atTable[20];
	DWORD dwId;
	DWORD i;

	atTable[0].bFlag = ptRec->ab[1]; atTable[0].dwId = 37;
	atTable[1].bFlag = ptRec->ab[2]; atTable[1].dwId = 2;
	atTable[2].bFlag = ptRec->ab[3]; atTable[2].dwId = 32;
	atTable[3].bFlag = ptRec->ab[3]; atTable[3].dwId = 30;
	atTable[4].bFlag = ptRec->ab[4]; atTable[4].dwId = 28;
	atTable[5].bFlag = ptRec->ab[5]; atTable[5].dwId = 29;
	atTable[6].bFlag = ptRec->ab[5]; atTable[6].dwId = 36;
	atTable[7].bFlag = ptRec->ab[5]; atTable[7].dwId = 31;
	atTable[8].bFlag = ptRec->ab[6]; atTable[8].dwId = 34;
	atTable[9].bFlag = ptRec->ab[6]; atTable[9].dwId = 33;
	atTable[10].bFlag = ptRec->ab[7]; atTable[10].dwId = 24;
	atTable[11].bFlag = ptRec->ab[8]; atTable[11].dwId = 25;
	atTable[12].bFlag = ptRec->ab[9]; atTable[12].dwId = 26;
	atTable[13].bFlag = ptRec->ab[10]; atTable[13].dwId = 35;
	atTable[14].bFlag = ptRec->ab[10]; atTable[14].dwId = 27;
	atTable[15].bFlag = ptRec->ab[11]; atTable[15].dwId = 15;
	atTable[16].bFlag = ptRec->ab[12]; atTable[16].dwId = 16;
	atTable[17].bFlag = ptRec->ab[13]; atTable[17].dwId = 19;
	atTable[18].bFlag = ptRec->ab[14]; atTable[18].dwId = 10;
	atTable[19].bFlag = ptRec->ab[15]; atTable[19].dwId = 12;

	if (D2CommonITEMSGetTypeStackable(dwItem))
		return FALSE;
	if (D2CommonITEMSGetTypeThrowable(dwItem))
		return FALSE;
	dwId = D2CommonITEMSGetType(dwItem);
	if (ptRec->ab[0] != 0 && D2CommonITEMSIsArmor(dwItem) &&
		dwId != 2 && dwId != 15 && dwId != 16 && dwId != 19 && dwId != 37)
		return TRUE;
	for (i = 0; i < 20; i++)
		if (dwId == atTable[i].dwId && atTable[i].bFlag != 0)
			return TRUE;
	return FALSE;
}

BOOL __stdcall ITEMSModsTestQualityFits(DWORD dwItem, PTITEMMODSREC3 ptRec)
{
	struct ITEMMODSENTRY atTable[9];
	DWORD dwId;
	DWORD i;

	atTable[0].bFlag = ptRec->ab[2]; atTable[0].dwId = 2;
	atTable[1].bFlag = ptRec->ab[3]; atTable[1].dwId = 24;
	atTable[2].bFlag = ptRec->ab[4]; atTable[2].dwId = 25;
	atTable[3].bFlag = ptRec->ab[5]; atTable[3].dwId = 26;
	atTable[4].bFlag = ptRec->ab[6]; atTable[4].dwId = 27;
	atTable[5].bFlag = ptRec->ab[6]; atTable[5].dwId = 35;
	atTable[6].bFlag = ptRec->ab[7]; atTable[6].dwId = 15;
	atTable[7].bFlag = ptRec->ab[8]; atTable[7].dwId = 16;
	atTable[8].bFlag = ptRec->ab[9]; atTable[8].dwId = 19;

	dwId = D2CommonITEMSGetType(dwItem);
	if (ptRec->ab[1] != 0 && D2CommonITEMSIsWeapon(dwItem) &&
		dwId != 26 && dwId != 27 && dwId != 35 && dwId != 24 && dwId != 25)
		return TRUE;
	if (ptRec->ab[0] != 0 && D2CommonITEMSIsArmor(dwItem) &&
		dwId != 2 && dwId != 15 && dwId != 16 && dwId != 19)
		return TRUE;
	for (i = 0; i < 9; i++)
		if (dwId == atTable[i].dwId && atTable[i].bFlag != 0)
			return TRUE;
	return FALSE;
}

DWORD __fastcall SeedNext(D2SEED *ptSeed);
DWORD __fastcall SeedRand(D2SEED *ptSeed, DWORD nMax);

static DWORD sgcadwItemField24Values[] = { 0, 1, 2, 3, 9, 7 };
static DWORD sgcnItemField24Values = arrsize(sgcadwItemField24Values);

void __stdcall ITEMSRollElixirKind(HUNIT hItem)
{
	DWORD nIndex;
	int nValue;

	nIndex = D2CommonSeedRand(D2CommonITEMSGetItemSeed(hItem), sgcnItemField24Values);
	D2CommonITEMSDataSetUniqueOrSetRow(hItem, sgcadwItemField24Values[nIndex]);
	if (sgcadwItemField24Values[nIndex] == 9)
		nValue = ((SeedNext((D2SEED *)D2CommonITEMSGetItemSeed(hItem)) & 3) + 1) << 8;
	else if (sgcadwItemField24Values[nIndex] == 7)
		nValue = ((SeedNext((D2SEED *)D2CommonITEMSGetItemSeed(hItem)) & 3) + 1) << 8;
	else
		nValue = 1;
	D2CommonStatsUnitSetValue(hItem, 71, nValue);
}

static void __fastcall ITEMSAddRandomSkillStats(HUNIT hItem, int nLevel, int nBase, int nUnused)
{
	int nCount;
	int nTier;
	int nSkillTier;
	int nFirst;
	int nSkill;
	int nSkillLevel;
	int anUsed[3];
	DWORD dwList;
	int nRand;
	int i;

	nRand = (int)D2CommonSeedRand(D2CommonITEMSGetItemSeed(hItem), 100);
	if (nRand > 90)
		nCount = 3;
	else if (nRand > 70)
		nCount = 2;
	else if (nRand > 30)
		nCount = 1;
	else
		return;
	nTier = 1;
	if (nLevel > 24)
		nTier = 4;
	else if (nLevel > 18)
		nTier = 3;
	else if (nLevel > 11)
		nTier = 2;
	anUsed[0] = -1;
	anUsed[1] = -1;
	anUsed[2] = -1;
	for (i = 0; i < nCount; i++)
	{
		nRand = (int)D2CommonSeedRand(D2CommonITEMSGetItemSeed(hItem), 100);
		if (nRand > 80)
			nSkillTier = nTier + 1;
		else if (nRand > 30)
			nSkillTier = nTier;
		else if (nRand > 10)
			nSkillTier = nTier - 1;
		else
			nSkillTier = nTier - 2;
		if (nSkillTier < 1)
			nSkillTier = 1;
		nFirst = nBase + nSkillTier * 5 - 5;
		do
		{
			nSkill = (int)SeedRand((D2SEED *)D2CommonITEMSGetItemSeed(hItem), 5) + nFirst;
		}
		while (nSkill == 73 || nSkill == anUsed[0] || nSkill == anUsed[1] || nSkill == anUsed[2]);
		anUsed[i] = nSkill;
		nSkillLevel = 1;
		nRand = (int)SeedRand((D2SEED *)D2CommonITEMSGetItemSeed(hItem), 100);
		if (nRand >= 90)
			nSkillLevel = 3;
		else if (nRand >= 60)
			nSkillLevel = 2;
		dwList = D2CommonStatsFindListByMask(hItem, 64);
		D2CommonStatsListSetValueB(dwList, i + 107,
			(nSkillLevel << 16) | nSkill);
	}
}

void __stdcall ITEMSAddRandomSkillStatsByType(HUNIT hItem, int nLevel)
{
	switch (D2CommonITEMSGetType(hItem))
	{
	case 24:
		ITEMSAddRandomSkillStats(hItem, nLevel, 96, 30);
		break;
	case 25:
		ITEMSAddRandomSkillStats(hItem, nLevel, 66, 30);
		break;
	case 26:
		ITEMSAddRandomSkillStats(hItem, nLevel, 36, 30);
		break;
	}
}
