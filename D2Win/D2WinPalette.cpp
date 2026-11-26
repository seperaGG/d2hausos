#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinPalette.cpp"
#include <stdio.h>
#include "D2Win.h"
#include "D2Macros.h"
#include "Archive.h"

HD2ARCHIVE __fastcall D2WinArchiveGetArchive(void);
void __fastcall D2CompSetEmblemColors(void);

#pragma pack(push, 1)
struct D2WINPL2FILE
{
 	PALETTEENTRY	atPalette[256];
 	BYTE		aabColorMaps[49][256];
 	BYTE		aabBlendModes[3][65536];
 	BYTE		abBlendMode3[65536];
 	BYTE		abBlendMode4[65536];
 	BYTE		aabHueRamps[128][256];
 	BYTE		abBlendMode6[65536];
 	BYTE		field_6b500[256];
 	BYTE		field_6b600[39];
 	BYTE		aabPreMaps[13][256];
};
#pragma pack(pop)

struct D2WINPALETTETABLES
{
 	BYTE		*apbBlendModes[3];
 	BYTE		*apbColorMaps[49];
 	BYTE		*apbPreMaps[13];
 	BYTE		*pbBlendMode3;
 	BYTE		*pbBlendMode4;
 	BYTE		*pbBlendMode6;
 	BYTE		*field_110;
 	BYTE		*field_114;
 	BYTE		*pbBrightColorMap;
 	BYTE		*pbRedColorMap;
};

static const char *const sgpszDefaultDat = "palette\\act1\\pal.dat";
static const char *const sgpszDefaultPl2 = "palette\\act1\\pal.pl2";
static const char *const sgpszActFormat = "%s\\palette\\act%d\\%s";
static const char *const sgpszDatName = "pal.dat";
static const char *const sgpszPl2Name = "pal.pl2";

static BOOL sgfRefresh = TRUE;

static BYTE sgabTable33500[65536] = { 0 };
static int sgnRefreshCount = 0;
static BYTE sgabTable5b500[65536] = { 0 };
static BYTE sgaabTable00400[49][256] = { 0 };
static BYTE sgabTable6b500[256] = { 0 };
static BYTE sgaabTable6b627[13][256] = { 0 };
static BYTE sgaabTable03500[3][65536] = { 0 };
static BYTE sgabRedIndex[256] = { 0 };
static BYTE sgabTable43500[65536] = { 0 };
static BYTE sgabTable6b600[39] = { 0 };
static PALETTEENTRY sgatPalette[256] = { 0 };
static BYTE sgaabTable53500[128][256] = { 0 };
static BYTE sgabBrightIndex[256] = { 0 };
static BOOL sgfRefreshTimed = 0;
static DWORD sgdwRefreshTick = 0;

static BOOL __fastcall sPaletteLoad(const char *pszDat, const char *pszPl2);

BOOL __fastcall D2WinPaletteSetAct(BYTE bAct)
{
	char szDat[MAX_PATH];
	char szPl2[MAX_PATH];
	int nAct;
	BOOL fResult;

	nAct = bAct + 1;
	if (nAct < 1 || nAct > 4)
		nAct = 1;
	sprintf(szDat, sgpszActFormat, "DATA\\GLOBAL", nAct, sgpszDatName);
	sprintf(szPl2, sgpszActFormat, "DATA\\GLOBAL", nAct, sgpszPl2Name);
	fResult = sPaletteLoad(szDat, szPl2);
	D2gfxScaleSetPalette(sgatPalette);
	return fResult;
}

static inline int sPaletteBrighten(int nPart)
{
	return nPart * 170 / 100 > 255 ? 255 : nPart * 170 / 100;
}

