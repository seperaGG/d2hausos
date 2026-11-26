#ifndef MATCH_D2WIN_IMPORTS_H
#define MATCH_D2WIN_IMPORTS_H

#include <windows.h>
#include "FogImports.h"
#include "StormImports.h"

extern "C" DWORD __fastcall FogOrdinal10147(DWORD, DWORD);
extern "C" DWORD __cdecl FogOrdinal10150(...);
extern "C" __declspec(dllimport) DWORD gdwInvBitMasks[];
#define FogSafesockGetInstallPath(pszPath, dwSize) \
	FogOrdinal10147((DWORD)(pszPath), (DWORD)(dwSize))
#define FogSafesockGetFileOption() \
	FogOrdinal10150()

extern "C" DWORD __stdcall StormOrdinal414(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal415(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal416(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal417(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal512(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal513(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal514(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal515(DWORD, DWORD, DWORD);
#define StormSMsgRegisterCommand(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal414((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgRegisterKeyDown(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal415((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgRegisterKeyUp(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal416((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgRegisterMessage(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal417((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgUnregisterCommand(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal512((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgUnregisterKeyDown(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal513((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgUnregisterKeyUp(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal514((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))
#define StormSMsgUnregisterMessage(hWnd, dwId, pfnHandler) \
	((BOOL)StormOrdinal515((DWORD)(hWnd), (DWORD)(dwId), (DWORD)(pfnHandler)))

extern "C" DWORD __cdecl D2gfxOrdinal10029(...);
extern "C" DWORD __stdcall D2gfxOrdinal10035(DWORD, DWORD);
extern "C" DWORD __stdcall D2gfxOrdinal10036(DWORD);
extern "C" DWORD __stdcall D2gfxOrdinal10037(DWORD);
extern "C" DWORD __stdcall D2gfxOrdinal10052(DWORD);
extern "C" DWORD __stdcall D2gfxOrdinal10053(DWORD);
extern "C" DWORD __stdcall D2gfxOrdinal10056(DWORD, BYTE);
extern "C" DWORD __stdcall D2gfxOrdinal10059(DWORD, DWORD, DWORD, DWORD, BYTE, DWORD);
extern "C" DWORD __stdcall D2gfxOrdinal10076(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
#define D2gfxWindowGetHandle() \
	((HWND)D2gfxOrdinal10029())
#define D2gfxWindowOpenSmack(dwFileName, dwVideo) \
	D2gfxOrdinal10035((DWORD)(dwFileName), (DWORD)(dwVideo))
#define D2gfxWindowDrawSmackFrame(dwArg) \
	D2gfxOrdinal10036((DWORD)(dwArg))
#define D2gfxWindowCloseSmack(dwArg) \
	D2gfxOrdinal10037((DWORD)(dwArg))
#define D2gfxScaleSetPalette(ptEntries) \
	D2gfxOrdinal10052((DWORD)(ptEntries))
#define D2gfxScaleSetEnvInfo(ptEnvInfo) \
	D2gfxOrdinal10053((DWORD)(ptEnvInfo))
#define D2gfxScaleDrawFrame(dwRect, bColor) \
	D2gfxOrdinal10056((DWORD)(dwRect), (DWORD)(bColor))
#define D2gfxScaleFillSpan(dwX1, dwY1, dwX2, dwY2, dwColor, dwMode) \
	D2gfxOrdinal10059((DWORD)(dwX1), (DWORD)(dwY1), (DWORD)(dwX2), (DWORD)(dwY2), (BYTE)(dwColor), \
		(DWORD)(dwMode))
#define D2gfxScaleDrawCel(dwCelContext, dwX, dwY, dwColor, dwMode, dwPreMap) \
	D2gfxOrdinal10076((DWORD)(dwCelContext), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(dwColor), (DWORD)(dwMode), \
		(DWORD)(dwPreMap))

extern "C" DWORD __stdcall D2CMPOrdinal10004(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10024(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10032(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10046(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10051(DWORD);
#define D2CMPPaletteFindNearest(pbPalette, dwEntries, nColor0, nColor1, nColor2) \
	D2CMPOrdinal10004((DWORD)(pbPalette), (DWORD)(dwEntries), (DWORD)(nColor0), (DWORD)(nColor1), \
		(DWORD)(nColor2))
#define D2CMPCelFileLoadSerialized(pFile, ppFile, dwUnused3, dwUnused4, nSpecVersion, dwUnused6) \
	D2CMPOrdinal10024((DWORD)(pFile), (DWORD)(ppFile), (DWORD)(dwUnused3), (DWORD)(dwUnused4), \
		(DWORD)(nSpecVersion), (DWORD)(dwUnused6))
#define D2CMPCelFileFreeHardware(ptFile) \
	((BOOL)D2CMPOrdinal10032((DWORD)(ptFile)))
#define D2CMPCelFileGetNumFrames(pFile) \
	D2CMPOrdinal10046((DWORD)(pFile))
#define D2CMPSpriteCacheGetExtension(fDcc) \
	((const char *)D2CMPOrdinal10051((DWORD)(fDcc)))

extern "C" DWORD __fastcall D2soundOrdinal10039(DWORD);
#define D2SoundFastPlay(pszFile) \
	D2soundOrdinal10039((DWORD)(pszFile))

extern "C" DWORD __fastcall D2LangOrdinal10004(WORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10017(DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10033(DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10037(DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10039(DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10042(DWORD, DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10043(DWORD, DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10049(DWORD, DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10053(DWORD, DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10059(DWORD, DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10044(DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10051(DWORD, DWORD, DWORD);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10055(DWORD, DWORD);
#define D2LangStrGetStringByIndex(wStringId) \
	((void *)D2LangOrdinal10004((WORD)(wStringId)))
#define D2LangUnicodeDefaultConstructorClosure(ptThis) \
	D2LangOrdinal10017((DWORD)(ptThis))
#define D2LangUnicodestrcat(pszDst, pszSrc) \
	((void *)Unicode::strcat((Unicode *)(DWORD)(pszDst), (const Unicode *)(DWORD)(pszSrc)))
#define D2LangUnicodestrcpy(pszDst, pszSrc) \
	((void *)Unicode::strcpy((Unicode *)(DWORD)(pszDst), (const Unicode *)(DWORD)(pszSrc)))
#define D2LangUnicodestrlen(pszStr) \
	((int)Unicode::strlen((const Unicode *)(DWORD)(pszStr)))
#define D2LangUnicodestrncpy(pszDst, pszSrc, nCount) \
	((void *)Unicode::strncpy((Unicode *)(DWORD)(pszDst), (const Unicode *)(DWORD)(pszSrc), (int)(nCount)))
#define D2LangUnicodestrnicmp(pszStr1, pszStr2, nCount) \
	((int)Unicode::strnicmp((const Unicode *)(DWORD)(pszStr1), (const Unicode *)(DWORD)(pszStr2), (unsigned int)(nCount)))
#define D2LangUnicodestrstr(pszStr, pszSub) \
	((void *)D2LangOrdinal10044((DWORD)(pszStr), (DWORD)(pszSub)))
#define D2LangUnicodetoUnicode(pszDst, pszSrc, nCount) \
	((void *)Unicode::toUnicode((Unicode *)(DWORD)(pszDst), (const char *)(DWORD)(pszSrc), (int)(nCount)))
#define D2LangUnicodetoUtf(pszDst, pszSrc, nCount) \
	((char *)D2LangOrdinal10051((DWORD)(pszDst), (DWORD)(pszSrc), (DWORD)(nCount)))
#define D2LangUnicodeunicode2Win(pszDst, pszSrc, nCount) \
	((char *)Unicode::unicode2Win((char *)(DWORD)(pszDst), (const Unicode *)(DWORD)(pszSrc), (int)(nCount)))
#define D2LangUnicodeunicodenwidth(pszSrc, nBytes) \
	((unsigned int)D2LangOrdinal10055((DWORD)(pszSrc), (DWORD)(nBytes)))
#define D2LangUnicodewin2Unicode(pszDst, pszSrc, nCount) \
	((void *)Unicode::win2Unicode((Unicode *)(DWORD)(pszDst), (const char *)(DWORD)(pszSrc), (int)(nCount)))

extern "C" DWORD __stdcall D2gfxOrdinal10078(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
#define D2gfxScaleDrawSpriteRows(dwCelContext, dwX, dwY, dwFromBottom, dwRows, dwMode) \
	D2gfxOrdinal10078((DWORD)(dwCelContext), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(dwFromBottom), \
		(DWORD)(dwRows), (DWORD)(dwMode))
extern "C" DWORD __stdcall D2gfxOrdinal10077(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
#define D2gfxScaleDrawCelPalette(dwCelContext, dwX, dwY, dwColor, dwMode, dwPaletteIndex) \
	D2gfxOrdinal10077((DWORD)(dwCelContext), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(dwColor), (DWORD)(dwMode), \
		(DWORD)(dwPaletteIndex))
extern "C" DWORD __cdecl D2gfxOrdinal10003(...);
#define D2GfxGetDisplayType() \
	((int)((DWORD (__stdcall *)(void))D2gfxOrdinal10003)())
extern "C" DWORD __stdcall D2gfxOrdinal10079(DWORD, DWORD, DWORD);
#define D2gfxScaleDrawSpriteShadow(dwCelContext, nX, dwY) \
	D2gfxOrdinal10079((DWORD)(dwCelContext), (DWORD)(nX), (DWORD)(dwY))
extern "C" DWORD __cdecl D2gfxOrdinal10028(...);
#define D2gfxWindowUpdateMinimized() \
	((BOOL)D2gfxOrdinal10028())
extern "C" DWORD __cdecl D2gfxOrdinal10030(...);
#define D2gfxWindowTestFullScreen() \
	((BOOL)D2gfxOrdinal10030())
extern "C" DWORD __stdcall D2gfxOrdinal10032(DWORD, DWORD);
#define D2gfxWindowSetResolution(nResolution, fForce) \
	D2gfxOrdinal10032((DWORD)(nResolution), (DWORD)(fForce))
extern "C" DWORD __stdcall D2gfxOrdinal10027(DWORD);
#define D2gfxWindowRequestMinimize(fMinimize) \
	D2gfxOrdinal10027((DWORD)(fMinimize))
extern "C" DWORD __fastcall D2gfxOrdinal10034(void);
#define D2gfxWindowIsCutscenePlaying() \
	D2gfxOrdinal10034()
extern "C" DWORD __stdcall D2gfxOrdinal10040(DWORD, DWORD);
#define D2gfxWindowGetScreenSize(dwWidth, dwHeight) \
	D2gfxOrdinal10040((DWORD)(dwWidth), (DWORD)(dwHeight))
extern "C" DWORD __cdecl D2gfxOrdinal10026(...);
#define D2gfxWindowDestroy() \
	((BOOL)D2gfxOrdinal10026())
extern "C" DWORD __stdcall D2gfxOrdinal10025(DWORD, DWORD);
#define D2gfxWindowCreate(dwWindowed, nResolution) \
	((BOOL)D2gfxOrdinal10025((DWORD)(dwWindowed), (DWORD)(nResolution)))
extern "C" DWORD __stdcall D2gfxOrdinal10062(DWORD);
#define D2gfxScaleClearBackBuffer(dwArg) \
	D2gfxOrdinal10062((DWORD)(dwArg))
extern "C" DWORD __cdecl D2gfxOrdinal10051(...);
#define D2GfxTestDisplayTypeGlideOrUp() \
	((BOOL)((DWORD (__stdcall *)(void))D2gfxOrdinal10051)())
extern "C" DWORD __stdcall D2gfxOrdinal10050(DWORD);
#define D2GfxReadScreenRgb(dwArg) \
	D2gfxOrdinal10050((DWORD)(dwArg))
extern "C" DWORD __fastcall D2GfxNukeCaches(void);
extern "C" DWORD __stdcall D2gfxOrdinal10000(DWORD, DWORD, DWORD, DWORD);
#define D2GfxInit(hInstance, pfnWndProc, nDisplayType, dwWindowed) \
	D2gfxOrdinal10000((DWORD)(hInstance), (DWORD)(pfnWndProc), (DWORD)(nDisplayType), (DWORD)(dwWindowed))
extern "C" DWORD __cdecl D2gfxOrdinal10049(...);
#define D2GfxEndFrameAndPresent() \
	((BOOL)((DWORD (__stdcall *)(void))D2gfxOrdinal10049)())
extern "C" DWORD __fastcall D2GfxCutsceneCreateWindow(void);
extern "C" DWORD __stdcall D2GfxCutsceneCloseWindow(DWORD);
extern "C" DWORD __fastcall D2gfxOrdinal10048(void);
#define D2GfxBeginFrame() \
	D2gfxOrdinal10048()
extern "C" DWORD __stdcall D2CMPOrdinal10037(DWORD);
#define D2CMPCelGetWidth(pCel) \
	D2CMPOrdinal10037((DWORD)(pCel))
extern "C" DWORD __stdcall D2CMPOrdinal10038(DWORD);
#define D2CMPCelGetHeight(pCel) \
	D2CMPOrdinal10038((DWORD)(pCel))
extern "C" DWORD __stdcall D2CMPOrdinal10036(DWORD);
#define D2CMPCelContextGetCel(pCelContext) \
	((void *)D2CMPOrdinal10036((DWORD)(pCelContext)))
extern "C" DWORD __stdcall D2CMPOrdinal10053(DWORD);
#define D2CMPSpriteCacheShutdown(fFree) \
	D2CMPOrdinal10053((DWORD)(fFree))
extern "C" DWORD __stdcall D2CMPOrdinal10054(DWORD);
#define D2CMPSpriteCacheLoadSettings(fAllow) \
	D2CMPOrdinal10054((DWORD)(fAllow))
extern "C" DWORD __stdcall D2CMPOrdinal10052(DWORD, DWORD, DWORD, DWORD);
#define D2CMPSpriteCacheInitAll(pPool, dwGfxMax, dwCelDataMax, dwMemory) \
	D2CMPOrdinal10052((DWORD)(pPool), (DWORD)(dwGfxMax), (DWORD)(dwCelDataMax), (DWORD)(dwMemory))
extern "C" DWORD __stdcall D2CMPOrdinal10055(DWORD, DWORD, DWORD);
#define D2CMPSpriteCacheGetContextCel(ptCelContext, fPoll, fAsyncAhead) \
	((BOOL)D2CMPOrdinal10055((DWORD)(ptCelContext), (DWORD)(fPoll), (DWORD)(fAsyncAhead)))
extern "C" DWORD __stdcall D2CMPOrdinal10039(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10040(DWORD);
#define D2CMPCelGetOffsetX(pCel) \
	((int)D2CMPOrdinal10039((DWORD)(pCel)))
#define D2CMPCelGetOffsetY(pCel) \
	((int)D2CMPOrdinal10040((DWORD)(pCel)))
extern "C" DWORD __cdecl D2LangOrdinal10007(void);
#define D2LangStrGetUseLanguage() \
	((int)D2LangOrdinal10007())
#define D2LangUnicodesprintf \
	Unicode::sprintf
#define D2LangUnicodeDefaultConstructorClosureRaw D2LangOrdinal10017
extern "C" DWORD __stdcall StormOrdinal412(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
#define StormSMsgDispatchMessage(hWnd, uMsg, wParam, lParam, pfHandled, plResult) \
	((BOOL)StormOrdinal412((DWORD)(hWnd), (DWORD)(uMsg), (DWORD)(wParam), (DWORD)(lParam), \
		(DWORD)(pfHandled), (DWORD)(plResult)))
extern "C" DWORD __stdcall StormOrdinal511(DWORD);
#define StormSMsgBreakHandlerChain(ptParams) \
	((BOOL)StormOrdinal511((DWORD)(ptParams)))
extern "C" DWORD __fastcall FogOrdinal10080(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
#define FogSafesockCreateFile(pszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, \
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile) \
	((HANDLE)FogOrdinal10080((DWORD)(pszFileName), (DWORD)(dwDesiredAccess), (DWORD)(dwShareMode), \
		(DWORD)(lpSecurityAttributes), (DWORD)(dwCreationDisposition), (DWORD)(dwFlagsAndAttributes), \
		(DWORD)(hTemplateFile)))
extern "C" DWORD __fastcall FogOrdinal10082(DWORD);
#define FogSafesockCloseHandle(hObject) \
	((BOOL)FogOrdinal10082((DWORD)(hObject)))
extern "C" DWORD __cdecl FogOrdinal10032(...);
#define FogErrorManagerIsInError() \
	((BOOL)FogOrdinal10032())
extern "C" DWORD __fastcall D2soundOrdinal10034(DWORD);
#define D2SoundSmpMusicSetState(dwState) \
	D2soundOrdinal10034((DWORD)(dwState))
extern "C" DWORD __cdecl D2soundOrdinal10065(...);
#define D2SoundSmpMusicFade() \
	((DWORD (__fastcall *)(void))D2soundOrdinal10065)()
extern "C" DWORD __fastcall D2soundOrdinal10070(DWORD);
#define D2SoundSetOptionsMusic(dwValue) \
	D2soundOrdinal10070((DWORD)(dwValue))
extern "C" DWORD __cdecl D2soundOrdinal10069(...);
#define D2SoundGetOptionsMusic() \
	((DWORD (__fastcall *)(void))D2soundOrdinal10069)()
extern "C" DWORD __cdecl D2soundOrdinal10066(...);
#define D2SoundFastIsActive() \
	((BOOL)((DWORD (__fastcall *)(void))D2soundOrdinal10066)())

extern "C" __declspec(dllimport) DWORD __stdcall ijlInit(DWORD);
extern "C" __declspec(dllimport) DWORD __stdcall ijlFree(DWORD);
extern "C" __declspec(dllimport) DWORD __stdcall ijlWrite(DWORD, DWORD);

#endif
