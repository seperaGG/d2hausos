#define D2_THIS_FILE "C:\\D2\\Source\\D2Server\\PlayerToken.cpp"
#include "D2Server.h"
#include "D2Macros.h"

static PLAYERTOKEN *sgptTokens;

static void __fastcall PlayerTokenExpire(void);

BOOL __fastcall PlayerTokenAdd(const char *pszName, const char *pszAccount, DWORD dwArg, DWORD dwToken,
	DWORD dwGameId)
{
	PLAYERTOKEN *ptToken;
	int i;

	PlayerTokenExpire();
	for (ptToken = sgptTokens; ptToken; ptToken = ptToken->ptNext)
	{
		if (!StormSStrCmp(ptToken->szName, pszName, 2147483647))
		{
			ptToken->dwGameId = dwGameId;
			ptToken->dwToken = dwToken;
			ptToken->dwTick = GetTickCount();
			StormSStrCopy(ptToken->szAccount, pszAccount, 16);
			ptToken->dwTokenResult = dwArg;
			return TRUE;
		}
	}
#line 52
	ptToken = (PLAYERTOKEN *)D2ALLOC2(sizeof(PLAYERTOKEN));
#line 53
	D2ASSERT(ptToken);
	for (i = 0; i < (int)(sizeof(PLAYERTOKEN) / sizeof(DWORD)); i++)
		((DWORD *)ptToken)[i] = 0;
	ptToken->dwGameId = dwGameId;
	ptToken->dwToken = dwToken;
	ptToken->dwTick = GetTickCount();
	StormSStrCopy(ptToken->szName, pszName, 16);
	StormSStrCopy(ptToken->szAccount, pszAccount, 16);
	ptToken->dwTokenResult = dwArg;
	ptToken->ptNext = sgptTokens;
	sgptTokens = ptToken;
	return TRUE;
}

BOOL __fastcall PlayerTokenFind(const char *pszName, DWORD dwToken, DWORD dwGameId, char *pszAccount,
	DWORD *pdwArg)
{
	PLAYERTOKEN *ptToken;
	PLAYERTOKEN **pptLink;
	DWORD dwNow;
	BOOL fResult;

	dwNow = GetTickCount();
	pptLink = &sgptTokens;
	for (ptToken = sgptTokens; ptToken; ptToken = ptToken->ptNext)
	{
		if (!StormSStrCmp(ptToken->szName, pszName, 2147483647))
		{
			*pptLink = ptToken->ptNext;
			if (ptToken->dwToken == dwToken && ptToken->dwGameId == dwGameId
				&& dwNow - ptToken->dwTick <= TOKEN_LIFETIME)
				fResult = TRUE;
			else
				fResult = FALSE;
			StormSStrCopy(pszAccount, ptToken->szAccount, 16);
			*pdwArg = ptToken->dwTokenResult;
#line 99
			D2FREE2(ptToken);
			return fResult;
		}
		pptLink = &ptToken->ptNext;
	}
	return FALSE;
}

void __fastcall PlayerTokenRemove(const char *pszName)
{
	PLAYERTOKEN *ptToken;
	PLAYERTOKEN **pptLink;

	pptLink = &sgptTokens;
	for (ptToken = sgptTokens; ptToken; ptToken = ptToken->ptNext)
	{
		if (!StormSStrCmp(ptToken->szName, pszName, 2147483647))
		{
			*pptLink = ptToken->ptNext;
#line 122
			D2FREE2(ptToken);
			return;
		}
		pptLink = &ptToken->ptNext;
	}
}

static void __fastcall PlayerTokenExpire(void)
{
	PLAYERTOKEN *ptToken;
	PLAYERTOKEN **pptLink;
	DWORD dwNow;

	dwNow = GetTickCount();
	pptLink = &sgptTokens;
	for (ptToken = sgptTokens; ptToken; ptToken = *pptLink)
	{
		if (dwNow - ptToken->dwTick > TOKEN_LIFETIME)
		{
			*pptLink = ptToken->ptNext;
#line 150
			D2FREE2(ptToken);
		}
		else
			pptLink = &ptToken->ptNext;
	}
}
