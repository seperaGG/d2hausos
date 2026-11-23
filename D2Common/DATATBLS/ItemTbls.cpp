#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\ItemTbls.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

struct ITEMMODCODE
{
 	const char	*pszCode;
 	int			nModOffset;
};
typedef struct ITEMMODCODE ITEMMODCODE;

struct WEAPONGROUP
{
 	char		szName[16];
 	int			nWeaponGroup;
};
typedef struct WEAPONGROUP WEAPONGROUP;

struct BOOKSPELL
{
 	DWORD		dwScrollSpellCode;
 	DWORD		dwBookSpellCode;
 	int			nSpellOffset;
 	int			nSkillScrollOffset;
 	int			nSkillBookOffset;
};
typedef struct BOOKSPELL BOOKSPELL;

struct GEMCONVERSION
{
 	DWORD		dwCode;
 	int			nGemOffset;
};
typedef struct GEMCONVERSION GEMCONVERSION;

#define NUM_WEAPON_GROUPS		14
#define NUM_SPELLFUNCOFFSETS	23
#define NUM_GEMOFFSETS			35

static WEAPONGROUP sgtWeaponGroupTbl[] =
{
	{ "none", 0 }, { "1hsword", 1 }, { "2hsword", 2 }, { "1haxe", 3 }, { "2haxe", 4 },
	{ "bow", 5 }, { "crossbow", 6 }, { "javelin", 7 }, { "spear", 8 }, { "polearm", 9 },
	{ "1hmace", 10 }, { "2hmace", 11 }, { "dagger", 12 }, { "staff", 13 },
};
static int sgnWeaponGroupTblSize = arrsize(sgtWeaponGroupTbl);

static ITEMMODCODE sgtItemModCodes[] =
{
	{ "ar00", 0 }, { "ar01", 3 }, { "ar02", 5 }, { "ar03", 6 }, { "ar04", 92 }, { "ar05", 97 },
	{ "ar06", 93 }, { "ar07", 4 }, { "ar08", 1 }, { "ar09", 2 }, { "st00", 7 }, { "dx00", 8 }, { "vt00", 9 },
	{ "in00", 10 }, { "mm00", 11 }, { "mm01", 12 }, { "mm03", 65 }, { "mh00", 13 }, { "mh01", 14 },
	{ "mh03", 66 }, { "ht00", 15 }, { "ht01", 96 }, { "bl00", 16 }, { "bl01", 82 }, { "bl02", 83 },
	{ "bl03", 84 }, { "dc00", 17 }, { "dc01", 18 }, { "dc02", 95 }, { "dc03", 19 }, { "df00", 20 },
	{ "df01", 21 }, { "dl00", 22 }, { "dl01", 23 }, { "dp00", 24 }, { "dp01", 25 }, { "dp02", 26 },
	{ "dm00", 28 }, { "dm01", 27 }, { "dm02", 29 }, { "dm03", 30 }, { "md00", 98 }, { "md01", 99 },
	{ "md02", 100 }, { "md03", 101 }, { "rf00", 31 }, { "rf01", 32 }, { "rl00", 33 }, { "rl01", 34 },
	{ "rc00", 35 }, { "rc01", 36 }, { "rm00", 37 }, { "rm01", 38 }, { "rp00", 39 }, { "rp01", 40 },
	{ "ra00", 41 }, { "ra01", 42 }, { "af00", 43 }, { "af01", 44 }, { "al00", 45 }, { "al01", 46 },
	{ "am00", 47 }, { "am01", 48 }, { "ac00", 49 }, { "ac01", 50 }, { "du00", 51 }, { "du01", 52 },
	{ "lf00", 53 }, { "at00", 54 }, { "at01", 55 }, { "at02", 56 }, { "at03", 57 }, { "at04", 105 },
	{ "ib00", 58 }, { "ib01", 59 }, { "ko00", 60 }, { "hl08", 61 }, { "hl10", 94 }, { "hl11", 62 },
	{ "hl12", 63 }, { "hl13", 116 }, { "tm00", 64 }, { "sk00", 67 }, { "sk01", 68 }, { "sk02", 69 },
	{ "sk03", 70 }, { "sk04", 71 }, { "sk05", 72 }, { "sk07", 103 }, { "sk08", 104 }, { "lt00", 73 },
	{ "lt01", 74 }, { "rq00", 75 }, { "ve00", 76 }, { "ve01", 77 }, { "ve02", 78 }, { "gh00", 79 },
	{ "gh01", 80 }, { "gh02", 81 }, { "ca00", 85 }, { "ca01", 86 }, { "ca02", 87 }, { "pd00", 88 },
	{ "nd00", 89 }, { "hw00", 90 }, { "hw01", 91 }, { "sp00", 102 }, { "sp01", 106 }, { "sp02", 107 },
	{ "sp03", 108 }, { "sp04", 109 }, { "sp05", 110 }, { "sp06", 112 }, { "sp07", 114 }, { "sp10", 115 },
	{ "sp11", 111 }, { "sp12", 117 }, { "sp13", 118 }, { "sp14", 119 }, { "sp15", 120 }, { "ds00", 113 }
};
static int sgnItemModCodes = arrsize(sgtItemModCodes);

static BOOKSPELL sgtSpellFuncConversionTbl[] =
{
	{ 543388521, 543908457, 0, 217, 218 },
	{ 544762981, 0, 1, -1, -1 },
	{ 543322728, 0, 2, -1, -1 },
	{ 543388532, 543908468, 3, 219, 220 },
	{ 544436342, 0, 4, -1, -1 },
	{ 543977590, 0, 5, -1, -1 },
	{ 544436345, 0, 6, -1, -1 },
	{ 543977593, 0, 7, -1, -1 },
	{ 544437874, 0, 8, -1, -1 },
	{ 543979122, 0, 9, -1, -1 },
	{ 544435575, 0, 10, -1, -1 },
	{ 543976823, 0, 11, -1, -1 },
	{ 544763746, 0, 12, -1, -1 },
	{ 540110952, 0, 13, -1, -1 },
	{ 540176488, 0, 14, -1, -1 },
	{ 540242024, 0, 15, -1, -1 },
	{ 540307560, 0, 16, -1, -1 },
	{ 540373096, 0, 17, -1, -1 },
	{ 540110957, 0, 18, -1, -1 },
	{ 540176493, 0, 19, -1, -1 },
	{ 540242029, 0, 20, -1, -1 },
	{ 540307565, 0, 21, -1, -1 },
	{ 540373101, 0, 22, -1, -1 },
};
static int sgnSpellFuncConversionTblSize = arrsize(sgtSpellFuncConversionTbl);

static GEMCONVERSION sgtGemConversionTbl[] =
{
	{ 544629607, 0 }, { 544630375, 1 }, { 544633703, 2 }, { 544635495, 3 }, { 544632935, 4 },
	{ 543318887, 5 }, { 543319655, 6 }, { 543322983, 7 }, { 543321191, 8 }, { 543322215, 9 },
	{ 543646567, 10 }, { 543647335, 11 }, { 543650663, 12 }, { 543648871, 13 }, { 543649895, 14 },
	{ 544367463, 15 }, { 544368231, 16 }, { 544371559, 17 }, { 544369767, 18 }, { 544370791, 19 },
	{ 544695143, 20 }, { 544695911, 21 }, { 544699239, 22 }, { 544697447, 23 }, { 544698471, 24 },
	{ 544826215, 25 }, { 544826983, 26 }, { 544830311, 27 }, { 544828519, 28 }, { 544829543, 29 },
	{ 543386483, 30 }, { 543583091, 31 }, { 544566131, 32 }, { 543976307, 33 }, { 544893811, 34 },
};
static int sgnGemConversionTblSize = arrsize(sgtGemConversionTbl);

struct MAGICITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTMAGICAFFIXTXT	ptItemMods;
 	PTMAGICAFFIXTXT	ptSuffixes;
 	PTMAGICAFFIXTXT	ptPrefixes;
};
typedef struct MAGICITEMMODSDATATBLDESC MAGICITEMMODSDATATBLDESC;
struct RAREITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTRAREAFFIXTXT	ptItemMods;
 	PTRAREAFFIXTXT	ptSuffixes;
 	PTRAREAFFIXTXT	ptPrefixes;
};
typedef struct RAREITEMMODSDATATBLDESC RAREITEMMODSDATATBLDESC;
struct UNIQUEITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTUNIQUEITEMSTXT	ptItemMods;
};
typedef struct UNIQUEITEMMODSDATATBLDESC UNIQUEITEMMODSDATATBLDESC;
struct SETITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTSETITEMSTXT	ptItemMods;
};
typedef struct SETITEMMODSDATATBLDESC SETITEMMODSDATATBLDESC;
struct HIQUALITYITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTQUALITYITEMSTXT	ptItemMods;
};
typedef struct HIQUALITYITEMMODSDATATBLDESC HIQUALITYITEMMODSDATATBLDESC;
struct GEMITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTGEMSTXT	ptItemMods;
};
typedef struct GEMITEMMODSDATATBLDESC GEMITEMMODSDATATBLDESC;
struct BOOKITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTBOOKSTXT	ptItemMods;
};
typedef struct BOOKITEMMODSDATATBLDESC BOOKITEMMODSDATATBLDESC;
struct LOWQUALITYITEMMODSDATATBLDESC
{
 	int			nSize;
 	PTLOWQUALITYITEMSTXT	ptItemMods;
};
typedef struct LOWQUALITYITEMMODSDATATBLDESC LOWQUALITYITEMMODSDATATBLDESC;
struct ITEMRATIODATATBLDESC
{
 	int			nSize;
 	PTITEMRATIOTXT	ptItemMods;
};
typedef struct ITEMRATIODATATBLDESC ITEMRATIODATATBLDESC;

