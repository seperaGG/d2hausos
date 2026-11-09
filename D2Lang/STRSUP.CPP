#define D2_THIS_FILE "C:\\D2\\Source\\D2Lang\\Unicode\\STRSUP.CPP"
#include "D2Lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "D2Macros.h"
#include "StormImports.h"
#include "FogImports.h"
#include <stdarg.h>

int __fastcall Unicode::strcmp(const Unicode *pszStr1, const Unicode *pszStr2)
{
	unsigned short wFirst;
	unsigned short wSecond;

	while (pszStr1->wUnit && pszStr2->wUnit && pszStr1->wUnit == pszStr2->wUnit)
	{
		pszStr1++;
		pszStr2++;
	}
	wFirst = pszStr1->wUnit;
	wSecond = pszStr2->wUnit;
	if (wSecond > wFirst)
		return -1;
	return wSecond < wFirst;
}

int __fastcall Unicode::stricmp(const Unicode *pszStr1, const Unicode *pszStr2)
{
	unsigned short wUp1;
	unsigned short wUp2;

	while (pszStr1->wUnit && pszStr2->wUnit)
	{
		wUp1 = UNICODE_TOUPPER(pszStr1->wUnit);
		wUp2 = UNICODE_TOUPPER(pszStr2->wUnit);
		if (wUp1 != wUp2)
			return wUp1 < wUp2 ? -1 : 1;
		pszStr1++;
		pszStr2++;
	}
	if (pszStr1->wUnit)
		return 1;
	return pszStr2->wUnit ? -1 : 0;
}

int __fastcall Unicode::strnicmp(const Unicode *pszStr1, const Unicode *pszStr2, unsigned int nCount)
{
	Unicode tUp1;
	Unicode tUp2;

	tUp1 = pszStr1->toUpper();
	tUp2 = pszStr2->toUpper();
	while (pszStr1->wUnit)
	{
		if (!pszStr2->wUnit || !nCount--)
			break;
		if (tUp1.wUnit != tUp2.wUnit)
			break;
		tUp1 = (pszStr1++)->toUpper();
		tUp2 = (pszStr2++)->toUpper();
	}
	if (tUp1.wUnit < tUp2.wUnit)
		return -1;
	return tUp2.wUnit < tUp1.wUnit;
}

Unicode * __fastcall Unicode::strchr(const Unicode *pszStr, Unicode tChar)
{
	while (pszStr->wUnit && pszStr->wUnit != tChar)
		pszStr++;

	return pszStr->wUnit ? (Unicode *)pszStr : NULL;
}

Unicode * __fastcall Unicode::strcat(Unicode *pszDst, const Unicode *pszSrc)
{
	Unicode *pszCur = pszDst;

	while (pszCur->wUnit)
		pszCur++;
	do
	{
		*pszCur = *pszSrc;
		pszCur++;
	} while ((pszSrc++)->wUnit);
	return pszDst;
}

Unicode * __fastcall Unicode::strncat(Unicode *pszDst, const Unicode *pszSrc, int nCount)
{
	Unicode *pszCur = pszDst;
	unsigned short wUnit;

	while (pszCur->wUnit)
		pszCur++;
	while (nCount-- && (wUnit = pszSrc->wUnit) != 0)
	{
		pszCur->wUnit = wUnit;
		pszCur++;
		pszSrc++;
	}
	pszCur->wUnit = 0;
	return pszDst;
}

Unicode * __fastcall Unicode::strncpy(Unicode *pszDst, const Unicode *pszSrc, int nCount)
{
	Unicode *pszCur = pszDst;
	unsigned short wUnit;

	while (nCount-- && (wUnit = pszSrc->wUnit) != 0)
	{
		pszCur->wUnit = wUnit;
		pszCur++;
		pszSrc++;
	}
	while (nCount != -1)
	{
		pszCur->wUnit = 0;
		pszCur++;
		nCount--;
	}
	return pszDst;
}

Unicode * __fastcall Unicode::strcpy(Unicode *pszDst, const Unicode *pszSrc)
{
	Unicode *pszCur = pszDst;

	do
	{
		*pszCur = *pszSrc;
		pszCur++;
	} while ((pszSrc++)->wUnit);
	return pszDst;
}

int __fastcall Unicode::strlen(const Unicode *pszStr)
{
	int nLength = 0;

	if (!pszStr)
		return 0;
	while (pszStr->wUnit)
	{
		pszStr++;
		nLength++;
	}
	return nLength;
}

