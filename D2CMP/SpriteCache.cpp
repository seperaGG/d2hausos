#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\Src\\SpriteCache.cpp"
#include <stdio.h>
#include "D2Cmp.h"
#include "D2Cmp.h"
#include "StormImports.h"
#include "D2Macros.h"
#include "FogImports.h"
#pragma inline_depth(2)

#line 19

#define SPRITECACHE_BLOCKS	5120

static D2CELDATAHASH sgtCelDataHash;
static D2GFXHASH sgtGfxHash;
static D2SPRITECACHE sgtGfxCache;
static D2LRUCACHE sgtCelDataCache;
static DWORD sgdwCelOffsets[OFFSET_ARRAYSIZE];
static BOOL sgbLowMemory;
static void (__fastcall *sgpfnFreeNode)(DWORD dwValue0, DWORD dwClass, DWORD dwMode);
static DWORD sgdwCelDataBlocks = 4096;
static BOOL sgbCompressedData = TRUE;
static const char *sgpszExtDcc = ".dcc";
static const char *sgpszExtDc6 = ".dc6";
static const char *sgpszLit = "lit";

void __fastcall SpriteCacheInit(D2SPRITECACHE *ptCache, void *pPool, DWORD dwMaxAllocated,
	void (__fastcall *pfnFree)(D2GFXELEMENT *ptElement));
D2SPRITEBLOCK * __fastcall SpriteCacheAlloc(D2SPRITECACHE *ptCache, DWORD dwSize, D2GFXELEMENT *pOwner,
	BOOL fLoad);
static D2SPRITEBLOCK * __fastcall SpriteCacheTakeFree(D2SPRITECACHE *ptCache, DWORD dwSize, BOOL fLoad);
D2SPRITEBLOCK * __fastcall SpriteCacheFree(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock, BOOL fLoads);
void __fastcall SpriteCacheGrow(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock, DWORD dwMore, BOOL fLoads);
static void __fastcall SpriteCacheLink(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock, BOOL fLoad);
static D2SPRITEBLOCK * __fastcall SpriteCachePopHead(D2SPRITECACHE *ptCache, BOOL fLoad);
static void __fastcall SpriteCacheUnlink(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock);
static D2CELDATA * __fastcall SpriteCacheGetCelData(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName);
static void __fastcall SpriteCacheMakePath(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName);
static void __fastcall SpriteCacheInitHardwareCelData(D2CELDATA *ptCelData, D2CELCONTEXT *ptCelContext,
	D2GFXNODE *ptGfxNode);
static BOOL __fastcall SpriteCacheLoadSynch(D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData, DWORD dwDirBlock,
	DWORD dwBlockSize, const char *pszPath);
static __inline D2CEL * __fastcall SpriteCacheGetCel(D2CELCONTEXT *ptCelContext, D2GFXNODE *ptGfxNode);
static void __fastcall SpriteCacheLoadAhead(D2CELCONTEXT *ptCelContext, DWORD dwDirBlock, DWORD dwDirBlockSize,
	D2CELDATA *ptCelData, const char *pszPath, BOOL fAsync);
static BOOL __fastcall SpriteCacheLoadAsync(D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData, DWORD dwDirBlock,
	DWORD dwBlockSize, const char *pszPath);
static void __fastcall SpriteCacheFreeNode(D2GFXELEMENT *ptElement);
static void __fastcall SpriteCacheFreeCelData(D2LRUBLOCK *ptBlock);
void __fastcall SpriteCacheTouch(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock);

const char * WINAPI SpriteCacheGetExtension(BOOL fDcc)
{
	if (fDcc && sgbCompressedData)
		return sgpszExtDcc;
	return sgpszExtDc6;
}

void WINAPI SpriteCacheMakeFileName(char *pszFileName, BOOL fDcc, const char *pszFormat, va_list pArgs)
{
	vsprintf(pszFileName, pszFormat, pArgs);
	strcat(pszFileName, fDcc && sgbCompressedData ? sgpszExtDcc : sgpszExtDc6);
}

void WINAPI SpriteCacheLoadSettings(BOOL fAllow)
{
	StormSRegLoadValue("Diablo II", "CompressedData", 1, &sgbCompressedData);
	if (!fAllow)
		sgbCompressedData = FALSE;
}

void WINAPI SpriteCacheInitAll(void *pPool, DWORD dwGfxMax, DWORD dwCelDataMax, DWORD dwMemory)
{
	DWORD *pSysInfo;

	CodecInit();
	if (!dwMemory)
	{
		pSysInfo = (DWORD *)(INT_PTR)FogErrorManagerGetSysInfo();
#line 258
		D2ASSERT(pSysInfo != NULL);
		dwMemory = pSysInfo[3] - 11534336;
	}
	sgbLowMemory = dwMemory < 67108864;
	CelDataHashInit(&sgtCelDataHash);
	memset(&sgtCelDataCache, 0, sizeof(sgtCelDataCache));
	LRUCacheInit(&sgtCelDataCache, pPool, dwCelDataMax, SpriteCacheFreeCelData, sgdwCelDataBlocks);
	memset(&sgtGfxHash, 0, sizeof(sgtGfxHash));
	GfxHashInit(&sgtGfxHash);
	memset(&sgtGfxCache, 0, sizeof(sgtGfxCache));
	SpriteCacheInit(&sgtGfxCache, pPool, dwGfxMax, SpriteCacheFreeNode);
}

static void __fastcall SpriteCacheFreeNode(D2GFXELEMENT *ptElement)
{
	D2GFXNODE *ptGfxNode;
	void (__fastcall *pfnFreeNode)(DWORD dwValue0, DWORD dwClass, DWORD dwMode);

	pfnFreeNode = sgpfnFreeNode;
	if (pfnFreeNode)
	{
		ptGfxNode = ptElement->ptGfxNode;
#line 174
		D2ASSERT(ptGfxNode);
		pfnFreeNode(ptGfxNode->field_00, ptGfxNode->dwClassId, ptGfxNode->dwMode);
	}
	GfxHashRemove(sgtGfxCache.pMemPool, &sgtGfxHash, ptElement);
}

static void __fastcall SpriteCacheFreeCelData(D2LRUBLOCK *ptBlock)
{
	D2CELDATAELEMENT *ptElement;

	ptElement = (D2CELDATAELEMENT *)ptBlock->pOwner;
	ptElement->lpBlock = NULL;
	CelDataHashRemove(sgtCelDataCache.pMemPool, &sgtCelDataHash, ptElement);
}

void WINAPI SpriteCacheShutdown(BOOL fFree)
{
	if (fFree)
	{
		while (sgtGfxCache.dwNumUsed)
			SpriteCacheFree(&sgtGfxCache, NULL, TRUE);
		if (sgtGfxCache.ptBlocks)
		{
#line 1825
			D2FREEPOOL(sgtGfxCache.pMemPool, sgtGfxCache.ptBlocks);
			sgtGfxCache.ptBlocks = NULL;
		}
		if (sgtGfxCache.pcs)
		{
			DeleteCriticalSection(sgtGfxCache.pcs);
#line 1843
			D2FREE2(sgtGfxCache.pcs);
			sgtGfxCache.pcs = NULL;
		}
	}
	memset(&sgtGfxCache, 0, sizeof(sgtGfxCache));
	if (fFree)
		LRUCacheDestroy(&sgtCelDataCache);
	memset(&sgtCelDataCache, 0, sizeof(sgtCelDataCache));
	CodecShutdown();
}

