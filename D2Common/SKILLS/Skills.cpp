#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\SKILLS\\Skills.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "SkillsLocal.h"
#include <stddef.h>

PTSKILL __fastcall SkillsFindByKey(HUNIT hUnit, DWORD dwKey);
static PTSKILL __fastcall SkillsFindNode(HUNIT hUnit, DWORD nSkillId);
DWORD __stdcall SkillsCalcNodeLevel(HUNIT hUnit, HSKILL hSkill, DWORD fCombine);
int __stdcall SkillsCalcParamOneByLevel(DWORD nSkillId, int nLevel);
int __stdcall SkillsCalcCurveParamOne(int nLevel, DWORD nSkillId);
PTSKILL __stdcall SkillsAddSkill(HUNIT hUnit, DWORD nSkillId);
void __stdcall SkillsSetSkillLevel(HUNIT hUnit, DWORD nSkillId, int nLevel, BOOL fRemove);
static void __fastcall SkillsRemoveSkill(HUNIT hUnit, DWORD nSkillId, BOOL fDecrement);
void __stdcall SkillsUpdatePassive(HUNIT hUnit, DWORD nSkillId);
struct SKILLSTHIS
{
	void SkillsCacheLeftSkill(HUNIT hUnit, DWORD nSkillId);
	void SkillsCacheRightSkill(HUNIT hUnit, DWORD nSkillId);
	void SkillsResetNode(PTSKILL ptSkill);
};
static int __fastcall SkillsTestWeapon(HUNIT hUnit, HSKILL hSkill);
static BOOL __fastcall SkillsTestCodeNotBow(DWORD dwCode);
static BOOL __fastcall SkillsTestInfernoCost(HUNIT hUnit, HSKILL hSkill);
DWORD __stdcall SkillsGetTypeParamThree(DWORD nSkillId);
BOOL __stdcall SkillsAddFieldA(DWORD dwUnitX, DWORD dwUnitY, DWORD dwTargetX, DWORD dwTargetY, DWORD dwRoom,
	DWORD dwPattern);

void __stdcall SkillsNothing(DWORD dwUnused1)
{
}

static void __fastcall SkillsApplyPierce(HUNIT hUnit, DWORD nState)
{
	PTSKILL ptSkill;
	int nLevel;
	PTSTATLIST ptList;

	ptSkill = SkillsFindByKey(hUnit, 33);
	nLevel = (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE);
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, nState);
	if (nLevel != 0)
	{
		if (ptList == NULL)
		{
			ptList = (PTSTATLIST)D2CommonStatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
			D2CommonStatsSetListState(ptList, nState);
			D2CommonStatsUnitLinkList(hUnit, ptList);
		}
	}
	else if (ptList != NULL)
	{
		D2CommonStatsUnitUnlinkList(hUnit, ptList);
		D2CommonStatsListFree(ptList);
		ptList = NULL;
	}
	if (ptList != NULL)
		D2CommonStatsListSetValueB(ptList, 166, nLevel);
}

static void __fastcall SkillsApplyIncreasedStamina(HUNIT hUnit, DWORD nState)
{
	PTSKILL ptSkill;
	int nLevel;
	PTSTATLIST ptList;

	ptSkill = SkillsFindByKey(hUnit, 141);
	nLevel = (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE);
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, nState);
	if (nLevel != 0)
	{
		if (ptList == NULL)
		{
			ptList = (PTSTATLIST)D2CommonStatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
			D2CommonStatsSetListState(ptList, nState);
			D2CommonStatsUnitLinkList(hUnit, ptList);
		}
	}
	else if (ptList != NULL)
	{
		D2CommonStatsUnitUnlinkList(hUnit, ptList);
		D2CommonStatsListFree(ptList);
		ptList = NULL;
	}
	if (ptList != NULL)
		D2CommonStatsListSetValueB(ptList, 163, SkillsCalcParamOneByLevel(SKILL_INCREASED_STAMINA, nLevel));
}

static void __fastcall SkillsApplyIronSkin(HUNIT hUnit, DWORD nState)
{
	PTSKILL ptSkill;
	int nLevel;
	PTSTATLIST ptList;

	ptSkill = SkillsFindByKey(hUnit, 145);
	nLevel = (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE);
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, nState);
	if (nLevel != 0)
	{
		if (ptList == NULL)
		{
			ptList = (PTSTATLIST)D2CommonStatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
			D2CommonStatsSetListState(ptList, nState);
			D2CommonStatsUnitLinkList(hUnit, ptList);
		}
	}
	else if (ptList != NULL)
	{
		D2CommonStatsUnitUnlinkList(hUnit, ptList);
		D2CommonStatsListFree(ptList);
		ptList = NULL;
	}
	if (ptList != NULL)
		D2CommonStatsListSetValueB(ptList, 171, SkillsCalcParamOneByLevel(SKILL_IRON_SKIN, nLevel));
}

static void __fastcall SkillsApplyIncreasedSpeed(HUNIT hUnit, DWORD nState)
{
	PTSKILL ptSkill;
	int nLevel;
	PTSTATLIST ptList;

	ptSkill = SkillsFindByKey(hUnit, 148);
	nLevel = (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE);
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, nState);
	if (nLevel != 0)
	{
		if (ptList == NULL)
		{
			ptList = (PTSTATLIST)D2CommonStatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
			D2CommonStatsSetListState(ptList, nState);
			D2CommonStatsUnitLinkList(hUnit, ptList);
		}
	}
	else if (ptList != NULL)
	{
		D2CommonStatsUnitUnlinkList(hUnit, ptList);
		D2CommonStatsListFree(ptList);
		ptList = NULL;
	}
	if (ptList != NULL)
		D2CommonStatsListSetValueB(ptList, 67, SkillsCalcCurveParamOne(nLevel, SKILL_INCREASED_SPEED));
}

static void __fastcall SkillsApplyNaturalResistance(HUNIT hUnit, DWORD nState)
{
	PTSKILL ptSkill;
	int nLevel;
	PTSTATLIST ptList;
	int nValue;

	ptSkill = SkillsFindByKey(hUnit, 153);
	nLevel = (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE);
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, nState);
	if (nLevel != 0)
	{
		if (ptList == NULL)
		{
			ptList = (PTSTATLIST)D2CommonStatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
			D2CommonStatsSetListState(ptList, nState);
			D2CommonStatsUnitLinkList(hUnit, ptList);
		}
	}
	else if (ptList != NULL)
	{
		D2CommonStatsUnitUnlinkList(hUnit, ptList);
		D2CommonStatsListFree(ptList);
		ptList = NULL;
	}
	if (ptList != NULL)
	{
		nValue = SkillsCalcCurveParamOne(nLevel, SKILL_NATURAL_RESISTANCE);
		D2CommonStatsListSetValueB(ptList, 39, nValue);
		D2CommonStatsListSetValueB(ptList, 41, nValue);
		D2CommonStatsListSetValueB(ptList, 43, nValue);
		D2CommonStatsListSetValueB(ptList, 45, nValue);
	}
}

static void __fastcall SkillsApplyPenetrate(HUNIT hUnit, DWORD nState)
{
	PTSKILL ptSkill;
	int nLevel;
	PTSTATLIST ptList;

	ptSkill = SkillsFindByKey(hUnit, 23);
	nLevel = (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE);
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, nState);
	if (nLevel != 0)
	{
		if (ptList == NULL)
		{
			ptList = (PTSTATLIST)D2CommonStatsAllocList(hUnit->dwMemPool, 0, 0, hUnit->eType, hUnit->dwId);
			D2CommonStatsSetListState(ptList, nState);
			D2CommonStatsUnitLinkList(hUnit, ptList);
		}
	}
	else if (ptList != NULL)
	{
		D2CommonStatsUnitUnlinkList(hUnit, ptList);
		D2CommonStatsListFree(ptList);
		ptList = NULL;
	}
	if (ptList != NULL)
		D2CommonStatsListSetValueB(ptList, 119, SkillsCalcParamOneByLevel(SKILL_PENETRATE, nLevel));
}

typedef void (__fastcall *PFNAPPLYPASSIVE)(HUNIT hUnit, DWORD nState);

struct PASSIVESKILL
{
 	DWORD		dwState;
 	DWORD		dwSkillId;
 	PFNAPPLYPASSIVE	pfnApplyPassive;
};
typedef struct PASSIVESKILL PASSIVESKILL;

static PASSIVESKILL sgtPassiveSkills[20] =
{
	{ 64, 9, NULL },
	{ 65, 13, NULL },
	{ 66, 18, NULL },
	{ 67, 23, SkillsApplyPenetrate },
	{ 68, 29, NULL },
	{ 69, 33, SkillsApplyPierce },
	{ 70, 37, NULL },
	{ 71, 61, NULL },
	{ 72, 63, NULL },
	{ 73, 65, NULL },
	{ 74, 127, NULL },
	{ 75, 128, NULL },
	{ 76, 129, NULL },
	{ 77, 134, NULL },
	{ 78, 135, NULL },
	{ 79, 136, NULL },
	{ 80, 141, SkillsApplyIncreasedStamina },
	{ 81, 145, SkillsApplyIronSkin },
	{ 82, 148, SkillsApplyIncreasedSpeed },
	{ 83, 153, SkillsApplyNaturalResistance },
};

static int sgnNumPassiveSkills = 20;

