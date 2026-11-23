#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\ITEMS\\Items.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "../DATATBLS/DataTbls.h"

struct D2SEED * __stdcall ITEMSGetItemSeed(HUNIT hItem);
void __stdcall ITEMSDataSetUniqueOrSetRow(HUNIT hItem, DWORD dwValue);

void __stdcall ITEMSCreateItemData(DWORD dwPool, HUNIT hItem)
{
	PTITEMDATA ptItemData;
	int i;

#line 370
	D2ASSERT(hItem);
#line 371
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
#line 372
	ptItemData = (PTITEMDATA)D2ALLOCPOOL(dwPool, sizeof(ITEMDATA));
#line 373
	D2ASSERT(ptItemData);
	for (i = 0; i < (int)(sizeof(ITEMDATA) / sizeof(DWORD)); i++)
		((DWORD *)ptItemData)[i] = 0;
	ptItemData->dwOwnerId = (DWORD)-1;
	hItem->uTypeData = (DWORD)ptItemData;
}

void __stdcall ITEMSFreeItemData(DWORD dwPool, HUNIT hItem)
{
#line 384
	D2ASSERT(hItem);
#line 385
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
#line 387
	D2FREEPOOL(dwPool, hItem->uTypeData);
}

BYTE __stdcall ITEMSGetCategory(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 395
	D2ASSERT(hItem);
#line 396
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 398
	D2ASSERT(ptItemData);
	return ptItemData->bCategory;
}

void __stdcall ITEMSSetCategory(HUNIT hItem, BYTE bCategory)
{
	PTITEMDATA ptItemData;

#line 407
	D2ASSERT(hItem);
#line 408
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 410
	D2ASSERT(ptItemData);
	ptItemData->bCategory = bCategory;
}

struct D2SEED * __stdcall ITEMSGetItemSeed(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 419
	D2ASSERT(hItem);
#line 420
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 422
	D2ASSERT(ptItemData);
	return &ptItemData->tSeed;
}

void __stdcall ITEMSAdvanceItemSeed(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 431
	D2ASSERT(hItem);
#line 432
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 434
	D2ASSERT(ptItemData);
	D2CommonSeedInit(&ptItemData->tSeed);
}

DWORD __stdcall ITEMSGetItemSeedValue(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 443
	D2ASSERT(hItem);
#line 444
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 446
	D2ASSERT(ptItemData);
	return ptItemData->dwSeedValue;
}

void __stdcall ITEMSSetItemSeedValue(HUNIT hItem, DWORD dwSeedValue)
{
	PTITEMDATA ptItemData;

#line 455
	D2ASSERT(hItem);
#line 456
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 458
	D2ASSERT(ptItemData);
	ptItemData->dwSeedValue = dwSeedValue;
}

DWORD __stdcall ITEMSDataGetModType(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 467
	D2ASSERT(hItem);
#line 468
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 470
	D2ASSERT(ptItemData);
	return ptItemData->dwModType;
}

void __stdcall ITEMSDataSetModType(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 479
	D2ASSERT(hItem);
#line 480
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 482
	D2ASSERT(ptItemData);
	ptItemData->dwModType = dwValue;
}

DWORD __stdcall ITEMSDataGetMagicPrefixAt(HUNIT hItem, int nIndex)
{
	PTITEMDATA ptItemData;

#line 491
	D2ASSERT(hItem);
#line 492
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 494
	D2ASSERT(ptItemData);
	return ptItemData->adwMagicPrefixes[nIndex];
}

void __stdcall ITEMSDataSetMagicPrefixAt(HUNIT hItem, DWORD dwValue, int nIndex)
{
	PTITEMDATA ptItemData;

#line 503
	D2ASSERT(hItem);
#line 504
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 506
	D2ASSERT(ptItemData);
	ptItemData->adwMagicPrefixes[nIndex] = dwValue;
}

DWORD __stdcall ITEMSDataGetMagicSuffixAt(HUNIT hItem, int nIndex)
{
	PTITEMDATA ptItemData;

#line 515
	D2ASSERT(hItem);
#line 516
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 518
	D2ASSERT(ptItemData);
	return ptItemData->adwMagicSuffixes[nIndex];
}

void __stdcall ITEMSDataSetMagicSuffixAt(HUNIT hItem, DWORD dwValue, int nIndex)
{
	PTITEMDATA ptItemData;

#line 527
	D2ASSERT(hItem);
#line 528
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 530
	D2ASSERT(ptItemData);
	ptItemData->adwMagicSuffixes[nIndex] = dwValue;
}

DWORD __stdcall ITEMSDataGetRarePrefix(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 539
	D2ASSERT(hItem);
#line 540
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 542
	D2ASSERT(ptItemData);
	return ptItemData->dwRarePrefix;
}

void __stdcall ITEMSDataSetRarePrefix(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 551
	D2ASSERT(hItem);
#line 552
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 554
	D2ASSERT(ptItemData);
	ptItemData->dwRarePrefix = dwValue;
}

DWORD __stdcall ITEMSDataGetRareSuffix(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 563
	D2ASSERT(hItem);
#line 564
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 566
	D2ASSERT(ptItemData);
	return ptItemData->dwRareSuffix;
}

void __stdcall ITEMSDataSetRareSuffix(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 575
	D2ASSERT(hItem);
#line 576
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 578
	D2ASSERT(ptItemData);
	ptItemData->dwRareSuffix = dwValue;
}

DWORD __stdcall ITEMSDataTestFlags(HUNIT hItem, DWORD dwMask)
{
	PTITEMDATA ptItemData;

#line 587
	D2ASSERT(hItem);
#line 588
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 590
	D2ASSERT(ptItemData);
	return ptItemData->dwFlags & dwMask;
}

void __stdcall ITEMSDataSetFlags(HUNIT hItem, DWORD dwMask, DWORD fSet)
{
	PTITEMDATA ptItemData;

#line 599
	D2ASSERT(hItem);
#line 600
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 602
	D2ASSERT(ptItemData);
	if (fSet)
		ptItemData->dwFlags |= dwMask;
	else
		ptItemData->dwFlags &= ~dwMask;
}

DWORD __stdcall ITEMSDataGetFlags(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 611
	D2ASSERT(hItem);
#line 612
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 614
	D2ASSERT(ptItemData);
	return ptItemData->dwFlags;
}

DWORD __stdcall ITEMSDataTestCmdFlags(HUNIT hItem, DWORD dwMask)
{
	PTITEMDATA ptItemData;

#line 623
	D2ASSERT(hItem);
#line 624
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 626
	D2ASSERT(ptItemData);
	return ptItemData->dwCmdFlags & dwMask;
}

void __stdcall ITEMSDataSetCmdFlags(HUNIT hItem, DWORD dwMask, DWORD fSet)
{
	PTITEMDATA ptItemData;

#line 635
	D2ASSERT(hItem);
#line 636
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 638
	D2ASSERT(ptItemData);
	if (fSet)
		ptItemData->dwCmdFlags |= dwMask;
	else
		ptItemData->dwCmdFlags &= ~dwMask;
}

DWORD __stdcall ITEMSDataGetCmdFlags(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 647
	D2ASSERT(hItem);
#line 648
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 650
	D2ASSERT(ptItemData);
	return ptItemData->dwCmdFlags;
}

void __stdcall ITEMSDataSetMagicPrefix(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 659
	D2ASSERT(hItem);
#line 660
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 662
	D2ASSERT(ptItemData);
	if (dwValue == 65535)
		dwValue = 0xFFFFFFFF;
	ptItemData->dwMagicPrefix = dwValue;
}

DWORD __stdcall ITEMSDataGetMagicPrefix(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 674
	D2ASSERT(hItem);
#line 675
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 677
	D2ASSERT(ptItemData);
	return ptItemData->dwMagicPrefix;
}

void __stdcall ITEMSDataSetMagicSuffix(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 686
	D2ASSERT(hItem);
#line 687
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 689
	D2ASSERT(ptItemData);
	if (dwValue == 65535)
		dwValue = 0xFFFFFFFF;
	ptItemData->dwMagicSuffix = dwValue;
}

DWORD __stdcall ITEMSDataGetMagicSuffix(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 701
	D2ASSERT(hItem);
#line 702
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 704
	D2ASSERT(ptItemData);
	return ptItemData->dwMagicSuffix;
}

DWORD __stdcall ITEMSGetLevelId(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 713
	D2ASSERT(hItem);
#line 714
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 716
	D2ASSERT(ptItemData);
#line 717
	D2ASSERT(ptItemData->nLevelId != 0);
	return ptItemData->nLevelId;
}

void __stdcall ITEMSSetLevelId(HUNIT hItem, DWORD nLevelId)
{
	PTITEMDATA ptItemData;

#line 726
	D2ASSERT(hItem);
#line 727
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 729
	D2ASSERT(ptItemData);
#line 730
	D2ASSERT(nLevelId != 0);
	ptItemData->nLevelId = nLevelId;
}

BYTE __stdcall ITEMSDataGetPage(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 739
	D2ASSERT(hItem);
#line 740
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 742
	D2ASSERT(ptItemData);
	return ptItemData->bPage;
}

