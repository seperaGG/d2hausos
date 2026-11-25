#include "Bnclient.h"
#include "FogImports.h"
#include "StormImports.h"
#include "BnConnect.h"

static DWORD sgdwLoginResult = 255;

BNMESSAGEDWORDS sgadwResult = { 0 };
DWORD sgdwCDKeyReplyCode = 0;
DWORD sgdwCreateAccountResult = 0;
BNMESSAGEDWORDS sgafHandled = { 0 };
BNNAME sgszCDKeyText = { 0 };
BOOL sgfCancelled = 0;
DWORD gdwLastAdId = 0;

int __fastcall BnConnectDeframe(BYTE *pbBuffer, int nLength)
{
	BYTE *pbCur;
	WORD wSize;
	int nLeft;

	pbCur = pbBuffer;
	for (;;)
	{
		while (*pbCur != 255 && pbCur - pbBuffer < nLength)
			pbCur++;
		wSize = ((BNHEADER *)pbCur)->wLength;
		nLeft = pbBuffer - pbCur + nLength;
		if ((DWORD)nLeft < sizeof(BNHEADER) || nLeft < (int)wSize)
			break;
		BnConnectDispatch(((BNHEADER *)pbCur)->bMessage, pbCur + sizeof(BNHEADER), wSize - sizeof(BNHEADER));
		pbCur += wSize;
	}
	nLeft = pbBuffer - pbCur + nLength;
	memcpy(pbBuffer, pbCur, nLeft);
	return nLeft;
}

static __inline int BnConnectSendInline(BYTE bMessage, const void *pData, DWORD dwLength)
{
	BNHEADER tHeader;
	BYTE abBuffer[8192];
	DWORD dwSize;

	tHeader.bSignature = 255;
	tHeader.bMessage = bMessage;
	tHeader.wLength = (WORD)(dwLength + sizeof(tHeader));
	if (!sgptConnection || sgfAbort)
	{
		BnConnectSetCancelled();
		return 0;
	}
	if (!pData)
		dwLength = 0;
	dwSize = dwLength + sizeof(tHeader);
	StormSMemCopy(abBuffer, &tHeader, sizeof(tHeader));
	if (pData && dwSize < sizeof(abBuffer))
		StormSMemCopy(abBuffer + sizeof(tHeader), pData, dwLength);
	return FogSafesockSend(sgptConnection, abBuffer, dwSize);
}

DWORD WINAPI BnConnectPump(LPVOID pArg)
{
	BYTE abBuffer[8196];
	DWORD dwResult;
	DWORD dwLast;
	DWORD dwNow;
	int nFill;
	int nRead;
	BOOL fReadable;

	dwResult = 1;
	if (sglPumps)
		return dwResult;
	InterlockedIncrement(&sglPumps);
	while (!sgfAbort && !sgfConnected)
		Sleep(10);
	dwLast = GetTickCount();
	nFill = 0;
	while (1)
	{
		if (!sgptConnection || sgfAbort || sgptConnection->fClosing)
			break;
		fReadable = FogSafesockCanRead(sgptConnection);
		if (!sgptConnection || sgfAbort || sgptConnection->fClosing)
			break;
		if (fReadable)
		{
			nRead = FogSafesockRecv(sgptConnection, abBuffer + nFill,
				8192 - nFill);
			if (nRead <= 0)
			{
				BnConnectSetCancelled();
				dwResult = 0;
				break;
			}
			nFill = BnConnectDeframe(abBuffer, nRead + nFill);
		}
		dwNow = GetTickCount();
		if (dwNow - dwLast >= 180000)
		{
			BnConnectSendInline(0, NULL, 0);
			dwLast = dwNow;
		}
	}
	InterlockedDecrement(&sglPumps);
	return dwResult;
}

BOOL __fastcall BnConnectGetPeerName(SOCKADDR *ptAddress, int *pnLength)
{
	if (sgptConnection)
		return FogSafesockGetPeerName(sgptConnection, ptAddress, pnLength);
	return FALSE;
}

int __fastcall BnConnectZero(void)
{
	return 0;
}

BOOL __fastcall BnConnectIsConnected(void)
{
	BNERRINFO tReason;

	BnMessQueuePeekReason(&tReason);
	if (!BnConnectIsCancelled() && !tReason.dwReason && sgptConnection)
		return TRUE;
	return FALSE;
}

