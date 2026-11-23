#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\CORE\\Display.cpp"
#include "D2Server.h"
#include <commctrl.h>
#include "D2Macros.h"

typedef int (__stdcall *PFNGAMECOUNTPLAYERS)(WORD wGameId);
typedef int (__stdcall *PFNGAMEGETPLAYERINFO)(WORD wGameId, D2SERVERUNITINFO *ptInfo, int nMax);
typedef int (__stdcall *PFNGAMEGETUNITINFO)(WORD wGameId, int nCount, D2SERVERUNITINFO *ptInfo,
	DWORD *peTypes, DWORD *pdwIds);
#define GameCountPlayers	((PFNGAMECOUNTPLAYERS)D2gameGameCountPlayersRaw)
#define GameGetPlayerInfo	((PFNGAMEGETPLAYERINFO)D2gameGameGetPlayerInfoRaw)
#define GameGetUnitInfo		((PFNGAMEGETUNITINFO)D2gameGameGetUnitInfoRaw)

#define IDC_GAMELIST		1000
#define IDC_GAMEINFO		1001
#define IDC_UNITLIST		1002
#define IDC_UNKNOWN3EB		1003
#define IDC_BUSYBAR			1004
#define IDC_BUSYTEXT		1005
#define IDC_MONSTERTEXT		1009
#define IDC_PLAYERS			1014
#define IDC_GAMES			1015
#define IDC_MONSTERS		1016
#define IDC_DETAIL			1018

#define NUM_GAMEINFO		25
#define NUM_PANES			5

typedef void (__fastcall *PFNDISPLAYPANE)(D2SERVERGAMEINFO *ptGame, int nArg);
struct DISPLAYPANE
{
 	int				nLine;
 	PFNDISPLAYPANE	pfnShow;
 	int				nArg;
};

typedef char GAMENAME[16];

static __int64 sgqwRefreshPeriod = 0;
static LARGE_INTEGER sgtProfileStart = { 0 };
__int64 gqwBusyCount = 0;
static __int64 sgqwBusyUnused = 0;
static LARGE_INTEGER sgtCounterFreq = { 0 };
static __int64 sgqwRefreshNext = 0;
static BOOL sgfPerfCounter = 0;
static DWORD sgdwBusy = 0;
static LARGE_INTEGER sgtBusyStart = { 0 };
D2SERVERGAMEINFO *gptGames = NULL;
static D2SERVERGAMEINFO *sgptInfoGame = NULL;
static GAMENAME sgszSelected = { 0 };
static BOOL sgfSelChanged = 0;
static DWORD sgdwDetailUnit = 0;

static void __fastcall PanePlayers(D2SERVERGAMEINFO *ptGame, int nArg);
static void __fastcall PaneUnitClasses(D2SERVERGAMEINFO *ptGame, int nType);
static D2SERVERGAMEINFO * __fastcall DisplayFindGame(WORD wGameId);
static void __fastcall DisplayAddGame(WORD wGameId);
static void __fastcall DisplayPruneGames(void);
static void __fastcall DisplayShowBusy(void);
static void __fastcall DisplayShowGameInfo(void);

static int sgnLastPane = -1;
static int sgnLastBusy = -1;

static DISPLAYPANE sgatPanes[NUM_PANES] =
{
	{ 3, PanePlayers, 0 },
	{ 4, PaneUnitClasses, 1 },
	{ 5, PaneUnitClasses, 2 },
	{ 6, PaneUnitClasses, 3 },
	{ 7, PaneUnitClasses, 4 },
};

static DWORD sgdwDetailGame = (DWORD)-1;
static DWORD sgdwDetailField = (DWORD)-1;

void __fastcall DisplaySetRefreshRate(int nPerSecond)
{
	LARGE_INTEGER tFreq;
	LARGE_INTEGER tNow;

	QueryPerformanceFrequency(&tFreq);
	sgqwRefreshPeriod = tFreq.QuadPart / (__int64)nPerSecond;
	QueryPerformanceCounter(&tNow);
	sgqwRefreshNext = tNow.QuadPart + sgqwRefreshPeriod;
}

