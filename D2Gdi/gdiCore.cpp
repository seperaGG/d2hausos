#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\gdiCore.cpp"
#include "gdiCore.h"
#include "StormImports.h"
#include "D2GdiImports.h"
#include "D2Macros.h"

char gszRegistrationBlock2[128] = "REGISTRATION_BLOCK";
int gnDisplaySizes[4] = { 640, 800, 480, 600 };

GDIENVINFO *gptEnvInfo = NULL;
HWND ghWnd = NULL;
static HPALETTE sghPalette = NULL;
static GDIENVINFO sgtEnvInfo = { 0 };
GDIDRAWCTX *gptDrawCtx = NULL;
int gnBufferWidth = 0;
HBITMAP ghBitmap = NULL;
PALETTEENTRY gatPaletteEntries[256] = { 0 };
GDIBLITTABLE *gptBlitTable = NULL;
int gnBufferHeight = 0;
RGBQUAD gargbColorTable[256] = { 0 };
BYTE *gpbOffscreenBuffer = NULL;
DWORD gfSuspended = FALSE;
static CRITICAL_SECTION *pcsPause = NULL;

BOOL WINAPI GdiAlwaysTrue(void);
BOOL __fastcall GdiSetup(GDIDRAWCTX *ptDrawCtx, GDIBLITTABLE *ptBlitTable);
BOOL __fastcall GdiTeardown(void);
static void __fastcall GdiCreateBackBuffer(BOOL fWide);
BOOL __fastcall GdiInit(HWND hWnd, BOOL fWide);
BOOL __fastcall GdiDestroyBackBuffer(void);
void WINAPI GdiSetSuspended(DWORD dwValue);
BOOL WINAPI GdiEnterAndClear(void);
BOOL WINAPI GdiLeavePause(void);
BOOL __fastcall GdiPresent(void);
BOOL __fastcall GdiExpandToRgb(BYTE *pbDest);
BOOL WINAPI GdiOpenCutscene(void);
void WINAPI GdiNothingPlayCutscene(DWORD dwValue);
BOOL WINAPI GdiIsCutscene(void);
void __fastcall GdiGetScreenSize(int *pnWidth, int *pnHeight);
void WINAPI GdiNothingSetUnreadDword(void);
void WINAPI GdiNothingSetGamma(void);
BOOL WINAPI GdiHasGamma(void);
void WINAPI GdiNothingSetPerspOrigin(void);
void WINAPI GdiNothingPerspProject(DWORD dwValue1, DWORD dwValue2, DWORD dwValue3);
void WINAPI GdiNothingPerspProjectDepth(DWORD dwValue1, DWORD dwValue2, DWORD dwValue3, DWORD dwValue4);
UINT __fastcall GdiSetPaletteEntries(PALETTEENTRY *ptEntries);
void __fastcall GdiSetEnvInfo(const GDIENVINFO *ptEnvInfo);
void WINAPI GdiNothingSetVertexColor(DWORD dwValue);
void WINAPI GdiNothingFlushCaches(void);

BOOL WINAPI GdiAlwaysTrue(void)
{
	return TRUE;
}

