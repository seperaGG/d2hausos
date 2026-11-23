#include "D2MultiLAN.h"

HCHILDDATA hButtonChild[1] = { 0 };
INIT_PARAMS *gptParams = NULL;
WORD gwEcho = 0;
WORD gwListEcho = 0;
WORD gwDetailEcho = 0;
HCELFILE ghBackgroundCel = NULL;
HCELFILE ghCursor = NULL;
HCELFILE ghScrollCel = NULL;
HCELFILE ghQuitButton = NULL;
HCELFILE ghGameButtonCel = NULL;
HCELFILE ghCheckboxCel = NULL;
int gfAttachedToServer = 0;

FONTDESC gtChatFont = { 4, 0 };
FONTDESC gtMenu42Font = { 3, 0 };
FONTDESC gtMenu30Font = { 2, 0 };
FONTDESC gtMenu8Font = { 0, 0 };
FONTDESC gtExocet8Font = { 1, 0 };
SCROLLBARPOS tPos = { 400, 390, 314, 0 };
SCROLLBARPOS gtJoinScroll = { 592, 393, 173, 0 };

static int __stdcall sExit(_PARAMS *ptParams);

D2WIN_STRUCT gtD2Windows[24] =
{
	{ 6, 27, 566, 128, 35, 0, 5103, &ghQuitButton, (DWORD)sExit, 0, 0, 0 },
	{ 4, 496, 82, 200, 22, 0, 0, NULL, 0, 0, (DWORD)&gtMenu30Font, 2 },
	{ 4, 428, 134, 150, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 4, 603, 134, 150, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 4, 433, 225, 200, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 1, 433, 156, 158, 20, 0, 0, NULL, 0, 0, (DWORD)&gtChatFont, 0 },
	{ 1, 608, 156, 158, 20, 0, 0, NULL, 0, 0, (DWORD)&gtChatFont, 1 },
	{ 4, 432, 399, 160, 173, 0, 0, &ghScrollCel, (DWORD)JoinGameListCallback, (DWORD)&gtJoinScroll,
		(DWORD)&gtChatFont, 5 },
	{ 6, 465, 510, 272, 35, 0, 5151, &ghGameButtonCel, (DWORD)GateJoinSelect, 0, 0, 0 },
	{ 4, 612, 399, 157, 194, 0, 0, NULL, 0, 0, (DWORD)&gtChatFont, 2 },
	{ 4, 106, 82, 200, 22, 0, 0, NULL, 0, 0, (DWORD)&gtMenu30Font, 2 },
	{ 4, 39, 134, 150, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 4, 215, 134, 150, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 4, 39, 202, 300, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 4, 261, 293, 300, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 16 },
	{ 4, 248, 356, 300, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 16 },
	{ 4, 316, 356, 150, 12, 0, 0, NULL, 0, 0, (DWORD)&gtExocet8Font, 0 },
	{ 1, 44, 161, 158, 20, 0, 0, NULL, 0, (DWORD)GateCreateCallback, (DWORD)&gtChatFont, 0 },
	{ 1, 220, 161, 158, 20, 0, 0, NULL, 0, (DWORD)GateCreateCallback, (DWORD)&gtChatFont, 1 },
	{ 1, 44, 229, 333, 20, 0, 0, NULL, 0, 0, (DWORD)&gtChatFont, 0 },
	{ 1, 288, 295, 27, 20, 0, 0, NULL, 0, 0, (DWORD)&gtChatFont, 0 },
	{ 1, 288, 357, 27, 20, 0, 0, NULL, 0, 0, (DWORD)&gtChatFont, 0 },
	{ 6, 64, 510, 272, 35, 0, 5150, &ghGameButtonCel, (DWORD)GateCreateCallback, 0, 0, 0 },
	{ 6, 42, 354, 15, 16, 0, 0, &ghCheckboxCel, 0, 0, 0, 2 },
};

HCHILDDATA hTextBox = NULL;
HCHILDDATA hEditBox = NULL;
MODSTATE geModState = (MODSTATE)0;

void __fastcall DestroyMenu(void)
{
	DestroyCreateGameWindows();
	DestroyJoinGameWindows();
}

void __fastcall LoadCelFiles(void)
{
	char szPath[MAX_PATH];

	sprintf(szPath, "%s\\UI\\BIGMENU\\langamebckg", "DATA\\GLOBAL");
	ghBackgroundCel = D2WinArchiveLoadCel(szPath, 0);
	sprintf(szPath, "%s\\UI\\CURSOR\\ohand", "DATA\\GLOBAL");
	ghCursor = D2WinArchiveLoadCel(szPath, 0);
	sprintf(szPath, "%s\\UI\\BIGMENU\\scrollbar", "DATA\\GLOBAL");
	ghScrollCel = D2WinArchiveLoadCel(szPath, 0);
	sprintf(szPath, "%s\\UI\\FrontEnd\\MediumButtonBlank", "DATA\\GLOBAL");
	ghQuitButton = D2WinArchiveLoadCel(szPath, 0);
	sprintf(szPath, "%s\\UI\\BIGMENU\\widebuttonblank", "DATA\\GLOBAL");
	ghGameButtonCel = D2WinArchiveLoadCel(szPath, 0);
	sprintf(szPath, "%s\\UI\\BIGMENU\\joingameclickbox", "DATA\\GLOBAL");
	ghCheckboxCel = D2WinArchiveLoadCel(szPath, 0);
}

