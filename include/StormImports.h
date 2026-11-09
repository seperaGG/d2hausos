#ifndef MATCH_STORMIMPORTS_H
#define MATCH_STORMIMPORTS_H
#include <windows.h>

extern "C" DWORD __stdcall StormOrdinal401(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal403(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal501(DWORD, DWORD, DWORD);

#define StormSMemAlloc(dwBytes, pszFile, nLine, dwFlags) \
	((void *)StormOrdinal401((DWORD)(dwBytes), (DWORD)(pszFile), (DWORD)(nLine), (DWORD)(dwFlags)))

#define StormSMemFree(pMemory, pszFile, nLine, dwFlags) \
	((BOOL)StormOrdinal403((DWORD)(pMemory), (DWORD)(pszFile), (DWORD)(nLine), (DWORD)(dwFlags)))

#define StormSStrCopy(pszDest, pszSource, dwMaxLength) \
	StormOrdinal501((DWORD)(pszDest), (DWORD)(pszSource), (DWORD)(dwMaxLength))

extern "C" DWORD __stdcall StormOrdinal252(DWORD);
extern "C" DWORD __stdcall StormOrdinal266(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal276(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal503(DWORD, DWORD, DWORD);
#define StormSFileCloseArchiveRaw StormOrdinal252
#define StormSFileOpenArchive(pszName, nPriority, dwFlags, phArchive) \
	((BOOL)StormOrdinal266((DWORD)(pszName), (DWORD)(nPriority), (DWORD)(dwFlags), (DWORD)(phArchive)))
#define StormSFileGetFileName(hFile, pszBuffer, dwBufferChars) \
	((BOOL)StormOrdinal276((DWORD)(hFile), (DWORD)(pszBuffer), (DWORD)(dwBufferChars)))
#define StormSStrAppend(pszDest, pszSource, dwMaxLength) \
	StormOrdinal503((DWORD)(pszDest), (DWORD)(pszSource), (DWORD)(dwMaxLength))

#endif
