#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinTextBox.cpp"
#include "D2Win.h"
#include "D2Macros.h"

void __fastcall D2WinFontDrawTextWindow(const WORD *pwszText, int nX, int nY, DWORD dwColor, int nOffset,
	int nWidth);
void __fastcall D2WinFontDrawTextInBox(const WORD *pwszText, int nX, int nY, int nWidth, DWORD dwRectColor,
	DWORD dwRectMode, DWORD dwColor);

BOOL __fastcall D2WinClearTextBox(D2WINTEXTBOX *hChild);
BOOL __fastcall D2WinTextBoxAddColorLineUnicode(D2WINTEXTBOX *hChild, const WORD *pwszText, DWORD dwColor,
	BYTE bBroken);
BOOL __fastcall D2WinTextBoxSetColumnText(D2WINTEXTBOX *hChild, const WORD *pwszText, int nLine,
	int nColumn);
BOOL __fastcall D2WinTextBoxDraw(D2WINTEXTBOX *ptChild);
static BOOL __stdcall D2WinTextBoxMouseDown(_PARAMS *pParams);
static BOOL __stdcall D2WinTextBoxOnKey(_PARAMS *pParams);

D2WINTEXTBOX * __fastcall D2WinTextBoxCreate(int nX, int nY, int nWidth, int nHeight, int nMarginX,
	int nMarginY, HCELFILE hCelFile, PFNWINMSG pfnCallback, DWORD dwTextFlags, D2WINFONTDESC *ptFont,
	D2WINTEXTBOXBAR *ptBar)
{
	D2WINTEXTBOX *ptTextData;

#line 39
	ptTextData = (D2WINTEXTBOX *)D2ALLOC2(sizeof(D2WINTEXTBOX));
	memset(ptTextData, 0, sizeof(D2WINTEXTBOX));
	if ((dwTextFlags & TEXTBOX_SCROLLBAR) && !ptBar)
		nWidth -= 26;
	if (dwTextFlags & TEXTBOX_SELECT)
		dwTextFlags |= TEXTBOX_NOWRAP;
	ptTextData->eType = D2WIN_TEXTBOX;
	ptTextData->dwFlags |= gdwBitMasks[0];
	ptTextData->dwFlags |= gdwBitMasks[2];
	ptTextData->dwTextBoxFlags = dwTextFlags;
	ptTextData->hCelFile = hCelFile;
	ptTextData->nX = nX;
	ptTextData->nWidth = nWidth;
	ptTextData->nTextMarginX = nMarginX;
	ptTextData->nY = nY;
	ptTextData->nHeight = nHeight;
	ptTextData->nTextMarginY = nMarginY;
	ptTextData->pfnDraw = (PFNWINCONTROL)D2WinTextBoxDraw;
	ptTextData->pfnMouseDown = D2WinTextBoxMouseDown;
	ptTextData->pfnMouseUp = NULL;
	ptTextData->pfnChar = NULL;
	if (dwTextFlags & TEXTBOX_SELECT)
		ptTextData->pfnKey = D2WinTextBoxOnKey;
	else
		ptTextData->pfnKey = NULL;
	ptTextData->nSelectedNum = -1;
	ptTextData->pfnCallback = pfnCallback;
	ptTextData->ptNextChild = NULL;
	ptTextData->ptTopLine = NULL;
	ptTextData->ptFirstLine = NULL;
	ptTextData->ptLastLine = NULL;
	ptTextData->nNumLines = 0;
	ptTextData->nTopLine = 0;
	ptTextData->nNumColumns = 1;
	if (ptFont)
	{
		ptTextData->tFont.dwFont = ptFont->dwFont;
		ptTextData->tFont.dwColor = ptFont->dwColor;
	}
	else
	{
		ptTextData->tFont.dwFont = 1;
		ptTextData->tFont.dwColor = 0;
	}
	if (dwTextFlags & TEXTBOX_SCROLLBAR)
	{
		ptTextData->hCelFile = NULL;
		if (ptBar)
		{
			nX = ptBar->nX;
			nY = ptBar->nY;
			nHeight = ptBar->nHeight;
		}
		else
		{
			nY -= 2;
			nHeight -= 16;
			nX += nWidth + 8;
		}
		ptTextData->ptScrollbar = D2WinScrollbarCreate(nX, nY, 10, nHeight, hCelFile, NULL);
		D2WinScrollbarSetTextBox(ptTextData->ptScrollbar, ptTextData);
		D2WinMainAddChild(ptTextData);
		return ptTextData;
	}
	ptTextData->ptScrollbar = NULL;
	D2WinMainAddChild(ptTextData);
	return ptTextData;
}

