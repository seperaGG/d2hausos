#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\DataTbls.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

struct DATATABLES
{
 	PTCHARSTATSTXT	ptCharStatsTxt;
 	PTSKILLSTXT	ptSkillsTxt;
 	int			nNumSkills;
 	PTANIMDATATBL	ptAnimData;
 	PTEXPERIENCETXT	ptExperienceTxt;
 	PTDIFFICULTYLEVELSTXT	ptDifficultyLevelsTxt;
 	int			nNumDifficultyLevels;
};
typedef struct DATATABLES DATATABLES;
static DATATABLES sgtDataTables;

#define MAX_SKILL_RESTRICTED_STATES		2
static const int sgcnRestrictedStates[MAX_SKILL_RESTRICTED_STATES] =
{
	0, 0,
};

DWORD __stdcall DataTblsGetExperience(int nClass, int nLevel)
{
	return (DWORD)(&sgtDataTables.ptExperienceTxt[nLevel + 1].Amazon)[nClass];
}

int __stdcall DataTblsGetMaxLevel(int nClass)
{
	return (&sgtDataTables.ptExperienceTxt[0].Amazon)[nClass];
}

int __stdcall DataTblsGetLevelFromExperience(int nClass, DWORD dwExperience)
{
	int nMaxLevel;
	PTEXPERIENCETXT ptRow;
	int nLevel;

	nMaxLevel = (&sgtDataTables.ptExperienceTxt[0].Amazon)[nClass];
	ptRow = &sgtDataTables.ptExperienceTxt[1];
	for (nLevel = 0; dwExperience >= (DWORD)(&ptRow->Amazon)[nClass]; nLevel++)
	{
		if (nLevel == nMaxLevel)
			break;
		ptRow++;
	}
	return nLevel;
}

PTCHARSTATSTXT __fastcall DataTblsGetCharStatsTxt(void)
{
	return sgtDataTables.ptCharStatsTxt;
}

PTCHARSTATSTXT __stdcall DataTblsGetCharStats(int nClass)
{
	return &sgtDataTables.ptCharStatsTxt[nClass];
}

PTSKILLSTXT __stdcall DataTblsGetSkill(int nSkill)
{
	return &sgtDataTables.ptSkillsTxt[nSkill];
}

int __fastcall DataTblsGetSkillCount(void)
{
	return sgtDataTables.nNumSkills;
}

PTANIMDATATBL __fastcall DataTblsGetAnimData(void)
{
	return sgtDataTables.ptAnimData;
}

