#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinList.cpp"
#include "D2Win.h"
#include "D2Macros.h"

static int sganLineHeights[NUM_FONTS] = { 8, 16, 30, 42 };

static HCELFILE sghPentagram = NULL;
static DWORD sgdwLastClickTick = 0;

void __fastcall D2WinListAddItemUnicode(D2WINLIST *ptListData, const WORD *pszStr, PFNWINMSG pfnCallback,
	BYTE bChar, DWORD dwKey, BOOL fFirst, BOOL fSelect);
BOOL __fastcall D2WinListDraw(D2WINLIST *ptListData);
static BOOL __stdcall D2WinListMouseDown(_PARAMS *ptMsg);
static BOOL __stdcall D2WinListChar(_PARAMS *ptMsg);
static BOOL __stdcall D2WinListKey(_PARAMS *ptMsg);

D2WINLIST * __fastcall D2WinListCreate(int nX, int nY, int nWidth, int nHeight, D2WINLISTENTRY *ptEntries,
	D2WINFONTDESC *ptFont)
{
	D2WINLIST *ptListData;
	D2WINLISTITEM *ptItem;
	D2WINLISTITEM *ptLast;
	D2WINLISTENTRY *ptEntry;
	const WORD *pszStr;

#line 46
	ptListData = (D2WINLIST *)D2ALLOC2(sizeof(D2WINLIST));
	memset(ptListData, 0, sizeof(D2WINLIST));
	ptListData->eType = D2WIN_LIST;
	ptListData->dwFlags |= gdwBitMasks[0];
	ptListData->dwFlags |= gdwBitMasks[2];
	ptListData->hCelFile = NULL;
	ptListData->nX = nX;
	ptListData->nWidth = nWidth;
	ptListData->nY = nY;
	ptListData->nHeight = nHeight;
	ptListData->pfnDraw = (PFNWINCONTROL)D2WinListDraw;
	ptListData->pfnMouseDown = D2WinListMouseDown;
	ptListData->pfnMouseUp = NULL;
	ptListData->pfnChar = D2WinListChar;
	ptListData->pfnKey = D2WinListKey;
	ptListData->pfnCallback = NULL;
	ptListData->ptNextChild = 0;
	ptListData->nPentagramFrame = 0;
	ptListData->ptFirst = NULL;
	ptListData->dwFont = ptFont->dwFont;
	ptListData->nLineHeight = sganLineHeights[ptListData->dwFont];
	ptListData->nLinePadding = sganLineHeights[ptListData->dwFont] >> 2;
	ptListData->nMaxTextWidth = 0;
	ptListData->nTopIndex = 0;
	ptListData->nNumItems = 0;
	ptListData->nSelectedIndex = -1;
	ptListData->nMaxLineOffset = nHeight / (ptListData->nLineHeight + ptListData->nLinePadding * 2) - 1;
	D2WinFontSet(ptFont->dwFont);
	for (ptEntry = ptEntries; ptEntry->dwStringId; ptEntry++)
	{
		int nTextWidth, nTextHeight;
#line 86
		ptItem = (D2WINLISTITEM *)D2ALLOC2(sizeof(D2WINLISTITEM));
		pszStr = (const WORD *)D2LangStrGetStringByIndex((WORD)ptEntry->dwStringId);
#line 88
		D2ASSERT(Unicode::strlen( pszStr ) < MAX_LIST_STRCHARS);
		Unicode::strcpy(ptItem->wszText, pszStr);
		D2WinFontGetTextSize(pszStr, &nTextWidth, &nTextHeight);
		nTextWidth += 8;
		if (nTextWidth > ptListData->nMaxTextWidth)
			ptListData->nMaxTextWidth = nTextWidth;
		ptItem->nTextWidth = nTextWidth;
		ptItem->pfnCallback = ptEntry->pfnCallback;
		ptItem->nLeft = 0;
		ptItem->nRight = 0;
		ptItem->nTop = 0;
		ptItem->nBottom = 0;
		ptItem->bChar = ptEntry->bChar;
		ptItem->dwKey = ptEntry->dwKey;
		ptItem->ptNext = NULL;
		if (ptListData->ptFirst)
		{
			for (ptLast = ptListData->ptFirst; ptLast->ptNext; ptLast = ptLast->ptNext)
				;
			ptLast->ptNext = ptItem;
		}
		else
		{
			ptListData->ptFirst = ptItem;
		}
		ptListData->nNumItems++;
	}
	D2WinFontSet(1);
	ptListData->ptSelected = ptListData->ptFirst;
	D2WinMainAddChild(ptListData);
	return ptListData;
}

