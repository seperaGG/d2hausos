#ifndef D2RE_BNCLIENT_H
#define D2RE_BNCLIENT_H

#include <stddef.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <process.h>
#include "Safesock.h"
#include "BnclientImports.h"
#include "BnclientLinkNames.h"

extern "C" DWORD __cdecl StormOrdinal548(DWORD dwLog, const char *pszFormat, ...);

#define NUM_BNMESSAGES		62
#define MAX_SIMULTANEOUS_CONNECTS	32
#define BNET_PORT			6112

struct BNHEADER
{
 	BYTE		bSignature;
 	BYTE		bMessage;
 	WORD		wLength;
};

struct BNRESOLVE
{
 	char		*pszResult;
 	char		*pszHost;
};

struct BNSERVER
{
 	char		szHost[256];
 	SAFESOCK	*ptSock;
};

struct BNGATEWAYS
{
 	char		cNone;
 	BYTE		unobserved_01[3];
 	BOOL		fChosen;
 	int			nGateways;
 	int			nGateway;
 	char		*regionList;
 	int			regionListSize;
 	DWORD		regionListVersion;
 	BOOL		fOverride;

	void Load(void);
	void PickByBias(int nBias);
	void Save(void);
	char * GetGateway(int nGateway);
	char * GetHost(int nGateway);
	long GetZone(int nGateway);
	char * GetName(int nGateway);
	void SetGateway(int nGateway);
	void Read(const char *pszValue);
	void ReadList(void);
	void WriteDefault(void);
	void Update(char *pszIni);
	char * FindSection(char *pszText, const char *pszName);
};
#define BASE_REGIONLIST_VERSION		1000

#define BNDL_VERSION		0x80000001
#define BNDL_PATCH			0x80000002
#define BNDL_AD				0x80000003
#define BNDL_SERVERINI		0x80000004

struct BNDOWNLOAD
{
 	WORD		wLength;
 	BYTE		field_02;
 	BYTE		field_03;
 	DWORD		dwPlatform;
 	DWORD		dwProduct;
 	DWORD		dwFirst;
 	DWORD		dwSecond;
 	DWORD		dwOffset;
 	FILETIME	tTime;
 	char		szFile[260];
 	char		szText[260];
 	DWORD		dwKind;
 	DWORD		dwFlags;
};

struct BNDLREPLY
{
 	DWORD		field_00;
 	DWORD		dwFileBytes;
 	DWORD		dwFirst;
 	DWORD		dwSecond;
 	FILETIME	tTime;
 	char		szFile[1];
};

struct BNAD
{
 	DWORD		dwFirst;
 	DWORD		dwSecond;
 	char		*pszFile;
 	char		*pszText;
 	BYTE		*pbData;
 	DWORD		dwBytes;
};

typedef BOOL (__fastcall *PFNBNRECV)(BYTE *pbData, DWORD dwLength);

struct BNERRINFO
{
 	DWORD		dwReason;
 	const char	*pszInfo;
};

#define BN_PLATFORM			1230518326
#define BN_PRODUCT			1144144982

struct BNREPLY35
{
 	DWORD		dwLength;
 	BYTE		abData[20];
 	DWORD		dwRealmPort;
};

