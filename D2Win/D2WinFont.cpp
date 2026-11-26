#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinFont.cpp"
#include "D2Win.h"
#include "D2Macros.h"
#include <time.h>
#include "Archive.h"

HD2ARCHIVE __fastcall D2WinArchiveGetArchive(void);

typedef D2FONTGLYPH *(__fastcall *PFNFONTGLYPH)(WORD dwChar);

static const char *const sgapszFontNames[NUM_FONTS] =
{
	"Font8", "Font16", "Font30", "Font42", "FontFormal10", "FontFormal12", "Font6", "Font24",
	"FontFormal11", "FontExocet10", "FontRidiculous", "FontExocet8", "ReallyTheLastSucker", "FontInGameChat"
};

static const DWORD sgadwValidChars[FONT_DWORDS_MAX] =
{
	0, 0xFFFFFFFF, 0xFFFFFFFF, 2147483647, 393216, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

typedef D2FONT D2FONTS[NUM_FONTS];
typedef char D2FONTDIR[20];
typedef Unicode D2FONTHOVERTEXT[1024];
typedef Unicode D2FONTCODE[2];
#define sghMonsterIndicators sghMonsterIndicators_s56u
#define sgwszHoverText sgwszHoverText_zd05
#define sgnHoverX sgnHoverX_l3
#define sgnHoverY sgnHoverY_p
#define sgdwHoverColor sgdwHoverColor_kxk5
#define sgnHoverAlign sgnHoverAlign_mr6
#define sgnHoverPercent sgnHoverPercent_gp
#define sgwszMonsterCode sgwszMonsterCode_w9
#define sgwszColorCode sgwszColorCode_k7e
#define sgpfnGetGlyph sgpfnGetGlyph_urf5
#define sgptFont sgptFont_y
#define sgdwLastPurge sgdwLastPurge_h9
static DWORD sgeFont = 1;
static D2FONTS sgatFonts;
static D2FONTDIR sgszFontDir;
static HCELFILE sghMonsterIndicators;
static D2FONTHOVERTEXT sgwszHoverText;
static int sgnHoverX;
static int sgnHoverY;
static DWORD sgdwHoverColor;
static int sgnHoverAlign;
static int sgnHoverPercent;
static D2FONTCODE sgwszColorCode;
static D2FONTCODE sgwszMonsterCode;
static PFNFONTGLYPH sgpfnGetGlyph;
static D2FONT * sgptFont;
static DWORD sgdwLastPurge;
static BYTE sgbHoverBarColor;

static inline BOOL FontIsCode(WORD wChar)
{
	if (wChar < 256)
		return wChar == 255;
	return FALSE;
}
#define FONT_IS_CODE(wChar)		FontIsCode(wChar)

#define FONT_HEIGHT()			((int)sgatFonts[sgeFont].ptTable->bHeight)
#define SCREEN_WIDTH()			(sgtWindowData.field_14 == 1 ? 800 : 640)
#define SCREEN_HEIGHT()			(sgtWindowData.field_14 == 1 ? 600 : 480)

static void __fastcall D2WinFontLoadTable(DWORD eFont);
static D2FONTGLYPH * __fastcall D2WinFontGetGlyphLatin(WORD dwChar);
static D2FONTGLYPH * __fastcall D2WinFontGetGlyphSearch(WORD dwChar);
static int __fastcall D2WinFontGetTextWidthLines(const WORD *pStr);
static void __fastcall sFontDrawText(const WORD *pwszText, int nX, int nY, int nWidth, DWORD dwColor,
	BOOL fCenter);
static int __fastcall D2WinFontGetLineWidth(const WORD *pStr);
static inline int D2WinFontCountLines(const WORD *pStr);

static inline void D2WinFontGetCelPath(char *pszPath, int Len, DWORD eFont)
{
	char szFilename[260];

	wsprintfA(szFilename, "%s\\%s\\", "DATA\\LOCAL", "Font");
#line 119
	D2ASSERT(Len > (int) (strlen (szFilename) + 20));
	wsprintfA(pszPath, "%s%s%s", szFilename, sgszFontDir, sgapszFontNames[eFont]);
}

static inline void D2WinFontFixGlyph(D2FONTGLYPH *ptGlyph) { }
static inline void D2WinFontGetTablePath(char *pszPath, int Len, DWORD eFont)
{
	char szFilename[260];

	wsprintfA(szFilename, "%s\\%s\\", "DATA\\LOCAL", "Font");
#line 131
	D2ASSERT(Len > (int) (strlen (szFilename) + 20));
	wsprintfA(pszPath, "%s%s%s.tbl", szFilename, sgszFontDir, sgapszFontNames[eFont]);
}

void __fastcall D2WinFontInit(void)
{

	char szName[80];
	int eLanguage;

	eLanguage = D2LangStrGetUseLanguage();
	if (eLanguage < 6)
	{
		sgpfnGetGlyph = D2WinFontGetGlyphLatin;
		strcpy(sgszFontDir, "Latin\\");
	}
	else
	{
		switch (eLanguage)
		{
		case 6:
			strcpy(sgszFontDir, "JPN\\");
			break;
		case 7:
			strcpy(sgszFontDir, "KOR\\");
			break;
		case 8:
		case 9:
			strcpy(sgszFontDir, "CHI\\");
			break;
		case 10:
			strcpy(sgszFontDir, "LATIN2\\");
			break;
		case 11:
			strcpy(sgszFontDir, "CYR\\");
			break;
		default:
			strcpy(sgszFontDir, "Latin\\");
			break;
		}
		sgpfnGetGlyph = D2WinFontGetGlyphSearch;
	}
	if (!sgatFonts[1].hCelFile)
	{
		D2WinFontGetCelPath(szName, sizeof(szName) - 1, 1);
		sgatFonts[1].hCelFile = D2WinArchiveLoadCel(szName, 0);
		D2WinFontLoadTable(1);
	}
	if (!sghMonsterIndicators)
	{ char szPath[260];
		wsprintfA(szPath, "%s\\UI\\Font\\MonsterIndicators", "DATA\\GLOBAL");
		sghMonsterIndicators = D2WinArchiveLoadCel(szPath, 0);
	}
	memset(sgwszHoverText, 0, sizeof(D2FONTHOVERTEXT));
	strcpy(szName, "c");
	sgptFont = &sgatFonts[1];
	Unicode::toUnicode(sgwszColorCode, szName, 2);
	strcpy(szName, "m");
	Unicode::toUnicode(sgwszMonsterCode, szName, 2);
}

static void __fastcall D2WinFontLoadTable(DWORD eFont)
{
	int i;
	char szName[80];
	void *pTable;

	D2WinFontGetTablePath(szName, sizeof(szName) - 1, eFont);
	if (sgatFonts[eFont].pTableFile)
	{
#line 147
		D2FREE2(sgatFonts[eFont].pTableFile);
	}
#line 149
	pTable = ArchiveAllocBufferAndRead(D2WinArchiveGetArchive(), szName, NULL, D2_THIS_FILE, __LINE__);
	sgatFonts[eFont].pTableFile = pTable;
	sgatFonts[eFont].ptTable = (D2FONTTABLE *)sgatFonts[eFont].pTableFile;
	sgatFonts[eFont].ptGlyphs = (D2FONTGLYPH *)((BYTE *)sgatFonts[eFont].pTableFile + sizeof(D2FONTTABLE));
	for (i = sgatFonts[eFont].ptTable->wNumGlyphs - 1; i >= 0; i--)
		D2WinFontFixGlyph(&sgatFonts[eFont].ptGlyphs[i]);
}

void __fastcall D2WinFontDestroy(void)
{
	int i;

	for (i = 0; i < NUM_FONTS; i++)
	{
		if (sgatFonts[i].hCelFile)
		{
			D2WinArchiveFreeCel(sgatFonts[i].hCelFile);
			sgatFonts[i].hCelFile = NULL;
		}
		if (sgatFonts[i].pTableFile)
		{
#line 256
			D2FREE2(sgatFonts[i].pTableFile);
			sgatFonts[i].pTableFile = NULL;
		}
	}
	D2WinArchiveFreeCel(sghMonsterIndicators);
	sghMonsterIndicators = NULL;
}

DWORD __fastcall D2WinFontSet(DWORD eFont)
{
	char szName[80];
	DWORD ePrevious;
	DWORD dwNow;
	D2FONT *ptFont;
	DWORD i;

	ePrevious = sgeFont;
	dwNow = (DWORD)time(NULL);
	if (ePrevious == eFont)
	{
		sgatFonts[eFont].dwLastSelectTime = dwNow;
		return ePrevious;
	}
	sgeFont = eFont;
	ptFont = &sgatFonts[eFont];
	if (!ptFont->hCelFile)
	{
		D2WinFontGetCelPath(szName, sizeof(szName) - 1, eFont);
		ptFont->hCelFile = D2WinArchiveLoadCel(szName, 0);
		D2WinFontLoadTable(eFont);
		sgatFonts[eFont].dwLastSelectTime = dwNow;
	}
	if (dwNow - sgdwLastPurge > 600)
	{
		sgdwLastPurge = dwNow;
		for (i = 0; i < NUM_FONTS; i++)
		{
			if (i != eFont && dwNow - sgatFonts[i].dwLastSelectTime > 240 && sgatFonts[i].hCelFile)
			{
				D2WinArchiveFreeCel(sgatFonts[i].hCelFile);
				sgatFonts[i].hCelFile = NULL;
			}
		}
	}
	sgptFont = ptFont;
	return ePrevious;
}

static D2FONTGLYPH * __fastcall D2WinFontGetGlyphLatin(WORD dwChar)
{
	if ((WORD)dwChar > 255)
		return sgatFonts[sgeFont].ptGlyphs;
	return &sgatFonts[sgeFont].ptGlyphs[(WORD)dwChar];
}

static D2FONTGLYPH * __fastcall D2WinFontGetGlyphSearch(WORD dwChar)
{
	D2FONT *ptFont;
	int nLow, nHigh, nMiddle;
	int nSteps;

	ptFont = &sgatFonts[sgeFont];
	nSteps = 0;
	nLow = 0;
	nHigh = ptFont->ptTable->wNumGlyphs - 1;
	while (nHigh >= nLow)
	{
		nSteps++;
		nMiddle = (nHigh + nLow) / 2;
		if ((WORD)dwChar == ptFont->ptGlyphs[nMiddle].wChar)
			return &ptFont->ptGlyphs[nMiddle];
		if ((WORD)dwChar < ptFont->ptGlyphs[nMiddle].wChar)
			nHigh = nMiddle - 1;
		else
			nLow = nMiddle + 1;
		if (nSteps > 1000)
			break;
	}
	return &ptFont->ptGlyphs[31];
}

int __fastcall D2WinFontGetTextWidthNoCodes(const WORD *pwszText, int nCount)
{
	int nWidth;
	int i;
	const WORD *pw;
	D2FONTGLYPH *ptGlyph;

	nWidth = 0;
	pw = pwszText;
	i = 0;
	while (*pw && i < nCount)
	{
		if (*pw != 10)
		{
			if (i + 3 < nCount && FONT_IS_CODE(*pw) && pw[1] == 'c' && pw[2] >= '0'
				&& pw[2] < '7')
			{
				pw += 3;
				i += 3;
				continue;
			}
			ptGlyph = sgpfnGetGlyph(*pw);
			nWidth += ptGlyph->bWidth;
		}
		pw++;
		i++;
	}
	return nWidth;
}

int __fastcall D2WinFontGetTextWidthN(const WORD *pwszText, int nCount)
{
	int nWidth;
	int i;
	const WORD *pw;

	nWidth = 0;
	pw = pwszText;
	for (i = 0; *pw && i < nCount; i++, pw++)
	{
		if (*pw != 10)
			nWidth += sgpfnGetGlyph(*pw)->bWidth;
	}
	return nWidth;
}

int __fastcall D2WinFontGetTextWidth(const WORD *pwszText)
{
	int nWidth;
	int nCount;
	int i;

	nCount = Unicode::strlen(pwszText);
	nWidth = 0;
	for (i = 0; pwszText[i] && i < nCount; i++)
	{
		if (pwszText[i] != 10)
			nWidth += sgpfnGetGlyph(pwszText[i])->bWidth;
	}
	return nWidth;
}

DWORD __fastcall D2WinFontGetHeight(void)
{
	return sgatFonts[sgeFont].ptTable->bHeight;
}

void __fastcall D2WinFontDrawTextRows(const WORD *pwszText, int nX, int nY, DWORD dwUnused4, int nFirstRow,
	int nRows)
{
	D2CELCONTEXT tCelContext;
	D2FONTGLYPH *ptGlyph;
	WORD awChar[2];
	int eDisplayType;
	int nLength;
	int nLeft;
	int nCelHeight;
	int i;

	if (nFirstRow < 0)
		return;
	eDisplayType = D2GfxGetDisplayType();
	nLength = Unicode::strlen(pwszText);
	i = 0;
	memset(&tCelContext, 0, sizeof(tCelContext));
	nLeft = nX;
	tCelContext.dwDirection = 0;
	while (i < nLength)
	{
		awChar[0] = pwszText[i++];
		if (FONT_IS_CODE(pwszText[i - 1]))
		{
			WORD wszCode[1];

			wszCode[0] = pwszText[i++];
			if (!Unicode::strnicmp(wszCode, sgwszColorCode, 1))
			{
				i++;
			}
			else if (!Unicode::strnicmp(wszCode, sgwszMonsterCode, 1))
			{
				ptGlyph = sgpfnGetGlyph(((const Unicode *)pwszText)[i++ - 1]);
				tCelContext.hCelFile = sghMonsterIndicators;
				tCelContext.dwFrame = ptGlyph->wFrame;
				D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, 5, 0);
				nY += (FONT_HEIGHT() * 16) / 10;
			}
		}
		else if (awChar[0] != 10)
		{
			ptGlyph = sgpfnGetGlyph(awChar[0]);
			if (eDisplayType != 6)
			{
				tCelContext.hCelFile = sgptFont->hCelFile;
				tCelContext.dwFrame = ptGlyph->wFrame;
				nCelHeight = (int)D2CMPCelGetHeight(D2CMPCelContextGetCel(&tCelContext));
				if (nFirstRow + nRows <= nCelHeight)
					D2gfxScaleDrawSpriteRows(&tCelContext, nX, nY, nFirstRow, nRows, 5);
				else
					D2gfxScaleDrawSpriteRows(&tCelContext, nX, nY, 0, nCelHeight, 5);
			}
			nX += ptGlyph->bWidth;
		}
		else
		{
			nX = nLeft;
			nY += (-FONT_HEIGHT() * 16) / 10;
		}
	}
}

