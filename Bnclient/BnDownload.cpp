#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\BnDownload.cpp"
#include "Bnclient.h"
#include "D2Macros.h"
#include "FogImports.h"
#include "StormImports.h"

typedef char BNPATHTEXT[260];
typedef BYTE BNPATCHLIST[256];
typedef char BNVERSIONTEXT[260];

static BOOL sgfCancel;
static DWORD sgdwPatchPercent;
static BNLOCK sgtLock;
static BNPATHTEXT sgszVersionFile = { 0 };
static LONG sglDownloads = 0;
static BNPATCHLIST sgabPatchList = { 0 };
static BNVERSIONTEXT sgszVersionText = { 0 };

typedef BOOL (__stdcall *PFNCHECKREVISION)(const char *pszExe, const char *pszBnclient,
	const char *pszD2Client, const char *pszText, DWORD *pdwFirst, DWORD *pdwSecond, char *pszResult);

struct BNMSG33
{
 	DWORD		dwKind;
 	DWORD		field_04;
 	char		szFile[260];
};

struct BNPCXHEADER
{
 	BYTE		bSignature;
 	BYTE		bVersion;
 	BYTE		bEncoding;
 	BYTE		bBitsPerPixel;
 	WORD		wLeft;
 	WORD		wTop;
 	WORD		wRight;
 	WORD		wBottom;
 	BYTE		unobserved_0c[53];
 	BYTE		bPlanes;
 	WORD		wBytesPerLine;
 	BYTE		unobserved_44[60];
};

union BNCOLOR
{
	DWORD		dwColor;
	struct
	{
		BYTE		bRed;
		BYTE		bGreen;
		BYTE		bBlue;
		BYTE		bAlpha;
	};
};

static unsigned __stdcall DOWNLOADTOANY(void *pArg);
static void __fastcall BnDownloadProcess(DWORD dwKind, DWORD dwFlags, DWORD dwFirst, DWORD dwSecond,
	const FILETIME *ptTime, const char *pszFile, const char *pszText, const BYTE *pbData, DWORD dwBytes);
static BOOL __fastcall BnDownloadRunPatch(int *upgradestatus);
static BOOL BnDownloadReceive(SOCKET hSocket, const char *pszName, DWORD dwFileBytes, DWORD dwKind,
	BYTE **ppbFile);
static void BnDownloadExtract(HANDLE hArchive, const char *pszFile, const char *pszPath);

BOOL __fastcall BnDownloadSetPatchList(const BYTE *pbData, DWORD dwLength)
{
	sgtLock.Enter();
	StormSMemCopy(sgabPatchList, pbData, dwLength);
	sgtLock.Leave();
	return TRUE;
}

BOOL __fastcall BnDownloadSetVersionText(const char *pszText)
{
	StormSStrCopy(sgszVersionText, pszText, 2147483647);
	return TRUE;
}

BOOL __fastcall BnDownloadRequestFile(const char *pszFile, DWORD dwKind)
{
	BNMSG33 tMessage;
	DWORD dwLength;
	char *pszEnd;

	tMessage.dwKind = dwKind;
	tMessage.field_04 = 0;
	dwLength = StormSStrCopy(tMessage.szFile, pszFile, sizeof(tMessage.szFile));
	pszEnd = &tMessage.szFile[dwLength] + 1;
	BnConnectSend(51, &tMessage, pszEnd - (char *)&tMessage);
	return TRUE;
}

static inline void BnDownloadWriteFile(const char *pszName, const BYTE *pbData, DWORD dwBytes, const FILETIME *ptTime)
{
	HANDLE hFile;
	DWORD dwWritten;

	hFile = FogSafesockCreateFile(pszName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		FogSafesockWriteFile(hFile, pbData, dwBytes, &dwWritten, 0);
		SetFileTime(hFile, ptTime, ptTime, ptTime);
		FogSafesockCloseHandle(hFile);
	}
}

