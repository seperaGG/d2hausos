#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinMain.cpp"
#include "D2Win.h"
#include "D2Macros.h"
#include <stdio.h>
#include <time.h>

void __fastcall D2WinFontInit(void);
void __fastcall D2WinFontDestroy(void);
void __fastcall D2WinListLoadPentagram(void);
void __fastcall D2WinListFreePentagram(void);
void __fastcall D2CompLoadPalettes(void);
D2COMPOSITEUNIT * __fastcall D2CompGetFirst(void);
D2COMPOSITEUNIT * __stdcall D2CompGetNext(D2COMPOSITEUNIT *ptUnit);
void __fastcall D2CompUnitDrawUnder(D2COMPOSITEUNIT *hCompositeUnit);
BOOL __fastcall D2CompUnitDraw(D2COMPOSITEUNIT *hCompositeUnit, DWORD dwColor);
BOOL __fastcall D2CompUnitDrawName(D2COMPOSITEUNIT *hCompositeUnit);
BOOL __fastcall D2CompUnitDrawInfo(D2COMPOSITEUNIT *hCompositeUnit);
void __fastcall D2WinRegisterMessages(HWND hWnd, D2WINMSGENTRY *ptTable, DWORD dwCount);
void __fastcall D2WinUnregisterMessages(HWND hWnd, D2WINMSGENTRY *ptTable, DWORD dwCount);
extern "C" void __fastcall D2WinEditBoxDeselect(D2WINEDITBOX *hChild);
BOOL __fastcall D2WinEditBoxOnChar(D2WINEDITBOX *hChild, char chKey);
BOOL __fastcall D2WinEditBoxOnKeyDown(D2WINEDITBOX *hChild, char chKey);
void __fastcall D2WinButtonClick(_PARAMS *ptMsg);

D2WINEDITBOX * __fastcall D2WinEditBoxCreate(int nX, int nY, int nWidth, int nHeight, int nMarginX,
	int nMarginY, HCELFILE hCelFile, PFNEDITRETURN pfnReturn, PFNWINMSG pfnCallback, DWORD dwEditFlags,
	D2WINFONTDESC *ptFont);
DWORD __fastcall D2WinEditBoxDestroy(D2WINEDITBOX *hChild);
D2WINIMAGE * __fastcall D2WinImageCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	PFNWINMSG pfnCallback, D2WINHOTSPOT *ptHotSpots, PFNWINMSG pfnKey);
DWORD __fastcall D2WinImageDestroy(D2WINIMAGE *ptImageData);
D2WINANIMIMAGE * __fastcall D2WinAnimImageCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	int nSpeed, PFNWINMSG pfnCallback, D2WINANIMSTATE *ptStates, DWORD dwDrawMode, PFNWINMSG pfnHover);
DWORD __fastcall D2WinAnimImageDestroy(D2WINANIMIMAGE *ptAnim);
D2WINBUTTON * __fastcall D2WinButtonCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	PFNWINMSG pfnCallback, DWORD dwKey, int nFirstCel, DWORD dwButtonFlags, DWORD dwStringId,
	PFNWINMSG pfnHover);
DWORD __fastcall D2WinButtonDestroy(D2WINBUTTON *ptBtnData);
D2WINLIST * __fastcall D2WinListCreate(int nX, int nY, int nWidth, int nHeight, D2WINLISTENTRY *ptEntries,
	D2WINFONTDESC *ptFont);
DWORD __fastcall D2WinListDestroy(D2WINLIST *ptListData);
D2WINACCOUNTLIST * __fastcall D2WinAccountListCreate(int nX, int nY, int nWidth, int nHeight,
	DWORD dwControlFlags,
	D2WINFONTDESC *ptFont);
DWORD __fastcall D2WinAccountListDestroy(D2WINACCOUNTLIST *ptListData);
typedef void (__stdcall *PFNWINLOOP)(int nPass);

struct D2WINJPEG
{
 	DWORD		unobserved_00;
 	BYTE		*pbImage;
 	int			nImageWidth;
 	int			nImageHeight;
 	int			nRowPadding;
 	DWORD		unobserved_14;
 	DWORD		field_18;
 	DWORD		unobserved_1c;
 	const char	*pszFileName;
 	DWORD		unobserved_24[2];
 	int			field_2c;
 	int			field_30;
 	DWORD		unobserved_34[7];
 	int			field_50;
 	BYTE		unobserved_54[19948];
};

static HCELFILE sghCursor = { 0 };
int sgnCursorDrawnX = 0;
int sgnCursorDrawnY = 0;
static DWORD sgdwCursorCel = 0;
D2WINDOWDATA sgtWindowData = { 0 };
static BOOL sgfQuitPosted = 0;
static DWORD sgdwLastScreenshot = 0;

