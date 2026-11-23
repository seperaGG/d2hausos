#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\CORE\\FromMCP.cpp"
#include "D2Server.h"
#include "D2Macros.h"

typedef PFNFROMMCP FROMMCPTABLE[NUM_FROMMCP];

static HINSTANCE sghInstance = NULL;
static FROMMCPTABLE sgapfnFromMCP2 = { 0 };

static int __fastcall ServerRun(INIT_PARAMS *pOptions);

static D2SERVERINTERFACE sgapvInterface =
{
	ServerRun,
	FromMCPNothingFirst,
	FromMCPIdentify,
	FromMCPNoInfo,
	FromMCPGameList,
	FromMCPRecreate,
	FromMCPNothingSecond,
	FromMCPCreateGame,
	FromMCPJoinGame,
	NULL, NULL, NULL, NULL,
	FromMCPGuildSave,
	FromMCPGuildRoster,
	NULL,
	FromMCPLadderLimits,
	NULL, NULL,
	FromMCPBroadcast,
	FromMCPShutdown,
};

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	sghInstance = hInstance;
	return TRUE;
}

void ** __cdecl QueryInterface(void)
{
	return sgapvInterface;
}

static int __fastcall ServerRun(INIT_PARAMS *pOptions)
{
	return ServerWinMain(sghInstance, NULL, pOptions, SW_SHOW);
}

BOOL __fastcall FromMCPNothingFirst(const BYTE *pbMsg, int nLength)
{
	return TRUE;
}

BOOL __fastcall FromMCPIdentify(const BYTE *pbMsg, int nLength)
{
	ToMCPSendVersion();
	return TRUE;
}

BOOL __fastcall FromMCPNoInfo(const BYTE *pbMsg, int nLength)
{
	ToMCPSendNoInfo();
	return TRUE;
}

BOOL __fastcall FromMCPGameList(const BYTE *pbMsg, int nLength)
{
	ToMCPGameList();
	return TRUE;
}

BOOL __fastcall FromMCPRecreate(const BYTE *pbMsg, int nLength)
{
#line 107
	D2FATAL("GTM_RECREATE shouldn't be sent from MCP to Server!\n");
}

BOOL __fastcall FromMCPNothingSecond(const BYTE *pbMsg, int nLength)
{
	return TRUE;
}

BOOL __fastcall FromMCPCreateGame(const BYTE *pbMsg, int nLength)
{
	MCPCREATEGAME *ptMsg = (MCPCREATEGAME *)pbMsg;
	WORD wGameId;
	char *pszName = ptMsg->szName;
	char *pszName2 = pszName + strlen(pszName) + 1;

	if (D2gameGameCreateRaw(pszName, pszName2, 0, ptMsg->dwFlags,
		ptMsg->field_09, ptMsg->field_0a, ptMsg->field_0b, &wGameId))
	{
		ToMCPCreateGameReply(wGameId, ptMsg->dwMcpId, 0);
		return TRUE;
	}
	ToMCPCreateGameReply(0, ptMsg->dwMcpId, 1);
	return TRUE;
}

BOOL __fastcall FromMCPJoinGame(const BYTE *pbMsg, int nLength)
{
	MCPJOINGAME *ptMsg = (MCPJOINGAME *)pbMsg;
	char *pszName = ptMsg->szName;

	PlayerTokenAdd(pszName, pszName + strlen(pszName) + 1, ptMsg->dwTokenResult,
		ptMsg->dwToken,
		ptMsg->dwServId);
	ToMCPJoinGameReply(ptMsg->dwMcpId, ptMsg->dwServId, ptMsg->dwToken, 0);
	return TRUE;
}

BOOL __fastcall FromMCPGuildSave(const BYTE *pbMsg, int nLength)
{
	MCPGUILD *ptMsg = (MCPGUILD *)pbMsg;

	if ((unsigned)nLength < 15)
		return FALSE;
	if (ptMsg->bOpcode != 12)
		return FALSE;
	D2gameGuildsReceiveSave(ptMsg->dwGameId, &ptMsg->field_05, ptMsg->wChunk, ptMsg->wOffset, ptMsg->wTotal,
		ptMsg->abData);
	return TRUE;
}

BOOL __fastcall FromMCPGuildRoster(const BYTE *pbMsg, int nLength)
{
	MCPGUILD *ptMsg = (MCPGUILD *)pbMsg;

	if ((unsigned)nLength < 15)
		return FALSE;
	if (ptMsg->bOpcode != 13)
		return FALSE;
	D2gameGuildsReceiveRoster(ptMsg->dwGameId, &ptMsg->field_05, ptMsg->wChunk, ptMsg->wOffset, ptMsg->wTotal,
		ptMsg->abData);
	return TRUE;
}

BOOL __fastcall FromMCPLadderLimits(const BYTE *pbMsg, int nLength)
{
	int nDst;

	if (nLength != 137)
		return FALSE;
	if (pbMsg[0] != 15)
		return FALSE;
	for (nDst = (int)&gtLadderLimits, pbMsg++; nDst < (int)(&gtLadderLimits + 1);
		nDst += sizeof(unsigned __int64))
	{
		*(unsigned __int64 *)nDst = *(const unsigned __int64 *)pbMsg;
		pbMsg += sizeof(unsigned __int64);
	}
	return TRUE;
}

BOOL __fastcall FromMCPBroadcast(const BYTE *pbMsg, int nLength)
{
	MCPBROADCAST *ptMsg = (MCPBROADCAST *)pbMsg;
	char szText[256];

	if ((unsigned)nLength <= 9)
		return FALSE;
	if (ptMsg->bOpcode != 18)
		return FALSE;
	StormSStrPrintf(szText, 256, "Diablo: %s", ptMsg->szText);
	D2gameGameBroadcast(ptMsg->dwSendAsEvent, ptMsg->dwEventArg, szText);
	return TRUE;
}

BOOL __fastcall FromMCPShutdown(const BYTE *pbMsg, int nLength)
{
	if (nLength != 9)
		return FALSE;
	if (pbMsg[0] != 19)
		return FALSE;
	ServerShutdownByMCP();
	return TRUE;
}

BOOL __fastcall FromMCPDispatch(const BYTE *pbMsg, int nLength)
{
	BOOL fResult;
	BYTE bOpcode;
	PFNFROMMCP pfnHandler;

	fResult = FALSE;
	if ((unsigned)nLength <= 0)
		return FALSE;
	bOpcode = pbMsg[0];
	if (bOpcode >= NUM_FROMMCP)
		return FALSE;
	pfnHandler = (PFNFROMMCP)sgapvInterface[1 + bOpcode];
	if (pfnHandler)
		fResult = pfnHandler(pbMsg, nLength);
	return fResult;
}

BOOL __fastcall FromMCPDispatchSecondTable(const BYTE *pbMsg, int nLength, DWORD dwUnused)
{
	BOOL fResult;
	BYTE bOpcode;
	PFNFROMMCP pfnHandler;

	fResult = FALSE;
	if ((unsigned)nLength <= 0)
		return FALSE;
	bOpcode = pbMsg[0];
	if (bOpcode >= NUM_FROMMCP)
		return FALSE;
	pfnHandler = sgapfnFromMCP2[bOpcode];
	if (pfnHandler)
		fResult = pfnHandler(pbMsg, nLength);
	return fResult;
}
