#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\CelCmp.cpp"
#include "D2Cmp.h"
#include "D2Cmp.h"
#include "StormImports.h"
#include "D2Macros.h"
#include "CelList.h"

static TSExportTableSync<CELLIST, HCELLIST, HLOCKEDCELLIST, CCritSect, 0> sgtCelLists;
#line 16

#define NUM_CELFORMATS		2

static void (__fastcall * const sgpfnEncodeFunc[NUM_CELFORMATS])(CELLIST *ptList, D2CELFILE *pFile) =
{
	FastCmpBuildCelFile, NULL
};
static void (__fastcall * const sgpfnDecodeFunc[NUM_CELFORMATS])(D2CEL *pCel, BYTE *pbDst, int nX,
	int nClipLeft, int nClipRight, int dwStride, UINT nLinesToSkip, UINT nLinesToDraw,
	const BYTE *pbColorMap, const BYTE *pbBlend, const BYTE *pbPreMap) =
{
	FastCmpDecode, NULL
};

typedef void (__fastcall *PFNCELCONVERT)(D2CELFILE *pSrc, D2CELFILE *pDst,
	BYTE (WINAPI *pfnMatch)(BYTE, BYTE, BYTE), DWORD dwKeyColor);
static const PFNCELCONVERT sgpfnConvertFunc[2] =
{
	FastCmpConvert24Bit,
	NULL,
};

typedef void (__fastcall *PFNCELREMAP)(D2CEL *pCel, const BYTE *pbMap);
static const PFNCELREMAP sgpfnRemapFunc[2] =
{
	FastCmpCelRemapColors,
	NULL,
};

extern const DWORD sgadwDirectionOrder32[7][32] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 4, 2, 5, 1, 6, 3, 7, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 8, 4, 9, 2, 10, 5, 11, 1, 12, 6, 13, 3, 14, 7, 15,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 16, 8, 17, 4, 18, 9, 19, 2, 20, 10, 21, 5, 22, 11, 23,
		1, 24, 12, 25, 6, 26, 13, 27, 3, 28, 14, 29, 7, 30, 15, 31,
	},
};
#line 49
static const DWORD sgadwNormalDirections[9 * 32] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 5, 6, 7, 0, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 5, 6, 7, 0, 2, 1, 3, 8, 9, 10, 11, 12, 13, 14, 15,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 5, 6, 7, 0, 2, 1, 3, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
extern const DWORD sgadwDirectionOrder64[7][64] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2,
		2, 2, 2, 2, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1,
		1, 1, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3,
		3, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0,
	},
	{
		0, 0, 8, 8, 8, 8, 4, 4, 4, 4, 9, 9, 9, 9, 2, 2,
		2, 2, 10, 10, 10, 10, 5, 5, 5, 5, 11, 11, 11, 11, 1, 1,
		1, 1, 12, 12, 12, 12, 6, 6, 6, 6, 13, 13, 13, 13, 3, 3,
		3, 3, 14, 14, 14, 14, 7, 7, 7, 7, 15, 15, 15, 15, 0, 0,
	},
	{
		0, 16, 16, 8, 8, 17, 17, 4, 4, 18, 18, 9, 9, 19, 19, 2,
		2, 20, 20, 10, 10, 21, 21, 5, 5, 22, 22, 11, 11, 23, 23, 1,
		1, 24, 24, 12, 12, 25, 25, 6, 6, 26, 26, 13, 13, 27, 27, 3,
		3, 28, 28, 14, 14, 29, 29, 7, 7, 30, 30, 15, 15, 31, 31, 0,
	},
	{
		0, 32, 16, 33, 8, 34, 17, 35, 4, 36, 18, 37, 9, 38, 19, 39,
		2, 40, 20, 41, 10, 42, 21, 43, 5, 44, 22, 45, 11, 46, 23, 47,
		1, 48, 24, 49, 12, 50, 25, 51, 6, 52, 26, 53, 13, 54, 27, 55,
		3, 56, 28, 57, 14, 58, 29, 59, 7, 60, 30, 61, 15, 62, 31, 63,
	},
};
#line 82
static const DWORD sgadwGameDirections[8][64] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5,
		5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 6, 6, 6, 6,
		6, 6, 6, 6, 2, 2, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7,
		7, 7, 7, 7, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
	},
	{
		4, 4, 8, 8, 8, 8, 0, 0, 0, 0, 9, 9, 9, 9, 5, 5,
		5, 5, 10, 10, 10, 10, 1, 1, 1, 1, 11, 11, 11, 11, 6, 6,
		6, 6, 12, 12, 12, 12, 2, 2, 2, 2, 13, 13, 13, 13, 7, 7,
		7, 7, 14, 14, 14, 14, 3, 3, 3, 3, 15, 15, 15, 15, 4, 4,
	},
	{
		4, 16, 16, 8, 8, 17, 17, 0, 0, 18, 18, 9, 9, 19, 19, 5,
		5, 20, 20, 10, 10, 21, 21, 1, 1, 22, 22, 11, 11, 23, 23, 6,
		6, 24, 24, 12, 12, 25, 25, 2, 2, 26, 26, 13, 13, 27, 27, 7,
		7, 28, 28, 14, 14, 29, 29, 3, 3, 30, 30, 15, 15, 31, 31, 4,
	},
	{
		4, 32, 16, 33, 8, 34, 17, 35, 0, 36, 18, 37, 9, 38, 19, 39,
		5, 40, 20, 41, 10, 42, 21, 43, 1, 44, 22, 45, 11, 46, 23, 47,
		6, 48, 24, 49, 12, 50, 25, 51, 2, 52, 26, 53, 13, 54, 27, 55,
		7, 56, 28, 57, 14, 58, 29, 59, 3, 60, 30, 61, 15, 62, 31, 63,
	},
};
static const DWORD sgadwGameDirections2[8][64] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 16, 16, 16, 16, 16, 16, 16, 16,
		16, 16, 16, 16, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 48, 48, 48, 48, 48, 48, 48, 48,
		48, 48, 48, 48, 48, 48, 48, 48, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8, 16, 16, 16, 16,
		16, 16, 16, 16, 24, 24, 24, 24, 24, 24, 24, 24, 32, 32, 32, 32,
		32, 32, 32, 32, 40, 40, 40, 40, 40, 40, 40, 40, 48, 48, 48, 48,
		48, 48, 48, 48, 56, 56, 56, 56, 56, 56, 56, 56, 0, 0, 0, 0,
	},
	{
		4, 4, 8, 8, 8, 8, 12, 12, 12, 12, 16, 16, 16, 16, 20, 20,
		20, 20, 24, 24, 24, 24, 28, 28, 28, 28, 32, 32, 32, 32, 36, 36,
		36, 36, 40, 40, 40, 40, 44, 44, 44, 44, 48, 48, 48, 48, 52, 52,
		52, 52, 56, 56, 56, 56, 60, 60, 60, 60, 0, 0, 0, 0, 4, 4,
	},
	{
		2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14, 16, 16, 18,
		18, 20, 20, 22, 22, 24, 24, 26, 26, 28, 28, 30, 30, 32, 32, 34,
		34, 36, 36, 38, 38, 40, 40, 42, 42, 44, 44, 46, 46, 48, 48, 50,
		50, 52, 52, 54, 54, 56, 56, 58, 58, 60, 60, 62, 62, 0, 0, 2,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
};
static const DWORD sgadwGameDirections3[8][64] =
{
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
		2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
		4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6,
		6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0,
	},
	{
		0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4,
		4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8,
		8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12,
		12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15, 0, 0,
	},
	{
		0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8,
		8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16,
		16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24,
		24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 0,
	},
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
		16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
		32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
		48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
	},
};