static void __fastcall ItemTblsResolveMagicMods(PTMAGICAFFIXTXT ptMods);

struct ITEMSTATSDATATBLDESC
{
 	int			nSize;
 	PTITEMSTXT	ptItemStats;
 	PTITEMSTXT	ptWeaponsStart;
 	PTITEMSTXT	ptArmorStart;
 	PTITEMSTXT	ptMiscStart;
};
typedef struct ITEMSTATSDATATBLDESC ITEMSTATSDATATBLDESC;

struct ITEMCODENODE
{
 	PTITEMSTXT	ptItemStats;
 	DWORD		dwCode;
 	struct ITEMCODENODE	*ptLower;
 	struct ITEMCODENODE	*ptHigher;
};
typedef struct ITEMCODENODE ITEMCODENODE, *PTITEMCODENODE;
static ITEMSTATSDATATBLDESC sgtItemStatsDataTblDesc = { 0 };
static GEMITEMMODSDATATBLDESC sgtGemItemModsDataTblDesc = { 0 };
static LOWQUALITYITEMMODSDATATBLDESC sgtLowQualityItemModsDataTblDesc = { 0 };
static BOOKITEMMODSDATATBLDESC sgtBookItemModsDataTblDesc = { 0 };
static RAREITEMMODSDATATBLDESC sgtRareItemModsDataTblDesc = { 0 };
int gnItemStatCost = 0;
PTITEMSTATCOSTTXT gptItemStatCostTxt = NULL;
static ITEMRATIODATATBLDESC sgtItemRatioDataTblDesc = { 0 };
static SETITEMMODSDATATBLDESC sgtSetItemModsDataTblDesc = { 0 };
static UNIQUEITEMMODSDATATBLDESC sgtUniqueItemModsDataTblDesc = { 0 };
int gnGambleItems = 0;
int *gpnGambleItems = NULL;
static MAGICITEMMODSDATATBLDESC sgtMagicItemModsDataTblDesc = { 0 };
static HIQUALITYITEMMODSDATATBLDESC sgtHiqualityItemModsDataTblDesc = { 0 };
PTITEMCODENODE gptItemCodeTree = NULL;

static void __fastcall ItemTblsInsertCodeNode(PTITEMCODENODE *pptLink, PTITEMCODENODE ptNode);

static __inline void ItemTblsSetSpellOffset(PTITEMSTXT ptStats)
{
	DWORD i;

#line 513
	D2ASSERT(ptStats);
#line 514
	D2ASSERT(sgnSpellFuncConversionTblSize == NUM_SPELLFUNCOFFSETS);
	ptStats->spelloffset = -1;
	for (i = 0; i < (DWORD)sgnSpellFuncConversionTblSize; i++)
	{
		if (ptStats->code == sgtSpellFuncConversionTbl[i].dwScrollSpellCode
			|| ptStats->code == sgtSpellFuncConversionTbl[i].dwBookSpellCode)
			ptStats->spelloffset = sgtSpellFuncConversionTbl[i].nSpellOffset;
	}
}

static __inline void ItemTblsSetWeaponGroup(PTITEMSTXT ptStats)
{
	int i;

#line 532
	D2ASSERT(ptStats);
#line 533
	D2ASSERT(sgnWeaponGroupTblSize == NUM_WEAPON_GROUPS);
	ptStats->bWeaponGroup = 0;
	for (i = 0; i < sgnWeaponGroupTblSize; i++)
	{
		if (!stricmp(ptStats->group, sgtWeaponGroupTbl[i].szName))
			ptStats->bWeaponGroup = (BYTE)sgtWeaponGroupTbl[i].nWeaponGroup;
	}
}

static __inline void ItemTblsSetGemOffset(PTITEMSTXT ptStats)
{
	DWORD i;

#line 549
	D2ASSERT(sgnGemConversionTblSize == NUM_GEMOFFSETS);
	ptStats->gemoffset = -1;
	for (i = 0; i < (DWORD)sgnGemConversionTblSize; i++)
	{
		if (ptStats->code == sgtGemConversionTbl[i].dwCode)
			ptStats->gemoffset = sgtGemConversionTbl[i].nGemOffset;
	}
}

static __inline void ItemTblsAddCodeNode(DWORD dwItem)
{
	PTITEMCODENODE ptNode;

#line 2246
	ptNode = (PTITEMCODENODE)D2ALLOCPOOL(NULL, sizeof(ITEMCODENODE));
	ptNode->ptItemStats = &sgtItemStatsDataTblDesc.ptItemStats[dwItem];
	ptNode->dwCode = ptNode->ptItemStats->code;
	ptNode->ptLower = NULL;
	ptNode->ptHigher = NULL;
	ItemTblsInsertCodeNode(&gptItemCodeTree, ptNode);
}

static __inline void ItemTblsSetName(PTITEMSTXT ptStats)
{
	char szCode[4];
	int k;

	for (k = 0; k < 16; k++)
		((DWORD *)ptStats->wszName)[k] = 0;
	szCode[3] = 0;
	szCode[0] = ((char *)&ptStats->code)[0] == ' ' ? 0 : ((char *)&ptStats->code)[0];
	szCode[1] = ((char *)&ptStats->code)[1] == ' ' ? 0 : ((char *)&ptStats->code)[1];
	szCode[2] = ((char *)&ptStats->code)[2] == ' ' ? 0 : ((char *)&ptStats->code)[2];
	(void)D2LangUnicodestrncpy(ptStats->wszName, D2LangStrGetStringByKey(szCode), 64);
}

