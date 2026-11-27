#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\Src\\CelDataHash.cpp"
#include "D2Cmp.h"
#include "StormImports.h"
#include "D2Macros.h"
#line 11

#define CELDATAHASH_SIZE	512

void __fastcall CelDataHashInit(D2CELDATAHASH *ptHashTable)
{
	memset(ptHashTable, 0, sizeof(D2CELDATAHASH));
}

static __inline DWORD CelDataHashKey(const char *pszFileName)
{
	DWORD dwHash;

#line 29
	D2ASSERT(pszFileName[0]);
	dwHash = StormSStrHash(pszFileName, 1, 0);
	return ((((dwHash & 33488896) ^ (dwHash >> 8)) >> 8 ^ (dwHash & 130816)) >> 8) ^ (dwHash & 511);
}

static __inline void CelDataHashFreeElement(void *pPool, D2CELDATAHASH *ptHashTable,
	D2CELDATAELEMENT *ptElement)
{
	if (ptHashTable->ptBuckets[ptElement->dwKey] == ptElement)
		ptHashTable->ptBuckets[ptElement->dwKey] = NULL;
#line 55
	D2FREEPOOL(pPool, ptElement);
}

void __fastcall CelDataHashFreeAll(void *pPool, D2CELDATAHASH *ptHashTable)
{
	D2CELDATAELEMENT *ptElement;
	D2CELDATAELEMENT *ptNext;
	DWORD dwKey;

	for (dwKey = 0; dwKey < CELDATAHASH_SIZE; dwKey++)
	{
		for (ptElement = ptHashTable->ptBuckets[dwKey]; ptElement; ptElement = ptNext)
		{
			ptNext = ptElement->ptNext;
			CelDataHashFreeElement(pPool, ptHashTable, ptElement);
		}
#line 88
		D2ASSERT(ptHashTable->nElements[dwKey] == 0);
	}
}

D2CELDATAELEMENT * __fastcall CelDataHashFind(D2CELDATAHASH *ptHashTable, const char *pszFileName)
{
	D2CELDATAELEMENT *ptElement;

	for (ptElement = ptHashTable->ptBuckets[CelDataHashKey(pszFileName)]; ptElement;
		ptElement = ptElement->ptNext)
	{
		if (!strcmp(pszFileName, ptElement->szFileName))
			return ptElement;
	}
	return NULL;
}

D2CELDATAELEMENT * __fastcall CelDataHashAdd(void *pPool, D2CELDATAHASH *ptHashTable, D2LRUBLOCK *lpBlock,
	const char *pszFileName)
{
	D2CELDATAELEMENT *ptElement;
	DWORD dwKey;

#line 128
	ptElement = (D2CELDATAELEMENT *)D2ALLOCPOOL(pPool, sizeof(D2CELDATAELEMENT));
	ptElement->lpBlock = lpBlock;
	dwKey = CelDataHashKey(pszFileName);
	ptElement->ptPrev = NULL;
	ptElement->ptNext = ptHashTable->ptBuckets[dwKey];
	if (ptElement->ptNext)
		ptElement->ptNext->ptPrev = ptElement;
	ptElement->dwKey = dwKey;
	strcpy(ptElement->szFileName, pszFileName);
	ptHashTable->ptBuckets[dwKey] = ptElement;
	return ptElement;
}

void __fastcall CelDataHashRemove(void *pPool, D2CELDATAHASH *ptHashTable, D2CELDATAELEMENT *ptElement)
{
#line 169
	D2ASSERT(ptElement);
	if (ptElement->ptPrev)
		ptElement->ptPrev->ptNext = ptElement->ptNext;
	if (ptElement->ptNext)
		ptElement->ptNext->ptPrev = ptElement->ptPrev;
	CelDataHashFreeElement(pPool, ptHashTable, ptElement);
}

void __fastcall CelDataHashNothing(D2CELDATAHASH *ptHashTable)
{
}