static const BYTE sgabDir8To4[8] = { 1, 1, 3, 3, 5, 5, 7, 7 };
static const BYTE sgabDir16To8[16] = { 0, 0, 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14 };
static const BYTE sgabDir8To4Alt[8] = { 3, 3, 5, 5, 7, 7, 1, 1 };
static const BYTE sgabDir16To8Alt[16] = { 2, 2, 4, 4, 6, 6, 8, 8, 10, 10, 12, 12, 14, 14, 0, 0 };

int __fastcall CelGetDirectionRow(DWORD nNumDirections)
{
#line 343
	D2ASSERT(nNumDirections);
	return GetFirstBit(nNumDirections);
}

DWORD WINAPI CelGetNormalDirection(DWORD nNormalDirection, DWORD nNumberOfDirections)
{
#line 355
	D2ASSERT(nNormalDirection < nNumberOfDirections);
#line 356
	D2ASSERT(GetFirstBit( nNumberOfDirections ) < 8);
#line 357
	D2ASSERT(BITCOUNT( nNumberOfDirections ) == 1);
	return sgadwNormalDirections[GetFirstBit(nNumberOfDirections) * 32 + nNormalDirection];
}

DWORD WINAPI CelGetGameDirection(DWORD nGameDirection, DWORD nLevelOfRotation)
{
#line 376
	D2ASSERT(nGameDirection < CELFILE_NUMGAMEDIRECTIONS);
#line 377
	D2ASSERT(GetFirstBit( nLevelOfRotation ) < 8);
#line 378
	D2ASSERT(BITCOUNT( nLevelOfRotation ) == 1);
	return sgadwGameDirections[GetFirstBit(nLevelOfRotation)][nGameDirection];
}

