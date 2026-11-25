#define D2_THIS_FILE "C:\\D2\\Source\\D2Gfx\\Src\\D2gfx.cpp"
#include <stdio.h>
#include "D2gfx.h"
#include "D2Macros.h"
#include "D2gfxMisc.h"

D2GFXOPTIONS gtGfxOptions = { FALSE, TRUE, FALSE, 100, TRUE, TRUE };

typedef D2GFXRENDERER * (__stdcall *PFNGFXGETRENDERER)(void);

DWORD gdwBlitParam;
D2GFXRENDERER *gpfnGraphics;
D2GFXSYSTEM gtGfxSystem;

static D2GFXBLITTABLE sgtBlitTable =
{
	CmnSubtileSetBuffer, CmnSubtileBlitBlock, CmnSubtileBlitBlockLit, CmnSubtileDrawTile,
	CmnSubtileBlitBlockGouraud, CmnSubtileBlitBlockAlpha, CmnSubtileBlitBlockGouraudAlpha,
};

static const char *sgcpszRendererDlls[] =
{
	NULL, "D2Gdi.dll", NULL, "D2DDraw.dll", "D2Glide.dll", NULL, "D2Direct3D.dll", NULL,
};

static char sgszWindowClass[] = "Diablo II";
static char sgszError1[] =
	"Error 1: Diablo II is unable to proceed.  Please run D2VidTst and try again.";

static HMODULE ghLib;

static void __cdecl D2GfxAtExit(void);

DWORD __stdcall D2GfxInit(HINSTANCE hInstance, WNDPROC pfnWndProc, int nDisplayType, DWORD dwWindowed)
{
	WNDCLASSA tWndClass;
	char szMessage[64];
	PFNGFXGETRENDERER pfnGetRenderer;
	BOOL fPerspective;

	gtGfxSystem.hInstance = hInstance;
	gtGfxSystem.pfnWndProc = pfnWndProc;
	tWndClass.style = nDisplayType == DISPLAYTYPE_OPENGL ? CS_OWNDC : 0;
	tWndClass.lpfnWndProc = pfnWndProc;
	tWndClass.cbClsExtra = 0;
	tWndClass.cbWndExtra = 0;
	tWndClass.hInstance = hInstance;
	tWndClass.hIcon = (HICON)LoadImageA(hInstance, MAKEINTRESOURCEA(102), IMAGE_ICON, 0, 0, 0);
	tWndClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
	tWndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	tWndClass.lpszMenuName = NULL;
	tWndClass.lpszClassName = sgszWindowClass;
	if (!RegisterClassA(&tWndClass))
	{
		if (FaultErrorBox(GetLastError()))
		{
			gfInDebugBreak = TRUE;
			__asm int 3;
			gfInDebugBreak = FALSE;
		}
		return 0;
	}
	CmnSubtileInit();
	atexit(D2GfxAtExit);
	gtGfxSystem.dwWindowed = dwWindowed;
	gtGfxSystem.eDispType = nDisplayType;
	if (!sgcpszRendererDlls[nDisplayType])
	{
		FogErrorManagerLog("Unsupported video mode - %d", nDisplayType);
		return 0;
	}
	ghLib = LoadLibraryA(sgcpszRendererDlls[nDisplayType]);
	if (!ghLib)
	{
		wsprintfA(szMessage, "Can't find DLL: %s", sgcpszRendererDlls[nDisplayType]);
#line 110
		D2FATAL(szMessage);
	}
	pfnGetRenderer = (PFNGFXGETRENDERER)GetProcAddress(ghLib, MAKEINTRESOURCEA(10000));
	if (!pfnGetRenderer)
	{
		wsprintfA(szMessage, "Error interfacing with Gfx DLL #%i", GetLastError());
#line 118
		D2FATAL(szMessage);
	}
	gpfnGraphics = pfnGetRenderer();
	gpfnGraphics->field_30(8, dwWindowed == 1);
	if (!gpfnGraphics->field_00(hInstance))
	{
		FogErrorManagerReport(9, NULL, 0, "%s",
			sgszError1);
		return 0;
	}
	fPerspective = gtGfxOptions.fPerspectiveAllowed;
	if (gtGfxSystem.eDispType < DISPLAYTYPE_GLIDE)
		fPerspective = FALSE;
	gtGfxOptions.fPerspectiveAllowed = fPerspective;
	return gpfnGraphics->field_04(&gtGfxOptions, &sgtBlitTable);
}

static void __cdecl D2GfxAtExit(void)
{
	WindowShowAppBars();
}

BOOL __stdcall D2GfxReturnFalse(DWORD dwUnused1, DWORD dwUnused2, DWORD dwUnused3)
{
	return FALSE;
}

