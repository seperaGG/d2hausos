#ifndef D2RE_D2MCPCLIENT_H
#define D2RE_D2MCPCLIENT_H

#include <windows.h>
#include <string.h>
#include "D2MCPClientImports.h"

#define D2MCP_NUM_MESSAGES	19
#define D2MCP_TEXT_MAX		1024
#define D2MCP_LADDER_MAX	8192
#define D2MCP_STATUS_NOGAME	40
#define D2MCP_STATUS_FAILED	2
#define D2MCP_CONNECT_WAIT	15000

struct D2QSSOCKET;

typedef BOOL (__fastcall *D2MCPRECV)(BYTE *pbMsg, DWORD dwLength);
typedef void (__fastcall *D2MCPWORDCALLBACK)(WORD wValue, BYTE *pbData);
typedef void (__fastcall *D2MCPLADDERCALLBACK)(DWORD dwCount, DWORD dwSize, BYTE *pbEntries);
typedef void (__fastcall *D2MCPNEWSCALLBACK)(DWORD dwType, DWORD dwLength, BYTE *pbData);

struct D2MCPCONNECT
{
 	DWORD		field_00;
 	DWORD		field_04;
 	DWORD		field_08;
 	DWORD		field_0c;
 	DWORD		dwAddress;
 	DWORD		dwPort;
 	DWORD		field_18;
 	DWORD		field_1c;
 	DWORD		field_20;
 	BYTE		field_24[20];
 	char		field_38[128];
};

struct D2MCPGAMECREATE
{
 	char		szGameName[16];
 	char		szPassword[16];
 	char		szDescription[32];
 	DWORD		dwFlags;
 	BYTE		field_44;
 	BYTE		bLevelDifference;
 	BYTE		bMaxPlayers;
};

struct D2MCPGAMEQUERY
{
 	char		field_00[20];
 	DWORD		dwFlags;
};

struct D2MCPGAMEINFO
{
 	BOOL		fTrailingData;
 	WORD		wToken;
 	WORD		unobserved_06;
 	DWORD		dwGameKey;
 	DWORD		dwElapsedSeconds;
 	BYTE		bLevel;
 	BYTE		bLevelDifference;
 	BYTE		bMaxPlayers;
 	BYTE		unobserved_13;
 	DWORD		dwNumPlayers;
 	DWORD		dwStatus;
 	char		szGameName[16];
 	char		szDescription[32];
 	WORD		awClasses[16];
 	WORD		awLevels[16];
 	char		aszPlayerNames[16][16];
};

struct D2MCPLADDER
{
 	DWORD		dwCount;
 	DWORD		dwSize;
 	BYTE		abEntries[D2MCP_LADDER_MAX - 8];
};

struct D2MCPLADDERDATA
{
 	D2MCPLADDER	tLadder;
 	D2MCPWORDCALLBACK	pfnCallback08;
 	D2MCPWORDCALLBACK	pfnCallback09;
 	D2MCPLADDERCALLBACK	pfnLadder;
 	D2MCPNEWSCALLBACK	pfnNews;
};

struct D2MCPLADDERBLOCK
{
 	DWORD		dwLadderCount;
 	DWORD		dwLadderSize;
 	DWORD		unobserved_08;
};
extern D2MCPLADDERDATA gtLadder;
extern D2MCPLADDERBLOCK gtLadderBlock;