void __fastcall D2WinFontDrawTextWindow(const WORD *pwszText, int nX, int nY, DWORD dwColor, int nOffset,
	int nWidth)
{
	D2CELCONTEXT tCelContext;
	D2FONTGLYPH *ptGlyph;
	WORD awChar[2];
	int nLength;
	int nLeft;
	int i;

	nLength = Unicode::strlen(pwszText);
	nLeft = nX;
	nX += nOffset;
	i = 0;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = sgptFont->hCelFile;
	tCelContext.dwDirection = 0;
	while (i < nLength)
	{
		if (nX > nLeft + nWidth)
			return;
		awChar[0] = pwszText[i++];
		if (FONT_IS_CODE(pwszText[i - 1]))
		{
			WORD wszCode[1];

			wszCode[0] = pwszText[i++];
			if (!Unicode::strnicmp(wszCode, sgwszColorCode, 1))
			{
				if (i >= nLength)
				{
					dwColor = 0;
				}
				else
				{
					dwColor = ((const Unicode *)pwszText)[i++] - '0';
					if ((int)dwColor >= 13)
						dwColor = 0;
				}
			}
			else
			{
				i--;
				ptGlyph = sgpfnGetGlyph(awChar[0]);
				tCelContext.dwFrame = ptGlyph->wFrame;
				if (nX > nLeft)
					D2gfxScaleDrawCelPalette(&tCelContext, nX, nY, -1, 5, dwColor);
				nX += ptGlyph->bWidth;
			}
		}
		else if (awChar[0] != 10)
		{
			ptGlyph = sgpfnGetGlyph(awChar[0]);
			tCelContext.dwFrame = ptGlyph->wFrame;
			if (nX > nLeft)
				D2gfxScaleDrawCelPalette(&tCelContext, nX, nY, -1, 5, dwColor);
			nX += ptGlyph->bWidth;
		}
		else
		{
			nX = nLeft;
			nY += (-FONT_HEIGHT() * 16) / 10;
		}
	}
}

