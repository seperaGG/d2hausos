#define D2_THIS_FILE "C:\\D2\\Source\\D2Gfx\\SRC\\Window.cpp"
#include <stdio.h>
#include <stdlib.h>
#include "D2gfx.h"
#include "D2Macros.h"
#include "D2gfxMisc.h"
#include "StormImports.h"
#include <shellapi.h>

struct D2GFXAPPBAR
{
 	HWND		hWnd;
 	WINDOWPLACEMENT	tPlacement;
};

#define WINDOW_APPBARS		4

static D2GFXAPPBAR sgatAppBars[WINDOW_APPBARS] = { 0 };
static int sgnFrameHeight = 0;
static char sgszMessage[256] = { 0 };
BOOL gfCriticalError = 0;
static BOOL sgfMinimizeRequested = 0;
static BOOL sgfMinimized = 0;
static BOOL sgfCursorShown = TRUE;

static char sgszWindowClass[] = "Diablo II";
static char sgszWindowTitle[] = "Diablo II";
static inline void WindowGetResolutionRect(int nResolution, RECT *ptRect)
{
	ptRect->left = 0;
	ptRect->top = 0;
	switch (nResolution)
	{
	case 0:
		ptRect->right = 640;
		ptRect->bottom = 480;
		break;
	case 1:
		ptRect->right = 800;
		ptRect->bottom = 600;
		break;
	default:
		ptRect->right = 0;
		ptRect->bottom = 0;
#line 88
		D2FATAL((const char *)(INT_PTR)FogDataArraysSprintf(sgszMessage, "Unknown resolution %d", nResolution));
	}
}

BOOL __stdcall WindowCreate(DWORD dwWindowed, int nResolution)
{
	RECT tRect;
	RECT tDesktop;
	DWORD dwStyle;
	int nX;
	int nY;
	int nWidth;
	int nHeight;
	BOOL fResult;

	if (FindWindowA(sgszWindowClass, NULL))
	{
		char szCaption[256];
		char szText[256];

		sprintf(szCaption, "Diablo II Critical Error");
		sprintf(szText, "Only one copy of Diablo II may run at a time.");
		MessageBoxA(NULL, szText, szCaption, MB_ICONEXCLAMATION);
		return FALSE;
	}
#line 116
	D2ASSERT(gpfnGraphics);
#line 117
	D2ASSERT(!gtGfxSystem.hWnd);
	gtGfxSystem.dwWindowed = dwWindowed;
	gtGfxSystem.nResolution = nResolution;
	WindowGetResolutionRect(nResolution, &tRect);
	sgnFrameHeight = tRect.bottom;
	if (dwWindowed == 1)
		dwStyle = WS_CAPTION;
	else
	{
		dwStyle = WS_POPUP | WS_SYSMENU;
		if (gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL)
			dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;
	}
	AdjustWindowRectEx(&tRect, dwStyle, FALSE, WS_EX_APPWINDOW);
	nWidth = tRect.right - tRect.left;
	nHeight = tRect.bottom - tRect.top;
	sgnFrameHeight = nHeight - sgnFrameHeight;
	nX = 0;
	nY = 0;
	if (gtGfxSystem.dwWindowed == 1)
	{
		GetClientRect(GetDesktopWindow(), &tDesktop);
		nX = (tDesktop.right - nWidth) >> 1;
		if (nX < 0)
			nX = 0;
		nY = (tDesktop.bottom - nHeight) >> 1;
		if (nY < 0)
			nY = 0;
	}
	else if (gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE)
	{
		nX = 0;
		nY = 0;
		nWidth = GetSystemMetrics(SM_CXSCREEN);
		nHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	gtGfxSystem.hWnd = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_TOPMOST, sgszWindowClass, sgszWindowTitle, dwStyle, nX,
		nY, nWidth, nHeight, NULL, NULL, gtGfxSystem.hInstance, NULL);
	if (!gtGfxSystem.hWnd)
	{
		if (FaultErrorBox(GetLastError()))
		{
			gfInDebugBreak = TRUE;
			__asm int 3;
			gfInDebugBreak = FALSE;
		}
		return FALSE;
	}
	ShowWindow(gtGfxSystem.hWnd, SW_SHOWNORMAL);
	UpdateWindow(gtGfxSystem.hWnd);
	SetFocus(gtGfxSystem.hWnd);
	GdiSetBatchLimit(1);
	if (sgfCursorShown)
	{
		while (ShowCursor(FALSE) >= 0)
			;
		sgfCursorShown = FALSE;
	}
	fResult = gpfnGraphics->field_0c(gtGfxSystem.hWnd, gtGfxSystem.nResolution);
	if (!fResult)
	{
		gfCriticalError = TRUE;
		if (!sgfCursorShown)
		{
			while (ShowCursor(TRUE) < 0)
				;
			sgfCursorShown = TRUE;
		}
		if (gtGfxSystem.hWnd)
		{
			DestroyWindow(gtGfxSystem.hWnd);
			gtGfxSystem.hWnd = NULL;
		}
	}
	{
		DWORD dwGamma;

		dwGamma = 100;
		StormSRegLoadValue("Diablo II", "Gamma", 0, &dwGamma);
		gtGfxOptions.dwGamma = dwGamma;
		D2GfxSetGamma(dwGamma);
	}
	SetWindowPos(gtGfxSystem.hWnd, HWND_NOTOPMOST, 0, 0, nWidth, nHeight, SWP_NOMOVE);
	return fResult;
}