static void __stdcall D2WinMainOnMouseMove(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnChar(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnEscape(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnKeyDown(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnControlDown(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnControlUp(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnSysCommand(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnActivateApp(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnPrintScreen(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnKeyM(_PARAMS *ptMsg);
static void __stdcall D2WinMainOnMouseButton(_PARAMS *ptMsg);
LRESULT __stdcall D2WinMainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL __fastcall D2WinMainFreeDeadChildren(void);
void __fastcall D2WinMainDraw(void);

static BOOL sgfCursor = TRUE;
static int sgnScreenshot = 1;

#define NUM_WINDOW_MESSAGES		27

static D2WINMSGENTRY sgatMessages[NUM_WINDOW_MESSAGES] =
{
	{ D2WINMSG_MESSAGE, WM_MOUSEMOVE, (void *)D2WinMainOnMouseMove },
	{ D2WINMSG_MESSAGE, WM_LBUTTONDOWN, (void *)D2WinMainOnMouseButton },
	{ D2WINMSG_MESSAGE, WM_LBUTTONUP, (void *)D2WinMainOnMouseButton },
	{ D2WINMSG_MESSAGE, WM_RBUTTONDOWN, (void *)D2WinMainOnMouseButton },
	{ D2WINMSG_MESSAGE, WM_RBUTTONUP, (void *)D2WinMainOnMouseButton },
	{ D2WINMSG_MESSAGE, WM_CHAR, (void *)D2WinMainOnChar },
	{ D2WINMSG_KEYDOWN, VK_BACK, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_LEFT, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_RIGHT, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_HOME, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_END, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_DELETE, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_UP, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_DOWN, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_PRIOR, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_NEXT, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_RETURN, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_ESCAPE, (void *)D2WinMainOnEscape },
	{ D2WINMSG_KEYDOWN, VK_TAB, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_SPACE, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, VK_CONTROL, (void *)D2WinMainOnControlDown },
	{ D2WINMSG_KEYDOWN, VK_F1, (void *)D2WinMainOnKeyDown },
	{ D2WINMSG_KEYDOWN, 'M', (void *)D2WinMainOnKeyM },
	{ D2WINMSG_MESSAGE, WM_ACTIVATEAPP, (void *)D2WinMainOnActivateApp },
	{ D2WINMSG_MESSAGE, WM_SYSCOMMAND, (void *)D2WinMainOnSysCommand },
	{ D2WINMSG_KEYUP, VK_SNAPSHOT, (void *)D2WinMainOnPrintScreen },
	{ D2WINMSG_KEYUP, VK_CONTROL, (void *)D2WinMainOnControlUp }
};

static BOOL sgfLoop = TRUE;

DWORD __stdcall D2WinCreateWindow(DWORD dwInstance, DWORD dwDisplayType, DWORD dwWindowed,
	DWORD dwAllowCompressed)
{
	D2CMPSpriteCacheLoadSettings(dwAllowCompressed);
	return D2GfxInit(dwInstance, D2WinMainWindowProc, dwDisplayType, dwWindowed);
}

BOOL __stdcall D2WinInitGraphics(DWORD dwWindowed, int nResolution)
{
	HWND hWnd;

	sgtWindowData.field_38 = TRUE;
	if (!D2gfxWindowCreate(dwWindowed, nResolution))
		return FALSE;
	if (nResolution == 1)
		D2CMPSpriteCacheInitAll(0, 8388608, 512000, 0);
	hWnd = D2gfxWindowGetHandle();
	sgtWindowData.field_0c = NULL;
	sgtWindowData.field_18 = NULL;
	sgtWindowData.field_1c = NULL;
	sgtWindowData.deadChildrenLocked = 0;
	sgtWindowData.field_28 = NULL;
	sgtWindowData.field_2c = NULL;
	sgtWindowData.field_24 = NULL;
	sgtWindowData.field_34 = NULL;
	sgtWindowData.field_30 = NULL;
	sgdwCursorCel = 0;
	sgtWindowData.field_44 = NULL;
	sgtWindowData.field_14 = nResolution;
	sgtWindowData.field_08 = hWnd;
	sgtWindowData.field_10 = dwWindowed;
	return TRUE;
}

BOOL __stdcall D2WinSetWindowProc(PFNWINDOWPROC pfnWindowProc)
{
	sgtWindowData.field_34 = pfnWindowProc;
	return TRUE;
}

BOOL __cdecl D2WinCreateParent(void)
{
	D2WinFontInit();
	D2WinFontSet(1);
	D2WinListLoadPentagram();
	D2CompLoadPalettes();
	sgtWindowData.field_34 = NULL;
	D2WinRegisterMessages(D2gfxWindowGetHandle(), sgatMessages, NUM_WINDOW_MESSAGES);
	D2WinPaletteSetAct(0);
	sgtWindowData.field_44 = NULL;
	return TRUE;
}

BOOL __fastcall D2WinCloseGraphics(void)
{
	sgtWindowData.field_38 = FALSE;
	sgtWindowData.field_08 = NULL;
	D2CMPSpriteCacheShutdown(1);
	D2gfxWindowDestroy();
	return TRUE;
}

extern "C" DWORD __stdcall D2WinCutsceneCreateWindow(void)
{
	sgtWindowData.field_14 = 0;
	return D2GfxCutsceneCreateWindow();
}

extern "C" void __stdcall D2WinCutsceneCloseWindow(int nResolution)
{
	sgtWindowData.field_14 = nResolution;
	if (nResolution == 1)
		D2CMPSpriteCacheShutdown(nResolution);
	D2GfxCutsceneCloseWindow(nResolution);
	if (nResolution == 1)
		D2CMPSpriteCacheInitAll(0, 8388608, 512000, 0);
}

BOOL __fastcall D2WinCloseParent(void)
{
	sgtWindowData.deadChildrenLocked = 0;
	D2WinMainFreeDeadChildren();
	D2WinListFreePentagram();
	D2WinFontDestroy();
	D2WinUnregisterMessages(D2gfxWindowGetHandle(), sgatMessages, NUM_WINDOW_MESSAGES);
	return TRUE;
}

BOOL __stdcall D2WinResizeWindow(int nResolution)
{
	sgtWindowData.field_14 = nResolution;
	D2CMPSpriteCacheShutdown(1);
	if (!D2gfxWindowSetResolution(nResolution, 0))
	{
		FogErrorManagerLog("Failed to resize window... this is fatal!");
		exit(-1);
	}
	D2WinPaletteRefresh(FALSE);
	if (nResolution == 1)
		D2CMPSpriteCacheInitAll(0, 8388608, 512000, 0);
	return TRUE;
}

extern "C" DWORD __stdcall D2WinNukeCaches(void)
{
	return D2GfxNukeCaches();
}

BOOL __stdcall D2WinSetBackground(HCELFILE hCelFile)
{
	sgtWindowData.field_0c = hCelFile;
	return TRUE;
}

BOOL __stdcall D2WinSetCursor(HCELFILE hCelFile)
{
	sghCursor = hCelFile;
	return TRUE;
}

extern "C" void __stdcall D2WinSetQuitCallback(PFNWINQUIT pfnQuit)
{
	sgtWindowData.field_44 = pfnQuit;
}

BOOL __fastcall D2WinQuitLoop(void)
{
	sgfLoop = FALSE;
	return TRUE;
}

DWORD __stdcall D2WinLoop(PFNWINLOOP pfnPass)
{
	MSG tMsg;
	LARGE_INTEGER liNow, liFrequency;
	LONGLONG llLast;
	int nPass;

	nPass = 0;
	llLast = 0;
	while (sgfLoop)
	{
		if (PeekMessageA(&tMsg, NULL, 0, 0, PM_NOREMOVE))
		{
			sgfLoop = GetMessageA(&tMsg, NULL, 0, 0);
			if (!sgfLoop && sgtWindowData.field_44)
				sgtWindowData.field_44();
			TranslateMessage(&tMsg);
			DispatchMessageA(&tMsg);
		}
		else
		{
			QueryPerformanceCounter(&liNow);
			QueryPerformanceFrequency(&liFrequency);
			if ((liNow.QuadPart - llLast) * 25 > liFrequency.QuadPart)
			{
				llLast = liNow.QuadPart;
				if (pfnPass)
					pfnPass(nPass++);
				D2WinMainDraw();
			}
			sgtWindowData.field_28 = sgtWindowData.field_24;
		}
	}
	D2WinMainDraw();
	if (!sgfQuitPosted)
		sgfLoop = TRUE;
	return 0;
}

BOOL __stdcall D2WinChildSetCelFile(D2WINCONTROL *ptChild, HCELFILE hCelFile)
{
	ptChild->hCelFile = hCelFile;
	return TRUE;
}

BOOL __stdcall D2WinSetKeyFocus(D2WINCONTROL *ptChild)
{
	D2WINCONTROL *ptOld = (D2WINCONTROL *)sgtWindowData.field_24;

	if (ptOld)
	{
		if (ptChild != ptOld)
			D2WinEditBoxDeselect((D2WINEDITBOX *)ptOld);
	}
	sgtWindowData.field_24 = ptChild;
	return TRUE;
}

D2WINCONTROL * __fastcall D2WinGetKeyFocus(void)
{
	return sgtWindowData.field_28;
}

BOOL __stdcall D2WinMainAddChild(D2WINCONTROL *ptChild)
{
	D2WINCONTROL *ptLast;

	if (!sgtWindowData.field_18)
	{
		sgtWindowData.field_18 = ptChild;
		return TRUE;
	}
	for (ptLast = sgtWindowData.field_18; ptLast->ptNextChild; ptLast = ptLast->ptNextChild)
		;
	ptLast->ptNextChild = ptChild;
	return TRUE;
}

BOOL __fastcall D2WinMainAddDeadChild(D2WINCONTROL *ptChild)
{
	D2WINCONTROL *ptLast;

	ptChild->ptNextChild = NULL;
	if (!sgtWindowData.field_1c)
	{
		sgtWindowData.field_1c = ptChild;
		return TRUE;
	}
	for (ptLast = sgtWindowData.field_1c; ptLast->ptNextChild; ptLast = ptLast->ptNextChild)
		;
	ptLast->ptNextChild = ptChild;
	return TRUE;
}

BOOL __fastcall D2WinMainFreeDeadChildren(void)
{
	D2WINCONTROL *ptChild;
	D2WINCONTROL *ptNext;

	if (sgtWindowData.deadChildrenLocked > 0)
		return TRUE;
	while (sgtWindowData.field_1c && !(sgtWindowData.field_1c->dwFlags & gdwBitMasks[0]))
	{
		ptChild = sgtWindowData.field_1c;
		ptNext = ptChild->ptNextChild;
#line 482
		D2FREE2(ptChild);
		sgtWindowData.field_1c = ptNext;
	}
	while (sgtWindowData.field_1c && !(sgtWindowData.field_1c->dwFlags & gdwBitMasks[1]))
	{
		ptChild = sgtWindowData.field_1c;
		ptNext = ptChild->ptNextChild;
#line 491
		D2FREE2(ptChild);
		sgtWindowData.field_1c = ptNext;
	}
	return TRUE;
}

DWORD __stdcall D2WinMainFreeChild(D2WINCONTROL *ptChild)
{
	D2WINCONTROL *ptPrevious;
	D2WINCONTROL *ptNext;

	if (sgtWindowData.field_28 == ptChild)
		sgtWindowData.field_28 = NULL;
	if (sgtWindowData.field_24 == ptChild)
		sgtWindowData.field_24 = NULL;
	if (sgtWindowData.field_30 == ptChild)
		sgtWindowData.field_30 = NULL;
	if (sgtWindowData.field_2c == ptChild)
		sgtWindowData.field_2c = NULL;
	ptPrevious = sgtWindowData.field_18;
	if (!ptPrevious)
		return FALSE;
	if (ptPrevious == ptChild)
	{
		sgtWindowData.field_18 = ptChild->ptNextChild;
		D2WinMainAddDeadChild(ptChild);
		return TRUE;
	}
	for (ptNext = ptPrevious->ptNextChild; ptNext; ptPrevious = ptNext, ptNext = ptNext->ptNextChild)
	{
		if (ptNext == ptChild)
		{
			ptPrevious->ptNextChild = ptNext->ptNextChild;
			D2WinMainAddDeadChild(ptChild);
			return TRUE;
		}
	}
	return TRUE;
}

D2WINCONTROL * __stdcall D2WinCreateChild(D2WIN_STRUCT *ptWindow)
{
	HCELFILE hCelFile;

	if (ptWindow->phCelFile)
		hCelFile = *ptWindow->phCelFile;
	else
		hCelFile = NULL;
	switch (ptWindow->eType)
	{
	case D2WIN_TEXTBOX:
		return D2WinTextBoxCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, ptWindow->field_14, ptWindow->field_18, hCelFile,
			(PFNWINMSG)ptWindow->dwCallback, ptWindow->dwFlags, (D2WINFONTDESC *)ptWindow->field_28,
			(D2WINTEXTBOXBAR *)ptWindow->field_24);
	case D2WIN_EDITBOX:
		return D2WinEditBoxCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, ptWindow->field_14, ptWindow->field_18, hCelFile,
			(PFNEDITRETURN)ptWindow->field_24, (PFNWINMSG)ptWindow->dwCallback, ptWindow->dwFlags,
			(D2WINFONTDESC *)ptWindow->field_28);
	case D2WIN_IMAGE:
		return D2WinImageCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, hCelFile, (PFNWINMSG)ptWindow->dwCallback, (D2WINHOTSPOT *)ptWindow->field_24,
			(PFNWINMSG)ptWindow->field_28);
	case D2WIN_ANIMIMAGE:
		return D2WinAnimImageCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, hCelFile, ptWindow->field_14, (PFNWINMSG)ptWindow->dwCallback,
			(D2WINANIMSTATE *)ptWindow->field_24, ptWindow->field_18, (PFNWINMSG)ptWindow->field_28);
	case D2WIN_SCROLLBAR:
		return D2WinScrollbarCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, hCelFile, (PFNWINMSG)ptWindow->dwCallback);
	case D2WIN_BUTTON:
		return D2WinButtonCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, hCelFile, (PFNWINMSG)ptWindow->dwCallback, ptWindow->field_14,
			ptWindow->field_24, ptWindow->dwFlags, ptWindow->field_18, (PFNWINMSG)ptWindow->field_28);
	case D2WIN_LIST:
		return D2WinListCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, (D2WINLISTENTRY *)ptWindow->field_24, (D2WINFONTDESC *)ptWindow->field_28);
	case D2WIN_TIMER:
		return TimerCreate(ptWindow->nX, (PFNWINMSG)ptWindow->dwCallback);
	case D2WIN_SMACK:
		return D2WinSmackCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, (PFNWINMSG)ptWindow->dwCallback);
	case D2WIN_PROGRESSBAR:
		return D2WinProgressBarCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight);
	case D2WIN_ACCOUNTLIST:
		return D2WinAccountListCreate(ptWindow->nX, ptWindow->nY, ptWindow->dwWidth,
			ptWindow->dwHeight, ptWindow->dwFlags, (D2WINFONTDESC *)ptWindow->field_28);
	default:
		return NULL;
	}
}

