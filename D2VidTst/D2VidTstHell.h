#ifndef D2RE_D2VIDTSTHELL_H
#define D2RE_D2VIDTSTHELL_H

#include <windows.h>

extern BOOL gfInDebugBreak;

extern const char *const gpszD2VidTstExe;

BOOL __cdecl FaultErrorBox(DWORD dwError, ...);

#endif
