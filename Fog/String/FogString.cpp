#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\String\\FogString.cpp"
#include "../FogLocal.h"
#include <ctype.h>
#include "D2Macros.h"

void __stdcall FogStringCapitalize(char *pszString)
{
	BOOL fWordStart;
	int nLength;
	int nCount;

	fWordStart = TRUE;
	if (!pszString)
		return;
	nLength = strlen(pszString);
	for (nCount = 0; nCount < nLength; nCount++)
	{
#line 120
		D2ASSERT(nCount < 1024);
		if (pszString[nCount] == ' ')
			fWordStart = TRUE;
		else if (fWordStart)
		{
			fWordStart = FALSE;
			pszString[nCount] = (char)toupper(pszString[nCount]);
		}
		else
			pszString[nCount] = (char)tolower(pszString[nCount]);
	}
}

BOOL __fastcall FogStringIsWindowsNT(void)
{
	OSVERSIONINFOA tVersion;
	int i;

	for (i = 0; i < (int)(sizeof(tVersion) / sizeof(DWORD)); i++)
		((DWORD *)&tVersion)[i] = 0;
	tVersion.dwOSVersionInfoSize = sizeof(tVersion);
	if (!GetVersionExA(&tVersion))
		return FALSE;
	return tVersion.dwPlatformId == VER_PLATFORM_WIN32_NT;
}
