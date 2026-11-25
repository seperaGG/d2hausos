#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\Mem\\mem.cpp"
#include "FogLocal.h"
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "D2Macros.h"

#define D2POOL_MAX_POOLS	40

extern "C" const DWORD gdwBitMasks[32];
extern "C" const DWORD gdwInvBitMasks[32];

#define SETBITDWORD(pdw, n)		((pdw)[(n) >> 5] |= gdwBitMasks[(n) & 31])
#define CLEARBITDWORD(pdw, n)	((pdw)[(n) >> 5] &= gdwInvBitMasks[(n) & 31])

struct D2POOL;

struct D2POOLBLOCK
{
 	BYTE		*pbElements;
 	DWORD		*pUsageTable;
 	int			nUsedElements;
 	D2POOLBLOCK	*ptPrev;
 	D2POOLBLOCK	*ptNext;
 	D2POOL		*ptPool;
};

struct D2POOL
{
 	CRITICAL_SECTION	tLock;
 	int			nElementSize;
 	int			nMemBlocksPerPool;
 	int			nBlockBytes;
 	int			nNumPoolBlocks;
 	D2POOLBLOCK	*ptHead;
 	D2POOLBLOCK	*pTail;
};

struct D2POOLTREE
{
 	D2POOLBLOCK	*ptPoolBlock;
 	BYTE		*pbElements;
};

struct D2POOLSYSTEM
{
 	BOOL		fDisabled;
 	CRITICAL_SECTION	tLock;
 	int			nPools;
 	D2POOL		*ptPools;
 	int			nNumTreeEntries;
 	int			nMaxPoolBlocks;
 	D2POOLTREE	*ptBlockTree;
 	DWORD		*apdwLargeBlocks[1023];
};

static const DWORD sgcdwPartialMasks[33] =
{
	0, 1, 3, 7, 15, 31, 63, 127,
	255, 511, 1023, 2047, 4095, 8191, 16383, 32767,
	65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607,
	16777215, 33554431, 67108863, 134217727, 268435455, 536870911, 1073741823, 2147483647,
	0xFFFFFFFF,
};

static D2POOLSYSTEM sgtDefaultPoolSystem = { 0 };
static D2POOLSYSTEM *sgptDefaultPoolSystem = &sgtDefaultPoolSystem;
static BOOL sgfPoolSystemInit = 0;
DWORD gdwTraceUnfreedMemory = 0;

void __cdecl MemPoolSystemDestroy(D2POOLSYSTEM *ptPoolSystem);
DWORD __cdecl MemPoolSystemInit(D2POOLSYSTEM **pptPoolSystem, int nLargestSize, int nMaxPoolBlocks);

static __inline DWORD TESTBITDWORD(DWORD *pdw, int n)
{
	return pdw[n >> 5] & gdwBitMasks[n & 31];
}

static __inline int MemLargeHash(DWORD *pdwBlock)
{
	return (int)(INT_PTR)pdwBlock % 1023;
}

static __inline int MemBitLength(unsigned int nValue)
{
	if (!nValue)
		return 0;
	__asm bsr eax, nValue
	__asm inc eax
	__asm mov nValue, eax
	return nValue;
}

static __inline int MemGetPoolClass(int nSize)
{
	return nSize < 3 ? 1 : MemBitLength(nSize - 1);
}

#define MEMFIRSTCLEARBIT(nBit, dwWord) \
	{ \
		nBit = (dwWord); \
		__asm mov ebx, nBit \
		__asm not ebx \
		__asm bsf eax, ebx \
		__asm mov nBit, eax \
	}

extern __inline void * __fastcall MemLargeAlloc(D2POOLSYSTEM *ptPoolSystem, int nSize, const char *pszFile, int nLine)
{
	DWORD *pdwBlock;
	int nHash;

	pdwBlock = (DWORD *)malloc(nSize + 4);
	if (!pdwBlock)
		return NULL;
	nHash = MemLargeHash(pdwBlock);
	EnterCriticalSection(&ptPoolSystem->tLock);
	*pdwBlock = (DWORD)(INT_PTR)ptPoolSystem->apdwLargeBlocks[nHash];
	ptPoolSystem->apdwLargeBlocks[nHash] = pdwBlock;
	LeaveCriticalSection(&ptPoolSystem->tLock);
	return pdwBlock + 1;
}

