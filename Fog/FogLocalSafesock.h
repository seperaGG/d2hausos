#ifndef FOGLOCALSAFESOCK_H
#define FOGLOCALSAFESOCK_H
#include <windows.h>

#define StormSFileEnableDirectAccess(dwFlags) \
	((BOOL)StormOrdinal263((DWORD)(dwFlags)))
#define StormSFileGetFileSize(hFile, pdwFileSizeHigh) \
	StormOrdinal265((DWORD)(hFile), (DWORD)(pdwFileSizeHigh))
#define StormSFileOpenFile(pszName, phFile) \
	((BOOL)StormOrdinal267((DWORD)(pszName), (DWORD)(phFile)))
#define StormSFileSetBasePath(pszPath) \
	((BOOL)StormOrdinal270((DWORD)(pszPath)))
#define StormSFileSetFilePointer(hFile, lDistance, plDistanceHigh, dwMethod) \
	StormOrdinal271((DWORD)(hFile), (DWORD)(lDistance), (DWORD)(plDistanceHigh), (DWORD)(dwMethod))
#define StormSFileSetIoErrorMode(dwMode, pfnCallback) \
	((BOOL)StormOrdinal274((DWORD)(dwMode), (DWORD)(pfnCallback)))
#define StormSFileEnableSeekOptimization(fEnable) \
	StormOrdinal286((DWORD)(fEnable))
#define StormSFileReadFileEx(hFile, pBuffer, dwBytes, pdwRead, ptOverlapped, ptCallback) \
	((BOOL)StormOrdinal287((DWORD)(hFile), (DWORD)(pBuffer), (DWORD)(dwBytes), (DWORD)(pdwRead), \
		(DWORD)(ptOverlapped), (DWORD)(ptCallback)))
#define StormSRegLoadString(pszKey, pszValue, dwFlags, pszBuffer, dwBufferChars) \
	((BOOL)StormOrdinal422((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(pszBuffer), \
		(DWORD)(dwBufferChars)))
#define StormSRegSaveString(pszKey, pszValue, dwFlags, pszString) \
	((BOOL)StormOrdinal425((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(pszString)))
#define StormSStrLen(pszString) \
	StormOrdinal506((DWORD)(pszString))
#define StormSStrChr(pszString, chFind) \
	((char *)StormOrdinal571((DWORD)(pszString), (DWORD)(chFind)))
#define StormSStrChrR(pszString, chFind) \
	((char *)StormOrdinal572((DWORD)(pszString), (DWORD)(chFind)))
#define StormSetOption(dwOption, pValue, dwSize) \
	((BOOL)StormOrdinal304((DWORD)(dwOption), (DWORD)(pValue), (DWORD)(dwSize)))
extern "C" DWORD __stdcall StormOrdinal263(DWORD);
extern "C" DWORD __stdcall StormOrdinal265(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal267(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal270(DWORD);
extern "C" DWORD __stdcall StormOrdinal271(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal274(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal286(DWORD);
extern "C" DWORD __stdcall StormOrdinal287(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal304(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal422(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal425(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal506(DWORD);
extern "C" DWORD __stdcall StormOrdinal571(DWORD, BYTE);
extern "C" DWORD __stdcall StormOrdinal572(DWORD, BYTE);
extern "C" DWORD __stdcall StormOrdinal253(DWORD);
extern "C" DWORD __cdecl StormOrdinal578(...);
#define StormSStrPrintf	StormOrdinal578

#endif
