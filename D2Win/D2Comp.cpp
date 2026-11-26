#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2Comp.cpp"
#include "D2Win.h"
#include "D2Macros.h"
#include "Archive.h"

HD2ARCHIVE __fastcall D2WinArchiveGetArchive(void);

struct D2COMPCODE
{
	DWORD		field_00;
	DWORD		field_04;
	DWORD		field_08;
};

struct D2COMPRGB
{
	BYTE		field_00;
	BYTE		field_01;
	BYTE		field_02;
};

struct D2COMPEXTRASTRING
{
	Unicode		wszText[256];
	int			nWidth;
	int			nHeight;
};

static DWORD sgadwClassTokens[21] =
{
	538987841, 538988371, 538985806, 538984784, 538984770, 538988370, 538986578, 538986578,
	538980687, 538980943, 538981199, 538981455, 538985549, 538982735, 538981711, 538982223,
	538982479, 538981967, 538980431, 538985300, 538988370
};
static DWORD sgdwClassTokens = 21;

static DWORD sgadwModeTokens[20] =
{
	538989636, 538989902, 538987607, 538988114, 538986567, 538988116, 538990420, 538980673,
	538980929, 538987586, 538985299, 538986580, 538987339, 538980691, 538980947, 538981203,
	538981459, 538985540, 538986567, 538986567
};
static DWORD sgdwModeTokens = 20;

static DWORD sgadwComponentTokens[NUM_COMPONENTS] =
{
	538985544, 538989140, 538986316, 538984786, 538984780, 538986578, 538986572, 538986579,
	538980691, 538980947, 538981203, 538981459, 538981715, 538981971, 538982227, 538982483
};
static DWORD sgdwComponentTokens = NUM_COMPONENTS;
static DWORD sgadwArmorClassTokens[3] =
{
	544500076, 543450477, 544831080
};
static DWORD sgdwArmorClassTokens = 3;

static DWORD sgadwWClassTokens[13] =
{
	0, 543716456, 544499761, 544499762, 544434225, 544434226, 544698210, 544694904,
	543585395, 544434737, 544500273, 544437041, 544502577
};
static DWORD sgdwWClassTokens = 13;

static D2COMPRGB sgatEmblemColorsA[10] =
{
	{ 188, 0, 0 }, { 224, 112, 32 }, { 252, 212, 92 }, { 252, 228, 164 },
	{ 244, 244, 244 }, { 184, 184, 184 }, { 116, 116, 116 }, { 76, 108, 172 },
	{ 88, 144, 76 }, { 178, 124, 88 }
};
static D2COMPRGB sgatEmblemColorsB[10] =
{
	{ 92, 0, 0 }, { 144, 0, 0 }, { 48, 28, 16 }, { 92, 36, 8 },
	{ 4, 36, 16 }, { 36, 92, 36 }, { 0, 0, 88 }, { 40, 40, 120 },
	{ 64, 64, 64 }, { 0, 0, 0 }
};

