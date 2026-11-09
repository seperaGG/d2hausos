#include "D2Lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct UTFTABLE UTFTABLE, *PTUTFTABLE;

struct UTFTABLE
{
 	short		sLeadMask;
 	short		sLeadBits;
 	short		sShift;
 	BYTE		unobserved_1[2];
 	long		lMaxChar;
 	long		lMinChar;
};

static UTFTABLE sgtUtfTable[] =
{
	{ 128, 0,  0, { 0 }, 127, 0 },
	{ 224, 192,  6, { 0 }, 2047, 128 },
	{ 240, 224, 12, { 0 }, 65535, 2048 },
	{ 248, 240, 18, { 0 }, 2097151, 65536 },
	{ 252, 248, 24, { 0 }, 67108863, 2097152 },
	{ 254, 252, 30, { 0 }, 2147483647, 67108864 },
	{ 0 }
};

static __inline int __fastcall UnisysEncodeUtf(char *pszOut, Unicode tChar);

static __inline int UnisysDecodeUtf(Unicode *pszOut, const char *pszSrc, int nBytes)
{
	const UTFTABLE *ptEntry;
	short sLead;
	short sNext;
	long lChar;
	int nLen = 0;

	if (nBytes <= 0)
		return -1;
	sLead = (BYTE)*pszSrc;
	lChar = sLead;
	for (ptEntry = sgtUtfTable; ptEntry->sLeadMask; ptEntry++)
	{
		nLen++;
		if ((ptEntry->sLeadMask & sLead) == ptEntry->sLeadBits)
		{
			lChar &= ptEntry->lMaxChar;
			if (lChar < ptEntry->lMinChar)
				return -1;
			pszOut->wUnit = (unsigned short)lChar;
			return nLen;
		}
		if (nBytes <= nLen)
			return -1;
		sNext = (BYTE)(*++pszSrc ^ 128);
		if (sNext & 192)
			return -1;
		lChar = (long)(((DWORD)lChar << 6) | sNext);
	}
	return -1;
}

Unicode * __fastcall Unicode::toUnicode(Unicode *pszDst, const char *pszSrc, int nCount)
{
	Unicode *pszCur;
	int nSrcLen;
	int nUnits = 0;
	int nOffset;
	int nLen;
	int nLimit;

	nSrcLen = ::strlen(pszSrc);
	nOffset = 0;
	if (pszSrc[0])
	{
		nLimit = nCount - 1;
		pszCur = pszDst;
		while (1)
		{
			if (nUnits >= nLimit)
				break;
			nLen = UnisysDecodeUtf(pszCur, pszSrc + nOffset, nSrcLen - nOffset);
			nUnits++;
			pszCur++;
			if (nLen == -1)
				break;
			nOffset += nLen;
			if (!pszSrc[nOffset])
				break;
		}
	}
	pszDst[nUnits].wUnit = 0;
	return pszDst;
}

static __inline int UnisysEncodeUtfInline(Unicode tChar, char *pszOut)
{
	const UTFTABLE *ptEntry;
	int nChar = tChar.wUnit;
	int nLen = 0;
	int nShift;

	for (ptEntry = sgtUtfTable; ptEntry->sLeadMask; ptEntry++)
	{
		nLen++;
		if (nChar <= ptEntry->lMaxChar)
		{
			nShift = ptEntry->sShift;
			*pszOut = (char)((nChar >> nShift) | (unsigned char)ptEntry->sLeadBits);
			while (nShift > 0)
			{
				nShift -= 6;
				*++pszOut = (char)(((nChar >> nShift) & 63) | 128);
			}
			return nLen;
		}
	}
	return -1;
}

char * __fastcall Unicode::toUtf(char *pszDst, const Unicode *pszSrc, int nCount)
{
	int nBytes = 0;
	int nLen;
	int i = 0;

	if (pszSrc[i].wUnit)
	{
		nCount--;
		while (1)
		{
			if (nBytes >= nCount)
				break;
			nLen = UnisysEncodeUtfInline(pszSrc[i], pszDst + nBytes);
			if (nLen == -1)
				break;
			nBytes += nLen;
			i++;
			if (!pszSrc[i].wUnit)
				break;
		}
	}
	pszDst[nBytes] = 0;
	return pszDst;
}

unsigned int __fastcall Unicode::utfnwidth(const Unicode *pszSrc, int nCount)
{
	unsigned int nBytes = 0;
	int i = 0;
	int nLen;
	char szBuf[12];

	if (pszSrc[i].wUnit)
	{
		while (1)
		{
			if (i >= nCount)
				break;
			nLen = UnisysEncodeUtfInline(pszSrc[i], szBuf);
			if (nLen == -1)
				break;
			nBytes += nLen;
			i++;
			if (!pszSrc[i].wUnit)
				break;
		}
	}
	return nBytes;
}

static __inline int UnisysDecodeUtfLength(long *plChar, const char *pszSrc, int nBytes)
{
	const UTFTABLE *ptEntry;
	int nLead;
	int nNext;
	long lChar;
	int nLen = 0;

	if (nBytes <= 0)
		return -1;
	nLead = (BYTE)*pszSrc;
	lChar = nLead;
	for (ptEntry = sgtUtfTable; ptEntry->sLeadMask; ptEntry++)
	{
		nLen++;
		if ((ptEntry->sLeadMask & nLead) == ptEntry->sLeadBits)
		{
			lChar &= ptEntry->lMaxChar;
			if (lChar < ptEntry->lMinChar)
				return -1;
			*plChar = lChar;
			return nLen;
		}
		if (nBytes <= nLen)
			return -1;
		nNext = (BYTE)*++pszSrc ^ 128;
		if (nNext & 192)
			return -1;
		lChar = (long)(((DWORD)lChar << 6) | nNext);
	}
	return -1;
}

unsigned int __fastcall Unicode::unicodenwidth(const char *pszSrc, int nBytes)
{
	unsigned int nUnits = 0;
	int nLen;
	int i;
	long lChar;

	while (nBytes > 0)
	{
		if (!*pszSrc)
			break;
		nLen = UnisysDecodeUtfLength(&lChar, pszSrc, nBytes);
		if (nLen != -1)
		{
			for (i = 0; i < nLen; i++)
			{
				if (!*pszSrc++)
					return nUnits;
			}
			nUnits++;
			nBytes -= nLen;
		}
		else
		{
			pszSrc++;
			nBytes--;
		}
	}
	return nUnits;
}

int Unicode::utfwidth(void) const
{
	char szBuf[8];
	return UnisysEncodeUtf(szBuf, wUnit);
}

static __inline int __fastcall UnisysEncodeUtf(char *pszOut, Unicode tChar)
{
	const UTFTABLE *ptEntry;
	int nChar = tChar.wUnit;
	int nLen = 0;
	int nShift;

	for (ptEntry = sgtUtfTable; ptEntry->sLeadMask; ptEntry++)
	{
		nLen++;
		if (nChar <= ptEntry->lMaxChar)
		{
			nShift = ptEntry->sShift;
			*pszOut = (char)((nChar >> nShift) | (unsigned char)ptEntry->sLeadBits);
			while (nShift > 0)
			{
				nShift -= 6;
				*++pszOut = (char)(((nChar >> nShift) & 63) | 128);
			}
			return nLen;
		}
	}
	return -1;
}