extern __inline void __fastcall MemLargeFree(D2POOLSYSTEM *ptPoolSystem, void *pMemory, const char *pszFile, int nLine)
{
	DWORD *pdwPrev;
	DWORD *pdwScan;
	int nHash;

	pMemory = (DWORD *)pMemory - 1;
	pdwPrev = NULL;
	nHash = MemLargeHash((DWORD *)pMemory);
	EnterCriticalSection(&ptPoolSystem->tLock);
	for (pdwScan = ptPoolSystem->apdwLargeBlocks[nHash]; pdwScan; pdwScan = (DWORD *)(INT_PTR)*pdwScan)
	{
		if (pdwScan == pMemory)
		{
			if (!pdwPrev)
				ptPoolSystem->apdwLargeBlocks[nHash] = (DWORD *)(INT_PTR)*pdwScan;
			else
				*pdwPrev = *pdwScan;
			LeaveCriticalSection(&ptPoolSystem->tLock);
			free(pMemory);
			return;
		}
		pdwPrev = pdwScan;
	}
	LeaveCriticalSection(&ptPoolSystem->tLock);
	free(pMemory);
}

void * __fastcall MemLargeRealloc(D2POOLSYSTEM *ptPoolSystem, void *pMemory, int nSize, const char *pszFile,
	int nLine)
{
	DWORD *pdwPrev;
	DWORD *pdwScan;
	int nHash;

	pMemory = (DWORD *)pMemory - 1;
	pdwPrev = NULL;
	nHash = MemLargeHash((DWORD *)pMemory);
	EnterCriticalSection(&ptPoolSystem->tLock);
	for (pdwScan = ptPoolSystem->apdwLargeBlocks[nHash]; pdwScan; pdwScan = (DWORD *)(INT_PTR)*pdwScan)
	{
		if (pdwScan == pMemory)
		{
			if (!pdwPrev)
				ptPoolSystem->apdwLargeBlocks[nHash] = (DWORD *)(INT_PTR)*pdwScan;
			else
				*pdwPrev = *pdwScan;
			pMemory = realloc(pMemory, nSize + 4);
			if (!pMemory)
				break;
			nHash = MemLargeHash((DWORD *)pMemory);
			*(DWORD *)pMemory = (DWORD)(INT_PTR)ptPoolSystem->apdwLargeBlocks[nHash];
			ptPoolSystem->apdwLargeBlocks[nHash] = (DWORD *)pMemory;
			LeaveCriticalSection(&ptPoolSystem->tLock);
			return (DWORD *)pMemory + 1;
		}
		pdwPrev = pdwScan;
	}
	LeaveCriticalSection(&ptPoolSystem->tLock);
	return NULL;
}

static __inline void __fastcall MemPoolTreeInsert(D2POOLSYSTEM *ptPoolSystem, D2POOLBLOCK *pPoolBlock);
static void __fastcall MemPoolValidate(D2POOL *pPool);
static D2POOLBLOCK * __fastcall MemPoolBlockNew(D2POOLSYSTEM *ptPoolSystem, D2POOL *pPool);
static int __fastcall MemFindFreeElement(DWORD *pdwUsage, int nElements);

