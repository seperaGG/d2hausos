#ifndef D2RE_GAME_H
#define D2RE_GAME_H

#include <windows.h>

#pragma pack(push, 1)
struct INIT_PARAMS
{
 	BYTE		bWindow;
 	BYTE		b3dfx;
 	BYTE		bOpenGL;
 	BYTE		bRave;
 	BYTE		bD3D;
 	BYTE		bPerspective;
 	BYTE		bQuality;
 	DWORD		dwGamma;
 	BYTE		bVSync;
 	DWORD		dwFrameRate;
 	DWORD		dwGameType;
 	WORD		wJoinId;
 	char		szGameName[24];
 	char		szServerIp[24];
 	char		szBattleNetIp[24];
 	char		szMcpIp[24];
 	BYTE		field_076[4];
 	BYTE		bNoPk;
 	BYTE		bOpenC;
 	BYTE		bAmazon;
 	BYTE		bPaladin;
 	BYTE		bSorceress;
 	BYTE		bNecromancer;
 	BYTE		bBarbarian;
 	BYTE		bInvincible;
 	BYTE		abAccountLogin[48];
 	char		szName[16];
 	BYTE		field_0c2[8];
 	BYTE		abSummary[256];
 	BYTE		bCTemp;
 	BYTE		bClass;
 	WORD		wCharFlags;
 	BYTE		bNoMonsters;
 	DWORD		dwMonsterClass;
 	BYTE		bMonsterInfo;
 	DWORD		dwMonsterDebug;
 	BYTE		bRare;
 	BYTE		bUnique;
 	BYTE		field_1da[2];
 	int			nAct;
 	BYTE		bNoPreload;
 	BYTE		bDirect;
 	BYTE		bLowEnd;
 	BYTE		bNoCompress;
 	WORD		wArena;
 	DWORD		dwGameFlags;
 	BYTE		field_1ea[3];
 	BYTE		bDifficulty;
 	BYTE		bLog;
 	BYTE		bMsgLog;
 	BYTE		bSafeMode;
 	BYTE		bNoSave;
 	DWORD		dwSeed;
 	BYTE		bCheats;
 	BYTE		bNoSound;
 	BYTE		bQuests;
 	BYTE		field_1f9;
 	DWORD		dwComInt;
 	BYTE		abGameToken[4];
 	char		szRealm[24];
 	BYTE		bGamePassword;
 	BYTE		field_21b[279];
 	BYTE		bSkipToBnet;
 	BYTE		bConnectMode;
 	BYTE		bShownLogo;
 	BYTE		field_335;
 	BYTE		abChannel[32];
 	BYTE		abFirstChannel[32];
};
#pragma pack(pop)

enum
{
	OPTION_FLAG = 0,
	OPTION_INT = 1,
	OPTION_STRING = 2,
};

struct OPTIONROW
{
 	char		szSection[16];
 	char		szKey[16];
 	char		szAlias[16];
 	BYTE		nType;
 	DWORD		dwOffset;
 	DWORD		dwDefault;
};

struct _STDINT
{
	int (__fastcall *pfnRun)(INIT_PARAMS *pOptions);
};

typedef _STDINT *(__cdecl *PFNQUERYINTERFACE)(void);
typedef void (__stdcall *PFNKEYHOOK)(DWORD dwArg);
typedef void (__stdcall *PFNKEYUNHOOK)(void);
typedef void (__cdecl *PFNCOMINT)(void);

void __fastcall GameOptionsParse(INIT_PARAMS *pOptions, const char *pszCmdLine);
void __fastcall GameOptionLowercase(char *pszText);
int __fastcall GameOptionFindAlias(const char *pszAlias);
void __fastcall GameOptionTrimValue(char *pszValue);
int __fastcall GameRunLoop(HINSTANCE hInstance, INIT_PARAMS *pOptions, int nMode);
void __fastcall GameSvcCmdLineSync(const char **ppszCmdLine);
int __fastcall GameMain(int nArgc, const char * const *ppszArgv);
void WINAPI GameServiceMain(DWORD dwArgc, LPSTR *ppszArgv);
void WINAPI GameServiceCtrlHandler(DWORD dwControl);

#endif
