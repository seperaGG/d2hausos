#include "D2Server.h"

extern "C" DWORD __fastcall FogOrdinal10038(void);

#define IDD_CONSOLE			104
#define IDC_CONSOLELIST		1006
#define IDC_CONSOLEEDIT		1007
#define CONSOLE_LINE		256
#define CONSOLE_MAXLINES	64

#define NUM_CONSOLECMD		9

typedef BOOL (__fastcall *PFNCONSOLECMD)(HWND hDlg, char *pszArgs);
struct CONSOLECMD
{
 	char			szName[256];
 	PFNCONSOLECMD	pfnCmd;
 	char			szHelp[256];
};

static HWND sghConsole = NULL;

static void ConsoleRunLine(HWND hDlg, char *pszLine);
static BOOL __fastcall ConsoleCmdProfile(HWND hDlg, char *pszArgs);
static BOOL __fastcall ConsoleCmdGlobalMessage(HWND hDlg, char *pszArgs);
static BOOL __fastcall ConsoleCmdLocalMessage(HWND hDlg, char *pszArgs);
static BOOL __fastcall ConsoleCmdPerf(HWND hDlg, char *pszArgs);
static BOOL __fastcall ConsoleCmdMem(HWND hDlg, char *pszArgs);
static BOOL __fastcall ConsoleCmdLocalMem(HWND hDlg, char *pszArgs);

static CONSOLECMD sgatConsoleCmd[NUM_CONSOLECMD] =
{
	{ "?", ConsoleCmdHelp, "Display Console Commands" },
	{ "help", ConsoleCmdHelp, "Display Console Commands" },
	{ "profile", ConsoleCmdProfile, "Restart Profiling" },
	{ "globalmessage", ConsoleCmdGlobalMessage, "Send message to all clients" },
	{ "gm", ConsoleCmdGlobalMessage, "Send message to all clients" },
	{ "localmessage", ConsoleCmdLocalMessage, "Send message to one game" },
	{ "perf", ConsoleCmdPerf, "Performance Logging" },
	{ "mem", ConsoleCmdMem, "Memory Dump" },
	{ "lmem", ConsoleCmdLocalMem, "Local Memory Dump" },
};

HWND __fastcall ConsoleGetDialog(void)
{
	return sghConsole;
}

BOOL __fastcall ConsoleCreateDialog(HWND hWndParent)
{
	HINSTANCE hInstance;

	hInstance = (HINSTANCE)GetWindowLongA(hWndParent, GWL_HINSTANCE);
	if (sghConsole)
		return FALSE;
	sghConsole = CreateDialogParamA(hInstance, MAKEINTRESOURCEA(IDD_CONSOLE), hWndParent, ConsoleDlgProc, 0);
	return sghConsole != NULL;
}

LRESULT __fastcall ConsoleCloseDialog(void)
{
	return SendMessageA(sghConsole, WM_CLOSE, 0, 0);
}