BOOL __fastcall WindowDestroy(void)
{
	BOOL fResult;

	fResult = TRUE;
#line 256
	D2ASSERT(gpfnGraphics);
	if (!sgfCursorShown)
	{
		while (ShowCursor(TRUE) < 0)
			;
		sgfCursorShown = TRUE;
	}
	if (!gpfnGraphics->field_10())
		fResult = FALSE;
	if (gtGfxSystem.hWnd)
	{
		if (!DestroyWindow(gtGfxSystem.hWnd))
			fResult = FALSE;
		gtGfxSystem.hWnd = NULL;
	}
	return fResult;
}

void __stdcall WindowRequestMinimize(BOOL fMinimize)
{
	sgfMinimizeRequested = fMinimize;
}

BOOL __cdecl WindowUpdateMinimized(void)
{
	if (sgfMinimized != sgfMinimizeRequested && gtGfxSystem.hWnd)
	{
#line 307
		D2ASSERT(gpfnGraphics);
		if (sgfMinimizeRequested || !IsIconic(gtGfxSystem.hWnd))
		{
			sgfMinimized = sgfMinimizeRequested;
			if (sgfMinimized)
			{
				ShowWindow(gtGfxSystem.hWnd, SW_MINIMIZE);
				WindowShowAppBars();
				while (ShowCursor(TRUE) < 0)
					;
			}
			else
			{
				WindowHideAppBars();
				while (ShowCursor(FALSE) >= 0)
					;
			}
			gpfnGraphics->field_14(gtGfxSystem.hWnd, gtGfxSystem.nResolution, sgfMinimized);
			if (!sgfMinimized)
			{
				ScaleSetPalette(NULL);
				D2GfxSetGamma(gtGfxOptions.dwGamma);
			}
		}
	}
	return sgfMinimized;
}

HWND __fastcall WindowGetHandle(void)
{
	return gtGfxSystem.hWnd;
}

DWORD __stdcall WindowGetScreenSize(DWORD dwWidth, DWORD dwHeight)
{
#line 367
	D2ASSERT(gpfnGraphics);
	return gpfnGraphics->field_50(dwWidth, dwHeight);
}

void __stdcall WindowHideCursor(BOOL fHide)
{
	if (!gtGfxSystem.dwWindowed)
		return;
	if (gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE || gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL
		|| gtGfxSystem.eDispType == DISPLAYTYPE_DIRECT3D)
		return;
	if (fHide)
	{
		if (sgfCursorShown)
		{
			while (ShowCursor(FALSE) >= 0)
				;
			sgfCursorShown = FALSE;
		}
	}
	else if (!sgfCursorShown)
	{
		while (ShowCursor(TRUE) < 0)
			;
		sgfCursorShown = TRUE;
	}
}

BOOL __fastcall WindowTestFullScreen(void)
{
	if (!gpfnGraphics)
		return FALSE;
	return gtGfxSystem.dwWindowed == 0;
}

DWORD __stdcall WindowSetFullScreen(BOOL fFullScreen)
{
#line 415
	D2ASSERT(gpfnGraphics);
	gtGfxSystem.dwWindowed = fFullScreen == FALSE;
	return gpfnGraphics->field_2c(fFullScreen, gtGfxSystem.dwWindowed);
}

