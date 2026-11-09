#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\gdiEngine.cpp"
#include "gdiCore.h"
#include <stdarg.h>
#include "D2Macros.h"

static HFONT sghFont = NULL;
static DWORD sgdwTextColor = 16777215;
static DWORD sgdwBkColor = 0;

static __inline int GdiAbs(int n)
{
	return n < 0 ? -n : n;
}

BYTE __fastcall GdiFindNearestPaletteIndex(int nRed, int nBlue, int nGreen)
{
	BYTE bBestIndex;
	int nBestDist;
	int nIndex;
	int nDist;
	int nRedDelta;
	int nGreenDelta;
	int nBlueDelta;

	bBestIndex = 0;
	nBestDist = 10000;
	for (nIndex = 1; nIndex < 256; nIndex++)
	{
		nRedDelta = GdiAbs(gargbColorTable[nIndex].rgbRed - nRed);
		nGreenDelta = GdiAbs(gargbColorTable[nIndex].rgbGreen - nGreen);
		nBlueDelta = GdiAbs(gargbColorTable[nIndex].rgbBlue - nBlue);
		nDist = nBlueDelta + nGreenDelta + nRedDelta;
		if (nDist < nBestDist)
		{
			nBestDist = nDist;
			bBestIndex = (BYTE)nIndex;
		}
	}
	return bBestIndex;
}

BOOL WINAPI GdiGetMemoryBudget(void)
{
	return FALSE;
}

void WINAPI GdiCreateFont(void)
{
	sghFont = CreateFontA(10, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, FIXED_PITCH, "Courier");
}

void WINAPI GdiDeleteFont(void)
{
	DeleteObject(sghFont);
}

int __fastcall GdiDrawDebugText(int nX, int nY, const char *pszFormat, va_list vaArgs)
{
	RECT rc;
	char szText[512];
	HDC hWndDC;
	HDC hMemDC;

	wvsprintfA(szText, pszFormat, vaArgs);
	hWndDC = GetDC(NULL);
	hMemDC = CreateCompatibleDC(hWndDC);
	SelectObject(hMemDC, GdiGetBitmapHandle());
	SelectObject(hMemDC, sghFont);
	SetTextColor(hMemDC, sgdwTextColor);
	SetBkColor(hMemDC, sgdwBkColor);
	SetBkMode(hMemDC, TRANSPARENT);
	SetRect(&rc, nX, nY, gnBufferWidth - nX, gnBufferHeight - nY);
	DrawTextA(hMemDC, szText, -1, &rc, 0);
	DeleteDC(hMemDC);
	return ReleaseDC(NULL, hWndDC);
}

void WINAPI GdiNothingDebugLightGrid(DWORD dwValue1, DWORD dwValue2)
{
}

DWORD WINAPI GdiGetDrawCtxFlag(void)
{
	return gptDrawCtx->dwBlendShadow;
}

void __fastcall GdiFillSpan(int nX1, int nY1, int nX2, int nY2, BYTE bValue, int nEnvIndex)
{
	BYTE *pbTable;
	DWORD dwKind;
	BYTE *pbRow;
	BYTE *pbPixel;
	DWORD dwRows;
	DWORD x;
	int nMaxX;
	int nMaxY;
	int nLeft;
	int nRight;
	int nTop;
	int nBottom;

	pbTable = GdiGetEnvValue(nEnvIndex, &dwKind);
	nMaxX = gnBufferWidth - 1;
	nX1 = nX1 > 0 ? nX1 : 0;
	nLeft = nX1 < nMaxX ? nX1 : nMaxX;
	nX2 = nX2 > 0 ? nX2 : 0;
	nRight = nX2 < nMaxX ? nX2 : nMaxX;
	if (nLeft == nRight)
		return;
	nMaxY = gnBufferHeight - 1;
	nTop = nY1 > 0 ? nY1 : 0;
	if (nTop >= nMaxY)
		nTop = nMaxY;
	nBottom = nY2 > 0 ? nY2 : 0;
	if (nBottom < nMaxY)
		nMaxY = nBottom;
	nBottom = nMaxY;
	if (nTop == nBottom)
		return;
#line 50
	D2ASSERTMSG(nBottom > nTop, "nY2 > nY1");
	pbRow = gpbOffscreenBuffer + gnBufferWidth * nTop + nLeft;
	dwRows = nBottom - nTop;
	switch (dwKind)
	{
	case 1:
		while (dwRows--)
		{
			pbPixel = pbRow;
			x = nRight - nLeft;
			while (x--)
			{
				*pbPixel = pbTable[*pbPixel];
				pbPixel++;
			}
			pbRow += gnBufferWidth;
		}
		break;
	case 2:
		while (dwRows--)
		{
			pbPixel = pbRow;
			x = nRight - nLeft;
			while (x--)
			{
				*pbPixel = pbTable[(*pbPixel << 8) + bValue];
				pbPixel++;
			}
			pbRow += gnBufferWidth;
		}
		break;
	default:
		while (dwRows--)
		{
			memset(pbRow, bValue, nRight - nLeft);
			pbRow += gnBufferWidth;
		}
		break;
	}
}

