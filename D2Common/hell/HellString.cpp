#include <windows.h>
#include "Archive.h"

DWORD __fastcall StringLength(LPCTSTR pszString)
{
	DWORD dwLength;

	dwLength = 0;
	while (*pszString++)
		dwLength++;
	return dwLength;
}

BOOL __fastcall StringEqual(LPCTSTR pszString1, LPCTSTR pszString2)
{
	while (*pszString1 && *pszString2 && *pszString1 == *pszString2)
	{
		pszString1++;
		pszString2++;
	}
	return (BYTE)(*pszString1 - *pszString2) == 0;
}

#define CHARCLASS_SPACE	1
#define CHARCLASS_DIGIT	2
static BYTE sgabCharClass[256] =
{
	0, 0, 0, 0, 0, 0, 0, 0, CHARCLASS_SPACE, CHARCLASS_SPACE, CHARCLASS_SPACE, CHARCLASS_SPACE, CHARCLASS_SPACE, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	CHARCLASS_SPACE, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	CHARCLASS_DIGIT, CHARCLASS_DIGIT, CHARCLASS_DIGIT, CHARCLASS_DIGIT, CHARCLASS_DIGIT, CHARCLASS_DIGIT, CHARCLASS_DIGIT,
	CHARCLASS_DIGIT, CHARCLASS_DIGIT, CHARCLASS_DIGIT
};

int __fastcall CharIsDigit(char chChar)
{
	return sgabCharClass[chChar] & CHARCLASS_DIGIT;
}

BOOL __fastcall StringReadNumber(LPCTSTR pszString, LPCTSTR *ppszEnd, int *pnValue)
{
	BOOL fResult;
	int nValue;

	if (!CharIsDigit(*pszString))
	{
		fResult = FALSE;
	}
	else
	{
		nValue = 0;
		while (*pszString && CharIsDigit(*pszString))
		{
			nValue = nValue * 10 + *pszString - '0';
			pszString++;
		}
		*pnValue = nValue;
		fResult = TRUE;
	}
	*ppszEnd = pszString;
	return fResult;
}

int __fastcall CharIsSpace(char chChar)
{
	return sgabCharClass[chChar] & CHARCLASS_SPACE;
}

LPTSTR __fastcall StringSkipSpaces(LPTSTR pszString)
{
	while (*pszString && CharIsSpace(*pszString))
		pszString++;
	return pszString;
}

LPCTSTR __fastcall StringCopyWord(LPTSTR pszDest, LPCTSTR pszSource)
{
	while (*pszSource && !CharIsSpace(*pszSource))
		*pszDest++ = *pszSource++;
	*pszDest = 0;
	return pszSource;
}

LPTSTR __fastcall StringFindLastChar(LPTSTR pszString, char chChar)
{
	LPTSTR pszLast;
	LPTSTR pszScan;

	pszLast = NULL;
	for (pszScan = pszString; *pszScan; pszScan++)
	{
		if (*pszScan == chChar)
			pszLast = pszScan;
	}
	return pszLast;
}

LPTSTR __fastcall PathCutFileName(LPTSTR pszPath)
{
	LPTSTR pszLast;
	LPTSTR pszScan;

	pszLast = NULL;
	for (pszScan = pszPath; *pszScan; pszScan++)
	{
		if (*pszScan == '\\')
			pszLast = pszScan;
	}
	if (!pszLast)
		*pszPath = 0;
	else
		*pszLast = 0;
	return pszPath;
}

LPTSTR __fastcall PathGetFileName(LPTSTR pszPath)
{
	LPTSTR pszLast;
	LPTSTR pszScan;

	pszLast = NULL;
	for (pszScan = pszPath; *pszScan; pszScan++)
	{
		if (*pszScan == '\\')
			pszLast = pszScan;
	}
	if (pszLast)
		return pszLast + 1;
	return pszPath;
}

LPTSTR __fastcall PathCutEndSlash(LPTSTR pszPath)
{
	LPTSTR pszLast;
	LPTSTR pszScan;

	pszLast = NULL;
	for (pszScan = pszPath; *pszScan; pszScan++)
	{
		if (*pszScan == '\\')
			pszLast = pszScan;
	}
	if (pszLast && *CharNextA(pszLast) == 0)
		*pszLast = 0;
	return pszPath;
}
