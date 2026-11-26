#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinEditBox.cpp"
#include "D2Win.h"
#include "D2Macros.h"

typedef char D2EDITCLIPBOARD[512];
static D2EDITCLIPBOARD sgszClipboard = { 0 };
static HGLOBAL sghClipboard = { 0 };

static __inline int D2WinMinInt(int a, int b)
{
	return a < b ? a : b;
}

static __inline int D2WinMaxInt(int a, int b)
{
	return a > b ? a : b;
}

#define SELECTION_START(pt)	  D2WinMinInt((pt)->nSelectionStart, (pt)->nSelectionEnd)
#define SELECTION_END(pt)	  D2WinMaxInt((pt)->nSelectionStart, (pt)->nSelectionEnd)

BOOL __fastcall D2WinEditBoxSetStringUnicode(D2WINEDITBOX *hChild, const WORD *pwszText);
static void __fastcall D2WinEditBoxDeleteSelection(D2WINEDITBOX *hChild);
static BOOL __fastcall D2WinEditBoxDraw(D2WINEDITBOX *ptChild);
static BOOL __fastcall D2WinEditBoxDrawMultiLine(D2WINEDITBOX *ptChild);
static void __fastcall D2WinEditBoxGetCaretPoint(D2WINEDITBOX *hChild, int *pnX, int *pnY);
static BOOL __stdcall D2WinEditBoxMouseDown(_PARAMS *ptMsg);
static BOOL __stdcall D2WinEditBoxMouseUp(_PARAMS *ptMsg);
static BOOL __fastcall D2WinEditBoxSetCaretFromPoint(_PARAMS *ptMsg, int nX, int nY);
static BOOL __stdcall D2WinEditBoxMouseDownMultiLine(_PARAMS *ptMsg);
static void __fastcall D2WinEditBoxUpdateView(D2WINEDITBOX *ptEditData);

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

D2WINEDITBOX * __fastcall D2WinEditBoxCreate(int nX, int nY, int nWidth, int nHeight, int nMarginX,
	int nMarginY, HCELFILE hCelFile, PFNEDITRETURN pfnReturn, PFNWINMSG pfnCallback, DWORD dwEditFlags,
	D2WINFONTDESC *ptFont)
{
	D2WINEDITBOX *ptEditData;

#line 44
	ptEditData = (D2WINEDITBOX *)D2ALLOC2(sizeof(D2WINEDITBOX));
	memset(ptEditData, 0, sizeof(D2WINEDITBOX));
	ptEditData->eType = D2WIN_EDITBOX;
	ptEditData->dwFlags |= gdwBitMasks[0];
	ptEditData->dwFlags |= gdwBitMasks[2];
	ptEditData->hCelFile = hCelFile;
	ptEditData->nX = nX;
	ptEditData->nWidth = nWidth;
	ptEditData->nHeight = nHeight;
	ptEditData->nTextMarginX = nMarginX;
	ptEditData->nTextMarginY = nMarginY;
	ptEditData->nMaxTextLength = 256;
	ptEditData->nY = nY;
	ptEditData->nFirstShown = 0;
	ptEditData->nLastShown = 0;
	ptEditData->nSelectionStart = -1;
	ptEditData->pfnDraw = (dwEditFlags & EDITBOX_MULTILINE) ? (PFNWINCONTROL)D2WinEditBoxDrawMultiLine
		: (PFNWINCONTROL)D2WinEditBoxDraw;
	ptEditData->pfnMouseDown = (dwEditFlags & EDITBOX_MULTILINE) ? D2WinEditBoxMouseDownMultiLine
		: D2WinEditBoxMouseDown;
	ptEditData->pfnMouseUp = D2WinEditBoxMouseUp;
	ptEditData->pfnChar = NULL;
	ptEditData->pfnKey = NULL;
	ptEditData->pfnEditKey = NULL;
	ptEditData->pfnCallback = pfnCallback;
	ptEditData->ptNextChild = 0;
	ptEditData->wszText[0] = 0;
	ptEditData->nCaretPos = 0;
	ptEditData->dwEditBoxFlags = dwEditFlags;
	ptEditData->pfnEditReturn = pfnReturn;
	if (ptFont)
	{
		ptEditData->tFont.dwFont = ptFont->dwFont;
		ptEditData->tFont.dwColor = ptFont->dwColor;
	}
	else
	{
		ptEditData->tFont.dwFont = 1;
		ptEditData->tFont.dwColor = 0;
	}
	ptEditData->ptNextBox = NULL;
	ptEditData->ptPrevBox = NULL;
	D2WinMainAddChild(ptEditData);
	return ptEditData;
}

DWORD __fastcall D2WinEditBoxDestroy(D2WINEDITBOX *hChild)
{
	D2WINEDITBOX *ptEditData;

#line 97
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 99
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	return D2WinMainFreeChild(ptEditData);
}

void __fastcall D2WinEditBoxSetKeyProc(D2WINEDITBOX *hChild, PFNEDITKEY pfnKeyProc)
{
	D2WINEDITBOX *ptEditData;

#line 108
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 110
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	ptEditData->pfnEditKey = pfnKeyProc;
}

void __fastcall D2WinEditBoxSetKeyPostProc(D2WINEDITBOX *hChild, PFNEDITPOST pfnKeyPostProc)
{
	D2WINEDITBOX *ptEditData;

#line 121
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 123
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	ptEditData->pfnEditPost = pfnKeyPostProc;
}

char * __fastcall D2WinEditBoxGetClipboard(void)
{
	HANDLE hText;
	char *pszText;

	if (!IsClipboardFormatAvailable(CF_TEXT))
		return "";
	if (!OpenClipboard(D2gfxWindowGetHandle()))
		return "";
	hText = GetClipboardData(CF_TEXT);
	if (hText)
	{
		pszText = (char *)GlobalLock(hText);
		if (pszText)
		{
			strncpy(sgszClipboard, pszText, sizeof(D2EDITCLIPBOARD));
			GlobalUnlock(hText);
		}
	}
	CloseClipboard();
	return sgszClipboard;
}

BOOL __fastcall D2WinEditBoxSetClipboard(const char *pszText)
{
	char *pszCopy;
	size_t nLength;

	if (sghClipboard)
		GlobalFree(sghClipboard);
	if (!OpenClipboard(D2gfxWindowGetHandle()))
		return FALSE;
	EmptyClipboard();
	nLength = strlen(pszText);
	sghClipboard = GlobalAlloc(GMEM_DDESHARE, nLength + 1);
	if (!sghClipboard)
	{
		CloseClipboard();
		return FALSE;
	}
	pszCopy = (char *)GlobalLock(sghClipboard);
	memcpy(pszCopy, pszText, nLength);
	pszCopy[nLength] = '\0';
	GlobalUnlock(sghClipboard);
	SetClipboardData(CF_TEXT, sghClipboard);
	CloseClipboard();
	return TRUE;
}

static inline BOOL D2WinEditBoxHasSelection(D2WINEDITBOX *ptEditData)
{
#line 233
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	return ptEditData->nSelectionStart != -1 && ptEditData->nSelectionStart != ptEditData->nSelectionEnd;
}

