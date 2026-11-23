#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\LvlTbls.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

extern LPCRITICAL_SECTION gptSubsFilesCS;

struct AUTOMAPRANGE
{
 	int			nStart;
 	int			nEnd;
};
typedef struct AUTOMAPRANGE AUTOMAPRANGE;
#define NUM_LEVEL_TYPES		29
#define NUM_BASE_TILES		19
typedef AUTOMAPRANGE AUTOMAPRANGES[NUM_LEVEL_TYPES];
static AUTOMAPRANGES sgtAutomapRanges = { 0 };
PTLEVELDATAREC gptLevelDataTxt = NULL;
PTLEVELSREC gptLevelDefsTxt = NULL;
static PTLVLTYPESTXT sgptDrlgLevelTypeDefs = NULL;
int *gpnPortalLevels = NULL;
static int sgnPortalLevels = 0;
int gnLvlTypes = 0;
PTLVLPRESTTXT gptLvlPrestTxt = NULL;
static PTLVLWARPTXT sgptWarpDefs = NULL;
static PTLVLMAZETXT sgptMazeDefs = NULL;
static int sgnMazeDefs = 0;
static PTLVLSUBTXT sgptTileSubs = NULL;
int gnLvlSub = 0;
static int *sgpnTileSubsTypeStart = NULL;
BYTE *gpbLvlTypeTiles = NULL;
PTPRESETDS1 *gpptPresetFiles = NULL;
static PTPRESETDS1 *sgpptSubsFiles = NULL;
static PTAUTOMAPSUB sgptAutomapSubs = NULL;
static int sgnAutomapSubs = 0;
static D2SEED sgtAutomapSeed;
static D2SEED *sgptAutomapSeed = &sgtAutomapSeed;

static const char sgcszLevelTypeNames[NUM_LEVEL_TYPES][16] =
{
	"None", "1 Town", "1 Wilderness", "1 Cave", "1 Crypt", "1 Monestary", "1 Courtyard", "1 Barracks",
	"1 Jail", "1 Cathedral", "1 Catacombs", "1 Tristram", "2 Town", "2 Sewer", "2 Harem", "2 Basement",
	"2 Desert", "2 Tomb", "2 Lair", "2 Arcane", "3 Town", "3 Jungle", "3 Kurast", "3 Spider", "3 Dungeon",
	"3 Sewer", "4 Town", "4 Mesa", "4 Lava",
};
static const char sgcszBaseTileNames[NUM_BASE_TILES][8] =
{
	"fl", "wl", "wr", "wtlr", "wtll", "wtr", "wbl", "wbr", "wld", "wrd", "wle", "wre", "co", "sh", "tr",
};

#define LEVEL_TYPE_NONE		0