const Unicode * __fastcall Unicode::strstri(const Unicode *pszStr, const Unicode *pszSub)
{
	Unicode tCur;
	int nMatched;

	if (!pszStr || !pszSub)
		return NULL;

	tCur = *pszStr;
	for (;;)
	{
		while (!Unicode::compare(tCur, *pszSub))
		{
			tCur = *(++pszStr);
			if (!tCur.wUnit)
				return NULL;
		}
		nMatched = 0;
		while (Unicode::compare(*pszStr, *pszSub) &&
			pszStr->wUnit && pszSub->wUnit)
		{
			pszStr++;
			pszSub++;
			nMatched++;
		}
		if (!pszSub->wUnit)
			return pszStr - nMatched;
		if (!pszStr->wUnit)
			return NULL;
		pszStr = pszStr - nMatched + 1;
		pszSub -= nMatched;
	}
}

const Unicode * __fastcall Unicode::strstr(const Unicode *pszStr, const Unicode *pszSub)
{
	int nMatched;
	unsigned short wStr;
	unsigned short wSub;

	if (!pszStr || !pszSub)
		return NULL;
	while (1)
	{
		while (*pszStr != *pszSub && *pszStr)
			pszStr++;

		if (!pszStr->wUnit)
			return NULL;
		nMatched = 0;
		while (1)
		{
			wStr = pszStr->wUnit;
			wSub = pszSub->wUnit;
			if (wStr != wSub || !wStr || !wSub)
				break;
			pszStr++;
			pszSub++;
			nMatched++;
		}
		if (!pszSub->wUnit)
			return pszStr - nMatched;
		if (!pszStr->wUnit)
			return NULL;
		pszStr = pszStr - nMatched + 1;
		pszSub -= nMatched;
	}
}

void __cdecl Unicode::sprintf(int nMax, Unicode *pszDst, const Unicode *pszFormat, ...)
{
	va_list pArgs;
	DWORD dwArg;
	WORD wUnit;
	WORD wPercent[4];
	char szNumber[16];
	const Unicode *puCurrent;
	const Unicode *puFound;
	const Unicode *pszArg;
	Unicode *pszEnd;
	Unicode tSpec;
	int nLen;
	int nArgLen;
	int nRemain;

	if (!pszFormat)
		return;
	dwArg = *(DWORD *)(&pszFormat + 1);
	nLen = 0;
	puCurrent = NULL;
	pszDst->wUnit = 0;
	if (!pszFormat->wUnit)
		return;
	va_start(pArgs, pszFormat);
	for (;;)
	{
		tSpec = 0;
		wPercent[0] = '%';
		wPercent[1] = 0;

		puFound = strstr(pszFormat, (Unicode *)wPercent);
		if (puFound)
		{
			puCurrent = puFound;
			tSpec = puCurrent[1];
		}
		while (pszFormat != puCurrent && (wUnit = pszFormat->wUnit) != 0)
		{
			if (nLen >= nMax)
			{
				pszDst[-1].wUnit = 0;
				return;
			}
			pszDst->wUnit = wUnit;
			pszDst++;
			pszFormat++;
			nLen++;
		}
		if (nLen >= nMax)
		{
			pszDst[-1].wUnit = 0;
			return;
		}
		if (!pszFormat->wUnit)
		{
			pszDst->wUnit = 0;
			return;
		}
		D2_OPAQUE(puCurrent);
		if (!puCurrent)
		{
#line 246
			D2ASSERT(puCurrent);
		}
		switch (tSpec)
		{
		case 'd':
		case 'u':
		{
			Unicode wNumber[15];
			if (tSpec == 'd')
				itoa((int)dwArg, szNumber, 10);
			else
				_ultoa(dwArg, szNumber, 10);
			toUnicode(wNumber, szNumber, 15);
			nArgLen = strlen(wNumber);
			if (nArgLen + nLen + 1 >= nMax)
				return;
			strcpy(pszDst, wNumber);
			nLen += nArgLen;
			pszDst += nArgLen;
		}
			break;
		case 's':
			pszArg = (const Unicode *)dwArg;
			nArgLen = strlen(pszArg);
			if (nArgLen && nArgLen + nLen + 1 < nMax)
			{
				strcpy(pszDst, pszArg);
				pszDst += nArgLen;
				nLen += nArgLen;
				break;
			}

			nRemain = nMax - nLen - 1;
			pszEnd = pszDst;
			while (pszEnd->wUnit)
				pszEnd++;
			while (nRemain-- && (wUnit = pszArg->wUnit) != 0)
			{
				pszEnd->wUnit = wUnit;
				pszEnd++;
				pszArg++;
			}
			pszEnd->wUnit = 0;
			return;
		case '%':
			if (nLen + 1 >= nMax)
				return;
			strcpy(pszDst, (Unicode *)wPercent);
			pszDst++;
			nLen++;
			break;
		case 0:

			strcpy(pszDst, (Unicode *)wPercent);
			return;
		default:
#line 317
			FogErrorManagerHalt("Unknown format specifier in Unicode::sprintf", D2_THIS_FILE, __LINE__);
			exit(-1);
		}
		pszFormat += 2;
		pArgs += sizeof(DWORD);
		dwArg = *(DWORD *)pArgs;
		if (!pszFormat->wUnit)
			return;
	}
}