D2SERVERGAMEINFO * __fastcall DisplayGetSelectedGame(void)
{
	char szName[16];
	D2SERVERGAMEINFO *ptGame;
	int nSel;

	nSel = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_GETCURSEL, 0, 0);
	if (nSel != LB_ERR)
	{
		SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_GETTEXT, nSel, (LPARAM)szName);
		if (strcmp(sgszSelected, szName))
		{
			StormSStrCopy(sgszSelected, szName, 2147483647);
			sgfSelChanged = TRUE;
		}
		for (ptGame = gptGames; ptGame; ptGame = ptGame->ptNext)
			if (!strcmp(ptGame->szName, szName))
				return ptGame;
	}
	return NULL;
}

void __fastcall DisplayProfileInit(void)
{
	if (QueryPerformanceFrequency(&sgtCounterFreq))
	{
		sgfPerfCounter = TRUE;
		sgqwBusyUnused = 0;
		QueryPerformanceCounter(&sgtProfileStart);
		return;
	}
	sgfPerfCounter = FALSE;
}

BOOL __fastcall DisplayBusyStart(void)
{
	return QueryPerformanceCounter(&sgtBusyStart);
}

BOOL __fastcall DisplayBusyUpdate(void)
{
	LARGE_INTEGER tNow;
	__int64 qwElapsed;

	QueryPerformanceCounter(&tNow);
	qwElapsed = tNow.QuadPart - sgtBusyStart.QuadPart;
	if (qwElapsed >= sgtCounterFreq.QuadPart)
	{
		sgdwBusy = (DWORD)(gqwBusyCount * 100 / qwElapsed);
		sgqwBusyUnused = 0;
		gqwBusyCount = 0;
		QueryPerformanceCounter(&sgtBusyStart);
		return TRUE;
	}
	return FALSE;
}

static void __fastcall PanePlayers(D2SERVERGAMEINFO *ptGame, int nArg)
{
	char aszClass[NUM_PLAYERCLASS][64] =
	{
		"Amazon %d", "Sorceress %d", "Necromancer %d", "Paladin %d", "Barbarian %d",
	};
	char szText[64];
	D2SERVERUNITINFO *ptInfo;
	D2SERVERUNITINFO *ptUnit;
	int nPlayers;
	int nFound;
	int nIndex;
	int i;

	nPlayers = GameCountPlayers((WORD)ptGame->adwStats[0]);
	if (!nPlayers)
		return;
#line 334
	ptInfo = (D2SERVERUNITINFO *)D2ALLOC2(nPlayers * sizeof(D2SERVERUNITINFO));
	if (!ptInfo)
		return;
	memset(ptInfo, 0, nPlayers * sizeof(D2SERVERUNITINFO));
	nFound = GameGetPlayerInfo((WORD)ptGame->adwStats[0], ptInfo, nPlayers);
	if (!nFound)
	{
#line 342
		D2FREE2(ptInfo);
		return;
	}
	SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_RESETCONTENT, 0, 0);
	for (i = 0, ptUnit = ptInfo; i < nFound; i++, ptUnit++)
	{
		if (ptUnit->szDesc[0])
			strcpy(szText, ptUnit->szDesc);
		else
			sprintf(szText, aszClass[ptUnit->dwClassId], ptUnit->dwUnitId);
		nIndex = (int)SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_ADDSTRING, 0, (LPARAM)szText);
		SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_SETITEMDATA, nIndex, ptUnit->dwUnitId);
	}
#line 360
	D2FREE2(ptInfo);
}

void __fastcall DisplayToLower(char *pszText)
{
	int nLength;
	int i;

	nLength = strlen(pszText);
	for (i = 0; i < nLength; i++)
		if ((signed char)pszText[i] <= 'Z' && (signed char)pszText[i] >= 'A')
			pszText[i] += 'a' - 'A';
}

static void __fastcall PaneUnitClasses(D2SERVERGAMEINFO *ptGame, int nType)
{
	char szText[128];
	D2SERVERUNITCLASS tHead;
	D2SERVERUNITCLASS *ptClass;
	D2SERVERUNITCLASS *ptFree;
	int i;

	for (i = 0; i < (int)(sizeof(tHead) / sizeof(DWORD)); i++)
		((DWORD *)&tHead)[i] = 0;
	tHead.dwClassId = (DWORD)-1;
	D2gameGameCountUnitClasses((WORD)ptGame->adwStats[0], &tHead, nType);
	SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_RESETCONTENT, 0, 0);
	for (ptClass = tHead.ptNext; ptClass; ptClass = ptClass->ptNext)
	{
		sprintf(szText, "%3d - %s (%d/%d)", ptClass->nCount + ptClass->field_08, ptClass->szDesc,
			ptClass->nCount, ptClass->field_08);
		DisplayToLower(szText);
		SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_ADDSTRING, 0, (LPARAM)szText);
	}
	ptClass = tHead.ptNext;
	while (ptClass)
	{
		ptFree = ptClass;
		ptClass = ptClass->ptNext;
#line 412
		D2FREE2(ptFree);
	}
}