void __fastcall LvlTblsLoadLevels(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Id", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Id) },
		{ "Pal", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Pal) },
		{ "Act", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Act) },
		{ "Rain", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Rain) },
		{ "Mud", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Mud) },
		{ "NoPer", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, NoPer) },
		{ "IsInside", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, IsInside) },
		{ "WarpDist", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, WarpDist) },
		{ "MonLvl1", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonLvl1) },
		{ "MonLvl2", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonLvl2) },
		{ "MonLvl3", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonLvl3) },
		{ "MonDen", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, MonDen) },
		{ "MonUMin", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonUMin) },
		{ "MonUMax", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonUMax) },
		{ "MonWndr", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonWndr) },
		{ "MonSpcWalk", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, MonSpcWalk) },
		{ "ObjGrp0", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[0]) },
		{ "ObjGrp1", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[1]) },
		{ "ObjGrp2", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[2]) },
		{ "ObjGrp3", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[3]) },
		{ "ObjGrp4", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[4]) },
		{ "ObjGrp5", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[5]) },
		{ "ObjGrp6", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[6]) },
		{ "ObjGrp7", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjGrp[7]) },
		{ "ObjPrb0", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[0]) },
		{ "ObjPrb1", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[1]) },
		{ "ObjPrb2", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[2]) },
		{ "ObjPrb3", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[3]) },
		{ "ObjPrb4", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[4]) },
		{ "ObjPrb5", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[5]) },
		{ "ObjPrb6", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[6]) },
		{ "ObjPrb7", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, ObjPrb[7]) },
		{ "Quest", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Quest) },
		{ "Mtot", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Mtot) },
		{ "M1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[0]) },
		{ "M2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[1]) },
		{ "M3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[2]) },
		{ "M4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[3]) },
		{ "M5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[4]) },
		{ "M6", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[5]) },
		{ "M7", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[6]) },
		{ "M8", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[7]) },
		{ "M9", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[8]) },
		{ "M10", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[9]) },
		{ "M11", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[10]) },
		{ "M12", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[11]) },
		{ "M13", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[12]) },
		{ "M14", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[13]) },
		{ "M15", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[14]) },
		{ "M16", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[15]) },
		{ "M17", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[16]) },
		{ "M18", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[17]) },
		{ "M19", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[18]) },
		{ "M20", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[19]) },
		{ "M21", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[20]) },
		{ "M22", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[21]) },
		{ "M23", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[22]) },
		{ "M24", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[23]) },
		{ "M25", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, M[24]) },
		{ "S1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[0]) },
		{ "S2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[1]) },
		{ "S3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[2]) },
		{ "S4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[3]) },
		{ "S5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[4]) },
		{ "S6", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[5]) },
		{ "S7", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[6]) },
		{ "S8", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[7]) },
		{ "S9", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[8]) },
		{ "S10", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[9]) },
		{ "S11", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[10]) },
		{ "S12", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[11]) },
		{ "S13", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[12]) },
		{ "S14", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[13]) },
		{ "S15", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[14]) },
		{ "S16", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[15]) },
		{ "S17", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[16]) },
		{ "S18", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[17]) },
		{ "S19", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[18]) },
		{ "S20", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[19]) },
		{ "S21", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[20]) },
		{ "S22", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[21]) },
		{ "S23", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[22]) },
		{ "S24", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[23]) },
		{ "S25", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, S[24]) },
		{ "Utot", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Utot) },
		{ "U1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[0]) },
		{ "U2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[1]) },
		{ "U3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[2]) },
		{ "U4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[3]) },
		{ "U5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[4]) },
		{ "U6", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[5]) },
		{ "U7", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[6]) },
		{ "U8", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[7]) },
		{ "U9", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[8]) },
		{ "U10", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[9]) },
		{ "U11", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[10]) },
		{ "U12", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[11]) },
		{ "U13", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[12]) },
		{ "U14", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[13]) },
		{ "U15", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[14]) },
		{ "U16", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[15]) },
		{ "U17", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[16]) },
		{ "U18", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[17]) },
		{ "U19", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[18]) },
		{ "U20", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[19]) },
		{ "U21", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[20]) },
		{ "U22", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[21]) },
		{ "U23", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[22]) },
		{ "U24", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[23]) },
		{ "U25", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, U[24]) },
		{ "C1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, C[0]) },
		{ "C2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, C[1]) },
		{ "C3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, C[2]) },
		{ "C4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, C[3]) },
		{ "C5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, C[4]) },
		{ "CA1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CA[0]) },
		{ "CA2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CA[1]) },
		{ "CA3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CA[2]) },
		{ "CA4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CA[3]) },
		{ "CA5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CA[4]) },
		{ "CD1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CD[0]) },
		{ "CD2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CD[1]) },
		{ "CD3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CD[2]) },
		{ "CD4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CD[3]) },
		{ "CD5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, CD[4]) },
		{ "Themes", EXCEL_TYPE_DWORD, 0, offsetof(LEVELDATAREC, Themes) },
		{ "SoundEnv", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, SoundEnv) },
		{ "Waypoint", EXCEL_TYPE_BYTE, 0, offsetof(LEVELDATAREC, Waypoint) },
		{ "LevelName", EXCEL_TYPE_STRING, 39, offsetof(LEVELDATAREC, LevelName) },
		{ "LevelWarp", EXCEL_TYPE_STRING, 39, offsetof(LEVELDATAREC, LevelWarp) },
		{ "EntryFile", EXCEL_TYPE_STRING, 39, offsetof(LEVELDATAREC, EntryFile) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSize;
	PTLEVELDATAREC ptLevels;
	DWORD i;
	int j;

	ptLevels = (PTLEVELDATAREC)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nSize,
		sizeof(LEVELDATAREC));
#line 185
	D2ASSERT(NUM_LEVEL_IDS == nSize);
	for (i = 0; i < (DWORD)nSize; i++)
	{
		for (j = 0; j < 20; j++)
			((DWORD *)ptLevels[i].wszLevelName)[j] = 0;
		for (j = 0; j < 20; j++)
			((DWORD *)ptLevels[i].wszLevelWarp)[j] = 0;
		(void)D2LangUnicodestrncpy(ptLevels[i].wszLevelName, D2LangStrGetStringByKey(ptLevels[i].LevelName),
			40);
		ptLevels[i].wszLevelName[39] = 0;
		(void)D2LangUnicodestrncpy(ptLevels[i].wszLevelWarp, D2LangStrGetStringByKey(ptLevels[i].LevelWarp),
			40);
		ptLevels[i].wszLevelWarp[39] = 0;
	}
	gptLevelDataTxt = ptLevels;
}

PTLEVELDATAREC __stdcall LvlTblsGetLevelData(int eLevelID)
{
#line 226
	D2ASSERT(eLevelID < NUM_LEVEL_IDS);
#line 227
	D2ASSERT(eLevelID > 0);
	return &gptLevelDataTxt[eLevelID];
}

BYTE __stdcall LvlTblsGetRain(int eLevelID)
{
	return gptLevelDataTxt[eLevelID].Rain;
}

BYTE __stdcall LvlTblsGetNoPer(int eLevelID)
{
	return gptLevelDataTxt[eLevelID].NoPer;
}

BYTE __stdcall LvlTblsGetMud(int eLevelID)
{
	return gptLevelDataTxt[eLevelID].Mud;
}

void __fastcall LvlTblsFreeLevels(void)
{
	DataTblsFreeBlock(gptLevelDataTxt);
}

int * __fastcall LvlTblsGetPortalLevels(int *pnCount)
{
	*pnCount = sgnPortalLevels;
	return gpnPortalLevels;
}

void __fastcall LvlTblsLoadLevelDefs(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Layer", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Layer) },
		{ "SizeX", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SizeX) },
		{ "SizeY", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SizeY) },
		{ "OffsetX", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, OffsetX) },
		{ "OffsetY", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, OffsetY) },
		{ "Depend", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Depend) },
		{ "DrlgType", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, DrlgType) },
		{ "LevelType", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, LevelType) },
		{ "SubType", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SubType) },
		{ "SubTheme", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SubTheme) },
		{ "SubWaypoint", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SubWaypoint) },
		{ "SubShrine", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SubShrine) },
		{ "Vis0", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[0]) },
		{ "Vis1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[1]) },
		{ "Vis2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[2]) },
		{ "Vis3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[3]) },
		{ "Vis4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[4]) },
		{ "Vis5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[5]) },
		{ "Vis6", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[6]) },
		{ "Vis7", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Vis[7]) },
		{ "Warp0", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[0]) },
		{ "Warp1", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[1]) },
		{ "Warp2", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[2]) },
		{ "Warp3", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[3]) },
		{ "Warp4", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[4]) },
		{ "Warp5", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[5]) },
		{ "Warp6", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[6]) },
		{ "Warp7", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Warp[7]) },
		{ "Intensity", EXCEL_TYPE_BYTE, 0, offsetof(LEVELSREC, Intensity) },
		{ "Red", EXCEL_TYPE_BYTE, 0, offsetof(LEVELSREC, Red) },
		{ "Green", EXCEL_TYPE_BYTE, 0, offsetof(LEVELSREC, Green) },
		{ "Blue", EXCEL_TYPE_BYTE, 0, offsetof(LEVELSREC, Blue) },
		{ "Portal", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Portal) },
		{ "Position", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, Position) },
		{ "SaveMonsters", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, SaveMonsters) },
		{ "LOSDraw", EXCEL_TYPE_DWORD, 0, offsetof(LEVELSREC, LOSDraw) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTLEVELSREC ptLevels;
	int nPortalIndex;
	int i;

	ptLevels = (PTLEVELSREC)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(LEVELSREC));
	gptLevelDefsTxt = ptLevels;
	sgnPortalLevels = 0;
	for (i = 0; i < NUM_LEVEL_IDS; i++)
	{
		if (ptLevels[i].Portal)
			sgnPortalLevels++;
	}
	if (sgnPortalLevels)
	{
#line 277
		gpnPortalLevels = (int *)D2ALLOCPOOL(NULL, sgnPortalLevels * sizeof(int));
		memset(gpnPortalLevels, 0, sgnPortalLevels * sizeof(int));
	}
	nPortalIndex = 0;
	for (i = 0; i < NUM_LEVEL_IDS; ptLevels++, i++)
	{
		if (ptLevels->Portal)
		{
#line 287
			D2ASSERT(nPortalIndex < sgnPortalLevels);
			gpnPortalLevels[nPortalIndex++] = i;
		}
	}
}

