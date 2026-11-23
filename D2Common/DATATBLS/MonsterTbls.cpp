#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\MonsterTbls.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

struct TREASURECLASSDATATBLDESC
{
 	int			nSize;
 	PTTREASURECLASSTXT	ptTreasureClass;
};
typedef struct TREASURECLASSDATATBLDESC TREASURECLASSDATATBLDESC;

struct MONITEMPERCENTDATATBLDESC
{
 	int			nSize;
 	PTMONITEMPERCENTTXT	ptPercent;
};
typedef struct MONITEMPERCENTDATATBLDESC MONITEMPERCENTDATATBLDESC;

struct MONSTATSRUN
{
 	int			nRunCount;
 	int			nRunIndex;
};
typedef struct MONSTATSRUN MONSTATSRUN, *PTMONSTATSRUN;
static MONITEMPERCENTDATATBLDESC sgtMonItemPercentDataTblDesc = { 0 };
PTMONSTATSRUN gptMonStatsRuns = NULL;
int gnMonStatsRunsUnread = 0;
int gnUniqueTitle = 0;
PTUNIQUENAMETXT gptUniqueTitleTxt = NULL;
int gnUniquePrefix = 0;
PTUNIQUENAMETXT gptUniquePrefixTxt = NULL;
int gnUniqueSuffix = 0;
PTUNIQUENAMETXT gptUniqueSuffixTxt = NULL;
int gnUniqueAppellation = 0;
PTUNIQUENAMETXT gptUniqueAppellationTxt = NULL;
static PTMONSTATS sgptMonsterInitStats = NULL;
PTSUPERUNIQUESTXT gptSuperUniquesTxt = NULL;
static TREASURECLASSDATATBLDESC sgtTreasureClassDataTblDesc = { 0 };

BOOL __stdcall MonsterTblsGetRun(int nClass, int *pnRunCount, int *pnRunIndex)
{
	PTMONSTATSRUN ptRun = &gptMonStatsRuns[nClass];

	*pnRunCount = ptRun->nRunCount;
	*pnRunIndex = ptRun->nRunIndex;
	switch (nClass)
	{
	case 274:
	case 275:
	case 276:
	case 277:
	case 379:
	case 380:
	case 381:
	case 382:
	case 383:
	case 384:
	case 385:
	case 386:
	case 387:
	case 388:
	case 389:
	case 390:
		return TRUE;
	}
	return FALSE;
}

static __inline void MonsterTblsInitSpeeds(int nSize)
{
	DWORD dwLastToken;
	DWORD dwSpeed;
	DWORD dwVelocity;
	DWORD dwToken;
	int i;

	D2_OPAQUE(sgptMonsterInitStats);
#line 51
	D2ASSERT(sgptMonsterInitStats);
#line 53
	D2ASSERT(nSize == NUM_MONSTERCLASS);
	dwSpeed = 0;
	dwLastToken = 0;
	dwVelocity = 0;
	for (i = 0; i < NUM_MONSTERCLASS; i++)
	{
		dwToken = ((PTTOKENTXT)D2CommonTokTblsGetMonsterToken(i, 0))->token;
		if (dwToken != dwLastToken)
		{
			dwLastToken = dwToken;
			dwSpeed = AnimTblsFindSequence(0, (DWORD)i, 2, 1, 0)->dwSpeed;
			sgptMonsterInitStats[i].nWalkAnimRate = (int)dwSpeed;
			dwVelocity = (DWORD)sgptMonsterInitStats[i].nVelocity;
		}
		else if (dwVelocity)
			sgptMonsterInitStats[i].nWalkAnimRate =
				(int)((DWORD)sgptMonsterInitStats[i].nVelocity * dwSpeed / dwVelocity);
		else
			sgptMonsterInitStats[i].nWalkAnimRate = (int)dwSpeed;
	}
}