static __inline int MemPoolLocateFree(D2POOLBLOCK **ppPoolBlock, D2POOL *pPool, D2POOLSYSTEM *ptPoolSystem)
{
	D2POOLBLOCK *pPoolBlock;
	int nBlockOffset;

	pPoolBlock = pPool->ptHead;
	if (!pPoolBlock)
	{
		pPoolBlock = MemPoolBlockNew(ptPoolSystem, pPool);
		pPool->ptHead = pPoolBlock;
		pPool->pTail = pPoolBlock;
		*ppPoolBlock = pPoolBlock;
		return 0;
	}
	if (pPoolBlock->nUsedElements < pPool->nMemBlocksPerPool)
	{
		nBlockOffset = MemFindFreeElement(pPoolBlock->pUsageTable, pPool->nMemBlocksPerPool);
		if (nBlockOffset >= 0)
		{
			*ppPoolBlock = pPoolBlock;
			return nBlockOffset;
		}
	}
	pPoolBlock = MemPoolBlockNew(ptPoolSystem, pPool);
	if (pPoolBlock)
	{
		pPoolBlock->ptNext = pPool->ptHead;
		pPool->ptHead->ptPrev = pPoolBlock;
		pPool->ptHead = pPoolBlock;
		MemPoolValidate(pPool);
	}
	*ppPoolBlock = pPoolBlock;
	return 0;
}

void * __fastcall MemPoolAlloc(D2POOLSYSTEM *ptPoolSystem, int nSize, const char *pszFile, int nLine)
{
	D2POOLSYSTEM *ptSystem;
	D2POOL *pPool;
	D2POOLBLOCK *pPoolBlock;
	int nBlockOffset;
	int nClass;
	void *pMemory;

	if (!ptPoolSystem)
		ptSystem = sgptDefaultPoolSystem;
	else
		ptSystem = ptPoolSystem;
	if (!sgfPoolSystemInit)
		MemPoolSystemInit(NULL, 262144, 32768);
	if (ptSystem->fDisabled)
		return MemLargeAlloc(ptSystem, nSize, pszFile, nLine);
	nClass = MemGetPoolClass(nSize);
	if (nClass > ptSystem->nPools)
		return MemLargeAlloc(ptSystem, nSize, pszFile, nLine);
	pPool = &ptSystem->ptPools[nClass - 1];
	EnterCriticalSection(&pPool->tLock);
	nBlockOffset = MemPoolLocateFree(&pPoolBlock, pPool, ptSystem);
#line 986
	D2ASSERT(nBlockOffset < pPool->nMemBlocksPerPool);
	if (!pPoolBlock)
	{
		LeaveCriticalSection(&pPool->tLock);
#line 991
		D2WARNMSG("insufficient memory for allocation allowed by memory system");
		return NULL;
	}
	pPoolBlock->nUsedElements++;
#line 997
	D2ASSERT(!TESTBITDWORD( pPoolBlock->pUsageTable, nBlockOffset ));
	SETBITDWORD(pPoolBlock->pUsageTable, nBlockOffset);
	if (pPoolBlock->nUsedElements >= pPool->nMemBlocksPerPool && pPoolBlock->ptNext
		&& pPoolBlock->ptNext->nUsedElements < pPool->nMemBlocksPerPool)
	{
		if (!pPoolBlock->ptPrev)
			pPool->ptHead = pPoolBlock->ptNext;
		else
			pPoolBlock->ptPrev->ptNext = pPoolBlock->ptNext;
		if (pPoolBlock->ptNext)
			pPoolBlock->ptNext->ptPrev = pPoolBlock->ptPrev;
#line 1011
		D2ASSERT(pPool->pTail);
		pPool->pTail->ptNext = pPoolBlock;
		pPoolBlock->ptPrev = pPool->pTail;
		pPool->pTail = pPoolBlock;
		pPoolBlock->ptNext = NULL; }
	pMemory = pPoolBlock->pbElements + pPool->nElementSize * nBlockOffset;
	LeaveCriticalSection(&pPool->tLock);
	return pMemory;
}