DWORD __stdcall D2WinDestroyChild(D2WINCONTROL **pptChild)
{
	D2WINCONTROL *ptChild;
	DWORD dwResult;

	if (!pptChild)
		return 0;
	ptChild = *pptChild;
	if (!ptChild)
		return 0;
	switch (ptChild->eType)
	{
	case D2WIN_TEXTBOX:
		dwResult = D2WinTextBoxDestroy((D2WINTEXTBOX *)ptChild);
		break;
	case D2WIN_EDITBOX:
		dwResult = D2WinEditBoxDestroy((D2WINEDITBOX *)ptChild);
		break;
	case D2WIN_IMAGE:
		dwResult = D2WinImageDestroy((D2WINIMAGE *)ptChild);
		break;
	case D2WIN_ANIMIMAGE:
		dwResult = D2WinAnimImageDestroy((D2WINANIMIMAGE *)ptChild);
		break;
	case D2WIN_SCROLLBAR:
		dwResult = D2WinScrollbarDestroy((D2WINSCROLLBAR *)ptChild);
		break;
	case D2WIN_BUTTON:
		dwResult = D2WinButtonDestroy((D2WINBUTTON *)ptChild);
		break;
	case D2WIN_LIST:
		dwResult = D2WinListDestroy((D2WINLIST *)ptChild);
		break;
	case D2WIN_TIMER:
		dwResult = D2WinTimerDestroy((D2WINTIMER *)ptChild);
		break;
	case D2WIN_SMACK:
		dwResult = D2WinSmackDestroy((D2WINSMACK *)ptChild);
		break;
	case D2WIN_PROGRESSBAR:
		dwResult = D2WinProgressBarDestroy((D2WINPROGRESSBAR *)ptChild);
		break;
	case D2WIN_ACCOUNTLIST:
		dwResult = D2WinAccountListDestroy((D2WINACCOUNTLIST *)ptChild);
		break;
	default:
		dwResult = 0;
		break;
	}
	*pptChild = NULL;
	return dwResult;
}

