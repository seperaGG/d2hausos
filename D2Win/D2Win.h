#ifndef D2RE_D2WIN_H
#define D2RE_D2WIN_H

#include "D2WinTypes.h"

DWORD __fastcall D2WinFontSet(DWORD eFont);
int __fastcall D2WinFontGetTextWidthN(const WORD *pwszText, int nCount);
int __fastcall D2WinFontGetTextWidth(const WORD *pwszText);
DWORD __fastcall D2WinFontGetHeight(void);
void __fastcall D2WinFontDrawText(const WORD *pwszText, int nX, int nY, DWORD dwColor, BOOL fCenter);
void __fastcall D2WinFontDrawTextWithMode(const WORD *pwszText, int nX, int nY, DWORD dwColor, BOOL fCenter,
	DWORD dwDrawMode);
void __fastcall D2WinFontGetTextSize(const WORD *pwszText, int *pnWidth, int *pnHeight);
void __fastcall D2WinFontDrawFramedText(const WORD *pwszText, int nX, int nY, DWORD dwRectColor,
	DWORD dwRectMode, DWORD dwColor);
BOOL __fastcall D2WinFontIsValidChar(char chChar);

BOOL __stdcall D2WinMainAddChild(D2WINCONTROL *ptControl);
DWORD __stdcall D2WinMainFreeChild(D2WINCONTROL *ptControl);
BOOL __stdcall D2WinSetKeyFocus(D2WINCONTROL *ptControl);
D2WINCONTROL * __fastcall D2WinGetKeyFocus(void);
BOOL __stdcall D2WinMouseInRect(const RECT *ptRect);
BOOL __fastcall D2WinIsMouseDown(void);
D2WINCONTROL * __fastcall D2WinMainGetInputChild(void);

BOOL __fastcall D2WinPaletteSetAct(BYTE bAct);
DWORD __fastcall D2WinPaletteRefresh(BOOL fTimed);
DWORD __stdcall D2WinPaletteGetColor(BYTE dwRed, BYTE dwGreen, BYTE dwBlue);

BOOL __fastcall PopupIsOpen(void);
BOOL __stdcall PopupOnClick(_PARAMS *ptMsg);
DWORD __fastcall D2WinPopupDestroy(void);

D2WINPROGRESSBAR * __fastcall D2WinProgressBarCreate(int nX, int nY, int nWidth, int nHeight);
DWORD __fastcall D2WinProgressBarDestroy(D2WINPROGRESSBAR *ptData);
D2WINSCROLLBAR * __fastcall D2WinScrollbarCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	PFNWINMSG pfnCallback);
DWORD __fastcall D2WinScrollbarDestroy(D2WINSCROLLBAR *ptScrollbarData);

extern "C" int __fastcall D2WinScrollbarGetMaxValue(D2WINSCROLLBAR *ptScrollbarData);
BOOL __fastcall D2WinScrollbarSetMaxValue(D2WINSCROLLBAR *ptScrollbarData, int nMaxValue);
BOOL __fastcall D2WinScrollbarSetValue(D2WINSCROLLBAR *ptScrollbarData, int nValue);
int __fastcall D2WinScrollbarGetValue(D2WINSCROLLBAR *ptScrollbarData);
BOOL __fastcall D2WinScrollbarSetTextBox(D2WINSCROLLBAR *ptScrollbarData, D2WINCONTROL *ptTextBox);

D2WINSMACK * __fastcall D2WinSmackCreate(int nX, int nY, int nWidth, int nHeight, PFNWINMSG pfnCallback);
DWORD __fastcall D2WinSmackDestroy(D2WINSMACK *ptData);
D2WINTEXTBOX * __fastcall D2WinTextBoxCreate(int nX, int nY, int nWidth, int nHeight, int nMarginX,
	int nMarginY, HCELFILE hCelFile, PFNWINMSG pfnCallback, DWORD dwTextFlags, D2WINFONTDESC *ptFont,
	D2WINTEXTBOXBAR *ptBar);
DWORD __fastcall D2WinTextBoxDestroy(D2WINTEXTBOX *hChild);
BOOL __fastcall D2WinTextBoxSetTop(D2WINTEXTBOX *hChild, int nLine);
D2WINTIMER * __fastcall TimerCreate(int nDelay, PFNWINMSG pfnCallback);
DWORD __fastcall D2WinTimerDestroy(D2WINTIMER *ptTimerData);

HCELFILE __fastcall D2WinArchiveLoadCel(const char *pszName, DWORD eType);
void __fastcall D2WinArchiveFreeCel(HCELFILE hCelFile);

void __fastcall D2WinRegisterMessage(HWND hWnd, DWORD eType, DWORD id, void *pfnCallback);
void __fastcall D2WinUnregisterMessage(HWND hWnd, DWORD eType, DWORD id, void *pfnCallback);

#endif