DWORD __stdcall WindowSetResolution(int nResolution, BOOL fForce)
{
	RECT tRect;
	RECT tDesktop;
	int nWidth;
	int nHeight;
	DWORD dwGamma;
	DWORD dwResult;

#line 429
	D2ASSERT(gpfnGraphics);
	if (!fForce && nResolution == gtGfxSystem.nResolution)
		return TRUE;
	gtGfxSystem.nResolution = nResolution;
	if (gtGfxSystem.dwWindowed == 1)
	{
		WindowGetResolutionRect(nResolution, &tRect);
		sgnFrameHeight = tRect.bottom;
		AdjustWindowRectEx(&tRect, WS_CAPTION, FALSE, WS_EX_APPWINDOW);
		nWidth = tRect.right - tRect.left;
		nHeight = tRect.bottom - tRect.top;
		sgnFrameHeight = nHeight - sgnFrameHeight;
		GetClientRect(GetDesktopWindow(), &tDesktop);
		SetWindowPos(gtGfxSystem.hWnd, HWND_NOTOPMOST, 0, 0, nWidth, nHeight,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	dwResult = gpfnGraphics->field_24(gtGfxSystem.hWnd, nResolution);
	dwGamma = 100;
	StormSRegLoadValue("Diablo II", "Gamma", 0, &dwGamma);
	gtGfxOptions.dwGamma = dwGamma;
	D2GfxSetGamma(dwGamma);
	return dwResult;
}

DWORD __fastcall D2GfxNukeCaches(void)
{
	return gpfnGraphics->field_d0();
}

DWORD __fastcall D2GfxCutsceneCreateWindow(void)
{
#line 558
	D2ASSERT(gpfnGraphics);
	if (gtGfxSystem.eDispType != DISPLAYTYPE_GDI && gtGfxSystem.eDispType != DISPLAYTYPE_DDRAW
		&& gtGfxSystem.eDispType != DISPLAYTYPE_DIRECT3D)
		WindowSetResolution(0, FALSE);
	if (gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE)
		D2GfxSetGamma(75);
	return gpfnGraphics->field_34();
}

DWORD __stdcall D2GfxCutsceneCloseWindow(int nResolution)
{
#line 582
	D2ASSERT(gpfnGraphics);
	if (!WindowSetResolution(nResolution, TRUE))
	{
		FogErrorManagerLog("Failed to resize window after cutscenes... this is fatal!");
		exit(-1);
	}
	return ScaleSetPalette(NULL);
}

DWORD __stdcall WindowPlayCutscene(DWORD dwFileName, DWORD dwMode, DWORD dwTickProc)
{
#line 604
	D2ASSERT(gpfnGraphics);
	return gpfnGraphics->field_38(dwFileName, dwMode, dwTickProc);
}

DWORD __fastcall WindowIsCutscenePlaying(void)
{
#line 616
	D2ASSERT(gpfnGraphics);
	return gpfnGraphics->field_3c();
}

DWORD __stdcall WindowOpenSmack(DWORD dwFileName, DWORD dwVideo)
{
#line 627
	D2ASSERT(gpfnGraphics);
	return gpfnGraphics->field_40(dwFileName, dwVideo);
}

DWORD __stdcall WindowDrawSmackFrame(DWORD dwArg)
{
#line 639
	D2ASSERT(gpfnGraphics);
	return gpfnGraphics->field_44(dwArg);
}

DWORD __stdcall WindowCloseSmack(DWORD dwArg)
{
#line 651
	D2ASSERT(gpfnGraphics);
	return gpfnGraphics->field_48(dwArg);
}

void __fastcall WindowHideAppBars(void)
{
	APPBARDATA tData;
	int i;

	if (gtGfxSystem.dwWindowed == 1)
		return;
	for (i = 0; i < (int)(sizeof(tData) / sizeof(DWORD)); i++)
		((DWORD *)&tData)[i] = 0;
	tData.cbSize = sizeof(tData);
	tData.hWnd = gtGfxSystem.hWnd;
	for (i = 0; i < WINDOW_APPBARS; i++)
	{
		tData.uEdge = i;
		sgatAppBars[i].hWnd = (HWND)SHAppBarMessage(ABM_GETAUTOHIDEBAR, &tData);
		if (sgatAppBars[i].hWnd)
		{
			sgatAppBars[i].tPlacement.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(sgatAppBars[i].hWnd, &sgatAppBars[i].tPlacement);
			ShowWindow(sgatAppBars[i].hWnd, SW_HIDE);
		}
	}
}

void __fastcall WindowShowAppBars(void)
{
	int i;

	if (gtGfxSystem.dwWindowed == 1)
		return;
	for (i = 0; i < WINDOW_APPBARS; i++)
	{
		if (sgatAppBars[i].hWnd)
			ShowWindow(sgatAppBars[i].hWnd, sgatAppBars[i].tPlacement.showCmd);
	}
}
