#include "D2Server.h"
#include "Bnclient.h"

static HMODULE sghBnclient = 0;
void * gpMcpConnection = NULL;
static DWORD sgdwMcpCheckTick = 0;
static BNCLIENTINTERFACE *sgptBnclient = NULL;
static DWORD sgdwBnetUnused = 0;

static BOOL sgfMcpConnected = TRUE;

BOOL __fastcall CharServConnectMCP(void)
{
	BYTE bHello;

	StormSStrCopy(gszMcpAddress, IniGetMcpIp(), 2147483647);
	gpMcpConnection = FogD2QSSocketCreate(gszMcpAddress, 6112, "GameServer->MCP");
	FogD2QSSocketSetDispatcher(gpMcpConnection, FromMCPDispatchSecondTable, 0);
	FogD2QSSocketStart(gpMcpConnection);
	bHello = 2;
	FogD2QSSocketSendRaw(gpMcpConnection, &bHello, 1);
	ToMCPSendIdentityRequest();
	D2NetServerSetBlockCallback(CharServHackListing);
	ServerLog(TRUE, "[SERVER]  Connected to MCP at %s", gszMcpAddress);
	return TRUE;
}

BOOL __fastcall CharServCheckMCP(void)
{
	DWORD dwTick;
	BYTE bHello;

	dwTick = GetTickCount();
	if (!sgdwMcpCheckTick)
		sgdwMcpCheckTick = dwTick;
	if (dwTick - sgdwMcpCheckTick < 60000)
		return TRUE;
	sgdwMcpCheckTick = dwTick;
	if (FogD2QSSocketGetConnection(gpMcpConnection))
	{
		sgfMcpConnected = TRUE;
		return TRUE;
	}
	if (sgfMcpConnected)
		sgfMcpConnected = FALSE;
	FogD2QSSocketDestroy(gpMcpConnection);
	StormSStrCopy(gszMcpAddress, IniGetMcpIp(), 2147483647);
	gpMcpConnection = FogD2QSSocketCreate(gszMcpAddress, 6112, "GameServer->MCP");
	FogD2QSSocketSetDispatcher(gpMcpConnection, FromMCPDispatchSecondTable, 0);
	FogD2QSSocketStart(gpMcpConnection);
	bHello = 2;
	FogD2QSSocketSendRaw(gpMcpConnection, &bHello, 1);
	ToMCPSendIdentityRequest();
	D2NetServerSetBlockCallback(CharServHackListing);
	ServerLog(TRUE, "[SERVER]  Connected to MCP at %s", gszMcpAddress);
	return TRUE;
}

void __fastcall CharServDisconnectMCP(void)
{
	if (gpMcpConnection)
	{
		D2NetServerSetBlockCallback(0);
		FogD2QSSocketDestroy(gpMcpConnection);
	}
}

BOOL __fastcall CharServReturnFalse(void)
{
	return FALSE;
}

BOOL __fastcall CharServHackListing(DWORD dwAddress)
{
	ServerLog(TRUE, "[SERVER]  Hack listing IP %s", FogSafesockAddressToString(dwAddress));
	return ToMCPSendDword(dwAddress);
}

BOOL __fastcall CharServLoadBnclient(void)
{
	char szText[100];
	HMODULE hModule;
	BNCLIENTINTERFACE *(__cdecl *pfnQuery)(void);

	if (sgptBnclient)
		return FALSE;
	hModule = LoadLibraryA("Bnclient.dll");
	if (!hModule)
	{
		sprintf(szText, "Cannot load Bnclient.dll: Error %d", GetLastError());
		MessageBoxA(NULL, szText, "D2Multi", MB_ICONHAND);
		return FALSE;
	}
	pfnQuery = (BNCLIENTINTERFACE *(__cdecl *)(void))GetProcAddress(hModule, "QueryInterface");
	if (!pfnQuery)
	{
		sprintf(szText, "Unable to QueryInterface for Bnclient.dll: Error %d", GetLastError());
		MessageBoxA(NULL, szText, "D2Multi", MB_ICONHAND);
		return FALSE;
	}
	sgptBnclient = pfnQuery();
	return TRUE;
}

BOOL __fastcall CharServBnclientDone(void)
{
	return sgptBnclient->pfnGetProgress() == 101;
}

BOOL __fastcall CharServUnloadBnclient(void)
{
	int nStatus;

	nStatus = 0;
	sgptBnclient->pfnApplyPatch(&nStatus);
	if (nStatus)
	{
		TerminateProcess(GetCurrentProcess(), 0);
		return FALSE;
	}
	FreeLibrary(sghBnclient);
	return TRUE;
}
