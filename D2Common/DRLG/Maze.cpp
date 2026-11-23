#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\Maze.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"
#include "MazeLocal.h"

struct MAZEWARPDEF
{
 	int			nDef;
 	int			nNewDef;
 	int			nFile;
 	int			nDirection;
};
typedef struct MAZEWARPDEF MAZEWARPDEF, *PTMAZEWARPDEF;

static ROOM *__fastcall sFindWestRoom(DRLGLEVEL *ptLevel);
static ROOM *__fastcall sFindEastRoom(DRLGLEVEL *ptLevel);
static ROOM *__fastcall sFindSouthRoom(DRLGLEVEL *ptLevel);
static ROOM *__fastcall sFindNorthRoom(DRLGLEVEL *ptLevel);
static ROOM *__fastcall sSetRoomPreset(ROOM *ptRoom, int nDef, int nFile, BOOL fUnfixed);
static BOOL __fastcall sRoomIsUnfixed(ROOM *ptRoom);
static DWORD __fastcall sSeedBits(D2SEED *ptSeed, DWORD dwMask);
static ROOM *__fastcall sReplaceRoomPreset(DRLGLEVEL *ptLevel, int nDef, int nNewDef, int nFile,
	BOOL fUnfixed);
static ROOM *__fastcall sAddRoomInDir(ROOM *ptSource, int nDirection, BOOL fMerge);
static void __fastcall sBuildRing(DRLGLEVEL *ptLevel, int nRooms);
static BOOL __fastcall sTryPlaceRoom(ROOM *ptRoom, ROOM *ptSource, int nDirection);
static void __fastcall sMergeRoom(ROOM *ptRoom);
static ROOM *__fastcall sGetRandomRoom(DRLGLEVEL *ptLevel);
static void __fastcall sGrowRooms(DRLGLEVEL *ptLevel);
static void __fastcall sStartTombs(DRLGLEVEL *ptLevel);
static void __fastcall sBuildArcaneRooms(DRLGLEVEL *ptLevel);
static void __fastcall sSetRoomFile(ROOM *ptRoom, int nFile);
static ROOM *__fastcall sNewRoomInDir(ROOM *ptSource, int nDirection, BOOL fMerge);
static void __fastcall sPlaceWarpRoom(DRLGLEVEL *ptLevel, const MAZEWARPDEF *ptWarp,
	int *pnDirection);