static D2COMPCODE sgatCodes[283] =
{
	{ 0,  0,  1 },     { 544500076,  0,  1 },
	{ 543450477,  0,  1 },     { 544831080,  0,  1 },
	{ 544235875,  0, 37 },     { 544238451,  0, 37 },
	{ 544042088,  0, 37 },     { 543975526,  0, 37 },
	{ 544041063,  0, 37 },     { 544109155,  0, 37 },
	{ 543912813,  0, 37 },     { 543518819,  0,  3 },
	{ 543911011,  0,  3 },     { 543321970,  0,  3 },
	{ 543782257,  0,  3 },     { 543253868,  0,  3 },
	{ 543255656,  0,  3 },     { 544568435,  0,  3 },
	{ 543649394,  0,  3 },     { 543974259,  0,  3 },
	{ 544106595,  0,  3 },     { 544436834,  0,  3 },
	{ 543977587,  0,  3 },     { 544500848,  0,  3 },
	{ 543452262,  0,  3 },     { 543716455,  0,  3 },
	{ 543978854,  0,  3 },     { 543389026,  0,  2 },
	{ 543650412,  0,  2 },     { 544500075,  0,  2 },
	{ 544698228,  0,  2 },     { 543975276,  0, 16 },
	{ 543975277,  0, 16 },     { 543975272,  0, 16 },
	{ 544498284,  0, 15 },     { 544498285,  0, 15 },
	{ 544498280,  0, 15 },     { 543973996,  0, 19 },
	{ 543973997,  0, 19 },     { 543973992,  0, 19 },
	{ 544041058,  0, 37 },     { 543716194,  0,  2 },
	{ 543912051,  0,  2 },     { 543452791,  4, 25 },
	{ 544110457,  4, 25 },     { 544110434,  4, 25 },
	{ 543321187,  4, 29 },     { 544760168,  4, 43 },
	{ 543454067,  4, 30 },     { 544039795,  4, 30 },
	{ 543383917,  4, 36 },     { 543520865,  4, 28 },
	{ 543386726,  4, 30 },     { 543454060,  4, 30 },
	{ 543255654,  4, 36 },     { 544040040,  4, 31 },
	{ 544041079,  4, 31 },     { 544436835,  4, 30 },
	{ 544435302,  4, 30 },     { 543977575,  2, 38 },
	{ 544436327,  2, 38 },     { 543977583,  2, 38 },
	{ 544436335,  2, 38 },     { 544368484,  2, 32 },
	{ 540226404,  2, 32 },     { 540226407,  2, 32 },
	{ 544368996,  2, 32 },     { 544629098,  2, 33 },
	{ 544631911,  2, 33 },     { 543975792,  2, 33 },
	{ 544042083,  5, 30 },     { 543454055,  5, 30 },
	{ 544370803,  3, 33 },     { 543781492,  3, 33 },
	{ 543910256,  3, 33 },     { 544370803,  3, 33 },
	{ 544694899,  6, 27 },     { 544694892,  6, 27 },
	{ 543318643,  6, 27 },     { 543318636,  6, 27 },
	{ 544502643,  8, 26 },     { 544502636,  8, 26 },
	{ 544502627,  8, 26 },     { 544502626,  8, 26 },
	{ 544760172,  8, 28 },     { 544826227,  8, 34 },
	{ 544762215,  8, 28 },     { 544765026,  8, 28 },
	{ 543973736,  8, 34 },     { 544563565,  8, 31 },
	{ 544760176,  8, 34 },     { 543973736,  8, 34 },
	{ 543324264,  7, 35 },     { 543324268,  7, 35 },
	{ 544760180,  4, 43 },     { 543387763,  4, 29 },
	{ 544367219,  4, 30 },     { 544502637,  4, 36 },
	{ 544236403,  4, 24 },     { 544043111,  2, 38 },
	{ 544043119,  2, 38 },     { 543583092,  2, 42 },
	{ 543452258,  2, 32 },     { 543454050,  4, 30 },
	{ 544699234,  4, 30 },     { 544434226,  5, 30 },
	{ 544109154,  3, 33 },     { 544694888,  6, 27 },
	{ 544694883,  6, 27 },     { 543324019,  6, 27 },
	{ 543324012,  6, 27 },     { 544502647,  8, 26 },
	{ 544760162,  8, 28 },     { 544366946,  8, 34 },
	{ 544760167,  8, 28 },     { 543324274,  7, 35 },
	{ 543324269,  7, 35 },     { 544498298,  0,  1 },
	{ 544366945,  0,  3 },     { 544240748,  0,  3 },
	{ 543976819,  0,  2 },     { 544437351,  0,  3 },
	{ 544633185,  0, 40 },     { 544633187,  0, 40 },
	{ 543974006,  0, 19 },     { 543974004,  0, 19 },
	{ 543975286,  0, 16 },     { 543975284,  0, 16 },
	{ 544498294,  0, 15 },     { 544498292,  0, 15 },
	{ 543321190,  5, 30 },     { 543780973,  4, 28 },
	{ 543973730,  4, 43 },     { 543647084,  4, 29 },
	{ 544500849,  0,  3 },     { 544760114,  4, 28 },
	{ 544760183,  4, 28 },     { 544110439,  4, 36 },
	{ 543255911,  8, 36 },     { 543454071,  4, 30 },
	{ 544434535,  5, 30 },     { 543452258,  2, 32 },
	{ 543583074,  2, 32 },     { 544240499,  2, 33 },
	{ 544240500,  2, 33 },     { 543518835,  3, 33 },
	{ 544567158,  8, 34 },     { 543388535,  8, 34 },
	{ 543781483,  2, 32 },     { 544501875,  2, 33 },
	{ 543388519,  2, 24 },     { 544240503,  2, 24 },
	{ 544502632,  3, 26 },     { 543585133,  3, 26 },
	{ 543712872,  4, 36 },     { 543254585,  4, 28 },
	{ 544760121,  4, 28 },     { 543240761,  4, 28 },
	{ 544238905,  4, 28 },     { 543258425,  4, 28 },
	{ 543255609,  8, 28 },     { 543253049,  8, 28 },
	{ 544498233,  8, 28 },     { 543254329,  8, 28 },
	{ 543778617,  8, 28 },     { 544110393,  4, 25 },
	{ 544700729,  4, 25 },     { 544694841,  4, 25 },
	{ 544696121,  4, 25 },     { 543974201,  4, 36 },
	{ 543388473,  4, 24 },     { 544436537,  4, 24 },
	{ 544438073,  4, 24 },     { 544240441,  4, 36 },
	{ 543255865,  4, 36 },     { 544501049,  4, 36 },
	{ 543974969,  4, 36 },     { 543717177,  4, 36 },
	{ 540634425,  8, 36 },     { 544040761,  8, 36 },
	{ 544437049,  4, 30 },     { 544043833,  4, 30 },
	{ 543322937,  4, 30 },     { 543385145,  4, 30 },
	{ 544367417,  4, 30 },     { 544432697,  4, 30 },
	{ 544435257,  4, 30 },     { 543455033,  4, 30 },
	{ 543699513,  4, 30 },     { 544039737,  4, 30 },
	{ 544433977,  4, 30 },     { 540631609,  4, 30 },
	{ 543319609,  4, 30 },     { 543450937,  4, 30 },
	{ 543646777,  2, 32 },     { 543777849,  2, 32 },
	{ 544369465,  2, 32 },     { 543973945,  2, 32 },
	{ 543913017,  2, 42 },     { 543257657,  4, 43 },
	{ 543908409,  2, 42 },     { 540566073,  4, 43 },
	{ 543255097,  2, 33 },     { 543780921,  2, 33 },
	{ 540635961,  2, 33 },     { 543975225,  2, 33 },
	{ 544437305,  2, 33 },     { 544371513,  3, 33 },
	{ 544371769,  3, 33 },     { 544367161,  3, 33 },
	{ 544502585,  3, 33 },     { 540635193,  3, 33 },
	{ 540500537,  8, 34 },     { 544175673,  8, 34 },
	{ 540570425,  8, 34 },     { 543256633,  8, 34 },
	{ 540633145,  8, 34 },     { 543389497,  8, 34 },
	{ 544437048,  8, 26 },     { 544435256,  8, 26 },
	{ 544432952,  8, 26 },     { 544432696,  8, 26 },
	{ 544438072,  8, 26 },     { 543322936,  6, 27 },
	{ 543320120,  6, 27 },     { 543321144,  6, 27 },
	{ 543318840,  6, 27 },     { 540570424,  6, 27 },
	{ 540568632,  6, 27 },     { 544699192,  6, 27 },
	{ 544697400,  6, 27 },     { 544762936,  7, 35 },
	{ 544763192,  7, 35 },     { 544761912,  7, 35 },
	{ 544764472,  7, 35 },     { 544235896,  0, 37 },
	{ 544238456,  0, 37 },     { 544042104,  0, 37 },
	{ 543975544,  0, 37 },     { 544041080,  0, 37 },
	{ 544109176,  0, 37 },     { 543912824,  0, 37 },
	{ 543782264,  0,  3 },     { 543253880,  0,  3 },
	{ 543255672,  0,  3 },     { 544568440,  0,  3 },
	{ 543649400,  0,  3 },     { 543974264,  0,  3 },
	{ 544106616,  0,  3 },     { 544436856,  0,  3 },
	{ 543977592,  0,  3 },     { 544500856,  0,  3 },
	{ 543452280,  0,  3 },     { 543716472,  0,  3 },
	{ 543978872,  0,  3 },     { 544366968,  0,  3 },
	{ 544240760,  0,  3 },     { 543389048,  0,  2 },
	{ 543976824,  0,  2 },     { 543650424,  0,  2 },
	{ 544500088,  0,  2 },     { 544698232,  0,  2 },
	{ 544437368,  0,  2 },     { 543648888,  0, 16 },
	{ 543651448,  0, 16 },     { 543649144,  0, 16 },
	{ 543650936,  0, 16 },     { 543647864,  0, 16 },
	{ 543321208,  0, 15 },     { 543323768,  0, 15 },
	{ 543321464,  0, 15 },     { 543323256,  0, 15 },
	{ 543320184,  0, 15 },     { 543321210,  0, 19 },
	{ 543323770,  0, 19 },     { 543321466,  0, 19 },
	{ 543323258,  0, 19 },     { 543320186,  0, 19 },
	{ 540633208,  0, 37 },     { 543716216,  0,  2 },
	{ 543912056,  0,  2 },     { 540108401,  4, 36 },
	{ 540173937,  4, 36 }
};
static DWORD sgdwCodes = 283;
static DWORD sgdwCompUnread = 1;

#define COMP_CODE_TABLE_MESSAGE(szMessage)   \
	(const char *)FogDataArraysSprintf((szMessage), \
		"Code table is the wrong size (appears to have %d entries, should be %d entries)", sgdwCodes, 283)

static const BYTE sgabGameDirections[7][CELFILE_NUMGAMEDIRECTIONS] =
{
	{ 0 },
	{ 0 },
	{ 0, 32 },
	{ 0, 16, 32, 48 },
	{ 0, 8, 16, 24, 32, 40, 48, 56 },
	{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60 },
	{ 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
	  32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62 }
};

typedef D2COMPPALETTE D2COMPPALETTES[9];
typedef BYTE D2COMPEMBLEMCOLORS[10];
static D2COMPPALETTES sgatPalettes = { 0 };
static D2COMPEMBLEMCOLORS sgabEmblemColorsB = { 0 };
static D2COMPEMBLEMCOLORS sgabEmblemColorsA = { 0 };
static D2COMPOSITEUNIT *sgptUnits = 0;
static TCOFNODE *sgptCofNodes = 0;
static DWORD sgdwNextFrameTick = 0;
static HCELFILE sghSquelchCelFile = { 0 };
static int sgnSquelchFrames = 0;
static int sgnSquelchFrame = 0;
static HCELFILE sghSelectedCelFile = { 0 };
static int sgnSelectedFrames = 0;
static int sgnSelectedFrame = 0;

static BYTE * __fastcall D2CompGetCofData(TGFXINFO *ptGfxInfo, DWORD eClass, DWORD eMode, DWORD *pdwArg4,
	DWORD *pdwArg5, DWORD dwWClassToken);
static BOOL __fastcall D2CompLoadCof(D2COMPOSITEUNIT *ptCompositeUnit);
static DWORD __fastcall D2CompGetWeaponClass(const BYTE *pbCodes);
static void __fastcall D2CompLoadPalette(const char *pszName, int nPalette);
const char * __stdcall D2CompGetTitle(int eClass, int nRank);

static BYTE *D2CompCofNodeGetData(TCOFNODE *ptCofNode)
{
	BYTE *lpbCofData;

	lpbCofData = ptCofNode->lpbCofFile;
#line 642
	D2ASSERT(lpbCofData);
	return lpbCofData;
}

static BYTE *D2CompReadCof(const char *pszPath, DWORD *pdwSize)
{
	BYTE *lpbCofData;

#line 656
	lpbCofData = (BYTE *)ArchiveAllocBufferAndRead(D2WinArchiveGetArchive(), pszPath, pdwSize, D2_THIS_FILE, __LINE__);
#line 657
	D2ASSERT(lpbCofData);
#line 666
	D2ASSERT(((PTCOFHEADER)lpbCofData)->bVersion == COF_VERSION);
	return lpbCofData;
}