struct BNCLIENTINTERFACE
{
 	HANDLE		(__fastcall *pfnConnect)(void);
 	BOOL		(__fastcall *pfnLogin)(const char *pszFirst, const char *pszSecond);
 	BOOL		(__fastcall *pfnLoginResult)(void);
 	BOOL		(__fastcall *pfnDisconnect)(void);
 	BOOL		(__fastcall *pfnSend0E)(const char *pszLine);
 	char *		(__fastcall *pfnGetName)(void);
 	BOOL		(__fastcall *pfnSend0C)(const char *pszChannel);
 	BYTE *		(__fastcall *pfnRequest0B)(void);
 	DWORD		(__fastcall *pfnPop)(int *pnKind, BYTE *pbData);
 	BOOL		(__fastcall *pfnGetError)(BNERRINFO *errInfo);
 	BOOL		(__fastcall *pfnRequestFile)(const char *pszFile, DWORD dwKind);
 	BOOL		(__fastcall *pfnSend15)(void);
 	BOOL		(__fastcall *pfnSend1C)(const char *pszFirst, const char *pszSecond,
						const char *pszThird);
 	BYTE *		(__fastcall *pfnRequest09)(const char *pszFirst, const char *pszSecond);
 	BOOL		(__fastcall *pfnSend22)(const char *pszFirst, const char *pszSecond);
 	BYTE *		(__fastcall *pfnRequestRealmList)(const char *pszText);
 	BNREPLY35 *	(__fastcall *pfnRequest35)(const char *pszRealm, const char *pszSecond);
 	BOOL		(__fastcall *pfnApplyPatch)(int *pnStatus);
 	DWORD		(__fastcall *pfnGetProgress)(void);
 	unsigned long (__fastcall *pfnDownloadPatch)(const char *pszList, DWORD dwLength);
 	BOOL		(__fastcall *pfnEnterChat)(const char *pszFirst, const char *pszSecond,
						DWORD dwThird, const char *pszFourth);
 	BOOL		(__fastcall *pfnLeaveChannel)(void);
 	int			(__fastcall *pfnZero)(void);
 	BOOL		(__fastcall *pfnCreateAccount)(const char *pszFirst, const char *pszSecond);
 	BOOL		(__fastcall *pfnCreateAccountResult)(DWORD *pdwResult);
 	BYTE *		(__fastcall *pfnRequest37)(DWORD dwFirst);
 	void		(__fastcall *pfnSend38)(const char *pszFirst, const char *pszSecond);
 	void		(__fastcall *pfnSend39)(const char *pszText);
 	BOOL		(__fastcall *pfnIsConnected)(void);
 	BOOL		(__fastcall *pfnSend16)(DWORD dwFirst, BOOL fSecond);
 	void		(__fastcall *pfnCancelDownload)(void);
 	int			(__fastcall *pfnSend27)(const char *pszSubject, DWORD dwCount,
						const char **ppszKeys, const char **ppszValues);
 	BOOL		(__fastcall *pfnSend26)(const char *pszSubject, DWORD dwCount, DWORD dwId,
						const char **ppszKeys);
 	BOOL		(__fastcall *pfnReadUserData)(DWORD dwCount, DWORD dwId, char **ppszValues,
						DWORD *pdwSizes);
 	void		(__fastcall *pfnGetStage)(int *pState);
 	void		(__cdecl *pfnLogPrintf)(const char *pszFormat, ...);
 	BOOL		(__cdecl *pfnLogControl)(int nWhat, int nValue);
 	BOOL		(__fastcall *pfnGet1C)(DWORD *pdwResult);
 	BOOL		(__fastcall *pfnSend02)(void);
};

extern DWORD gdwLastAdId;
extern DWORD gdwNonce;
extern BNGATEWAYS gtGateways;

int __fastcall BnConnectSend(BYTE bMessage, const void *pData, DWORD dwLength);
void __cdecl BnConnectDebugPrint(const char *pszFormat, ...);
BOOL __fastcall BnConnectIsCancelled(void);
BOOL __fastcall BnConnectWaitMessage(BYTE bMessage);
BOOL __fastcall BnConnectIsHandled(BYTE bMessage);
void __fastcall BnConnectClearMessage(BYTE bMessage);
DWORD __fastcall BnConnectGetResult(BYTE bMessage);

BOOL __fastcall BnMessQueueAdd(int nKind, const BYTE *pbData, DWORD dwLength);
BOOL __fastcall BnMessQueueAddText(int nKind, const char *pszText);
DWORD __fastcall BnMessQueuePop(int *pnKind, BYTE *pbData);
BOOL __fastcall BnMessQueueDrain(void);
BOOL __fastcall BnMessQueueSetReason(DWORD dwReason);
BOOL __fastcall BnMessQueueForceReason(DWORD dwReason);
BOOL __fastcall BnMessQueueSetInfo(const char *pszInfo);
BOOL __fastcall BnMessQueueGetError(BNERRINFO *errInfo);
BOOL __fastcall BnMessQueuePeekReason(BNERRINFO *errInfo);
BOOL __fastcall BnMessQueueSendCDKey(void);
BOOL __fastcall BnMessQueueSendRegistration(void);
void __fastcall BnMessQueueSetNonce(DWORD dwNonce);
DWORD __fastcall BnMessQueueGetNonce(void);

