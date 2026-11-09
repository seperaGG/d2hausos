#include <stddef.h>
#include <limits.h>

#define D2_THIS_FILE "C:\\D2\\Source\\D2MCPClient\\Src\\ToMCP.cpp"
#include "D2MCPClient.h"
#include "D2Macros.h"

#pragma pack(push, 1)

struct D2MCPMSG01
{
 	BYTE		bId;
 	DWORD		field_01;
 	DWORD		field_05;
 	DWORD		field_09;
 	DWORD		field_0d;
 	DWORD		field_11;
 	DWORD		field_15;
 	DWORD		field_19;
 	BYTE		field_1d[20];
 	char		szText[D2MCP_TEXT_MAX];
};

struct D2MCPMSG02
{
 	BYTE		bId;
 	DWORD		dwClass;
 	WORD		wFlags;
 	char		szText[D2MCP_TEXT_MAX];
};

struct D2MCPMSG03
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwFlags;
 	BYTE		field_07;
 	BYTE		bLevelDifference;
 	BYTE		bMaxPlayers;
 	char		szText[D2MCP_TEXT_MAX];
};

struct D2MCPMSG05
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwFlags;
 	char		szText[D2MCP_TEXT_MAX];
};

struct D2MCPMSGHEAD
{
 	BYTE		bId;
 	WORD		wToken;
};

struct D2MCPMSGTOKEN
{
 	BYTE		bId;
 	WORD		wToken;
 	char		szText[D2MCP_TEXT_MAX];
};

struct D2MCPMSGTEXT
{
 	BYTE		bId;
 	char		szText[D2MCP_TEXT_MAX];
};

struct D2MCPMSG11
{
 	BYTE		bId;
 	BYTE		bValue;
};
#pragma pack(pop)

static char *ToMCPAppendString(char *pszTexts, const char *pszText, DWORD *lpdwSize, DWORD dwMaxSize)
{
	DWORD dwLen = strlen(pszText);

	*lpdwSize += dwLen + 1;
#line 38
	D2ASSERT(*lpdwSize < dwMaxSize);
	strcpy(pszTexts, pszText);
	return pszTexts + dwLen + 1;
}

BOOL __fastcall ToMCPSendHandshake(D2MCPCONNECT *ptConnect)
{
	D2MCPMSG01 tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.bId = 1;
	tMsg.field_01 = ptConnect->field_00;
	tMsg.field_05 = ptConnect->field_04;
	tMsg.field_09 = ptConnect->field_08;
	tMsg.field_0d = ptConnect->field_0c;
	tMsg.field_11 = ptConnect->field_18;
	tMsg.field_15 = ptConnect->field_1c;
	tMsg.field_19 = ptConnect->field_20;
	memcpy(tMsg.field_1d, ptConnect->field_24, sizeof(tMsg.field_1d));
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, ptConnect->field_38, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSG01, szText);
#line 84
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendCreateCharacter(const char *pszName, DWORD dwClass, WORD wFlags)
{
	D2MCPMSG02 tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.dwClass = dwClass;
	tMsg.wFlags = wFlags;
	tMsg.bId = 2;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, pszName, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSG02, szText);
#line 117
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendSelectCharacter(const char *pszName)
{
	D2MCPMSGTEXT tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.bId = 7;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, pszName, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTEXT, szText);
