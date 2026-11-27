#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\LRUCache.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 10

void __fastcall LRUCacheInit(D2LRUCACHE *ptCache, void *pPool, DWORD dwMaxAllocated,
	void (__fastcall *pfnFree)(D2LRUBLOCK *ptBlock), DWORD dwMaxBlocks)
{
	int i;
	D2LRUBLOCK *ptBlocks;

	ptCache->pMemPool = pPool;
	ptCache->dwMaxAllocated = dwMaxAllocated;
	ptCache->dwAllocated = 0;
	ptCache->field_28 = pfnFree;
	ptCache->dwMaxBlocks = dwMaxBlocks;
#line 47
	ptCache->ptBlocks = (D2LRUBLOCK *)D2ALLOCPOOL(ptCache->pMemPool, ptCache->dwMaxBlocks * sizeof(D2LRUBLOCK));
	memset(ptCache->ptBlocks, 0, dwMaxBlocks * sizeof(D2LRUBLOCK));
	for (i = 0; (DWORD)i < ptCache->dwMaxBlocks; i++)
	{
		ptBlocks = ptCache->ptBlocks;
		if ((DWORD)i < ptCache->dwMaxBlocks - 1)
			ptBlocks[i].ptNext = &ptBlocks[i + 1];
		if (i > 0)
		{
			ptBlocks[i].ptPrev = &ptCache->ptBlocks[i - 1];
		}
		ptBlocks[i].lpData = NULL;
		ptBlocks[i].dwSize = 0;
		ptBlocks[i].pOwner = NULL;
	}
	ptCache->dwNumUsedBlocks = 0;
	ptCache->ptUsedHead = NULL;
	ptCache->ptUsedTail = NULL;
	ptCache->ptFreeHead = ptCache->ptBlocks;
	ptCache->ptFreeTail = &ptCache->ptBlocks[ptCache->dwMaxBlocks - 1];
	ptCache->dwNumAllocs = 0;
	ptCache->dwNumUses = 0;
}

void __fastcall LRUCacheDestroy(D2LRUCACHE *ptCache)
{
	while (ptCache->dwNumUsedBlocks)
		LRUCacheFree(ptCache, NULL);
	if (ptCache->ptBlocks)
	{
#line 96
		D2FREEPOOL(ptCache->pMemPool, ptCache->ptBlocks);
		ptCache->ptBlocks = NULL;
	}
}

static __inline D2LRUBLOCK *LRUCacheTakeFree(D2LRUCACHE *ptCache)
{
	D2LRUBLOCK *ptBlock;

	ptBlock = ptCache->ptFreeHead;
	if (!ptBlock)
		return NULL;
	ptCache->ptFreeHead = ptBlock->ptNext;
	if (ptCache->ptFreeHead)
		ptCache->ptFreeHead->ptPrev = NULL;
	else
		ptCache->ptFreeTail = NULL;
	ptBlock->ptNext = NULL;
	ptBlock->ptPrev = NULL;
	return ptBlock;
}

static __inline void LRUCacheAppendUsed(D2LRUCACHE *ptCache, D2LRUBLOCK *ptBlock)
{
	if (!ptCache->ptUsedTail)
	{
		ptCache->ptUsedHead = ptBlock;
		ptCache->ptUsedTail = ptBlock;
		return;
	}
	ptCache->ptUsedTail->ptNext = ptBlock;
	ptBlock->ptPrev = ptCache->ptUsedTail;
	ptCache->ptUsedTail = ptBlock;
}

static __inline D2LRUBLOCK *LRUCacheUseFree(D2LRUCACHE *ptCache, DWORD dwSize)
{
	D2LRUBLOCK *ptBlock;

	if (ptCache->dwNumUsedBlocks == ptCache->dwMaxBlocks
		|| ptCache->dwAllocated + dwSize > ptCache->dwMaxAllocated)
		return NULL;
	ptBlock = LRUCacheTakeFree(ptCache);
	LRUCacheAppendUsed(ptCache, ptBlock);
	D2_OPAQUE(ptBlock);
#line 154
	D2ASSERT(ptBlock);
	ptCache->dwNumUsedBlocks++;
	ptCache->dwAllocated += dwSize;
	return ptBlock;
}