static void __fastcall sPlaceCatacombsWarps(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceCaveWarps(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceCryptWarps(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceJailWarps(DRLGLEVEL *ptLevel);
static void __fastcall sStartCatacombs(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceSewerWarpsActTwo(DRLGLEVEL *ptLevel);
static ROOM *__fastcall sAddWarpRoom(DRLGLEVEL *ptLevel, int nDirection, int nDef,
	int nFile);
static BOOL __fastcall sTestAddRoomInDir(ROOM *ptRoom, int nDirection);
static void __fastcall sPlaceTombStuff(DRLGLEVEL *ptLevel);
static ROOM *__fastcall sAddRoomWithPreset(ROOM *ptSource, int nDirection, int nDef, int nFile,
	BOOL fChamber);
static void __fastcall sPlaceLairWarps(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceArcaneWarps(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceDungeonWarps(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceSewerWarpsActThree(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceKurastWarps(DRLGLEVEL *ptLevel);
static void __fastcall sConnectBarracks(DRLGLEVEL *ptLevel);
static void __fastcall sSetRoomMazeSize(ROOM *ptRoom);
static void __fastcall sPlaceRiverOfFlame(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceThemeRooms(DRLGLEVEL *ptLevel);
static void __fastcall sRoomsToPresets(DRLGLEVEL *ptLevel);

static const int sgcanChamberDefs[16][3] =
{
	{ 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
	{ 356, 360, 659 }, { 355, 359, 660 }, { 0, 0, 0 }, { 0, 0, 0 }, { 357, 361, 661 },
	{ 354, 358, 662 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 },
};
static const int sgcanTombEntryDefs[4] = { 447, 444, 446, 445 };

static const MAZEWARPDEF sgcatMazeWarps[33][4] =
{
	{ { 60, 86, -1, 3 }, { 54, 84, -1, 0 }, { 56, 85, -1, 1 }, { 53, 83, -1, 2 } },
	{ { 60, 98, -1, 3 }, { 54, 96, -1, 0 }, { 56, 97, -1, 1 }, { 53, 95, -1, 2 } },
	{ { 60, 94, -1, 3 }, { 54, 92, -1, 0 }, { 56, 93, -1, 1 }, { 53, 91, -1, 2 } },
	{ { 60, 102, -1, 3 }, { 54, 100, -1, 0 }, { 56, 101, -1, 1 }, { 53, 99, -1, 2 } },
	{ { 60, 90, -1, 3 }, { 54, 88, -1, 0 }, { 56, 89, -1, 1 }, { 53, 87, -1, 2 } },
	{ { 116, 142, -1, 3 }, { 110, 140, -1, 0 }, { 112, 141, -1, 1 }, { 109, 139, -1, 2 } },
	{ { 116, 150, -1, 3 }, { 110, 148, -1, 0 }, { 112, 149, -1, 1 }, { 109, 147, -1, 2 } },
	{ { 116, 158, -1, 3 }, { 110, 156, -1, 0 }, { 112, 157, -1, 1 }, { 109, 155, -1, 2 } },
	{ { 116, 154, -1, 3 }, { 110, 152, -1, 0 }, { 112, 153, -1, 1 }, { 109, 151, -1, 2 } },
	{ { 116, 146, -1, 3 }, { 110, 144, -1, 0 }, { 112, 145, -1, 1 }, { 109, 143, -1, 2 } },
	{ { 213, 239, -1, 3 }, { 207, 237, -1, 0 }, { 209, 238, -1, 1 }, { 206, 236, -1, 2 } },
	{ { 213, 247, -1, 3 }, { 207, 245, -1, 0 }, { 209, 246, -1, 1 }, { 206, 244, -1, 2 } },
	{ { 213, 243, -1, 3 }, { 207, 241, -1, 0 }, { 209, 242, -1, 1 }, { 206, 240, -1, 2 } },
	{ { 213, 251, -1, 3 }, { 207, 249, -1, 0 }, { 209, 250, -1, 1 }, { 206, 248, -1, 2 } },
	{ { 213, 255, -1, 3 }, { 207, 253, -1, 0 }, { 209, 254, -1, 1 }, { 206, 252, -1, 2 } },
	{ { 265, 294, -1, 3 }, { 259, 292, -1, 0 }, { 261, 293, -1, 1 }, { 258, 291, -1, 2 } },
	{ { 265, 298, -1, 3 }, { 259, 296, -1, 0 }, { 261, 297, -1, 1 }, { 258, 295, -1, 2 } },
	{ { 309, 335, -1, 3 }, { 303, 333, -1, 0 }, { 305, 334, -1, 1 }, { 302, 332, -1, 2 } },
	{ { 309, 340, -1, 3 }, { 303, 338, -1, 0 }, { 305, 339, -1, 1 }, { 302, 337, -1, 2 } },
	{ { 309, 348, -1, 3 }, { 303, 346, -1, 0 }, { 305, 347, -1, 1 }, { 302, 345, -1, 2 } },
	{ { 309, 344, -1, 3 }, { 303, 342, -1, 0 }, { 305, 343, -1, 1 }, { 302, 341, -1, 2 } },
	{ { 309, 352, -1, 3 }, { 303, 350, -1, 0 }, { 305, 351, -1, 1 }, { 302, 349, -1, 2 } },
	{ { 421, 451, -1, 3 }, { 415, 449, -1, 0 }, { 417, 450, -1, 1 }, { 414, 448, -1, 2 } },
	{ { 421, 479, -1, 3 }, { 415, 477, -1, 0 }, { 417, 478, -1, 1 }, { 414, 476, -1, 2 } },
	{ { 421, 475, -1, 3 }, { 415, 473, -1, 0 }, { 417, 474, -1, 1 }, { 414, 472, -1, 2 } },
	{ { 421, 467, -1, 3 }, { 415, 465, -1, 0 }, { 417, 466, -1, 1 }, { 414, 464, -1, 2 } },
	{ { 421, 459, -1, 3 }, { 415, 457, -1, 0 }, { 417, 458, -1, 1 }, { 414, 456, -1, 2 } },
	{ { 421, 455, -1, 3 }, { 415, 453, -1, 0 }, { 417, 454, -1, 1 }, { 414, 452, -1, 2 } },
	{ { 421, 463, -1, 3 }, { 415, 461, -1, 0 }, { 417, 462, -1, 1 }, { 414, 460, -1, 2 } },
	{ { 421, 471, -1, 3 }, { 415, 469, -1, 0 }, { 417, 470, -1, 1 }, { 414, 468, -1, 2 } },
	{ { 489, 500, -1, 3 }, { 483, 498, -1, 0 }, { 485, 499, -1, 1 }, { 482, 497, -1, 2 } },
	{ { 489, 504, -1, 3 }, { 483, 502, -1, 0 }, { 485, 503, -1, 1 }, { 482, 501, -1, 2 } },
	{ { 489, 508, -1, 3 }, { 483, 506, -1, 0 }, { 485, 507, -1, 1 }, { 482, 505, -1, 2 } },
};

static const MAZEWARPDEF sgctMaggotLairWarp = { 485, 509, -1, 1 };

static const MAZEWARPDEF sgcatMazeWarpsB[8][4] =
{
	{ { 517, 528, -1, 3 }, { 511, 526, -1, 0 }, { 513, 527, -1, 1 }, { 510, 525, -1, 2 } },
	{ { 672, 698, -1, 3 }, { 666, 696, -1, 0 }, { 668, 697, -1, 1 }, { 665, 695, -1, 2 } },
	{ { 672, 702, -1, 3 }, { 666, 700, -1, 0 }, { 668, 701, -1, 1 }, { 665, 699, -1, 2 } },
	{ { 712, 742, -1, 3 }, { 706, 740, -1, 0 }, { 708, 741, -1, 1 }, { 705, 739, -1, 2 } },
	{ { 712, 746, -1, 3 }, { 706, 744, -1, 0 }, { 708, 745, -1, 1 }, { 705, 743, -1, 2 } },
	{ { 761, 787, -1, 3 }, { 755, 785, -1, 0 }, { 757, 786, -1, 1 }, { 754, 784, -1, 2 } },
	{ { 761, 795, -1, 3 }, { 755, 793, -1, 0 }, { 757, 794, -1, 1 }, { 754, 792, -1, 2 } },
	{ { 761, 791, -1, 3 }, { 755, 789, -1, 0 }, { 757, 790, -1, 1 }, { 754, 788, -1, 2 } },
};

typedef ROOM *(__fastcall *PFNMAZEEDGEROOM)(DRLGLEVEL *ptLevel);
static const PFNMAZEEDGEROOM scpfnBarracksEdgeRoom[4] =
{
	sFindEastRoom, sFindSouthRoom, sFindWestRoom, NULL,
};

static const MAZEWARPDEF sgcatBarracksWarps[2][4] =
{
	{ { 175, 201, -1, 3 }, { 169, 199, -1, 0 }, { 171, 200, -1, 1 }, { 168, 198, -1, 2 } },
	{ { 175, 205, -1, 3 }, { 169, 203, -1, 0 }, { 171, 204, -1, 1 }, { 168, 202, -1, 2 } },
};

static const MAZEWARPDEF sgcatRiverWarps[2] =
{
	{ 837, 853, -1, 2 }, { 838, 854, -1, 0 },
};

static BOOL __fastcall sRoomIsUnfixed(ROOM *ptRoom)
{
	return !(ptRoom->field_20.ptPreset->dwFlags & 2);
}

static DWORD __fastcall sSeedBits(D2SEED *ptSeed, DWORD dwMask)
{
	return SeedNext(ptSeed) & dwMask;
}

static ROOM *__fastcall sFindWestRoom(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	ROOM *ptFound;

	ptFound = NULL;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptFound && ptRoom->tCoords.nTileX >= ptFound->tCoords.nTileX)
			continue;
		if (sTestAddRoomInDir(ptRoom, 0))
			ptFound = ptRoom;
	}
	return ptFound;
}

static ROOM *__fastcall sSetChamberPreset(ROOM *ptRoom, BOOL fUnfixed)
{
	PTDRLGROOMLINK ptLink;
	int nDef;
	int nFile;

	nDef = 0;
	nFile = -1;
	for (ptLink = ptRoom->ptLinks; ptLink; ptLink = ptLink->ptNext)
	{
		switch (ptLink->nDirection)
		{
		case 0:
			nDef |= 1;
			break;
		case 1:
			nDef |= 8;
			break;
		case 2:
			nDef |= 2;
			break;
		case 3:
			nDef |= 4;
			break;
		}
	}
	switch (ptRoom->ptLevel->dwLevelType)
	{
	case 3:
		nDef += 52;
		break;
	case 4:
		nDef += 108;
		break;
	case 7:
		nDef += 167;
		break;
	case 8:
		nDef += 205;
		break;
	case 10:
		nDef += 257;
		break;
	case 13:
		nDef += 301;
		break;
	case 14:
		nDef = sgcanChamberDefs[nDef][0];
		break;
	case 15:
		nDef = sgcanChamberDefs[nDef][1];
		if (ptRoom->ptLevel->nLevelId == 52 && nDef == 361)
			nFile = 2;
		if (ptRoom->ptLevel->nLevelId == 54 && (nDef == 361 || nDef == 359))
			nFile = 3;
		break;
	case 17:
		nDef += 413;
		break;
	case 18:
		nDef += 481;
		break;
	case 19:
		nDef += 509;
		break;
	case 22:
		nDef += 753;
		break;
	case 23:
		nDef = sgcanChamberDefs[nDef][2];
		if (ptRoom->ptLevel->nLevelId == 84 && nDef == 662)
			nDef = 664;
		if (ptRoom->ptLevel->nLevelId == 85 && nDef == 661)
			nDef = 663;
		break;
	case 24:
		nDef += 664;
		break;
	case 25:
		nDef += 704;
		break;
	case 28:
		nDef += 836;
		break;
	default:
#line 295
		D2FATAL("sSetChamberPreset() - Some really bad voodoo here!");
	}
	if (nDef)
		sSetRoomPreset(ptRoom, nDef, nFile, fUnfixed);
	return ptRoom;
}

static ROOM *__fastcall sFindEastRoom(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	ROOM *ptFound;

	ptFound = NULL;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptFound && ptRoom->tCoords.nTileX <= ptFound->tCoords.nTileX)
			continue;
		if (sTestAddRoomInDir(ptRoom, 2))
			ptFound = ptRoom;
	}
	return ptFound;
}

static ROOM *__fastcall sFindSouthRoom(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	ROOM *ptFound;

	ptFound = NULL;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptFound && ptRoom->tCoords.nTileY <= ptFound->tCoords.nTileY)
			continue;
		if (sTestAddRoomInDir(ptRoom, 3))
			ptFound = ptRoom;
	}
	return ptFound;
}

static ROOM *__fastcall sFindNorthRoom(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	ROOM *ptFound;

	ptFound = NULL;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		if (ptFound && ptRoom->tCoords.nTileY >= ptFound->tCoords.nTileY)
			continue;
		if (sTestAddRoomInDir(ptRoom, 1))
			ptFound = ptRoom;
	}
	return ptFound;
}

void __fastcall MazeLevelInit(DRLGLEVEL *ptLevel)
{
	ptLevel->field_38.ptMazeDef = LvlTblsGetMazeDef((int)ptLevel->nLevelId);
	DrlgLevelSetRect(ptLevel->ptAct, ptLevel);
}

void __fastcall MazeLevelGenerate(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;

	ptRoom = DrlgRoomNew(ptLevel, 2);
	sSetRoomMazeSize(ptRoom);
	ptRoom->tCoords.nTileX = ptLevel->tCoords.nTileX +
		(ptLevel->tCoords.nSizeTileX - ptRoom->tCoords.nSizeTileX) / 2;
	ptRoom->tCoords.nTileY = ptLevel->tCoords.nTileY +
		(ptLevel->tCoords.nSizeTileY - ptRoom->tCoords.nSizeTileY) / 2;
	DrlgRoomLink(ptLevel, ptRoom);
	switch (ptLevel->dwLevelType)
	{
	case 3:
		sGrowRooms(ptLevel);
		sPlaceCaveWarps(ptLevel);
		break;
	case 4:
		sGrowRooms(ptLevel);
		sPlaceCryptWarps(ptLevel);
		break;
	case 7:
		sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		break;
	case 8:
		sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		sPlaceJailWarps(ptLevel);
		break;
	case 10:
		sStartCatacombs(ptLevel);
		sGrowRooms(ptLevel);
		sPlaceCatacombsWarps(ptLevel);
		break;
	case 13:
		sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		sPlaceSewerWarpsActTwo(ptLevel);
		break;
	case 17:
		if (ptLevel->nLevelId == 61)
			sSetRoomPreset(ptRoom, 480, -1, FALSE);
		else
		{
			sStartTombs(ptLevel);
			sGrowRooms(ptLevel);
			sPlaceTombStuff(ptLevel);
		}
		break;
	case 18:
		sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		sPlaceLairWarps(ptLevel);
		break;
	case 19:
		sBuildArcaneRooms(ptLevel);
		sPlaceArcaneWarps(ptLevel);
		break;
	case 14:
	case 15:
	case 23:
		sBuildRing(ptLevel, 2);
		break;
	case 22:
		sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		sPlaceKurastWarps(ptLevel);
		break;
	case 24:
		sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		sPlaceDungeonWarps(ptLevel);
		break;
	case 25:
		if (ptLevel->nLevelId == 92)
		{
			sBuildRing(ptLevel, 5);
			ptRoom = sReplaceRoomPreset(ptLevel, 709, 735, -1, FALSE);
#line 1871
			D2WARN(ptRoom);
			ptRoom = sReplaceRoomPreset(ptLevel, 710, 736, -1, FALSE);
#line 1872
			D2WARN(ptRoom);
			ptRoom = sReplaceRoomPreset(ptLevel, 713, 737, -1, FALSE);
#line 1873
			D2WARN(ptRoom);
			ptRoom = sReplaceRoomPreset(ptLevel, 714, 738, -1, FALSE);
#line 1874
			D2WARN(ptRoom);
		}
		else
			sBuildRing(ptLevel, 2);
		sGrowRooms(ptLevel);
		sPlaceSewerWarpsActThree(ptLevel);
		break;
	case 28:
		sGrowRooms(ptLevel);
		break;
	default:
#line 1889
		D2FATAL("MazeLevelGenerate() - Some really bad voodoo here!");
	}
	switch (ptLevel->nLevelId)
	{
	case 28:
		sConnectBarracks(ptLevel);
		break;
	case 107:
		sPlaceRiverOfFlame(ptLevel);
		break;
	default:
		DrlgLevelFitRooms(ptLevel);
		break;
	}
	sPlaceThemeRooms(ptLevel);
	sRoomsToPresets(ptLevel);
}

static void __fastcall sPlaceCaveWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[0][nDirection], &nDirection);
	if (ptLevel->nLevelId == 8)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[1][nDirection], &nDirection);
	else
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[2][nDirection], &nDirection);
	if (ptLevel->nLevelId == 9)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[3][nDirection], &nDirection);
	if (ptLevel->nLevelId == 10)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[4][nDirection], &nDirection);
}

