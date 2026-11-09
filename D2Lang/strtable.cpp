#define D2_THIS_FILE "C:\\D2\\Source\\D2Lang\\StrTable\\strtable.cpp"
#include <windows.h>
#include "D2Lang.h"
#include "strtable.h"
#include "Archive.h"
#include "D2Macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LANGUAGE_ENGLISH	0
#define NUM_LANGUAGES		12

static HD2ARCHIVE sghD2ArchiveData;
static HD2ARCHIVE sghD2ArchivePatch;
static STRTABLEHEADER *sghStringTable;
static Unicode **sgpUnicode;
static Unicode *sgpUnicodeText;
static int sgnRefCount;
static STRMISSING *sgptMissing;
static Unicode sgwSuffixThousands('K');
static Unicode sgwSuffixMillions('M');
static Unicode sgwSuffixBillions('B');
static Unicode sgwSuffixTrillions('T');
static BYTE sgbUseLoaded;
static int sgnUseLanguage;

const char *const sgpszStringTableFormat = "data\\local\\lng\\%s\\string.tbl";
const char *const sgpszSysMapFormat = "data\\local\\font\\%s\\default.map";
const char *const sgpszDefaultLanguage = "ENG";

const char *const sgpszD2DataArchive = "d2data.mpq";
const char *const sgpszPatchArchive = "patch_d2.mpq";
const char *const sgpszProbeFile = "DATA\\DUMMY";

static char sgszNotTranslated[] = " -not translated call ken";

static WORD sgcwCRCTable[256] =
{
	0, 4129, 8258, 12387, 16516, 20645, 24774, 28903,
	33032, 37161, 41290, 45419, 49548, 53677, 57806, 61935,
	4657, 528, 12915, 8786, 21173, 17044, 29431, 25302,
	37689, 33560, 45947, 41818, 54205, 50076, 62463, 58334,
	9314, 13379, 1056, 5121, 25830, 29895, 17572, 21637,
	42346, 46411, 34088, 38153, 58862, 62927, 50604, 54669,
	13907, 9842, 5649, 1584, 30423, 26358, 22165, 18100,
	46939, 42874, 38681, 34616, 63455, 59390, 55197, 51132,
	18628, 22757, 26758, 30887, 2112, 6241, 10242, 14371,
	51660, 55789, 59790, 63919, 35144, 39273, 43274, 47403,
	23285, 19156, 31415, 27286, 6769, 2640, 14899, 10770,
	56317, 52188, 64447, 60318, 39801, 35672, 47931, 43802,
	27814, 31879, 19684, 23749, 11298, 15363, 3168, 7233,
	60846, 64911, 52716, 56781, 44330, 48395, 36200, 40265,
	32407, 28342, 24277, 20212, 15891, 11826, 7761, 3696,
	65439, 61374, 57309, 53244, 48923, 44858, 40793, 36728,
	37256, 33193, 45514, 41451, 53516, 49453, 61774, 57711,
	4224, 161, 12482, 8419, 20484, 16421, 28742, 24679,
	33721, 37784, 41979, 46042, 49981, 54044, 58239, 62302,
	689, 4752, 8947, 13010, 16949, 21012, 25207, 29270,
	46570, 42443, 38312, 34185, 62830, 58703, 54572, 50445,
	13538, 9411, 5280, 1153, 29798, 25671, 21540, 17413,
	42971, 47098, 34713, 38840, 59231, 63358, 50973, 55100,
	9939, 14066, 1681, 5808, 26199, 30326, 17941, 22068,
	55628, 51565, 63758, 59695, 39368, 35305, 47498, 43435,
	22596, 18533, 30726, 26663, 6336, 2273, 14466, 10403,
	52093, 56156, 60223, 64286, 35833, 39896, 43963, 48026,
	19061, 23124, 27191, 31254, 2801, 6864, 10931, 14994,
	64814, 60687, 56684, 52557, 48554, 44427, 40424, 36297,
	31782, 27655, 23652, 19525, 15522, 11395, 7392, 3265,
	61215, 65342, 53085, 57212, 44955, 49082, 36825, 40952,
	28183, 32310, 20053, 24180, 11923, 16050, 3793, 7920
};

