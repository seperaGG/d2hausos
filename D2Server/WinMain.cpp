#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\CORE\\WinMain.cpp"
#include <time.h>
#include "D2Server.h"
#include "D2Macros.h"
#include "GameTbl.h"

#define IDD_STATUS			102
#define IDC_DISPLAY			1010
#define IDC_UPTIME			1017
#define IDC_LOG				1019
#define IDC_CONSOLELINE		1021
#define IDM_PERF			1011
#define IDM_CONSOLE			1012
#define IDM_EXIT			1013
#define IDM_PERFLOG			1017
#define WM_SERVERLOG		(WM_USER + 1)

#define MAX_LOGLINES		100
#define SERVER_MAXGAMES		1024
#define NUM_STARTUP			45
#define LIST_PERIOD			30000
#define STATUS_PERIOD		60000

typedef void (__fastcall *PFNSTARTUP)(INIT_PARAMS *pOptions);

typedef BOOL (__fastcall *PFNEVCLOSEGAME)(DWORD dwGameId);
typedef BOOL (__fastcall *PFNEVLEAVEGAME)(DWORD dwGameId, DWORD dwClass, DWORD dwLevel, DWORD dwExperience,
	DWORD dwExperienceHigh, DWORD dwFlags, const char *pszName, const char *pszSummary);
typedef BOOL (__fastcall *PFNEVGETCHARACTER)(const char *pszName, DWORD dwClientId);
typedef BOOL (__fastcall *PFNEVSAVECHARACTER)(const char *pszName, const char *pszAccount, const BYTE *pbData,
	int nLength, DWORD dwArg);
typedef BOOL (__cdecl *PFNEVLOG)(BOOL fToFile, char *pszFormat, ...);
typedef BOOL (__fastcall *PFNEVENTERGAME)(DWORD dwArg, const char *pszText, WORD wClass, WORD wLevel,
	WORD wArg3);
typedef BOOL (__fastcall *PFNEVFINDTOKEN)(const char *pszName, DWORD dwToken, DWORD dwGameId,
	char *pszAccount, DWORD *pdwArg);
typedef BOOL (__fastcall *PFNEVSAVEGUILD)(const char *pszTag, const BYTE *pbData, int nLength);
typedef BOOL (__fastcall *PFNEVUNLOCK)(const char *pszName);
typedef BOOL (__fastcall *PFNEVENT9)(const char *pszText1, BYTE bArg, const char *pszText2, DWORD dwArg);
typedef BOOL (__fastcall *PFNEVENT10)(const char *pszName, WORD wClass, DWORD dwLevel,
	unsigned __int64 qwExperience, DWORD dwFlags);
typedef BOOL (__fastcall *PFNEVENT11)(DWORD dwArg, const char *pszText, WORD wClass, BYTE bArg);
struct SERVEREVENTS
{
 	PFNEVCLOSEGAME		fpCloseGame;
 	PFNEVLEAVEGAME		fpLeaveGame;
 	PFNEVGETCHARACTER	fpGetDatabaseCharacter;
 	PFNEVSAVECHARACTER	fpSaveDatabaseCharacter;
 	PFNEVLOG			fpServerLogMessage;
 	PFNEVENTERGAME		fpEnterGame;
 	PFNEVFINDTOKEN		fpFindPlayerToken;
 	PFNEVSAVEGUILD		fpSaveDatabaseGuild;
 	PFNEVUNLOCK			fpUnlockDatabaseCharacter;
 	PFNEVENT9			pfnGuildEvent;
 	PFNEVENT10			pfnReportPlayer;
 	PFNEVENT11			pfnLevelUp;
};

struct SERVERSYSINFO
{
 	BYTE		unobserved_00[20];
 	SYSTEM_INFO	tSystemInfo;
};

typedef BOOL (__fastcall *PFNLANGLOAD)(D2ARCHIVE *hArchive);
#define LangLoad			((PFNLANGLOAD)D2LangStrNewTableRaw)

static void __fastcall ServerStartupNothingFirst(INIT_PARAMS *pOptions);
static void __fastcall ServerStartupNothingSecond(INIT_PARAMS *pOptions);
static void __fastcall ServerStartupMcpIp(INIT_PARAMS *pOptions);
static void __fastcall ServerStartupBnetIp(INIT_PARAMS *pOptions);
static void __fastcall ServerStartupLog(INIT_PARAMS *pOptions);