static void __fastcall DataTblsLoadExperience(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Amazon", EXCEL_TYPE_DWORD, 0, offsetof(EXPERIENCETXT, Amazon) },
		{ "Sorceress", EXCEL_TYPE_DWORD, 0, offsetof(EXPERIENCETXT, Sorceress) },
		{ "Necromancer", EXCEL_TYPE_DWORD, 0, offsetof(EXPERIENCETXT, Necromancer) },
		{ "Paladin", EXCEL_TYPE_DWORD, 0, offsetof(EXPERIENCETXT, Paladin) },
		{ "Barbarian", EXCEL_TYPE_DWORD, 0, offsetof(EXPERIENCETXT, Barbarian) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;

	sgtDataTables.ptExperienceTxt = (PTEXPERIENCETXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer,
		nBufferSize,
		&nRows, sizeof(EXPERIENCETXT));
}

static __inline int DataTblsGetRestrictedState(int nLoadedValue)
{
#line 339
	D2ASSERT(nLoadedValue < MAX_SKILL_RESTRICTED_STATES);
	return sgcnRestrictedStates[nLoadedValue];
}

static PTSKILLSTXT DataTblsFindSkillRow(PTSKILLSTXT ptSkills, int nRows, char *szName)
{
	PTSKILLSTXT ptRow;
	DWORD j;

	if (szName[0])
	{
		ptRow = ptSkills;
		for (j = 0; j < (DWORD)nRows; ptRow++, j++)
		{
			if (!strcmp(ptRow->skill, szName))
				return ptRow;
		}
	}
	return NULL;
}

static __inline int DataTblsGetMonAnimMode(char *szAnim)
{
	int nMonMode;

	nMonMode = 7;
	if (szAnim[0] == 'a')
	{
		nMonMode = 4;
		if (szAnim[1] == '2')
			nMonMode = 5;
	}
	if (szAnim[0] == 'r')
		nMonMode = 15;
	if (szAnim[0] == 's')
	{
		if (szAnim[1] == 'e')
			nMonMode = 14;
		else if (szAnim[1] == 'k')
		{
			if (szAnim[2] == '1')
				nMonMode = 8;
			if (szAnim[2] == '2')
				nMonMode = 9;
			if (szAnim[2] == '3')
				nMonMode = 10;
			if (szAnim[2] == '4')
				nMonMode = 11;
		}
	}
	return nMonMode;
}

static int __fastcall DataTblsLoadSkills(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "skill", EXCEL_TYPE_STRING, 30, offsetof(SKILLSTXT, skill) },
		{ "charclass", EXCEL_TYPE_STRING, 30, offsetof(SKILLSTXT, charclass) },
		{ "ClassReq", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, ClassReq) },
		{ "AttackRank", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, AttackRank) },
		{ "itemclass1", EXCEL_TYPE_CODE, 0, offsetof(SKILLSTXT, itemclass[0]) },
		{ "itemclass2", EXCEL_TYPE_CODE, 0, offsetof(SKILLSTXT, itemclass[1]) },
		{ "itemclass3", EXCEL_TYPE_CODE, 0, offsetof(SKILLSTXT, itemclass[2]) },
		{ "itemclass4", EXCEL_TYPE_CODE, 0, offsetof(SKILLSTXT, itemclass[3]) },
		{ "itemclass5", EXCEL_TYPE_CODE, 0, offsetof(SKILLSTXT, itemclass[4]) },
		{ "itemclass6", EXCEL_TYPE_CODE, 0, offsetof(SKILLSTXT, itemclass[5]) },
		{ "anim", EXCEL_TYPE_STRING, 7, offsetof(SKILLSTXT, anim) },
		{ "monanim", EXCEL_TYPE_STRING, 7, offsetof(SKILLSTXT, monanim) },
		{ "seqnum", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, seqnum) },
		{ "Durability", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, Durability) },
		{ "Shiver", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, Shiver) },
		{ "UseAttackRate", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, UseAttackRate) },
		{ "LineOfSight", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, LineOfSight) },
		{ "TargetableOnly", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, TargetableOnly) },
		{ "SearchEnemyXY", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, SearchEnemyXY) },
		{ "SearchMonsterNear", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, SearchMonsterNear) },
		{ "SelectDead", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, SelectDead) },
		{ "SearchOpenXY", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, SearchOpenXY) },
		{ "TargetPet", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, TargetPet) },
		{ "TargetAlly", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, TargetAlly) },
		{ "AttackNoMana", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, AttackNoMana) },
		{ "range", EXCEL_TYPE_STRING, 7, offsetof(SKILLSTXT, range) },
		{ "reqlevel", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, reqlevel) },
		{ "reqstr", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, reqstr) },
		{ "reqdex", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, reqdex) },
		{ "reqint", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, reqint) },
		{ "reqvit", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, reqvit) },
		{ "reqskill1", EXCEL_TYPE_STRING, 30, offsetof(SKILLSTXT, reqskill1) },
		{ "reqskill2", EXCEL_TYPE_STRING, 30, offsetof(SKILLSTXT, reqskill2) },
		{ "reqskill3", EXCEL_TYPE_STRING, 30, offsetof(SKILLSTXT, reqskill3) },
		{ "state1", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, state1) },
		{ "state2", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, state2) },
		{ "state3", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, state3) },
		{ "skillpage", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, skillpage) },
		{ "skillrow", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, skillrow) },
		{ "skillcolumn", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, skillcolumn) },
		{ "iconcel", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, iconcel) },
		{ "leftskl", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, leftskl) },
		{ "manashift", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, manashift) },
		{ "mana", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, mana) },
		{ "lvlmana", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, lvlmana) },
		{ "Interrupt", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, Interrupt) },
		{ "InTown", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, InTown) },
		{ "Periodic", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, Periodic) },
		{ "Passive", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, Passive) },
		{ "Param1", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Param[0]) },
		{ "Param2", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Param[1]) },
		{ "Param3", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Param[2]) },
		{ "Param4", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Param[3]) },
		{ "Param5", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Param[4]) },
		{ "Param6", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Param[5]) },
		{ "InGame", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, InGame) },
		{ "Open", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Open) },
		{ "Beta", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, Beta) },
		{ "ToHit", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, ToHit) },
		{ "LevToHit", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, LevToHit) },
		{ "HitShift", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, HitShift) },
		{ "SrcDam", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, SrcDam) },
		{ "MinDam", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, MinDam) },
		{ "MaxDam", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, MaxDam) },
		{ "LevDam", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, LevDam) },
		{ "EType", EXCEL_TYPE_BYTE, 0, offsetof(SKILLSTXT, EType) },
		{ "EMin", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, EMin) },
		{ "EMax", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, EMax) },
		{ "ELev", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, ELev) },
		{ "ELen", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, ELen) },
		{ "ELevLen", EXCEL_TYPE_DWORD, 0, offsetof(SKILLSTXT, ELevLen) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTSKILLSTXT ptSkills;
	PTSKILLSTXT ptSkill;
	int nMode;
	int nMonMode;
	int nRange;
	DWORD i;
	DWORD j;
	int k;

	ptSkills = (PTSKILLSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(SKILLSTXT));
	ptSkill = ptSkills;
	for (i = 0; i < (DWORD)nRows; ptSkill++, i++)
	{
		ptSkill->dwSkillId = i;

		ptSkill->ptReqSkill1 = DataTblsFindSkillRow(ptSkills, nRows, ptSkill->reqskill1);
		ptSkill->ptReqSkill2 = DataTblsFindSkillRow(ptSkills, nRows, ptSkill->reqskill2);
		ptSkill->ptReqSkill3 = DataTblsFindSkillRow(ptSkills, nRows, ptSkill->reqskill3);

		nMonMode = DataTblsGetMonAnimMode(ptSkill->monanim);

		if (!strncmp(ptSkill->charclass, "monst", 5))
			nMode = 18;
		else
		{
			nMode = 10;
			if (ptSkill->anim[0] == 'a')
			{
				nMode = 7;
				if (ptSkill->anim[1] == '2')
					nMode = 8;
			}
			if (ptSkill->anim[0] == 'k')
				nMode = 12;
			if (ptSkill->anim[0] == 't')
				nMode = 11;
			if (ptSkill->anim[0] == 'r')
				nMode = 3;
			if (ptSkill->anim[0] == 's')
			{
				if (ptSkill->anim[1] == 'e')
					nMode = 18;
				else if (ptSkill->anim[1] == 'k')
				{
					if (ptSkill->anim[2] == '1')
						nMode = 13;
					if (ptSkill->anim[2] == '2')
						nMode = 14;
					if (ptSkill->anim[2] == '3')
						nMode = 15;
					if (ptSkill->anim[2] == '4')
						nMode = 16;
				}
			}
		}
		ptSkill->dwAnimMode = nMode;
		ptSkill->dwMonAnimMode = nMonMode;

		switch (ptSkill->range[0])
		{
		case 'b':
			nRange = 3;
			break;
		case 'h':
			nRange = 1;
			break;
		case 'r':
			nRange = 2;
			break;
		case 'l':
			nRange = 4;
			break;
		default:
			nRange = 0;
			break;
		}
		ptSkill->dwRangeType = nRange;

		ptSkill->dwNeedsShield = 0;
		ptSkill->dwNotBow = 0;
		for (k = 0; k < 6; k++)
		{
			switch (ptSkill->itemclass[k])
			{
			case 544694817:
				ptSkill->dwNotBow = 1;
				break;
			case 543453994:
				ptSkill->dwNeedsShield = 1;
				break;
			}
		}

		ptSkill->state1 = DataTblsGetRestrictedState(ptSkill->state1);
		ptSkill->state2 = DataTblsGetRestrictedState(ptSkill->state2);
		ptSkill->state3 = DataTblsGetRestrictedState(ptSkill->state3);
	}
	sgtDataTables.ptSkillsTxt = ptSkills;
	sgtDataTables.nNumSkills = nRows;
	return nRows;
}

