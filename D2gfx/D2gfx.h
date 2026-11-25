#ifndef D2RE_D2GFX_D2GFX_H
#define D2RE_D2GFX_D2GFX_H

#include <windows.h>
#include "D2gfxLinkNames.h"
#include <stdarg.h>
#include "FogImports.h"

extern "C" DWORD __cdecl FogOrdinal10025(...);
#define FogErrorManagerReport FogOrdinal10025

enum EDISPLAYTYPE
{
	DISPLAYTYPE_GDI = 1,
	DISPLAYTYPE_DDRAW = 3,
	DISPLAYTYPE_GLIDE = 4,
	DISPLAYTYPE_OPENGL = 5,
	DISPLAYTYPE_DIRECT3D = 6,
	DISPLAYTYPE_RAVE = 7
};

struct D2GFXOPTIONS
{
 	BOOL		fPerspective;
 	BOOL		fPerspectiveAllowed;
 	BOOL		fFlatLighting;
 	DWORD		dwGamma;
 	BOOL		fVSync;
 	BOOL		fBlendShadow;
};
struct D2GFXENVINFO
{
 	BYTE		*pbBlendMode2;
 	BYTE		*pbBlendMode1;
 	BYTE		*pbBlendMode0;
 	BYTE		*apbColorMaps[49];
 	BYTE		unobserved_0d0[288 - 208];
};

struct D2GFXTILEPART
{
 	short		sOffsetX;
 	short		sOffsetY;
 	BYTE		unobserved_04[2];
 	BYTE		bColumn;
 	BYTE		bRow;
 	WORD		wFlags;
 	BYTE		unobserved_0a[6];
 	BYTE		*ptData;
};

struct D2GFXTILE
{
 	BYTE		unobserved_00[80];
 	int			nCompCount;
 	D2GFXTILEPART *ptCompArray;
};

struct D2GFXLIGHT
{
 	int			nLight;
 	BYTE		unobserved_04[12];
};

struct D2GFXBLITTABLE
{
 	void		(__fastcall *field_00)(BYTE *pbBuffer, int nPitch, DWORD dwHeight, DWORD dwArg2);
 	void		(__fastcall *field_04)(BYTE *pbData, int nX, int nY);
 	void		(__fastcall *field_08)(BYTE *pbData, int nX, int nY, BYTE bLight);
 	void		(__fastcall *field_0c)(D2GFXTILE *ptTile, int nX, int nY,
					D2GFXLIGHT (*ptLight)[8]);
 	void		(__fastcall *field_10)(BYTE *pbData, int nX, int nY, int *pnLight);
 	void		(__fastcall *field_14)(BYTE *pbData, int nX, int nY, BYTE bAlpha);
 	void		(__fastcall *field_18)(BYTE *pbData, int nX, int nY, int *pnLight, BYTE bAlpha);
};

