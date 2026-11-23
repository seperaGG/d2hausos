#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Monsters\\MonsterKillsLog.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

struct MONKILLSROW4
{
 	DWORD		dwComponent;
 	DWORD		dwStatValue;
 	DWORD		dwCostBonus;
 	DWORD		dwLevelBonus;
};

struct MONKILLSROW6
{
 	DWORD		dwHandVariation;
 	DWORD		field_04;
 	DWORD		dwDamageMin;
 	DWORD		dwDamageMax;
 	DWORD		dwCost;
 	DWORD		dwLevel;
};

struct MONKILLSROW5
{
 	DWORD		dwHireSpecial;
 	DWORD		dwCostBonus;
 	DWORD		dwLevelBonus;
 	DWORD		dwSkillLevel;
 	DWORD		dwStringId;
};

struct MONKILLSROW7
{
 	int			nLifeBonus;
 	int			nDefenseBonus;
 	int			nLevelBonus;
 	int			nDamageMinFactor;
 	int			nDamageMaxFactor;
 	int			nSkillLevelBonus;
 	int			nCostFactor;
};

int gnMonKillsUnreadA = 6;
static int sgcanMonKillsBaseA[4] = { 0, 2, 4, 0 };
static int sgcanMonKillsRangeA[4] = { 2, 2, 2, 0 };
int gnMonKillsUnreadB = 6;
static int sgcanMonKillsBaseB[4] = { 0, 2, 4, 0 };
static int sgcanMonKillsRangeB[4] = { 2, 2, 2, 0 };
int gnMonKillsUnreadC = 6;
static int sgcanMonKillsBaseC[4] = { 0, 2, 4, 0 };
static int sgcanMonKillsRangeC[4] = { 2, 2, 2, 0 };
int gnMonKillsUnreadD = 12;
static int sgcanMonKillsBaseD[4] = { 0, 3, 6, 0 };
static int sgcanMonKillsRangeD[4] = { 3, 3, 6, 0 };

static int sgcanMonKillsStart[4][2] =
{
	{ 150, 3 }, { 300, 9 }, { 600, 15 }, { 0, 0 },
};

static struct MONKILLSROW4 sgcatMonKillsRowsA[6] =
{
	{ 0, 30, 0, 0 }, { 0, 60, 100, 2 }, { 0, 120, 0, 0 }, { 1, 180, 110, 3 }, { 0, 240, 0, 0 },
	{ 1, 400, 170, 3 },
};

static struct MONKILLSROW4 sgcatMonKillsRowsB[6] =
{
	{ 0, 15, 0, 0 }, { 1, 45, 80, 1 }, { 0, 25, 0, 0 }, { 1, 60, 90, 2 }, { 0, 80, 0, 0 }, { 1, 150, 125, 2 },
};

static struct MONKILLSROW6 sgcatMonKillsRowsC[6] =
{
	{ 0, 1, 1, 3, 0, 0 }, { 1, 1, 2, 4, 75, 2 }, { 0, 1, 2, 6, 0, 0 }, { 1, 1, 3, 8, 125, 2 },
	{ 1, 1, 4, 10, 200, 4 }, { 0, 1, 3, 8, 0, 0 },
};

static struct MONKILLSROW5 sgcatMonKillsRowsD[12] =
{
	{ 0, 0, 0, 0, 0 },
	{ 1, 55, 2, 0, 3371 },
	{ 2, 85, 2, 0, 3372 },
	{ 3, 0, 0, 0, 0 },
	{ 4, 65, 1, 3, 3373 },
	{ 5, 75, 1, 0, 3374 },
	{ 6, 0, 0, 2, 3375 },
	{ 7, 50, 1, 2, 3376 },
	{ 8, 0, 0, 2, 3377 },
	{ 9, 145, 1, 3, 3378 },
	{ 10, 190, 2, 3, 3379 },
	{ 11, 130, 1, 3, 3380 },
};

static struct MONKILLSROW7 sgcatMonKillsDifficulty[3] =
{
	{ 0, 0, 0, 1, 1, 0, 1 }, { 80, 40, 25, 2, 2, 3, 2 }, { 120, 60, 40, 3, 3, 5, 3 },
};

