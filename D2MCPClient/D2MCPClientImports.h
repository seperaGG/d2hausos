#ifndef MATCH_D2MCPCLIENT_IMPORTS_H
#define MATCH_D2MCPCLIENT_IMPORTS_H

#include <windows.h>

extern "C" DWORD __fastcall FogOrdinal10013(DWORD);
extern "C" DWORD __fastcall FogOrdinal10015(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10047(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10048(DWORD);
extern "C" DWORD __stdcall FogOrdinal10049(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10050(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10051(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10052(DWORD);
extern "C" DWORD __stdcall FogOrdinal10054(DWORD);
extern "C" DWORD __stdcall FogOrdinal10056(DWORD, DWORD, DWORD);
extern "C" DWORD __stdcall FogOrdinal10058(DWORD);

#define FogSafesockAddressToString(dwAddress) \
	((const char *)FogOrdinal10013((DWORD)(dwAddress)))
#define FogSafesockGetError(ptSock, pdwStep, pdwError) \
	((BOOL)FogOrdinal10015((DWORD)(ptSock), (DWORD)(pdwStep), (DWORD)(pdwError)))
#define FogD2QSSocketCreate(szIpAddr, nPort, pszName) \
	((void *)FogOrdinal10047((DWORD)(szIpAddr), (DWORD)(nPort), (DWORD)(pszName)))
#define FogD2QSSocketDestroy(pSocket) \
	FogOrdinal10048((DWORD)(pSocket))
#define FogD2QSSocketSend(pSocket, pbData, nBytes) \
	((BOOL)FogOrdinal10049((DWORD)(pSocket), (DWORD)(pbData), (DWORD)(nBytes)))
#define FogD2QSSocketSendRaw(pSocket, pbData, wBytes) \
	((BOOL)FogOrdinal10050((DWORD)(pSocket), (DWORD)(pbData), (DWORD)(wBytes)))
#define FogD2QSSocketReceive(pSocket, pbBuffer, wMax) \
	FogOrdinal10051((DWORD)(pSocket), (DWORD)(pbBuffer), (DWORD)(wMax))
#define FogD2QSSocketStart(pSocket) \
	FogOrdinal10052((DWORD)(pSocket))
#define FogD2QSSocketStop(pSocket) \
	FogOrdinal10054((DWORD)(pSocket))
#define FogD2QSSocketSetDispatcher(pSocket, fpDispatcher, pContext) \
	FogOrdinal10056((DWORD)(pSocket), (DWORD)(fpDispatcher), (DWORD)(pContext))
#define FogD2QSSocketGetConnection(pSocket) \
	((void *)FogOrdinal10058((DWORD)(pSocket)))

#endif
