#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinScrollbar.cpp"
#include "D2Win.h"
#include "D2Macros.h"

BOOL __fastcall D2WinScrollbarDraw(D2WINSCROLLBAR *ptScrollbarData);
static BOOL __stdcall D2WinScrollbarMouseDown(_PARAMS *ptMsg);
static BOOL __stdcall D2WinScrollbarMouseUp(_PARAMS *ptMsg);

D2WINSCROLLBAR * __fastcall D2WinScrollbarCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	PFNWINMSG pfnCallback)
{
	D2WINSCROLLBAR *ptScrollbarData;

#line 28
	ptScrollbarData = (D2WINSCROLLBAR *)D2ALLOC2(sizeof(D2WINSCROLLBAR));
	memset(ptScrollbarData, 0, sizeof(D2WINSCROLLBAR));
	ptScrollbarData->eType = D2WIN_SCROLLBAR;
	ptScrollbarData->dwFlags |= gdwBitMasks[0];
	ptScrollbarData->dwFlags |= gdwBitMasks[2];
	ptScrollbarData->hCelFile = hCelFile;
	ptScrollbarData->nX = nX;
	ptScrollbarData->nY = nY;
	ptScrollbarData->nWidth = nWidth;
	ptScrollbarData->nHeight = nHeight;
	ptScrollbarData->pfnDraw = (PFNWINCONTROL)D2WinScrollbarDraw;
	ptScrollbarData->ptNextChild = NULL;
	ptScrollbarData->pfnMouseDown = D2WinScrollbarMouseDown;
	ptScrollbarData->pfnMouseUp = D2WinScrollbarMouseUp;
	ptScrollbarData->pfnChar = NULL;
	ptScrollbarData->pfnKey = NULL;
	ptScrollbarData->pfnCallback = pfnCallback;
	ptScrollbarData->fUpHeld = FALSE;
	ptScrollbarData->fDownHeld = FALSE;
	ptScrollbarData->nMaxValue = 0;
	ptScrollbarData->nValue = 0;
	ptScrollbarData->ptTextBox = NULL;
	D2WinMainAddChild(ptScrollbarData);
	return ptScrollbarData;
}

DWORD __fastcall D2WinScrollbarDestroy(D2WINSCROLLBAR *ptScrollbarData)
{
	return D2WinMainFreeChild(ptScrollbarData);
}

extern "C" int __fastcall D2WinScrollbarGetMaxValue(D2WINSCROLLBAR *ptScrollbarData)
{
#line 73
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	return ptScrollbarData->nMaxValue;
}

BOOL __fastcall D2WinScrollbarSetMaxValue(D2WINSCROLLBAR *ptScrollbarData, int nMaxValue)
{
#line 84
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	if (nMaxValue > 0)
	{
		ptScrollbarData->nMaxValue = nMaxValue;
		return TRUE;
	}
	ptScrollbarData->nMaxValue = 0;
	return TRUE;
}

BOOL __fastcall D2WinScrollbarSetValue(D2WINSCROLLBAR *ptScrollbarData, int nValue)
{
#line 99
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	if (nValue > ptScrollbarData->nMaxValue)
		nValue = ptScrollbarData->nMaxValue;
	ptScrollbarData->nValue = nValue;
	return TRUE;
}

int __fastcall D2WinScrollbarGetValue(D2WINSCROLLBAR *ptScrollbarData)
{
#line 114
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	return ptScrollbarData->nValue;
}

BOOL __fastcall D2WinScrollbarSetTextBox(D2WINSCROLLBAR *ptScrollbarData, D2WINCONTROL *ptTextBox)
{
#line 125
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	ptScrollbarData->ptTextBox = ptTextBox;
	return TRUE;
}

