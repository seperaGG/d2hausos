#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\CORE\\CharServ.cpp"
#include "D2Server.h"
#include "D2Macros.h"
#include <time.h>

struct CHARSERV
{
 	void		*pConnection;
 	char		szAddress[36];
 	HANDLE		hThread;
};

#pragma pack(push, 1)
struct CHARSERVPIECE
{
 	BYTE		bOpcode;
 	DWORD		unobserved_01;
 	DWORD		dwClientId;
 	WORD		wLength;
 	WORD		wTotal;
 	DWORD		dwDrop;
 	BYTE		abData[1];
};

struct CHARSERVREC1
{
	BYTE		bTag;
	DWORD		dwArg;
	DWORD		dwChecksum;
	char		szText[4131];
};

struct CHARSERVREC2
{
	BYTE		bTag;
	DWORD		dwTime;
	DWORD		dwArg;
	DWORD		dwChecksum;
	char		szText[4131];
};

struct CHARSERVREC3
{
	BYTE		bTag;
	DWORD		dwArg;
	DWORD		dwClientId;
	char		szName[19];
};

struct CHARSERVREC4
{
	BYTE		bTag;
	WORD		wArg;
	DWORD		dwArg;
	char		szName[17];
};

struct CHARSERVREC5
{
	BYTE		bTag;
	DWORD		dwArg;
	char		szName[19];
};

struct CHARSERVREC7
{
	BYTE		bTag;
	char		szName[19];
};
#pragma pack(pop)

typedef BOOL (__fastcall *PFNCHARSERV)(const BYTE *pbMsg, int nLength);

static DWORD sgnCharServ = 0;
static CHARSERV sgatCharServ[MAX_CHAR_SERV] = { 0 };
static DWORD sgdwLastPing = 0;

BOOL __fastcall CharServRecvPiece(const BYTE *pbMsg, int nLength)
{
	const CHARSERVPIECE *ptPiece = (const CHARSERVPIECE *)pbMsg;

	D2gameGameReceiveData(ptPiece->dwClientId, ptPiece->abData, ptPiece->wLength, ptPiece->wTotal,
		ptPiece->dwDrop);
	return TRUE;
}

BOOL __fastcall CharServOnNothingFirst(const BYTE *pbMsg, int nLength)
{
	return TRUE;
}

BOOL __fastcall CharServOnNothingSecond(const BYTE *pbMsg, int nLength)
{
	return TRUE;
}

static PFNCHARSERV sgapfnCharServ[9] =
{
	NULL, NULL, NULL,
	CharServRecvPiece,
	NULL,
	CharServOnNothingFirst,
	NULL, NULL,
	CharServOnNothingSecond,
};

BOOL __fastcall CharServDispatch(DWORD dwUnused, const BYTE *pbMsg, DWORD nLength)
{
	BOOL fResult;
	BYTE bOpcode;
	PFNCHARSERV pfnHandler;

	fResult = FALSE;
	if (nLength <= 0)
		return FALSE;
	bOpcode = pbMsg[0];
	if (bOpcode >= 9)
		return FALSE;
	pfnHandler = sgapfnCharServ[bOpcode];
	if (pfnHandler)
		fResult = pfnHandler(pbMsg, nLength);
	return fResult;
}

BOOL __fastcall CharServSend(const BYTE *pbRecord, int nLength)
{
	DWORD dwServ;
	int i;

	dwServ = sgnCharServ;
	for (i = 0; i < IniGetNumCharServ(); i++)
	{
		if (sgatCharServ[dwServ].pConnection
			&& FogD2QSSocketSend(sgatCharServ[dwServ].pConnection, pbRecord, nLength))
			break;
		dwServ = (dwServ + 1) % IniGetNumCharServ();
	}
	sgnCharServ = dwServ;
	return i < IniGetNumCharServ();
}

BOOL __fastcall CharServSendChecksummedData(const char *pszText1, const char *pszText2, const BYTE *pbData,
	int nLength,
	DWORD dwArg)
{
	CHARSERVREC1 tRecord;
	char *pszText;
	int nChecksum;
	int nText;
	int i;

	tRecord.bTag = 1;
	tRecord.dwArg = dwArg;
	nChecksum = 0;
	for (i = 0; i < nLength; i++)
		nChecksum = pbData[i] + (nChecksum < 0) + nChecksum * 2;
	tRecord.dwChecksum = nChecksum;
	strcpy(tRecord.szText, pszText1);
	nText = strlen(pszText1) + 1;
	pszText = tRecord.szText + nText;
	strcpy(pszText, pszText2);
	nText += strlen(pszText2) + 1;
#line 208
	D2ASSERT(*(WORD *)pbData == nLength);
	memcpy(pszText + nText, pbData, nLength);
	nText += nLength + 9;
	return CharServSend((const BYTE *)&tRecord, nText);
}