DWORD __stdcall SkillsLookupKeyAFromKeyB(DWORD dwKeyB)
{
	int i;

	for (i = 0; i < sgnNumPassiveSkills; i++)
		if (sgtPassiveSkills[i].dwSkillId == dwKeyB)
			return sgtPassiveSkills[i].dwState;
	return 0;
}

DWORD __stdcall SkillsLookupKeyBFromKeyA(DWORD dwKeyA)
{
	int i;

	for (i = 0; i < sgnNumPassiveSkills; i++)
		if (sgtPassiveSkills[i].dwState == dwKeyA)
			return sgtPassiveSkills[i].dwSkillId;
	return 0;
}

void __stdcall SkillsUpdatePassive(HUNIT hUnit, DWORD nSkillId)
{
	int i;

	for (i = 0; i < sgnNumPassiveSkills; i++)
	{
		if (sgtPassiveSkills[i].dwSkillId == nSkillId)
		{
			if (sgtPassiveSkills[i].pfnApplyPassive == NULL)
				return;
#line 300
			D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtPassiveSkills[i].pfnApplyPassive), "sgtPassiveSkills[i].pfnApplyPassive");

			sgtPassiveSkills[i].pfnApplyPassive(hUnit, sgtPassiveSkills[i].dwState);
			D2CommonStatsUnitSetActiveStateValue(hUnit, sgtPassiveSkills[i].dwState, TRUE);
			return;
		}
	}
}

int __stdcall SkillsRefreshPassives(HUNIT hUnit)
{
	int i;

	for (i = 0; i < sgnNumPassiveSkills; i++)
	{
		if (sgtPassiveSkills[i].pfnApplyPassive == NULL)
			continue;
		if (SkillsFindByKey(hUnit, sgtPassiveSkills[i].dwSkillId) == NULL)
			continue;
		if (!D2CommonStatsGetState(hUnit, sgtPassiveSkills[i].dwState))
			continue;
#line 325
		D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgtPassiveSkills[i].pfnApplyPassive),
			"sgtPassiveSkills[i].pfnApplyPassive");
		sgtPassiveSkills[i].pfnApplyPassive(hUnit, sgtPassiveSkills[i].dwState);
		D2CommonStatsUnitSetActiveStateValue(hUnit, sgtPassiveSkills[i].dwState, TRUE);
	}
	return sgnNumPassiveSkills;
}

DWORD __stdcall SkillsGetNodeModeIfAny(DWORD dwUnit, PTSKILL ptSkill)
{
	if (ptSkill)
		return ptSkill->dwAnimMode;
	return 0;
}

DWORD __stdcall SkillsGetNodeShiver(HSKILL hSkill)
{
	if (hSkill)
		return hSkill->ptType->Shiver;
	return 0;
}

DWORD __stdcall SkillsGetNodeRange(HSKILL hSkill)
{
	if (hSkill)
		return hSkill->ptType->dwRangeType;
	return 0;
}

PTSKILLHEAD __stdcall SkillsAllocNode(void *pPool)
{
	PTSKILLHEAD ptInfo;
	unsigned i;

#line 410
	ptInfo = (PTSKILLHEAD)D2ALLOCPOOL(pPool, 24);
	for (i = 0; i < 24 / sizeof(DWORD); i++)
		((DWORD *)ptInfo)[i] = 0;
	ptInfo->pPool = (struct D2POOLSYSTEM *)pPool;
	return ptInfo;
}

void __stdcall SkillsRebuildCaches(HUNIT hUnit)
{
	PTSKILLHEAD ptInfo;
	PTSKILL ptNode;
	PTSKILLTYPEINFO ptTable;
	BOOL fFound;

	ptInfo = hUnit->field_c8;
#line 341
	D2ASSERT(ptInfo);
	ptNode = ptInfo->ptFirst;
	while (ptNode != NULL && ptNode->ptType->id != 0)
		ptNode = ptNode->ptNext;
	if (ptNode == NULL)
	{
		SkillsAddSkill(hUnit, SKILL_ATTACK);
		SkillsAddSkill(hUnit, SKILL_KICK);
		SkillsAddSkill(hUnit, SKILL_THROW);
		SkillsAddSkill(hUnit, SKILL_ITEM_START);
		SkillsAddSkill(hUnit, SKILL_BOOK_OF_IDENTIFY);
		SkillsAddSkill(hUnit, SKILL_SCROLL_OF_TOWNPORTAL);
		SkillsAddSkill(hUnit, SKILL_BOOK_OF_TOWNPORTAL);
		if (hUnit->dwClassId == 2)
			SkillsAddSkill(hUnit, SKILL_UNSUMMON);
		if (hUnit->dwClassId == 4)
		{
			SkillsAddSkill(hUnit, SKILL_LEFT_HAND_THROW);
			SkillsAddSkill(hUnit, SKILL_LEFT_HAND_SWING);
		}
	}

	ptTable = (PTSKILLTYPEINFO)D2CommonUnitGetLeftSkill(hUnit);
	if (ptTable == NULL)
	{
		ptTable = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(SKILL_ATTACK);
#line 751
		D2ASSERT(ptTable);
		ptNode = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
		fFound = FALSE;
		while (ptNode != NULL && !fFound)
		{
			if (ptNode->ptType == ptTable)
				fFound = TRUE;
			else
				ptNode = ptNode->ptNext;
		}
		if (fFound)
			hUnit->field_c8->ptLeftSkill = ptNode;
	}

	ptTable = (PTSKILLTYPEINFO)D2CommonUnitGetRightSkill(hUnit);
	if (ptTable == NULL)
	{
		ptTable = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(SKILL_ATTACK);
#line 774
		D2ASSERT(ptTable);
		ptNode = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
		fFound = FALSE;
		while (ptNode != NULL && !fFound)
		{
			if (ptNode->ptType == ptTable)
				fFound = TRUE;
			else
				ptNode = ptNode->ptNext;
		}
		if (fFound)
			hUnit->field_c8->ptRightSkill = ptNode;
	}
}

PTSKILL __stdcall SkillsNodeGetNext(PTSKILL ptSkill)
{
#line 455
	D2ASSERT(ptSkill);
	return ptSkill->ptNext;
}

PTSKILL __stdcall SkillsHeadGetFirst(PTSKILLHEAD ptInfo)
{
#line 464
	D2ASSERT(ptInfo);
	return ptInfo->ptFirst;
}

PTSKILL __fastcall SkillsHeadGetLeftSkill(PTSKILLHEAD ptInfo)
{
#line 473
	D2ASSERT(ptInfo);
	return ptInfo->ptLeftSkill;
}

PTSKILL __fastcall SkillsHeadGetRightSkill(PTSKILLHEAD ptInfo)
{
#line 482
	D2ASSERT(ptInfo);
	return ptInfo->ptRightSkill;
}

void __fastcall SkillsHeadSetThirdCached(PTSKILLHEAD ptInfo, PTSKILL ptSkill)
{
#line 491
	D2ASSERT(ptInfo);
	ptInfo->ptUsedSkill = ptSkill;
}

PTSKILL __fastcall SkillsHeadGetThirdCached(PTSKILLHEAD ptInfo)
{
#line 500
	D2ASSERT(ptInfo);
	return ptInfo->ptUsedSkill;
}

static __inline PTSKILL SkillsListFind(PTSKILL ptNode, DWORD dwKey)
{
	for (; ptNode; ptNode = ptNode->ptNext)
	{
		if (ptNode->ptType->id == dwKey)
			return ptNode;
	}
	return NULL;
}

PTSKILL __fastcall SkillsFindByKey(HUNIT hUnit, DWORD dwKey)
{
	PTSKILLHEAD ptInfo;
	PTSKILL ptNode;

	ptInfo = hUnit->field_c8;
#line 341
	D2ASSERT(ptInfo);
	return SkillsListFind(ptInfo->ptFirst, dwKey);
}

void __stdcall SkillsDecrementSkill(HUNIT hUnit, DWORD nSkillId)
{
	SkillsRemoveSkill(hUnit, nSkillId, TRUE);
}

static void __fastcall SkillsRemoveSkill(HUNIT hUnit, DWORD nSkillId, BOOL fDecrement)
{
	PTSKILLHEAD ptInfo;
	PTSKILL ptSkill;
	PTSKILL ptPrev;
	DWORD dwId08;
	DWORD dwId0c;
	DWORD nState;

	ptInfo = hUnit->field_c8;
	ptPrev = NULL;
	ptSkill = ptInfo->ptFirst;
	dwId08 = ptInfo->ptLeftSkill->ptType->id;
	dwId0c = ptInfo->ptRightSkill->ptType->id;

	nState = SkillsLookupKeyAFromKeyB(nSkillId);
	if (nState != 0)
		D2CommonStatsUnitSetActiveStateBit(hUnit, nState, FALSE);

	if (nSkillId == dwId08)
		((SKILLSTHIS *)0)->SkillsCacheLeftSkill(hUnit, SKILL_ATTACK);
	if (nSkillId == dwId0c)
		((SKILLSTHIS *)0)->SkillsCacheRightSkill(hUnit, SKILL_ATTACK);
	if (ptInfo->ptUsedSkill != NULL && nSkillId == SkillsGetNodeType(ptInfo->ptUsedSkill))
		ptInfo->ptUsedSkill = NULL;

	while (ptSkill != NULL && ptSkill->ptType->id != nSkillId)
	{
		ptPrev = ptSkill;
		ptSkill = ptSkill->ptNext;
	}
	if (ptSkill != NULL && (!fDecrement || (int)--ptSkill->dwSkillLevel <= 0))
	{
		if (ptPrev != NULL)
			ptPrev->ptNext = ptSkill->ptNext;
		else
			ptInfo->ptFirst = ptSkill->ptNext;
#line 568
		D2FREEPOOL(hUnit->dwMemPool, ptSkill);
	}
	SkillsUpdatePassive(hUnit, nSkillId);
}