static BOOL __fastcall sPaletteLoad(const char *pszDat, const char *pszPl2)
{
	D2WINPALETTETABLES tTables;
	PALETTEENTRY atDatPalette[256];
	HD2ARCHIVE hArchive;
	BYTE *pbDat;
	BYTE *pbSrc;
	D2WINPL2FILE *ptPl2;
	int nRed, nGreen, nBlue;
	BYTE bRed;
	int i;

#line 99
	pbDat = (BYTE *)ArchiveAllocBufferAndRead(D2WinArchiveGetArchive(), pszDat, NULL, D2_THIS_FILE, __LINE__);
	pbSrc = pbDat;
	for (i = 0; i < 256; i++)
	{
		atDatPalette[i].peBlue = *pbSrc++;
		atDatPalette[i].peGreen = *pbSrc++;
		atDatPalette[i].peRed = *pbSrc++;
		atDatPalette[i].peFlags = 0;
	}
	(void)atDatPalette;
#line 114
	ptPl2 = (D2WINPL2FILE *)ArchiveAllocBufferAndRead(D2WinArchiveGetArchive(), pszPl2, NULL, D2_THIS_FILE, __LINE__);
	memcpy(sgatPalette, ptPl2->atPalette, sizeof(ptPl2->atPalette));
	memcpy(sgaabTable03500, ptPl2->aabBlendModes, sizeof(sgaabTable03500));
	memcpy(sgaabTable00400, ptPl2->aabColorMaps, sizeof(sgaabTable00400));
	memcpy(sgabTable43500, ptPl2->abBlendMode4, sizeof(sgabTable43500));
	memcpy(sgabTable33500, ptPl2->abBlendMode3, sizeof(sgabTable33500));
	memcpy(sgaabTable00400[32], ptPl2->aabColorMaps[32], 16 * 256);
	memcpy(sgaabTable00400[48], ptPl2->aabColorMaps[48], 256);
	memcpy(sgaabTable53500, ptPl2->aabHueRamps, sizeof(sgaabTable53500));
	memcpy(sgabTable5b500, ptPl2->abBlendMode6, sizeof(sgabTable5b500));
	memcpy(sgabTable6b500, ptPl2->field_6b500, sizeof(sgabTable6b500));
	memcpy(sgabTable6b600, ptPl2->field_6b600, sizeof(sgabTable6b600));
	memcpy(sgaabTable6b627, ptPl2->aabPreMaps, sizeof(sgaabTable6b627));
	for (i = 0; i < 256; i++)
		sgatPalette[i].peFlags = 5;
	for (i = 0; i < 256; i++)
	{
		nRed = sPaletteBrighten(sgatPalette[i].peRed);
		nGreen = sPaletteBrighten(sgatPalette[i].peGreen);
		nBlue = sPaletteBrighten(sgatPalette[i].peBlue);
		sgabBrightIndex[i] = (BYTE)D2CMPPaletteFindNearest(sgatPalette, 256, nRed & 255,
			nGreen & 255, nBlue & 255);
		bRed = sgatPalette[i].peRed;
		sgabRedIndex[i] = (BYTE)D2CMPPaletteFindNearest(sgatPalette, 256, bRed, 0, 0);
	}
	for (i = 0; i < 3; i++)
		tTables.apbBlendModes[i] = sgaabTable03500[i];
	for (i = 0; i < 49; i++)
		tTables.apbColorMaps[i] = sgaabTable00400[i];
	for (i = 0; i < 13; i++)
		tTables.apbPreMaps[i] = sgaabTable6b627[i];
	tTables.pbBlendMode3 = sgabTable33500;
	tTables.pbBlendMode4 = sgabTable43500;
	tTables.pbBlendMode6 = sgabTable5b500;
	tTables.field_110 = sgabTable6b500;
	tTables.field_114 = sgabTable6b600;
	tTables.pbBrightColorMap = sgabBrightIndex;
	tTables.pbRedColorMap = sgabRedIndex;
	D2gfxScaleSetEnvInfo(&tTables);
	D2gfxScaleSetPalette(sgatPalette);
#line 150
	D2FREE2(pbDat);
#line 153
	D2FREE2(ptPl2);
	return TRUE;
}

BYTE * __fastcall D2WinPaletteGetHueRamp(DWORD dwRow)
{
	return (BYTE *)sgaabTable53500 + (dwRow << 8);
}

void __fastcall D2WinPaletteUpdate(void)
{
	if (!sgfRefresh)
		return;
	if (sgfRefreshTimed)
	{
		if (GetTickCount() - sgdwRefreshTick <= 2000)
			return;
		sgdwRefreshTick = GetTickCount();
		sgnRefreshCount--;
		if (sgnRefreshCount <= 0)
		{
			sgfRefresh = FALSE;
			sgfRefreshTimed = FALSE;
		}
	}
	else
		sgfRefresh = FALSE;
	D2gfxScaleSetPalette(sgatPalette);
}

DWORD __fastcall D2WinPaletteRefresh(BOOL fTimed)
{
	if (fTimed)
	{
		sgfRefreshTimed = TRUE;
		sgdwRefreshTick = GetTickCount();
		sgnRefreshCount = 2;
		sgfRefresh = TRUE;
		return D2gfxScaleSetPalette(sgatPalette);
	}
	sgfRefreshTimed = FALSE;
	sgfRefresh = FALSE;
	return D2gfxScaleSetPalette(sgatPalette);
}

BOOL __stdcall D2WinPaletteLoad(const char *pszDat, const char *pszPl2)
{
	char szDat[MAX_PATH];
	char szPl2[MAX_PATH];

	if (!pszDat)
	{
		wsprintfA(szDat, "%s\\%s", "DATA\\GLOBAL", sgpszDefaultDat);
		wsprintfA(szPl2, "%s\\%s", "DATA\\GLOBAL", sgpszDefaultPl2);
		sPaletteLoad(szDat, szPl2);
	}
	else
		sPaletteLoad(pszDat, pszPl2);
	D2CompSetEmblemColors();
	return TRUE;
}

DWORD __stdcall D2WinPaletteGetColor(BYTE dwRed, BYTE dwGreen, BYTE dwBlue)
{
	return D2CMPPaletteFindNearest(sgatPalette, 256, dwRed, dwGreen, dwBlue);
}

DWORD __fastcall D2WinPaletteApply(void)
{
	return D2gfxScaleSetPalette(sgatPalette);
}

void __fastcall D2WinPaletteCopyPL2Tables(BYTE *pbBuffer)
{
	memcpy(pbBuffer, sgaabTable00400, sizeof(sgaabTable00400));
}
