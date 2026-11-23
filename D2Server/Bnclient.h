#ifndef D2SERVER_BNCLIENT_H
#define D2SERVER_BNCLIENT_H
#include <windows.h>
struct BNERRINFO;
struct BNREPLY35;

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
#endif