BOOL __fastcall GdiSetup(GDIDRAWCTX *ptDrawCtx, GDIBLITTABLE *ptBlitTable)
{
	gptDrawCtx = ptDrawCtx;
	gptBlitTable = ptBlitTable;
#line 85
	pcsPause = (CRITICAL_SECTION *)D2ALLOC2(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(pcsPause);
	return TRUE;
}

BOOL __fastcall GdiTeardown(void)
{
	if (pcsPause)
	{
		DeleteCriticalSection(pcsPause);
#line 99
		D2FREE2(pcsPause);
		pcsPause = NULL;
	}
	return TRUE;
}

void WINAPI GdiNothingFlushCaches(void)
{
}

BOOL __fastcall GdiInit(HWND hWnd, BOOL fWide)
{
	struct { WORD palVersion; WORD palNumEntries; PALETTEENTRY palPalEntry[256]; } tLogPal;

	ghWnd = hWnd;
	GdiCreateBackBuffer(fWide);
	memset(gatPaletteEntries, 0, sizeof(gatPaletteEntries));
	tLogPal.palVersion = 768;
	tLogPal.palNumEntries = 256;
	memset(tLogPal.palPalEntry, 0, sizeof(tLogPal.palPalEntry));
	sghPalette = CreatePalette((LOGPALETTE *)&tLogPal);
	GdiCreateFont();
	GdiSmackInit(fWide);
	return TRUE;
}

static void __fastcall GdiCreateBackBuffer(BOOL fWide)
{
	struct { BITMAPINFOHEADER bmiHeader; RGBQUAD bmiColors[256]; } tBmi;
	HDC hDC;

#line 120
	D2ASSERT(gpbOffscreenBuffer == NULL);
	if (!fWide)
	{
		gnBufferWidth = 640;
		gnBufferHeight = 480;
		FogErrorManagerLog("Opening GDI window at 640x480...");
	}
	else
	{
		gnBufferWidth = 800;
		gnBufferHeight = 600;
		FogErrorManagerLog("Opening GDI window at 800x600...");
	}
	memset(&tBmi, 0, sizeof(tBmi));
	tBmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	tBmi.bmiHeader.biWidth = gnBufferWidth;
	tBmi.bmiHeader.biHeight = -gnBufferHeight;
	tBmi.bmiHeader.biPlanes = 1;
	tBmi.bmiHeader.biBitCount = 8;
	tBmi.bmiHeader.biCompression = BI_RGB;
	hDC = GetDC(NULL);
	ghBitmap = CreateDIBSection(hDC, (BITMAPINFO *)&tBmi, DIB_RGB_COLORS, (void **)&gpbOffscreenBuffer, NULL,
		0);
	ReleaseDC(NULL, hDC);
	gptBlitTable->pfnSetSurface(gpbOffscreenBuffer, gnBufferWidth, gnBufferHeight, 0);
}

BOOL WINAPI GdiOpenCutscene(void)
{
	return FALSE;
}

void WINAPI GdiNothingPlayCutscene(DWORD dwValue)
{
}

BOOL WINAPI GdiIsCutscene(void)
{
	return FALSE;
}

BOOL __fastcall GdiDestroyBackBuffer(void)
{
	GdiDeleteFont();
	DeleteObject(ghBitmap);
	gpbOffscreenBuffer = NULL;
	gptBlitTable->pfnSetSurface(NULL, 0, 0, 0);
	return TRUE;
}

void WINAPI GdiSetSuspended(DWORD dwValue)
{
#line 235
	D2ASSERT(pcsPause);
	EnterCriticalSection(pcsPause);
	gfSuspended = dwValue;
	LeaveCriticalSection(pcsPause);
}

BOOL WINAPI GdiEnterAndClear(void)
{
	if (gfSuspended)
		return FALSE;
#line 255
	D2ASSERT(pcsPause);
	EnterCriticalSection(pcsPause);
	GdiClearBackBuffer(TRUE);
	return TRUE;
}

BOOL WINAPI GdiLeavePause(void)
{
#line 269
	D2ASSERT(pcsPause);
	LeaveCriticalSection(pcsPause);
	return TRUE;
}

BOOL __fastcall GdiPresent(void)
{
	HDC hWndDC;
	HDC hMemDC;

	hWndDC = GetDC(ghWnd);
	hMemDC = CreateCompatibleDC(hWndDC);
	SelectObject(hMemDC, ghBitmap);
	SetDIBColorTable(hMemDC, 0, 256, gargbColorTable);
	SelectPalette(hWndDC, sghPalette, 0);
	RealizePalette(hWndDC);
	BitBlt(hWndDC, 0, 0, gnBufferWidth, gnBufferHeight, hMemDC, 0, 0, SRCCOPY);
	GdiFlush();
	DeleteDC(hMemDC);
	ReleaseDC(ghWnd, hWndDC);
	return TRUE;
}

BOOL __fastcall GdiExpandToRgb(BYTE *pbDest)
{
	BYTE *pbSrcRow;
	BYTE *pbSrc;
	BYTE *pbOut;
	int x;
	int y;
	RGBQUAD *ptColor;

	pbSrcRow = gpbOffscreenBuffer;
	for (y = 0; y < gnBufferHeight; y++)
	{
		pbOut = pbDest;
		pbSrc = pbSrcRow;
		for (x = 0; x < gnBufferWidth; x++)
		{
			ptColor = &gargbColorTable[*pbSrc++];
			*pbOut++ = ptColor->rgbRed;
			*pbOut++ = ptColor->rgbGreen;
			*pbOut++ = ptColor->rgbBlue;
		}
		pbSrcRow += gnBufferWidth;
		pbDest += gnBufferWidth * 3;
	}
	return TRUE;
}

void __fastcall GdiGetScreenSize(int *pnWidth, int *pnHeight)
{
	*pnWidth = gnBufferWidth;
	*pnHeight = gnBufferHeight;
}

UINT __fastcall GdiSetPaletteEntries(PALETTEENTRY *ptEntries)
{
	int i;
	UINT nResult;

	for (i = 0; i < 256; i++)
	{
		gargbColorTable[i].rgbRed = ptEntries[i].peRed;
		gargbColorTable[i].rgbGreen = ptEntries[i].peGreen;
		gargbColorTable[i].rgbBlue = ptEntries[i].peBlue;
	}
	nResult = SetPaletteEntries(sghPalette, 0, 256, ptEntries);
	memcpy(gatPaletteEntries, ptEntries, sizeof(gatPaletteEntries));
	return nResult;
}

void __fastcall GdiSetEnvInfo(const GDIENVINFO *ptEnvInfo)
{
	sgtEnvInfo = *ptEnvInfo;
	gptEnvInfo = &sgtEnvInfo;
}

HBITMAP WINAPI GdiGetBitmapHandle(void)
{
	return ghBitmap;
}

void WINAPI GdiNothingSetUnreadDword(void)
{
}

void WINAPI GdiNothingSetGamma(void)
{
}

BOOL WINAPI GdiHasGamma(void)
{
	return FALSE;
}

void WINAPI GdiNothingSetVertexColor(DWORD dwValue)
{
}

void WINAPI GdiNothingSetPerspOrigin(void)
{
}

void WINAPI GdiNothingPerspProject(DWORD dwValue1, DWORD dwValue2, DWORD dwValue3)
{
}

void WINAPI GdiNothingPerspProjectDepth(DWORD dwValue1, DWORD dwValue2, DWORD dwValue3, DWORD dwValue4)
{
}

void WINAPI GdiNothingResetPerspDepth(void)
{
}

BOOL __fastcall GdiReinit(DWORD dwUnused, BOOL fWide)
{
	DeleteObject(ghBitmap);
	gpbOffscreenBuffer = NULL;
	gptBlitTable->pfnSetSurface(NULL, 0, 0, 0);
	GdiCreateBackBuffer(fWide);
	GdiSmackInit(fWide);
	return TRUE;
}

BOOL WINAPI GdiAlwaysFalse(void)
{
	return FALSE;
}

BOOL WINAPI GdiGetFrameOpenQuery(void)
{
	return FALSE;
}