BOOL WINAPI SpriteCacheLoadDone(BYTE *pbData, DWORD dwSize, DWORD *pCallback)
{
	D2CELDATA *ptCelData;
	D2GFXBLOCK *ptGfxBlock;

	ptCelData = (D2CELDATA *)(INT_PTR)pCallback[1];
	ptGfxBlock = (D2GFXBLOCK *)(INT_PTR)pCallback[2];
	pCallback[5] = (DWORD)(INT_PTR)CodecDecodeSprite((DWORD *)pbData, sgtGfxCache.pMemPool, dwSize, ptCelData,
		ptGfxBlock, pCallback[3], pCallback[4], &pCallback[6], ptCelData->szFileName);
	ptGfxBlock->bFlags &= ~GFXBLOCK_PENDING;
	SpriteCacheGrow(&sgtGfxCache, ptGfxBlock->ptCacheBlock, pCallback[6], FALSE);
	if (!pCallback[5])
		ptCelData->bFlags |= CELDATAFLAG_NOFILE;
	return TRUE;
}

static __inline DWORD SpriteCacheTurn(DWORD dwDirection, D2CELDATA *ptCelData)
{
	if (ptCelData->tHeader[CELHDR_DIRS] > 1)
		return (dwDirection << 6) / ptCelData->tHeader[CELHDR_DIRS];
	return 1;
}

BOOL WINAPI SpriteCacheGetContextCel(D2CELCONTEXT *ptCelContext, BOOL fPoll, BOOL fAsyncAhead)
{
	D2CELDATA *ptCelData;
	D2GFXELEMENT *ptElement;
	D2GFXNODE *ptGfxNode;
	DWORD dwDirBlock;
	DWORD dwDirBlockSize;
	DWORD dwDirection;
	void *hAsyncData;
	BOOL fLoaded;
	BOOL fResult;

#line 1416
	D2ASSERT(ptCelContext);
	ptCelContext->hCel = NULL;
	if (ptCelContext->hCelFile)
	{
		ptCelContext->hCel = (D2CEL *)(INT_PTR)CelContextGetCel(ptCelContext);
		return TRUE;
	}
	char szPath[MAX_PATH] = { 0 };
	char szName[MAX_PATH] = { 0 };
	ptCelData = SpriteCacheGetCelData(ptCelContext, szPath, szName);
	dwDirBlock = 0;
	dwDirBlockSize = 0;
	ptElement = GfxHashFind(&sgtGfxHash, ptCelContext, &dwDirBlock, &dwDirBlockSize, ptCelData);
	fResult = TRUE;
	fLoaded = FALSE;
	if (dwDirBlockSize > 1 && dwDirBlock > 0)
	{
		if (ptElement)
		{
			hAsyncData = ptElement->ptGfxNode->tGfxBlock.lpAsyncData;
#line 1454
			D2ASSERT(hAsyncData);
			fLoaded = FogAsyncDataIsLoaded(hAsyncData);
		}
		if (!fLoaded && !(ptCelData->bFlags & CELDATAFLAG_NOFILE))
		{
			dwDirection = ptCelContext->dwDirection * ptCelData->tHeader[CELHDR_DIRS] >> 6;
			ptCelContext->dwDirection = SpriteCacheTurn(CelConvertDirection(dwDirection,
				ptCelData->tHeader[CELHDR_DIRS], dwDirBlockSize,
				ptCelContext->bUnitFlags & CELCONTEXTUNIT_CONVERT), ptCelData);
			ptElement = GfxHashFind(&sgtGfxHash, ptCelContext, &dwDirBlock, &dwDirBlockSize, ptCelData);
#line 1465
			D2ASSERT(dwDirBlock == 0);
		}
	}
	if (ptElement)
	{
		ptGfxNode = ptElement->ptGfxNode;
#line 1473
		D2ASSERT(ptGfxNode);
		if (ptGfxNode->tGfxBlock.bFlags & GFXBLOCK_NOFILE)
			return FALSE;
#line 1481
		D2ASSERT(ptGfxNode->tGfxBlock.lpAsyncData);
		if (fPoll)
			return FogAsyncDataIsLoaded(ptGfxNode->tGfxBlock.lpAsyncData) != 0;
		SpriteCacheTouch(&sgtGfxCache, ptElement->ptCacheBlock);
		if (!ptGfxNode->tGfxBlock.fHardwareCelData)
			SpriteCacheInitHardwareCelData(ptCelData, ptCelContext, ptGfxNode);
		ptCelContext->hCel = SpriteCacheGetCel(ptCelContext, ptGfxNode);
#line 1508
		D2ASSERT(ptCelContext->hCel);
		if (!fLoaded && !dwDirBlock && dwDirBlockSize > 1)
			SpriteCacheLoadAhead(ptCelContext, 0, dwDirBlockSize, ptCelData, szPath, fAsyncAhead);
	}
	else
	{
		if (fPoll)
			return FALSE;
		fResult = SpriteCacheLoadSynch(ptCelContext, ptCelData, dwDirBlock, dwDirBlockSize, szPath);
		if (fResult)
			SpriteCacheLoadAhead(ptCelContext, dwDirBlock, dwDirBlockSize, ptCelData, szPath, fAsyncAhead);
	}
	return fResult;
}

static D2CELDATA * __fastcall SpriteCacheGetCelData(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName)
{
	D2CELDATAELEMENT *ptHashTableElement;
	D2CELDATA *ptCelData;
	DWORD dwFileSize;
	DWORD dwTotalCels;
	DWORD dwNumDirections;
	DWORD dwNumFrames;
	DWORD dwHeader0;
	DWORD dwHeader1;
	DWORD dwHeader2;
	HANDLE hFile;
	DWORD i;

	SpriteCacheMakePath(ptCelContext, pszPath, pszName);
	ptHashTableElement = CelDataHashFind(&sgtCelDataHash, pszName);
	if (ptHashTableElement)
	{
		LRUCacheTouch(&sgtCelDataCache, ptHashTableElement->lpBlock);
		ptCelData = (D2CELDATA *)ptHashTableElement->lpBlock->lpData;
	}
	else
	{
		ptHashTableElement = CelDataHashAdd(sgtCelDataCache.pMemPool, &sgtCelDataHash, NULL, pszName);
#line 810
		D2ASSERT(ptHashTableElement);
		ptHashTableElement->lpBlock = LRUCacheAlloc(&sgtCelDataCache, sizeof(D2CELDATA), ptHashTableElement);
#line 815
		D2ASSERT(ptHashTableElement->lpBlock);
		ptCelData = (D2CELDATA *)ptHashTableElement->lpBlock->lpData;
		memset(ptCelData, 0, sizeof(D2CELDATA));
		strcpy(ptCelData->szFileName, pszName);
		if (ptCelContext->dwFlags & CELCONTEXT_DCC)
		{
			if (!CodecReadHeader(pszPath, ptCelData))
				ptCelData->bFlags |= CELDATAFLAG_NOFILE;
		}
		else
		{
			FogSafesockOpenStormFile(pszPath, &hFile);
			if (hFile)
			{
				{
					DWORD tHeader[7];

					memset(tHeader, 0, sizeof(tHeader));
					FogSafesockReadStormFile(hFile, tHeader, 6 * sizeof(DWORD), 0, 0, 0);
					dwNumDirections = tHeader[4];
					dwNumFrames = tHeader[5];
					dwHeader1 = tHeader[1];
					dwHeader0 = tHeader[0];
					dwHeader2 = tHeader[2];
				}
				dwFileSize = FogSafesockGetStormFileSize(hFile, 0);
				ptCelData->tHeader[CELHDR_DIRS] = dwNumDirections;
				ptCelData->tHeader[CELHDR_FRAMES] = dwNumFrames;
				ptCelData->tHeader[1] = dwHeader1;
				ptCelData->tHeader[0] = dwHeader0;
				ptCelData->tHeader[2] = dwHeader2;
				dwTotalCels = dwNumFrames * dwNumDirections;
#line 858
				D2ASSERT(dwTotalCels < OFFSET_ARRAYSIZE);
				FogSafesockReadStormFile(hFile, sgdwCelOffsets, dwTotalCels * sizeof(DWORD), 0, 0, 0);
				FogSafesockCloseStormFile(hFile);
#line 863
				D2ASSERTMSG(ptCelData->tHeader[CELHDR_DIRS] < MAX_DIROFFSETS, "ptCelData->tHeader.dwNumDirections < MAX_DIROFFSETS");

				for (i = 0; i < ptCelData->tHeader[CELHDR_DIRS]; i++)
					ptCelData->adwDirOffsets[i] = sgdwCelOffsets[ptCelData->tHeader[CELHDR_FRAMES] * i];
				ptCelData->adwDirOffsets[ptCelData->tHeader[CELHDR_DIRS]] = dwFileSize;
			}
			else
				ptCelData->bFlags |= CELDATAFLAG_NOFILE;
		}
	}
#line 881
	D2ASSERTMSG(ptCelData, "[sValidateCelData] This should not happen. Please tell Jon Morin!\n");
	return ptCelData;
}