PTSKILL __stdcall SkillsExchangeSelected(HUNIT hUnit, PTSKILL ptNewSelected)
{
	PTSKILLHEAD ptHead;
	PTSKILL ptOld = NULL;

	ptHead = hUnit->field_c8;
	if (ptHead != NULL)
	{
		ptOld = ptHead->ptSelected;
		ptHead->ptSelected = ptNewSelected;
	}
	return ptOld;
}

PTSKILL __stdcall SkillsGetSelectedUnsafe(HUNIT hUnit)
{
	return hUnit->field_c8->ptSelected;
}

void __stdcall SkillsFreeList(HUNIT hUnit)
{
	PTSKILLHEAD ptHead;
	PTSKILL ptNode;
	PTSKILL ptNext;

	ptHead = hUnit->field_c8;
	if (ptHead == NULL)
		return;
	ptNode = ptHead->ptFirst;
	while (ptNode != NULL)
	{
		ptNext = ptNode->ptNext;
#line 621
		D2FREEPOOL(hUnit->dwMemPool, ptNode);
		ptNode = ptNext;
	}
#line 624
	D2FREEPOOL(hUnit->dwMemPool, ptHead);
	hUnit->field_c8 = NULL;
}

PTSKILL __stdcall SkillsAddSkill(HUNIT hUnit, DWORD nSkillId)
{
	DWORD nState;
	PTSKILLTYPEINFO ptType;
	PTSKILL ptSkill;
	PTSKILLHEAD ptInfo;
	PTSKILL ptLast;
	BOOL fFound = FALSE;
	unsigned i;

	nState = SkillsLookupKeyAFromKeyB(nSkillId);
	if (nState != 0)
		D2CommonStatsUnitSetActiveStateBit(hUnit, nState, TRUE);
	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
#line 645
	D2ASSERTMSG(ptType, "Attempt to add a skill that doesn't exist!");
	ptSkill = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
	while (ptSkill != NULL && !fFound)
	{
		if (ptSkill->ptType == ptType)
			fFound = TRUE;
		else
			ptSkill = ptSkill->ptNext;
	}
	if (!fFound)
	{
		ptInfo = hUnit->field_c8;
#line 662
		ptSkill = (PTSKILL)D2ALLOCPOOL(ptInfo->pPool, sizeof(SKILL));
		for (i = 0; i < sizeof(SKILL) / sizeof(DWORD); i++)
			((DWORD *)ptSkill)[i] = 0;
		ptSkill->ptType = ptType;
		if (hUnit->eType == UNIT_MONSTER)
		{
			if (D2CommonMonsterUnitGetTypeLocalIndex(hUnit) == 360)
				ptSkill->dwAnimMode = 8;
			else
				ptSkill->dwAnimMode = ptType->dwMonAnimMode;
		}
		else
		{
			ptSkill->dwAnimMode = ptType->dwAnimMode;
		}
		ptSkill->dwSkillLevel = 1;
		ptSkill->ptNext = NULL;
		if (ptInfo->ptFirst != NULL)
		{
			ptLast = ptInfo->ptFirst;
			while (ptLast->ptNext != NULL)
				ptLast = ptLast->ptNext;
			ptLast->ptNext = ptSkill;
		}
		else
		{
			ptInfo->ptFirst = ptSkill;
		}
		SkillsUpdatePassive(hUnit, nSkillId);
		return ptSkill;
	}

	ptSkill->dwSkillLevel++;
#line 701
	D2ASSERTMSG((int)ptSkill->dwSkillLevel <= 20 || hUnit->eType == UNIT_MONSTER, "Skill set beyond max level!\nPlease tell Ted Bisson!");
	SkillsUpdatePassive(hUnit, nSkillId);
	return ptSkill;
}

void __stdcall SkillsSetSkillLevel(HUNIT hUnit, DWORD nSkillId, int nLevel, BOOL fRemove)
{
	PTSKILL ptSkill;

	if (nLevel == 0)
	{
		if (fRemove)
		{
			SkillsRemoveSkill(hUnit, nSkillId, FALSE);
		}
		else
		{
			ptSkill = SkillsFindByKey(hUnit, nSkillId);
#line 722
			D2ASSERTMSG(ptSkill, "Unit doesn't have the skill!");
			ptSkill->dwSkillLevel = 0;
		}
	}
	else
	{
		ptSkill = SkillsFindByKey(hUnit, nSkillId);
		if (ptSkill == NULL)
		{
			ptSkill = SkillsAddSkill(hUnit, nSkillId);
#line 733
			D2ASSERTMSG(ptSkill, "Unable to add skill!");
		}
		ptSkill->dwSkillLevel = nLevel;
#line 737
		D2ASSERTMSG(hUnit->eType == UNIT_MONSTER || nLevel <= 20, "Skill set beyond max level!\nPlease tell Ted Bisson!");

	}
	SkillsUpdatePassive(hUnit, nSkillId);
}

void SKILLSTHIS::SkillsCacheLeftSkill(HUNIT hUnit, DWORD nSkillId)
{
	BOOL fFound;
	PTSKILLTYPEINFO ptTable;
	PTSKILL ptNode;

	ptTable = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
#line 751
	D2ASSERT(ptTable);
	ptNode = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
	fFound = FALSE;
	while (ptNode != NULL && !fFound)
	{
		if (ptNode->ptType == ptTable)
			fFound = TRUE;
		else
			ptNode = ptNode->ptNext;
	}
	if (fFound)
		hUnit->field_c8->ptLeftSkill = ptNode;
}

void SKILLSTHIS::SkillsCacheRightSkill(HUNIT hUnit, DWORD nSkillId)
{
	BOOL fFound;
	PTSKILLTYPEINFO ptTable;
	PTSKILL ptNode;

	ptTable = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
#line 774
	D2ASSERT(ptTable);
	ptNode = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
	fFound = FALSE;
	while (ptNode != NULL && !fFound)
	{
		if (ptNode->ptType == ptTable)
			fFound = TRUE;
		else
			ptNode = ptNode->ptNext;
	}
	if (fFound)
		hUnit->field_c8->ptRightSkill = ptNode;
}

DWORD __stdcall SkillsGetNodeType(HSKILL hSkill)
{
#line 797
	D2ASSERT(hSkill);
	return hSkill->ptType->id;
}

#define NUM_SKILLS_PER_MONSTER	5

DWORD __fastcall SkillsGetNodeSlot(HUNIT hUnit, PTSKILL ptSkill)
{
	DWORD dwKey;
	DWORD dwClassId;
	BYTE *pbMonStats;
	int nI;

#line 806
	D2ASSERT(ptSkill);
	if (UnitGetType (hUnit) == 0)
		return ptSkill->ptType->seqnum;
#line 812
	D2ASSERT(UnitGetType (hUnit) == UNIT_MONSTER);
	dwClassId = hUnit->dwClassId;
	dwKey = ptSkill->ptType->id;
	if (dwKey == 199)
		return 0;
	pbMonStats = (BYTE *)D2CommonMonsterTblsGetStats(dwClassId);
	for (nI = 0; nI < NUM_SKILLS_PER_MONSTER; nI++)
		if (*(DWORD *)(pbMonStats + offsetof(MONSTATS, Skill1) + nI * 4) == dwKey)
			return pbMonStats[offsetof(MONSTATS, Skill1Seq) + nI];
#line 829
	D2ASSERT(nI < NUM_SKILLS_PER_MONSTER);
	return 0;
}

static __inline int SkillsGetTypeCost(PTSKILLTYPEINFO ptData, int nLevel)
{
	int n;

	n = nLevel - 1;
#line 842
	D2ASSERT(ptData);
	if (n < 0)
		n = 0;
	return (int)((ptData->mana + ptData->lvlmana * n) << ptData->manashift);
}

static __inline BOOL SkillsTestHands(HUNIT hUnit, PTSKILL ptSkill)
{
	PTSKILLTYPEINFO ptType;
	DWORD dwCode;
	BOOL fRestricted;
	int i;

	ptType = ptSkill->ptType;
	fRestricted = FALSE;
	if (ptType->dwNeedsShield != 0)
		return D2CommonINVENTORYFindShieldInHands(hUnit->ptInventory, 0);
	else
	{ DWORD dwOut;
		dwCode = D2CommonCOMPOSITGetUnitWeaponClass(hUnit, hUnit->ptInventory, &dwOut, (DWORD)-1, 1);
		if (ptSkill->ptType->dwNotBow != 0)
			return SkillsTestCodeNotBow(dwCode);
		for (i = 0; i < 6; i++)
		{
			if (ptType->itemclass[i] == dwCode)
				return TRUE;
			if (ptType->itemclass[i] != 544766072)
				fRestricted = TRUE;
		}
	}
	if (fRestricted)
		return FALSE;
	return TRUE;
}

