#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\gdiSprite.cpp"
#include "gdiCore.h"
#include "D2GdiImports.h"
#include "D2Macros.h"
#include "gdiCel.h"

struct GDIBLENDPAIR
{
 	DWORD		dwLightTopLeft;
 	DWORD		dwLightTopRight;
 	DWORD		dwLightBottomRight;
 	DWORD		dwLightBottomLeft;
};

void __fastcall GdiSpriteDrawRows(D2CELCONTEXT *ptCelContext, int nX, int nY, int nFromBottom, int nRows,
	int nMode)
{
	BYTE *pbBlend;

	if (!D2CMPSpriteCacheGetContextCel(ptCelContext, 0, 1))
		return;
	if (!GdiCelIsVisible(ptCelContext->hCel, nX, nY, FALSE))
		return;
	switch (nMode)
	{
	case 0: pbBlend = gptEnvInfo->pbBlendMode0; break;
	case 1: pbBlend = gptEnvInfo->pbBlendMode1; break;
	case 2: pbBlend = gptEnvInfo->pbBlendMode2; break;
	case 3: pbBlend = gptEnvInfo->pbBlendMode3; break;
	case 4: pbBlend = gptEnvInfo->pbBlendMode4; break;
	case 6: pbBlend = gptEnvInfo->pbBlendMode6; break;
	default: pbBlend = NULL; break;
	}
	D2CMPCelContextDrawRows(ptCelContext, nX, nY, gnGdiClipLeft, gnGdiClipRight,
		gpbOffscreenBuffer, gnBufferHeight, gnBufferWidth, NULL, pbBlend,
		nFromBottom, nRows, NULL);
}

void __fastcall GdiSpriteDrawShadow(D2CELCONTEXT *ptCelContext, int nX, int nY)
{
	BYTE *pbTable;

	if (!D2CMPSpriteCacheGetContextCel(ptCelContext, 0, 1))
		return;
	if (!GdiCelIsVisible(ptCelContext->hCel, nX, nY, TRUE))
		return;
	pbTable = NULL;
	if (GdiGetDrawCtxFlag())
		pbTable = gptEnvInfo->pbBlendMode2;
	D2CMPCelContextDrawShadow(ptCelContext, nX, nY, gnGdiClipLeft, gnGdiClipRight,
		gpbOffscreenBuffer, gnBufferHeight, gnBufferWidth, pbTable, 0);
}

void __stdcall GdiSpriteStub(DWORD dwY, DWORD dwColor)
{
}

void __fastcall GdiSpriteWarp(int nX, int nY)
{
#line 313
	D2ASSERT(nX > 0 && nX < gnBufferWidth - 80);
#line 314
	D2ASSERT(nY > 0 && nY < gnBufferHeight - 80);
	D2CMPFastCmpWarp(gpbOffscreenBuffer + nY * gnBufferWidth + nX, gnBufferWidth);
}

BOOL __fastcall GdiSpriteDrawUnitBlocks(D2GDITILE *ptTile, int nX, int nY, DWORD dwUnused, int nBoundaryMode)
{
	int nRightBound;
	int nLeftBound;
	int nBottomBound;
	int i;
	int nPartX;
	int nPartY;
	BOOL fFound;

	fFound = FALSE;
	if (D2CMPTileCacheLoadTile(ptTile, 1))
	{
		nRightBound = gnBufferWidth;
		nLeftBound = -32;
		nBottomBound = gnBufferHeight + 32;
		if (nBoundaryMode == -160)
			nRightBound -= 320;
		else if (nBoundaryMode == 160)
			nLeftBound = 288;
		i = 0;
		if (ptTile->nCompCount > 0)
		{
			do
			{
				nPartX = ptTile->ptCompArray[i].sOffsetX + nX;
				nPartY = ptTile->ptCompArray[i].sOffsetY + nY;
				if (nPartX >= nLeftBound && nPartX < nRightBound && nPartY >= -32 && nPartY < nBottomBound)
				{
					fFound = TRUE;
					if (GdiGetDrawCtxFlag())
						gptBlitTable->pfnDrawBlockFlags(ptTile->ptCompArray[i].ptData, nPartX, nPartY, 192);
					else
						gptBlitTable->pfnDrawBlock(ptTile->ptCompArray[i].ptData, nPartX, nPartY);
				}
				i++;
			} while (i < ptTile->nCompCount);
		}
	}
	return fFound;
}