void __fastcall MonsterTblsLoadMonStats(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Class", EXCEL_TYPE_STRING, 59, offsetof(MONSTATS, Class) },
		{ "Descriptor", EXCEL_TYPE_STRING, 59, offsetof(MONSTATS, Descriptor) },
		{ "BaseId", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, nBaseId) },
		{ "Spawned", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Spawned) },
		{ "Code", EXCEL_TYPE_STRING, 4, offsetof(MONSTATS, Code) },
		{ "NoMap", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, NoMap) },
		{ "SizeX", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, dwSizeX) },
		{ "SizeY", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, SizeY) },
		{ "Height", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Height) },
		{ "OverlayHeight", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, OverlayHeight) },
		{ "Velocity", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, nVelocity) },
		{ "Run", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Run) },
		{ "Level", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Level[0]) },
		{ "Level(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Level[1]) },
		{ "Level(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Level[2]) },
		{ "MeleeRng", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bMeleeRng) },
		{ "Rarity", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Rarity) },
		{ "MinGrp", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bMinGrp) },
		{ "MaxGrp", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bMaxGrp) },
		{ "HD", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, HD) },
		{ "TR", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TR) },
		{ "LG", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LG) },
		{ "RA", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, RA) },
		{ "LA", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LA) },
		{ "RH", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, RH) },
		{ "LH", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LH) },
		{ "SH", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, SH) },
		{ "S1", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[0]) },
		{ "S2", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[1]) },
		{ "S3", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[2]) },
		{ "S4", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[3]) },
		{ "S5", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[4]) },
		{ "S6", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[5]) },
		{ "S7", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[6]) },
		{ "S8", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S[7]) },
		{ "TotalPieces", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TotalPieces) },
		{ "BaseW", EXCEL_TYPE_CODE, 0, offsetof(MONSTATS, BaseW) },
		{ "AIParam1", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, AIParam[0]) },
		{ "AIParam2", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, AIParam[1]) },
		{ "AIParam3", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, AIParam[2]) },
		{ "AIParam4", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, AIParam[3]) },
		{ "AIParam5", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, AIParam[4]) },
		{ "ModeDH", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeDH) },
		{ "ModeN", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeN) },
		{ "ModeW", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeW) },
		{ "ModeGH", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeGH) },
		{ "ModeA1", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeA1) },
		{ "ModeA2", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeA2) },
		{ "ModeB", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeB) },
		{ "ModeC", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeC) },
		{ "ModeS1", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeS1) },
		{ "ModeS2", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeS2) },
		{ "ModeS3", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeS3) },
		{ "ModeS4", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeS4) },
		{ "ModeDD", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeDD) },
		{ "ModeKB", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeKB) },
		{ "ModeSQ", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ModeSQ) },
		{ "ModeRN", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bModeRN) },
		{ "ElMode", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ElMode) },
		{ "ElType", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ElType) },
		{ "ElPct", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ElPct) },
		{ "ElMinD", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ElMinD) },
		{ "ElMaxD", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ElMaxD) },
		{ "ElDur", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ElDur) },
		{ "MissA1", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissA1) },
		{ "MissA2", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissA2) },
		{ "MissS1", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, dwMissS1) },
		{ "MissS2", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissS2) },
		{ "MissS3", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissS3) },
		{ "MissS4", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissS4) },
		{ "MissC", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissC) },
		{ "MissSQ", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MissSQ) },
		{ "A1Move", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, A1Move) },
		{ "A2Move", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, A2Move) },
		{ "S1Move", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S1Move) },
		{ "S2Move", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S2Move) },
		{ "S3Move", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S3Move) },
		{ "S4Move", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, S4Move) },
		{ "CMove", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bCMove) },
		{ "Align", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bAlign) },
		{ "IsMelee", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bIsMelee) },
		{ "IsSel", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, IsSel) },
		{ "IsSel2", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, IsSel2) },
		{ "NeverSel", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, NeverSel) },
		{ "CorpseSel", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, CorpseSel) },
		{ "IsAtt", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bIsAtt) },
		{ "IsNPC", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bIsNPC) },
		{ "IsCritter", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bIsCritter) },
		{ "InTown", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bInTown) },
		{ "Bleed", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Bleed) },
		{ "Shadow", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Shadow) },
		{ "Light", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Light) },
		{ "NoUniqueShift", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, NoUniqueShift) },
		{ "CompositeDeath", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, CompositeDeath) },
		{ "Skill1", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Skill1) },
		{ "Skill1Seq", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill1Seq) },
		{ "Skill1Lvl", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill1Lvl) },
		{ "Skill2", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Skill2) },
		{ "Skill2Seq", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill2Seq) },
		{ "Skill2Lvl", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill2Lvl) },
		{ "Skill3", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Skill3) },
		{ "Skill3Seq", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill3Seq) },
		{ "Skill3Lvl", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill3Lvl) },
		{ "Skill4", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Skill4) },
		{ "Skill4Seq", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill4Seq) },
		{ "Skill4Lvl", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill4Lvl) },
		{ "Skill5", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Skill5) },
		{ "Skill5Seq", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill5Seq) },
		{ "Skill5Lvl", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, Skill5Lvl) },
		{ "LightR", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LightR) },
		{ "LightG", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LightG) },
		{ "LightB", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LightB) },
		{ "DamageResist", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, DamageResist[0]) },
		{ "DamageResist(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, DamageResist[1]) },
		{ "DamageResist(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, DamageResist[2]) },
		{ "MagicResist", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, MagicResist[0]) },
		{ "MagicResist(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, MagicResist[1]) },
		{ "MagicResist(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, MagicResist[2]) },
		{ "FireResist", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, FireResist[0]) },
		{ "FireResist(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, FireResist[1]) },
		{ "FireResist(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, FireResist[2]) },
		{ "LightResist", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LightResist[0]) },
		{ "LightResist(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LightResist[1]) },
		{ "LightResist(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, LightResist[2]) },
		{ "ColdResist", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ColdResist[0]) },
		{ "ColdResist(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ColdResist[1]) },
		{ "ColdResist(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, ColdResist[2]) },
		{ "PoisonResist", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, PoisonResist[0]) },
		{ "PoisonResist(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, PoisonResist[1]) },
		{ "PoisonResist(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, PoisonResist[2]) },
		{ "DamageRegen", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, nDamageRegen) },
		{ "eLUndead", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bLUndead) },
		{ "eHUndead", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bHUndead) },
		{ "eDemon", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, eDemon) },
		{ "eMagicUsing", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, eMagicUsing) },
		{ "eLarge", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, eLarge) },
		{ "eSmall", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, eSmall) },
		{ "eFlying", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bFlying) },
		{ "eOpenDoors", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bOpenDoors) },
		{ "eBoss", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, eBoss) },
		{ "eSpawnCol", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bSpawnCol) },
		{ "PixHeight", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, PixHeight) },
		{ "Interact", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bInteract) },
		{ "MinHP", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, anMinHP[0]) },
		{ "MinHP(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, anMinHP[1]) },
		{ "MinHP(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, anMinHP[2]) },
		{ "MaxHP", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MaxHP[0]) },
		{ "MaxHP(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MaxHP[1]) },
		{ "MaxHP(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, MaxHP[2]) },
		{ "AC", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, AC[0]) },
		{ "AC(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, AC[1]) },
		{ "AC(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, AC[2]) },
		{ "Exp", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Exp[0]) },
		{ "Exp(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Exp[1]) },
		{ "Exp(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, Exp[2]) },
		{ "ToBlock", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bToBlock) },
		{ "A1MinD", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, pnAttack1MinDamage[0]) },
		{ "A1MinD(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, pnAttack1MinDamage[1]) },
		{ "A1MinD(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, pnAttack1MinDamage[2]) },
		{ "A1MaxD", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, pnAttack1MaxDamage[0]) },
		{ "A1MaxD(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, pnAttack1MaxDamage[1]) },
		{ "A1MaxD(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, pnAttack1MaxDamage[2]) },
		{ "A1ToHit", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A1ToHit[0]) },
		{ "A1ToHit(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A1ToHit[1]) },
		{ "A1ToHit(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A1ToHit[2]) },
		{ "A2MinD", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2MinD[0]) },
		{ "A2MinD(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2MinD[1]) },
		{ "A2MinD(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2MinD[2]) },
		{ "A2MaxD", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2MaxD[0]) },
		{ "A2MaxD(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2MaxD[1]) },
		{ "A2MaxD(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2MaxD[2]) },
		{ "A2ToHit", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2ToHit[0]) },
		{ "A2ToHit(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2ToHit[1]) },
		{ "A2ToHit(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, A2ToHit[2]) },
		{ "S1MinD", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1MinD[0]) },
		{ "S1MinD(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1MinD[1]) },
		{ "S1MinD(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1MinD[2]) },
		{ "S1MaxD", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1MaxD[0]) },
		{ "S1MaxD(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1MaxD[1]) },
		{ "S1MaxD(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1MaxD[2]) },
		{ "S1ToHit", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1ToHit[0]) },
		{ "S1ToHit(N)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1ToHit[1]) },
		{ "S1ToHit(H)", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, S1ToHit[2]) },
		{ "SpawnComponents", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bSpawnComponents) },
		{ "Soft", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bSoft) },
		{ "Heart", EXCEL_TYPE_CODE, 0, offsetof(MONSTATS, Heart) },
		{ "BodyPart", EXCEL_TYPE_CODE, 0, offsetof(MONSTATS, BodyPart) },
		{ "TreasureClass1", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[0][0]) },
		{ "TreasureClass2", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[0][1]) },
		{ "TreasureClass3", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[0][2]) },
		{ "TreasureClass4", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[0][3]) },
		{ "TreasureClass1(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[1][0]) },
		{ "TreasureClass2(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[1][1]) },
		{ "TreasureClass3(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[1][2]) },
		{ "TreasureClass4(N)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[1][3]) },
		{ "TreasureClass1(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[2][0]) },
		{ "TreasureClass2(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[2][1]) },
		{ "TreasureClass3(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[2][2]) },
		{ "TreasureClass4(H)", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, TreasureClass[2][3]) },
		{ "SpawnPctBonus", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bSpawnPctBonus) },
		{ "Killable", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bKillable) },
		{ "Switch", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bSwitch) },
		{ "Restore", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bRestore) },
		{ "NeverCount", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bNeverCount) },
		{ "HitClass", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, HitClass) },
		{ "SplEndDeath", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, SplEndDeath) },
		{ "SplGetModeChart", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, SplGetModeChart) },
		{ "SplEndGeneric", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, SplEndGeneric) },
		{ "SplClientEnd", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, SplClientEnd) },
		{ "DeadCollision", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bDeadCollision) },
		{ "UnflatDead", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, UnflatDead) },
		{ "BloodLocal", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, BloodLocal) },
		{ "DeathDamage", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bDeathDamage) },
		{ "PetIgnore", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, bPetIgnore) },
		{ "NoGfxHitTest", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, NoGfxHitTest) },
		{ "HitTestTop", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, HitTestTop) },
		{ "HitTestLeft", EXCEL_TYPE_DWORD, 0, offsetof(MONSTATS, HitTestLeft) },
		{ "HitTestWidth", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, HitTestWidth) },
		{ "HitTestHeight", EXCEL_TYPE_BYTE, 0, offsetof(MONSTATS, HitTestHeight) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSize;
	PTMONSTATS ptMonStats;
	PTMONSTATSRUN ptRun;
	PTMONSTATS ptCrawl;
	int nBaseId;
	int nCount;
	int i;
	DWORD j;
	int k;

	ptMonStats = (PTMONSTATS)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nSize,
		sizeof(MONSTATS));
	for (i = 0; i < nSize; i++)
	{
		for (k = 0; k < 15; k++)
			((DWORD *)ptMonStats[i].awName)[k] = 0;
		for (k = 0; k < 15; k++)
			((DWORD *)ptMonStats[i].awDescriptor)[k] = 0;
		(void)D2LangUnicodestrncpy(ptMonStats[i].awName, D2LangStrGetStringByKey(ptMonStats[i].Class),
			60);
		(void)D2LangUnicodestrncpy(ptMonStats[i].awDescriptor,
			D2LangStrGetStringByKey(ptMonStats[i].Descriptor), 60);
		ptMonStats[i].Rarity++;
	}
	sgptMonsterInitStats = ptMonStats;
	j = nSize;
