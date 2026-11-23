#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\Arena.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

DWORD __fastcall ArenaGetZero(void)
{
	return 0;
}

typedef int CHARTEMPLATELEVELS[64];

static CHARTEMPLATELEVELS sgnCharTemplateLevel = { 0 };
static PTCHARTEMPLATETXT sgptCharTemplates = NULL;
static int sgnCharTemplates = 0;

static const char sgcszClassCodes[NUM_PLAYERCLASS][4] =
{
	"ama", "sor", "nec", "pal", "bar",
};

void __fastcall ArenaLoadCharTemplate(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[100] =
	{
		{ "Name", EXCEL_TYPE_STRING, 29, offsetof(CHARTEMPLATETXT, Name) },
		{ "class", EXCEL_TYPE_STRING, 3, offsetof(CHARTEMPLATETXT, Class) },
		{ "act", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, act) },
		{ "level", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, level) },
		{ "str", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, str) },
		{ "dex", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, dex) },
		{ "vit", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, vit) },
		{ "int", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, Int) },
		{ "Hitpoints", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, Hitpoints) },
		{ "Mana", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, Mana) },
		{ "ManaRegenBonus", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, ManaRegenBonus) },
		{ "Velocity", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, Velocity) },
		{ "AttackRate", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, AttackRate) },
		{ "OtherRate", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, OtherRate) },
		{ "RightSkill", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, RightSkill) },
		{ "Skill1", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[0]) },
		{ "SkillLevel1", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[0]) },
		{ "Skill2", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[1]) },
		{ "SkillLevel2", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[1]) },
		{ "Skill3", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[2]) },
		{ "SkillLevel3", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[2]) },
		{ "Skill4", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[3]) },
		{ "SkillLevel4", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[3]) },
		{ "Skill5", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[4]) },
		{ "SkillLevel5", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[4]) },
		{ "Skill6", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[5]) },
		{ "SkillLevel6", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[5]) },
		{ "Skill7", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[6]) },
		{ "SkillLevel7", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[6]) },
		{ "Skill8", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[7]) },
		{ "SkillLevel8", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[7]) },
		{ "Skill9", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, Skill[8]) },
		{ "SkillLevel9", EXCEL_TYPE_DWORD, 0, offsetof(CHARTEMPLATETXT, SkillLevel[8]) },
		{ "item1", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item1) },
		{ "item1loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item1loc) },
		{ "item1count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item1count) },
		{ "item2", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item2) },
		{ "item2loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item2loc) },
		{ "item2count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item2count) },
		{ "item3", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item3) },
		{ "item3loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item3loc) },
		{ "item3count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item3count) },
		{ "item4", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item4) },
		{ "item4loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item4loc) },
		{ "item4count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item4count) },
		{ "item5", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item5) },
		{ "item5loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item5loc) },
		{ "item5count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item5count) },
		{ "item6", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item6) },
		{ "item6loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item6loc) },
		{ "item6count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item6count) },
		{ "item7", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item7) },
		{ "item7loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item7loc) },
		{ "item7count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item7count) },
		{ "item8", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item8) },
		{ "item8loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item8loc) },
		{ "item8count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item8count) },
		{ "item9", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item9) },
		{ "item9loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item9loc) },
		{ "item9count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item9count) },
		{ "item10", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item10) },
		{ "item10loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item10loc) },
		{ "item10count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item10count) },
		{ "item11", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item11) },
		{ "item11loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item11loc) },
		{ "item11count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item11count) },
		{ "item12", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item12) },
		{ "item12loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item12loc) },
		{ "item12count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item12count) },
		{ "item13", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item13) },
		{ "item13loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item13loc) },
		{ "item13count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item13count) },
		{ "item14", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item14) },
		{ "item14loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item14loc) },
		{ "item14count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item14count) },
		{ "item15", EXCEL_TYPE_CODE, 0, offsetof(CHARTEMPLATETXT, item15) },
		{ "item15loc", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item15loc) },
		{ "item15count", EXCEL_TYPE_BYTE, 0, offsetof(CHARTEMPLATETXT, item15count) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTCHARTEMPLATETXT ptTemplate;
	PTCHARTEMPLATETXT ptTemplates;
	int nLevel;
	int nMaxLevel;
	int i;
	int k;

	ptTemplates = (PTCHARTEMPLATETXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(CHARTEMPLATETXT));
	ptTemplate = ptTemplates;
	for (i = nRows; i > 0; i--, ptTemplate++)
	{
		for (k = 0; k < NUM_PLAYERCLASS; k++)
		{
			if (!strcmp(sgcszClassCodes[k], ptTemplate->Class))
				ptTemplate->nClassId = k;
		}
	}
	for (i = 0; i < (int)arrsize(sgnCharTemplateLevel); i++)
		sgnCharTemplateLevel[i] = 0;
	nMaxLevel = 0;
	for (i = 0; i < nRows; i++)
	{
		nLevel = ptTemplates[i].level;
#line 117
		D2ASSERT(nLevel >= nMaxLevel);
		if (nLevel > nMaxLevel)
		{
			sgnCharTemplateLevel[nLevel] = i;
			nMaxLevel = nLevel;
		}
	}
	sgptCharTemplates = ptTemplates;
	sgnCharTemplates = nRows;
}

void __fastcall ArenaFreeCharTemplate(void)
{
	DataTblsFreeBlock(sgptCharTemplates);
	sgptCharTemplates = NULL;
}

int __fastcall ArenaGetCharTemplateCount(void)
{
	return sgnCharTemplates;
}

PTCHARTEMPLATETXT __fastcall ArenaGetCharTemplate(int nTemplate, int nLevel)
{
	int nIndex;

#line 252
	D2ASSERT(nTemplate > 0);
	nIndex = sgnCharTemplateLevel[nLevel] + nTemplate - 1;
#line 255
	D2ASSERT(nIndex < sgnCharTemplates);
	return &sgptCharTemplates[nIndex];
}

int __fastcall ArenaGetCharTemplateClass(int nTemplate, int nLevel)
{
	return ArenaGetCharTemplate(nTemplate, nLevel)->nClassId;
}