static __inline BOOL SkillsTestThrowPotion(HUNIT hUnit, PTSKILL ptSkill)
{
	PTINVENTORY ptInventory;
	HUNIT hItem;
	HUNIT hOther;

	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	if (ptInventory == NULL)
		return FALSE;
	switch (D2CommonSkillsGetNodeType(ptSkill))
	{
	case 97:
		break;
	case 133:
	case 147:
		ptInventory = (PTINVENTORY)hUnit->ptInventory;
		if (ptInventory == NULL)
			break;
		hItem = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 4);
		hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 5);
		if (hItem != NULL && D2CommonITEMSGetType(hItem) == 38)
			return FALSE;
		if (hOther != NULL && D2CommonITEMSGetType(hOther) == 38)
			return FALSE;
		break;
	default:
		if (ptSkill->ptType->dwRangeType == 1 || ptSkill->ptType->dwRangeType == 3)
		{
			hItem = (HUNIT)D2CommonINVENTORYFindEquippedMatch(ptInventory);
			if (hItem != NULL && D2CommonITEMSGetType(hItem) == 38
				&& ((PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(D2CommonSkillsGetNodeType(ptSkill)))->SelectDead == 0)
				return FALSE;
		}
		break;
	}
	return TRUE;
}

int __stdcall SkillsTestUsable(HUNIT hUnit, PTSKILL ptSkill)
{
	PTSKILLTYPEINFO ptType;
	BOOL fBusy;

#line 1269
	D2ASSERT(ptSkill);
	if (ptSkill->ptType->InGame != 0)
	{
		if (SkillsCalcNodeLevel(hUnit, ptSkill, TRUE) == 0)
			return 7;
		if (ptSkill->ptType->Periodic)
			return 6;
		if (ptSkill->ptType->Passive)
			return 5;
		if (!SkillsTestWeapon(hUnit, ptSkill))
			return 2;

		if (!SkillsTestHands(hUnit, ptSkill))
			return 2;

		ptType = ptSkill->ptType;
		if (D2CommonStatsUnitGetValue(hUnit, 8) < SkillsGetTypeCost(ptType, (int)SkillsCalcNodeLevel(hUnit, ptSkill, TRUE)))
			return 1;

		ptType = ptSkill->ptType;
		fBusy = StatsUnitTestFalse(hUnit);
		if (!fBusy)
			fBusy = ptType->state1;
		if (fBusy)
			return 4;

		if (!SkillsTestThrowPotion(hUnit, ptSkill))
			return 2;
		return !SkillsTestInfernoCost(hUnit, ptSkill);
	}
	return 3;
}

static __inline BOOL SkillsTestThrowPair(HUNIT *phItem, HUNIT hOther)
{
	if (!D2CommonITEMSGetTypeThrowable(*phItem) || !D2CommonITEMSGetTypeThrowable(hOther)
		|| !D2CommonITEMSValidateQuantity(*phItem) || !D2CommonITEMSValidateQuantity(hOther))
		return FALSE;
	return TRUE;
}

static int __fastcall SkillsTestWeapon(HUNIT hUnit, HSKILL hSkill)
{
	DWORD nSkillId;
	DWORD dwClass;
	PTINVENTORY ptInventory;
	DWORD eType;
	HUNIT hItem;
	HUNIT hOther;
	DWORD nLoc;
	BOOL fPrimaryWeapon;
	int nResult;
	BYTE bType;

	nSkillId = SkillsGetNodeType(hSkill);
	dwClass = hUnit->dwClassId;
	eType = hUnit->eType;
	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	switch (nSkillId)
	{
	case 0:
	case 7:
	case 11:
	case 12:
	case 16:
	case 21:
	case 22:
	case 26:
	case 27:
	case 31:
		if (!D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon))
			break;
		if (nLoc == 5)
			hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 4);
		else
			hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 5);
		if (hOther)
			break;
		switch (D2CommonITEMSGetType(hItem))
		{
		case 5:
		case 6:
			return FALSE;
		case 27:
		case 35:
			nResult = (int)D2CommonStatsGetTotalValue(hItem, 157);
			if (nResult == 0)
				return nResult;
			if (nSkillId == 0 || nSkillId == 12 || nSkillId == 26)
				return TRUE;
			return FALSE;
		default:
			return TRUE;
		}

	case 2:
		nResult = 0;
		if (eType == UNIT_PLAYER && dwClass == 4)
		{
			if (D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon))
			{
				if (!fPrimaryWeapon)
				{
					if (nLoc == 5)
						hItem = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 4);
					else
						hItem = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 5);
				}
				if (hItem != NULL)
				{
					nResult = (int)D2CommonITEMSGetTypeThrowable(hItem);
					if (!nResult)
						nResult = (int)D2CommonStatsGetTotalValue(hItem, 125);
				}
			}
		}
		else if (D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon))
		{
#line 944
			D2ASSERT(fPrimaryWeapon);
			nResult = (int)D2CommonITEMSGetTypeThrowable(hItem);
			if (!nResult)
				nResult = (int)D2CommonStatsGetTotalValue(hItem, 125);
		}
		return nResult;

	case 4:
		nResult = 0;
		if (eType != UNIT_PLAYER || dwClass != 4)
			return FALSE;
		if (D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon))
		{
			if (fPrimaryWeapon)
			{
				if (nLoc == 5)
					hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 4);
				else
					hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 5);
				if (hOther != NULL)
				{
					nResult = (int)D2CommonITEMSGetTypeThrowable(hOther);
					if (!nResult)
						nResult = (int)D2CommonStatsGetTotalValue(hOther, 125);
				}
			}
			else
			{
				nResult = (int)D2CommonITEMSGetTypeThrowable(hItem);
				if (!nResult)
					nResult = (int)D2CommonStatsGetTotalValue(hItem, 125);
			}
		}
		return nResult;

	case 5:
		nResult = 0;
		if (eType != UNIT_PLAYER || dwClass != 4)
			return FALSE;
		if (D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon))
		{
			if (fPrimaryWeapon)
			{
				if (nLoc == 5)
					hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 4);
				else
					hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 5);
				if (hOther != NULL)
					nResult = (int)D2CommonITEMSIsWeapon(hOther);
			}
			else
				nResult = (int)D2CommonITEMSIsWeapon(hItem);
		}
		return nResult;

	case 73:
		nResult = FALSE;
		if (eType == UNIT_PLAYER && dwClass == 2
			&& D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon)
			&& D2CommonITEMSGetType(hItem) == 32)
			nResult = TRUE;
		return nResult;

	case 217:
	case 218:
	case 219:
	case 220:
		if ((int)hSkill->dwCount > 0)
			break;
		return FALSE;

	case 10:
	case 14:
	case 19:
	case 24:
	case 30:
	case 34:
		hItem = (HUNIT)D2CommonINVENTORYFindEquippedMatch(ptInventory);
		if (hItem == NULL)
			return FALSE;
		bType = ((PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId))->bWeaponGroup;
		if (bType == 7 || bType == 8)
			break;
		return FALSE;

	case 15:
	case 20:
	case 25:
	case 35:
		hItem = (HUNIT)D2CommonINVENTORYFindEquippedMatch(ptInventory);
		if (hItem == NULL)
			return FALSE;
		if (((PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId))->bWeaponGroup != 7)
			return FALSE;
		if (D2CommonITEMSGetTypeThrowable(hItem))
			break;
		return FALSE;

	case 140:
		if (eType != UNIT_PLAYER || dwClass != 4)
			return FALSE;
		if (!D2CommonINVENTORYGetWeapon(ptInventory, &hItem, &nLoc, &fPrimaryWeapon))
			return FALSE;
		if (hItem == NULL)
			return FALSE;
		if (nLoc == 5)
			hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 4);
		else
			hOther = (HUNIT)D2CommonINVENTORYGetLocSlot(ptInventory, 5);
		if (hOther == NULL)
			return FALSE;
		return SkillsTestThrowPair(&hItem, hOther);

	default:
		break;
	}
	return TRUE;
}

#define SKILLS_CODE_CHAR(szCode, i)	((szCode)[i] ? (szCode)[i] : ' ')

static BOOL __fastcall SkillsTestCodeNotBow(DWORD dwCode)
{
	union { char sz[4]; DWORD dw; } tBow, tXbw;

	tBow.sz[0] = SKILLS_CODE_CHAR("bow", 0);
	tBow.sz[1] = SKILLS_CODE_CHAR("bow", 1);
	tBow.sz[2] = SKILLS_CODE_CHAR("bow", 2);
	tBow.sz[3] = SKILLS_CODE_CHAR("bow", 3);
	tXbw.sz[0] = SKILLS_CODE_CHAR("xbw", 0);
	tXbw.sz[1] = SKILLS_CODE_CHAR("xbw", 1);
	tXbw.sz[2] = SKILLS_CODE_CHAR("xbw", 2);
	tXbw.sz[3] = SKILLS_CODE_CHAR("xbw", 3);
	if (dwCode == tBow.dw || dwCode == tXbw.dw)
		return FALSE;
	return TRUE;
}

