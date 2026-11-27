#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\Palette.cpp"
#include <math.h>
#include "D2Cmp.h"

#include "D2Macros.h"
#line 11

#define NUMBER_PALETTE_ENTRIES	256

static int sgnSquares[512];
static int sgfMatchKeyColor;
static DWORD sgnPaletteEnd;
static DWORD sgnPaletteStart;
static BYTE sgabPaletteFile[768];
static BYTE sgabUsable[NUMBER_PALETTE_ENTRIES];
static BYTE sgabPalette[NUMBER_PALETTE_ENTRIES][4];
static BOOL sgfPaletteLoaded;
static DWORD sgdwKeyColor;
static BOOL sgfDarkToBlack;
static BOOL sgfUseMask;

#define PALETTE_ADDTABLE	210176

static void __fastcall PaletteBuildAddTable(BYTE *pbPalette);

#define PALETTE_TABLESIZE	443175

struct D2HSL
{
	double		dHue;
	double		dSat;
	double		dLum;
};

static void __fastcall PaletteBuildLightenTables(BYTE *pbTables);
static void __fastcall PaletteBuildProductTable(BYTE *pbTables);
static void __fastcall PaletteBuildHueTables(BYTE *pbTables, const D2HSL *ptHsl);
static void __fastcall PaletteBuildLightTable(BYTE *pbTables, D2HSL *ptHsl);
static void __fastcall PaletteBuildDarkTables(BYTE *pbTables);
static void __fastcall PaletteBuildTintTables(BYTE *pbTables);
static void __fastcall PaletteHslToRgb(const D2HSL *ptHsl, BYTE *pbRgb);
BYTE WINAPI PaletteFindNearest(const BYTE *pbPalette, DWORD dwEntries, int nColor0, int nColor1,
	int nColor2);

