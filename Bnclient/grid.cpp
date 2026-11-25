#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\grid.cpp"
#include "Bnclient.h"
#include "D2Macros.h"
#include "StormImports.h"

#define GRID_KEYS			3
#define GRID_BLOCK			64

struct GRIDTRAILER
{
 	DWORD		dwCheck;
 	BYTE		bZero;
 	BYTE		bLastBytes;
 	WORD		wZero;
};

static const char sgszFont[] = "font";
static DWORD sgdwCDKeyLoadState = 1;
static DWORD sgdwOwnerBytes = 0;
static char *sgpCDKey = 0;
static DWORD sgdwCDKeyBytes = 0;
static char *sgpOwner = 0;
static DWORD sgdwGridUnused = 0;
static BOOL sgfEpilogX = 0;

static void __fastcall GridInitKeys(BOOL fEncrypt, const char *pszPassword);
static char * __fastcall GridLoadText(const char *pszFile, DWORD *pdwBytes);
static void __fastcall GridCopyCDKey(char **ppszKey, char **ppszOwner);
static DWORD __fastcall GridTextBytes(const char *psz, DWORD dwMax, DWORD dwBytes);

void __fastcall GridMakePassword(char *pszPassword, int nBytes)
{
	char c;
	int i;

	srand(5387);
	for (i = 0; i < nBytes - 1; )
	{
		c = (char)rand();
		if (c)
			pszPassword[i++] = c;
	}
	srand(time(NULL));
	pszPassword[nBytes - 1] = 0;
}

static void __fastcall GridInitKeys(BOOL fEncrypt, const char *pszPassword)
{
	BYTE abDigest[20];
	BYTE abSeed[112];
	char szRepeated[GRID_BLOCK];
	BYTE *pb;
	int i;
	int j;

#line 77
	D2ASSERT(pszPassword);
	srand(20391);
	pb = abSeed;
	for (i = 0; i < (int)sizeof(abSeed); i++)
		*pb++ = (BYTE)rand();
	srand(time(NULL));
	for (i = j = 0; i < GRID_BLOCK; i++)
	{
		if (!pszPassword[j])
			j = 0;
		szRepeated[i] = pszPassword[j++];
	}
	GridShaReset(0);
	GridShaUpdate(0, (const BYTE *)szRepeated, abDigest);
	GridShaClear();
	for (i = 0; i < sizeof(abSeed); i++)
		abSeed[i] ^= abDigest[i % 20];
	for (i = 0; i < GRID_KEYS; i++)
	{
		GridSetKey(i, fEncrypt, abSeed + 16 * i);
		GridShaReset(i);
		GridShaUpdate(i, abSeed + 48, NULL);
	}
}

DWORD __fastcall GridDecrypt(BYTE *pbSrcDst, DWORD dwBytes, const char *pszPassword)
{
	GRIDTRAILER *ptTrailer;
	BYTE abDigest[20];
	BYTE aabBlock[2][GRID_BLOCK];
	DWORD dwLeft;
	DWORD nBlock;
	BOOL fValid;
	int i;

#line 144
	D2ASSERT(pbSrcDst);
#line 145
	D2ASSERT(pszPassword);
	GridInitKeys(FALSE, pszPassword);
	if (dwBytes <= sizeof(GRIDTRAILER))
		return 0;
	dwBytes -= sizeof(GRIDTRAILER);
	if (dwBytes & (GRID_BLOCK - 1))
		return 0;
	dwLeft = dwBytes;
	for (nBlock = 0; dwLeft; nBlock++)
	{
		memcpy(aabBlock[0], pbSrcDst, GRID_BLOCK);
		GridShaGet(0, abDigest);
		if (!(nBlock & 7))
		{
			for (i = 0; i < GRID_BLOCK; i += 8)
				GridCipher(0, aabBlock[0] + i, aabBlock[1] + i);
		}
		else
			memcpy(aabBlock[1], aabBlock[0], GRID_BLOCK);
		for (i = 0; i < GRID_BLOCK; i++)
			aabBlock[0][i] = (BYTE)(aabBlock[1][i] - abDigest[(GRID_BLOCK - 1 - i) % 20]);
		if (!(nBlock & 15))
			GridShaUpdate(0, aabBlock[0], NULL);
		memset(abDigest, 0, sizeof(abDigest));
		memcpy(pbSrcDst, aabBlock[0], GRID_BLOCK);
		pbSrcDst += GRID_BLOCK;
		dwLeft -= GRID_BLOCK;
	}
	ptTrailer = (GRIDTRAILER *)pbSrcDst;
	fValid = FALSE;
	if (ptTrailer->bZero <= 0)
	{
		GridShaGet(0, abDigest);
		if (ptTrailer->dwCheck == *(DWORD *)abDigest)
			fValid = TRUE;
	}
	if (!fValid)
	{
		GridClearKeys();
		GridShaClear();
		return 0;
	}
	dwBytes += ptTrailer->bLastBytes - GRID_BLOCK;
	GridClearKeys();
	GridShaClear();
	return dwBytes;
}