void __stdcall ITEMSDataSetPage(HUNIT hItem, BYTE bValue)
{
	PTITEMDATA ptItemData;

#line 751
	D2ASSERT(hItem);
#line 752
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 754
	D2ASSERT(ptItemData);
	ptItemData->bPage = bValue;
}

BYTE __stdcall ITEMSDataGetSwapPage(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 763
	D2ASSERT(hItem);
#line 764
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 766
	D2ASSERT(ptItemData);
	return ptItemData->bSwapPage;
}

void __stdcall ITEMSDataSetSwapPage(HUNIT hItem, BYTE bValue)
{
	PTITEMDATA ptItemData;

#line 775
	D2ASSERT(hItem);
#line 776
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 778
	D2ASSERT(ptItemData);
	ptItemData->bSwapPage = bValue;
}

BYTE __stdcall ITEMSDataGetPreviousPage(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 788
	D2ASSERT(hItem);
#line 789
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 791
	D2ASSERT(ptItemData);
	return ptItemData->bPreviousPage;
}

void __stdcall ITEMSDataSetPreviousPage(HUNIT hItem, BYTE bValue)
{
	PTITEMDATA ptItemData;

#line 800
	D2ASSERT(hItem);
#line 801
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 803
	D2ASSERT(ptItemData);
	ptItemData->bPreviousPage = bValue;
}

char * __stdcall ITEMSGetNameBuffer(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 812
	D2ASSERT(hItem);
#line 813
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 815
	D2ASSERT(ptItemData);
	return ptItemData->achBuf;
}

void __stdcall ITEMSSetNameBuffer(HUNIT hItem, const char *pszSrc)
{
	PTITEMDATA ptItemData;

#line 824
	D2ASSERT(hItem);
#line 825
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 827
	D2ASSERT(ptItemData);
	strcpy(ptItemData->achBuf, pszSrc);
}

BYTE __stdcall ITEMSDataGetEarLevel(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 836
	D2ASSERT(hItem);
#line 837
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 839
	D2ASSERT(ptItemData);
	return ptItemData->bEarLevel;
}

void __stdcall ITEMSDataSetEarLevel(HUNIT hItem, BYTE bValue)
{
	PTITEMDATA ptItemData;

#line 848
	D2ASSERT(hItem);
#line 849
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 851
	D2ASSERT(ptItemData);
	ptItemData->bEarLevel = bValue;
}

BYTE __stdcall ITEMSDataGetGfxIndex(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 860
	D2ASSERT(hItem);
#line 861
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 863
	D2ASSERT(ptItemData);
	return ptItemData->bGfxIndex;
}

void __stdcall ITEMSDataSetGfxIndex(HUNIT hItem, BYTE bValue)
{
	PTITEMDATA ptItemData;

#line 872
	D2ASSERT(hItem);
#line 873
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 875
	D2ASSERT(ptItemData);
	ptItemData->bGfxIndex = bValue;
}

struct ITEMMODCOSTREC
{
	DWORD	field_00;
	DWORD	field_04;
};
typedef struct ITEMMODCOSTREC ITEMMODCOSTREC;

static ITEMMODCOSTREC sgtModCostTbl[] =
{
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 1, 8 }, { 1, 8 },
	{ 1, 8 }, { 1, 8 }, { 1, 8 }, { 1, 8 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 2, 100 }, { 0, 0 }, { 2, 100 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 2, 100 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 },
};
static DWORD sgnModCostSize = arrsize(sgtModCostTbl);

DWORD gadwItemsUnread[1] = { 0 };

static DWORD __fastcall ITEMSCostCalcByDefense(HUNIT hItem, PTITEMSTXT ptItemStats, DWORD dwCost, DWORD dwModType);
static DWORD __fastcall ITEMSCostCalcPlain(HUNIT hItem, PTITEMSTXT ptItemStats, DWORD dwCost, DWORD dwModType);

static ITEMTRANSLATEREC sgTranslateTbl[NUM_ITEM_TRANSLATE_ENTRIES] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, NULL },
	{ 2, 0, 1, 1, 0, 1, 1, 1, 0, ITEMSCostCalcByDefense },
	{ 3, 0, 1, 1, 0, 1, 1, 1, 0, ITEMSCostCalcByDefense },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 1, NULL },
	{ 5, 0, 0, 0, 0, 0, 0, 1, 1, NULL },
	{ 6, 0, 0, 0, 0, 0, 0, 1, 1, NULL },
	{ 7, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 8, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 9, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 10, 0, 1, 0, 0, 0, 1, 1, 1, NULL },
	{ 11, 0, 0, 0, 0, 0, 0, 0, 1, NULL },
	{ 12, 0, 1, 0, 0, 0, 1, 1, 1, NULL },
	{ 13, 0, 0, 0, 0, 0, 1, 1, 1, NULL },
	{ 14, 0, 0, 0, 0, 0, 0, 0, 0, NULL },
	{ 15, 0, 1, 1, 0, 1, 1, 1, 0, ITEMSCostCalcByDefense },
	{ 16, 0, 1, 1, 0, 1, 1, 1, 0, ITEMSCostCalcByDefense },
	{ 17, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 18, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 19, 0, 1, 1, 0, 1, 1, 1, 0, ITEMSCostCalcByDefense },
	{ 20, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 21, 0, 0, 0, 0, 0, 0, 0, 1, NULL },
	{ 22, 0, 0, 0, 0, 0, 1, 0, 1, NULL },
	{ 23, 0, 0, 0, 0, 0, 0, 0, 0, NULL },
	{ 24, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 25, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 26, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 27, 0, 1, 0, 1, 0, 1, 1, 0, NULL },
	{ 28, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 29, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 30, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 31, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 32, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 33, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 34, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 35, 0, 1, 0, 1, 0, 1, 1, 0, NULL },
	{ 36, 0, 1, 1, 1, 0, 1, 1, 0, NULL },
	{ 37, 0, 1, 1, 0, 1, 1, 1, 0, ITEMSCostCalcByDefense },
	{ 38, 0, 1, 0, 1, 0, 0, 1, 0, NULL },
	{ 39, 0, 0, 0, 0, 0, 0, 0, 0, NULL },
	{ 40, 0, 0, 0, 0, 0, 1, 0, 1, ITEMSCostCalcPlain },
	{ 41, 0, 0, 0, 0, 0, 0, 0, 1, NULL },
	{ 42, 0, 1, 0, 1, 0, 1, 1, 0, NULL },
	{ 43, 0, 1, 0, 1, 0, 1, 1, 0, NULL },
};
static int sgTranslateTblSize = NUM_ITEM_TRANSLATE_ENTRIES;

static DWORD sgcadwVendorQuests[4] = { 4, 9, 17, 41 };

static DWORD sgcadwItemSkillStats[] = { 107, 108, 109 };
static int sgcnItemSkillStats = arrsize(sgcadwItemSkillStats);

static DWORD sgPotionTierTbl1[5] =
	{ 540110952u, 540176488u, 540242024u, 540307560u, 540373096u };
static int sgnPotionTierCount1 = 5;
static DWORD sgPotionTierTbl2[5] =
	{ 540110957u, 540176493u, 540242029u, 540307565u, 540373101u };
static int sgnPotionTierCount2 = 5;
static DWORD sgPotionTierTbl3[2] =
	{ 543979122u, 544437874u };
static int sgnPotionTierCount3 = 2;

struct ITEMWCLASSREC
{
	DWORD	dwCode;
	WORD	wIndex;
	WORD	unobserved_6;
};
typedef struct ITEMWCLASSREC ITEMWCLASSREC;

static ITEMWCLASSREC sgtWClassTbl[] =
{
	{ 544698210, 1, 0 },
	{ 544434225, 2, 0 },
	{ 544499761, 3, 0 },
	{ 543585395, 4, 0 },
	{ 544434226, 5, 0 },
	{ 544499762, 6, 0 },
	{ 544694904, 7, 0 },
};
static DWORD sgnWClassTblSize = arrsize(sgtWClassTbl);

static DWORD sgItemField00Tbl[4] =
	{ 7u, 5u, 6u, 4u };
static DWORD sgdwItemField00TblSize = 4;

static DWORD sgadwNoStackQualities[] = { 0, 4, 5, 6, 7 };
static DWORD sgdwNoStackQualities = arrsize(sgadwNoStackQualities);

DWORD __stdcall ITEMSGetTranslateTblFirst(int eType)
{
#line 884
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].field_00;
}

DWORD __stdcall ITEMSTestTypeIdentifiable(int eType)
{
#line 893
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwIdentifiable;
}

DWORD __stdcall ITEMSGetTypeThrowableNoCheck(DWORD dwItemClassIndex);
DWORD __stdcall ITEMSGetType(HUNIT hItem); DWORD __stdcall ITEMSGetTypeNoCheck(DWORD dwItemClassIndex);