struct D2MCPGLOBALS
{
 	BOOL		fReply0C;
 	DWORD		dwReply0E;
 	BOOL		fReply0B;
 	BYTE		unobserved_26a4[32];
 	char		szGameAddress[18];
 	WORD		wReplyToken;
 	BOOL		fReply02;
 	DWORD		dwGameId;
 	WORD		wToken;
 	WORD		unobserved_26e2;
 	DWORD		dwGameToken;
 	BOOL		fReply0D;
 	char		szReply0E1[36];
 	DWORD		dwStatus;
 	DWORD		unobserved_2714;
 	D2MCPCONNECT	tConnect;
 	BOOL		fReply10;
 	D2QSSOCKET	*ptSocket;
 	BOOL		fReply0F;
 	DWORD		unobserved_27dc;
 	D2MCPGAMEINFO	tGame;
 	char		szReply0E2[28];
 	BOOL		fReply0A;
 	BOOL		fReply07;
 	DWORD		unobserved_2990;
 	HANDLE		hReplyEvent;
};
extern D2MCPGLOBALS gtMCP;

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
HANDLE __stdcall D2MCPConnect(D2MCPCONNECT *ptConnect);
void __cdecl D2MCPClose(void);
BOOL __cdecl D2MCPIsConnected(void);
BOOL __cdecl D2MCPPoll(void);
void __fastcall D2MCPSetCallbackSecond(D2MCPWORDCALLBACK pfn);
void __fastcall D2MCPSetCallbackFirst(D2MCPWORDCALLBACK pfn);
void __fastcall D2MCPSetLadderCallback(D2MCPLADDERCALLBACK pfn);
char * __cdecl D2MCPGetGameAddress(void);
WORD __cdecl D2MCPGetReplyToken(void);
DWORD __cdecl D2MCPGetGameId(void);
DWORD __cdecl D2MCPGetGameToken(void);
D2MCPGAMEINFO * __cdecl D2MCPGetGameInfo(void);
void __cdecl D2MCPClearGameInfo(void);
BOOL __cdecl D2MCPGetCreateCharacterReply(void);
void __stdcall D2MCPSetCreateCharacterReply(BOOL f);
BOOL __cdecl D2MCPGetSelectCharacterReply(void);
void __stdcall D2MCPSetSelectCharacterReply(BOOL f);
BOOL __cdecl D2MCPGetDeleteCharacterReply(void);
void __stdcall D2MCPSetDeleteCharacterReply(BOOL f);
void __stdcall D2MCPSetStatusReplyFirst(BOOL f);
BOOL __cdecl D2MCPGetStatusReplyFirst(void);
BOOL __cdecl D2MCPGetStatusReplySecond(void);
void __stdcall D2MCPSetStatusReplySecond(BOOL f);
DWORD __stdcall D2MCPGetTextPair(char *pszText1, char *pszText2);
void __stdcall D2MCPSetTextPair(DWORD dwValue, const char *pszText1, const char *pszText2);
BOOL __cdecl D2MCPGetStatusReplyThird(void);
void __stdcall D2MCPSetStatusReplyThird(BOOL f);
BOOL __cdecl D2MCPGetStatusReplyFourth(void);
void __stdcall D2MCPSetStatusReplyFourth(BOOL f);
BOOL __cdecl D2MCPGetAddressReply(void);
void __stdcall D2MCPSetAddressReply(BOOL f);
DWORD __cdecl D2MCPGetStatus(void);
void __stdcall D2MCPSetStatus(DWORD dwStatus);
void __fastcall D2MCPSetNewsCallback(D2MCPNEWSCALLBACK pfn);

BOOL __stdcall ToMCPCreateCharacter(const char *pszName, DWORD dwClass, DWORD dwFlags);
BOOL __stdcall ToMCPSelectCharacter(const char *pszName);
BOOL __fastcall ToMCPSendHandshake(D2MCPCONNECT *ptConnect);
BOOL __fastcall ToMCPSendCreateCharacter(const char *pszName, DWORD dwClass, WORD wFlags);
BOOL __fastcall ToMCPSendSelectCharacter(const char *pszName);
BOOL __fastcall ToMCPSendCreateGame(D2MCPGAMECREATE *ptGame, WORD wToken);
BOOL __fastcall ToMCPSendJoinGame(const char *pszText1, const char *pszText2, WORD wToken);
BOOL __fastcall ToMCPSendGameListQuery(D2MCPGAMEQUERY *ptQuery, WORD wToken);
BOOL __fastcall ToMCPSendGameDetailsQuery(const char *pszText, WORD wToken);
BOOL __fastcall ToMCPSendTextAndWordFirst(const char *pszText, WORD wToken);
BOOL __fastcall ToMCPSendTextAndWordSecond(const char *pszText, WORD wToken);
BOOL __fastcall ToMCPSendDeleteCharacter(WORD wToken);
BOOL __fastcall ToMCPSendTwoTexts(const char *pszText1, const char *pszText2);
BOOL __fastcall ToMCPSendText(const char *pszText);
BOOL __cdecl ToMCPSendBareFirst(void);
BOOL __cdecl ToMCPSendBareSecond(void);
BOOL __cdecl ToMCPSendBareThird(void);
BOOL __fastcall ToMCPSendLadder(BYTE bValue);
BOOL __cdecl ToMCPSendNewsRequest(void);

#endif