static BYTE *D2CompGetDrawOrder(BYTE *lpbCofData, int nDirection, int nFrame)
{
	BYTE *lpbFirstByte;
	PTCOFHEADER ptHeader;

	D2ASSERT(lpbCofData);
	ptHeader = (PTCOFHEADER)lpbCofData;
	lpbFirstByte = lpbCofData + sizeof(TCOFHEADER)
		+ (ptHeader->bNumFrames * nDirection + nFrame + (int)sizeof(TCOFLAYER)) * ptHeader->bNumLayers
		+ ptHeader->bNumFrames;
#line 685
	D2ASSERT(lpbFirstByte);
	return lpbFirstByte;
}

static TCOFNODE *D2CompAddCofNode(DWORD eClass, DWORD eMode, DWORD dwWClassToken, const char *pszPath)
{
	TCOFNODE *ptCofNode;
	TCOFNODE *ptNode;

	for (ptNode = sgptCofNodes; ptNode; ptNode = ptNode->ptNext)
	{
		if (ptNode->dwMode == eMode && ptNode->dwClassId == eClass
			&& ptNode->dwWClassToken == dwWClassToken)
		{
			ptNode->nRefCount++;
			break;
		}
	}
	if (ptNode)
		return ptNode;
#line 732
	ptCofNode = (TCOFNODE *)D2ALLOC2(sizeof(TCOFNODE));
	memset(ptCofNode, 0, sizeof(TCOFNODE));
	ptCofNode->dwMode = eMode;
	ptCofNode->dwClassId = eClass;
	ptCofNode->nRefCount++;
	ptCofNode->dwWClassToken = dwWClassToken;
	ptCofNode->ptNext = sgptCofNodes;
	ptCofNode->ptPrev = NULL;
	if (sgptCofNodes)
		sgptCofNodes->ptPrev = ptCofNode;
	sgptCofNodes = ptCofNode;
	ptCofNode->lpbCofFile = D2CompReadCof(pszPath, &ptCofNode->dwFileSize);
	return ptCofNode;
}

static void D2CompRemoveCofNode(TCOFNODE *ptRemCofNode)
{
	TCOFNODE *ptNext;
	TCOFNODE *ptPrev;

#line 762
	D2ASSERT(ptRemCofNode);
	if (!sgptCofNodes)
		return;
	if (--ptRemCofNode->nRefCount > 0)
		return;
	ptPrev = ptRemCofNode->ptPrev;
	ptNext = ptRemCofNode->ptNext;
#line 774
	D2FREE2(ptRemCofNode->lpbCofFile);
	ptRemCofNode->lpbCofFile = NULL;
	if (!ptPrev)
	{
		sgptCofNodes = ptNext;
		if (ptNext)
			ptNext->ptPrev = NULL;
	}
	else if (!ptNext)
		ptPrev->ptNext = NULL;
	else
	{
		ptPrev->ptNext = ptNext;
		ptNext->ptPrev = ptPrev;
	}
#line 799
	D2FREE2(ptRemCofNode);
}

static BYTE D2CompGetGameDirection(BYTE bDirection, DWORD dwDirections)
{
	BYTE bDir;

#line 1041
	D2ASSERT(bDirection < CELFILE_NUMGAMEDIRECTIONS);
	bDir = sgabGameDirections[ArchiveBitFindFirstSet(dwDirections)][bDirection];
#line 1043
	D2ASSERT(bDir <= CELFILE_NUMGAMEDIRECTIONS);
	return bDir;
}

void __fastcall sGetCelContextInfo(D2COMPOSITEUNIT *hCompositeUnit, D2CELCONTEXT *ptCelContext);

static int D2CompCountCels(D2COMPOSITEUNIT *hCompositeUnit, BYTE *lpbCofData, BYTE *lpbFirstByte,
	int *pnMax, BYTE bDirection, int nFrame)
{
	D2CELCONTEXT tCelContext;
	int eComponent;
	int nCels;
	int nValue;
	int i;

	nCels = 0;
	for (i = 0; i < (int)((PTCOFHEADER)lpbCofData)->bNumLayers; i++)
	{
		eComponent = lpbFirstByte[i];
#line 1065
		D2ASSERT(eComponent < NUM_COMPONENTS);
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = NULL;
		tCelContext.dwDirection = bDirection;
		tCelContext.bComponent = (BYTE)eComponent;
		tCelContext.dwFrame = nFrame;
		sGetCelContextInfo(hCompositeUnit, &tCelContext);
		tCelContext.dwFlags |= 2;
		if (D2CMPSpriteCacheGetContextCel(&tCelContext, 0, 1) && tCelContext.hCel)
		{
			nValue = D2CMPCelGetOffsetY(tCelContext.hCel) + hCompositeUnit->nX;
			if (nValue > *pnMax)
				*pnMax = nValue;
			nCels++;
		}
	}
	return nCels;
}

static void D2CompAddExtraString(D2COMPEXTRASTRING *ptStrings, int *nStrCount, const WORD *pwszText,
	int *pnMaxWidth, int *pnTotalHeight)
{
	D2COMPEXTRASTRING *ptString;
	int nWidth;

	ptString = &ptStrings[*nStrCount];
#line 1458
	D2ASSERT(*nStrCount < MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS);
	(void)D2LangUnicodestrcpy(ptString->wszText, pwszText);
	D2WinFontGetTextSize((const WORD *)ptString->wszText, &ptString->nWidth, &ptString->nHeight);
	nWidth = ptString->nWidth + 6;
	if (*pnMaxWidth <= nWidth)
		*pnMaxWidth = nWidth;
	*pnTotalHeight += ptString->nHeight;
	(*nStrCount)++;
}

static void D2CompUnlinkAndFree(D2COMPOSITEUNIT *hCompositeUnit)
{
	D2COMPOSITEUNIT *ptPrev = hCompositeUnit->ptPrev;
	D2COMPOSITEUNIT *ptNext = hCompositeUnit->ptNext;

	if (!ptPrev)
	{
		sgptUnits = ptNext;
		if (ptNext)
			ptNext->ptPrev = NULL;
	}
	else
	{
		ptPrev->ptNext = ptNext;
		if (ptNext)
			ptNext->ptPrev = ptPrev;
	}
#line 1710
	D2FREE2(hCompositeUnit);
}

static void D2CompAllocGfxInfo(D2COMPOSITEUNIT *ptUnit)
{
	TGFXINFO *ptGfxInfo;

#line 1722
	ptGfxInfo = (TGFXINFO *)D2ALLOC2(sizeof(TGFXINFO));
#line 1723
	D2ASSERT(ptGfxInfo);
	ptGfxInfo->ptCofNode = NULL;
	ptUnit->ptGfxInfo = ptGfxInfo;
}

void __cdecl operator delete(void *p)
{
	if (p)
		StormSMemFree(p, "delete", -1, 0);
}

void * __cdecl operator new(unsigned int nBytes)
{
	return StormSMemAlloc(nBytes, "new", -1, 0);
}