struct MONKILLSRECORD
{
 	int			nKind;
 	DWORD		dwComponentTR;
 	DWORD		dwComponentHD;
 	DWORD		dwComponentLH;
 	DWORD		dwComponentSH;
 	DWORD		dwComponentRH;
 	int			nLife;
 	int			nDefense;
 	int			nLevel;
 	int			nDamageMin;
 	int			nDamageMax;
 	DWORD		nHireSpecial;
 	int			nSkillLevel;
 	int			nCost;
 	WORD		wFlags;
};
typedef struct MONKILLSRECORD MONKILLSRECORD, *PTMONKILLSRECORD;

void __fastcall SeedInit(D2SEED *ptSeed);
void __fastcall SeedSet(D2SEED *ptSeed, DWORD dwSeed);
DWORD __fastcall SeedRand(D2SEED *ptSeed, DWORD nMax);

BOOL __fastcall MonsterKillsLogRollRecord(DWORD dwSeed, PTMONKILLSRECORD ptRecord, BYTE bDifficulty)
{
	D2SEED tSeed;
	int nKind;
	int nRow;
	int nBase;
	int nHireSpecial;

	if (!dwSeed || ptRecord == NULL || bDifficulty > 2)
		return FALSE;
	nKind = ptRecord->nKind;
	if (nKind < 0 || nKind > 3)
		return FALSE;
	ptRecord->wFlags = 0;
	SeedInit(&tSeed);
	SeedSet(&tSeed, dwSeed);
	ptRecord->nCost = sgcanMonKillsStart[nKind][0];
	ptRecord->nLevel = sgcanMonKillsStart[nKind][1];

	nBase = sgcanMonKillsBaseA[nKind];
	nRow = (sgcanMonKillsRangeA[nKind] ? (int)SeedRand(&tSeed, sgcanMonKillsRangeA[nKind]) : 0) + nBase;
	ptRecord->dwComponentTR = sgcatMonKillsRowsA[nRow].dwComponent;
	ptRecord->nLife = sgcatMonKillsRowsA[nRow].dwStatValue;
	ptRecord->nLevel += sgcatMonKillsRowsA[nRow].dwLevelBonus;
	ptRecord->nCost += sgcatMonKillsRowsA[nRow].dwCostBonus;

	nBase = sgcanMonKillsBaseB[nKind];
	nRow = (sgcanMonKillsRangeB[nKind] ? (int)SeedRand(&tSeed, sgcanMonKillsRangeB[nKind]) : 0) + nBase;
	ptRecord->dwComponentHD = sgcatMonKillsRowsB[nRow].dwComponent;
	ptRecord->nDefense = sgcatMonKillsRowsB[nRow].dwStatValue;
	ptRecord->nLevel += sgcatMonKillsRowsB[nRow].dwLevelBonus;
	ptRecord->nCost += sgcatMonKillsRowsB[nRow].dwCostBonus;

	nBase = sgcanMonKillsBaseC[nKind];
	nRow = (sgcanMonKillsRangeC[nKind] ? (int)SeedRand(&tSeed, sgcanMonKillsRangeC[nKind]) : 0) + nBase;
	if (nKind < 3)
	{
		ptRecord->dwComponentLH = sgcatMonKillsRowsC[nRow].dwHandVariation;
		ptRecord->dwComponentRH = 0;

	}
	else
	{
		ptRecord->dwComponentLH = 0;
		ptRecord->dwComponentRH = sgcatMonKillsRowsC[nRow].dwHandVariation;
	}
	ptRecord->nDamageMin = sgcatMonKillsRowsC[nRow].dwDamageMin;
	ptRecord->nDamageMax = sgcatMonKillsRowsC[nRow].dwDamageMax;
	if (sgcatMonKillsRowsC[nRow].field_04 == 0)
		ptRecord->wFlags |= 1;
	ptRecord->nLevel += sgcatMonKillsRowsC[nRow].dwLevel;
	ptRecord->nCost += sgcatMonKillsRowsC[nRow].dwCost;

	nBase = sgcanMonKillsBaseD[nKind];
	nRow = (sgcanMonKillsRangeD[nKind] ? (int)SeedRand(&tSeed, sgcanMonKillsRangeD[nKind]) : 0) + nBase;
	nHireSpecial = sgcatMonKillsRowsD[nRow].dwHireSpecial;
	ptRecord->nHireSpecial = nHireSpecial;
	ptRecord->nSkillLevel = sgcatMonKillsRowsD[nRow].dwSkillLevel;
	if (nHireSpecial >= 9 && nHireSpecial <= 11)
		ptRecord->dwComponentSH = 1;
	else
		ptRecord->dwComponentSH = 0;
	ptRecord->nLevel += sgcatMonKillsRowsD[nRow].dwLevelBonus;
	ptRecord->nCost += sgcatMonKillsRowsD[nRow].dwCostBonus;

	if (bDifficulty)
	{
		ptRecord->nLife += sgcatMonKillsDifficulty[bDifficulty].nLifeBonus;
		ptRecord->nDefense += sgcatMonKillsDifficulty[bDifficulty].nDefenseBonus;
		ptRecord->nLevel += sgcatMonKillsDifficulty[bDifficulty].nLevelBonus;
		ptRecord->nDamageMin *= sgcatMonKillsDifficulty[bDifficulty].nDamageMinFactor;
		ptRecord->nDamageMax *= sgcatMonKillsDifficulty[bDifficulty].nDamageMaxFactor;
		ptRecord->nSkillLevel += sgcatMonKillsDifficulty[bDifficulty].nSkillLevelBonus;
		ptRecord->nCost *= sgcatMonKillsDifficulty[bDifficulty].nCostFactor;
	}
	return TRUE;
}

