#include "D2MCPClient.h"

D2MCPGLOBALS gtMCP;

char * __cdecl D2MCPGetGameAddress(void)
{
	return gtMCP.szGameAddress;
}

WORD __cdecl D2MCPGetReplyToken(void)
{
	return gtMCP.wReplyToken;
}

DWORD __cdecl D2MCPGetGameId(void)
{
	return gtMCP.dwGameId;
}

void __stdcall D2MCPSetStatus(DWORD dwStatus)
{
	gtMCP.dwStatus = dwStatus;
}

DWORD __cdecl D2MCPGetStatus(void)
{
	return gtMCP.dwStatus;
}

BOOL __cdecl D2MCPGetCreateCharacterReply(void)
{
	return gtMCP.fReply02;
}

void __stdcall D2MCPSetCreateCharacterReply(BOOL f)
{
	gtMCP.fReply02 = f;
}

BOOL __cdecl D2MCPGetSelectCharacterReply(void)
{
	return gtMCP.fReply07;
}

void __stdcall D2MCPSetSelectCharacterReply(BOOL f)
{
	gtMCP.fReply07 = f;
}

BOOL __cdecl D2MCPGetDeleteCharacterReply(void)
{
	return gtMCP.fReply0A;
}

void __stdcall D2MCPSetDeleteCharacterReply(BOOL f)
{
	gtMCP.fReply0A = f;
}

void __stdcall D2MCPSetStatusReplyFirst(BOOL f)
{
	gtMCP.fReply0B = f;
}

BOOL __cdecl D2MCPGetStatusReplyFirst(void)
{
	return gtMCP.fReply0B;
}

BOOL __cdecl D2MCPGetStatusReplySecond(void)
{
	return gtMCP.fReply0C;
}

void __stdcall D2MCPSetStatusReplySecond(BOOL f)
{
	gtMCP.fReply0C = f;
}

DWORD __stdcall D2MCPGetTextPair(char *pszText1, char *pszText2)
{
	strcpy(pszText1, gtMCP.szReply0E1);
	strcpy(pszText2, gtMCP.szReply0E2);
	return gtMCP.dwReply0E;
}

void __stdcall D2MCPSetTextPair(DWORD dwValue, const char *pszText1, const char *pszText2)
{
	strcpy(gtMCP.szReply0E1, pszText1);
	strcpy(gtMCP.szReply0E2, pszText2);
	gtMCP.dwReply0E = dwValue;
}

D2MCPGAMEINFO * __cdecl D2MCPGetGameInfo(void)
{
	return &gtMCP.tGame;
}

void __cdecl D2MCPClearGameInfo(void)
{
	int i;

	for (i = 0; i < (int)(sizeof(D2MCPGAMEINFO) / sizeof(DWORD)); i++)
		((DWORD *)&gtMCP.tGame)[i] = 0;
}

BOOL __cdecl D2MCPGetStatusReplyThird(void)
{
	return gtMCP.fReply0D;
}

void __stdcall D2MCPSetStatusReplyThird(BOOL f)
{
	gtMCP.fReply0D = f;
}

BOOL __cdecl D2MCPGetStatusReplyFourth(void)
{
	return gtMCP.fReply0F;
}

void __stdcall D2MCPSetStatusReplyFourth(BOOL f)
{
	gtMCP.fReply0F = f;
}

BOOL __cdecl D2MCPGetAddressReply(void)
{
	return gtMCP.fReply10;
}

void __stdcall D2MCPSetAddressReply(BOOL f)
{
	gtMCP.fReply10 = f;
}

DWORD __cdecl D2MCPGetGameToken(void)
{
	return gtMCP.dwGameToken;
}

BOOL __stdcall ToMCPCreateCharacter(const char *pszName, DWORD dwClass, DWORD dwFlags)
{
	return ToMCPSendCreateCharacter(pszName, dwClass, (WORD)dwFlags);
}

BOOL __stdcall ToMCPSelectCharacter(const char *pszName)
{
	return ToMCPSendSelectCharacter(pszName);
}
