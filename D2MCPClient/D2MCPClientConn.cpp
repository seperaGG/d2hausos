#include "D2MCPClient.h"

#define D2MCP_SOCKET_NAME	"clt->mcp"

BOOL __fastcall D2MCPDispatchPoll(BYTE *pbMsg, DWORD dwLength);
BOOL __fastcall D2MCPDispatchSocket(BYTE *pbMsg, DWORD dwLength, void *pContext);

static DWORD D2MCPConnectThread(void);

HANDLE __stdcall D2MCPConnect(D2MCPCONNECT *ptConnect)
{
	DWORD dwThreadId;

	gtMCP.tConnect = *ptConnect;
	return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)D2MCPConnectThread, NULL, 0, &dwThreadId);
}

static DWORD D2MCPConnectThread(void)
{
	D2QSSOCKET *ptSocket;
	const char *pszAddress;
	DWORD dwConnection;
	DWORD dwStep;
	DWORD dwError;
	BYTE bSelector;

	pszAddress = FogSafesockAddressToString(gtMCP.tConnect.dwAddress);
	gtMCP.dwStatus = 0;
	ptSocket = (D2QSSOCKET *)FogD2QSSocketCreate(pszAddress, gtMCP.tConnect.dwPort,
		D2MCP_SOCKET_NAME);
	gtMCP.ptSocket = ptSocket;
	FogD2QSSocketSetDispatcher(ptSocket, D2MCPDispatchSocket, 0);
	FogD2QSSocketStart(ptSocket);
	dwStep = 0;
	dwConnection = (DWORD)FogD2QSSocketGetConnection(ptSocket);
	if (dwConnection)
	{
		FogSafesockGetError(dwConnection, &dwStep, &dwError);
		if (!dwStep)
		{
			if (ptSocket != gtMCP.ptSocket)
			{
				FogD2QSSocketStop(ptSocket);
				return 0;
			}
			bSelector = 1;
			FogD2QSSocketSendRaw(ptSocket, &bSelector, 1);
			if (!gtMCP.hReplyEvent)
				gtMCP.hReplyEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
			ToMCPSendHandshake(&gtMCP.tConnect);
			if (WaitForSingleObject(gtMCP.hReplyEvent, D2MCP_CONNECT_WAIT))
				gtMCP.dwStatus = D2MCP_STATUS_FAILED;
			return 1;
		}
	}
	gtMCP.dwStatus = D2MCP_STATUS_FAILED;
	return 0;
}

void __cdecl D2MCPClose(void)
{
	if (gtMCP.ptSocket)
		FogD2QSSocketDestroy(gtMCP.ptSocket);
	gtMCP.ptSocket = NULL;
}

BOOL __cdecl D2MCPIsConnected(void)
{
	return (DWORD)FogD2QSSocketGetConnection(gtMCP.ptSocket) != 0;
}

BOOL __cdecl D2MCPPoll(void)
{
	BYTE abMsg[1024];
	WORD wLength;

	if (!gtMCP.ptSocket)
		return FALSE;
	wLength = (WORD)FogD2QSSocketReceive(gtMCP.ptSocket, abMsg, sizeof(abMsg));
	if (wLength)
		D2MCPDispatchPoll(abMsg, wLength);
	return TRUE;
}