BOOL __fastcall CharServSaveCharacter(const char *pszName, const char *pszAccount, const BYTE *pbData,
	int nLength, DWORD dwArg)
{
	CHARSERVREC2 tRecord;
	char *pszText;
	int nChecksum;
	int nSize;
	int nText;
	int i;

	tRecord.bTag = 2;
	tRecord.dwTime = FogAsyncDataGetTime();
	tRecord.dwArg = dwArg;
	nChecksum = 0;
	for (i = 0; i < nLength; i++)
		nChecksum = pbData[i] + (nChecksum < 0) + nChecksum * 2;
	tRecord.dwChecksum = nChecksum;
	pszText = tRecord.szText;
	strcpy(pszText, pszName);
	nText = strlen(pszName) + 1;
	nSize = nText;
	pszText += nText;
	strcpy(pszText, pszAccount);
	nText = strlen(pszAccount) + 1;
#line 249
	D2ASSERT(*(WORD *)pbData == nLength);
	memcpy(pszText + nText, pbData, nLength);
	nText += nLength;
	nSize += nText + 13;
	return CharServSend((const BYTE *)&tRecord, nSize);
}

BOOL __fastcall CharServLoadCharacter(const char *pszName, DWORD dwClientId)
{
	CHARSERVREC3 tRecord;

	tRecord.dwClientId = dwClientId;
	tRecord.bTag = 3;
	tRecord.dwArg = 1;
	strcpy(tRecord.szName, pszName);
	return CharServSend((const BYTE *)&tRecord, strlen(pszName) + 10);
}

BOOL __fastcall CharServSendValueAndName(const char *pszName, DWORD dwArg)
{
	CHARSERVREC5 tRecord;

	tRecord.dwArg = dwArg;
	tRecord.bTag = 5;
	strcpy(tRecord.szName, pszName);
	return CharServSend((const BYTE *)&tRecord, strlen(pszName) + 6);
}

BOOL __fastcall CharServSendWordValueAndName(const char *pszName, WORD wArg, DWORD dwArg)
{
	CHARSERVREC4 tRecord;

	tRecord.dwArg = dwArg;
	tRecord.wArg = wArg;
	tRecord.bTag = 4;
	strcpy(tRecord.szName, pszName);
	return CharServSend((const BYTE *)&tRecord, strlen(pszName) + 8);
}

BOOL __fastcall CharServUnlockCharacter(const char *pszName)
{
	CHARSERVREC7 tRecord;

	tRecord.bTag = 7;
	strcpy(tRecord.szName, pszName);
	return CharServSend((const BYTE *)&tRecord, strlen(pszName) + 2);
}

BOOL __fastcall CharServPing(void)
{
	BYTE bTag;

	bTag = 8;
	return CharServSend(&bTag, 1);
}

DWORD WINAPI CharServThread(LPVOID pvParam)
{
	char szName[256];
	struct in_addr tAddress;
	BOOL fConnected;
	DWORD dwStart;
	BYTE bHello;
	int i;

	fConnected = FALSE;
	for (i = 0; i < IniGetNumCharServ(); i++)
	{
		tAddress.s_addr = IniGetCharServ(i);
		StormSStrCopy(sgatCharServ[i].szAddress, inet_ntoa(tAddress), 2147483647);
		ServerLog(TRUE, "[CHARACTER SERVER]  Connecting to Character Server at %s",
			sgatCharServ[i].szAddress);
	}
	for (;;)
	{
		for (i = 0; i < IniGetNumCharServ(); i++)
		{
			if (sgatCharServ[i].szAddress && sgatCharServ[i].szAddress[0] != '0')
			{
				sprintf(szName, "Game Server->Char Server #%d", i);
				sgatCharServ[i].pConnection = FogD2QSSocketCreate(sgatCharServ[i].szAddress, 6113,
					szName);
				FogD2QSSocketSetDispatcher(sgatCharServ[i].pConnection, 0, 0);
				FogD2QSSocketStart(sgatCharServ[i].pConnection);
				bHello = 5;
				FogD2QSSocketSendRaw(sgatCharServ[i].pConnection, &bHello, 1);
				if (FogD2QSSocketGetConnection(sgatCharServ[i].pConnection))
				{
					ServerLog(TRUE, "[CHARACTER SERVER]  Connected to Character Server at %s", sgatCharServ[i].szAddress);
					fConnected = TRUE;
				}
			}
		}
		if (fConnected)
			break;
		Sleep(100);
	}
	srand(GetTickCount());
	dwStart = rand();
	dwStart %= IniGetNumCharServ();
	sgnCharServ = (dwStart + 1) % IniGetNumCharServ();
	while (sgnCharServ != dwStart && !sgatCharServ[sgnCharServ].pConnection)
		sgnCharServ = (sgnCharServ + 1) % IniGetNumCharServ();
	ServerLog(TRUE, "[CHARACTER SERVER]  Chose Primary Character Server: %s",
		sgatCharServ[sgnCharServ].szAddress);
	return 1;
}

