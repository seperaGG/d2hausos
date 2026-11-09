#include "D2MCPClient.h"
#include "StormImports.h"

#pragma pack(push, 1)
struct D2MCPREPLYSTATUS
{
 	BYTE		bId;
 	DWORD		dwStatus;
};

struct D2MCPREPLY03
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwGameId;
 	DWORD		dwStatus;
};

struct D2MCPREPLY04
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwGameId;
 	DWORD		dwAddress;
 	DWORD		dwGameToken;
 	DWORD		dwStatus;
};

struct D2MCPREPLY05
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwGameKey;
 	BYTE		bNumPlayers;
 	DWORD		dwStatus;
 	char		szText[1];
};

struct D2MCPREPLY06
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwStatus;
 	DWORD		dwElapsedSeconds;
 	BYTE		bLevel;
 	BYTE		bLevelDifference;
 	BYTE		bMaxPlayers;
 	BYTE		bCount;
 	BYTE		abClasses[16];
 	BYTE		abLevels[16];
 	char		szText[1];
};

struct D2MCPREPLYWORD
{
 	BYTE		bId;
 	WORD		wValue;
 	BYTE		abData[1];
};

struct D2MCPREPLY0A
{
 	BYTE		bId;
 	WORD		wToken;
 	DWORD		dwStatus;
};

struct D2MCPREPLY0E
{
 	BYTE		bId;
 	char		szText[1];
};

struct D2MCPREPLY10
{
 	BYTE		bId;
 	DWORD		dwGameId;
 	DWORD		dwGameToken;
 	DWORD		dwAddress;
 	DWORD		dwStatus;
};

struct D2MCPREPLY11
{
 	BYTE		bId;
 	BYTE		field_01;
 	WORD		wTotal;
 	WORD		wLength;
 	WORD		wOffset;
 	BYTE		abData[1];
};

struct D2MCPREPLY12
{
 	BYTE		bId;
 	BYTE		bType;
 	BYTE		abData[1];
};
#pragma pack(pop)

D2MCPLADDERDATA gtLadder = {0};
static int sgnLadderReceived = 0;
static int sgnLadderTotal = 0;
D2MCPLADDERBLOCK gtLadderBlock = {0};

BOOL __fastcall D2MCPOnNothing(BYTE *pbMsg, DWORD dwLength)
{
	return TRUE;
}

BOOL __fastcall D2MCPOnHandshakeReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	SetEvent(gtMCP.hReplyEvent);
	return TRUE;
}

BOOL __fastcall D2MCPOnCreateCharacterReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	D2MCPSetCreateCharacterReply(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnSelectCharacterReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	D2MCPSetSelectCharacterReply(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnCreateGameReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY03 *ptReply = (D2MCPREPLY03 *)pbMsg;
	DWORD dwGameId = ptReply->dwGameId;

	if (ptReply->wToken == gtMCP.wToken)
	{
		D2MCPSetStatus(ptReply->dwStatus);
		gtMCP.dwGameId = dwGameId;
		gtMCP.wReplyToken = gtMCP.wToken;
	}
	return TRUE;
}

BOOL __fastcall D2MCPOnJoinGameReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY04 *ptReply = (D2MCPREPLY04 *)pbMsg;
	DWORD dwGameId = ptReply->dwGameId;
	DWORD dwAddress = ptReply->dwAddress;
	DWORD dwGameToken = ptReply->dwGameToken;
	DWORD dwStatus = ptReply->dwStatus;

	if (ptReply->wToken == gtMCP.wToken)
	{
		gtMCP.wReplyToken = gtMCP.wToken;
		gtMCP.dwGameId = dwGameId;
		gtMCP.dwGameToken = dwGameToken;
		if (!dwAddress)
		{
			if (dwStatus)
				D2MCPSetStatus(dwStatus);
			else
				D2MCPSetStatus(D2MCP_STATUS_NOGAME);
			return TRUE;
		}
		strcpy(gtMCP.szGameAddress, FogSafesockAddressToString(dwAddress));
	}
	return TRUE;
}

BOOL __fastcall D2MCPOnGameListReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY05 *ptReply = (D2MCPREPLY05 *)pbMsg;
	DWORD dwGameKey = ptReply->dwGameKey;
	DWORD dwStatus = ptReply->dwStatus;
	char *pszText;
	char *pszText2;
	char *pszText1;

	if (ptReply->wToken == gtMCP.wToken)
	{
		gtMCP.tGame.wToken = gtMCP.wToken;
		gtMCP.tGame.dwGameKey = dwGameKey;
		gtMCP.tGame.dwNumPlayers = ptReply->bNumPlayers;
		gtMCP.tGame.dwStatus = dwStatus;
		if (dwStatus == (DWORD)-2)
		{
			D2MCPSetStatus(51);
			return TRUE;
		}
		if (dwStatus == (DWORD)-1)
		{
			D2MCPSetStatus(50);
			return TRUE;
		}
		pszText = ptReply->szText;
		pszText1 = pszText;
		pszText += strlen(pszText) + 1;
		pszText2 = pszText;
		pszText += strlen(pszText) + 1;
		strcpy(gtMCP.tGame.szGameName, pszText1);
		strcpy(gtMCP.tGame.szDescription, pszText2);
		gtMCP.tGame.fTrailingData = (DWORD)(pszText - (char *)pbMsg) < dwLength;
	}
	return TRUE;
}

