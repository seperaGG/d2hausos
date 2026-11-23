#ifndef D2SERVERIMPORTS_H
#define D2SERVERIMPORTS_H
#include <windows.h>

extern "C"
{
DWORD __stdcall D2CommonOrdinal10553(void);
DWORD __stdcall D2CommonOrdinal10554(DWORD, DWORD, DWORD);
DWORD __fastcall D2CommonOrdinal10980(DWORD);
DWORD __fastcall D2CommonOrdinal10983(void);
DWORD __fastcall D2CommonOrdinal10984(void);
DWORD __fastcall D2LangOrdinal10000(DWORD);
DWORD __fastcall D2LangOrdinal10001(void);
DWORD __stdcall D2NetOrdinal10003(DWORD, DWORD);
DWORD __cdecl D2NetOrdinal10004(...);
DWORD __stdcall D2NetOrdinal10018(DWORD);
DWORD __stdcall D2NetOrdinal10022(DWORD);
DWORD __stdcall D2NetOrdinal10023(DWORD);
DWORD __stdcall D2NetOrdinal10026(DWORD);
DWORD __stdcall D2gameOrdinal10002(DWORD, DWORD);
DWORD __cdecl D2gameOrdinal10003(...);
DWORD __cdecl D2gameOrdinal10006(...);
DWORD __stdcall D2gameOrdinal10007(DWORD, DWORD, WORD, WORD, DWORD);
DWORD __cdecl D2gameOrdinal10012(...);
DWORD __stdcall D2gameOrdinal10014(WORD, void *);
DWORD __stdcall D2gameOrdinal10015(DWORD, DWORD);
DWORD __fastcall D2gameOrdinal10016(DWORD, DWORD, WORD);
DWORD __fastcall D2gameOrdinal10017(DWORD, DWORD, WORD, DWORD, DWORD);
DWORD __fastcall D2gameOrdinal10018(DWORD, DWORD, WORD, DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall D2gameOrdinal10019(WORD, DWORD, DWORD);
DWORD __stdcall D2gameOrdinal10020(DWORD);
DWORD __stdcall D2gameOrdinal10021(DWORD, DWORD, DWORD);
DWORD __fastcall D2gameOrdinal10022(DWORD, DWORD, WORD, DWORD);
DWORD __stdcall D2gameOrdinal10023(DWORD);
DWORD __cdecl D2gameOrdinal10039(...);
DWORD __cdecl D2gameOrdinal10040(...);
DWORD __fastcall D2gameOrdinal10041(DWORD, DWORD);
DWORD __fastcall D2gameOrdinal10043(DWORD, DWORD);
DWORD __fastcall D2gameOrdinal10045(DWORD, DWORD);
DWORD __cdecl D2gameOrdinal10046(...);
DWORD __stdcall D2gameOrdinal10048(DWORD, DWORD, WORD, WORD, WORD, DWORD);
DWORD __stdcall D2gameOrdinal10049(DWORD, DWORD, WORD, WORD, WORD, DWORD);
DWORD __cdecl D2gameOrdinal10050(...);
DWORD __cdecl D2gameOrdinal10051(...);
DWORD __fastcall FogOrdinal10012(void);
DWORD __fastcall FogOrdinal10013(DWORD);
DWORD __fastcall FogOrdinal10019(DWORD);
DWORD __cdecl FogOrdinal10020(...);
DWORD __cdecl FogOrdinal10026(...);
DWORD __fastcall FogOrdinal10033(DWORD, DWORD, DWORD, DWORD);
DWORD __fastcall FogOrdinal10036(void);
DWORD __fastcall FogOrdinal10037(void);
DWORD __stdcall FogOrdinal10047(DWORD, DWORD, DWORD);
DWORD __stdcall FogOrdinal10048(DWORD);
DWORD __stdcall FogOrdinal10049(DWORD, DWORD, WORD);
DWORD __stdcall FogOrdinal10050(DWORD, DWORD, DWORD);
DWORD __stdcall FogOrdinal10051(DWORD, DWORD, DWORD);
DWORD __stdcall FogOrdinal10052(DWORD);
DWORD __stdcall FogOrdinal10053(DWORD);
DWORD __stdcall FogOrdinal10055(DWORD, DWORD);
DWORD __stdcall FogOrdinal10056(DWORD, DWORD, DWORD);
DWORD __stdcall FogOrdinal10058(DWORD);
DWORD __cdecl FogOrdinal10113(...);
DWORD __fastcall FogOrdinal10139(DWORD);
DWORD __fastcall FogOrdinal10153(void);
DWORD __cdecl FogOrdinal10154(...);
DWORD __stdcall StormOrdinal401(DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall StormOrdinal403(DWORD, DWORD, DWORD, DWORD);
DWORD __stdcall StormOrdinal501(DWORD, DWORD, DWORD);
DWORD __stdcall StormOrdinal508(DWORD, DWORD, DWORD);
DWORD __stdcall StormOrdinal571(DWORD, BYTE);
DWORD __cdecl StormOrdinal578(...);
}

#define D2CommonDataTblsFreeAll() \
	D2CommonOrdinal10553()

#define D2CommonDataTblsLoadAll(hArchive, dwLoadFiles, dwLoadSubFiles) \
	D2CommonOrdinal10554((DWORD)(hArchive), (DWORD)(dwLoadFiles), (DWORD)(dwLoadSubFiles))

#define D2CommonLoggingToggle(fEnable) \
	D2CommonOrdinal10980((DWORD)(fEnable))

#define D2CommonProfDump() \
	D2CommonOrdinal10983()

#define D2CommonProfDumpAndReset() \
	D2CommonOrdinal10984()

#define D2LangStrDeleteTable() \
	D2LangOrdinal10001()

#define D2LangStrNewTable() \
	((BOOL)D2LangOrdinal10000())

#define D2LangStrNewTableRaw D2LangOrdinal10000

#define D2NetServerInit(nMode, fForceSelect) \
	((BYTE *)D2NetOrdinal10003((DWORD)(nMode), (DWORD)(fForceSelect)))

#define D2NetServerService(dwTimeout) \
	D2NetOrdinal10022((DWORD)(dwTimeout))

#define D2NetServerSetBlockCallback(dwCallback) \
	D2NetOrdinal10018((DWORD)(dwCallback))

#define D2NetServerSetBlocking(dwValue) \
	D2NetOrdinal10023((DWORD)(dwValue))

#define D2NetServerSetMaxConnections(dwCount) \
	D2NetOrdinal10026((DWORD)(dwCount))

#define D2NetServerShutdown() \
	D2NetOrdinal10004()

#define D2gameGameBroadcast(bEvent, dwArg, szText) \
	D2gameOrdinal10021((DWORD)(bEvent), (DWORD)(dwArg), (DWORD)(szText))

#define D2gameGameCloseAll() \
	D2gameOrdinal10006()

#define D2gameGameCountAllUnits(ptCensus) \
	D2gameOrdinal10020((DWORD)(ptCensus))

#define D2gameGameCountPlayers(wGameId) \
	((int)((DWORD (__stdcall *)(WORD))D2gameOrdinal10016)((WORD)(wGameId)))

#define D2gameGameCountPlayersRaw D2gameOrdinal10016

#define D2gameGameCountUnitClasses(wGameId, ptList, eType) \
	D2gameOrdinal10019((DWORD)(wGameId), (DWORD)(ptList), (DWORD)(eType))

#define D2gameGameCreateRaw D2gameOrdinal10047

#define D2gameGameGetCount() \
	((int)D2gameOrdinal10012())

#define D2gameGameGetFps() \
	D2gameOrdinal10051()

#define D2gameGameGetIds(pwIds, nMax) \
	((int)D2gameOrdinal10015((DWORD)(pwIds), (DWORD)(nMax)))

#define D2gameGameGetInfo(wGameId, ptInfo) \
	((BOOL)D2gameOrdinal10014((WORD)(wGameId), (void *)(ptInfo)))

#define D2gameGameGetPlayerInfo(wGameId, ptInfo, nMax) \
	((int)((DWORD (__stdcall *)(WORD, void *, DWORD))D2gameOrdinal10017)((WORD)(wGameId), (void *)(ptInfo), \
		(DWORD)(nMax)))

#define D2gameGameGetPlayerInfoRaw D2gameOrdinal10017

#define D2gameGameGetUnitInfo(wGameId, nCount, ptInfo, peTOUs, pdwIds) \
	((int)((DWORD (__stdcall *)(WORD, DWORD, void *, void *, void *))D2gameOrdinal10018)((WORD)(wGameId), \
		(DWORD)(nCount), (void *)(ptInfo), (void *)(peTOUs), (void *)(pdwIds)))

#define D2gameGameGetUnitInfoRaw D2gameOrdinal10018

#define D2gameGameListFree() \
	((BOOL)D2gameOrdinal10050())

#define D2gameGameProcessMessages() \
	D2gameOrdinal10003()

#define D2gameGameReceiveData(dwClientId, pbData, wLength, wTotal, bDrop) \
	D2gameOrdinal10007((DWORD)(dwClientId), (DWORD)(pbData), (wLength), (wTotal), \
		(DWORD)(bDrop))

#define D2gameGameSendText(wGameId, szText) \
	((DWORD (__stdcall *)(WORD, void *))D2gameOrdinal10022)((WORD)(wGameId), (void *)(szText))

#define D2gameGameServerInit() \
	((BOOL)D2gameOrdinal10046())

#define D2gameGameSetDataTable(ptGameDataTbl, phGameList) \
	D2gameOrdinal10002((DWORD)(ptGameDataTbl), (DWORD)(phGameList))

#define D2gameGameSetEventCallbacks(ptTable) \
	D2gameOrdinal10023((DWORD)(ptTable))

#define D2gameGuildsReceiveRoster(wGameId, dwUnused, wChunk, wOffset, wTotal, pbChunk) \
	((BOOL)D2gameOrdinal10049((DWORD)(wGameId), (DWORD)(dwUnused), (wChunk), (wOffset), \
		(wTotal), (DWORD)(pbChunk)))

#define D2gameGuildsReceiveSave(wGameId, dwUnused, wChunk, wOffset, wTotal, pbChunk) \
	((BOOL)D2gameOrdinal10048((DWORD)(wGameId), (DWORD)(dwUnused), (wChunk), (wOffset), \
		(wTotal), (DWORD)(pbChunk)))

#define D2gameTaskCreateQueue() \
	((int)((DWORD (__cdecl *)(void))D2gameOrdinal10041)())

#define D2gameTaskFreeQueues() \
	D2gameOrdinal10040()

#define D2gameTaskGetDue(nQueue, pptTask) \
	((int)D2gameOrdinal10043((DWORD)(nQueue), (DWORD)(pptTask)))

#define D2gameTaskInitTimer() \
	D2gameOrdinal10039()

#define D2gameTaskProcessGame(nQueue, ptTask) \
	D2gameOrdinal10045((DWORD)(nQueue), (DWORD)(ptTask))

#define FogAsyncDataGetFreeDiskSpaceRaw FogOrdinal10153

#define FogAsyncDataGetMemoryStatus(pdwAvailPhys, pdwTotalPhys, pdwAvailPageFile, pdwTotalPageFile) \
	FogOrdinal10154((DWORD)(pdwAvailPhys), (DWORD)(pdwTotalPhys), (DWORD)(pdwAvailPageFile), \
		(DWORD)(pdwTotalPageFile))

#define FogAsyncDataGetTime() \
	FogOrdinal10036()

#define FogD2QSSocketCreate(szIpAddr, nPort, pszName) \
	((void *)FogOrdinal10047((DWORD)(szIpAddr), (DWORD)(nPort), (DWORD)(pszName)))

#define FogD2QSSocketDestroy(pSocket) \
	FogOrdinal10048((DWORD)(pSocket))

#define FogD2QSSocketGetConnection(pSocket) \
	((void *)FogOrdinal10058((DWORD)(pSocket)))

#define FogD2QSSocketReceive(pSocket, pbBuffer, wMax) \
	FogOrdinal10051((DWORD)(pSocket), (DWORD)(pbBuffer), (DWORD)(wMax))

#define FogD2QSSocketSend(pSocket, pbData, nBytes) \
	((BOOL)FogOrdinal10049((DWORD)(pSocket), (DWORD)(pbData), (nBytes)))

#define FogD2QSSocketSendRaw(pSocket, pbData, wBytes) \
	((BOOL)FogOrdinal10050((DWORD)(pSocket), (DWORD)(pbData), (DWORD)(wBytes)))

#define FogD2QSSocketSetDispatcher(pSocket, fpDispatcher, pContext) \
	FogOrdinal10056((DWORD)(pSocket), (DWORD)(fpDispatcher), (DWORD)(pContext))

#define FogD2QSSocketStart(pSocket) \
	FogOrdinal10052((DWORD)(pSocket))

#define FogD2QSSocketStartAsync(pSocket) \
	((HANDLE)FogOrdinal10053((DWORD)(pSocket)))

#define FogD2QSSocketWait(pSocket, dwMilliseconds) \
	FogOrdinal10055((DWORD)(pSocket), (DWORD)(dwMilliseconds))

#define FogErrorManagerAlloc(nSize, pszFile, nLine, nUnused) \
	((void *)FogOrdinal10033((DWORD)(nSize), (DWORD)(pszFile), (DWORD)(nLine), (DWORD)(nUnused)))

#define FogErrorManagerGetSysInfo() \
	((void *)FogOrdinal10020())

#define FogErrorManagerLog \
	FogOrdinal10026

#define FogErrorManagerSetLogName(pszName) \
	FogOrdinal10019((DWORD)(pszName))

#define FogErrorManagerSetPoolsEnabled(fEnabled) \
	FogOrdinal10139((DWORD)(fEnabled))

#define FogMemGetTickCountRaw FogOrdinal10038

#define FogMemNothing() \
	FogOrdinal10113()

#define FogMemNothingRaw FogOrdinal10113

#define FogMemPerformanceReset() \
	FogOrdinal10037()

#define FogSafesockAddressToString(dwAddress) \
	((const char *)FogOrdinal10013((DWORD)(dwAddress)))

#define FogSafesockGetLocalAddress() \
	((const char *)FogOrdinal10012())

#define StormSMemAlloc(dwBytes, pszFile, nLine, dwFlags) \
	((void *)StormOrdinal401((DWORD)(dwBytes), (DWORD)(pszFile), (DWORD)(nLine), (DWORD)(dwFlags)))

#define StormSMemFree(pMemory, pszFile, nLine, dwFlags) \
	((BOOL)StormOrdinal403((DWORD)(pMemory), (DWORD)(pszFile), (DWORD)(nLine), (DWORD)(dwFlags)))

#define StormSStrChr(pszString, chFind) \
	((char *)StormOrdinal571((DWORD)(pszString), (DWORD)(chFind)))

#define StormSStrCmp(pszString1, pszString2, dwMaxLength) \
	((int)StormOrdinal508((DWORD)(pszString1), (DWORD)(pszString2), (DWORD)(dwMaxLength)))

#define StormSStrCopy(pszDest, pszSource, dwMaxLength) \
	StormOrdinal501((DWORD)(pszDest), (DWORD)(pszSource), (DWORD)(dwMaxLength))

#define StormSStrPrintf \
	StormOrdinal578

#endif