DWORD __fastcall D2WinTextBoxDestroy(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 135
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	if (ptTextData->ptScrollbar)
		D2WinScrollbarDestroy(ptTextData->ptScrollbar);
	D2WinClearTextBox(hChild);
	return D2WinMainFreeChild(ptTextData);
}

BOOL __fastcall D2WinClearTextBox(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	D2WINTEXTLINE *ptNext;
	int nColumn;

	ptTextData = hChild;
#line 150
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	ptLine = ptTextData->ptFirstLine;
	while (ptLine)
	{
		ptNext = ptLine->ptNext;
		for (nColumn = 0; nColumn < MAX_TEXTBOX_COLUMNS; nColumn++)
		{
			if (ptLine->szText[nColumn])
#line 160
				D2FREE2(ptLine->szText[nColumn]);
		}
#line 162
		D2FREE2(ptLine);
		ptLine = ptNext;
	}
	ptTextData->ptTopLine = NULL;
	ptTextData->ptFirstLine = NULL;
	ptTextData->ptLastLine = NULL;
	ptTextData->nNumLines = 0;
	ptTextData->nTopLine = 0;
	return TRUE;
}

BOOL __fastcall D2WinTextBoxSetLineSpacing(D2WINTEXTBOX *hChild, int nBrokenSpacing, int nSpacing)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 179
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	ptTextData->nBrokenLineSpacing = nBrokenSpacing;
	ptTextData->nLineSpacing = nSpacing;
	return TRUE;
}

BOOL __fastcall D2WinTextBoxAddLineUTF(D2WINTEXTBOX *hChild, const char *pszText)
{

#line 194
	D2ASSERT(hChild);
	Unicode wszText[256];
	Unicode::win2Unicode(wszText, pszText, 256);
	return D2WinTextBoxAddColorLineUnicode(hChild, (const WORD *)wszText, 4, 0);
}

BOOL __fastcall D2WinTextBoxAddColorLineUTF(D2WINTEXTBOX *hChild, const char *pszText, DWORD dwColor)
{

	Unicode wszText[256];
#line 208
	D2ASSERT(hChild);
	Unicode::win2Unicode(wszText, pszText, 256);
	return D2WinTextBoxAddColorLineUnicode(hChild, (const WORD *)wszText, dwColor, 0);
}

BOOL __fastcall D2WinTextBoxAddLineUnicode(D2WINTEXTBOX *hChild, const WORD *pwszText)
{
	return D2WinTextBoxAddColorLineUnicode(hChild, pwszText, 4, 0);
}

static inline int sTextBoxGetVisibleLines(D2WINTEXTBOX *ptTextData)
{
	int nHeight;
	int nLineHeight;

#line 448
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	D2WinFontSet(ptTextData->tFont.dwFont);
	nHeight = ptTextData->nHeight - ptTextData->nTextMarginY * 2;
	nLineHeight = (int)D2WinFontGetHeight();
	return (nHeight - nLineHeight) / ((int)D2WinFontGetHeight() + 4) + 1;
}

static inline BOOL D2WinTextBoxIsFull(D2WINTEXTBOX *ptTextData)
{
	D2WINTEXTLINE *ptLine;
	int nLines;
	int nHeight;
	int nLineHeight;

#line 620
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	nLines = 0;
	for (ptLine = ptTextData->ptTopLine; ptLine; ptLine = ptLine->ptNext)
		nLines++;
	D2WinFontSet(ptTextData->tFont.dwFont);
	nHeight = ptTextData->nHeight - ptTextData->nTextMarginY * 2;
	nLineHeight = (int)D2WinFontGetHeight();
	return nLines > (nHeight - nLineHeight) / ((int)D2WinFontGetHeight() + 4) + 1;
}

