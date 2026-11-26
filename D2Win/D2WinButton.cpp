#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinButton.cpp"
#include "D2Win.h"
#include "D2Macros.h"

BOOL __fastcall D2WinButtonDraw(D2WINBUTTON *ptBtnData);
static BOOL __stdcall D2WinButtonMouseDown(_PARAMS *ptMsg);
static BOOL __stdcall D2WinButtonMouseUp(_PARAMS *ptMsg);
static BOOL __stdcall D2WinButtonKey(_PARAMS *ptMsg);

D2WINBUTTON * __fastcall D2WinButtonCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	PFNWINMSG pfnCallback, DWORD dwKey, int nFirstCel, DWORD dwButtonFlags, DWORD dwStringId,
	PFNWINMSG pfnHover)
{
	D2WINBUTTON *ptBtnData;

#line 36
	ptBtnData = (D2WINBUTTON *)D2ALLOC2(sizeof(D2WINBUTTON));
	memset(ptBtnData, 0, sizeof(D2WINBUTTON));
	ptBtnData->eType = D2WIN_BUTTON;
	ptBtnData->dwFlags |= gdwBitMasks[0];
	ptBtnData->dwFlags |= gdwBitMasks[2];
	ptBtnData->nX = nX;
	ptBtnData->nY = nY;
	ptBtnData->hCelFile = hCelFile;
	ptBtnData->nWidth = nWidth;
	ptBtnData->nHeight = nHeight;
	ptBtnData->pfnDraw = (PFNWINCONTROL)D2WinButtonDraw;
	ptBtnData->pfnMouseDown = D2WinButtonMouseDown;
	ptBtnData->pfnMouseUp = D2WinButtonMouseUp;
	ptBtnData->pfnChar = 0;
	if (dwKey)
		ptBtnData->pfnKey = D2WinButtonKey;
	else
		ptBtnData->pfnKey = NULL;
	ptBtnData->pfnCallback = pfnCallback;
	ptBtnData->dwKey = dwKey;
	ptBtnData->ptNextChild = 0;
	ptBtnData->fPressed = FALSE;
	ptBtnData->fMouseInside = FALSE;
	ptBtnData->dwButtonFlags = dwButtonFlags;
	ptBtnData->nFirstCel = nFirstCel;
	if (dwStringId)
	{
		(void)D2LangUnicodestrcpy(ptBtnData->wszText, D2LangStrGetStringByIndex(dwStringId));
	}
	ptBtnData->nCelsPerState = ((nWidth + 255) >> 8) * ((nHeight + 255) >> 8);
	ptBtnData->pfnHover = pfnHover;
	ptBtnData->fLastHover = FALSE;
	D2WinMainAddChild(ptBtnData);
	return ptBtnData;
}

DWORD __fastcall D2WinButtonDestroy(D2WINBUTTON *ptBtnData)
{
#line 85
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	return D2WinMainFreeChild(ptBtnData);
}

BOOL __fastcall D2WinButtonIsPressed(D2WINBUTTON *ptBtnData)
{
#line 97
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	if (ptBtnData->fPressed && ptBtnData->fMouseInside)
		return TRUE;
	return FALSE;
}

void __fastcall D2WinButtonSetPressed(D2WINBUTTON *ptBtnData, BOOL fPressed)
{
#line 110
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	ptBtnData->fPressed = fPressed;
	ptBtnData->fMouseInside = fPressed;
}

void __fastcall D2WinButtonSetText(D2WINBUTTON *ptBtnData, DWORD dwFont, DWORD dwStringId, DWORD dwColor)
{
#line 128
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	ptBtnData->dwFont = dwFont;
	ptBtnData->dwColor = dwColor;
	(void)D2LangUnicodestrcpy(ptBtnData->wszText, D2LangStrGetStringByIndex(dwStringId));
}

void __fastcall D2WinButtonSetRect(D2WINBUTTON *ptBtnData, int nX, int nY, int nWidth, int nHeight)
{
#line 149
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	ptBtnData->nX = nX;
	ptBtnData->nY = nY;
	ptBtnData->nWidth = nWidth;
	ptBtnData->nHeight = nHeight;
}

void __fastcall D2WinButtonClick(_PARAMS *ptMsg)
{
	D2WINBUTTON *ptBtnData;
	_PARAMS tMsg;

	ptBtnData = (D2WINBUTTON *)ptMsg->window;
#line 166
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	if (ptBtnData->pfnMouseDown)
	{
		ptBtnData->nClickFrames = 1;
		ptBtnData->pfnMouseDown(ptMsg);
		if (ptBtnData->pfnMouseUp)
		{
			tMsg.window = ptMsg->window;
			sgtWindowData.deadChildrenLocked++;
			ptBtnData->pfnMouseUp(&tMsg);
			sgtWindowData.deadChildrenLocked--;
		}
	}
}