void __fastcall BnDownloadFile(DWORD dwKind, DWORD dwFlags, DWORD dwFirst, DWORD dwSecond,
	const FILETIME *ptTime, const char *pszFile, const char *pszText)
{
	BYTE *pbData;
	DWORD dwBytes;
	FILETIME tCached;
	BNDOWNLOAD *ptRequest;
	unsigned nThreadId;
	unsigned long hThread;

	if (!pszFile || !*pszFile)
		return;
	if (!pszText)
		pszText = "";
	if (CacheFind(pszFile, &tCached, &pbData, &dwBytes))
	{
		if (!CompareFileTime(ptTime, &tCached))
		{
			if (dwFlags & 2)
				BnDownloadWriteFile(pszFile, pbData, dwBytes, ptTime);
			BnConnectDebugPrint("<%s is in cache.  Processing>", pszFile);
			BnDownloadProcess(dwKind, dwFlags, dwFirst, dwSecond, ptTime, pszFile, pszText, pbData, dwBytes);
			CacheFree(pbData, dwBytes);
			return;
		}
		CacheFree(pbData, dwBytes);
	}
#line 150
	ptRequest = (BNDOWNLOAD *)StormSMemAlloc(sizeof(BNDOWNLOAD), D2_THIS_FILE, __LINE__, 0);
	if (!ptRequest)
		return;
	memset(ptRequest, 0, sizeof(BNDOWNLOAD));
	ptRequest->wLength = (WORD)(strlen(pszFile) + 1 + offsetof(BNDOWNLOAD, szFile));
	ptRequest->field_02 = 0;
	ptRequest->field_03 = 1;
	ptRequest->dwPlatform = BN_PLATFORM;
	ptRequest->dwProduct = BN_PRODUCT;
	ptRequest->dwFirst = dwFirst;
	ptRequest->dwSecond = dwSecond;
	ptRequest->dwOffset = 0;
	ptRequest->dwKind = dwKind;
	ptRequest->dwFlags = dwFlags;
	ptRequest->tTime = *ptTime;
	StormSStrCopy(ptRequest->szFile, pszFile, sizeof(ptRequest->szFile));
	StormSStrCopy(ptRequest->szText, pszText, sizeof(ptRequest->szText));
	InterlockedIncrement(&sglDownloads);
	hThread = _beginthreadex(NULL, 0, DOWNLOADTOANY, ptRequest, 0, &nThreadId);
	if (hThread)
		CloseHandle((HANDLE)hThread);
}