struct D2GFXRENDERER
{
 	BOOL		(__fastcall *field_00)(HINSTANCE hInstance);
 	DWORD		(__fastcall *field_04)(D2GFXOPTIONS *ptOptions, D2GFXBLITTABLE *ptBlitTable);
 	void		(__stdcall *field_08)(void);
 	BOOL		(__fastcall *field_0c)(HWND hWnd, int nResolution);
 	BOOL		(__stdcall *field_10)(void);
 	void		(__fastcall *field_14)(HWND hWnd, int nResolution, BOOL fMinimized);
 	DWORD		(__stdcall *field_18)(void);
 	void		(__stdcall *field_1c)(void);
 	void		(__stdcall *field_20)(void);
 	DWORD		(__fastcall *field_24)(HWND hWnd, int nResolution);
 	DWORD		(__fastcall *field_28)(DWORD dwArg);
 	DWORD		(__fastcall *field_2c)(DWORD dwFullScreen, BOOL fWindowed);
 	void		(__fastcall *field_30)(DWORD dwQuery, BOOL fArg2);
 	DWORD		(__stdcall *field_34)(void);
 	DWORD		(__fastcall *field_38)(DWORD dwFileName, DWORD dwMode, DWORD dwTickProc);
 	DWORD		(__stdcall *field_3c)(void);
 	DWORD		(__fastcall *field_40)(DWORD dwFileName, DWORD dwVideo);
 	DWORD		(__fastcall *field_44)(DWORD dwArg);
 	DWORD		(__fastcall *field_48)(DWORD dwArg);
 	DWORD		(__stdcall *field_4c)(void);
 	DWORD		(__fastcall *field_50)(DWORD dwWidthOut, DWORD dwHeightOut);
 	void		(__fastcall *field_54)(int nScale);
 	void		(__fastcall *field_58)(DWORD dwGamma);
 	DWORD		(__stdcall *field_5c)(void);
 	void		(__fastcall *field_60)(int nX, int nY);
 	void		(__fastcall *field_64)(DWORD dwX, DWORD dwY, DWORD dwZ, DWORD dwScreenXOut,
					DWORD dwScreenYOut);
 	void		(__fastcall *field_68)(DWORD dwX, DWORD dwY, DWORD dwZ, DWORD dwScreenXOut,
					DWORD dwScreenYOut, DWORD dwFar);
 	DWORD		(__stdcall *field_6c)(void);
 	DWORD		(__fastcall *field_70)(PALETTEENTRY *ptEntries);
 	void		(__fastcall *field_74)(D2GFXENVINFO *ptEnvInfo);
 	void		(__fastcall *field_78)(DWORD dwRed, DWORD dwGreen, DWORD dwBlue);
 	void		(__fastcall *field_7c)(DWORD dwTile, DWORD dwVertices, DWORD dwX, DWORD dwY,
					DWORD dwTileX, DWORD dwTileY, DWORD dwAlpha, DWORD dwBoundaryMode, DWORD dwFilter);
 	void		(__fastcall *field_80)(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor,
					DWORD dwMode, DWORD dwBoundary, DWORD dwPreMap);
 	DWORD		(__fastcall *field_84)(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor,
					DWORD dwMode, DWORD dwPreMap);
 	void		(__fastcall *field_88)(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor,
					DWORD dwMode, DWORD dwPaletteIndex);
 	void		(__fastcall *field_8c)(DWORD dwCelContext, DWORD dwX, DWORD dwY,
					DWORD dwFromBottom, DWORD dwRows, DWORD dwMode);
 	void		(__fastcall *field_90)(DWORD dwCelContext, DWORD dwX, DWORD dwY);
 	DWORD		(__fastcall *field_94)(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor);
 	DWORD		(__fastcall *field_98)(DWORD dwTile, DWORD dwX, DWORD dwY, DWORD dwLight,
					DWORD dwBoundaryMode);
 	DWORD		(__fastcall *field_9c)(DWORD dwTile, DWORD dwX, DWORD dwY, DWORD dwLight,
					DWORD dwBoundaryMode, DWORD dwAlpha);
 	DWORD		(__fastcall *field_a0)(DWORD dwTile, DWORD dwX, DWORD dwY, DWORD dwLight,
					DWORD dwBoundaryMode);
 	void		(__fastcall *field_a4)(DWORD dwRect, BYTE bColor);
 	void		(__fastcall *field_a8)(DWORD dwRect, BYTE bColor);
 	void		(__fastcall *field_ac)(DWORD dwRect, BYTE bColor);
 	void		(__fastcall *field_b0)(DWORD dwCenter, BYTE bSize, DWORD dwColor);
 	void		(__fastcall *field_b4)(DWORD dwX1, DWORD dwY1, DWORD dwX2, DWORD dwY2,
					DWORD dwColor, DWORD dwMode);
 	DWORD		(__fastcall *field_b8)(DWORD dwX1, DWORD dwY1, DWORD dwX2, DWORD dwY2,
					DWORD dwColor, DWORD dwAlpha);
 	void		(__fastcall *field_bc)(DWORD dwX1, DWORD dwY1, DWORD dwX2, DWORD dwY2,
					DWORD dwColor, DWORD dwAlpha);
 	void		(__fastcall *field_c0)(DWORD dwArg);
 	void		(__fastcall *field_c4)(DWORD dwX, DWORD dwY, const char *pszFormat,
					va_list pArgs);
 	DWORD		(__fastcall *field_c8)(DWORD dwPalette, DWORD dwGrid, DWORD dwLightX,
					DWORD dwLightY);
 	DWORD		(__fastcall *field_cc)(DWORD dwX, DWORD dwY);
 	DWORD		(__stdcall *field_d0)(void);
};

struct D2GFXSYSTEM
{
 	HINSTANCE	hInstance;
 	WNDPROC		pfnWndProc;
 	int			eDispType;
 	DWORD		dwWindowed;
 	int			nResolution;
 	HWND		hWnd;
};

extern D2GFXOPTIONS gtGfxOptions;
extern DWORD gdwBlitParam;
extern D2GFXRENDERER *gpfnGraphics;
extern D2GFXSYSTEM gtGfxSystem;
extern BOOL gfCriticalError;

void __fastcall CmnSubtileInit(void);
void __fastcall CmnSubtileSetEnv(D2GFXENVINFO *ptEnvInfo);
void __fastcall CmnSubtileSetBuffer(BYTE *pbBuffer, int nPitch, DWORD dwHeight, DWORD dwArg2);
void __fastcall CmnSubtileBlitBlock(BYTE *pbData, int nX, int nY);
void __fastcall CmnSubtileBlitBlockLit(BYTE *pbData, int nX, int nY, BYTE bLight);
void __fastcall CmnSubtileDrawTile(D2GFXTILE *ptTile, int nX, int nY, D2GFXLIGHT (*ptLight)[8]);
void __fastcall CmnSubtileBlitBlockGouraud(BYTE *pbData, int nX, int nY, int *pnLight);
void __fastcall CmnSubtileBlitBlockAlpha(BYTE *pbData, int nX, int nY, BYTE bAlpha);
void __fastcall CmnSubtileBlitBlockGouraudAlpha(BYTE *pbData, int nX, int nY, int *pnLight, BYTE bAlpha);

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
void __stdcall D2GfxSetGamma(DWORD dwGamma);

DWORD __stdcall ScaleSetPalette(PALETTEENTRY *ptEntries);
void __stdcall ScaleSetEnvInfo(D2GFXENVINFO *ptEnvInfo);

void __fastcall WindowHideAppBars(void);
void __fastcall WindowShowAppBars(void);

#endif