static inline int D2WinFontClamp(int n, int nMin, int nMax)
{
	int nValue;

	nValue = n <= nMin ? nMin : n;
	return nValue < nMax ? nValue : nMax;
}

void __fastcall D2WinFontDrawHoverText(void)
{
	int nWidth;
	int nTextHeight;
	int nX, nY;
	int nLeft, nBottom;
	int nScreenWidth, nScreenHeight;
	int nFill, nSplit;

	if (!Unicode::strlen(sgwszHoverText))
		return;
	nWidth = D2WinFontGetTextWidthLines((const WORD *)sgwszHoverText) + 8;
	nTextHeight = (FONT_HEIGHT() * D2WinFontCountLines((const WORD *)sgwszHoverText) * 17) / 10;
	nX = sgnHoverX;
	if (sgnHoverAlign == 1)
		nX -= nWidth >> 1;
	nY = sgnHoverY + 2;
	nScreenWidth = SCREEN_WIDTH();
	nScreenHeight = SCREEN_HEIGHT();
	nLeft = D2WinFontClamp(nX, 0, nScreenWidth - nWidth);
	nBottom = D2WinFontClamp(nY, nTextHeight, nScreenHeight - 31);
	if (sgnHoverPercent)
	{
		if (!sgbHoverBarColor)
			sgbHoverBarColor = (BYTE)D2WinPaletteGetColor(80, 0, 0);
		nFill = (sgnHoverPercent * nWidth) >> 7;
		nSplit = nLeft + nFill;
		D2gfxScaleFillSpan(nLeft, nBottom - nTextHeight, nSplit, nBottom, sgbHoverBarColor, 1);
		nFill = nWidth - nFill;
		if (nFill)
		{
			D2gfxScaleFillSpan(nSplit, nBottom - nTextHeight, nSplit + nFill,
				nBottom, 0, 2);
		}
	}
	else
	{
		D2gfxScaleFillSpan(nLeft, nBottom - nTextHeight, nLeft + nWidth, nBottom, 0, 2);
	}
	sFontDrawText((const WORD *)sgwszHoverText, nLeft, nBottom - (FONT_HEIGHT() * 3) / 10, nWidth, sgdwHoverColor, TRUE);
}