#line 94
	ptRun = (PTMONSTATSRUN)D2ALLOCPOOL(NULL, nSize * sizeof(MONSTATSRUN));
	gptMonStatsRuns = ptRun;
	ptCrawl = sgptMonsterInitStats;
#line 99
	D2ASSERT(ptCrawl != NULL);
	nBaseId = ptCrawl->nBaseId;
	nCount = 0;
	for (; j > 0; j--)
	{
		if (nBaseId != ptCrawl->nBaseId)
		{
			nBaseId = ptCrawl->nBaseId;
			for (k = 0; k < nCount; k++)
			{
				ptRun->nRunCount = nCount;
				ptRun->nRunIndex = k;
				ptRun++;
			}
			nCount = 0;
		}
		nCount++;
		ptCrawl++;
	}
	for (k = 0; k < nCount; k++)
	{
		ptRun->nRunCount = nCount;
		ptRun->nRunIndex = k;
		ptRun++;
	}
	MonsterTblsInitSpeeds(nSize);
}

void __fastcall MonsterTblsFreeMonStats(void)
{
	if (sgptMonsterInitStats)
		DataTblsFreeBlock(sgptMonsterInitStats);
	sgptMonsterInitStats = NULL;
	if (gptMonStatsRuns)
	{
#line 429
		D2FREEPOOL(NULL, gptMonStatsRuns);
	}
	gptMonStatsRuns = NULL;
	if (gptUniqueTitleTxt)
		DataTblsFreeBlock(gptUniqueTitleTxt);
	gptUniqueTitleTxt = NULL;
	if (gptUniquePrefixTxt)
		DataTblsFreeBlock(gptUniquePrefixTxt);
	gptUniquePrefixTxt = NULL;
	if (gptUniqueSuffixTxt)
		DataTblsFreeBlock(gptUniqueSuffixTxt);
	gptUniqueSuffixTxt = NULL;
	if (gptUniqueAppellationTxt)
		DataTblsFreeBlock(gptUniqueAppellationTxt);
	gptUniqueAppellationTxt = NULL;
	if (gptSuperUniquesTxt)
		DataTblsFreeBlock(gptSuperUniquesTxt);
	gptSuperUniquesTxt = NULL;
}

