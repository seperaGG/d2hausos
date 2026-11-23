#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutDesr.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

static void __fastcall sPlaceDefs(DRLGLEVEL *ptLevel, int *pnDefs, int nCount,
	BOOL fAllFiles);
static void __fastcall sStampRandomLayout(DRLGLEVEL *ptLevel);
static void __fastcall sRunLvlSubPasses(DRLGLEVEL *ptLevel);
static void __fastcall sAddTheExitsAndStuff(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceOasisDefs(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceCityRuins(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceCityDefs(DRLGLEVEL *ptLevel);
static void __fastcall sStampCanyonLayout(DRLGLEVEL *ptLevel);
static void __fastcall sPlaceCanyonDefs(DRLGLEVEL *ptLevel);

struct OUTDESRSTAMP
{
 	int			nDef;
 	int			nFile;
 	int			nX;
 	int			nY;
};
static const struct OUTDESRSTAMP sgctDesertLayouts[8][5] =
{
	{ { 376, 1, 0, 4 }, { 378, -1, 2, 4 }, { 377, -1, 4, 4 }, { 377, -1, 6, 4 }, { 376, 2, 8, 4 } },
	{ { 376, 1, 0, 4 }, { 377, -1, 2, 4 }, { 378, -1, 4, 4 }, { 377, -1, 6, 4 }, { 376, 2, 8, 4 } },
	{ { 376, 1, 0, 4 }, { 377, -1, 2, 4 }, { 377, -1, 4, 4 }, { 378, -1, 6, 4 }, { 376, 2, 8, 4 } },
	{ { 376, 2, 8, 4 }, { 377, -1, 6, 4 }, { 382, -1, 4, 4 }, { 381, -1, 4, 6 }, { 379, 2, 4, 8 } },
	{ { 376, 2, 8, 4 }, { 378, -1, 6, 4 }, { 382, -1, 4, 4 }, { 380, -1, 4, 6 }, { 379, 2, 4, 8 } },
	{ { 379, 1, 4, 0 }, { 381, -1, 4, 2 }, { 380, -1, 4, 4 }, { 380, -1, 4, 6 }, { 379, 2, 4, 8 } },
	{ { 379, 1, 4, 0 }, { 380, -1, 4, 2 }, { 381, -1, 4, 4 }, { 380, -1, 4, 6 }, { 379, 2, 4, 8 } },
	{ { 379, 1, 4, 0 }, { 380, -1, 4, 2 }, { 380, -1, 4, 4 }, { 381, -1, 4, 6 }, { 379, 2, 4, 8 } },
};

static const struct OUTDESRSTAMP sgctCanyonLayout[9] =
{
	{ 384, 0, 8, 0 }, { 383, 2, 6, 0 }, { 383, 1, 4, 0 }, { 383, 0, 2, 0 }, { 387, 0, 0, 0 },
	{ 385, 0, 0, 2 }, { 385, 1, 0, 4 }, { 385, 2, 0, 6 }, { 386, 0, 0, 8 },
};

static void __fastcall sStampTownSide(DRLGLEVEL *ptLevel)
{
	PTOUTDOORINFO ptOutdoor;
	DRLGLEVELLINK *ptLink;

	ptOutdoor = ptLevel->field_38.pOutdoor;
	for (ptLink = ptOutdoor->ptLevelLinks; ptLink; ptLink = ptLink->ptNext)
	{
		if (ptLink->ptLevel->nLevelId == 40)
		{
			if (ptLink->nDirection == 3)
				OutdoorsStampPreset(ptLevel, 0, ptOutdoor->tCellRect.nSizeTileY - 1, 363, -1, FALSE);
			else
				OutdoorsStampPreset(ptLevel, ptOutdoor->tCellRect.nSizeTileX - 1, 0, 362, -1, FALSE);
			break;
		}
	}
}

static void __fastcall sPlaceRockyDefs(DRLGLEVEL *ptLevel)
{
	int anDefs[7];

	anDefs[0] = 395;
	anDefs[1] = 411;
	anDefs[2] = 401;
	anDefs[3] = 402;
	anDefs[4] = 399;
	anDefs[5] = 398;
	anDefs[6] = 403;
	sPlaceDefs(ptLevel, anDefs, 7, FALSE);
}

static void __fastcall sPlaceHillsDefs(DRLGLEVEL *ptLevel)
{
	int anDefs[4];
	int anBerms[4];

	anDefs[0] = 395;
	anDefs[1] = 411;
	anDefs[2] = 400;
	anDefs[3] = 398;
	anBerms[0] = 404;
	anBerms[1] = 405;
	anBerms[2] = 406;
	anBerms[3] = 407;
	sPlaceDefs(ptLevel, anDefs, 4, FALSE);
	sPlaceDefs(ptLevel, anBerms, 4, TRUE);
}

static void __fastcall sPlaceOases(DRLGLEVEL *ptLevel)
{
	int anDefs[2];

	anDefs[0] = 396;
	anDefs[1] = 397;
	sPlaceDefs(ptLevel, anDefs, 2, FALSE);
}

void __fastcall OutDesrGenerate(DRLGLEVEL *ptLevel)
{
	OutdoorsMarkOutline(ptLevel);
	OutdoorsMarkInterior(ptLevel);
	switch (ptLevel->nLevelId)
	{
	case 41:
		sStampTownSide(ptLevel);
		sRunLvlSubPasses(ptLevel);
		sAddTheExitsAndStuff(ptLevel);
		OutdoorsPlaceShrines(ptLevel, 5);
		sPlaceRockyDefs(ptLevel);
		break;
	case 42:
		sStampRandomLayout(ptLevel);
		sRunLvlSubPasses(ptLevel);
		sAddTheExitsAndStuff(ptLevel);
		OutdoorsPlaceWaypoint(ptLevel);
		OutdoorsPlaceShrines(ptLevel, 5);
		sPlaceHillsDefs(ptLevel);
		break;
	case 43:
		sStampRandomLayout(ptLevel);
		sRunLvlSubPasses(ptLevel);
		sAddTheExitsAndStuff(ptLevel);
		sPlaceOases(ptLevel);
		OutdoorsPlaceWaypoint(ptLevel);
		OutdoorsPlaceShrines(ptLevel, 5);
		sPlaceOasisDefs(ptLevel);
		break;
	case 44:
		sStampRandomLayout(ptLevel);
		sRunLvlSubPasses(ptLevel);
		sAddTheExitsAndStuff(ptLevel);
		sPlaceCityRuins(ptLevel);
		OutdoorsPlaceWaypoint(ptLevel);
		OutdoorsPlaceShrines(ptLevel, 5);
		sPlaceCityDefs(ptLevel);
		break;
	case 45:
		sAddTheExitsAndStuff(ptLevel);
		break;
	case 46:
		sStampCanyonLayout(ptLevel);
		sRunLvlSubPasses(ptLevel);
		OutdoorsPlaceShrines(ptLevel, 5);
		sPlaceCanyonDefs(ptLevel);
		break;
	}
}

static void __fastcall sPlaceDefs(DRLGLEVEL *ptLevel, int *pnDefs, int nCount, BOOL fAllFiles)
{
	int nIndex;
	int nFiles;
	int nFile;
	int n;

	if (nCount)
		nIndex = (int)SeedRand(&ptLevel->tSeed, nCount);
	else
		nIndex = 0;
	D2CommonLvlTblsGetPrest(pnDefs[nIndex]);
	for (n = nCount; n > 0; n--)
	{
		if (fAllFiles)
		{
			nFiles = ((PTLVLPRESTTXT)D2CommonLvlTblsGetPrest(pnDefs[nIndex]))->Files;
			for (nFile = 0; nFile < nFiles; nFile++)
				OutdoorsPlacePreset(ptLevel, pnDefs[nIndex], nFile, 0, 15);
		}
		else
			OutdoorsPlacePreset(ptLevel, pnDefs[nIndex], -1, 0, 15);
		nIndex = (nIndex + 1) % nCount;
	}
}

static void __fastcall sStampRandomLayout(DRLGLEVEL *ptLevel)
{
	const int *pnY;
	int i;

	pnY = &sgctDesertLayouts[SeedRand(&ptLevel->tSeed, 8)][0].nY;
	for (i = 0; i < 5; i++, pnY += 4)
		OutdoorsStampPreset(ptLevel, pnY[-1], pnY[0], pnY[-3],
			pnY[-2],
			FALSE);
}

static void __fastcall sRunLvlSubPasses(DRLGLEVEL *ptLevel)
{
	OutdoorsRunLvlSubPass(ptLevel, 2, 364);
	OutdoorsRunLvlSubPass(ptLevel, 1, 364);
	OutdoorsRunLvlSubPass(ptLevel, 3, 364);
}

static void __fastcall sAddTheExitsAndStuff(DRLGLEVEL *ptLevel)
{
	BOOL fAdded;

	switch (ptLevel->nLevelId)
	{
	case 41:
	case 42:
		fAdded = OutdoorsPlacePreset(ptLevel, 388, -1, 0, 15);
		break;
	case 43:
		fAdded = OutdoorsPlacePreset(ptLevel, 390, -1, 0, 15);
		break;
	case 44:
		fAdded = OutdoorsPlacePreset(ptLevel, 412, -1, 0, 15);
		break;
	case 45:
		fAdded = OutdoorsPlacePreset(ptLevel, 389, -1, 0, 15);
		break;
	default:
		fAdded = FALSE;
		break;
	}
#line 169
	D2ASSERTMSG(fAdded, "sAddTheExitsAndStuff() - Could not add the exit!");
}

static void __fastcall sPlaceOasisDefs(DRLGLEVEL *ptLevel)
{
	int anDefs[5];

	anDefs[1] = 411;
	anDefs[2] = 399;
	anDefs[3] = 398;
	anDefs[4] = 403;
	anDefs[0] = 395;
	sPlaceDefs(ptLevel, &anDefs[1], 4, FALSE);
	sPlaceDefs(ptLevel, &anDefs[0], 1, TRUE);
	sPlaceDefs(ptLevel, &anDefs[0], 1, TRUE);
}

static void __fastcall sPlaceCityRuins(DRLGLEVEL *ptLevel)
{
	int anDefs[4];

	anDefs[0] = 413;
	anDefs[1] = 408;
	anDefs[2] = 409;
	anDefs[3] = 410;
	sPlaceDefs(ptLevel, anDefs, 4, FALSE);
}

static void __fastcall sPlaceCityDefs(DRLGLEVEL *ptLevel)
{
	int anDefs[6];

	anDefs[1] = 395;
	anDefs[2] = 400;
	anDefs[3] = 398;
	anDefs[4] = 404;
	anDefs[5] = 405;
	anDefs[0] = 411;
	sPlaceDefs(ptLevel, &anDefs[1], 5, FALSE);
	sPlaceDefs(ptLevel, &anDefs[0], 1, TRUE);
	sPlaceDefs(ptLevel, &anDefs[0], 1, TRUE);
}

static void __fastcall sStampCanyonLayout(DRLGLEVEL *ptLevel)
{
	const struct OUTDESRSTAMP *ptStamp;

	for (ptStamp = sgctCanyonLayout; (int)&ptStamp->nY < (int)&sgctCanyonLayout[9].nY; ptStamp++)
		OutdoorsStampPreset(ptLevel, ptStamp->nX, ptStamp->nY, ptStamp->nDef,
			ptStamp->nFile,
			FALSE);
	OutdoorsStampPreset(ptLevel, 4, 4, 394, -1, FALSE);
}

static void __fastcall sPlaceCanyonDefs(DRLGLEVEL *ptLevel)
{
	int anDefs[7];

	anDefs[2] = 401;
	anDefs[3] = 402;
	anDefs[4] = 406;
	anDefs[5] = 407;
	anDefs[6] = 403;
	anDefs[0] = 392;
	anDefs[1] = 393;
	sPlaceDefs(ptLevel, &anDefs[2], 5, FALSE);
	sPlaceDefs(ptLevel, &anDefs[0], 2, TRUE);
}