static int __fastcall D2WinFontGetTextWidthLines(const WORD *pStr)
{
	int nWidth, nWidest;
	int nLength;
	int i;

#line 444
	D2ASSERT(pStr);
	nWidth = 0;
	nWidest = 0;
	nLength = Unicode::strlen(pStr);
	for (i = 0; i < nLength; i++)
	{
		if (!*pStr)
			break;
		if (*pStr == 10)
		{
			if (nWidth > nWidest)
				nWidest = nWidth;
			nWidth = 0;
			pStr++;
		}
		else if (FONT_IS_CODE(*pStr))
		{
			pStr++;
			if (!Unicode::strnicmp(pStr, sgwszMonsterCode, 1))
				nWidth += sgpfnGetGlyph(sgwszMonsterCode[0])->bWidth;
			pStr += 2;
			i += 2;
		}
		else
		{
			nWidth += sgpfnGetGlyph(*pStr)->bWidth;
			pStr++;
		}
	}
	if (nWidest > nWidth)
		nWidth = nWidest;
	return nWidth;
}

static void __fastcall sFontDrawText(const WORD *pwszText, int nX, int nY, int nWidth, DWORD dwColor,
	BOOL fCenter)
{
	D2CELCONTEXT tCelContext;
	D2FONTGLYPH *ptGlyph;
	WORD awChar[1];
	int nLength;
	int nLeft;
	int i;

	nLength = Unicode::strlen(pwszText);
	nLeft = nX;
	if (fCenter)
		nX += (nWidth - D2WinFontGetLineWidth(pwszText)) >> 1;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = sgptFont->hCelFile;
	tCelContext.dwDirection = 0;
	i = 0;
	while (i < nLength)
	{
		awChar[0] = pwszText[i++];
		if (FONT_IS_CODE(pwszText[i - 1]))
		{
			WORD wszCode[1];

			wszCode[0] = pwszText[i++];
			if (!Unicode::strnicmp(wszCode, sgwszColorCode, 1))
			{
				if (i >= nLength)
				{
					dwColor = 0;
				}
				else
				{
					dwColor = ((const Unicode *)pwszText)[i++] - '0';
					if ((int)dwColor >= 13)
						dwColor = 0;
				}
			}
			else
			{
				i--;
				ptGlyph = sgpfnGetGlyph(awChar[0]);
				tCelContext.dwFrame = ptGlyph->wFrame;
				D2gfxScaleDrawCelPalette(&tCelContext, nX, nY, -1, 5, dwColor);
				nX += ptGlyph->bWidth;
			}
		}
		else if (awChar[0] != 10)
		{
			ptGlyph = sgpfnGetGlyph(awChar[0]);
			tCelContext.dwFrame = ptGlyph->wFrame;
			D2gfxScaleDrawCelPalette(&tCelContext, nX, nY, -1, 5, dwColor);
			nX += ptGlyph->bWidth;
		}
		else
		{
			if (fCenter)
				nX = ((nWidth - D2WinFontGetLineWidth(&pwszText[i])) >> 1) + nLeft;
			else
				nX = nLeft;
			nY += (-FONT_HEIGHT() * 16) / 10;
		}
	}
}

