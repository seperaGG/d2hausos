#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\Src\\Count.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 10

static __inline int CountSameBody(const BYTE *pSrc, const BYTE *pEnd, int nMax, int *pnAtEnd, int *pnRestSame)
{
	BYTE bValue;
	int nCount;

	bValue = *pSrc;
	nCount = 0;
	while (1)
	{
		if (bValue != *pSrc || nCount >= nMax)
			break;
		nCount++;
		pSrc++;
		if (pSrc >= pEnd)
			break;
	}
	if (pSrc == pEnd)
	{
		*pnAtEnd = 1;
		*pnRestSame = 0;
		return nCount;
	}
	*pnAtEnd = 0;
	while (pSrc < pEnd && bValue == *pSrc)
		pSrc++;
	*pnRestSame = pSrc == pEnd;
	return nCount;}

int __fastcall CountSame(const BYTE *pSrc, const BYTE *pEnd, int nMax, int *pnAtEnd, int *pnRestSame)
{
#line 25
	D2ASSERT(pSrc < pEnd);
	return CountSameBody(pSrc, pEnd, nMax, pnAtEnd, pnRestSame);
}

static __inline int CountValueBody(BYTE bValue, const BYTE *pSrc, const BYTE *pEnd, int nMax, int *pnAtEnd,
	int *pnRestSame)
{
	int nCount;

	nCount = 0;
	while (1)
	{
		if (bValue != *pSrc || nCount >= nMax)
			break;
		nCount++;
		pSrc++;
		if (pSrc >= pEnd)
			break;
	}
	if (pSrc == pEnd)
	{
		*pnAtEnd = 1;
		*pnRestSame = 0;
		return nCount;
	}
	*pnAtEnd = 0;
	while (pSrc < pEnd && bValue == *pSrc)
		pSrc++;
	*pnRestSame = pSrc == pEnd;
	return nCount;}

int __fastcall CountValue(BYTE bValue, const BYTE *pSrc, const BYTE *pEnd, int nMax, int *pnAtEnd,
	int *pnRestSame)
{
#line 69
	D2ASSERT(pSrc < pEnd);
	return CountValueBody(bValue, pSrc, pEnd, nMax, pnAtEnd, pnRestSame);
}

static __inline int CountUntilBody(const BYTE *pSrc, const BYTE *pEnd, BYTE bValue, int nMax, int *pnAtEnd,
	int *pnRestSame)
{
	int nCount;

	nCount = 0;
	while (1)
	{
		if (*pSrc == bValue || nCount >= nMax)
			break;
		nCount++;
		pSrc++;
		if (pSrc >= pEnd)
			break;
	}
	if (pSrc == pEnd)
	{
		*pnAtEnd = 1;
		*pnRestSame = 0;
		return nCount;
	}
	*pnAtEnd = 0;
	while (pSrc < pEnd && *pSrc == bValue)
		pSrc++;
	*pnRestSame = pSrc == pEnd;
	return nCount;}

int __fastcall CountUntil(const BYTE *pSrc, const BYTE *pEnd, BYTE bValue, int nMax, int *pnAtEnd,
	int *pnRestSame)
{
#line 111
	D2ASSERT(pSrc < pEnd);
	return CountUntilBody(pSrc, pEnd, bValue, nMax, pnAtEnd, pnRestSame);
}