static LOCALE sgLocaleTable[NUM_LANGUAGES] =
{
	{ "ENU", "ENG", "ENG", "USA", 0 },
	{ "ESP", "ESP", "ESP", "ESP", 1 },
	{ "DEU", "DEU", "DEU", "DEU", 2 },
	{ "FRA", "FRA", "FRA", "FRA", 3 },
	{ "POR", "POR", "POR", "BRA", 4 },
	{ "ITA", "ITA", "ITA", "ITA", 5 },
	{ "JPN", "JPN", "ENG", "JPN", 6 },
	{ "KOR", "KOR", "ENG", "HAN", 7 },
	{ "CHI", "CHI", "ENG", "CHI", 8 },
	{ "CHI", "CHI", "ENG", "CHI", 9 },
	{ "POL", "POL", "POL", "POL", 10 },
	{ "RUS", "RUS", "RUS", "RUS", 11 },
};

Unicode * __fastcall StrAddMissing(const char *pszKey);

BOOL __cdecl StrOpenArchives(void)
{
#line 130
	D2ASSERT(!sghD2ArchiveData);
	sghD2ArchiveData = ArchiveOpen(gpszD2LangModuleName, sgpszD2DataArchive, "D2DATA", sgpszProbeFile, NULL);
	if (!sghD2ArchiveData)
		return FALSE;
	sghD2ArchivePatch =
		ArchiveOpen(gpszD2LangModuleName, sgpszPatchArchive, "PATCH_D2", sgpszProbeFile, NULL);
	if (!sghD2ArchivePatch)
	{
		ArchiveClose(sghD2ArchiveData);
		sghD2ArchiveData = NULL;
		return FALSE;
	}
	return TRUE;
}

void __cdecl StrCloseArchives(void)
{
	if (sghD2ArchiveData)
	{
		ArchiveClose(sghD2ArchiveData);
		sghD2ArchiveData = NULL;
	}
	if (sghD2ArchivePatch)
	{
		ArchiveClose(sghD2ArchivePatch);
		sghD2ArchivePatch = NULL;
	}
}

Unicode * __fastcall StrGetString(STRTABLEHEADER *hStringTable, Unicode **pUnicode, WORD usStr)
{
	STRTABLEHEADER *pHeader = hStringTable;
	WORD *lpDefine;
	STRTABLENODE *pNode;

	lpDefine = (WORD *)((BYTE *)hStringTable + sizeof(STRTABLEHEADER));
	pNode = (STRTABLENODE *)(lpDefine + pHeader->usNumElements);
#line 226
	D2ASSERT(usStr < pHeader->usNumElements);
#line 233
	D2ASSERT(lpDefine[usStr] < pHeader->iHashTableSize);
#line 237
	D2ASSERT((LPBYTE)(&pNode[lpDefine[usStr]]) < (LPBYTE)hStringTable + pHeader->dwFileLen);
#line 238
	D2ASSERT(pNode[lpDefine[usStr]].bUsed == TRUE);
#line 240
	D2ASSERT(pUnicode[lpDefine[usStr]] != NULL);
	return pUnicode[lpDefine[usStr]];
}

static __inline void StrFreeTable(void *pTable)
{

#line 264
	D2FREE2(pTable);
}

DWORD __stdcall StrHashString(const char *pszKey, DWORD dwTableSize)
{
	DWORD dwHash = 0;
	DWORD dwHigh;

	while (*pszKey)
	{
		dwHash = (dwHash << 4) + *pszKey;
		dwHigh = dwHash & 0xF0000000;
		if (dwHigh)
		{
			dwHash &= 268435455;
			dwHash ^= dwHigh >> 24;
		}
		pszKey++;
	}
	return dwHash % dwTableSize;
}