static int __fastcall D2WinFontGetLineWidth(const WORD *pStr)
{
	int nWidth;
	int nLength;
	int i;

#line 491
	D2ASSERT(pStr);
	nWidth = 0;
	nLength = Unicode::strlen(pStr);
	for (i = 0; i < nLength; i++)
	{
		if (*pStr == 10)
			break;
		if (FONT_IS_CODE(*pStr))
		{
			pStr++;
			if (!Unicode::strnicmp(pStr, sgwszMonsterCode, 1))
				nWidth += sgpfnGetGlyph(sgwszMonsterCode[0])->bWidth;
			i += 2;
			pStr += 2;
		}
		else
		{
			nWidth += sgpfnGetGlyph(*pStr)->bWidth;
			pStr++;
		}
	}
	return nWidth;
}

static inline int D2WinFontCountLines(const WORD *pStr)
{
	const WORD *pChar;
	int nLines;
	int nLength;
	int i;

#line 525
	D2ASSERT(pStr);
	nLines = 1;
	nLength = Unicode::strlen(pStr);
	pChar = pStr;
	for (i = 0; i < nLength; i++, pChar++)
	{
		if (*pChar == 10)
			nLines++;
	}
	return nLines;
}

void __fastcall D2WinFontSetHoverText(const WORD *pwszText, int nX, int nY, DWORD dwColor, int nAlign)
{
	if (pwszText && Unicode::strlen(pwszText) < 1024)
		Unicode::strcpy(sgwszHoverText, pwszText);
	else
		memset(sgwszHoverText, 0, sizeof(D2FONTHOVERTEXT));
	sgnHoverX = nX;
	sgnHoverY = nY;
	sgdwHoverColor = dwColor;
	sgnHoverAlign = nAlign;
	sgnHoverPercent = 0;
}