void __fastcall D2WinChildSetEnabled(D2WINCONTROL *ptChild, BOOL fEnabled)
{
#line 687
	D2ASSERT(ptChild != NULL);
	if (fEnabled)
		ptChild->dwFlags |= gdwBitMasks[0];
	else
		ptChild->dwFlags &= gdwInvBitMasks[0];
}

DWORD __fastcall D2WinChildGetEnabled(D2WINCONTROL *ptChild)
{
#line 701
	D2ASSERT(ptChild != NULL);
	return ptChild->dwFlags & gdwBitMasks[0];
}

void __fastcall D2WinChildSetVisible(D2WINCONTROL *ptChild, BOOL fVisible)
{
#line 714
	D2ASSERT(ptChild != NULL);
	if (fVisible)
		ptChild->dwFlags |= gdwBitMasks[2];
	else
		ptChild->dwFlags &= gdwInvBitMasks[2];
}

DWORD __fastcall D2WinChildGetVisible(D2WINCONTROL *ptChild)
{
#line 728
	D2ASSERT(ptChild != NULL);
	return gdwBitMasks[2] & ptChild->dwFlags;
}

BOOL __stdcall D2WinMouseInRect(const RECT *ptRect)
{
	return PtInRect(ptRect, sgtWindowData.tMouse);
}