BOOL __fastcall StrNewTable(HD2ARCHIVE hArchive)
{
	char szLanguage[4];
	char szDirectory[32];
	char szFormat[260];
	char szPath[260];
	char szSysMap[260];
	STRTABLEHEADER *pHeader;
	STRTABLENODE *pNode;
	WORD usCRC;
	Unicode *pszCur;
	char *pszText;
	int nTotal;
	int nUnits;
	int i;

	if (sghStringTable)
	{
		sgnRefCount++;
		return TRUE;
	}
	StrGetLanguageName(szLanguage, 0);

	if (!stricmp(szLanguage, "JPN"))
		::strcpy(szDirectory, "JPN");
	else if (!stricmp(szLanguage, "KOR"))
		::strcpy(szDirectory, "KOR");
	else if (!stricmp(szLanguage, "POL"))
		::strcpy(szDirectory, "LATIN2");
	else if (!stricmp(szLanguage, "RUS"))
		::strcpy(szDirectory, "CYR");
	else
		::strcpy(szDirectory, "LATIN");
	sprintf(szSysMap, sgpszSysMapFormat, szDirectory);
	Unicode::loadSysMap(sghD2ArchiveData, szSysMap);
	::strncpy(szFormat, sgpszStringTableFormat, 259);
	sprintf(szPath, szFormat, szLanguage);
	pHeader = (STRTABLEHEADER *)
#line 169
		ArchiveAllocBufferAndRead(sghD2ArchiveData, szPath, NULL, D2_THIS_FILE, __LINE__);

	{
		DWORD dwIndexEnd = pHeader->dwFileLen;
		DWORD dwIndexStart = pHeader->dwIndexStart;
		const BYTE *pbData = (const BYTE *)pHeader;
		WORD *pusCRC = &usCRC;

#line 814
		D2ASSERT(dwIndexStart < dwIndexEnd);
		DWORD i = dwIndexStart;
		*pusCRC = 65535;
		do
		{
			*pusCRC = (WORD)(sgcwCRCTable[(*pusCRC >> 8) ^ pbData[i]] ^ (*pusCRC << 8));
			i++;
		} while (i < dwIndexEnd);
	}
#line 183
	D2ASSERT(usCRC == pHeader->usCRC);
	pNode = (STRTABLENODE *)((WORD *)((BYTE *)pHeader + sizeof(STRTABLEHEADER)) + pHeader->usNumElements);
	for (i = 0; i < pHeader->iHashTableSize; i++, pNode++)
	{
#line 201
		D2ASSERT(pNode->dwKeyOffset < pHeader->dwFileLen);
#line 202
		D2ASSERT(pNode->dwStringOffset < pHeader->dwFileLen);
	}
	sghStringTable = pHeader;
	if (!pHeader)
		return FALSE;

#line 363
	sgpUnicode = (Unicode **)D2ALLOC2(pHeader->iHashTableSize * sizeof(Unicode *));
	memset(sgpUnicode, 0, pHeader->iHashTableSize * sizeof(Unicode *));
	sgnRefCount++;
	nTotal = 0;
	pNode = (STRTABLENODE *)((WORD *)((BYTE *)sghStringTable + sizeof(STRTABLEHEADER)) +
		pHeader->usNumElements);
	for (i = 0; i < pHeader->iHashTableSize; i++)
	{
		pszText = (char *)sghStringTable + pNode[i].dwStringOffset;
		nTotal += Unicode::unicodeWidth(pszText, pNode[i].wStringLength) + 1;
	}
#line 386
	sgpUnicodeText = (Unicode *)D2ALLOC2(nTotal * sizeof(Unicode));
	pszCur = sgpUnicodeText;
	for (i = 0; i < pHeader->iHashTableSize; i++)
	{
		sgpUnicode[i] = pszCur;
		pszText = (char *)sghStringTable + pNode[i].dwStringOffset;
		nUnits = Unicode::unicodeWidth(pszText, pNode[i].wStringLength) + 1;
		Unicode::toUnicode(sgpUnicode[i], pszText, nUnits);
		pszCur += nUnits;
	}

	{
		char szChar[8];
		Unicode *pszSuffix;

#line 439
		D2ASSERT(sghStringTable != NULL);
		pszSuffix = StrGetString(sghStringTable, sgpUnicode, 5328);
		Unicode::unicode2Win(szChar, pszSuffix, 5);
		if (::strlen(szChar) > 1)
			sgwSuffixThousands.wUnit = 'K';
		else
			sgwSuffixThousands.wUnit = (WORD)(signed char)szChar[0];
#line 439
		D2ASSERT(sghStringTable != NULL);
		pszSuffix = StrGetString(sghStringTable, sgpUnicode, 5329);
		Unicode::unicode2Win(szChar, pszSuffix, 5);
		if (::strlen(szChar) > 1)
			sgwSuffixMillions.wUnit = 'M';
		else
			sgwSuffixMillions.wUnit = (WORD)(signed char)szChar[0];
#line 439
		D2ASSERT(sghStringTable != NULL);
		pszSuffix = StrGetString(sghStringTable, sgpUnicode, 5330);
		Unicode::unicode2Win(szChar, pszSuffix, 5);
		if (::strlen(szChar) > 1)
			sgwSuffixBillions.wUnit = 'B';
		else
			sgwSuffixBillions.wUnit = (WORD)(signed char)szChar[0];
#line 439
		D2ASSERT(sghStringTable != NULL);
		pszSuffix = StrGetString(sghStringTable, sgpUnicode, 5331);
		Unicode::unicode2Win(szChar, pszSuffix, 5);
		if (::strlen(szChar) > 1)
			sgwSuffixTrillions.wUnit = 'T';
		else
			sgwSuffixTrillions.wUnit = (WORD)(signed char)szChar[0];
	}
	return TRUE;
}

