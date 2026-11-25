#ifndef MATCH_BNCLIENT_IMPORTS_H
#define MATCH_BNCLIENT_IMPORTS_H
#include <windows.h>
#include "StormImports.h"

extern "C" DWORD __stdcall D2CMPOrdinal10006(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10008(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10009(BYTE);
extern "C" DWORD __stdcall D2CMPOrdinal10010(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10011(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10013(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10014(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10015(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10016(DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10023(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10027(DWORD);
extern "C" DWORD __stdcall D2CMPOrdinal10028(DWORD);
extern "C" DWORD __stdcall D2LangOrdinal10006(DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10000(void);
extern "C" DWORD __fastcall FogOrdinal10003(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10004(DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10005(DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10006(DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10007(DWORD);
extern "C" DWORD __fastcall FogOrdinal10011(DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10080(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10082(DWORD);
extern "C" DWORD __fastcall FogOrdinal10084(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10085(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10146(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal251(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal253(DWORD);
extern "C" DWORD __stdcall StormOrdinal265(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal268(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal269(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal279(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal280(DWORD);
extern "C" DWORD __stdcall StormOrdinal421(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal422(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal424(DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal425(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal426(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal428(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal465(DWORD);
extern "C" DWORD __stdcall StormOrdinal491(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal494(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal506(DWORD);
extern "C" DWORD __stdcall StormOrdinal541(DWORD);
extern "C" DWORD __stdcall StormOrdinal542(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal550(DWORD, DWORD);
extern "C" DWORD __stdcall StormOrdinal571(DWORD, BYTE);

#define D2CMPCelFileFree(pCelFile) \
	D2CMPOrdinal10027((DWORD)(pCelFile))

#define D2CMPCelFileFreeSerialized(pSerializedFile) \
	D2CMPOrdinal10028((DWORD)(pSerializedFile))

#define D2CMPCelFileSerialize(pFile, ppSerializedFile, pdwSize) \
	((BOOL)D2CMPOrdinal10023((DWORD)(pFile), (DWORD)(ppSerializedFile), (DWORD)(pdwSize)))

#define D2CMPCelListAddImage(dwHandle, pbImage, dwImageByteStride, dwPixelHeight, dwPixelWidth, dwOffsetX, \
		dwOffsetY, dwValue) \
	D2CMPOrdinal10015((DWORD)(dwHandle), (DWORD)(pbImage), (DWORD)(dwImageByteStride), \
		(DWORD)(dwPixelHeight), (DWORD)(dwPixelWidth), (DWORD)(dwOffsetX), (DWORD)(dwOffsetY), \
		(DWORD)(dwValue))

#define D2CMPCelListBuild(dwHandle, ppFile) \
	D2CMPOrdinal10016((DWORD)(dwHandle), (DWORD)(ppFile))

#define D2CMPCelListCreate(format, dwNumDirections, pdwKeyColor, f24Bit) \
	D2CMPOrdinal10013((DWORD)(format), (DWORD)(dwNumDirections), (DWORD)(pdwKeyColor), (DWORD)(f24Bit))

#define D2CMPCelListFree(dwHandle) \
	D2CMPOrdinal10014((DWORD)(dwHandle))

#define D2CMPPaletteLoad(pszPalette, PaletteStartEntry, PaletteEndEntry) \
	((BOOL)D2CMPOrdinal10006((DWORD)(pszPalette), (DWORD)(PaletteStartEntry), (DWORD)(PaletteEndEntry)))

#define D2CMPPaletteQuantize(pbRgb, pbOut, dwCount, a4) \
	D2CMPOrdinal10008((DWORD)(pbRgb), (DWORD)(pbOut), (DWORD)(dwCount), (DWORD)(a4))

#define D2CMPPaletteSetDarkToBlack(fDark) \
	D2CMPOrdinal10011((DWORD)(fDark))

#define D2CMPPaletteSetKeyColor(dwColor) \
	D2CMPOrdinal10009((DWORD)(dwColor))

#define D2CMPPaletteSetMatchKeyColor(fMatch) \
	D2CMPOrdinal10010((DWORD)(fMatch))

#define D2LangStrGetLanguageName(pszDst, eField) \
	D2LangOrdinal10006((DWORD)(pszDst), (DWORD)(eField))

#define FogSafesockCanRead(ptSock) \
	((BOOL)FogOrdinal10007((DWORD)(ptSock)))

#define FogSafesockClose(ptSock) \
	((DWORD (__fastcall *)(DWORD))FogOrdinal10004)((DWORD)(ptSock))

#define FogSafesockCloseHandle(hObject) \
	((BOOL)FogOrdinal10082((DWORD)(hObject)))

#define FogSafesockConnectWithFlags(pszHost, nPort, pfnThread, pThreadArg, pszName, dwFlags) \
	((void *)FogOrdinal10003((DWORD)(pszHost), (DWORD)(nPort), (DWORD)(pfnThread), (DWORD)(pThreadArg), \
		(DWORD)(pszName), (DWORD)(dwFlags)))

#define FogSafesockCreateFile(pszFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, \
		dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile) \
	((HANDLE)FogOrdinal10080((DWORD)(pszFileName), (DWORD)(dwDesiredAccess), (DWORD)(dwShareMode), \
		(DWORD)(lpSecurityAttributes), (DWORD)(dwCreationDisposition), (DWORD)(dwFlagsAndAttributes), \
		(DWORD)(hTemplateFile)))

#define FogSafesockGetPeerName(ptSock, ptAddress, pnLength) \
	((BOOL)FogOrdinal10011((DWORD)(ptSock), (DWORD)(ptAddress), (DWORD)(pnLength)))

#define FogSafesockRecv(ptSock, pBuffer, nLength) \
	((int)FogOrdinal10005((DWORD)(ptSock), (DWORD)(pBuffer), (DWORD)(nLength)))

#define FogSafesockSend(ptSock, pBuffer, nLength) \
	((int)FogOrdinal10006((DWORD)(ptSock), (DWORD)(pBuffer), (DWORD)(nLength)))

#define FogSafesockSetFilePointer(hFile, lDistance, plDistanceHigh, dwMoveMethod) \
	FogOrdinal10085((DWORD)(hFile), (DWORD)(lDistance), (DWORD)(plDistanceHigh), (DWORD)(dwMoveMethod))

#define FogSafesockSetNonBlocking(ptSock, dwNonBlocking) \
	((BOOL)FogOrdinal10146((DWORD)(ptSock), (DWORD)(dwNonBlocking)))

#define FogSafesockStartup() \
	((BOOL)FogOrdinal10000())

#define FogSafesockWriteFile(hFile, lpBuffer, dwToWrite, lpdwWritten, lpOverlapped) \
	((BOOL)FogOrdinal10084((DWORD)(hFile), (DWORD)(lpBuffer), (DWORD)(dwToWrite), (DWORD)(lpdwWritten), \
		(DWORD)(lpOverlapped)))

#define StormSErrSetLastError(dwError) \
	StormOrdinal465((DWORD)(dwError))

#define StormSFileAuthenticateArchive(hArchive, pdwResult) \
	((BOOL)StormOrdinal251((DWORD)(hArchive), (DWORD)(pdwResult)))

#undef StormSFileCloseArchiveRaw
#define StormSFileCloseArchiveRaw(a, b) StormOrdinal252((DWORD)(b))
#define StormSFileCloseFileRaw(a, b) StormOrdinal253((DWORD)(b))

#define StormSFileGetFileSize(hFile, pdwFileSizeHigh) \
	StormOrdinal265((DWORD)(hFile), (DWORD)(pdwFileSizeHigh))

#define StormSFileLoadFile(pszName, ppBuffer, pdwSize, dwExtra, ptOverlapped) \
	((BOOL)StormOrdinal279((DWORD)(pszName), (DWORD)(ppBuffer), (DWORD)(pdwSize), (DWORD)(dwExtra), \
		(DWORD)(ptOverlapped)))

#define StormSFileOpenFileEx(hArchive, pszName, dwFlags, phFile) \
	((BOOL)StormOrdinal268((DWORD)(hArchive), (DWORD)(pszName), (DWORD)(dwFlags), (DWORD)(phFile)))

#define StormSFileReadFile(hFile, pBuffer, dwBytes, pdwRead, ptOverlapped) \
	((BOOL)StormOrdinal269((DWORD)(hFile), (DWORD)(pBuffer), (DWORD)(dwBytes), (DWORD)(pdwRead), \
		(DWORD)(ptOverlapped)))

#define StormSFileUnloadFile(pBuffer) \
	((BOOL)StormOrdinal280((DWORD)(pBuffer)))

#define StormSLogAddIndent(dwLog, nDelta) \
	((int)StormOrdinal550((DWORD)(dwLog), (DWORD)(nDelta)))

#define StormSLogClose(dwLog) \
	StormOrdinal541((DWORD)(dwLog))

#define StormSLogCreate(pszFile, dwUnused, pdwLog) \
	((BOOL)StormOrdinal542((DWORD)(pszFile), (DWORD)(dwUnused), (DWORD)(pdwLog)))

#define StormSLogWriteRaw StormOrdinal548

#define StormSMemCopy(pDest, pSource, dwBytes) \
	StormOrdinal491((DWORD)(pDest), (DWORD)(pSource), (DWORD)(dwBytes))

#define StormSMemZero(pDest, dwBytes) \
	StormOrdinal494((DWORD)(pDest), (DWORD)(dwBytes))

#define StormSRegDeleteValue(pszKey, pszValue, dwFlags) \
	((BOOL)StormOrdinal428((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags)))

#define StormSRegLoadData(pszKey, pszValue, dwFlags, pData, dwBufferSize, pdwDataSize) \
	((BOOL)StormOrdinal421((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(pData), \
		(DWORD)(dwBufferSize), (DWORD)(pdwDataSize)))

#define StormSRegLoadString(pszKey, pszValue, dwFlags, pszBuffer, dwBufferChars) \
	((BOOL)StormOrdinal422((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(pszBuffer), \
		(DWORD)(dwBufferChars)))

#define StormSRegSaveData(pszKey, pszValue, dwFlags, pData, dwDataSize) \
	((BOOL)StormOrdinal424((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(pData), \
		(DWORD)(dwDataSize)))

#define StormSRegSaveString(pszKey, pszValue, dwFlags, pszString) \
	((BOOL)StormOrdinal425((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(pszString)))

#define StormSRegSaveValue(pszKey, pszValue, dwFlags, dwValue) \
	((BOOL)StormOrdinal426((DWORD)(pszKey), (DWORD)(pszValue), (DWORD)(dwFlags), (DWORD)(dwValue)))

#define StormSStrChr(pszString, chFind) \
	((char *)StormOrdinal571((DWORD)(pszString), (DWORD)(chFind)))

#define StormSStrLen(pszString) \
	StormOrdinal506((DWORD)(pszString))

#endif