void __fastcall LvlTblsFreeLevelDefs(void)
{
	DataTblsFreeBlock(gptLevelDefsTxt);
	if (gpnPortalLevels)
	{
#line 381
		D2FREEPOOL(NULL, gpnPortalLevels);
	}
}

PTLEVELSREC __fastcall LvlTblsGetLevelDef(int eLevelID)
{
	return &gptLevelDefsTxt[eLevelID];
}

void __fastcall LvlTblsLoadLvlTypes(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "File 1", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[0]) },
		{ "File 2", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[1]) },
		{ "File 3", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[2]) },
		{ "File 4", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[3]) },
		{ "File 5", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[4]) },
		{ "File 6", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[5]) },
		{ "File 7", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[6]) },
		{ "File 8", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[7]) },
		{ "File 9", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[8]) },
		{ "File 10", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[9]) },
		{ "File 11", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[10]) },
		{ "File 12", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[11]) },
		{ "File 13", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[12]) },
		{ "File 14", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[13]) },
		{ "File 15", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[14]) },
		{ "File 16", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[15]) },
		{ "File 17", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[16]) },
		{ "File 18", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[17]) },
		{ "File 19", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[18]) },
		{ "File 20", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[19]) },
		{ "File 21", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[20]) },
		{ "File 22", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[21]) },
		{ "File 23", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[22]) },
		{ "File 24", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[23]) },
		{ "File 25", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[24]) },
		{ "File 26", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[25]) },
		{ "File 27", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[26]) },
		{ "File 28", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[27]) },
		{ "File 29", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[28]) },
		{ "File 30", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[29]) },
		{ "File 31", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[30]) },
		{ "File 32", EXCEL_TYPE_STRING, 59, offsetof(LVLTYPESTXT, File[31]) },
		{ "Act", EXCEL_TYPE_BYTE, 0, offsetof(LVLTYPESTXT, Act) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	char szPath[60];
	int nRows;
	PTLVLTYPESTXT ptLvlTypes;
	char *pszFile;
	int nLength;
	int k;
	DWORD i;
	DWORD j;

	ptLvlTypes = (PTLVLTYPESTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(LVLTYPESTXT));
	sgptDrlgLevelTypeDefs = ptLvlTypes;
	for (i = 0; i < (DWORD)nRows; ptLvlTypes++, i++)
	{
		for (j = 0; j < 32; j++)
		{
			pszFile = ptLvlTypes->File[j];
			nLength = strlen(pszFile);
			if (nLength != 0)
			{
				for (k = 0; k < nLength; k++)
				{
					if (pszFile[k] == '/')
						pszFile[k] = '\\';
				}
			}
			if (nLength > 1)
			{
				wsprintfA(szPath, "%s\\%s", "DATA\\GLOBAL\\TILES", pszFile);
				strcpy(pszFile, szPath);
			}
		}
	}
	gnLvlTypes = nRows;
}