static inline BOOL D2WinTextBoxAddBrokenLine(D2WINTEXTBOX *ptTextData, const WORD *pwszText, DWORD dwColor)
{
	WORD *pwszChar;
	WORD *pwszSpace;
	int nWidth;
	int nLength;
	int nHead;

#line 848
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	nWidth = ptTextData->nWidth - ptTextData->nTextMarginX * 2;
	Unicode wszText[300];
	Unicode::strncpy(wszText, pwszText, 300);
	nHead = 0;
	nLength = Unicode::strlen(wszText);
	pwszChar = (WORD *)wszText;
	pwszSpace = NULL;
	while (nHead < nLength)
	{
		if (*pwszChar == ' ')
			pwszSpace = pwszChar;
		if (D2WinFontGetTextWidthN(pwszText, nHead + 1) >= nWidth)
		{
			if (pwszSpace)
			{
				nHead = pwszSpace - (WORD *)wszText;
				pwszChar = pwszSpace;
			}
			break;
		}
		nHead++;
		pwszChar++;
	}
	Unicode wszHead[300];
	Unicode wszTail[300];
	Unicode::strncpy(wszHead, wszText, nHead);
	wszHead[nHead] = 0;
	Unicode::strncpy(wszTail, pwszChar, nLength - nHead);
	wszTail[nLength - nHead] = 0;
	D2WinTextBoxAddColorLineUnicode(ptTextData, (const WORD *)wszHead, dwColor, 1);
	D2WinTextBoxAddColorLineUnicode(ptTextData, (const WORD *)wszTail, dwColor, 0);
	return TRUE;
}

BOOL __fastcall D2WinTextBoxAddColorLineUnicode(D2WINTEXTBOX *hChild, const WORD *pwszText, DWORD dwColor,
	BYTE bBroken)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	WORD *pwszStart;
	D2WINSCROLLBAR *ptBar;
	BOOL fMoved;
	BOOL fAtEnd;
	int nWidth;
	int nMax;
	int nValue;
	int nColumn;

	ptTextData = hChild;
#line 228
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	Unicode wszText[1024];
	Unicode::strcpy(wszText, pwszText);
	pwszStart = (WORD *)wszText;
	while (*pwszStart && *pwszStart < 256 && isspace(*pwszStart))
		pwszStart++;
	nWidth = ptTextData->nWidth - ptTextData->nTextMarginX * 2;
	D2WinFontSet(ptTextData->tFont.dwFont);
	if (!(ptTextData->dwTextBoxFlags & TEXTBOX_NOWRAP) && D2WinFontGetTextWidthN(pwszStart, 255) >= nWidth)
		return D2WinTextBoxAddBrokenLine(ptTextData, pwszStart, dwColor);
#line 250
	ptLine = (D2WINTEXTLINE *)D2ALLOC2(sizeof(D2WINTEXTLINE));
	ptLine->szText[0] = (WORD *)D2ALLOC2(Unicode::strlen(pwszStart) * 2 + 2);
	Unicode::strcpy(ptLine->szText[0], pwszStart);
	for (nColumn = 1; nColumn < MAX_TEXTBOX_COLUMNS; nColumn++)
		ptLine->szText[nColumn] = NULL;
	ptLine->dwColor = dwColor;
	ptLine->bBroken = bBroken;
	ptLine->ptNext = NULL;
	fMoved = FALSE;
	if (!ptTextData->ptFirstLine)
	{
		ptTextData->ptFirstLine = ptLine;
		ptTextData->ptLastLine = ptLine;
		ptTextData->ptTopLine = ptLine;
		ptTextData->nNumLines++;
	}
	else
	{
		ptTextData->ptLastLine->ptNext = ptLine;
		ptTextData->ptLastLine = ptLine;
		ptTextData->nNumLines++;
		if (D2WinTextBoxIsFull(ptTextData))
		{
			ptBar = ptTextData->ptScrollbar;
			fAtEnd = TRUE;
			if (ptBar)
			{
				nMax = D2WinScrollbarGetMaxValue(ptBar);
				if (D2WinScrollbarGetValue(ptBar) != nMax)
					fAtEnd = FALSE;
			}
			if (fAtEnd)
			{
				fMoved = TRUE;
				ptTextData->ptTopLine = ptTextData->ptTopLine->ptNext;
				ptTextData->nTopLine++;
			}
		}
	}
	if (ptTextData->ptScrollbar)
	{
		nValue = D2WinScrollbarGetValue(ptTextData->ptScrollbar);
		D2WinScrollbarSetMaxValue(ptTextData->ptScrollbar,
			ptTextData->nNumLines - sTextBoxGetVisibleLines(ptTextData));
		D2WinScrollbarSetValue(ptTextData->ptScrollbar, fMoved ? ptTextData->nTopLine : nValue);
	}
	return TRUE;
}