extern "C" void __fastcall D2WinEditBoxDeselect(D2WINEDITBOX *hChild)
{
	D2WINEDITBOX *ptEditData;

#line 243
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 245
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	ptEditData->nSelectionStart = -1;
	ptEditData->nSelectionEnd = 0;
}

extern "C" void __fastcall D2WinEditBoxSelectAll(D2WINEDITBOX *hChild)
{
	D2WINEDITBOX *ptEditData;

#line 258
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 260
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	ptEditData->nSelectionStart = 0;
	ptEditData->nSelectionEnd = Unicode::strlen(ptEditData->wszText);
}

static inline BOOL D2WinEditBoxTextFits(D2WINEDITBOX *ptEditData, const WORD *pwszText)
{
	char szText[512];
	char szLine[256];
	char *psz;
	char *pszLine;
	int nAvailWidth;
	struct
	{
		int nTop;
		int nHeight;
	} tSpan;
	int nY;
	int nCaretWidth;
	int nFontHeight;
	int nIndex;
	BOOL fWrapped;

	tSpan.nHeight = ptEditData->nHeight - ptEditData->nTextMarginY * 2;
	if (!(ptEditData->dwEditBoxFlags & EDITBOX_MULTILINE))
		return TRUE;
#line 1301
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	D2WinFontSet(ptEditData->tFont.dwFont);
	tSpan.nTop = (int)D2WinFontGetHeight() + (ptEditData->nY - ptEditData->nHeight) + ptEditData->nTextMarginY;
	char szCaret[] = "_";
	Unicode wszCaret[2];
	Unicode::win2Unicode(wszCaret, szCaret, 2);
	nCaretWidth = D2WinFontGetTextWidthN((const WORD *)wszCaret, 2);
	Unicode wszLine[256];
	Unicode::unicode2Win(szText, pwszText, 256);
	pszLine = szText;
	psz = szText;
	nY = tSpan.nTop;
	nFontHeight = (int)D2WinFontGetHeight();
	nAvailWidth = ptEditData->nWidth - ptEditData->nTextMarginX * 2;
	while (*psz)
	{
		nIndex = psz - pszLine;
		fWrapped = FALSE;
		memset(szLine, 0, sizeof(szLine));
		while (*psz && *psz != '\r' && *psz != '\n')
		{
			szLine[nIndex] = *psz;
			Unicode::win2Unicode(wszLine, szLine, 256);
			if (D2WinFontGetTextWidthN((const WORD *)wszLine, nIndex) + nCaretWidth > nAvailWidth)
			{
				fWrapped = TRUE;
				break;
			}
			psz++;
			nIndex++;
		}
		Unicode::win2Unicode(wszLine, szLine, 256);
		if (fWrapped)
			nY += nFontHeight;
		if (*psz == '\r' && psz[1] == '\n')
		{
			while (*psz == '\r' && psz[1] == '\n')
			{
				psz += 2;
				nY += nFontHeight;
			}
		}
		else if (*psz == '\r' && psz[1] != '\n')
		{
			do
			{
				psz++;
				nY += nFontHeight;
			} while (*psz == '\r');
		}
		pszLine = psz;
	}
	if (nY - tSpan.nTop >= tSpan.nHeight)
		return FALSE;
	return TRUE;
}

static inline BOOL D2WinEditBoxIsCharFF(WORD wChar)
{
	if (wChar >= 256)
		return FALSE;
	return wChar == 255;
}