DWORD __fastcall D2WinListDestroy(D2WINLIST *ptListData)
{
	D2WINLISTITEM *ptItem;
	D2WINLISTITEM *ptNext;

#line 128
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	for (ptItem = ptListData->ptFirst; ptItem; ptItem = ptNext)
	{
		ptNext = ptItem->ptNext;
#line 133
		D2FREE2(ptItem);
	}
	return D2WinMainFreeChild(ptListData);
}

void __fastcall D2WinListAddItemUTF(D2WINLIST *ptListData, const char *pszStr, PFNWINMSG pfnCallback,
	BYTE bChar, DWORD dwKey, BOOL fFirst, BOOL fSelect)
{

	Unicode wszStr[MAX_LIST_STRCHARS];
	Unicode::win2Unicode(wszStr, pszStr, MAX_LIST_STRCHARS);
	D2WinListAddItemUnicode(ptListData, (const WORD *)wszStr, pfnCallback, bChar, dwKey, fFirst, fSelect);
}

void __fastcall D2WinListAddItemUnicode(D2WINLIST *ptListData, const WORD *pszStr, PFNWINMSG pfnCallback,
	BYTE bChar, DWORD dwKey, BOOL fFirst, BOOL fSelect)
{
	D2WINLISTITEM *ptItem;
	D2WINLISTITEM *ptLast;
	int nTextWidth, nTextHeight;

#line 161
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	D2WinFontSet(ptListData->dwFont);
#line 165
	ptItem = (D2WINLISTITEM *)D2ALLOC2(sizeof(D2WINLISTITEM));
	D2ASSERT(Unicode::strlen( pszStr ) < MAX_LIST_STRCHARS);
	Unicode::strcpy(ptItem->wszText, pszStr);
	D2WinFontGetTextSize(pszStr, &nTextWidth, &nTextHeight);
	nTextWidth += 8;
	if (nTextWidth > ptListData->nMaxTextWidth)
		ptListData->nMaxTextWidth = nTextWidth;
	ptItem->nTextWidth = nTextWidth;
	ptItem->pfnCallback = pfnCallback;
	ptItem->nLeft = 0;
	ptItem->nRight = 0;
	ptItem->nTop = 0;
	ptItem->nBottom = 0;
	ptItem->bChar = bChar;
	ptItem->dwKey = dwKey;
	if (!fFirst)
	{
		if (ptListData->ptFirst)
		{
			for (ptLast = ptListData->ptFirst; ptLast->ptNext; ptLast = ptLast->ptNext)
				;
			ptLast->ptNext = ptItem;
			ptItem->ptNext = NULL;
		}
		else
		{
			ptListData->ptFirst = ptItem;
			ptItem->ptNext = NULL;
		}
	}
	else
	{
		ptItem->ptNext = ptListData->ptFirst;
		ptListData->ptFirst = ptItem;
	}
	ptListData->nNumItems++;
	D2WinFontSet(1);
	if (fSelect)
	{
		ptListData->ptSelected = ptItem;
		ptListData->nSelectedIndex = ptListData->nNumItems - 1;
	}
}

int __fastcall D2WinListGetSelected(D2WINLIST *ptListData)
{
	D2WINLISTITEM *ptItem;
	int i;

#line 210
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	for (i = 0, ptItem = ptListData->ptFirst; ptItem; ptItem = ptItem->ptNext, i++)
	{
		if (ptListData->ptSelected == ptItem)
			break;
	}
	if (ptItem)
		return i;
	return -1;
}