void __fastcall sGetCelContextInfo(D2COMPOSITEUNIT *hCompositeUnit, D2CELCONTEXT *ptCelContext)
{
	TGFXINFO *ptGfxInfo;
	BYTE *lpbCofData;
	TCOFLAYER *ptLayer;
	int eComponent;
	DWORD dwArg4;
	DWORD dwArg5;
	DWORD i;
	char szToken[4];

	eComponent = ptCelContext->bComponent;
	ptCelContext->dwClassId = hCompositeUnit->dwClassId;
	ptCelContext->dwMode = hCompositeUnit->dwMode;
	ptCelContext->tUnitInfo.dwComponentToken = sgadwComponentTokens[eComponent];
	ptCelContext->tUnitInfo.dwWClassToken = sgadwWClassTokens[hCompositeUnit->dwWeaponClass];
	ptCelContext->tUnitInfo.dwUnitToken = sgadwClassTokens[hCompositeUnit->dwClassId];
	ptCelContext->tUnitInfo.dwVariantToken = 544500076;
	if (hCompositeUnit->adwGearTokens[eComponent])
		ptCelContext->tUnitInfo.dwVariantToken = hCompositeUnit->adwGearTokens[eComponent];
	ptGfxInfo = hCompositeUnit->ptGfxInfo;
#line 931
	D2ASSERT(ptGfxInfo);
	dwArg5 = 0;
	dwArg4 = 0;
	lpbCofData = D2CompGetCofData(ptGfxInfo, hCompositeUnit->dwClassId, hCompositeUnit->dwMode, &dwArg4,
		&dwArg5, sgadwWClassTokens[hCompositeUnit->dwWeaponClass]);
#line 935
	D2ASSERT(lpbCofData);
	ptLayer = (TCOFLAYER *)(lpbCofData + sizeof(TCOFHEADER));
	for (i = 0; i < ((PTCOFHEADER)lpbCofData)->bNumLayers; ptLayer++, i++)
	{
#line 943
		D2ASSERT(eComponent < NUM_COMPONENTS);
		if (ptLayer->bComponent == eComponent)
		{
			if ((int)ptCelContext->dwClassId < 5)
				ptCelContext->dwUnitType = 0;
			else
				ptCelContext->dwUnitType = 1;
			ptCelContext->tUnitInfo.dwModeToken = sgadwModeTokens[ptCelContext->dwMode];
			if (!ptCelContext->tUnitInfo.dwVariantToken)
#line 959
				D2FATAL("[sGetCelContextInfo] This should not happen. Please tell Jon Morin!\n");
			*(DWORD *)szToken = 0;
			szToken[0] = ptLayer->szWClass[0] ? ptLayer->szWClass[0] : ' ';

			szToken[1] = ptLayer->szWClass[1] ? ptLayer->szWClass[1] : ' ';

			szToken[2] = ptLayer->szWClass[2] ? ptLayer->szWClass[2] : ' ';

			szToken[3] = ptLayer->szWClass[3] ? ptLayer->szWClass[3] : ' ';

			ptCelContext->tUnitInfo.dwWClassToken = *(DWORD *)szToken;
			break;
		}
	}
#line 971
	D2ASSERT(ptCelContext->tUnitInfo.dwComponentToken);
	D2ASSERT(ptCelContext->tUnitInfo.dwUnitToken);
	D2ASSERT(ptCelContext->tUnitInfo.dwWClassToken);
	if (!ptCelContext->tUnitInfo.dwVariantToken)
		ptCelContext->tUnitInfo.dwVariantToken = 544766072;
	if (!ptCelContext->tUnitInfo.dwModeToken)
		ptCelContext->tUnitInfo.dwModeToken = 544766072;
}

static inline TUNITCOFNODE *D2CompFindUnitCofNode(TGFXINFO *ptGfxInfo, DWORD eMode, DWORD dwWClassToken)
{
	TUNITCOFNODE *ptUnitCofNode;

#line 875
	D2ASSERT(ptGfxInfo);
	for (ptUnitCofNode = ptGfxInfo->ptCofNode; ptUnitCofNode; ptUnitCofNode = ptUnitCofNode->ptNext)
	{
		if (ptUnitCofNode->ptCofNode->dwMode == eMode
			&& ptUnitCofNode->ptCofNode->dwWClassToken == dwWClassToken)
			return ptUnitCofNode;
	}
	return NULL;
}

static inline void D2CompGetCofPath(DWORD eClass, DWORD eMode, DWORD dwWClassToken, char *pszPath)
{
	char szWClass[4];
	char szMode[4];
	char szClass[4];
	const char *pszToken;
	DWORD dwClassToken;
	DWORD dwModeToken;
	HANDLE hFile;

	dwClassToken = sgadwClassTokens[eClass];
	dwModeToken = sgadwModeTokens[eMode];
	pszToken = (const char *)&dwWClassToken;
	szWClass[0] = pszToken[0] == ' ' ? 0 : pszToken[0];
	szWClass[1] = pszToken[1] == ' ' ? 0 : pszToken[1];
	szWClass[2] = pszToken[2] == ' ' ? 0 : pszToken[2];
	szWClass[3] = 0;
	pszToken = (const char *)&dwClassToken;
	szClass[0] = pszToken[0] == ' ' ? 0 : pszToken[0];
	szClass[1] = pszToken[1] == ' ' ? 0 : pszToken[1];
	szClass[2] = pszToken[2] == ' ' ? 0 : pszToken[2];
	szClass[3] = 0;
	pszToken = (const char *)&dwModeToken;
	szMode[0] = pszToken[0] == ' ' ? 0 : pszToken[0];
	szMode[1] = pszToken[1] == ' ' ? 0 : pszToken[1];
	szMode[2] = pszToken[2] == ' ' ? 0 : pszToken[2];
	szMode[3] = 0;
	wsprintfA(pszPath, "%s\\%s\\COF\\%s%s%s.COF", eClass < 5 ? "DATA\\GLOBAL\\CHARS" : "DATA\\GLOBAL\\MONSTERS",
		szClass, szClass, szMode, szWClass);
	if (FogSafesockOpenStormFile(pszPath, &hFile))
	{
		FogSafesockCloseStormFile(hFile);
		return;
	}
	*pszPath = 0;
}

static BYTE * __fastcall D2CompGetCofData(TGFXINFO *ptGfxInfo, DWORD eClass, DWORD eMode, DWORD *pdwArg4,
	DWORD *pdwArg5, DWORD dwWClassToken)
{
	TUNITCOFNODE *ptUnitCofNode;
	TCOFNODE *ptCofNode;

	ptUnitCofNode = D2CompFindUnitCofNode(ptGfxInfo, eMode, dwWClassToken);
	if (!ptUnitCofNode)
	{
		char szPath[MAX_PATH] = { 0 };

		D2CompGetCofPath(eClass, eMode, dwWClassToken, szPath);
		if (!szPath[0])
			return NULL;
#line 889
		ptUnitCofNode = (TUNITCOFNODE *)D2ALLOC2(sizeof(TUNITCOFNODE));
		D2ASSERT(ptUnitCofNode);
		memset(ptUnitCofNode, 0, sizeof(TUNITCOFNODE));
		ptCofNode = D2CompAddCofNode(eClass, eMode, dwWClassToken, szPath);
#line 893
		D2ASSERT(ptCofNode);
		ptUnitCofNode->ptCofNode = ptCofNode;
		ptUnitCofNode->ptNext = ptGfxInfo->ptCofNode;
		ptGfxInfo->ptCofNode = ptUnitCofNode;
	}
#line 899
	D2ASSERT(ptUnitCofNode->ptCofNode->lpbCofFile);
	return ptUnitCofNode->ptCofNode->lpbCofFile;
}

void __fastcall D2CompUnitDrawUnder(D2COMPOSITEUNIT *hCompositeUnit)
{
	TGFXINFO *ptGfxInfo;
	TUNITCOFNODE *ptUnitCofNode;
	BYTE *lpbCofData;
	BYTE *lpbFirstByte;
	int eComponent;
	int nFrame;
	int nX;
	int nY;
	DWORD dwToken;
	DWORD eMode;
	DWORD j;
	int nMax;

#line 1102
	D2ASSERT(hCompositeUnit);
	if (!(hCompositeUnit->dwFlags & gdwBitMasks[0]) || !(hCompositeUnit->dwFlags & gdwBitMasks[2]))
		return;
	nX = hCompositeUnit->nX - 2;
	nY = hCompositeUnit->nY;
	ptGfxInfo = hCompositeUnit->ptGfxInfo;
#line 1118
	D2ASSERT(ptGfxInfo);
	eMode = hCompositeUnit->dwMode;
	dwToken = sgadwWClassTokens[hCompositeUnit->dwWeaponClass];
	for (ptUnitCofNode = ptGfxInfo->ptCofNode; ptUnitCofNode; ptUnitCofNode = ptUnitCofNode->ptNext)
	{
		if (ptUnitCofNode->ptCofNode->dwMode == eMode
			&& ptUnitCofNode->ptCofNode->dwWClassToken == dwToken)
			break;
	}
	if (!ptUnitCofNode)
		return;
	lpbCofData = ptUnitCofNode->ptCofNode->lpbCofFile;
#line 1126
	D2ASSERT(lpbCofData);
	nFrame = hCompositeUnit->nAnimPos >> 8;
	lpbFirstByte = D2CompGetDrawOrder(lpbCofData, hCompositeUnit->bDirection, nFrame);
	nMax = 0;
	if (D2CompCountCels(hCompositeUnit, lpbCofData, lpbFirstByte, &nMax, hCompositeUnit->bDirection, nFrame) <= 0)
		return;
	j = 0;
	for (; j < ((PTCOFHEADER)lpbCofData)->bNumLayers; j++)
	{
		D2CELCONTEXT tCelContext;

		eComponent = lpbFirstByte[j];
#line 1141
		D2ASSERT(eComponent < NUM_COMPONENTS);
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.bComponent = (BYTE)eComponent;
		tCelContext.dwDirection = hCompositeUnit->bDirection;
		tCelContext.hCelFile = NULL;
		tCelContext.dwFrame = nFrame;
		sGetCelContextInfo(hCompositeUnit, &tCelContext);
		tCelContext.dwFlags |= 2;
		D2gfxScaleDrawSpriteShadow(&tCelContext, nX, nY);
	}
}

