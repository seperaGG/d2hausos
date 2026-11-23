#include "D2MultiLAN.h"

D2NETSRVTCPJOINENTRY sgtDisplayedGameList[MAX_GAMELIST];
D2NETSRVTCPJOINENTRY sgtGameList[MAX_GAMELIST];
static char gszQueuedDetailGameName[256];
HCHILDDATA hGateCreateText = NULL;
HCHILDDATA hGateCreateInput = NULL;
static HCHILDDATA sghJoinGameList = NULL;
static HCHILDDATA sghJoinGameDescription = NULL;
static HCHILDDATA sghJoinGameOKButton = NULL;
static HCHILDDATA sghJoinGameCancelButton = NULL;
static HCHILDDATA sghJoinGameName = NULL;
static HCHILDDATA sghJoinGamePassword = NULL;
static HCHILDDATA sghJoinGameTitleText = NULL;
static HCHILDDATA sghJoinGameNameText = NULL;
static HCHILDDATA sghJoinGamePasswordText = NULL;
static HCHILDDATA sghJoinGameDescText = NULL;
static HCHILDDATA sghCreateGameTitleText = NULL;
static HCHILDDATA sghCreateGameNameText = NULL;
static HCHILDDATA sghCreateGamePasswordText = NULL;
static HCHILDDATA sghCreateGameDescText = NULL;
static HCHILDDATA sghCreateGameMaxPlayersText = NULL;
static HCHILDDATA sghCreatGameLevel1Text = NULL;
static HCHILDDATA sghCreatGameLevel2Text = NULL;
static HCHILDDATA sghCreateGameOKButton = NULL;
static HCHILDDATA sghCreateGameName = NULL;
static HCHILDDATA sghCreateGamePassword = NULL;
static HCHILDDATA sghCreateGameDescription = NULL;
static HCHILDDATA sghCreateGamePlayers = NULL;
static HCHILDDATA sghCreateGameLevels = NULL;
static HCHILDDATA sghCreateGameLevelCheckbox = NULL;
static HCHILDDATA sghCreateGameCancelButton = NULL;
int sgnDisplayedGameListEntries = 0;
int sgnGameListEntries = 0;

static int __stdcall sJoinGameNameCallback(void *pvEdit, int nPos, char *pchKey);
static int __stdcall sJoinGameNamePostCallback(_PARAMS *ptParams);
static int __stdcall sCreateGameNameCallback(void *pvEdit, int nPos, char *pchKey);
static int __stdcall sCreateGameNamePostCallback(_PARAMS *ptParams);
static int __stdcall sPasswordCallback(void *pvEdit, int nPos, char *pchKey);

void __fastcall JoinGameListRebuildEnd(void)
{
	char szPlayers[16];
	char szSelected[256];
	const Unicode *pwszSelected;
	BOOL fSelected;
	int nLine;
	int i;

	if (!sghJoinGameList)
		return;
	pwszSelected = D2WinTextBoxGetSelectedLine(sghJoinGameList);
	fSelected = FALSE;
	if (pwszSelected)
	{
		Unicode::toUtf(szSelected, pwszSelected, sizeof(szSelected));
		fSelected = TRUE;
	}
	D2WinClearTextBox(sghJoinGameList);
	sgnDisplayedGameListEntries = sgnGameListEntries;
	if (!sgnGameListEntries)
		return;
	memcpy(sgtDisplayedGameList, sgtGameList, sgnGameListEntries * sizeof(D2NETSRVTCPJOINENTRY));
	for (i = 0; i < sgnDisplayedGameListEntries; i++)
	{
		D2WinTextBoxAddLineUTF(sghJoinGameList, sgtDisplayedGameList[i].szName);
		nLine = D2WinTextBoxGetLastLineNum(sghJoinGameList);
		itoa(sgtDisplayedGameList[i].wNumPlayers, szPlayers, 10);
		D2WinTextBoxAddColumnTextUTF(sghJoinGameList, szPlayers, nLine, 1);
		if (fSelected && !strcmp(szSelected, sgtDisplayedGameList[i].szName))
			D2WinTextBoxSetSelectedNum(sghJoinGameList, i);
	}
}

void __fastcall AddToGameList(D2NETSRVTCPJOINENTRY *ptEntry)
{
	WORD wId;
	int nEntries;
	int i;

	wId = ptEntry->wGameId;
	if (wId == 65535 && !ptEntry->szName[0]) { JoinGameListRebuildEnd(); return; }
	for (nEntries = sgnGameListEntries, i = 0; i < nEntries; i++)
	{
		if (sgtGameList[i].wGameId == wId)
			return;
	}
	sgtGameList[nEntries].wGameId = wId;
	sgtGameList[nEntries].wNumPlayers = ptEntry->wNumPlayers;
	strcpy(sgtGameList[nEntries].szName, ptEntry->szName);
	strcpy(sgtGameList[nEntries].field_11, ptEntry->field_11);
	sgnGameListEntries = nEntries + 1;
}