static void __fastcall sPlaceCryptWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[5][nDirection], &nDirection);
	if (ptLevel->nLevelId == 18)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[6][nDirection], &nDirection);
	if (ptLevel->nLevelId == 19)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[8][nDirection], &nDirection);
	if ((int)ptLevel->nLevelId >= 21 && (int)ptLevel->nLevelId <= 24)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[9][nDirection], &nDirection);
}

static void __fastcall sPlaceJailWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[10][nDirection], &nDirection);
	if (ptLevel->nLevelId == 29)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[13][nDirection], &nDirection);
	if (ptLevel->nLevelId == 30)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[14][nDirection], &nDirection);
	if (ptLevel->nLevelId == 31)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[11][nDirection], &nDirection);
	else
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[12][nDirection], &nDirection);
}

static void __fastcall sStartCatacombs(DRLGLEVEL *ptLevel)
{
	ROOM *ptFirst;

	ptFirst = ptLevel->ptRoomFirst;
	if (ptLevel->nLevelId == 34)
	{
		sAddRoomInDir(ptFirst, 1, TRUE);
		sAddRoomInDir(ptFirst, 2, TRUE);
		sAddRoomInDir(ptFirst, 3, TRUE);
		sAddRoomInDir(ptFirst, 0, TRUE);
		sSetRoomPreset(ptFirst, 290, -1, FALSE);
	}
	else if (sSeedBits(&ptLevel->tSeed, 1))
	{
		sAddRoomInDir(ptFirst, 0, TRUE);
		sAddRoomInDir(ptFirst, 2, TRUE);
		sSetRoomPreset(ptFirst, 288, -1, FALSE);
	}
	else
	{
		sAddRoomInDir(ptFirst, 1, TRUE);
		sAddRoomInDir(ptFirst, 3, TRUE);
		sSetRoomPreset(ptFirst, 289, -1, FALSE);
	}
}