static __inline void SpriteCacheToken(char *pszToken, const char *pszCode)
{
	pszToken[0] = pszCode[0] == ' ' ? 0 : pszCode[0];
	pszToken[1] = pszCode[1] == ' ' ? 0 : pszCode[1];
	pszToken[2] = pszCode[2] == ' ' ? 0 : pszCode[2];
	pszToken[3] = 0;
}

static __inline void SpriteCacheItemPath(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName)
{
#line 322
	D2ASSERT(ptCelContext->tUnitInfo.pszFileName);
	strcpy(pszName, ptCelContext->tUnitInfo.pszFileName);
	if (ptCelContext->dwMode == 4)
	{
		if (ptCelContext->bUnitFlags & CELCONTEXTUNIT_ITEMLIT)
			wsprintfA(pszPath, "%s\\items\\%s.dc6", "DATA\\GLOBAL", pszName);
		else
			wsprintfA(pszPath, "%s\\items\\%s.dc6", "DATA\\GLOBAL", pszName);
		return;
	}
	if (ptCelContext->dwMode == 3)
	{
		wsprintfA(pszPath, "%s\\items\\%s.dc6", "DATA\\GLOBAL", pszName);
		return;
	}
#line 345
	D2FATAL("[sCreateItemFileName] This should never happen. Please tell Jon Morin!\n");
}

static __inline void SpriteCacheOverlayPath(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName)
{
#line 358
	D2ASSERT(ptCelContext->tUnitInfo.pszFileName);
	strcpy(pszName, ptCelContext->tUnitInfo.pszFileName);
	if (ptCelContext->dwOverlayClass != 122)
	{
		if (sgbCompressedData)
		{
			wsprintfA(pszPath, "%s\\%s.dcc", "DATA\\GLOBAL\\OVERLAYS", pszName);
			ptCelContext->dwFlags |= CELCONTEXT_DCC;
		}
		else
			wsprintfA(pszPath, "%s\\%s.dc6", "DATA\\GLOBAL\\OVERLAYS", pszName);
	}
	else
		wsprintfA(pszPath, "%s\\%s.dc6", "DATA\\GLOBAL\\OVERLAYS", pszName);
}

static __inline void SpriteCacheMissilePath(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName)
{
#line 390
	D2ASSERT(ptCelContext->tUnitInfo.pszFileName);
	strcpy(pszName, ptCelContext->tUnitInfo.pszFileName);
	if (sgbCompressedData)
	{
		wsprintfA(pszPath, "%s\\%s.dcc", "DATA\\GLOBAL\\MISSILES", pszName);
		ptCelContext->dwFlags |= CELCONTEXT_DCC;
	}
	else
		wsprintfA(pszPath, "%s\\%s.dc6", "DATA\\GLOBAL\\MISSILES", pszName);
}

static void __fastcall SpriteCacheMakePath(D2CELCONTEXT *ptCelContext, char *pszPath, char *pszName)
{
	char szUnit[4];
	char szComponent[4];
	char szVariant[4];
	char szMode[4];
	char szWClass[4];
	BOOL fDcc;

	if (CELCONTEXT_ISOVERLAY(ptCelContext))
	{
		SpriteCacheOverlayPath(ptCelContext, pszPath, pszName);
		return;
	}
	if (ptCelContext->dwUnitType == UNITGFX_ITEM)
	{
		SpriteCacheItemPath(ptCelContext, pszPath, pszName);
		return;
	}
	if (ptCelContext->dwUnitType == UNITGFX_MISSILE)
	{
		SpriteCacheMissilePath(ptCelContext, pszPath, pszName);
		return;
	}
	SpriteCacheToken(szUnit, ptCelContext->tUnitInfo.achUnitToken);
	SpriteCacheToken(szComponent, ptCelContext->tUnitInfo.achComponentToken);
	SpriteCacheToken(szVariant, ptCelContext->tUnitInfo.achVariantToken);
	SpriteCacheToken(szMode, ptCelContext->tUnitInfo.achModeToken);
	SpriteCacheToken(szWClass, ptCelContext->tUnitInfo.achWClassToken);
	strcpy(pszName, szUnit);
	strcat(pszName, szComponent);
	strcat(pszName, szVariant ? szVariant : sgpszLit);
	strcat(pszName, szMode);
	strcat(pszName, szWClass);
	fDcc = sgbCompressedData;
	switch (ptCelContext->dwUnitType)
	{
	case UNITGFX_PLAYER:
		strcpy(pszPath, "DATA\\GLOBAL\\CHARS");
		break;
	case UNITGFX_MONSTER:
		strcpy(pszPath, "DATA\\GLOBAL\\MONSTERS");
		switch (ptCelContext->dwClassId)
		{
		case 243:
		case 284:
			if (ptCelContext->dwMode)
				break;
		case 242:
		case 251:
		case 367:
			fDcc = FALSE;
			break;
		}
		break;
	case UNITGFX_OBJECT:
		if (ptCelContext->dwClassId == 342)
			fDcc = FALSE;
		strcpy(pszPath, "DATA\\GLOBAL\\OBJECTS");
		break;
	default:
#line 487
		D2FATAL("Invalid Unit Type");
	}
	strcat(pszPath, "\\");
	strcat(pszPath, szUnit);
	strcat(pszPath, "\\");
	strcat(pszPath, szComponent);
	strcat(pszPath, "\\");
	strcat(pszPath, pszName);
	if (fDcc)
	{
		strcat(pszPath, ".DCC");
		ptCelContext->dwFlags |= CELCONTEXT_DCC;
	}
	else
		strcat(pszPath, ".DC6");
}