#pragma auto_inline(off)
static BOOL __fastcall SkillsTestInfernoCost(HUNIT hUnit, HSKILL hSkill)
{
	int nCost;

	if (SkillsGetNodeType(hSkill) == 41 && !D2CommonStatsGetState(hUnit, 12))
	{
		nCost = (int)SkillsGetTypeParamThree(SKILL_INFERNO);
		if (D2CommonStatsUnitGetValue(hUnit, 8) < nCost << 8)
			return FALSE;
	}
	return TRUE;
}
#pragma auto_inline(on)

DWORD __fastcall SkillsGetNodeTypeRecord(PTSKILL ptSkill)
{
#line 1309
	D2ASSERT(ptSkill);
	return (DWORD)ptSkill->ptType;
}

DWORD __stdcall SkillsGetComboFlags(HUNIT hUnit)
{
	DWORD dwCombo = 0;

	if (D2CommonStatsGetState(hUnit, 51))
		dwCombo = 1;
	if (D2CommonStatsGetState(hUnit, 134))
		dwCombo += 2;
	return dwCombo;
}

DWORD __stdcall SkillsCalcNodeLevel(HUNIT hUnit, HSKILL hSkill, DWORD fCombine)
{
	DWORD dwLevel = 0;

	if (hSkill != NULL)
	{
		dwLevel = hSkill->dwSkillLevel;
		if (fCombine)
			dwLevel += hSkill->dwSkillBonus + SkillsGetComboFlags(hUnit);
	}
	return dwLevel;
}

void __stdcall SkillsSetNodeMode(PTSKILL ptSkill, DWORD dwValue)
{
#line 1353
	D2ASSERT(ptSkill);
	ptSkill->dwAnimMode = dwValue;
}

DWORD __stdcall SkillsGetNodeMode(PTSKILL ptSkill)
{
#line 1363
	D2ASSERT(ptSkill);
	return ptSkill->dwAnimMode;
}

DWORD __stdcall SkillsGetNodeScratchA(PTSKILL ptSkill)
{
#line 1372
	D2ASSERT(ptSkill);
	return ptSkill->dwScratchA;
}

void __stdcall SkillsSetNodeScratchA(PTSKILL ptSkill, DWORD dwValue)
{
#line 1381
	D2ASSERT(ptSkill);
	ptSkill->dwScratchA = dwValue;
}

DWORD __stdcall SkillsGetNodeScratchB(PTSKILL ptSkill)
{
#line 1390
	D2ASSERT(ptSkill);
	return ptSkill->dwScratchB;
}

void __stdcall SkillsSetNodeScratchB(PTSKILL ptSkill, DWORD dwValue)
{
#line 1399
	D2ASSERT(ptSkill);
	ptSkill->dwScratchB = dwValue;
}

void SKILLSTHIS::SkillsResetNode(PTSKILL ptSkill)
{
#line 1408
	D2ASSERT(ptSkill);
	ptSkill->dwScratchA = 0;
	ptSkill->dwScratchB = 0;
	ptSkill->dwFlags &= 0xFFFFFFDF;
}

DWORD __stdcall SkillsGetNodeScratchC(PTSKILL ptSkill)
{
#line 1420
	D2ASSERT(ptSkill);
	return ptSkill->dwScratchC;
}

DWORD __stdcall SkillsGetNodeScratchD(PTSKILL ptSkill)
{
#line 1429
	D2ASSERT(ptSkill);
	return ptSkill->dwScratchD;
}

DWORD __stdcall SkillsGetNodeScratchE(PTSKILL ptSkill)
{
#line 1438
	D2ASSERT(ptSkill);
	return ptSkill->dwScratchE;
}

DWORD __stdcall SkillsGetNodeScratchF(PTSKILL ptSkill)
{
#line 1447
	D2ASSERT(ptSkill);
	return ptSkill->dwScratchF;
}

void __stdcall SkillsSetNodeScratchC(PTSKILL ptSkill, DWORD dwValue)
{
#line 1456
	D2ASSERT(ptSkill);
	ptSkill->dwScratchC = dwValue;
}

void __stdcall SkillsSetNodeScratchD(PTSKILL ptSkill, DWORD dwValue)
{
#line 1465
	D2ASSERT(ptSkill);
	ptSkill->dwScratchD = dwValue;
}

void __stdcall SkillsSetNodeScratchE(PTSKILL ptSkill, DWORD dwValue)
{
#line 1474
	D2ASSERT(ptSkill);
	ptSkill->dwScratchE = dwValue;
}

void __stdcall SkillsSetNodeScratchF(PTSKILL ptSkill, DWORD dwValue)
{
#line 1483
	D2ASSERT(ptSkill);
	ptSkill->dwScratchF = dwValue;
}

void __stdcall SkillsSetNodeFlags(PTSKILL ptSkill, DWORD dwValue)
{
#line 1492
	D2ASSERT(ptSkill);
	ptSkill->dwFlags = dwValue;
}

DWORD __stdcall SkillsGetNodeFlags(HSKILL hSkill)
{
	if (hSkill)
		return hSkill->dwFlags;
	return 0;
}

DWORD __stdcall SkillsGetTypeReqLevel(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	return ptType->reqlevel;
}

DWORD __stdcall SkillsCalcReqLevel(HUNIT hUnit, DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;
	DWORD dwResult;
	PTSKILLHEAD ptInfo;
	PTSKILL ptNode;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	dwResult = 0;
	if (ptType != NULL)
	{
		dwResult = ptType->reqlevel;
		ptInfo = hUnit->field_c8;
#line 341
		D2ASSERT(ptInfo);
		ptNode = ptInfo->ptFirst;
		while (ptNode != NULL && ptNode->ptType->id != nSkillId)
			ptNode = ptNode->ptNext;
		if (ptNode != NULL)
			dwResult += ptNode->dwSkillLevel;
	}
	return dwResult;
}

DWORD __fastcall SkillsGetLevelByKey(HUNIT hUnit, DWORD nSkillId)
{
	PTSKILLHEAD ptInfo;
	PTSKILL ptNode;

	ptInfo = hUnit->field_c8;
#line 341
	D2ASSERT(ptInfo);
	ptNode = ptInfo->ptFirst;
	while (ptNode != NULL && ptNode->ptType->id != nSkillId)
		ptNode = ptNode->ptNext;
	if (ptNode != NULL)
		return ptNode->dwSkillLevel;
	return 0;
}

BOOL __stdcall SkillsTestPrerequisites(HUNIT hUnit, PTSKILLTYPEINFO ptType, DWORD nSkillId)
{
	int nLevel;
	int nRequired;
	PTSKILLTYPEINFO ptRecord;
	PTSKILLTYPEINFO ptRequired;
	PTSKILL ptSkills;
	PTSKILL ptNode;

	nLevel = D2CommonStatsUnitGetValue(hUnit, 12);
	ptRecord = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	nRequired = 0;
	if (ptRecord != NULL)
	{
		nRequired = ptRecord->reqlevel;
		ptNode = SkillsFindNode(hUnit, nSkillId);
		if (ptNode != NULL)
			nRequired += ptNode->dwSkillLevel;
	}
	if (nRequired > nLevel)
		return FALSE;
	if (ptType->ptReqSkill1 == NULL && (int)SkillsCalcReqLevel(hUnit, nSkillId) > nLevel)
		return FALSE;

	ptRequired = ptType->ptReqSkill1;
	if (ptRequired != NULL)
	{
		ptSkills = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
#line 1512
		D2ASSERT(ptSkills);
		while (1)
		{
			if (ptSkills->ptType == ptRequired)
				break;
			ptSkills = ptSkills->ptNext;
			if (ptSkills == NULL)
				return FALSE;
		}
		ptNode = SkillsFindByKey(hUnit, ptType->ptReqSkill1->id);
		if (ptNode == NULL || ptNode->dwSkillLevel == 0)
			return FALSE;
	}
	ptRequired = ptType->ptReqSkill2;
	if (ptRequired != NULL)
	{
		ptSkills = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
#line 1512
		D2ASSERT(ptSkills);
		while (1)
		{
			if (ptSkills->ptType == ptRequired)
				break;
			ptSkills = ptSkills->ptNext;
			if (ptSkills == NULL)
				return FALSE;
		}
		ptNode = SkillsFindByKey(hUnit, ptType->ptReqSkill2->id);
		if (ptNode == NULL || ptNode->dwSkillLevel == 0)
			return FALSE;
	}
	ptRequired = ptType->ptReqSkill3;
	if (ptRequired != NULL)
	{
		ptSkills = (PTSKILL)D2CommonUnitGetFirstSkill(hUnit);
#line 1512
		D2ASSERT(ptSkills);
		while (1)
		{
			if (ptSkills->ptType == ptRequired)
				break;
			ptSkills = ptSkills->ptNext;
			if (ptSkills == NULL)
				return FALSE;
		}
		ptNode = SkillsFindByKey(hUnit, ptType->ptReqSkill3->id);
		if (ptNode == NULL || ptNode->dwSkillLevel == 0)
			return FALSE;
	}
	return TRUE;
}

#pragma auto_inline(off)
static PTSKILL __fastcall SkillsFindNode(HUNIT hUnit, DWORD nSkillId)
{
	PTSKILLHEAD ptInfo;
	PTSKILL ptNode;

	ptInfo = hUnit->field_c8;
#line 341
	D2ASSERT(ptInfo);
	for (ptNode = ptInfo->ptFirst; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->ptType->id == nSkillId)
			return ptNode;
	}
	return NULL;
}
#pragma auto_inline(on)