static WORD *DisplayGetGameIds(int *pnIds)
{
	WORD *ptwGameIds;
	int nGames;
	DWORD dwSize;

	*pnIds = 0;
	nGames = D2gameGameGetCount();
	dwSize = nGames * sizeof(WORD);
	if (!nGames)
		return NULL;
#line 72
	ptwGameIds = (WORD *)D2ALLOC2(dwSize);
#line 73
	D2ASSERT(ptwGameIds);
	memset(ptwGameIds, 0, dwSize);
	*pnIds = D2gameGameGetIds(ptwGameIds, nGames);
	return ptwGameIds;
}

static void DisplaySyncGames(void)
{
	D2SERVERGAMEINFO *ptGame;
	WORD *pwIds;
	int nIds;
	int i;

	pwIds = DisplayGetGameIds(&nIds);
	for (ptGame = gptGames; ptGame; ptGame = ptGame->ptNext)
		ptGame->fSeen = FALSE;
	for (i = 0; i < nIds; i++)
	{
		ptGame = DisplayFindGame(pwIds[i]);
		if (ptGame)
			ptGame->fSeen = TRUE;
		else
			DisplayAddGame(pwIds[i]);
	}
	DisplayPruneGames();
	if (pwIds)
	{
#line 205
		D2FREE2(pwIds);
	}
}

static void DisplayShowPane(void)
{
	D2SERVERGAMEINFO *ptGame;
	int nSel;
	int i;

	i = 0;
	nSel = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_GETCURSEL, 0, 0);
	if (nSel != LB_ERR && nSel != sgnLastPane)
	{
		sgnLastPane = nSel;
		while (1)
		{
			if (i >= NUM_PANES)
				break;
			if (sgatPanes[i].nLine == nSel)
			{
				ptGame = DisplayGetSelectedGame();
				if (ptGame)
					sgatPanes[i].pfnShow(ptGame, sgatPanes[i].nArg);
				break;
			}
			i++;
		}
	}
}

static void DisplayShowDetail(void)
{
	char szText[256];
	DWORD adwIds[16];
	DWORD aeTypes[16];
	D2SERVERUNITINFO atInfo[16];
	D2SERVERGAMEINFO *ptGame;
	DWORD dwUnit;
	int nSel;
	int nArg;
	int nFound;
	int i;

	ptGame = DisplayGetSelectedGame();
	if (ptGame)
	{
		nSel = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_GETCURSEL, 0, 0);
		if (nSel != LB_ERR)
		{
			for (i = 0; i < NUM_PANES; i++)
				if (sgatPanes[i].nLine == nSel)
					break;
			if (i < NUM_PANES)
			{
				nArg = sgatPanes[i].nArg;
				nSel = (int)SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_GETCURSEL, 0, 0);
				if (nSel != LB_ERR)
				{
					dwUnit = SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_GETITEMDATA, nSel, 0);
					if (!nArg && (ptGame->adwStats[0] != sgdwDetailGame || sgdwDetailField
						|| dwUnit != sgdwDetailUnit))
					{
						sgdwDetailGame = ptGame->adwStats[0];
						sgdwDetailField = 0;
						sgdwDetailUnit = dwUnit;
						SendDlgItemMessageA(ghStatusDialog, IDC_DETAIL, LB_RESETCONTENT, 0, 0);
						aeTypes[0] = 0;
						adwIds[0] = dwUnit;
						nFound = GameGetUnitInfo((WORD)ptGame->adwStats[0], 1, atInfo, aeTypes, adwIds);
						for (i = 0; i < nFound; i++)
						{
							sprintf(szText, "%s", atInfo[0].szDesc);
							SendDlgItemMessageA(ghStatusDialog, IDC_DETAIL, LB_INSERTSTRING, i,
								(LPARAM)szText);
						}
					}
				}
			}
		}
	}
}