static void __fastcall DataTblsLoadDifficultyLevels(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "ResistPenalty", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, ResistPenalty) },
		{ "DeathExpPenalty", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, DeathExpPenalty) },
		{ "UberCodeOddsNormal", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, UberCodeOddsNormal) },
		{ "UberCodeOddsGood", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, UberCodeOddsGood) },
		{ "MonsterSkillBonus", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, MonsterSkillBonus) },
		{ "MonsterFreezeDivisor", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, MonsterFreezeDivisor) },
		{ "MonsterColdDivisor", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, MonsterColdDivisor) },
		{ "AiCurseDivisor", EXCEL_TYPE_DWORD, 0, offsetof(DIFFICULTYLEVELSTXT, AiCurseDivisor) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSize;

	sgtDataTables.ptDifficultyLevelsTxt = (PTDIFFICULTYLEVELSTXT)DataTblsAllocAndTokenize(tDesc,
		(char *)pBuffer,
		nBufferSize, &nSize, sizeof(DIFFICULTYLEVELSTXT));
	sgtDataTables.nNumDifficultyLevels = nSize;
#line 556
	D2ASSERT(nSize == NUM_DIFFICULTY_LEVELS);
}

PTDIFFICULTYLEVELSTXT __stdcall DataTblsGetDifficultyLevel(int nDifficultyLevel)
{
#line 564
	D2ASSERT(nDifficultyLevel < sgtDataTables.nNumDifficultyLevels && nDifficultyLevel >= 0);
	return &sgtDataTables.ptDifficultyLevelsTxt[nDifficultyLevel];
}