void __fastcall CreateJoinGameWindows(void)
{
	DWORD dwTicks;

	sghJoinGameTitleText = D2WinCreateChild(&gtD2Windows[1]);
	D2WinTextBoxAddLineUnicode(sghJoinGameTitleText, StrGetStringByIndex(5153));
	sghJoinGameNameText = D2WinCreateChild(&gtD2Windows[2]);
	D2WinTextBoxAddLineUnicode(sghJoinGameNameText, StrGetStringByIndex(5274));
	sghJoinGamePasswordText = D2WinCreateChild(&gtD2Windows[3]);
	D2WinTextBoxAddLineUnicode(sghJoinGamePasswordText, StrGetStringByIndex(5225));
	sghJoinGameDescText = D2WinCreateChild(&gtD2Windows[4]);
	D2WinTextBoxAddLineUnicode(sghJoinGameDescText, StrGetStringByIndex(5275));
	sghJoinGameOKButton = D2WinCreateChild(&gtD2Windows[8]);
	sghJoinGameName = D2WinCreateChild(&gtD2Windows[5]);
	D2WinEditBoxSetKeyProc(sghJoinGameName, sJoinGameNameCallback);
	D2WinEditBoxSetKeyPostProc(sghJoinGameName, sJoinGameNamePostCallback);
	sghJoinGamePassword = D2WinCreateChild(&gtD2Windows[6]);
	D2WinEditBoxSetTabNext(sghJoinGameName, sghJoinGamePassword);
	D2WinEditBoxSetTabNext(sghJoinGamePassword, sghJoinGameName);
	sghJoinGameList = D2WinCreateChild(&gtD2Windows[7]);
	D2WinTextBoxSetNumColumns(sghJoinGameList, 2);
	D2WinTextBoxSetColumnX(sghJoinGameList, 1, 135);
	sghJoinGameDescription = D2WinCreateChild(&gtD2Windows[9]);
	{
		static DWORD dwLastTimeButtonClicked = 0;

		dwTicks = GetTickCount() - dwLastTimeButtonClicked;
		if (dwTicks >= GAMELIST_TICKS)
		{
			dwLastTimeButtonClicked += dwTicks;
			JoinGameListRebuildStart();
		}
	}
	D2WinSetKeyFocus(sghJoinGameName);
	sgnDisplayedGameListEntries = 0;
	sgnGameListEntries = 0;
	sJoinGameNamePostCallback(NULL);
}

void __fastcall JoinGameListRebuildStart(void)
{
	BYTE bMsg;

	sgnGameListEntries = 0;
	bMsg = 3;
	D2NetSendToServer(0, &bMsg, 1);
}

static int __stdcall sJoinGameNameCallback(void *pvEdit, int nPos, char *pchKey)
{
	D2WinTextBoxSetSelectedNum(sghJoinGameList, -1);
	return TRUE;
}

#define ENABLE_OK_BY_NAME(hEdit, hOKButton, nSize) \
	{ \
		char szName[nSize]; \
		const Unicode *pwszName; \
		BOOL fEnable; \
		pwszName = D2WinEditBoxGetString(hEdit); \
		fEnable = FALSE; \
		if (pwszName) \
		{ \
			Unicode::toUtf(szName, pwszName, nSize - 1); \
			if (strlen(szName) >= 1) \
				fEnable = TRUE; \
		} \
		D2WinChildSetEnabled(hOKButton, fEnable); \
	}

static int __stdcall sJoinGameNamePostCallback(_PARAMS *ptParams)
{
	ENABLE_OK_BY_NAME(sghJoinGameName, sghJoinGameOKButton, 256);
	return TRUE;
}

static int gsnQueuedDetailEcho = -1;

void __fastcall QueueDetailMessage(char *pszName, WORD wEcho)
{
	strncpy(gszQueuedDetailGameName, pszName, sizeof(gszQueuedDetailGameName));
	gsnQueuedDetailEcho = wEcho;
}

void __fastcall SendDetailMessage(void)
{
}

int __stdcall JoinGameListCallback(_PARAMS *ptParams)
{
	char szName[256];
	const Unicode *pwszName;
	int nSelected;

	nSelected = D2WinTextBoxGetSelectedNum(sghJoinGameList);
	D2WinClearTextBox(sghJoinGameDescription);
	if (nSelected != -1)
	{
		pwszName = D2WinTextBoxGetSelectedLine(sghJoinGameList);
		if (pwszName)
		{
			Unicode::toUtf(szName, pwszName, sizeof(szName));
			do ++gwEcho; while (!gwEcho);
			QueueDetailMessage(szName, gwEcho);
			D2WinEditBoxSetStringUnicode(sghJoinGameName, pwszName);
		}
	}
	return 0;
}