static DWORD sgadwPaletteRamps[5][256] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 5, 5, 5, 6, 6,
		6, 7, 7, 7, 8, 8, 9, 10,
		10, 11, 12, 12, 13, 14, 15, 16,
		18, 19, 21, 23, 24, 26, 28, 29,
		31, 32, 34, 36, 37, 39, 41, 43,
		46, 49, 51, 54, 57, 59, 62, 65,
		67, 70, 73, 76, 79, 82, 86, 89,
		92, 96, 99, 102, 106, 109, 112, 116,
		118, 120, 122, 124, 126, 129, 131, 133,
		135, 137, 140, 142, 144, 146, 148, 151,
		151, 152, 153, 154, 155, 156, 157, 158,
		158, 159, 160, 161, 162, 163, 164, 165,
		165, 166, 167, 168, 169, 170, 171, 172,
		172, 172, 173, 173, 174, 174, 175, 175,
		176, 176, 176, 177, 177, 178, 178, 179,
		179, 180, 180, 181, 181, 181, 181, 182,
		182, 182, 183, 183, 183, 184, 184, 184,
		185, 185, 185, 186, 186, 186, 186, 187,
		187, 187, 188, 188, 188, 189, 189, 189,
		190, 190, 190, 191, 191, 191, 191, 192,
		192, 192, 192, 193, 193, 193, 193, 194,
		194, 194, 194, 195, 195, 195, 195, 195,
		195, 195, 195, 196, 196, 196, 196, 196,
		196, 196, 196, 197, 197, 197, 197, 197,
		197, 197, 197, 197, 197, 197, 197, 197,
		197, 197, 197, 197, 197, 197, 197, 197
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 2, 2, 2,
		3, 3, 3, 4, 4, 4, 5, 5,
		5, 6, 6, 7, 8, 8, 9, 10,
		10, 11, 12, 13, 13, 14, 15, 15,
		16, 17, 17, 18, 19, 19, 21, 22,
		24, 25, 26, 28, 29, 30, 32, 33,
		34, 36, 37, 38, 40, 41, 43, 44,
		45, 47, 48, 50, 51, 53, 54, 55,
		57, 58, 60, 61, 63, 64, 65, 66,
		67, 68, 69, 70, 71, 73, 74, 75,
		76, 77, 78, 79, 80, 82, 83, 84,
		85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 99,
		100, 101, 102, 103, 104, 104, 105, 106,
		107, 108, 108, 109, 110, 111, 112, 112,
		113, 113, 114, 114, 115, 116, 116, 117,
		117, 118, 119, 119, 120, 120, 121, 122,
		122, 122, 123, 123, 123, 124, 124, 124,
		125, 125, 125, 126, 126, 127, 127, 127,
		127, 127, 127, 127, 128, 128, 128, 128,
		128, 128, 128, 129, 129, 129, 129, 129,
		129, 129, 129, 129, 129, 129, 129, 129,
		129, 129, 129, 129, 129, 129, 129, 129,
		129, 129, 129, 129, 129, 129, 129, 129,
		129, 129, 129, 129, 129, 129, 129, 129
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 2, 2, 2, 2, 3, 3, 3,
		4, 4, 4, 4, 5, 5, 5, 6,
		6, 6, 6, 7, 7, 7, 8, 8,
		8, 9, 9, 9, 10, 10, 11, 11,
		11, 12, 12, 13, 13, 13, 14, 14,
		15, 15, 15, 16, 16, 17, 17, 17,
		18, 18, 19, 19, 19, 20, 20, 21,
		21, 21, 22, 22, 22, 23, 23, 24,
		24, 24, 25, 25, 26, 26, 27, 27,
		28, 28, 29, 29, 30, 30, 31, 31,
		32, 32, 33, 33, 34, 34, 35, 36,
		36, 37, 38, 38, 39, 40, 40, 41,
		42, 42, 43, 44, 44, 45, 46, 46,
		47, 47, 48, 48, 49, 49, 50, 50,
		51, 51, 52, 52, 53, 53, 53, 54,
		54, 55, 55, 55, 56, 56, 57, 57,
		57, 58, 58, 59, 59, 60, 60, 60,
		60, 60, 60, 60, 61, 61, 61, 61,
		61, 61, 62, 62, 62, 62, 62, 62,
		62, 62, 62, 62, 62, 62, 62, 62,
		62, 62, 62, 62, 62, 63, 63, 63,
		63, 63, 63, 63, 63, 63, 63, 63,
		63, 63, 63, 63, 63, 63, 63, 63
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 2, 2, 2, 3,
		3, 3, 3, 4, 4, 4, 4, 5,
		5, 5, 6, 6, 7, 7, 8, 9,
		9, 10, 11, 11, 12, 12, 13, 14,
		14, 15, 16, 17, 19, 20, 22, 24,
		25, 27, 29, 30, 32, 34, 35, 37,
		38, 40, 41, 43, 44, 46, 47, 49,
		50, 52, 53, 55, 56, 57, 60, 63,
		66, 69, 71, 74, 77, 80, 83, 86,
		88, 91, 94, 97, 100, 103, 106, 110,
		113, 117, 120, 124, 127, 131, 134, 138,
		141, 145, 148, 151, 154, 157, 160, 164,
		167, 170, 173, 176, 179, 183, 186, 189,
		192, 195, 198, 202, 203, 205, 206, 208,
		209, 211, 212, 214, 215, 217, 218, 220,
		221, 223, 224, 226, 227, 229, 231, 231,
		232, 233, 233, 234, 235, 235, 236, 237,
		238, 238, 239, 240, 240, 241, 242, 243,
		243, 243, 244, 244, 245, 245, 246, 246,
		247, 247, 248, 248, 249, 249, 249, 249,
		249, 249, 250, 250, 250, 250, 250, 250,
		251, 251, 251, 251, 251, 252, 252, 252,
		252, 252, 252, 252, 252, 253, 253, 253,
		253, 253, 253, 253, 253, 254, 254, 254,
		254, 254, 254, 254, 254, 254, 254, 254,
		254, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255
	},
	{
		197, 196, 196, 196, 196, 196, 196, 196,
		195, 195, 195, 195, 195, 195, 195, 195,
		194, 194, 194, 194, 194, 194, 194, 194,
		193, 193, 193, 193, 192, 192, 192, 192,
		191, 191, 191, 191, 190, 190, 190, 190,
		189, 189, 189, 189, 188, 188, 187, 187,
		187, 186, 186, 186, 185, 185, 184, 184,
		184, 183, 183, 183, 182, 182, 181, 181,
		181, 180, 180, 180, 179, 179, 178, 178,
		177, 177, 176, 176, 176, 175, 175, 174,
		174, 173, 173, 172, 172, 172, 171, 171,
		170, 170, 169, 169, 168, 168, 168, 167,
		167, 166, 166, 165, 165, 164, 164, 164,
		163, 163, 162, 162, 161, 161, 160, 160,
		160, 159, 159, 158, 158, 157, 157, 156,
		156, 156, 155, 154, 154, 153, 152, 152,
		151, 151, 150, 149, 149, 148, 148, 147,
		146, 145, 144, 144, 143, 142, 141, 141,
		140, 139, 138, 138, 137, 136, 135, 135,
		133, 132, 131, 130, 129, 128, 127, 126,
		125, 124, 123, 122, 121, 120, 119, 117,
		116, 115, 113, 112, 110, 109, 107, 106,
		104, 103, 101, 100, 99, 96, 94, 92,
		89, 87, 85, 82, 80, 78, 75, 73,
		71, 69, 67, 65, 63, 61, 59, 57,
		55, 53, 51, 49, 47, 45, 43, 41,
		39, 38, 37, 35, 34, 33, 32, 30,
		29, 28, 27, 25, 24, 23, 22, 21,
		21, 20, 20, 19, 19, 18, 18, 17,
		17, 16, 16, 15, 15, 14, 14, 13,
		13, 12, 11, 11, 11, 11, 11, 10,
		10, 10, 10, 9, 9, 9, 9, 9
	}
};