BOOL __fastcall D2WinTextBoxAddColumnTextUTF(D2WINTEXTBOX *hChild, const char *pszText, int nLine,
	int nColumn)
{
	Unicode wszText[1024];

	Unicode::win2Unicode(wszText, pszText, 256);
	return D2WinTextBoxSetColumnText(hChild, (const WORD *)wszText, nLine, nColumn);
}

BOOL __fastcall D2WinTextBoxAddColumnTextUnicode(D2WINTEXTBOX *hChild, const WORD *pwszText, int nLine,
	int nColumn)
{
	return D2WinTextBoxSetColumnText(hChild, pwszText, nLine, nColumn);
}

BOOL __fastcall D2WinTextBoxSetColumnText(D2WINTEXTBOX *hChild, const WORD *pwszText, int nLine,
	int nColumn)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	int n;

	ptTextData = hChild;
#line 335
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	D2ASSERT(ptTextData->nNumColumns > nColumn);
	Unicode wszText[1024];
	Unicode::strcpy(wszText, pwszText);
	ptLine = ptTextData->ptFirstLine;
	for (n = 0; n < nLine; n++)
	{
		if (!ptLine)
			return TRUE;
		ptLine = ptLine->ptNext;
	}
	if (!ptLine)
		return TRUE;
#line 351
	D2ASSERT(ptLine->szText[ nColumn ] == NULL);
	ptLine->szText[nColumn] = (WORD *)D2ALLOC2(Unicode::strlen(wszText) * 2 + 2);
	Unicode::strcpy(ptLine->szText[nColumn], wszText);
	return TRUE;
}

int __fastcall D2WinTextBoxGetNumLines(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 365
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	return ptTextData->nNumLines;
}

int __fastcall D2WinTextBoxGetTop(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 377
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	return ptTextData->nTopLine;
}

int __fastcall D2WinTextBoxGetSelectedNum(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 389
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	return ptTextData->nSelectedNum;
}

int __fastcall D2WinTextBoxSetSelectedNum(D2WINTEXTBOX *hChild, int nLine)
{
	D2WINTEXTBOX *ptTextData;
	int nLast;

	ptTextData = hChild;
#line 401
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	nLast = ptTextData->nNumLines - 1;
	if (nLine <= -1)
		nLine = -1;
	if (nLine < nLast)
		nLast = nLine;
	ptTextData->nSelectedNum = nLast;
	return nLast;
}

BOOL __fastcall D2WinTextBoxSetTop(D2WINTEXTBOX *hChild, int nLine)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	int n;

	ptTextData = hChild;
#line 416
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	if (nLine >= ptTextData->nNumLines)
		nLine = ptTextData->nNumLines;
	ptLine = ptTextData->ptFirstLine;
	for (n = 0; n < nLine; n++)
	{
		if (!ptLine->ptNext)
			break;
		ptLine = ptLine->ptNext;
	}
	if (ptLine)
	{
		ptTextData->ptTopLine = ptLine;
		ptTextData->nTopLine = nLine;
	}
	if (ptTextData->ptScrollbar)
		D2WinScrollbarSetValue(ptTextData->ptScrollbar, nLine);
	return TRUE;
}

int __fastcall D2WinTextBoxGetVisibleLines(D2WINTEXTBOX *hChild)
{
#line 447
	D2ASSERT(hChild);
	return sTextBoxGetVisibleLines(hChild);
}

const WORD * __fastcall D2WinTextBoxGetSelectedLine(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	int n;
	int nSel;

	ptTextData = hChild;
#line 467
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	nSel = ptTextData->nSelectedNum;
	if (nSel == -1 || !(ptTextData->dwTextBoxFlags & TEXTBOX_SELECT))
		return NULL;
	for (ptLine = ptTextData->ptFirstLine, n = 0; ptLine; ptLine = ptLine->ptNext)
	{
		if (nSel == n)
			return ptLine->szText[0];
		n++;
	}
	return NULL;
}

void __fastcall D2WinTextBoxSetNumColumns(D2WINTEXTBOX *hChild, int nNumColumns)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 497
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
#line 500
	D2ASSERT(( nNumColumns > 0 ) && ( nNumColumns <= MAX_TEXTBOX_COLUMNS ));
