#include "Bnclient.h"
#include "StormImports.h"

DWORD gdwNonce = 0;

struct BNMSG1E
{
 	DWORD		dwOne;
 	DWORD		dwVersion;
 	DWORD		dwAuthority;
 	DWORD		dwClientId;
 	DWORD		dwClientToken;
 	char		szTexts[1024];
};

struct BNMSG29
{
 	DWORD		dwTick;
 	DWORD		dwNonce;
 	BYTE		abHash[20];
 	char		szAccount[128];
};
struct BNHASH29
{
 	DWORD		dwTick;
 	DWORD		dwNonce;
 	BYTE		abPasswordHash[20];
};

BOOL __fastcall BnMessQueueSendRegistration(void)
{
	BNMSG1E tMessage;

	StormSRegLoadValue("Configuration", "Registration Version", 2, &tMessage.dwVersion);
	StormSRegLoadValue("Configuration", "Registration Authority", 2, &tMessage.dwAuthority);
	StormSRegLoadValue("Configuration", "Client ID", 2, &tMessage.dwClientId);
	StormSRegLoadValue("Configuration", "Client Token", 2, &tMessage.dwClientToken);
	tMessage.dwOne = 1;
	tMessage.szTexts[0] = 0;
	tMessage.szTexts[1] = 0;
	BnConnectSend(30, &tMessage, offsetof(BNMSG1E, szTexts) + 2);
	return TRUE;
}

void __fastcall BnMessQueueSetNonce(DWORD dwNonce)
{
	gdwNonce = dwNonce;
}

DWORD __fastcall BnMessQueueGetNonce(void)
{
	return gdwNonce;
}

BOOL __fastcall BnMessQueueSendSecondLogin(const char *pszAccount, const char *pszPassword)
{
	BNHASH29 tHashed;
	BNMSG29 tMessage;
	char szPassword[128];
	DWORD dwLength;

	BnConnectClearMessage(41);
	StormSStrCopy(szPassword, pszPassword, sizeof(szPassword));
	dwLength = StormSStrLen(szPassword);
	CharLowerBuffA(szPassword, dwLength);
	tHashed.dwTick = GetTickCount();
	tHashed.dwNonce = gdwNonce;
	GridHash(tHashed.abPasswordHash, szPassword, dwLength);
	tMessage.dwTick = tHashed.dwTick;
	tMessage.dwNonce = tHashed.dwNonce;
	GridHash(tMessage.abHash, &tHashed, sizeof(tHashed));
	BnConnectSend(41, &tMessage,
		StormSStrCopy(tMessage.szAccount, pszAccount, sizeof(tMessage.szAccount))
		+ offsetof(BNMSG29, szAccount) + 1);
	return TRUE;
}