DWORD __stdcall ITEMSTestRepairable(HUNIT hItem)
{
	int eType;

	if (!ITEMSDataTestFlags(hItem, 16))
		return 0;
	if (ITEMSGetTypeThrowableNoCheck(hItem->dwClassId))
		return 0;
	if (hItem->eType != UNIT_ITEM)
		eType = 1;
	else
		eType = (int)ITEMSGetTypeNoCheck(hItem->dwClassId);
#line 913
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwRepairable;
}

DWORD __stdcall ITEMSGetTypeWClass(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 923
	D2ASSERT(hItem);
#line 924
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 926
	D2ASSERT(ptStats);
	return ptStats->wclass;
}

DWORD __stdcall ITEMSGetTypeTwoHandedWClass(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 935
	D2ASSERT(hItem);
#line 936
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 938
	D2ASSERT(ptStats);
	return ptStats->dwTwoHandedWClass;
}

DWORD __stdcall ITEMSGetTypeCode(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 952
	D2ASSERT(hItem);
#line 953
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 955
	D2ASSERT(ptStats);
	return ptStats->code;
}

DWORD __stdcall ITEMSGetTypeAlternateGfxOrCode(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 964
	D2ASSERT(hItem);
#line 965
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 967
	D2ASSERT(ptStats);
	if (ptStats->alternategfx != 0)
		return ptStats->alternategfx;
	return ptStats->code;
}

BYTE __stdcall ITEMSGetTypeComponent(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 976
	D2ASSERT(hItem);
#line 977
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 979
	D2ASSERT(ptStats);
	return ptStats->component;
}

void __stdcall ITEMSGetTypeInvWidthHeight(HUNIT hItem, BYTE *pbInvWidth, BYTE *pbInvHeight)
{
	PTITEMTYPEDATA ptStats;

#line 990
	D2ASSERT(hItem);
#line 991
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 993
	D2ASSERT(ptStats);
	*pbInvWidth = ptStats->invwidth;
	*pbInvHeight = ptStats->invheight;
}

void __stdcall ITEMSGetTypeBodyLocs(HUNIT hItem, BYTE *pbBodyLoc1, BYTE *pbBodyLoc2)
{
	PTITEMTYPEDATA ptStats;

#line 1007
	D2ASSERT(hItem);
#line 1008
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1010
	D2ASSERT(ptStats);
	if (ptStats != NULL)
	{
		*pbBodyLoc1 = ptStats->bodyloc1;
		*pbBodyLoc2 = ptStats->bodyloc2;
	}
}

DWORD __stdcall ITEMSGetType(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;
	DWORD dwClassId;

#line 1022
	D2ASSERT(hItem);
	if (UnitGetType(hItem) != UNIT_ITEM)
		return 1;
	dwClassId = hItem->dwClassId;
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwClassId);
#line 1035
	D2ASSERT(ptItemStats);
	return ptItemStats->type;
}

DWORD __stdcall ITEMSGetTypeNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1035
	D2ASSERT(ptItemStats);
	return ptItemStats->type;
}

BYTE __stdcall ITEMSGetTypeLevel(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 1044
	D2ASSERT(hItem);
#line 1045
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1047
	D2ASSERT(ptStats);
	return ptStats->level;
}

DWORD __stdcall ITEMSMaskFlags(DWORD dwFlagsA, DWORD dwFlagsB)
{
	return dwFlagsA & dwFlagsB;
}

void __stdcall ITEMSSetOrClearFlags(DWORD *pdwFlags, DWORD dwMask, DWORD fSet)
{
	if (fSet)
		*pdwFlags |= dwMask;
	else
		*pdwFlags &= ~dwMask;
}

DWORD __stdcall ITEMSTestFitsInventorySpace(HUNIT hItem, HUNIT hOwner, int *pnFitsWidth,
	int *pnFitsHeight, int *pnFitsCount)
{
	PTITEMTYPEDATA ptItemStats;
	DWORD dwSocketFlag;
	int nStatVal;
	int nWidthScaled, nHeightScaled;
	int nWidthStatResult, nHeightStatResult, nCountStatResult;
	int nOrd10715Result;
	int fFitsWidth, fFitsHeight, fFitsCount;

#line 1079
	D2ASSERT(hItem);
#line 1080
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	fFitsWidth = 1;
	fFitsHeight = 1;
	fFitsCount = 1;
	dwSocketFlag = ITEMSDataTestFlags(hItem, 16);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1087
	D2ASSERT(ptItemStats);

	nHeightScaled = nWidthScaled = 0;
	nStatVal = (int)D2CommonStatsGetTotalValue(hItem, 91);
	if (nStatVal != 0)
	{
		nWidthScaled = ((int)ptItemStats->reqstr * nStatVal) / 100;
		nHeightScaled = ((int)ptItemStats->reqdex * nStatVal) / 100;
	}

	nWidthStatResult = D2CommonStatsUnitGetValue(hOwner, 0);
	if (nWidthStatResult > 0 && nWidthStatResult < (int)ptItemStats->reqstr + nWidthScaled)
		fFitsWidth = 0;

	nHeightStatResult = D2CommonStatsUnitGetValue(hOwner, 2);
	if (nHeightStatResult > 0 && nHeightStatResult < (int)ptItemStats->reqdex + nHeightScaled)
		fFitsHeight = 0;

	if (dwSocketFlag != 0)
	{
		nOrd10715Result = D2CommonITEMSGetRequiredLevel(hItem);
		if (nOrd10715Result != -1)
		{
			nCountStatResult = D2CommonStatsUnitGetValue(hOwner, 12);
			if (nCountStatResult > 0 && nCountStatResult < nOrd10715Result)
				fFitsCount = 0;
		}
	}

	if (pnFitsWidth != NULL)
		*pnFitsWidth = fFitsWidth;
	if (pnFitsHeight != NULL)
		*pnFitsHeight = fFitsHeight;
	if (pnFitsCount != NULL)
		*pnFitsCount = fFitsCount;

	if (fFitsWidth)
	{
		if (!fFitsHeight)
			return FALSE;
		if (!fFitsCount)
			return FALSE;
		if (dwSocketFlag == 0)
			return FALSE;

		if (UnitGetType(hItem) == UNIT_ITEM)
		{
			if (ITEMSGetType(hItem) == 18)
			{
				if (D2CommonStatsUnitGetValue(hItem, 70) <= 0)
					return FALSE;
			} }
		return TRUE; }
	return FALSE;
}

DWORD __stdcall ITEMSIsArmorNoCheck(DWORD dwItemClassIndex);
DWORD __stdcall ITEMSIsWeaponNoCheck(DWORD dwItemClassIndex);

BOOL __stdcall ITEMSTestUbercodeUnsetNoCheck(DWORD dwItemClassIndex)
{
	PTITEMSTXT ptItemStats;

	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(dwItemClassIndex);
	if (!ITEMSIsArmorNoCheck(dwItemClassIndex)
		&& !ITEMSIsWeaponNoCheck(dwItemClassIndex))
		return FALSE;
	if (ptItemStats->ubercode != 538976304)
		return FALSE;
	if (ITEMSGetTypeNoCheck(dwItemClassIndex) == 38)
		return FALSE;
	return ptItemStats->quest == 0;
}

BOOL __stdcall ITEMSTestUbercodeUnset(HUNIT hItem)
{
#line 1201
	D2ASSERT(hItem);
#line 1202
	D2ASSERT(UnitGetType (hItem) == UNIT_ITEM);
	return ITEMSTestUbercodeUnsetNoCheck(hItem->dwClassId);
}

int __stdcall ITEMSGetRequiredLevel(HUNIT hItem)
{
	int nLevel;
	PTMAGICAFFIXTXT ptPrefix;
	PTMAGICAFFIXTXT ptSuffix;
	PTUNIQUEITEMSTXT ptUniqueMods;
	PTSETITEMSTXT ptSetMods;
	PTSKILLSTXT ptSkill;
	DWORD dwSkill;
	DWORD dwModType;
	int i;

	dwModType = ITEMSDataGetModType(hItem);
	nLevel = -1;
	switch (dwModType)
	{
	case 4:
		ptPrefix = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(ITEMSDataGetMagicPrefixAt(hItem, 0));
		ptSuffix = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(ITEMSDataGetMagicSuffixAt(hItem, 0));
		if (ptPrefix != NULL && nLevel <= ptPrefix->level)
			nLevel = ptPrefix->level;
		if (ptSuffix != NULL && nLevel <= ptSuffix->level)
			nLevel = ptSuffix->level;
		break;
	case 6:
		for (i = 0; i < 3; i++)
		{
			ptPrefix = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(ITEMSDataGetMagicPrefixAt(hItem, i));
			ptSuffix = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(ITEMSDataGetMagicSuffixAt(hItem, i));
			if (ptPrefix != NULL && nLevel <= ptPrefix->level)
				nLevel = ptPrefix->level;
			if (ptSuffix != NULL && nLevel <= ptSuffix->level)
				nLevel = ptSuffix->level;
		}
		break;
	case 7:
		ptUniqueMods = (PTUNIQUEITEMSTXT)D2CommonItemTblsGetUniqueItem(ITEMSDataGetUniqueOrSetRow(hItem));
		if (ptUniqueMods != NULL && nLevel <= ptUniqueMods->level)
			nLevel = ptUniqueMods->level;
		break;
	case 5:
		ptSetMods = (PTSETITEMSTXT)D2CommonItemTblsGetSetItem(ITEMSDataGetUniqueOrSetRow(hItem));
		if (ptSetMods != NULL && nLevel <= ptSetMods->level)
			nLevel = ptSetMods->level;
		break;
	}
	if (nLevel != -1)
	{
		nLevel = nLevel * 3 / 4;
		if (nLevel < 1)
			nLevel = 1;
	}
	for (i = 0; i < sgcnItemSkillStats; i++)
	{
		dwSkill = D2CommonStatsGetTotalValue(hItem, sgcadwItemSkillStats[i]);
		if (dwSkill == 0)
			continue;
		ptSkill = (PTSKILLSTXT)D2CommonDataTblsGetSkill(dwSkill & 65535);
		if ((int)ptSkill->reqlevel > nLevel)
			nLevel = ptSkill->reqlevel;
	}
	if (D2CommonITEMSTestUbercodeUnset(hItem) && nLevel < 25)
		nLevel = 25;
	return nLevel;
}