#line 502
	D2ASSERT(ptTextData->nNumColumns <= nNumColumns);
	ptTextData->nNumColumns = nNumColumns;
	if (nNumColumns > 1)
		ptTextData->dwTextBoxFlags |= TEXTBOX_NOWRAP;
}

void __fastcall D2WinTextBoxSetColumnX(D2WINTEXTBOX *hChild, int nColumn, int nX)
{
	D2WINTEXTBOX *ptTextData;

	ptTextData = hChild;
#line 515
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
#line 518
	D2ASSERT(( nColumn >= 0 ) && ( nColumn < ptTextData->nNumColumns ));
	ptTextData->anColumnX[nColumn] = nX;
}

void __fastcall D2WinTextBoxSetColumnAlign(D2WINTEXTBOX *hChild, int nColumn, DWORD dwAlign)
{
	D2WINTEXTBOX *ptTextData;
	DWORD *pdwAlign;

	ptTextData = hChild;
#line 528
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
#line 531
	D2ASSERT(nColumn >= 0);
	pdwAlign = ptTextData->adwColumnAlign;
	D2_OPAQUE(pdwAlign);
	pdwAlign[nColumn] = dwAlign;
}

int __fastcall D2WinTextBoxGetLastLineNum(D2WINTEXTBOX *hChild)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	D2WINTEXTLINE *ptNext;
	int n;

	ptTextData = hChild;
#line 541
	D2ASSERT(hChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	n = 0;
	ptLine = ptTextData->ptFirstLine;
	for (; ptLine; ptLine = ptNext)
	{
		ptNext = ptLine->ptNext;
		if (ptNext)
			n++;
	}
	return n;
}

void __fastcall D2WinTextBoxNotWorking(void)
{
#line 561
	D2FATAL("NOT WORKING! drb");
}

BOOL __fastcall D2WinTextBoxDraw(D2WINTEXTBOX *ptChild)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTLINE *ptLine;
	D2CELCONTEXT tCelContext;
	BYTE bColor;
	int nLeft;
	int nY;
	int nLineHeight;
	int nInnerWidth;
	int nVisible;
	int nRow;
	int nColumn;
	int nColumnWidth;
	int nX;
	int nTextWidth;
	int nLength;
	int nFit;
	int n;

	ptTextData = ptChild;
