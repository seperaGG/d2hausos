#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinProgressBar.cpp"
#include "D2Win.h"
#include "D2Macros.h"

BOOL __fastcall D2WinProgressBarDraw(D2WINPROGRESSBAR *ptData);

D2WINPROGRESSBAR * __fastcall D2WinProgressBarCreate(int nX, int nY, int nWidth, int nHeight)
{
	D2WINPROGRESSBAR *ptData;

#line 26
	ptData = (D2WINPROGRESSBAR *)D2ALLOC2(sizeof(D2WINPROGRESSBAR));
	memset(ptData, 0, sizeof(D2WINPROGRESSBAR));
	ptData->eType = D2WIN_PROGRESSBAR;
	ptData->dwFlags |= gdwBitMasks[0];
	ptData->dwFlags |= gdwBitMasks[2];
	ptData->hCelFile = NULL;
	ptData->nX = nX;
	ptData->nY = nY;
	ptData->nWidth = nWidth;
	ptData->nHeight = nHeight;
	ptData->pfnDraw = (PFNWINCONTROL)D2WinProgressBarDraw;
	ptData->ptNextChild = NULL;
	ptData->pfnMouseDown = NULL;
	ptData->pfnMouseUp = NULL;
	ptData->pfnChar = NULL;
	ptData->pfnKey = NULL;
	ptData->pfnCallback = NULL;
	ptData->flFilledPart = 0;
	D2WinMainAddChild(ptData);
	return ptData;
}

BOOL __stdcall D2WinProgressBarSet(D2WINPROGRESSBAR *ptData, float fPart)
{
#line 58
	D2ASSERT(ptData->eType == D2WIN_PROGRESSBAR);
	memcpy(&ptData->flFilledPart, &fPart, sizeof(ptData->flFilledPart));
	return TRUE;
}

DWORD __fastcall D2WinProgressBarDestroy(D2WINPROGRESSBAR *ptData)
{
	return D2WinMainFreeChild(ptData);
}

BOOL __fastcall D2WinProgressBarDraw(D2WINPROGRESSBAR *ptData)
{
	BYTE bColor;

#line 79
	D2ASSERT(ptData->eType == D2WIN_PROGRESSBAR);
	if (!(gdwBitMasks[2] & ptData->dwFlags) || !(gdwBitMasks[0] & ptData->dwFlags))
		return FALSE;
	bColor = (BYTE)D2WinPaletteGetColor(255, 0, 0);
	D2gfxScaleFillSpan(ptData->nX, ptData->nY,
		(int)(__int64)((float)ptData->nWidth * ptData->flFilledPart) + ptData->nX, ptData->nHeight
			+ ptData->nY,
		bColor, 5);
	return TRUE;
}