BOOL __fastcall D2MCPOnGameDetailsReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY06 *ptReply = (D2MCPREPLY06 *)pbMsg;
	char *pszText;
	char *pszDescription;
	char *pszName;
	int i;
	DWORD dwStatus = ptReply->dwStatus;
	DWORD dwElapsedSeconds = ptReply->dwElapsedSeconds;

	if (ptReply->wToken == gtMCP.wToken)
	{
		gtMCP.tGame.wToken = gtMCP.wToken;
		gtMCP.tGame.dwStatus = dwStatus;
		gtMCP.tGame.dwNumPlayers = ptReply->bCount;
		gtMCP.tGame.dwElapsedSeconds = dwElapsedSeconds;
		gtMCP.tGame.bLevel = ptReply->bLevel;
		gtMCP.tGame.bLevelDifference = ptReply->bLevelDifference;
		gtMCP.tGame.bMaxPlayers = ptReply->bMaxPlayers;
		if (dwStatus == (DWORD)-1)
		{
			D2MCPSetStatus(50);
			return TRUE;
		}
		if (dwStatus == (DWORD)-2)
		{
			D2MCPSetStatus(51);
			return TRUE;
		}
		pszText = ptReply->szText;
		pszDescription = pszText;
		pszText += strlen(pszText) + 1;
		strcpy(gtMCP.tGame.szDescription, pszDescription);
		for (i = 0; i < ptReply->bCount; i++)
		{
			gtMCP.tGame.awClasses[i] = ptReply->abClasses[i];
			gtMCP.tGame.awLevels[i] = ptReply->abLevels[i];
			pszName = pszText;
			pszText += strlen(pszText) + 1;
			strcpy(gtMCP.tGame.aszPlayerNames[i], pszName);
		}
	}
	return TRUE;
}

BOOL __fastcall D2MCPOnCallbackFirst(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLYWORD *ptReply = (D2MCPREPLYWORD *)pbMsg;

	if (gtLadder.pfnCallback08)
		gtLadder.pfnCallback08(ptReply->wValue, ptReply->abData);
	return TRUE;
}

BOOL __fastcall D2MCPOnCallbackSecond(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLYWORD *ptReply = (D2MCPREPLYWORD *)pbMsg;

	if (gtLadder.pfnCallback09)
		gtLadder.pfnCallback09(ptReply->wValue, ptReply->abData + 1);
	return TRUE;
}

BOOL __fastcall D2MCPOnDeleteCharacterReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY0A *ptReply = (D2MCPREPLY0A *)pbMsg;

	if (ptReply->wToken == gtMCP.wToken)
	{
		D2MCPSetStatus(ptReply->dwStatus);
		D2MCPSetDeleteCharacterReply(TRUE);
	}
	return TRUE;
}

