#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinImage.cpp"
#include "D2Win.h"
#include "D2Macros.h"

BOOL __fastcall D2WinImageDraw(D2WINIMAGE *ptImageData);
static BOOL __stdcall D2WinImageMouseDown(_PARAMS *ptMsg);

D2WINIMAGE * __fastcall D2WinImageCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	PFNWINMSG pfnCallback, D2WINHOTSPOT *ptHotSpots, PFNWINMSG pfnKey)
{
	D2WINIMAGE *ptImageData;

#line 28
	ptImageData = (D2WINIMAGE *)D2ALLOC2(sizeof(D2WINIMAGE));
	memset(ptImageData, 0, sizeof(D2WINIMAGE));
	ptImageData->eType = D2WIN_IMAGE;
	ptImageData->dwFlags |= gdwBitMasks[0];
	ptImageData->dwFlags |= gdwBitMasks[2];
	ptImageData->hCelFile = hCelFile;
	ptImageData->nWidth = nWidth;
	ptImageData->nHeight = nHeight;
	ptImageData->pfnKey = pfnKey;
	ptImageData->nX = nX;
	ptImageData->nY = nY;
	ptImageData->pfnDraw = (PFNWINCONTROL)D2WinImageDraw;
	ptImageData->ptNextChild = 0;
	ptImageData->pfnMouseDown = D2WinImageMouseDown;
	ptImageData->pfnMouseUp = NULL;
	ptImageData->pfnChar = 0;
	ptImageData->pfnCallback = pfnCallback;
	ptImageData->ptHotSpots = ptHotSpots;
	ptImageData->nCel = 0;
	ptImageData->dwDrawMode = 5;
	D2WinMainAddChild(ptImageData);
	return ptImageData;
}

BOOL __stdcall D2WinImageSetCelFile(D2WINIMAGE *ptImageData, HCELFILE hCelFile, D2WINHOTSPOT *ptHotSpots)
{
#line 62
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	ptImageData->hCelFile = hCelFile;
	ptImageData->ptHotSpots = ptHotSpots;
	return TRUE;
}

BOOL __stdcall D2WinImageSetPosition(D2WINIMAGE *ptImageData, int nX, int nY)
{
#line 75
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	ptImageData->nX = nX;
	ptImageData->nY = nY;
	return TRUE;
}

DWORD __fastcall D2WinImageDestroy(D2WINIMAGE *ptImageData)
{
	return D2WinMainFreeChild(ptImageData);
}

BOOL __stdcall D2WinImageSetCel(D2WINIMAGE *ptImageData, int nCel)
{
	int nLast;

#line 97
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	nLast = (int)D2CMPCelFileGetNumFrames(ptImageData->hCelFile);
	if (nCel > nLast - 1)
		nCel = nLast - 1;
	if (nCel < 0)
		nCel = 0;
	ptImageData->nCel = nCel;
	return TRUE;
}

BOOL __stdcall D2WinImageSetDrawMode(D2WINIMAGE *ptImageData, DWORD dwDrawMode)
{
#line 115
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	ptImageData->dwDrawMode = 5;
	return TRUE;
}

int __stdcall D2WinImageGetCels(D2WINIMAGE *ptImageData)
{
#line 127
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	return (int)D2CMPCelFileGetNumFrames(ptImageData->hCelFile);
}

BOOL __fastcall D2WinImageDraw(D2WINIMAGE *ptImageData)
{
	D2CELCONTEXT tCelContext;
	int nRows, nCols;
	int nRow, nCol;
	int nY;
	int nLeft;

#line 140
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	if (!(ptImageData->dwFlags & gdwBitMasks[2]) || !(ptImageData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	if (ptImageData->hCelFile)
	{
		nY = ptImageData->nY - ptImageData->nHeight + 1;
		nLeft = ptImageData->nX;
		nCols = (ptImageData->nWidth + 255) >> 8;
		nRows = ((ptImageData->nHeight + 255) >> 8) - 1;
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.hCelFile = ptImageData->hCelFile;
		tCelContext.dwDirection = 0;
		tCelContext.dwFrame = ptImageData->nCel;
		for (nRow = nRows; nRow > 0; nRow--)
		{
			for (nCol = 0; nCol < nCols; nCol++)
			{
				D2gfxScaleDrawCel(&tCelContext, nLeft + nCol * 256, nY + 256, -1, ptImageData->dwDrawMode,
					0);
				tCelContext.dwFrame++;
			}
			nY += 256;
		}
		if (nRows >= 0)
		{
			for (nCol = 0; nCol < nCols; nCol++)
			{
				D2gfxScaleDrawCel(&tCelContext, nLeft + nCol * 256, ptImageData->nY, -1,
					ptImageData->dwDrawMode, 0);
				tCelContext.dwFrame++;
			}
		}
	}
	return TRUE;
}

static BOOL __stdcall D2WinImageMouseDown(_PARAMS *ptMsg)
{
	D2WINIMAGE *ptImageData;
	D2WINHOTSPOT *ptHotSpots;
	D2WINHOTSPOT *ptHotSpot;
	int nX, nY;
	int i;
	int nHotY;

	ptImageData = (D2WINIMAGE *)ptMsg->window;
#line 205
	D2ASSERT(ptImageData->eType == D2WIN_IMAGE);
	if (!(ptImageData->dwFlags & gdwBitMasks[2]) || !(ptImageData->dwFlags & gdwBitMasks[0]))
		return FALSE;
	nX = sgtWindowData.tMouse.x - ptImageData->nX;
	nY = ptImageData->nHeight - ptImageData->nY + sgtWindowData.tMouse.y;
	ptHotSpots = ptImageData->ptHotSpots;
	if (ptHotSpots)
	{
		i = 0;
		if (ptHotSpots[i].pfnCallback)
		while (1)
		{
			ptHotSpot = &ptHotSpots[i];
			nHotY = ptHotSpot->nY;
			if (nX >= ptHotSpot->nX && nY >= nHotY - ptHotSpot->nHeight
				&& nX < ptHotSpot->nWidth + ptHotSpot->nX && nY < nHotY)
			{
				return ptHotSpots[i].pfnCallback(ptMsg);
			}
			if (!ptHotSpots[++i].pfnCallback)
				break;
		}
	}
	if (ptImageData->pfnCallback)
		return ptImageData->pfnCallback(ptMsg);
	return FALSE;
}