static void DisplayShowCounts(void)
{
	D2SERVERCENSUS tCensus;
	char szText[256];
	int nUnits;

	D2gameGameCountAllUnits(&tCensus);
	nUnits = tCensus.nMonsters + tCensus.field_08;
	sprintf(szText, "%d", tCensus.nGames);
	SendDlgItemMessageA(ghStatusDialog, IDC_GAMES, WM_SETTEXT, 0, (LPARAM)szText);
	sprintf(szText, "%d", tCensus.nPlayers);
	SendDlgItemMessageA(ghStatusDialog, IDC_PLAYERS, WM_SETTEXT, 0, (LPARAM)szText);
	PerfSetSecondLine(szText);
	sprintf(szText, "%d", tCensus.nMonsters);
	SendDlgItemMessageA(ghStatusDialog, IDC_MONSTERS, WM_SETTEXT, 0, (LPARAM)szText);
	sprintf(szText, "%d  (%d / %d)", nUnits, tCensus.nMonsters,
		tCensus.field_08);
	PerfSetFirstLine(szText);
	SendDlgItemMessageA(ghStatusDialog, IDC_MONSTERTEXT, WM_SETTEXT, 0, (LPARAM)szText);
	PerfDraw(sgdwBusy, tCensus.nMonsters);
	PerfLogValues(sgdwBusy, (DWORD *)&tCensus);
}

void __fastcall DisplayRefresh(void)
{
	LARGE_INTEGER tNow;
	SYSTEM_INFO tSystem;

	QueryPerformanceCounter(&tNow);
	if (sgqwRefreshNext <= tNow.QuadPart)
	{
		GetSystemInfo(&tSystem);
		sgqwRefreshNext = (sgqwRefreshPeriod + tNow.QuadPart) / (__int64)tSystem.dwNumberOfProcessors;
		DisplaySyncGames();
		DisplayShowGameInfo();
		DisplayShowPane();
		DisplayShowDetail();
		DisplayShowCounts();
	}
	DisplayShowBusy();
}

static D2SERVERGAMEINFO * __fastcall DisplayFindGame(WORD wGameId)
{
	D2SERVERGAMEINFO *ptGame;

	for (ptGame = gptGames; ptGame; ptGame = ptGame->ptNext)
		if (ptGame->adwStats[0] == wGameId)
			return ptGame;
	return NULL;
}

static void __fastcall DisplayAddGame(WORD wGameId)
{
	D2SERVERGAMEINFO *ptGameInfo;
	int i;

#line 103
	ptGameInfo = (D2SERVERGAMEINFO *)D2ALLOC2(sizeof(D2SERVERGAMEINFO));
#line 104
	D2ASSERT(ptGameInfo);
	for (i = 0; i < (int)(sizeof(D2SERVERGAMEINFO) / sizeof(DWORD)); i++)
		((DWORD *)ptGameInfo)[i] = 0;
	if (!D2gameGameGetInfo(wGameId, ptGameInfo))
	{
#line 110
		D2FREE2(ptGameInfo);
		return;
	}
	ptGameInfo->ptNext = gptGames;
	gptGames = ptGameInfo;
	ptGameInfo->fSeen = TRUE;
	SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_ADDSTRING, 0, (LPARAM)ptGameInfo->szName);
}

static void __fastcall DisplayPruneGames(void)
{
	char szName[16];
	D2SERVERGAMEINFO *ptPrev;
	D2SERVERGAMEINFO *ptGame;
	int nCount;
	int i;

	ptPrev = NULL;
	ptGame = gptGames;
	while (ptGame)
	{
		if (!ptGame->fSeen)
		{
			nCount = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_GETCOUNT, 0, 0);
			for (i = 0; i < nCount; i++)
			{
				SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_GETTEXT, i, (LPARAM)szName);
				if (!strcmp(szName, ptGame->szName))
				{
					SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_DELETESTRING, i, 0);
					break;
				}
			}
			if (ptPrev)
			{
				ptPrev->ptNext = ptGame->ptNext;
#line 161
				D2FREE2(ptGame);
				ptGame = ptPrev->ptNext;
			}
			else
			{
				gptGames = ptGame->ptNext;
#line 165
				D2FREE2(ptGame);
				ptGame = gptGames;
			}
		}
		else
		{
			ptPrev = ptGame;
			ptGame = ptGame->ptNext;
		}
	}
}