Unicode * __fastcall StrGetStringByIndex(WORD wStringId)
{
#line 439
	D2ASSERT(sghStringTable != NULL);
	return StrGetString(sghStringTable, sgpUnicode, wStringId);
}

Unicode * __fastcall StrAddMissing(const char *pszKey)
{
	STRMISSING *ptMissing;
	char *pszText;
	int nLength;
	int nUnits;

	for (ptMissing = sgptMissing; ptMissing; ptMissing = ptMissing->ptNext)
	{
		if (!::strcmp(pszKey, ptMissing->pszKey))
			return ptMissing->pszText;
	}
#line 458
	ptMissing = (STRMISSING *)D2ALLOC2(sizeof(STRMISSING));
	ptMissing->ptNext = sgptMissing;
	sgptMissing = ptMissing;
#line 469
	ptMissing->pszKey = (char *)D2ALLOC2(::strlen(pszKey) + 1);
	::strcpy(ptMissing->pszKey, pszKey);
	nLength = ::strlen(sgszNotTranslated) + ::strlen(pszKey);
#line 477
	pszText = (char *)D2ALLOC2(nLength + 1);
	::strcpy(pszText, pszKey);
	::strcat(pszText, sgszNotTranslated);
	nUnits = Unicode::unicodeWidth(pszText, nLength);
#line 482
	ptMissing->pszText = (Unicode *)D2ALLOC2(nUnits * sizeof(Unicode) + sizeof(Unicode));
	Unicode::toUnicode(ptMissing->pszText, pszText, nUnits);
#line 488
	D2FREE2(pszText);
	return ptMissing->pszText;
}

WORD __stdcall StrGetStringIndexByKey(const char *pszKey, Unicode **ppString)
{
	STRTABLENODE *pNodes;
	DWORD iHash;
	DWORD nTries = 0;

#line 502
	D2ASSERT(sghStringTable != NULL);
	if (!pszKey)
	{
#line 507
		D2ASSERT(0);
	}
	if (!*pszKey)
	{
		static Unicode sgtEmpty;

		*ppString = &sgtEmpty;
		return 0;
	}
	pNodes = (STRTABLENODE *)((WORD *)((BYTE *)sghStringTable + sizeof(STRTABLEHEADER)) +
		sghStringTable->usNumElements);

	{
		const char *pszHash = pszKey;
		DWORD dwHash = 0;
		DWORD dwHigh;

		while (*pszHash)
		{
			dwHash = (dwHash << 4) + *pszHash;
			dwHigh = dwHash & 0xF0000000;
			if (dwHigh)
			{
				dwHash &= 268435455;
				dwHash ^= dwHigh >> 24;
			}
			pszHash++;
		}
		iHash = dwHash % sghStringTable->iHashTableSize;
	}
	while (nTries < sghStringTable->dwMaxTries)
	{
		nTries++;
		if (pNodes[iHash].bUsed == 1 &&
			!::strcmp((char *)sghStringTable + pNodes[iHash].dwKeyOffset, pszKey))
		{
#line 534
			D2ASSERT(sgpUnicode[iHash] != NULL);
			*ppString = sgpUnicode[iHash];
			return pNodes[iHash].wStringIndex;
		}
		iHash = (int)(iHash + 1) % sghStringTable->iHashTableSize;
	}
	*ppString = StrAddMissing(pszKey);
	return 0;
}