DWORD __stdcall ITEMSTestTypeBodyLocEqual(HUNIT hItem, DWORD dwValue)
{
	BYTE bBodyLoc1, bBodyLoc2;

#line 1309
	D2ASSERT(hItem);
#line 1310
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ITEMSGetTypeBodyLocs(hItem, &bBodyLoc1, &bBodyLoc2);
	if (bBodyLoc1 == dwValue || bBodyLoc2 == dwValue)
		return TRUE;
	return FALSE;
}

DWORD __stdcall ITEMSGetTypeThrowableNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1326
	D2ASSERT(ptItemStats);
	return ptItemStats->throwable;
}

DWORD __stdcall ITEMSGetTypeThrowable(HUNIT hItem)
{
#line 1335
	D2ASSERT(hItem);
#line 1336
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeThrowableNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSGetTypeMissileType(HUNIT hUnit)
{
	PTITEMTYPEDATA ptStats;

#line 1345
	D2ASSERT(hUnit);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hUnit->dwClassId);
	return ptStats->missiletype;
}

BYTE __stdcall ITEMSGetTypeRangeAdder(HUNIT hUnit)
{
	PTITEMTYPEDATA ptStats;

	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hUnit->dwClassId);
	return ptStats->rangeadder;
}

DWORD __stdcall ITEMSTestTypeWClassEqualNoCheck(DWORD dwItemClassIndex, DWORD dwValue)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1366
	D2ASSERT(ptItemStats);
	return ptItemStats->wclass == dwValue;
}

DWORD __stdcall ITEMSTestTypeWClassEqual(HUNIT hItem, DWORD dwValue)
{
#line 1379
	D2ASSERT(hItem);
#line 1380
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSTestTypeWClassEqualNoCheck(hItem->dwClassId, dwValue);
}

DWORD __stdcall ITEMSGetTypeQuiveredNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1390
	D2ASSERT(ptItemStats);
	return ptItemStats->quivered;
}

DWORD __stdcall ITEMSGetTypeQuivered(HUNIT hItem)
{
#line 1400
	D2ASSERT(hItem);
#line 1401
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeQuiveredNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSGetTypeTwoHandedNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1411
	D2ASSERT(ptItemStats);
	return ptItemStats->bTwoHanded;
}

DWORD __stdcall ITEMSGetTypeTwoHanded(HUNIT hItem)
{
#line 1420
	D2ASSERT(hItem);
#line 1421
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeTwoHandedNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSGetTypeStackable(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 1430
	D2ASSERT(hItem);
#line 1431
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1433
	D2ASSERT(ptItemStats);
	return ptItemStats->stackable;
}

DWORD __stdcall ITEMSGetTypeBelt(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 1442
	D2ASSERT(hItem);
#line 1443
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1445
	D2ASSERT(ptItemStats);
	return ptItemStats->belt;
}

DWORD __stdcall ITEMSTestTypeGroupMatch(HUNIT hItem1, HUNIT hItem2)
{
	PTITEMTYPEDATA ptStats1, ptStats2;
	BOOL fFound1, fFound2;
	DWORD dwClass1, dwClass2;
	DWORD i;

	if (hItem1 && hItem2)
	{
		dwClass1 = hItem1->dwClassId;
		dwClass2 = hItem2->dwClassId;
		if (dwClass1 == dwClass2)
			return TRUE;
		ptStats1 = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwClass1);
		ptStats2 = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwClass2);

		fFound1 = FALSE;
		fFound2 = FALSE;
		for (i = 0; i < sgnPotionTierCount1; i++)
		{
			if (!fFound1 && ptStats1->code == sgPotionTierTbl1[i])
				fFound1 = TRUE;
			if (!fFound2 && ptStats2->code == sgPotionTierTbl1[i])
				fFound2 = TRUE;
			if (fFound1 && fFound2)
				return TRUE;
		}

		fFound1 = FALSE;
		fFound2 = FALSE;
		for (i = 0; i < sgnPotionTierCount2; i++)
		{
			if (!fFound1 && ptStats1->code == sgPotionTierTbl2[i])
				fFound1 = TRUE;
			if (!fFound2 && ptStats2->code == sgPotionTierTbl2[i])
				fFound2 = TRUE;
			if (fFound1 && fFound2)
				return TRUE;
		}

		fFound1 = FALSE;
		fFound2 = FALSE;
		for (i = 0; i < sgnPotionTierCount3; i++)
		{
			if (!fFound1 && ptStats1->code == sgPotionTierTbl3[i])
				fFound1 = TRUE;
			if (!fFound2 && ptStats2->code == sgPotionTierTbl3[i])
				fFound2 = TRUE;
			if (fFound1 && fFound2)
				return TRUE;
		}
	}
	return FALSE;
}

DWORD __stdcall ITEMSTestAutobeltOrFits(PTINVENTORY ptInventory, HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 1554
	D2ASSERT(hItem);
#line 1555
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1557
	D2ASSERT(ptItemStats);
	if (!(ptItemStats->autobelt)) {
		if (INVENTORYTestItemFits(ptInventory, hItem) == (DWORD)-1)
			return FALSE; }
	return TRUE;
}

DWORD __stdcall ITEMSGetTypeUseable(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 1573
	D2ASSERT(hItem);
#line 1574
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1576
	D2ASSERT(ptItemStats);
	return ptItemStats->useable;
}

DWORD __stdcall ITEMSGetTypeUnique(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 1585
	D2ASSERT(hItem);
#line 1586
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1588
	D2ASSERT(ptItemStats);
	return ptItemStats->unique;
}

DWORD __stdcall ITEMSGetTypeQuest(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 1597
	D2ASSERT(hItem);
#line 1598
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 1600
	D2ASSERT(ptItemStats);
	return ptItemStats->quest;
}

DWORD __stdcall ITEMSGetTypeHasInv(HUNIT hItem);
int __stdcall StatsUnitCalcMaxDurability(HUNIT hUnit);
int __stdcall StatsUnitCalcArmorClass(HUNIT hUnit);

static DWORD __fastcall ITEMSCostCalcByDefense(HUNIT hItem, PTITEMSTXT ptItemStats, DWORD dwCost, DWORD dwModType)
{
	int nDefense;

	nDefense = StatsUnitCalcArmorClass(hItem);
	if (ptItemStats->maxac - ptItemStats->minac + 1 != 0 && ptItemStats->maxac != 0)
		return (DWORD)(nDefense * dwCost) / (DWORD)ptItemStats->minac;
	return dwCost;
}

static DWORD __fastcall ITEMSCostCalcPlain(HUNIT hItem, PTITEMSTXT ptItemStats, DWORD dwCost, DWORD dwModType)
{
	return dwCost;
}

PTITEMSTATCOSTTXT __stdcall ItemTblsGetItemStatCost(int nStat);

void __fastcall ITEMSGetStatModsCost(HUNIT hItem, DWORD dwCost, DWORD *pdwCost)
{
	ITEMMODCOSTREC *ptModCost;
	PTITEMSTATCOSTTXT ptItemStatCost;
	int nBonus;
	int nValue;
	int i;

	*pdwCost = 0;
#line 1706
	D2ASSERT(sgnModCostSize < NUM_STATS);
	for (i = 0; i < NUM_STATS; i++)
	{
		nBonus = D2CommonStatsUnitGetBonusValue(hItem, i);
		if (nBonus == 0)
			continue;
		nValue = nBonus;
		if ((DWORD)i < sgnModCostSize)
		{
			ptModCost = &sgtModCostTbl[i];
			if (ptModCost->field_04 != 0 && ptModCost->field_00 != 0)
			{
				if (ptModCost->field_04 == 2)
					nValue = nBonus / ptModCost->field_04;
				else
					nValue = nBonus >> ptModCost->field_04;
			}
		}
		if (nValue < 0)
			nValue = -nValue;
		ptItemStatCost = ItemTblsGetItemStatCost(i);
		if (ptItemStatCost == NULL)
			continue;
		*pdwCost += ptItemStatCost->add * nValue;
		if (ptItemStatCost->multiply != 0 && ptItemStatCost->divide != 0)
			*pdwCost += (DWORD)(ptItemStatCost->multiply * dwCost) / (DWORD)(ptItemStatCost->divide * 4);
	}
}

