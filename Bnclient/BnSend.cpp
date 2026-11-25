#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\BnSend.cpp"
#include "Bnclient.h"
#include "D2Macros.h"
#include "FogImports.h"
#include "StormImports.h"

static DWORD sgdwRealmLogons = 0;
static BYTE *sgpb0B = 0;
static BYTE *sgpbRealmList = 0;
static BYTE *sgpb09 = 0;
static BNREPLY35 *sgptReply35 = 0;
static BYTE *sgpb26 = 0;
static BYTE *sgpb37 = 0;

struct BNMSG3A
{
 	DWORD		dwTick;
 	DWORD		dwNonce;
 	BYTE		abHash[20];
 	char		szAccount[128];
};
struct BNHASH3A
{
 	DWORD		dwTick;
 	DWORD		dwNonce;
 	BYTE		abPasswordHash[20];
};

struct BNMSG3D
{
 	BYTE		abHash[20];
 	char		szAccount[128];
};

struct BNMSG06
{
 	DWORD		dwPlatform;
 	DWORD		dwProduct;
 	DWORD		field_08;
 	DWORD		field_0c;
};

struct BNMSG07
{
 	DWORD		dwPlatform;
 	DWORD		dwProduct;
 	DWORD		field_08;
 	DWORD		dwFirst;
 	DWORD		dwSecond;
 	char		szText[256];
};

struct BNMSG34
{
 	DWORD		field_00;
 	DWORD		field_04;
 	char		szText[128];
};

struct BNMSG0C
{
 	DWORD		dwFlags;
 	char		szChannel[128];
};

struct BNMSG15
{
 	DWORD		dwPlatform;
 	DWORD		dwProduct;
 	DWORD		dwLast;
 	DWORD		dwTime;
};

struct BNMSG21
{
 	DWORD		dwPlatform;
 	DWORD		dwProduct;
 	DWORD		dwLast;
 	char		szTexts[1280];
};

struct BNMSG1C
{
 	DWORD		dwSecondGiven;
 	DWORD		field_04;
 	DWORD		field_08;
 	DWORD		field_0c;
 	DWORD		field_10;
 	char		szTexts[128];
};

struct BNMSG09
{
 	DWORD		field_00;
 	DWORD		field_04;
 	DWORD		field_08;
 	DWORD		field_0c;
 	char		szTexts[128];
};

struct BNMSG22
{
 	DWORD		dwProduct;
 	DWORD		field_04;
 	char		szTexts[256];
};

struct BNMSG35
{
 	DWORD		dwCount;
 	BYTE		abHash[20];
 	char		szRealm[128];
};
struct BNHASH35
{
 	DWORD		dwCount;
 	DWORD		dwNonce;
 	BYTE		abSecondHash[20];
};

struct BNMSG37
{
 	DWORD		dwFirst;
 	char		szText[128];
};

struct BNMSG38
{
 	DWORD		unset_00;
 	DWORD		unset_04;
 	DWORD		field_08;
 	char		szTexts[128];
};

struct BNMSG12
{
 	FILETIME	tSystemTime;
 	FILETIME	tLocalTime;
 	LONG		lBias;
 	LCID		dwSystemLcid;
 	LCID		dwUserLcid;
 	DWORD		dwUserLangId;
 	char		szTexts[128];
};

struct BNMSG16
{
 	DWORD		dwFirst;
 	DWORD		fSecond;
};

struct BNMSG27
{
 	DWORD		dwOne;
 	DWORD		dwCount;
 	char		szTexts[1];
};
struct BNMSG26
{
 	DWORD		dwOne;
 	DWORD		dwCount;
 	DWORD		dwId;
 	char		szTexts[1];
};

int __fastcall BnSendVersionResult(DWORD dwFirst, DWORD dwSecond, const char *pszText);
void __fastcall BnSendFreeRealmList(void);
void __fastcall BnSendFreeRealmLogon(void);
void __fastcall BnSendFreeCharacterList(void);

BOOL __fastcall BnSendLogin(const char *pszAccount, const char *pszPassword)
{
	BNHASH3A tHashed;
	BNMSG3A tMessage;
	char szPassword[128];
	DWORD dwLength;

	BnConnectClearMessage(58);
	StormSStrCopy(szPassword, pszPassword, sizeof(szPassword));
	dwLength = StormSStrLen(szPassword);
	CharLowerBuffA(szPassword, dwLength);
	tHashed.dwTick = GetTickCount();
	tHashed.dwNonce = gdwNonce;
	GridHash(tHashed.abPasswordHash, szPassword, dwLength);
	tMessage.dwTick = tHashed.dwTick;
	tMessage.dwNonce = tHashed.dwNonce;
	GridHash(tMessage.abHash, &tHashed, sizeof(tHashed));
	BnConnectSend(58, &tMessage,
		StormSStrCopy(tMessage.szAccount, pszAccount, sizeof(tMessage.szAccount))
		+ offsetof(BNMSG3A, szAccount) + 1);
	return TRUE;
}

