#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\gdiSpCel.cpp"
#include "gdiCore.h"
#include "D2GdiImports.h"
#include "D2Macros.h"
#include "gdiCel.h"

static DWORD sgadwEnvKind[8] = { 2, 2, 2, 1, 1, 0, 1, 0 };

int gnGdiClipRight = 0;
int gnGdiClipLeft = 0;

BYTE * __fastcall GdiGetEnvValue(int nIndex, DWORD *pdwKind)
{
	*pdwKind = sgadwEnvKind[nIndex];
	switch (nIndex)
	{
	case 0:
		return gptEnvInfo->pbBlendMode0;
	case 1:
		return gptEnvInfo->pbBlendMode1;
	case 2:
		return gptEnvInfo->pbBlendMode2;
	case 3:
		return gptEnvInfo->pbBlendMode3;
	case 4:
		return gptEnvInfo->pbBlendMode4;
	case 6:
		return gptEnvInfo->pbBlendMode6;
	}
	return NULL;
}

void __fastcall GdiSmackInit(BOOL fWide)
{
	if (!fWide)
	{
		gnGdiClipLeft = 0;
		gnGdiClipRight = 640;
	}
	else
	{
		gnGdiClipLeft = 0;
		gnGdiClipRight = 800;
	}
}

BOOL __fastcall GdiDrawCelSkip(D2CELCONTEXT *ptCelContext, int nX, int nY, DWORD dwColor, int nMode,
	DWORD dwUnused, const BYTE *pbPreMap)
{
	return GdiDrawCel(ptCelContext, nX, nY, dwColor, nMode, pbPreMap);
}

BOOL __fastcall GdiDrawCel(D2CELCONTEXT *ptCelContext, int nX, int nY, DWORD dwColor, int nMode,
	const BYTE *pbPreMap)
{
	DWORD dwKind;
	int nColor;
	const BYTE *pbColorMap;
	const BYTE *pbBlend;

	if (!D2CMPSpriteCacheGetContextCel(ptCelContext, 0, 1))
		return FALSE;
	if (!GdiCelIsVisible(ptCelContext->hCel, nX, nY, FALSE))
		return FALSE;
	nColor = dwColor & 255;
	pbColorMap = NULL;
	pbBlend = GdiGetEnvValue(nMode, &dwKind);
	if (nColor != 255)
		pbColorMap = gptEnvInfo->apbColorMaps[nColor >> 3];
	if (nMode == 7)
		pbColorMap = gptEnvInfo->pbBrightColorMap;
	D2CMPCelContextDraw(ptCelContext, nX, nY, gnGdiClipLeft, gnGdiClipRight, gpbOffscreenBuffer,
		gnBufferHeight, gnBufferWidth, pbColorMap, pbBlend, pbPreMap);
	return TRUE;
}

BOOL __fastcall GdiDrawCelPreMap(D2CELCONTEXT *ptCelContext, int nX, int nY, DWORD dwColor, int nMode,
	int nPreMap)
{
	DWORD dwKind;
	int nColor;
	const BYTE *pbColorMap;
	const BYTE *pbPreMap;
	const BYTE *pbBlend;

	if (!D2CMPSpriteCacheGetContextCel(ptCelContext, 0, 1))
		return FALSE;
	if (!GdiCelIsVisible(ptCelContext->hCel, nX, nY, FALSE))
		return FALSE;
	pbColorMap = NULL;
	pbPreMap = NULL;
	pbBlend = GdiGetEnvValue(nMode, &dwKind);
	nColor = dwColor & 255;
	if (nColor != 255)
		pbColorMap = gptEnvInfo->apbColorMaps[nColor >> 3];
	if (nMode == 7)
		pbColorMap = gptEnvInfo->pbBrightColorMap;
	if (nPreMap)
		pbPreMap = gptEnvInfo->apbPreMaps[nPreMap];
	D2CMPCelContextDraw(ptCelContext, nX, nY, gnGdiClipLeft, gnGdiClipRight, gpbOffscreenBuffer,
		gnBufferHeight, gnBufferWidth, pbColorMap, pbBlend, pbPreMap);
	return TRUE;
}