DWORD __fastcall ITEMSGetRepairCost(HUNIT hItem, DWORD dwCost)
{
	int nMaxDurability;
	int nMissing;

	nMaxDurability = StatsUnitCalcMaxDurability(hItem);
	if (nMaxDurability > 0)
	{
		nMissing = ((nMaxDurability - D2CommonStatsUnitGetValue(hItem, 72)) << 10) / nMaxDurability;
		return max(1, (nMissing * dwCost + 512) >> 10);
	}
	return 0;
}

static int ITEMSGetTypeOrOne(HUNIT hUnit)
{
	if (UnitGetType(hUnit) != UNIT_ITEM)
		return 1;
	return (int)ITEMSGetTypeNoCheck(hUnit->dwClassId);
}

static BOOL ITEMSTestSkillItem(HUNIT hItem)
{
	int eType;

	eType = ITEMSGetTypeOrOne(hItem);
	if (eType < 24 || eType > 26)
		return FALSE;
	return TRUE;
}

static DWORD ITEMSGetSkillStatsCost(HUNIT hItem, DWORD dwCost)
{
	DWORD dwStat;
	int nLevel;
	BYTE bRow;
	DWORD dwSum;
	DWORD dwSumCubed;
	int i;
	PTSKILLSTXT ptSkill;

	if (!ITEMSTestSkillItem(hItem))
		return 0;
	dwSum = 0;
	dwSumCubed = 0;
	for (i = 0; i < sgcnItemSkillStats; i++)
	{
		dwStat = D2CommonStatsGetTotalValue(hItem, sgcadwItemSkillStats[i]);
		if (dwStat == 0)
			continue;
		nLevel = (int)dwStat >> 16;
		ptSkill = (PTSKILLSTXT)D2CommonDataTblsGetSkill(dwStat & 65535);
		bRow = ptSkill->skillrow + 1;
		dwSum += nLevel * bRow;
		dwSumCubed += nLevel * bRow * bRow * bRow;
	}
	return (dwSum + 10) * dwCost / 5 + dwSumCubed * 200;
}

DWORD __stdcall ITEMSGetPrice(HUNIT hItem, DWORD dwCapFlag, DWORD dwQuestArg, int nVendor, int nMode)
{
	DWORD dwQuantity;
	PTITEMSTXT ptItemStats;
	int nClass;
	int eItemType;
	DWORD dwCost;
	DWORD dwAdd;
	DWORD dwModType;
	BOOL fFixed;
	BOOL fSocketed;
	BOOL fAffixed;
	BOOL fBit10;
	PTBOOKSTXT ptBookMods;
	PTMAGICAFFIXTXT ptPrefix;
	PTMAGICAFFIXTXT ptSuffix;
	PTRAREAFFIXTXT ptRare1;
	PTRAREAFFIXTXT ptRare2;
	int nMaxDurability;
	DWORD dwInventory;
	DWORD dwNode;
	HUNIT hSocket;
	BYTE bAct;
	DWORD dwQuest;
	DWORD dwMax;

	fFixed = FALSE;
	dwQuantity = D2CommonStatsUnitGetValue(hItem, 70);
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsFindItemByCode(ITEMSGetTypeCode(hItem), &nClass);
	eItemType = (int)ITEMSGetTypeNoCheck(nClass);
	fAffixed = FALSE;
	fSocketed = FALSE;
	if (ITEMSDataTestFlags(hItem, 131072))
		return 1;

	if (eItemType == 5 || eItemType == 6)
	{
		dwCost = max(5, dwQuantity >> 2);
		fFixed = TRUE;
	}
	else if (eItemType == 18)
	{
		ptBookMods = (PTBOOKSTXT)D2CommonItemTblsGetBook(ITEMSDataGetMagicSuffixAt(hItem, 0));
#line 1798
		D2ASSERT(ptBookMods);
		dwCost = ptBookMods->costpercharge * dwQuantity + ptBookMods->basecost;
		fFixed = TRUE;
	}
	else if (eItemType == 22)
	{
		ptBookMods = (PTBOOKSTXT)D2CommonItemTblsGetBook(ITEMSDataGetMagicSuffixAt(hItem, 0));
#line 1805
		D2ASSERT(ptBookMods);
		dwCost = ptBookMods->costpercharge * 2;
		fFixed = TRUE;
	}
	else
	{
		dwCost = ptItemStats->cost;
		if (ITEMSGetTypeStackable(hItem) && dwQuantity > 1)
			dwCost *= dwQuantity;
	}

	if (nMode == 2)
	{
		dwCost = (dwCost * 5 + 600) * 5;
		if (ITEMSIsArmor(hItem))
			dwCost >>= 1;
		if (dwCost > 800000)
			dwCost = 800000;
		return dwCost;
	}

	if (ITEMSGetTypeHasInv(hItem))
	{
		dwInventory = hItem->ptInventory;
		if (D2CommonINVENTORYGetCategoryListCount(dwInventory) > 0)
		{
			fFixed = TRUE;
			fSocketed = TRUE;
		}
	}
	fBit10 = ITEMSDataTestFlags(hItem, 16) != 0;
	dwModType = ITEMSDataGetModType(hItem);
	ptPrefix = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(ITEMSDataGetMagicPrefixAt(hItem, 0));
	ptSuffix = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(ITEMSDataGetMagicSuffixAt(hItem, 0));
	if (fBit10)
	{
		if (!fFixed)
		{
			DWORD dwModsCost;

			dwAdd = 0;
			if (ptPrefix != NULL)
			{
				if (ptPrefix->multiply > 0 && ptPrefix->divide > 0)
					dwAdd = (DWORD)(ptPrefix->multiply * dwCost) / (DWORD)ptPrefix->divide;
				dwAdd += ptPrefix->add;
			}
			if (ptSuffix != NULL)
			{
				if (ptSuffix->multiply > 0 && ptSuffix->divide > 0)
					dwAdd += (DWORD)(ptSuffix->multiply * dwCost) / (DWORD)ptSuffix->divide;
				dwAdd += ptSuffix->add;
			}
			ITEMSGetStatModsCost(hItem, dwCost, &dwModsCost);
			dwAdd += dwModsCost;
			if (dwModType == 6)
			{
				ptRare1 = (PTRAREAFFIXTXT)D2CommonItemTblsGetRareAffix(ITEMSDataGetRarePrefix(hItem));
				ptRare2 = (PTRAREAFFIXTXT)D2CommonItemTblsGetRareAffix(ITEMSDataGetRareSuffix(hItem));
				if (ptRare1 != NULL)
				{
					if (ptRare1->divide > 0 && ptRare1->multiply > 0)
						dwAdd += (DWORD)(ptRare1->multiply * dwCost) / (DWORD)ptRare1->divide;
					dwAdd += ptRare1->add;
				}
				if (ptRare2 != NULL)
				{
					if (ptRare2->divide > 0 && ptRare2->multiply > 0)
						dwAdd += (DWORD)(ptRare2->multiply * dwCost) / (DWORD)ptRare2->divide;
					dwAdd += ptRare2->add;
				}
			}
			dwCost += dwAdd;
			if (dwAdd != 0)
				fAffixed = TRUE;
		}
		if (!fSocketed)
			dwCost += ITEMSGetSkillStatsCost(hItem, dwCost);
	}

	if (dwModType == 1)
		dwCost >>= 2;
	else if (eItemType < NUM_ITEM_TRANSLATE_ENTRIES && sgTranslateTbl[eItemType].pfnCostCalc != NULL
		&& fBit10)
	{
#line 1925
		D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgTranslateTbl[eItemType].pfnCostCalc), "sgTranslateTbl[eItemType].pfnCostCalc");

		dwCost = sgTranslateTbl[eItemType].pfnCostCalc(hItem, ptItemStats, dwCost, dwModType);
	}

	if (!ITEMSGetTypeQuivered(hItem) && !ITEMSGetTypeThrowable(hItem))
	{
		nMaxDurability = StatsUnitCalcMaxDurability(hItem);
		if (nMaxDurability != 0 && nMaxDurability != D2CommonStatsUnitGetValue(hItem, 72)
			&& nMode == 1)
			dwCost = (DWORD)(((D2CommonStatsUnitGetValue(hItem, 72) << 10) / nMaxDurability)
				* dwCost) >> 10;
	}

	if (fSocketed)
	{
		dwCost <<= 1;
		if (ITEMSGetTypeHasInv(hItem))
		{
			dwInventory = hItem->ptInventory;
			for (dwNode = (DWORD)D2CommonINVENTORYGetCategoryListHead(dwInventory); dwNode != 0;
				dwNode = D2CommonINVENTORYNodeGetNext(dwNode))
			{
				hSocket = (HUNIT)D2CommonINVENTORYNodeGetKeyA(dwNode);
				if (hSocket != NULL && ITEMSGetTypeOrOne(hSocket) == 20)
					dwCost += ((PTITEMSTXT)D2CommonItemTblsGetItemStats(hSocket->dwClassId))->cost / 10;
			}
		}
	}

	switch (nVendor)
	{
	case 147:
		if (nMode == 0)
			dwCost += dwCost >> 4;
		else if (nMode == 1)
			dwCost >>= 2;
		bAct = 0;
		break;
	case 154:
	case 178:
	case 253:
	case 257:
		if (nVendor == 154)
			bAct = 0;
		else if (nVendor == 178)
			bAct = 1;
		else if (nVendor == 253)
			bAct = 2;
		else
			bAct = 3;
		if (nMode == 0)
			dwCost -= dwCost >> 4;
		else if (nMode == 1)
			dwCost >>= 2;
		else
			dwCost = ITEMSGetRepairCost(hItem, dwCost >> 3);
		break;
	case 177:
	case 199:
	case 202:
		if (nMode == 1)
			dwCost >>= 2;
		bAct = 1;
		break;
	case 252:
	case 254:
	case 255:
		if (nMode == 1)
			dwCost >>= 2;
		bAct = 2;
		break;
	case 405:
		if (nMode == 1)
			dwCost >>= 2;
		bAct = 3;
		break;
	default:
		if (nMode == 1)
			dwCost >>= 2;
		bAct = 0;
		break;
	}
	if (bAct < 4 && nMode == 0)
	{
		dwQuest = sgcadwVendorQuests[bAct];
		if (dwQuest != 41 && D2CommonStepTestGridBit(dwQuestArg, dwQuest, 0) == 1)
			dwCost = dwCost * 90 / 100;
	}

	if (dwCost <= 0)
		return 1;
	if (fAffixed == TRUE && nMode == 0)
		dwCost += 100;
	else if (nMode == 1)
	{
		if (dwCapFlag == 0)
		{
			dwMax = (bAct + 1) * 5000;
			if (dwMax < dwCost)
				dwCost = dwMax;
		}
		else if ((int)dwCost > 25000)
			dwCost = 25000;
	}
	return dwCost;
}