Unicode * __fastcall StrGetStringByKey(const char *pszKey)
{
	Unicode *pszString = NULL;

	StrGetStringIndexByKey(pszKey, &pszString);
	return pszString;
}

void __fastcall StrDeleteTable(void)
{
	STRMISSING *ptMissing;
	STRMISSING *ptNext;

	if (!sghStringTable)
		return;
	if (--sgnRefCount)
		return;
	if (sgpUnicode)
	{
#line 638
		D2FREE2(sgpUnicodeText);
		sgpUnicodeText = NULL;
#line 642
		D2FREE2(sgpUnicode);
		sgpUnicode = NULL;
	}
	StrFreeTable(sghStringTable);
	sghStringTable = NULL;
	for (ptMissing = sgptMissing; ptMissing; ptMissing = ptNext)
	{
		ptNext = ptMissing->ptNext;
#line 699
		D2FREE2(ptMissing->pszText);
#line 700
		D2FREE2(ptMissing->pszKey);
#line 701
		D2FREE2(ptMissing);
	}
	sgptMissing = NULL;
	Unicode::unloadSysMap();
}

void __cdecl StrCRCInit(WORD *pusCRC)
{
	*pusCRC = 65535;
}

void __cdecl StrCRCUpdate(WORD *pusCRC, BYTE bData)
{
	*pusCRC = (WORD)(sgcwCRCTable[(*pusCRC >> 8) ^ bData] ^ (*pusCRC << 8));
}

WORD __fastcall StrCRCResult(WORD usCRC)
{
	return usCRC;
}

void __stdcall StrComputeCRC(WORD *pusCRC, const BYTE *pbData, DWORD dwIndexStart, DWORD dwIndexEnd)
{
#line 814
	D2ASSERT(dwIndexStart < dwIndexEnd);
	DWORD i = dwIndexStart;
	*pusCRC = 65535;
	do
	{
		*pusCRC = (WORD)(sgcwCRCTable[(*pusCRC >> 8) ^ pbData[i]] ^ (*pusCRC << 8));
		i++;
	} while (i < dwIndexEnd);
}

void __fastcall StrLocaleToDirectory(char *pszLanguage)
{
	int i;

	for (i = 0; i < NUM_LANGUAGES; i++)
	{
		if (!stricmp(pszLanguage, sgLocaleTable[i].pszSystemLanguage))
		{
			::strcpy(pszLanguage, sgLocaleTable[i].pszDirectory);
			return;
		}
	}
	::strcpy(pszLanguage, sgLocaleTable[0].pszDirectory);
}

int __fastcall StrLocaleToLanguage(const char *pszLanguage)
{
	int i;

	for (i = 0; i < NUM_LANGUAGES; i++)
	{
		if (!stricmp(pszLanguage, sgLocaleTable[i].pszSystemLanguage))
			return sgLocaleTable[i].eLanguage;
	}
	return 0;
}

int __cdecl StrGetUseLanguage(void)
{
	BYTE *pbUse;

	if (!sgbUseLoaded)
	{
		sgbUseLoaded = 1;
		pbUse = (BYTE *)
#line 909
			ArchiveAllocBufferAndRead(sghD2ArchiveData, "data\\local\\use", NULL, D2_THIS_FILE, __LINE__);
		sgnUseLanguage = *pbUse;
#line 911
		D2FREE2(pbUse);
		if (sgnUseLanguage >= NUM_LANGUAGES)
			sgnUseLanguage = 0;
	}
	return sgnUseLanguage;
}