void __fastcall LvlTblsFreeLvlTypes(void)
{
	DataTblsFreeBlock(sgptDrlgLevelTypeDefs);
}

PTLVLTYPESTXT __stdcall LvlTblsGetLevelTypeDef(int nType)
{
#line 503
	D2ASSERT(sgptDrlgLevelTypeDefs);
	return &sgptDrlgLevelTypeDefs[nType];
}

BOOL __stdcall LvlTblsTestLevelTypeAct(int nType, BYTE bAct)
{
	PTLVLTYPESTXT ptType;

	ptType = LvlTblsGetLevelTypeDef(nType);
	if (ptType && ptType->Act == bAct)
		return TRUE;
	return FALSE;
}

void __stdcall LvlTblsGetLevelTypeFile(int nType, int nFile, char *pszFile)
{
	PTLVLTYPESTXT ptType;

	ptType = LvlTblsGetLevelTypeDef(nType);
	pszFile[0] = '\0';
	if (ptType && nFile < (int)arrsize(ptType->File))
		strcpy(pszFile, ptType->File[nFile]);
}

void __fastcall LvlTblsLoadLvlPrest(void *pBuffer, int nBufferSize, BOOL fLoadFiles)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Def", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Def) },
		{ "LevelId", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, LevelId) },
		{ "Populate", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Populate) },
		{ "Logicals", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Logicals) },
		{ "Outdoors", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Outdoors) },
		{ "Animate", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Animate) },
		{ "KillEdge", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, KillEdge) },
		{ "FillBlanks", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, FillBlanks) },
		{ "SizeX", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, SizeX) },
		{ "SizeY", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, SizeY) },
		{ "AutoMap", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, AutoMap) },
		{ "Scan", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Scan) },
		{ "Pops", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Pops) },
		{ "PopPad", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, PopPad) },
		{ "Files", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Files) },
		{ "File1", EXCEL_TYPE_STRING, 59, offsetof(LVLPRESTTXT, File[0]) },
		{ "File2", EXCEL_TYPE_STRING, 59, offsetof(LVLPRESTTXT, File[1]) },
		{ "File3", EXCEL_TYPE_STRING, 59, offsetof(LVLPRESTTXT, File[2]) },
		{ "File4", EXCEL_TYPE_STRING, 59, offsetof(LVLPRESTTXT, File[3]) },
		{ "File5", EXCEL_TYPE_STRING, 59, offsetof(LVLPRESTTXT, File[4]) },
		{ "File6", EXCEL_TYPE_STRING, 59, offsetof(LVLPRESTTXT, File[5]) },
		{ "Dt1Mask", EXCEL_TYPE_DWORD, 0, offsetof(LVLPRESTTXT, Dt1Mask) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	char szPath[60];
	int nSize;
	PTLVLPRESTTXT ptLvlPrest, ptRow;
	char *pszFile;
	int nLength;
	int k;
	DWORD i;
	DWORD j;

	nSize = 0;
	ptLvlPrest = (PTLVLPRESTTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nSize,
		sizeof(LVLPRESTTXT));
#line 583
	D2ASSERT(nSize == NUM_PRESET_DEFS);
	gptLvlPrestTxt = ptLvlPrest;
	if (fLoadFiles)
	{
#line 598
		gpptPresetFiles = (PTPRESETDS1 *)D2ALLOC2(NUM_PRESET_DEFS * 6 * sizeof(PTPRESETDS1));
		memset(gpptPresetFiles, 0, NUM_PRESET_DEFS * 6 * sizeof(PTPRESETDS1));
	}
	for (i = 0, ptRow = ptLvlPrest; i < (DWORD)nSize; i++, ptRow++)
	{
		for (j = 0; j < 6; j++)
		{
			pszFile = ptRow->File[j];
			nLength = strlen(pszFile);
			if (nLength != 0)
				for (k = 0; k < nLength; k++)
				{
					if (pszFile[k] == '/')
						pszFile[k] = '\\';
				}
			if (nLength > 1)
			{
				wsprintfA(szPath, "%s\\%s", "DATA\\GLOBAL\\TILES", pszFile);
				strcpy(pszFile, szPath);
			}
			if (fLoadFiles && strlen(pszFile) > 1)
				PresetLoadFile(&gpptPresetFiles[i * 6 + j], NULL, pszFile);
		}
	}
}