static inline BYTE *D2CompGetColorShift(int nColor)
{
	int nPalette;
	int nShade;

	nPalette = nColor >> 5;
	nShade = nColor & 31;
	if (nShade >= 0 && nShade < 21 && nPalette >= 0 && nPalette < 9 && nPalette != 0
		&& (nPalette <= 2 || nPalette > 4))
		return sgatPalettes[nPalette].aabShifts[nShade];
	return NULL;
}

BOOL __fastcall D2CompUnitDraw(D2COMPOSITEUNIT *hCompositeUnit, DWORD dwColor)
{
	TGFXINFO *ptGfxInfo;
	TUNITCOFNODE *ptUnitCofNode;
	BYTE *lpbCofData;
	BYTE *lpbFirstByte;
	D2CELCONTEXT tCelContext;
	BYTE abEmblemShift[256];
	BYTE *pbShift;
	DWORD dwDrawMode;
	DWORD dwTick;
	int eComponent;
	int nFrame;
	int nColor;
	BYTE bDir;
	DWORD dwToken;
	DWORD eMode;
	int i;

#line 1166
	D2ASSERT(hCompositeUnit);
	if (!(hCompositeUnit->dwFlags & gdwBitMasks[0]))
		return FALSE;
	if (hCompositeUnit->dwFlags & gdwBitMasks[10])
	{
#line 1177
		D2ASSERT(sghSelectedCelFile);
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = sghSelectedCelFile;
		tCelContext.dwFrame = sgnSelectedFrame;
		tCelContext.dwDirection = 0;
		D2gfxScaleDrawCel(&tCelContext, hCompositeUnit->nX + 5, hCompositeUnit->nY - 5,
			-1, 3, 0);
	}
	if (hCompositeUnit->hEmblemCelFile)
	{
		abEmblemShift[1] = hCompositeUnit->bEmblemColorA;
		abEmblemShift[2] = hCompositeUnit->bEmblemColorB;
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = hCompositeUnit->hEmblemCelFile;
		tCelContext.dwDirection = 0;
		tCelContext.dwFrame = 0;
		tCelContext.field_10 = 1;
		D2gfxScaleDrawCel(&tCelContext, hCompositeUnit->nX - 20, hCompositeUnit->nY - 60,
			-1, 5, abEmblemShift);
	}
	ptGfxInfo = hCompositeUnit->ptGfxInfo;
#line 1213
	D2ASSERT(ptGfxInfo);
	eMode = hCompositeUnit->dwMode;
	dwToken = sgadwWClassTokens[hCompositeUnit->dwWeaponClass];
	for (ptUnitCofNode = ptGfxInfo->ptCofNode; ptUnitCofNode; ptUnitCofNode = ptUnitCofNode->ptNext)
	{
		if (ptUnitCofNode->ptCofNode->dwMode == eMode
			&& ptUnitCofNode->ptCofNode->dwWClassToken == dwToken)
			break;
	}
	if (!ptUnitCofNode)
		return FALSE;
	lpbCofData = ptUnitCofNode->ptCofNode->lpbCofFile;
#line 1222
	D2ASSERT(lpbCofData);
	nFrame = hCompositeUnit->nAnimPos >> 8;
	lpbFirstByte = D2CompGetDrawOrder(lpbCofData, hCompositeUnit->bDirection, nFrame);
	if ((hCompositeUnit->dwFlags & gdwBitMasks[5]) && (hCompositeUnit->dwFlags & gdwBitMasks[4]))
		dwDrawMode = 1;
	else
		dwDrawMode = 5;
	for (i = 0; i < (int)((PTCOFHEADER)lpbCofData)->bNumLayers; i++)
	{
		eComponent = lpbFirstByte[i];
		memset(&tCelContext, 0, sizeof(tCelContext));
		bDir = D2CompGetGameDirection(hCompositeUnit->bDirection, hCompositeUnit->bNumDirections);
		tCelContext.hCelFile = NULL;
		tCelContext.dwDirection = bDir;
		tCelContext.dwFrame = nFrame;
		tCelContext.bComponent = (BYTE)eComponent;
		sGetCelContextInfo(hCompositeUnit, &tCelContext);
		tCelContext.dwFlags |= 2;
		nColor = hCompositeUnit->abColors[eComponent];
		pbShift = NULL;
		if (nColor != 255)
			pbShift = D2CompGetColorShift(nColor);
		D2gfxScaleDrawCel(&tCelContext, hCompositeUnit->nX, hCompositeUnit->nY, dwColor,
			dwDrawMode, pbShift);
	}
	hCompositeUnit->nAnimPos += hCompositeUnit->nAnimRate;
	if ((hCompositeUnit->nAnimPos >> 8) >= hCompositeUnit->nFrames - 1)
		hCompositeUnit->nAnimPos = 0;
	dwTick = GetTickCount();
	if (dwTick > sgdwNextFrameTick)
	{
		sgdwNextFrameTick = dwTick + 66;
		if (++sgnSquelchFrame >= sgnSquelchFrames)
			sgnSquelchFrame = 0;
		if (++sgnSelectedFrame >= sgnSelectedFrames)
			sgnSelectedFrame = 0;
	}
	if (hCompositeUnit->dwFlags & gdwBitMasks[9])
	{
#line 1291
		D2ASSERT(sghSquelchCelFile);
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = sghSquelchCelFile;
		tCelContext.dwFrame = sgnSquelchFrame;
		tCelContext.dwDirection = 0;
		D2gfxScaleDrawCel(&tCelContext, hCompositeUnit->nX, hCompositeUnit->nY + 10,
			-1, 3, 0);
	}
	return TRUE;
}

static void D2CompDrawCutName(D2COMPOSITEUNIT *hCompositeUnit, Unicode *pwszName, int nY, BOOL fFlag5)
{
	char szName[512];
	BOOL fCut;
	int nLength;
	int nWidth;
	int nHeight;
	int i;

	D2WinFontSet(12);
	i = 0;
	Unicode::unicode2Win(szName, pwszName, 300);
	nLength = strlen(szName);
	fCut = FALSE;
	for (; i < nLength; i++)
	{
		if (D2WinFontGetTextWidthN((const WORD *)pwszName, i + 1) >= 120)
		{
			fCut = TRUE;
			break;
		}
	}
	szName[i] = 0;
	if (fCut)
		strcat(szName, "...");
	Unicode::win2Unicode(pwszName, szName, 299);
	D2WinFontGetTextSize((const WORD *)pwszName, &nWidth, &nHeight);
	D2WinFontDrawText((const WORD *)pwszName, hCompositeUnit->nX - (nWidth >> 1), nY, fFlag5, 0);
}