#line 660
	D2ASSERT(ptChild);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	if (!(gdwBitMasks[2] & ptTextData->dwFlags) || !(gdwBitMasks[0] & ptTextData->dwFlags))
		return FALSE;
	bColor = (BYTE)D2WinPaletteGetColor(64, 64, 64);
	if (ptTextData->hCelFile)
	{
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = ptTextData->hCelFile;
		tCelContext.dwDirection = 0;
		tCelContext.dwFrame = 0;
		D2gfxScaleDrawCel(&tCelContext, ptTextData->nX, ptTextData->nY - 1, -1, 5, 0);
	}
	D2WinFontSet(ptTextData->tFont.dwFont);
	nLeft = ptTextData->nTextMarginX + ptTextData->nX;
	nY = (int)D2WinFontGetHeight() + (ptTextData->nY - ptTextData->nHeight);
	nY += ptTextData->nTextMarginY;
	nLineHeight = (int)D2WinFontGetHeight() + 4;
	sTextBoxGetVisibleLines(ptTextData);
	nInnerWidth = ptTextData->nWidth - ptTextData->nTextMarginX * 2;
	if ((ptTextData->dwTextBoxFlags & TEXTBOX_MARQUEE) && ptTextData->ptTopLine)
	{
		ptTextData->nScrollOffset += -2;
		if (ptTextData->nScrollOffset < -D2WinFontGetTextWidth(ptTextData->ptTopLine->szText[0]))
			ptTextData->nScrollOffset = nInnerWidth;
	}
	if ((ptTextData->dwTextBoxFlags & TEXTBOX_ROLL) && ptTextData->ptTopLine)
	{
		ptTextData->nScrollOffset += -2;
		if (ptTextData->nScrollOffset < -nLineHeight)
		{
			ptTextData->nScrollOffset = 0;
			ptTextData->ptTopLine = ptTextData->ptTopLine->ptNext;
		}
	}
	ptLine = ptTextData->ptTopLine;
	nRow = 0;
	nVisible = sTextBoxGetVisibleLines(ptTextData);
	while (ptLine)
	{
		for (nColumn = 0; nColumn < ptTextData->nNumColumns; nColumn++)
		{
			nColumnWidth = ptTextData->nWidth;
			if (nColumn < ptTextData->nNumColumns - 1)
				nColumnWidth = ptTextData->anColumnX[nColumn + 1];
			if (ptTextData->anColumnX[nColumn])
			{
				nX = nLeft + ptTextData->anColumnX[nColumn];
				if (nColumn + 1 < ptTextData->nNumColumns)
					nColumnWidth = ptTextData->anColumnX[nColumn + 1] - ptTextData->anColumnX[nColumn];
				else
					nColumnWidth = ptTextData->nWidth - ptTextData->anColumnX[nColumn];
				if (ptTextData->adwColumnAlign[nColumn] & TEXTBOX_CENTER)
				{
					n = (nColumnWidth - D2WinFontGetTextWidth(ptLine->szText[nColumn])) / 2;
					if (n < 0)
						n = 0;
					nX += n;
				}
				else if (ptTextData->adwColumnAlign[nColumn] & TEXTBOX_RIGHT)
				{
					if (!ptLine->szText[nColumn])
						break;
					nTextWidth = D2WinFontGetTextWidth(ptLine->szText[nColumn]);
					if (nTextWidth < 0)
						nTextWidth = 0;
					nX = nX + (nColumnWidth - nTextWidth);
				}
			}
			else if (ptTextData->dwTextBoxFlags & TEXTBOX_CENTER)
			{
				n = (ptTextData->nWidth - D2WinFontGetTextWidth(ptLine->szText[0])) / 2;
				if (n < 0)
					n = 0;
				nX = n + ptTextData->nX;
			}
			else if (ptTextData->dwTextBoxFlags & TEXTBOX_RIGHT)
			{
				nTextWidth = D2WinFontGetTextWidth(ptLine->szText[0]);
				if (nTextWidth < 0)
					nTextWidth = 0;
				nX = ptTextData->nX - nTextWidth;
			}
			else
			{
				nX = nLeft;
			}
			Unicode wszText[1024];
			Unicode::strcpy(wszText, ptLine->szText[nColumn]);
			nFit = 0;
			nLength = Unicode::strlen(wszText);
			while (nFit < nLength)
			{
				if (D2WinFontGetTextWidthN((const WORD *)wszText, nFit + 1) >= nColumnWidth)
					break;
				nFit++;
			}
			wszText[nFit] = 0;
			if (ptTextData->nSelectedNum == ptTextData->nTopLine + nRow
				&& (ptTextData->dwTextBoxFlags & TEXTBOX_SELECT))
			{
				if (nColumn == 0)
					D2WinFontDrawTextInBox((const WORD *)wszText, nX, nY, nInnerWidth, bColor, 5, 0);
				else
					D2WinFontDrawText((const WORD *)wszText, nX, nY, 0, FALSE);
			}
			else if (ptTextData->dwTextBoxFlags & TEXTBOX_MARQUEE)
			{
				D2WinFontDrawTextWindow(ptLine->szText[nColumn], nX, nY, ptLine->dwColor,
					ptTextData->nScrollOffset, nInnerWidth);
			}
			else if (ptTextData->dwTextBoxFlags & TEXTBOX_ROLL)
			{
				D2WinFontDrawText((const WORD *)wszText, nX, ptTextData->nScrollOffset + nY, ptLine->dwColor, FALSE);
			}
			else
			{
				D2WinFontDrawText((const WORD *)wszText, nX, nY, ptLine->dwColor, FALSE);
			}
		}
		switch (ptLine->bBroken)
		{
		case 0:
			nY += ptTextData->nLineSpacing + nLineHeight;
			break;
		case 1:
			nY += ptTextData->nBrokenLineSpacing + nLineHeight;
			break;
		}
		ptLine = ptLine->ptNext;
		nRow++;
		if (nRow >= nVisible)
			break;
	}
	D2WinFontSet(1);
	return TRUE;
}