void __fastcall D2GfxRelease(void)
{
	char szText[256];
	char szCaption[256];

	gpfnGraphics->field_08();
	UnregisterClassA(sgszWindowClass, gtGfxSystem.hInstance);
	if (gfCriticalError)
	{
		sprintf(szCaption, "Diablo II Critical Error");
		switch (gtGfxSystem.eDispType)
		{
		case DISPLAYTYPE_GDI:
			sprintf(szText, "Error 21: A critical error has occurred while initializing windowed mode.");
			break;
		case DISPLAYTYPE_DDRAW:
			sprintf(szText, "Error 22: A critical error has occurred while initializing DirectDraw.");
			break;
		case DISPLAYTYPE_GLIDE:
			sprintf(szText, "Error 23: A critical error has occurred while initializing Glide.");
			break;
		case DISPLAYTYPE_OPENGL:
			sprintf(szText, "Error 24: A critical error has occurred while initializing OpenGL.");
			break;
		case DISPLAYTYPE_DIRECT3D:
			sprintf(szText, "Error 25: A critical error has occurred while initializing Direct3D.");
			break;
		}
		MessageBoxA(NULL, szText, szCaption, MB_ICONEXCLAMATION);
	}
#line 131
	D2ASSERT(ghLib != NULL);
	FreeLibrary(ghLib);
	ghLib = NULL;
	gpfnGraphics = NULL;
}

int __fastcall D2GfxGetDisplayType(void)
{
	return gtGfxSystem.eDispType;
}

DWORD __fastcall D2GfxGetWindowed(void)
{
	return gtGfxSystem.dwWindowed;
}

DWORD __fastcall D2GfxGetResolution(void)
{
	return gtGfxSystem.nResolution;
}

BOOL __fastcall D2GfxGetPerspectiveAllowed(void)
{
	return gtGfxOptions.fPerspectiveAllowed;
}

void __stdcall D2GfxSetPerspectiveAllowed(BOOL fValue)
{
	if (gtGfxSystem.eDispType < DISPLAYTYPE_GLIDE)
		fValue = FALSE;
	gtGfxOptions.fPerspectiveAllowed = fValue;
}

BOOL __fastcall D2GfxTestDisplayType3D(void)
{
	return gtGfxSystem.eDispType >= DISPLAYTYPE_GLIDE;
}

void __fastcall D2GfxTogglePerspective(void)
{
	gtGfxOptions.fPerspective = gtGfxOptions.fPerspective == FALSE;
}

BOOL __fastcall D2GfxGetPerspective(void)
{
	return gtGfxOptions.fPerspectiveAllowed ? gtGfxOptions.fPerspective : FALSE;
}

void __stdcall D2GfxSetPerspective(BOOL fPerspective)
{
	gtGfxOptions.fPerspective = fPerspective;
}

void __fastcall D2GfxToggleBlendShadow(void)
{
	gtGfxOptions.fBlendShadow = gtGfxOptions.fBlendShadow == FALSE;
}

BOOL __fastcall D2GfxGetBlendShadow(void)
{
	return gtGfxOptions.fBlendShadow;
}

void __stdcall D2GfxSetBlendShadow(BOOL fValue)
{
	gtGfxOptions.fBlendShadow = fValue;
}

void __fastcall D2GfxToggleFlatLighting(void)
{
	gtGfxOptions.fFlatLighting = gtGfxOptions.fFlatLighting == FALSE;
}

BOOL __fastcall D2GfxGetFlatLighting(void)
{
	return gtGfxOptions.fFlatLighting;
}

void __stdcall D2GfxSetFlatLighting(BOOL fValue)
{
	gtGfxOptions.fFlatLighting = fValue;
}

void __stdcall D2GfxSetGamma(DWORD dwGamma)
{
	gtGfxOptions.dwGamma = dwGamma;
	gpfnGraphics->field_58(dwGamma);
}

DWORD __fastcall D2GfxHasGammaControl(void)
{
	return gpfnGraphics->field_5c();
}

void __fastcall D2GfxEnableVSync(void)
{
	gtGfxOptions.fVSync = TRUE;
}

DWORD __fastcall D2GfxGetMemoryBudget(void)
{
	return gpfnGraphics->field_4c();
}

void __fastcall D2GfxSetBlitParam(DWORD dwValue)
{
	gdwBlitParam = dwValue;
}

void __fastcall D2GfxQueryRenderer(DWORD dwQuery, BOOL fArg2)
{
	gpfnGraphics->field_30(dwQuery, fArg2);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason) { case DLL_PROCESS_ATTACH:
		gtGfxSystem.hWnd = NULL; }
	return TRUE;
}
