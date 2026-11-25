#ifndef FOGLOCALASYNC_H
#define FOGLOCALASYNC_H
#include <windows.h>

DWORD __fastcall SafesockOpenStormFile(DWORD dwFileName, DWORD dwFileHandleOut);
DWORD __fastcall SafesockCloseStormFile(DWORD dwArg);
DWORD __fastcall SafesockReadStormFile(DWORD dwFile, DWORD dwBuffer, DWORD dwBytes, DWORD dwBytesReadOut,
	DWORD dwOverlapped, DWORD dwCallback);
DWORD __fastcall SafesockGetStormFileSize(DWORD dwFile, DWORD dwFileSizeHighOut);
DWORD __fastcall SafesockSetStormFilePointer(DWORD dwFile, DWORD dwDistance, DWORD dwDistanceHighOut,
	DWORD dwMethod);
BOOL __fastcall SafesockGetSavePath(char *pszPath, DWORD dwSize);
void __cdecl ErrorManagerError(int nLevel, const char *pszMessage, const char *pszFile, int nLine);
struct D2POOLSYSTEM;
void * __fastcall ErrorManagerPoolAlloc(D2POOLSYSTEM *pPool, int nSize, const char *pszFile, int nLine, int nUnused);
BOOL __fastcall ErrorManagerPoolFree(D2POOLSYSTEM *pPool, void *pMemory, const char *pszFile, int nLine, int nUnused);

#define FogSafesockOpenStormFile(dwFileName, dwFileHandleOut) \
	SafesockOpenStormFile((DWORD)(dwFileName), (DWORD)(dwFileHandleOut))
#define FogSafesockCloseStormFile(dwArg) \
	SafesockCloseStormFile((DWORD)(dwArg))
#define FogSafesockReadStormFile(dwFile, dwBuffer, dwBytes, dwBytesReadOut, dwOverlapped, dwCallback) \
	SafesockReadStormFile((DWORD)(dwFile), (DWORD)(dwBuffer), (DWORD)(dwBytes), (DWORD)(dwBytesReadOut), \
		(DWORD)(dwOverlapped), (DWORD)(dwCallback))
#define FogSafesockGetStormFileSize(dwFile, dwFileSizeHighOut) \
	SafesockGetStormFileSize((DWORD)(dwFile), (DWORD)(dwFileSizeHighOut))
#define FogSafesockSetStormFilePointer(dwFile, dwDistance, dwDistanceHighOut, dwMethod) \
	SafesockSetStormFilePointer((DWORD)(dwFile), (DWORD)(dwDistance), (DWORD)(dwDistanceHighOut), (DWORD)(dwMethod))
#define FogErrorManagerError(nLevel, pszMessage, pszFile, nLine) \
	ErrorManagerError((int)(nLevel), (const char *)(pszMessage), (const char *)(pszFile), (int)(nLine))
void __cdecl ErrorManagerLog(const char *pszFormat, ...);
#define FogErrorManagerLog	ErrorManagerLog
#define FogErrorManagerPoolAllocRaw(pPool, nSize, pszFile, nLine, nUnused) \
	ErrorManagerPoolAlloc((D2POOLSYSTEM *)(pPool), (int)(nSize), (const char *)(pszFile), (int)(nLine), (int)(nUnused))
#define FogErrorManagerPoolFreeRaw(pPool, pMemory, pszFile, nLine, nUnused) \
	ErrorManagerPoolFree((D2POOLSYSTEM *)(pPool), (void *)(pMemory), (const char *)(pszFile), (int)(nLine), (int)(nUnused))

extern "C" DWORD __stdcall StormOrdinal282(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal283(DWORD);
extern "C" DWORD __stdcall StormOrdinal284(DWORD);
extern "C" DWORD __stdcall StormOrdinal285(DWORD);
#define StormSFilePrioritizeRequest(pKey, fUrgent)	StormOrdinal282((DWORD)(pKey), (DWORD)(fUrgent))
#define StormSFileCancelRequest(pKey)	((BOOL)StormOrdinal283((DWORD)(pKey)))
#define StormSFileSetAsyncRate(dwRate)	StormOrdinal284((DWORD)(dwRate))
#define StormSFileSetDataChunkSize(dwSize)	StormOrdinal285((DWORD)(dwSize))

extern "C" DWORD __stdcall StormOrdinal491(DWORD, DWORD, DWORD);
#define StormSMemCopy(pDest, pSource, dwBytes) \
	StormOrdinal491((DWORD)(pDest), (DWORD)(pSource), (DWORD)(dwBytes))

#endif