DWORD __stdcall ITEMSGetTypeMaxStackNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2100
	D2ASSERT(ptItemStats);
	return ptItemStats->maxstack;
}

DWORD __stdcall ITEMSGetTypeMaxStack(HUNIT hItem)
{
#line 2109
	D2ASSERT(hItem);
#line 2110
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeMaxStackNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSGetTypeMinStackNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2120
	D2ASSERT(ptItemStats);
	return ptItemStats->minstack;
}

DWORD __stdcall ITEMSGetTypeMinStack(HUNIT hItem)
{
#line 2129
	D2ASSERT(hItem);
#line 2130
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeMinStackNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSTestTypeBitfieldFourthBit(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2139
	D2ASSERT(hItem);
#line 2140
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2142
	D2ASSERT(ptItemStats);
	return ptItemStats->bitfield1 & 8;
}

BYTE __stdcall ITEMSGetSpellIcon(HUNIT hItem)
{
	PTBOOKSTXT ptBookMods;
	PTITEMSTXT ptItemStats;

#line 2154
	D2ASSERT(hItem);
#line 2155
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	if (ITEMSGetType(hItem) == 18 || ITEMSGetType(hItem) == 22)
	{
		ptBookMods = (PTBOOKSTXT)D2CommonItemTblsGetBook(ITEMSDataGetMagicSuffixAt(hItem, 0));
#line 2159
		D2ASSERT(ptBookMods);
		return ptBookMods->spellicon;
	}
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2163
	D2ASSERT(ptItemStats);
	return ptItemStats->spellicon;
}

BYTE __stdcall ITEMSGetTypeDurWarning(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2174
	D2ASSERT(hItem);
#line 2175
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2177
	D2ASSERT(ptItemStats);
	return ptItemStats->durwarning;
}

BYTE __stdcall ITEMSGetTypeQntWarning(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2186
	D2ASSERT(hItem);
#line 2187
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2189
	D2ASSERT(ptItemStats);
	return ptItemStats->qntwarning;
}

DWORD __stdcall ITEMSGetTypeStrBonus(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2198
	D2ASSERT(hItem);
#line 2199
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2202
	D2ASSERT(ptItemStats);
	return ptItemStats->strbonus;
}

DWORD __stdcall ITEMSGetTypeDexBonus(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2211
	D2ASSERT(hItem);
#line 2212
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2215
	D2ASSERT(ptItemStats);
	return ptItemStats->dexbonus;
}

DWORD __stdcall ITEMSGetTypeHasInvNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2225
	D2ASSERT(ptItemStats);
	return ptItemStats->hasinv;
}

DWORD __stdcall ITEMSGetTypeHasInv(HUNIT hItem)
{
#line 2234
	D2ASSERT(hItem);
#line 2235
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeHasInvNoCheck(hItem->dwClassId);
}

BOOL __stdcall ITEMSTestTypeHasDurability(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2244
	D2ASSERT(hItem);
#line 2245
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2247
	D2ASSERT(ptItemStats);
	if (ptItemStats->durability > 0)
	{
		if (ptItemStats->nodurability == 0)
			return TRUE;
	}
	return FALSE;
}

DWORD __stdcall ITEMSTestKeepsRolledQuality(HUNIT hItem)
{
	int eType;

#line 2260
	D2ASSERT(hItem);
#line 2261
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	eType = (int)ITEMSGetTypeNoCheck(hItem->dwClassId);
#line 2263
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwKeepsRolledQuality;
}

BYTE __stdcall ITEMSGetTypeGemSocketsNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2273
	D2ASSERT(ptItemStats);
	return ptItemStats->gemsockets;
}

BYTE __stdcall ITEMSGetTypeGemSockets(HUNIT hItem)
{
#line 2282
	D2ASSERT(hItem);
#line 2283
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSGetTypeGemSocketsNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSIsWeaponNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;
	int eType;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1035
	D2ASSERT(ptItemStats);
	eType = (int)ptItemStats->type;
#line 2293
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwWeapon;
}

DWORD __stdcall ITEMSIsWeapon(HUNIT hItem)
{
#line 2302
	D2ASSERT(hItem);
#line 2303
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSIsWeaponNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSTestTypeBitfieldFirstBit(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2313
	D2ASSERT(ptItemStats);
	return ptItemStats->bitfield1 & 1;
}

DWORD __stdcall ITEMSTestTypeBitfieldSecondBit(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2323
	D2ASSERT(ptItemStats);
	return ptItemStats->bitfield1 & 2;
}

DWORD __stdcall ITEMSTestTypeBitfieldThirdBit(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2333
	D2ASSERT(ptItemStats);
	return ptItemStats->bitfield1 & 4;
}

BOOL __stdcall ITEMSTestTypeSellableNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2343
	D2ASSERT(ptItemStats);
	if (!(ptItemStats->quest >= 1)) {
		if (ptItemStats->type != 39)
			return TRUE; }
	return FALSE;
}

DWORD __stdcall ITEMSDataGetUniqueOrSetRow(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 2361
	D2ASSERT(hItem);
#line 2362
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 2364
	D2ASSERT(ptItemData);
	return ptItemData->dwUniqueOrSetRow;
}

void __stdcall ITEMSDataSetUniqueOrSetRow(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 2373
	D2ASSERT(hItem);
#line 2374
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 2376
	D2ASSERT(ptItemData);
	ptItemData->dwUniqueOrSetRow = dwValue;
}

void __stdcall ITEMSDataSetOwnerId(HUNIT hItem, DWORD dwValue)
{
	PTITEMDATA ptItemData;

#line 2384
	D2ASSERT(hItem);
#line 2385
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 2387
	D2ASSERT(ptItemData);
	ptItemData->dwOwnerId = dwValue;
}

DWORD __stdcall ITEMSDataGetOwnerId(HUNIT hItem)
{
	PTITEMDATA ptItemData;

#line 2395
	D2ASSERT(hItem);
#line 2396
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 2398
	D2ASSERT(ptItemData);
	return ptItemData->dwOwnerId;
}

DWORD __stdcall ITEMSIsArmorNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;
	int eType;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1035
	D2ASSERT(ptItemStats);
	eType = (int)ptItemStats->type;
#line 2408
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwArmor;
}

DWORD __stdcall ITEMSIsArmor(HUNIT hItem)
{
#line 2417
	D2ASSERT(hItem);
#line 2418
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSIsArmorNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSTestHasBodyLoc(HUNIT hItem)
{
	BYTE bBodyLoc1, bBodyLoc2;
	int eType;

#line 2427
	D2ASSERT(hItem);
#line 2428
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ITEMSGetTypeBodyLocs(hItem, &bBodyLoc1, &bBodyLoc2);
	if (bBodyLoc1 == 0 && bBodyLoc2 == 0)
		return 0;
	if (hItem->eType != UNIT_ITEM)
		eType = 1;
	else
		eType = (int)ITEMSGetTypeNoCheck(hItem->dwClassId);
#line 2438
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwHasBodyLoc;
}

DWORD __stdcall ITEMSIsMiscellaneousNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;
	int eType;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 1035
	D2ASSERT(ptItemStats);
	eType = (int)ptItemStats->type;
#line 2448
	D2ASSERT(eType <= sgTranslateTblSize);
	return sgTranslateTbl[eType].dwMiscellaneous;
}