static ROOM *__fastcall sSetRoomPreset(ROOM *ptRoom, int nDef, int nFile, BOOL fUnfixed)
{
	PTPRESETINFO ptInfo;

	ptInfo = ptRoom->field_20.ptPreset;
	ptInfo->dwDef = (DWORD)nDef;
	ptInfo->nIndex = nFile;
	if (fUnfixed)
		ptInfo->dwFlags &= ~2;
	else
		ptInfo->dwFlags |= 2;
	return ptRoom;
}

static ROOM *__fastcall sReplaceRoomPreset(DRLGLEVEL *ptLevel, int nDef, int nNewDef, int nFile,
	BOOL fUnfixed)
{
	ROOM *ptRoom;
	PTPRESETINFO ptInfo;
	BOOL fChangeable;

	for (ptRoom = ptLevel->ptRoomFirst; ptRoom != NULL; ptRoom = ptRoom->ptLevelNext)
	{
		fChangeable = sRoomIsUnfixed(ptRoom);
		if (!fChangeable)
			continue;
		ptInfo = ptRoom->field_20.ptPreset;
		if (ptInfo->dwDef == (DWORD)nDef)
		{
			sSetRoomPreset(ptRoom, nNewDef, nFile, fUnfixed);
			return ptRoom;
		}
	}
	return NULL;
}

static ROOM *__fastcall sAddRoomInDir(ROOM *ptSource, int nDirection, BOOL fMerge)
{
	ROOM *ptRoom;

	ptRoom = sNewRoomInDir(ptSource, nDirection, fMerge);
	if (ptRoom != NULL)
	{
		if (fMerge)
			sSetChamberPreset(ptSource, TRUE);
		sSetChamberPreset(ptRoom, TRUE);
	}
	return ptRoom;
}

static void __fastcall sBuildRing(DRLGLEVEL *ptLevel, int nRooms)
{
	ROOM *ptFirst;
	ROOM *ptRoom;
	int i;

	ptFirst = ptLevel->ptRoomFirst;
	ptRoom = ptFirst;
	for (i = 0; i < nRooms - 1; i++)
		ptRoom = sAddRoomInDir(ptRoom, 1, TRUE);
	for (i = 0; i < nRooms - 1; i++)
		ptRoom = sAddRoomInDir(ptRoom, 0, TRUE);
	for (i = 0; i < nRooms - 1; i++)
		ptRoom = sAddRoomInDir(ptRoom, 3, TRUE);
	for (i = 0; i < nRooms - 2; i++)
		ptRoom = sAddRoomInDir(ptRoom, 2, TRUE);
	DrlgRoomLinkRooms(ptRoom, ptFirst, 2);
	sSetChamberPreset(ptRoom, TRUE);
	sSetChamberPreset(ptFirst, TRUE);
}

static BOOL __fastcall sTryPlaceRoom(ROOM *ptRoom, ROOM *ptSource, int nDirection)
{
	DRLGCOORDS *ptCoords;
	PTDRLGROOMLINK ptLink;

	ptCoords = &ptRoom->tCoords;
	switch (nDirection)
	{
	case 0:
		ptCoords->nTileX = ptSource->tCoords.nTileX - ptSource->tCoords.nSizeTileX;
		ptCoords->nTileY = ptSource->tCoords.nTileY;
		break;
	case 1:
		ptCoords->nTileX = ptSource->tCoords.nTileX;
		ptCoords->nTileY = ptSource->tCoords.nTileY - ptSource->tCoords.nSizeTileY;
		break;
	case 2:
		ptCoords->nTileX = ptSource->tCoords.nSizeTileX + ptSource->tCoords.nTileX;
		ptCoords->nTileY = ptSource->tCoords.nTileY;
		break;
	case 3:
		ptCoords->nTileX = ptSource->tCoords.nTileX;
		ptCoords->nTileY = ptSource->tCoords.nSizeTileY + ptSource->tCoords.nTileY;
		break;
	case 4:
		ptCoords->nTileX = ptSource->tCoords.nTileX - ptSource->tCoords.nSizeTileX;
		ptCoords->nTileY = ptSource->tCoords.nTileY - ptSource->tCoords.nSizeTileY;
		break;
	case 5:
		ptCoords->nTileX = ptSource->tCoords.nSizeTileX + ptSource->tCoords.nTileX;
		ptCoords->nTileY = ptSource->tCoords.nTileY - ptSource->tCoords.nSizeTileY;
		break;
	case 6:
		ptCoords->nTileX = ptSource->tCoords.nSizeTileX + ptSource->tCoords.nTileX;
		ptCoords->nTileY = ptSource->tCoords.nSizeTileY + ptSource->tCoords.nTileY;
		break;
	case 7:
		ptCoords->nTileX = ptSource->tCoords.nTileX - ptSource->tCoords.nSizeTileX;
		ptCoords->nTileY = ptSource->tCoords.nSizeTileY + ptSource->tCoords.nTileY;
		break;
	}
	for (ptLink = ptSource->ptLinks; ptLink; ptLink = ptLink->ptNext)
	{
		if (!DrlgRectGapTest(ptCoords, ptLink->ptCoords, 0))
			return FALSE;
	}
	return DrlgRoomFitsInLevel(ptRoom->ptLevel, ptRoom, ptSource, 0);
}