BOOL __stdcall SkillsTestStatRequirements(HUNIT hUnit, PTSKILLTYPEINFO ptType, DWORD nSkillId)
{
	int nLevel, nStat0, nStat2, nStat1, nStat3;

	nLevel = D2CommonStatsUnitGetValue(hUnit, 12);
	nStat0 = D2CommonStatsUnitGetValue(hUnit, 0);
	nStat2 = D2CommonStatsUnitGetValue(hUnit, 2);
	nStat1 = D2CommonStatsUnitGetValue(hUnit, 1);
	nStat3 = D2CommonStatsUnitGetValue(hUnit, 3);
	if ((int)SkillsCalcReqLevel(hUnit, nSkillId) > nLevel)
		return FALSE;
	if (ptType->reqstr > nStat0)
		return FALSE;
	if (ptType->reqdex > nStat2)
		return FALSE;
	if (ptType->reqint > nStat1)
		return FALSE;
	if (ptType->reqvit > nStat3)
		return FALSE;
	return ptType->InGame != 0;
}

DWORD __stdcall SkillsGetTypeParamOne(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param1;
	return 0;
}

DWORD __stdcall SkillsGetTypeParamTwo(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param2;
	return 0;
}

DWORD __stdcall SkillsGetTypeParamThree(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param3;
	return 0;
}

DWORD __stdcall SkillsGetTypeParamFour(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param4;
	return 0;
}

DWORD __stdcall SkillsGetTypeParamFive(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param5;
	return 0;
}

DWORD __stdcall SkillsGetTypeParamSix(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param6;
	return 0;
}

int __stdcall SkillsCalcParamOneByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param1 + ptType->Param2 * (nLevel - 1);
	return 0;
}

int __stdcall SkillsCalcParamThreeByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param3 + ptType->Param4 * (nLevel - 1);
	return 0;
}

int __stdcall SkillsCalcParamFiveByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->Param5 + ptType->Param6 * (nLevel - 1);
	return 0;
}

int __stdcall SkillsCalcToHitByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (ptType)
		return ptType->ToHit + ptType->LevToHit * (nLevel - 1);
	return 0;
}

DWORD __stdcall SkillsSumNodeBonus(HUNIT hUnit, DWORD dwKey)
{
	PTSKILL ptNode;
	DWORD dwCombo;
	DWORD dwBonus;

	if (hUnit != NULL)
	{
		ptNode = SkillsFindByKey(hUnit, dwKey);
		dwBonus = 0;
		if (ptNode != NULL)
		{
			dwBonus = ptNode->dwSkillLevel;
			dwCombo = 0;
			if (D2CommonStatsGetState(hUnit, 51))
				dwCombo = 1;
			if (D2CommonStatsGetState(hUnit, 134))
				dwCombo += 2;
			dwBonus += ptNode->dwSkillBonus + dwCombo;
		}
		return dwBonus;
	}
	return 0;
}

int __stdcall SkillsCalcManaCost(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;
	int n;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (!(ptType == NULL)) {
		n = ptType->mana + ptType->lvlmana * (nLevel - 1);
		return n << ptType->manashift; }
	return 0;
}

int __stdcall SkillsCalcValueA(DWORD nSkillId, HUNIT hUnit, int nLevel)
{
	PTSKILLTYPEINFO ptType;
	int nBonus;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	nBonus = 0;
	if (ptType->SrcDam)
	{
		HUNIT hUnitSaved = hUnit;
		int nPreHalf = 0;
		int nT3;

		if (hUnitSaved->ptInventory != 0)
		{
			DWORD dwLocal;

			if (D2CommonINVENTORYGetWeapon(hUnitSaved->ptInventory, &nSkillId, &dwLocal, &hUnit) &&
				D2CommonITEMSGetTypeStrBonus(nSkillId))
				nPreHalf = (D2CommonStatsUnitGetValue(hUnitSaved, 12) * D2CommonStatsUnitGetValue(hUnitSaved, 0)) / 100;
		}
		nT3 = D2CommonStatsUnitGetValue(hUnitSaved, 21) + nPreHalf;
		nBonus = ((int)(BYTE)ptType->SrcDam * nT3) / 2;
	}
	return ptType->MinDam + ptType->LevDam * (nLevel - 1) + nBonus;
}

int __stdcall SkillsCalcValueB(DWORD nSkillId, HUNIT hUnit, int nLevel)
{
	PTSKILLTYPEINFO ptType;
	int nBonus;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	nBonus = 0;
	if (ptType->SrcDam)
	{
		HUNIT hUnitSaved = hUnit;
		int nPreHalf = 0;
		int nT3;

		if (hUnitSaved->ptInventory != 0)
		{
			DWORD dwLocal;

			if (D2CommonINVENTORYGetWeapon(hUnitSaved->ptInventory, &nSkillId, &dwLocal, &hUnit) &&
				D2CommonITEMSGetTypeStrBonus(nSkillId))
				nPreHalf = (D2CommonStatsUnitGetValue(hUnitSaved, 12) * D2CommonStatsUnitGetValue(hUnitSaved, 0)) / 100;
		}
		nT3 = D2CommonStatsUnitGetValue(hUnitSaved, 22) + nPreHalf;
		nBonus = ((int)(BYTE)ptType->SrcDam * nT3) / 2;
	}
	return ptType->MaxDam + ptType->LevDam * (nLevel - 1) + nBonus;
}

int __stdcall SkillsCalcEMinByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	return ptType->EMin + ptType->ELev * (nLevel - 1);
}

int __stdcall SkillsCalcEMaxByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	return ptType->EMax + ptType->ELev * (nLevel - 1);
}

int __stdcall SkillsCalcELenByLevel(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	return ptType->ELen + ptType->ELevLen * (nLevel - 1);
}

DWORD __stdcall SkillsGetTypeHitShift(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	return ptType->HitShift;
}

BYTE * __stdcall SkillsGetTypeCharClass(DWORD nSkillId)
{
	PTSKILLTYPEINFO ptSkill;

	ptSkill = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
#line 1929
	D2ASSERT(ptSkill);
	return &ptSkill->charclass;
}

DWORD __stdcall SkillsGetNodeCount(PTSKILL ptSkill)
{
#line 1938
	D2ASSERT(ptSkill);
	return ptSkill->dwCount;
}

void __stdcall SkillsSetNodeCount(PTSKILL ptSkill, DWORD dwValue)
{
#line 1947
	D2ASSERT(ptSkill);
	ptSkill->dwCount = dwValue;
}

void __stdcall SkillsClearFlagIfMatched(HUNIT hUnit, DWORD *pdwFlag, DWORD *pdwOut, DWORD *pdwIndex)
{
	static const DWORD scadwIndexTable[16] =
	{
		0, 1, 2, 4, 7, 8, 9, 10, 13, 14, 15, 16, 17, 19, 18, 3,
	};
	static const struct { DWORD dwKey, dwVal; } scatMatchTable[2] =
	{
		{ 63, 0 },
		{ 93, 0 },
	};
	static DWORD sgdwMatchTableCount = 2;
	unsigned i;

	if (!D2CommonStatsUnitTestMatchState(hUnit))
		return;
	switch (*pdwFlag)
	{
	case 1:
		for (i = 0; i < sgdwMatchTableCount; i++)
		{
			if (D2CommonStatsGetState(hUnit, scatMatchTable[i].dwKey))
			{
				*pdwIndex = scadwIndexTable[*pdwIndex];
				*pdwFlag = 0;
				*pdwOut = scatMatchTable[i].dwVal;
				return;
			}
		}
		break;
	}
}

void __stdcall SkillsUpdateNodeSeed(HUNIT hUnit, DWORD dwMode, DWORD dwKey)
{
	PTSKILL ptSkill;
	DWORD dwValue;
	DWORD nT1;
	DWORD nT2;

	ptSkill = SkillsFindByKey(hUnit, dwKey);

	if (dwMode != 0)
	{
		dwValue = (hUnit->eType << 16) + hUnit->dwId;
	}
	else
	{
		nT1 = D2CommonPathGetTargetXRaw((DWORD)hUnit->ptPath);
		nT2 = D2CommonPathGetTargetYRaw((DWORD)hUnit->ptPath);
		dwValue = nT1 + 3 * nT2;
	}

#line 1420
	D2ASSERT(ptSkill);
	if (ptSkill->dwScratchC == dwValue)
	{
		ptSkill->dwScratchD += 7;
		ptSkill->dwScratchC = dwValue;
	}
	else
	{
		ptSkill->dwScratchD = dwValue;
		ptSkill->dwScratchC = dwValue;
	}
	D2CommonSeedSet(&hUnit->tSeed, ptSkill->dwScratchD);
}

DWORD __stdcall SkillsGetNodeRangeForUnit(DWORD dwUnit, HSKILL hSkill)
{
	DWORD dwField;

	dwField = SkillsGetNodeRange(hSkill);
	if (dwField == 3)
		return D2CommonCOMPOSITIsRangedAttack(dwUnit) ? 2 : 1;
	return dwField;
}