BOOL __fastcall D2WinMainIsControlDown(void)
{
	return sgtWindowData.field_40;
}

static inline void D2WinMainReadMouse(_PARAMS *ptMsg, int *pnX, int *pnY)
{
	int nX, nY;
	int nWidth, nHeight;
	BOOL fMoved;

	nX = (WORD)ptMsg->dwLParam;
	nY = ptMsg->dwLParam >> 16;
	D2gfxWindowGetScreenSize(&nWidth, &nHeight);
	nWidth--;
	nHeight--;
	if (D2GfxTestDisplayTypeGlideOrUp())
	{
		fMoved = FALSE;
		if (nX > nWidth)
		{
			nX = nWidth;
			fMoved = TRUE;
		}
		if (nY > nHeight)
		{
			nY = nHeight;
			fMoved = TRUE;
		}
		if (fMoved)
			SetCursorPos(nX, nY);
	}
	*pnX = nX;
	*pnY = nY;
}

static void __stdcall D2WinMainOnMouseMove(_PARAMS *ptMsg)
{
	int nX, nY;

	D2WinMainReadMouse(ptMsg, &nX, &nY);
	sgtWindowData.tMouse.y = nY;
	sgtWindowData.tMouse.x = nX;
}

void __fastcall D2WinMainDrawCursor(void)
{
	D2CELCONTEXT tCelContext;
	int nX, nY;

	if (sghCursor && sgfCursor)
	{
		nX = sgtWindowData.tMouse.x;
		nY = sgtWindowData.tMouse.y;
		sgnCursorDrawnX = nX;
		sgnCursorDrawnY = nY;
		if (nX <= 0)
			nX = 0;
		if (nY <= 0)
			nY = 0;
		memset(&tCelContext, 0, sizeof(tCelContext));
		tCelContext.dwDirection = 0;
		tCelContext.hCelFile = sghCursor;
		tCelContext.dwFrame = sgdwCursorCel;
		D2gfxScaleDrawCel(&tCelContext, nX, nY - 1, -1, 5, 0);
	}
}

