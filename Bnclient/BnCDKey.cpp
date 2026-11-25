#include "Bnclient.h"
#include "StormImports.h"

static DWORD sgdwCdKeyToken;
static const signed char sgacKeyDigits[256] =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, 0, -1, 1, -1, 2, 3, 4, 5, -1, -1, -1, -1, -1, -1,
	-1, -1, 6, 7, 8, 9, 10, 11, 12, -1, 13, 14, -1, 15, 16, -1,
	17, -1, 18, -1, 19, -1, 20, 21, 22, -1, 23, -1, -1, -1, -1, -1,
	-1, -1, 6, 7, 8, 9, 10, 11, 12, -1, 13, 14, -1, 15, 16, -1,
	17, -1, 18, -1, 19, -1, 20, 21, 22, -1, 23, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

struct BNMSG36
{
 	DWORD		dwCDKeyLoadState;
 	DWORD		dwKeyLength;
 	DWORD		dwProduct;
 	DWORD		dwValue1;
 	DWORD		dwNonce;
 	DWORD		dwToken;
 	BYTE		abHash[20];
 	char		szName[128];
};

struct BNHASH36
{
 	DWORD		dwToken;
 	DWORD		dwNonce;
 	DWORD		dwProduct;
 	DWORD		dwValue1;
 	DWORD		dwValue2;
};

BOOL __fastcall BnMessQueueSendCDKey(void)
{
	BNHASH36 tHashed;
	BNMSG36 tMessage;
	char szKey[32];
	const char *pszName;
	const char *pszKey;
	DWORD dwLoadState;
	DWORD dwChecksum;
	DWORD dwBit;
	DWORD dwFold;
	DWORD dwPair;
	DWORD dwDigit;
	DWORD dwMask;
	DWORD i;
	int n;
	int j;
	char c;
	BOOL fSent;
	BOOL fValid;

	GridLoadCDKey();
	pszName = GridGetOwner();
	pszKey = GridGetCDKey();
	dwLoadState = GridGetCDKeyLoadState();
	dwChecksum = 0;
	sgdwCdKeyToken += time(NULL);
	sgdwCdKeyToken += GetTickCount();
	tHashed.dwToken = sgdwCdKeyToken;
	tHashed.dwNonce = BnMessQueueGetNonce();
	tHashed.dwProduct = 0;
	tHashed.dwValue1 = 0;
	tHashed.dwValue2 = 0;
	fValid = FALSE;
	if (pszKey)
	{
		StormSStrCopy(szKey, pszKey, sizeof(szKey));
		dwBit = 1;
		for (i = 0; i < 16; i += 2)
		{
			dwPair = sgacKeyDigits[(BYTE)szKey[i]] * 24 + sgacKeyDigits[(BYTE)szKey[i + 1]];
			if (dwPair >= 256)
			{
				dwPair -= 256;
				dwChecksum |= dwBit;
			}
			dwDigit = dwPair >> 4 & 15;
			szKey[i] = (char)(dwDigit >= 10 ? dwDigit + 'A' - 10 : dwDigit + '0');
			dwDigit = dwPair & 15;
			szKey[i + 1] = (char)(dwDigit >= 10 ? dwDigit + 'A' - 10 : dwDigit + '0');
			dwBit <<= 1;
		}
		dwFold = 3;
		for (i = 0; i < 16; i++)
		{
			c = (char)tolower(szKey[i]);
			dwFold += dwFold * 2 ^ (c >= 'a' ? c - 'a' + 10 : c - '0');
		}
		if ((dwFold & 255) == dwChecksum)
		{
			for (n = 15; n >= 0; n--)
			{
				j = (n + 7) & 15;
				c = szKey[n];
				szKey[n] = szKey[j];
				szKey[j] = c;
			}
			dwMask = 330078017;
			for (n = 15; n >= 0; n--)
			{
				szKey[n] = (char)toupper(szKey[n]);
				if (szKey[n] <= '7')
				{
					szKey[n] = (char)((dwMask & 7) ^ szKey[n]);
					dwMask >>= 3;
				}
				else if (szKey[n] < 'A')
					szKey[n] = (char)((n & 1) ^ szKey[n]);
			}
			{
				char szProduct[3];
				char szValue1[7];
				char szValue2[9];

				StormSStrCopy(szProduct, szKey, sizeof(szProduct));
				StormSStrCopy(szValue1, szKey + 2, sizeof(szValue1));
				StormSStrCopy(szValue2, szKey + 8, sizeof(szValue2));
				tHashed.dwProduct = strtoul(szProduct, NULL, 16);
				tHashed.dwValue1 = strtoul(szValue1, NULL, 16);
				tHashed.dwValue2 = strtoul(szValue2, NULL, 16);
			}
			fValid = TRUE;
		}
	}
	if (!fValid)
		fSent = FALSE;
	else
	{
		tMessage.dwCDKeyLoadState = dwLoadState;
		tMessage.dwKeyLength = StormSStrLen(pszKey);
		tMessage.dwProduct = tHashed.dwProduct;
		tMessage.dwValue1 = tHashed.dwValue1;
		tMessage.dwNonce = tHashed.dwNonce;
		tMessage.dwToken = tHashed.dwToken;
		GridHash(tMessage.abHash, &tHashed, sizeof(tHashed));
		fSent = BnConnectSend(54, &tMessage,
			StormSStrCopy(tMessage.szName, pszName, sizeof(tMessage.szName))
			+ offsetof(BNMSG36, szName) + 1) != 0;
	}
	GridFreeCDKey();
	if (!fSent)
		BnMessQueueForceReason(3);
	return fSent;
}