void __fastcall UpdateJoinGameDetails(D2MCPGAMELIST *ptGame)
{
	char szLine[256];
	char *pszName;
	int i;

	D2WinTextBoxAddLineUTF(sghJoinGameDescription, ptGame->szDescription);
	D2WinTextBoxAddLineUTF(sghJoinGameDescription, "");
	for (i = 0; i < ptGame->nNumPlayers; i++)
	{
		pszName = ptGame->aszPlayerNames[i];
		switch (ptGame->awClasses[i])
		{
		case 0:
			sprintf(szLine, "%s (Lvl %d Ama)", pszName, ptGame->awLevels[i]);
			break;
		case 1:
			sprintf(szLine, "%s (Lvl %d Sor)", pszName, ptGame->awLevels[i]);
			break;
		case 2:
			sprintf(szLine, "%s (Lvl %d Nec)", pszName, ptGame->awLevels[i]);
			break;
		case 3:
			sprintf(szLine, "%s (Lvl %d Pal)", pszName, ptGame->awLevels[i]);
			break;
		case 4:
			sprintf(szLine, "%s (Lvl %d Bar)", pszName, ptGame->awLevels[i]);
			break;
		default:
			sprintf(szLine, "%s (Lvl %d)", pszName, ptGame->awLevels[i]);
			break;
		}
		D2WinTextBoxAddLineUTF(sghJoinGameDescription, szLine);
	}
}

void __fastcall DestroyJoinGameWindows(void)
{
	D2WinDestroyChild(&sghJoinGameName);
	D2WinDestroyChild(&sghJoinGamePassword);
	D2WinDestroyChild(&sghJoinGameList);
	D2WinDestroyChild(&sghJoinGameDescription);
	D2WinDestroyChild(&sghJoinGameOKButton);
	D2WinDestroyChild(&sghJoinGameCancelButton);
	D2WinDestroyChild(&sghJoinGameTitleText);
	D2WinDestroyChild(&sghJoinGameNameText);
	D2WinDestroyChild(&sghJoinGamePasswordText);
	D2WinDestroyChild(&sghJoinGameDescText);
}

void __fastcall CreateCreateGameWindows(void)
{
	sghCreateGameTitleText = D2WinCreateChild(&gtD2Windows[10]);
	D2WinTextBoxAddLineUnicode(sghCreateGameTitleText, StrGetStringByIndex(5152));
	sghCreateGameNameText = D2WinCreateChild(&gtD2Windows[11]);
	D2WinTextBoxAddLineUnicode(sghCreateGameNameText, StrGetStringByIndex(5274));
	sghCreateGamePasswordText = D2WinCreateChild(&gtD2Windows[12]);
	D2WinTextBoxAddLineUnicode(sghCreateGamePasswordText, StrGetStringByIndex(5225));
	sghCreateGameDescText = D2WinCreateChild(&gtD2Windows[13]);
	D2WinTextBoxAddLineUnicode(sghCreateGameDescText, StrGetStringByIndex(5257));
	sghCreateGameMaxPlayersText = D2WinCreateChild(&gtD2Windows[14]);
	D2WinTextBoxAddLineUnicode(sghCreateGameMaxPlayersText, StrGetStringByIndex(5258));
	sghCreatGameLevel1Text = D2WinCreateChild(&gtD2Windows[15]);
	D2WinTextBoxAddLineUnicode(sghCreatGameLevel1Text, StrGetStringByIndex(5259));
	sghCreatGameLevel2Text = D2WinCreateChild(&gtD2Windows[16]);
	D2WinTextBoxAddLineUnicode(sghCreatGameLevel2Text, StrGetStringByIndex(5260));
	sghCreateGameOKButton = D2WinCreateChild(&gtD2Windows[22]);
	sghCreateGameName = D2WinCreateChild(&gtD2Windows[17]);
	D2WinEditBoxSetKeyProc(sghCreateGameName, sCreateGameNameCallback);
	D2WinEditBoxSetKeyPostProc(sghCreateGameName, sCreateGameNamePostCallback);
	sghCreateGamePassword = D2WinCreateChild(&gtD2Windows[18]);
	D2WinEditBoxSetKeyProc(sghCreateGamePassword, sPasswordCallback);
	sghCreateGameDescription = D2WinCreateChild(&gtD2Windows[19]);
	D2WinEditBoxSetKeyProc(sghCreateGameDescription, sPasswordCallback);
	D2WinEditBoxSetTabNext(sghCreateGameName, sghCreateGamePassword);
	D2WinEditBoxSetTabNext(sghCreateGamePassword, sghCreateGameDescription);
	D2WinEditBoxSetTabNext(sghCreateGameDescription, sghCreateGameName);
	sghCreateGamePlayers = D2WinCreateChild(&gtD2Windows[20]);
	D2WinEditBoxSetStringUTF(sghCreateGamePlayers, "8");
	D2WinEditBoxSetKeyProc(sghCreateGamePlayers, D2WinEditBoxNumericCallback);
	sghCreateGameLevels = D2WinCreateChild(&gtD2Windows[21]);
	D2WinEditBoxSetStringUTF(sghCreateGameLevels, "5");
	D2WinEditBoxSetKeyProc(sghCreateGameLevels, D2WinEditBoxNumericCallback);
	sghCreateGameLevelCheckbox = D2WinCreateChild(&gtD2Windows[23]);
	D2WinSetKeyFocus(sghCreateGameName);
	sCreateGameNamePostCallback(NULL);
}