int __cdecl StrGetSystemLanguage(void)
{
	HKEY hKey;
	DWORD dwSize;
	char szIntl[4];
	DWORD dwType;
	OSVERSIONINFOA tVersion;
	char szLanguage[256];
	int i;

	tVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	if (!GetVersionExA(&tVersion))
		return 0;
	if (tVersion.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		GetProfileStringA("Intl", "sLanguage", sgpszDefaultLanguage, szIntl, 4);
		for (i = 0; i < NUM_LANGUAGES; i++)
		{
			if (!stricmp(szIntl, sgLocaleTable[i].pszSystemLanguage))
				return sgLocaleTable[i].eLanguage;
		}
		return 0;
	}
	else if (tVersion.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (RegOpenKeyExA(HKEY_CURRENT_USER, "Control Panel\\International", 0, KEY_READ, &hKey))
			return 0;
		if (RegQueryValueExA(hKey, "sLanguage", NULL, &dwType, (BYTE *)szLanguage, &dwSize))
			return 0;
		if (!::strlen(szLanguage) || ::strlen(szLanguage) >= 4)
			return 0;
		RegCloseKey(hKey);
		for (i = 0; i < NUM_LANGUAGES; i++)
		{
			if (!stricmp(szLanguage, sgLocaleTable[i].pszSystemLanguage))
				return sgLocaleTable[i].eLanguage;
		}
		return 0;
	}
	return 0;
}

void __stdcall StrGetLanguageName(char *pszDst, int eField)
{
	int nLanguage;
	BYTE *pbUse;
	int i;

	if (!sgbUseLoaded)
	{
		sgbUseLoaded = 1;
		pbUse = (BYTE *)
#line 909
			ArchiveAllocBufferAndRead(sghD2ArchiveData, "data\\local\\use", NULL, D2_THIS_FILE, __LINE__);
		sgnUseLanguage = *pbUse;
#line 911
		D2FREE2(pbUse);
		if (sgnUseLanguage >= NUM_LANGUAGES)
			sgnUseLanguage = 0;
	}
	nLanguage = sgnUseLanguage;
#line 990
	D2ASSERT(sgLocaleTable[0].eLanguage == LANGUAGE_ENGLISH);
	i = 0;
	if (nLanguage < NUM_LANGUAGES)
	{
		for (i = 0; i < NUM_LANGUAGES; i++)
		{
			if (sgLocaleTable[i].eLanguage == nLanguage)
				break;
		}
		if (i >= NUM_LANGUAGES)
			i = 0;
	}
	switch (eField)
	{
	case 1:
		::strcpy(pszDst, sgLocaleTable[i].pszFallbackLanguage);
		break;
	case 2:
		::strcpy(pszDst, sgLocaleTable[i].pszRegion);
		break;
	default:
		::strcpy(pszDst, sgLocaleTable[i].pszDirectory);
		break;
	}
}

void __stdcall StrFormatInt(Unicode *pszDst, int nValue, int nMaxLength)
{
	char szNumber[64];
	Unicode *pszCur;
	Unicode *pszOut;
	int nGroup = 0;
	WORD wChar;
	int nLen;
	int i;

#line 1029
	D2ASSERT(nMaxLength > 2);
	pszCur = pszDst;
	if (nValue < 0)
	{
		pszCur->wUnit = '-';
		pszCur++;
		nMaxLength--;
		nValue = -nValue;
	}
	itoa(nValue, szNumber, 10);
	nLen = ::strlen(szNumber);
	if (nLen)
	{
		i = (nLen - 1) / 3 + nLen;
		if (i >= 1 && i < --nMaxLength)
		{
			pszOut = pszCur + i;
			pszOut->wUnit = 0;
			while (nLen)
			{
#line 1055
				D2ASSERT(i);
				wChar = (signed char)szNumber[--nLen];
				i--;
				pszOut--;
				pszOut->wUnit = wChar;
				if (++nGroup >= 3)
				{
					nGroup = 0;
					i--;
					(--pszOut)->wUnit = ',';
				}
			}
			return;
		}
	}

	*(DWORD *)&pszDst = 0;
	Unicode::toUnicode((Unicode *)&pszDst, "*", 2);
	Unicode::strcpy(pszCur, (Unicode *)&pszDst);
}

