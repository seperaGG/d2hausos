#ifndef MATCH_FOGIMPORTS_H
#define MATCH_FOGIMPORTS_H
#include <windows.h>

extern "C" DWORD __fastcall FogOrdinal10014(DWORD);
extern "C" DWORD __cdecl FogOrdinal10021(...);
extern "C" DWORD __cdecl FogOrdinal10022(...);
extern "C" DWORD __fastcall FogOrdinal10033(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10034(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10114(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10115(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10117(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10119(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10120(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10122(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10123(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10125(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10126(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10127(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10128(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10129(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10131(DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10133(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10151(DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10155(DWORD, DWORD);

#define FogSafesockResolve(pszHost) \
	((const char *)FogOrdinal10014((DWORD)(pszHost)))

#define FogQServerCreate(nMode, nNumMsgLists, nPort, fForce98, pfnParse, pfnAccept, pfnClose, pfnLocalSend) \
	((void *)FogOrdinal10114((DWORD)(nMode), (DWORD)(nNumMsgLists), (DWORD)(nPort), (DWORD)(fForce98), \
		(DWORD)(pfnParse), (DWORD)(pfnAccept), (DWORD)(pfnClose), (DWORD)(pfnLocalSend)))

#define FogQServerDestroy(ptServer, pbData, nBytes) \
	((BOOL)FogOrdinal10115((DWORD)(ptServer), (DWORD)(pbData), (DWORD)(nBytes)))

#define FogQServerWait(ptServer, dwMilliseconds) \
	FogOrdinal10117((DWORD)(ptServer), (DWORD)(dwMilliseconds))

#define FogQServerGetMessage(ptServer, nMsgType, pbBuffer, dwBuffSize) \
	((int)FogOrdinal10119((DWORD)(ptServer), (DWORD)(nMsgType), (DWORD)(pbBuffer), (DWORD)(dwBuffSize)))

#define FogQServerSend(ptServer, hSocket, pbData, nBytes) \
	((int)FogOrdinal10120((DWORD)(ptServer), (DWORD)(hSocket), (DWORD)(pbData), (DWORD)(nBytes)))

#define FogQServerGetAddressString(ptServer, hSocket, pszAddress) \
	FogOrdinal10122((DWORD)(ptServer), (DWORD)(hSocket), (DWORD)(pszAddress))

#define FogQServerDisconnect(ptServer, hSocket) \
	FogOrdinal10123((DWORD)(ptServer), (DWORD)(hSocket))

#define FogQServerClose(ptServer, hSocket) \
	FogOrdinal10125((DWORD)(ptServer), (DWORD)(hSocket))

#define FogQServerSetBlockHook(ptServer, pfnHook) \
	FogOrdinal10126((DWORD)(ptServer), (DWORD)(pfnHook))

#define FogQServerSetUnusedHook(ptServer, pfnHook) \
	FogOrdinal10127((DWORD)(ptServer), (DWORD)(pfnHook))

#define FogQServerSetClientValue(ptServer, hSocket, dwValue) \
	FogOrdinal10128((DWORD)(ptServer), (DWORD)(hSocket), (DWORD)(dwValue))

#define FogQServerGetClientValue(ptServer, hSocket) \
	FogOrdinal10129((DWORD)(ptServer), (DWORD)(hSocket))

#define FogQServerInject(ptServer, pbData, nBytes, hSocket) \
	((BOOL)FogOrdinal10131((DWORD)(ptServer), (DWORD)(pbData), (DWORD)(nBytes), (DWORD)(hSocket)))

#define FogQServerPump(ptServer, dwMilliseconds) \
	((BOOL)FogOrdinal10133((DWORD)(ptServer), (DWORD)(dwMilliseconds)))

#define FogQServerSetBlocking(ptServer, fBlocking) \
	((BOOL)FogOrdinal10151((DWORD)(ptServer), (DWORD)(fBlocking)))

#define FogQServerSetAddressLimit(ptServer, dwLimit) \
	FogOrdinal10155((DWORD)(ptServer), (DWORD)(dwLimit))

#define FogErrorManagerAssertRaw FogOrdinal10021
#define FogErrorManagerHaltRaw FogOrdinal10022
#define FogErrorManagerAllocRaw FogOrdinal10033
#define FogErrorManagerFreeRaw FogOrdinal10034

extern "C" DWORD __cdecl FogOrdinal10024(...);
#define FogErrorManagerAlloc(nSize, pszFile, nLine, nUnused) \
	((void *)FogOrdinal10033((DWORD)(nSize), (DWORD)(pszFile), (DWORD)(nLine), (DWORD)(nUnused)))
extern "C" DWORD __fastcall FogOrdinal10075(DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10076(DWORD);
extern "C" DWORD __fastcall FogOrdinal10077(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10078(DWORD, DWORD);
extern "C" DWORD __fastcall FogOrdinal10079(DWORD, DWORD, DWORD, DWORD);
#define FogErrorManagerError(nLevel, pszMessage, pszFile, nLine) \
	FogOrdinal10024((DWORD)(nLevel), (DWORD)(pszMessage), (DWORD)(pszFile), (DWORD)(nLine))
#define FogErrorManagerHalt(pszMessage, pszFile, nLine) \
	FogOrdinal10022((DWORD)(pszMessage), (DWORD)(pszFile), (DWORD)(nLine))
#define FogErrorManagerAssertRaw FogOrdinal10021
#define FogErrorManagerHaltRaw FogOrdinal10022
#define FogErrorManagerAllocRaw FogOrdinal10033
#define FogErrorManagerFreeRaw FogOrdinal10034
#define FogSafesockOpenStormFileRaw FogOrdinal10075
#define FogSafesockCloseStormFileRaw FogOrdinal10076
#define FogSafesockReadStormFileRaw FogOrdinal10077
#define FogSafesockGetStormFileSizeRaw FogOrdinal10078
#define FogSafesockSetStormFilePointerRaw FogOrdinal10079

#endif