void __fastcall LvlTblsFreeLvlPrest(void)
{
	int i;

	if (gpptPresetFiles)
	{
		for (i = 0; i < NUM_PRESET_DEFS * 6; i++)
		{
			if (gpptPresetFiles[i])
				PresetFreeFile(&gpptPresetFiles[i]);
		}
#line 631
		D2FREE2(gpptPresetFiles);
		gpptPresetFiles = NULL;
	}
	DataTblsFreeBlock(gptLvlPrestTxt);
}

PTLVLPRESTTXT __stdcall LvlTblsGetPrest(int nIndex)
{
	return &gptLvlPrestTxt[nIndex];
}

PTLVLPRESTTXT __fastcall LvlTblsGetPrestByLevel(int nLevelId)
{
	int i;

	for (i = 0; i < NUM_PRESET_DEFS; i++)
	{
		if (gptLvlPrestTxt[i].LevelId == nLevelId)
			return &gptLvlPrestTxt[i];
	}
	return NULL;
}

PTLVLWARPTXT __fastcall LvlTblsLoadLvlWarp(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "SelectX", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, SelectX) },
		{ "SelectY", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, SelectY) },
		{ "SelectDX", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, SelectDX) },
		{ "SelectDY", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, SelectDY) },
		{ "ExitWalkX", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, ExitWalkX) },
		{ "ExitWalkY", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, ExitWalkY) },
		{ "OffsetX", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, OffsetX) },
		{ "OffsetY", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, OffsetY) },
		{ "LitVersion", EXCEL_TYPE_DWORD, 0, offsetof(LVLWARPTXT, LitVersion) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSize = 0;
	PTLVLWARPTXT ptLvlWarp;

	ptLvlWarp = (PTLVLWARPTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nSize,
		sizeof(LVLWARPTXT));
#line 689
	D2ASSERT(nSize == NUM_WARP_TYPES);
	sgptWarpDefs = ptLvlWarp;
	return ptLvlWarp;
}

void __fastcall LvlTblsFreeLvlWarp(void)
{
	DataTblsFreeBlock(sgptWarpDefs);
}

PTLVLWARPTXT __fastcall LvlTblsGetWarpDef(int nWarp)
{
#line 713
	D2ASSERT(sgptWarpDefs);
	if (nWarp == -1)
		return NULL;
	return &sgptWarpDefs[nWarp];
}