extern const char * const gpszServerExe = "D2SERVE.EXE";
static BOOL sgfBnetIp = TRUE;
static BOOL sgfRealm = TRUE;
HWND ghStatusDialog;
static WNDPROC sgpfnEditProc;
static long sgtStartTime;
static LONG sgnWorkers;
LADDERLIMITS gtLadderLimits;
MCPADDRESS gszMcpAddress;
static GAMEDATATBL sgtGameDataTbl;
static HINSTANCE sghServerInstance;
static DWORD sgdwServerUnused = 0;
static DWORD sghGameList = 0;
static BOOL sgfShutdown = 0;
static DWORD sgdwListTick = 0;
static DWORD sgdwStatusTick = 0;

static PFNSTARTUP sgapfnStartup[NUM_STARTUP] =
{
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	ServerStartupBnetIp,
	ServerStartupMcpIp,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	ServerStartupNothingFirst,
	NULL, NULL, NULL, NULL, NULL,
	ServerStartupLog,
	NULL, NULL, NULL, NULL,
	ServerStartupNothingSecond,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static SERVEREVENTS sgtEvents =
{
	ServerCloseGame,
	ServerLeaveGame,
	ServerLoadCharacter,
	ServerSaveCharacter,
	ServerLog,
	ServerEnterGame,
	PlayerTokenFind,
	ServerSaveGuild,
	ServerUnlockCharacter,
	ServerSendTwoTexts,
	ServerSendCharacterInfo,
	ServerSendIdClassText,
};

void __fastcall ServerRunStartup(INIT_PARAMS *pOptions)
{
	int i;

	for (i = 0; i < NUM_STARTUP; i++)
	{
		if (sgapfnStartup[i])
			sgapfnStartup[i](pOptions);
	}
}

static void __fastcall ServerStartupNothingFirst(INIT_PARAMS *pOptions)
{
}

static void __fastcall ServerStartupNothingSecond(INIT_PARAMS *pOptions)
{
}

static void __fastcall ServerStartupMcpIp(INIT_PARAMS *pOptions)
{
	if (pOptions->szMcpIp[0] != '0')
	{
		sgfRealm = TRUE;
		lstrcpyA(gszMcpAddress, pOptions->szMcpIp);
		return;
	}
	sgfRealm = FALSE;
}

static void __fastcall ServerStartupBnetIp(INIT_PARAMS *pOptions)
{
	sgfBnetIp = pOptions->szBattleNetIp[0] != '0';
}

static void __fastcall ServerStartupLog(INIT_PARAMS *pOptions)
{
	if (pOptions->bLog)
		D2CommonLoggingToggle(1);
}

BOOL __cdecl ServerLog(BOOL fToFile, char *pszFormat, ...)
{
	char szLine[256];
	char *pszCopy;
	va_list pArgs;

	va_start(pArgs, pszFormat);
	vsprintf(szLine, pszFormat, pArgs);
	if (fToFile)
		FogErrorManagerLog(szLine);
#line 294
	pszCopy = (char *)D2ALLOC2(strlen(szLine) + 1);
	StormSStrCopy(pszCopy, szLine, 2147483647);
	return PostMessageA(ghStatusDialog, WM_SERVERLOG, 0, (LPARAM)pszCopy);
}

BOOL __fastcall ServerUnlockCharacter(const char *pszName)
{
	return CharServUnlockCharacter(pszName);
}

BOOL __fastcall ServerEnterGame(DWORD dwArg, const char *pszText, WORD wClass, WORD wLevel, WORD wArg3)
{
	return ToMCPSendPlayerEntered(dwArg, pszText, wClass, wLevel, wArg3);
}

BOOL __fastcall ServerLeaveGame(DWORD dwGameId, DWORD dwClass, DWORD dwLevel, DWORD dwExperience,
	DWORD dwExperienceHigh, DWORD dwFlags, const char *pszName, const char *pszSummary)
{
	CharServUnlockCharacter(pszName);
	return ToMCPPlayerLeft(dwGameId, dwClass, dwLevel, dwExperience, dwExperienceHigh, dwFlags, pszName,
		pszSummary);
}

BOOL __fastcall ServerLoadCharacter(const char *pszName, DWORD dwClientId)
{
	return CharServLoadCharacter(pszName, dwClientId);
}

BOOL __fastcall ServerSaveCharacter(const char *pszName, const char *pszAccount, const BYTE *pbData,
	int nLength, DWORD dwArg)
{
	if ((int)*(const WORD *)pbData != nLength)
	{
		FogErrorManagerLog("Invalid size of record in SrvDatabaseSaveCharacter for '%s'", pszName);
#line 375
		D2ASSERT(0);
	}
	return CharServSaveCharacter(pszName, pszAccount, pbData, nLength, dwArg);
}

BOOL __fastcall ServerCloseGame(DWORD dwGameId)
{
	return ToMCPCloseGame(dwGameId);
}

BOOL __fastcall ServerSaveGuild(const char *pszTag, const BYTE *pbData, int nLength)
{
	return ToMCPGuildSave(pszTag, pbData, nLength);
}

BOOL __fastcall ServerSendTwoTexts(const char *pszText1, BYTE bArg, const char *pszText2, DWORD dwArg)
{
	return ToMCPSendTwoTexts(pszText1, bArg, pszText2, dwArg);
}

BOOL __fastcall ServerSendCharacterInfo(const char *pszName, WORD wClass, DWORD dwLevel,
	unsigned __int64 qwExperience,
	DWORD dwFlags)
{
	return ToMCPCharInfo(pszName, wClass, dwLevel, qwExperience, dwFlags);
}

BOOL __fastcall ServerSendIdClassText(DWORD dwArg, const char *pszText, WORD wClass, BYTE bArg)
{
	return ToMCPSendIdClassText(dwArg, pszText, wClass, bArg);
}

HWND __fastcall ServerGetStatusDialog(void)
{
	return ghStatusDialog;
}

LRESULT CALLBACK ServerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szLine[256];

	switch (uMsg)
	{
	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;
	case WM_CHAR:
		if (wParam == VK_RETURN)
		{
			if (SendDlgItemMessageA(ghStatusDialog, IDC_CONSOLELINE, WM_GETTEXT, sizeof szLine, (LPARAM)szLine))
				ConsoleExecute(ghStatusDialog, szLine);
			szLine[0] = 0;
			SendDlgItemMessageA(ghStatusDialog, IDC_CONSOLELINE, WM_SETTEXT, 0, (LPARAM)szLine);
			SetFocus(GetDlgItem(ghStatusDialog, IDC_CONSOLELINE));
			return 1;
		}
		break;
	}
	return CallWindowProcA(sgpfnEditProc, hWnd, uMsg, wParam, lParam);
}