BOOL __fastcall D2WinEditBoxOnChar(D2WINEDITBOX *hChild, char chKey)
{
	D2WINEDITBOX *ptEditData;

	WORD wKey;
	WORD *pwszCaret;
	WORD *pwszSelection;
	const char *pszClipboard;
	const char *pszFrom;
	char *pszTo;
	int nLength;
	int nStart, nEnd;
	int nCount;
	int nCaret;
	int i;

#line 325
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 327
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (!(ptEditData->dwFlags & gdwBitMasks[2]) || !(ptEditData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	if (chKey == '\t' && (ptEditData->dwEditBoxFlags & EDITBOX_TABFILTER))
	{
		if (ptEditData->pfnEditKey)
		{
			sgtWindowData.deadChildrenLocked++;
			ptEditData->pfnEditKey(ptEditData, ptEditData->nCaretPos, &chKey);
			sgtWindowData.deadChildrenLocked--;
		}
		return FALSE;
	}
	wKey = (BYTE)chKey;
	if (!D2WinFontIsValidChar(chKey)
		&& !((ptEditData->dwEditBoxFlags & EDITBOX_CHARFF) && D2WinEditBoxIsCharFF(wKey))
		&& !((ptEditData->dwEditBoxFlags & EDITBOX_MULTILINE) && chKey == '\r')
		&& !((ptEditData->dwEditBoxFlags & EDITBOX_CHAR0E) && chKey == 14)
		&& chKey != 22 && chKey != 3 && chKey != 24 && chKey != 1)
	{
		return FALSE;
	}
	if (ptEditData->pfnEditKey)
	{
		if (!ptEditData->pfnEditKey(ptEditData, ptEditData->nCaretPos, &chKey)
			&& chKey != 22 && chKey != 3 && chKey != 24 && chKey != 1)
		{
			return FALSE;
		}
	}
	if (D2WinEditBoxHasSelection(ptEditData) && chKey != 3 && chKey != 24 && chKey != 1)
		D2WinEditBoxDeleteSelection(ptEditData);
	nLength = Unicode::strlen(ptEditData->wszText);
	Unicode wszNew[512];
	Unicode::strcpy(wszNew, ptEditData->wszText);
	pwszCaret = (WORD *)&wszNew[ptEditData->nCaretPos];
	switch (chKey)
	{ char szText[400];
	case 22:
	{
		pszClipboard = D2WinEditBoxGetClipboard();
		if (strlen(pszClipboard) + nLength > (DWORD)(ptEditData->nMaxTextLength - 1))
			return FALSE;
		if (ptEditData->pfnEditKey)
		{
			nCount = (int)strlen(pszClipboard);
			nCaret = ptEditData->nCaretPos;
			for (i = 0; i < nCount; i++)
			{
				if (!D2WinFontIsValidChar(pszClipboard[i]))
					return FALSE;
				if (!ptEditData->pfnEditKey(ptEditData, nCaret + i, (char *)&pszClipboard[i]))
					return FALSE;
			}
		}
		Unicode wszHead[256];
		Unicode wszInsert[256];
		Unicode wszTail[256];
		if (ptEditData->nSelectionStart == -1)
		{
			nStart = ptEditData->nCaretPos;
			nEnd = ptEditData->nCaretPos;
		}
		else
		{
			nStart = SELECTION_START(ptEditData);
			nEnd = SELECTION_END(ptEditData);
		}
		if (ptEditData->dwEditBoxFlags & EDITBOX_MULTILINE)
		{
			strcpy(szText, pszClipboard);
		}
		else
		{
			for (pszFrom = pszClipboard, pszTo = szText; *pszFrom; pszFrom++)
			{
				if (*pszFrom != '\r' && *pszFrom != '\n')
					*pszTo++ = *pszFrom;
			}
			*pszTo = '\0';
		}
		Unicode::strncpy(wszHead, wszNew, nStart);

		Unicode::strcpy(wszTail, &wszNew[nEnd]);
		Unicode::win2Unicode(wszInsert, szText, (int)strlen(szText) + 1);
		Unicode::strcpy(wszNew, wszHead);
		Unicode::strcat(wszNew, wszInsert);
		Unicode::strcat(wszNew, wszTail);
		D2WinEditBoxDeselect(ptEditData);
		pwszCaret = (WORD *)&wszNew[strlen(szText) + nStart];
		break;
	}
	case 3:
		if (D2WinEditBoxHasSelection(ptEditData))
		{
			nStart = SELECTION_START(ptEditData);
			nEnd = SELECTION_END(ptEditData);
			nCount = nEnd - nStart + 1;
			if (nCount <= 2)
				nCount = 2;
			Unicode::unicode2Win(szText, &wszNew[nStart], nCount);
			D2WinEditBoxSetClipboard(szText);
		}
		break;
	case 24:
		if (D2WinEditBoxHasSelection(ptEditData))
		{
			nStart = SELECTION_START(ptEditData);
			nEnd = SELECTION_END(ptEditData);
			nCount = nEnd - nStart + 1;
			if (nCount <= 2)
				nCount = 2;
			pwszSelection = (WORD *)&wszNew[nStart];
			Unicode::unicode2Win(szText, pwszSelection, nCount);
			D2WinEditBoxSetClipboard(szText);
			D2WinEditBoxDeleteSelection(ptEditData);
			Unicode::strlen(ptEditData->wszText);
			Unicode::strcpy(wszNew, ptEditData->wszText);
			pwszCaret = pwszSelection;
		}
		break;
	case 1:
		D2WinEditBoxSelectAll(ptEditData);
		break;
	default:
	{
		char szTail[512];

		if (nLength >= ptEditData->nMaxTextLength - 1)
			return FALSE;
		if (nLength < 255)
		{
			if (chKey == '\r')
			{
				Unicode::unicode2Win(&szTail[2], pwszCaret, 256);
				szTail[0] = '\r';
				szTail[1] = '\n';
				Unicode::win2Unicode(pwszCaret, szTail, 256);
				pwszCaret += 2;
			}
			else
			{
				Unicode::unicode2Win(&szTail[1], pwszCaret, 256);
				szTail[0] = chKey;
				Unicode::win2Unicode(pwszCaret, szTail, 256);
				pwszCaret++;
			}
			D2WinEditBoxDeselect(ptEditData);
		}
		break;
	}
	}
	if (D2WinEditBoxTextFits(ptEditData, (const WORD *)wszNew))
	{
		Unicode::strcpy(ptEditData->wszText, wszNew);
		ptEditData->nCaretPos = pwszCaret - (WORD *)wszNew;
		D2WinEditBoxUpdateView(ptEditData);
		if (ptEditData->pfnEditPost)
			ptEditData->pfnEditPost(0);
	}
	return TRUE;
}

static void __fastcall D2WinEditBoxDeleteSelection(D2WINEDITBOX *hChild)
{
	D2WINEDITBOX *ptEditData;
	int nStart, nEnd;
	int nLength;

#line 273
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 275
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	Unicode wszHead[256];
	Unicode wszTail[256];
	nStart = SELECTION_START(ptEditData);
	nEnd = SELECTION_END(ptEditData);
	if (nStart)
		Unicode::strncpy(wszHead, ptEditData->wszText, nStart);
	else
		wszHead[0].wChar = 0;
	Unicode::strcpy(wszTail, &ptEditData->wszText[nEnd]);
	Unicode::strcpy(ptEditData->wszText, wszHead);
	Unicode::strcat(ptEditData->wszText, wszTail);
	nLength = Unicode::strlen(ptEditData->wszText);
	if (nStart <= nLength)
		ptEditData->nCaretPos = nStart;
	else
		ptEditData->nCaretPos = nLength;
	D2WinEditBoxDeselect(ptEditData);
}

static __inline void D2WinEditBoxCaretLines(D2WINEDITBOX *ptEditData, int nLines)
{
	_PARAMS tMsg;
	int nX, nY;
	int nHeight;

	D2WinEditBoxGetCaretPoint(ptEditData, &nX, &nY);
	nHeight = (int)D2WinFontGetHeight();
	tMsg.window = ptEditData;
	tMsg.dwMessage = 0;
	D2WinEditBoxSetCaretFromPoint(&tMsg, nX, nY + nLines * nHeight);
}

BOOL __fastcall D2WinEditBoxOnKeyDown(D2WINEDITBOX *hChild, char chKey)
{
	D2WINEDITBOX *ptEditData;
	D2WINEDITBOX *ptNextData;
	WORD *pwsz;
	BOOL fChanged;
	BOOL fDeleted;
	DWORD dwShift;
	DWORD dwKeepFocus;
	int nCount;

#line 560
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 562
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (!(ptEditData->dwFlags & gdwBitMasks[2]) || !(ptEditData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	pwsz = &ptEditData->wszText[ptEditData->nCaretPos];
	fChanged = FALSE;
	fDeleted = FALSE;
	dwShift = (GetKeyState(VK_SHIFT) >> 4) & 15;
	switch (chKey)
	{
	case VK_BACK:
	case VK_DELETE:
		if (D2WinEditBoxHasSelection(ptEditData))
		{
			D2WinEditBoxDeleteSelection(ptEditData);
			fChanged = TRUE;
			fDeleted = TRUE;
			pwsz = &ptEditData->wszText[ptEditData->nCaretPos];
		}
		break;
	case VK_END:
	case VK_HOME:
	case VK_LEFT:
	case VK_UP:
	case VK_RIGHT:
	case VK_DOWN:
		if (dwShift)
		{
			if (ptEditData->nSelectionStart == -1)
			{
				ptEditData->nSelectionStart = ptEditData->nCaretPos;
				ptEditData->nSelectionEnd = ptEditData->nCaretPos;
			}
		}
		else
		{
			D2WinEditBoxDeselect(ptEditData);
		}
		break;
	}
	switch (chKey)
	{
	case VK_HOME:
		pwsz = ptEditData->wszText;
		if (dwShift)
			ptEditData->nSelectionEnd = 0;
		break;
	case VK_END:
		while (*pwsz)
			pwsz++;
		if (dwShift)
			ptEditData->nSelectionEnd = pwsz - ptEditData->wszText;
		break;
	case VK_UP:
		if (ptEditData->dwEditBoxFlags & EDITBOX_MULTILINE)
		{
			D2WinEditBoxCaretLines(ptEditData, -1);
			pwsz = &ptEditData->wszText[ptEditData->nCaretPos];
		}
		else
		{
			pwsz = ptEditData->wszText;
			if (dwShift)
				ptEditData->nSelectionEnd = 0;
		}
		break;
	case VK_F1:
		return FALSE;
	case VK_DOWN:
		if (ptEditData->dwEditBoxFlags & EDITBOX_MULTILINE)
		{
			D2WinEditBoxCaretLines(ptEditData, 1);
			pwsz = &ptEditData->wszText[ptEditData->nCaretPos];
		}
		else
		{
			while (*pwsz)
				pwsz++;
			if (dwShift)
				ptEditData->nSelectionEnd = pwsz - ptEditData->wszText;
		}
		break;
	case VK_LEFT:
		pwsz--;
		if (*pwsz == '\n')
			pwsz--;
		if (pwsz < ptEditData->wszText)
			pwsz = ptEditData->wszText;
		else if (dwShift && ptEditData->nSelectionStart != -1)
			ptEditData->nSelectionEnd = pwsz - ptEditData->wszText;
		break;
	case VK_RIGHT:
		if (*pwsz)
		{
			pwsz++;
			if (*pwsz && *pwsz == '\n')
				pwsz++;
			if (dwShift && ptEditData->nSelectionStart != -1)
				ptEditData->nSelectionEnd = pwsz - ptEditData->wszText;
		}
		break;
	case VK_BACK:
		if (pwsz > ptEditData->wszText && !fDeleted)
		{
			pwsz--;
			nCount = Unicode::strlen(pwsz + 1) + 1;
			memmove(pwsz, pwsz + 1, nCount * 2);
			if (pwsz > ptEditData->wszText && pwsz[-1] == '\r')
			{
				pwsz--;
				memmove(pwsz, pwsz + 1, nCount * 2);
			}
			fChanged = TRUE;
		}
		break;
	case VK_DELETE:
		if (*pwsz && !fDeleted)
		{
			nCount = Unicode::strlen(pwsz + 1) + 1;
			memmove(pwsz, pwsz + 1, nCount * 2);
			if (*pwsz && *pwsz == '\n')
				memmove(pwsz, pwsz + 1, nCount * 2 - 2);
			fChanged = TRUE;
		}
		break;
	case VK_RETURN:
		if (ptEditData->pfnEditReturn)
		{
			dwKeepFocus = ptEditData->dwEditBoxFlags & EDITBOX_KEEPFOCUS;
			sgtWindowData.deadChildrenLocked++;
			ptEditData->pfnEditReturn(ptEditData->wszText);
			sgtWindowData.deadChildrenLocked--;
			if (dwKeepFocus)
				pwsz = &ptEditData->wszText[ptEditData->nCaretPos];
			else
				return TRUE;
		}
		break;
	case VK_ESCAPE:
		if (ptEditData->pfnEditReturn)
		{
			sgtWindowData.deadChildrenLocked++;
			ptEditData->pfnEditReturn(NULL);
			sgtWindowData.deadChildrenLocked--;
		}
		return TRUE;
	case VK_TAB:
		if (GetKeyState(VK_SHIFT) >= 0)
		{
			ptNextData = ptEditData->ptNextBox;
			if (ptNextData)
			{
				while (1)
				{
					if (ptNextData == ptEditData)
						break;
					if ((ptNextData->dwFlags & gdwBitMasks[2]) && (ptNextData->dwFlags & gdwBitMasks[0]))
						break;
					ptNextData = ptNextData->ptNextBox;
					if (!ptNextData)
						break;
				}
				D2WinSetKeyFocus(ptNextData);
			}
		}
		else
		{
			ptNextData = ptEditData->ptPrevBox;
			if (ptNextData)
			{
				while (1)
				{
					if (ptNextData == ptEditData)
						break;
					if ((ptNextData->dwFlags & gdwBitMasks[2]) && (ptNextData->dwFlags & gdwBitMasks[0]))
						break;
					ptNextData = ptNextData->ptPrevBox;
					if (!ptNextData)
						break;
				}
				D2WinSetKeyFocus(ptNextData);
			}
		}
		return TRUE;
	}
	ptEditData->nCaretPos = pwsz - ptEditData->wszText;
	D2WinEditBoxUpdateView(ptEditData);
	if (fChanged && ptEditData->pfnEditKey)
	{
		if (!ptEditData->pfnEditKey(ptEditData, ptEditData->nCaretPos, &chKey) && chKey != VK_BACK
			&& chKey != VK_DELETE)
		{
			return FALSE;
		}
	}
	if (ptEditData->pfnEditPost)
		ptEditData->pfnEditPost(0);
	return TRUE;
}
#line 751

void __fastcall D2WinEditBoxSetMaxLength(D2WINEDITBOX *hChild, int nMaxLength)
{
	D2WINEDITBOX *ptEditData;

#line 878
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 881
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	ptEditData->nMaxTextLength = nMaxLength;
}

BOOL __fastcall D2WinEditBoxSetStringUTF(D2WINEDITBOX *hChild, const char *pszText)
{

	Unicode wszText[256];
	Unicode::win2Unicode(wszText, pszText, 256);
	return D2WinEditBoxSetStringUnicode(hChild, (const WORD *)wszText);
}

#pragma auto_inline(off)
BOOL __fastcall D2WinEditBoxSetStringUnicode(D2WINEDITBOX *hChild, const WORD *pwszText)
{
	D2WINEDITBOX *ptEditData;
	int nLength;

#line 903
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 905
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	nLength = Unicode::strlen(pwszText);
	if (nLength)
		Unicode::strcpy(ptEditData->wszText, pwszText);
	else
		ptEditData->wszText[0] = 0;
	ptEditData->nCaretPos = nLength;
	D2WinEditBoxUpdateView(ptEditData);
	if (ptEditData->pfnEditPost)
		ptEditData->pfnEditPost(0);
	return TRUE;
}
#pragma auto_inline(on)

WORD * __fastcall D2WinEditBoxGetString(D2WINEDITBOX *hChild)
{
	D2WINEDITBOX *ptEditData;

#line 935
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 937
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	return ptEditData->wszText;
}

void __fastcall D2WinEditBoxSetTabNext(D2WINEDITBOX *hChild, D2WINEDITBOX *hNext)
{
	D2WINEDITBOX *ptEditData;
	D2WINEDITBOX *ptNextData;

#line 947
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 949
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
#line 951
	D2ASSERT(hNext);
	ptNextData = hNext;
#line 953
	D2ASSERT(ptNextData->eType == D2WIN_EDITBOX);
	ptEditData->ptNextBox = ptNextData;
	ptNextData->ptPrevBox = ptEditData;
}

static BOOL __fastcall D2WinEditBoxDraw(D2WINEDITBOX *ptChild)
{
	D2WINEDITBOX *ptEditData;
	D2CELCONTEXT tCelContext;
	_PARAMS tMsg;
	BYTE bSelectionColor;
	int nTextX, nTextY;
	int nLength;
	int nStart, nCount;
	int nBefore;
	int i;

#line 965
	D2ASSERT(ptChild);
	ptEditData = ptChild;
#line 967
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (!(ptEditData->dwFlags & gdwBitMasks[2]) || !(ptEditData->dwFlags & gdwBitMasks[0]))
		return TRUE;
	if (!D2WinIsMouseDown())
		ptEditData->fMouseDown = FALSE;
	if (ptEditData->fMouseDown)
	{
		tMsg.window = ptEditData;
		tMsg.dwMessage = WM_LBUTTONDOWN;
		D2WinEditBoxMouseDown(&tMsg);
	}
	Unicode wszShown[300];
	if (ptEditData->hCelFile)
	{
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = ptEditData->hCelFile;
		tCelContext.dwDirection = 0;
		tCelContext.dwFrame = 0;
		D2gfxScaleDrawCel(&tCelContext, ptEditData->nX, ptEditData->nY - 1, -1, 5, 0);
	}
	D2WinFontSet(ptEditData->tFont.dwFont);
	nTextX = ptEditData->nTextMarginX + ptEditData->nX;
	nTextY = ptEditData->nTextMarginY - ptEditData->nHeight + ptEditData->nY + 16;
	Unicode::strncpy(wszShown, &ptEditData->wszText[ptEditData->nFirstShown],
		ptEditData->nLastShown - ptEditData->nFirstShown + 1);
	nLength = Unicode::strlen(wszShown);
	if (ptEditData->dwEditBoxFlags & EDITBOX_PASSWORD)
	{
		char szStar[] = "*";
		Unicode wszChar[2];

		Unicode::win2Unicode(wszChar, szStar, 2);
		for (i = 0; i < nLength; i++)
			wszShown[i] = wszChar[0];
	}
	if (D2WinEditBoxHasSelection(ptEditData))
	{ int nTextHeight;
		int nHeadWidth, nSelectedWidth, nTailX;
		bSelectionColor = (BYTE)D2WinPaletteGetColor(64, 64, 64);
		Unicode wszHead[256];
		Unicode wszSelected[256];
		Unicode wszTail[256];
		nStart = SELECTION_START(ptEditData)
			- ptEditData->nFirstShown;
		nStart = nStart < 0 ? 0 : nStart;
		nCount = SELECTION_END(ptEditData)
			- ptEditData->nFirstShown - nStart;
		nCount = nCount < 0 ? 0 : nCount;
		Unicode::strncpy(wszHead, wszShown, nStart);
		Unicode::strncpy(wszSelected, &wszShown[nStart], nCount);
		Unicode::strcpy(wszTail, &wszShown[nCount + nStart]);
		D2WinFontGetTextSize((const WORD *)wszHead, &nHeadWidth, &nTextHeight);
		D2WinFontGetTextSize((const WORD *)wszSelected, &nSelectedWidth, &nTextHeight);
		nHeadWidth += nTextX;
		nTailX = nSelectedWidth + nHeadWidth;
		D2WinFontDrawText((const WORD *)wszHead, nTextX, nTextY, ptEditData->tFont.dwColor, FALSE);
		D2WinFontDrawFramedText((const WORD *)wszSelected, nHeadWidth, nTextY, bSelectionColor, 5,
			ptEditData->tFont.dwColor);
		D2WinFontDrawText((const WORD *)wszTail, nTailX, nTextY, ptEditData->tFont.dwColor, FALSE);
	}
	else
	{
		D2WinFontDrawText((const WORD *)wszShown, nTextX, nTextY, ptEditData->tFont.dwColor, FALSE);
	}
	if ((D2WINEDITBOX *)D2WinGetKeyFocus() == ptEditData && ((GetTickCount() / 250) & 1))
	{
		char szCaret[] = "_";
		Unicode wszChar[2];

		Unicode::win2Unicode(wszChar, szCaret, 2);
		nBefore = ptEditData->nCaretPos - ptEditData->nFirstShown;
		if (nBefore)
			nTextX += D2WinFontGetTextWidthN((const WORD *)wszShown, nBefore);
		D2WinFontDrawText((const WORD *)wszChar, nTextX, nTextY, ptEditData->tFont.dwColor, FALSE);
	}
	return TRUE;
}

static BOOL __fastcall D2WinEditBoxDrawMultiLine(D2WINEDITBOX *ptChild)
{
	D2WINEDITBOX *ptEditData;
	_PARAMS tMsg;
	char szText[256];
	char szLine[256];
	char *psz;
	char *pszLine;
	BYTE bSelectionColor;
	BOOL fCaret;
	BOOL fCaretDrawn;
	BOOL fWrapped;
	int nX, nY;
	int nTop;
	int nCaretWidth;
	int nFontHeight;
	int nAvailWidth, nAvailHeight;
	int nIndex;
	int nSelStart, nSelEnd;
	int nStart, nEnd;
	int nLineStart, nLineEnd;
	int nSelectionX;
	int nBefore;
	int nCount;
	int nWidth, nHeight;
	int nColumn;
	int nCaretX;

#line 1081
	D2ASSERT(ptChild);
	ptEditData = ptChild;
#line 1083
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (!(ptEditData->dwFlags & gdwBitMasks[2]) || !(ptEditData->dwFlags & gdwBitMasks[0]))
		return TRUE;
	if (!D2WinIsMouseDown())
		ptEditData->fMouseDown = FALSE;
	if (ptEditData->fMouseDown)
	{
		tMsg.window = ptEditData;
		tMsg.dwMessage = WM_LBUTTONDOWN;
		D2WinEditBoxMouseDownMultiLine(&tMsg);
	}
	Unicode wszCopy[300];
	D2WinFontSet(ptEditData->tFont.dwFont);
	nX = ptEditData->nX + ptEditData->nTextMarginX;
	nTop = (int)D2WinFontGetHeight() + (ptEditData->nY - ptEditData->nHeight) + ptEditData->nTextMarginY;
	Unicode::strcpy(wszCopy, ptEditData->wszText);
	bSelectionColor = (BYTE)D2WinPaletteGetColor(64, 64, 64);
	char szCaret[] = "_";
	Unicode wszCaret[2];
	Unicode::win2Unicode(wszCaret, szCaret, 2);
	nCaretWidth = D2WinFontGetTextWidthN((const WORD *)wszCaret, 2);
	if ((D2WINEDITBOX *)D2WinGetKeyFocus() == ptEditData && ((GetTickCount() / 250) & 1))
		fCaret = TRUE;
	else
		fCaret = FALSE;
	fCaretDrawn = FALSE;
	Unicode wszLine[256];
	Unicode::unicode2Win(szText, ptEditData->wszText, 256);
	pszLine = szText;
	psz = szText;
	nY = nTop;
	nFontHeight = (int)D2WinFontGetHeight();
	nAvailWidth = ptEditData->nWidth - ptEditData->nTextMarginX * 2;
	nAvailHeight = ptEditData->nHeight - ptEditData->nTextMarginY * 2;
	if (!szText[0])
	{
		if (!fCaret)
			return TRUE;
		D2WinFontDrawText((const WORD *)wszCaret, nX, nTop, ptEditData->tFont.dwColor, FALSE);
		fCaretDrawn = TRUE;
	}
	while (*psz)
	{
		if (nY - nTop >= nAvailHeight)
			return TRUE;
		nIndex = psz - pszLine;
		fWrapped = FALSE;
		memset(szLine, 0, sizeof(szLine));
		while (*psz && *psz != '\r' && *psz != '\n')
		{
			szLine[nIndex] = *psz;
			Unicode::win2Unicode(wszLine, szLine, 256);
			if (D2WinFontGetTextWidthN((const WORD *)wszLine, nIndex) + nCaretWidth > nAvailWidth)
			{
				fWrapped = TRUE;
				break;
			}
			psz++;
			nIndex++;
		}
		if (D2WinEditBoxHasSelection(ptEditData))
		{
			nSelStart = SELECTION_START(ptEditData);
			nSelEnd = SELECTION_END(ptEditData);
			nLineStart = pszLine - szText;
			nLineEnd = psz - szText;
			nSelectionX = nX;
			if (nSelStart < nLineStart && nSelEnd >= nLineEnd)
			{
				strncpy(szLine, pszLine, nIndex);
				szLine[psz - pszLine] = '\0';
				Unicode::win2Unicode(wszLine, szLine, 256);
				D2WinFontGetTextSize((const WORD *)wszLine, &nWidth, &nHeight);
				D2gfxScaleFillSpan(nX, nY - nFontHeight, nWidth + nX, nY, bSelectionColor, 5);
			}
			else if ((nSelStart >= nLineStart && nSelStart < nLineEnd)
				|| (nSelEnd >= nLineStart && nSelEnd < nLineEnd))
			{
				nStart = D2WinMaxInt(nSelStart, nLineStart);
				nEnd = D2WinMinInt(nSelEnd, nLineEnd);
				nBefore = nStart - nLineStart;
				if (nBefore)
				{
					strncpy(szLine, pszLine, nBefore);
					szLine[nBefore] = '\0';
					Unicode::win2Unicode(wszLine, szLine, 256);
					D2WinFontGetTextSize((const WORD *)wszLine, &nWidth, &nHeight);
					nSelectionX = nWidth + nX;
				}
				nCount = nEnd - nStart;
				strncpy(szLine, pszLine + nBefore, nCount);
				szLine[nCount] = '\0';
				Unicode::win2Unicode(wszLine, szLine, 256);
				D2WinFontGetTextSize((const WORD *)wszLine, &nWidth, &nHeight);
				D2gfxScaleFillSpan(nSelectionX, nY - nFontHeight, nWidth + nSelectionX, nY, bSelectionColor,
					5);
			}
		}
		strncpy(szLine, pszLine, nIndex);
		szLine[psz - pszLine] = '\0';
		Unicode::win2Unicode(wszLine, szLine, 256);
		D2WinFontDrawText((const WORD *)wszLine, nX, nY, ptEditData->tFont.dwColor, FALSE);
		if (fCaret && !fCaretDrawn)
		{
			nColumn = ptEditData->nCaretPos - (pszLine - szText);
			if (nColumn >= 0 && nColumn <= nIndex)
			{
				nCaretX = nX;
				if (nColumn)
					nCaretX = D2WinFontGetTextWidthN((const WORD *)wszLine, nColumn) + nX;
				D2WinFontDrawText((const WORD *)wszCaret, nCaretX, nY, ptEditData->tFont.dwColor, FALSE);
				fCaretDrawn = TRUE;
			}
		}
		if (fWrapped)
			nY += nFontHeight;
		if (*psz == '\r' && psz[1] == '\n')
		{
			while (*psz == '\r' && psz[1] == '\n')
			{
				psz += 2;
				nY += nFontHeight;
				if (fCaret && !fCaretDrawn && ptEditData->nCaretPos == psz - szText
					&& nY - nTop < nAvailHeight)
				{
					D2WinFontDrawText((const WORD *)wszCaret, nX, nY, ptEditData->tFont.dwColor, FALSE);
				}
			}
		}
		else if (*psz == '\r' && psz[1] != '\n')
		{
			do
			{
				psz++;
				nY += nFontHeight;
				if (fCaret && !fCaretDrawn && ptEditData->nCaretPos == psz - szText
					&& nY - nTop < nAvailHeight)
				{
					D2WinFontDrawText((const WORD *)wszCaret, nX, nY, ptEditData->tFont.dwColor, FALSE);
				}
			} while (*psz == '\r');
		}
		pszLine = psz;
	}
	return TRUE;
}

static void __fastcall D2WinEditBoxGetCaretPoint(D2WINEDITBOX *hChild, int *pnX, int *pnY)
{
	D2WINEDITBOX *ptEditData;
	char szText[256];
	char szLine[256];
	WORD *pwszText;
	char *psz;
	char *pszLine;
	BOOL fWrapped;
	int nX, nY;
	int nTop;
	int nCaretWidth;
	int nFontHeight;
	int nAvailWidth, nAvailHeight;
	int nIndex;
	int nColumn;
	int nCaretX;
	int nLow, nHigh;

#line 1393
	D2ASSERT(hChild);
	ptEditData = hChild;
#line 1395
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	Unicode wszCopy[300];
	D2WinFontSet(ptEditData->tFont.dwFont);
	nX = ptEditData->nX + ptEditData->nTextMarginX;
	nTop = (int)D2WinFontGetHeight() + (ptEditData->nTextMarginY - ptEditData->nHeight) + ptEditData->nY;
	pwszText = ptEditData->wszText;
	Unicode::strcpy(wszCopy, pwszText);
	char szCaret[] = "_";
	Unicode wszCaret[2];
	Unicode::win2Unicode(wszCaret, szCaret, 2);
	nCaretWidth = D2WinFontGetTextWidthN((const WORD *)wszCaret, 2);
	Unicode wszLine[256];
	Unicode::unicode2Win(szText, pwszText, 256);
	pszLine = szText;
	psz = szText;
	nY = nTop;
	nFontHeight = (int)D2WinFontGetHeight();
	nAvailWidth = ptEditData->nWidth - ptEditData->nTextMarginX * 2;
	nAvailHeight = ptEditData->nHeight - ptEditData->nTextMarginY * 2;
	*pnX = -1;
	*pnY = -1;
	while (*psz)
	{
		if (nY - nTop >= nAvailHeight)
			break;
		nIndex = psz - pszLine;
		fWrapped = FALSE;
		memset(szLine, 0, sizeof(szLine));
		while (*psz && *psz != '\r' && *psz != '\n')
		{
			szLine[nIndex] = *psz;
			Unicode::win2Unicode(wszLine, szLine, 256);
			if (D2WinFontGetTextWidthN((const WORD *)wszLine, nIndex) + nCaretWidth > nAvailWidth)
			{
				fWrapped = TRUE;
				break;
			}
			psz++;
			nIndex++;
		}
		nColumn = ptEditData->nCaretPos - (pszLine - szText);
		if (*pnX == -1 && nColumn >= 0 && nColumn <= nIndex)
		{
			nCaretX = nX;
			if (nColumn)
			{
				Unicode::win2Unicode(wszLine, szLine, 256);
				nCaretX = D2WinFontGetTextWidthN((const WORD *)wszLine, nColumn) + nX;
			}
			*pnX = nCaretX;
			*pnY = nY;
		}
		if (fWrapped)
			nY += nFontHeight;
		if (*psz == '\r' && psz[1] == '\n')
		{
			while (*psz == '\r' && psz[1] == '\n')
			{
				psz += 2;
				nY += nFontHeight;
				if (*pnX == -1 && ptEditData->nCaretPos == psz - szText && nY - nTop < nAvailHeight)
				{
					*pnX = nX;
					*pnY = nY;
				}
			}
		}
		else if (*psz == '\r' && psz[1] != '\n')
		{
			do
			{
				psz++;
				nY += nFontHeight;
				if (*pnX == -1 && ptEditData->nCaretPos == psz - szText && nY - nTop < nAvailHeight)
				{
					*pnX = nX;
					*pnY = nY;
				}
			} while (*psz == '\r');
		}
		pszLine = psz;
	}
	if (*pnX != -1)
	{
		*pnX = *pnX - ptEditData->nX - 1;
		*pnY += ptEditData->nHeight - ptEditData->nY - 1;
		nLow = ptEditData->nTextMarginX;
		nHigh = ptEditData->nWidth - ptEditData->nTextMarginX - 1;
		if (*pnX > nLow)
			nLow = *pnX;
		*pnX = nLow < nHigh ? nLow : nHigh;
		nLow = ptEditData->nTextMarginY;
		nHigh = ptEditData->nHeight - ptEditData->nTextMarginY - 1;
		if (*pnY > nLow)
			nLow = *pnY;
		*pnY = nLow < nHigh ? nLow : nHigh;
	}
}

static BOOL __stdcall D2WinEditBoxMouseDown(_PARAMS *ptMsg)
{
	D2WINEDITBOX *ptEditData;
	int nMouseX, nMouseY;
	int nRight;
	int nHeight;
	int nCount;
	int nCaret;
	int nNext;

	ptEditData = (D2WINEDITBOX *)ptMsg->window;
#line 1529
	D2ASSERT(ptEditData);
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (!(ptEditData->dwFlags & gdwBitMasks[2]) || !(ptEditData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	nMouseX = sgtWindowData.tMouse.x;
	nMouseY = sgtWindowData.tMouse.y;
	D2WinSetKeyFocus(ptEditData);
	nMouseX -= ptEditData->nX + ptEditData->nTextMarginX;
	nHeight = ptEditData->nHeight;
	nMouseY += nHeight - ptEditData->nTextMarginY - ptEditData->nY;
	nRight = ptEditData->nWidth - ptEditData->nTextMarginX;
	if (ptEditData->fMouseDown || (nMouseX >= -ptEditData->nTextMarginX && nMouseX < nRight
		&& nMouseY > -ptEditData->nTextMarginY && nMouseY < nHeight - ptEditData->nTextMarginY))
	{
		nCount = 1;
		D2WinFontSet(ptEditData->tFont.dwFont);
		if (ptEditData->fMouseDown)
		{
			if (nMouseX < -ptEditData->nTextMarginX)
			{
				ptEditData->nCaretPos = D2WinMaxInt(ptEditData->nCaretPos - 1, 0);
				D2WinEditBoxUpdateView(ptEditData);
			}
			else if (nMouseX >= nRight)
			{
				nCaret = Unicode::strlen(ptEditData->wszText) - 1;
				nNext = ptEditData->nCaretPos + 1;
				if (nNext < nCaret)
					nCaret = nNext;
				ptEditData->nCaretPos = nCaret;
				D2WinEditBoxUpdateView(ptEditData);
			}
		}
		while (nMouseX > D2WinFontGetTextWidthN(&ptEditData->wszText[ptEditData->nFirstShown], nCount))
		{
			if (nCount > Unicode::strlen(&ptEditData->wszText[ptEditData->nFirstShown]))
				break;
			nCount++;
		}
		nCaret = ptEditData->nCaretPos;
		ptEditData->nCaretPos = ptEditData->nFirstShown + nCount - 1;
		if (!(GetKeyState(VK_SHIFT) & 240) && !ptEditData->fMouseDown)
		{
			D2WinEditBoxDeselect((D2WINEDITBOX *)ptMsg->window);
		}
		else
		{
			if (ptEditData->nSelectionStart == -1)
				ptEditData->nSelectionStart = nCaret;
			ptEditData->nSelectionEnd = ptEditData->nCaretPos;
		}
	}
	if (ptMsg->dwMessage == WM_LBUTTONDOWN)
		ptEditData->fMouseDown = TRUE;
	if (ptEditData->pfnCallback)
		return ptEditData->pfnCallback(ptMsg);
	return FALSE;
}

static BOOL __stdcall D2WinEditBoxMouseUp(_PARAMS *ptMsg)
{
	D2WINEDITBOX *ptEditData;

	ptEditData = (D2WINEDITBOX *)ptMsg->window;
#line 1618
	D2ASSERT(ptEditData);
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (ptEditData->fMouseDown)
	{
		ptEditData->fMouseDown = FALSE;
		return TRUE;
	}
	return FALSE;
}

static BOOL __fastcall D2WinEditBoxSetCaretFromPoint(_PARAMS *ptMsg, int nX, int nY)
{
	D2WINEDITBOX *ptEditData;
	char szText[256];
	char szLine[256];
	char *psz;
	char *pszLine;
	BOOL fWrapped;
	BOOL fFound;
	int nLow, nHigh;
	int nCaretWidth;
	int nFontHeight;
	int nAvailWidth;
	int nLineY;
	int nIndex;
	int nCount;
	int nCaret;

	ptEditData = (D2WINEDITBOX *)ptMsg->window;
#line 1639
	D2ASSERT(ptEditData);
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	if (!(ptEditData->dwFlags & gdwBitMasks[2]) || !(ptEditData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	D2WinSetKeyFocus(ptEditData);
	if (ptEditData->fMouseDown)
	{
		nLow = ptEditData->nTextMarginX;
		nHigh = ptEditData->nWidth - ptEditData->nTextMarginX - 1;
		if (nX > nLow)
			nLow = nX;
		nX = nLow < nHigh ? nLow : nHigh;
		nLow = ptEditData->nTextMarginY;
		nHigh = ptEditData->nHeight - ptEditData->nTextMarginY - 1;
		if (nY > nLow)
			nLow = nY;
		nY = nLow < nHigh ? nLow : nHigh;
	}
	if (nX >= -ptEditData->nTextMarginX && nX < ptEditData->nWidth - ptEditData->nTextMarginX
		&& nY >= -ptEditData->nTextMarginY && nY < ptEditData->nHeight - ptEditData->nTextMarginY)
	{
		D2WinFontSet(ptEditData->tFont.dwFont);
		char szCaret[] = "_";
		Unicode wszCaret[2];
		Unicode::win2Unicode(wszCaret, szCaret, 2);
		nCaretWidth = D2WinFontGetTextWidth((const WORD *)wszCaret);
		Unicode::unicode2Win(szText, ptEditData->wszText, 256);
		pszLine = szText;
		psz = szText;
		nLineY = 0;
		nFontHeight = (int)D2WinFontGetHeight();
		fFound = FALSE;
		nCaret = -1;
		nAvailWidth = ptEditData->nWidth - ptEditData->nTextMarginX * 2;
		while (*psz)
		{
			Unicode wszLine[256];
			fWrapped = FALSE;
			nIndex = psz - pszLine;
			memset(szLine, 0, sizeof(szLine));
			while (*psz && *psz != '\r' && *psz != '\n')
			{
				szLine[nIndex] = *psz;
				Unicode::win2Unicode(wszLine, szLine, 256);
				if (D2WinFontGetTextWidthN((const WORD *)wszLine, nIndex) + nCaretWidth > nAvailWidth)
				{
					fWrapped = TRUE;
					break;
				}
				psz++;
				nIndex++;
			}
			if (nY >= nLineY && nY < nLineY + nFontHeight)
			{
				nCount = 1;
				Unicode::win2Unicode(wszLine, szLine, 256);
				while (nX > D2WinFontGetTextWidthN((const WORD *)wszLine, nCount))
				{
					if (nCount > nIndex)
						break;
					nCount++;
				}
				nCaret = ptEditData->nCaretPos;
				fFound = TRUE;
				ptEditData->nCaretPos = nCount + (pszLine - szText) - 1;
			}
			if (fWrapped)
			{
				nLineY += nFontHeight;
			}
			else if (*psz == '\r' && psz[1] != '\n')
			{
				psz++;
				nLineY += nFontHeight;
			}
			else if (*psz == '\r' && psz[1] == '\n')
			{
				psz += 2;
				nLineY += nFontHeight;
			}
			pszLine = psz;
		}
		if (!fFound)
		{
			nCaret = ptEditData->nCaretPos;
			ptEditData->nCaretPos = Unicode::strlen(ptEditData->wszText);
		}
		if (((GetKeyState(VK_SHIFT) & 240) || ptEditData->fMouseDown) && nCaret != -1)
		{
			if (ptEditData->nSelectionStart == -1)
				ptEditData->nSelectionStart = nCaret;
			ptEditData->nSelectionEnd = ptEditData->nCaretPos;
		}
		else
		{
			D2WinEditBoxDeselect(ptEditData);
		}
	}
	if (ptMsg->dwMessage == WM_LBUTTONDOWN)
		ptEditData->fMouseDown = TRUE;
	if (ptEditData->pfnCallback)
		return ptEditData->pfnCallback(ptMsg);
	return FALSE;
}

static BOOL __stdcall D2WinEditBoxMouseDownMultiLine(_PARAMS *ptMsg)
{
	D2WINEDITBOX *ptEditData;
	int nX, nY;

	ptEditData = (D2WINEDITBOX *)ptMsg->window;
#line 1789
	D2ASSERT(ptEditData);
	D2ASSERT(ptEditData->eType == D2WIN_EDITBOX);
	nX = sgtWindowData.tMouse.x - ptEditData->nTextMarginX - ptEditData->nX;
	nY = ptEditData->nHeight - ptEditData->nTextMarginY - ptEditData->nY + sgtWindowData.tMouse.y;
	return D2WinEditBoxSetCaretFromPoint(ptMsg, nX, nY);
}

BOOL __stdcall D2WinEditBoxNumericCallback(D2WINEDITBOX *ptEditData, int nPos, char *pchKey)
{
	if (*pchKey >= '0' && *pchKey <= '9')
		return TRUE;
	return FALSE;
}

BOOL __stdcall D2WinEditBoxAlphaCallback(D2WINEDITBOX *ptEditData, int nPos, char *pchKey)
{
	if ((*pchKey >= 'a' && *pchKey <= 'z') || (*pchKey >= 'A' && *pchKey <= 'Z'))
		return TRUE;
	return FALSE;
}

static void __fastcall D2WinEditBoxUpdateView(D2WINEDITBOX *ptEditData)
{
	int nAvailWidth;
	int nLength;
	int nTextWidth;
	int nWidth;
	int nCaretWidth;
	int nFirst;
	int i;

	D2WinFontSet(ptEditData->tFont.dwFont);
	nAvailWidth = ptEditData->nWidth - ptEditData->nTextMarginX * 2;
	nLength = Unicode::strlen(ptEditData->wszText);
	Unicode wszShown[300];
	if (ptEditData->dwEditBoxFlags & EDITBOX_PASSWORD)
	{
		char szStar[] = "*";
		Unicode wszChar[2];
		Unicode::win2Unicode((WORD *)wszChar, szStar, 2);
		for (i = 0; i < nLength; i++)
			wszShown[i] = wszChar[0].wChar;
	}
	else
	{
		Unicode::strcpy(wszShown, ptEditData->wszText);
	}
	nTextWidth = D2WinFontGetTextWidth((WORD *)wszShown);
	char szCaret[] = "_";
	Unicode wszCaret[2];
	Unicode::win2Unicode((WORD *)wszCaret, szCaret, 2);
	nCaretWidth = D2WinFontGetTextWidth((WORD *)wszCaret);
	nWidth = ptEditData->nCaretPos == nLength ? nTextWidth + nCaretWidth : nTextWidth;
	if (nWidth > nAvailWidth)
	{
		if (ptEditData->nCaretPos == nLength)
			nAvailWidth -= nCaretWidth;
		if (ptEditData->nCaretPos > ptEditData->nLastShown)
		{
			ptEditData->nLastShown = ptEditData->nCaretPos;
			ptEditData->nFirstShown = ptEditData->nCaretPos - 1;
			while (ptEditData->nFirstShown)
			{
				if (D2WinFontGetTextWidthN((WORD *)&wszShown[ptEditData->nFirstShown],
					ptEditData->nLastShown - ptEditData->nFirstShown + 1) >= nAvailWidth)
				{
					break;
				}
				ptEditData->nFirstShown--;
			}
			ptEditData->nFirstShown++;
		}
		else if (ptEditData->nCaretPos <= ptEditData->nFirstShown + 1)
		{
			nFirst = ptEditData->nCaretPos - 1;
			nFirst = nFirst < 0 ? 0 : nFirst;
			ptEditData->nFirstShown = nFirst;
			ptEditData->nLastShown = nFirst + 1;
			while (ptEditData->nLastShown < nLength)
			{
				if (D2WinFontGetTextWidthN((WORD *)&wszShown[ptEditData->nFirstShown],
					ptEditData->nLastShown - ptEditData->nFirstShown + 1) >= nAvailWidth)
				{
					break;
				}
				ptEditData->nLastShown++;
			}
			ptEditData->nLastShown--;
		}
		else if (D2WinFontGetTextWidth((WORD *)&wszShown[ptEditData->nFirstShown]) < nAvailWidth)
		{
			ptEditData->nLastShown = nLength - 1;
			ptEditData->nFirstShown = ptEditData->nLastShown - 1;
			while (ptEditData->nFirstShown)
			{
				if (D2WinFontGetTextWidth((WORD *)&wszShown[ptEditData->nFirstShown]) >= nAvailWidth)
					break;
				ptEditData->nFirstShown--;
			}
			ptEditData->nFirstShown++;
		}
		else
		{
			ptEditData->nLastShown = ptEditData->nFirstShown + 1;
			while (ptEditData->nLastShown < nLength)
			{
				if (D2WinFontGetTextWidthN((WORD *)&wszShown[ptEditData->nFirstShown],
					ptEditData->nLastShown - ptEditData->nFirstShown + 1) >= nAvailWidth)
				{
					break;
				}
				ptEditData->nLastShown++;
			}
			ptEditData->nLastShown--;
		}
	}
	else
	{
		ptEditData->nFirstShown = 0;
		ptEditData->nLastShown = nLength - 1;
	}
}
