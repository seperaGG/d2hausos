#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\RANDOM\\Random.cpp"
#include <time.h>
#include "../D2Common.h"
#include "D2Macros.h"

#define SEED_MULTIPLIER		1791398085
#define SEED_CARRY		666

static inline DWORD SeedAdvance(D2SEED *ptSeed)
{
	unsigned __int64 qwNext;

	qwNext = (unsigned __int64)ptSeed->dwLow * SEED_MULTIPLIER + ptSeed->dwHigh;
	ptSeed->dwLow = (DWORD)qwNext;
	ptSeed->dwHigh = (DWORD)(qwNext >> 32);
	return ptSeed->dwLow;
}

DWORD __fastcall SeedInvent(DWORD nSalt)
{
	return ((nSalt + time(NULL) + GetTickCount()) * 793315989 - 775096599) & 2147483647;
}

void __fastcall SeedInit(D2SEED *ptSeed)
{
	ptSeed->dwLow = 1;
	ptSeed->dwHigh = SEED_CARRY;
}

DWORD __fastcall SeedNext(D2SEED *ptSeed)
{
	return SeedAdvance(ptSeed);
}

void __fastcall SeedSet(D2SEED *ptSeed, DWORD dwSeed)
{
	ptSeed->dwLow = dwSeed;
	ptSeed->dwHigh = SEED_CARRY;
}

DWORD __fastcall SeedGetLow(D2SEED *ptSeed)
{
	return ptSeed->dwLow;
}

void __fastcall SeedSetState(D2SEED *ptSeed, DWORD dwLow, DWORD dwHigh)
{
	ptSeed->dwLow = dwLow;
	ptSeed->dwHigh = dwHigh;
}

void __fastcall SeedGetState(D2SEED *ptSeed, DWORD *pdwLow, DWORD *pdwHigh)
{
	*pdwLow = ptSeed->dwLow;
	*pdwHigh = ptSeed->dwHigh;
}

DWORD __fastcall SeedGetHigh(D2SEED *ptSeed)
{
	return ptSeed->dwHigh;
}

void __fastcall SeedNothing(void)
{
}

int __fastcall SeedBetween(D2SEED *ptSeed, int nLowBound, int nHighBound)
{
	DWORD dwRange;

#line 197
	D2ASSERT(nHighBound > nLowBound);
	dwRange = nHighBound - nLowBound;
	if (dwRange & (dwRange - 1))
		return SeedAdvance(ptSeed) % dwRange + nLowBound;
	return (SeedAdvance(ptSeed) & (dwRange - 1)) + nLowBound;
}

DWORD __fastcall SeedRand(D2SEED *ptSeed, DWORD nMax)
{
#line 212
	D2ASSERT((int)nMax > 0);
	if (nMax & (nMax - 1))
		return SeedAdvance(ptSeed) % nMax;
	return SeedAdvance(ptSeed) & (nMax - 1);
}

DWORD __fastcall RandomNothingZero(void)
{
	return 0;
}