static void __fastcall sMergeRoom(ROOM *ptRoom)
{
	ROOM *ptOther;
	PTLVLMAZETXT ptMazeDef;
	PTDRLGROOMLINK ptLink;
	int nEdge;

	if (!sRoomIsUnfixed(ptRoom))
		return;
	ptOther = ptRoom->ptLevel->ptRoomFirst;
	ptMazeDef = ptRoom->ptLevel->field_38.ptMazeDef;
	for (; ptOther; ptOther = ptOther->ptLevelNext)
	{
		if (ptOther == ptRoom || !sRoomIsUnfixed(ptOther))
			continue;
		{
			int nGapX;
			int nGapY;

			nGapX = 0;
			nGapY = 0;
			if (DrlgRectTestApartAndGaps(&ptRoom->tCoords, &ptOther->tCoords, 1, &nGapX, &nGapY))
				continue;
			if (nGapX == nGapY)
				continue;
		}
		for (ptLink = ptRoom->ptLinks; ptLink; ptLink = ptLink->ptNext)
		{
			if (ptLink->ptOtherRoom == ptOther)
				goto next;
		}
		if ((int)SeedRand(&ptOther->tSeed, 1000) >= ptMazeDef->Merge)
			continue;
		nEdge = DrlgRectEdgeDir(&ptOther->tCoords, &ptRoom->tCoords);
		if (nEdge == -1)
			continue;
		DrlgRoomLinkRooms(ptOther, ptRoom, nEdge);
		sSetChamberPreset(ptOther, TRUE);
next:
		;
	}
}

static ROOM *__fastcall sGetRandomRoom(DRLGLEVEL *ptLevel)
{
	int n;
	ROOM *ptRoom;

	n = ptLevel->nNumRooms ? (int)SeedRand(&ptLevel->tSeed, (DWORD)ptLevel->nNumRooms) : 0;
	ptRoom = ptLevel->ptRoomFirst;
	while (n--)
		ptRoom = ptRoom->ptLevelNext;
	return ptRoom;
}

static void __fastcall sGrowRooms(DRLGLEVEL *ptLevel)
{
	int nRooms;
	ROOM *ptRoom;
	int nDirection;

	nRooms = ptLevel->field_38.ptMazeDef->Rooms;
	if ((int)ptLevel->nLevelId == ptLevel->ptAct->nTombLevelIdA)
		nRooms *= 3;
	if ((int)ptLevel->nLevelId == ptLevel->ptAct->nTombLevelIdB)
		nRooms *= 2;
	while (ptLevel->nNumRooms < nRooms)
	{
		ptRoom = sGetRandomRoom(ptLevel);
		nDirection = sSeedBits(&ptRoom->tSeed, 3);
		if (sRoomIsUnfixed(ptRoom))
			sAddRoomInDir(ptRoom, nDirection, TRUE);
	}
}

static void __fastcall sStartTombs(DRLGLEVEL *ptLevel)
{
	ROOM *ptFirst;
	int nDirection;
	int i;

	ptFirst = ptLevel->ptRoomFirst;
	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	for (i = 0; i < 3; i++)
	{
		sAddRoomInDir(ptFirst, nDirection, TRUE);
		nDirection = (nDirection + 1) % 4;
	}
	sSetRoomPreset(ptFirst, sgcanTombEntryDefs[nDirection], -1, FALSE);
}

static void __fastcall sSetRoomFile(ROOM *ptRoom, int nFile)
{
	ptRoom->field_20.ptPreset->nIndex = nFile;
}

static void __fastcall sBuildArcaneRooms(DRLGLEVEL *ptLevel)
{
	ROOM *ptFirst;
	ROOM *aptRooms[60];
	int nStart;
	int i;
	int n;

	ptFirst = ptLevel->ptRoomFirst;
	nStart = sSeedBits(&ptLevel->tSeed, 3);
	n = 0;
	memset(aptRooms, 0, sizeof(aptRooms));
	for (i = 0; i < 4; i++)
	{
		aptRooms[n] = sAddRoomInDir(ptFirst, i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], (i + 3) % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], (i + 1) % 4, TRUE);
		n++;
		sAddRoomInDir(aptRooms[n - 1], i % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 2], (i + 1) % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], (i + 2) % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], (i + 2) % 4, TRUE);
		n++;
		sAddRoomInDir(aptRooms[n - 1], (i + 3) % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 2], (i + 2) % 4, TRUE);
		n++;
		aptRooms[n] = sAddRoomInDir(aptRooms[n - 1], (i + 2) % 4, TRUE);
		n++;
	}
	for (i = 0; i < 60; i++)
	{
		if (aptRooms[i])
			sSetRoomFile(aptRooms[i], (i / 15 + nStart) % 4);
	}
	sSetRoomFile(ptFirst, 4);
}

static ROOM *__fastcall sNewRoomInDir(ROOM *ptSource, int nDirection, BOOL fMerge)
{
	ROOM *ptRoom;

	ptRoom = DrlgRoomNew(ptSource->ptLevel, 2);
	sSetRoomMazeSize(ptRoom);
	if (!sTryPlaceRoom(ptRoom, ptSource, nDirection))
	{
		DrlgRoomFree(ptRoom);
		return NULL;
	}
	DrlgRoomLinkRooms(ptSource, ptRoom, nDirection);
	if (fMerge)
		sMergeRoom(ptRoom);
	DrlgRoomLink(ptSource->ptLevel, ptRoom);
	return ptRoom;
}

