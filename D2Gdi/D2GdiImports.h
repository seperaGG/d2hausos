#ifndef MATCH_D2GDI_IMPORTS_H
#define MATCH_D2GDI_IMPORTS_H

#include <windows.h>

extern "C"
{
DWORD __stdcall D2CMPOrdinal10004(DWORD, DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall D2CMPOrdinal10033(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall D2CMPOrdinal10034(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall D2CMPOrdinal10035(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall D2CMPOrdinal10037(DWORD);
DWORD __stdcall D2CMPOrdinal10038(DWORD);
DWORD __stdcall D2CMPOrdinal10039(DWORD);
DWORD __stdcall D2CMPOrdinal10040(DWORD);
DWORD __stdcall D2CMPOrdinal10048(DWORD, DWORD);
DWORD __stdcall D2CMPOrdinal10055(DWORD, DWORD, DWORD);
DWORD __stdcall D2CMPOrdinal10091(DWORD, DWORD);
__declspec(dllimport) DWORD __stdcall SmackClose(DWORD);
__declspec(dllimport) DWORD __stdcall SmackDoFrame(DWORD);
__declspec(dllimport) DWORD __stdcall SmackNextFrame(DWORD);
__declspec(dllimport) DWORD __stdcall SmackOpen(DWORD, DWORD, DWORD);
__declspec(dllimport) DWORD __stdcall SmackToBuffer(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
__declspec(dllimport) DWORD __stdcall SmackWait(DWORD);
}

#define D2CMPPaletteFindNearest(pbPalette, dwEntries, nColor0, nColor1, nColor2) \
	D2CMPOrdinal10004((DWORD)(pbPalette), (DWORD)(dwEntries), (DWORD)(nColor0), (DWORD)(nColor1), \
		(DWORD)(nColor2))
#define D2CMPCelContextDraw(pCelContext, nX, nY, nClipLeft, nClipRight, pDestSurface, nSurfaceHeight, \
		dwStride, pbColorMap, pbBlend, pbPreMap) \
	D2CMPOrdinal10033((DWORD)(pCelContext), (DWORD)(nX), (DWORD)(nY), (DWORD)(nClipLeft), \
		(DWORD)(nClipRight), (DWORD)(pDestSurface), (DWORD)(nSurfaceHeight), (DWORD)(dwStride), \
		(DWORD)(pbColorMap), (DWORD)(pbBlend), (DWORD)(pbPreMap))
#define D2CMPCelContextDrawRows(pCelContext, nX, nY, nClipLeft, nClipRight, pDestSurface, nSurfaceHeight, \
		dwStride, pbColorMap, pbBlend, nFromBottom, nRows, pbPreMap) \
	D2CMPOrdinal10034((DWORD)(pCelContext), (DWORD)(nX), (DWORD)(nY), (DWORD)(nClipLeft), \
		(DWORD)(nClipRight), (DWORD)(pDestSurface), (DWORD)(nSurfaceHeight), (DWORD)(dwStride), \
		(DWORD)(pbColorMap), (DWORD)(pbBlend), (DWORD)(nFromBottom), (DWORD)(nRows), (DWORD)(pbPreMap))
#define D2CMPCelContextDrawShadow(pCelContext, nX, nY, nClipLeft, nClipRight, pDestSurface, nSurfaceHeight, \
		dwStride, pbTable, dwColor) \
	D2CMPOrdinal10035((DWORD)(pCelContext), (DWORD)(nX), (DWORD)(nY), (DWORD)(nClipLeft), \
		(DWORD)(nClipRight), (DWORD)(pDestSurface), (DWORD)(nSurfaceHeight), (DWORD)(dwStride), \
		(DWORD)(pbTable), (DWORD)(dwColor))
#define D2CMPCelGetWidth(pCel) \
	D2CMPOrdinal10037((DWORD)(pCel))
#define D2CMPCelGetHeight(pCel) \
	D2CMPOrdinal10038((DWORD)(pCel))
#define D2CMPCelGetOffsetX(pCel) \
	((int)D2CMPOrdinal10039((DWORD)(pCel)))
#define D2CMPCelGetOffsetY(pCel) \
	((int)D2CMPOrdinal10040((DWORD)(pCel)))
#define D2CMPFastCmpWarp(pbSurface, nPitch) \
	D2CMPOrdinal10048((DWORD)(pbSurface), (DWORD)(nPitch))
#define D2CMPSpriteCacheGetContextCel(ptCelContext, fPoll, fAsyncAhead) \
	((BOOL)D2CMPOrdinal10055((DWORD)(ptCelContext), (DWORD)(fPoll), (DWORD)(fAsyncAhead)))
#define D2CMPTileCacheLoadTile(ptTile, dwUnused) \
	((BOOL)D2CMPOrdinal10091((DWORD)(ptTile), (DWORD)(dwUnused)))

#endif