static int __fastcall MemFindFreeElement(DWORD *pdwUsage, int nElements)
{
	int nWords;
	int nBase;
	int nResult;
	int nBit;

	nWords = nElements >> 5;
	nBase = 0;
	nResult = -1;
	for (;;)
	{
		switch (nWords)
		{
		default:
			if (pdwUsage[7] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[7]);
				return nBit + nBase + 224;
			}
		case 7:
			if (pdwUsage[6] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[6]);
				return nBit + nBase + 192;
			}
		case 6:
			if (pdwUsage[5] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[5]);
				return nBit + nBase + 160;
			}
		case 5:
			if (pdwUsage[4] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[4]);
				return nBit + nBase + 128;
			}
		case 4:
			if (pdwUsage[3] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[3]);
				return nBit + nBase + 96;
			}
		case 3:
			if (pdwUsage[2] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[2]);
				return nBit + nBase + 64;
			}
		case 2:
			if (pdwUsage[1] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[1]);
				return nBit + nBase + 32;
			}
		case 1:
			if (pdwUsage[0] != 0xFFFFFFFF)
			{
				MEMFIRSTCLEARBIT(nBit, pdwUsage[0]);
				return nBit + nBase;
			}
		case 0:
			break;
		}
		if (nWords <= 8)
			break;
		nWords -= 8;
		pdwUsage += 8;
		nBase += 256;
	}
	if (!((nElements & 31) && pdwUsage[nWords] != sgcdwPartialMasks[nElements & 31]))
		return nResult;
	MEMFIRSTCLEARBIT(nBit, pdwUsage[nWords]);
	nResult = (nWords << 5) + nBit + nBase;

	return nResult;
}

static __inline void __fastcall MemPoolTreeInsert(D2POOLSYSTEM *ptPoolSystem, D2POOLBLOCK *pPoolBlock)
{
	D2POOLTREE *ptTree;
	BYTE *pbKey;
	int nLow;
	int nHigh;
	int nMid;

	ptTree = ptPoolSystem->ptBlockTree;
	EnterCriticalSection(&ptPoolSystem->tLock);
	if (ptPoolSystem->nNumTreeEntries >= ptPoolSystem->nMaxPoolBlocks)
	{
#line 473
		D2WARN(0);
#line 474
		D2FATAL("Pool Blocks Overflowed");
	}
	nLow = 0;
	nHigh = ptPoolSystem->nNumTreeEntries;
	pbKey = pPoolBlock->pbElements;
	while (nLow < nHigh)
	{
		nMid = nLow + ((nHigh - nLow) >> 1);
		if (pbKey > ptTree[nMid].pbElements)
			nLow = nMid + 1;
		else
			nHigh = nMid;
	}
	if (nLow < ptPoolSystem->nNumTreeEntries)
		memmove(&ptTree[nLow + 1], &ptTree[nLow], (ptPoolSystem->nNumTreeEntries - nLow) * sizeof(D2POOLTREE));
	ptTree[nLow].ptPoolBlock = pPoolBlock;
	ptTree[nLow].pbElements = pPoolBlock->pbElements;
	ptPoolSystem->nNumTreeEntries++;
	LeaveCriticalSection(&ptPoolSystem->tLock);
}

static void __fastcall MemPoolValidate(D2POOL *pPool)
{
}

static __inline D2POOLBLOCK * __fastcall MemPoolBlockNew(D2POOLSYSTEM *ptPoolSystem, D2POOL *pPool)
{
	D2POOLBLOCK *pPoolBlock;
	BYTE *pbElements;
	int nUsageBytes;

	if (ptPoolSystem->nNumTreeEntries >= ptPoolSystem->nMaxPoolBlocks)
	{
#line 599
		D2FATAL("MEMORY ERROR: Pool Blocks Overflowed");
	}
	nUsageBytes = ((pPool->nMemBlocksPerPool + 31) >> 5) * 4;
	pPoolBlock = (D2POOLBLOCK *)malloc(nUsageBytes + sizeof(D2POOLBLOCK));
	pbElements = (BYTE *)malloc(pPool->nBlockBytes);
	if (!pPoolBlock || !pbElements)
	{
#line 627
		D2FATAL("MEMORY ERROR: Out of Memory");
	}
	memset(pPoolBlock, 0, sizeof(D2POOLBLOCK));
	pPoolBlock->pUsageTable = (DWORD *)(pPoolBlock + 1);
	memset(pPoolBlock->pUsageTable, 0, nUsageBytes);
	pPoolBlock->pbElements = pbElements;
	pPoolBlock->ptPool = pPool;
	pPool->nNumPoolBlocks++;
	MemPoolTreeInsert(ptPoolSystem, pPoolBlock);
	return pPoolBlock;
}