BOOL CALLBACK ConsoleDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szLine[CONSOLE_LINE];

	switch (uMsg)
	{
	case WM_CLOSE:
		sghConsole = NULL;
		EndDialog(hDlg, 0);
		return TRUE;

	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			if (SendDlgItemMessageA(hDlg, IDC_CONSOLEEDIT, WM_GETTEXT, CONSOLE_LINE, (LPARAM)szLine))
				ConsoleRunLine(hDlg, szLine);
			szLine[0] = 0;
			SendDlgItemMessageA(hDlg, IDC_CONSOLEEDIT, WM_SETTEXT, 0, (LPARAM)szLine);
			SetFocus(GetDlgItem(hDlg, IDC_CONSOLEEDIT));
			break;

		case IDCANCEL:
			sghConsole = NULL;
			EndDialog(hDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

static BOOL __fastcall ConsoleCmdProfile(HWND hDlg, char *pszArgs)
{
	D2CommonProfDumpAndReset();
	ServerLog(FALSE, "Profiler Restarted");
	return TRUE;
}

static BOOL __fastcall ConsoleCmdGlobalMessage(HWND hDlg, char *pszArgs)
{
	char szText[256];

	strcpy(szText, "Diablo: ");
	strcat(szText, pszArgs);
	D2gameGameBroadcast(0, 0, szText);
	ServerLog(FALSE, szText);
	return TRUE;
}

static BOOL __fastcall ConsoleCmdLocalMessage(HWND hDlg, char *pszArgs)
{
	char szText[256];
	D2SERVERGAMEINFO *ptGame;

	strcpy(szText, "Diablo: ");
	strcat(szText, pszArgs);
	ptGame = DisplayGetSelectedGame();
	if (ptGame)
	{
		D2gameGameSendText(ptGame->adwStats[0], szText);
		ServerLog(FALSE, szText);
	}
	else
		ServerLog(FALSE, "ERROR: No game selected to send to.");
	return TRUE;
}

static BOOL __fastcall ConsoleCmdPerf(HWND hDlg, char *pszArgs)
{
	DisplayToLower(pszArgs);
	switch (pszArgs[0])
	{
	case 'c':
		FogMemPerformanceReset();
		ServerLog(FALSE, "Clear Performance Accumulators.");
		return TRUE;

	case 'd':
		FogMemGetTickCountRaw();
		ServerLog(FALSE, "Dump Performance Accumulators.");
		return TRUE;
	}
	ServerLog(FALSE, "ERROR: Invalid performance param, use (c)lear or (d)ump.");
	return TRUE;
}

static BOOL __fastcall ConsoleCmdMem(HWND hDlg, char *pszArgs)
{
	DisplayToLower(pszArgs);
	switch (pszArgs[0])
	{
	case 'p':
		FogMemNothingRaw(NULL, 0);
		return TRUE;

	case 'l':
		FogMemNothingRaw(NULL, 1);
		return TRUE;

	case 'c':
		FogMemNothingRaw(NULL, 2);
		return TRUE;

	case 'n':
		FogMemNothingRaw(NULL, 3);
		return TRUE;
	}
	ServerLog(FALSE, "ERROR: Invalid memory param, use (p)ool, (l)ine, (c)lear, or (n)um.");
	return TRUE;
}

static BOOL __fastcall ConsoleCmdLocalMem(HWND hDlg, char *pszArgs)
{
	D2SERVERGAMEINFO *ptGame;
	void *pMemPool;

	DisplayToLower(pszArgs);
	ptGame = DisplayGetSelectedGame();
	if (ptGame)
	{
		pMemPool = ptGame->pMemPool;
		switch (pszArgs[0])
		{
		case 'p':
			FogMemNothingRaw(pMemPool, 0);
			return TRUE;

		case 'l':
			FogMemNothingRaw(pMemPool, 1);
			return TRUE;

		case 'c':
			FogMemNothingRaw(pMemPool, 2);
			return TRUE;

		case 'n':
			FogMemNothingRaw(pMemPool, 3);
			return TRUE;
		}
		ServerLog(FALSE, "ERROR: Invalid memory param, use (p)ool, (l)ine, (c)lear, or (n)um.");
		return TRUE;
	}
	ServerLog(FALSE, "ERROR: No game selected to memory dump.");
	return FALSE;
}

static __inline BOOL ConsoleIsSeparator(char ch)
{
	char szSep[2];
	int k;

	szSep[0] = ' ';
	szSep[1] = '\t';
	for (k = 0; k < 2; k++)
		if (ch == szSep[k])
			return TRUE;
	return FALSE;
}

static __inline void ConsoleRunLine(HWND hDlg, char *pszLine)
{
	char szCmd[CONSOLE_LINE];
	char szRest[CONSOLE_LINE];
	char *pszCmd;
	char *pszArgs;
	char ch;
	int i;
	int j;

	pszCmd = szCmd;
	pszArgs = szRest;
	pszArgs[0] = 0;
	i = 0;
	for (;;)
	{
		ch = pszLine[i];
		if (ConsoleIsSeparator(ch) || ch == 0)
			break;
		pszCmd[i] = pszLine[i];
		if (++i >= CONSOLE_LINE)
			break;
	}
	pszCmd[i] = 0;
	if (pszLine[i])
	{
		while (1)
		{
			if (i >= CONSOLE_LINE)
				break;
			if (!ConsoleIsSeparator(pszLine[i]))
				break;
			if (!pszLine[++i])
				break;
		}
		for (j = 0; pszLine[i] && i < CONSOLE_LINE; j++)
			pszArgs[j] = pszLine[i++];
		pszArgs[j] = 0;
	}
	DisplayToLower(pszCmd);
	for (i = 0; i < NUM_CONSOLECMD; i++)
	{
		if (!strcmp(pszCmd, sgatConsoleCmd[i].szName))
		{
			if (!sgatConsoleCmd[i].pfnCmd(hDlg, pszArgs))
				ServerLog(FALSE, pszLine);
			return;
		}
	}
	strcpy(pszLine, "ERROR: Unknown command \"");
	strcat(pszLine, pszCmd);
	strcat(pszLine, "\"");
	ServerLog(FALSE, pszLine);
}

void __fastcall ConsoleExecute(HWND hDlg, char *pszLine)
{
	ConsoleRunLine(hDlg, pszLine);
}

BOOL __fastcall ConsoleCmdHelp(HWND hDlg, char *pszArgs)
{
	char szText[256];
	int i;

	if (sghConsole)
		ConsoleAddLine("Console Commands:");
	for (i = 0; i < NUM_CONSOLECMD; i++)
	{
		strcpy(szText, "    ");
		strcat(szText, sgatConsoleCmd[i].szName);
		strcat(szText, " - ");
		strcat(szText, sgatConsoleCmd[i].szHelp);
		ServerLog(FALSE, szText);
	}
	return TRUE;
}

void __fastcall ConsoleAddLine(const char *pszText)
{
	int nCount;

	if (!sghConsole)
		return;
	if ((int)SendDlgItemMessageA(sghConsole, IDC_CONSOLELIST, LB_GETCOUNT, 0, 0) > CONSOLE_MAXLINES)
		SendDlgItemMessageA(sghConsole, IDC_CONSOLELIST, LB_DELETESTRING, 0, 0);
	SendDlgItemMessageA(sghConsole, IDC_CONSOLELIST, LB_ADDSTRING, 0, (LPARAM)pszText);
	nCount = (int)SendDlgItemMessageA(sghConsole, IDC_CONSOLELIST, LB_GETCOUNT, 0, 0);
	SendDlgItemMessageA(sghConsole, IDC_CONSOLELIST, LB_SETCURSEL, nCount - 1, 0);
}