PTMONSTATS __stdcall MonsterTblsGetStats(int nClass)
{
#line 456
	D2ASSERT(sgptMonsterInitStats);
	return &sgptMonsterInitStats[nClass];
}

void __stdcall MonsterTblsSetVelocity(int nClass, int nVelocity)
{
#line 463
	D2ASSERT(sgptMonsterInitStats);
	sgptMonsterInitStats[nClass].nVelocity = nVelocity;
}

void __fastcall MonsterTblsLoadUniqueNames(void *pBuffer, int nBufferSize, int nType)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Name", EXCEL_TYPE_STRING, 59, offsetof(UNIQUENAMETXT, Name) },
		{ "MonType1", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[0]) },
		{ "MonType2", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[1]) },
		{ "MonType3", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[2]) },
		{ "MonType4", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[3]) },
		{ "MonType5", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[4]) },
		{ "MonType6", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[5]) },
		{ "MonType7", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[6]) },
		{ "MonType8", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[7]) },
		{ "MonType9", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[8]) },
		{ "MonType10", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[9]) },
		{ "MonType11", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[10]) },
		{ "MonType12", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[11]) },
		{ "MonType13", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[12]) },
		{ "MonType14", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[13]) },
		{ "MonType15", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[14]) },
		{ "MonType16", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[15]) },
		{ "MonType17", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[16]) },
		{ "MonType18", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[17]) },
		{ "MonType19", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[18]) },
		{ "MonType20", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[19]) },
		{ "MonType21", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[20]) },
		{ "MonType22", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[21]) },
		{ "MonType23", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[22]) },
		{ "MonType24", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[23]) },
		{ "MonType25", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[24]) },
		{ "MonType26", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[25]) },
		{ "MonType27", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[26]) },
		{ "MonType28", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[27]) },
		{ "MonType29", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[28]) },
		{ "MonType30", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[29]) },
		{ "MonType31", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[30]) },
		{ "MonType32", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[31]) },
		{ "MonType33", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[32]) },
		{ "MonType34", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[33]) },
		{ "MonType35", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[34]) },
		{ "MonType36", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUENAMETXT, MonType[35]) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int *pnCount;
	PTUNIQUENAMETXT *pptNames;
	DWORD i;

	switch (nType)
	{
	case 0:
		pnCount = &gnUniqueTitle;
		pptNames = &gptUniqueTitleTxt;
		break;
	case 1:
		pnCount = &gnUniquePrefix;
		pptNames = &gptUniquePrefixTxt;
		break;
	case 2:
		pnCount = &gnUniqueSuffix;
		pptNames = &gptUniqueSuffixTxt;
		break;
	default:
		pnCount = &gnUniqueAppellation;
		pptNames = &gptUniqueAppellationTxt;
		break;
	}
	*pptNames = (PTUNIQUENAMETXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, pnCount,
		sizeof(UNIQUENAMETXT));
	for (i = 0; i < (DWORD)*pnCount; i++)
	{
		if (strlen((*pptNames)[i].Name))
		{
			memset((*pptNames)[i].wszName, 0, 60);
			(void)D2LangUnicodestrncpy((*pptNames)[i].wszName,
				D2LangStrGetStringByKey((*pptNames)[i].Name), 59);
		}
	}
}

