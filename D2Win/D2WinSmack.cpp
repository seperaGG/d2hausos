#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinSmack.cpp"
#include "D2Win.h"
#include "D2Macros.h"

static char sgszSmackFile[260] = { 0 };
DWORD sgdwSmackField374 = 0;
static BOOL sgfSmackReopen = 0;

BOOL __fastcall D2WinSmackDraw(D2WINSMACK *ptData);
static BOOL __stdcall D2WinSmackMouseDown(_PARAMS *ptMsg);

D2WINSMACK * __fastcall D2WinSmackCreate(int nX, int nY, int nWidth, int nHeight, PFNWINMSG pfnCallback)
{
	D2WINSMACK *ptData;

#line 33
	ptData = (D2WINSMACK *)D2ALLOC2(sizeof(D2WINSMACK));
	memset(ptData, 0, sizeof(D2WINSMACK));
	ptData->eType = D2WIN_SMACK;
	ptData->dwFlags |= gdwBitMasks[0];
	ptData->dwFlags |= gdwBitMasks[2];
	ptData->nX = nX;
	ptData->nY = nY;
	ptData->nWidth = nWidth;
	ptData->nHeight = nHeight;
	ptData->ptNextChild = NULL;
	ptData->pfnMouseUp = NULL;
	ptData->pfnChar = NULL;
	ptData->pfnKey = NULL;
	ptData->pfnCallback = pfnCallback;
	ptData->pfnDraw = (PFNWINCONTROL)D2WinSmackDraw;
	ptData->pfnMouseDown = D2WinSmackMouseDown;
	ptData->szFile[0] = '\0';
	memset(&ptData->tView, 0, sizeof(ptData->tView));
	ptData->tView.nX = (nWidth - 468) / 2 + nX;
	ptData->tView.nY = (nHeight - 60) / 2 + nY;
	D2WinMainAddChild(ptData);
	return ptData;
}

BOOL __stdcall D2WinSmackOpen(D2WINSMACK *ptData, const char *pszFile)
{
#line 80
	D2ASSERT(ptData->eType == D2WIN_SMACK);
	strncpy(ptData->szFile, pszFile, sizeof(ptData->szFile));
	D2gfxWindowCloseSmack(&ptData->tView);
	D2gfxWindowOpenSmack(ptData->szFile, &ptData->tView);
	strncpy(sgszSmackFile, pszFile, sizeof(sgszSmackFile));
	return TRUE;
}

BOOL __stdcall D2WinSmackClose(D2WINSMACK *ptData)
{
#line 98
	D2ASSERT(ptData->eType == D2WIN_SMACK);
	D2gfxWindowCloseSmack(&ptData->tView);
	if (!sgfSmackReopen)
		DeleteFileA(ptData->szFile);
	return TRUE;
}

BOOL __stdcall D2WinSmackSetPos(D2WINSMACK *ptData, int nX, int nY)
{
#line 119
	D2ASSERT(ptData->eType == D2WIN_SMACK);
	ptData->nX = nX;
	ptData->nY = nY;
	ptData->tView.nX = nX;
	ptData->tView.nY = nY;
	return TRUE;
}

DWORD __fastcall D2WinSmackDestroy(D2WINSMACK *ptData)
{
#line 136
	D2ASSERT(ptData->eType == D2WIN_SMACK);
	D2gfxWindowCloseSmack(&ptData->tView);
	if (!sgfSmackReopen)
		DeleteFileA(ptData->szFile);
	return D2WinMainFreeChild(ptData);
}

void __fastcall D2WinSmackRequestReopen(void)
{
	sgfSmackReopen = TRUE;
}

BOOL __fastcall D2WinSmackDraw(D2WINSMACK *ptData)
{
	const char *pszFile;

#line 160
	D2ASSERT(ptData->eType == D2WIN_SMACK);
	if (!(gdwBitMasks[2] & ptData->dwFlags) || !(gdwBitMasks[0] & ptData->dwFlags))
		return FALSE;
	if (sgfSmackReopen)
	{
		if (sgszSmackFile[0])
		{
			pszFile = sgszSmackFile;
			D2_OPAQUE(pszFile);
			strncpy(ptData->szFile, pszFile, sizeof(ptData->szFile));
			D2gfxWindowCloseSmack(&ptData->tView);
			D2gfxWindowOpenSmack(ptData->szFile, &ptData->tView);
			strncpy(sgszSmackFile, pszFile, sizeof(sgszSmackFile));
		}
		sgfSmackReopen = FALSE;
	}
	if (ptData->tView.ptSmack)
	{
		D2gfxWindowDrawSmackFrame(&ptData->tView);
		sgdwSmackField374 = ptData->tView.ptSmack->dwFrameNum;
	}
	return TRUE;
}

static BOOL __stdcall D2WinSmackMouseDown(_PARAMS *ptMsg)
{
	D2WINSMACK *ptData;

	ptData = (D2WINSMACK *)ptMsg->window;
#line 197
	D2ASSERT(ptData->eType == D2WIN_SMACK);
	if (!(gdwBitMasks[2] & ptData->dwFlags) || !(gdwBitMasks[0] & ptData->dwFlags))
		return FALSE;
	if (!ptData->pfnCallback)
		return FALSE;
	return ptData->pfnCallback(ptMsg);
}