BOOL __fastcall BnSendCreateAccount(const char *pszAccount, const char *pszPassword)
{
	char szPassword[128];
	BNMSG3D tMessage;
	DWORD dwLength;

	StormSStrCopy(szPassword, pszPassword, sizeof(szPassword));
	dwLength = StormSStrLen(szPassword);
	CharLowerBuffA(szPassword, dwLength);
	GridHash(tMessage.abHash, szPassword, dwLength);
	BnConnectSend(61, &tMessage,
		StormSStrCopy(tMessage.szAccount, pszAccount, sizeof(tMessage.szAccount))
		+ offsetof(BNMSG3D, szAccount) + 1);
	return TRUE;
}

int __fastcall BnSendVersionResult(DWORD dwFirst, DWORD dwSecond, const char *pszText)
{
	BNMSG07 tMessage;
	int i;

	for (i = 0; i < (int)(sizeof(tMessage) / sizeof(DWORD)); i++)
		((DWORD *)&tMessage)[i] = 0;
	tMessage.field_08 = 0;
	tMessage.dwPlatform = BN_PLATFORM;
	tMessage.dwProduct = BN_PRODUCT;
	tMessage.dwFirst = dwFirst;
	tMessage.dwSecond = dwSecond;
	StormSStrCopy(tMessage.szText, pszText, sizeof(tMessage.szText));
	FogErrorManagerLog("%x %x, %d %d, %d %s\n", BN_PLATFORM, BN_PRODUCT, 0, dwFirst, dwSecond, pszText);
	return BnConnectSend(7, &tMessage, strlen(tMessage.szText) + 1 + offsetof(BNMSG07, szText)) != 0;
}

BOOL __fastcall BnSendVersion(void)
{
	BNMSG06 tMessage;
	DWORD dwFirst;
	DWORD dwSecond;

	BnConnectClearMessage(7);
	memset(&tMessage, 0, sizeof(tMessage));
	tMessage.dwPlatform = BN_PLATFORM;
	tMessage.dwProduct = BN_PRODUCT;
	tMessage.field_08 = 0;
	BnConnectClearMessage(6);
	BnConnectClearMessage(7);
	BnDownloadSetVersionFile("");
	if (BnConnectSend(6, &tMessage, sizeof(tMessage)))
	{
		while (!BnDownloadHasVersionFile())
		{
			if (BnConnectIsHandled(7))
				break;
			if (BnConnectIsCancelled())
				return FALSE;
			Sleep(10);
		}
		BnDownloadClearPatchList();
		if (!BnConnectIsHandled(7))
		{
			char szText[256] = "";
			if (!BnDownloadCheckRevision(&dwFirst, &dwSecond, szText))
			{
				BnMessQueueSetReason(1);
				return FALSE;
			}
			BnDownloadSetPatchList((const BYTE *)"\0", 2);
			BnSendVersionResult(dwFirst, dwSecond, szText);
			BnConnectWaitMessage(7);
		}
	}
	return FALSE;
}

void __fastcall BnSendStoreRealmList(const BYTE *pbData, DWORD dwLength)
{
	BnSendFreeRealmList();
#line 296
	sgpbRealmList = (BYTE *)D2ALLOC2(dwLength);
	FogErrorManagerLog("Storing Realm List.\n");
	memcpy(sgpbRealmList, pbData, dwLength);
}

void __fastcall BnSendFreeRealmList(void)
{
	if (sgpbRealmList)
	{
		FogErrorManagerLog("Freeing Realm List.\n");
#line 331
		D2FREE2(sgpbRealmList);
	}
	sgpbRealmList = NULL;
}