int WINAPI CelConvertDirection(DWORD nDirection, DWORD nStartDirs, int nEndDirs, BOOL fAlternate)
{
#line 397
	D2ASSERT(nDirection < nStartDirs);
	if (nStartDirs == 8)
	{
		if (nEndDirs == 4)
		{
			if (fAlternate)
				return sgabDir8To4Alt[nDirection];
			return sgabDir8To4[nDirection];
		}
	}
	else if (nStartDirs == 16 && nEndDirs == 8)
	{
		if (fAlternate)
			return sgabDir16To8Alt[nDirection];
		return sgabDir16To8[nDirection];
	}
#line 422
	D2FATAL("Could not convert direction!");
}

int WINAPI CelGetDirectionConversion(int nFrom, int nTo)
{
	if (nFrom == 4)
	{
		if (nTo == 8)
			return 0;
	}
	else if (nFrom == 8 && nTo == 16)
		return 1;
#line 445
	D2FATAL("Could not convert direction!");
}

void __cdecl operator delete(void *pMemory)
{
	if (pMemory)
		StormSMemFree(pMemory, "delete", -1, 0);
}

DWORD WINAPI CelGetRoundedGameDirection(DWORD nGameDirection, DWORD nLevelOfRotation)
{
#line 458
	D2ASSERT(nGameDirection < CELFILE_NUMGAMEDIRECTIONS);
#line 459
	D2ASSERT(GetFirstBit( nLevelOfRotation ) < 8);
#line 460
	D2ASSERT(BITCOUNT( nLevelOfRotation ) == 1);
	return sgadwGameDirections2[GetFirstBit(nLevelOfRotation)][nGameDirection];
}

void * __cdecl operator new(unsigned int nSize)
{
	return StormSMemAlloc(nSize, "new", -1, 0);
}

DWORD WINAPI CelGetDirectionIndex(DWORD nGameDirection, DWORD nLevelOfRotation)
{
#line 473
	D2ASSERT(nGameDirection < CELFILE_NUMGAMEDIRECTIONS);
#line 474
	D2ASSERT(GetFirstBit( nLevelOfRotation ) < 8);
#line 475
	D2ASSERT(BITCOUNT( nLevelOfRotation ) == 1);
	return sgadwGameDirections3[GetFirstBit(nLevelOfRotation)][nGameDirection];
}

static PFNFREEHARDWARE sgpfnFreeHardware;

#define CELLIST_ERR_EMPTY	0x85200001
#define CELLIST_ERR_UNEVEN	0x85200002

DWORD WINAPI CelListCreate(int format, DWORD dwNumDirections, const DWORD *pdwKeyColor, BOOL f24Bit)
{
	CELLIST *ptList;
	DWORD dwHandle;
	int nLocked;

#line 492
	D2ASSERT(BITCOUNT( dwNumDirections ) == 1);
#line 493
	D2ASSERT(dwNumDirections <= 32);
#line 494
	D2ASSERT(format < NUM_CELFORMATS);
	ptList = sgtCelLists.NewLock(&dwHandle, &nLocked);
	ptList->tEntries.DeleteAll();
	ptList->dwNumDirections = dwNumDirections;
	ptList->format = format;
	ptList->nCels = 0;
	ptList->dwFlags = 0;
	if (f24Bit)
	{
		ptList->dwFlags = CELLIST_24BIT;
		if (pdwKeyColor)
			ptList->dwKeyColor = *pdwKeyColor;
	}
	sgtCelLists.Unlock(nLocked);
	return dwHandle;
}

void WINAPI CelListAddImage(DWORD dwHandle, const BYTE *pbImage, DWORD dwImageByteStride, DWORD dwPixelHeight,
	DWORD dwPixelWidth, DWORD dwOffsetX, DWORD dwOffsetY, DWORD dwValue)
{
	CELLIST *ptList;
	TCELENTRY *ptEntry;
	DWORD dwRowBytes;
	int nLocked;

#line 534
	D2ASSERT(dwPixelHeight && dwPixelWidth);
#line 535
	D2ASSERT(dwImageByteStride >= dwPixelWidth);
	ptList = sgtCelLists.Lock(dwHandle, &nLocked, FALSE);
	D2_OPAQUE(ptList);
	ptEntry = ptList->tEntries.NewNode(LINK_BEFORE, 0, 0);
	D2_OPAQUE(ptEntry);
	ptList->nCels++;
	ptEntry->field_10 = dwPixelHeight;
	ptEntry->field_14 = dwPixelHeight * dwPixelWidth;
	ptEntry->field_0c = dwPixelWidth;
	ptEntry->field_18 = dwOffsetX;
	ptEntry->field_1c = dwOffsetY;
	ptEntry->field_08 = dwValue;
	dwRowBytes = dwPixelWidth;
	if (ptList->dwFlags & CELLIST_24BIT)
		dwRowBytes = 3 * dwPixelWidth;
#line 550
	ptEntry->field_20 = (BYTE *)D2ALLOC2(dwRowBytes * dwPixelHeight);
	MemCopyRect(ptEntry->field_20, pbImage, dwRowBytes, dwImageByteStride, dwPixelHeight, dwRowBytes);
	sgtCelLists.Unlock(nLocked);
}

