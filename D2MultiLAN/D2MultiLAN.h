#ifndef D2RE_D2MULTILAN_H
#define D2RE_D2MULTILAN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DECLARE_HANDLE(HCHILDDATA);
DECLARE_HANDLE(HCELFILE);

struct __declspec(dllimport) Unicode
{
	WORD wChar;
	Unicode(WORD wCh = 0);
	static int __fastcall strlen(const Unicode *pwszStr);
	static char * __fastcall toUtf(char *pszDest, const Unicode *pwszSrc, int nCount);
};

struct _PARAMS;

struct FONTDESC
{
	DWORD dwFont;
	DWORD dwColor;
};

struct SCROLLBARPOS
{
	int nX;
	int nY;
	int nHeight;
	DWORD unobserved_0c;
};

struct D2WIN_STRUCT
{
	int eType;
	int nX;
	int nY;
	DWORD dwWidth;
	DWORD dwHeight;
	DWORD field_14;
	DWORD dwStringId;
	HCELFILE *phCelFile;
	DWORD dwCallback;
	DWORD field_24;
	DWORD field_28;
	DWORD dwFlags;
};

enum MODSTATE
{
	MODSTATE_MODSTATE0 = 0,
	MODSTATE_CLIENT = 1,
	MODSTATE_LAUNCHER = 4
};

#pragma pack(push, 1)
struct INIT_PARAMS
{
	BYTE unobserved_000[16];
	DWORD dwGameType;
	WORD wJoinId;
	char szGameName[24];
	char szServerIp[24];
	BYTE unobserved_046[468];
	BYTE bGamePassword;
	BYTE unobserved_21b[379];
};

struct D2NETSRVTCPJOINENTRY
{
	BYTE bCmd;
	char szName[16];
	char field_11[32];
	WORD wNumPlayers;
	WORD wGameId;
};
#pragma pack(pop)

struct D2MCPGAMELIST
{
	BYTE unobserved_00[20];
	int nNumPlayers;
	BYTE unobserved_18[20];
	char szDescription[32];
	WORD awClasses[16];
	WORD awLevels[16];
	char aszPlayerNames[16][16];
};

struct _STDINT
{
	int (__fastcall *pfnRun)(INIT_PARAMS *pOptions);
};

typedef int (__stdcall *PFNWINLOOP)(DWORD dwUnused);
typedef int (__stdcall *PFNEDITKEY)(void *pvEdit, int nPos, char *pchKey);
typedef int (__stdcall *PFNEDITPOST)(_PARAMS *ptParams);

#define MAX_GAMELIST		1000
#define GAMELIST_TICKS		5000
#define DETAIL_TICKS		1000
#define GAMETYPE_LANCREATE	8
#define GAMETYPE_LANJOIN	9

extern "C"
{
void * __stdcall SMemAlloc(unsigned int nBytes, const char *pszFile, int nLine, DWORD dwFlags);
BOOL __stdcall SMemFree(void *pMemory, const char *pszFile, int nLine, DWORD dwFlags);

int __stdcall D2NetClientInit(int nMode, const char *pszHost);
int __stdcall D2NetClientClose(void);
int __stdcall D2NetGetRemoteConnection(void);
int __stdcall D2NetSendToServer(BYTE bQueue, const BYTE *pbData, int nSize);
int __stdcall D2NetGetCltSysMsg(BYTE *pbBuffer, DWORD dwSize);

int __fastcall StrNewTable(DWORD hArchive);
void __fastcall StrDeleteTable(void);
const Unicode * __fastcall StrGetStringByIndex(WORD wStringId);

BOOL __stdcall D2WinSetBackground(HCELFILE hCelFile);
int __stdcall D2WinLoop(PFNWINLOOP pfnPass);
BOOL __stdcall D2WinQuitLoop(void);
BOOL __stdcall D2WinSetCursor(HCELFILE hCelFile);
BOOL __stdcall D2WinSetKeyFocus(HCHILDDATA hChild);
BOOL __stdcall D2WinResizeWindow(int nResolution);
BOOL __stdcall D2WinCreateParent(void);
BOOL __stdcall D2WinCloseParent(void);
HCHILDDATA __stdcall D2WinCreateChild(D2WIN_STRUCT *ptWindow);
void __stdcall D2WinDestroyChild(HCHILDDATA *phChild);
BOOL __fastcall D2WinChildSetEnabled(HCHILDDATA hChild, BOOL fEnabled);
DWORD __fastcall D2WinArchiveGetArchive(void);
HCELFILE __fastcall D2WinArchiveLoadCel(const char *pszName, DWORD dwType);
void __fastcall D2WinArchiveFreeCel(HCELFILE hCelFile);
BOOL __fastcall D2WinTextBoxAddLineUnicode(HCHILDDATA hChild, const Unicode *pwszText);
BOOL __fastcall D2WinTextBoxAddLineUTF(HCHILDDATA hChild, const char *pszText);
BOOL __fastcall D2WinClearTextBox(HCHILDDATA hChild);
const Unicode * __fastcall D2WinTextBoxGetSelectedLine(HCHILDDATA hChild);
int __fastcall D2WinTextBoxGetSelectedNum(HCHILDDATA hChild);
int __fastcall D2WinTextBoxSetSelectedNum(HCHILDDATA hChild, int nLine);
BOOL __fastcall D2WinTextBoxSetNumColumns(HCHILDDATA hChild, int nNumColumns);
BOOL __fastcall D2WinTextBoxAddColumnTextUTF(HCHILDDATA hChild, const char *pszText, int nLine, int nColumn);
BOOL __fastcall D2WinTextBoxSetColumnX(HCHILDDATA hChild, int nColumn, int nX);
int __fastcall D2WinTextBoxGetLastLineNum(HCHILDDATA hChild);
void __fastcall D2WinEditBoxSetKeyProc(HCHILDDATA hChild, PFNEDITKEY pfnKeyProc);
void __fastcall D2WinEditBoxSetKeyPostProc(HCHILDDATA hChild, PFNEDITPOST pfnKeyPostProc);
BOOL __fastcall D2WinEditBoxSetStringUnicode(HCHILDDATA hChild, const Unicode *pwszText);
BOOL __fastcall D2WinEditBoxSetStringUTF(HCHILDDATA hChild, const char *pszText);
const Unicode * __fastcall D2WinEditBoxGetString(HCHILDDATA hChild);
int __stdcall D2WinEditBoxNumericCallback(void *pvEdit, int nPos, char *pchKey);
void __fastcall D2WinEditBoxSetTabNext(HCHILDDATA hChild, HCHILDDATA hNext);
}