BYTE * __fastcall BnSendRequestRealmList(const char *pszText)
{
	BNMSG34 tMessage;
	DWORD dwLength;
	int i;

	for (i = 0; i < (int)(sizeof(tMessage) / sizeof(DWORD)); i++)
		((DWORD *)&tMessage)[i] = 0;
	if (pszText)
		StormSStrCopy(tMessage.szText, pszText, sizeof(tMessage.szText));
	else
		tMessage.szText[0] = 0;
	dwLength = strlen(tMessage.szText) + offsetof(BNMSG34, szText) + 1;
	BnConnectClearMessage(52);
	if (!BnConnectSend(52, &tMessage, dwLength))
	{
		FogErrorManagerLog("Failed to send Realm List\n");
		return NULL;
	}
	BnConnectWaitMessage(52);
	return sgpbRealmList;
}

void __fastcall BnSendStoreChannelList(const BYTE *pbData, DWORD dwLength)
{
	BnSendFreeChannelList();
#line 368
	sgpb0B = (BYTE *)D2ALLOC2(dwLength);
	memcpy(sgpb0B, pbData, dwLength);
}

void __fastcall BnSendFreeChannelList(void)
{
	if (sgpb0B)
	{
#line 377
		D2FREE2(sgpb0B);
	}
	sgpb0B = NULL;
}

BYTE * __fastcall BnSendRequestChannelList(void)
{
	DWORD dwProduct;

	dwProduct = BN_PRODUCT;
	BnConnectClearMessage(11);
	BnConnectSend(11, &dwProduct, sizeof(dwProduct));
	BnConnectWaitMessage(11);
	return sgpb0B;
}

BOOL __fastcall BnSendJoinChannel(const char *pszChannel)
{
	BNMSG0C tMessage;
	int i;

	for (i = 0; i < (int)(sizeof(tMessage) / sizeof(DWORD)); i++)
		((DWORD *)&tMessage)[i] = 0;
	if (pszChannel && *pszChannel)
	{
		tMessage.dwFlags = 2;
		StormSStrCopy(tMessage.szChannel, pszChannel, sizeof(tMessage.szChannel));
	}
	else
	{
		tMessage.dwFlags = 1;
		StormSStrCopy(tMessage.szChannel, "Diablo II", sizeof(tMessage.szChannel));
	}
	BnConnectSend(12, &tMessage, strlen(tMessage.szChannel) + 1 + offsetof(BNMSG0C, szChannel));
	return TRUE;
}

BOOL __fastcall BnSendEscape(BYTE *pbData, DWORD dwLength)
{
	BYTE *pb;

	for (pb = pbData; (DWORD)(pb - pbData) < dwLength; pb++)
	{
		if (pb - pbData == 127)
			return FALSE;
		if (*pb == 0)
		{
			StormSMemCopy(pb + 2, pb + 1, 128);
			dwLength++;
			*pb = 255;
			pb[1] = 1;
			pb++;
		}
		else if (*pb == 255)
		{
			StormSMemCopy(pb + 2, pb + 1, 128);
			dwLength++;
			*pb = 255;
			pb[1] = 255;
			pb++;
		}
	}
	return TRUE;
}

BOOL __fastcall BnSendEnterChat(const char *pszFirst, const char *pszSecond, DWORD dwUnused)
{
	DWORD dwProduct;

	dwProduct = BN_PRODUCT;
	BnConnectSend(11, &dwProduct, sizeof(dwProduct));
	char szTexts[256] = "";
	StormSStrCopy(szTexts, pszFirst, 128);
	StormSStrCopy(szTexts + strlen(szTexts) + 1, pszSecond, 128);
	return BnConnectSend(10, szTexts,
		strlen(szTexts) + strlen(szTexts + strlen(szTexts) + 1) + 2) != 0;
}

BOOL __fastcall BnSendLeaveChannel(void)
{
	return BnConnectSend(16, NULL, 0) != 0;
}

BOOL __fastcall BnSendChatLine(const char *pszLine)
{
	if (!pszLine || !strlen(pszLine))
		return FALSE;
	return BnConnectSend(14, pszLine, strlen(pszLine) + 1) != 0;
}

BOOL __fastcall BnSendAdvertisementRequest(void)
{
	BNMSG15 tMessage;

	tMessage.dwPlatform = BN_PLATFORM;
	tMessage.dwProduct = BN_PRODUCT;
	tMessage.dwLast = gdwLastAdId;
	tMessage.dwTime = time(NULL);
	BnConnectSend(21, &tMessage, sizeof(tMessage));
	return TRUE;
}

BOOL __fastcall BnSendForAdvertisement(const char *pszFile, const char *pszText)
{
	BNMSG21 tMessage;

	tMessage.dwLast = gdwLastAdId;
	tMessage.dwPlatform = BN_PLATFORM;
	tMessage.dwProduct = BN_PRODUCT;
	tMessage.szTexts[0] = 0;
	tMessage.szTexts[1] = 0;
	BnConnectSend(33, &tMessage, offsetof(BNMSG21, szTexts) + 2);
	return TRUE;
}