int __fastcall ItemTblsLoadItems(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2,
	void *pBuffer3, int nBufferSize3)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "completed", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, completed) },
		{ "rarity", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, rarity) },
		{ "level", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, level) },
		{ "mindam", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, mindam) },
		{ "maxdam", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, maxdam) },
		{ "2handmindam", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, bTwoHandMinDam) },
		{ "2handmaxdam", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, bTwoHandMaxDam) },
		{ "rangeadder", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, rangeadder) },
		{ "strbonus", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, strbonus) },
		{ "dexbonus", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, dexbonus) },
		{ "minac", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, minac) },
		{ "maxac", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, maxac) },
		{ "speed", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, speed) },
		{ "reqstr", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, reqstr) },
		{ "reqdex", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, reqdex) },
		{ "absorbs", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, absorbs) },
		{ "invwidth", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, invwidth) },
		{ "invheight", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, invheight) },
		{ "block", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, block) },
		{ "durability", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, durability) },
		{ "nodurability", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, nodurability) },
		{ "missile", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, missile) },
		{ "replenish", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, replenish) },
		{ "special", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, special) },
		{ "component", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, component) },
		{ "bodyloc1", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, bodyloc1) },
		{ "bodyloc2", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, bodyloc2) },
		{ "cost", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, cost) },
		{ "code", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, code) },
		{ "alternategfx", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, alternategfx) },
		{ "ubercode", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, ubercode) },
		{ "spelloffset", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, spelloffset) },
		{ "wclass", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, wclass) },
		{ "2handedwclass", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, dwTwoHandedWClass) },
		{ "group", EXCEL_TYPE_OPTSTRING, 15, offsetof(ITEMSTXT, group) },
		{ "1or2handed", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, bOneOrTwoHanded) },
		{ "hit class", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, bHitClass) },
		{ "name", EXCEL_TYPE_STRING, 63, offsetof(ITEMSTXT, name) },
		{ "flippyfile", EXCEL_TYPE_STRING, 31, offsetof(ITEMSTXT, flippyfile) },
		{ "invfile", EXCEL_TYPE_STRING, 31, offsetof(ITEMSTXT, invfile) },
		{ "uniqueinvfile", EXCEL_TYPE_STRING, 31, offsetof(ITEMSTXT, uniqueinvfile) },
		{ "BetterGem", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, BetterGem) },
		{ "rArm", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, rArm) },
		{ "lArm", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, lArm) },
		{ "torso", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, torso) },
		{ "legs", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, legs) },
		{ "rspad", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, rspad) },
		{ "lspad", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, lspad) },
		{ "2handed", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, bTwoHanded) },
		{ "useable", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, useable) },
		{ "type", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, type) },
		{ "subtype", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, subtype) },
		{ "sound", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, sound) },
		{ "unique", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, unique) },
		{ "quest", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, quest) },
		{ "transparent", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, transparent) },
		{ "transtbl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, transtbl) },
		{ "throwable", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, throwable) },
		{ "lightradius", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, lightradius) },
		{ "quivered", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, quivered) },
		{ "belt", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, belt) },
		{ "autobelt", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, autobelt) },
		{ "stackable", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, stackable) },
		{ "minstack", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, minstack) },
		{ "maxstack", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, maxstack) },
		{ "spawnable", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, spawnable) },
		{ "missiletype", EXCEL_TYPE_WORD, 0, offsetof(ITEMSTXT, missiletype) },
		{ "spellicon", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, spellicon) },
		{ "durwarning", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, durwarning) },
		{ "qntwarning", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, qntwarning) },
		{ "hasinv", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, hasinv) },
		{ "gemsockets", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, gemsockets) },
		{ "gemoffset", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, gemoffset) },
		{ "SkipName", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, SkipName) },
		{ "bitfield1", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTXT, bitfield1) },
		{ "CharsiMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[2]) },
		{ "CharsiMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[2]) },
		{ "CharsiMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[2]) },
		{ "CharsiMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[2]) },
		{ "CharsiMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[2]) },
		{ "GheedMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[1]) },
		{ "GheedMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[1]) },
		{ "GheedMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[1]) },
		{ "GheedMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[1]) },
		{ "GheedMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[1]) },
		{ "AkaraMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[0]) },
		{ "AkaraMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[0]) },
		{ "AkaraMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[0]) },
		{ "AkaraMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[0]) },
		{ "AkaraMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[0]) },
		{ "FaraMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[3]) },
		{ "FaraMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[3]) },
		{ "FaraMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[3]) },
		{ "FaraMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[3]) },
		{ "FaraMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[3]) },
		{ "LysanderMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[4]) },
		{ "LysanderMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[4]) },
		{ "LysanderMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[4]) },
		{ "LysanderMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[4]) },
		{ "LysanderMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[4]) },
		{ "DrognanMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[5]) },
		{ "DrognanMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[5]) },
		{ "DrognanMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[5]) },
		{ "DrognanMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[5]) },
		{ "DrognanMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[5]) },
		{ "HraltiMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[6]) },
		{ "HraltiMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[6]) },
		{ "HraltiMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[6]) },
		{ "HraltiMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[6]) },
		{ "HraltiMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[6]) },
		{ "AlkorMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[7]) },
		{ "AlkorMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[7]) },
		{ "AlkorMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[7]) },
		{ "AlkorMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[7]) },
		{ "AlkorMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[7]) },
		{ "OrmusMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[8]) },
		{ "OrmusMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[8]) },
		{ "OrmusMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[8]) },
		{ "OrmusMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[8]) },
		{ "OrmusMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[8]) },
		{ "ElzixMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[9]) },
		{ "ElzixMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[9]) },
		{ "ElzixMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[9]) },
		{ "ElzixMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[9]) },
		{ "ElzixMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[9]) },
		{ "AshearaMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[10]) },
		{ "AshearaMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[10]) },
		{ "AshearaMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[10]) },
		{ "AshearaMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[10]) },
		{ "AshearaMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[10]) },
		{ "CainMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[11]) },
		{ "CainMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[11]) },
		{ "CainMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[11]) },
		{ "CainMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[11]) },
		{ "CainMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[11]) },
		{ "HalbuMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[12]) },
		{ "HalbuMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[12]) },
		{ "HalbuMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[12]) },
		{ "HalbuMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[12]) },
		{ "HalbuMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[12]) },
		{ "JamellaMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Min[13]) },
		{ "JamellaMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Max[13]) },
		{ "JamellaMagicMin", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMin[13]) },
		{ "JamellaMagicMax", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicMax[13]) },
		{ "JamellaMagicLvl", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, MagicLvl[13]) },
		{ "Transmogrify", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, Transmogrify) },
		{ "TMogType", EXCEL_TYPE_CODE, 0, offsetof(ITEMSTXT, TMogType) },
		{ "TMogMin", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, TMogMin) },
		{ "TMogMax", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, TMogMax) },
		{ "weapongroupenum", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, weapongroupenum) },
		{ "weaponspeedenum", EXCEL_TYPE_OPTBYTE, 0, offsetof(ITEMSTXT, weaponspeedenum) },
		{ "minmisdam", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, minmisdam) },
		{ "maxmisdam", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, maxmisdam) },
		{ "Transform", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, Transform) },
		{ "InvTrans", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, InvTrans) },
		{ "NightmareUpgrade", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, NightmareUpgrade) },
		{ "HellUpgrade", EXCEL_TYPE_BYTE, 0, offsetof(ITEMSTXT, HellUpgrade) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nWeapons;
	int nArmor;
	int nMisc;
	int nItems;
	PTITEMSTXT ptWeapons;
	PTITEMSTXT ptArmor;
	PTITEMSTXT ptMisc;
	PTITEMSTXT ptItems;
	PTITEMSTXT ptArmorStart;
	PTITEMSTXT ptStats;
	DWORD i;

	ptWeapons = (PTITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer1, nBufferSize1, &nWeapons,
		sizeof(ITEMSTXT));
	for (i = 0; i < (DWORD)nWeapons; i++)
	{
		ptStats = &ptWeapons[i];
		ItemTblsSetName(ptStats);
		ItemTblsSetWeaponGroup(ptStats);
	}
	ptArmor = (PTITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer2, nBufferSize2, &nArmor,
		sizeof(ITEMSTXT));
	for (i = 0; i < (DWORD)nArmor; i++)
	{
		ptStats = &ptArmor[i];
		ItemTblsSetName(ptStats);
	}
	ptMisc = (PTITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer3, nBufferSize3, &nMisc,
		sizeof(ITEMSTXT));
	for (i = 0; i < (DWORD)nMisc; i++)
	{
		ptStats = &ptMisc[i];
		ItemTblsSetName(ptStats);
		ItemTblsSetSpellOffset(ptStats);
		ItemTblsSetGemOffset(ptStats);
	}

	nItems = nWeapons + nArmor + nMisc;
#line 839
	ptItems = (PTITEMSTXT)D2ALLOCPOOL(NULL, nItems * sizeof(ITEMSTXT));
	memcpy(ptItems, ptWeapons, nWeapons * sizeof(ITEMSTXT));
	ptStats = &ptItems[nWeapons];
	ptArmorStart = ptStats;
	memcpy(ptStats, ptArmor, nArmor * sizeof(ITEMSTXT));
	ptStats += nArmor;
	memcpy(ptStats, ptMisc, nMisc * sizeof(ITEMSTXT));
	DataTblsFreeBlock(ptWeapons);
	DataTblsFreeBlock(ptArmor);
	DataTblsFreeBlock(ptMisc);
	sgtItemStatsDataTblDesc.ptMiscStart = ptStats;
	sgtItemStatsDataTblDesc.nSize = nItems;
	sgtItemStatsDataTblDesc.ptItemStats = ptItems;
	sgtItemStatsDataTblDesc.ptWeaponsStart = ptItems;
	sgtItemStatsDataTblDesc.ptArmorStart = ptArmorStart;
	for (i = 0; i < (DWORD)sgtItemStatsDataTblDesc.nSize; i++)
		ItemTblsAddCodeNode(i);
	return sgtItemStatsDataTblDesc.nSize;
}

static void __fastcall ItemTblsFreeCodeNode(PTITEMCODENODE ptNode);

void __fastcall ItemTblsFreeItems(void)
{
	PTITEMSTXT ptStats;

	if (gptItemCodeTree)
		ItemTblsFreeCodeNode(gptItemCodeTree);
	ptStats = sgtItemStatsDataTblDesc.ptItemStats;
	gptItemCodeTree = NULL;
	if (ptStats)
	{
#line 874
		D2FREEPOOL(NULL, ptStats);
	}
	sgtItemStatsDataTblDesc.ptItemStats = NULL;
}

ITEMSTATSDATATBLDESC * __fastcall ItemTblsGetItemStatsDesc(void)
{
	return &sgtItemStatsDataTblDesc;
}

PTITEMSTXT __stdcall ItemTblsGetItemStats(DWORD nIndex)
{
	PTITEMSTXT ptStats;

	if (nIndex >= (DWORD)sgtItemStatsDataTblDesc.nSize)
		return NULL;
#line 894
	D2ASSERT(sgtItemStatsDataTblDesc.ptItemStats);
	ptStats = sgtItemStatsDataTblDesc.ptItemStats + nIndex;
#line 895
	D2ASSERT((sgtItemStatsDataTblDesc.ptItemStats + nIndex));
	return ptStats;
}

PTITEMSTXT __stdcall ItemTblsFindItemByCode(DWORD dwCode, int *pnIndex)
{
	PTITEMCODENODE ptNode;
	PTITEMSTXT ptStats;

	ptNode = gptItemCodeTree;
	while (ptNode)
	{
		if (dwCode == ptNode->dwCode)
		{
			ptStats = ptNode->ptItemStats;
			if (!ptStats)
				return NULL;
			*pnIndex = ptStats - sgtItemStatsDataTblDesc.ptItemStats;
			return ptStats;
		}
		if (dwCode < ptNode->dwCode)
			ptNode = ptNode->ptLower;
		else if (dwCode > ptNode->dwCode)
			ptNode = ptNode->ptHigher;
	}
	return NULL;
}

int __stdcall ItemTblsFindItemIndexByCode(DWORD dwCode)
{
	PTITEMCODENODE ptNode;
	PTITEMSTXT ptStats;

	ptNode = gptItemCodeTree;
	while (ptNode)
	{
		if (dwCode == ptNode->dwCode)
		{
			ptStats = ptNode->ptItemStats;
			if (!ptStats)
				return -1;
			return ptStats - sgtItemStatsDataTblDesc.ptItemStats;
		}
		if (dwCode < ptNode->dwCode)
			ptNode = ptNode->ptLower;
		else if (dwCode > ptNode->dwCode)
			ptNode = ptNode->ptHigher;
	}
	return -1;
}

void __fastcall ItemTblsLoadMagicAffixes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "nummods", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, nummods) },
		{ "mod1code", EXCEL_TYPE_STRING, 4, offsetof(MAGICAFFIXTXT, mod[0].code) },
		{ "mod1offset", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[0].nModOffset) },
		{ "mod1min", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[0].min) },
		{ "mod1max", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[0].max) },
		{ "mod2code", EXCEL_TYPE_STRING, 4, offsetof(MAGICAFFIXTXT, mod[1].code) },
		{ "mod2offset", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[1].nModOffset) },
		{ "mod2min", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[1].min) },
		{ "mod2max", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[1].max) },
		{ "mod3code", EXCEL_TYPE_STRING, 4, offsetof(MAGICAFFIXTXT, mod[2].code) },
		{ "mod3offset", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[2].nModOffset) },
		{ "mod3min", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[2].min) },
		{ "mod3max", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, mod[2].max) },
		{ "transform", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, transform) },
		{ "transformcolor", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, transformcolor) },
		{ "Armor", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Armor) },
		{ "Weapon", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Weapon) },
		{ "Shield", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Shield) },
		{ "Scepter", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Scepter) },
		{ "Wand", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Wand) },
		{ "Staff", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Staff) },
		{ "Bow", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Bow) },
		{ "Boots", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Boots) },
		{ "Gloves", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Gloves) },
		{ "Belt", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Belt) },
		{ "Ring", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Ring) },
		{ "Amulet", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, Amulet) },
		{ "divide", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, divide) },
		{ "multiply", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, multiply) },
		{ "add", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, add) },
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(MAGICAFFIXTXT, name) },
		{ "spawnable", EXCEL_TYPE_BYTE, 0, offsetof(MAGICAFFIXTXT, spawnable) },
		{ "level", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, level) },
		{ "group", EXCEL_TYPE_DWORD, 0, offsetof(MAGICAFFIXTXT, group) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSuffixes;
	int nPrefixes;
	int nRows;
	PTMAGICAFFIXTXT ptSuffixes;
	PTMAGICAFFIXTXT ptPrefixes;
	PTMAGICAFFIXTXT ptItemMods;
	PTMAGICAFFIXTXT ptPrefixStart;
	void *pString;
	DWORD i;

	ptSuffixes = (PTMAGICAFFIXTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer1, nBufferSize1, &nSuffixes,
		sizeof(MAGICAFFIXTXT));
	pString = NULL;
	for (i = 0; i < (DWORD)nSuffixes; i++)
	{
		ptSuffixes[i].wNameString = D2LangStrGetStringIndexByKey(ptSuffixes[i].name, &pString);
		ItemTblsResolveMagicMods(&ptSuffixes[i]);
	}
	ptPrefixes = (PTMAGICAFFIXTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer2, nBufferSize2, &nPrefixes,
		sizeof(MAGICAFFIXTXT));
	for (i = 0; i < (DWORD)nPrefixes; i++)
	{
		ptPrefixes[i].wNameString = D2LangStrGetStringIndexByKey(ptPrefixes[i].name, &pString);
		ItemTblsResolveMagicMods(&ptPrefixes[i]);
	}
	nRows = nPrefixes + nSuffixes;
#line 1057
	ptItemMods = (PTMAGICAFFIXTXT)D2ALLOCPOOL(NULL, nRows * sizeof(MAGICAFFIXTXT));
	memcpy(ptItemMods, ptSuffixes, nSuffixes * sizeof(MAGICAFFIXTXT));
	ptPrefixStart = &ptItemMods[nSuffixes];
	memcpy(ptPrefixStart, ptPrefixes, nPrefixes * sizeof(MAGICAFFIXTXT));
	DataTblsFreeBlock(ptSuffixes);
	DataTblsFreeBlock(ptPrefixes);
	sgtMagicItemModsDataTblDesc.nSize = nRows;
	sgtMagicItemModsDataTblDesc.ptItemMods = ptItemMods;
	sgtMagicItemModsDataTblDesc.ptSuffixes = ptItemMods;
	sgtMagicItemModsDataTblDesc.ptPrefixes = ptPrefixStart;
}

static void __fastcall ItemTblsResolveMagicMods(PTMAGICAFFIXTXT ptMods)
{
	PTITEMMODTXT ptMod;
	BOOL fFound;
	DWORD j;
	DWORD k;

#line 937
	D2ASSERT(ptMods);
	for (j = 0; j < ptMods->nummods; j++)
	{
		ptMod = &ptMods->mod[j];
		ptMod->nModOffset = -1;
		fFound = FALSE;
		for (k = 0; k < (DWORD)sgnItemModCodes; k++)
		{
			if (!stricmp(ptMod->code, sgtItemModCodes[k].pszCode))
			{
				ptMod->nModOffset = sgtItemModCodes[k].nModOffset;
				fFound = TRUE;
				break;
			}
		}
		if (ptMod->code[0])
		{
#line 952
			D2ASSERT(fFound);
		}
	}
}

void __fastcall ItemTblsFreeMagicAffixes(void)
{
	if (sgtMagicItemModsDataTblDesc.ptItemMods)
	{
#line 1082
		D2FREEPOOL(NULL, sgtMagicItemModsDataTblDesc.ptItemMods);
	}
	sgtMagicItemModsDataTblDesc.ptItemMods = NULL;
}

MAGICITEMMODSDATATBLDESC * __fastcall ItemTblsGetMagicAffixDesc(void)
{
	return &sgtMagicItemModsDataTblDesc;
}

PTMAGICAFFIXTXT __stdcall ItemTblsGetMagicAffix(int nIndex)
{
	PTMAGICAFFIXTXT ptRow;

	if (nIndex < sgtMagicItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1102
		D2ASSERT(sgtMagicItemModsDataTblDesc.ptItemMods);
		ptRow = sgtMagicItemModsDataTblDesc.ptItemMods + nIndex;
#line 1103
		D2ASSERT((sgtMagicItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

void __fastcall ItemTblsLoadRareAffixes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Armor", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Armor) },
		{ "Helm", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Helm) },
		{ "Shield", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Shield) },
		{ "Sword", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Sword) },
		{ "Axe", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Axe) },
		{ "Mace", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Mace) },
		{ "Spear", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Spear) },
		{ "Scepter", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Scepter) },
		{ "Wand", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Wand) },
		{ "Staff", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Staff) },
		{ "Bow", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Bow) },
		{ "Boots", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Boots) },
		{ "Gloves", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Gloves) },
		{ "Belt", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Belt) },
		{ "Ring", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Ring) },
		{ "Amulet", EXCEL_TYPE_BYTE, 0, offsetof(RAREAFFIXTXT, Amulet) },
		{ "divide", EXCEL_TYPE_DWORD, 0, offsetof(RAREAFFIXTXT, divide) },
		{ "multiply", EXCEL_TYPE_DWORD, 0, offsetof(RAREAFFIXTXT, multiply) },
		{ "add", EXCEL_TYPE_DWORD, 0, offsetof(RAREAFFIXTXT, add) },
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(RAREAFFIXTXT, name) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSuffixes;
	int nPrefixes;
	int nRows;
	PTRAREAFFIXTXT ptSuffixes;
	PTRAREAFFIXTXT ptPrefixes;
	PTRAREAFFIXTXT ptItemMods;
	PTRAREAFFIXTXT ptPrefixStart;
	void *pString;
	DWORD i;

	ptSuffixes = (PTRAREAFFIXTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer1, nBufferSize1, &nSuffixes,
		sizeof(RAREAFFIXTXT));
	pString = NULL;
	for (i = 0; i < (DWORD)nSuffixes; i++)
		ptSuffixes[i].wNameString = D2LangStrGetStringIndexByKey(ptSuffixes[i].name, &pString);
	ptPrefixes = (PTRAREAFFIXTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer2, nBufferSize2, &nPrefixes,
		sizeof(RAREAFFIXTXT));
	for (i = 0; i < (DWORD)nPrefixes; i++)
		ptPrefixes[i].wNameString = D2LangStrGetStringIndexByKey(ptPrefixes[i].name, &pString);
	nRows = nSuffixes + nPrefixes;
#line 1184
	ptItemMods = (PTRAREAFFIXTXT)D2ALLOCPOOL(NULL, nRows * sizeof(RAREAFFIXTXT));
	memcpy(ptItemMods, ptSuffixes, nSuffixes * sizeof(RAREAFFIXTXT));
	ptPrefixStart = &ptItemMods[nSuffixes];
	memcpy(ptPrefixStart, ptPrefixes, nPrefixes * sizeof(RAREAFFIXTXT));
	DataTblsFreeBlock(ptSuffixes);
	DataTblsFreeBlock(ptPrefixes);
	sgtRareItemModsDataTblDesc.ptItemMods = ptItemMods;
	sgtRareItemModsDataTblDesc.ptSuffixes = ptItemMods;
	sgtRareItemModsDataTblDesc.ptPrefixes = ptPrefixStart;
	sgtRareItemModsDataTblDesc.nSize = nRows;
}

void __fastcall ItemTblsFreeRareAffixes(void)
{
	if (sgtRareItemModsDataTblDesc.ptItemMods)
	{
#line 1209
		D2FREEPOOL(NULL, sgtRareItemModsDataTblDesc.ptItemMods);
	}
	sgtRareItemModsDataTblDesc.ptItemMods = NULL;
}

RAREITEMMODSDATATBLDESC * __fastcall ItemTblsGetRareAffixDesc(void)
{
	return &sgtRareItemModsDataTblDesc;
}

PTRAREAFFIXTXT __stdcall ItemTblsGetRareAffix(int nIndex)
{
	PTRAREAFFIXTXT ptRow;

	if (nIndex < sgtRareItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1229
		D2ASSERT(sgtRareItemModsDataTblDesc.ptItemMods);
		ptRow = sgtRareItemModsDataTblDesc.ptItemMods + nIndex;
#line 1230
		D2ASSERT((sgtRareItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

int __fastcall ItemTblsLoadUniqueItems(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "code", EXCEL_TYPE_CODE, 0, offsetof(UNIQUEITEMSTXT, code) },
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(UNIQUEITEMSTXT, name) },
		{ "beta", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUEITEMSTXT, beta) },
		{ "level", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, level) },
		{ "transform", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUEITEMSTXT, transform) },
		{ "invtransform", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUEITEMSTXT, invtransform) },
		{ "transformcolor", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUEITEMSTXT, transformcolor) },
		{ "nummods", EXCEL_TYPE_BYTE, 0, offsetof(UNIQUEITEMSTXT, nummods) },
		{ "itemmod1code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[0].code) },
		{ "itemmod1min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[0].min) },
		{ "itemmod1max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[0].max) },
		{ "itemmod2code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[1].code) },
		{ "itemmod2min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[1].min) },
		{ "itemmod2max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[1].max) },
		{ "itemmod3code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[2].code) },
		{ "itemmod3min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[2].min) },
		{ "itemmod3max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[2].max) },
		{ "itemmod4code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[3].code) },
		{ "itemmod4min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[3].min) },
		{ "itemmod4max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[3].max) },
		{ "itemmod5code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[4].code) },
		{ "itemmod5min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[4].min) },
		{ "itemmod5max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[4].max) },
		{ "itemmod6code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[5].code) },
		{ "itemmod6min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[5].min) },
		{ "itemmod6max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[5].max) },
		{ "itemmod7code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[6].code) },
		{ "itemmod7min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[6].min) },
		{ "itemmod7max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[6].max) },
		{ "itemmod8code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[7].code) },
		{ "itemmod8min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[7].min) },
		{ "itemmod8max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[7].max) },
		{ "itemmod9code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[8].code) },
		{ "itemmod9min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[8].min) },
		{ "itemmod9max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[8].max) },
		{ "itemmod10code", EXCEL_TYPE_STRING, 4, offsetof(UNIQUEITEMSTXT, itemmod[9].code) },
		{ "itemmod10min", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[9].min) },
		{ "itemmod10max", EXCEL_TYPE_DWORD, 0, offsetof(UNIQUEITEMSTXT, itemmod[9].max) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTUNIQUEITEMSTXT ptUniqueItems;
	PTUNIQUEITEMSTXT ptMods;
	PTITEMMODTXT ptMod;
	BOOL fFound;
	void *pString;
	DWORD i;
	DWORD j;
	DWORD k;

	ptUniqueItems = (PTUNIQUEITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(UNIQUEITEMSTXT));
	pString = NULL;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		ptUniqueItems[i].wNameString = D2LangStrGetStringIndexByKey(ptUniqueItems[i].name,
			&pString);
		ptMods = &ptUniqueItems[i];
#line 1239
		D2ASSERT(ptMods);
		for (j = 0; j < ptMods->nummods; j++)
		{
			ptMod = &ptMods->itemmod[j];
			ptMod->nModOffset = -1;
			fFound = FALSE;
			for (k = 0; k < (DWORD)sgnItemModCodes; k++)
			{
				if (!stricmp(ptMod->code, sgtItemModCodes[k].pszCode))
				{
					ptMod->nModOffset = sgtItemModCodes[k].nModOffset;
					fFound = TRUE;
					break;
				}
			}
			if (ptMod->code[0])
			{
#line 1254
				D2ASSERT(fFound);
			}
		}
	}
	sgtUniqueItemModsDataTblDesc.ptItemMods = ptUniqueItems;
	sgtUniqueItemModsDataTblDesc.nSize = nRows;
	return nRows;
}

void __fastcall ItemTblsFreeUniqueItems(void)
{
	DataTblsFreeBlock(sgtUniqueItemModsDataTblDesc.ptItemMods);
}

UNIQUEITEMMODSDATATBLDESC * __fastcall ItemTblsGetUniqueItemDesc(void)
{
	return &sgtUniqueItemModsDataTblDesc;
}

PTUNIQUEITEMSTXT __stdcall ItemTblsGetUniqueItem(int nIndex)
{
	PTUNIQUEITEMSTXT ptRow;

	if (nIndex < sgtUniqueItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1359
		D2ASSERT(sgtUniqueItemModsDataTblDesc.ptItemMods);
		ptRow = sgtUniqueItemModsDataTblDesc.ptItemMods + nIndex;
#line 1360
		D2ASSERT((sgtUniqueItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

PTSETITEMSTXT __fastcall ItemTblsLoadSetItems(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 95, offsetof(SETITEMSTXT, name) },
		{ "prefix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, prefix) },
		{ "numitems", EXCEL_TYPE_BYTE, 0, offsetof(SETITEMSTXT, numitems) },
		{ "numproperties", EXCEL_TYPE_BYTE, 0, offsetof(SETITEMSTXT, numproperties) },
		{ "level", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, level) },
		{ "transform", EXCEL_TYPE_BYTE, 0, offsetof(SETITEMSTXT, transform) },
		{ "transformcolor", EXCEL_TYPE_BYTE, 0, offsetof(SETITEMSTXT, transformcolor) },
		{ "item1", EXCEL_TYPE_CODE, 0, offsetof(SETITEMSTXT, item1) },
		{ "item1suffix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, item1suffix) },
		{ "item2", EXCEL_TYPE_CODE, 0, offsetof(SETITEMSTXT, item2) },
		{ "item2suffix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, item2suffix) },
		{ "item3", EXCEL_TYPE_CODE, 0, offsetof(SETITEMSTXT, item3) },
		{ "item3suffix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, item3suffix) },
		{ "item4", EXCEL_TYPE_CODE, 0, offsetof(SETITEMSTXT, item4) },
		{ "item4suffix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, item4suffix) },
		{ "item5", EXCEL_TYPE_CODE, 0, offsetof(SETITEMSTXT, item5) },
		{ "item5suffix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, item5suffix) },
		{ "item6", EXCEL_TYPE_CODE, 0, offsetof(SETITEMSTXT, item6) },
		{ "item6suffix", EXCEL_TYPE_STRING, 31, offsetof(SETITEMSTXT, item6suffix) },
		{ "propertymod1code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[0].code) },
		{ "propertymod1min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[0].min) },
		{ "propertymod1max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[0].max) },
		{ "propertymod2code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[1].code) },
		{ "propertymod2min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[1].min) },
		{ "propertymod2max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[1].max) },
		{ "propertymod3code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[2].code) },
		{ "propertymod3min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[2].min) },
		{ "propertymod3max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[2].max) },
		{ "propertymod4code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[3].code) },
		{ "propertymod4min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[3].min) },
		{ "propertymod4max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[3].max) },
		{ "propertymod5code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[4].code) },
		{ "propertymod5min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[4].min) },
		{ "propertymod5max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[4].max) },
		{ "propertymod6code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[5].code) },
		{ "propertymod6min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[5].min) },
		{ "propertymod6max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[5].max) },
		{ "propertymod7code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[6].code) },
		{ "propertymod7min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[6].min) },
		{ "propertymod7max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[6].max) },
		{ "propertymod8code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[7].code) },
		{ "propertymod8min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[7].min) },
		{ "propertymod8max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[7].max) },
		{ "propertymod9code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[8].code) },
		{ "propertymod9min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[8].min) },
		{ "propertymod9max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[8].max) },
		{ "propertymod10code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[9].code) },
		{ "propertymod10min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[9].min) },
		{ "propertymod10max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[9].max) },
		{ "propertymod11code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[10].code) },
		{ "propertymod11min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[10].min) },
		{ "propertymod11max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[10].max) },
		{ "propertymod12code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[11].code) },
		{ "propertymod12min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[11].min) },
		{ "propertymod12max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[11].max) },
		{ "propertymod13code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[12].code) },
		{ "propertymod13min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[12].min) },
		{ "propertymod13max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[12].max) },
		{ "propertymod14code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[13].code) },
		{ "propertymod14min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[13].min) },
		{ "propertymod14max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[13].max) },
		{ "propertymod15code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[14].code) },
		{ "propertymod15min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[14].min) },
		{ "propertymod15max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[14].max) },
		{ "propertymod16code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[15].code) },
		{ "propertymod16min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[15].min) },
		{ "propertymod16max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[15].max) },
		{ "propertymod17code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[16].code) },
		{ "propertymod17min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[16].min) },
		{ "propertymod17max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[16].max) },
		{ "propertymod18code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[17].code) },
		{ "propertymod18min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[17].min) },
		{ "propertymod18max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[17].max) },
		{ "propertymod19code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[18].code) },
		{ "propertymod19min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[18].min) },
		{ "propertymod19max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[18].max) },
		{ "propertymod20code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[19].code) },
		{ "propertymod20min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[19].min) },
		{ "propertymod20max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[19].max) },
		{ "propertymod21code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[20].code) },
		{ "propertymod21min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[20].min) },
		{ "propertymod21max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[20].max) },
		{ "propertymod22code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[21].code) },
		{ "propertymod22min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[21].min) },
		{ "propertymod22max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[21].max) },
		{ "propertymod23code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[22].code) },
		{ "propertymod23min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[22].min) },
		{ "propertymod23max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[22].max) },
		{ "propertymod24code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[23].code) },
		{ "propertymod24min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[23].min) },
		{ "propertymod24max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[23].max) },
		{ "propertymod25code", EXCEL_TYPE_STRING, 4, offsetof(SETITEMSTXT, propertymod[24].code) },
		{ "propertymod25min", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[24].min) },
		{ "propertymod25max", EXCEL_TYPE_DWORD, 0, offsetof(SETITEMSTXT, propertymod[24].max) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTSETITEMSTXT ptSetItems;
	PTSETITEMSTXT ptMods;
	PTITEMMODTXT ptMod;
	BOOL fFound;
	void *pString;
	DWORD i;
	DWORD j;
	DWORD k;

	ptSetItems = (PTSETITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(SETITEMSTXT));
	pString = NULL;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		ptSetItems[i].wNameString = D2LangStrGetStringIndexByKey(ptSetItems[i].name, &pString);
		ptSetItems[i].wPrefixString = D2LangStrGetStringIndexByKey(ptSetItems[i].prefix, &pString);
		ptSetItems[i].wItem1SuffixString = D2LangStrGetStringIndexByKey(ptSetItems[i].item1suffix,
			&pString);
		ptSetItems[i].wItem2SuffixString = D2LangStrGetStringIndexByKey(ptSetItems[i].item2suffix,
			&pString);
		ptSetItems[i].wItem3SuffixString = D2LangStrGetStringIndexByKey(ptSetItems[i].item3suffix,
			&pString);
		ptSetItems[i].wItem4SuffixString = D2LangStrGetStringIndexByKey(ptSetItems[i].item4suffix,
			&pString);
		ptSetItems[i].wItem5SuffixString = D2LangStrGetStringIndexByKey(ptSetItems[i].item5suffix,
			&pString);
		ptSetItems[i].wItem6SuffixString = D2LangStrGetStringIndexByKey(ptSetItems[i].item6suffix,
			&pString);
		ptMods = &ptSetItems[i];
#line 1369
		D2ASSERT(ptMods);
		for (j = 0; j < arrsize(ptMods->propertymod); j++)
		{
			ptMod = &ptMods->propertymod[j];
			ptMod->nModOffset = -1;
			fFound = FALSE;
			for (k = 0; k < (DWORD)sgnItemModCodes; k++)
			{
				if (!stricmp(ptMod->code, sgtItemModCodes[k].pszCode))
				{
					ptMod->nModOffset = sgtItemModCodes[k].nModOffset;
					fFound = TRUE;
					break;
				}
			}
			if (ptMod->code[0])
			{
#line 1384
				D2ASSERT(fFound);
			}
		}
	}
	sgtSetItemModsDataTblDesc.nSize = nRows;
	sgtSetItemModsDataTblDesc.ptItemMods = ptSetItems;
	return ptSetItems;
}

void __fastcall ItemTblsFreeSetItems(void)
{
	DataTblsFreeBlock(sgtSetItemModsDataTblDesc.ptItemMods);
}

SETITEMMODSDATATBLDESC * __fastcall ItemTblsGetSetItemDesc(void)
{
	return &sgtSetItemModsDataTblDesc;
}

PTSETITEMSTXT __stdcall ItemTblsGetSetItem(int nIndex)
{
	PTSETITEMSTXT ptRow;

	if (nIndex < sgtSetItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1554
		D2ASSERT(sgtSetItemModsDataTblDesc.ptItemMods);
		ptRow = sgtSetItemModsDataTblDesc.ptItemMods + nIndex;
#line 1555
		D2ASSERT((sgtSetItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

void __fastcall ItemTblsLoadQualityItems(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "nummods", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, nummods) },
		{ "mod1code", EXCEL_TYPE_STRING, 4, offsetof(QUALITYITEMSTXT, mod[0].code) },
		{ "mod1offset", EXCEL_TYPE_DWORD, 0, offsetof(QUALITYITEMSTXT, mod[0].nModOffset) },
		{ "mod1min", EXCEL_TYPE_DWORD, 0, offsetof(QUALITYITEMSTXT, mod[0].min) },
		{ "mod1max", EXCEL_TYPE_DWORD, 0, offsetof(QUALITYITEMSTXT, mod[0].max) },
		{ "mod2code", EXCEL_TYPE_STRING, 4, offsetof(QUALITYITEMSTXT, mod[1].code) },
		{ "mod2offset", EXCEL_TYPE_DWORD, 0, offsetof(QUALITYITEMSTXT, mod[1].nModOffset) },
		{ "mod2min", EXCEL_TYPE_DWORD, 0, offsetof(QUALITYITEMSTXT, mod[1].min) },
		{ "mod2max", EXCEL_TYPE_DWORD, 0, offsetof(QUALITYITEMSTXT, mod[1].max) },
		{ "effect1", EXCEL_TYPE_STRING, 31, offsetof(QUALITYITEMSTXT, effect1) },
		{ "effect2", EXCEL_TYPE_STRING, 31, offsetof(QUALITYITEMSTXT, effect2) },
		{ "armor", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, armor) },
		{ "weapon", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, weapon) },
		{ "shield", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, shield) },
		{ "scepter", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, scepter) },
		{ "wand", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, wand) },
		{ "staff", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, staff) },
		{ "bow", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, bow) },
		{ "boots", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, boots) },
		{ "gloves", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, gloves) },
		{ "belt", EXCEL_TYPE_BYTE, 0, offsetof(QUALITYITEMSTXT, belt) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTQUALITYITEMSTXT ptQualityItems;
	PTQUALITYITEMSTXT ptMods;
	PTITEMMODTXT ptMod;
	BOOL fFound;
	char *pszEffect;
	void *pString;
	DWORD i;
	DWORD j;
	DWORD k;

	ptQualityItems = (PTQUALITYITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(QUALITYITEMSTXT));
	for (i = 0; i < (DWORD)nRows; i++)
	{
		ptMods = &ptQualityItems[i];
#line 1564
		D2ASSERT(ptMods);
		for (j = 0; j < ptMods->nummods; j++)
		{
			ptMod = &ptMods->mod[j];
			ptMod->nModOffset = -1;
			fFound = FALSE;
			for (k = 0; k < (DWORD)sgnItemModCodes; k++)
			{
				if (!stricmp(ptMod->code, sgtItemModCodes[k].pszCode))
				{
					ptMod->nModOffset = sgtItemModCodes[k].nModOffset;
					fFound = TRUE;
					break;
				}
			}
			if (ptMod->code[0])
			{
#line 1579
				D2ASSERT(fFound);
			}
		}
	}
	pString = NULL;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		ptMods = &ptQualityItems[i];
		pszEffect = ptMods->effect1;
		if (pszEffect)
			ptMods->wEffect1String = D2LangStrGetStringIndexByKey(pszEffect, &pString);
		pszEffect = ptMods->effect2;
		if (pszEffect)
			ptMods->wEffect2String = D2LangStrGetStringIndexByKey(pszEffect, &pString);
	}
	sgtHiqualityItemModsDataTblDesc.ptItemMods = ptQualityItems;
	sgtHiqualityItemModsDataTblDesc.nSize = nRows;

}

void __fastcall ItemTblsFreeQualityItems(void)
{
	DataTblsFreeBlock(sgtHiqualityItemModsDataTblDesc.ptItemMods);
}

HIQUALITYITEMMODSDATATBLDESC * __fastcall ItemTblsGetQualityItemDesc(void)
{
	return &sgtHiqualityItemModsDataTblDesc;
}

PTQUALITYITEMSTXT __stdcall ItemTblsGetQualityItem(int nIndex)
{
	PTQUALITYITEMSTXT ptRow;

	if (nIndex < sgtHiqualityItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1678
		D2ASSERT(sgtHiqualityItemModsDataTblDesc.ptItemMods);
		ptRow = sgtHiqualityItemModsDataTblDesc.ptItemMods + nIndex;
#line 1679
		D2ASSERT((sgtHiqualityItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

int __fastcall ItemTblsLoadGems(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(GEMSTXT, name) },
		{ "code", EXCEL_TYPE_CODE, 0, offsetof(GEMSTXT, code) },
		{ "transform", EXCEL_TYPE_BYTE, 0, offsetof(GEMSTXT, transform) },
		{ "nummods", EXCEL_TYPE_BYTE, 0, offsetof(GEMSTXT, nummods) },
		{ "weaponmod1code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, weaponmod[0].code) },
		{ "weaponmod1offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[0].nModOffset) },
		{ "weaponmod1min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[0].min) },
		{ "weaponmod1max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[0].max) },
		{ "weaponmod2code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, weaponmod[1].code) },
		{ "weaponmod2offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[1].nModOffset) },
		{ "weaponmod2min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[1].min) },
		{ "weaponmod2max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[1].max) },
		{ "weaponmod3code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, weaponmod[2].code) },
		{ "weaponmod3offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[2].nModOffset) },
		{ "weaponmod3min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[2].min) },
		{ "weaponmod3max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, weaponmod[2].max) },
		{ "helmmod1code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, helmmod[0].code) },
		{ "helmmod1offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[0].nModOffset) },
		{ "helmmod1min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[0].min) },
		{ "helmmod1max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[0].max) },
		{ "helmmod2code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, helmmod[1].code) },
		{ "helmmod2offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[1].nModOffset) },
		{ "helmmod2min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[1].min) },
		{ "helmmod2max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[1].max) },
		{ "helmmod3code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, helmmod[2].code) },
		{ "helmmod3offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[2].nModOffset) },
		{ "helmmod3min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[2].min) },
		{ "helmmod3max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, helmmod[2].max) },
		{ "shieldmod1code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, shieldmod[0].code) },
		{ "shieldmod1offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[0].nModOffset) },
		{ "shieldmod1min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[0].min) },
		{ "shieldmod1max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[0].max) },
		{ "shieldmod2code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, shieldmod[1].code) },
		{ "shieldmod2offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[1].nModOffset) },
		{ "shieldmod2min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[1].min) },
		{ "shieldmod2max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[1].max) },
		{ "shieldmod3code", EXCEL_TYPE_STRING, 4, offsetof(GEMSTXT, shieldmod[2].code) },
		{ "shieldmod3offset", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[2].nModOffset) },
		{ "shieldmod3min", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[2].min) },
		{ "shieldmod3max", EXCEL_TYPE_DWORD, 0, offsetof(GEMSTXT, shieldmod[2].max) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTGEMSTXT ptGems;
	PTGEMSTXT ptMods;
	PTITEMMODTXT ptModLists[3];
	PTITEMMODTXT ptMod;
	BOOL fFound;
	void *pString;
	DWORD i;
	DWORD j;
	DWORD k;
	DWORD m;

	ptGems = (PTGEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(GEMSTXT));
	pString = NULL;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		char szCode[4];

		szCode[3] = 0;
		szCode[0] = ((char *)&ptGems[i].code)[0] == ' ' ? 0 : ((char *)&ptGems[i].code)[0];
		szCode[1] = ((char *)&ptGems[i].code)[1] == ' ' ? 0 : ((char *)&ptGems[i].code)[1];
		szCode[2] = ((char *)&ptGems[i].code)[2] == ' ' ? 0 : ((char *)&ptGems[i].code)[2];
		ptGems[i].wCodeString = D2LangStrGetStringIndexByKey(szCode, &pString);
		ptMods = &ptGems[i];
		ptModLists[0] = ptMods->weaponmod;
		ptModLists[1] = ptMods->helmmod;
		ptModLists[2] = ptMods->shieldmod;
#line 1695
		D2ASSERT(ptMods);
		for (j = 0; j < 3; j++)
		{
			for (k = 0; k < ptMods->nummods; k++)
			{
				ptMod = &ptModLists[j][k];
				ptMod->nModOffset = -1;
				fFound = FALSE;
				for (m = 0; m < (DWORD)sgnItemModCodes; m++)
				{
					if (!stricmp(ptMod->code, sgtItemModCodes[m].pszCode))
					{
						ptMod->nModOffset = sgtItemModCodes[m].nModOffset;
						fFound = TRUE;
						break;
					}
				}
				if (ptMod->code[0])
				{
#line 1711
					D2ASSERT(fFound);
				}
			}
		}
	}
	sgtGemItemModsDataTblDesc.ptItemMods = ptGems;
	sgtGemItemModsDataTblDesc.nSize = nRows;
	return nRows;
}

void __fastcall ItemTblsFreeGems(void)
{
	DataTblsFreeBlock(sgtGemItemModsDataTblDesc.ptItemMods);
}

GEMITEMMODSDATATBLDESC * __fastcall ItemTblsGetGemDesc(void)
{
	return &sgtGemItemModsDataTblDesc;
}

PTGEMSTXT __stdcall ItemTblsGetGem(int nIndex)
{
	PTGEMSTXT ptRow;

	if (nIndex < sgtGemItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1821
		D2ASSERT(sgtGemItemModsDataTblDesc.ptItemMods);
		ptRow = sgtGemItemModsDataTblDesc.ptItemMods + nIndex;
#line 1822
		D2ASSERT((sgtGemItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

int __fastcall ItemTblsLoadBooks(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(BOOKSTXT, name) },
		{ "completed", EXCEL_TYPE_BYTE, 0, offsetof(BOOKSTXT, completed) },
		{ "scrollspellcode", EXCEL_TYPE_CODE, 0, offsetof(BOOKSTXT, scrollspellcode) },
		{ "bookspellcode", EXCEL_TYPE_CODE, 0, offsetof(BOOKSTXT, bookspellcode) },
		{ "spelloffset", EXCEL_TYPE_DWORD, 0, offsetof(BOOKSTXT, spelloffset) },
		{ "skillscrolloffset", EXCEL_TYPE_DWORD, 0, offsetof(BOOKSTXT, skillscrolloffset) },
		{ "skillbookoffset", EXCEL_TYPE_DWORD, 0, offsetof(BOOKSTXT, skillbookoffset) },
		{ "spellicon", EXCEL_TYPE_BYTE, 0, offsetof(BOOKSTXT, spellicon) },
		{ "basecost", EXCEL_TYPE_DWORD, 0, offsetof(BOOKSTXT, basecost) },
		{ "costpercharge", EXCEL_TYPE_DWORD, 0, offsetof(BOOKSTXT, costpercharge) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTBOOKSTXT ptBooks;
	PTBOOKSTXT ptStats;
	void *pString;
	DWORD i;
	DWORD j;

	ptBooks = (PTBOOKSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(BOOKSTXT));
	pString = NULL;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		ptBooks[i].wNameString = D2LangStrGetStringIndexByKey(ptBooks[i].name, &pString);
		ptStats = &ptBooks[i];
#line 1831
		D2ASSERT(ptStats);
		ptStats->spelloffset = -1;
		for (j = 0; j < (DWORD)sgnSpellFuncConversionTblSize; j++)
		{
			if (ptStats->scrollspellcode == sgtSpellFuncConversionTbl[j].dwScrollSpellCode)
			{
				ptStats->spelloffset = sgtSpellFuncConversionTbl[j].nSpellOffset;
				ptStats->skillscrolloffset = sgtSpellFuncConversionTbl[j].nSkillScrollOffset;
				ptStats->skillbookoffset = sgtSpellFuncConversionTbl[j].nSkillBookOffset;
			}
		}
	}
	sgtBookItemModsDataTblDesc.ptItemMods = ptBooks;
	sgtBookItemModsDataTblDesc.nSize = nRows;
	return nRows;
}

void __fastcall ItemTblsFreeBooks(void)
{
	DataTblsFreeBlock(sgtBookItemModsDataTblDesc.ptItemMods);
}

BOOKITEMMODSDATATBLDESC * __fastcall ItemTblsGetBookDesc(void)
{
	return &sgtBookItemModsDataTblDesc;
}

PTBOOKSTXT __stdcall ItemTblsGetBook(int nIndex)
{
	PTBOOKSTXT ptRow;

	if (nIndex < sgtBookItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1917
		D2ASSERT(sgtBookItemModsDataTblDesc.ptItemMods);
		ptRow = sgtBookItemModsDataTblDesc.ptItemMods + nIndex;
#line 1918
		D2ASSERT((sgtBookItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

void __fastcall ItemTblsLoadLowQualityItems(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Name", EXCEL_TYPE_STRING, 31, offsetof(LOWQUALITYITEMSTXT, Name) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTLOWQUALITYITEMSTXT ptLowQualityItems;
	DWORD i;
	DWORD j;

	ptLowQualityItems = (PTLOWQUALITYITEMSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize,
		&nRows, sizeof(LOWQUALITYITEMSTXT));
	for (i = 0; i < (DWORD)nRows; i++)
	{
		for (j = 0; j < 32 / sizeof(DWORD); j++)
			((DWORD *)ptLowQualityItems[i].wszName)[j] = 0;
		(void)D2LangUnicodestrncpy(ptLowQualityItems[i].wszName,
			D2LangStrGetStringByKey(ptLowQualityItems[i].Name), 31);
	}
	sgtLowQualityItemModsDataTblDesc.nSize = nRows;
	sgtLowQualityItemModsDataTblDesc.ptItemMods = ptLowQualityItems;
}

void __fastcall ItemTblsFreeLowQualityItems(void)
{
	DataTblsFreeBlock(sgtLowQualityItemModsDataTblDesc.ptItemMods);
}

LOWQUALITYITEMMODSDATATBLDESC * __fastcall ItemTblsGetLowQualityItemDesc(void)
{
	return &sgtLowQualityItemModsDataTblDesc;
}

PTLOWQUALITYITEMSTXT __stdcall ItemTblsGetLowQualityItem(int nIndex)
{
	PTLOWQUALITYITEMSTXT ptRow;

	if (nIndex < sgtLowQualityItemModsDataTblDesc.nSize && nIndex != -1)
	{
#line 1984
		D2ASSERT(sgtLowQualityItemModsDataTblDesc.ptItemMods);
		ptRow = sgtLowQualityItemModsDataTblDesc.ptItemMods + nIndex;
#line 1985
		D2ASSERT((sgtLowQualityItemModsDataTblDesc.ptItemMods + nIndex));
		return ptRow;
	}
	return NULL;
}

PTITEMRATIOTXT __fastcall ItemTblsLoadItemRatio(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Unique", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, Unique) },
		{ "UniqueDivisor", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, UniqueDivisor) },
		{ "Rare", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, Rare) },
		{ "RareDivisor", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, RareDivisor) },
		{ "Set", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, Set) },
		{ "SetDivisor", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, SetDivisor) },
		{ "Magic", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, Magic) },
		{ "MagicDivisor", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, MagicDivisor) },
		{ "HiQuality", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, HiQuality) },
		{ "HiQualityDivisor", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, HiQualityDivisor) },
		{ "Normal", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, Normal) },
		{ "NormalDivisor", EXCEL_TYPE_DWORD, 0, offsetof(ITEMRATIOTXT, NormalDivisor) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTITEMRATIOTXT ptItemRatio;

	ptItemRatio = (PTITEMRATIOTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(ITEMRATIOTXT));
	sgtItemRatioDataTblDesc.ptItemMods = ptItemRatio;
	sgtItemRatioDataTblDesc.nSize = nRows;
	return ptItemRatio;
}

void __fastcall ItemTblsFreeItemRatio(void)
{
	DataTblsFreeBlock(sgtItemRatioDataTblDesc.ptItemMods);
}

ITEMRATIODATATBLDESC * __fastcall ItemTblsGetItemRatioDesc(void)
{
	return &sgtItemRatioDataTblDesc;
}

PTITEMRATIOTXT __stdcall ItemTblsGetItemRatio(DWORD dwItemClassIndex, DWORD dwUnused)
{
	int nRatioIndex;
	PTITEMRATIOTXT ptRow;

	nRatioIndex = 0;
	if (D2CommonITEMSTestUbercodeUnsetNoCheck(dwItemClassIndex))
		nRatioIndex = 1;
	D2_OPAQUE(nRatioIndex);
	if (nRatioIndex < sgtItemRatioDataTblDesc.nSize && nRatioIndex >= 0)
	{
#line 2060
		D2ASSERT(sgtItemRatioDataTblDesc.ptItemMods);
		ptRow = sgtItemRatioDataTblDesc.ptItemMods + nRatioIndex;
#line 2061
		D2ASSERT((sgtItemRatioDataTblDesc.ptItemMods + nRatioIndex));
		return ptRow;
	}
	return NULL;
}

PTITEMSTATCOSTTXT __fastcall ItemTblsLoadItemStatCost(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "divide", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTATCOSTTXT, divide) },
		{ "multiply", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTATCOSTTXT, multiply) },
		{ "add", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTATCOSTTXT, add) },
		{ "ItemSpecific", EXCEL_TYPE_DWORD, 0, offsetof(ITEMSTATCOSTTXT, ItemSpecific) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSize;
	PTITEMSTATCOSTTXT ptItemStatCost;

	ptItemStatCost = (PTITEMSTATCOSTTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nSize,
		sizeof(ITEMSTATCOSTTXT));
	gnItemStatCost = nSize;
#line 2096
	D2ASSERT(nSize == NUM_STATS);
	gptItemStatCostTxt = ptItemStatCost;
	return ptItemStatCost;
}

void __fastcall ItemTblsFreeItemStatCost(void)
{
	DataTblsFreeBlock(gptItemStatCostTxt);
}

PTITEMSTATCOSTTXT __stdcall ItemTblsGetItemStatCost(int nStat)
{
	if ((DWORD)nStat < (DWORD)gnItemStatCost)
		return &gptItemStatCostTxt[nStat];
	return NULL;
}

void __fastcall ItemTblsLoadGamble(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "code", EXCEL_TYPE_CODE, 0, offsetof(GAMBLETXT, code) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTGAMBLETXT ptGamble;
	int nCount;
	int nId;
	DWORD i;

	ptGamble = (PTGAMBLETXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(GAMBLETXT));
	if (nRows)
	{
#line 2152
		gpnGambleItems = (int *)D2ALLOCPOOL(NULL, nRows * sizeof(int));
	}
	else
		gpnGambleItems = NULL;
	nCount = 0;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		if (ItemTblsFindItemByCode(ptGamble[i].code, &nId))
		{
			gpnGambleItems[nCount] = nId;
			nCount++;
		}
	}
	gnGambleItems = nCount;
	DataTblsFreeBlock(ptGamble);
}

void __fastcall ItemTblsFreeGamble(void)
{
	if (gpnGambleItems)
	{
#line 2182
		D2FREEPOOL(NULL, gpnGambleItems);
		gpnGambleItems = NULL;
		gnGambleItems = 0;
	}
}

int * __fastcall ItemTblsGetGambleItems(void)
{
	return &gnGambleItems;
}

static void __fastcall ItemTblsInsertCodeNode(PTITEMCODENODE *pptLink, PTITEMCODENODE ptNode)
{
	if (!*pptLink)
	{
		*pptLink = ptNode;
		return;
	}
	if (ptNode->dwCode < (*pptLink)->dwCode)
		ItemTblsInsertCodeNode(&(*pptLink)->ptLower, ptNode);
	else
		ItemTblsInsertCodeNode(&(*pptLink)->ptHigher, ptNode);
}

static void __fastcall ItemTblsFreeCodeNode(PTITEMCODENODE ptNode)
{
	if (ptNode->ptLower)
		ItemTblsFreeCodeNode(ptNode->ptLower);
	if (ptNode->ptHigher)
		ItemTblsFreeCodeNode(ptNode->ptHigher);
#line 2271
	D2FREEPOOL(NULL, ptNode);
}