WORD * __fastcall MonsterTblsGetRandomUniqueTitle(PTUNIT hUnit)
{
	DWORD dwIndex;

	if (gnUniqueTitle)
		dwIndex = D2CommonSeedRand(&hUnit->tSeed, gnUniqueTitle);
	else
		dwIndex = 0;
	return gptUniqueTitleTxt[dwIndex].wszName;
}

WORD * __fastcall MonsterTblsGetRandomUniquePrefix(PTUNIT hUnit)
{
	DWORD dwIndex;

	if (gnUniquePrefix)
		dwIndex = D2CommonSeedRand(&hUnit->tSeed, gnUniquePrefix);
	else
		dwIndex = 0;
	return gptUniquePrefixTxt[dwIndex].wszName;
}

WORD * __fastcall MonsterTblsRollUniqueSuffix(PTUNIT hUnit)
{
	DWORD dwIndex;

	if (gnUniqueSuffix)
		dwIndex = D2CommonSeedRand(&hUnit->tSeed, gnUniqueSuffix);
	else
		dwIndex = 0;
	return gptUniqueSuffixTxt[dwIndex].wszName;
}

WORD * __fastcall MonsterTblsRollUniqueAppellation(PTUNIT hUnit)
{
	DWORD dwIndex;

	if (gnUniqueAppellation)
		dwIndex = D2CommonSeedRand(&hUnit->tSeed, gnUniqueAppellation);
	else
		dwIndex = 0;
	return gptUniqueAppellationTxt[dwIndex].wszName;
}