BOOL __fastcall BnSendCreateGame(const char *pszFirst, const char *pszSecond, const char *pszThird)
{
	BNMSG1C tMessage;
	char *pszText; int nLength;

	tMessage.dwSecondGiven = 0;
	tMessage.field_04 = 0;
	tMessage.field_08 = 0;
	tMessage.field_0c = 0;
	tMessage.field_10 = 0;
	if (!pszFirst)
		pszFirst = "";
	if (!pszSecond)
		pszSecond = "";
	if (!pszThird)
		pszThird = "";
	if (*pszSecond)
		tMessage.dwSecondGiven = 1;
	pszText = tMessage.szTexts + StormSStrCopy(tMessage.szTexts, pszFirst, 128) + 1;
	pszText += StormSStrCopy(pszText, pszSecond, 128) + 1;
	nLength = StormSStrCopy(pszText, pszThird, 128) + (pszText - tMessage.szTexts) + offsetof(BNMSG1C, szTexts) + 1;
	BnConnectSend(28, &tMessage, nLength);
	return TRUE;
}

BOOL __fastcall BnSendBareMessage(void)
{
	BnConnectSend(2, NULL, 0);
	return TRUE;
}

BOOL __fastcall BnSendGetCreateGameResult(DWORD *pdwResult)
{
	*pdwResult = BnConnectGetResult(28);
	return BnConnectIsHandled(28);
}

void __fastcall BnSendStoreGameList(const BYTE *pbData, DWORD dwLength)
{
	BnSendFreeGameList();
#line 646
	sgpb09 = (BYTE *)D2ALLOC2(dwLength);
	memcpy(sgpb09, pbData, dwLength);
}

void __fastcall BnSendFreeGameList(void)
{
	if (sgpb09)
	{
#line 714
		D2FREE2(sgpb09);
	}
	sgpb09 = NULL;
}

BYTE * __fastcall BnSendRequestGameList(const char *pszFirst, const char *pszSecond)
{
	BNMSG09 tMessage;
	char *pszText;
	DWORD dwLength;

	tMessage.field_00 = 0;
	tMessage.field_04 = 0;
	tMessage.field_08 = 0;
	tMessage.field_0c = 25;
	if (pszFirst)
		pszText = tMessage.szTexts + StormSStrCopy(tMessage.szTexts, pszFirst, 128) + 1;
	else
		pszText = tMessage.szTexts + StormSStrCopy(tMessage.szTexts, "", 128) + 1;
	if (pszSecond)
		pszText += StormSStrCopy(pszText, pszSecond, 128) + 1;
	else
		pszText += StormSStrCopy(pszText, "", 128) + 1;
	dwLength = StormSStrCopy(pszText, "", 128) + (pszText - tMessage.szTexts)
		+ offsetof(BNMSG09, szTexts) + 1;
	BnConnectClearMessage(9);
	BnConnectSend(9, &tMessage, dwLength);
	BnConnectWaitMessage(9);
	return sgpb09;
}

BOOL __fastcall BnSendProductAndTwoTexts(const char *pszFirst, const char *pszSecond)
{
	BNMSG22 tMessage;
	char *pszText; int nLength;

	tMessage.dwProduct = BN_PRODUCT;
	tMessage.field_04 = 0;
	pszText = tMessage.szTexts + StormSStrCopy(tMessage.szTexts, pszFirst, 128) + 1;
	nLength = StormSStrCopy(pszText, pszSecond, 128) + (pszText - tMessage.szTexts) + offsetof(BNMSG22, szTexts) + 1;
	BnConnectSend(34, &tMessage, nLength);
	return TRUE;
}

void __fastcall BnSendFreeRealmLogon(void)
{
	if (sgptReply35)
	{
#line 786
		D2FREE2(sgptReply35);
	}
	sgptReply35 = NULL;
}

void __fastcall BnSendStoreRealmLogon(const BYTE *pbData, DWORD dwLength)
{
	BnSendFreeRealmLogon();
#line 795
	sgptReply35 = (BNREPLY35 *)D2ALLOC2(dwLength + sizeof(DWORD));
	sgptReply35->dwLength = dwLength;
	memcpy(sgptReply35->abData, pbData, dwLength);
	if (dwLength != 8 && dwLength >= 56)
	{ BNREPLY35 *ptReply = sgptReply35; ptReply->dwRealmPort = ntohs((u_short)ptReply->dwRealmPort); }
}

