#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\CORE\\ToMCP.cpp"
#include "D2Server.h"
#include "D2Macros.h"

#pragma pack(push, 1)
struct TOMCPID
{
	BYTE		bTag;
	DWORD		dwId1;
	DWORD		dwId2;
	DWORD		field_09;
	DWORD		field_0d;
};

struct TOMCPNOINFO
{
	BYTE		bTag;
	DWORD		field_01;
	DWORD		field_05;
	BYTE		field_09;
	BYTE		unobserved_0a[255];
	BYTE		unobserved_109[3];
};

struct TOMCPGAMEINFO
{
	BYTE		bTag;
	DWORD		field_01;
	DWORD		field_05;
	BYTE		field_09;
	BYTE		field_0a;
	BYTE		field_0b;
	BYTE		field_0c;
	DWORD		fMore;
	char		szText[515];
};

struct TOMCPDWORD
{
	BYTE		bTag;
	DWORD		dwValue;
	BYTE		unobserved_05[3];
};

struct TOMCPCREATE
{
	BYTE		bTag;
	DWORD		dwMcpId;
	DWORD		dwServId;
	DWORD		dwError;
	BYTE		unobserved_0d[3];
};

struct TOMCPJOIN
{
	BYTE		bTag;
	DWORD		dwMcpId;
	DWORD		dwServId;
	DWORD		dwToken;
	DWORD		dwError;
	BYTE		unobserved_11[3];
};

struct TOMCPLEFT
{
	BYTE		bTag;
	DWORD		dwGameId;
	DWORD		dwPacked;
	WORD		wLevel;
	DWORD		dwExperience;
	DWORD		field_0f;
	WORD		wFlags;
	char		szText[515];
};

struct TOMCP0A
{
	BYTE		bTag;
	DWORD		dwValue;
	WORD		field_05;
	WORD		field_07;
	WORD		field_09;
	char		szText[513];
};

struct TOMCPGUILD
{
	BYTE		bTag;
	char		szTag[4];
	WORD		wOffset;
	WORD		wChunk;
	WORD		wTotal;
	BYTE		abData[GUILD_CHUNK];
	BYTE		unobserved_19b;
};

struct TOMCP0E
{
	BYTE		bTag;
	BYTE		field_01;
	char		szText1[4];
	DWORD		field_06;
	char		szText2[16];
	BYTE		unobserved_1a[2];
};

struct TOMCPCHAR
{
	BYTE		bTag;
	DWORD		dwPacked;
	unsigned __int64	qwExperience;
	WORD		wFlags;
	char		szName[513];
};

struct TOMCP10
{
	BYTE		bTag;
	DWORD		dwValue;
	BYTE		bClass;
	BYTE		field_06;
	char		szText[513];
};

struct TOMCPSTATUS
{
	BYTE		bTag;
	WORD		field_01;
	WORD		wMaxGames;
	WORD		field_05;
	WORD		wMaxPlayers;
	DWORD		dwUptime;
	DWORD		adwMemory[4];
	DWORD		dwDiskFree;
	BYTE		field_21;
};
#pragma pack(pop)

static DWORD __fastcall ToMCPPackCharacter(DWORD dwClass, DWORD dwLevel, WORD dwFlags)
{
	DWORD dwPacked;

	dwPacked = ((dwLevel & 255) << 16) | (dwClass & 7);
	if (dwFlags & 4)
	{
		dwPacked |= 32;
		if (dwFlags & 8)
			dwPacked |= 16;
	}
	if ((dwFlags & 7936) >= 1024)
		dwPacked = (dwPacked & ~7936) | (dwFlags & 7936);
	return dwPacked;
}

BOOL __fastcall ToMCPSendIdentityRequest(void)
{
	BYTE bTag;

	bTag = 1;
	return FogD2QSSocketSend(gpMcpConnection, &bTag, 1);
}

BOOL __fastcall ToMCPSendVersion(void)
{
	TOMCPID tRecord;

	tRecord.field_09 = 0;
	tRecord.field_0d = 0;
	tRecord.bTag = 2;
	tRecord.dwId1 = 1230518326;
	tRecord.dwId2 = 1144144982;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, sizeof(TOMCPID));
}

BOOL __fastcall ToMCPSendNoInfo(void)
{
	TOMCPNOINFO tRecord;

	tRecord.field_01 = 0;
	tRecord.field_05 = 0;
	tRecord.field_09 = 0;
	tRecord.bTag = 3;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 265);
}