static void __fastcall DisplayShowBusy(void)
{
	char szText[100];

	if (!sgfPerfCounter || sgdwBusy == (DWORD)sgnLastBusy)
		return;
	SendDlgItemMessageA(ghStatusDialog, IDC_BUSYBAR, PBM_SETPOS, sgdwBusy, 0);
	sprintf(szText, "%d%% Busy", sgdwBusy);
	SetWindowTextA(GetDlgItem(ghStatusDialog, IDC_BUSYTEXT), szText);
	sgnLastBusy = sgdwBusy;
}

static void __fastcall DisplayShowGameInfo(void)
{
	D2SERVERGAMEINFO *ptGame = DisplayGetSelectedGame();
	char aszFormat[NUM_GAMEINFO][64] =
	{
		"Game Id: %d", "Seed: %d", "Clients: %d", "Players: %d", "Monsters: %d", "Objects: %d", "Items: %d",
		"Missiles: %d", "Unique Items: %d", "NPCs: %d", "Next Player Id: %d", "Next Monster Id: %d",
		"Next Object Id: %d", "Next Item Id: %d", "Next Missile Id: %d", "Next Warp Id: %d",
		"Path Toward Pct: %d", "Path Clock  Pct: %d", "Path Countr Pct: %d", "Path FoWall Pct: %d",
		"Path Astar  Pct: %d", "Path Total Call: %d", "Frames(h): %d", "Time (min): %d", "Frame Rate: %d",
	};
	char szText[64];
	D2SERVERGAMEINFO tInfo;
	int nSel;
	int i;

	if (!ptGame)
	{
		sgptInfoGame = NULL;
		SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_RESETCONTENT, 0, 0);
		SendDlgItemMessageA(ghStatusDialog, IDC_UNITLIST, LB_RESETCONTENT, 0, 0);
		SendDlgItemMessageA(ghStatusDialog, IDC_UNKNOWN3EB, LB_RESETCONTENT, 0, 0);
		sgfSelChanged = TRUE;
		return;
	}
	for (i = 0; i < (int)(sizeof(tInfo) / sizeof(DWORD)); i++)
		((DWORD *)&tInfo)[i] = 0;
	D2gameGameGetInfo((WORD)ptGame->adwStats[0], &tInfo);
	for (i = 0; i < NUM_GAMEINFO; i++)
	{
		if (tInfo.adwStats[i] == ptGame->adwStats[i] && !sgfSelChanged)
			continue;
		wsprintfA(szText, aszFormat[i], tInfo.adwStats[i]);
		nSel = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_GETCURSEL, 0, 0);
		SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_DELETESTRING, i, 0);
		SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_INSERTSTRING, i, (LPARAM)szText);
		if (nSel != LB_ERR)
			SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_SETCURSEL, nSel, 0);
		ptGame->adwStats[i] = tInfo.adwStats[i];
	}
	if (sgptInfoGame != ptGame)
	{
		i = 0;
		sgptInfoGame = ptGame;
		nSel = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMEINFO, LB_GETCURSEL, 0, 0);
		if (nSel != LB_ERR)
		{
			sgnLastPane = nSel;
			while (1)
			{
				if (i >= NUM_PANES)
					break;
				if (sgatPanes[i].nLine == nSel)
				{
					ptGame = DisplayGetSelectedGame();
					if (ptGame)
						sgatPanes[i].pfnShow(ptGame, sgatPanes[i].nArg);
					break;
				}
				i++;
			}
		}
	}
	sgfSelChanged = FALSE;
}

void __fastcall DisplayFreeGames(void)
{
	char szName[16];
	D2SERVERGAMEINFO *ptGame;
	int nCount;
	int i;

	for (ptGame = gptGames; ptGame; ptGame = gptGames)
	{
		nCount = (int)SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_GETCOUNT, 0, 0);
		for (i = 0; i < nCount; i++)
		{
			SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_GETTEXT, i, (LPARAM)szName);
			if (!strcmp(szName, ptGame->szName))
			{
				SendDlgItemMessageA(ghStatusDialog, IDC_GAMELIST, LB_DELETESTRING, i, 0);
				break;
			}
		}
		gptGames = ptGame->ptNext;
#line 694
		D2FREE2(ptGame);
	}
}