void __cdecl BnConnectDebugPrint(const char *pszFormat, ...)
{
}

BOOL __fastcall BnConnectSetCancelled(void)
{
	return sgfCancelled = TRUE;
}

BOOL __fastcall BnConnectIsCancelled(void)
{
	return sgfCancelled;
}

DWORD __fastcall BnConnectGetLoginResult(void)
{
	return sgdwLoginResult;
}

char * __fastcall BnConnectGetCDKeyText(void)
{
	return sgszCDKeyText;
}

DWORD __fastcall BnConnectGetCreateAccountResult(void)
{
	return sgdwCreateAccountResult;
}

BOOL __fastcall BnConnectReset(void)
{
	int i;

	for (i = 0; i < NUM_BNMESSAGES; i++)
		sgadwResult[i] = 0;
	for (i = 0; i < NUM_BNMESSAGES; i++)
		sgafHandled[i] = 0;
	sgdwLoginResult = 255;
	sgfCancelled = FALSE;
	BnMessQueueForceReason(0);
	return TRUE;
}

BOOL __fastcall BnConnectWaitMessage(BYTE bMessage)
{
	DWORD dwStart;
	DWORD *pdwHandled;

	dwStart = GetTickCount();
	pdwHandled = &sgafHandled[bMessage];
	while (!*pdwHandled)
	{
		if (sgfCancelled)
		{
			BnMessQueueSetReason(14);
			return FALSE;
		}
		if (GetTickCount() - dwStart > 45000)
		{
			BnMessQueueSetReason(13);
			return FALSE;
		}
		Sleep(10);
	}
	return TRUE;
}

BOOL __fastcall BnConnectIsHandled(BYTE bMessage)
{
	return sgafHandled[bMessage];
}

void __fastcall BnConnectClearMessage(BYTE bMessage)
{
	sgafHandled[bMessage] = 0;
	sgadwResult[bMessage] = 0;
}

DWORD __fastcall BnConnectGetResult(BYTE bMessage)
{
	return sgadwResult[bMessage];
}

void __fastcall BnConnectSetResult(BYTE bMessage, DWORD dwResult)
{
	sgadwResult[bMessage] = dwResult;
}

BOOL __fastcall BnConnectOnKeepalive(BYTE *pbData, DWORD dwLength)
{
	return TRUE;
}

BOOL __fastcall BnConnectOnKeepResult(BYTE *pbData, DWORD dwLength)
{
	sgadwResult[42] = *(DWORD *)pbData;
	return TRUE;
}

BOOL __fastcall BnConnectOnCreateAccountReply(BYTE *pbData, DWORD dwLength)
{
	sgdwCreateAccountResult = *(DWORD *)pbData;
	sgadwResult[61] = TRUE;
	return TRUE;
}

BOOL __fastcall BDClientId(BYTE *pbData, DWORD dwLength)
{
	DWORD *pdwData = (DWORD *)pbData;
	DWORD dwId, dwAuthority, dwToken;

	if (!pbData || dwLength < 16)
	{
		BnclientLogPrintf("ERROR BDClientId: invalid response");
		return FALSE;
	}
	dwAuthority = pdwData[1];
	dwId = pdwData[2];
	dwToken = pdwData[3];
	StormSRegSaveValue("Configuration", "Registration Version", 2, pdwData[0]);
	StormSRegSaveValue("Configuration", "Registration Authority", 2, dwAuthority);
	StormSRegSaveValue("Configuration", "Client ID", 2, dwId);
	StormSRegSaveValue("Configuration", "Client Token", 2, dwToken);
	BnclientLogControl(2, 4);
	BnclientLogPrintf("client acknowledged");
	BnclientLogControl(2, -4);
	return TRUE;
}

BOOL __fastcall BnConnectOnKeepNonce(BYTE *pbData, DWORD dwLength)
{
	BnMessQueueSetNonce(*(DWORD *)pbData);
	return TRUE;
}

BOOL __fastcall BnConnectOnCreateGameReply(BYTE *pbData, DWORD dwLength)
{
	if (!*(DWORD *)pbData)
		sgadwResult[28] = TRUE;
	return TRUE;
}

BOOL __fastcall BnConnectOnLogonAcknowledged(BYTE *pbData, DWORD dwLength)
{
	BnMessQueueSetNonce(((DWORD *)pbData)[1]);
	BnclientLogControl(2, 4);
	BnclientLogPrintf("logon acknowledged");
	BnclientLogControl(2, -4);
	return TRUE;
}