BOOL __fastcall CharServConnect(HWND hDlg)
{
	BOOL fResult;
	HANDLE hThread;
	DWORD dwThreadId;
	MSG tMsg;
	BOOL fRunning;
	char *pszAddress;
	int i;

	fResult = FALSE;
	fRunning = TRUE;
	hThread = CreateThread(NULL, 0, CharServThread, NULL, 0, &dwThreadId);
	while (fRunning && WaitForSingleObject(hThread, 0) != WAIT_OBJECT_0)
	{
		if (PeekMessageA(&tMsg, NULL, 0, 0, PM_NOREMOVE))
		{
			fRunning = GetMessageA(&tMsg, NULL, 0, 0);
			if (!IsDialogMessageA(hDlg, &tMsg))
			{
				TranslateMessage(&tMsg);
				DispatchMessageA(&tMsg);
			}
		}
		else
			Sleep(10);
	}
	for (i = 0; i < IniGetNumCharServ(); i++)
	{
		pszAddress = sgatCharServ[i].szAddress;
		if (pszAddress && pszAddress[0] != '0' && FogD2QSSocketGetConnection(sgatCharServ[i].pConnection))
			fResult = TRUE;
	}
	return fResult;
}

BOOL __fastcall CharServWatchdog(void)
{
	char szName[256];
	BOOL fResult;
	DWORD dwNow;
	BYTE bTag;
	int i;

	fResult = FALSE;
	dwNow = time(NULL);
	if (!sgdwLastPing)
		sgdwLastPing = dwNow;
	if (dwNow - sgdwLastPing < 60)
		return TRUE;
	sgdwLastPing = dwNow;
	for (i = 0; i < IniGetNumCharServ(); i++)
	{
		if (!sgatCharServ[i].szAddress[0] || sgatCharServ[i].szAddress[0] == '0')
			continue;
		if (FogD2QSSocketGetConnection(sgatCharServ[i].pConnection))
		{
			bTag = 8;
			CharServSend(&bTag, 1);
			fResult = TRUE;
			continue;
		}
		FogD2QSSocketDestroy(sgatCharServ[i].pConnection);
		sgatCharServ[i].pConnection = NULL;
		bTag = 5;
		if (sgatCharServ[i].hThread)
		{
			if (WaitForSingleObject(sgatCharServ[i].hThread, 0) == WAIT_TIMEOUT)
				continue;
			CloseHandle(sgatCharServ[i].hThread);
			sgatCharServ[i].hThread = NULL;
			if (!sgatCharServ[i].pConnection)
			{
				StormSStrPrintf((DWORD)szName, 256, (DWORD)"Game Server->Char Server #%d", i);
				sgatCharServ[i].pConnection = FogD2QSSocketCreate(sgatCharServ[i].szAddress, 6113,
					szName);
				sgatCharServ[i].hThread = FogD2QSSocketStartAsync(sgatCharServ[i].pConnection);
			}
			else
			{
				FogD2QSSocketSendRaw(sgatCharServ[i].pConnection, &bTag, 1);
				ServerLog(TRUE, "[CHARACTER SERVER]  Connected to Character Server at %s", sgatCharServ[i].szAddress);
			}
		}
		else
		{
			ServerLog(TRUE, "[CHARACTER SERVER]  Lost connection with %s", sgatCharServ[i].szAddress);
			StormSStrPrintf((DWORD)szName, 256, (DWORD)"Game Server->Char Server #%d", i);
			sgatCharServ[i].pConnection = FogD2QSSocketCreate(sgatCharServ[i].szAddress, 6113, szName);
			sgatCharServ[i].hThread = FogD2QSSocketStartAsync(sgatCharServ[i].pConnection);
			ServerLog(TRUE, "[CHARACTER SERVER]  Connecting to Character Server at %s", sgatCharServ[i].szAddress);
		}
	}
	return fResult;
}

void __fastcall CharServDisconnect(void)
{
	int i;

	for (i = 0; i < MAX_CHAR_SERV; i++)
	{
		if (sgatCharServ[i].pConnection)
			FogD2QSSocketDestroy(sgatCharServ[i].pConnection);
		sgatCharServ[i].pConnection = NULL;
	}
}

BOOL __fastcall CharServDrain(void)
{
	BYTE abMsg[4128];
	WORD wLength;
	int i;

	for (i = 0; i < IniGetNumCharServ(); i++)
	{
		if (!sgatCharServ[i].pConnection)
			continue;
		wLength = (WORD)FogD2QSSocketReceive(sgatCharServ[i].pConnection, abMsg, sizeof abMsg);
		if (wLength)
			CharServDispatch(0, abMsg, wLength);
	}
	return TRUE;
}