BOOL __fastcall GdiSpriteDrawWallShaded(D2GDITILE *ptTile, int nX, int nY, const BYTE *pbShadeTable,
	int nBoundaryMode, DWORD dwAlpha)
{
	int nRightBound;
	int nLeftBound;
	int nBottomBound;
	int i;
	int nPartX;
	int nPartY;
	int nColumn;
	BOOL fFound;
	D2GDITILEPART *ptPart;
	GDIBLENDPAIR tBlend;

	fFound = FALSE;
	if (D2CMPTileCacheLoadTile(ptTile, 1))
	{
		nRightBound = gnBufferWidth;
		nLeftBound = -32;
		nBottomBound = gnBufferHeight + 32;
		if (nBoundaryMode == -160)
			nRightBound -= 320;
		else if (nBoundaryMode == 160)
			nLeftBound = 288;
		i = 0;
		if (ptTile->nCompCount > 0)
		{
			do
			{
				nPartX = ptTile->ptCompArray[i].sOffsetX + nX;
				nPartY = ptTile->ptCompArray[i].sOffsetY + nY;
				if (nPartX >= nLeftBound && nPartX < nRightBound && nPartY >= -32 && nPartY < nBottomBound)
				{
					fFound = TRUE;
					ptPart = &ptTile->ptCompArray[i];
					nColumn = ptPart->sOffsetX >> 5;
					tBlend.dwLightTopLeft = pbShadeTable[nColumn * 4];
					tBlend.dwLightBottomLeft = tBlend.dwLightTopLeft;
					tBlend.dwLightTopRight = pbShadeTable[nColumn * 4 + 4];
					tBlend.dwLightBottomRight = tBlend.dwLightTopRight;
					gptBlitTable->pfnDrawBlockShadedArg(ptPart->ptData, nPartX, nPartY, &tBlend, dwAlpha);
				}
				i++;
			} while (i < ptTile->nCompCount);
		}
	}
	return fFound;
}

BOOL __fastcall GdiSpriteDrawWallLow(D2GDITILE *ptTile, int nX, int nY, const BYTE *pbShadeTable,
	int nBoundaryMode)
{
	int nRightBound;
	int nLeftBound;
	int nBottomBound;
	int i;
	int nPartX;
	int nPartY;
	int nColumn;
	BOOL fFound;
	D2GDITILEPART *ptPart;
	GDIBLENDPAIR tBlend;

	fFound = FALSE;
	if (D2CMPTileCacheLoadTile(ptTile, 1))
	{
		nRightBound = gnBufferWidth;
		nLeftBound = -32;
		nBottomBound = gnBufferHeight + 32;
		if (nBoundaryMode == -160)
			nRightBound -= 320;
		else if (nBoundaryMode == 160)
			nLeftBound = 288;
		i = 0;
		if (ptTile->nCompCount > 0)
		{
			do
			{
				nPartX = ptTile->ptCompArray[i].sOffsetX + nX;
				nPartY = ptTile->ptCompArray[i].sOffsetY + nY;
				if (nPartX >= nLeftBound && nPartX < nRightBound && nPartY >= -32 && nPartY < nBottomBound)
				{
					fFound = TRUE;
					ptPart = &ptTile->ptCompArray[i];
					nColumn = ptPart->sOffsetX >> 5;
					tBlend.dwLightTopLeft = pbShadeTable[nColumn * 4];
					tBlend.dwLightBottomLeft = tBlend.dwLightTopLeft;
					tBlend.dwLightTopRight = pbShadeTable[nColumn * 4 + 4];
					tBlend.dwLightBottomRight = tBlend.dwLightTopRight;
					gptBlitTable->pfnDrawBlockShaded(ptPart->ptData, nPartX, nPartY, &tBlend);
				}
				i++;
			} while (i < ptTile->nCompCount);
		}
	}
	return fFound;
}