extern HCHILDDATA hButtonChild[1];
extern INIT_PARAMS *gptParams;
extern WORD gwEcho;
extern WORD gwListEcho;
extern WORD gwDetailEcho;
extern HCELFILE ghBackgroundCel;
extern HCELFILE ghCursor;
extern HCELFILE ghScrollCel;
extern HCELFILE ghQuitButton;
extern HCELFILE ghGameButtonCel;
extern HCELFILE ghCheckboxCel;
extern int gfAttachedToServer;
extern FONTDESC gtMenu8Font;
extern HCHILDDATA hTextBox;
extern HCHILDDATA hEditBox;
extern MODSTATE geModState;
extern struct HINSTANCE__ ghMultiLANInstance[5];
extern D2NETSRVTCPJOINENTRY sgtDisplayedGameList[MAX_GAMELIST];
extern D2NETSRVTCPJOINENTRY sgtGameList[MAX_GAMELIST];
extern HCHILDDATA hGateCreateText;
extern HCHILDDATA hGateCreateInput;
extern int sgnDisplayedGameListEntries;
extern int sgnGameListEntries;
extern FONTDESC gtChatFont;
extern FONTDESC gtMenu42Font;
extern FONTDESC gtMenu30Font;
extern FONTDESC gtExocet8Font;
extern SCROLLBARPOS tPos;
extern SCROLLBARPOS gtJoinScroll;
extern D2WIN_STRUCT gtD2Windows[24];
extern _STDINT gtD2MultiLANInt;

void __fastcall DestroyMenu(void);
void __fastcall LoadCelFiles(void);
void __fastcall FreeCelFiles(void);
void __fastcall ProcessSysMsgFromServer(BYTE *pbMsg, int nSize);
int __stdcall sAttachIdle(DWORD dwUnused);
void __fastcall SendForceDisconnect(void);
int __stdcall sDetachIdle(DWORD dwUnused);
int __stdcall sIdle(DWORD dwUnused);
int __fastcall StartLANChat(INIT_PARAMS *pParams);
int __fastcall StartChat(INIT_PARAMS *pParams);
void __fastcall JoinGameListRebuildEnd(void);
void __fastcall AddToGameList(D2NETSRVTCPJOINENTRY *ptEntry);
void __fastcall CreateJoinGameWindows(void);
void __fastcall JoinGameListRebuildStart(void);
void __fastcall QueueDetailMessage(char *pszName, WORD wEcho);
void __fastcall SendDetailMessage(void);
int __stdcall JoinGameListCallback(_PARAMS *ptParams);
void __fastcall UpdateJoinGameDetails(D2MCPGAMELIST *ptGame);
void __fastcall DestroyJoinGameWindows(void);
void __fastcall CreateCreateGameWindows(void);
void __fastcall DestroyCreateGameWindows(void);
int __stdcall GateCreateCallback(_PARAMS *ptParams);
int __stdcall GateJoinSelect(_PARAMS *ptParams);

#endif