BOOL __fastcall D2CompUnitDrawName(D2COMPOSITEUNIT *hCompositeUnit)
{
	const char *pszTitle;
	BOOL fFlag5;
	int nRank;
	int nY;

#line 1365
	D2ASSERT(hCompositeUnit);
	if (!(hCompositeUnit->dwFlags & gdwBitMasks[1]))
		return FALSE;
	fFlag5 = (hCompositeUnit->dwFlags & gdwBitMasks[5]) != 0;
	nY = hCompositeUnit->nY + 6;
	Unicode wszName[256];
	if (hCompositeUnit->dwFlags & gdwBitMasks[8])
		nY += 15;
	if (!(hCompositeUnit->dwFlags & gdwBitMasks[6]) && !(hCompositeUnit->dwFlags & gdwBitMasks[7]))
	{
		if (hCompositeUnit->dwProduct == 1144144982 || hCompositeUnit->dwProduct == 1144145240
			|| hCompositeUnit->dwProduct == 1144148820)
			Unicode::strcpy(wszName, hCompositeUnit->wszCharacterName);
		else
			Unicode::strcpy(wszName, hCompositeUnit->wszAccountName);
	}
	else
	{
		nRank = ((hCompositeUnit->dwFlags >> 5) & 1) * 3 + ((hCompositeUnit->dwFlags >> 7) & 1) * 2
			+ ((hCompositeUnit->dwFlags >> 6) & 1);
		pszTitle = D2CompGetTitle(hCompositeUnit->dwClassId, nRank);
		Unicode wszTitle[256];
		Unicode::win2Unicode(wszTitle, pszTitle, 255);
		memset(wszName, 0, sizeof(wszName));
		Unicode::strcpy(wszName, wszTitle);
		Unicode::strcat(wszName, hCompositeUnit->wszCharacterName);
	}
	Unicode wszBrace[256];
	if (Unicode::strlen(hCompositeUnit->wszBracedText))
	{
		Unicode::toUnicode(wszBrace, " {", 256);
		Unicode::strcat(wszName, wszBrace);
		Unicode::strcat(wszName, hCompositeUnit->wszBracedText);
		Unicode::toUnicode(wszBrace, "}", 256);
		Unicode::strcat(wszName, wszBrace);
	}
	D2CompDrawCutName(hCompositeUnit, wszName, nY, fFlag5);
	return TRUE;
}

BOOL __fastcall D2CompUnitDrawInfo(D2COMPOSITEUNIT *hCompositeUnit)
{
	RECT tRect;
	BOOL fDiablo2;
	int nValidStrings;
	int nMaxWidth;
	int nTotalHeight;
	int nBoxX;
	int nBoxY;
	int nLeft;
	int nTop;
	int nShift;
	int nTextY;
	int i;

#line 1474
	D2ASSERT(hCompositeUnit);
	Unicode wszText[256];
	if (!(hCompositeUnit->dwFlags & gdwBitMasks[3]))
		return FALSE;
	SetRect(&tRect, hCompositeUnit->nX - 30, hCompositeUnit->nY - 80,
		hCompositeUnit->nX + 30, hCompositeUnit->nY + 20);
	if (!D2WinMouseInRect(&tRect))
		return FALSE;
	nBoxY = hCompositeUnit->nY - 80;
	nBoxX = hCompositeUnit->nX;
	nMaxWidth = 0;
	nTotalHeight = 0;
	D2COMPEXTRASTRING atStrings[MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS];
	nValidStrings = 0;
	D2WinFontSet(1);
	fDiablo2 = FALSE;
	if (hCompositeUnit->dwClassId == 15)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Moderator", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
		fDiablo2 = TRUE;
	}
	else if (hCompositeUnit->dwClassId == 16)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Sysop", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	else if (hCompositeUnit->dwClassId == 14)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Blizzard Rep", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	else if (hCompositeUnit->dwClassId == 19)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Speaker", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	else if (hCompositeUnit->dwClassId == 17)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Referee", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	else if (hCompositeUnit->dwClassId == 18)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Chat", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	else if (hCompositeUnit->dwClassId == 20)
	{
		(void)D2LangUnicodetoUnicode(wszText, "Open Character", 256);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	else if (hCompositeUnit->dwProduct == 1144144982 || hCompositeUnit->dwProduct == 1144145240
		|| hCompositeUnit->dwProduct == 1144148820)
		fDiablo2 = TRUE;
	else
	{
		switch (hCompositeUnit->dwProduct)
		{
		case 1397053520:
			(void)D2LangUnicodetoUnicode(wszText, "Brood War", 256);
			break;
		case 1146246220:
		case 1146377044:
			(void)D2LangUnicodetoUnicode(wszText, "Diablo", 256);
			break;
		case 1146308690:
			(void)D2LangUnicodetoUnicode(wszText, "Diablo Shareware", 256);
			break;
		case 1397966930:
			(void)D2LangUnicodetoUnicode(wszText, "Starcraft Shareware", 256);
			break;
		case 1398030674:
			(void)D2LangUnicodetoUnicode(wszText, "Starcraft", 256);
			break;
		case 1462911566:
			(void)D2LangUnicodetoUnicode(wszText, "Warcraft II", 256);
			break;
		default:
			(void)D2LangUnicodetoUnicode(wszText, "Unknown", 256);
			break;
		}
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	if (fDiablo2 && hCompositeUnit->nLevel > 0)
	{
		memset(wszText, 0, sizeof(wszText));
		D2LangUnicodesprintf(256, wszText, (const Unicode *)D2LangStrGetStringByIndex(5017), hCompositeUnit->nLevel);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
		if (hCompositeUnit->dwFlags & gdwBitMasks[5])
			D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)D2LangStrGetStringByIndex(5016),
				&nMaxWidth, &nTotalHeight);
		(void)D2LangUnicodetoUnicode(wszText, "Realm: ", 256);
		(void)D2LangUnicodestrcat(wszText, hCompositeUnit->wszRealmName);
		D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	}
	(void)D2LangUnicodetoUnicode(wszText, "Account Name: ", 256);
	(void)D2LangUnicodestrcat(wszText, hCompositeUnit->wszAccountName);
	D2CompAddExtraString(atStrings, &nValidStrings, (const WORD *)wszText, &nMaxWidth, &nTotalHeight);
	nTotalHeight += 6;
	nLeft = nBoxX - (nMaxWidth >> 1);
	nShift = 0;
	if (nLeft < 0)
	{
		nShift = -nLeft;
		nLeft = 0;
	}
	else if (nLeft + nMaxWidth > 800)
	{
		nShift = 800 - nLeft - nMaxWidth;
		nLeft = 800 - nMaxWidth;
	}
	nTop = nBoxY - nTotalHeight;
	D2gfxScaleFillSpan(nLeft, nTop, nLeft + nMaxWidth, nBoxY, 0, 2);
	nBoxX += nShift;
	nTextY = nTop + 3;
#line 1628
	D2ASSERT(nValidStrings <= MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS);
	for (i = 0; i < nValidStrings; i++)
	{
		nTextY += atStrings[i].nHeight;
		D2WinFontDrawText((const WORD *)atStrings[i].wszText, nBoxX - (atStrings[i].nWidth >> 1), nTextY, 0, 0);
	}
	return TRUE;
}

void __stdcall D2CompFree(D2COMPOSITEUNIT *hCompositeUnit, BOOL fFreeUnit)
{
	TGFXINFO *ptGfxInfo;
	TUNITCOFNODE *ptUnitCofNode;
	TUNITCOFNODE *ptNext;

#line 1739
	D2ASSERT(hCompositeUnit);
	ptGfxInfo = hCompositeUnit->ptGfxInfo;
#line 1743
	D2ASSERT(ptGfxInfo);
	for (ptUnitCofNode = ptGfxInfo->ptCofNode; ptUnitCofNode; ptUnitCofNode = ptNext)
	{
		ptNext = ptUnitCofNode->ptNext;
		D2CompRemoveCofNode(ptUnitCofNode->ptCofNode);
#line 1751
		D2FREE2(ptUnitCofNode);
	}
	ptGfxInfo->ptCofNode = NULL;
#line 1757
	D2FREE2(ptGfxInfo);
	if (hCompositeUnit->hEmblemCelFile)
	{
		D2WinArchiveFreeCel(hCompositeUnit->hEmblemCelFile);
		hCompositeUnit->hEmblemCelFile = NULL;
	}
	if (fFreeUnit)
		D2CompUnlinkAndFree(hCompositeUnit);
}

void __stdcall D2CompSetUserData(D2COMPOSITEUNIT *hCompositeUnit, const WORD *pwszAccount,
	const WORD *pwszCharacter, const WORD *pwszBraced, const WORD *pwszRealm, int nLevel, DWORD dwProduct)
{
#line 1785
	D2ASSERT(hCompositeUnit);
	(void)D2LangUnicodestrcpy(hCompositeUnit->wszAccountName, pwszAccount);
	(void)D2LangUnicodestrcpy(hCompositeUnit->wszCharacterName, pwszCharacter);
	(void)D2LangUnicodestrcpy(hCompositeUnit->wszBracedText, pwszBraced);
	(void)D2LangUnicodestrcpy(hCompositeUnit->wszRealmName, pwszRealm);
	hCompositeUnit->nLevel = nLevel;
	hCompositeUnit->dwProduct = dwProduct;
}