DWORD __stdcall ITEMSIsMiscellaneous(HUNIT hItem)
{
#line 2457
	D2ASSERT(hItem);
#line 2458
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSIsMiscellaneousNoCheck(hItem->dwClassId);
}

DWORD __stdcall ITEMSGetZeroA(DWORD dwUnused1)
{
	return 0;
}

DWORD __stdcall ITEMSGetZeroB(DWORD dwUnused1)
{
	return 0;
}

DWORD __stdcall ITEMSGetConstantFive(DWORD dwUnused1)
{
	return 5;
}

WORD __stdcall ITEMSGetWClassIndex(HUNIT hItem)
{
	PTITEMSTXT ptItemStats;
	DWORD i;

	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2568
	D2ASSERT(ptItemStats);
	for (i = 0; i < sgnWClassTblSize; i++)
	{
		if (ptItemStats->wclass == sgtWClassTbl[i].dwCode)
			return sgtWClassTbl[i].wIndex;
	}
	return 0;
}

BOOL __stdcall ITEMSTestTypeTransmogrifyNoCheck(DWORD dwItemClassIndex)
{
	PTITEMTYPEDATA ptItemStats;

	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(dwItemClassIndex);
#line 2584
	D2ASSERT(ptItemStats);
	return ptItemStats->Transmogrify != 0;
}

BOOL __stdcall ITEMSTestTypeTransmogrify(HUNIT hItem)
{
#line 2597
	D2ASSERT(hItem);
#line 2598
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	return ITEMSTestTypeTransmogrifyNoCheck(hItem->dwClassId);
}

BOOL __stdcall ITEMSTestModTypeMagicOrAbove(HUNIT hItem)
{
	DWORD dwModType;
	DWORD i;

	dwModType = ITEMSDataGetModType(hItem);
	for (i = 0; i < sgdwItemField00TblSize; i++)
	{
		if (dwModType == sgItemField00Tbl[i])
			return TRUE;
	}
	return FALSE;
}

BOOL __stdcall ITEMSTestSellable(HUNIT hItem)
{
	if (!(ITEMSDataTestFlags(hItem, 4096) != 0)) {
		return ITEMSTestTypeSellableNoCheck(hItem->dwClassId); }
	return FALSE;
}

BYTE __stdcall ITEMSGetTypeHitClass(HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;

#line 2641
	D2ASSERT(hItem);
#line 2642
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2644
	D2ASSERT(ptStats);
	return ptStats->bHitClass;
}

BOOL __stdcall ITEMSTestOneOrTwoHanded(HUNIT hPlayer, HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2653
	D2ASSERT(hItem);
	ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2656
	D2ASSERT(ptItemStats);
	if (ptItemStats->bOneOrTwoHanded != 0)
		return TRUE;
#line 2662
	D2ASSERT(hPlayer);
	if (hPlayer->eType == UNIT_PLAYER && hPlayer->dwClassId == 4
		&& ITEMSGetType(hItem) == 30 && ITEMSGetTypeTwoHanded(hItem))
		return TRUE;
	return FALSE;
}

#define NUM_ICLASSES		9
#define NUM_ITRANSFORMS		21

static void ITEMSPackTransform(int nClass, int nTransform, BYTE *pbTransform)
{
#line 2684
	D2ASSERT(nClass < NUM_ICLASSES);
#line 2685
	D2ASSERT(nTransform < NUM_ITRANSFORMS);
	*pbTransform = (BYTE)((nTransform & 31) + (nClass << 5));
}

BYTE * __stdcall ITEMSGetColorTransform(HUNIT hUnit, HUNIT hItemUnit, BYTE *pbTransform, BOOL fInventory)
{
	HUNIT hItem = hItemUnit;
	PTITEMSTXT ptItemStats;
	PTITEMSTXT ptGemItemStats;
	PTGEMSTXT ptGemMods;
	DWORD dwQuality;
	DWORD ptInventory;
	void *pNode;
	HUNIT hGem;
	int i;
	int j;

	if (hItem == NULL)
		return NULL;
	if (hUnit != NULL && D2CommonStatsGetState(hUnit, 16))
	{
		PTITEMSTXT ptWeaponStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
		int nGroup = ptWeaponStats->bWeaponGroup;

		switch (nGroup)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
			return D2CMPPalShiftGetTransform(ptWeaponStats->Transform, 9);
		}
	}
	dwQuality = ITEMSDataGetModType(hItem);
	if (dwQuality == 4 || dwQuality == 6)
	{
		for (i = 0; i < 3; i++)
		{
			DWORD dwAffix;
			PTMAGICAFFIXTXT ptMagicMods;

			dwAffix = ITEMSDataGetMagicSuffixAt(hItem, i);
			if (dwAffix == (DWORD)-1)
				continue;
			ptMagicMods = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(dwAffix);
#line 2734
			D2ASSERT(ptMagicMods);
			if (ptMagicMods->transform)
			{
				PTITEMSTXT ptAffixStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);

				ITEMSPackTransform(ptAffixStats->Transform, ptMagicMods->transformcolor, pbTransform);
				if (fInventory)
					return D2CMPPalShiftGetTransform(ptAffixStats->InvTrans, ptMagicMods->transformcolor);
				return D2CMPPalShiftGetTransform(ptAffixStats->Transform, ptMagicMods->transformcolor);
			}
		}
		for (j = 0; j < 3; j++)
		{
			DWORD dwAffix;
			PTMAGICAFFIXTXT ptMagicMods;

			dwAffix = ITEMSDataGetMagicPrefixAt(hItem, j);
			if (dwAffix == (DWORD)-1)
				continue;
			ptMagicMods = (PTMAGICAFFIXTXT)D2CommonItemTblsGetMagicAffix(dwAffix);
#line 2752
			D2ASSERT(ptMagicMods);
			if (ptMagicMods->transform)
			{
				PTITEMSTXT ptAffixStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);

				ITEMSPackTransform(ptAffixStats->Transform, ptMagicMods->transformcolor, pbTransform);
				if (fInventory)
					return D2CMPPalShiftGetTransform(ptAffixStats->InvTrans, ptMagicMods->transformcolor);
				return D2CMPPalShiftGetTransform(ptAffixStats->Transform, ptMagicMods->transformcolor);
			}
		}
		return NULL;
	}
	if (dwQuality == 7)
	{
		DWORD dwRow = ITEMSDataGetUniqueOrSetRow(hItem);
		PTUNIQUEITEMSTXT ptUniqueMods;
		PTITEMSTXT ptUniqueStats;

		ptUniqueMods = (PTUNIQUEITEMSTXT)D2CommonItemTblsGetUniqueItem(dwRow);
#line 2767
		D2ASSERT(ptUniqueMods);
		if (!ptUniqueMods->transform)
			return NULL;
		ptUniqueStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
		ITEMSPackTransform(ptUniqueStats->Transform, ptUniqueMods->transformcolor, pbTransform);
		if (fInventory)
		{
			if (!ptUniqueMods->invtransform)
				return NULL;
			return D2CMPPalShiftGetTransform(ptUniqueStats->InvTrans, ptUniqueMods->transformcolor);
		}
		return D2CMPPalShiftGetTransform(ptUniqueStats->Transform, ptUniqueMods->transformcolor);
	}
	if (dwQuality == 5)
	{
		DWORD dwRow = ITEMSDataGetUniqueOrSetRow(hItem);
		PTSETITEMSTXT ptSetItemMods;
		PTITEMSTXT ptSetStats;

		ptSetItemMods = (PTSETITEMSTXT)D2CommonItemTblsGetSetItem(dwRow);
#line 2783
		D2ASSERT(ptSetItemMods);
		if (!ptSetItemMods->transform)
			return NULL;
		ptSetStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
		ITEMSPackTransform(ptSetStats->Transform, ptSetItemMods->transformcolor, pbTransform);
		if (fInventory)
			return D2CMPPalShiftGetTransform(ptSetStats->InvTrans, ptSetItemMods->transformcolor);
		return D2CMPPalShiftGetTransform(ptSetStats->Transform, ptSetItemMods->transformcolor);
	}
	if (!ITEMSGetTypeHasInv(hItem))
		return NULL;
	if (ITEMSGetTypeGemSockets(hItem) <= 0)
		return NULL;
	if (!ITEMSDataTestFlags(hItem, 2048))
		return NULL;
	ptInventory = hItem->ptInventory;
	pNode = D2CommonINVENTORYGetCategoryListHead(ptInventory);
	if (pNode == NULL)
		return NULL;
	hGem = (HUNIT)D2CommonINVENTORYNodeGetKeyA(pNode);
	if (hGem == NULL)
		return NULL;
	if (UnitGetType(hGem) != UNIT_ITEM)
		return NULL;
	if (ITEMSGetType(hGem) != 20)
		return NULL;
	ptGemItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hGem->dwClassId);