static __inline D2POOLBLOCK *MemPoolTreeFind(D2POOLSYSTEM *ptPoolSystem, BYTE *pMemory)
{
	D2POOLTREE *ptTree;
	D2POOLBLOCK *pPoolBlock;
	int nLow;
	int nHigh;
	int nMid;

	ptTree = ptPoolSystem->ptBlockTree;
	EnterCriticalSection(&ptPoolSystem->tLock);

	nLow = 0;
	nHigh = ptPoolSystem->nNumTreeEntries;
	while (nLow < nHigh)
	{
		nMid = nLow + ((nHigh - nLow) >> 1);
		if (pMemory >= ptTree[nMid].pbElements)
			nLow = nMid + 1;
		else
			nHigh = nMid;
	}
	nMid = nLow - 1;
	if (nMid < 0 || nMid >= ptPoolSystem->nNumTreeEntries)
		pPoolBlock = NULL;
	else
		pPoolBlock = ptTree[nMid].ptPoolBlock;
	LeaveCriticalSection(&ptPoolSystem->tLock);
	return pPoolBlock;
}

static void __fastcall MemPoolBlockRelease(D2POOLSYSTEM *ptPoolSystem, D2POOL *pPool, D2POOLBLOCK *pPoolBlock);

void __fastcall MemPoolFree(D2POOLSYSTEM *ptPoolSystem, void **ppMemory, const char *pszFile, int nLine)
{
	D2POOLSYSTEM *ptSystem;
	D2POOLBLOCK *pPoolBlock;
	D2POOL *pPool;
	int nBlockOffset;

	if (!ptPoolSystem)
		ptSystem = sgptDefaultPoolSystem;
	else
		ptSystem = ptPoolSystem;
	if (!*ppMemory)
		return;
	pPoolBlock = MemPoolTreeFind(ptSystem, (BYTE *)*ppMemory);
	if (pPoolBlock && (BYTE *)*ppMemory >= pPoolBlock->pbElements)
	{
		pPool = pPoolBlock->ptPool;
		nBlockOffset = (int)((BYTE *)*ppMemory - pPoolBlock->pbElements) / pPool->nElementSize;
		if (nBlockOffset >= pPool->nMemBlocksPerPool)
		{
			MemLargeFree(ptSystem, *ppMemory, pszFile, nLine);
			return;
		}
		EnterCriticalSection(&pPool->tLock);
		if (TESTBITDWORD(pPoolBlock->pUsageTable, nBlockOffset))
		{
			CLEARBITDWORD(pPoolBlock->pUsageTable, nBlockOffset);
			pPoolBlock->nUsedElements--;
			MemPoolBlockRelease(ptSystem, pPool, pPoolBlock);
			LeaveCriticalSection(&pPool->tLock);
		}
		else
			LeaveCriticalSection(&pPool->tLock);
		return;
	}
	MemLargeFree(ptSystem, *ppMemory, pszFile, nLine);
}