BOOL __fastcall D2CompSetMode(D2COMPOSITEUNIT *hCompositeUnit, DWORD eMode)
{
	TGFXINFO *ptGfxInfo;
	TUNITCOFNODE *ptUnitCofNode;
	DWORD dwNodeMode;
	BYTE *lpbCofData;
	PTCOFHEADER ptHeader;

#line 1805
	D2ASSERT(hCompositeUnit);
	ptGfxInfo = hCompositeUnit->ptGfxInfo;
	for (ptUnitCofNode = ptGfxInfo->ptCofNode; ptUnitCofNode;
		ptUnitCofNode = ptUnitCofNode->ptNext)
	{
		dwNodeMode = ptUnitCofNode->ptCofNode->dwMode;
		if (dwNodeMode == eMode)
		{
			lpbCofData = D2CompCofNodeGetData(ptUnitCofNode->ptCofNode);
			ptHeader = (PTCOFHEADER)lpbCofData;
			hCompositeUnit->dwMode = eMode;
			hCompositeUnit->nFrames = ptHeader->bNumFrames;
			hCompositeUnit->nAnimPos = 0;
			hCompositeUnit->nAnimRate = ptHeader->dwAnimRate;
			hCompositeUnit->bNumDirections = ptHeader->bNumDirections;
			return TRUE;
		}
	}
	return FALSE;
}

int __stdcall D2CompSetMarkerCels(HCELFILE hSquelchCelFile, HCELFILE hSelectedCelFile)
{
	sghSquelchCelFile = hSquelchCelFile;
	sgnSquelchFrames = (int)D2CMPCelFileGetNumFrames(hSquelchCelFile);
	sghSelectedCelFile = hSelectedCelFile;
	return sgnSelectedFrames = (int)D2CMPCelFileGetNumFrames(hSelectedCelFile);
}

D2COMPOSITEUNIT * __stdcall D2CompCreate(DWORD eClass, DWORD eMode, const BYTE *pbCodes, const BYTE *pbColors)
{
	D2COMPOSITEUNIT *ptNewUnit;
	char szMessage[256];
	DWORD i;

#line 1926
	D2ASSERTMSG(sgdwCodes == 283, COMP_CODE_TABLE_MESSAGE(szMessage));
#line 1928
	ptNewUnit = (D2COMPOSITEUNIT *)D2ALLOC2(sizeof(D2COMPOSITEUNIT));
	D2ASSERT(ptNewUnit);
	memset(ptNewUnit, 0, sizeof(D2COMPOSITEUNIT));
	D2CompAllocGfxInfo(ptNewUnit);
	ptNewUnit->dwClassId = eClass;
	if (eClass >= 5)
		ptNewUnit->dwWeaponClass = 1;
	else
	{
		ptNewUnit->dwWeaponClass = D2CompGetWeaponClass(pbCodes);
		if (!ptNewUnit->dwWeaponClass)
			return NULL;
	}
	ptNewUnit->dwMode = eMode;
	ptNewUnit->dwFlags |= gdwBitMasks[0];
	ptNewUnit->dwFlags |= gdwBitMasks[1];
	ptNewUnit->dwFlags |= gdwBitMasks[2];
	for (i = 0; i < NUM_COMPONENTS; pbCodes++, i++)
	{
		ptNewUnit->field_78[i] = 1;
		if (*pbCodes > 0 && *pbCodes < sgdwCodes && *pbCodes != 255)
		{
			ptNewUnit->adwGearTokens[i] = sgatCodes[*pbCodes].field_00;
			ptNewUnit->field_78[i] = sgatCodes[*pbCodes].field_08;
		}
		else
			ptNewUnit->adwGearTokens[i] = 0;
	}
	for (i = 0; i < NUM_COMPONENTS; i++)
	{
		ptNewUnit->abColors[i] = *pbColors++;
		if (ptNewUnit->abColors[i] != 255)
			ptNewUnit->abColors[i]--;
	}
	if (D2CompLoadCof(ptNewUnit) && !D2CompSetMode(ptNewUnit, eMode))
		return NULL;
	ptNewUnit->ptPrev = NULL;
	ptNewUnit->ptNext = sgptUnits;
	if (sgptUnits)
		sgptUnits->ptPrev = ptNewUnit;
	sgptUnits = ptNewUnit;
	return ptNewUnit;
}

static BOOL __fastcall D2CompLoadCof(D2COMPOSITEUNIT *ptCompositeUnit)
{
	TGFXINFO *ptGfxInfo;
	BYTE *lpbCurrCofData;
	DWORD dwArg4;
	DWORD dwArg5;

#line 1646
	D2ASSERT(ptCompositeUnit);
	if (ptCompositeUnit->dwClassId >= sgdwClassTokens || ptCompositeUnit->dwMode >= sgdwModeTokens
		|| ptCompositeUnit->dwWeaponClass >= sgdwWClassTokens)
	{
		ptCompositeUnit->dwClassId = 5;
		ptCompositeUnit->dwMode = 5;
		ptCompositeUnit->dwWeaponClass = 1;
	}
	ptGfxInfo = ptCompositeUnit->ptGfxInfo;
#line 1659
	D2ASSERT(ptGfxInfo);
	dwArg5 = 0;
	dwArg4 = 0;
	lpbCurrCofData = D2CompGetCofData(ptGfxInfo, ptCompositeUnit->dwClassId, ptCompositeUnit->dwMode,
		&dwArg4, &dwArg5, sgadwWClassTokens[ptCompositeUnit->dwWeaponClass]);
	if (!lpbCurrCofData)
	{
		ptCompositeUnit->dwClassId = 5;
		ptCompositeUnit->dwMode = 5;
		ptCompositeUnit->dwWeaponClass = 1;
		dwArg5 = 0;
		dwArg4 = 0;
		lpbCurrCofData = D2CompGetCofData(ptGfxInfo, 5, 5, &dwArg4, &dwArg5, sgadwWClassTokens[1]);
#line 1674
		D2ASSERT(lpbCurrCofData);
	}
	return TRUE;
}

static DWORD __fastcall D2CompGetWeaponClass(const BYTE *pbCodes)
{
	DWORD dwRight;
	DWORD dwLeft;

	dwRight = 0;
	if (pbCodes[5] != 255)
		dwRight = sgatCodes[pbCodes[5]].field_04;
	dwLeft = 0;
	if (pbCodes[6] != 255)
		dwLeft = sgatCodes[pbCodes[6]].field_04;
	if (!dwRight && !dwLeft)
		return 1;
	if (dwRight == dwLeft && (dwRight == 6 || dwRight == 7))
		return dwRight;
	if (!dwRight)
		return dwLeft;
	if (!dwLeft)
		return dwRight;
	if (dwRight == 4)
	{
		if (dwLeft == 4)
			return 11;
		if (dwLeft == 2)
			return 9;
	}
	else if (dwRight == 2)
	{
		if (dwLeft == 2)
			return 10;
		if (dwLeft == 4)
			return 12;
	}
	if (dwRight == 5 && dwLeft == 4 || dwRight == 4 && dwLeft == 5 || dwRight == 5 && dwLeft == 5)
		return 11;
	return 0;
}

