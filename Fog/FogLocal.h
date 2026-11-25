#ifndef FOGLOCAL_H
#define FOGLOCAL_H
#define MATCH_FOGIMPORTS_H
#include <windows.h>
#include "FogLinkNames.h"

void __cdecl ErrorManagerAssert(const char *pszExpression, const char *pszFile, int nLine);
void __cdecl ErrorManagerHalt(const char *pszMessage, const char *pszFile, int nLine);
void __cdecl ErrorManagerWarn(const char *pszMessage, const char *pszFile, int nLine);
void * __fastcall ErrorManagerAlloc(int nSize, const char *pszFile, int nLine, int nUnused);
BOOL __fastcall ErrorManagerFree(void *pMemory, const char *pszFile, int nLine, int nUnused);
char * __cdecl DataArraysSprintf(char *pszBuffer, const char *pszFormat, ...);

#define FogErrorManagerHalt(pszMessage, pszFile, nLine) \
	ErrorManagerHalt((const char *)(pszMessage), (const char *)(pszFile), (int)(nLine))
#define FogErrorManagerAssertRaw	ErrorManagerAssert
#define FogErrorManagerHaltRaw		ErrorManagerHalt
#define FogErrorManagerWarnRaw		ErrorManagerWarn
#define FogErrorManagerAllocRaw(nSize, pszFile, nLine, nUnused) \
	ErrorManagerAlloc((int)(nSize), (const char *)(pszFile), (int)(nLine), (int)(nUnused))
#define FogErrorManagerFreeRaw(pMemory, pszFile, nLine, nUnused) \
	ErrorManagerFree((void *)(pMemory), (const char *)(pszFile), (int)(nLine), (int)(nUnused))
#define FogDataArraysSprintf		DataArraysSprintf

#endif