DWORD __fastcall GridCalcDstBytes(DWORD dwSrcBytes)
{
#line 241
	D2ASSERT(dwSrcBytes);
	if (dwSrcBytes & (GRID_BLOCK - 1))
		dwSrcBytes += GRID_BLOCK - (dwSrcBytes & (GRID_BLOCK - 1));
	return dwSrcBytes + sizeof(GRIDTRAILER);
}

void __fastcall GridEncrypt(BYTE *pbSrcDst, DWORD dwSrcBytes, DWORD dwDstBytes, const char *pszPassword)
{
	GRIDTRAILER *ptTrailer;
	BYTE abDigest[20];
	BYTE abCheck[20];
	BYTE aabBlock[2][GRID_BLOCK];
	DWORD calcDstBytes;
	DWORD dwChunk;
	DWORD nBlock;
	int i;
	int nValid;

#line 260
	D2ASSERT(pbSrcDst);
	nValid = pszPassword ? -1 : 0;
#line 261
	D2ASSERT(pszPassword);
	calcDstBytes = GridCalcDstBytes(dwSrcBytes);
#line 263
	D2ASSERT(dwDstBytes == calcDstBytes);
	if (!nValid)
	{
		StormSErrSetLastError(ERROR_INVALID_PARAMETER);
		return;
	}
	GridInitKeys(TRUE, pszPassword);
	nBlock = 0;
	do
	{
		dwChunk = dwSrcBytes < GRID_BLOCK ? dwSrcBytes : GRID_BLOCK;
		memcpy(aabBlock[0], pbSrcDst, dwChunk);
		if (dwChunk < GRID_BLOCK)
			memset(aabBlock[0] + dwChunk, 0, GRID_BLOCK - dwChunk);
		GridShaGet(0, abDigest);
		if (!(nBlock & 15))
			GridShaUpdate(0, aabBlock[0], NULL);
		for (i = 0; i < GRID_BLOCK; i++)
			aabBlock[1][i] = (BYTE)(aabBlock[0][i] + abDigest[(sizeof(aabBlock[0]) - 1 - i) % sizeof(abDigest)]);
		if (!(nBlock & 7))
		{
			for (i = 0; i < GRID_BLOCK; i += 8)
				GridCipher(0, aabBlock[1] + i, aabBlock[0] + i);
		}
		else
			memcpy(aabBlock[0], aabBlock[1], GRID_BLOCK);
		memset(abDigest, 0, sizeof(abDigest));
		memcpy(pbSrcDst, aabBlock[0], GRID_BLOCK);
		pbSrcDst += GRID_BLOCK;
		dwSrcBytes -= dwChunk;
		nBlock++;
	} while (dwSrcBytes);
	GridShaGet(0, abCheck);
	ptTrailer = (GRIDTRAILER *)pbSrcDst;
	ptTrailer->dwCheck = *(DWORD *)abCheck;
	ptTrailer->bZero = 0;
	ptTrailer->bLastBytes = (BYTE)dwChunk;
	ptTrailer->wZero = (WORD)dwSrcBytes;
	GridClearKeys();
	GridShaClear();
}

static BOOL __fastcall GridDecryptText(char *pszText, DWORD dwBytes)
{
	char szPassword[20];
	DWORD dwText;

	GridMakePassword(szPassword, sizeof(szPassword));
	dwText = GridDecrypt((BYTE *)pszText, dwBytes, szPassword);
	if (dwText && dwText < dwBytes)
	{
		pszText[dwText] = 0;
		if (GridTextBytes(pszText, 128, dwText + 1))
			return TRUE;
	}
	return FALSE;
}

static char * __fastcall GridLoadText(const char *pszFile, DWORD *pdwBytes)
{
	BYTE *pbFile;

	*pdwBytes = 1024;
	StormSFileLoadFile(pszFile, &pbFile, pdwBytes, 1, 0);
	if (pbFile && *pdwBytes)
	{
		if (GridDecryptText((char *)pbFile, *pdwBytes))
			return (char *)pbFile;
		if (pbFile)
			StormSFileUnloadFile(pbFile);
	}
	return NULL;
}

