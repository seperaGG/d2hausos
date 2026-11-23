#ifndef MATCH_ROOMTILELOCAL_H
#define MATCH_ROOMTILELOCAL_H
#ifndef D2REALLOCPOOL
#define D2REALLOCPOOL(pPool, p, nSize)	((void *)FogErrorManagerPoolReallocRaw((DWORD)(pPool), (DWORD)(p), (DWORD)(nSize), (DWORD)D2_THIS_FILE, __LINE__, 0))
#endif
#endif