static BOOL BnDownloadMakeAd(const BYTE *pbFile, DWORD dwBytes)
{
	BNPCXHEADER tHeader;
	BYTE abPalette[768];
	const BYTE *pbIn;
	BYTE *pbRgb;
	BYTE *pbIndex;
	BYTE *pbCel;
	void *pCelFile;
	char szPalette[260];
	BNCOLOR tKeyColor;
	DWORD dwList;
	DWORD dwCelBytes;

	HANDLE hFile;
	int Width;
	int Height;
	int nPixels;
	int nLeft;
	int nOut;
	int nX;
	int nY;
	int nLine;
	BYTE bCount;
	BYTE bValue;
	BYTE b;

	memcpy(&tHeader, pbFile, sizeof(tHeader));
	if (tHeader.bSignature != 10)
		return TRUE;
	if (tHeader.bEncoding != 1 || tHeader.bVersion != 5 || tHeader.bBitsPerPixel != 8 || tHeader.bPlanes != 1
		|| tHeader.wLeft != 0 || tHeader.wTop != 0)
		return FALSE;
	memcpy(abPalette, pbFile + dwBytes - sizeof(abPalette), sizeof(abPalette));
	pbIn = pbFile + sizeof(tHeader);
	Width = tHeader.wRight + 1;
	Height = tHeader.wBottom + 1;
#line 314
	D2ASSERT(Width > 0 && Width < 2048);
#line 315
	D2ASSERT(Height > 0 && Height < 2048);
	nPixels = Height * Width;
#line 318
	pbRgb = (BYTE *)D2ALLOC2(3 * nPixels);
#line 319
	pbIndex = (BYTE *)D2ALLOC2(nPixels);
	memset(pbRgb, 0, 3 * nPixels);
	nLine = tHeader.wBytesPerLine;
	nOut = 0;
	memset(pbIndex, 0, nPixels);
	nLeft = nPixels;
	nX = 0;
	nY = 0;
	do
	{
		b = *pbIn++;
		if ((b & 192) == 192)
		{
			bCount = b & 63;
			bValue = *pbIn++;
		}
		else
		{
			bValue = b;
			bCount = 1;
		}
		nLeft -= bCount;
		if (nLeft < 0)
			continue;
		while (1)
		{
			if (!bCount)
				break;
			nX++;
			pbRgb[3 * nOut] = abPalette[3 * bValue + 2];
			pbRgb[3 * nOut + 1] = abPalette[3 * bValue + 1];
			pbRgb[3 * nOut + 2] = abPalette[3 * bValue];
			if (nX >= nLine)
			{
				nX = 0;
				nY++;
			}
			nOut++;
			bCount--;
		}
	} while (nY < Height && nLeft > 0);
	tKeyColor.bRed = 0;
	tKeyColor.bGreen = 0;
	tKeyColor.bBlue = 0;
	tKeyColor.bAlpha = 0;
	wsprintfA(szPalette, "%s\\palette\\act1\\pal.dat", "DATA\\GLOBAL");
	D2CMPPaletteLoad(szPalette, 1, 223);
	D2CMPPaletteSetDarkToBlack(0);
	D2CMPPaletteSetMatchKeyColor(
		0);
	D2CMPPaletteSetKeyColor(tKeyColor.dwColor);
	D2CMPPaletteQuantize(pbRgb, pbIndex, nPixels, 0);
	dwList = D2CMPCelListCreate(0, 1, &tKeyColor, 0);
	D2CMPCelListAddImage(dwList, pbIndex, Width, Height, Width, 0, 0, 0);
#line 375
	D2FREE2(pbIndex);
#line 376
	D2FREE2(pbRgb);
	D2CMPCelListBuild(dwList, &pCelFile);
	if (!pCelFile)
		return FALSE;
	D2CMPCelFileSerialize(pCelFile, &pbCel, &dwCelBytes);
	hFile = FogSafesockCreateFile("temp.dc5", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{ DWORD dwWritten;
		FogSafesockWriteFile(hFile, pbCel, dwCelBytes, &dwWritten, 0);
		FogSafesockCloseHandle(hFile);
	}
	D2CMPCelFileFreeSerialized(pbCel);
	D2CMPCelFileFree(pCelFile);
	D2CMPCelListFree(dwList);
	return TRUE;
}

static inline BYTE *BnDownloadCopyData(const BYTE *pbData, DWORD dwBytes)
{
	BYTE *pbCopy;

	if (dwBytes && pbData)
	{
#line 474
		pbCopy = (BYTE *)D2ALLOC2(dwBytes);
		memcpy(pbCopy, pbData, dwBytes);
		return pbCopy;
	}
	return NULL;
}

static void __fastcall BnDownloadProcess(DWORD dwKind, DWORD dwFlags, DWORD dwFirst, DWORD dwSecond,
	const FILETIME *ptTime, const char *pszFile, const char *pszText, const BYTE *pbData, DWORD dwBytes)
{
	BNAD tAd;
	char szLine[512];
	char *pszFileCopy;
	char *pszTextCopy;
	char *pszEntry;
	char *pszIni;
	DWORD dwIni;

	switch (dwKind)
	{
	case BNDL_AD:
		tAd.dwFirst = dwFirst;
		tAd.dwSecond = dwSecond;
		if (pszFile && strlen(pszFile))
		{
#line 456
			pszFileCopy = (char *)D2ALLOC2(strlen(pszFile) + 1);
			strcpy(pszFileCopy, pszFile);
		}
		else
			pszFileCopy = NULL;
		tAd.pszFile = pszFileCopy;
		if (pszText && strlen(pszText))
		{
#line 465
			pszTextCopy = (char *)D2ALLOC2(strlen(pszText) + 1);
			strcpy(pszTextCopy, pszText);
		}
		else
			pszTextCopy = NULL;
		tAd.pszText = pszTextCopy;
		tAd.pbData = BnDownloadCopyData(pbData, dwBytes);
		tAd.dwBytes = dwBytes;
		BnSendForAdvertisement(pszFile, pszText);
		sprintf(szLine, "<Received Ad: %s, %s>\n", tAd.pszFile, tAd.pszText);
		BnMessQueueAddText(1, szLine);
		if (!BnDownloadMakeAd(pbData, dwBytes))
			return;
		BnMessQueueAdd(3, (const BYTE *)&tAd, sizeof(tAd));
		return;
	case BNDL_PATCH:
		sgtLock.Enter();
		pszEntry = (char *)sgabPatchList;
		while (*pszEntry && _stricmp(pszEntry, pszFile))
		{
			pszEntry += strlen(pszEntry) + 1;
		}
		if (*pszEntry && pszEntry[strlen(pszEntry) + 1])
		{ FILETIME tNone;
			tNone.dwLowDateTime = 0;
			tNone.dwHighDateTime = 0;
			BnDownloadFile(BNDL_PATCH, 2, 0, 0, &tNone, pszEntry + strlen(pszEntry) + 1, NULL);
			sgtLock.Leave();
			return;
		}
		sgabPatchList[0] = 0;
		sgabPatchList[1] = 0;
		sgtLock.Leave();
		return;
	case BNDL_VERSION:
		BnConnectDebugPrint("Processing Version file");
		strcpy(sgszVersionFile, pszFile);
		return;
	case BNDL_SERVERINI:
		BnConnectDebugPrint("Processing Bnserver.ini file");
		dwIni = StormSStrLen(pbData);
#line 540
		pszIni = (char *)StormSMemAlloc(dwIni + 1, D2_THIS_FILE, __LINE__, 0);
		StormSStrCopy(pszIni, pbData, dwIni + 1);
		gtGateways.Update(pszIni);
#line 546
		StormSMemFree(pszIni, D2_THIS_FILE, __LINE__, 0);
		gtGateways.Save();
		return;
	}
}

void __fastcall BnDownloadCancel(void)
{
	sgfCancel = TRUE;
}

static inline DWORD BnDownloadSizeOnDisk(const char *pszFile, const FILETIME *ptTime)
{
	HANDLE hFile;
	FILETIME tLocal;
	LARGE_INTEGER tDifference;
	DWORD dwBytes;

	hFile = FogSafesockCreateFile(pszFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		GetFileTime(hFile, NULL, NULL, &tLocal);
		tDifference.QuadPart = *(__int64 *)&tLocal - *(__int64 *)ptTime;
		if (tDifference.QuadPart < 0)
			tDifference.QuadPart = -tDifference.QuadPart;
		if (!tDifference.HighPart && tDifference.LowPart <= 40000000)
		{
			dwBytes = GetFileSize(hFile, NULL);
			FogSafesockCloseHandle(hFile);
			return dwBytes;
		}
		FogSafesockCloseHandle(hFile);
	}
	return 0;
}

static inline BOOL BnDownloadReceiveToFile(SOCKET hSocket, const char *pszName, DWORD dwPos, DWORD dwFileBytes,
	const FILETIME *ptTime, DWORD dwKind)
{
	HANDLE hFile;
	DWORD dwChunk;
	DWORD dwWritten;
	int nGot;
	BYTE abBuffer[1460];

	hFile = FogSafesockCreateFile(pszName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	if (dwPos)
		FogSafesockSetFilePointer(hFile, dwPos, 0, 0);
	while (dwPos < dwFileBytes)
	{
		dwChunk = min(sizeof(abBuffer), dwFileBytes - dwPos);
		nGot = recv(hSocket, (char *)abBuffer, dwChunk, 0);
		if (nGot == SOCKET_ERROR || nGot <= 0 || sgfCancel)
		{
			FogSafesockCloseHandle(hFile);
			return FALSE;
		}
		FogSafesockWriteFile(hFile, abBuffer, nGot, &dwWritten, 0);
		SetFileTime(hFile, ptTime, ptTime, ptTime);
		dwPos += nGot;
		if (dwKind == BNDL_PATCH)
			sgdwPatchPercent = dwPos * 100 / dwFileBytes;
	}
	FogSafesockCloseHandle(hFile);
	return TRUE;
}

static inline BOOL BnDownloadReceiveToMemory(SOCKET hSocket, const char *pszName, DWORD dwFileBytes,
	const FILETIME *ptTime, DWORD dwFlags, DWORD dwKind, BYTE **ppbFile)
{
	if (!BnDownloadReceive(hSocket, pszName, dwFileBytes, dwKind, ppbFile))
		return FALSE;
	if (dwFlags & 1)
		CacheStore(pszName, *ppbFile, dwFileBytes, ptTime, 1296000, 2592000);
	if (dwFlags & 2)
		BnDownloadWriteFile(pszName, *ppbFile, dwFileBytes, ptTime);
	return TRUE;
}

static unsigned __stdcall DOWNLOADTOANY(void *pArg)
{
	BNDOWNLOAD *ptRequest = (BNDOWNLOAD *)pArg;
	BNDOWNLOAD tRequest;
	BNDLREPLY *ptReply;
	BYTE *pbFile;
	char *pszServer;
	int nRetries;
	char szServers[512];
	char szHost[256];
	SOCKADDR_IN tAddress;
	int nAddressLength;
	SOCKET hSocket;
	int nOption;
	int nTimeout;
	char bSelector;
	DWORD dwReplySize;
	DWORD dwOnDisk;
	DWORD dwFlags;
	DWORD dwKind;

	pbFile = NULL;
	ptReply = NULL;
	pszServer = NULL;
	nRetries = 0;
	szServers[0] = 0;
	sgfCancel = FALSE;
	nAddressLength = sizeof(tAddress);
	if (!BnConnectGetPeerName((SOCKADDR *)&tAddress, &nAddressLength))
	{
		BnConnectGetGateway(szHost, sizeof(szHost));
		BnConnectResolve(szServers, szHost);
		pszServer = strtok(szServers, " ,;");
		memset(&tAddress, 0, sizeof(tAddress));
		tAddress.sin_family = AF_INET;
		tAddress.sin_port = htons(BNET_PORT);
		tAddress.sin_addr.s_addr = inet_addr(pszServer);
	}
	else
	{
		tAddress.sin_port = htons(BNET_PORT);
		memset(tAddress.sin_zero, 0, sizeof(tAddress.sin_zero));
	}
	while ((hSocket = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET)
	{
		if (connect(hSocket, (SOCKADDR *)&tAddress, sizeof(tAddress)))
		{
			if (sgfCancel || ptRequest->dwKind != BNDL_PATCH)
			{
				BnConnectDebugPrint("Cancelled download.");
				goto done;
			}
			closesocket(hSocket);
			hSocket = INVALID_SOCKET;
			if (++nRetries > 3)
			{
				BnConnectDebugPrint("ERROR: Failed recv from BNET for download.");
				goto done;
			}
			BnConnectDebugPrint("WARNING: Failed to connect to download port. Retrying...");
			Sleep(1000);
			if (szServers[0])
			{
				if (!pszServer)
					pszServer = strtok(szServers, " ,;");
				else
					pszServer = strtok(NULL, " ,;");
			}
			if (!pszServer)
				goto done;
			tAddress.sin_addr.s_addr = inet_addr(pszServer);
		}
		else
		{
			nOption = 1;
			setsockopt(hSocket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&nOption, sizeof(nOption));
			nTimeout = 15000;
			setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&nTimeout, sizeof(nTimeout));
			bSelector = 2;
			if (send(hSocket, &bSelector, 1, 0) != 1)
			{
				BnConnectDebugPrint("ERROR: Failed send to BNET for download.");
				goto done;
			}
			tRequest = *ptRequest;
			if (send(hSocket, (const char *)&tRequest, ptRequest->wLength, 0) != ptRequest->wLength)
			{
				BnConnectDebugPrint("ERROR: Failed send to BNET for download.");
				goto done;
			}
			if (recv(hSocket, (char *)&dwReplySize, 4, 0) != 4)
			{
				BnConnectDebugPrint("ERROR: Failed send to BNET for download.");
				goto done;
			}
#line 690
			ptReply = (BNDLREPLY *)StormSMemAlloc(dwReplySize, D2_THIS_FILE, __LINE__, 0);
			if (!ptReply)
				goto done;
			if (recv(hSocket, (char *)&ptReply->dwFileBytes, dwReplySize - sizeof(DWORD), 0)
				!= (int)(dwReplySize - sizeof(DWORD)))
			{
				BnConnectDebugPrint("ERROR: Failed recv from BNET for download.");
				goto done;
			}
			if (ptRequest->dwFlags == 2)
			{
				dwOnDisk = BnDownloadSizeOnDisk(ptReply->szFile, &ptReply->tTime);
				if (dwOnDisk != ptRequest->dwOffset)
				{
					ptRequest->dwOffset = dwOnDisk;
					closesocket(hSocket);
#line 716
					StormSMemFree(ptReply, D2_THIS_FILE, __LINE__, 0);
					ptReply = NULL;
					continue;
				}
			}
			dwFlags = ptRequest->dwFlags;
			if (dwFlags == 2)
			{
				if (!BnDownloadReceiveToFile(hSocket, ptReply->szFile, ptRequest->dwOffset, ptReply->dwFileBytes,
					&ptReply->tTime, ptRequest->dwKind))
				{
					if (sgfCancel)
					{
						BnConnectDebugPrint("ERROR: Cancelled download.");
						goto done;
					}
					closesocket(hSocket);
#line 741
					StormSMemFree(ptReply, D2_THIS_FILE, __LINE__, 0);
					ptReply = NULL;
					BnConnectDebugPrint("WARNING: Failed to save to disk.");
					continue;
				}
			}
			else
			{
				if (!BnDownloadReceiveToMemory(hSocket, ptReply->szFile, ptReply->dwFileBytes, &ptReply->tTime,
					dwFlags, ptRequest->dwKind, &pbFile))
				{
					if (sgfCancel)
					{
						BnConnectDebugPrint("ERROR: Cancelled download.");
						goto done;
					}
					closesocket(hSocket);
#line 768
					StormSMemFree(ptReply, D2_THIS_FILE, __LINE__, 0);
					ptReply = NULL;
					BnConnectDebugPrint("ERROR: Failed to copy file.");
					continue;
				}
			}
			BnDownloadProcess(ptRequest->dwKind, ptRequest->dwFlags, ptReply->dwFirst, ptReply->dwSecond,
				&ptReply->tTime, ptReply->szFile, ptRequest->szText, pbFile, ptReply->dwFileBytes);
			goto done;
		}
	}
done:
	dwKind = ptRequest->dwKind;
	if (hSocket != INVALID_SOCKET)
		closesocket(hSocket);
	if (pbFile)
	{
#line 793
		StormSMemFree(pbFile, D2_THIS_FILE, __LINE__, 0);
	}
	if (ptReply)
	{
#line 794
		StormSMemFree(ptReply, D2_THIS_FILE, __LINE__, 0);
	}
	if (ptRequest)
	{
#line 795
		StormSMemFree(ptRequest, D2_THIS_FILE, __LINE__, 0);
	}
	if (dwKind == BNDL_PATCH && BnDownloadGetProgress() < 100)
		BnDownloadClearPatchList();
	InterlockedDecrement(&sglDownloads);
	_endthreadex(0);
	return 0;
}

static BOOL BnDownloadReceive(SOCKET hSocket, const char *pszName, DWORD dwFileBytes, DWORD dwKind,
	BYTE **ppbFile)
{
	DWORD dwPos;
	DWORD dwChunk;
	int nGot;

#line 824
	*ppbFile = (BYTE *)StormSMemAlloc(dwFileBytes, D2_THIS_FILE, __LINE__, 8);
	for (dwPos = 0; dwPos < dwFileBytes; )
	{
		dwChunk = min(1460, dwFileBytes - dwPos);
		nGot = recv(hSocket, (char *)*ppbFile + dwPos, dwChunk, 0);
		if (nGot == SOCKET_ERROR || nGot <= 0 || sgfCancel)
		{
#line 834
			StormSMemFree(*ppbFile, D2_THIS_FILE, __LINE__, 0);
			*ppbFile = NULL;
			BnConnectDebugPrint("<ERROR: Failed to recv file  %s in DOWNLOADTOANY, "
				"filebytes = %d, currpos = %d, bytes = %d", pszName, dwFileBytes, dwPos, nGot);
			return FALSE;
		}
		dwPos += nGot;
		if (dwKind == BNDL_PATCH)
			sgdwPatchPercent = dwPos * 100 / dwFileBytes;
	}
	return TRUE;
}

static BOOL BnDownloadExtractLibrary(HANDLE hArchive, char *pszLibrary)
{
	HANDLE hFile;
	BYTE *pbLibrary;
	DWORD dwBytes;
	DWORD dwWritten;
	HANDLE hOut;
	BOOL fExtracted;

	fExtracted = FALSE;
	strcpy(pszLibrary, sgszVersionFile);
	hFile = NULL;
	if (strchr(pszLibrary, '.'))
		*strchr(pszLibrary, '.') = 0;
	strcat(pszLibrary, ".dll");
	if (StormSFileOpenFileEx(hArchive, pszLibrary, 0, &hFile))
	{
		dwBytes = StormSFileGetFileSize(hFile, 0);
#line 992
		pbLibrary = (BYTE *)StormSMemAlloc(dwBytes, D2_THIS_FILE, __LINE__, 0);
		if (pbLibrary)
		{
			StormSFileReadFile(hFile, pbLibrary, dwBytes, 0, 0);
			hOut = FogSafesockCreateFile(pszLibrary, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL, 0);
			if (hOut != INVALID_HANDLE_VALUE)
			{
				FogSafesockWriteFile(hOut, pbLibrary, dwBytes, &dwWritten, 0);
				FogSafesockCloseHandle(hOut);
				fExtracted = TRUE;
			}
#line 1018
			StormSMemFree(pbLibrary, D2_THIS_FILE, __LINE__, 0);
		}
	}
	if (hFile)
		StormSFileCloseFileRaw((DWORD)hFile, (DWORD)hFile);
	return fExtracted;
}

BOOL __fastcall BnDownloadCheckRevision(DWORD *pdwFirst, DWORD *pdwSecond, char *pszText)
{
	HANDLE hArchive;
	HMODULE hLibrary;
	PFNCHECKREVISION pfnCheckRevision;
	DWORD dwAuthentication;
	DWORD dwStart;
	BOOL fResult;

	if (pdwFirst)
		*pdwFirst = 0;
	if (pdwSecond)
		*pdwSecond = 0;
	if (pszText)
		*pszText = 0;
	BnConnectDebugPrint("<Running CHECK VERSION>");
	char szLibrary[260] = "";
	hArchive = NULL;
	fResult = FALSE;
	dwStart = GetTickCount();
	while (!sgszVersionFile || !*sgszVersionFile)
	{
		Sleep(10);
		if (GetTickCount() - dwStart > 20000)
			return TRUE;
	}
	if (!StormSFileOpenArchive(sgszVersionFile, 0, 0, &hArchive))
	{
		FogErrorManagerLog("Failed to open version dll %s!", sgszVersionFile);
		return FALSE;
	}
	StormSFileAuthenticateArchive(hArchive, &dwAuthentication);
	if (dwAuthentication && dwAuthentication < 5)
		BnConnectDebugPrint("<ERROR: Failed file archive authentication!>");
	else if (!BnDownloadExtractLibrary(hArchive, szLibrary))
		BnConnectDebugPrint("<ERROR: Failed to extract Versioning DLL!>");
	else
	{
		hLibrary = LoadLibraryA(szLibrary);
		if (hLibrary)
		{
			pfnCheckRevision = (PFNCHECKREVISION)GetProcAddress(hLibrary, "CheckRevision");
			if (!pfnCheckRevision)
				BnConnectDebugPrint("<ERROR: Failed to execute DLL!>");
			else
			{
				char szExe[260] = "";
				char szBnclient[260] = "";
				char szD2Client[260] = "";
				char *pszName;

				GetModuleFileNameA(GetModuleHandleA(NULL), szExe, sizeof(szExe));
				StormSStrCopy(szBnclient, szExe, 2147483647);
				pszName = strrchr(szBnclient, '\\') + 1;
				StormSStrCopy(pszName, "bnclient.dll", 2147483647);
				StormSStrCopy(szD2Client, szExe, 2147483647);
				pszName = strrchr(szD2Client, '\\') + 1;
				StormSStrCopy(pszName, "d2client.dll", 2147483647);
				fResult = pfnCheckRevision(szExe, szBnclient, szD2Client, sgszVersionText, pdwFirst, pdwSecond,
					pszText);
			}
			FreeLibrary(hLibrary);
		}
	}
	if (hArchive)
		StormSFileCloseArchiveRaw((DWORD)hArchive, (DWORD)hArchive);
	DeleteFileA(sgszVersionFile);
	if (szLibrary[0])
		DeleteFileA(szLibrary);
	if (!fResult)
		FogErrorManagerLog("checkrevision failed!");
	return fResult;
}

BOOL __fastcall BnDownloadApplyPatch(int *pnStatus)
{
	int nStatus;

	BnDownloadRunPatch(&nStatus);
	switch (nStatus)
	{
	case 0:
		return TRUE;
	case 1:
		return TRUE;
	case 2:
		*pnStatus = 1;
		break;
	}
	return FALSE;
}

static BOOL __fastcall BnDownloadRunPatch(int *upgradestatus)
{
	char szValue[900];
	STARTUPINFOA tStartup;
	PROCESS_INFORMATION tProcess;
	char *pszCommand;
	char *pszPatch;
	char *p;
	char *q;
	HANDLE hArchive;
	HANDLE hFile;
	DWORD dwBytes;
	BYTE *pbList;

	D2_OPAQUE(upgradestatus);
#line 1236
	D2ASSERT(upgradestatus);
	char szDir[260] = "";
	GetModuleFileNameA(NULL, szDir, sizeof(szDir));
	szDir[sizeof(szDir) - 1] = 0;
	p = szDir;
	if (*p)
	{
		while (1)
		{
			if (!StormSStrChr(p + 1, '\\'))
				break;
			p = StormSStrChr(p + 1, '\\');
			if (!*p)
				break;
		}
	}
	*p = 0;
	char szPatches[1024] = "";
	if (!StormSRegLoadData("Patch", "Patches", 2, szPatches, sizeof(szPatches) - 1, 0)
		|| !szPatches[0])
	{
		*upgradestatus = 0;
		return FALSE;
	}
	char szCommands[1024] = "";
	szPatches[sizeof(szPatches) - 2] = 0;
	szPatches[sizeof(szPatches) - 1] = 0;
	pszCommand = szCommands;
	for (pszPatch = szPatches; *pszPatch; pszPatch += StormSStrLen(pszPatch) + 1)
	{
		if (!StormSFileOpenArchive(pszPatch, 0, 0, &hArchive))
		{
			*upgradestatus = -1;
			return FALSE;
		}
		if (StormSFileOpenFileEx(hArchive, "Prepatch.lst", 0, &hFile))
		{
			dwBytes = StormSFileGetFileSize(hFile, 0);
#line 1298
			pbList = (BYTE *)StormSMemAlloc(dwBytes, D2_THIS_FILE, __LINE__, 0);
			if (StormSFileReadFile(hFile, pbList, dwBytes, 0, 0))
			{
				p = (char *)pbList;
				while ((DWORD)((BYTE *)p - pbList) < dwBytes)
				{
					char szLine[256] = "";

					for (q = szLine; (DWORD)((BYTE *)p - pbList) < dwBytes && q - szLine < 255 && *p
						&& *p != '\r' && *p != '\n'; )
					{
						*q++ = *p++;
						*q = 0;
					}
					while ((DWORD)((BYTE *)p - pbList) < dwBytes && (*p == '\r' || *p == '\n'))
						p++;
					char szFile[260] = "";
					char szPath[260] = "";
					if (StormSStrChr(szLine, ' '))
					{
						q = StormSStrChr(szLine, ' ');
						q++;
						while (*q == ' ')
							q++;
						StormSStrCopy(szFile, q, sizeof(szFile));
						wsprintfA(szPath, "%s\\%s", szDir, q);
					}
					if (!_strnicmp(szLine, "delete ", 7))
						DeleteFileA(szPath);
					else if (!_strnicmp(szLine, "extract ", 8))
						BnDownloadExtract(hArchive, szFile, szPath);
					else if (!_strnicmp(szLine, "execute ", 8))
						pszCommand += StormSStrCopy(pszCommand, szPath, 2147483647) + 1;
				}
			}
#line 1351
			StormSMemFree(pbList, D2_THIS_FILE, __LINE__, 0);
			StormSFileCloseFileRaw((DWORD)hFile, (DWORD)hFile);
		}
		StormSFileCloseArchiveRaw((DWORD)hArchive, (DWORD)hArchive);
	}
	sprintf(szValue, "%s\\Game.exe", szDir);
	StormSRegSaveString("Patch", "Launcher", 2, szValue);
	sprintf(szValue, "%s\\%s;%s\\%s", szDir, "d2data.mpq", szDir, "d2sfx.mpq");
	StormSRegSaveString("Patch", "SrcData", 2, szValue);
	sprintf(szValue, "%s\\%s", szDir, "patch_d2.mpq");
	StormSRegSaveString("Patch", "DstData", 2, szValue);
	if (pszCommand == szCommands)
	{
		wsprintfA(pszCommand, "%s\\bnupdate.exe", szDir);
		pszCommand += StormSStrLen(pszCommand) + 1;
	}
	*pszCommand = 0;
	for (p = szCommands; *p; p += StormSStrLen(p) + 1)
	{
		StormSMemZero(&tStartup, sizeof(tStartup));
		tStartup.cb = sizeof(tStartup);
		if (!CreateProcessA(NULL, p, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, szDir, &tStartup,
			&tProcess))
		{
			*upgradestatus = -1;
			return FALSE;
		}
		CloseHandle(tProcess.hThread);
		CloseHandle(tProcess.hProcess);
	}
	*upgradestatus = 2;
	return TRUE;
}

static void BnDownloadExtract(HANDLE hArchive, const char *pszFile, const char *pszPath)
{
	HANDLE hFile;
	HANDLE hOut;
	BYTE *pbData;
	DWORD dwBytes;
	DWORD dwWritten;

	if (!StormSFileOpenFileEx(hArchive, pszFile, 0, &hFile))
		return;
	dwBytes = StormSFileGetFileSize(hFile, 0);
#line 1442
	pbData = (BYTE *)StormSMemAlloc(dwBytes, D2_THIS_FILE, __LINE__, 0);
	if (StormSFileReadFile(hFile, pbData, dwBytes, 0, 0))
	{
		hOut = FogSafesockCreateFile(pszPath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (hOut != INVALID_HANDLE_VALUE)
		{
			FogSafesockWriteFile(hOut, pbData, dwBytes, &dwWritten, 0);
			FogSafesockCloseHandle(hOut);
		}
	}
#line 1457
	StormSMemFree(pbData, D2_THIS_FILE, __LINE__, 0);
	StormSFileCloseFileRaw((DWORD)hFile, (DWORD)hFile);
}

void __fastcall BnDownloadSetProgress(DWORD dwUnused, DWORD dwDone, DWORD dwTotal)
{
	sgdwPatchPercent = dwDone * 100 / dwTotal;
}

void __fastcall BnDownloadClearPatchList(void)
{
	sgtLock.Enter();
	sgabPatchList[0] = 0;
	sgabPatchList[1] = 0;
	sgdwPatchPercent = 0;
	sgtLock.Leave();
}

DWORD __fastcall BnDownloadGetProgress(void)
{
	DWORD dwProgress;

	dwProgress = 101;
	sgtLock.Enter();
	if (sgabPatchList[0] && sgabPatchList[1])
		dwProgress = sgdwPatchPercent;
	sgtLock.Leave();
	return dwProgress;
}

unsigned long __fastcall BnDownloadPatch(const char *pszList, DWORD dwLength)
{
	FILETIME tNone;
	FILETIME tCached;
	BYTE *pbData;
	DWORD dwBytes;
	BNDOWNLOAD *ptRequest;
	unsigned nThreadId;

	if (!FogSafesockStartup())
		return 0;
	sgdwPatchPercent = 0;
	sgtLock.Enter();
	StormSMemCopy(sgabPatchList, pszList, dwLength);
	sgtLock.Leave();
	StormSRegSaveData("Patch", "Patches", 130, pszList, dwLength);
	tNone.dwLowDateTime = 0;
	tNone.dwHighDateTime = 0;
	if (!pszList || !*pszList)
		return 0;
	if (CacheFind(pszList, &tCached, &pbData, &dwBytes))
	{
		if (!CompareFileTime(&tNone, &tCached))
		{
			BnDownloadWriteFile(pszList, pbData, dwBytes, &tNone);
			BnDownloadProcess(BNDL_PATCH, 2, 0, 0, &tNone, pszList, NULL, pbData, dwBytes);
			CacheFree(pbData, dwBytes);
			return 0;
		}
		CacheFree(pbData, dwBytes);
	}
#line 1720
	ptRequest = (BNDOWNLOAD *)StormSMemAlloc(sizeof(BNDOWNLOAD), D2_THIS_FILE, __LINE__, 0);
	if (!ptRequest)
		return 0;
	memset(ptRequest, 0, sizeof(BNDOWNLOAD));
	ptRequest->wLength = (WORD)(strlen(pszList) + 1 + offsetof(BNDOWNLOAD, szFile));
	ptRequest->field_02 = 0;
	ptRequest->field_03 = 1;
	ptRequest->dwPlatform = BN_PLATFORM;
	ptRequest->dwProduct = BN_PRODUCT;
	ptRequest->dwFirst = 0;
	ptRequest->dwSecond = 0;
	ptRequest->dwOffset = 0;
	ptRequest->dwKind = BNDL_PATCH;
	ptRequest->dwFlags = 2;
	ptRequest->tTime = tNone;
	StormSStrCopy(ptRequest->szFile, pszList, sizeof(ptRequest->szFile));
	StormSStrCopy(ptRequest->szText, "", sizeof(ptRequest->szText));
	InterlockedIncrement(&sglDownloads);
	return _beginthreadex(NULL, 0, DOWNLOADTOANY, ptRequest, 0, &nThreadId);
}

BOOL __fastcall BnDownloadSetVersionFile(const char *pszFile)
{
	StormSStrCopy(sgszVersionFile, pszFile, 2147483647);
	return TRUE;
}

BOOL __fastcall BnDownloadHasVersionFile(void)
{
	return sgszVersionFile[0] != 0;
}