static void __fastcall sPlaceWarpRoom(DRLGLEVEL *ptLevel, const MAZEWARPDEF *ptWarp, int *pnDirection)
{
	ROOM *ptRoom;

	ptRoom = sReplaceRoomPreset(ptLevel, ptWarp->nDef, ptWarp->nNewDef, ptWarp->nFile, FALSE);
	if (ptRoom == NULL)
		sAddWarpRoom(ptLevel, ptWarp->nDirection, ptWarp->nNewDef, ptWarp->nFile);
	if (pnDirection != NULL)
		*pnDirection = (*pnDirection + 1) % 4;
}

static void __fastcall sPlaceCatacombsWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[15][nDirection], &nDirection);
	if (ptLevel->nLevelId == 35)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[16][nDirection], NULL);
}

static void __fastcall sPlaceSewerWarpsActTwo(DRLGLEVEL *ptLevel)
{
	ROOM *ptEdge;
	ROOM *ptNew;
	int nSide;
	int nDirection;

	nSide = sSeedBits(&ptLevel->tSeed, 1) << 1 | 1;
	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	switch (ptLevel->nLevelId)
	{
	case 47:
		ptEdge = sFindNorthRoom(ptLevel);
		ptNew = sAddRoomInDir(ptEdge, 1, TRUE);
		ptNew = sAddRoomInDir(ptNew, 1, TRUE);
		sAddRoomWithPreset(ptNew, 0, 333, 0, TRUE);
		ptEdge = sFindEastRoom(ptLevel);
		ptNew = sAddRoomInDir(ptEdge, 2, TRUE);
		ptNew = sAddRoomInDir(ptNew, 2, TRUE);
		ptNew = sAddRoomWithPreset(ptNew, nSide, 336, 0, TRUE);
		sAddRoomInDir(ptNew, nSide, FALSE);
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[18][nDirection], NULL);
		break;
	case 48:
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[17][nDirection], &nDirection);
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[19][nDirection], &nDirection);
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[18][nDirection], &nDirection);
		break;
	case 49:
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[17][nDirection], &nDirection);
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[20][nDirection], &nDirection);
		break;
	case 65:
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[17][nDirection], &nDirection);
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[21][nDirection], &nDirection);
		break;
	}
}

static ROOM *__fastcall sAddWarpRoom(DRLGLEVEL *ptLevel, int nDirection, int nDef, int nFile)
{
	ROOM *ptSource;
	ROOM *ptRoom;

	ptRoom = NULL;
	for (ptSource = ptLevel->ptRoomFirst; ptSource; ptSource = ptSource->ptLevelNext)
	{
		if (sRoomIsUnfixed(ptSource))
		{
			ptRoom = sAddRoomWithPreset(ptSource, nDirection, nDef, nFile, TRUE);
			if (ptRoom != NULL)
				break;
		}
	}
	return ptRoom;
}

static BOOL __fastcall sTestAddRoomInDir(ROOM *ptRoom, int nDirection)
{
	PTDRLGROOMLINK ptLink;
	ROOM *ptNew;
	BOOL fFits;

	if (!sRoomIsUnfixed(ptRoom))
		return FALSE;
	for (ptLink = ptRoom->ptLinks; ptLink; ptLink = ptLink->ptNext)
	{
		if (ptLink->nDirection == nDirection)
			return FALSE;
	}
	ptNew = sAddRoomInDir(ptRoom, nDirection, FALSE);
	fFits = ptNew != NULL;
	if (ptNew)
		DrlgRoomFree(ptNew);
	return fFits;
}