int __fastcall ServerGetMaxGames(void)
{
	return SERVER_MAXGAMES;
}

int __fastcall ServerGetMaxPlayers(void)
{
	return SERVER_MAXGAMES;
}

long __fastcall ServerGetUptime(void)
{
	long tNow;

	tNow = time(&tNow);
	return (long)difftime(tNow, sgtStartTime);
}

BOOL CALLBACK ServerDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hLine;
	char szText[64];
	HMENU hMenu;
	DWORD dwSeconds;
	int nLast;

	switch (uMsg)
	{
	case WM_DESTROY:
		hLine = GetDlgItem(hDlg, IDC_CONSOLELINE);
		SetWindowLongA(hLine, GWL_WNDPROC, (LONG)sgpfnEditProc);
		return FALSE;

	case WM_INITDIALOG:
		hLine = GetDlgItem(hDlg, IDC_CONSOLELINE);
		sgpfnEditProc = (WNDPROC)SetWindowLongA(hLine, GWL_WNDPROC, (LONG)ServerEditProc);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		case IDM_EXIT:
			D2gameGameCloseAll();
			if (PerfGetDialog())
				PerfCloseDialog();
			if (ConsoleGetDialog())
				ConsoleCloseDialog();
			PostQuitMessage(0);
			return TRUE;

		case IDC_DISPLAY:
			dwSeconds = (DWORD)ServerGetUptime();
			sprintf(szText, "%d:%02d:%02d", dwSeconds / 3600, dwSeconds % 3600 / 60, dwSeconds % 60);
			SendDlgItemMessageA(ghStatusDialog, IDC_UPTIME, WM_SETTEXT, 0, (LPARAM)szText);
			DisplayRefresh();
			return TRUE;

		case IDM_PERF:
			hMenu = GetMenu(hDlg);
			if (PerfGetDialog())
			{
				CheckMenuItem(hMenu, IDM_PERF, MF_UNCHECKED);
				PerfCloseDialog();
			}
			else
			{
				CheckMenuItem(hMenu, IDM_PERF, MF_CHECKED);
				PerfCreateDialog(hDlg);
			}
			return FALSE;

		case IDM_CONSOLE:
			hMenu = GetMenu(hDlg);
			if (ConsoleGetDialog())
			{
				CheckMenuItem(hMenu, IDM_CONSOLE, MF_UNCHECKED);
				ConsoleCloseDialog();
			}
			else
			{
				CheckMenuItem(hMenu, IDM_CONSOLE, MF_CHECKED);
				ConsoleCreateDialog(hDlg);
			}
			return FALSE;

		case IDM_PERFLOG:
			hMenu = GetMenu(hDlg);
			if (PerfGetLogging())
			{
				CheckMenuItem(hMenu, IDM_PERFLOG, MF_UNCHECKED);
				PerfSetLogging(FALSE);
			}
			else
			{
				CheckMenuItem(hMenu, IDM_PERFLOG, MF_CHECKED);
				PerfSetLogging(TRUE);
			}
			return FALSE;
		}
		break;

	case WM_SERVERLOG:
		SendDlgItemMessageA(ghStatusDialog, IDC_LOG, LB_INSERTSTRING, (WPARAM)-1, lParam);
		nLast = (int)SendDlgItemMessageA(ghStatusDialog, IDC_LOG, LB_GETCOUNT, 0, 0) - 1;
		if (nLast >= MAX_LOGLINES)
		{
			SendDlgItemMessageA(ghStatusDialog, IDC_LOG, LB_DELETESTRING, 0, 0);
			nLast--;
		}
		SendDlgItemMessageA(ghStatusDialog, IDC_LOG, LB_SETCURSEL, nLast, 0);