void __fastcall GdiFillSpanSelect(int nX1, int nY1, int nX2, int nY2, BYTE bValue, BYTE bSelector)
{
	int nEnvIndex;

	if (bSelector >= 192)
		nEnvIndex = 5;
	else if (bSelector >= 128)
		nEnvIndex = 2;
	else if (bSelector >= 64)
		nEnvIndex = 1;
	else if (bSelector <= 0)
		return;
	else
		nEnvIndex = 0;
	GdiFillSpan(nX1, nY1, nX2, nY2, bValue, nEnvIndex);
}

static void __fastcall GdiDrawPixel(int nX, int nY, BYTE bColor)
{
	if (nX >= 0 && nX < gnBufferWidth && nY >= 0 && nY < gnBufferHeight)
		gpbOffscreenBuffer[nX + gnBufferWidth * nY] = bColor;
}

void __fastcall GdiDrawLine(int nX1, int nY1, int nX2, int nY2, BYTE bColor, DWORD dwUnused)
{
	int nDeltaX;
	int nDeltaY;
	int nError;
	int i;
	int nY;

	nDeltaX = nX2 - nX1;
	nDeltaY = nY2 - nY1;
	nY = nY1;
	if (nDeltaX < 0)
	{
		nX2 = -1;
		nDeltaX = -nDeltaX;
	}
	else
		nX2 = 1;
	if (nDeltaY < 0)
	{
		nY2 = -1;
		nDeltaY = -nDeltaY;
	}
	else
		nY2 = 1;
	GdiDrawPixel(nX1, nY1, bColor);
	nError = 0;
	if (nDeltaX < nDeltaY)
	{
		for (i = 0; i < nDeltaY; i++)
		{
			nY += nY2;
			nError += nDeltaX;
			if (nError > nDeltaY)
			{
				nX1 += nX2;
				nError -= nDeltaY;
			}
			GdiDrawPixel(nX1, nY, bColor);
		}
	}
	else
	{
		for (i = 0; i < nDeltaX; i++)
		{
			nX1 += nX2;
			nError += nDeltaY;
			if (nError > nDeltaX)
			{
				nY += nY2;
				nError -= nDeltaX;
			}
			GdiDrawPixel(nX1, nY, bColor);
		}
	}
}

static void __fastcall GdiDrawSegment(const POINT *ptFrom, const POINT *ptTo, BYTE bColor);

void __fastcall GdiDrawDiamond(const RECT *prc, BYTE bColor)
{
	POINT tTop;
	POINT tRight;
	POINT tBottom;
	POINT tLeft;
	int nMidX;
	int nMidY;
	int nWidth;
	int nHeight;

#line 294
	D2ASSERT(prc->left <= prc->right);
#line 295
	D2ASSERT(prc->top <= prc->bottom);
	nWidth = prc->right - prc->left;
	nHeight = prc->bottom - prc->top;
	if (nWidth == 0 || nHeight == 0)
		return;
	nMidX = ((DWORD)nWidth >> 1) + prc->left;
	nMidY = ((DWORD)nHeight >> 1) + prc->top;
	tTop.x = nMidX;
	tTop.y = prc->top;
	tRight.x = prc->right;
	tRight.y = nMidY;
	tBottom.x = nMidX;
	tBottom.y = prc->bottom;
	tLeft.x = prc->left;
	tLeft.y = nMidY;
	GdiDrawSegment(&tTop, &tRight, bColor);
	GdiDrawSegment(&tRight, &tBottom, bColor);
	GdiDrawSegment(&tBottom, &tLeft, bColor);
	GdiDrawSegment(&tLeft, &tTop, bColor);
}