PTLVLMAZETXT __fastcall LvlTblsLoadLvlMaze(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Level", EXCEL_TYPE_DWORD, 0, offsetof(LVLMAZETXT, Level) },
		{ "Rooms", EXCEL_TYPE_DWORD, 0, offsetof(LVLMAZETXT, Rooms) },
		{ "SizeX", EXCEL_TYPE_DWORD, 0, offsetof(LVLMAZETXT, SizeX) },
		{ "SizeY", EXCEL_TYPE_DWORD, 0, offsetof(LVLMAZETXT, SizeY) },
		{ "Merge", EXCEL_TYPE_DWORD, 0, offsetof(LVLMAZETXT, Merge) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTLVLMAZETXT ptLvlMaze;

	ptLvlMaze = (PTLVLMAZETXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(LVLMAZETXT));
	sgptMazeDefs = ptLvlMaze;
	sgnMazeDefs = nRows;
	return ptLvlMaze;
}

void __fastcall LvlTblsFreeLvlMaze(void)
{
	DataTblsFreeBlock(sgptMazeDefs);
}

PTLVLMAZETXT __fastcall LvlTblsGetMazeDef(int nLevelId)
{
	int ii;

#line 764
	D2ASSERT(sgptMazeDefs);
	for (ii = 0; ii < sgnMazeDefs; ii++)
	{
		if (sgptMazeDefs[ii].Level == nLevelId)
			return &sgptMazeDefs[ii];
	}
#line 773
	D2ASSERT(ii < sgnMazeDefs);
}
#line 776

void __fastcall LvlTblsLoadLvlSub(HD2ARCHIVE hArchive, void *pBuffer, int nBufferSize, BOOL fLoadFiles,
	BOOL fLoadFiles2)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Type", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Type) },
		{ "File", EXCEL_TYPE_STRING, 59, offsetof(LVLSUBTXT, File) },
		{ "CheckAll", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, CheckAll) },
		{ "BordType", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, BordType) },
		{ "GridSize", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, GridSize) },
		{ "Dt1Mask", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Dt1Mask) },
		{ "Prob0", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Prob[0]) },
		{ "Trials0", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Trials[0]) },
		{ "Max0", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Max[0]) },
		{ "Prob1", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Prob[1]) },
		{ "Trials1", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Trials[1]) },
		{ "Max1", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Max[1]) },
		{ "Prob2", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Prob[2]) },
		{ "Trials2", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Trials[2]) },
		{ "Max2", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Max[2]) },
		{ "Prob3", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Prob[3]) },
		{ "Trials3", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Trials[3]) },
		{ "Max3", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Max[3]) },
		{ "Prob4", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Prob[4]) },
		{ "Trials4", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Trials[4]) },
		{ "Max4", EXCEL_TYPE_DWORD, 0, offsetof(LVLSUBTXT, Max[4]) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	char szPath[60];
	int nRows;
	PTLVLSUBTXT ptLvlSub;
	int nMaxType;
	int nType;
	char *pszFile;
	int nLength;
	int k;
	int i;
	int ii;

	nRows = 0;
	ptLvlSub = (PTLVLSUBTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(LVLSUBTXT));
	sgptTileSubs = ptLvlSub;
	gnLvlSub = nRows;
	nMaxType = 0;
	for (i = 0; i < nRows; i++)
	{
		if (ptLvlSub[i].Type > nMaxType)
			nMaxType = ptLvlSub[i].Type;
	}
	if (nMaxType)
	{
#line 841
		sgpnTileSubsTypeStart = (int *)D2ALLOCPOOL(NULL, (nMaxType + 1) * sizeof(int));
		memset(sgpnTileSubsTypeStart, 0, (nMaxType + 1) * sizeof(int));
		nType = 0;
		for (i = 0; i < nRows; i++)
		{
			if (sgptTileSubs[i].Type != nType)
			{
				nType = sgptTileSubs[i].Type;
				sgpnTileSubsTypeStart[nType] = i;
			}
		}
	}
	if (!fLoadFiles)
	{
#line 857
		D2ASSERT(gptSubsFilesCS == FALSE);
#line 859
		gptSubsFilesCS = (LPCRITICAL_SECTION)D2ALLOC2(sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(gptSubsFilesCS);
	}
	if (fLoadFiles || fLoadFiles2)
	{
#line 865
		sgpptSubsFiles = (PTPRESETDS1 *)D2ALLOC2(gnLvlSub * sizeof(PTPRESETDS1));
		memset(sgpptSubsFiles, 0, gnLvlSub * sizeof(PTPRESETDS1));
	}
	for (ii = 0; ii < nRows; ii++)
	{
		pszFile = ptLvlSub[ii].File;
		nLength = strlen(pszFile);
		if (nLength != 0)
			for (k = 0; k < nLength; k++)
			{
				if (pszFile[k] == '/')
					pszFile[k] = '\\';
			}
		if (nLength > 1)
		{
			wsprintfA(szPath, "%s\\%s", "DATA\\GLOBAL\\TILES", pszFile);
			strcpy(pszFile, szPath);
		}
		if (fLoadFiles || fLoadFiles2)
		{
			PresetLoadFile(&sgpptSubsFiles[ii], NULL, pszFile);
#line 879
			D2ASSERT(sgpptSubsFiles[ii]->nClusters);
		}
	}
}

void __fastcall LvlTblsFreeLvlSub(void)
{
	int i;

	if (sgpptSubsFiles)
	{
		for (i = 0; i < gnLvlSub; i++)
		{
			if (sgpptSubsFiles[i])
				PresetFreeFile(&sgpptSubsFiles[i]);
		}
#line 895
		D2FREE2(sgpptSubsFiles);
		sgpptSubsFiles = NULL;
	}
	if (sgptTileSubs)
	{
		for (i = 0; i < gnLvlSub; i++)
			TileSubFreeRow(&sgptTileSubs[i]);
		DataTblsFreeBlock(sgptTileSubs);
		sgptTileSubs = NULL;
	}
	if (sgpnTileSubsTypeStart)
	{
#line 911
		D2FREEPOOL(NULL, sgpnTileSubsTypeStart);
		sgpnTileSubsTypeStart = NULL;
	}
	if (gptSubsFilesCS)
	{
		DeleteCriticalSection(gptSubsFilesCS);
#line 918
		D2FREE2(gptSubsFilesCS);
		gptSubsFilesCS = NULL;
	}
}

PTLVLSUBTXT __fastcall LvlTblsGetTileSubs(int nType)
{
#line 928
	D2ASSERT(nType >= 0);
#line 929
	D2ASSERT(sgptTileSubs);
#line 930
	D2ASSERT(sgpnTileSubsTypeStart);
	return &sgptTileSubs[sgpnTileSubsTypeStart[nType]];
}

void __fastcall LvlTblsLoadLvlTypeFiles(HD2ARCHIVE hArchive)
{
	BYTE *pbTiles;
	char *pszFile;
	int i;
	int j;
	int nSize;

#line 959
	D2ASSERT(sgptDrlgLevelTypeDefs);
#line 962
	nSize = gnLvlTypes * 4096; gpbLvlTypeTiles = (BYTE *)D2ALLOCPOOL(NULL, nSize);
	memset(gpbLvlTypeTiles, 0, nSize);
	pbTiles = gpbLvlTypeTiles;
	for (i = 0; i < gnLvlTypes; i++)
	{
		for (j = 0; j < 32; j++)
		{
			pszFile = sgptDrlgLevelTypeDefs[i].File[j];
			if (!strcmp(pszFile, "0"))
				break;
			D2CMPTileLibraryAddProject(pbTiles, pszFile);
		}
		pbTiles += 4096;
	}
}

void __fastcall LvlTblsFreeLvlTypeFiles(void)
{
	if (gpbLvlTypeTiles)
	{
#line 998
		D2FREEPOOL(NULL, gpbLvlTypeTiles);
	}
}

static __inline int LvlTblsGetLevelType(const char *pszName)
{
	int ii;

	if (pszName[0] == '0')
		return 0;
	for (ii = 0; ii < NUM_LEVEL_TYPES; ii++)
	{
		if (!strcmp(pszName, sgcszLevelTypeNames[ii]))
			return ii;
	}
#line 1101
	D2ASSERT(ii < NUM_LEVEL_TYPES);
	return ii;
}

static __inline int LvlTblsGetBaseTile(const char *pszName)
{
	int ii;

	if (pszName[0] == '0')
		return 0;
	for (ii = 0; ii < NUM_BASE_TILES; ii++)
	{
		if (!strcmp(pszName, sgcszBaseTileNames[ii]))
			return ii;
	}
#line 1143
	D2ASSERT(ii < NUM_BASE_TILES);
	return ii;
}

void __fastcall LvlTblsLoadAutomap(HD2ARCHIVE hArchive, void *pBuffer, int nBufferSize)
{
	D2CommonSeedInit(sgptAutomapSeed);
	D2CommonSeedSet(sgptAutomapSeed, 0);
	EXCELCOLDESC tDesc[] =
	{
		{ "LevelName", EXCEL_TYPE_STRING, 15, offsetof(AUTOMAPTXT, LevelName) },
		{ "TileName", EXCEL_TYPE_STRING, 7, offsetof(AUTOMAPTXT, TileName) },
		{ "Style", EXCEL_TYPE_BYTE, 0, offsetof(AUTOMAPTXT, Style) },
		{ "StartSequence", EXCEL_TYPE_BYTE, 0, offsetof(AUTOMAPTXT, StartSequence) },
		{ "EndSequence", EXCEL_TYPE_BYTE, 0, offsetof(AUTOMAPTXT, EndSequence) },
		{ "Cel1", EXCEL_TYPE_DWORD, 0, offsetof(AUTOMAPTXT, Cel[0]) },
		{ "Cel2", EXCEL_TYPE_DWORD, 0, offsetof(AUTOMAPTXT, Cel[1]) },
		{ "Cel3", EXCEL_TYPE_DWORD, 0, offsetof(AUTOMAPTXT, Cel[2]) },
		{ "Cel4", EXCEL_TYPE_DWORD, 0, offsetof(AUTOMAPTXT, Cel[3]) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nSize;
	PTAUTOMAPTXT ptAutomap;
	PTAUTOMAPSUB ptSubs;
	int nType;
	int ii;
	int k;

	ptAutomap = (PTAUTOMAPTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nSize,
		sizeof(AUTOMAPTXT));
#line 1188
	ptSubs = (PTAUTOMAPSUB)D2ALLOCPOOL(NULL, nSize * sizeof(AUTOMAPSUB));
	memset(ptSubs, 0, nSize * sizeof(AUTOMAPSUB));
	for (ii = 0; ii < nSize; ii++)
	{
		ptSubs[ii].nLevelType = LvlTblsGetLevelType(ptAutomap[ii].LevelName);
		ptSubs[ii].nTile = LvlTblsGetBaseTile(ptAutomap[ii].TileName);
		ptSubs[ii].bStyle = ptAutomap[ii].Style;
		ptSubs[ii].bStartSequence = ptAutomap[ii].StartSequence;
		ptSubs[ii].bEndSequence = ptAutomap[ii].EndSequence;
		ptSubs[ii].anCels[0] = ptAutomap[ii].Cel[0];
		ptSubs[ii].anCels[1] = ptAutomap[ii].Cel[1];
		ptSubs[ii].anCels[2] = ptAutomap[ii].Cel[2];
		ptSubs[ii].anCels[3] = ptAutomap[ii].Cel[3];
		ptSubs[ii].nCelNum = 0;
		for (k = 0; k < 4; k++)
		{
			if (ptSubs[ii].anCels[k] == -1)
				break;
			ptSubs[ii].nCelNum++;
		}
#line 1210
		D2ASSERT(ptSubs[ii].nCelNum);
	}
	DataTblsFreeBlock(ptAutomap);
	sgnAutomapSubs = nSize;
	sgptAutomapSubs = ptSubs;
	for (k = 0; k < NUM_LEVEL_TYPES; k++)
	{
		sgtAutomapRanges[k].nStart = -1;
		sgtAutomapRanges[k].nEnd = -1;
	}
	for (ii = 0; ii < nSize; )
	{
		nType = ptSubs[ii].nLevelType;
		sgtAutomapRanges[nType].nStart = ii;
		while (ii + 1 < nSize && nType == ptSubs[ii + 1].nLevelType)
			ii++;
		ii++;
		sgtAutomapRanges[nType].nEnd = ii;
	}
#line 1237
	D2ASSERT(ii <= nSize);
}

int __fastcall LvlTblsGetAutomapCel(int eLevelType, int nTile, int nStyle, int nSequence)
{
	int nStart;
	int nEnd;
	int i;
	PTAUTOMAPSUB ptSub;
	int nCel;

#line 1244
	D2ASSERT(eLevelType != LEVEL_TYPE_NONE);
	nStart = sgtAutomapRanges[eLevelType].nStart;
	nEnd = sgtAutomapRanges[eLevelType].nEnd;
	if (nStart == -1)
		return -1;
#line 1254
	D2ASSERT(nEnd != -1);
	for (i = nStart; i < nEnd; i++)
	{
		ptSub = &sgptAutomapSubs[i];
		if (ptSub->nLevelType == eLevelType && ptSub->nTile == nTile
			&& (ptSub->bStyle == 255 || ptSub->bStyle == nStyle)
			&& (ptSub->bStartSequence == 255
				|| (ptSub->bStartSequence <= nSequence && ptSub->bEndSequence >= nSequence)))
		{
			if (sgptAutomapSubs[i].nCelNum)
				nCel = D2CommonSeedRand(sgptAutomapSeed, sgptAutomapSubs[i].nCelNum);
			else
				nCel = 0;
			return sgptAutomapSubs[i].anCels[nCel];
		}
	}
	return -1;
}

void __fastcall LvlTblsFreeAutomap(void)
{
	if (sgptAutomapSubs)
	{
#line 1276
		D2FREEPOOL(NULL, sgptAutomapSubs);
	}
	sgptAutomapSubs = NULL;
}
