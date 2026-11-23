#ifndef D2RE_D2SERVER_H
#define D2RE_D2SERVER_H

#include <windows.h>
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "Archive.h"
#include "Game.h"
#include "D2ServerImports.h"
#include "D2ServerLinkNames.h"

extern "C" BOOL __stdcall D2gameOrdinal10047(char *pszName, char *pszPassword, DWORD dwArenaArg,
	DWORD dwFlags,
	BYTE bArg5, BYTE bArg6, BYTE bArg7, WORD *pwGameId);
#define MAX_CHAR_SERV		3
#define NUM_PLAYERCLASS		5
#define NUM_FROMMCP			20
#define MAX_ACCEPT			256
#define MAX_ANNOUNCE		20
#define ANNOUNCE_LEN		256
#define TOKEN_LIFETIME		120000
#define GUILD_CHUNK			400

typedef BOOL (__fastcall *PFNFROMMCP)(const BYTE *pbMsg, int nLength);

typedef void *D2SERVERINTERFACE[1 + NUM_FROMMCP];

struct LADDERLIMITS
{
 	unsigned __int64	qwHardcoreLimit;
 	unsigned __int64	aqwHardcoreClassLimits[8];
 	unsigned __int64	qwStandardLimit;
 	unsigned __int64	aqwStandardClassLimits[7];
};

struct REALMACCEPT
{
 	struct in_addr	tNet;
 	int			nBits;
};

struct REALMINFO
{
 	char		szName[16];
 	char		szDesc[128];
 	char		szAdminPass[16];
 	char		szPublicPass[16];
 	char		szBnetIp[1024];
 	char		szMcpIp[24];
 	DWORD		dwMcpIp;
 	REALMACCEPT	atAccept[MAX_ACCEPT];
 	int			nAccept;
 	DWORD		adwCharServ[MAX_CHAR_SERV];
 	int			nCharServ;
 	char		aszAnnounce[MAX_ANNOUNCE][ANNOUNCE_LEN];
 	int			nAnnounce;
 	int			nMultiClient;
};

struct PLAYERTOKEN
{
 	DWORD		dwToken;
 	DWORD		dwGameId;
 	DWORD		dwTick;
 	char		szName[16];
 	char		szAccount[16];
 	DWORD		dwTokenResult;
 	PLAYERTOKEN	*ptNext;
};

struct D2SERVERGAMEINFO
{
 	DWORD		adwStats[25];
 	char		szName[16];
 	char		field_74[16];
 	BYTE		field_84;
 	BYTE		field_85;
 	BYTE		field_86;
 	BYTE		unobserved_87;
 	DWORD		dwArenaFlags;
 	void		*pMemPool;
 	BOOL		fSeen;
 	D2SERVERGAMEINFO	*ptNext;
};

struct D2SERVERUNITINFO
{
 	DWORD		eType;
 	DWORD		dwClassId;
 	DWORD		dwUnitId;
 	BYTE		unobserved_0c[36 - 12];
 	char		szDesc[16];
};

struct D2SERVERUNITCLASS
{
 	DWORD		dwClassId;
 	int			nCount;
 	int			field_08;
 	char		szDesc[128];
 	D2SERVERUNITCLASS	*ptNext;
};

struct D2SERVERCENSUS
{
 	int			nGames;
 	int			nMonsters;
 	int			field_08;
 	int			nPlayers;
};

struct D2SERVERSTATUS
{
 	WORD		wNumGames;
 	WORD		field_02;
 	BYTE		bFramesPerSecond;
};

#pragma pack(push, 1)
struct MCPCREATEGAME
{
 	BYTE		bOpcode;
 	DWORD		dwMcpId;
 	DWORD		dwFlags;
 	BYTE		field_09;
 	BYTE		field_0a;
 	BYTE		field_0b;
 	char		szName[1];
};

struct MCPJOINGAME
{
 	BYTE		bOpcode;
 	DWORD		dwMcpId;
 	DWORD		dwServId;
 	DWORD		dwToken;
 	DWORD		dwTokenResult;
 	char		szName[1];
};

struct MCPGUILD
{
 	BYTE		bOpcode;
 	DWORD		dwGameId;
 	DWORD		field_05;
 	WORD		wOffset;
 	WORD		wChunk;
 	WORD		wTotal;
 	BYTE		abData[1];
};

struct MCPLADDERLIMITS
{
 	BYTE		bOpcode;
 	LADDERLIMITS	tLimits;
};

struct MCPBROADCAST
{
 	BYTE		bOpcode;
 	DWORD		dwSendAsEvent;
 	DWORD		dwEventArg;
 	char		szText[1];
};
#pragma pack(pop)

