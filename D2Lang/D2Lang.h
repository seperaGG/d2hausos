#ifndef D2LANG_H
#define D2LANG_H

#include <windows.h>
#include <ctype.h>
#include "Archive.h"
#include "D2LangLinkNames.h"

#define UNICODE_TOUPPER(w)	((w) < 256 ? Unicode::_toUpperTable[(w)] : (w))

extern const char *const gpszD2LangModuleName;

struct __declspec(dllexport) Unicode
{
 	unsigned short	wUnit;

	Unicode(unsigned short wChar = 0) { wUnit = wChar; }

	operator unsigned short(void) const
	{
		return wUnit;
	}
	int isWhitespace(void) const
	{
		if (wUnit < 256)
			return isspace(wUnit);
		return 0;
	}
	int isNewline(void) const
	{
		return wUnit == 10;
	}
	int isPipe(void) const
	{
		if (wUnit < 256)
			return wUnit == 255;
		return 0;
	}
	int isASCII(void) const
	{
		return wUnit < 128;
	}
	int isAlpha(void) const
	{
		if (wUnit < 128 && isalpha((char)wUnit))
			return 1;
		return 0;
	}
	Unicode toUpper(void) const
	{
		return Unicode(UNICODE_TOUPPER(wUnit));
	}
	Unicode toLower(void) const
	{
		return Unicode(wUnit < 256 ? _toLowerTable[wUnit] : wUnit);
	}
	int compare(Unicode tOther) const
	{
		return UNICODE_TOUPPER(wUnit) == UNICODE_TOUPPER(tOther.wUnit);
	}
	static int __fastcall compare(Unicode tFirst, Unicode tSecond)
	{
		return UNICODE_TOUPPER(tFirst.wUnit) == UNICODE_TOUPPER(tSecond.wUnit);
	}
	static int __fastcall isWordEnd(const Unicode *pszStr, unsigned int nIndex)
	{
		if (nIndex && isalnum(pszStr[nIndex]) && !isalnum(pszStr[nIndex + 1]))
			return 1;
		return 0;
	}
	static int __fastcall strcoll(const Unicode *pszStr1, const Unicode *pszStr2)
	{
		return strncoll(pszStr1, pszStr2, 2147483647);
	}

	static int __fastcall isLineBreak(const Unicode *pszStr, unsigned int nIndex);
	static int __fastcall strcmp(const Unicode *pszStr1, const Unicode *pszStr2);
	static int __fastcall stricmp(const Unicode *pszStr1, const Unicode *pszStr2);
	static int __fastcall strnicmp(const Unicode *pszStr1, const Unicode *pszStr2, unsigned int nCount);
	static Unicode * __fastcall strchr(const Unicode *pszStr, Unicode tChar);
	static Unicode * __fastcall strcat(Unicode *pszDst, const Unicode *pszSrc);
	static Unicode * __fastcall strncat(Unicode *pszDst, const Unicode *pszSrc, int nCount);
	static Unicode * __fastcall strncpy(Unicode *pszDst, const Unicode *pszSrc, int nCount);
	static Unicode * __fastcall strcpy(Unicode *pszDst, const Unicode *pszSrc);
	static int __fastcall strlen(const Unicode *pszStr);
	static void __cdecl sprintf(int nMax, Unicode *pszDst, const Unicode *pszFormat, ...);
	static const Unicode * __fastcall strstri(const Unicode *pszStr, const Unicode *pszSub);
	static const Unicode * __fastcall strstr(const Unicode *pszStr, const Unicode *pszSub);
	int isLeftToRight(void) const;
	static Unicode * __fastcall win2Unicode(Unicode *pszDst, const char *pszSrc, int nCount);
	static char * __fastcall unicode2Win(char *pszDst, const Unicode *pszSrc, int nCount);

	enum Direction
	{
		DIRECTION_F0 = 240,
		DIRECTION_F1 = 241,
		DIRECTION_F2 = 242
	};
	Direction directionality(void);
	static int __fastcall strncoll(const Unicode *pszStr1, const Unicode *pszStr2, int nCount);
	static void __fastcall unloadSysMap(void);
	static int __fastcall loadSysMap(HD2ARCHIVE hArchive, const char *pszFileName);
	static Unicode * __fastcall sys2Unicode(Unicode *pszDst, const char *pszSrc, int nCount);
	static char * __fastcall unicode2Sys(char *pszDst, const Unicode *pszSrc, int nCount);
	static unsigned long __fastcall sysWidth(const Unicode *pszStr, int nCount);
	static unsigned long __fastcall unicodeWidth(const char *pszStr, int nBytes);
	static Unicode * __fastcall toUnicode(Unicode *pszDst, const char *pszSrc, int nCount);
	static char * __fastcall toUtf(char *pszDst, const Unicode *pszSrc, int nCount);
	static unsigned int __fastcall utfnwidth(const Unicode *pszSrc, int nCount);
	static unsigned int __fastcall unicodenwidth(const char *pszSrc, int nBytes);
	int utfwidth(void) const;

private:
	static unsigned short _toLowerTable[256];
	static unsigned short _toUpperTable[256];
};

BOOL __fastcall StrNewTable(HD2ARCHIVE hArchive);
void __fastcall StrDeleteTable(void);
DWORD __stdcall StrHashString(const char *pszKey, DWORD dwTableSize);
Unicode * __fastcall StrGetStringByKey(const char *pszKey);
Unicode * __fastcall StrGetStringByIndex(WORD wStringId);
void __stdcall StrComputeCRC(WORD *pusCRC, const BYTE *pbData, DWORD dwIndexStart,
	DWORD dwIndexEnd);

void __stdcall StrGetLanguageName(char *pszDst, int eField);
int __cdecl StrGetUseLanguage(void);
int __cdecl StrGetSystemLanguage(void);
void __stdcall StrFormatInt(Unicode *pszDst, int nValue, int nMaxLength);
void __stdcall StrFormatUnsigned(Unicode *pszDst, unsigned int nValue, int nMaxLength);
void __stdcall StrFormatInt64(Unicode *pszDst, __int64 nValue, int nMaxLength);
void __stdcall StrFormatUInt64Suffix(Unicode *pszDst, unsigned __int64 nValue, int nMaxLength);
WORD __stdcall StrGetStringIndexByKey(const char *pszKey, Unicode **ppString);

#endif