void WINAPI CelListFree(DWORD dwHandle)
{
	CELLIST *ptList;
	int nLocked;

	ptList = sgtCelLists.Lock(dwHandle, &nLocked, FALSE);
	D2_OPAQUE(ptList);
	for (TCELENTRY *ptEntry = ptList->tEntries.Head(), *ptDelete = NULL; (INT_PTR)ptEntry > 0;
		ptDelete
			? (ptEntry = ptList->tEntries.FreeElement(ptEntry),
				ptEntry = (INT_PTR)ptDelete > 0 ? ptEntry : NULL,
				ptDelete = NULL,
				ptEntry)
			: ptEntry = ptList->tEntries.LinkNext(ptEntry))
	{
#line 569
		D2FREE2(ptEntry->field_20);
	}
	ptList->tEntries.DeleteAll();
	sgtCelLists.DeleteLocked(ptList, nLocked);
}

DWORD WINAPI CelListBuild(DWORD dwHandle, D2CELFILE **ppFile)
{
	CELLIST *ptList;
	D2CELFILE *pFile;
	int nLocked;

	ptList = sgtCelLists.Lock(dwHandle, &nLocked, FALSE);
	D2_OPAQUE(ptList);
	*ppFile = NULL;
	if (!ptList->nCels)
		return CELLIST_ERR_EMPTY;
	if (ptList->nCels % ptList->dwNumDirections)
		return CELLIST_ERR_UNEVEN;
#line 596
	pFile = (D2CELFILE *)D2ALLOC2(ptList->nCels * sizeof(D2CEL *) + offsetof(D2CELFILE, aptCels));
	pFile->dwVersion = CELFILE_VERSION;
	pFile->dwFlags = ptList->dwFlags;
	pFile->format = ptList->format;
	pFile->dwNumDirections = ptList->dwNumDirections;
	pFile->dwCelsPerDirection = ptList->nCels / ptList->dwNumDirections;
	if (ptList->dwFlags & CELLIST_24BIT)
	{
		RawBuildCelFile(ptList, pFile);
		pFile->dwKeyColor = ptList->dwKeyColor;
	}
	else
		sgpfnEncodeFunc[ptList->format](ptList, pFile);
	sgtCelLists.Unlock(nLocked);
	*ppFile = pFile;
	return 0;
}

void * WINAPI CelFileCheckHandle(void *hFile, DWORD dwUnused)
{
#line 649
	D2ASSERT(hFile);
	return hFile;
}

BOOL WINAPI CelFileConvert24Bit(D2CELFILE *pFile, D2CELFILE **ppFile,
	BYTE (WINAPI *pfnMatch)(BYTE, BYTE, BYTE), DWORD dwKeyColor)
{
	D2CELFILE *pNewFile;
	DWORD dwCels;

#line 666
	D2ASSERT(pFile->dwFlags & CELFILE_24BIT);
	dwCels = pFile->dwNumDirections * pFile->dwCelsPerDirection;
#line 670
	pNewFile = (D2CELFILE *)D2ALLOC2(dwCels * sizeof(D2CEL *) + offsetof(D2CELFILE, aptCels));

	pNewFile->dwVersion = CELFILE_VERSION;
	pNewFile->dwFlags = pFile->dwFlags & ~7;
	pNewFile->dwNumDirections = pFile->dwNumDirections;
	pNewFile->dwCelsPerDirection = pFile->dwCelsPerDirection;
	pNewFile->format = 0;
	sgpfnConvertFunc[pFile->format](pFile, pNewFile, pfnMatch, dwKeyColor);
	*ppFile = pNewFile;
	return TRUE;
}