BOOL __fastcall ToMCPSendGameInfo(D2SERVERGAMEINFO *ptGame, BOOL fLast)
{
	TOMCPGAMEINFO tRecord;
	char *pszText;
	int nLength;
	WORD wSize;

	tRecord.field_01 = ptGame->adwStats[0];
	tRecord.field_05 = ptGame->dwArenaFlags;
	tRecord.field_09 = ptGame->field_84;
	tRecord.field_0a = ptGame->field_85;
	tRecord.field_0b = ptGame->field_86;
	tRecord.field_0c = (BYTE)ptGame->adwStats[2];
	tRecord.bTag = 4;
	tRecord.fMore = fLast == 0;
	nLength = StormSStrCopy(tRecord.szText, ptGame->szName, 512);
	pszText = tRecord.szText + nLength + 1;
	nLength = StormSStrCopy(pszText, ptGame->field_74, 512);
	pszText += nLength + 1;
	wSize = StormSStrCopy(pszText, gszIniBlank, 512) + (WORD)(pszText - tRecord.szText) + 18;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, wSize);
}

BOOL __fastcall ToMCPGameList(void)
{
	D2SERVERGAMEINFO tGame;
	WORD awGameIds[1024];
	int nGames;
	int i;

	D2gameGameGetCount();
	nGames = D2gameGameGetIds(awGameIds, 1024);
	for (i = 0; i < nGames; i++)
		ToMCPSendGameInfo(&tGame, i == nGames - 1);
	return TRUE;
}

BOOL __fastcall ToMCPCloseGame(DWORD dwGameId)
{
	TOMCPDWORD tRecord;

	tRecord.dwValue = dwGameId;
	tRecord.bTag = 5;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 5);
}

BOOL __stdcall ToMCPCreateGameReply(int nServId, int nMcpId, int nError)
{
	TOMCPCREATE tRecord;

	ServerLog(FALSE, "[Server->MCP]  (GTM_CREATEGAME)  MCP ID: %d  SERV ID: %d  ERROR: %d", nMcpId, nServId,
		nError);
	tRecord.bTag = 6;
	tRecord.dwMcpId = nMcpId;
	tRecord.dwServId = nServId;
	tRecord.dwError = nError;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 13);
}

BOOL __fastcall ToMCPJoinGameReply(DWORD dwMcpId, DWORD dwServId, DWORD dwToken, DWORD dwError)
{
	TOMCPJOIN tRecord;

	ServerLog(FALSE, "[Server->MCP]  (GTM_JOINGAME)  MCP ID: %d  SERV ID: %d  TOK: %08x  ERR: %08x", dwMcpId,
		dwServId, dwToken, dwError);
	tRecord.bTag = 7;
	tRecord.dwMcpId = dwMcpId;
	tRecord.dwServId = dwServId;
	tRecord.dwToken = dwToken;
	tRecord.dwError = dwError;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 17);
}

BOOL __fastcall ToMCPPlayerLeft(DWORD dwGameId, DWORD dwClass, DWORD dwLevel, DWORD dwExperience,
	DWORD dwExperienceHigh, DWORD dwFlags, const char *pszName, const char *pszSummary)
{
	TOMCPLEFT tRecord;
	char *pszText;
	int nLength;

	tRecord.dwPacked = ToMCPPackCharacter(dwClass, dwLevel, dwFlags);
	tRecord.dwExperience = dwExperience;
	tRecord.wFlags = (WORD)dwFlags;
	tRecord.dwGameId = dwGameId;
	tRecord.bTag = 8;
	tRecord.wLevel = (WORD)dwLevel;
	tRecord.field_0f = dwExperienceHigh;
	nLength = StormSStrCopy(tRecord.szText, pszName, 512);
	pszText = tRecord.szText + nLength + 1;
	nLength = StormSStrCopy(pszText, pszSummary, 512);
	nLength += (pszText - tRecord.szText) + 22;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, nLength);
}

BOOL __fastcall ToMCPSendPlayerEntered(DWORD dwArg, const char *pszText, WORD wClass, WORD wLevel, WORD wArg3)
{
	TOMCP0A tRecord;
	int nLength;

	tRecord.dwValue = dwArg;
	tRecord.field_05 = wClass;
	tRecord.field_07 = wLevel;
	tRecord.bTag = 10;
	tRecord.field_09 = wArg3;
	nLength = StormSStrCopy(tRecord.szText, pszText, 512);
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, nLength + 12);
}

BOOL __fastcall ToMCPSendDword(DWORD dwArg)
{
	TOMCPDWORD tRecord;

	tRecord.dwValue = dwArg;
	tRecord.bTag = 9;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 5);
}