#line 2810
	D2ASSERT(ptGemItemStats);
	ptGemMods = (PTGEMSTXT)D2CommonItemTblsGetGem(ptGemItemStats->gemoffset);
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
	ITEMSPackTransform(ptItemStats->Transform, ptGemMods->transform, pbTransform);
	if (fInventory)
		return D2CMPPalShiftGetTransform(ptItemStats->InvTrans, ptGemMods->transform);
	return D2CMPPalShiftGetTransform(ptItemStats->Transform, ptGemMods->transform);
}

BOOL __stdcall ITEMSTestModifiable(HUNIT hItem)
{
	DWORD dwClass;
	DWORD dwType;
	DWORD dwQuality;
	PTITEMSTXT ptItemStats;

	if (ITEMSGetType(hItem) == 4)
		return FALSE;
	if (ITEMSDataTestFlags(hItem, 4096))
		return FALSE;
	dwClass = hItem->dwClassId;
	if (!ITEMSTestTypeBitfieldFirstBit(dwClass))
		return FALSE;
	dwType = ITEMSGetType(hItem);
	if (dwType == 10 || dwType == 12)
		return FALSE;
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(dwClass);
	if (ptItemStats->throwable)
		return FALSE;
	if (ptItemStats->quest && ITEMSGetTypeCode(hItem) != 543647084)
		return FALSE;
	if (ITEMSGetTypeHasInv(hItem) && ITEMSGetTypeGemSockets(hItem) > 0 && D2CommonITEMSDataTestFlags(hItem, 2048))
		return FALSE;
	dwQuality = D2CommonITEMSDataGetModType(hItem);
	if (dwQuality == 5 || dwQuality == 4 || dwQuality == 6 || dwQuality == 7)
		return FALSE;
	return TRUE;
}

BYTE __stdcall ITEMSGetWeaponGroupEnum(HUNIT hItem)
{
	PTITEMTYPEDATA ptItemStats;

#line 2302
	D2ASSERT(hItem);
#line 2303
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	if (ITEMSIsWeaponNoCheck(hItem->dwClassId) != 0)
	{
		ptItemStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2887
		D2ASSERT(ptItemStats);
		return ptItemStats->weapongroupenum;
	}
	return 11;
}

BOOL __stdcall ITEMSTestCanStack(HUNIT hItem1, HUNIT hItem2)
{
	DWORD dwQuality1, dwQuality2;
	DWORD dwValue1;
	DWORD i;

	if (!(!ITEMSGetTypeStackable(hItem1)))
	{
		if (!ITEMSGetTypeStackable(hItem2))
			return FALSE;
		if (hItem1->eType != hItem2->eType)
			return FALSE;
		if (hItem1->dwClassId != hItem2->dwClassId)
			return FALSE;
		dwQuality1 = ITEMSDataGetModType(hItem1);
		dwQuality2 = ITEMSDataGetModType(hItem2);
		if (dwQuality1 != dwQuality2)
			return FALSE;
		for (i = 0; i < sgdwNoStackQualities; i++)
		{
			if (dwQuality1 == sgadwNoStackQualities[i])
				return FALSE;
			if (dwQuality2 == sgadwNoStackQualities[i])
				return FALSE; }
		dwValue1 = ITEMSDataGetUniqueOrSetRow(hItem1);
		if (dwValue1 != ITEMSDataGetUniqueOrSetRow(hItem2))
			return FALSE;
		if (D2CommonStatsUnitGetValue(hItem1, 21) != D2CommonStatsUnitGetValue(hItem2, 21))
			return FALSE;
		if (D2CommonStatsUnitGetValue(hItem1, 22) == D2CommonStatsUnitGetValue(hItem2, 22))
			return TRUE;

	}
	return FALSE;
}

BOOL __stdcall ITEMSValidateQuantity(HUNIT hUnit)
{
	int n;

	if (!(hUnit == NULL)) {
		n = D2CommonStatsUnitGetValue(hUnit, 70);
		if (n >= 0)
			return TRUE;
		if (!D2CommonStatsGetTotalValue(hUnit, 125))
			return FALSE;
		D2CommonStatsUnitSetValue(hUnit, 70, 0);
		return TRUE; }
	return FALSE;
}

BOOL __stdcall ITEMSTestSetComplete(HUNIT hUnit, HUNIT hItem)
{
	BYTE abFound[6];
	DWORD nFound;
	DWORD dwSetId;
	PTSETITEMSTXT ptSetMods;
	PTSETITEMINFO ptSetItemInfo;
	PTINVLISTNODE ptNode;
	HUNIT hEquip;
	PTITEMSTXT ptItemStats;
	DWORD i;

	if (hItem == NULL)
		return FALSE;
	if (ITEMSDataGetModType(hItem) != 5)
		return FALSE;
	for (i = 0; i < 6; i++)
		abFound[i] = 0;
	nFound = 0;
	dwSetId = ITEMSDataGetUniqueOrSetRow(hItem);
	ptSetMods = (PTSETITEMSTXT)D2CommonItemTblsGetSetItem(dwSetId);
#line 2983
	D2ASSERT(ptSetMods);
	ptSetItemInfo = (PTSETITEMINFO)&ptSetMods->item1;
	D2_OPAQUE(ptSetItemInfo);
#line 2985
	D2ASSERT(ptSetItemInfo);
	if (hUnit->ptInventory != 0)
	{
		for (ptNode = (PTINVLISTNODE)D2CommonINVENTORYGetCategoryListHead(hUnit->ptInventory); ptNode != NULL;
			ptNode = (PTINVLISTNODE)D2CommonINVENTORYNodeGetNext(ptNode))
		{
			if (D2CommonINVENTORYNodeGetCategory(ptNode) != 3)
				continue;
			hEquip = (HUNIT)D2CommonINVENTORYNodeGetKeyA(ptNode);
			if (hEquip == NULL)
				continue;
			if (ITEMSDataGetModType(hEquip) != 5)
				continue;
			if (ITEMSDataGetUniqueOrSetRow(hEquip) != dwSetId)
				continue;
			ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hEquip->dwClassId);
#line 2996
			D2ASSERT(ptItemStats);
			for (i = 0; i < 6; i++)
			{
				if (ptItemStats->code == ptSetItemInfo[i].item && !abFound[i])
				{
					abFound[i] = 1;
					nFound++;
				}
			}
		}
	}
	return nFound == ptSetMods->numitems;
}

BOOL __stdcall ITEMSTestEffectsActive(HUNIT hItem)
{
	PTITEMDATA ptItemData;

	if (hItem != NULL) {
#line 588
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 590
	D2ASSERT(ptItemData);
	if (!(ptItemData->dwFlags & 256)) {
	ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 590
	D2ASSERT(ptItemData);
	if (!(ptItemData->dwFlags & 16384))
		return TRUE; } }

	return FALSE;
}

DWORD __stdcall ITEMSCountUnidentified(HUNIT hUnit)
{
	DWORD dwNode;
	HUNIT hItem;
	PTITEMDATA ptItemData;
	DWORD dwResult;
	int nCount;
	DWORD ptInventory;

	ptInventory = hUnit->ptInventory;
	nCount = 0;
	dwNode = (DWORD)D2CommonINVENTORYGetCategoryListHead(ptInventory);
	while (dwNode != 0)
	{
		hItem = (HUNIT)D2CommonINVENTORYNodeGetKeyA(dwNode);
#line 587
		D2ASSERT(hItem);
#line 588
		D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
		ptItemData = (PTITEMDATA)hItem->uTypeData;
#line 590
		D2ASSERT(ptItemData);
		if (!(ptItemData->dwFlags & 16))
		{
			dwResult = D2CommonINVENTORYNodeGetCategory(dwNode);
			if (dwResult == 1)
			{
				if (ITEMSDataGetPage(hItem) == 0 || ITEMSDataGetPage(hItem) == 3)
					nCount++;
			}
			else if (dwResult == 3)
			{
				nCount++;
			}
		}
		dwNode = D2CommonINVENTORYNodeGetNext(dwNode);
	}
	return nCount;
}

DWORD __stdcall ITEMSScaleValueByPlayerClass(HUNIT hUnit, DWORD dwValue)
{
	DWORD dwRes = dwValue;

	if (hUnit->eType == UNIT_PLAYER)
	{
		switch (hUnit->dwClassId)
		{
		case 0:
		case 3:
			dwRes = dwValue + ((int)dwValue >> 1);
			break;
		case 4:
			dwRes = dwValue * 2;
			break;
		}
	}
	return dwRes;
}

DWORD __stdcall ITEMSScaleValueByPlayerClassAlt(HUNIT hUnit, DWORD dwValue)
{
	DWORD dwResult = dwValue;

	if (hUnit->eType == UNIT_PLAYER)
	{
		switch (hUnit->dwClassId)
		{
		case 1:
		case 2:
			dwResult = dwValue * 2;
			break;
		case 0:
		case 3:
			dwResult = dwValue + ((int)dwValue >> 1);
			break;
		}
	}
	return dwResult;
}