BOOL __fastcall BnConnectOnKeepZeroFlag(BYTE *pbData, DWORD dwLength)
{
	sgdwLoginResult = *(DWORD *)pbData == 0;
	return TRUE;
}

BOOL __fastcall BnConnectOnLoginReply(BYTE *pbData, DWORD dwLength)
{
	DWORD dwResult = *(DWORD *)pbData;

	sgdwLoginResult = dwResult;
	switch (dwResult)
	{
	case 0:
		break;
	case 1:
		BnMessQueueSetReason(9);
		break;
	case 2:
		BnMessQueueSetReason(10);
		break;
	case 3:
		BnMessQueueSetReason(11);
		break;
	default:
		BnMessQueueSetReason(14);
		break;
	}
	return TRUE;
}

BOOL __fastcall BnConnectOnVersionVerdict(BYTE *pbData, DWORD dwLength)
{
	FILETIME tNone;
	DWORD dwResult = *(DWORD *)pbData;
	BYTE *pbList;
	DWORD dwList;

	StormSRegSaveData("Patch", "Patches", 130, "", 1);
	if (dwResult == 1)
	{
		pbList = pbData + sizeof(DWORD);
		dwList = dwLength - sizeof(DWORD);
		BnDownloadSetPatchList(pbList, dwList);
		StormSRegSaveData("Patch", "Patches", 130, pbList, dwList);
		tNone.dwLowDateTime = 0;
		tNone.dwHighDateTime = 0;
		BnDownloadFile(BNDL_PATCH, 2, 0, 0, &tNone, (const char *)pbList, NULL);
		sgadwResult[7] = FALSE;
		BnclientLogPrintf("downloading upgrade");
		return TRUE;
	}
	if (dwResult == 2)
	{
		sgadwResult[7] = TRUE;
		BnclientLogPrintf("user has latest version");
		return TRUE;
	}
	if (dwResult == 3)
	{
		BnMessQueueSetReason(1);
		sgadwResult[7] = FALSE;
		BnclientLogPrintf("User must reinstall and patch");
		return TRUE;
	}
	BnMessQueueSetReason(1);
	sgadwResult[7] = FALSE;
	BnclientLogPrintf("user has unknown version");
	return TRUE;
}

BOOL __fastcall BnConnectOnVersionFile(BYTE *pbData, DWORD dwLength)
{
	char *pszFile = (char *)pbData + 8;

	if (strlen(pszFile) + 1 + 8 < dwLength)
		BnDownloadSetVersionText(pszFile + strlen(pszFile) + 1);
	BnDownloadFile(BNDL_VERSION, 3, 0, 0, (const FILETIME *)pbData, pszFile, NULL);
	return TRUE;
}

BOOL __fastcall BnConnectOnEcho(BYTE *pbData, DWORD dwLength)
{
	BnConnectSend(37, pbData, dwLength);
	return TRUE;
}

BOOL __fastcall BnConnectOnCDKeyReply(BYTE *pbData, DWORD dwLength)
{
	DWORD dwResult = *(DWORD *)pbData;

	sgszCDKeyText[0] = 0;
	sgdwCDKeyReplyCode = dwResult;
	sgadwResult[54] = FALSE;
	switch (dwResult)
	{
	case 1:
		sgadwResult[54] = TRUE;
		break;
	case 2:
		BnMessQueueForceReason(3);
		break;
	case 3:
		BnMessQueueForceReason(5);
		break;
	case 4:
		BnMessQueueForceReason(4);
		break;
	case 5:
		BnMessQueueForceReason(6);
		StormSStrCopy(sgszCDKeyText, pbData + sizeof(DWORD), sizeof(sgszCDKeyText));
		BnMessQueueSetInfo(sgszCDKeyText);
		break;
	}
	return TRUE;
}

BOOL __fastcall BnConnectOnServerList(BYTE *pbData, DWORD dwLength)
{
	StormSRegSaveString("Configuration", "Server List", 2, pbData + sizeof(DWORD));
	return TRUE;
}

BOOL __fastcall BnConnectOnChannelList(BYTE *pbData, DWORD dwLength)
{
	BnSendStoreChannelList(pbData, dwLength);
	return TRUE;
}