void __fastcall D2WinButtonSetFirstCel(D2WINBUTTON *ptBtnData, int nFirstCel)
{
#line 190
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	ptBtnData->nFirstCel = nFirstCel;
}

static inline BOOL D2WinButtonHitTest(D2WINBUTTON *ptBtnData, int nX, int nY)
{
	int nLeft = ptBtnData->nX;
	int nBottom = ptBtnData->nY;

	if (nX >= nLeft && nY >= nBottom - ptBtnData->nHeight && nX < ptBtnData->nWidth + nLeft && nY < nBottom)
		return TRUE;
	return FALSE;
}

static inline void D2WinButtonDrawText(D2WINBUTTON *ptBtnData, int nTextY)
{
	int nFontHeight;
	int nHalf;
	int nTextX;

	D2WinFontSet(ptBtnData->dwFont);
	nFontHeight = (int)D2WinFontGetHeight();
	nHalf = (ptBtnData->nWidth - D2WinFontGetTextWidth(ptBtnData->wszText)) / 2;
	if (nHalf < 0)
		nHalf = 0;
	nTextX = ptBtnData->nX + nHalf;
	nTextY -= (ptBtnData->nHeight - nFontHeight) / 2;
	nTextY += ptBtnData->nY;
	if ((ptBtnData->fPressed && ptBtnData->fMouseInside) || ptBtnData->nClickFrames)
	{
		nTextX -= 2;
		nTextY += 2;
	}
	D2WinFontDrawTextWithMode(ptBtnData->wszText, nTextX, nTextY, ptBtnData->dwColor, 0, 4);
}

BOOL __fastcall D2WinButtonDraw(D2WINBUTTON *ptBtnData)
{
	D2CELCONTEXT tCelContext;
	_PARAMS tMsg;
	DWORD dwDrawMode;
	int nCel;
	int nRows, nCols;
	int nRow, nCol;
	int nY;
	int nLeft;
	int nTextOffset;

#line 221
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	nCel = ptBtnData->nFirstCel;
	if ((ptBtnData->dwFlags & gdwBitMasks[2]) && (ptBtnData->dwFlags & gdwBitMasks[0]))
	{
		if (ptBtnData->fPressed)
		{
			if (ptBtnData->dwButtonFlags & 3)
			{
				ptBtnData->fMouseInside = TRUE;
			}
			else
			{
				ptBtnData->fMouseInside = D2WinButtonHitTest(ptBtnData, sgtWindowData.tMouse.x, sgtWindowData.tMouse.y);
			}
		}
		if ((ptBtnData->fPressed && ptBtnData->fMouseInside) || ptBtnData->nClickFrames)
			nCel += ptBtnData->nCelsPerState;
	}
	if (ptBtnData->hCelFile && (ptBtnData->dwFlags & gdwBitMasks[2]))
	{
		dwDrawMode = (ptBtnData->dwFlags & gdwBitMasks[0]) ? 5 : 1;
		nY = ptBtnData->nY - ptBtnData->nHeight + 1;
		nLeft = ptBtnData->nX;
		nCols = (ptBtnData->nWidth + 255) >> 8;
		nRows = ((ptBtnData->nHeight + 255) >> 8) - 1;
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = ptBtnData->hCelFile;
		tCelContext.dwDirection = 0;
		for (nRow = 0; nRow < nRows; nRow++)
		{
			for (nCol = 0; nCol < nCols; nCol++)
			{
				tCelContext.dwFrame = nCel + nRow * 4 + nCol;
				D2gfxScaleDrawCel(&tCelContext, nLeft + nCol * 256, nY + (nRow + 1) * 256, -1, dwDrawMode, 0);
			}
		}
		if (nRows >= 0)
		{
			for (nCol = 0; nCol < nCols; nCol++)
			{
				tCelContext.dwFrame = nCel + nRows * 4 + nCol;
				D2gfxScaleDrawCel(&tCelContext, nLeft + nCol * 256, ptBtnData->nY, -1, dwDrawMode, 0);
			}
		}
		if (ptBtnData->nHeight >= 35)
		{
			ptBtnData->dwFont = 9;
			nTextOffset = 4;
		}
		else if (ptBtnData->nHeight == 25)
		{
			ptBtnData->dwFont = 10;
			nTextOffset = 2;
		}
		else if (ptBtnData->nHeight > 20)
		{
			ptBtnData->dwFont = 10;
			nTextOffset = 0;
		}
		else
		{
			ptBtnData->dwFont = 10;
			nTextOffset = 1;
		}
		ptBtnData->dwColor = 0;
		if (ptBtnData->wszText[0])
			D2WinButtonDrawText(ptBtnData, nTextOffset);
	}
	if (ptBtnData->nClickFrames == 2)
	{
		ptBtnData->nClickFrames = 0;
		ptBtnData->fMouseInside = TRUE;
		tMsg.window = ptBtnData;
		sgtWindowData.deadChildrenLocked++;
		ptBtnData->pfnMouseUp(&tMsg);
		sgtWindowData.deadChildrenLocked--;
	}
	else if (ptBtnData->nClickFrames)
	{
		ptBtnData->nClickFrames++;
	}
	if (ptBtnData->pfnHover)
	{
		if (D2WinMainGetInputChild())
		{
			ptBtnData->fLastHover = FALSE;
		}
		else
		{
			ptBtnData->fLastHover = D2WinButtonHitTest(ptBtnData, sgtWindowData.tMouse.x, sgtWindowData.tMouse.y);
		}
		tMsg.window = ptBtnData;
		tMsg.dwLParam = ptBtnData->fLastHover;
		sgtWindowData.deadChildrenLocked++;
		ptBtnData->pfnHover(&tMsg);
		sgtWindowData.deadChildrenLocked--;
	}
	return TRUE;
}