static BYTE sgabPaletteTints[39] =
{
	255, 255, 255, 255, 77, 77, 0, 255, 0, 105, 105, 255, 199, 179, 119, 105,
	105, 105, 0, 0, 0, 208, 194, 125, 255, 168, 0, 255, 255, 100, 0, 128,
	0, 174, 0, 255, 0, 200, 0
};

#define PALETTE_NEAREST(pbPal, bColor0, bColor1, bColor2, bDest) \
	{ \
		DWORD dwBest = 0xFFFFFFFF; \
		DWORD nBest = 0; \
		DWORD k; \
		for (k = 0; k < 256; k++) \
		{ \
			int nDelta0 = (pbPal)[4 * k] - (bColor0); \
			int nDelta1 = (pbPal)[4 * k + 1] - (bColor1); \
			int nDelta2 = (pbPal)[4 * k + 2] - (bColor2); \
			DWORD dwDistance = nDelta2 * nDelta2 + nDelta1 * nDelta1 + nDelta0 * nDelta0; \
			if (dwDistance < dwBest) \
			{ \
				nBest = k; \
				dwBest = dwDistance; \
			} \
		} \
		(bDest) = (BYTE)nBest; \
	}

void WINAPI PaletteCreateAddTable(BYTE *pbPalette, DWORD dwUnused)
{
	PaletteBuildAddTable(pbPalette);
}

static void __fastcall PaletteBuildAddTable(BYTE *pbPalette)
{
	const BYTE *pbFirst;
	const BYTE *pbSecond;
	int nSum0, nSum1, nSum2;
	BYTE bColor0, bColor1, bColor2;
	int i, j;

	for (i = 0; i < 256; i++)
	{
		pbFirst = pbPalette + 4 * i;
		for (j = 0; j < 256; j++)
		{
			pbSecond = pbPalette + 4 * j;
			nSum0 = pbSecond[0] + pbFirst[0];
			bColor0 = (BYTE)nSum0;
			if (nSum0 >= 255)
				bColor0 = 255;
			nSum1 = pbSecond[1] + pbFirst[1];
			bColor1 = (BYTE)nSum1;
			if (nSum1 >= 255)
				bColor1 = 255;
			nSum2 = pbFirst[2] + pbSecond[2];
			bColor2 = (BYTE)nSum2;
			if (nSum2 >= 255)
				bColor2 = 255;
			PALETTE_NEAREST(pbPalette, bColor0, bColor1, bColor2, pbPalette[PALETTE_ADDTABLE + 256 * j + i])
		}
	}
}

static __inline void PaletteRgbToHsl(const BYTE *pbColor, D2HSL *ptHsl)
{
	double dRed, dGreen, dBlue;
	double dMaxGB, dMinGB;
	double dMax, dMin;
	double dSum;
	double dDelta;
	double dHue, dSat, dLum;
	DWORD i;

	for (i = 0; i < 256; i++)
	{
		dRed = pbColor[0] * (1.0 / 255.0);
		dGreen = pbColor[1] * (1.0 / 255.0);
		dBlue = pbColor[2] * (1.0 / 255.0);
		dMaxGB = max(dGreen, dBlue);
		dMax = max(dRed, dMaxGB);
		dMinGB = min(dGreen, dBlue);
		dMin = min(dRed, dMinGB);
		dSum = dMin + dMax;
		dLum = dSum * 0.5;
#line 63
		D2ASSERT(dLum >= 0.0f && dLum <= 1.0f);
		if (dMax == dMin)
		{
			ptHsl->dHue = 0.0;
			ptHsl->dSat = 0.0;
			ptHsl->dLum = dLum;
			pbColor += 4;
			ptHsl++;
			continue;
		}
		dDelta = dMax - dMin;
		if (dLum <= 0.5)
			dSat = dDelta / dSum;
		else
			dSat = dDelta / (2.0 - dSum);
#line 77
		D2ASSERT(dSat >= 0.0f && dSat <= 1.0f);
		if (dRed == dMax)
			dHue = (dGreen - dBlue) / dDelta;
		else if (dGreen == dMax)
			dHue = (dBlue - dRed) / dDelta + 2.0;
		else
			dHue = (dRed - dGreen) / dDelta + 4.0;
		dHue *= 60.0;
		if (dHue < 0.0)
			dHue += 360.0;
#line 92
		D2ASSERT(dHue >= 0.0f && dHue < 360.0f);
		ptHsl->dHue = dHue;
		ptHsl->dSat = dSat;
		ptHsl->dLum = dLum;
		pbColor += 4;
		ptHsl++;
	}
}

