#ifndef MATCH_D2MACROS_H
#define MATCH_D2MACROS_H

#include <stdlib.h>
#include <string.h>

#include "FogImports.h"

#ifndef D2_THIS_FILE
#error "D2_THIS_FILE must be defined per translation unit"

#define D2_OPAQUE(x)	((void)0)
#define arrsize(a)		(sizeof(a) / sizeof((a)[0]))

#endif

#define FogAssert(pszExpr, pszFile, nLine)	FogErrorManagerAssertRaw((pszExpr), (pszFile), (nLine))

#define D2ASSERT(expr) \
	if (!(expr)) { FogAssert(#expr, D2_THIS_FILE, __LINE__); exit(-1); }

#define D2FATAL(pszMessage) \
	{ FogErrorManagerHaltRaw((pszMessage), D2_THIS_FILE, __LINE__); exit(-1); }

#define D2ALLOC2(nSize)	((void *)FogErrorManagerAllocRaw((DWORD)(nSize), (DWORD)D2_THIS_FILE, __LINE__, 0))
#define D2FREE2(p)		FogErrorManagerFreeRaw((DWORD)(p), (DWORD)D2_THIS_FILE, __LINE__, 0)

#define D2_OPAQUE(x)	((void)0)
#define arrsize(a)		(sizeof(a) / sizeof((a)[0]))

#define D2ASSERTMSG(expr, pszMessage) \
	if (!(expr)) { FogAssert(pszMessage, D2_THIS_FILE, __LINE__); exit(-1); }
#define D2WARN(expr) \
	if (!(expr)) FogErrorManagerWarnRaw(#expr, D2_THIS_FILE, __LINE__)
#define D2WARNMSG(pszMessage)	FogErrorManagerWarnRaw((pszMessage), D2_THIS_FILE, __LINE__)
#define D2ALLOCPOOL(pPool, nSize)	((void *)FogErrorManagerPoolAllocRaw((DWORD)(pPool), (DWORD)(nSize), (DWORD)D2_THIS_FILE, \
	__LINE__, 0))
#define D2FREEPOOL(pPool, p)	FogErrorManagerPoolFreeRaw((DWORD)(pPool), (DWORD)(p), (DWORD)D2_THIS_FILE, __LINE__, 0)
#define D2ASYNCLOAD(pPool, pszFile, fAsync, dwOffset, nSize, pbBuffer, pCallback) \
	((void *)(INT_PTR)FogAsyncDataLoadRaw((DWORD)(pPool), (DWORD)(pszFile), (DWORD)(fAsync), (DWORD)(dwOffset), \
	(DWORD)(nSize), (DWORD)(pbBuffer), (DWORD)(pCallback), (DWORD)D2_THIS_FILE, __LINE__))

#endif
