#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinAccountList.cpp"
#include "D2Win.h"
#include "D2Macros.h"

void __fastcall D2WinAccountListClear(D2WINACCOUNTLIST *ptListData);
static BOOL __fastcall D2WinAccountListDraw(D2WINACCOUNTLIST *ptListData);
static BOOL __stdcall D2WinAccountListMouseDown(_PARAMS *ptMsg);

D2WINACCOUNTLIST * __fastcall D2WinAccountListCreate(int nX, int nY, int nWidth, int nHeight,
	DWORD dwControlFlags,
	D2WINFONTDESC *ptFont)
{
	D2WINACCOUNTLIST *ptListData;
#line 36
	ptListData = (D2WINACCOUNTLIST *)D2ALLOC2(sizeof(D2WINACCOUNTLIST));
	memset(ptListData, 0, sizeof(D2WINACCOUNTLIST));
	ptListData->eType = D2WIN_ACCOUNTLIST;
	ptListData->dwFlags |= gdwBitMasks[0];
	ptListData->dwFlags |= gdwBitMasks[2];
	ptListData->field_4c = dwControlFlags;
	ptListData->nX = nX;
	ptListData->nY = nY;
	ptListData->nWidth = nWidth;
	ptListData->nHeight = nHeight;
	ptListData->pfnDraw = (PFNWINCONTROL)D2WinAccountListDraw;
	ptListData->pfnMouseDown = D2WinAccountListMouseDown;
	ptListData->pfnMouseUp = NULL;
	ptListData->pfnChar = 0;
	ptListData->pfnKey = NULL;
	ptListData->pfnCallback = NULL;
	ptListData->ptFirst = NULL;
	ptListData->ptSelected = NULL;
	ptListData->nTopIndex = 0;
	ptListData->ptNextChild = 0;
	if (ptFont)
	{
		ptListData->tFont.dwFont = ptFont->dwFont;
		ptListData->tFont.dwColor = ptFont->dwColor;
	}
	else
	{
		ptListData->tFont.dwFont = 1;
		ptListData->tFont.dwColor = 0;
	}
	D2WinMainAddChild(ptListData);
	return ptListData;
}

DWORD __fastcall D2WinAccountListDestroy(D2WINACCOUNTLIST *ptListData)
{
#line 80
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
	D2WinAccountListClear(ptListData);
	return D2WinMainFreeChild(ptListData);
}

void __fastcall D2WinAccountListClear(D2WINACCOUNTLIST *ptListData)
{
	D2WINACCOUNT *ptAccount;
	D2WINACCOUNT *ptNext;

#line 93
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
	for (ptAccount = ptListData->ptFirst; ptAccount; ptAccount = ptNext)
	{
		ptNext = ptAccount->ptNext;
#line 99
		D2FREE2(ptAccount);
	}
}

void __fastcall D2WinAccountListAdd(D2WINACCOUNTLIST *ptListData, const char *pszName)
{
	D2WINACCOUNT *ptAccount;

#line 114
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
#line 119
	ptAccount = (D2WINACCOUNT *)D2ALLOC2(sizeof(D2WINACCOUNT));
	strncpy(ptAccount->szName, pszName, sizeof(ptAccount->szName));
	if (!ptListData->ptFirst)
		ptAccount->ptNext = NULL;
	else
		ptAccount->ptNext = ptListData->ptFirst;
	ptListData->ptFirst = ptAccount;
}

BOOL __fastcall D2WinAccountListRemove(D2WINACCOUNTLIST *ptListData, const char *pszName)
{
	D2WINACCOUNT *ptAccount;
	D2WINACCOUNT *ptPrev;

#line 144
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
	ptPrev = NULL;
	for (ptAccount = ptListData->ptFirst; ptAccount; ptAccount = ptAccount->ptNext)
	{
		if (strcmp(ptAccount->szName, pszName) == 0)
		{
			if (ptListData->ptSelected == ptAccount)
				ptListData->ptSelected = NULL;
			if (ptPrev)
				ptPrev->ptNext = ptAccount->ptNext; else
				ptListData->ptFirst = ptAccount->ptNext;
#line 167
			D2FREE2(ptAccount);
			return TRUE; }
		ptPrev = ptAccount;
	}
	return FALSE;
}

BOOL __fastcall D2WinAccountListGetSelected(D2WINACCOUNTLIST *ptListData, char *pszName)
{
#line 185
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
	if (ptListData->ptSelected) {
		strcpy(pszName, ptListData->ptSelected->szName);
		return TRUE; }
	return FALSE;
}

static BOOL __fastcall D2WinAccountListDraw(D2WINACCOUNTLIST *ptListData)
{
	D2WINACCOUNT *ptAccount;
	int nLeft, nTop;
	int nX, nY;
	int i;

#line 208
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
	if (!(ptListData->dwFlags & gdwBitMasks[2]) || !(ptListData->dwFlags & gdwBitMasks[0])
		|| !ptListData->ptFirst)
		return FALSE;
	D2WinFontSet(ptListData->tFont.dwFont);
	Unicode awszName[256];
	i = 0;
	nLeft = ptListData->nX;
	ptAccount = ptListData->ptFirst;
	nTop = (int)D2WinFontGetHeight() + (ptListData->nY - ptListData->nHeight);
	for (; ptAccount; ptAccount = ptAccount->ptNext, i++)
	{
		if (i < ptListData->nTopIndex || i >= ptListData->nTopIndex + 24)
			continue;
		nX = nLeft + i % 6 * 120;
		nY = nTop + i / 6 * 20;
		D2gfxScaleFillSpan(nX, nY - 14, nX + 28, nY, 85, 5);
		(void)D2LangUnicodewin2Unicode((WORD *)awszName, ptAccount->szName, 256);
		D2WinFontDrawText((const WORD *)awszName, nX + 32, nY, 0, 0);
		if (ptAccount == ptListData->ptSelected)
			D2gfxScaleFillSpan(nX - 1, nY - 16, nX + 118, nY + 1, 255, 1);
	}
	return TRUE;
}

static BOOL __stdcall D2WinAccountListMouseDown(_PARAMS *ptMsg)
{
	D2WINACCOUNTLIST *ptListData;
	D2WINACCOUNT *ptAccount;
	int nIndex;
	int i;

	ptListData = (D2WINACCOUNTLIST *)ptMsg->window;
#line 269
	D2ASSERT(ptListData->eType == D2WIN_ACCOUNTLIST);
	if (!(ptListData->dwFlags & gdwBitMasks[2]) || !(ptListData->dwFlags & gdwBitMasks[0])
		|| !ptListData->ptFirst)
		return FALSE;
	nIndex = (ptListData->nHeight - ptListData->nY + sgtWindowData.tMouse.y + 4) / 20 * 6
		+ (sgtWindowData.tMouse.x - ptListData->nX) / 120;
	i = 0;
	for (ptAccount = ptListData->ptFirst; ptAccount; ptAccount = ptAccount->ptNext)
	{
		if (i == nIndex)
		{
			ptListData->ptSelected = ptAccount;
			return TRUE;
		}
		i++;
	}
	ptListData->ptSelected = NULL;
	return TRUE;
}