BOOL __fastcall CharServOpenArchives(void);
HD2ARCHIVE __fastcall CharServGetArchive(void);
void __fastcall CharServCloseArchives(void);
BOOL __fastcall CharServConnectMCP(void);
BOOL __fastcall CharServCheckMCP(void);
void __fastcall CharServDisconnectMCP(void);
BOOL __fastcall CharServReturnFalse(void);
BOOL __fastcall CharServHackListing(DWORD dwAddress);
BOOL __fastcall CharServLoadBnclient(void);
BOOL __fastcall CharServBnclientDone(void);
BOOL __fastcall CharServUnloadBnclient(void);
BOOL __fastcall CharServRecvPiece(const BYTE *pbMsg, int nLength);
BOOL __fastcall CharServOnNothingFirst(const BYTE *pbMsg, int nLength);
BOOL __fastcall CharServOnNothingSecond(const BYTE *pbMsg, int nLength);
BOOL __fastcall CharServDispatch(DWORD dwUnused, const BYTE *pbMsg, DWORD nLength);
BOOL __fastcall CharServSend(const BYTE *pbRecord, int nLength);
BOOL __fastcall CharServSendChecksummedData(const char *pszText1, const char *pszText2, const BYTE *pbData,
	int nLength,
	DWORD dwArg);
BOOL __fastcall CharServSaveCharacter(const char *pszName, const char *pszAccount, const BYTE *pbData,
	int nLength, DWORD dwArg);
BOOL __fastcall CharServLoadCharacter(const char *pszName, DWORD dwClientId);
BOOL __fastcall CharServSendValueAndName(const char *pszName, DWORD dwArg);
BOOL __fastcall CharServSendWordValueAndName(const char *pszName, WORD wArg, DWORD dwArg);
BOOL __fastcall CharServUnlockCharacter(const char *pszName);
BOOL __fastcall CharServPing(void);
DWORD WINAPI CharServThread(LPVOID pvParam);
BOOL __fastcall CharServConnect(HWND hDlg);
BOOL __fastcall CharServWatchdog(void);
void __fastcall CharServDisconnect(void);
BOOL __fastcall CharServDrain(void);