void __stdcall DataTblsNothing(DWORD dwUnused1, DWORD dwUnused2, DWORD dwUnused3, DWORD dwUnused4)
{
}

void * __stdcall DataTblsLoadExcelFile(HD2ARCHIVE hArchive, char *pszName, int *pnSize)
{
	char szExt[] = ".txt";
	char szPath[MAX_PATH];

	wsprintfA(szPath, "%s\\%s%s", "DATA\\GLOBAL\\EXCEL", pszName, szExt);
#line 596
	return ArchiveAllocBufferAndRead(hArchive, szPath, (DWORD *)pnSize, D2_THIS_FILE, __LINE__);
}

static void __fastcall DataTblsLoadCharStats(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tCharStatsDesc[] =
	{
		{ "class", EXCEL_TYPE_STRING, 15, offsetof(CHARSTATSTXT, Class) },
		{ "str", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, str) },
		{ "dex", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, dex) },
		{ "vit", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, vit) },
		{ "int", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, Int) },
		{ "hpadd", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, hpadd) },
		{ "PercentStr", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, PercentStr) },
		{ "PercentDex", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, PercentDex) },
		{ "PercentVit", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, PercentVit) },
		{ "PercentInt", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, PercentInt) },
		{ "ManaRegen", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, ManaRegen) },
		{ "ToHitFactor", EXCEL_TYPE_DWORD, 0, offsetof(CHARSTATSTXT, ToHitFactor) },
		{ "WalkVelocity", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, WalkVelocity) },
		{ "RunVelocity", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, RunVelocity) },
		{ "RunDrain", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, RunDrain) },
		{ "LifePerLevel", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, LifePerLevel) },
		{ "StaminaPerLevel", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, StaminaPerLevel) },
		{ "ManaPerLevel", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, ManaPerLevel) },
		{ "LifePerVitality", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, LifePerVitality) },
		{ "StaminaPerVitality", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, StaminaPerVitality) },
		{ "ManaPerMagic", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, ManaPerMagic) },
		{ "BlockFactor", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, BlockFactor) },
		{ "StartSkill", EXCEL_TYPE_DWORD, 0, offsetof(CHARSTATSTXT, StartSkill) },
		{ "basewclass", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, basewclass) },
		{ "item1", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item1) },
		{ "item1loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item1loc) },
		{ "item1count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item1count) },
		{ "item2", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item2) },
		{ "item2loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item2loc) },
		{ "item2count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item2count) },
		{ "item3", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item3) },
		{ "item3loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item3loc) },
		{ "item3count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item3count) },
		{ "item4", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item4) },
		{ "item4loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item4loc) },
		{ "item4count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item4count) },
		{ "item5", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item5) },
		{ "item5loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item5loc) },
		{ "item5count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item5count) },
		{ "item6", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item6) },
		{ "item6loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item6loc) },
		{ "item6count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item6count) },
		{ "item7", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item7) },
		{ "item7loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item7loc) },
		{ "item7count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item7count) },
		{ "item8", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item8) },
		{ "item8loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item8loc) },
		{ "item8count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item8count) },
		{ "item9", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item9) },
		{ "item9loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item9loc) },
		{ "item9count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item9count) },
		{ "item10", EXCEL_TYPE_CODE, 0, offsetof(CHARSTATSTXT, item10) },
		{ "item10loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item10loc) },
		{ "item10count", EXCEL_TYPE_BYTE, 0, offsetof(CHARSTATSTXT, item10count) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTCHARSTATSTXT ptCharStats;
	DWORD i;

	ptCharStats = (PTCHARSTATSTXT)DataTblsAllocAndTokenize(tCharStatsDesc, (char *)pBuffer, nBufferSize,
		&nRows, sizeof(CHARSTATSTXT));
	for (i = 0; i < (DWORD)nRows; i++)
	{
		memset(ptCharStats[i].wszClassName, 0, 16);
		(void)D2LangUnicodestrncpy(ptCharStats[i].wszClassName,
			D2LangStrGetStringByKey(ptCharStats[i].Class), 16);
	}
	sgtDataTables.ptCharStatsTxt = ptCharStats;
}

void * __stdcall DataTblsAllocAndTokenize(void *pDescriptors, char *pBuffer, int nSize, int *pnRows,
	int nStride)
{
	EXCELFILE *ptFile;
	int nRows;
	int nByteSize;
	void *pRecords;

	ptFile = (EXCELFILE *)D2CommonExcelOpen(pBuffer, nSize);
	nRows = (int)D2CommonExcelFileGetRowCount(ptFile);
	nByteSize = nRows * nStride;
#line 696
	pRecords = D2ALLOCPOOL(NULL, nByteSize);
	memset(pRecords, 0, nByteSize);
	D2CommonExcelParseRecords(ptFile, pDescriptors, pRecords, nRows, nStride);
	D2CommonExcelFileFree(ptFile);
	*pnRows = nRows;
	return pRecords;
}

void __stdcall DataTblsFreeBlock(void *p)
{
#line 713
	D2FREEPOOL(NULL, p);
}

void __stdcall DataTblsFreeAll(void)
{
	DataTblsFreeBlock(sgtDataTables.ptCharStatsTxt);
	MissileTblsFree();
	OverlayTblsFree();
	DataTblsFreeBlock(sgtDataTables.ptSkillsTxt);
	ItemTblsFreeItems();
	ItemTblsFreeMagicAffixes();
	ItemTblsFreeRareAffixes();
	ItemTblsFreeUniqueItems();
	ItemTblsFreeSetItems();
	ItemTblsFreeQualityItems();
	ItemTblsFreeLowQualityItems();
	ItemTblsFreeGems();
	ItemTblsFreeBooks();
	ItemTblsFreeItemRatio();
	ItemTblsFreeItemStatCost();
	ItemTblsFreeGamble();
	TokTblsFree();
	DataTblsFreeBlock(sgtDataTables.ptExperienceTxt);
	AnimTblsFree(sgtDataTables.ptAnimData);
	MonsterTblsFreeMonStats();
	LvlTblsFreeLevels();
	LvlTblsFreeLevelDefs();
	LvlTblsFreeLvlTypes();
	LvlTblsFreeLvlPrest();
	LvlTblsFreeLvlWarp();
	LvlTblsFreeLvlMaze();
	LvlTblsFreeLvlTypeFiles();
	LvlTblsFreeLvlSub();
	LvlTblsFreeAutomap();
	ObjTblsFreeObjects();
	ObjTblsFreeObjGroup();
	ObjTblsFreeShrines();
	InvTblsFree();
	FieldTblsFree();
	BeltTblsFree();
	MonsterTblsFreeTreasureClass();
	MonsterTblsFreeMonItemPercent();
	ArenaFreeCharTemplate();
	TokTblsFreeNothing();
	DataTblsFreeBlock(sgtDataTables.ptDifficultyLevelsTxt);
}

void __stdcall DataTblsLoadAll(HD2ARCHIVE hArchive, DWORD dwLoadFiles, DWORD dwLoadSubFiles)
{
	char szPath[MAX_PATH];
	void *pBuffer;
	void *pBuffer2;
	void *pBuffer3;
	int nSize;
	int nWeaponsSize;
	int nArmorSize;
	int nMiscSize;
	int nSuffixSize;
	int nPrefixSize;
	int nTypeSize;
	int nModeSize;

	pBuffer = DataTblsLoadExcelFile(hArchive, "charstats", &nSize);
	DataTblsLoadCharStats(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "missiles", &nSize);
	MissileTblsLoad(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "overlay", &nSize);
	OverlayTblsLoad(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "skills", &nSize);
	DataTblsLoadSkills(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "chartemplate", &nSize);
	ArenaLoadCharTemplate(pBuffer, nSize);

	pBuffer = DataTblsLoadExcelFile(hArchive, "weapons", &nWeaponsSize);
	pBuffer2 = DataTblsLoadExcelFile(hArchive, "armor", &nArmorSize);
	pBuffer3 = DataTblsLoadExcelFile(hArchive, "misc", &nMiscSize);
	ItemTblsLoadItems(pBuffer, nWeaponsSize, pBuffer2, nArmorSize, pBuffer3, nMiscSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "magicsuffix", &nSuffixSize);
	pBuffer2 = DataTblsLoadExcelFile(hArchive, "magicprefix", &nPrefixSize);
	ItemTblsLoadMagicAffixes(pBuffer, nSuffixSize, pBuffer2, nPrefixSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "raresuffix", &nSuffixSize);
	pBuffer2 = DataTblsLoadExcelFile(hArchive, "rareprefix", &nPrefixSize);
	ItemTblsLoadRareAffixes(pBuffer, nSuffixSize, pBuffer2, nPrefixSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "uniqueitems", &nSize);
	ItemTblsLoadUniqueItems(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "setitems", &nSize);
	ItemTblsLoadSetItems(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "gems", &nSize);
	ItemTblsLoadGems(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "books", &nSize);
	ItemTblsLoadBooks(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "qualityitems", &nSize);
	ItemTblsLoadQualityItems(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "lowqualityitems", &nSize);
	ItemTblsLoadLowQualityItems(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "itemratio", &nSize);
	ItemTblsLoadItemRatio(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "itemstatcost", &nSize);
	ItemTblsLoadItemStatCost(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "gamble", &nSize);
	ItemTblsLoadGamble(pBuffer, nSize);

	pBuffer = DataTblsLoadExcelFile(hArchive, "plrtype", &nTypeSize);
	pBuffer2 = DataTblsLoadExcelFile(hArchive, "plrmode", &nModeSize);
	TokTblsLoadPlrTypeModes(pBuffer, nTypeSize, pBuffer2, nModeSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "montype", &nTypeSize);
	pBuffer2 = DataTblsLoadExcelFile(hArchive, "monmode", &nModeSize);
	TokTblsLoadMonTypeModes(pBuffer, nTypeSize, pBuffer2, nModeSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "objtype", &nTypeSize);
	pBuffer2 = DataTblsLoadExcelFile(hArchive, "objmode", &nModeSize);
	TokTblsLoadObjTypeModes(pBuffer, nTypeSize, pBuffer2, nModeSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "composit", &nSize);
	TokTblsLoadComposit(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "armtype", &nSize);
	TokTblsLoadArmType(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "experience", &nSize);
	DataTblsLoadExperience(pBuffer, nSize);
	sgtDataTables.ptAnimData = AnimTblsLoad(hArchive);

	pBuffer = DataTblsLoadExcelFile(hArchive, "monstats", &nSize);
	MonsterTblsLoadMonStats(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "levels", &nSize);
	LvlTblsLoadLevels(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "levels", &nSize);
	LvlTblsLoadLevelDefs(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "lvltypes", &nSize);
	LvlTblsLoadLvlTypes(pBuffer, nSize);
	if (dwLoadFiles)
		LvlTblsLoadLvlTypeFiles(hArchive);
	pBuffer = DataTblsLoadExcelFile(hArchive, "lvlprest", &nSize);
	LvlTblsLoadLvlPrest(pBuffer, nSize, dwLoadFiles);
	pBuffer = DataTblsLoadExcelFile(hArchive, "lvlwarp", &nSize);
	LvlTblsLoadLvlWarp(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "lvlmaze", &nSize);
	LvlTblsLoadLvlMaze(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "lvlsub", &nSize);
	LvlTblsLoadLvlSub(hArchive, pBuffer, nSize, dwLoadFiles, dwLoadSubFiles);
	pBuffer = DataTblsLoadExcelFile(hArchive, "automap", &nSize);
	LvlTblsLoadAutomap(hArchive, pBuffer, nSize);

	pBuffer = DataTblsLoadExcelFile(hArchive, "objects", &nSize);
	ObjTblsLoadObjects(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "objgroup", &nSize);
	ObjTblsLoadObjGroup(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "shrines", &nSize);
	ObjTblsLoadShrines(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "inventory", &nSize);
	InvTblsLoad(pBuffer, nSize);

	wsprintfA(szPath, "%s\\expfield.d2", "DATA\\GLOBAL");
#line 1000
	pBuffer = ArchiveAllocBufferAndRead(hArchive, szPath, (DWORD *)&nSize, D2_THIS_FILE, __LINE__);
	FieldTblsLoad(pBuffer, nSize);

	pBuffer = DataTblsLoadExcelFile(hArchive, "belts", &nSize);
	BeltTblsLoad(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "uniquetitle", &nSize);
	MonsterTblsLoadUniqueNames(pBuffer, nSize, 0);
	pBuffer = DataTblsLoadExcelFile(hArchive, "uniqueprefix", &nSize);
	MonsterTblsLoadUniqueNames(pBuffer, nSize, 1);
	pBuffer = DataTblsLoadExcelFile(hArchive, "uniquesuffix", &nSize);
	MonsterTblsLoadUniqueNames(pBuffer, nSize, 2);
	pBuffer = DataTblsLoadExcelFile(hArchive, "uniqueappellation", &nSize);
	MonsterTblsLoadUniqueNames(pBuffer, nSize, 3);
	pBuffer = DataTblsLoadExcelFile(hArchive, "treasureclass", &nSize);
	MonsterTblsLoadTreasureClass(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "monitempercent", &nSize);
	MonsterTblsLoadMonItemPercent(pBuffer, nSize);
	pBuffer = DataTblsLoadExcelFile(hArchive, "superuniques", &nSize);
	MonsterTblsLoadSuperUniques(pBuffer, nSize);
	TokTblsLoadNothing(hArchive);
	pBuffer = DataTblsLoadExcelFile(hArchive, "difficultylevels", &nSize);
	DataTblsLoadDifficultyLevels(pBuffer, nSize);
}