#line 145
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendCreateGame(D2MCPGAMECREATE *ptGame, WORD wToken)
{
	D2MCPMSG03 tMsg;
	DWORD dwSize;
	char *pszTo;

	D2MCPSetStatus(0);
	tMsg.dwFlags = ptGame->dwFlags;
	tMsg.field_07 = ptGame->field_44;
	tMsg.bLevelDifference = ptGame->bLevelDifference;
	tMsg.bMaxPlayers = ptGame->bMaxPlayers;
	gtMCP.wToken = wToken;
	tMsg.bId = 3;
	tMsg.wToken = wToken;
	dwSize = 0;
	pszTo = ToMCPAppendString(tMsg.szText, ptGame->szGameName, &dwSize, D2MCP_TEXT_MAX);
	pszTo = ToMCPAppendString(pszTo, ptGame->szPassword, &dwSize, D2MCP_TEXT_MAX);
	ToMCPAppendString(pszTo, ptGame->szDescription, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSG03, szText);
#line 184
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendJoinGame(const char *pszText1, const char *pszText2, WORD wToken)
{
	D2MCPMSGTOKEN tMsg;
	DWORD dwSize;
	char *pszTo;

	D2MCPSetStatus(0);
	tMsg.bId = 4;
	tMsg.wToken = wToken;
	gtMCP.wToken = wToken;
	dwSize = 0;
	pszTo = ToMCPAppendString(tMsg.szText, pszText1, &dwSize, D2MCP_TEXT_MAX);
	ToMCPAppendString(pszTo, pszText2, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTOKEN, szText);
#line 221
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendGameListQuery(D2MCPGAMEQUERY *ptQuery, WORD wToken)
{
	D2MCPMSG05 tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.wToken = wToken;
	tMsg.dwFlags = ptQuery->dwFlags;
	gtMCP.wToken = wToken;
	tMsg.bId = 5;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, ptQuery->field_00, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSG05, szText);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendGameDetailsQuery(const char *pszText, WORD wToken)
{
	D2MCPMSGTOKEN tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	gtMCP.wToken = wToken;
	tMsg.bId = 6;
	tMsg.wToken = wToken;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, pszText, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTOKEN, szText);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendTextAndWordFirst(const char *pszText, WORD wToken)
{
	D2MCPMSGTOKEN tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.bId = 8;
	tMsg.wToken = wToken;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, pszText, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTOKEN, szText);
#line 316
	D2ASSERT(dwSize < USHRT_MAX);
	gtMCP.wToken = wToken;
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendTextAndWordSecond(const char *pszText, WORD wToken)
{
	D2MCPMSGTOKEN tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.bId = 9;
	tMsg.wToken = wToken;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, pszText, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTOKEN, szText);
#line 349
	D2ASSERT(dwSize < USHRT_MAX);
	gtMCP.wToken = wToken;
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendDeleteCharacter(WORD wToken)
{
	D2MCPMSGHEAD tMsg;

	D2MCPSetStatus(0);
	tMsg.bId = 10;
	tMsg.wToken = wToken;
	gtMCP.wToken = wToken;
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, sizeof(tMsg));
}

BOOL __fastcall ToMCPSendTwoTexts(const char *pszText1, const char *pszText2)
{
	D2MCPMSGTEXT tMsg;
	DWORD dwSize;
	char *pszTo;

	D2MCPSetStatus(0);
	tMsg.bId = 11;
	dwSize = 0;
	pszTo = ToMCPAppendString(tMsg.szText, pszText1, &dwSize, D2MCP_TEXT_MAX);
	ToMCPAppendString(pszTo, pszText2, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTEXT, szText);
#line 402
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __fastcall ToMCPSendText(const char *pszText)
{
	D2MCPMSGTEXT tMsg;
	DWORD dwSize;

	D2MCPSetStatus(0);
	tMsg.bId = 12;
	dwSize = 0;
	ToMCPAppendString(tMsg.szText, pszText, &dwSize, D2MCP_TEXT_MAX);
	dwSize += offsetof(D2MCPMSGTEXT, szText);
#line 430
	D2ASSERT(dwSize < USHRT_MAX);
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, dwSize);
}

BOOL __cdecl ToMCPSendBareFirst(void)
{
	BYTE bId;

	D2MCPSetStatus(0);
	bId = 13;
	return FogD2QSSocketSend(gtMCP.ptSocket, &bId, sizeof(bId));
}

BOOL __cdecl ToMCPSendBareSecond(void)
{
	BYTE bId;

	D2MCPSetStatus(0);
	bId = 15;
	return FogD2QSSocketSend(gtMCP.ptSocket, &bId, sizeof(bId));
}

BOOL __cdecl ToMCPSendBareThird(void)
{
	BYTE bId;

	D2MCPSetStatus(0);
	bId = 16;
	return FogD2QSSocketSend(gtMCP.ptSocket, &bId, sizeof(bId));
}

BOOL __fastcall ToMCPSendLadder(BYTE bValue)
{
	D2MCPMSG11 tMsg;

	D2MCPSetStatus(0);
	tMsg.bId = 17;
	tMsg.bValue = bValue;
	return FogD2QSSocketSend(gtMCP.ptSocket, &tMsg, sizeof(tMsg));
}

BOOL __cdecl ToMCPSendNewsRequest(void)
{
	BYTE bId;

	D2MCPSetStatus(0);
	bId = 18;
	return FogD2QSSocketSend(gtMCP.ptSocket, &bId, sizeof(bId));
}
