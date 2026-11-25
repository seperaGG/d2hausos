#ifndef D2RE_D2GFXMISC_H
#define D2RE_D2GFXMISC_H

#include <windows.h>
#include "D2gfxLinkNames.h"

extern BOOL gfInDebugBreak;

extern const char *const gpszD2GfxDll;

BOOL __cdecl FaultErrorBox(DWORD dwError, ...);

#endif