BOOL __fastcall BnSendLogin(const char *pszAccount, const char *pszPassword);
BOOL __fastcall BnSendCreateAccount(const char *pszAccount, const char *pszPassword);
BOOL __fastcall BnSendVersion(void);
void __fastcall BnSendStoreRealmList(const BYTE *pbData, DWORD dwLength);
BYTE * __fastcall BnSendRequestRealmList(const char *pszText);
void __fastcall BnSendStoreChannelList(const BYTE *pbData, DWORD dwLength);
void __fastcall BnSendFreeChannelList(void);
BYTE * __fastcall BnSendRequestChannelList(void);
BOOL __fastcall BnSendJoinChannel(const char *pszChannel);
BOOL __fastcall BnSendEnterChat(const char *pszFirst, const char *pszSecond, DWORD dwUnused);
BOOL __fastcall BnSendLeaveChannel(void);
BOOL __fastcall BnSendChatLine(const char *pszLine);
BOOL __fastcall BnSendAdvertisementRequest(void);
BOOL __fastcall BnSendForAdvertisement(const char *pszFile, const char *pszText);
BOOL __fastcall BnSendCreateGame(const char *pszFirst, const char *pszSecond, const char *pszThird);
BOOL __fastcall BnSendBareMessage(void);
BOOL __fastcall BnSendGetCreateGameResult(DWORD *pdwResult);
void __fastcall BnSendStoreGameList(const BYTE *pbData, DWORD dwLength);
void __fastcall BnSendFreeGameList(void);
BYTE * __fastcall BnSendRequestGameList(const char *pszFirst, const char *pszSecond);
BOOL __fastcall BnSendProductAndTwoTexts(const char *pszFirst, const char *pszSecond);
void __fastcall BnSendStoreRealmLogon(const BYTE *pbData, DWORD dwLength);
BNREPLY35 * __fastcall BnSendRequestRealmLogon(const char *pszRealm, const char *pszSecond);
void __fastcall BnSendStoreCharacterList(const BYTE *pbData, DWORD dwLength);
BYTE * __fastcall BnSendRequestCharacterList(DWORD dwFirst);
void __fastcall BnSendCreateCharacter(const char *pszFirst, const char *pszSecond);
void __fastcall BnSendSelectCharacter(const char *pszText);
int __fastcall BnSendLocaleAndTime(void);
BOOL __fastcall BnSendDwordAndFlag(DWORD dwFirst, BOOL fSecond);
int __fastcall BnSendWriteUserData(const char *pszSubject, DWORD dwCount, const char **ppszKeys,
	const char **ppszValues);
void __fastcall BnSendStoreUserData(const BYTE *pbData, DWORD dwLength);
BOOL __fastcall BnSendReadUserData(const char *pszSubject, DWORD dwCount, DWORD dwId, const char **ppszKeys);
BOOL __fastcall BattleReadUserData(DWORD dwCount, DWORD dwId, char **ppszValues, DWORD *pdwSizes);

struct GRIDHASH;
void __fastcall GridMakePassword(char *pszPassword, int nBytes);
DWORD __fastcall GridDecrypt(BYTE *pbSrcDst, DWORD dwBytes, const char *pszPassword);
DWORD __fastcall GridCalcDstBytes(DWORD dwSrcBytes);
void __fastcall GridEncrypt(BYTE *pbSrcDst, DWORD dwSrcBytes, DWORD dwDstBytes, const char *pszPassword);
void __fastcall GridLoadCDKey(void);
void __fastcall GridCheckEpilogX(void);
void __fastcall GridFreeCDKey(void);
DWORD __fastcall GridGetCDKeyLoadState(void);
BOOL __fastcall GridGetEpilogX(void);
DWORD __fastcall GridHashCDKey(void);
const char * __fastcall GridGetOwner(void);
const char * __fastcall GridGetCDKey(void);
DWORD __fastcall IdeaMul(DWORD a, DWORD b);
DWORD __fastcall IdeaMulInv(DWORD x);
void __fastcall IdeaExpandKey(const WORD *pwUserKey, WORD *pwKey);
void __fastcall IdeaInvertKey(const WORD *pwIn, WORD *pwOut);
void __fastcall IdeaCipher(const WORD *pwIn, WORD *pwOut, const WORD *pwKey);
void __fastcall GridClearKeys(void);
void __fastcall GridCipher(int nKey, const BYTE *pbIn, BYTE *pbOut);
void __fastcall GridSetKey(int nKey, BOOL fEncrypt, const BYTE *pbKey);
void __fastcall GridHashInit(GRIDHASH *pt);
void __fastcall GridHashBlock(GRIDHASH *pt);
void __fastcall GridHash(BYTE *pbHash, const void *pData, DWORD dwLength);
void __fastcall GridShaClear(void);
void __fastcall GridShaGet(int nIndex, BYTE *pbDigest);
void __fastcall GridShaUpdate(int nIndex, const BYTE *pbData, BYTE *pbDigest);
void __fastcall GridShaReset(int nIndex);