void __fastcall D2WinMainDrawUnits(void)
{
	D2COMPOSITEUNIT *ptUnit;

	for (ptUnit = D2CompGetFirst(); ptUnit; ptUnit = D2CompGetNext(ptUnit))
		D2CompUnitDrawUnder(ptUnit);
	for (ptUnit = D2CompGetFirst(); ptUnit; ptUnit = D2CompGetNext(ptUnit))
		D2CompUnitDraw(ptUnit, (DWORD)-1);
	for (ptUnit = D2CompGetFirst(); ptUnit; ptUnit = D2CompGetNext(ptUnit))
	{
		D2CompUnitDrawName(ptUnit);
		D2CompUnitDrawInfo(ptUnit);
	}
}

void __fastcall D2WinMainDraw(void)
{
	D2WINCONTROL *ptChild;
	int nBottom;
	int nX, nY;

	if (D2gfxWindowUpdateMinimized())
		return;
	for (ptChild = sgtWindowData.field_18; ptChild; ptChild = ptChild->ptNextChild)
	{
		if (ptChild->pfnUpdate)
			ptChild->pfnUpdate(ptChild);
	}
	if (sgtWindowData.field_34)
		return;
	switch (sgtWindowData.field_14)
	{
	case 1:
		nBottom = 600;
		break;
	default:
		nBottom = 480;
		break;
	}
	if (D2GfxBeginFrame())
	{
		D2gfxScaleClearBackBuffer(0);
		if (sgtWindowData.field_0c)
		{
			D2CELCONTEXT tCelContext;

			memset(&tCelContext, 0, sizeof(tCelContext));
			tCelContext.hCelFile = sgtWindowData.field_0c;
			tCelContext.dwDirection = 0;
			tCelContext.dwFrame = 0;
			for (nY = 256; nY < 768; nY += 256)
			{
				for (nX = 0; nX < 1024; nX += 256)
				{
					D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, 5, 0);
					tCelContext.dwFrame++;
				}
			}
			nY = nBottom - 1;
			for (nX = 0; nX < 1024; nX += 256)
			{
				D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, 5, 0);
				tCelContext.dwFrame++;
			}
		}
		for (ptChild = sgtWindowData.field_18; ptChild; ptChild = ptChild->ptNextChild)
		{
			if (ptChild->pfnDraw && !(ptChild->dwFlags & gdwBitMasks[3]))
				ptChild->pfnDraw(ptChild);
		}
		D2WinMainDrawUnits();
		for (ptChild = sgtWindowData.field_18; ptChild; ptChild = ptChild->ptNextChild)
		{
			if (ptChild->pfnDraw && (ptChild->dwFlags & gdwBitMasks[3]))
				ptChild->pfnDraw(ptChild);
		}
		D2WinMainDrawCursor();
		D2GfxEndFrameAndPresent();
	}
	D2WinMainFreeDeadChildren();
}

LRESULT __stdcall D2WinMainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL fUseResult;
	LRESULT lResult;

	if (!sgtWindowData.field_38)
		return 0;
	if (sgtWindowData.field_34)
		return sgtWindowData.field_34(hWnd, uMsg, wParam, lParam);
	fUseResult = FALSE;
	lResult = 0;
	if (uMsg == WM_CLOSE)
	{
		sgfQuitPosted = TRUE;
		PostQuitMessage(0);
		return 0;
	}
	if (StormSMsgDispatchMessage(hWnd, uMsg, wParam, lParam, &fUseResult, &lResult) && fUseResult)
		return lResult;
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

static inline BOOL D2WinMainTakesInput(D2WINCONTROL *ptChild)
{
	D2WINCONTROL *ptOther;

	if (!sgtWindowData.field_30)
		return TRUE;
	for (ptOther = sgtWindowData.field_30; ptOther; ptOther = ptOther->ptNextChild)
	{
		if (ptOther == ptChild)
			return TRUE;
	}
	return FALSE;
}

