#ifndef D2RE_FOG_MEM_H
#define D2RE_FOG_MEM_H

#include <windows.h>

struct D2POOLSYSTEM;

void * __fastcall MemPoolAlloc(D2POOLSYSTEM *ptPoolSystem, int nSize, const char *pszFile, int nLine);
void __fastcall MemPoolFree(D2POOLSYSTEM *ptPoolSystem, void **ppMemory, const char *pszFile, int nLine);
void * __fastcall MemPoolRealloc(D2POOLSYSTEM *ptPoolSystem, void *pMemory, int nSize, const char *pszFile,
	int nLine);
DWORD __cdecl MemPoolSystemInit(D2POOLSYSTEM **pptPoolSystem, int nLargestSize, int nMaxPoolBlocks);
void __cdecl MemPoolSystemDestroy(D2POOLSYSTEM *ptPoolSystem);

extern DWORD gdwTraceUnfreedMemory;

#endif