#line 606
		D2FREE2(lParam);
		break;
	}
	return FALSE;
}

DWORD WINAPI ServerHousekeepThread(LPVOID pvParam)
{
	BYTE abMsg[1024];
	D2SERVERSTATUS tStatus;
	DWORD dwNow;
	WORD wLength;

	while (!sgfShutdown)
	{
		CharServDrain();
		if (!FogD2QSSocketWait(gpMcpConnection, 0))
		{
			while (1)
			{
				wLength = (WORD)FogD2QSSocketReceive(gpMcpConnection, abMsg, sizeof abMsg);
				if (!wLength)
					break;
				FromMCPDispatch(abMsg, wLength);
			}
		}
		dwNow = GetTickCount();
		if (!sgdwListTick || dwNow - sgdwListTick > LIST_PERIOD)
		{
			sgdwListTick = dwNow;
			ToMCPSendTick();
		}
		if (!sgdwStatusTick || dwNow - sgdwStatusTick > STATUS_PERIOD)
		{
			tStatus.bFramesPerSecond = (BYTE)D2gameGameGetFps();
			tStatus.field_02 = 0;
			tStatus.wNumGames = (WORD)D2gameGameGetCount();
			sgdwStatusTick = dwNow;
			ToMCPStatus(&tStatus);
		}
		CharServCheckMCP();
		if (!CharServWatchdog())
		{
			D2gameGameCloseAll();
			if (PerfGetDialog())
				PerfCloseDialog();
			if (ConsoleGetDialog())
				ConsoleCloseDialog();
			FogErrorManagerLog(
				"FATAL: Lost connection to character server.  Check character server ip address\n"
				"in registry or verify that this computer's ip address is listed in the accept\n"
				"list on the character server in cs.ini.");
			sgfShutdown = TRUE;
		}
		Sleep(1);
	}
	return 1;
}

void __fastcall ServerShutdownByMCP(void)
{
	FogErrorManagerLog("Shutdown Remotely by MCP");
	D2gameGameCloseAll();
	if (PerfGetDialog())
		PerfCloseDialog();
	if (ConsoleGetDialog())
		ConsoleCloseDialog();
	sgfShutdown = TRUE;
}

int __fastcall ServerWorkerStep(DWORD dwQueue, int *pnRound)
{
	void *ptTask;
	int nWait;

	nWait = D2gameTaskGetDue(dwQueue, &ptTask);
	if (!ptTask || !((*pnRound)++ & 7))
	{
		if (D2NetServerService(nWait < 0 ? 0 : nWait))
			D2gameGameProcessMessages();
	}
	if (ptTask)
		D2gameTaskProcessGame(dwQueue, ptTask);
	return 1;
}