D2LRUBLOCK * __fastcall LRUCacheAlloc(D2LRUCACHE *ptCache, DWORD dwSize, void *pOwner)
{
	D2LRUBLOCK *ptBlock;

#line 115
	D2ASSERT(dwSize < ptCache->dwMaxAllocated);
	ptCache->dwNumAllocs++;
	ptCache->dwNumUses++;
	ptBlock = LRUCacheUseFree(ptCache, dwSize);
	while (!ptBlock)
	{
		LRUCacheFree(ptCache, NULL);
		ptBlock = LRUCacheUseFree(ptCache, dwSize);
	}
#line 128
	ptBlock->lpData = D2ALLOCPOOL(ptCache->pMemPool, dwSize);
#line 129
	D2ASSERT(ptBlock->lpData);
	ptBlock->dwSize = dwSize;
	ptBlock->pOwner = pOwner;
	return ptBlock;
}

static __inline void LRUCacheUnlink(D2LRUCACHE *ptCache, D2LRUBLOCK *ptBlock)
{
	D2LRUBLOCK *ptHead;

	ptHead = ptCache->ptUsedHead;
	if (!ptHead)
		return;
	if (ptBlock == ptHead)
	{
		ptCache->ptUsedHead = ptHead->ptNext;
		if (ptCache->ptUsedHead)
			ptCache->ptUsedHead->ptPrev = NULL;
		else
			ptCache->ptUsedTail = NULL;
		ptHead->ptNext = NULL;
		ptHead->ptPrev = NULL;
	}
	else
	{
		if (ptBlock == ptCache->ptUsedTail)
		{
			ptCache->ptUsedTail = ptCache->ptUsedTail->ptPrev;
			ptCache->ptUsedTail->ptNext = NULL;
		}
		else
		{
			ptBlock->ptPrev->ptNext = ptBlock->ptNext;
			ptBlock->ptNext->ptPrev = ptBlock->ptPrev;
		}
		ptBlock->ptNext = NULL;
		ptBlock->ptPrev = NULL;
	}
}

void __fastcall LRUCacheFree(D2LRUCACHE *ptCache, D2LRUBLOCK *ptBlock)
{
	D2LRUBLOCK *ptHead;

	if (!ptCache->dwNumUsedBlocks)
		return;
	if (!ptBlock)
	{
		ptHead = ptCache->ptUsedHead;
		if (!ptHead)
			ptBlock = NULL;
		else
		{
			ptCache->ptUsedHead = ptHead->ptNext;
			if (ptCache->ptUsedHead)
				ptCache->ptUsedHead->ptPrev = NULL;
			else
				ptCache->ptUsedTail = NULL;
			ptHead->ptNext = NULL;
			ptHead->ptPrev = NULL;
			ptBlock = ptHead;
		}
	}
	else
		LRUCacheUnlink(ptCache, ptBlock);
	if (ptBlock)
	{
		ptCache->dwAllocated -= ptBlock->dwSize;
		if (ptBlock->pOwner)
			ptCache->field_28(ptBlock);
		if (ptBlock->lpData)
		{
#line 195
			D2FREEPOOL(ptCache->pMemPool, ptBlock->lpData);
			ptBlock->lpData = NULL;
			ptBlock->dwSize = 0;
			ptBlock->pOwner = NULL;
		}
		if (!ptCache->ptFreeTail)
		{
			ptCache->ptFreeHead = ptBlock;
			ptCache->ptFreeTail = ptBlock;
		}
		else
		{
			ptCache->ptFreeTail->ptNext = ptBlock;
			ptBlock->ptPrev = ptCache->ptFreeTail;
			ptCache->ptFreeTail = ptBlock;
		}
	}
	ptCache->dwNumUsedBlocks--;
}

void __fastcall LRUCacheTouch(D2LRUCACHE *ptCache, D2LRUBLOCK *ptBlock)
{
	if (!ptCache->dwNumUsedBlocks)
		return;
	ptCache->dwNumUses++;
#line 228
	D2ASSERT(ptBlock);
	LRUCacheUnlink(ptCache, ptBlock);
	LRUCacheAppendUsed(ptCache, ptBlock);
}

void __fastcall LRUCacheForEach(D2LRUCACHE *ptCache,
	BOOL (__fastcall *pfnVisit)(D2LRUBLOCK *ptBlock, DWORD dwArg), DWORD dwArg)
{
	D2LRUBLOCK *ptBlock;

#line 396
	D2ASSERT(ptCache);
	for (ptBlock = ptCache->ptUsedHead; ptBlock; ptBlock = ptBlock->ptNext)
	{
		if (!pfnVisit(ptBlock, dwArg))
			break;
	}
}
