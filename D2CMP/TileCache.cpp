#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\TileCache.cpp"
#include "D2Cmp.h"

#include "D2Macros.h"
#line 11

#define TILECACHE_BLOCKS	4096

static D2LRUCACHE sgtTileCache;
static BOOL sgbTileCacheInit;
static DWORD sgdwTileCacheSize;
static void (__fastcall *sgpfnHardwareFreeCallback)(D2TILE *ptTile);

void __fastcall TileCacheFreeBlock(D2LRUBLOCK *ptBlock);

void WINAPI TileCacheInit(void *pPool, DWORD dwSize)
{
	sgdwTileCacheSize = dwSize;
	memset(&sgtTileCache, 0, sizeof(sgtTileCache));
	LRUCacheInit(&sgtTileCache, pPool, dwSize, TileCacheFreeBlock, TILECACHE_BLOCKS);
	sgbTileCacheInit = TRUE;
}

void WINAPI TileCacheShutdown(BOOL bFree)
{
	if (!sgbTileCacheInit)
	{
		TileProjectsFreeAll();
		return;
	}
	if (bFree)
		LRUCacheDestroy(&sgtTileCache);
	memset(&sgtTileCache, 0, sizeof(sgtTileCache));
	sgdwTileCacheSize = 0;
	TileProjectsFreeAll();
}

void __fastcall TileCacheFreeBlock(D2LRUBLOCK *ptBlock)
{
	D2TILE *ptTile;

	ptTile = (D2TILE *)ptBlock->pOwner;
	if (sgpfnHardwareFreeCallback)
	{
#line 93
		D2ASSERTMSG(!IsBadCodePtr((FARPROC)sgpfnHardwareFreeCallback), "sgpfnHardwareFreeCallback");
		sgpfnHardwareFreeCallback(ptTile);
	}
	ptTile->ptCompArray = NULL;
	ptTile->ptLRUCache = NULL;
}

BOOL WINAPI TileCacheLoadTile(D2TILE *ptTile, DWORD dwUnused)
{
	HANDLE hFile;

	if (ptTile->ptLRUCache)
	{
		LRUCacheTouch(&sgtTileCache, ptTile->ptLRUCache);
		return TRUE;
	}
	if (!ptTile->nCompCount)
		return FALSE;
	ptTile->ptLRUCache = LRUCacheAlloc(&sgtTileCache, ptTile->dwCompSize, ptTile);
	ptTile->ptCompArray = (D2TILECOMP *)ptTile->ptLRUCache->lpData;
	memset(ptTile->ptCompArray, 0, ptTile->dwCompSize);
	if (!ArchiveOpenFile(NULL, ptTile->pszProjectName, &hFile, FALSE))
		return FALSE;
	ArchiveSeekFile(NULL, hFile, ptTile->dwCompOffset, NULL, FILE_BEGIN);
	ArchiveReadFile(NULL, hFile, ptTile->ptCompArray, ptTile->dwCompSize);
	ArchiveCloseFile(NULL, hFile);
	TileProjectTileParts(ptTile);
	return TRUE;
}

void WINAPI TileCacheReleaseTile(D2TILE *ptTile)
{
	LRUCacheFree(&sgtTileCache, ptTile->ptLRUCache);
	ptTile->ptCompArray = NULL;
	ptTile->ptLRUCache = NULL;
}

DWORD __fastcall TileCacheProjectSize(const char *pszName)
{
	HANDLE hFile;
	D2TILEPROJECT tProject;

	if (!ArchiveOpenFile(NULL, pszName, &hFile, FALSE))
		return 0;
	ArchiveReadFile(NULL, hFile, &tProject, sizeof(tProject));
	ArchiveCloseFile(NULL, hFile);
	return sizeof(D2TILEPROJECT) + tProject.nNumTiles * sizeof(D2TILE);
}

void __fastcall TileCacheSetHardwareFree(void (__fastcall *pfnFree)(D2TILE *ptTile))
{
	sgpfnHardwareFreeCallback = pfnFree;
}

void WINAPI TileCacheGetStats(DWORD *pdwSize, DWORD *pdwAllocs, DWORD *pdwUses, DWORD *pdwPercent)
{
	*pdwSize = sgdwTileCacheSize;
	*pdwUses = sgtTileCache.dwNumUses;
	*pdwAllocs = sgtTileCache.dwNumAllocs;
	if (sgtTileCache.dwMaxAllocated)
		*pdwPercent = sgtTileCache.dwAllocated * 100 / sgtTileCache.dwMaxAllocated;
	else
		*pdwPercent = (DWORD)-1;
}

void __fastcall TileCacheResetStats(void)
{
	sgtTileCache.dwNumUses = 0;
	sgtTileCache.dwNumAllocs = 0;
}

void __fastcall TileCacheReleaseProject(D2TILEPROJECT *ptProject)
{
	D2TILE *ptTile;
	int nTile;

	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		if (ptTile->ptLRUCache)
		{
			LRUCacheFree(&sgtTileCache, ptTile->ptLRUCache);
			ptTile->ptCompArray = NULL;
			ptTile->ptLRUCache = NULL;
		}
	}
}
