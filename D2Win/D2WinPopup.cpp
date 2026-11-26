#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinPopup.cpp"
#include "D2Win.h"
#include "D2Macros.h"

D2WINPOPUP *sgptPopup;

static BOOL __fastcall PopupUpdate(D2WINPOPUP *ptData);
static BOOL __fastcall PopupDraw(D2WINPOPUP *ptData);

void __fastcall D2WinPopupCreate(void)
{
	D2WINPOPUP *ptData;

	if (sgptPopup)
	{
		D2WinMainFreeChild(sgptPopup);
		sgptPopup = NULL;
	}
#line 41
	ptData = (D2WINPOPUP *)D2ALLOC2(sizeof(D2WINPOPUP));
	memset(ptData, 0, sizeof(D2WINPOPUP));
	ptData->eType = D2WIN_POPUP;
	ptData->dwFlags |= gdwBitMasks[0];
	ptData->dwFlags |= gdwBitMasks[2];
	ptData->dwFlags |= gdwBitMasks[3];
	ptData->nX = sgtWindowData.tMouse.x;
	ptData->nY = sgtWindowData.tMouse.y;
	ptData->pfnDraw = (PFNWINCONTROL)PopupDraw;
	ptData->pfnUpdate = (PFNWINCONTROL)PopupUpdate;
	ptData->ptNextChild = NULL;
	ptData->pfnMouseDown = NULL;
	ptData->pfnMouseUp = NULL;
	ptData->pfnChar = NULL;
	ptData->pfnKey = NULL;
	ptData->pfnCallback = NULL;
	ptData->nHoverIndex = -1;
	D2WinMainAddChild(ptData);
	sgptPopup = ptData;
}

DWORD __fastcall D2WinPopupDestroy(void)
{
	DWORD dwResult;

	dwResult = 0;
	if (sgptPopup)
	{
		dwResult = D2WinMainFreeChild(sgptPopup);
		sgptPopup = NULL;
	}
	return dwResult;
}

BOOL __fastcall PopupIsOpen(void)
{
	D2WINPOPUP *ptData;

	ptData = sgptPopup;
	if (ptData)
	{
#line 98
		D2ASSERT(ptData->eType == D2WIN_POPUP);
		if ((gdwBitMasks[2] & ptData->dwFlags) && (gdwBitMasks[0] & ptData->dwFlags))
			return TRUE;
	}
	return FALSE;
}

BOOL __stdcall D2WinPopupAddEntry(D2WINPOPUPENTRY *ptEntry)
{
	D2WINPOPUP *ptData;
	int nWidth;

	ptData = sgptPopup;
#line 116
	D2ASSERT(ptData->eType == D2WIN_POPUP);
	if (ptData->nNumEntries >= NUM_POPUP_ENTRIES)
		return FALSE;
	memcpy(&ptData->atEntries[ptData->nNumEntries], ptEntry, sizeof(D2WINPOPUPENTRY));
	D2WinFontSet(1);
	nWidth = D2WinFontGetTextWidth(ptEntry->wszText);
	if (ptData->nWidth > nWidth)
		nWidth = ptData->nWidth;
	ptData->nWidth = nWidth;
	ptData->nNumEntries++;
	ptData->nHeight = ptData->nNumEntries * 20;
	return TRUE;
}

BOOL __stdcall PopupOnClick(_PARAMS *ptMsg)
{
	D2WINPOPUP *ptData;

	ptData = sgptPopup;
#line 144
	D2ASSERT(ptData->eType == D2WIN_POPUP);
	if ((gdwBitMasks[2] & ptData->dwFlags) && (gdwBitMasks[0] & ptData->dwFlags) && ptData->nHoverIndex != -1
		&& ptData->atEntries[ptData->nHoverIndex].pfnCallback)
	{
		ptData->atEntries[ptData->nHoverIndex].pfnCallback();
		return TRUE;
	}
	return FALSE;
}

static BOOL __fastcall PopupUpdate(D2WINPOPUP *ptData)
{
	int nX, nY;

#line 166
	D2ASSERT(ptData->eType == D2WIN_POPUP);
	if (!(gdwBitMasks[2] & ptData->dwFlags) || !(gdwBitMasks[0] & ptData->dwFlags))
		return FALSE;
	ptData->nHoverIndex = -1;
	nX = sgtWindowData.tMouse.x - ptData->nX;
	nY = ptData->nHeight - ptData->nY + sgtWindowData.tMouse.y;
	if (nX >= 0 && nX <= ptData->nWidth && nY >= 0 && nY <= ptData->nHeight)
		ptData->nHoverIndex = nY / 20;
	return TRUE;
}

static BOOL __fastcall PopupDraw(D2WINPOPUP *ptData)
{
	RECT tRect;
	BYTE bColor;
	int i;

#line 198
	D2ASSERT(ptData->eType == D2WIN_POPUP);
	if (!(gdwBitMasks[2] & ptData->dwFlags) || !(gdwBitMasks[0] & ptData->dwFlags))
		return FALSE;
	bColor = (BYTE)D2WinPaletteGetColor(63, 63, 63);
	tRect.left = ptData->nX - 5;
	tRect.top = ptData->nY - ptData->nHeight - 5;
	tRect.right = ptData->nWidth + ptData->nX + 5;
	tRect.bottom = ptData->nY + 5;
	D2gfxScaleDrawFrame(&tRect, 255);
	D2gfxScaleFillSpan(tRect.left, tRect.bottom, tRect.right, tRect.top, bColor, 2);
	D2WinFontSet(1);
	for (i = 0; i < ptData->nNumEntries; i++)
	{
		D2WinFontDrawText(ptData->atEntries[i].wszText, ptData->nX,
			ptData->nY + (i - ptData->nNumEntries + 1) * 20, i == ptData->nHoverIndex ? 3 : 0, FALSE);
	}
	return TRUE;
}
