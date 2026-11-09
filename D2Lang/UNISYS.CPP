#define D2_THIS_FILE "C:\\D2\\Source\\D2Lang\\Unicode\\UNISYS.CPP"
#include "D2Lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Archive.h"
#include "D2Macros.h"
#include "StormImports.h"

static inline void __cdecl operator delete[](void *pMemory)
{
	if (pMemory)
		StormSMemFree(pMemory, "delete[]", -1, 0);
}

static inline void * __cdecl operator new[](unsigned int nSize)
{
	return StormSMemAlloc(nSize, "new[]", -1, 0);
}

static int __cdecl UnisysComparePairs(const void *pKey, const void *pElement);

typedef struct SYSMAPPAIR SYSMAPPAIR, *PTSYSMAPPAIR;

struct SYSMAPPAIR
{
 	WORD		wSysCode;
 	WORD		wUnit;
};

static WORD sgwSysMapCount;
static SYSMAPPAIR *sgptSysMapPairs;
static BYTE *sgpbLeadRanges;
static char sgszSysMapName[40];
static WORD *sgpwSysMapIndex;
static BYTE sgbNumLeadRanges;

void __fastcall Unicode::unloadSysMap(void)
{
	if (sgpbLeadRanges)
	{
		delete[] sgpbLeadRanges;
		sgpbLeadRanges = NULL;
	}
	if (sgptSysMapPairs)
	{
		delete[] sgptSysMapPairs;
		sgptSysMapPairs = NULL;
	}
	if (sgpwSysMapIndex)
	{
		delete[] sgpwSysMapIndex;
		sgpwSysMapIndex = NULL;
	}
}

int __fastcall Unicode::loadSysMap(HD2ARCHIVE hArchive, const char *pszFileName)
{
	BYTE *pbFile;
	BYTE *pbCur;
	WORD *pwCount;
	BYTE *pbNumRanges;
	SYSMAPPAIR *ptPair;
	char szName[40];
	int nLen;
	BOOL fFound;
	int i;

#line 101
	pbFile = (BYTE *)ArchiveAllocBufferAndRead(hArchive, pszFileName, NULL, D2_THIS_FILE, __LINE__);
	if (::strncmp((char *)pbFile, "KAMAP\n", 5))
	{
#line 105
		D2FREE2(pbFile);
		return 0;
	}
	pbCur = pbFile + 6;
	nLen = 0;
	fFound = FALSE;
	while (!fFound)
	{
		if (pbCur[nLen] != 26)
			nLen++;
		else
			fFound = TRUE;
		if (nLen >= 40)
		{
#line 125
			D2FREE2(pbFile);
			return 0;
		}
	}
	::strncpy(szName, (char *)pbCur, nLen + 1);
	szName[nLen + 1] = 0;
	::strtok(szName, "\n");
	pbCur += nLen + 5;
	pwCount = (WORD *)pbCur;
	pbCur += 2;
	pbNumRanges = pbCur;
	pbCur++;

	if (sgpbLeadRanges)
		delete[] sgpbLeadRanges;
	sgpbLeadRanges = new BYTE[*pbNumRanges * 2];
	if (!sgpbLeadRanges)
	{
#line 149
		D2FREE2(pbFile);
		return 0;
	}
	memcpy(sgpbLeadRanges, pbCur, *pbNumRanges * 2);
	pbCur += *pbNumRanges * 2;

	if (sgptSysMapPairs)
		delete[] sgptSysMapPairs;
	sgptSysMapPairs = new SYSMAPPAIR[*pwCount];
	if (!sgptSysMapPairs)
	{
#line 162
		D2FREE2(pbFile);
		return 0;
	}
	for (i = 0; i < *pwCount; i++)
	{
		ptPair = &sgptSysMapPairs[i];
		ptPair->wSysCode = pbCur[0];
		ptPair->wUnit = pbCur[2];
		pbCur += 4;
	}

	if (sgpwSysMapIndex)
		delete[] sgpwSysMapIndex;
	sgpwSysMapIndex = new WORD[*pwCount];
	if (!sgpwSysMapIndex)
	{
#line 180
		D2FREE2(pbFile);
		return 0;
	}
	for (i = 0; i < *pwCount; i++)
	{
		sgpwSysMapIndex[i] = *(WORD *)pbCur;
		pbCur += 2;
	}

	sgwSysMapCount = *pwCount;
	sgbNumLeadRanges = *pbNumRanges;
	::strcpy(sgszSysMapName, szName);
#line 194
	D2FREE2(pbFile);
	return 1;
}