static void __fastcall SpriteCacheInitHardwareCelData(D2CELDATA *ptCelData, D2CELCONTEXT *ptCelContext,
	D2GFXNODE *ptGfxNode)
{
	char szMessage[256];
	D2CELSLOT *ptSlot;
	BYTE *lpbData;
	D2CEL *pCel;
	DWORD dwOffset;
	DWORD dwFrame;
	DWORD dwSlots;

#line 1019
	D2ASSERT(ptCelContext);
#line 1020
	D2ASSERT(ptGfxNode);
#line 1021
	D2ASSERT(!(ptCelData->bFlags & CELDATAFLAG_NOFILE));
#line 1024
	D2ASSERTMSG(!ptGfxNode->tGfxBlock.fHardwareCelData, "!ptGfxBlock->fHardwareCelData");
	dwSlots = (DWORD)ptGfxNode->tGfxBlock.bCelsPerDirection * ptGfxNode->tGfxBlock.bNumDirections;
#line 1029
	ptSlot = (D2CELSLOT *)D2ALLOCPOOL(sgtGfxCache.pMemPool, dwSlots * sizeof(D2CELSLOT));
	memset(ptSlot, 0, dwSlots * sizeof(D2CELSLOT));
	lpbData = FogAsyncDataGetBuffer(ptGfxNode->tGfxBlock.lpAsyncData);
#line 1040
	D2ASSERT(lpbData);
	dwOffset = ptGfxNode->tGfxBlock.nOffsets[ptGfxNode->tGfxBlock.bFirstDirection];
	for (dwFrame = 0; dwFrame
		< (DWORD)ptGfxNode->tGfxBlock.bCelsPerDirection * ptGfxNode->tGfxBlock.bNumDirections;
		dwFrame++)
	{
		pCel = (D2CEL *)(lpbData + dwOffset);
		if (pCel->dwFlags || pCel->dwWidth > 256 || pCel->dwHeight > 256)
		{
			char szPath[MAX_PATH] = { 0 };
			char szName[MAX_PATH] = { 0 };

			SpriteCacheMakePath(ptCelContext, szPath, szName);
#line 1162
			D2FATAL((char *)(INT_PTR)FogDataArraysSprintf((DWORD)szMessage, (DWORD) "[sInitializeHardwareCelData] There offsets are screwed for the Cel File:\n\n%s\n\n" "Please tell Jon Morin!\n", (DWORD)szPath));

		}
		pCel->ptSlot = ptSlot;
		ptSlot++;
		dwOffset += CELSIZE(pCel->dwDataSize + pCel->dwExtraSize);
	}
	ptGfxNode->tGfxBlock.fHardwareCelData = TRUE;
}

static __inline D2GFXNODE *SpriteCacheInitNode(D2SPRITEBLOCK *ptBlock, D2CELCONTEXT *ptCelContext)
{
	D2GFXNODE *ptGfxNode;

	ptGfxNode = (D2GFXNODE *)ptBlock->lpData;
	memset(ptGfxNode, 0, sizeof(D2GFXNODE));
	ptGfxNode->adwTokens[0] = ptCelContext->tUnitInfo.dwUnitToken;
	ptGfxNode->adwTokens[3] = ptCelContext->tUnitInfo.dwModeToken;
	ptGfxNode->adwTokens[1] = ptCelContext->tUnitInfo.dwComponentToken;
	ptGfxNode->adwTokens[2] = ptCelContext->tUnitInfo.dwVariantToken;
	ptGfxNode->adwTokens[4] = ptCelContext->tUnitInfo.dwWClassToken;
	ptGfxNode->tGfxBlock.ptCacheBlock = ptBlock;
	return ptGfxNode;
}

static __inline D2GFXNODE *SpriteCacheNewSynchNode(D2CELCONTEXT *ptCelContext, D2SPRITEBLOCK **pptBlock)
{
	D2SPRITEBLOCK *ptBlock;

	ptBlock = SpriteCacheAlloc(&sgtGfxCache, sizeof(D2GFXNODE), NULL, FALSE);
#line 1189
	D2ASSERT(ptBlock);
	*pptBlock = ptBlock;
	return SpriteCacheInitNode(ptBlock, ptCelContext);
}

static __inline void SpriteCacheSetBlock(D2GFXNODE *ptGfxNode, D2CELDATA *ptCelData, DWORD dwDirBlock,
	DWORD dwBlockSize)
{
	ptGfxNode->tGfxBlock.bNumDirections = (BYTE)dwBlockSize;
	ptGfxNode->tGfxBlock.bFirstDirection = (BYTE)(dwBlockSize * dwDirBlock);
	ptGfxNode->tGfxBlock.bCelsPerDirection = (BYTE)ptCelData->tHeader[CELHDR_FRAMES];
	ptGfxNode->tGfxBlock.field_03 = (BYTE)(ptCelData->tHeader[CELHDR_DIRS] + 1);
}

static __inline DWORD SpriteCacheDirection(D2CELCONTEXT *ptCelContext, D2GFXBLOCK *ptGfxBlock)
{
	ptCelContext->dwCelsPerDirection = ptGfxBlock->bCelsPerDirection;
	D2_OPAQUE(ptGfxBlock);
#line 898
	D2ASSERT(ptGfxBlock);
	if (CELCONTEXT_ISOVERLAY(ptCelContext))
		return CelGetGameDirection(ptCelContext->dwDirection, ptGfxBlock->field_03 - 1);
	if (ptCelContext->dwUnitType == UNITGFX_OBJECT)
		return 0;
	if (ptCelContext->dwUnitType == UNITGFX_ITEM)
		return ptCelContext->dwDirection;
	return CelGetGameDirection(ptCelContext->dwDirection, ptGfxBlock->field_03 - 1);
}

static __inline void SpriteCacheSetCel(D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData, D2GFXNODE *ptGfxNode)
{
	if (!ptGfxNode->tGfxBlock.fHardwareCelData)
		SpriteCacheInitHardwareCelData(ptCelData, ptCelContext, ptGfxNode);
	ptCelContext->hCel = SpriteCacheGetCel(ptCelContext, ptGfxNode);
#line 1242
	D2ASSERT(ptCelContext->hCel);
}

static __inline BOOL SpriteCacheAddSynchNode(D2SPRITEBLOCK *ptBlock, D2GFXNODE *ptGfxNode,
	D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData)
{
	D2GFXELEMENT *ptHashTableElement;

	ptHashTableElement = GfxHashAdd(sgtGfxCache.pMemPool, &sgtGfxHash, (D2GFXNODE *)ptBlock->lpData, ptCelContext,
		ptCelData);
#line 1219
	D2ASSERT(ptHashTableElement);
	ptBlock->ptOwner = ptHashTableElement;
	ptHashTableElement->ptCacheBlock = ptBlock;
	if (ptCelData->bFlags & CELDATAFLAG_NOFILE)
	{
		ptGfxNode->tGfxBlock.bFlags |= GFXBLOCK_NOFILE;
		return FALSE;
	}
	SpriteCacheSetCel(ptCelContext, ptCelData, ptGfxNode);
	return TRUE;
}