void __stdcall StrFormatUnsigned(Unicode *pszDst, unsigned int nValue, int nMaxLength)
{
	char szNumber[64];
	Unicode *pszOut;
	int nGroup = 0;
	WORD wChar;
	int nLen;
	int i;

#line 1074
	D2ASSERT(nMaxLength > 2);
	_ultoa(nValue, szNumber, 10);
	nLen = ::strlen(szNumber);
	if (nLen)
	{
		i = (nLen - 1) / 3 + nLen;
		if (i >= 1 && i < --nMaxLength)
		{
			pszOut = pszDst + i;
			pszOut->wUnit = 0;
			while (nLen)
			{
#line 1093
				D2ASSERT(i);
				wChar = (signed char)szNumber[--nLen];
				i--;
				pszOut--;
				pszOut->wUnit = wChar;
				if (++nGroup >= 3)
				{
					nGroup = 0;
					i--;
					(--pszOut)->wUnit = ',';
				}
			}
			return;
		}
	}

	*(DWORD *)(&nValue + 1) = 0;
	Unicode::toUnicode((Unicode *)(&nValue + 1), "*", 2);
	Unicode::strcpy(pszDst, (Unicode *)(&nValue + 1));
}

void __stdcall StrFormatInt64(Unicode *pszDst, __int64 nValue, int nMaxLength)
{
	char szNumber[64];
	Unicode *pszCur;
	Unicode *pszOut;
	int nGroup = 0;
	WORD wChar;
	int nLen;
	int i;

#line 1112
	D2ASSERT(nMaxLength > 2);
	if (nValue < 0)
	{
		pszCur = pszDst;
		pszCur->wUnit = '-';
		pszCur++;
		nMaxLength--;
		nValue = -nValue;
	}
	else
		pszCur = pszDst;
	_i64toa(nValue, szNumber, 10);
	nLen = ::strlen(szNumber);
	if (nLen)
	{
		i = (nLen - 1) / 3 + nLen;
		if (i >= 1 && i < --nMaxLength)
		{
			pszOut = pszCur + i;
			pszOut->wUnit = 0;
			while (nLen)
			{
#line 1138
				D2ASSERT(i);
				wChar = (signed char)szNumber[--nLen];
				i--;
				pszOut--;
				pszOut->wUnit = wChar;
				if (++nGroup >= 3)
				{
					nGroup = 0;
					if (i > 0)
					{
						i--;
						(--pszOut)->wUnit = ',';
					}
				}
			}
			return;
		}
	}

	*(DWORD *)&pszDst = 0;
	Unicode::toUnicode((Unicode *)&pszDst, "*", 2);
	Unicode::strcpy(pszCur, (Unicode *)&pszDst);
}

void __stdcall StrFormatUInt64Suffix(Unicode *pszDst, unsigned __int64 nValue, int nMaxLength)
{
	char szNumber[64];
	DWORD dwStar;
	Unicode *pszOut;
	int nGroup = 0;
	WORD wChar;
	int nSuffix;
	int nLen;
	int i;

#line 1162
	D2ASSERT(nMaxLength > 2);
	_ui64toa(nValue, szNumber, 10);
	nLen = ::strlen(szNumber);
	if (nLen)
	{

		nSuffix = (nLen - 1) / 3 - 1;
		if (nSuffix >= 4)
			nSuffix = 4;
		nLen -= 3 * nSuffix;
		i = (nLen - 1) / 3 + nLen;
		nMaxLength -= 2;
		if (i >= 1 && i < nMaxLength)
		{
			pszDst[i + 1].wUnit = 0;
			switch (nSuffix)
			{
			case 0:
				pszDst[i].wUnit = 0;
				break;
			case 1:
				pszDst[i].wUnit = sgwSuffixThousands.wUnit;
				break;
			case 2:
				pszDst[i].wUnit = sgwSuffixMillions.wUnit;
				break;
			case 3:
				pszDst[i].wUnit = sgwSuffixBillions.wUnit;
				break;
			case 4:
				pszDst[i].wUnit = sgwSuffixTrillions.wUnit;
				break;
			}
			if (!nLen)
				return;
			pszOut = pszDst + i;
			while (nLen)
			{
#line 1211
				D2ASSERT(i);
				wChar = (signed char)szNumber[--nLen];
				i--;
				pszOut--;
				pszOut->wUnit = wChar;
				if (++nGroup >= 3)
				{
					nGroup = 0;
					if (i > 0)
					{
						i--;
						(--pszOut)->wUnit = ',';
					}
				}
			}
			return;
		}
	}
	dwStar = 0;
	Unicode::toUnicode((Unicode *)&dwStar, "*", 2);
	Unicode::strcpy(pszDst, (Unicode *)&dwStar);
}