void __fastcall D2WinFontSetHoverTextBar(const WORD *pwszText, int nX, int nY, DWORD dwColor, int nAlign,
	int nPercent)
{
	if (pwszText && Unicode::strlen(pwszText) < 1024)
		Unicode::strcpy(sgwszHoverText, pwszText);
	else
		memset(sgwszHoverText, 0, sizeof(D2FONTHOVERTEXT));
	sgnHoverY = nY;
	sgnHoverX = nX;
	sgdwHoverColor = dwColor;
	sgnHoverAlign = nAlign;
	sgnHoverPercent = nPercent;
}

void __fastcall D2WinFontDrawText(const WORD *pwszText, int nX, int nY, DWORD dwColor, BOOL fCenter)
{
	int nWidth;

	nWidth = 0;
	if (fCenter)
		nWidth = D2WinFontGetTextWidthLines(pwszText) + 8;
	sFontDrawText(pwszText, nX, nY, nWidth, dwColor, fCenter);
}

static inline void sFontDrawTextMode(const WORD *pwszText, int nX, int nY, int nWidth, DWORD dwColor,
	BOOL fCenter, DWORD dwDrawMode)
{
	D2CELCONTEXT tCelContext;
	D2FONTGLYPH *ptGlyph;
	WORD awChar[1];
	int nLength;
	int nLeft;
	int i;

	nLength = Unicode::strlen(pwszText);
	nLeft = nX;
	i = 0;
	if (fCenter)
		nX += (nWidth - D2WinFontGetLineWidth(pwszText)) >> 1;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = sgptFont->hCelFile;
	tCelContext.dwDirection = 0;
	while (i < nLength)
	{
		awChar[0] = pwszText[i++];
		if (FONT_IS_CODE(pwszText[i - 1]))
		{
			WORD wszCode[1];

			wszCode[0] = pwszText[i++];
			if (!Unicode::strnicmp(wszCode, sgwszColorCode, 1))
			{
				if (i >= nLength)
				{
					dwColor = 0;
				}
				else
				{
					dwColor = ((const Unicode *)pwszText)[i++] - '0';
					if ((int)dwColor >= 13)
						dwColor = 0;
				}
			}
			else
			{
				i--;
				ptGlyph = sgpfnGetGlyph(awChar[0]);
				tCelContext.dwFrame = ptGlyph->wFrame;
				D2gfxScaleDrawCelPalette(&tCelContext, nX, nY, -1, dwDrawMode, dwColor);
				nX += ptGlyph->bWidth;
			}
		}
		else if (awChar[0] != 10)
		{
			ptGlyph = sgpfnGetGlyph(awChar[0]);
			tCelContext.dwFrame = ptGlyph->wFrame;
			D2gfxScaleDrawCelPalette(&tCelContext, nX, nY, -1, dwDrawMode, dwColor);
			nX += ptGlyph->bWidth;
		}
		else
		{
			if (fCenter)
				nX = ((nWidth - D2WinFontGetLineWidth(&pwszText[i])) >> 1) + nLeft;
			else
				nX = nLeft;
			nY += (-FONT_HEIGHT() * 16) / 10;
		}
	}
}