static __inline void PaletteBuildShadeTables(BYTE *pbTables)
{
	const BYTE *pbColor;
	DWORD nLevel;
	DWORD i;

	for (nLevel = 0; nLevel < 32; nLevel++)
	{
		for (i = 0; i < 256; i++)
		{
			pbColor = pbTables + 4 * i;
			pbTables[1024 + 256 * nLevel + i] = PaletteFindNearest(pbTables, 256, pbColor[0] * (nLevel + 1) >> 5,
				pbColor[1] * (nLevel + 1) >> 5, pbColor[2] * (nLevel + 1) >> 5);
		}
	}
}

static __inline BYTE * PaletteCreateTablesBody(const BYTE *pPal)
{
	D2HSL atHsl[256];
	BYTE *pbTables;
	const BYTE *pbFirst;
	const BYTE *pbSecond;
	BYTE bColor0, bColor1, bColor2;
	DWORD nAlpha;
	DWORD nBeta;
	DWORD t, i, j;

	pbTables = (BYTE *)MemVirtualAlloc(PALETTE_TABLESIZE);
	memcpy(pbTables, pPal, 1024);
	PaletteRgbToHsl(pbTables, atHsl);
	for (t = 0; t < 3; t++)
	{
		switch (t)
		{
		case 0:
			nAlpha = 191;
			break;
		case 1:
			nAlpha = 127;
			break;
		default:
			nAlpha = 63;
			break;
		}
		for (j = 0; j < 256; j++)
		{
			pbSecond = pbTables + 4 * j;
			nBeta = 255 - nAlpha;
			for (i = 0; i < 256; i++)
			{
				pbFirst = pbTables + 4 * i;
				bColor0 = (BYTE)((pbFirst[0] * nBeta + pbSecond[0] * nAlpha) / 255u);
				bColor1 = (BYTE)((pbFirst[1] * nBeta + pbSecond[1] * nAlpha) / 255u);
				bColor2 = (BYTE)((pbFirst[2] * nBeta + pbSecond[2] * nAlpha) / 255u);
				pbTables[13568 + (t << 16) + j + 256 * i] = (BYTE)PaletteFindNearest(pbTables, 256,
					bColor0, bColor1, bColor2);
			}
		}
	}
	PaletteBuildShadeTables(pbTables);
	PaletteBuildLightenTables(pbTables);
	PaletteBuildAddTable(pbTables);
	PaletteBuildProductTable(pbTables);
	PaletteBuildHueTables(pbTables, atHsl);
	PaletteBuildLightTable(pbTables, atHsl);
	PaletteBuildDarkTables(pbTables);
	memcpy(pbTables + 439808, sgabPaletteTints, sizeof(sgabPaletteTints));
	PaletteBuildTintTables(pbTables);
	return pbTables;
}

#pragma auto_inline(off)
BYTE * WINAPI PaletteCreateTables(const BYTE *pPal)
{
#line 683
	D2ASSERT(pPal);
	return PaletteCreateTablesBody(pPal);
}
#pragma auto_inline(on)

static void __fastcall PaletteBuildProductTable(BYTE *pbTables)
{
	const BYTE *pbFirst;
	const BYTE *pbSecond;
	BYTE bColor0, bColor1, bColor2;
	DWORD i, j;

	for (j = 0; j < 256; j++)
	{
		pbSecond = pbTables + 4 * j;
		for (i = 0; i < 256; i++)
		{
			pbFirst = pbTables + 4 * i;
			bColor0 = (BYTE)(pbFirst[0] * pbSecond[0] / 255u);
			bColor1 = (BYTE)(pbFirst[1] * pbSecond[1] / 255u);
			bColor2 = (BYTE)(pbSecond[2] * pbFirst[2] / 255u);
			PALETTE_NEAREST(pbTables, bColor0, bColor1, bColor2, pbTables[275712 + 256 * i + j])
		}
	}
}