void __fastcall DestroyCreateGameWindows(void)
{
	D2WinDestroyChild(&sghCreateGameTitleText);
	D2WinDestroyChild(&sghCreateGameNameText);
	D2WinDestroyChild(&sghCreateGamePasswordText);
	D2WinDestroyChild(&sghCreateGameDescText);
	D2WinDestroyChild(&sghCreateGameMaxPlayersText);
	D2WinDestroyChild(&sghCreatGameLevel1Text);
	D2WinDestroyChild(&sghCreatGameLevel2Text);
	D2WinDestroyChild(&sghCreateGameName);
	D2WinDestroyChild(&sghCreateGamePassword);
	D2WinDestroyChild(&sghCreateGameDescription);
	D2WinDestroyChild(&sghCreateGamePlayers);
	D2WinDestroyChild(&sghCreateGameLevels);
	D2WinDestroyChild(&sghCreateGameOKButton);
	D2WinDestroyChild(&sghCreateGameCancelButton);
	D2WinDestroyChild(&sghCreateGameLevelCheckbox);
}

static int __stdcall sCreateGameNameCallback(void *pvEdit, int nPos, char *pchKey)
{
	char chKey;

	chKey = *pchKey;
	switch (chKey)
	{
	case ' ':
	case '\'':
	case '-':
	case '_':
		return TRUE;
	}
	if (chKey >= '0' && chKey <= '9') return TRUE;
	if (chKey >= 'A' && chKey <= 'Z')
		return TRUE;
	if (chKey >= 'a' && chKey <= 'z')
		return TRUE;
	return FALSE;
}

static int __stdcall sCreateGameNamePostCallback(_PARAMS *ptParams)
{
	ENABLE_OK_BY_NAME(sghCreateGameName, sghCreateGameOKButton, 16);
	return TRUE;
}

static int __stdcall sPasswordCallback(void *pvEdit, int nPos, char *pchKey)
{
	return *pchKey >= ' ' && (int)*pchKey < 128;
}

int __stdcall GateCreateCallback(_PARAMS *ptParams)
{
	const Unicode *pwszName;
	const Unicode *pwszPassword;

	pwszName = D2WinEditBoxGetString(sghCreateGameName);
	if (pwszName)
	{
		if (Unicode::strlen(pwszName) < 1)
			return FALSE;
		gptParams->dwGameType = GAMETYPE_LANCREATE;
		Unicode::toUtf(gptParams->szGameName, pwszName, 15);
		pwszPassword = D2WinEditBoxGetString(sghCreateGamePassword);
		Unicode::toUtf((char *)&gptParams->bGamePassword, pwszPassword, 15);
		geModState = MODSTATE_CLIENT;
		DestroyMenu();
		D2WinSetCursor(NULL);
		D2WinQuitLoop();
	}
	return TRUE;
}

int __stdcall GateJoinSelect(_PARAMS *ptParams)
{
	char szName[256];
	char szPassword[256];
	const Unicode *pwszName;
	const Unicode *pwszPassword;
	int i;

	pwszName = D2WinEditBoxGetString(sghJoinGameName);
	pwszPassword = D2WinEditBoxGetString(sghJoinGamePassword);
	if (!pwszName) return FALSE;
	Unicode::toUtf(szName, pwszName, sizeof(szName));
	Unicode::toUtf(szPassword, pwszPassword, sizeof(szPassword));
	gptParams->dwGameType = GAMETYPE_LANJOIN;
	strcpy(gptParams->szGameName, szName);
	strcpy((char *)&gptParams->bGamePassword, szPassword);
	for (i = 0; i < sgnGameListEntries; i++)
	{
		if (!strcmp(sgtDisplayedGameList[i].szName, szName))
		{
			gptParams->wJoinId = sgtDisplayedGameList[i].wGameId;
			geModState = MODSTATE_CLIENT;
			DestroyMenu();
			D2WinSetCursor(NULL);
			D2WinQuitLoop();
			return TRUE;
		}
	}
	return FALSE;
}