PTTREASURECLASSTXT __fastcall MonsterTblsLoadTreasureClass(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "NumCodes", EXCEL_TYPE_BYTE, 0, offsetof(TREASURECLASSTXT, NumCodes) },
		{ "Code1", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[0]) },
		{ "Code2", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[1]) },
		{ "Code3", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[2]) },
		{ "Code4", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[3]) },
		{ "Code5", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[4]) },
		{ "Code6", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[5]) },
		{ "Code7", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[6]) },
		{ "Code8", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[7]) },
		{ "Code9", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[8]) },
		{ "Code10", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[9]) },
		{ "Code11", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[10]) },
		{ "Code12", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[11]) },
		{ "Code13", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[12]) },
		{ "Code14", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[13]) },
		{ "Code15", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[14]) },
		{ "Code16", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[15]) },
		{ "Code17", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[16]) },
		{ "Code18", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[17]) },
		{ "Code19", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[18]) },
		{ "Code20", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[19]) },
		{ "Code21", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[20]) },
		{ "Code22", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[21]) },
		{ "Code23", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[22]) },
		{ "Code24", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[23]) },
		{ "Code25", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[24]) },
		{ "Code26", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[25]) },
		{ "Code27", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[26]) },
		{ "Code28", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[27]) },
		{ "Code29", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[28]) },
		{ "Code30", EXCEL_TYPE_CODE, 0, offsetof(TREASURECLASSTXT, Code[29]) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};

	sgtTreasureClassDataTblDesc.ptTreasureClass = (PTTREASURECLASSTXT)DataTblsAllocAndTokenize(tDesc,
		(char *)pBuffer, nBufferSize, &sgtTreasureClassDataTblDesc.nSize, sizeof(TREASURECLASSTXT));
	return sgtTreasureClassDataTblDesc.ptTreasureClass;
}

void __fastcall MonsterTblsFreeTreasureClass(void)
{
	DataTblsFreeBlock(sgtTreasureClassDataTblDesc.ptTreasureClass);
}

TREASURECLASSDATATBLDESC * __fastcall MonsterTblsGetTreasureClassDesc(void)
{
	return &sgtTreasureClassDataTblDesc;
}

PTTREASURECLASSTXT __stdcall MonsterTblsGetTreasureClass(int nIndex)
{
	if (nIndex < sgtTreasureClassDataTblDesc.nSize && nIndex != -1)
	{
#line 694
		D2ASSERT(sgtTreasureClassDataTblDesc.ptTreasureClass);
#line 695
		D2ASSERT((sgtTreasureClassDataTblDesc.ptTreasureClass + nIndex));
		return sgtTreasureClassDataTblDesc.ptTreasureClass + nIndex;
	}
	return NULL;
}

