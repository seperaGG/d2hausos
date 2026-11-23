#ifndef MATCH_D2COMMONEXT_H
#define MATCH_D2COMMONEXT_H

#include <windows.h>
#include "StormImports.h"

extern "C" DWORD __stdcall StormOrdinal494(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal508(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal509(DWORD, DWORD, DWORD);
#define StormSMemZero(pDest, dwBytes) \
	StormOrdinal494((DWORD)(pDest), (DWORD)(dwBytes))
#define StormSStrCmp(pszString1, pszString2, dwMaxLength) \
	((int)StormOrdinal508((DWORD)(pszString1), (DWORD)(pszString2), (DWORD)(dwMaxLength)))
#define StormSStrCmpI(pszString1, pszString2, dwMaxLength) \
	((int)StormOrdinal509((DWORD)(pszString1), (DWORD)(pszString2), (DWORD)(dwMaxLength)))

extern "C" DWORD __stdcall D2CMPOrdinal10078(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10079(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10081(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10082(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10085(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10087(DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10088(DWORD, BYTE, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10098(DWORD, DWORD);
#define D2CMPPalShiftGetTransform(eClass, eTransform) \
	((BYTE *)D2CMPOrdinal10098((DWORD)(eClass), (DWORD)(eTransform)))
#define D2CMPTileLibraryAddProject(ptTileLibrary, szName) \
	D2CMPOrdinal10087((DWORD)(ptTileLibrary), (DWORD)(szName))
#define D2CMPTileLibraryFindTiles(ptTileLibrary, dwValue1, dwValue2, dwValue3, pptTiles, nMax) \
	((int)((DWORD (__stdcall *)(void *, DWORD, DWORD, DWORD, void *, \
		DWORD))D2CMPOrdinal10088)((void *)(ptTileLibrary), (DWORD)(dwValue1), (DWORD)(dwValue2), \
		(DWORD)(dwValue3), (void *)(pptTiles), (DWORD)(nMax)))
#define D2CMPTileGetRarity(hTile) \
	D2CMPOrdinal10081((DWORD)(hTile))
#define D2CMPTileGetStyle(hTile) \
	D2CMPOrdinal10078((DWORD)(hTile))
#define D2CMPTileGetFlags(hTile) \
	D2CMPOrdinal10079((DWORD)(hTile))
#define D2CMPTileGetCollisionMask(hTile) \
	((BYTE *)D2CMPOrdinal10085((DWORD)(hTile)))
#define D2CMPTileGetSequence(hTile) \
	D2CMPOrdinal10082((DWORD)(hTile))

extern "C" DWORD __fastcall D2LangOrdinal10003(DWORD);
extern "C" DWORD __fastcall D2LangOrdinal10004(WORD);
extern "C" WORD __stdcall D2LangOrdinal10013(const char *, void **);
extern "C" __declspec(dllimport) DWORD __fastcall D2LangOrdinal10042(DWORD, DWORD, DWORD);
extern int gnD2LangSymPad0; extern int gnD2LangSymPad1; extern int gnD2LangSymPad2;
#define D2LangStrGetStringIndexByKey(pszKey, ppString) \
	((WORD)D2LangOrdinal10013((pszKey), (ppString)))
#define D2LangStrGetStringByKey(pszKey) \
	((void *)D2LangOrdinal10003((DWORD)(pszKey)))
#define D2LangUnicodestrncpy(pszDst, pszSrc, nCount) \
	((void *)D2LangOrdinal10042((DWORD)(pszDst), (DWORD)(pszSrc), (DWORD)(nCount)))
#define D2LangStrGetStringByIndex(wStringId) \
	((void *)D2LangOrdinal10004((WORD)(wStringId)))

#endif