static void __stdcall D2WinMainOnChar(_PARAMS *ptMsg)
{
	D2WINCONTROL *ptChild;
	BOOL fHandled;
	DWORD dwKey;

	dwKey = ptMsg->dwWParam;
	if (sgtWindowData.field_28 && D2WinMainTakesInput(sgtWindowData.field_28)
		&& D2WinEditBoxOnChar((D2WINEDITBOX *)sgtWindowData.field_28, dwKey))
	{
		StormSMsgBreakHandlerChain(ptMsg);
		return;
	}
	ptChild = sgtWindowData.field_30;
	if (!ptChild)
		ptChild = sgtWindowData.field_18;
	for (; ptChild; ptChild = ptChild->ptNextChild)
	{
		if (ptChild->pfnChar)
		{
			ptMsg->window = ptChild;
			sgtWindowData.deadChildrenLocked++;
			fHandled = ptChild->pfnChar(ptMsg);
			sgtWindowData.deadChildrenLocked--;
			if (fHandled)
				break;
		}
	}
	StormSMsgBreakHandlerChain(ptMsg);
}

static void __stdcall D2WinMainOnKeyDown(_PARAMS *ptMsg);

static void __stdcall D2WinMainOnEscape(_PARAMS *ptMsg)
{
	if (sgtWindowData.field_2c && D2WinMainTakesInput(sgtWindowData.field_2c))
	{
		ptMsg->window = sgtWindowData.field_2c;
		D2WinButtonClick(ptMsg);
		StormSMsgBreakHandlerChain(ptMsg);
		return;
	}
	D2WinMainOnKeyDown(ptMsg);
}

static void __stdcall D2WinMainOnKeyDown(_PARAMS *ptMsg)
{
	D2WINCONTROL *ptChild;
	BOOL fHandled;
	DWORD dwKey;

	dwKey = ptMsg->dwWParam;
	if (sgtWindowData.field_28 && D2WinMainTakesInput(sgtWindowData.field_28)
		&& D2WinEditBoxOnKeyDown((D2WINEDITBOX *)sgtWindowData.field_28, dwKey))
	{
		StormSMsgBreakHandlerChain(ptMsg);
		return;
	}
	ptChild = sgtWindowData.field_30;
	if (!ptChild)
		ptChild = sgtWindowData.field_18;
	for (; ptChild; ptChild = ptChild->ptNextChild)
	{
		if (ptChild->pfnKey != NULL)
		{
			ptMsg->window = ptChild;
			sgtWindowData.deadChildrenLocked++;
			fHandled = ptChild->pfnKey(ptMsg);
			sgtWindowData.deadChildrenLocked--;
			if (fHandled)
				break;
		}
	}
	StormSMsgBreakHandlerChain(ptMsg);
}

static void __stdcall D2WinMainOnControlDown(_PARAMS *ptMsg)
{
	sgtWindowData.field_40 = TRUE;
	StormSMsgBreakHandlerChain(ptMsg);
}

static void __stdcall D2WinMainOnControlUp(_PARAMS *ptMsg)
{
	sgtWindowData.field_40 = FALSE;
	StormSMsgBreakHandlerChain(ptMsg);
}

static void __stdcall D2WinMainOnSysCommand(_PARAMS *ptMsg)
{
	WORD wCommand;

	wCommand = (WORD)ptMsg->dwWParam & 65520;
	if (wCommand == SC_TASKLIST || wCommand == SC_KEYMENU || (wCommand == SC_MOVE
		&& D2gfxWindowTestFullScreen()))
	{
		ptMsg->dwHandled = 1;
		ptMsg->dwResult = 0;
	}
}

static void __stdcall D2WinMainOnActivateApp(_PARAMS *ptMsg)
{
	if (ptMsg->dwWParam)
	{
		if (!FogErrorManagerIsInError())
		{
			D2gfxWindowRequestMinimize(0);
			D2WinPaletteRefresh(TRUE);
		}
	}
	else
		D2gfxWindowRequestMinimize(1);
	if (D2gfxWindowIsCutscenePlaying())
		D2gfxWindowUpdateMinimized();
	ptMsg->dwHandled = 0;
}