BOOL __fastcall BnDownloadSetPatchList(const BYTE *pbData, DWORD dwLength);
BOOL __fastcall BnDownloadSetVersionText(const char *pszText);
BOOL __fastcall BnDownloadRequestFile(const char *pszFile, DWORD dwKind);
void __fastcall BnDownloadFile(DWORD dwKind, DWORD dwFlags, DWORD dwFirst, DWORD dwSecond,
	const FILETIME *ptTime, const char *pszFile, const char *pszText);
void __fastcall BnDownloadCancel(void);
BOOL __fastcall BnDownloadCheckRevision(DWORD *pdwFirst, DWORD *pdwSecond, char *pszText);
BOOL __fastcall BnDownloadApplyPatch(int *pnStatus);
void __fastcall BnDownloadSetProgress(DWORD dwUnused, DWORD dwDone, DWORD dwTotal);
void __fastcall BnDownloadClearPatchList(void);
DWORD __fastcall BnDownloadGetProgress(void);
unsigned long __fastcall BnDownloadPatch(const char *pszList, DWORD dwLength);
BOOL __fastcall BnDownloadSetVersionFile(const char *pszFile);
BOOL __fastcall BnDownloadHasVersionFile(void);

char * __stdcall BNetGWSkipToLineEnd(char *p, char *pEnd);
char * __stdcall BNetGWSkipLineBreaks(char *p, char *pEnd);
char * __stdcall BNetGWFindKey(char *pszText, const char *pszKey);

class BNLOCK
{
	CRITICAL_SECTION	m_tSection;
public:
	BNLOCK() { InitializeCriticalSection(&m_tSection); }
	~BNLOCK() { DeleteCriticalSection(&m_tSection); }
	void Enter(void) { EnterCriticalSection(&m_tSection); }
	void Leave(void) { LeaveCriticalSection(&m_tSection); }
};

void __fastcall CacheClose(void);
BOOL __fastcall CacheFree(BYTE *pbData, DWORD dwBytes);
BOOL __fastcall CacheOpen(void);
BOOL __fastcall CacheFind(const char *pszFile, FILETIME *ptTime, BYTE **ppbData, DWORD *pdwBytes);
BOOL __fastcall CacheStore(const char *pszFile, const BYTE *pbData, DWORD dwBytes, const FILETIME *ptTime,
	DWORD dwKeepSeconds, DWORD dwExpireSeconds);

void __cdecl BnclientLogPrintf(const char *pszFormat, ...);
BOOL __cdecl BnclientLogControl(int nWhat, int nValue);

BOOL __fastcall BnMessQueueInit(void);

BOOL __fastcall BnConnectGetPeerName(SOCKADDR *ptAddress, int *pnLength);
void __fastcall BnConnectGetGateway(char *pszHost, int nSize);
void __fastcall BnConnectResolve(char *pszResult, char *pszHost);

HANDLE __fastcall BnConnectConnect(void);
BOOL __fastcall BnConnectLogin(const char *pszFirst, const char *pszSecond);
BOOL __fastcall BnConnectLoginResult(void);
BOOL __fastcall BnConnectDisconnect(void);
char * __fastcall BnConnectGetName(void);
BOOL __fastcall BnConnectEnterChat(const char *pszFirst, const char *pszSecond, DWORD dwThird,
	const char *pszFourth);
BOOL __fastcall BnConnectLeaveChannel(void);
int __fastcall BnConnectZero(void);
BOOL __fastcall BnConnectCreateAccount(const char *pszFirst, const char *pszSecond);
BOOL __fastcall BnConnectCreateAccountResult(DWORD *pdwResult);
BOOL __fastcall BnConnectIsConnected(void);
void __fastcall BnConnectGetStage(int *pState);

#endif