D2WINLISTITEM * __fastcall D2WinListGetItem(D2WINLIST *ptListData, int nItem)
{
	D2WINLISTITEM *ptItem;
	int i;

#line 229
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	for (i = 0, ptItem = ptListData->ptFirst; ptItem; ptItem = ptItem->ptNext, i++)
	{
		if (i == nItem)
			break;
	}
	if (ptItem)
		return ptItem;
	return NULL;
}

int __fastcall D2WinListGetCount(D2WINLIST *ptListData)
{
#line 248
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	return ptListData->nNumItems;
}

void __fastcall D2WinListLoadPentagram(void)
{
	char szPath[260];

#line 259
	D2ASSERT(sghPentagram == NULL);
	wsprintfA(szPath, "%s\\UI\\CURSOR\\Pentspin", "DATA\\GLOBAL");
	sghPentagram = D2WinArchiveLoadCel(szPath, 0);
}

void __fastcall D2WinListFreePentagram(void)
{
	D2WinArchiveFreeCel(sghPentagram);
	sghPentagram = NULL;
}

BOOL __fastcall D2WinListDraw(D2WINLIST *ptListData)
{
	D2CELCONTEXT tCelContext;
	D2WINLISTITEM *ptItem;
	int nY;
	int nTextX;
	int nPentY;
	int nPentOffset;
	int nPentRightX;
	int i;
	int nWidth;

#line 281
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	D2WinFontSet(ptListData->dwFont);
	ptItem = ptListData->ptFirst;
	nY = ptListData->nLinePadding - ptListData->nHeight + ptListData->nLineHeight + ptListData->nY;
	for (i = ptListData->nTopIndex; i > 0; i--)
	{
		if (ptItem)
			ptItem = ptItem->ptNext;
	}
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = sghPentagram;
	tCelContext.dwDirection = 0;
	tCelContext.dwFrame = ptListData->nPentagramFrame;
	while (ptItem && nY < ptListData->nY)
	{
		nWidth = ptListData->nWidth;
		nTextX = ((nWidth - ptItem->nTextWidth) >> 1) + ptListData->nX;
		if (ptItem == ptListData->ptSelected)
		{
			nPentY = nY + 3;
			if (ptListData->dwFont == 2)
				nPentY += 6;
			nPentOffset = (ptListData->nWidth - ptListData->nMaxTextWidth) >> 2;
			nPentRightX = ptListData->nX - nPentOffset + ptListData->nWidth - 25;
			D2gfxScaleDrawCel(&tCelContext, ptListData->nX + nPentOffset - 25, nPentY, -1, 5,
				0);
			D2gfxScaleDrawCel(&tCelContext, nPentRightX, nPentY, -1, 5, 0);
		}
		D2WinFontDrawText(ptItem->wszText, nTextX, nY, 0, FALSE);
		ptItem->nLeft = ptListData->nX;
		ptItem->nRight = ptItem->nLeft + ptListData->nWidth;
		ptItem->nTop = nY - ptListData->nLinePadding - ptListData->nLineHeight;
		ptItem->nBottom = ptListData->nLinePadding + nY;
		nY += ptListData->nLineHeight + ptListData->nLinePadding * 2;
		ptItem = ptItem->ptNext;
	}
	ptListData->nPentagramFrame++;
	if (ptListData->nPentagramFrame >= 8)
		ptListData->nPentagramFrame = 0;
	D2WinFontSet(1);
	return TRUE;
}