static BOOL __fastcall SpriteCacheLoadSynch(D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData, DWORD dwDirBlock,
	DWORD dwBlockSize, const char *pszPath)
{
	char szMessage[256];
	D2SPRITEBLOCK *ptBlock;
	D2GFXNODE *ptGfxNode;
	DWORD *pdwOffset;
	DWORD dwFirstOffset;
	DWORD dwSize;
	DWORD dwFirst;
	DWORD dwLast;
	void *hAsyncData;
	DWORD i;

	ptGfxNode = SpriteCacheNewSynchNode(ptCelContext, &ptBlock);
	if (!(ptCelData->bFlags & CELDATAFLAG_NOFILE))
	{
		ptGfxNode->dwClassId = ptCelContext->dwClassId;
		ptGfxNode->dwMode = ptCelContext->dwMode;
		D2_OPAQUE(ptCelData);
		D2_OPAQUE(ptGfxNode);
		D2_OPAQUE(ptCelContext);
#line 569
		D2ASSERT(ptCelData);
#line 570
		D2ASSERT(ptGfxNode);
#line 571
		D2ASSERT(ptCelContext);
		if (CELCONTEXT_ISOVERLAY(ptCelContext) ||
			ptCelContext->dwUnitType == UNITGFX_MISSILE)
		{
#line 579
			D2ASSERT(dwBlockSize == 1);
			dwFirst = CelGetGameDirection(ptCelContext->dwDirection, ptCelData->tHeader[CELHDR_DIRS]);
			dwLast = dwFirst + 1;
		}
		else if (ptCelContext->dwUnitType == UNITGFX_OBJECT)
		{
			dwFirst = 0;
			dwLast = 1;
		}
		else if (ptCelContext->dwUnitType == UNITGFX_ITEM)
		{
			dwFirst = ptCelContext->dwDirection;
			dwLast = dwFirst + 1;
		}
		else
		{
			if (ptCelContext->dwUnitType == UNITGFX_MONSTER && dwBlockSize == 1)
				dwFirst = CelGetGameDirection(ptCelContext->dwDirection,
					ptCelData->tHeader[CELHDR_DIRS]);
			else
				dwFirst = dwDirBlock * dwBlockSize;
			dwLast = dwFirst + dwBlockSize;
		}
		pdwOffset = &ptCelData->adwDirOffsets[dwFirst];
		dwSize = ptCelData->adwDirOffsets[dwLast] - *pdwOffset;
		if (!dwSize)
		{
			char szPath[MAX_PATH] = { 0 };
			char szName[MAX_PATH] = { 0 };

			SpriteCacheMakePath(ptCelContext, szPath, szName);
#line 619
			D2ASSERTMSG(0, (char *)(INT_PTR)FogDataArraysSprintf((DWORD)szMessage, (DWORD) "[sValidateSynchLoadDirectionBlock] There are differences between the CEL Data and the " "Cel File:\n\n%s\n\nPlease tell Jon Morin!\n", (DWORD)szPath));

		}
#line 621
		D2ASSERT(!ptGfxNode->tGfxBlock.lpAsyncData);
		SpriteCacheSetBlock(ptGfxNode, ptCelData, dwDirBlock, dwBlockSize);
		dwFirstOffset = *pdwOffset;
		for (i = dwFirst; i <= dwLast; i++)
		{
#line 637
			D2ASSERT(i < MAX_DIROFFSETS);
			ptGfxNode->tGfxBlock.nOffsets[i] = ptCelData->adwDirOffsets[i] - dwFirstOffset;
#line 639
			D2ASSERT(ptGfxNode->tGfxBlock.nOffsets[i] >= 0);
		}
		if (ptCelData->bFlags & CELDATAFLAG_DCC)
		{
			DWORD tLoad[7];

			memset(tLoad, 0, sizeof(tLoad));
			tLoad[0] = (DWORD)(INT_PTR)SpriteCacheLoadDone;
			tLoad[1] = (DWORD)(INT_PTR)ptCelData;
			tLoad[2] = (DWORD)(INT_PTR)&ptGfxNode->tGfxBlock;
			tLoad[3] = dwDirBlock * dwBlockSize;
			tLoad[4] = dwBlockSize;
			ptGfxNode->tGfxBlock.bFlags |= GFXBLOCK_PENDING;
#line 662
			hAsyncData = D2ASYNCLOAD(sgtGfxCache.pMemPool, pszPath, FALSE, *pdwOffset, dwSize, NULL, tLoad);
			ptGfxNode->tGfxBlock.lpAsyncData = hAsyncData;
			FogAsyncDataGetSize(ptGfxNode->tGfxBlock.lpAsyncData);
		}
		else
		{
#line 676
			hAsyncData = D2ASYNCLOAD(sgtGfxCache.pMemPool, pszPath, FALSE, *pdwOffset, dwSize, NULL, NULL);
			ptGfxNode->tGfxBlock.lpAsyncData = hAsyncData;
		}
#line 681
		D2ASSERT(ptGfxNode->tGfxBlock.lpAsyncData);
	}
	return SpriteCacheAddSynchNode(ptBlock, ptGfxNode, ptCelContext, ptCelData);
}

static __inline D2CEL * __fastcall SpriteCacheGetCel(D2CELCONTEXT *ptCelContext, D2GFXNODE *ptGfxNode)
{
	BYTE *lpbData;
	D2GFXBLOCK *ptGfxBlock;
	D2CEL *pCel;
	DWORD dwOffset;
	DWORD dwFrames;

#line 965
	D2ASSERT(ptGfxNode);
#line 966
	D2ASSERT(ptGfxNode->tGfxBlock.lpAsyncData);
	ptGfxBlock = &ptGfxNode->tGfxBlock;
	lpbData = FogAsyncDataGetBuffer(ptGfxBlock->lpAsyncData);
	if (lpbData)
	{
		dwOffset = ptGfxBlock->nOffsets[SpriteCacheDirection(ptCelContext, ptGfxBlock)];
		dwFrames = ptCelContext->dwFrame + 1;
		do
		{
			pCel = (D2CEL *)(lpbData + dwOffset);
			dwOffset += CELSIZE(pCel->dwDataSize + pCel->dwExtraSize);
		}
		while (--dwFrames);
		return pCel;
	}
	return NULL;
}

static void __fastcall SpriteCacheLoadAhead(D2CELCONTEXT *ptCelContext, DWORD dwDirBlock, DWORD dwDirBlockSize,
	D2CELDATA *ptCelData, const char *pszPath, BOOL fAsync)
{
	BOOL fAhead;
	DWORD nDirs;

#line 1346
	D2ASSERT(ptCelData);
#line 1347
	D2ASSERT(ptCelContext);
	fAhead = TRUE;
	if (!CELCONTEXT_ISOVERLAY(ptCelContext) &&
		ptCelContext->dwUnitType == UNITGFX_PLAYER)
	{
		if (!(ptCelContext->bUnitFlags & CELCONTEXTUNIT_PLAYERSEP) || !ptCelContext->dwMode ||
			ptCelContext->dwMode == 17 || sgbLowMemory)
			fAhead = FALSE;
	}
	if (dwDirBlock || dwDirBlockSize <= 1 || !fAhead)
		return;
	nDirs = 2 * dwDirBlockSize;
	ptCelContext->dwDirection = SpriteCacheTurn(CelGetDirectionConversion(dwDirBlockSize, nDirs), ptCelData);
	if (GfxHashFind(&sgtGfxHash, ptCelContext, &dwDirBlock, &dwDirBlockSize, ptCelData))
		return;
#line 1385
	D2ASSERT(dwDirBlock == 1);
#line 1386
	D2ASSERT(dwDirBlockSize > 1);
	if (fAsync)
		SpriteCacheLoadAsync(ptCelContext, ptCelData, 1, dwDirBlockSize, pszPath);
	else
		SpriteCacheLoadSynch(ptCelContext, ptCelData, 1, dwDirBlockSize, pszPath);
}

static __inline BOOL SpriteCacheAddAsyncNode(D2SPRITEBLOCK *ptBlock, D2GFXNODE *ptGfxNode,
	D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData)
{
	D2GFXELEMENT *ptHashTableElement;

	ptHashTableElement = GfxHashAdd(sgtGfxCache.pMemPool, &sgtGfxHash, (D2GFXNODE *)ptBlock->lpData, ptCelContext,
		ptCelData);
#line 1304
	D2ASSERT(ptHashTableElement);
	ptBlock->ptOwner = ptHashTableElement;
	ptHashTableElement->ptCacheBlock = ptBlock;
	if (ptCelData->bFlags & CELDATAFLAG_NOFILE)
	{
		ptGfxNode->tGfxBlock.bFlags |= GFXBLOCK_NOFILE;
		return FALSE;
	}
	ptGfxNode->tGfxBlock.bFlags |= GFXBLOCK_BLOCKS;
	return TRUE;
}