static __inline void MemPoolBlockDelete(D2POOLSYSTEM *ptPoolSystem, D2POOLBLOCK *pPoolBlock)
{
	D2POOLTREE *ptTree;
	BYTE *pbElements;
	int nLow;
	int nHigh;
	int nMid;
	BOOL fFound;

	ptTree = ptPoolSystem->ptBlockTree;
	EnterCriticalSection(&ptPoolSystem->tLock);
	fFound = FALSE;
	nLow = 0;
	nHigh = ptPoolSystem->nNumTreeEntries;
	pbElements = pPoolBlock->pbElements;
	while (nLow < nHigh)
	{
		nMid = nLow + ((nHigh - nLow) >> 1);
		if (pbElements > ptTree[nMid].pbElements)
			nLow = nMid + 1;
		else if (pbElements >= ptTree[nMid].pbElements)
		{
			fFound = TRUE;
			break;
		}
		else
			nHigh = nMid;
	}
	if (fFound)
	{
		if (nMid < ptPoolSystem->nNumTreeEntries - 1)
			memmove(&ptTree[nMid], &ptTree[nMid + 1],
				(ptPoolSystem->nNumTreeEntries - (nMid + 1)) * sizeof(D2POOLTREE));
		ptPoolSystem->nNumTreeEntries--;
		LeaveCriticalSection(&ptPoolSystem->tLock);
	}
	else
	{
		LeaveCriticalSection(&ptPoolSystem->tLock);
#line 540
		D2WARNMSG("Unable to find pool block in allocation tree!");
	}
	free(pPoolBlock->pbElements);
	free(pPoolBlock);
}

static void __fastcall MemPoolBlockRelease(D2POOLSYSTEM *ptPoolSystem, D2POOL *pPool, D2POOLBLOCK *pPoolBlock)
{
	if (pPoolBlock->ptPrev && pPoolBlock->ptPrev->nUsedElements >= pPool->nMemBlocksPerPool)
	{
		pPoolBlock->ptPrev->ptNext = pPoolBlock->ptNext;
		if (pPoolBlock->ptNext)
			pPoolBlock->ptNext->ptPrev = pPoolBlock->ptPrev;
		else
			pPool->pTail = pPoolBlock->ptPrev;
		pPoolBlock->ptNext = pPool->ptHead;
		pPool->ptHead->ptPrev = pPoolBlock;
		pPool->ptHead = pPoolBlock;
		pPoolBlock->ptPrev = NULL;
	}
	if (pPoolBlock->nUsedElements <= 0 && pPoolBlock->ptNext
		&& pPoolBlock->ptNext->nUsedElements < pPool->nMemBlocksPerPool)
	{
		if (pPoolBlock->ptPrev)
			pPoolBlock->ptPrev->ptNext = pPoolBlock->ptNext;
		if (pPoolBlock->ptNext)
			pPoolBlock->ptNext->ptPrev = pPoolBlock->ptPrev;
		if (pPool->ptHead == pPoolBlock)
			pPool->ptHead = pPoolBlock->ptNext;
		if (pPool->pTail == pPoolBlock)
			pPool->pTail = pPoolBlock->ptPrev;
		pPool->nNumPoolBlocks--;
		LeaveCriticalSection(&pPool->tLock);
		MemPoolBlockDelete(ptPoolSystem, pPoolBlock);
		EnterCriticalSection(&pPool->tLock);
	}
}

void * __fastcall MemPoolRealloc(D2POOLSYSTEM *ptPoolSystem, void *pMemory, int nSize, const char *pszFile,
	int nLine)
{
	D2POOLSYSTEM *ptSystem;
	D2POOLBLOCK *pPoolBlock;
	D2POOL *pPool;
	int nBlockOffset;
	void *pNew;

	if (!ptPoolSystem)
		ptSystem = sgptDefaultPoolSystem;
	else
		ptSystem = ptPoolSystem;
	pNew = NULL;
	if (!pMemory)
	{
		pNew = MemPoolAlloc(ptPoolSystem, nSize, pszFile, nLine);
#line 1266
		D2ASSERT(pNew);
		return pNew;
	}
	pPoolBlock = MemPoolTreeFind(ptSystem, (BYTE *)pMemory);
	if (!pPoolBlock)
	{
		pNew = MemLargeRealloc(ptSystem, pMemory, nSize, pszFile, nLine);
#line 1274
		D2ASSERT(pNew);
		return pNew;
	}
	if ((BYTE *)pMemory < pPoolBlock->pbElements)
	{
		pNew = MemLargeRealloc(ptSystem, pMemory, nSize, pszFile, nLine);
#line 1281
		D2ASSERT(pNew);
		return pNew;
	}
	pPool = pPoolBlock->ptPool;
	nBlockOffset = (int)((BYTE *)pMemory - pPoolBlock->pbElements) / pPool->nElementSize;
	if (nBlockOffset >= pPool->nMemBlocksPerPool)
	{
		pNew = MemLargeRealloc(ptSystem, pMemory, nSize, pszFile, nLine);
#line 1293
		D2ASSERT(pNew);
		return pNew;
	}
	EnterCriticalSection(&pPool->tLock);
	if (TESTBITDWORD(pPoolBlock->pUsageTable, nBlockOffset))
	{
		if (pPool->nElementSize >= nSize)
			pNew = pMemory;
		else
		{
			pNew = MemPoolAlloc(ptPoolSystem, nSize, pszFile, nLine);
#line 1337
			D2ASSERT(pNew);
			memcpy(pNew, pMemory, pPool->nElementSize);
			CLEARBITDWORD(pPoolBlock->pUsageTable, nBlockOffset);
			pPoolBlock->nUsedElements--;
			MemPoolBlockRelease(ptSystem, pPool, pPoolBlock);
		}
		LeaveCriticalSection(&pPool->tLock);
	}
	else
	{
		LeaveCriticalSection(&pPool->tLock);
#line 1371
		D2WARNMSG("pool realloc was called for a block that has already been freed");
	}
#line 1374
	if (!pNew) D2WARNMSG("Failure to reallocate memory");
	return pNew;
}