void WINAPI CelFileConvertVersion(D2CELFILE5 *pOldFile, D2CELFILE **ppFile, DWORD *pdwSize)
{
	D2CELFILE *pFile;
	D2CEL5 *pOldCel;
	D2CEL *pCel;
	BYTE *pbNext;
	DWORD dwCels;
	DWORD dwSize;
	DWORD i;

#line 696
	D2ASSERT(pOldFile->dwFlags & CELFILE_SERIALIZED);
#line 697
	D2ASSERT(pOldFile->dwVersion == ( CELFILE_VERSION - 1 ));
	pOldFile->dwFlags |= CELFILE_POINTERS;
	for (i = 0; i < pOldFile->dwNumDirections * pOldFile->dwCelsPerDirection; i++)
	{
		pOldCel = (D2CEL5 *)((BYTE *)pOldFile + (DWORD)pOldFile->aptCels[i]);
		pOldFile->aptCels[i] = pOldCel;
		if (pOldCel->dwExtraSize)
			pOldCel->pbExtraData = (BYTE *)pOldFile + (DWORD)pOldCel->pbExtraData;
		else
			pOldCel->pbExtraData = NULL;
	}
	dwCels = pOldFile->dwNumDirections * pOldFile->dwCelsPerDirection;
	dwSize = dwCels * sizeof(D2CEL *) + offsetof(D2CELFILE, aptCels);
	for (i = 0; i < dwCels; i++)
		dwSize += CELSIZE(pOldFile->aptCels[i]->dwDataSize) + pOldFile->aptCels[i]->dwExtraSize;
#line 720
	pFile = (D2CELFILE *)D2ALLOC2(dwSize);
	pFile->dwVersion = CELFILE_VERSION;
	if (pOldFile->dwFlags & CELFILE_24BIT)
		pFile->dwFlags = CELFILE_24BIT | CELFILE_SERIALIZED;
	else
		pFile->dwFlags = CELFILE_SERIALIZED;
	pFile->format = 0;
	pFile->dwKeyColor = 0;
	pFile->dwNumDirections = pOldFile->dwNumDirections;
	pFile->dwCelsPerDirection = pOldFile->dwCelsPerDirection;
	pbNext = (BYTE *)&pFile->aptCels[dwCels];
	for (i = 0; i < dwCels; i++)
	{
		pCel = (D2CEL *)pbNext;
		pFile->aptCels[i] = (D2CEL *)(pbNext - (BYTE *)pFile);
		pCel->dwFlags = pOldFile->aptCels[i]->dwFlags;
		pCel->dwWidth = pOldFile->aptCels[i]->dwWidth;
		pCel->dwHeight = pOldFile->aptCels[i]->dwHeight;
		pCel->nOffsetX = pOldFile->aptCels[i]->dwOffsetX;
		pCel->nOffsetY = pOldFile->aptCels[i]->dwOffsetY;
		pCel->dwExtraSize = pOldFile->aptCels[i]->dwExtraSize;
		pCel->dwDataSize = pOldFile->aptCels[i]->dwDataSize;
		memcpy(pCel->abData, pOldFile->aptCels[i]->abData, pCel->dwDataSize);
		pbNext += CELSIZE(pCel->dwDataSize);
		if (pOldFile->aptCels[i]->pbExtraData)
		{
			memcpy(pbNext, &pOldFile->aptCels[i]->pbExtraData, pOldFile->aptCels[i]->dwExtraSize);
			pCel->ptSlot = (D2CELSLOT *)(pbNext - (BYTE *)pFile);
			pbNext += pOldFile->aptCels[i]->dwExtraSize;
		}
		else
			pCel->ptSlot = NULL;
	}
	*ppFile = pFile;
	*pdwSize = dwSize;
}

void WINAPI CelFileForEachPixel(D2CELFILE *pFile, void (WINAPI *pfnPixel)(DWORD))
{
	DWORD nDirection;
	DWORD nFrame;

#line 783
	D2ASSERT(pFile->dwFlags & CELFILE_24BIT);
	for (nDirection = 0; nDirection < pFile->dwNumDirections; nDirection++)
	{
		for (nFrame = 0; nFrame < pFile->dwCelsPerDirection; nFrame++)
			RawCelForEachPixel(pFile->aptCels[pFile->dwCelsPerDirection * nDirection + nFrame], pfnPixel,
				pFile->dwKeyColor);
	}
}