static BOOL __fastcall SpriteCacheLoadAsync(D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData, DWORD dwDirBlock,
	DWORD dwBlockSize, const char *pszPath)
{
	D2SPRITEBLOCK *ptBlock;
	D2GFXNODE *ptGfxNode;
	DWORD *pdwOffset;
	DWORD dwFirstOffset;
	DWORD dwSize;
	DWORD dwFirst;
	DWORD dwLast;
	void *hAsyncData;
	DWORD i;

	ptBlock = SpriteCacheAlloc(&sgtGfxCache, sizeof(D2GFXNODE), NULL, TRUE);
	if (ptBlock)
	{
		ptGfxNode = SpriteCacheInitNode(ptBlock, ptCelContext);
		while (1)
		{
			if (!(ptCelData->bFlags & CELDATAFLAG_NOFILE))
			{
				ptGfxNode->dwClassId = ptCelContext->dwClassId;
				ptGfxNode->dwMode = ptCelContext->dwMode;
				D2_OPAQUE(ptCelData);
				D2_OPAQUE(ptGfxNode);
				D2_OPAQUE(ptCelContext);
#line 699
				D2ASSERT(ptCelData);
#line 700
				D2ASSERT(ptGfxNode);
#line 701
				D2ASSERT(ptCelContext);
				dwFirst = 0;
				dwLast = 0;
				if (ptCelContext->dwUnitType == UNITGFX_MONSTER || ptCelContext->dwUnitType == UNITGFX_PLAYER)
				{
					if (ptCelContext->dwUnitType == UNITGFX_MONSTER && dwBlockSize == 1 &&
						ptCelData->tHeader[CELHDR_DIRS] != 1)
						break;
					dwFirst = dwDirBlock * dwBlockSize;
					dwLast = dwFirst + dwBlockSize;
				}
				pdwOffset = &ptCelData->adwDirOffsets[dwFirst];
				dwSize = ptCelData->adwDirOffsets[dwLast] - *pdwOffset;
#line 725
				D2ASSERT(dwSize);
#line 726
				D2ASSERT(!ptGfxNode->tGfxBlock.lpAsyncData);
				SpriteCacheSetBlock(ptGfxNode, ptCelData, dwDirBlock, dwBlockSize);
				dwFirstOffset = *pdwOffset;
				for (i = dwFirst; i <= dwLast; i++)
				{
#line 745
					D2ASSERT(i < MAX_DIROFFSETS);
					ptGfxNode->tGfxBlock.nOffsets[i] = ptCelData->adwDirOffsets[i] - dwFirstOffset;
#line 747
					D2ASSERT(ptGfxNode->tGfxBlock.nOffsets[i] >= 0);
				}
				if (ptCelData->bFlags & CELDATAFLAG_DCC)
				{
					DWORD tLoad[7];

					memset(tLoad, 0, sizeof(tLoad));
					tLoad[3] = dwDirBlock * dwBlockSize;
					tLoad[2] = (DWORD)(INT_PTR)&ptGfxNode->tGfxBlock;
					tLoad[4] = dwBlockSize;
					ptGfxNode->tGfxBlock.bFlags |= GFXBLOCK_PENDING;
					tLoad[0] = (DWORD)(INT_PTR)SpriteCacheLoadDone;
					tLoad[1] = (DWORD)(INT_PTR)ptCelData;
#line 766
					hAsyncData = D2ASYNCLOAD(sgtGfxCache.pMemPool, pszPath, TRUE, *pdwOffset, dwSize, NULL, tLoad);
					ptGfxNode->tGfxBlock.lpAsyncData = hAsyncData;
				}
				else
				{
#line 775
					hAsyncData = D2ASYNCLOAD(sgtGfxCache.pMemPool, pszPath, TRUE, *pdwOffset, dwSize, NULL, NULL);
					ptGfxNode->tGfxBlock.lpAsyncData = hAsyncData;
				}
				if (ptGfxNode->tGfxBlock.lpAsyncData && dwSize != (DWORD)-1)
					SpriteCacheGrow(&sgtGfxCache, ptBlock, dwSize, TRUE);
				else
					break;
			}
			return SpriteCacheAddAsyncNode(ptBlock, ptGfxNode, ptCelContext, ptCelData);
		}
		SpriteCacheFree(&sgtGfxCache, ptBlock, TRUE);
	}
	return FALSE;
}

BOOL WINAPI SpriteCacheLoadContext(D2CELCONTEXT *ptCelContext)
{
	D2CELDATA *ptCelData;
	D2GFXELEMENT *ptHashTableElement;
	DWORD dwDirBlock;
	DWORD dwDirBlockSize;
	DWORD dwDirection;

#line 1556
	D2ASSERT(ptCelContext);
	if (ptCelContext->hCelFile)
	{
		ptCelContext->hCel = (D2CEL *)(INT_PTR)CelContextGetCel(ptCelContext);
		return TRUE;
	}
	{
		char szPath[MAX_PATH] = { 0 };
		char szName[MAX_PATH] = { 0 };

		ptCelData = SpriteCacheGetCelData(ptCelContext, szPath, szName);
		dwDirBlock = 0;
		dwDirBlockSize = 0;
		ptHashTableElement = GfxHashFind(&sgtGfxHash, ptCelContext, &dwDirBlock, &dwDirBlockSize, ptCelData);
		if (ptHashTableElement)
		{
			if (ptHashTableElement->ptGfxNode->tGfxBlock.bFlags & GFXBLOCK_NOFILE)
				ptCelContext->dwFlags |= CELCONTEXT_LOADED;
			return TRUE;
		}
		if (dwDirBlockSize > 1 && dwDirBlock > 0 && !(ptCelData->bFlags & CELDATAFLAG_NOFILE))
		{
			dwDirection = ptCelContext->dwDirection * ptCelData->tHeader[CELHDR_DIRS] >> 6;
			ptCelContext->dwDirection = SpriteCacheTurn(CelConvertDirection(dwDirection,
				ptCelData->tHeader[CELHDR_DIRS], dwDirBlockSize, ptCelContext->bUnitFlags & CELCONTEXTUNIT_CONVERT),
				ptCelData);
			ptHashTableElement = GfxHashFind(&sgtGfxHash, ptCelContext, &dwDirBlock, &dwDirBlockSize, ptCelData);
#line 1611
			D2ASSERT(ptHashTableElement);
#line 1612
			D2ASSERT(dwDirBlock == 0);
		}
		return SpriteCacheLoadAsync(ptCelContext, ptCelData, dwDirBlock, dwDirBlockSize, szPath);
	}
}

void __fastcall SpriteCacheNothing(void)
{
	GfxHashNothing(&sgtGfxHash);
}

DWORD __fastcall SpriteCacheGetAllocated(void)
{
	DWORD dwAllocated;

#line 1639
	D2ASSERT(sgtGfxCache.pcs);
	EnterCriticalSection(sgtGfxCache.pcs);
	dwAllocated = sgtGfxCache.dwAllocated;
	LeaveCriticalSection(sgtGfxCache.pcs);
	return dwAllocated;
}

DWORD __fastcall SpriteCacheGetMax(void)
{
	return sgtGfxCache.dwMaxAllocated;
}

void __fastcall SpriteCacheCelDataNothing(void)
{
	CelDataHashNothing(&sgtCelDataHash);
}