void __cdecl MemPoolSystemCompact(D2POOLSYSTEM *ptPoolSystem)
{
	D2POOLSYSTEM *ptSystem;
	D2POOL *pPool;
	D2POOLBLOCK *pPoolBlock;
	D2POOLBLOCK *pNext;
	int i;

	if (!ptPoolSystem)
		ptSystem = sgptDefaultPoolSystem;
	else
		ptSystem = ptPoolSystem;
	if (!ptSystem->nPools)
		return;
	EnterCriticalSection(&ptSystem->tLock);
	for (i = 0; i < ptSystem->nPools; i++)
	{
		pPool = &ptSystem->ptPools[i];
		EnterCriticalSection(&pPool->tLock);
		for (pPoolBlock = pPool->ptHead; pPoolBlock; pPoolBlock = pNext)
		{
			pNext = pPoolBlock->ptNext;
			if (pPoolBlock->nUsedElements)
				continue;
			if (pPoolBlock->ptPrev)
				pPoolBlock->ptPrev->ptNext = pNext;
			if (pPoolBlock->ptNext)
				pPoolBlock->ptNext->ptPrev = pPoolBlock->ptPrev;
			if (pPool->ptHead == pPoolBlock)
				pPool->ptHead = pPoolBlock->ptNext;
			if (pPool->pTail == pPoolBlock)
				pPool->pTail = pPoolBlock->ptPrev;
			pPool->nNumPoolBlocks--;
			MemPoolBlockDelete(ptSystem, pPoolBlock);
		}
		LeaveCriticalSection(&pPool->tLock);
	}
	LeaveCriticalSection(&ptSystem->tLock);
}

void __cdecl MemPoolSystemDestroy(D2POOLSYSTEM *ptPoolSystem)
{
	D2POOL *pPool;
	D2POOLBLOCK *pPoolBlock;
	D2POOLBLOCK *pNext;
	DWORD *pdwBlock;
	DWORD *pdwNext;
	int i;

	if (!ptPoolSystem)
		ptPoolSystem = sgptDefaultPoolSystem;
	if (!ptPoolSystem->nPools)
		return;
	EnterCriticalSection(&ptPoolSystem->tLock);
	if (!ptPoolSystem->nPools)
		LeaveCriticalSection(&ptPoolSystem->tLock);
	else
	{
		for (i = 0; i < ptPoolSystem->nPools; i++)
		{
			EnterCriticalSection(&ptPoolSystem->ptPools[i].tLock);
			for (pPoolBlock = ptPoolSystem->ptPools[i].ptHead; pPoolBlock; pPoolBlock = pNext)
			{
				pNext = pPoolBlock->ptNext;
				free(pPoolBlock->pbElements);
				free(pPoolBlock);
			}
			DeleteCriticalSection(&ptPoolSystem->ptPools[i].tLock);
		}
		free(ptPoolSystem->ptPools);
		free(ptPoolSystem->ptBlockTree);
		for (i = 0; i < 1023; i++)
		{
			for (pdwBlock = ptPoolSystem->apdwLargeBlocks[i]; pdwBlock; pdwBlock = pdwNext)
			{
				pdwNext = (DWORD *)(INT_PTR)*pdwBlock;
				free(pdwBlock);
			}
		}
		DeleteCriticalSection(&ptPoolSystem->tLock);
		for (i = 0; i < 1035; i++)
			((DWORD *)ptPoolSystem)[i] = 0;
	}
	if (ptPoolSystem != &sgtDefaultPoolSystem)
		free(ptPoolSystem);
}