static void __fastcall PaletteBuildDarkTables(BYTE *pbTables)
{
	const BYTE *pbFirst;
	const BYTE *pbSecond;
	BYTE b0, b1, b2;
	int nMax;
	int n0, n1, n2;
	DWORD i, j;

	for (i = 0; i < 256; i++)
	{
		b0 = pbTables[4 * i];
		b1 = pbTables[4 * i + 1];
		b2 = pbTables[4 * i + 2];
		if (b0 > 0)
			b0 = (BYTE)(b0 + b0 / -3);
		if (b2 > 0)
			b2 = (BYTE)(b2 + b2 / -3);
		if (b1 > 0)
			b1 = (BYTE)(b1 + b1 / -3);
		PALETTE_NEAREST(pbTables, b0, b1, b2, pbTables[439552 + i])
	}
	for (j = 0; j < 256; j++)
	{
		pbSecond = pbTables + 4 * j;
		n0 = pbSecond[0];
		n1 = pbSecond[1];
		n0 = n0 > n1 ? n0 : n1;
		n2 = pbSecond[2];
		nMax = n0 > n2 ? n0 : n2;
		for (i = 0; i < 256; i++)
		{
			pbFirst = pbTables + 4 * i;
			b0 = (BYTE)((DWORD)(pbFirst[0] * (255 - nMax) + pbSecond[0] * nMax) / 255);
			b1 = (BYTE)((DWORD)(pbFirst[1] * (255 - nMax) + pbSecond[1] * nMax) / 255);
			b2 = (BYTE)((DWORD)(pbFirst[2] * (255 - nMax) + pbSecond[2] * nMax) / 255);
			PALETTE_NEAREST(pbTables, b0, b1, b2, pbTables[374016 + 256 * i + j])
		}
	}
}

static void __fastcall PaletteBuildTintTables(BYTE *pbTables)
{
	BYTE *pbDest;
	int nGrey;
	int nColor0, nColor1, nColor2;
	int k, i;

	for (k = 0; k < 12; k++)
	{
		for (i = 0; i < 256; i++)
		{
			nGrey = pbTables[4 * i];
			nColor0 = pbTables[439811 + 3 * k] * nGrey / 255;
			nColor1 = pbTables[439812 + 3 * k] * nGrey / 255;
			nColor2 = pbTables[439813 + 3 * k] * nGrey / 255;
			pbDest = pbTables + 440103 + 256 * k;
			PALETTE_NEAREST(pbTables, (BYTE)nColor0, (BYTE)nColor1, (BYTE)nColor2, pbDest[i])
		}
	}
}

static void __fastcall PaletteBuildLightenTables(BYTE *pbTables)
{
	const BYTE *pbColor;
	DWORD nLevel;
	DWORD i;

	for (nLevel = 0; nLevel < 16; nLevel++)
	{
		for (i = 0; i < 256; i++)
		{
			pbColor = pbTables + 4 * i;
			PALETTE_NEAREST(pbTables,
				pbColor[0] + (255 - pbColor[0]) * (nLevel + 1) / 16,
				pbColor[1] + (255 - pbColor[1]) * (nLevel + 1) / 16,
				pbColor[2] + (255 - pbColor[2]) * (nLevel + 1) / 16,
				pbTables[9216 + 256 * nLevel + i])
		}
	}
}

static __inline void PaletteMatchAll(const BYTE *pbTables, const BYTE *pbRgb, BYTE *pbOut)
{
	DWORD i;

	for (i = 0; i < 256; i++)
		pbOut[i] = PaletteFindNearest(pbTables, 256, pbRgb[4 * i], pbRgb[4 * i + 1], pbRgb[4 * i + 2]);
}