DWORD __fastcall SpriteCacheGetCelDataAllocated(void)
{
	return sgtCelDataCache.dwAllocated;
}

DWORD __fastcall SpriteCacheGetCelDataMax(void)
{
	return sgtCelDataCache.dwMaxAllocated;
}

void __fastcall SpriteCacheInit(D2SPRITECACHE *ptCache, void *pPool, DWORD dwMaxAllocated,
	void (__fastcall *pfnFree)(D2GFXELEMENT *ptElement))
{
	int i;
	D2SPRITEBLOCK *ptBlock;

	ptCache->pMemPool = pPool;
	ptCache->dwMaxAllocated = dwMaxAllocated;
	ptCache->dwAllocated = 0;
	ptCache->field_2c = pfnFree;
#line 1761
	ptCache->ptBlocks = (D2SPRITEBLOCK *)D2ALLOCPOOL(pPool, SPRITECACHE_BLOCKS * sizeof(D2SPRITEBLOCK));
	memset(ptCache->ptBlocks, 0, SPRITECACHE_BLOCKS * sizeof(D2SPRITEBLOCK));
	for (i = 0; i < SPRITECACHE_BLOCKS; i++)
	{
		ptBlock = &ptCache->ptBlocks[i];
		if (i < SPRITECACHE_BLOCKS - 1)
			ptBlock->ptNext = &ptCache->ptBlocks[i + 1];
		if (i > 0)
			ptBlock->ptPrev = &ptCache->ptBlocks[i - 1];
		ptBlock->lpData = NULL;
		ptBlock->dwSize = 0;
		ptBlock->ptOwner = NULL;
	}
	ptCache->dwNumUsed = 0;
	ptCache->ptFreeHead = ptCache->ptBlocks;
	ptCache->ptFreeTail = &ptCache->ptBlocks[SPRITECACHE_BLOCKS - 1];
	ptCache->ptUsedHead = NULL;
	ptCache->ptUsedTail = NULL;
	ptCache->ptLoadHead = NULL;
	ptCache->ptLoadTail = NULL;
	ptCache->dwNumAllocs = 0;
	ptCache->dwNumUses = 0;
	if (!ptCache->pcs)
	{
#line 1801
		ptCache->pcs = (CRITICAL_SECTION *)D2ALLOC2(sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(ptCache->pcs);
	}
}

void __fastcall SpriteCacheDestroy(D2SPRITECACHE *ptCache)
{
	while (ptCache->dwNumUsed)
		SpriteCacheFree(ptCache, NULL, TRUE);
	if (ptCache->ptBlocks)
	{
#line 1825
		D2FREEPOOL(ptCache->pMemPool, ptCache->ptBlocks);
		ptCache->ptBlocks = NULL;
	}
	if (ptCache->pcs)
	{
		DeleteCriticalSection(ptCache->pcs);
#line 1843
		D2FREE2(ptCache->pcs);
		ptCache->pcs = NULL;
	}
}

D2SPRITEBLOCK * __fastcall SpriteCacheAlloc(D2SPRITECACHE *ptCache, DWORD dwSize, D2GFXELEMENT *pOwner,
	BOOL fLoad)
{
	D2SPRITEBLOCK *ptBlock;
	int nOldest;

#line 1863
	D2ASSERT(dwSize < ptCache->dwMaxAllocated);
	nOldest = 0;
	if (fLoad)
	{
		if (ptCache->ptUsedHead && (int)ptCache->ptUsedHead->dwUseTick <= nOldest)
			nOldest = (int)ptCache->ptUsedHead->dwUseTick;
		if (ptCache->ptLoadHead && nOldest >= (int)ptCache->ptLoadHead->dwUseTick)
			nOldest = (int)ptCache->ptLoadHead->dwUseTick;
	}
	ptBlock = SpriteCacheTakeFree(ptCache, dwSize, fLoad);
	while (!ptBlock)
	{
		if (fLoad && GetTickCount() - (DWORD)nOldest < 15000)
			return NULL;
		SpriteCacheFree(ptCache, NULL, TRUE);
		ptBlock = SpriteCacheTakeFree(ptCache, dwSize, fLoad);
	}
	ptBlock->fLoad = fLoad;
#line 1898
	ptBlock->lpData = D2ALLOCPOOL(ptCache->pMemPool, dwSize);
	ptBlock->dwSize = dwSize;
	ptBlock->ptOwner = pOwner;
	ptCache->dwNumAllocs++;
	ptCache->dwNumUses++;
	return ptBlock;
}

static D2SPRITEBLOCK * __fastcall SpriteCacheTakeFree(D2SPRITECACHE *ptCache, DWORD dwSize, BOOL fLoad)
{
	D2SPRITEBLOCK *ptBlock = NULL;
	D2SPRITEBLOCK *ptHead;

#line 1923
	D2ASSERT(ptCache->pcs);
	EnterCriticalSection(ptCache->pcs);
	if (ptCache->dwNumUsed != SPRITECACHE_BLOCKS && dwSize + ptCache->dwAllocated <= ptCache->dwMaxAllocated)
	{
		ptHead = ptCache->ptFreeHead;
		if (!ptHead)
			ptBlock = NULL;
		else
		{
			ptCache->ptFreeHead = ptHead->ptNext;
			if (ptCache->ptFreeHead)
				ptCache->ptFreeHead->ptPrev = NULL;
			else
				ptCache->ptFreeTail = NULL;
			ptHead->ptNext = NULL;
			ptHead->ptPrev = NULL;
			ptBlock = ptHead;
		}
		SpriteCacheLink(ptCache, ptBlock, fLoad);
		D2_OPAQUE(ptBlock);
#line 1937
		D2ASSERT(ptBlock);
		ptCache->dwNumUsed++;
		ptCache->dwAllocated += dwSize;
	}
	LeaveCriticalSection(ptCache->pcs);
	return ptBlock;
}

D2SPRITEBLOCK * __fastcall SpriteCacheFree(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock, BOOL fLoads)
{
	if (!ptCache->dwNumUsed)
		return NULL;
	if (!ptBlock)
	{
		if (!ptCache->ptLoadHead)
			ptBlock = SpriteCachePopHead(ptCache, FALSE);
		else if (!ptCache->ptUsedHead)
			ptBlock = SpriteCachePopHead(ptCache, fLoads);
		else if (fLoads)
			ptBlock = SpriteCachePopHead(ptCache, ptCache->ptLoadHead->dwUseTick
				< ptCache->ptUsedHead->dwUseTick);
		else
			ptBlock = SpriteCachePopHead(ptCache, FALSE);
	}
	else
		SpriteCacheUnlink(ptCache, ptBlock);
	if (ptBlock)
	{
#line 1994
		D2ASSERT(ptCache->pcs);
		EnterCriticalSection(ptCache->pcs);
		ptCache->dwAllocated -= ptBlock->dwSize;
		LeaveCriticalSection(ptCache->pcs);
		if (ptBlock->ptOwner)
			ptCache->field_2c(ptBlock->ptOwner);
		if (ptBlock->lpData)
		{
#line 2011
			D2FREEPOOL(ptCache->pMemPool, ptBlock->lpData);
			ptBlock->lpData = NULL;
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
		ptCache->dwNumUsed--;
	}
	return ptBlock;
}

void __fastcall SpriteCacheTouch(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock)
{
	if (!ptCache->dwNumUsed)
		return;
	ptCache->dwNumUses++;
#line 2043
	D2ASSERT(ptBlock);
	SpriteCacheUnlink(ptCache, ptBlock);
	ptBlock->fLoad = FALSE;
	SpriteCacheLink(ptCache, ptBlock, FALSE);
}

void __fastcall SpriteCacheGrow(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock, DWORD dwMore, BOOL fLoads)
{
	D2SPRITEBLOCK *ptFreedBlock;
	DWORD dwAllocated;
	DWORD dwNewSize;

#line 2065
	D2ASSERT(ptCache->pcs);
	EnterCriticalSection(ptCache->pcs);
	dwNewSize = ptBlock->dwSize + dwMore;
	ptCache->dwAllocated -= ptBlock->dwSize;
	ptBlock->dwSize = dwNewSize;
	ptCache->dwAllocated += dwNewSize;
	dwAllocated = ptCache->dwAllocated;
	LeaveCriticalSection(ptCache->pcs);
	while (dwAllocated > ptCache->dwMaxAllocated)
	{
		ptFreedBlock = SpriteCacheFree(ptCache, NULL, fLoads);
		if (!ptFreedBlock)
			break;
#line 2084
		D2ASSERT(ptFreedBlock != ptBlock);
		EnterCriticalSection(ptCache->pcs);
		dwAllocated = ptCache->dwAllocated;
		LeaveCriticalSection(ptCache->pcs);
	}
}

static void __fastcall SpriteCacheLink(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock, BOOL fLoad)
{
	if (fLoad)
	{
		do
		{
			if (!ptCache->ptLoadTail)
			{
				ptCache->ptLoadHead = ptBlock;
				ptCache->ptLoadTail = ptBlock;
			}
			else
			{
				ptCache->ptLoadTail->ptNext = ptBlock;
				ptBlock->ptPrev = ptCache->ptLoadTail;
				ptCache->ptLoadTail = ptBlock;
			}
		}
		while (0);
	}
	else
	{
		if (!ptCache->ptUsedTail)
		{
			ptCache->ptUsedHead = ptBlock;
			ptCache->ptUsedTail = ptBlock;
		}
		else
		{
			ptCache->ptUsedTail->ptNext = ptBlock;
			ptBlock->ptPrev = ptCache->ptUsedTail;
			ptCache->ptUsedTail = ptBlock;
		}
	}
	ptBlock->dwUseTick = GetTickCount();
}

static D2SPRITEBLOCK * __fastcall SpriteCachePopHead(D2SPRITECACHE *ptCache, BOOL fLoad)
{
	D2SPRITEBLOCK *ptBlock;
	D2SPRITEBLOCK *ptHead;
	D2SPRITEBLOCK **pptNext;

	if (fLoad)
	{
		ptHead = ptCache->ptLoadHead;
		if (!ptHead)
			return NULL;
		ptBlock = ptHead;
		pptNext = &ptBlock->ptNext;
		ptHead = *pptNext;
		ptCache->ptLoadHead = ptHead;
		if (ptHead)
			ptHead->ptPrev = NULL;
		else
			ptCache->ptLoadTail = NULL;
	}
	else
	{
		ptHead = ptCache->ptUsedHead;
		if (!ptHead)
			return NULL;
		ptBlock = ptHead;
		pptNext = &ptBlock->ptNext;
		ptHead = *pptNext;
		ptCache->ptUsedHead = ptHead;
		if (ptHead)
			ptHead->ptPrev = NULL;
		else
			ptCache->ptUsedTail = NULL;
	}
	*pptNext = NULL;
	ptBlock->ptPrev = NULL;
	return ptBlock;
}

static void __fastcall SpriteCacheUnlink(D2SPRITECACHE *ptCache, D2SPRITEBLOCK *ptBlock)
{
#line 2257
	D2ASSERT(ptBlock);
	if (ptBlock->fLoad)
	{
		if (!ptCache->ptLoadHead)
			return;
		if (ptBlock == ptCache->ptLoadHead)
		{
			SpriteCachePopHead(ptCache, TRUE);
			return;
		}
		if (ptBlock == ptCache->ptLoadTail)
		{
			ptCache->ptLoadTail = ptCache->ptLoadTail->ptPrev;
			ptCache->ptLoadTail->ptNext = NULL;
			ptBlock->ptNext = NULL;
			ptBlock->ptPrev = NULL;
			return;
		}
	}
	else
	{
		if (!ptCache->ptUsedHead)
			return;
		if (ptBlock == ptCache->ptUsedHead)
		{
			SpriteCachePopHead(ptCache, FALSE);
			return;
		}
		if (ptBlock == ptCache->ptUsedTail)
		{
			ptCache->ptUsedTail = ptCache->ptUsedTail->ptPrev;
			ptCache->ptUsedTail->ptNext = NULL;
			ptBlock->ptNext = NULL;
			ptBlock->ptPrev = NULL;
			return;
		}
	}
	ptBlock->ptPrev->ptNext = ptBlock->ptNext;
	ptBlock->ptNext->ptPrev = ptBlock->ptPrev;
	ptBlock->ptNext = NULL;
	ptBlock->ptPrev = NULL;
}

int __fastcall SpriteCacheCountLoading(void)
{
	D2SPRITEBLOCK *ptBlock;
	D2GFXNODE *ptGfxNode;
	void *hAsyncData;
	int nLoading = 0;

	for (ptBlock = sgtGfxCache.ptLoadHead; ptBlock; ptBlock = ptBlock->ptNext)
	{
		ptGfxNode = (D2GFXNODE *)ptBlock->lpData;
		if (!ptGfxNode || (ptGfxNode->tGfxBlock.bFlags & GFXBLOCK_NOFILE))
			continue;
		hAsyncData = ptGfxNode->tGfxBlock.lpAsyncData;
#line 2346
		D2ASSERT(hAsyncData);
		if (!FogAsyncDataIsLoaded(hAsyncData))
			nLoading++;
	}
	return nLoading;
}

BOOL WINAPI SpriteCacheIsLoading(DWORD dwValue0, DWORD dwMode, DWORD dwClass)
{
	D2SPRITEBLOCK *ptBlock;
	D2GFXNODE *ptGfxNode;
	void *hAsyncData;

	for (ptBlock = sgtGfxCache.ptLoadHead; ptBlock; ptBlock = ptBlock->ptNext)
	{
		ptGfxNode = (D2GFXNODE *)ptBlock->lpData;
		if (!ptGfxNode || (ptGfxNode->tGfxBlock.bFlags & GFXBLOCK_NOFILE) || ptGfxNode->dwClassId
			!= dwClass ||
			ptGfxNode->dwMode != dwMode || ptGfxNode->field_00 != dwValue0)
			continue;
		hAsyncData = ptGfxNode->tGfxBlock.lpAsyncData;
#line 2378
		D2ASSERT(hAsyncData);
		if (!FogAsyncDataIsLoaded(hAsyncData))
			return TRUE;
	}
	return FALSE;
}

void __fastcall SpriteCacheNothingSecond(void)
{
}

void __fastcall SpriteCacheAbandonLoads(void)
{
	D2SPRITEBLOCK *ptBlock;
	D2GFXNODE *ptGfxNode;
	void *hAsyncData;

	for (ptBlock = sgtGfxCache.ptLoadHead; ptBlock; ptBlock = ptBlock->ptNext)
	{
		ptGfxNode = (D2GFXNODE *)ptBlock->lpData;
		if (!ptGfxNode)
			continue;
		hAsyncData = ptGfxNode->tGfxBlock.lpAsyncData;
		if (hAsyncData && !FogAsyncDataIsLoaded(hAsyncData))
		{
			FogAsyncDataFree(hAsyncData);
			ptGfxNode->tGfxBlock.lpAsyncData = NULL;
		}
	}
}