static BOOL __stdcall D2WinTextBoxMouseDown(_PARAMS *pParams)
{
	D2WINTEXTBOX *ptTextData;
	int nY;
	int nLine;
	int nLast;

	ptTextData = (D2WINTEXTBOX *)pParams->window;
#line 903
	D2ASSERT(pParams->window);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	if (!(gdwBitMasks[2] & ptTextData->dwFlags) || !(gdwBitMasks[0] & ptTextData->dwFlags))
		return FALSE;
	if (ptTextData->nNumLines < 1)
		return FALSE;
	if (ptTextData->dwTextBoxFlags & TEXTBOX_SELECT)
	{
		nY = ptTextData->nHeight - ptTextData->nY + sgtWindowData.tMouse.y;
		D2WinFontSet(ptTextData->tFont.dwFont);
		ptTextData->nSelectedNum = (nY - ptTextData->nTextMarginY) / ((int)D2WinFontGetHeight() + 4) + ptTextData->nTopLine;
		if (ptTextData->dwTextBoxFlags & TEXTBOX_KEEPSELECTED)
		{
			nLast = ptTextData->nNumLines - 1;
			nLine = (ptTextData->nSelectedNum <= 0) ? 0 : ptTextData->nSelectedNum;
			if (nLine < nLast)
				nLast = nLine;
			ptTextData->nSelectedNum = nLast;
		}
		else if (ptTextData->nSelectedNum < 0 || ptTextData->nSelectedNum >= ptTextData->nNumLines)
		{
			ptTextData->nSelectedNum = -1;
		}
	}
	if (!ptTextData->pfnCallback)
		return FALSE;
	return ptTextData->pfnCallback(pParams);
}

static BOOL __stdcall D2WinTextBoxOnKey(_PARAMS *pParams)
{
	D2WINTEXTBOX *ptTextData;
	D2WINTEXTBOX *hChild;
	int nNewTop;
	int nTop;
	int nVisible;
	int nLast;
	int n;

	ptTextData = (D2WINTEXTBOX *)pParams->window;
#line 951
	D2ASSERT(pParams->window);
	D2ASSERT(ptTextData->eType == D2WIN_TEXTBOX);
	if (!(gdwBitMasks[2] & ptTextData->dwFlags) || !(gdwBitMasks[0] & ptTextData->dwFlags))
		return FALSE;
	if (ptTextData->nNumLines < 1)
		return FALSE;
	if (ptTextData->nSelectedNum == -1)
		return FALSE;
	if (D2WinGetKeyFocus())
		return FALSE;
	nTop = ptTextData->nTopLine;
	hChild = (D2WINTEXTBOX *)pParams->window;
	nVisible = D2WinTextBoxGetVisibleLines(hChild);
	switch (pParams->dwWParam)
	{
	case VK_PRIOR:
		n = ptTextData->nSelectedNum;
		if (n <= 0)
			return FALSE;
		ptTextData->nSelectedNum = n - nVisible <= 0 ? 0 : n - nVisible;
		nTop -= nVisible;
		nNewTop = nTop;
		nNewTop = nNewTop <= 0 ? 0 : nNewTop;
		break;
	case VK_NEXT:
		nLast = ptTextData->nNumLines - 1;
		n = ptTextData->nSelectedNum;
		if (n >= nLast)
			return FALSE;
		n += nVisible;
		if (n < nLast)
			nLast = n;
		ptTextData->nSelectedNum = nLast;
		nLast = ptTextData->nNumLines - nVisible;
		nNewTop = nVisible + nTop;
		if (nNewTop >= nLast)
			nNewTop = nLast;
		break;
	case VK_UP:
		n = ptTextData->nSelectedNum;
		if (n <= 0)
			return FALSE;
		ptTextData->nSelectedNum = n - 1;
		nNewTop = nTop - 1;
		nNewTop = nNewTop <= 0 ? 0 : nNewTop;
		break;
	case VK_DOWN:
		n = ptTextData->nSelectedNum;
		if (n >= ptTextData->nNumLines - 1)
			return FALSE;
		ptTextData->nSelectedNum = n + 1;
		nNewTop = nTop + 1;
		if (nNewTop >= ptTextData->nNumLines - nVisible)
			nNewTop = ptTextData->nNumLines - nVisible;
		break;
	case VK_HOME:
		nNewTop = 0;
		ptTextData->nSelectedNum = 0;
		break;
	case VK_END:
		ptTextData->nSelectedNum = ptTextData->nNumLines - 1;
		nNewTop = ptTextData->nNumLines - nVisible;
		break;
	default:
		return FALSE;
	}
	if (ptTextData->nNumLines > nVisible)
		D2WinTextBoxSetTop((D2WINTEXTBOX *)pParams->window, nNewTop);
	if (ptTextData->pfnCallback)
		ptTextData->pfnCallback(pParams);
	return FALSE;
}