BOOL WINAPI CelFileSerialize(D2CELFILE *pFile, D2CELFILE **ppSerializedFile, DWORD *pdwSize)
{
	D2CELFILE *pSerializedFile;
	D2CEL *pCel;
	BYTE *pbNext;
	D2CELSLOT *ptSlots;
	DWORD dwCels;
	DWORD dwSize;
	DWORD dwCelSize;
	DWORD i;

	dwCels = pFile->dwCelsPerDirection * pFile->dwNumDirections;
	dwSize = dwCels * sizeof(D2CEL *) + offsetof(D2CELFILE, aptCels);
	for (i = 0; i < dwCels; i++)
		dwSize += CELSIZE(pFile->aptCels[i]->dwDataSize) + pFile->aptCels[i]->dwExtraSize;
#line 818
	pSerializedFile = (D2CELFILE *)D2ALLOC2(dwSize);
	memcpy(pSerializedFile, pFile, offsetof(D2CELFILE, aptCels) + sizeof(D2CEL *));
	pSerializedFile->dwFlags = (pSerializedFile->dwFlags & ~CELFILE_POINTERS) | CELFILE_SERIALIZED;
	pbNext = (BYTE *)&pSerializedFile->aptCels[dwCels];
	ptSlots = NULL;
	for (i = 0; i < dwCels; i++)
	{
		pCel = (D2CEL *)pbNext;
		dwCelSize = CELSIZE(pFile->aptCels[i]->dwDataSize);
		pSerializedFile->aptCels[i] = (D2CEL *)(pbNext - (BYTE *)pSerializedFile);
		memcpy(pbNext, pFile->aptCels[i], dwCelSize);
		pbNext += dwCelSize;
		if (pFile->aptCels[i]->ptSlot && pFile->aptCels[i]->ptSlot->pbExtraData)
		{
			memcpy(pbNext, pFile->aptCels[i]->ptSlot->pbExtraData, pFile->aptCels[i]->dwExtraSize);
			if (i == 0)
				ptSlots = pFile->aptCels[0]->ptSlot;
		}
		pCel->ptSlot = (D2CELSLOT *)(pbNext - (BYTE *)pSerializedFile);
		pbNext += pFile->aptCels[i]->dwExtraSize;
	}
	if (ptSlots)
	{
#line 847
		D2FREE2(ptSlots);
	}
	*ppSerializedFile = pSerializedFile;
	*pdwSize = dwSize;
	return TRUE;
}

void WINAPI CelFileLoadSerialized(D2CELFILE *pFile, D2CELFILE **ppFile, DWORD dwUnused3, DWORD dwUnused4,
	int nSpecVersion, DWORD dwUnused6)
{
	D2CELSLOT *ptSlot;
	DWORD dwSize;
	DWORD i;

#line 869
	D2ASSERT(pFile != NULL);
#line 878
	D2ASSERT(pFile->dwFlags & CELFILE_SERIALIZED);
	if (nSpecVersion == -1)
	{
#line 881
		D2ASSERT(pFile->dwVersion == CELFILE_VERSION);
	}
	else
	{
#line 883
		D2ASSERT(pFile->dwVersion == (DWORD)nSpecVersion);
	}
	if (pFile->dwFlags & CELFILE_POINTERS)
		return;
	pFile->dwFlags |= CELFILE_POINTERS;
	dwSize = pFile->dwNumDirections * pFile->dwCelsPerDirection * sizeof(D2CELSLOT);
#line 894
	ptSlot = (D2CELSLOT *)D2ALLOC2(dwSize);
	memset(ptSlot, 0, dwSize);
	for (i = 0; i < pFile->dwNumDirections * pFile->dwCelsPerDirection; i++)
	{
		pFile->aptCels[i] = (D2CEL *)((BYTE *)pFile + (DWORD)pFile->aptCels[i]);
		pFile->aptCels[i]->ptSlot = ptSlot;
		ptSlot++;
	}
	*ppFile = pFile;
}

void WINAPI CelFileFreeSerialized(void *pSerializedFile)
{
#line 948
	D2ASSERT(pSerializedFile);
#line 949
	D2FREE2(pSerializedFile);
}

void WINAPI CelFileFree(D2CELFILE *pCelFile)
{
#line 962
	D2ASSERT(pCelFile);
	if (pCelFile->dwFlags & (CELFILE_SERIALIZED | CELFILE_POINTERS))
	{
		if (pCelFile->dwFlags & CELFILE_POINTERS)
			CelFileFreeHardware(pCelFile);
#line 969
		D2FREE2(pCelFile);
	}
}

void WINAPI CelFileRemapColors(D2CELFILE *pCelFile, const BYTE *pbMap)
{
	DWORD i;

#line 984
	D2ASSERT(pCelFile);
	for (i = 0; i < pCelFile->dwNumDirections * pCelFile->dwCelsPerDirection; i++)
		sgpfnRemapFunc[pCelFile->format](pCelFile->aptCels[i], pbMap);
}