static BOOL __stdcall D2WinButtonMouseDown(_PARAMS *ptMsg)
{
	D2WINBUTTON *ptBtnData;
	DWORD dwButtonFlags;
	BOOL fPress = TRUE;

	ptBtnData = (D2WINBUTTON *)ptMsg->window;
#line 385
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	if (!(ptBtnData->dwFlags & gdwBitMasks[2]) || !(ptBtnData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	dwButtonFlags = ptBtnData->dwButtonFlags;
	if (dwButtonFlags & 2)
	{
		ptBtnData->fPressed = !ptBtnData->fPressed;
		ptBtnData->fMouseInside = !ptBtnData->fMouseInside;
	}
	else if (ptBtnData->fPressed && ptBtnData->fMouseInside)
		fPress = FALSE;
	else
	{
		ptBtnData->fPressed = TRUE;
		ptBtnData->fMouseInside = TRUE;
	}
	if (fPress)
	{
		if (!(dwButtonFlags & 4))
			D2SoundFastPlay("data\\global\\sfx\\cursor\\button.wav");
		if ((ptBtnData->dwButtonFlags & 3) && ptBtnData->pfnCallback)
		{
#line 421
			D2ASSERT(sgtWindowData.deadChildrenLocked > 0);
			ptBtnData->pfnCallback(ptMsg);
		}
	}
	return TRUE;
}

static BOOL __stdcall D2WinButtonMouseUp(_PARAMS *ptMsg)
{
	D2WINBUTTON *ptBtnData;
	BOOL fResult;

	ptBtnData = (D2WINBUTTON *)ptMsg->window;
#line 438
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	fResult = FALSE;
	ptBtnData->dwFlags |= gdwBitMasks[1];
	if (!(ptBtnData->dwFlags & gdwBitMasks[2]) || !(ptBtnData->dwFlags & gdwBitMasks[0]))
	{
		ptBtnData->dwFlags &= gdwInvBitMasks[1];
		return FALSE;
	}
	if (!(ptBtnData->dwButtonFlags & 3))
	{
		ptBtnData->fPressed = FALSE;
		if (ptBtnData->fMouseInside && ptBtnData->pfnCallback)
		{
#line 455
			D2ASSERT(sgtWindowData.deadChildrenLocked > 0);
			fResult = ptBtnData->pfnCallback(ptMsg);
		}
		ptBtnData->fMouseInside = FALSE;
	}
	ptBtnData->dwFlags &= gdwInvBitMasks[1];
	return fResult;
}

static BOOL __stdcall D2WinButtonKey(_PARAMS *ptMsg)
{
	D2WINBUTTON *ptBtnData;
	BOOL fResult;

	ptBtnData = (D2WINBUTTON *)ptMsg->window;
#line 477
	D2ASSERT(ptBtnData->eType == D2WIN_BUTTON);
	fResult = FALSE;
	if (ptMsg->dwMessage != WM_CHAR) {
		if (ptBtnData->dwKey == ptMsg->dwWParam && ptBtnData->pfnMouseDown)
		{
			ptBtnData->nClickFrames = 1;
#line 487
			D2ASSERT(sgtWindowData.deadChildrenLocked > 0);
			fResult = ptBtnData->pfnMouseDown(ptMsg);
		}
	}
	return fResult;
}
