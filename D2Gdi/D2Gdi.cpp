#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\D2Gdi.cpp"
#include "gdiCore.h"

BOOL WINAPI GdiAlwaysTrue(void);
BOOL __fastcall GdiSetup(GDIDRAWCTX *ptDrawCtx, GDIBLITTABLE *ptBlitTable);
BOOL __fastcall GdiTeardown(void);
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

DWORD __cdecl GdiGetInterface(void)
{
	static void *sgapfnInterface[53] =
	{
		(void *)GdiAlwaysTrue,
		(void *)GdiSetup,
		(void *)GdiTeardown,
		(void *)GdiInit,
		(void *)GdiDestroyBackBuffer,
		(void *)GdiSetSuspended,
		(void *)GdiEnterAndClear,
		(void *)GdiLeavePause,
		(void *)GdiPresent,
		(void *)GdiReinit,
		(void *)GdiExpandToRgb,
		(void *)GdiAlwaysFalse,
		(void *)GdiGetFrameOpenQuery,
		(void *)GdiOpenCutscene,
		(void *)GdiNothingPlayCutscene,
		(void *)GdiIsCutscene,
		(void *)GdiSmackOpen,
		(void *)GdiSmackUpdate,
		(void *)GdiSmackClose,
		(void *)GdiGetMemoryBudget,
		(void *)GdiGetScreenSize,
		(void *)GdiNothingSetUnreadDword,
		(void *)GdiNothingSetGamma,
		(void *)GdiHasGamma,
		(void *)GdiNothingSetPerspOrigin,
		(void *)GdiNothingPerspProject,
		(void *)GdiNothingPerspProjectDepth,
		(void *)GdiNothingResetPerspDepth,
		(void *)GdiSetPaletteEntries,
		(void *)GdiSetEnvInfo,
		(void *)GdiNothingSetVertexColor,
		(void *)GdiDrawTile,
		(void *)GdiDrawCelSkip,
		(void *)GdiDrawCel,
		(void *)GdiDrawCelPreMap,
		(void *)GdiSpriteDrawRows,
		(void *)GdiSpriteDrawShadow,
		(void *)GdiSpriteStub,
		(void *)GdiSpriteDrawWallLow,
		(void *)GdiSpriteDrawWallShaded,
		(void *)GdiSpriteDrawUnitBlocks,
		(void *)GdiDrawDiamond,
		(void *)GdiDrawFrame,
		(void *)GdiFillRect,
		(void *)GdiFillSquare,
		(void *)GdiFillSpan,
		(void *)GdiFillSpanSelect,
		(void *)GdiDrawLine,
		(void *)GdiClearBackBuffer,
		(void *)GdiDrawDebugText,
		(void *)GdiNothingDebugLightGrid,
		(void *)GdiSpriteWarp,
		(void *)GdiNothingFlushCaches,
	};

	return (DWORD)sgapfnInterface;
}

char gszRegistrationTable[128] = "REGISTRATION_TABLE";
char gszRegistrationBlock[128] = "REGISTRATION_BLOCK";