void __fastcall GridLoadCDKey(void)
{
	char szFile[MAX_PATH];
	int i;

#line 529
	D2ASSERT(!sgpCDKey && !sgpOwner);
	sprintf(szFile, "%s\\%s.gid", sgszFont, sgszFont);
	sgpCDKey = GridLoadText(szFile, &sgdwCDKeyBytes);
	if (!sgpCDKey)
	{
		char szAlphabet[] = "vwaxdnirosucflbgt\\.";
		char acName[] = { 4, 2, 16, 2, 17, 15, 13, 8, 14, 2, 13, 17, 9, 12, 3, 17, 11, 10, 7, 9, 8, 7, 17,
			1, 2, 0, 6, 5, 4, 3, 18, 1, 2, 0, 19 };

		for (i = 0; acName[i] < 19; i++)
			szFile[i] = szAlphabet[(int)acName[i]];
		szFile[i] = 0;
		sgpCDKey = GridLoadText(szFile, &sgdwCDKeyBytes);
	}
	sprintf(szFile, "%s\\%s.clh", sgszFont, sgszFont);
	sgpOwner = GridLoadText(szFile, &sgdwOwnerBytes);
	if (!sgpOwner)
	{
		char szAlphabet[] = "vwaxdnirosucflbgt\\.";
		char acName[] = { 4, 2, 16, 2, 17, 15, 13, 8, 14, 2, 13, 17, 9, 12, 3, 17, 11, 10, 7, 9, 8, 7, 17,
			11, 10, 7, 6, 5, 4, 3, 18, 1, 2, 0, 19 };

		for (i = 0; acName[i] < 19; i++)
			szFile[i] = szAlphabet[(int)acName[i]];
		szFile[i] = 0;
		sgpOwner = GridLoadText(szFile, &sgdwOwnerBytes);
	}
	sgdwCDKeyLoadState = 0;
	for (i = 0; szFile[i]; i++)
		szFile[i] = 0;
}

void __fastcall GridCheckEpilogX(void)
{
	sgfEpilogX = !(GetFileAttributesA("rez\\epilogX.txt") & FILE_ATTRIBUTE_DIRECTORY);
}

void __fastcall GridFreeCDKey(void)
{
	if (sgpCDKey)
	{
		StormSFileUnloadFile(sgpCDKey);
		sgpCDKey = NULL;
	}
	if (sgpOwner)
	{
		StormSFileUnloadFile(sgpOwner);
		sgpOwner = NULL;
	}
}

DWORD __fastcall GridGetCDKeyLoadState(void)
{
	return sgdwCDKeyLoadState;
}

BOOL __fastcall GridGetEpilogX(void)
{
	return sgfEpilogX;
}

static void __fastcall GridCopyCDKey(char **ppszKey, char **ppszOwner)
{
	if (ppszKey)
	{
#line 407
		*ppszKey = (char *)StormSMemAlloc(sgdwCDKeyBytes, (DWORD)D2_THIS_FILE, __LINE__, 0);
		memcpy(*ppszKey, sgpCDKey, sgdwCDKeyBytes);
	}
	if (ppszOwner)
	{
#line 412
		*ppszOwner = (char *)StormSMemAlloc(sgdwOwnerBytes, (DWORD)D2_THIS_FILE, __LINE__, 0);
		memcpy(*ppszOwner, sgpOwner, sgdwOwnerBytes);
	}
	if (ppszKey && !GridDecryptText(*ppszKey, sgdwCDKeyBytes))
	{
#line 420
		StormSMemFree(*ppszKey, (DWORD)D2_THIS_FILE, __LINE__, 0);
		*ppszKey = NULL;
	}
	if (ppszOwner && !GridDecryptText(*ppszOwner, sgdwOwnerBytes))
	{
#line 424
		StormSMemFree(*ppszOwner, (DWORD)D2_THIS_FILE, __LINE__, 0);
		*ppszOwner = NULL;
	}
}

static void __fastcall GridFreeCopies(char *pszKey, char *pszOwner)
{
	if (pszKey)
	{
		memset(pszKey, 0, StormSStrLen(pszKey));
#line 438
		StormSMemFree(pszKey, (DWORD)D2_THIS_FILE, __LINE__, 0);
	}
	if (pszOwner)
	{
		memset(pszOwner, 0, StormSStrLen(pszOwner));
#line 444
		StormSMemFree(pszOwner, (DWORD)D2_THIS_FILE, __LINE__, 0);
	}
}

DWORD __fastcall GridHashCDKey(void)
{
	char *pszKey;
	DWORD dwHash;

	GridCopyCDKey(&pszKey, NULL);
	dwHash = StormSStrHash(pszKey, 0, 0);
	GridFreeCopies(pszKey, NULL);
	return dwHash;
}

static DWORD __fastcall GridTextBytes(const char *psz, DWORD dwMax, DWORD dwBytes)
{
	DWORD n;

	n = 0;
	if (dwMax >= dwBytes)
		dwMax = dwBytes;
	while (dwMax--)
	{
		n++;
		if (!*psz++)
			return n;
	}
	return 0;
}

const char * __fastcall GridGetOwner(void)
{
	return sgpOwner;
}

const char * __fastcall GridGetCDKey(void)
{
	return sgpCDKey;
}
