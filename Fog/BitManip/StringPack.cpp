#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\BitManip\\StringPack.cpp"
#include "../FogLocal.h"
#include "StringPack.h"
#include "BitManip.h"
#include "D2Macros.h"

#define STRPACK_CODE(pStrPack, c)	(((BYTE *)(pStrPack) + 4)[(signed char)(c)])

void __stdcall BitManipInitBuffer(BITBUFFER *ptBuffer, BYTE *pbData, int nBytes);
void __stdcall BitManipWrite(BITBUFFER *ptBuffer, int nValue, int nBits);
DWORD __stdcall BitManipReadUnsigned(BITBUFFER *ptBuffer, int nBits);

static __inline int StringPackBitsNeeded(int nCount)
{
#line 26
	D2ASSERTMSG(nCount > 0 && nCount <= 256, "Tried to create a StrPack with an illegal size");
	if (nCount <= 16)
	{
		if (nCount <= 4)
			return (nCount > 2) + 1;
		return (nCount > 8) + 3;
	}
	if (nCount <= 64)
		return (nCount > 32) + 5;
	return (nCount > 128) + 7;
}

void __stdcall StringPackCreate(STRPACK *pStrPack, int nMaxBits, const char *szAlphabet)
{
	BYTE bCount;
	const char *pszCur;
	int i;

#line 65
	D2ASSERT(pStrPack);
	pszCur = szAlphabet;
#line 66
	D2ASSERTMSG(*szAlphabet, "Tried to create a StrPack with no characters");
	for (i = 0; i < (int)(sizeof(STRPACK) / sizeof(DWORD)); i++)
		((DWORD *)pStrPack)[i] = 0;
	bCount = 0;
	pStrPack->abCodes[0] = 0;
	pStrPack->abChars[0] = 0;
	while (*pszCur)
	{
		if (STRPACK_CODE(pStrPack, *pszCur))
		{
			pszCur++;
			continue;
		}
		bCount++;
#line 82
		D2ASSERTMSG(bCount < (1 << nMaxBits), "Tried to create StringPack with illegal size");
		STRPACK_CODE(pStrPack, *pszCur) = bCount;
		pStrPack->abChars[bCount] = *pszCur++;
	}
	pStrPack->nPackedBits = StringPackBitsNeeded(bCount);
}

void __stdcall StringPackPack(const STRPACK *pStrPack, const char *szString, BYTE *pbBuffer, int nBytes,
	BOOL fReserveEnd)
{
	BITBUFFER tBuffer;
	int nChars = 0;
	int nMax;
	int nBits;

#line 110
	D2ASSERT(pStrPack);
#line 111
	D2ASSERT(szString);
#line 112
	D2ASSERT(pbBuffer);
#line 113
	D2ASSERT(pStrPack->nPackedBits);
	BitManipInitBuffer(&tBuffer, pbBuffer, nBytes);
	nBits = pStrPack->nPackedBits;
	nMax = (nBits * nBytes + 7) >> 3;
	if (fReserveEnd)
		nMax--;
	for (; *szString; szString++)
	{
		if (++nChars >= nMax)
			break;
		BitManipWrite(&tBuffer, (signed char)STRPACK_CODE(pStrPack, *szString), nBits);
	}
	if (fReserveEnd || nChars < nMax)
		BitManipWrite(&tBuffer, 0, nBits);
}

void __stdcall StringPackUnpack(const STRPACK *pStrPack, char *szString, BYTE *pbBuffer, int nBytes,
	int nMaxChars)
{
	BITBUFFER tBuffer;
	int nOut = 0;
	int nCodes;
	int nBits;
	BYTE bCode;

#line 151
	D2ASSERT(pStrPack);
#line 152
	D2ASSERT(szString);
#line 153
	D2ASSERT(pbBuffer);
#line 154
	D2ASSERT(pStrPack->nPackedBits);
	nMaxChars--;
	nCodes = (nBytes * 8 + 7) / pStrPack->nPackedBits;
	if (nCodes < nMaxChars)
		nMaxChars = nCodes;
	BitManipInitBuffer(&tBuffer, pbBuffer, nBytes);
	nBits = pStrPack->nPackedBits;
	while (nOut < nMaxChars)
	{
		bCode = (BYTE)BitManipReadUnsigned(&tBuffer, nBits);
		szString[nOut++] = ((BYTE *)pStrPack + 259)[(signed char)bCode];
		if (!bCode)
			return;
	}
	szString[nOut] = 0;
}