void WINAPI CelContextDraw(D2CELCONTEXT *pCelContext, int nX, int nY, int nClipLeft, int nClipRight,
	BYTE *pDestSurface, int nSurfaceHeight, DWORD dwStride, const BYTE *pbColorMap, const BYTE *pbBlend,
	const BYTE *pbPreMap)
{
	D2CELFILE *pCelFile;
	D2CEL *pCel;
	DWORD eFormat;
	BYTE *pbDest;
	int nLines;
	int nSkip;

	pCelFile = pCelContext->hCelFile;
	if (pCelFile)
	{
#line 1017
		D2ASSERT(pCelFile->dwVersion == CELFILE_VERSION);
#line 1018
		D2ASSERT(!( pCelFile->dwFlags & CELFILE_24BIT ));
	}
#line 1021
	D2ASSERT(pDestSurface);
	pCel = pCelContext->hCel;
#line 1024
	D2ASSERT(pCel);
	nLines = pCel->dwHeight;
	nX += pCel->nOffsetX;
	nY += pCel->nOffsetY;
	nSkip = 0;
	if (nY - nLines < 0)
		nLines = nY + 1;
	if (nLines <= 0)
		return;
	if (nY >= nSurfaceHeight)
	{
		nSkip = nY - nSurfaceHeight + 1;
		nLines -= nSkip;
		if (nLines <= 0)
			return;
		nY = nSurfaceHeight - 1;
	}
	if (!nClipLeft && !nClipRight && (nX < 0 || nY < 0))
		return;
	pbDest = pDestSurface + nY * dwStride + nX;
	eFormat = 0;
	if (pCelFile)
		eFormat = pCelFile->format;
#line 1064
	D2ASSERT(sgpfnDecodeFunc[eFormat]);
	sgpfnDecodeFunc[eFormat](pCel, pbDest, nX, nClipLeft, nClipRight, dwStride, nSkip, nLines, pbColorMap,
		pbBlend, pbPreMap);
}

BOOL WINAPI CelFileCopy24Bit(D2CELFILE *pCelFile, DWORD nDirection, DWORD nFrame, BYTE *pbDest)
{
	DWORD nCel;

#line 1095
	D2ASSERT(pCelFile->dwVersion == CELFILE_VERSION);
#line 1096
	D2ASSERT(pCelFile->dwFlags & CELFILE_24BIT);
	nCel = CelGetGameDirection(nDirection, pCelFile->dwNumDirections) * pCelFile->dwCelsPerDirection + nFrame;
	memcpy(pbDest, pCelFile->aptCels[nCel]->abData,
		pCelFile->aptCels[nCel]->dwHeight * pCelFile->aptCels[nCel]->dwWidth * 3);
	return TRUE;
}

void WINAPI CelContextDrawRows(D2CELCONTEXT *pCelContext, int nX, int nY, int nClipLeft, int nClipRight,
	BYTE *pDestSurface, int nSurfaceHeight, DWORD dwStride, const BYTE *pbColorMap, const BYTE *pbBlend,
	int nFromBottom, int nRows, const BYTE *pbPreMap)
{
	D2CELFILE *pCelFile;
	D2CEL *pCel;
	int nLast;
	int nFirst;

	pCelFile = pCelContext->hCelFile;
	if (pCelFile)
	{
#line 1133
		D2ASSERT(pCelFile->dwVersion == CELFILE_VERSION);
#line 1134
		D2ASSERT(!( pCelFile->dwFlags & CELFILE_24BIT ));
	}
#line 1137
	D2ASSERT(pDestSurface);
	pCel = pCelContext->hCel;
#line 1140
	D2ASSERT(pCel);
	nX += pCel->nOffsetX;
	nY += pCel->nOffsetY;
	nLast = nY - nFromBottom;
	nFirst = nLast - nRows;
	if (nLast < 0 || nFirst >= nSurfaceHeight)
		return;
	if (nFirst < 0)
		nFirst = 0;
	if (nLast >= nSurfaceHeight)
		nLast = nSurfaceHeight - 1;
#line 1165
	D2ASSERT(sgpfnDecodeFunc[pCelFile->format]);
	sgpfnDecodeFunc[pCelFile->format](pCel, pDestSurface + nLast * dwStride + nX, nX, nClipLeft, nClipRight,
		dwStride, nY - nLast, nLast - nFirst, pbColorMap, pbBlend, pbPreMap);
}