static void __fastcall PaletteBuildHueTables(BYTE *pbTables, const D2HSL *ptHsl)
{
	BYTE abRgb[256 * 4];
	D2HSL atWork[256];
	BYTE bMagnitude;
	DWORD h, i;

	for (h = 0; h < 24; h++)
	{
		for (i = 0; i < 256; i++)
		{
			atWork[i] = ptHsl[i];
			atWork[i].dHue += h * 15.0f;
			if (atWork[i].dHue > 360.0)
				atWork[i].dHue -= 360.0;
		}
		PaletteHslToRgb(atWork, abRgb);
		for (i = 0; i < 256; i++)
			pbTables[341248 + 256 * h + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	}
	for (h = 0; h < 24; h++)
	{
		for (i = 0; i < 256; i++)
		{
			atWork[i] = ptHsl[i];
			atWork[i].dHue += h * 15.0f;
			if (atWork[i].dHue > 360.0)
				atWork[i].dHue -= 360.0;
			atWork[i].dSat = 0.5;
			atWork[i].dLum = atWork[i].dLum - (double)0.1f;
			if (atWork[i].dLum < 0.0)
				atWork[i].dLum = 0.0;
		}
		PaletteHslToRgb(atWork, abRgb);
		for (i = 0; i < 256; i++)
			pbTables[347392 + 256 * h + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	}
	for (h = 0; h < 24; h++)
	{
		for (i = 0; i < 256; i++)
		{
			atWork[i] = ptHsl[i];
			atWork[i].dHue += h * 15.0f;
			if (atWork[i].dHue > 360.0)
				atWork[i].dHue -= 360.0;
			atWork[i].dSat = 0.5;
			atWork[i].dLum = atWork[i].dLum + (double)0.2f;
			if (atWork[i].dLum > 1.0)
				atWork[i].dLum = 1.0;
		}
		PaletteHslToRgb(atWork, abRgb);
		for (i = 0; i < 256; i++)
			pbTables[353536 + 256 * h + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	}
	for (i = 0; i < 256; i++)
	{
		atWork[i] = ptHsl[i];
		atWork[i].dSat = 0.0;
		atWork[i].dLum = atWork[i].dLum * 0.5;
	}
	PaletteHslToRgb(atWork, abRgb);
	for (i = 0; i < 256; i++)
		pbTables[359680 + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	for (i = 0; i < 256; i++)
	{
		atWork[i] = ptHsl[i];
		atWork[i].dSat = 0.0;
		atWork[i].dLum = atWork[i].dLum * ((double)0.5f / (double)0.6f) +
			(double)0.5f / (double)0.6f * (double)0.2f;
	}
	PaletteHslToRgb(atWork, abRgb);
	for (i = 0; i < 256; i++)
		pbTables[359936 + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	for (h = 0; h < 24; h++)
	{
		for (i = 0; i < 256; i++)
		{
			if (ptHsl[i].dHue > 45.0 && ptHsl[i].dHue < 315.0)
			{
				atWork[i] = ptHsl[i];
				atWork[i].dHue += h * 15.0f;
				if (atWork[i].dHue > 360.0)
					atWork[i].dHue -= 360.0;
			}
		}
		PaletteHslToRgb(atWork, abRgb);
		for (i = 0; i < 256; i++)
			pbTables[360192 + 256 * h + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	}
	for (h = 0; h < 12; h++)
	{
		for (i = 0; i < 256; i++)
		{
			atWork[i] = ptHsl[i];
			atWork[i].dHue = h * 30.0f;
			atWork[i].dSat = 1.0;
		}
		PaletteHslToRgb(atWork, abRgb);
		for (i = 0; i < 256; i++)
			pbTables[366592 + 256 * h + i] = PaletteFindNearest(pbTables, 256, abRgb[4 * i], abRgb[4 * i + 1], abRgb[4 * i + 2]);
	}
	for (i = 0; i < 256; i++)
	{
		bMagnitude = (BYTE)(int)sqrt((double)(pbTables[4 * i] * pbTables[4 * i] +
			pbTables[4 * i + 2] * pbTables[4 * i + 2] + pbTables[4 * i + 1] * pbTables[4 * i + 1]));
		pbTables[369664 + i] = PaletteFindNearest(pbTables, 256, bMagnitude, 0, 0);
		pbTables[369920 + i] = PaletteFindNearest(pbTables, 256, 0, bMagnitude, 0);
		pbTables[370176 + i] = PaletteFindNearest(pbTables, 256, 0, 0, bMagnitude);
	}
}

static __inline BYTE PaletteHueToByte(double dN1, double dN2, double dHue)
{
	double dValue;

	if (dHue > 360.0)
		dHue -= 360.0;
	if (dHue < 0.0)
		dHue += 360.0;
	if (dHue < 60.0)
		dValue = dN1 + (dN2 - dN1) * dHue * (1.0 / 60.0);
	else if (dHue < 180.0)
		dValue = dN2;
	else if (dHue < 240.0)
		dValue = dN1 + (dN2 - dN1) * (240.0 - dHue) * (1.0 / 60.0);
	else
		dValue = dN1;
	return (BYTE)(int)(dValue * 255.0);
}

static void __fastcall PaletteHslToRgb(const D2HSL *ptHsl, BYTE *pbRgb)
{
	double dM1, dM2;
	DWORD i;
	BYTE bRed, bGreen, bBlue;

	for (i = 0; i < 256; i++, pbRgb += 4, ptHsl++)
	{
		if (ptHsl->dSat == 0.0)
		{
			bBlue = bGreen = bRed = (BYTE)(int)(ptHsl->dLum * 255.0);
		}
		else
		{
			if (ptHsl->dLum <= 0.5)
				dM2 = (ptHsl->dSat + 1.0) * ptHsl->dLum;
			else
				dM2 = ptHsl->dLum + ptHsl->dSat - ptHsl->dLum * ptHsl->dSat;
			dM1 = (ptHsl->dLum + ptHsl->dLum) - dM2;
			bRed = PaletteHueToByte(dM1, dM2, ptHsl->dHue + 120.0);
			bGreen = PaletteHueToByte(dM1, dM2, ptHsl->dHue);
			bBlue = PaletteHueToByte(dM1, dM2, ptHsl->dHue - 120.0);
		}
		pbRgb[0] = bRed;
		pbRgb[1] = bGreen;
		pbRgb[2] = bBlue;
	}
}

static void __fastcall PaletteBuildLightTable(BYTE *pbTables, D2HSL *ptHsl)
{
	BYTE abRgb[256 * 4];
	int i;

	for (i = 0; i < 256; i++)
	{
		if (ptHsl[i].dLum == 0.0)
			continue;
		ptHsl[i].dLum = ptHsl[i].dLum + (double)0.2f;
		if (ptHsl[i].dLum > 1.0)
			ptHsl[i].dLum = 1.0;
	}
	PaletteHslToRgb(ptHsl, abRgb);
	PaletteMatchAll(pbTables, abRgb, pbTables + 13312);
}

DWORD WINAPI PaletteCreateBlendTable(DWORD dwPalette, DWORD *pdwSize)
{
	*pdwSize = PALETTE_TABLESIZE;
	return (DWORD)PaletteCreateTables((const BYTE *)dwPalette);
}

void WINAPI PaletteFreeTable(void *pTable)
{
	if (pTable)
		MemVirtualFree(pTable);
}

BYTE WINAPI PaletteFindNearest(const BYTE *pbPalette, DWORD dwEntries, int nColor0, int nColor1,
	int nColor2)
{
	DWORD dwBest;
	DWORD dwDistance;
	DWORD nBest;
	DWORD i;
	int nDelta0, nDelta1, nDelta2;

	nBest = 0;
	dwBest = 0xFFFFFFFF;
	for (i = 0; i < dwEntries; i++)
	{
		nDelta0 = pbPalette[4 * i] - nColor0;
		nDelta1 = pbPalette[4 * i + 1] - nColor1;
		nDelta2 = pbPalette[4 * i + 2] - nColor2;
		dwDistance = nDelta2 * nDelta2 + nDelta1 * nDelta1 + nDelta0 * nDelta0;
		if (dwDistance < dwBest)
		{
			nBest = i;
			dwBest = dwDistance;
		}
	}
	return (BYTE)nBest;
}

BYTE WINAPI PaletteFindFarthest(const BYTE *pbPalette, DWORD dwEntries, int nColor0, int nColor1,
	int nColor2)
{
	DWORD dwBest;
	DWORD dwDistance;
	DWORD nBest;
	DWORD i;
	int nDelta0, nDelta1, nDelta2;

	nBest = 0;
	dwBest = 0;
	for (i = 0; i < dwEntries; i++)
	{
		nDelta0 = pbPalette[4 * i] - nColor0;
		nDelta1 = pbPalette[4 * i + 1] - nColor1;
		nDelta2 = pbPalette[4 * i + 2] - nColor2;
		dwDistance = nDelta2 * nDelta2 + nDelta1 * nDelta1 + nDelta0 * nDelta0;
		if (dwDistance > dwBest)
		{
			nBest = i;
			dwBest = dwDistance;
		}
	}
	return (BYTE)nBest;
}

void WINAPI PaletteSetKeyColor(DWORD dwColor)
{
	BYTE *pbColor = (BYTE *)&dwColor;

	sgdwKeyColor = ((pbColor[0] << 8 | pbColor[1]) << 8) | pbColor[2];
}

static __inline BOOL PaletteLoadPcx(const char *pszPalette)
{
	BYTE abHeader[128];
	BYTE abPcxPalette[769];
	HANDLE hFile;
	DWORD dwPaletteRead;
	DWORD dwRead;
	int i;

	hFile = CreateFileA(pszPalette, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugStringA("Unable to open ");
		OutputDebugStringA(pszPalette);
		OutputDebugStringA("\n");
		return FALSE;
	}
	memset(abHeader, 0, sizeof(abHeader));
	dwRead = 0;
	ReadFile(hFile, abHeader, sizeof(abHeader), &dwRead, NULL);
	if (dwRead != sizeof(abHeader) || abHeader[0] != 10 || abHeader[1] != 5 || abHeader[3] != 8)
		OutputDebugStringA("File is not a valid 256-color PCX file.\n");
	else
	{
		memset(abPcxPalette, 0, sizeof(abPcxPalette));
		SetFilePointer(hFile, -769, NULL, FILE_END);
		ReadFile(hFile, abPcxPalette, sizeof(abPcxPalette), &dwPaletteRead, NULL);
		if (dwPaletteRead == sizeof(abPcxPalette) && abPcxPalette[0] == 12)
		{
			for (i = 0; i < NUMBER_PALETTE_ENTRIES; i++)
			{
				sgabPalette[i][0] = abPcxPalette[3 * i + 1];
				sgabPalette[i][1] = abPcxPalette[3 * i + 2];
				sgabPalette[i][2] = abPcxPalette[3 * i + 3];
			}
			CloseHandle(hFile);
			return TRUE;
		}
		OutputDebugStringA("Unable to extract palette from\n");
	}
	CloseHandle(hFile);
	return FALSE;
}

BOOL WINAPI PaletteLoad(const char *pszPalette, int PaletteStartEntry, int PaletteEndEntry)
{
	HANDLE hFile;
	DWORD dwSizeHigh;
	int i;

#line 950
	D2ASSERT(pszPalette != NULL);
#line 951
	D2ASSERT(PaletteStartEntry <= PaletteEndEntry);
#line 952
	D2ASSERT(PaletteStartEntry >= 0);
#line 953
	D2ASSERT(PaletteEndEntry < NUMBER_PALETTE_ENTRIES);
	sgnPaletteEnd = PaletteEndEntry;
	sgnPaletteStart = 1;
	for (i = -256; i < 256; i++)
		sgnSquares[i + 256] = i * i;
	sgdwKeyColor = 0;
	if (lstrlenA(pszPalette))
	{
		hFile = CreateFileA(pszPalette, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			OutputDebugStringA("Unable to open ");
			OutputDebugStringA(pszPalette);
			OutputDebugStringA("\n");
			return FALSE;
		}
		if (GetFileSize(hFile, &dwSizeHigh) == 768)
		{
			DWORD dwRead;

			dwRead = 0;
			ReadFile(hFile, sgabPaletteFile, 768, &dwRead, NULL);
			if (dwRead == 768)
			{
				for (i = 0; i < NUMBER_PALETTE_ENTRIES; i++)
				{
					sgabPalette[i][2] = sgabPaletteFile[3 * i];
					sgabPalette[i][1] = sgabPaletteFile[3 * i + 1];
					sgabPalette[i][0] = sgabPaletteFile[3 * i + 2];
				}
				sgfPaletteLoaded = TRUE;
				CloseHandle(hFile);
				return sgfPaletteLoaded;
			}
			sgfPaletteLoaded = FALSE;
			CloseHandle(hFile);
			return sgfPaletteLoaded;
		}
		CloseHandle(hFile);
		sgfPaletteLoaded = PaletteLoadPcx(pszPalette);
		return sgfPaletteLoaded;
	}
	for (i = 0; i < NUMBER_PALETTE_ENTRIES; i++)
	{
		sgabPalette[i][0] = (BYTE)i;
		sgabPalette[i][1] = (BYTE)i;
		sgabPalette[i][2] = (BYTE)i;
	}
	sgfPaletteLoaded = TRUE;
	return TRUE;
}

void WINAPI PaletteSetMatchKeyColor(BOOL fMatch)
{
	sgfMatchKeyColor = fMatch;
}

BYTE WINAPI PaletteMatchColor(BYTE bColor2, BYTE bColor1, BYTE bColor0)
{
	DWORD dwBest;
	DWORD dwDistance;
	DWORD nBest;
	DWORD i;

	dwBest = 2147483647;
	nBest = 0;
	for (i = sgnPaletteStart; i <= sgnPaletteEnd; i++)
	{
		if (sgfUseMask && !sgabUsable[i])
			continue;
		if (sgfMatchKeyColor && sgdwKeyColor == (((DWORD)bColor2 << 16) | ((DWORD)bColor1 << 8) | bColor0))
		{
			nBest = 0;
			break;
		}
		dwDistance = sgnSquares[256 + bColor2 - sgabPalette[i][2]] +
			sgnSquares[256 + bColor0 - sgabPalette[i][0]] +
			sgnSquares[256 + bColor1 - sgabPalette[i][1]];
		if (dwDistance < dwBest)
		{
			dwBest = dwDistance;
			nBest = i;
		}
	}
	return (BYTE)nBest;
}

DWORD WINAPI PaletteQuantize(const BYTE *pbRgb, BYTE *pbOut, DWORD dwCount,
	BYTE (WINAPI *pfnMatch)(BYTE bColor2, BYTE bColor1, BYTE bColor0))
{
	BYTE (WINAPI *pfnCall)(BYTE, BYTE, BYTE);
	const BYTE *pbIn = pbRgb;
	BYTE *pbDest = pbOut;
	BYTE b2, b1, b0;

	if (!sgfPaletteLoaded)
		return 0;
	pfnCall = !pfnMatch ? (BYTE (WINAPI *)(BYTE, BYTE, BYTE))(void *)PaletteMatchColor : pfnMatch;
	for (; dwCount > 0; dwCount--)
	{
		b2 = *pbIn++;
		b1 = *pbIn++;
		b0 = *pbIn++;
		if (sgfDarkToBlack && b2 <= 10 && b1 <= 10 && b0 <= 10)
		{
			b2 = 4;
			b0 = 0;
			b1 = 0;
		}
		*pbDest++ = pfnCall(b2, b1, b0);
	}
	return 0;
}

void WINAPI PaletteSetDarkToBlack(BOOL fDark)
{
	sgfDarkToBlack = fDark;
}

void WINAPI PaletteSetUsable(const BYTE *pbUsable)
{
	sgfUseMask = TRUE;
	memcpy(sgabUsable, pbUsable, sizeof(sgabUsable));
}