BOOL __stdcall D2CompReset(D2COMPOSITEUNIT *ptUnit, DWORD eClass, DWORD eMode, const BYTE *pbCodes,
	const BYTE *pbColors)
{
	char szMessage[256];
	DWORD i;
	BOOL fResult;

#line 2008
	D2ASSERTMSG(sgdwCodes == 283, COMP_CODE_TABLE_MESSAGE(szMessage));
#line 2011
	D2ASSERT(ptUnit);
	D2CompAllocGfxInfo(ptUnit);
	ptUnit->dwClassId = eClass;
	if (eClass >= 5)
	{
		ptUnit->dwWeaponClass = 1;
		fResult = TRUE;
	}
	else
	{
		fResult = D2CompGetWeaponClass(pbCodes);
		ptUnit->dwWeaponClass = fResult;
	}
	if (fResult)
	{
		ptUnit->dwMode = eMode;
		ptUnit->dwFlags |= gdwBitMasks[0];
		ptUnit->dwFlags |= gdwBitMasks[1];
		ptUnit->dwFlags |= gdwBitMasks[2];
		for (i = 0; i < NUM_COMPONENTS; pbCodes++, i++)
		{
			ptUnit->field_78[i] = 1;
			if (*pbCodes > 0 && *pbCodes < sgdwCodes && *pbCodes != 255)
			{
				ptUnit->adwGearTokens[i] = sgatCodes[*pbCodes].field_00;
				ptUnit->field_78[i] = sgatCodes[*pbCodes].field_08;
			}
			else
				ptUnit->adwGearTokens[i] = 0;
		}
		for (i = 0; i < NUM_COMPONENTS; i++)
		{
			ptUnit->abColors[i] = *pbColors++;
			if (ptUnit->abColors[i] != 255)
				ptUnit->abColors[i]--;
		}
		fResult = D2CompLoadCof(ptUnit);
		if (fResult)
			return D2CompSetMode(ptUnit, eMode);
	}
	return fResult;
}
#line 2047

void __cdecl D2CompFreeAll(void)
{
	D2COMPOSITEUNIT *ptUnit;
	D2COMPOSITEUNIT *ptNext;

	for (ptUnit = sgptUnits; ptUnit; ptUnit = ptNext)
	{
		ptNext = ptUnit->ptNext;
		D2CompFree(ptUnit, FALSE);
#line 2080
		D2FREE2(ptUnit);
	}
	sgptUnits = NULL;
}

D2COMPOSITEUNIT * __fastcall D2CompGetFirst(void)
{
	return sgptUnits;
}

D2COMPOSITEUNIT * __stdcall D2CompGetNext(D2COMPOSITEUNIT *ptUnit)
{
#line 2100
	D2ASSERT(ptUnit);
	return ptUnit->ptNext;
}

void __stdcall D2CompSetDirection(D2COMPOSITEUNIT *ptUnit, BYTE bDirection)
{
	int nMax;
	int nDirection;

#line 2111
	D2ASSERT(ptUnit);
	nMax = ptUnit->bNumDirections;
	nDirection = bDirection;
	nDirection = nDirection <= 0 ? 0 : nDirection;
	ptUnit->bDirection = (BYTE)(nDirection >= nMax ? nMax : nDirection);
}

BYTE __stdcall D2CompGetDirections(D2COMPOSITEUNIT *ptUnit)
{
#line 2121
	D2ASSERT(ptUnit);
	return ptUnit->bNumDirections;
}

int __stdcall D2CompGetFrames(D2COMPOSITEUNIT *ptUnit)
{
#line 2131
	D2ASSERT(ptUnit);
	return ptUnit->nFrames;
}

void __stdcall D2CompSetFrame(D2COMPOSITEUNIT *ptUnit, int nFrame)
{
	int nMax;

#line 2142
	D2ASSERT(ptUnit);
	nMax = ptUnit->nFrames << 8;
	nFrame = nFrame > 0 ? nFrame : 0;
	if (nFrame < nMax)
		nMax = nFrame;
	ptUnit->nAnimPos = nMax;
}

void __stdcall D2CompSetX(D2COMPOSITEUNIT *ptUnit, int nX)
{
#line 2153
	D2ASSERT(ptUnit);
	ptUnit->nX = nX;
}

void __stdcall D2CompSetY(D2COMPOSITEUNIT *ptUnit, int nY)
{
#line 2164
	D2ASSERT(ptUnit);
	ptUnit->nY = nY;
}

DWORD __stdcall D2CompGetFlags(D2COMPOSITEUNIT *hCompositeUnit)
{
#line 2173
	D2ASSERT(hCompositeUnit);
	return hCompositeUnit->dwFlags;
}

void __stdcall D2CompSetFlags(D2COMPOSITEUNIT *hCompositeUnit, DWORD dwFlags)
{
#line 2186
	D2ASSERT(hCompositeUnit);
	hCompositeUnit->dwFlags = dwFlags;
}

void __stdcall D2CompSetEmblem(D2COMPOSITEUNIT *hCompositeUnit, BYTE bIcon, BYTE bColorA, BYTE bColorB)
{
	char szPath[512];

#line 2200
	D2ASSERT(hCompositeUnit);
	if (bIcon < 1 || bIcon > 32)
		return;
	wsprintfA(szPath, "%s\\ui\\emblems\\icon%02dc", "DATA\\GLOBAL", bIcon - 1);
	hCompositeUnit->hEmblemCelFile = D2WinArchiveLoadCel(szPath, 0);
	if (bColorA >= 1 && bColorA <= 10)
		hCompositeUnit->bEmblemColorA = sgabEmblemColorsA[bColorA - 1];
	else
		hCompositeUnit->bEmblemColorA = 0;
	if (bColorB >= 1 && bColorB <= 10)
		hCompositeUnit->bEmblemColorB = sgabEmblemColorsB[bColorB - 1];
	else
		hCompositeUnit->bEmblemColorB = 0;
}

void __fastcall D2CompLoadPalettes(void)
{
	D2CompLoadPalette("grey", 1);
	D2CompLoadPalette("grey2", 2);
	D2CompLoadPalette("gold", 3);
	D2CompLoadPalette("brown", 4);
	D2CompLoadPalette("greybrown", 5);
	D2CompLoadPalette("invgrey", 6);
	D2CompLoadPalette("invgrey2", 7);
	D2CompLoadPalette("invgreybrown", 8);
}

static void __fastcall D2CompLoadPalette(const char *pszName, int nPalette)
{
	char szPath[256];
	HANDLE hFile;

	strcpy(szPath, "Data\\Global\\Items\\Palette\\");
	strcat(szPath, pszName);
	strcat(szPath, ".dat");
	if (ArchiveOpenFile(NULL, szPath, &hFile, TRUE) == FALSE)
	{
#line 2240
		D2WARN(0);
		memset(sgatPalettes[nPalette].aabShifts, 0, sizeof(sgatPalettes[nPalette].aabShifts));
	}
	else
	{
		ArchiveReadFile(NULL, hFile, sgatPalettes[nPalette].aabShifts, sizeof(sgatPalettes[nPalette].aabShifts));
		ArchiveCloseFile(NULL, hFile);
	}
}

void __fastcall D2CompSetEmblemColors(void)
{
	int i;

	for (i = 0; i < 10; i++)
		sgabEmblemColorsA[i] = (BYTE)D2WinPaletteGetColor(sgatEmblemColorsA[i].field_00,
			sgatEmblemColorsA[i].field_01, sgatEmblemColorsA[i].field_02);
	for (i = 0; i < 10; i++)
		sgabEmblemColorsB[i] = (BYTE)D2WinPaletteGetColor(sgatEmblemColorsB[i].field_00,
			sgatEmblemColorsB[i].field_01, sgatEmblemColorsB[i].field_02);
}

const char * __stdcall D2CompGetTitleOf(int eClass, int nNumber, BOOL fHigher)
{
	int nRank;

	if (nNumber < 4)
		nRank = 0;
	else if (nNumber < 8)
		nRank = 1;
	else
		nRank = (nNumber >= 12) + 2;
	if (fHigher && nRank)
		nRank += 3;
	return D2CompGetTitle(eClass, nRank);
}

static BOOL D2CompIsManClass(int eClass)
{
	return eClass < 0 || (eClass > 1 && eClass != 7);
}

const char * __stdcall D2CompGetTitle(int eClass, int nRank)
{
	if (D2CompIsManClass(eClass))
	{
		switch (nRank)
		{
		case 1:
			return "Sir ";
		case 2:
			return "Lord ";
		case 3:
			return "Baron ";
		case 4:
			return "Count ";
		case 5:
			return "Duke ";
		case 6:
			return "King ";
		}
		return "";
	}
	else
	{
		switch (nRank)
		{
		case 1:
			return "Dame ";
		case 2:
			return "Lady ";
		case 3:
			return "Baroness ";
		case 4:
			return "Countess ";
		case 5:
			return "Duchess ";
		case 6:
			return "Queen ";
		}
		return "";
	}
}