void WINAPI CelContextDrawShadow(D2CELCONTEXT *pCelContext, int nX, int nY, int nClipLeft, int nClipRight,
	BYTE *pDestSurface, int nSurfaceHeight, DWORD dwStride, const BYTE *pbTable, DWORD dwColor)
{
	D2CELFILE *pCelFile;
	D2CEL *pCel;
	int nRows;
	int nSkip;

	pCelFile = pCelContext->hCelFile;
	if (pCelFile)
	{
#line 1201
		D2ASSERT(pCelFile->dwVersion == CELFILE_VERSION);
#line 1202
		D2ASSERT(!( pCelFile->dwFlags & CELFILE_24BIT ));
	}
#line 1205
	D2ASSERT(pDestSurface);
	pCel = pCelContext->hCel;
#line 1208
	D2ASSERT(pCel);
	nX += pCel->nOffsetX + pCel->nOffsetY / 2;
	nY += pCel->nOffsetY / 2;
	nRows = pCel->dwHeight >> 1;
	nSkip = 0;
	if (nY - (int)pCel->dwHeight / 2 < 0)
		nRows = nY + 1;
	if (nRows <= 0)
		return;
	if (nY >= nSurfaceHeight)
	{
		nSkip = nY - nSurfaceHeight + 1;
		nRows -= nSkip;
		if (nRows <= 0)
			return;
		nY = nSurfaceHeight - 1;
	}
	nX -= nSkip;
	FastCmpDrawShadow(pCel, pDestSurface + nY * dwStride + nX, nX, nClipLeft, nClipRight, dwStride, nSkip,
		nRows, pbTable, dwColor);
}

BOOL WINAPI ValidCelContext(D2CELCONTEXT *pCelContext)
{
	D2CELFILE *pFile;

#line 1258
	D2ASSERT(pCelContext);
	pFile = pCelContext->hCelFile;
#line 1261
	D2ASSERT(pFile);
	if (pFile->dwVersion != CELFILE_VERSION)
		return FALSE;
	if (pCelContext->dwDirection >= CELFILE_NUMGAMEDIRECTIONS)
		return FALSE;
	if (pCelContext->dwFrame > pFile->dwCelsPerDirection)
		return FALSE;
	return TRUE;
}

D2CEL * WINAPI CelContextGetCel(D2CELCONTEXT *pCelContext)
{
	D2CELFILE *pFile;

#line 1283
	D2ASSERT(pCelContext);
#line 1284
	D2ASSERT(ValidCelContext( pCelContext ));
	pFile = pCelContext->hCelFile;
	return pFile->aptCels[CelGetGameDirection(pCelContext->dwDirection,
		pFile->dwNumDirections) * pFile->dwCelsPerDirection + pCelContext->dwFrame];
}

DWORD WINAPI CelGetWidth(D2CEL *pCel)
{
#line 1304
	D2ASSERT(pCel);
	return pCel->dwWidth;
}

DWORD WINAPI CelGetHeight(D2CEL *pCel)
{
#line 1318
	D2ASSERT(pCel);
	return pCel->dwHeight;
}

int WINAPI CelGetOffsetX(D2CEL *pCel)
{
#line 1332
	D2ASSERT(pCel);
	return pCel->nOffsetX;
}

int WINAPI CelGetOffsetY(D2CEL *pCel)
{
#line 1346
	D2ASSERT(pCel);
	return pCel->nOffsetY;
}

BYTE * WINAPI CelGetData(D2CEL *ptCel)
{
#line 1361
	D2ASSERT(ptCel);
	return ptCel->abData;
}

DWORD WINAPI CelFileGetNumDirections(D2CELFILE *pFile)
{
#line 1375
	D2ASSERT(pFile->dwVersion == CELFILE_VERSION);
	return pFile->dwNumDirections;
}

DWORD WINAPI CelFileGetNumFrames(D2CELFILE *pFile)
{
#line 1390
	D2ASSERT(pFile->dwVersion == CELFILE_VERSION);
	return pFile->dwCelsPerDirection;
}

void WINAPI CelSetHardwareFree(PFNFREEHARDWARE pfnFree)
{
	sgpfnFreeHardware = pfnFree;
}

PFNFREEHARDWARE __fastcall CelGetHardwareFree(void)
{
	return sgpfnFreeHardware;
}

BOOL WINAPI CelFileFreeHardware(D2CELFILE *ptFile)
{
	D2CELSLOT *lpParent;
	D2CEL **pptCel;
	DWORD i;

	if (!ptFile)
		return FALSE;
#line 1422
	D2ASSERT(ptFile->dwVersion == CELFILE_VERSION);
	lpParent = ptFile->aptCels[0]->ptSlot;
#line 1426
	D2ASSERT(lpParent);
	for (i = 0, pptCel = &ptFile->aptCels[i]; i < ptFile->dwNumDirections * ptFile->dwCelsPerDirection; i++, pptCel++)
	{
		if (*pptCel && (*pptCel)->ptSlot)
		{
			if (sgpfnFreeHardware)
				sgpfnFreeHardware((*pptCel)->ptSlot);
			(*pptCel)->ptSlot = NULL;
		}
	}
#line 1465
	D2FREE2(lpParent);
	return TRUE;
}

D2CELSLOT * WINAPI CelGetSlot(D2CEL *ptCel)
{
#line 1478
	D2ASSERT(ptCel);
	return ptCel->ptSlot;
}

void __fastcall CelListCheck(CELLIST *pList)
{
}