void __fastcall D2WinFontDrawTextWithMode(const WORD *pwszText, int nX, int nY, DWORD dwColor, BOOL fCenter,
	DWORD dwDrawMode)
{
	int nWidth;

	nWidth = 0;
	if (fCenter)
		nWidth = D2WinFontGetTextWidthLines(pwszText) + 8;
	sFontDrawTextMode(pwszText, nX, nY, nWidth, dwColor, fCenter, dwDrawMode);
}

static inline void sFontDrawTextPlain(const WORD *pwszText, int nX, int nY)
{
	D2CELCONTEXT tCelContext;
	D2FONTGLYPH *ptGlyph;
	WORD awChar[1];
	int nLeft;
	int nLength;

	nLeft = nX;
	nLength = Unicode::strlen(pwszText);
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = sgptFont->hCelFile;
	tCelContext.dwDirection = 0;
	if (nLength <= 0)
		return;
	for (;;)
	{
		awChar[0] = *pwszText++;
		if (FONT_IS_CODE(awChar[0]))
		{
			pwszText += 2;
		}
		else if (awChar[0] != 10)
		{
			ptGlyph = sgpfnGetGlyph(awChar[0]);
			tCelContext.dwFrame = ptGlyph->wFrame;
			D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, 5, 0);
			nX += ptGlyph->bWidth;
		}
		else
		{
			nX = nLeft;
			nY += (-(FONT_HEIGHT() * 15)) / 10;
		}
	}
}

void __fastcall D2WinFontDrawTextPlain(const WORD *pwszText, int nX, int nY)
{
	sFontDrawTextPlain(pwszText, nX, nY);
}

void __fastcall D2WinFontDrawBoxedText(const WORD *pwszText, int nX, int nY, DWORD dwRectColor,
	DWORD dwRectMode, DWORD dwColor)
{
	int nWidth;
	int nTextHeight;
	int nLeft, nBottom;
	int nScreenWidth, nScreenHeight;

	nWidth = D2WinFontGetTextWidthLines(pwszText) + 8;
	nTextHeight = (FONT_HEIGHT() * D2WinFontCountLines(pwszText) * 17) / 10;
	nLeft = nX;
	nBottom = nY + 2;
	nScreenWidth = SCREEN_WIDTH();
	nScreenHeight = SCREEN_HEIGHT();
	nLeft = D2WinFontClamp(nLeft, 0, nScreenWidth - nWidth);
	nBottom = D2WinFontClamp(nBottom, nTextHeight, nScreenHeight - 31);
	D2gfxScaleFillSpan(nLeft, nBottom - nTextHeight, nLeft + nWidth, nBottom, dwRectColor, dwRectMode);
	sFontDrawText(pwszText, nLeft, nBottom - (FONT_HEIGHT() * 3) / 10, nWidth, dwColor, TRUE);
}

void __fastcall D2WinFontDrawFramedText(const WORD *pwszText, int nX, int nY, DWORD dwRectColor,
	DWORD dwRectMode, DWORD dwColor)
{
	int nWidth;
	int nHeight;
	int nScreenWidth, nScreenHeight;
	int nBottom;
	int nLeft;

	nWidth = D2WinFontGetTextWidthLines(pwszText);
	nHeight = FONT_HEIGHT();
	nBottom = nY + 2;
	nScreenWidth = SCREEN_WIDTH();
	nScreenHeight = SCREEN_HEIGHT();
	nLeft = nX;
	if (nWidth + nX > nScreenWidth)
		nLeft = nScreenWidth - nWidth;
	if (nBottom > nScreenHeight - 31)
		nBottom = nScreenHeight - 31;
	if (nBottom - nHeight < 0)
		nBottom += nBottom;
	D2gfxScaleFillSpan(nLeft, nBottom - nHeight, nLeft + nWidth, nBottom, dwRectColor, dwRectMode);
	sFontDrawText(pwszText, nLeft, nBottom - 2, nWidth, dwColor, TRUE);
}

void __fastcall D2WinFontGetTextSize(const WORD *pwszText, int *pnWidth, int *pnHeight)
{
	*pnWidth = D2WinFontGetTextWidthLines(pwszText);
	*pnHeight = (FONT_HEIGHT() * D2WinFontCountLines(pwszText) * 17) / 10;
}

BOOL __fastcall D2WinFontIsValidChar(char chChar)
{
	DWORD dwOffset;

	if (FONT_IS_CODE((BYTE)chChar))
		return FALSE;
	dwOffset = (DWORD)chChar / 32 % FONT_DWORDS_MAX;
#line 1106
	D2ASSERT(dwOffset < FONT_DWORDS_MAX);
	if ((sgadwValidChars[dwOffset] >> ((BYTE)chChar % 32)) & 1)
		return TRUE;
	return FALSE;
}