Unicode * __fastcall Unicode::sys2Unicode(Unicode *pszDst, const char *pszSrc, int nCount)
{
	WORD wCode;
	int nUnit;
	int nUnits;
	int nLo;
	int nHi;
	int nMid;
	int j;

	if (!sgpwSysMapIndex)
		return win2Unicode(pszDst, pszSrc, nCount);
	for (nUnits = 0; nUnits < nCount; nUnits++)
	{
		if (!*pszSrc)
			break;
		if ((BYTE)*pszSrc >= 32)
		{
			for (j = 0; j < sgbNumLeadRanges; j++)
			{
				if (sgpbLeadRanges[j * 2] <= (BYTE)*pszSrc && (BYTE)*pszSrc <= sgpbLeadRanges[j * 2 + 1])
					goto Single;
			}
			wCode = MAKEWORD(pszSrc[1], pszSrc[0]);
			pszSrc += 2;
		}
		else
		{
Single:
			wCode = (BYTE)*pszSrc;
			pszSrc++;
		}
		if (wCode < 32)
			nUnit = wCode;
		else
		{
			nLo = 0;
			nHi = sgwSysMapCount - 1;
			if (wCode < sgptSysMapPairs[sgpwSysMapIndex[0]].wSysCode ||
				wCode > sgptSysMapPairs[sgpwSysMapIndex[nHi]].wSysCode)
				nMid = -1;
			else
			{
				if (wCode == sgptSysMapPairs[sgpwSysMapIndex[nHi]].wSysCode)
					nMid = nHi;
				else
				{
					nMid = nHi >> 1;
					do
					{
						if (wCode < sgptSysMapPairs[sgpwSysMapIndex[nMid]].wSysCode)
							nHi = nMid;
						else
							nLo = nMid;
						nMid = (nHi + nLo) >> 1;
					} while (nHi - nLo > 1);
					if (sgptSysMapPairs[sgpwSysMapIndex[nMid]].wSysCode != wCode)
						nMid = -1;
				}
			}
			if (nMid < 0)
				nUnit = -1;
			else
				nUnit = sgptSysMapPairs[sgpwSysMapIndex[nMid]].wUnit;
		}
		if (nUnit < 0)
			nUnits--;
		else
			pszDst[nUnits].wUnit = (WORD)nUnit;
	}
	if (!*pszSrc && nUnits < nCount)
		pszDst[nUnits].wUnit = 0;
	return pszDst;
}

char * __fastcall Unicode::unicode2Sys(char *pszDst, const Unicode *pszSrc, int nCount)
{
	SYSMAPPAIR tKey;
	SYSMAPPAIR *ptFound;
	int nCode;
	int nBytes;
	int j;

	if (!sgptSysMapPairs)
		return unicode2Win(pszDst, pszSrc, nCount);
	for (nBytes = 0; nBytes < nCount; pszSrc++)
	{
		if (!pszSrc->wUnit)
			break;
		if (pszSrc->wUnit < 32)
			nCode = pszSrc->wUnit;
		else
		{
			tKey.wUnit = pszSrc->wUnit;
			ptFound = (SYSMAPPAIR *)bsearch(&tKey, sgptSysMapPairs, sgwSysMapCount, sizeof(SYSMAPPAIR),
				UnisysComparePairs);
			if (!ptFound)
				continue;
			nCode = ptFound->wSysCode;
		}
		if (nCode < 0)
			continue;
		if (!(nCode & 65280))
		{
			if ((BYTE)nCode < 32)
				goto Single;
			for (j = 0; j < sgbNumLeadRanges; j++)
			{
				if (sgpbLeadRanges[j * 2] <= (BYTE)nCode && (BYTE)nCode <= sgpbLeadRanges[j * 2 + 1])
					goto Single;
			}
		}
		pszDst[nBytes++] = (char)((WORD)nCode >> 8);
		pszDst[nBytes++] = (char)nCode;
		continue;
Single:
		pszDst[nBytes++] = (char)nCode;
	}
	if (!pszSrc->wUnit && nBytes < nCount)
		pszDst[nBytes] = 0;
	return pszDst;
}

static int __cdecl UnisysComparePairs(const void *pKey, const void *pElement)
{
	return ((const SYSMAPPAIR *)pKey)->wUnit - ((const SYSMAPPAIR *)pElement)->wUnit;
}

unsigned long __fastcall Unicode::sysWidth(const Unicode *pszStr, int nCount)
{
	SYSMAPPAIR tKey;
	SYSMAPPAIR *ptFound;
	int nCode;
	unsigned long nBytes;
	int i;
	int j;

	if (!sgptSysMapPairs)
		return strlen(pszStr);
	nBytes = 0;
	for (i = 0; i < nCount; i++)
	{
		if (!pszStr[i].wUnit)
			break;
		if (pszStr[i].wUnit < 32)
			nCode = pszStr[i].wUnit;
		else
		{
			tKey.wUnit = pszStr[i].wUnit;
			ptFound = (SYSMAPPAIR *)bsearch(&tKey, sgptSysMapPairs, sgwSysMapCount, sizeof(SYSMAPPAIR),
				UnisysComparePairs);
			if (!ptFound)
				continue;
			nCode = ptFound->wSysCode;
		}
		if (nCode < 0)
			continue;
		if (!(nCode & 65280))
		{
			if ((BYTE)nCode < 32)
				goto OneByte;
			j = 0;
			if (j < sgbNumLeadRanges)
			{
				while (1)
				{
					if (sgpbLeadRanges[j * 2] <= (BYTE)nCode && (BYTE)nCode <= sgpbLeadRanges[j * 2 + 1])
						break;
					j++;
					if (j >= sgbNumLeadRanges)
						goto TwoBytes;
				}
OneByte:
				nBytes++;
				continue;
			}
		}
TwoBytes:
		nBytes += 2;
	}
	return nBytes;
}

unsigned long __fastcall Unicode::unicodeWidth(const char *pszStr, int nBytes)
{
	unsigned long nUnits;
	int nPos;
	int j;
	BYTE bByte;

	if (!sgpwSysMapIndex)
		return ::strlen(pszStr);
	nUnits = 0;
	nPos = 0;
	while (nPos < nBytes)
	{
		bByte = pszStr[nPos];
		if (!bByte)
			break;
		if (bByte >= 32)
		{
			for (j = 0; j < sgbNumLeadRanges; j++)
			{
				if (sgpbLeadRanges[j * 2] <= bByte && bByte <= sgpbLeadRanges[j * 2 + 1])
					goto OneByte;
			}
			nPos += 2;
			goto Counted;
		}
OneByte:
		nPos++;
Counted:
		nUnits++;
	}
	return nUnits;
}