BOOL __fastcall D2WinScrollbarDraw(D2WINSCROLLBAR *ptScrollbarData)
{
	D2CELCONTEXT tCelContext;
	_PARAMS tMsg;
	HCELFILE hCelFile;
	int nX, nY;
	int n;

#line 149
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	hCelFile = ptScrollbarData->hCelFile;
	nY = ptScrollbarData->nY - 1;
	nX = ptScrollbarData->nX;
	if (!ptScrollbarData->nMaxValue)
		return TRUE;
	if (!D2WinIsMouseDown())
		ptScrollbarData->fDragging = FALSE;
	if (ptScrollbarData->fDragging)
	{
		tMsg.window = ptScrollbarData;
		tMsg.dwMessage = WM_LBUTTONDOWN;
		D2WinScrollbarMouseDown(&tMsg);
	}
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.dwDirection = 0;
	tCelContext.hCelFile = hCelFile;
	tCelContext.dwFrame = (ptScrollbarData->fDownHeld ? 2 : 0) + 1;
	D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, 5, 0);
	tCelContext.dwFrame = 5;
	if (10 < ptScrollbarData->nHeight - 10)
	{
		for (n = 10; n < ptScrollbarData->nHeight - 10;)
		{
			nY -= 10; n += 10;
			D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, 5, 0);
		}
	}
	tCelContext.dwFrame = ptScrollbarData->fUpHeld ? 2 : 0;
	D2gfxScaleDrawCel(&tCelContext, nX, ptScrollbarData->nY - ptScrollbarData->nHeight + 9, -1,
		5, 0);
	tCelContext.dwFrame = 4;
	D2gfxScaleDrawCel(&tCelContext, nX, (ptScrollbarData->nHeight - 30) * ptScrollbarData->nValue
		/ ptScrollbarData->nMaxValue - ptScrollbarData->nHeight + ptScrollbarData->nY + 19, -1, 5, 0);
	return TRUE;
}

static BOOL __stdcall D2WinScrollbarMouseDown(_PARAMS *ptMsg)
{
	D2WINSCROLLBAR *ptScrollbarData;
	int nX, nY, nLimit;

	ptScrollbarData = (D2WINSCROLLBAR *)ptMsg->window;
#line 216
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	nX = sgtWindowData.tMouse.x - ptScrollbarData->nX;
	nY = ptScrollbarData->nHeight - ptScrollbarData->nY + sgtWindowData.tMouse.y;
	if (nX < 0 || nX > ptScrollbarData->nWidth)
		return FALSE;
	nLimit = ptScrollbarData->nHeight - 10;
	if (ptMsg->dwMessage == WM_LBUTTONDOWN)
		ptScrollbarData->fDragging = TRUE;
	if (nY < 11 && !ptScrollbarData->fDownHeld)
	{
		ptScrollbarData->fUpHeld = TRUE;
		ptScrollbarData->nValue = ptScrollbarData->nValue - 1;
	}
	else if (nY > nLimit && !ptScrollbarData->fUpHeld)
	{
		ptScrollbarData->fDownHeld = TRUE;
		ptScrollbarData->nValue = ptScrollbarData->nValue + 1;
	}
	else if (!ptScrollbarData->fDownHeld && !ptScrollbarData->fUpHeld)
	{
		ptScrollbarData->nValue = (ptScrollbarData->nMaxValue + 1) * (nY - 11)
			/ (ptScrollbarData->nHeight - 20);
	}
	if (ptScrollbarData->nValue < 0)
		ptScrollbarData->nValue = 0;
	else if (ptScrollbarData->nValue > ptScrollbarData->nMaxValue)
		ptScrollbarData->nValue = ptScrollbarData->nMaxValue;
	if (ptScrollbarData->ptTextBox && ptMsg->dwMessage == WM_LBUTTONDOWN)
		D2WinTextBoxSetTop((D2WINTEXTBOX *)ptScrollbarData->ptTextBox, ptScrollbarData->nValue);
	if (ptScrollbarData->pfnCallback)
		return ptScrollbarData->pfnCallback(ptMsg);
	return FALSE;
}

static BOOL __stdcall D2WinScrollbarMouseUp(_PARAMS *ptMsg)
{
	D2WINSCROLLBAR *ptScrollbarData;

	ptScrollbarData = (D2WINSCROLLBAR *)ptMsg->window;
#line 293
	D2ASSERT(ptScrollbarData->eType == D2WIN_SCROLLBAR);
	if (!ptScrollbarData->fUpHeld && !ptScrollbarData->fDownHeld && !ptScrollbarData->fDragging)
		return FALSE;
	ptScrollbarData->fUpHeld = FALSE;
	ptScrollbarData->fDownHeld = FALSE;
	ptScrollbarData->fDragging = FALSE;
	return TRUE;
}