BOOL __fastcall ToMCPSendTick(void)
{
	BYTE bTag;

	bTag = 11;
	return FogD2QSSocketSend(gpMcpConnection, &bTag, 1);
}

BOOL __fastcall ToMCPGuildSave(const char *pszTag, const BYTE *pbData, int nLength)
{
	TOMCPGUILD tRecord;
	BOOL fResult;

	if (!nLength)
		return TRUE;
	tRecord.bTag = 12;
	strcpy(tRecord.szTag, pszTag);
	tRecord.wTotal = (WORD)nLength;
	tRecord.wOffset = 0;
	do
	{
		tRecord.wChunk = GUILD_CHUNK;
		if (nLength <= GUILD_CHUNK)
			tRecord.wChunk = (WORD)nLength;
		memcpy(tRecord.abData, pbData + tRecord.wOffset, tRecord.wChunk);
		fResult = FogD2QSSocketSend(gpMcpConnection, &tRecord, tRecord.wChunk + 11);
		nLength -= tRecord.wChunk;
		tRecord.wOffset += tRecord.wChunk;
	}
	while (nLength);
	return fResult;
}

BOOL __fastcall ToMCPSendTwoTexts(const char *pszText1, BYTE bArg, const char *pszText2, DWORD dwArg)
{
	TOMCP0E tRecord;

	tRecord.field_06 = dwArg;
	tRecord.field_01 = bArg;
	tRecord.bTag = 14;
	strcpy(tRecord.szText1, pszText1);
	strcpy(tRecord.szText2, pszText2);
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 26);
}

BOOL __fastcall ToMCPCharInfo(const char *pszName, WORD wClass, DWORD dwLevel, unsigned __int64 qwExperience,
	DWORD dwFlags)
{
	TOMCPCHAR tRecord;
	int nLength;

#line 297
	D2ASSERT(wClass < NUM_PLAYERCLASS);
	if ((WORD)(dwFlags & 4))
	{
		if (qwExperience < gtLadderLimits.qwHardcoreLimit && qwExperience
			< gtLadderLimits.aqwHardcoreClassLimits[wClass])
			return FALSE;
	}
	else
	{
		if (qwExperience < gtLadderLimits.qwStandardLimit && qwExperience
			< gtLadderLimits.aqwStandardClassLimits[wClass])
			return FALSE;
	}
	tRecord.dwPacked = ToMCPPackCharacter(wClass, dwLevel, dwFlags);
	tRecord.bTag = 15;
	tRecord.qwExperience = qwExperience;
	tRecord.wFlags = (WORD)dwFlags;
	nLength = StormSStrCopy(tRecord.szName, pszName, 512);
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, nLength + 16);
}
BOOL __fastcall ToMCPSendIdClassText(DWORD dwArg, const char *pszText, WORD wClass, BYTE bArg)
{
	TOMCP10 tRecord;
	int nLength;

#line 349
	D2ASSERT(wClass < NUM_PLAYERCLASS);
	tRecord.dwValue = dwArg;
	tRecord.bClass = (BYTE)wClass;
	tRecord.bTag = 16;
	tRecord.field_06 = bArg;
	nLength = StormSStrCopy(tRecord.szText, pszText, 512);
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, nLength + 8);
}

BOOL __fastcall ToMCPStatus(const D2SERVERSTATUS *ptStatus)
{
	TOMCPSTATUS tRecord;
	unsigned __int64 qwDiskFree;

	tRecord.bTag = 17;
	tRecord.field_01 = ptStatus->wNumGames;
	tRecord.wMaxGames = (WORD)ServerGetMaxGames();
	tRecord.field_05 = ptStatus->field_02;
	tRecord.wMaxPlayers = (WORD)ServerGetMaxPlayers();
	tRecord.dwUptime = ServerGetUptime();
	FogAsyncDataGetMemoryStatus(&tRecord.adwMemory[0], &tRecord.adwMemory[1], &tRecord.adwMemory[2],
		&tRecord.adwMemory[3]);
	qwDiskFree = ((unsigned __int64 (__cdecl *)(void))FogAsyncDataGetFreeDiskSpaceRaw)() >> 10;
	if (qwDiskFree > 0xFFFFFFFF)
		qwDiskFree = 0xFFFFFFFF;
	tRecord.dwDiskFree = (DWORD)qwDiskFree;
	tRecord.field_21 = ptStatus->bFramesPerSecond;
	return FogD2QSSocketSend(gpMcpConnection, &tRecord, 34);
}