DWORD WINAPI ServerWorkerThread(LPVOID pvParam)
{
	DWORD dwQueue;
	int nRound;

	dwQueue = D2gameTaskCreateQueue();
	nRound = 0;
	while (1)
	{
		if (sgfShutdown)
			break;
		ServerWorkerStep(dwQueue, &nRound);
	}
	InterlockedDecrement(&sgnWorkers);
	return 1;
}

void __fastcall ServerNothingExport(void)
{
}

int __stdcall ServerWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, INIT_PARAMS *pOptions, int nCmdShow)
{

	sghServerInstance = hInstance;
	FogErrorManagerSetPoolsEnabled(TRUE);
	CharServOpenArchives();
	ServerRunStartup(pOptions);
	ghStatusDialog = CreateDialogParamA(sghServerInstance, MAKEINTRESOURCE(IDD_STATUS), NULL, ServerDlgProc,
		0);
	ShowWindow(ghStatusDialog, nCmdShow);
	UpdateWindow(ghStatusDialog);
	FogErrorManagerSetLogName("D2SDebug");
	if (IniRead())
	{
		if (sgfRealm)
			CharServLoadBnclient();
		if (LangLoad(CharServGetArchive()))
		{
			D2CommonDataTblsLoadAll(CharServGetArchive(), 1, 0);
			(void)D2NetServerInit(0, 0);
			D2NetServerSetMaxConnections(IniGetMultiClient());
			D2NetServerSetBlocking(1);
			D2gameGameServerInit();
			if (sgfRealm)
			{
				CharServConnectMCP();
				CharServConnect(ghStatusDialog);
				D2gameGameSetEventCallbacks(&sgtEvents);
			}
			D2gameGameSetDataTable(&sgtGameDataTbl, &sghGameList);
			sgtStartTime = time(&sgtStartTime);
			D2gameTaskInitTimer();
			{
			HANDLE hThread;
			DWORD dwThreadId;
			int nThreads;
			SERVERSYSINFO *pSysInfo;

			if (sgfRealm)
			{
				hThread = CreateThread(NULL, 0, ServerHousekeepThread, NULL, 0, &dwThreadId);
				CloseHandle(hThread);
			}
			pSysInfo = (SERVERSYSINFO *)FogErrorManagerGetSysInfo();
			nThreads = (int)pSysInfo->tSystemInfo.dwNumberOfProcessors;
			while (nThreads--)
			{
				DWORD dwWorkerId;

				InterlockedIncrement(&sgnWorkers);
				hThread = CreateThread(NULL, 0, ServerWorkerThread, NULL, 0, &dwWorkerId);
				if (!hThread)
					InterlockedDecrement(&sgnWorkers);
				else
					CloseHandle(hThread);
			}
			}
			DisplayProfileInit();
			DisplaySetRefreshRate(2);
			DisplayBusyStart();
			ServerLog(TRUE, "[SERVER]  Finished Loading...");
			{
			MSG tMsg;
			BOOL fRunning;

			fRunning = TRUE;
			tMsg.wParam = 0;
			gqwBusyCount = 0;
			while (1)
			{
				if (sgfShutdown)
					break;
				if (PeekMessageA(&tMsg, NULL, 0, 0, PM_NOREMOVE))
				{
					fRunning = GetMessageA(&tMsg, NULL, 0, 0);
					if (!IsDialogMessageA(PerfGetDialog(), &tMsg)
						|| !IsDialogMessageA(ConsoleGetDialog(), &tMsg))
					{
						TranslateMessage(&tMsg);
						DispatchMessageA(&tMsg);
					}
				}
				else
					Sleep(1);
				if (!fRunning)
					break;
			}
			}
		}
	}
	sgfShutdown = TRUE;
	while (sgnWorkers > 0)
		Sleep(100);
	sgfShutdown = FALSE;
	DisplayFreeGames();
	D2gameTaskFreeQueues();
	D2NetServerShutdown();
	if (sgfRealm)
	{
		CharServDisconnectMCP();
		CharServDisconnect();
	}
	D2gameGameListFree();
	D2CommonDataTblsFreeAll();
	D2LangStrDeleteTable();
	D2CommonProfDump();
	CharServCloseArchives();
	return 0;
}