void __fastcall D2WinFontDrawTextInBox(const WORD *pwszText, int nX, int nY, int nWidth, DWORD dwRectColor,
	DWORD dwRectMode, DWORD dwColor)
{
	int nTextHeight;
	int nPad;
	int nScreenWidth, nScreenHeight;
	int nLeft, nBottom;

	nTextHeight = (FONT_HEIGHT() * D2WinFontCountLines(pwszText) * 17) / 10;
	nBottom = nY + 2;
	nPad = (FONT_HEIGHT() * 4) / 10;
	nScreenWidth = SCREEN_WIDTH();
	nScreenHeight = SCREEN_HEIGHT();
	if (nX + nWidth > nScreenWidth)
		nX = nScreenWidth - nWidth;
	if (nBottom > nScreenHeight - 31)
		nBottom = nScreenHeight - 31;
	if (nBottom - nTextHeight < 0)
		nBottom += nBottom;
	nLeft = nX - 4;
	D2gfxScaleFillSpan(nLeft, nPad - nTextHeight + nBottom, nLeft + nWidth, nBottom, dwRectColor, dwRectMode);
	sFontDrawText(pwszText, nX, nBottom - 2, 0, dwColor, FALSE);
}

void __fastcall D2WinFontDrawCelFile(HCELFILE hCelFile, int nX, int nY, int eAlign, DWORD dwDrawMode,
	DWORD dwColor)
{
	D2CELCONTEXT tCelContext;
	DWORD dwCels;
	DWORD dwWidth;
	D2CEL *pCel;
	DWORD i;

	dwCels = D2CMPCelFileGetNumFrames(hCelFile);
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = hCelFile;
	tCelContext.dwDirection = 0;
	if (eAlign == 1 || eAlign == 2)
	{
		dwWidth = 0;
		for (i = 0; i < dwCels; i++)
		{
			tCelContext.dwFrame = i;
			pCel = (D2CEL *)D2CMPCelContextGetCel(&tCelContext);
			dwWidth += D2CMPCelGetWidth(pCel);
		}
		if (eAlign == 1)
			nX += -1 - (int)(dwWidth >> 1);
		else
			nX -= dwWidth;
	}
	for (i = 0; i < dwCels; i++)
	{
		tCelContext.dwFrame = i;
		if (dwColor == (DWORD)-1)
			D2gfxScaleDrawCel(&tCelContext, nX + 256 * i, nY, dwColor, dwDrawMode, 0);
		else
			D2gfxScaleDrawCelPalette(&tCelContext, nX + 256 * i, nY, -1, dwDrawMode, dwColor);
	}
}

void __fastcall D2WinFontDrawHoverTextAt(const WORD *pwszText, int nX, int nY, DWORD dwColor, int nAlign,
	int nPercent)
{
	int nWidth;
	int nTextHeight;
	int nLeft, nBottom;
	int nScreenWidth, nScreenHeight;
	int nFill, nRest, nSplit;

	if (!Unicode::strlen(pwszText))
		return;
	nWidth = D2WinFontGetTextWidthLines(pwszText) + 8;
	nTextHeight = (FONT_HEIGHT() * D2WinFontCountLines(pwszText) * 17) / 10;
	nLeft = nX;
	if (nAlign == 1)
		nLeft -= nWidth >> 1;
	nBottom = nY + 2;
	nScreenWidth = SCREEN_WIDTH();
	nScreenHeight = SCREEN_HEIGHT();
	nLeft = D2WinFontClamp(nLeft, 0, nScreenWidth - nWidth);
	nBottom = D2WinFontClamp(nBottom, nTextHeight, nScreenHeight - 31);
	if (nPercent)
	{
		if (!sgbHoverBarColor)
			sgbHoverBarColor = (BYTE)D2WinPaletteGetColor(80, 0, 0);
		nFill = (nWidth * nPercent) >> 7;
		nSplit = nLeft + nFill;
		D2gfxScaleFillSpan(nLeft, nBottom - nTextHeight, nSplit, nBottom, sgbHoverBarColor, 1);
		nRest = nWidth - nFill;
		if (nRest)
		{
			D2gfxScaleFillSpan(nSplit, nBottom - nTextHeight, nSplit + nRest,
				nBottom, 0, 2);
		}
	}
	else
	{
		D2gfxScaleFillSpan(nLeft, nBottom - nTextHeight, nLeft + nWidth, nBottom, 0, 2);
	}
	sFontDrawText(pwszText, nLeft, nBottom - (FONT_HEIGHT() * 3) / 10, nWidth, dwColor, TRUE);
}