static void __fastcall sPlaceTombStuff(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	PTPRESETINFO ptInfo;
	int nDirection;
	const MAZEWARPDEF *ptWarp;

	ptInfo = NULL;
	nDirection = 0;
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		ptInfo = ptRoom->field_20.ptPreset;
		if ((int)ptInfo->dwDef > 428)
			break;
	}
	switch (ptInfo->dwDef)
	{
	case 446:
		nDirection = 0;
		break;
	case 445:
		nDirection = 1;
		break;
	case 447:
		nDirection = 2;
		break;
	case 444:
		nDirection = 3;
		break;
	default:
#line 1154
		D2ASSERTMSG(FALSE, "sPlaceTombStuff() - Why isn't the first room the preset warp room?");
	}
	if ((int)ptLevel->nLevelId >= 55 && (int)ptLevel->nLevelId <= 58)
	{
		ptWarp = &sgcatMazeWarps[22][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if (ptLevel->nLevelId == 57)
	{
		ptWarp = &sgcatMazeWarps[23][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if (ptLevel->nLevelId == 59 || ptLevel->nLevelId == 61)
	{
		ptWarp = &sgcatMazeWarps[24][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if ((int)ptLevel->nLevelId >= 66 && (int)ptLevel->nLevelId <= 72 &&
		(int)ptLevel->nLevelId != ptLevel->ptAct->nTombLevelIdA)
	{
		ptWarp = &sgcatMazeWarps[24][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if (ptLevel->nLevelId == 59)
	{
		ptWarp = &sgcatMazeWarps[25][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if (ptLevel->nLevelId == 60)
	{
		ptWarp = &sgcatMazeWarps[26][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if (ptLevel->nLevelId == 59 || ptLevel->nLevelId == 61)
	{
		ptWarp = &sgcatMazeWarps[27][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if ((int)ptLevel->nLevelId == ptLevel->ptAct->nTombLevelIdA)
	{
		ptWarp = &sgcatMazeWarps[28][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
	if ((int)ptLevel->nLevelId == ptLevel->ptAct->nTombLevelIdB)
	{
		ptWarp = &sgcatMazeWarps[29][nDirection];
		sPlaceWarpRoom(ptLevel, ptWarp, &nDirection);
	}
}

static ROOM *__fastcall sAddRoomWithPreset(ROOM *ptSource, int nDirection, int nDef, int nFile,
	BOOL fChamber)
{
	ROOM *ptRoom;

	ptRoom = sNewRoomInDir(ptSource, nDirection, FALSE);
	if (ptRoom)
	{
		if (fChamber)
			sSetChamberPreset(ptSource, TRUE);
		sSetRoomPreset(ptRoom, nDef, nFile, FALSE);
	}
	return ptRoom;
}

static void __fastcall sPlaceLairWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	if (ptLevel->nLevelId == 64)
	{
		sPlaceWarpRoom(ptLevel, &sgctMaggotLairWarp, NULL);
		nDirection = 3;
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[32][nDirection], &nDirection);
	}
	else
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[31][nDirection], &nDirection);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarps[30][nDirection], NULL);
}

static void __fastcall sPlaceArcaneWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[0][nDirection], NULL);
}

static void __fastcall sPlaceDungeonWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[1][nDirection], &nDirection);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[2][nDirection], NULL);
}

static void __fastcall sPlaceSewerWarpsActThree(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[3][nDirection], &nDirection);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[4][nDirection], NULL);
}

static void __fastcall sPlaceKurastWarps(DRLGLEVEL *ptLevel)
{
	int nDirection;

	nDirection = sSeedBits(&ptLevel->tSeed, 3);
	sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[5][nDirection], &nDirection);
	if (ptLevel->nLevelId == 101)
	{
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[6][nDirection], &nDirection);
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[7][nDirection], &nDirection);
	}
	if (ptLevel->nLevelId == 100)
		sPlaceWarpRoom(ptLevel, &sgcatMazeWarpsB[7][nDirection], NULL);
}

static void __fastcall sConnectBarracks(DRLGLEVEL *ptLevel)
{
	DRLGLEVEL *ptCourt;
	PTLVLMAZETXT ptMazeDef;
	ROOM *ptRoom;
	ROOM *ptNew;
	int nFile;
	int nX;
	int nY;
	int nMinX;
	int nMinY;
	int nMaxX;
	int nMaxY;

	ptCourt = DrlgGetLevel(ptLevel->ptAct, LEVEL_ACT_1_COURTYARD_1);
	nFile = ptCourt->field_38.ptPreset->nFile;
	ptRoom = scpfnBarracksEdgeRoom[nFile](ptLevel);
	ptMazeDef = ptLevel->field_38.ptMazeDef;
	nX = ptCourt->tCoords.nTileX;
	nY = ptCourt->tCoords.nTileY;
	switch (nFile)
	{
	case 0:
		ptNew = sAddRoomWithPreset(ptRoom, 2, 167, 0, TRUE);
		DrlgLevelLinkNew((PTDRLGLEVELLINK *)&ptNew->ptLinks, ptCourt, 2, 0);
		nX -= ptNew->tCoords.nTileX + ptMazeDef->SizeX;
		nY += ptCourt->tCoords.nSizeTileY / 2 - ptNew->tCoords.nTileY;
		break;
	case 1:
		ptNew = sAddRoomWithPreset(ptRoom, 3, 167, 1, TRUE);
		DrlgLevelLinkNew((PTDRLGLEVELLINK *)&ptNew->ptLinks, ptCourt, 3, 0);
		nX += ptCourt->tCoords.nSizeTileX / 2 - ptNew->tCoords.nTileX - 6;
		nY -= ptNew->tCoords.nTileY + ptMazeDef->SizeY;
		break;
	case 2:
		ptNew = sAddRoomWithPreset(ptRoom, 0, 167, 2, TRUE);
		DrlgLevelLinkNew((PTDRLGLEVELLINK *)&ptNew->ptLinks, ptCourt, 0, 0);
		nX += ptCourt->tCoords.nSizeTileX - ptNew->tCoords.nTileX;
		nY += ptCourt->tCoords.nSizeTileY / 2 - ptNew->tCoords.nTileY + 1;
		break;
	}
	if (sSeedBits(&ptLevel->tSeed, 1))
	{
		sPlaceWarpRoom(ptLevel, &sgcatBarracksWarps[0][nFile], &nFile);
		sPlaceWarpRoom(ptLevel, &sgcatBarracksWarps[1][nFile], NULL);
	}
	else
	{
		sPlaceWarpRoom(ptLevel, &sgcatBarracksWarps[1][nFile], &nFile);
		sPlaceWarpRoom(ptLevel, &sgcatBarracksWarps[0][nFile], NULL);
	}
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		ptRoom->tCoords.nTileX += nX;
		ptRoom->tCoords.nTileY += nY;
	}
	nMinX = 0;
	nMinY = 0;
	nMaxX = 0;
	nMaxY = 0;
	DrlgLevelGetRoomBounds(ptLevel, &nMinX, &nMinY, &nMaxX, &nMaxY);
	ptLevel->tCoords.nTileX = nMinX;
	ptLevel->tCoords.nTileY = nMinY;
	ptLevel->tCoords.nSizeTileX = nMaxX - nMinX;
	ptLevel->tCoords.nSizeTileY = nMaxY - nMinY;
}

static void __fastcall sSetRoomMazeSize(ROOM *ptRoom)
{
	PTLVLMAZETXT ptMazeDef;

	ptMazeDef = ptRoom->ptLevel->field_38.ptMazeDef;
	ptRoom->tCoords.nSizeTileX = ptMazeDef->SizeX;
	ptRoom->tCoords.nSizeTileY = ptMazeDef->SizeY;
}

static void __fastcall sPlaceRiverOfFlame(DRLGLEVEL *ptLevel)
{
	DRLGLEVEL *ptSanctum;
	void *pPool;
	ROOM *ptRoom;
	ROOM *ptNew;
	ROOM **pptRooms;
	int nRooms;
	int nX;
	int nY;
	int i;
	int j;
	int nMinX;
	int nMinY;
	int nMaxX;
	int nMaxY;

	ptSanctum = DrlgGetLevel(ptLevel->ptAct, LEVEL_ACT_4_DIABLO_1);
	pPool = ptLevel->ptAct->pMemPool;
	sAddRoomWithPreset(sFindSouthRoom(ptLevel), 3, 852, -1, TRUE);
	ptNew = sAddRoomWithPreset(sFindNorthRoom(ptLevel), 1, 855, -1, TRUE);
	ptNew = sAddRoomWithPreset(ptNew, 1, 856, -1, FALSE);
	ptNew = sAddRoomWithPreset(ptNew, 1, 856, -1, FALSE);
	DrlgLevelLinkNew((PTDRLGLEVELLINK *)&ptNew->ptLinks, ptSanctum, 1, 0);
	nX = ptNew->tCoords.nSizeTileX * 2 - ptNew->tCoords.nTileX + ptSanctum->tCoords.nTileX;
	nY = ptSanctum->tCoords.nSizeTileY - ptNew->tCoords.nTileY + ptSanctum->tCoords.nTileY;
	sPlaceWarpRoom(ptLevel, &sgcatRiverWarps[sSeedBits(&ptLevel->tSeed, 1)], NULL);
#line 1493
	pptRooms = (ROOM **)D2ALLOCPOOL(pPool, ptLevel->nNumRooms * sizeof(ROOM *));
	nRooms = ptLevel->nNumRooms;
	ptRoom = ptLevel->ptRoomFirst;
	for (i = 0; i < nRooms; i++)
	{
		pptRooms[i] = ptRoom;
		ptRoom = ptRoom->ptLevelNext;
	}
	for (i = 0; i < nRooms; i++)
	{
		if (pptRooms[i] == ptNew)
			continue;
		for (j = 0; j < 8; j++)
			sAddRoomWithPreset(pptRooms[i], j, 836, -1, FALSE);
	}
#line 1516
	D2FREEPOOL(pPool, pptRooms);
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		ptRoom->tCoords.nTileX += nX;
		ptRoom->tCoords.nTileY += nY;
	}
	nMinX = 0;
	nMinY = 0;
	nMaxX = 0;
	nMaxY = 0;
	DrlgLevelGetRoomBounds(ptLevel, &nMinX, &nMinY, &nMaxX, &nMaxY);
	ptLevel->tCoords.nTileX = nMinX;
	ptLevel->tCoords.nTileY = nMinY;
	ptLevel->tCoords.nSizeTileX = nMaxX - nMinX;
	ptLevel->tCoords.nSizeTileY = nMaxY - nMinY;
}