BOOL __fastcall BnConnectOnGameList(BYTE *pbData, DWORD dwLength)
{
	BnSendStoreGameList(pbData, dwLength);
	return TRUE;
}

BOOL __fastcall BnConnectOnNothingSecond(BYTE *pbData, DWORD dwLength)
{
	return TRUE;
}

BOOL __fastcall BnConnectOnEnterChatReply(BYTE *pbData, DWORD dwLength)
{
	BnMessQueueAddText(2, (const char *)pbData);
	BnConnectSetName((const char *)pbData);
	return TRUE;
}

BOOL __fastcall BnConnectOnFileReply(BYTE *pbData, DWORD dwLength)
{
	const FILETIME *ptTime = (const FILETIME *)(pbData + 8);

	BnDownloadFile(*(DWORD *)pbData, 5, 0, 0, ptTime, (const char *)(ptTime + 1), NULL);
	return TRUE;
}

BOOL __fastcall BnConnectOnAdvertisement(BYTE *pbData, DWORD dwLength)
{
	DWORD dwId = *(DWORD *)pbData;
	const FILETIME *ptTime;
	char *pszFirst;
	char *pszSecond;

	if (dwLength >= 17 && dwId != gdwLastAdId)
	{
		ptTime = (const FILETIME *)(pbData + 8);
		pszFirst = (char *)(ptTime + 1);
		gdwLastAdId = dwId;
		pszSecond = pszFirst + strlen(pszFirst) + 1;
		if (*pszFirst && *pszSecond)
			BnDownloadFile(BNDL_AD, 5, dwId, ((DWORD *)pbData)[1], ptTime, pszFirst,
				pszSecond);
	}
	return TRUE;
}

BOOL __fastcall BnConnectOnRealmList(BYTE *pbData, DWORD dwLength)
{
	BnSendStoreRealmList(pbData, dwLength);
	return TRUE;
}

BOOL __fastcall BnConnectOnRealmLogonReply(BYTE *pbData, DWORD dwLength)
{
	BnSendStoreRealmLogon(pbData, dwLength);
	return TRUE;
}

BOOL __fastcall BnConnectOnCharacterList(BYTE *pbData, DWORD dwLength)
{
	BnSendStoreCharacterList(pbData, dwLength);
	return TRUE;
}

BOOL __fastcall BnConnectOnUserData(BYTE *pbData, DWORD dwLength)
{
	BnSendStoreUserData(pbData, dwLength);
	return TRUE;
}

static PFNBNRECV sgapfnRecv[NUM_BNMESSAGES] =
{
	 	BnConnectOnKeepalive, NULL, NULL, NULL,
	 	BnConnectOnServerList, BDClientId, BnConnectOnVersionFile, BnConnectOnVersionVerdict,
	 	NULL, BnConnectOnGameList, BnConnectOnEnterChatReply, BnConnectOnChannelList,
	 	NULL, NULL, NULL, BnConnectOnLobbyNotification,
	 	NULL, NULL, NULL, NULL,
	 	NULL, BnConnectOnAdvertisement, NULL, NULL,
	 	NULL, NULL, NULL, NULL,
	 	BnConnectOnCreateGameReply, BnConnectOnLogonAcknowledged, NULL, NULL,
	 	BnConnectOnNothingSecond, NULL, NULL, NULL,
	 	NULL, BnConnectOnEcho, BnConnectOnUserData, NULL,
	 	BnConnectOnKeepNonce, BnConnectOnKeepZeroFlag, BnConnectOnKeepResult, NULL,
	 	NULL, NULL, NULL, NULL,
	 	NULL, NULL, NULL, BnConnectOnFileReply,
	 	BnConnectOnRealmList, BnConnectOnRealmLogonReply, BnConnectOnCDKeyReply,
			BnConnectOnCharacterList,
	 	NULL, NULL, BnConnectOnLoginReply, NULL,
	 	NULL, BnConnectOnCreateAccountReply
};

BOOL __fastcall BnConnectDispatch(BYTE bMessage, BYTE *pbData, DWORD dwLength)
{
	BOOL fResult;
	PFNBNRECV pfnRecv;

	fResult = FALSE;
	if (bMessage < NUM_BNMESSAGES)
	{
		pfnRecv = sgapfnRecv[bMessage];
		if (pfnRecv)
			fResult = pfnRecv(pbData, dwLength);
		sgafHandled[bMessage] = TRUE;
	}
	return fResult;
}