DWORD __cdecl MemPoolSystemInit(D2POOLSYSTEM **pptPoolSystem, int nLargestSize, int nMaxPoolBlocks)
{
	D2POOLSYSTEM *ptPoolSystem;
	int nPools;
	int nTotal;
	int nElementSize;
	int i;

	if (!pptPoolSystem)
	{
		ptPoolSystem = sgptDefaultPoolSystem;
		sgfPoolSystemInit = TRUE;
		MemPoolSystemDestroy(ptPoolSystem);
	}
	else
	{
		ptPoolSystem = (D2POOLSYSTEM *)malloc(sizeof(D2POOLSYSTEM));
		memset(ptPoolSystem, 0, sizeof(D2POOLSYSTEM));
		*pptPoolSystem = ptPoolSystem;
	}
	InitializeCriticalSection(&ptPoolSystem->tLock);
	EnterCriticalSection(&ptPoolSystem->tLock);
	nPools = MemGetPoolClass(nLargestSize);
#line 1757
	D2ASSERT(nPools <= D2POOL_MAX_POOLS);
	ptPoolSystem->nPools = nPools;
	ptPoolSystem->nMaxPoolBlocks = nMaxPoolBlocks;
	ptPoolSystem->ptPools = (D2POOL *)malloc(nPools * sizeof(D2POOL));
	ptPoolSystem->ptBlockTree = (D2POOLTREE *)malloc(nMaxPoolBlocks * sizeof(D2POOLTREE));
	memset(ptPoolSystem->ptPools, 0, nPools * sizeof(D2POOL));
	memset(ptPoolSystem->ptBlockTree, 0, nMaxPoolBlocks * sizeof(D2POOLTREE));
	nTotal = 1 << nPools;
	for (i = 0; i < nPools; i++)
	{
		InitializeCriticalSection(&ptPoolSystem->ptPools[i].tLock);
		nElementSize = 1 << (i + 1);
		ptPoolSystem->ptPools[i].nElementSize = nElementSize;
		ptPoolSystem->ptPools[i].nMemBlocksPerPool =
			nElementSize <= 1000000 && (nElementSize << 8) < nTotal ? 256 : nTotal / nElementSize;

		ptPoolSystem->ptPools[i].nBlockBytes = ptPoolSystem->ptPools[i].nMemBlocksPerPool
			* ptPoolSystem->ptPools[i].nElementSize;
	}
	memset(ptPoolSystem->apdwLargeBlocks, 0, sizeof(ptPoolSystem->apdwLargeBlocks));
	LeaveCriticalSection(&ptPoolSystem->tLock);
	return 0;
}

void __cdecl MemPoolSystemDisable(D2POOLSYSTEM *ptPoolSystem, BOOL fDisable)
{
	if (!ptPoolSystem)
		ptPoolSystem = sgptDefaultPoolSystem;
	if (!ptPoolSystem->nPools)
		return;
	EnterCriticalSection(&ptPoolSystem->tLock);
	ptPoolSystem->fDisabled = fDisable;
	LeaveCriticalSection(&ptPoolSystem->tLock);
}