static void __fastcall sPlaceThemeRooms(DRLGLEVEL *ptLevel)
{
	int nBase;
	int nPick;
	int nCount;
	int nTries;
	int i;
	int a;
	int b;
	int t;

	switch (ptLevel->dwLevelType)
	{
	case 3:
		nBase = 52;
		break;
	case 4:
		nBase = 108;
		break;
	case 7:
		nBase = 167;
		break;
	case 8:
		nBase = 205;
		break;
	case 10:
		nBase = 257;
		break;
	case 13:
		nBase = 301;
		break;
	case 17:
		nBase = 413;
		break;
	case 22:
		nBase = 753;
		break;
	case 24:
		nBase = 664;
		break;
	case 25:
		nBase = 704;
		break;
	default:
		return;
	}
	if (ptLevel->nLevelId == 8)
		return;
	int anOrder[15] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	nPick = (int)SeedRand(&ptLevel->tSeed, 15);
	nCount = max(ptLevel->nNumRooms / 5 + 1, 2);
	nTries = ptLevel->nNumRooms * 2;
	for (i = 15; i; i--)
	{
		a = (int)SeedRand(&ptLevel->tSeed, 15);
		b = (int)SeedRand(&ptLevel->tSeed, 15);
		t = anOrder[a];
		anOrder[a] = anOrder[b];
		anOrder[b] = t;
	}
	while (nCount && nTries)
	{
		if (sReplaceRoomPreset(ptLevel, anOrder[nPick] + nBase, anOrder[nPick] + nBase + 15, -1, FALSE))
			nCount--;
		nTries--;
		nPick = (nPick + 1) % 15;
	}
}

static void __fastcall sRoomsToPresets(DRLGLEVEL *ptLevel)
{
	ROOM *ptRoom;
	ROOM *ptNext;
	ROOM *ptNew;
	PTPRESETINFO ptInfo;
	PTPRESETSUBINFO ptRegion;
	void *pPool;
	int nDef;
	int nFiles;
	int nBase;
	PTDRLGFILECYCLE ptCycle;
	PTDRLGROOMLINK ptLink;
	DRLGCOORDS *ptCoords;

	ptNext = ptLevel->ptRoomFirst;
	while (ptNext)
	{
		ptRoom = ptNext;
		ptNext = ptRoom->ptLevelNext;
		ptInfo = ptRoom->field_20.ptPreset;
		ptCoords = &ptRoom->tCoords;
		ptRegion = PresetRegionNew(ptLevel, (int)ptInfo->dwDef, ptCoords, &ptLevel->tSeed);
		pPool = ptLevel->ptAct->pMemPool;
		if (ptInfo->nIndex != -1)
			ptRegion->nIndex = (DWORD)ptInfo->nIndex;
		else
		{
			nDef = ptRegion->pRecord->Def;
			nFiles = ptRegion->pRecord->Files;
			switch (ptLevel->dwLevelType)
			{
			case 3:
				nBase = 52;
				break;
			case 4:
				nBase = 108;
				break;
			case 7:
				nBase = 167;
				break;
			case 8:
				nBase = 205;
				break;
			case 10:
				nBase = 257;
				break;
			case 13:
				nBase = 301;
				break;
			case 17:
				nBase = 413;
				break;
			case 18:
				nBase = 481;
				break;
			case 22:
				nBase = 753;
				break;
			case 24:
				nBase = 664;
				break;
			case 25:
				nBase = 704;
				break;
			case 28:
				nBase = 836;
				break;
			default:
				goto rooms;
			}
			if (nDef > nBase && nDef < nBase + 16)
			{
				for (ptCycle = ptLevel->ptFileCycleFirst; ptCycle; ptCycle = ptCycle->ptNext)
				{
					if (ptCycle->nDef == nDef)
						break;
				}
				if (!ptCycle)
				{
#line 1671
					ptCycle = (PTDRLGFILECYCLE)D2ALLOCPOOL(pPool, sizeof(DRLGFILECYCLE));
					memset(ptCycle, 0, sizeof(DRLGFILECYCLE));
					ptCycle->nDef = nDef;
					ptCycle->nFiles = nFiles;
					ptCycle->nLastFile = nFiles ? (int)SeedRand(&ptLevel->tSeed, (DWORD)nFiles) : 0;
					ptCycle->ptNext = ptLevel->ptFileCycleFirst;
					ptLevel->ptFileCycleFirst = ptCycle;
				}
				ptCycle->nLastFile = (ptCycle->nLastFile + 1) % ptCycle->nFiles;
				ptRegion->nIndex = (DWORD)ptCycle->nLastFile;
			}
		}
rooms:
		ptNew = PresetRegionToRooms(ptLevel, ptRegion, 0,
			ptCoords->nSizeTileX <= 12 && ptCoords->nSizeTileY <= 12);
		for (ptLink = ptRoom->ptLinks; ptLink; ptLink = ptLink->ptNext)
		{
			if (ptLink->nKind == 1)
				DrlgRoomLinkRooms(ptNew, ptLink->ptOtherRoom, ptLink->nDirection);
		}
		DrlgRoomFree(ptRoom);
	}
}

void __fastcall MazeLevelFree(DRLGLEVEL *ptLevel, BOOL fKeep)
{
	if (!fKeep)
		ptLevel->field_38.ptMazeDef = NULL;
}