static BOOL __stdcall D2WinListMouseDown(_PARAMS *ptMsg)
{
	D2WINLIST *ptListData;
	D2WINLISTITEM *ptItem;
	int nMouseX, nMouseY;
	DWORD dwTick;

	ptListData = (D2WINLIST *)ptMsg->window;
#line 340
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	nMouseX = sgtWindowData.tMouse.x;
	nMouseY = sgtWindowData.tMouse.y;
	dwTick = GetTickCount();
	for (ptItem = ptListData->ptFirst; ptItem; ptItem = ptItem->ptNext)
	{
		if (nMouseX > ptItem->nLeft && nMouseX < ptItem->nRight
			&& nMouseY > ptItem->nTop && nMouseY < ptItem->nBottom)
		{
			if (ptItem == ptListData->ptSelected && dwTick - sgdwLastClickTick < 300)
			{
				if (ptItem->pfnCallback)
					return ptItem->pfnCallback(ptMsg);
			}
			else
			{
				ptListData->ptSelected = ptItem;
			}
		}
	}
	sgdwLastClickTick = dwTick;
	if (ptListData->pfnCallback)
		return ptListData->pfnCallback(ptMsg);
	return FALSE;
}

static BOOL __stdcall D2WinListChar(_PARAMS *ptMsg)
{
	D2WINLIST *ptListData;
	D2WINLISTITEM *ptItem;

	ptListData = (D2WINLIST *)ptMsg->window;
#line 377
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	for (ptItem = ptListData->ptFirst; ptItem; ptItem = ptItem->ptNext)
	{
		if (ptItem->bChar == (BYTE)ptMsg->dwWParam && ptItem->pfnCallback)
			return ptItem->pfnCallback(ptMsg);
	}
	return FALSE;
}

static __inline BOOL D2WinListSetSelected(D2WINLIST *ptListData, D2WINLISTITEM *ptItem)
{
	D2WINLISTITEM *ptWalk;
	int nMore;

	ptListData->ptSelected = ptItem;
	ptListData->nSelectedIndex = 0;
	for (ptWalk = ptListData->ptFirst; ptWalk != ptItem; ptWalk = ptWalk->ptNext)
		ptListData->nSelectedIndex++;
	if (ptListData->nTopIndex > ptListData->nSelectedIndex)
	{
		ptListData->nTopIndex = ptListData->nSelectedIndex;
	}
	else
	{
		nMore = ptListData->nSelectedIndex - ptListData->nTopIndex;
		if (nMore > ptListData->nMaxLineOffset)
		{
			nMore -= ptListData->nMaxLineOffset;
			do
			{
				ptListData->nTopIndex++;
			} while (--nMore);
		}
	}
	D2WinSetKeyFocus(NULL);
	return TRUE;
}

static BOOL __stdcall D2WinListKey(_PARAMS *ptMsg)
{
	D2WINLIST *ptListData;
	D2WINLISTITEM *ptItem;

	ptListData = (D2WINLIST *)ptMsg->window;
#line 399
	D2ASSERT(ptListData->eType == D2WIN_LIST);
	if (ptMsg->dwMessage == WM_CHAR)
		return FALSE;
	switch (ptMsg->dwWParam)
	{
	case VK_DOWN:
		ptItem = ptListData->ptSelected->ptNext;
		if (!ptItem)
			ptItem = ptListData->ptFirst;
		return D2WinListSetSelected(ptListData, ptItem);
	case VK_UP:
		ptItem = ptListData->ptFirst;
		if (ptListData->ptFirst == ptListData->ptSelected)
		{
			while (ptItem->ptNext)
				ptItem = ptItem->ptNext;
		}
		else
		{
			while (ptItem->ptNext != ptListData->ptSelected)
				ptItem = ptItem->ptNext;
		}
		return D2WinListSetSelected(ptListData, ptItem);
	case VK_RETURN:
		D2WinSetKeyFocus(NULL);
		if (ptListData->ptSelected->pfnCallback)
			return ptListData->ptSelected->pfnCallback(ptMsg);
	default:
		for (ptItem = ptListData->ptFirst; ptItem; ptItem = ptItem->ptNext)
		{
			if ((WORD)ptItem->dwKey == ptMsg->dwWParam && ptItem->pfnCallback)
				return ptItem->pfnCallback(ptMsg);
		}
		return FALSE;
	}
}