BNREPLY35 * __fastcall BnSendRequestRealmLogon(const char *pszRealm, const char *pszSecond)
{
	BNHASH35 tHashed;
	BNMSG35 tMessage;
	DWORD dwLength;

	if (!pszRealm)
		return NULL;
	tHashed.dwCount = ++sgdwRealmLogons;
	tHashed.dwNonce = gdwNonce;
	GridHash(tHashed.abSecondHash, pszSecond, strlen(pszSecond));
	tMessage.dwCount = sgdwRealmLogons;
	GridHash(tMessage.abHash, &tHashed, sizeof(tHashed));
	dwLength = StormSStrCopy(tMessage.szRealm, pszRealm, sizeof(tMessage.szRealm)) + 1;
	BnConnectClearMessage(53);
	dwLength += offsetof(BNMSG35, szRealm);
	if (!BnConnectSend(53, &tMessage, dwLength))
		return NULL;
	BnConnectWaitMessage(53);
	return sgptReply35;
}

void __fastcall BnSendFreeCharacterList(void)
{
	if (sgpb37)
	{
#line 878
		D2FREE2(sgpb37);
	}
	sgpb37 = NULL;
}

void __fastcall BnSendStoreCharacterList(const BYTE *pbData, DWORD dwLength)
{
	BnSendFreeCharacterList();
#line 892
	sgpb37 = (BYTE *)D2ALLOC2(dwLength);
	memcpy(sgpb37, pbData, dwLength);
}

BYTE * __fastcall BnSendRequestCharacterList(DWORD dwFirst)
{
	BNMSG37 tMessage;
	DWORD dwLength;

	tMessage.dwFirst = dwFirst;
	dwLength = StormSStrCopy(tMessage.szText, "", sizeof(tMessage.szText))
		+ offsetof(BNMSG37, szText) + 1;
	BnConnectClearMessage(55);
	if (!BnConnectSend(55, &tMessage, dwLength))
		return NULL;
	BnConnectWaitMessage(55);
	return sgpb37;
}

void __fastcall BnSendCreateCharacter(const char *pszFirst, const char *pszSecond)
{
	BNMSG38 tMessage;
	char *pszText;
	DWORD dwLength;

	if (!pszFirst)
		return;
	if (!pszSecond)
		pszSecond = "";
	tMessage.field_08 = 0;
	pszText = tMessage.szTexts + StormSStrCopy(tMessage.szTexts, pszFirst, 128) + 1;
	dwLength = StormSStrCopy(pszText, pszSecond, 128) + (pszText - tMessage.szTexts)
		+ offsetof(BNMSG38, szTexts) + 1;
	BnConnectClearMessage(56);
	BnConnectSend(56, &tMessage, dwLength);
}

void __fastcall BnSendSelectCharacter(const char *pszText)
{
	char szText[128];
	DWORD dwLength;

	if (!pszText)
		return;
	dwLength = StormSStrCopy(szText, pszText, sizeof(szText)) + 1;
	BnConnectClearMessage(57);
	BnConnectSend(57, szText, dwLength);
}

int __fastcall BnSendLocaleAndTime(void)
{
	SYSTEMTIME tLocal;
	SYSTEMTIME tSystem;
	BNMSG12 tMessage;
	TIME_ZONE_INFORMATION tZone;
	char *pszText;
	int i, nLength;

	for (i = 0; i < (int)(sizeof(tZone) / sizeof(DWORD)); i++)
		((DWORD *)&tZone)[i] = 0;
	GetTimeZoneInformation(&tZone);
	GetSystemTime(&tSystem);
	GetLocalTime(&tLocal);
	SystemTimeToFileTime(&tSystem, &tMessage.tSystemTime);
	SystemTimeToFileTime(&tLocal, &tMessage.tLocalTime);
	tMessage.lBias = tZone.Bias;
	tMessage.dwSystemLcid = GetSystemDefaultLCID();
	tMessage.dwUserLcid = GetUserDefaultLCID();
	tMessage.dwUserLangId = GetUserDefaultLangID();
	tMessage.szTexts[0] = 0;
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SABBREVLANGNAME, tMessage.szTexts, 64);
	pszText = tMessage.szTexts + strlen(tMessage.szTexts) + 1;
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_ICOUNTRY, pszText, 64);
	pszText += strlen(pszText) + 1;
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SABBREVCTRYNAME, pszText, 64);
	pszText += strlen(pszText) + 1;
	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, pszText, 64);
	nLength = strlen(pszText) + (pszText - tMessage.szTexts) + offsetof(BNMSG12, szTexts) + 1;
	return BnConnectSend(18, &tMessage, nLength);
}