void __fastcall FreeCelFiles(void)
{
	if (ghBackgroundCel)
	{
		D2WinArchiveFreeCel(ghBackgroundCel);
		ghBackgroundCel = NULL;
	}
	if (ghCursor)
	{
		D2WinArchiveFreeCel(ghCursor);
		ghCursor = NULL;
	}
	if (ghScrollCel)
	{
		D2WinArchiveFreeCel(ghScrollCel);
		ghScrollCel = NULL;
	}
	if (ghQuitButton)
	{
		D2WinArchiveFreeCel(ghQuitButton);
		ghQuitButton = NULL;
	}
	if (ghGameButtonCel)
	{
		D2WinArchiveFreeCel(ghGameButtonCel);
		ghGameButtonCel = NULL;
	}
	if (ghCheckboxCel)
	{
		D2WinArchiveFreeCel(ghCheckboxCel);
		ghCheckboxCel = NULL;
	}
}

static int __stdcall sExit(_PARAMS *ptParams)
{
	geModState = MODSTATE_LAUNCHER;
	D2WinQuitLoop();
	return TRUE;
}

void __fastcall ProcessSysMsgFromServer(BYTE *pbMsg, int nSize)
{
	if (nSize == -1)
		return;
	switch (pbMsg[0])
	{
	case 0:
		gfAttachedToServer = TRUE;
		break;
	case 1:
		gfAttachedToServer = FALSE;
		break;
	case 2:
		return;
	case 3:
		AddToGameList((D2NETSRVTCPJOINENTRY *)pbMsg);
		break;
	case 4:
	case 5:
		return;
	}
}

int __stdcall sAttachIdle(DWORD dwUnused)
{
	BYTE abMsg[512];
	int nSize;

	nSize = D2NetGetCltSysMsg(abMsg, sizeof(abMsg));
	ProcessSysMsgFromServer(abMsg, nSize);
	if (gfAttachedToServer)
		D2WinQuitLoop();
	return TRUE;
}

void __fastcall SendForceDisconnect(void)
{
	BYTE bMsg;

	bMsg = 7;
	D2NetSendToServer(0, &bMsg, 1);
}

int __stdcall sDetachIdle(DWORD dwUnused)
{
	BYTE abMsg[512];
	int nSize;

	nSize = D2NetGetCltSysMsg(abMsg, sizeof(abMsg));
	ProcessSysMsgFromServer(abMsg, nSize);
	if (!gfAttachedToServer || !D2NetGetRemoteConnection())
		D2WinQuitLoop();
	return TRUE;
}

int __stdcall sIdle(DWORD dwUnused)
{
	static int snMultiState = -1;
	BYTE abMsg[512];
	Unicode awszText[128];
	DWORD dwTicks;
	BOOL fRepeat;
	int nSize;

	do
	{
		if (gfAttachedToServer)
		{
			nSize = D2NetGetCltSysMsg(abMsg, sizeof(abMsg));
			ProcessSysMsgFromServer(abMsg, nSize);
		}
		static DWORD dwLastListTime = 0;
		static DWORD dwLastDetailTime = 0;

		fRepeat = FALSE;
		dwTicks = GetTickCount() - dwLastListTime;
		if (dwTicks >= GAMELIST_TICKS)
		{
			dwLastListTime += dwTicks;
			fRepeat = TRUE;
			JoinGameListRebuildStart();
		}
		dwTicks = GetTickCount() - dwLastDetailTime;
		if (dwTicks >= DETAIL_TICKS)
		{
			dwLastDetailTime += dwTicks;
			fRepeat = TRUE;
			SendDetailMessage();
		}
	} while (fRepeat);
	return 0;
}

int __fastcall StartLANChat(INIT_PARAMS *pParams)
{
	if (StrNewTable(D2WinArchiveGetArchive()) == FALSE)
	{
		return 1;
	}
	else
	{
		geModState = MODSTATE_MODSTATE0;
		LoadCelFiles();
		D2WinSetCursor(ghCursor);
		hTextBox = NULL;
		D2WinSetBackground(NULL);
		if (D2NetGetRemoteConnection() == 0)
		{
			D2NetClientInit(0, pParams->szServerIp);
			D2WinLoop(sAttachIdle);
		}
		D2WinSetBackground(ghBackgroundCel);
		hButtonChild[0] = D2WinCreateChild(&gtD2Windows[0]);
		DestroyMenu();
		CreateJoinGameWindows();
		CreateCreateGameWindows();
		D2WinLoop(sIdle);
		DestroyMenu();
		D2WinDestroyChild(hButtonChild);
		D2WinSetBackground(NULL);
		D2WinSetCursor(NULL);
		FreeCelFiles();
		StrDeleteTable();
		SendForceDisconnect();
		D2NetClientClose();
	}
	return 1;
}

inline void __cdecl operator delete(void *pMemory)
{
	if (pMemory)
		SMemFree(pMemory, "delete", -1, 0);
}

inline void * __cdecl operator new(unsigned int nSize)
{
	return SMemAlloc(nSize, "new", -1, 0);
}

int __fastcall StartChat(INIT_PARAMS *pParams)
{
	gptParams = pParams;
	D2WinCreateParent();
	D2WinResizeWindow(1);
	StartLANChat(pParams);
	D2WinCloseParent();
	return geModState;
}