BOOL __fastcall D2MCPOnStatusReplyFirst(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	D2MCPSetStatusReplyFirst(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnStatusReplySecond(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	D2MCPSetStatusReplySecond(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnTextPair(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY0E *ptReply = (D2MCPREPLY0E *)pbMsg;
	char *pszText1 = ptReply->szText;
	char *pszText2 = pszText1 + strlen(pszText1) + 1;

	D2MCPSetTextPair(1, pszText1, pszText2);
	return TRUE;
}

BOOL __fastcall D2MCPOnStatusReplyThird(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	D2MCPSetStatusReplyThird(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnStatusReplyFourth(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPSetStatus(((D2MCPREPLYSTATUS *)pbMsg)->dwStatus);
	D2MCPSetStatusReplyFourth(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnAddressReply(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY10 *ptReply = (D2MCPREPLY10 *)pbMsg;
	DWORD dwGameId = ptReply->dwGameId;
	DWORD dwAddress = ptReply->dwAddress;
	DWORD dwGameToken = ptReply->dwGameToken;
	DWORD dwStatus = ptReply->dwStatus;

	D2MCPSetStatus(dwStatus);
	gtMCP.dwGameId = dwGameId;
	gtMCP.dwGameToken = dwGameToken;
	if (!dwAddress)
	{
		if (dwStatus)
			D2MCPSetStatus(dwStatus);
		else
			D2MCPSetStatus(D2MCP_STATUS_NOGAME);
		D2MCPSetAddressReply(TRUE);
		return TRUE;
	}
	strcpy(gtMCP.szGameAddress, FogSafesockAddressToString(dwAddress));
	D2MCPSetAddressReply(TRUE);
	return TRUE;
}

BOOL __fastcall D2MCPOnLadderChunk(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY11 *ptReply = (D2MCPREPLY11 *)pbMsg;
	WORD wTotal = ptReply->wTotal;
	WORD wChunk = ptReply->wLength;
	WORD wOffset = ptReply->wOffset;

	if (dwLength < 8)
		return FALSE;
	if (wTotal < 8)
		return FALSE;
	if (wTotal != sgnLadderTotal)
		sgnLadderTotal = wTotal;
	sgnLadderReceived = wOffset;
	if (sgnLadderReceived + wChunk > sgnLadderTotal)
		return FALSE;
	memcpy((BYTE *)&gtLadder.tLadder + sgnLadderReceived, ptReply->abData, wChunk);
	sgnLadderReceived += wChunk;
	if (sgnLadderReceived < sgnLadderTotal)
		return TRUE;
	gtLadderBlock.dwLadderCount = gtLadder.tLadder.dwCount;
	gtLadderBlock.dwLadderSize = gtLadder.tLadder.dwSize;
	if (sgnLadderTotal != (int)(((gtLadder.tLadder.dwSize + 12) * gtLadder.tLadder.dwCount + 8) & 65535))
		return FALSE;
	if (gtLadder.pfnLadder)
		gtLadder.pfnLadder(gtLadder.tLadder.dwCount, gtLadder.tLadder.dwSize, gtLadder.tLadder.abEntries);
	return TRUE;
}

BOOL __fastcall D2MCPOnNews(BYTE *pbMsg, DWORD dwLength)
{
	D2MCPREPLY12 *ptReply = (D2MCPREPLY12 *)pbMsg;

	if (dwLength < 2)
		return FALSE;
	if (gtLadder.pfnNews)
		gtLadder.pfnNews(ptReply->bType, dwLength - 2, ptReply->abData);
	return TRUE;
}

static D2MCPRECV sgapfnPollHandlers[D2MCP_NUM_MESSAGES] =
{
	D2MCPOnNothing,
	NULL,
	D2MCPOnCreateCharacterReply,
	D2MCPOnCreateGameReply,
	D2MCPOnJoinGameReply,
	D2MCPOnGameListReply,
	D2MCPOnGameDetailsReply,
	D2MCPOnSelectCharacterReply,
	D2MCPOnCallbackFirst,
	D2MCPOnCallbackSecond,
	D2MCPOnDeleteCharacterReply,
	D2MCPOnStatusReplyFirst,
	D2MCPOnStatusReplySecond,
	D2MCPOnStatusReplyThird,
	D2MCPOnTextPair,
	D2MCPOnStatusReplyFourth,
	D2MCPOnAddressReply,
	D2MCPOnLadderChunk,
	D2MCPOnNews,
};

static D2MCPRECV sgapfnSocketHandlers[D2MCP_NUM_MESSAGES] =
{
	NULL,
	D2MCPOnHandshakeReply,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

BOOL __fastcall D2MCPDispatchPoll(BYTE *pbMsg, DWORD dwLength)
{
	BOOL fResult = FALSE;
	BYTE bId;
	D2MCPRECV pfnRecv;

	if (dwLength <= 0)
		return FALSE;
	bId = pbMsg[0];
	if (bId >= D2MCP_NUM_MESSAGES)
		return FALSE;
	pfnRecv = sgapfnPollHandlers[bId];
	if (pfnRecv)
		fResult = pfnRecv(pbMsg, dwLength);
	return fResult;
}

BOOL __fastcall D2MCPDispatchSocket(BYTE *pbMsg, DWORD dwLength, void *pContext)
{
	BOOL fResult = FALSE;
	BYTE bId;
	D2MCPRECV pfnRecv;

	if (dwLength <= 0)
		return FALSE;
	bId = pbMsg[0];
	if (bId >= D2MCP_NUM_MESSAGES)
		return FALSE;
	pfnRecv = sgapfnSocketHandlers[bId];
	if (pfnRecv)
		fResult = pfnRecv(pbMsg, dwLength);
	return fResult;
}

void __fastcall D2MCPSetCallbackFirst(D2MCPWORDCALLBACK pfn)
{
	gtLadder.pfnCallback08 = pfn;
}

void __fastcall D2MCPSetCallbackSecond(D2MCPWORDCALLBACK pfn)
{
	gtLadder.pfnCallback09 = pfn;
}

void __fastcall D2MCPSetLadderCallback(D2MCPLADDERCALLBACK pfn)
{
	gtLadder.pfnLadder = pfn;
}

void __fastcall D2MCPSetNewsCallback(D2MCPNEWSCALLBACK pfn)
{
	gtLadder.pfnNews = pfn;
}
