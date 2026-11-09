#ifndef D2RE_STRTABLE_H
#define D2RE_STRTABLE_H

#include <windows.h>

#pragma pack(push, 1)

typedef struct STRTABLEHEADER STRTABLEHEADER, *PTSTRTABLEHEADER;

struct STRTABLEHEADER
{
 	WORD		usCRC;
 	WORD		usNumElements;
 	int			iHashTableSize;
 	BYTE		unobserved_1[1];
 	DWORD		dwIndexStart;
 	DWORD		dwMaxTries;
 	DWORD		dwFileLen;
};

typedef struct STRTABLENODE STRTABLENODE, *PTSTRTABLENODE;

struct STRTABLENODE
{
 	BYTE		bUsed;
 	WORD		wStringIndex;
 	BYTE		unobserved_2[4];
 	DWORD		dwKeyOffset;
 	DWORD		dwStringOffset;
 	WORD		wStringLength;
};

#pragma pack(pop)

typedef struct STRMISSING STRMISSING, *PTSTRMISSING;

struct STRMISSING
{
 	Unicode		*pszText;
 	char		*pszKey;
 	STRMISSING	*ptNext;
};

typedef struct LOCALE LOCALE, *PTLOCALE;

struct LOCALE
{
 	const char	*pszSystemLanguage;
 	const char	*pszDirectory;
 	const char	*pszFallbackLanguage;
 	const char	*pszRegion;
 	int			eLanguage;
};

#endif