HWND __fastcall ConsoleGetDialog(void);
BOOL __fastcall ConsoleCreateDialog(HWND hWndParent);
LRESULT __fastcall ConsoleCloseDialog(void);
BOOL CALLBACK ConsoleDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __fastcall ConsoleExecute(HWND hDlg, char *pszLine);
BOOL __fastcall ConsoleCmdHelp(HWND hDlg, char *pszArgs);
void __fastcall ConsoleAddLine(const char *pszText);
void __fastcall DisplaySetRefreshRate(int nPerSecond);
D2SERVERGAMEINFO * __fastcall DisplayGetSelectedGame(void);
void __fastcall DisplayProfileInit(void);
BOOL __fastcall DisplayBusyStart(void);
BOOL __fastcall DisplayBusyUpdate(void);
void __fastcall DisplayToLower(char *pszText);
void __fastcall DisplayRefresh(void);
void __fastcall DisplayFreeGames(void);

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
void ** __cdecl QueryInterface(void);
BOOL __fastcall FromMCPNothingFirst(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPIdentify(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPNoInfo(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPGameList(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPRecreate(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPNothingSecond(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPCreateGame(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPJoinGame(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPGuildSave(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPGuildRoster(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPLadderLimits(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPBroadcast(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPShutdown(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPDispatch(const BYTE *pbMsg, int nLength);
BOOL __fastcall FromMCPDispatchSecondTable(const BYTE *pbMsg, int nLength, DWORD dwUnused);

void __fastcall IniGetModuleDir(char *pszPath);
BOOL __stdcall IniIsAccepted(DWORD dwAddress);
int __fastcall IniFindKeyword(const char *pszKeyword);
BOOL __fastcall IniRead(void);
char * __fastcall IniGetName(void);
char * __fastcall IniGetDesc(void);
char * __fastcall IniGetAdminPass(void);
char * __fastcall IniGetPublicPass(void);
char * __fastcall IniGetBnetIp(void);
char * __fastcall IniGetMcpIp(void);
int __fastcall IniGetMultiClient(void);
int __fastcall IniGetNumCharServ(void);
DWORD __fastcall IniGetCharServ(int i);
int __fastcall IniGetNumAnnounce(void);
char * __fastcall IniGetAnnounce(int nIndex);
void __fastcall IniClearAnnounce(void);
BOOL __fastcall IniDeleteAnnounce(int nIndex);
BOOL __fastcall IniSetAnnounce(int nIndex, const char *pszText);
BOOL __fastcall IniAddAnnounce(const char *pszText);
BOOL __fastcall IniInsertAnnounce(int nIndex, const char *pszText);
HWND __fastcall PerfGetDialog(void);
BOOL __fastcall PerfCreateDialog(HWND hWndParent);
LRESULT __fastcall PerfCloseDialog(void);
BOOL CALLBACK PerfDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __fastcall PerfSetFirstLine(const char *pszText);
void __fastcall PerfSetSecondLine(const char *pszText);
void __fastcall PerfDraw(int nBusy, int nGames);
BOOL __fastcall PerfGetLogging(void);
void __fastcall PerfSetLogging(BOOL fLogging);
BOOL __fastcall PerfGetLogPath(char *pszPath);
void __fastcall PerfSetLogPath(const char *pszPath);
void __fastcall PerfLogValues(int nBusy, const DWORD *pdwValues);
void __fastcall PerfLogText(const char *pszText);

BOOL __fastcall PlayerTokenAdd(const char *pszName, const char *pszAccount, DWORD dwArg, DWORD dwToken,
	DWORD dwGameId);
BOOL __fastcall PlayerTokenFind(const char *pszName, DWORD dwToken, DWORD dwGameId, char *pszAccount,
	DWORD *pdwArg);
void __fastcall PlayerTokenRemove(const char *pszName);

BOOL __fastcall ToMCPSendIdentityRequest(void);
BOOL __fastcall ToMCPSendVersion(void);
BOOL __fastcall ToMCPSendNoInfo(void);
BOOL __fastcall ToMCPSendGameInfo(D2SERVERGAMEINFO *ptGame, BOOL fLast);
BOOL __fastcall ToMCPGameList(void);
BOOL __fastcall ToMCPCloseGame(DWORD dwGameId);
BOOL __stdcall ToMCPCreateGameReply(int nServId, int nMcpId, int nError);
BOOL __fastcall ToMCPJoinGameReply(DWORD dwMcpId, DWORD dwServId, DWORD dwToken, DWORD dwError);
BOOL __fastcall ToMCPPlayerLeft(DWORD dwGameId, DWORD dwClass, DWORD dwLevel, DWORD dwExperience,
	DWORD dwExperienceHigh, DWORD dwFlags, const char *pszName, const char *pszSummary);
BOOL __fastcall ToMCPSendPlayerEntered(DWORD dwArg, const char *pszText, WORD wClass, WORD wLevel,
	WORD wArg3);
BOOL __fastcall ToMCPSendDword(DWORD dwArg);
BOOL __fastcall ToMCPSendTick(void);
BOOL __fastcall ToMCPGuildSave(const char *pszTag, const BYTE *pbData, int nLength);
BOOL __fastcall ToMCPSendTwoTexts(const char *pszText1, BYTE bArg, const char *pszText2, DWORD dwArg);
BOOL __fastcall ToMCPCharInfo(const char *pszName, WORD wClass, DWORD dwLevel, unsigned __int64 qwExperience,
	DWORD dwFlags);
BOOL __fastcall ToMCPSendIdClassText(DWORD dwArg, const char *pszText, WORD wClass, BYTE bArg);
BOOL __fastcall ToMCPStatus(const D2SERVERSTATUS *ptStatus);

typedef char MCPADDRESS[264];
typedef char INIBLANK[8];
extern void *gpMcpConnection;
extern MCPADDRESS gszMcpAddress;
extern __int64 gqwBusyCount;
extern LADDERLIMITS gtLadderLimits;
extern INIBLANK gszIniBlank;
extern HWND ghStatusDialog;

void __fastcall ServerRunStartup(INIT_PARAMS *pOptions);
BOOL __cdecl ServerLog(BOOL fToFile, char *pszFormat, ...);
BOOL __fastcall ServerUnlockCharacter(const char *pszName);
BOOL __fastcall ServerLoadCharacter(const char *pszName, DWORD dwClientId);
BOOL __fastcall ServerCloseGame(DWORD dwGameId);
BOOL __fastcall ServerEnterGame(DWORD dwArg, const char *pszText, WORD wClass, WORD wLevel, WORD wArg3);
BOOL __fastcall ServerLeaveGame(DWORD dwGameId, DWORD dwClass, DWORD dwLevel, DWORD dwExperience,
	DWORD dwExperienceHigh, DWORD dwFlags, const char *pszName, const char *pszSummary);
BOOL __fastcall ServerSaveCharacter(const char *pszName, const char *pszAccount, const BYTE *pbData,
	int nLength, DWORD dwArg);
BOOL __fastcall ServerSaveGuild(const char *pszTag, const BYTE *pbData, int nLength);
BOOL __fastcall ServerSendTwoTexts(const char *pszText1, BYTE bArg, const char *pszText2, DWORD dwArg);
BOOL __fastcall ServerSendCharacterInfo(const char *pszName, WORD wClass, DWORD dwLevel,
	unsigned __int64 qwExperience,
	DWORD dwFlags);
BOOL __fastcall ServerSendIdClassText(DWORD dwArg, const char *pszText, WORD wClass, BYTE bArg);
HWND __fastcall ServerGetStatusDialog(void);
LRESULT CALLBACK ServerEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int __fastcall ServerGetMaxGames(void);
int __fastcall ServerGetMaxPlayers(void);
long __fastcall ServerGetUptime(void);
BOOL CALLBACK ServerDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ServerHousekeepThread(LPVOID pvParam);
void __fastcall ServerShutdownByMCP(void);
int __fastcall ServerWorkerStep(DWORD dwQueue, int *pnRound);
DWORD WINAPI ServerWorkerThread(LPVOID pvParam);
void __fastcall ServerNothingExport(void);
int __stdcall ServerWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, INIT_PARAMS *pOptions,
	int nCmdShow);

#endif