PTMONITEMPERCENTTXT __fastcall MonsterTblsLoadMonItemPercent(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "HeartPercent", EXCEL_TYPE_BYTE, 0, offsetof(MONITEMPERCENTTXT, HeartPercent) },
		{ "BodyPartPercent", EXCEL_TYPE_BYTE, 0, offsetof(MONITEMPERCENTTXT, BodyPartPercent) },
		{ "TreasureClassPercent", EXCEL_TYPE_BYTE, 0, offsetof(MONITEMPERCENTTXT, TreasureClassPercent) },
		{ "ComponentPercent", EXCEL_TYPE_BYTE, 0, offsetof(MONITEMPERCENTTXT, ComponentPercent) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};

	sgtMonItemPercentDataTblDesc.ptPercent = (PTMONITEMPERCENTTXT)DataTblsAllocAndTokenize(tDesc,
		(char *)pBuffer, nBufferSize, &sgtMonItemPercentDataTblDesc.nSize, sizeof(MONITEMPERCENTTXT));
	return sgtMonItemPercentDataTblDesc.ptPercent;
}

void __fastcall MonsterTblsFreeMonItemPercent(void)
{
	DataTblsFreeBlock(sgtMonItemPercentDataTblDesc.ptPercent);
}

MONITEMPERCENTDATATBLDESC * __fastcall MonsterTblsGetMonItemPercentDesc(void)
{
	return &sgtMonItemPercentDataTblDesc;
}

PTMONITEMPERCENTTXT __stdcall MonsterTblsGetMonItemPercent(int nIndex)
{
	if (nIndex < sgtMonItemPercentDataTblDesc.nSize && nIndex != -1)
	{
#line 753
		D2ASSERT(sgtMonItemPercentDataTblDesc.ptPercent);
#line 754
		D2ASSERT((sgtMonItemPercentDataTblDesc.ptPercent + nIndex));
		return sgtMonItemPercentDataTblDesc.ptPercent + nIndex;
	}
	return NULL;
}

void __fastcall MonsterTblsLoadSuperUniques(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Name", EXCEL_TYPE_STRING, 59, offsetof(SUPERUNIQUESTXT, Name) },
		{ "Class", EXCEL_TYPE_DWORD, 0, offsetof(SUPERUNIQUESTXT, Class) },
		{ "Mod1", EXCEL_TYPE_DWORD, 0, offsetof(SUPERUNIQUESTXT, Mod1) },
		{ "Mod2", EXCEL_TYPE_DWORD, 0, offsetof(SUPERUNIQUESTXT, Mod2) },
		{ "Mod3", EXCEL_TYPE_DWORD, 0, offsetof(SUPERUNIQUESTXT, Mod3) },
		{ "MinGrp", EXCEL_TYPE_DWORD, 0, offsetof(SUPERUNIQUESTXT, MinGrp) },
		{ "MaxGrp", EXCEL_TYPE_DWORD, 0, offsetof(SUPERUNIQUESTXT, MaxGrp) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTSUPERUNIQUESTXT ptSuperUniques;
	DWORD i;
	DWORD j;

	ptSuperUniques = (PTSUPERUNIQUESTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(SUPERUNIQUESTXT));
	for (i = 0; i < (DWORD)nRows; i++)
	{
		for (j = 0; j < 60 / sizeof(DWORD); j++)
			((DWORD *)ptSuperUniques[i].wszName)[j] = 0;
		(void)D2LangUnicodestrncpy(ptSuperUniques[i].wszName,
			D2LangStrGetStringByKey(ptSuperUniques[i].Name), 60);
	}
	gptSuperUniquesTxt = ptSuperUniques;
}

PTSUPERUNIQUESTXT __stdcall MonsterTblsGetSuperUnique(int nIndex)
{
	return &gptSuperUniquesTxt[nIndex];
}

BOOL __stdcall MonsterTblsTestSecondIsSel(PTUNIT hMonster)
{
#line 808
	D2ASSERT(UnitGetType (hMonster) == UNIT_MONSTER);
	return MonsterTblsGetStats(hMonster->dwClassId)->IsSel2 != 0;
}

BOOL __stdcall MonsterTblsTestNeverSel(PTUNIT hMonster)
{
#line 819
	D2ASSERT(UnitGetType (hMonster) == UNIT_MONSTER);
	return MonsterTblsGetStats(hMonster->dwClassId)->NeverSel != 0;
}