BOOL __stdcall SkillsUnitTestGuardF(HUNIT hUnit, DWORD dwGate)
{
	DWORD dwRecord;
	BYTE bGuard;

	bGuard = 0;
	if (hUnit && hUnit->eType == 1 && (hUnit->dwMode == 12 || (dwGate && hUnit->dwMode == 0))
		&& !D2CommonStatsUnitTestStateGroupC(hUnit))
	{
		dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		bGuard = *(BYTE *)(dwRecord + 666);
	}
	if (!bGuard)
		return FALSE;
	dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
	return *(DWORD *)(dwRecord + 412) != 0;
}

BOOL __stdcall SkillsUnitTestGuardD(HUNIT hUnit)
{
	DWORD dwRecord;
	BYTE bGuard;

	bGuard = 0;
	if (hUnit && hUnit->eType == 1 && hUnit->dwMode == 12 && !D2CommonStatsUnitTestStateGroupC(hUnit))
	{
		dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		bGuard = *(BYTE *)(dwRecord + 666);
	}
	if (!bGuard)
		return FALSE;
	return D2CommonUnitTestMonsterSoft(hUnit) != 0;
}

BOOL __stdcall SkillsUnitTestGuardA(HUNIT hUnit)
{
	DWORD dwRecord;
	BYTE bGuard;

	bGuard = 0;
	if (hUnit && hUnit->eType == 1 && hUnit->dwMode == 12 && !D2CommonStatsUnitTestStateGroupC(hUnit))
	{
		dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		bGuard = *(BYTE *)(dwRecord + 666);
	}
	if (!bGuard)
		return FALSE;
	return TRUE;
}

BOOL __stdcall SkillsUnitTestGuardE(HUNIT hUnit)
{
	DWORD dwRecord;
	BYTE bGuard;

	bGuard = 0;
	if (hUnit && hUnit->eType == 1 && hUnit->dwMode == 12 && !D2CommonStatsUnitTestStateGroupC(hUnit))
	{
		dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		bGuard = *(BYTE *)(dwRecord + 666);
	}
	if (!bGuard)
		return FALSE;
	return D2CommonUnitTestMonsterSoft(hUnit) != 0;
}

BOOL __stdcall SkillsUnitTestGuardB(HUNIT hUnit)
{
	DWORD dwRecord;
	BYTE bGuard;

	bGuard = 0;
	if (hUnit && hUnit->eType == 1 && hUnit->dwMode == 12 && !D2CommonStatsUnitTestStateGroupC(hUnit))
	{
		dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		bGuard = *(BYTE *)(dwRecord + 666);
	}
	if (!bGuard)
		return FALSE;
	return TRUE;
}

BOOL __stdcall SkillsUnitTestGuardC(HUNIT hUnit)
{
	DWORD dwRecord;
	BYTE bGuard;

	bGuard = 0;
	if (hUnit && hUnit->eType == 1 && hUnit->dwMode == 12 && !D2CommonStatsUnitTestStateGroupC(hUnit))
	{
		dwRecord = D2CommonMonsterTblsGetStats(hUnit->dwClassId);
		bGuard = *(BYTE *)(dwRecord + 666);
	}
	if (!bGuard)
		return FALSE;
	return TRUE;
}

static struct
{
	DWORD	field_00;
	DWORD	dwKeyA;
	DWORD	field_04;
	DWORD	field_08;
	DWORD	nGroup;
} sgatDifficultyTable[6] =
{
	{ 74, 127, 1,  2,  0 },
	{ 75, 128, 3,  4,  0 },
	{ 76, 129, 10, 11, 0 },
	{ 77, 134, 9,  0,  1 },
	{ 79, 136, 7,  8,  1 },
	{ 78, 135, 14, 14, 1 },
};

static DWORD sgnDifficultyCount = 6;

int __stdcall SkillsGetItemSkillValue(HUNIT hUnit, int nBonus, HUNIT hItem, HSKILL hSkill,
	DWORD dwToHitColumn)
{
	PTSKILL ptNode;
	PTITEMTYPEDATA ptStats;
	DWORD nId;
	ptNode = SkillsFindByKey(hUnit, 135);
	if (ptNode == NULL)
		return 0;
	if (hSkill == NULL)
		hSkill = (HSKILL)D2CommonUnitGetSkillThirdCached(hUnit);
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2323
	D2ASSERT(ptStats);
	if (!ptStats->throwable)
		return 0;
	nId = SkillsGetNodeType(hSkill);
	switch (nId)
	{
	case 2:
	case 4:
	case 140:
		break;
	default:
		return 0;
	}
	return D2CommonSkillsGetDifficultyTableEntry(nBonus + SkillsCalcNodeLevel(hUnit, ptNode, TRUE), 135,
		dwToHitColumn);
}

int __stdcall SkillsGetItemSkillValueAny(HUNIT hUnit, int nBonus, HUNIT hItem, int *pnOut, HSKILL hSkill,
	DWORD dwToHitColumn)
{
	PTSKILL ptNode;
	PTITEMTYPEDATA ptStats;
	DWORD nId;
	DWORD dwType;
	int nValue;
	int nLevel;
	int i;

	if (hItem == NULL)
		return 0;
	if (hSkill == NULL)
		hSkill = (HSKILL)D2CommonUnitGetSkillThirdCached(hUnit);
	ptNode = SkillsFindNode(hUnit, SKILL_THROWING_MASTERY);
	if (ptNode != NULL)
	{
		if (hSkill == NULL)
			hSkill = (HSKILL)D2CommonUnitGetSkillThirdCached(hUnit);
		ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2323
		D2ASSERT(ptStats);
		if (ptStats->throwable)
		{
			nId = SkillsGetNodeType(hSkill);
			switch (nId)
			{
			case 2: case 4: case 140:
				nValue = D2CommonSkillsGetDifficultyTableEntry(nBonus + SkillsCalcNodeLevel(hUnit, ptNode,
					TRUE), 135, dwToHitColumn);
				if (nValue)
					return nValue;
				break;
			}
		}
	}
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2358
	D2ASSERT(ptStats);
	dwType = ptStats->bWeaponGroup;
	for (i = 0; i < (int)sgnDifficultyCount; i++)
	{
		ptNode = SkillsFindByKey(hUnit, sgatDifficultyTable[i].dwKeyA);
		if (ptNode == NULL)
			continue;
		if ((dwType != 0 && dwType == sgatDifficultyTable[i].field_04)
			|| dwType == sgatDifficultyTable[i].field_08)
		{
			nLevel = SkillsCalcNodeLevel(hUnit, ptNode, TRUE) + nBonus;
			if (pnOut != NULL)
				*pnOut = nLevel;
			return D2CommonSkillsGetDifficultyTableEntry(nLevel, sgatDifficultyTable[i].dwKeyA,
				dwToHitColumn);
		}
	}
	return 0;
}

BOOL __stdcall SkillsAddFieldA(DWORD dwUnitX, DWORD dwUnitY, DWORD dwTargetX, DWORD dwTargetY, DWORD dwRoom,
	DWORD dwPattern)
{
	DWORD hField;
	int nT;
	DWORD dwResult;

	nT = dwTargetX - dwUnitX;
	if ((nT < 0 ? -nT : nT) >= D2CommonFieldTblsGetWidth())
		return FALSE;

	nT = dwTargetY - dwUnitY;
	if ((nT < 0 ? -nT : nT) >= D2CommonFieldTblsGetHeight())
		return FALSE;
	hField = D2CommonFieldTblsAllocPairRaw();
#line 2405
	D2ASSERT(hField);
	D2CommonFieldTblsSetPairRaw(hField, dwUnitX, dwUnitY);
	dwResult = !D2CommonFieldTblsFindCandidateRaw(hField, dwRoom, dwTargetX, dwTargetY, dwPattern);
	D2CommonFieldTblsFreePairRaw(hField);
	return dwResult;
}

BOOL __stdcall SkillsAddFieldB(DWORD dwUnitX, DWORD dwUnitY, DWORD dwTarget, DWORD dwPattern)
{
	void *hField;
	DWORD dwResult1, dwResult2, dwResult3;
	int nDx, nDy, nT;
	DWORD dwResult;

	dwResult1 = D2CommonUnitGetPathRoom(dwTarget);
	dwResult2 = D2CommonUnitGetY(dwTarget);
	dwResult3 = D2CommonUnitGetX(dwTarget);
	nT = (int)(dwResult3 - dwUnitX);
	nDx = nT < 0 ? -nT : nT;
	if (nDx >= D2CommonFieldTblsGetWidth())
		return FALSE;
	nT = (int)(dwResult2 - dwUnitY);
	nDy = nT < 0 ? -nT : nT;
	if (nDy >= D2CommonFieldTblsGetHeight())
		return FALSE;
	hField = (void *)D2CommonFieldTblsAllocPair();
#line 2405
	D2ASSERT(hField);
	D2CommonFieldTblsSetPair(hField, dwUnitX, dwUnitY);
	dwResult = D2CommonFieldTblsFindCandidate(hField, dwResult1, dwResult3, dwResult2, dwPattern) == 0;
	D2CommonFieldTblsFreePair(hField);
	return dwResult;
}

int __stdcall SkillsCalcManaPoints(DWORD nSkillId, int nLevel)
{
	PTSKILLTYPEINFO ptSkill;
	int n;

	ptSkill = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
#line 2440
	D2ASSERT(ptSkill);
	n = ptSkill->mana + ptSkill->lvlmana * (nLevel - 1);
	n = (n << ptSkill->manashift) >> 8;
	if (n < 0)
		n = 0;
	return n;
}