BOOL __fastcall BnSendDwordAndFlag(DWORD dwFirst, BOOL fSecond)
{
	BNMSG16 tMessage;

	tMessage.dwFirst = dwFirst;
	tMessage.fSecond = fSecond != 0;
	BnConnectSend(22, &tMessage, sizeof(tMessage));
	return TRUE;
}

int __fastcall BnSendWriteUserData(const char *pszSubject, DWORD dwCount, const char **ppszKeys,
	const char **ppszValues)
{
	BNMSG27 *ptMessage;
	char *pszText;
	DWORD dwSize;
	DWORD i;
	int nSent;

	dwSize = StormSStrLen(pszSubject) + 1;
	for (i = 0; i < dwCount; i++)
	{
		dwSize += StormSStrLen(ppszKeys[i]) + 1;
		dwSize += StormSStrLen(ppszValues[i]) + 1;
	}
#line 1055
	ptMessage = (BNMSG27 *)D2ALLOC2(dwSize + offsetof(BNMSG27, szTexts));
	ptMessage->dwOne = 1;
	ptMessage->dwCount = dwCount;
	pszText = ptMessage->szTexts;
	pszText += StormSStrCopy(pszText, pszSubject, 2147483647) + 1;
	for (i = 0; i < dwCount; i++)
		pszText += StormSStrCopy(pszText, ppszKeys[i], 2147483647) + 1;
	for (i = 0; i < dwCount; i++)
		pszText += StormSStrCopy(pszText, ppszValues[i], 2147483647) + 1;
	BnConnectClearMessage(39);
	nSent = BnConnectSend(39, ptMessage, pszText - (char *)ptMessage);
#line 1080
	D2FREE2(ptMessage);
	return nSent;
}

void __fastcall BnSendStoreUserData(const BYTE *pbData, DWORD dwLength)
{
	if (sgpb26)
	{
#line 1092
		D2FREE2(sgpb26);
	}
#line 1093
	sgpb26 = (BYTE *)D2ALLOC2(dwLength);
	memcpy(sgpb26, pbData, dwLength);
}

BOOL __fastcall BnSendReadUserData(const char *pszSubject, DWORD dwCount, DWORD dwId, const char **ppszKeys)
{
	BNMSG26 *ptMessage;
	char *pszText;
	DWORD dwSize;
	DWORD i;
	int nSent;

	dwSize = StormSStrLen(pszSubject) + 1;
	for (i = 0; i < dwCount; i++)
		dwSize += StormSStrLen(ppszKeys[i]) + 1;
#line 1118
	ptMessage = (BNMSG26 *)D2ALLOC2(dwSize + offsetof(BNMSG26, szTexts));
	ptMessage->dwOne = 1;
	ptMessage->dwCount = dwCount;
	ptMessage->dwId = dwId;
	pszText = ptMessage->szTexts;
	pszText += StormSStrCopy(pszText, pszSubject, 2147483647) + 1;
	for (i = 0; i < dwCount; i++)
		pszText += StormSStrCopy(pszText, ppszKeys[i], 2147483647) + 1;
	BnConnectClearMessage(38);
	nSent = BnConnectSend(38, ptMessage, pszText - (char *)ptMessage);
#line 1141
	D2FREE2(ptMessage);
	return nSent != 0;
}

BOOL __fastcall BattleReadUserData(DWORD dwCount, DWORD dwId, char **ppszValues, DWORD *pdwSizes)
{
	BNMSG26 *ptReply;
	const char *pszText;
	DWORD i;

	if (!BnConnectIsHandled(38))
		return FALSE;
	ptReply = (BNMSG26 *)sgpb26;
	if (!ptReply || ptReply->dwOne != 1 || ptReply->dwCount != dwCount || ptReply->dwId != dwId)
	{
		BnConnectDebugPrint("BattleReadUserData: Invalid response from server");
		return FALSE;
	}
	pszText = ptReply->szTexts;
	for (i = 0; i < dwCount; i++)
	{
		if (*pszText)
		{
			strncpy(ppszValues[i], pszText, pdwSizes[i]);
			ppszValues[i][pdwSizes[i]] = 0;
		}
		else
			ppszValues[i][0] = 0;
		pszText += StormSStrLen(pszText) + 1;
	}
	return TRUE;
}
