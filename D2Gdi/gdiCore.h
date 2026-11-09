#ifndef D2RE_D2GDI_GDICORE_H
#define D2RE_D2GDI_GDICORE_H

#include <windows.h>
#include <stdarg.h>

struct GDIDRAWCTX
{
 	BYTE		unobserved_000[20];
 	DWORD		dwBlendShadow;
};

struct GDIBLITTABLE
{
 	void		(__fastcall *pfnSetSurface)(BYTE *pbSurface, int nWidth, int nHeight,
					DWORD dwZero);
 	DWORD		(__fastcall *pfnDrawBlock)(void *pData, int nX, int nY);
 	void		*unobserved_008;
 	DWORD		(__fastcall *pfnDrawTile)(void *ptTile, int nX, DWORD dwY, DWORD dwVertices);
 	DWORD		(__fastcall *pfnDrawBlockShaded)(void *pData, int nX, int nY, const void *pShade);
 	DWORD		(__fastcall *pfnDrawBlockFlags)(void *pData, int nX, int nY, DWORD dwFlags);
 	DWORD		(__fastcall *pfnDrawBlockShadedArg)(void *pData, int nX, int nY,
					const void *pShade, DWORD dwArg);
};

struct GDIENVINFO
{
 	BYTE		*pbBlendMode2;
 	BYTE		*pbBlendMode1;
 	BYTE		*pbBlendMode0;
 	BYTE		*apbColorMaps[32];
 	BYTE		unobserved_08c[208 - 140];
 	BYTE		*apbPreMaps[13];
 	BYTE		*pbBlendMode3;
 	BYTE		*pbBlendMode4;
 	BYTE		*pbBlendMode6;
 	BYTE		unobserved_110[280 - 272];
 	BYTE		*pbBrightColorMap;
 	BYTE		unobserved_11c[288 - 284];
};

typedef struct D2CEL D2CEL;

struct D2CELCONTEXT
{
 	D2CEL		*hCel;
};

struct D2GDITILEPART
{
 	short		sOffsetX;
 	short		sOffsetY;
 	BYTE		unobserved_04[12];
 	void		*ptData;
};

struct D2GDITILE
{
 	BYTE		unobserved_00[80];
 	int			nCompCount;
 	D2GDITILEPART *ptCompArray;
};

extern BYTE *gpbOffscreenBuffer;
extern int gnBufferWidth;
extern int gnBufferHeight;
extern GDIBLITTABLE *gptBlitTable;
extern GDIDRAWCTX *gptDrawCtx;
extern GDIENVINFO *gptEnvInfo;
extern HWND ghWnd;
extern HBITMAP ghBitmap;
extern PALETTEENTRY gatPaletteEntries[256];
extern DWORD gfSuspended;
extern RGBQUAD gargbColorTable[256];
extern int gnGdiClipRight;
extern int gnGdiClipLeft;

HBITMAP WINAPI GdiGetBitmapHandle(void);

void WINAPI GdiNothingResetPerspDepth(void);
BOOL __fastcall GdiReinit(DWORD dwUnused, BOOL fWide);
BOOL WINAPI GdiAlwaysFalse(void);
BOOL WINAPI GdiGetFrameOpenQuery(void);
BYTE __fastcall GdiFindNearestPaletteIndex(int nRed, int nBlue, int nGreen);
BOOL WINAPI GdiGetMemoryBudget(void);
void WINAPI GdiCreateFont(void);
void WINAPI GdiDeleteFont(void);
int __fastcall GdiDrawDebugText(int nX, int nY, const char *pszFormat, va_list vaArgs);
void WINAPI GdiNothingDebugLightGrid(DWORD dwValue1, DWORD dwValue2);
DWORD WINAPI GdiGetDrawCtxFlag(void);
void __fastcall GdiFillSpan(int nX1, int nY1, int nX2, int nY2, BYTE bValue, int nEnvIndex);
void __fastcall GdiFillSpanSelect(int nX1, int nY1, int nX2, int nY2, BYTE bValue, BYTE bSelector);
void __fastcall GdiDrawLine(int nX1, int nY1, int nX2, int nY2, BYTE bColor, DWORD dwUnused);
void __fastcall GdiDrawDiamond(const RECT *prc, BYTE bColor);
void __fastcall GdiDrawFrame(const RECT *prc, BYTE bColor);
void __fastcall GdiFillRect(RECT *prc, BYTE bColor);
void __fastcall GdiFillSquare(const POINT *ptCenter, BYTE bSize, BYTE bColor);
void __fastcall GdiClearBackBuffer(BOOL fAbovePanel);

struct GDISMACKVIDEO;
DWORD __fastcall GdiDrawTile(D2GDITILE *ptTile, DWORD dwVertices, int nX, DWORD dwY, DWORD dwTileX,
	DWORD dwTileY, DWORD dwAlpha, int nBoundaryMode, DWORD dwFilter);
void __fastcall GdiSmackOpen(const char *pszFileName, GDISMACKVIDEO *ptVideo);
void __fastcall GdiSmackUpdate(GDISMACKVIDEO *ptVideo);
void __fastcall GdiSmackClose(GDISMACKVIDEO *ptVideo);
BYTE * __fastcall GdiGetEnvValue(int nIndex, DWORD *pdwKind);
void __fastcall GdiSmackInit(BOOL fWide);
BOOL __fastcall GdiDrawCelSkip(D2CELCONTEXT *ptCelContext, int nX, int nY, DWORD dwColor, int nMode,
	DWORD dwUnused, const BYTE *pbPreMap);
BOOL __fastcall GdiDrawCel(D2CELCONTEXT *ptCelContext, int nX, int nY, DWORD dwColor, int nMode,
	const BYTE *pbPreMap);
BOOL __fastcall GdiDrawCelPreMap(D2CELCONTEXT *ptCelContext, int nX, int nY, DWORD dwColor, int nMode,
	int nPreMap);

void __fastcall GdiSpriteDrawRows(D2CELCONTEXT *ptCelContext, int nX, int nY, int nFromBottom, int nRows,
	int nMode);
void __fastcall GdiSpriteDrawShadow(D2CELCONTEXT *ptCelContext, int nX, int nY);
void WINAPI GdiSpriteStub(DWORD dwY, DWORD dwColor);
void __fastcall GdiSpriteWarp(int nX, int nY);
BOOL __fastcall GdiSpriteDrawUnitBlocks(D2GDITILE *ptTile, int nX, int nY, DWORD dwUnused,
	int nBoundaryMode);
BOOL __fastcall GdiSpriteDrawWallShaded(D2GDITILE *ptTile, int nX, int nY, const BYTE *pbShadeTable,
	int nBoundaryMode, DWORD dwAlpha);
BOOL __fastcall GdiSpriteDrawWallLow(D2GDITILE *ptTile, int nX, int nY, const BYTE *pbShadeTable,
	int nBoundaryMode);

#endif