WCHAR * __fastcall MonsterKillsLogGetString(DWORD nIndex)
{
	WORD wStringId;

	wStringId = (WORD)sgcatMonKillsRowsD[nIndex].dwStringId;
	if (wStringId == 0)
		return NULL;
	return (WCHAR *)D2LangStrGetStringByIndex(wStringId);
}

int __fastcall MonsterKillsLogClassify(DWORD dwClassId, WORD wNameString)
{
	switch (dwClassId)
	{
	case 271:
		return 0;
	case 338:
		return 1;
	case 359:
		return 2;
	}
	if (wNameString >= 3411 && wNameString <= 3451)
		return 0;
	if (wNameString >= 1019 && wNameString <= 1039)
		return 1;
	if (wNameString >= 1040 && wNameString <= 1059)
		return 2;
	return -1;
}

void __stdcall MonsterKillsLogSetFlags(DWORD dwPlayer, int eType)
{
	DWORD *pdw;

	pdw = (DWORD *)D2CommonUnitGetPlrInfo(dwPlayer);
#line 25
	D2ASSERT(eType < NUM_MONSTERCLASS);
	pdw[10 + eType] |= 0xF0000000;
}

void __stdcall MonsterKillsLogIncrement(DWORD dwPlayer, int eType)
{
	DWORD *pdw;
	DWORD dwFlags, dwOldCount;

	pdw = (DWORD *)D2CommonUnitGetPlrInfo(dwPlayer);
#line 38
	D2ASSERT(eType < NUM_MONSTERCLASS);
	dwFlags = pdw[10 + eType] & 0xF0000000;
	dwOldCount = pdw[10 + eType] & 268435455;
	pdw[10 + eType] = dwOldCount + 1;
	if (dwOldCount > 1000000)
		pdw[10 + eType] = 1000000;
	pdw[10 + eType] |= dwFlags;
}

DWORD __stdcall MonsterKillsLogGetCount(DWORD dwPlayer, int eType)
{
	DWORD *pdw;

	pdw = (DWORD *)D2CommonUnitGetPlrInfo(dwPlayer);
#line 58
	D2ASSERT(eType < NUM_MONSTERCLASS);
	return pdw[10 + eType] & 268435455;
}

BOOL __stdcall MonsterKillsLogTestFlags(DWORD dwPlayer, int eType)
{
	DWORD *pdw;

	pdw = (DWORD *)D2CommonUnitGetPlrInfo(dwPlayer);
#line 72
	D2ASSERT(eType < NUM_MONSTERCLASS);
	return (pdw[10 + eType] & 0xF0000000) != 0;
}

void __stdcall MonsterKillsLogClearRecord(DWORD dwPlayer)
{
	DWORD *pdw;
	int i;

	pdw = (DWORD *)D2CommonUnitGetPlrInfo(dwPlayer);
	for (i = 0; i < 410; i++)
		pdw[10 + i] = 0;
}