int __stdcall SkillsCalcCountPercent(int nCount)
{
	return (100 * (nCount + 1)) / (nCount + 8);
}

DWORD __stdcall SkillsGetBonusByKey(HUNIT hUnit, DWORD dwKey)
{
	PTSKILLHEAD ptInfo;
	PTSKILL ptNode;

	ptInfo = hUnit->field_c8;
#line 341
	D2ASSERT(ptInfo);
	ptNode = ptInfo->ptFirst;
	while (ptNode != NULL && ptNode->ptType->id != dwKey)
		ptNode = ptNode->ptNext;
	if (ptNode)
		return ptNode->dwSkillBonus;
	return 0;
}

void __stdcall SkillsSetSkillBonus(HUNIT hUnit, DWORD nSkillId, int nBonus)
{
	PTSKILL ptSkill;

	ptSkill = SkillsFindByKey(hUnit, nSkillId);
	if (ptSkill == NULL)
	{
		if (nBonus <= 0)
			return;
		ptSkill = SkillsAddSkill(hUnit, nSkillId);
#line 2490
		D2ASSERTMSG(ptSkill, "Attempt to set skill level for a skill you can't get!\nTell Ted Bisson");
		SkillsSetSkillLevel(hUnit, nSkillId, 0, FALSE);
	}
	ptSkill->dwSkillBonus = nBonus;
	SkillsUpdatePassive(hUnit, nSkillId);
}

void __stdcall SkillsAdjustBonusByKey(HUNIT hUnit, DWORD dwKey, int nDelta)
{
	PTSKILL ptNode;
	int n;

	ptNode = SkillsFindByKey(hUnit, dwKey);
	if (ptNode == NULL)
	{
		if (nDelta <= 0)
			return;
		ptNode = SkillsAddSkill(hUnit, dwKey);
#line 2516
		D2ASSERTMSG(ptNode, "Attempt to change skill level for a skill you can't get!\nTell Ted Bisson");
		SkillsSetSkillLevel(hUnit, dwKey, 0, FALSE);
	}
	n = ptNode->dwSkillBonus + nDelta;
	ptNode->dwSkillBonus = n;
	if (n < 0)
		ptNode->dwSkillBonus = 0;
}

static const int scanFixedTable4[4] =
{
	0, 0, 28, 30,
};

int __stdcall SkillsGetDifficultyTableEntry(int nLevel, DWORD dwKey, int nToHitColumn)
{
	int i;
	int nGroup;
	int n;

	for (i = 0; i < (int)sgnDifficultyCount; i++)
	{
		if (dwKey == sgatDifficultyTable[i].dwKeyA)
			break;
	}
	if (i >= (int)sgnDifficultyCount)
		nGroup = 0;
	else
	{
		nGroup = sgatDifficultyTable[i].nGroup;
#line 2557
		D2ASSERT(nGroup < 2);
	}
	if (nLevel < 2)
		return scanFixedTable4[nGroup + nLevel * 2];
	n = (nToHitColumn != 0) ? 8 : 5;
	return scanFixedTable4[nGroup + 2] + n * (nLevel - 1);
}

int __stdcall SkillsCalcCurveValue(int nLevel, int nMin, int nMax)
{
	int nQ;
	int nResult;

	nQ = (110 * nLevel) / (nLevel + 6);
	nResult = nMin + (nQ * (nMax - nMin)) / 100;
	if (nResult > nMax) return nMax; return nResult;
}

int __stdcall SkillsCalcCurveParamOne(int nLevel, DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (!(ptType == NULL)) {
		return SkillsCalcCurveValue(nLevel, ptType->Param1, ptType->Param2); }
	return 0;
}

int __stdcall SkillsCalcCurveParamThree(int nLevel, DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (!(ptType == NULL)) {
		return SkillsCalcCurveValue(nLevel, ptType->Param3, ptType->Param4); }
	return 0;
}

int __stdcall SkillsCalcCurveParamFive(int nLevel, DWORD nSkillId)
{
	PTSKILLTYPEINFO ptType;

	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(nSkillId);
	if (!(ptType == NULL)) {
		return SkillsCalcCurveValue(nLevel, ptType->Param5, ptType->Param6); }
	return 0;
}

struct SKILLCOORD
{
 	int			nX;
 	int			nY;
};
typedef struct SKILLCOORD SKILLCOORD;

BOOL __stdcall SkillsGetFieldPastTarget(HUNIT hUnit, HUNIT hTarget, int *pnX, int *pnY)
{
	int nX;
	int nY;
	int nTargetX;
	int nTargetY;
	int nDist;
	int nSpotX;
	int nSpotY;
	int nUnitX;
	int nUnitY;
	DWORD dwPattern;
	void *hRoom;

	if (hTarget != NULL && D2CommonUnitTestPositionClear(hUnit, hTarget, 0))
		return FALSE;
	nX = (int)D2CommonUnitGetX(hUnit);
	nY = (int)D2CommonUnitGetY(hUnit);
	if (hTarget != NULL)
	{
		nTargetX = (int)D2CommonUnitGetX(hTarget);
		nTargetY = (int)D2CommonUnitGetY(hTarget);
	}
	else
	{
		nTargetX = (int)D2CommonPathGetTargetXRaw((DWORD)hUnit->ptPath);
		nTargetY = (int)D2CommonPathGetTargetYRaw((DWORD)hUnit->ptPath);
	}
	nDist = D2CommonUnitUtilGetDistance(hUnit, nTargetX, nTargetY);
	if (nDist == 0)
		return FALSE;
	nSpotX = nTargetX + 2 * (nTargetX - nX) / nDist;
	nSpotY = nTargetY + 2 * (nTargetY - nY) / nDist;
	hRoom = (void *)D2CommonUnitGetPathRoom(hUnit);
#line 2668
	D2ASSERT(hRoom);
	dwPattern = D2CommonPathGetCollisionPatternByUnit(hUnit);
	if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nSpotX, nSpotY, dwPattern, 7177))
	{ SKILLCOORD tCoord;
		tCoord.nX = nTargetX;
		tCoord.nY = nTargetY;
		hRoom = D2CommonCollisnFindSpawnPointNear(hRoom, &tCoord, D2CommonUnitGetSizeX(hUnit),
			7177, 0);
		if (hRoom == NULL)
			return FALSE;
		nSpotX = tCoord.nX;
		nSpotY = tCoord.nY;
	}
	nUnitY = (int)D2CommonUnitGetY(hUnit);
	nUnitX = (int)D2CommonUnitGetX(hUnit);
	if (!SkillsAddFieldA(nUnitX, nUnitY, nSpotX, nSpotY, (DWORD)hRoom, 2052))
		return FALSE;
	*pnX = nSpotX;
	*pnY = nSpotY;
	return TRUE;
}

BOOL __stdcall SkillsAddListRange(HUNIT hUnit, int *pnMin, int *pnMax, BOOL fThird)
{
	PTSTATLIST ptList;
	PTSKILLTYPEINFO ptType;
	int nLevel;
	int nMin;
	int nMax;
	int nPercent;

	if (hUnit == NULL)
		return FALSE;
	ptList = (PTSTATLIST)D2CommonStatsFindListByState(hUnit, 16);
	if (ptList == NULL)
		return FALSE;
	ptType = (PTSKILLTYPEINFO)D2CommonDataTblsGetSkill(SKILL_ENCHANT);
	nLevel = (int)D2CommonStatsListGetValueA(ptList, 165);
	nMin = (int)(ptType->EMin + ptType->ELev * (nLevel - 1));
	nMax = (int)(ptType->EMax + ptType->ELev * (nLevel - 1));
	if (fThird)
	{
		nMin /= 3;
		nMax /= 3;
	}
	if (hUnit->dwClassId == 1 && D2CommonStatsGetState(hUnit, 71))
	{

		nPercent = SkillsCalcCurveParamOne(((int)SkillsCalcNodeLevel(hUnit, SkillsFindByKey(hUnit, 61), TRUE)), SKILL_FIRE_MASTERY);
		nMin += nMin * nPercent / 100;
		nMax += nMax * nPercent / 100;
	}
	*pnMin += nMin;
	*pnMax += nMax;
	return TRUE;
}

DWORD __stdcall SkillsGetRangedAttackCost(HUNIT hUnit)
{
	DWORD dwClass;

	dwClass = D2CommonUnitGetItemFlagsWord(hUnit);
#line 2776
	D2ASSERTMSG(D2CommonCOMPOSITIsRangedAttack(hUnit), "COMPOSITIsRangedAttack (hUnit)");
	if (dwClass == 1)
	{
		if (D2CommonStatsGetTotalValue(hUnit, 157))
			return 27;
		if (D2CommonStatsGetTotalValue(hUnit, 158))
			return 41;
		return 0;
	}
	if (dwClass == 7)
	{
		if (D2CommonStatsGetTotalValue(hUnit, 157))
			return 27;
		if (D2CommonStatsGetTotalValue(hUnit, 158))
			return 41;
		return 31;
	}
	return 65535;
}

static const BYTE scabSkillIdFlag[221] =
{
	0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
};

BOOL __stdcall SkillsTestIdFlag(DWORD nSkillId)
{
	switch (nSkillId)
	{
	case 0: case 1: case 2: case 3: case 4: case 5:
	case 217: case 218: case 219: case 220:
		return FALSE;
	}
	return TRUE;
}