static void __fastcall GdiDrawSegment(const POINT *ptFrom, const POINT *ptTo, BYTE bColor)
{
	int nDeltaX;
	int nDeltaY;
	int nLengthSq;
	int nSteps;
	int nStepX;
	int nStepY;
	int nAccX;
	int nAccY;
	int nX;
	int nX0;
	int nY;
	int nY0;
	int nMove;
	int i;
	BYTE *pbPixel;

	nX0 = ptFrom->x;
	nY0 = ptFrom->y;
	nDeltaX = ptTo->x - nX0;
	nDeltaY = ptTo->y - nY0;
	if (!(nDeltaX || nDeltaY))
		return;
	nLengthSq = nDeltaY * nDeltaY + nDeltaX * nDeltaX;
	nSteps = 0;
	if (nLengthSq > 0)
	{
		do
		{
			nSteps++;
		} while (nSteps * nSteps < nLengthSq);
	}
	nStepX = (nDeltaX << 16) / nSteps;
	nStepY = (nDeltaY << 16) / nSteps;
	nAccX = 0;
	nAccY = 0;
	i = 0;
	nY = nY0;
	nX = nX0;
	while (nY < 0 || nY >= gnBufferHeight || nX < 0 || nX >= gnBufferWidth)
	{
		nAccX += nStepX;
		if (nAccX >= 65536 || nAccX <= -65536)
		{
			nMove = nAccX / 65536;
			nX += nMove;
			nAccX += nMove * -65536;
		}
		nAccY += nStepY;
		if (nAccY >= 65536 || nAccY <= -65536)
		{
			nMove = nAccY / 65536;
			nY += nMove;
			nAccY += nMove * -65536;
		}
		if (i >= nSteps)
			return;
		i++;
	}
	pbPixel = gpbOffscreenBuffer + nX + gnBufferWidth * nY;
	*pbPixel = bColor;
	i++;
	if (i >= nSteps)
		return;
	while (1)
	{
		nAccX += nStepX;
		if (nAccX >= 65536 || nAccX <= -65536)
		{
			nMove = nAccX / 65536;
			nX += nMove;
			pbPixel += nMove;
			nAccX += nMove * -65536;
		}
		if (nX < 0)
			return;
		if (nX >= gnBufferWidth)
			return;
		nAccY += nStepY;
		if (nAccY >= 65536 || nAccY <= -65536)
		{
			nMove = nAccY / 65536;
			nY += nMove;
			pbPixel += gnBufferWidth * nMove;
			nAccY += nMove * -65536;
		}
		if (nY < 0)
			return;
		if (nY >= gnBufferHeight)
			return;
		*pbPixel = bColor;
		i++;
		if (i >= nSteps)
			return;
	}
}

void __fastcall GdiDrawFrame(const RECT *prc, BYTE bColor)
{
	POINT tTopRight;
	POINT tBottomRight;
	POINT tTopLeft;
	POINT tBottomLeft;

#line 322
	D2ASSERT(prc->left <= prc->right);
#line 323
	D2ASSERT(prc->top <= prc->bottom);
	if (prc->right - prc->left == 0 || prc->bottom - prc->top == 0)
		return;
	tTopRight.x = prc->right;
	tBottomRight.x = prc->right;
	tBottomRight.y = prc->bottom;
	tBottomLeft.y = prc->bottom;
	tTopLeft.x = prc->left;
	tTopLeft.y = prc->top;
	tTopRight.y = prc->top;
	tBottomLeft.x = prc->left;
	GdiDrawSegment(&tTopLeft, &tTopRight, bColor);
	GdiDrawSegment(&tTopRight, &tBottomRight, bColor);
	GdiDrawSegment(&tBottomRight, &tBottomLeft, bColor);
	GdiDrawSegment(&tBottomLeft, &tTopLeft, bColor);
}

void __fastcall GdiFillRect(RECT *prc, BYTE bColor)
{
	BYTE *pbRow;
	DWORD nWidth;
	int nHeight;
	int nRows;
	union { BYTE b; int n; } uFill;

	uFill.b = bColor;
#line 350
	D2ASSERT(prc->left <= prc->right);
#line 351
	D2ASSERT(prc->top <= prc->bottom);
	if (prc->right < 0 || prc->left >= gnBufferWidth || prc->bottom < 0 || prc->top >= gnBufferHeight)
		return;
	if (prc->left < 0)
		prc->left = 0;
	if (prc->right >= gnBufferWidth)
		prc->right = gnBufferWidth - 1;
	if (prc->top < 0)
		prc->top = 0;
	if (prc->bottom >= gnBufferHeight)
		prc->bottom = gnBufferHeight - 1;
	nWidth = prc->right - prc->left;
	nHeight = prc->bottom - prc->top;
	if (nWidth == 0 || nHeight == 0)
		return;
	pbRow = gpbOffscreenBuffer + gnBufferWidth * prc->top + prc->left;
	nRows = nHeight;
	do
	{
		if (nWidth > 0)
			memset(pbRow, uFill.n, nWidth);
		pbRow += gnBufferWidth;
	} while (--nRows);
}

void __fastcall GdiFillSquare(const POINT *ptCenter, BYTE bSize, BYTE bColor)
{
	RECT rc;

	if (!bSize)
		return;
	bSize--;
	SetRect(&rc, ptCenter->x - bSize, ptCenter->y - bSize, ptCenter->x + bSize + 1,
		ptCenter->y + bSize + 1);
	GdiFillRect(&rc, bColor);
}

void __fastcall GdiClearBackBuffer(BOOL fAbovePanel)
{
	if (fAbovePanel)
		memset(gpbOffscreenBuffer, 0, gnBufferWidth * (gnBufferHeight - 47));
	else
		memset(gpbOffscreenBuffer, 0, gnBufferWidth * gnBufferHeight);
}