static __inline void sScreenshotWrite(BYTE *pbPixels, int nWidth, int nHeight)
{
	D2WINJPEG tJpeg;
	char szName[256];
	HANDLE hFile;

	while (1)
	{
		sprintf(szName, "Screenshot%d.jpg", sgnScreenshot);
		sgnScreenshot++;
		hFile = FogSafesockCreateFile(szName, GENERIC_READ, 0, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			break;
		FogSafesockCloseHandle(hFile);
	}
	memset(&tJpeg, 0, sizeof(tJpeg));
	ijlInit((DWORD)&tJpeg);
	tJpeg.nRowPadding = ((nWidth * 3 + 3) & ~3) - nWidth * 3;
	tJpeg.nImageWidth = nWidth;
	tJpeg.nImageHeight = nHeight;
	tJpeg.pbImage = pbPixels;
	tJpeg.field_18 = 1;
	tJpeg.pszFileName = szName;
	tJpeg.field_2c = nWidth;
	tJpeg.field_30 = nHeight;
	tJpeg.field_50 = 100;
	ijlWrite((DWORD)&tJpeg, 8);
	ijlFree((DWORD)&tJpeg);
}

void __fastcall D2WinScreenshot(void)
{
	DWORD dwNow;
	int nWidth, nHeight;
	BYTE *pbPixels;

	dwNow = GetTickCount();
	if (dwNow - sgdwLastScreenshot < 1000)
		return;
	nWidth = 0;
	nHeight = 0;
	D2gfxWindowGetScreenSize(&nWidth, &nHeight);
#line 1545
	pbPixels = (BYTE *)D2ALLOC2(nHeight * nWidth * 3);
	if (D2GfxReadScreenRgb(pbPixels))
		sScreenshotWrite(pbPixels, nWidth, nHeight);
	sgdwLastScreenshot = dwNow;
#line 1559
	D2FREE2(pbPixels);
}

static void __stdcall D2WinMainOnPrintScreen(_PARAMS *ptMsg)
{
	D2WinScreenshot();
	ptMsg->dwHandled = 1;
	ptMsg->dwResult = 0;
}

static void __stdcall D2WinMainOnKeyM(_PARAMS *ptMsg)
{
	if (sgtWindowData.field_40 && D2SoundFastIsActive())
	{
		if (D2SoundGetOptionsMusic())
		{
			D2SoundSmpMusicFade();
			D2SoundSetOptionsMusic(0);
		}
		else
		{
			D2SoundSmpMusicSetState(1);
			D2SoundSetOptionsMusic(1);
		}
		ptMsg->dwHandled = 1;
		ptMsg->dwResult = 0;
	}
}

static inline BOOL D2WinMainChildHasPoint(D2WINCONTROL *ptChild, int nX, int nY)
{
	int nLeft = ptChild->nX;
	int nBottom = ptChild->nY;

	return nX >= nLeft && nY >= nBottom - ptChild->nHeight && nX < ptChild->nWidth + nLeft && nY < nBottom;
}

static void __stdcall D2WinMainOnMouseButton(_PARAMS *ptMsg)
{
	D2WINCONTROL *ptChild;
	int nX, nY;
	BOOL fHandled;

	D2WinMainReadMouse(ptMsg, &nX, &nY);
	sgtWindowData.tMouse.x = nX;
	sgtWindowData.tMouse.y = nY;
	if (PopupIsOpen())
	{
		fHandled = PopupOnClick(ptMsg);
		D2WinPopupDestroy();
		if (fHandled)
			return;
	}
	ptChild = sgtWindowData.field_30;
	if (!ptChild)
		ptChild = sgtWindowData.field_18;
	switch (ptMsg->dwMessage)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		sgtWindowData.field_3c = TRUE;
		for (; ptChild; ptChild = ptChild->ptNextChild)
		{
			nY = sgtWindowData.tMouse.y;
			nX = sgtWindowData.tMouse.x;
			if (ptChild->pfnHitTest ? ptChild->pfnHitTest(ptChild)
				: D2WinMainChildHasPoint(ptChild, nX, nY))
			{
				ptMsg->window = ptChild;
				if (ptChild->pfnMouseDown)
				{
					sgtWindowData.deadChildrenLocked++;
					fHandled = ptChild->pfnMouseDown(ptMsg);
					sgtWindowData.deadChildrenLocked--;
					if (fHandled)
						break;
				}
			}
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		sgtWindowData.field_3c = FALSE;
		for (; ptChild; ptChild = ptChild->ptNextChild)
		{
			ptMsg->window = ptChild;
			if (ptChild->pfnMouseUp)
			{
				sgtWindowData.deadChildrenLocked++;
				ptChild->pfnMouseUp(ptMsg);
				sgtWindowData.deadChildrenLocked--;
			}
		}
		break;
	default:
		break;
	}
	StormSMsgBreakHandlerChain(ptMsg);
}

void __stdcall D2WinShowCursor(BOOL fShow)
{
	sgfCursor = fShow;
}

void __stdcall D2WinChildSetOnTop(D2WINCONTROL *ptChild, BOOL fOnTop)
{
	if (fOnTop)
		ptChild->dwFlags |= gdwBitMasks[3];
	else
		ptChild->dwFlags &= gdwInvBitMasks[3];
}

void __stdcall D2WinSetEscapeButton(D2WINCONTROL *ptChild)
{
	sgtWindowData.field_2c = ptChild;
}

BOOL __fastcall D2WinIsMouseDown(void)
{
	return sgtWindowData.field_3c;
}

BOOL __stdcall D2WinSetInputChild(D2WINCONTROL *ptChild)
{
	sgtWindowData.field_30 = ptChild;
	return TRUE;
}

void * __stdcall D2WinDereference(void **ppValue)
{
	if (!ppValue)
		return NULL;
	return *ppValue;
}

D2WINCONTROL * __fastcall D2WinMainGetInputChild(void)
{
	return sgtWindowData.field_30;
}
