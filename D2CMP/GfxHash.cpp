#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\Src\\GfxHash.cpp"
#include "D2Cmp.h"
#include "D2Cmp.h"
#include "StormImports.h"
#include "D2Macros.h"
#include "FogImports.h"
#line 13

void __fastcall GfxHashInit(D2GFXHASH *ptHashTable)
{
	memset(ptHashTable, 0, sizeof(D2GFXHASH));
}

static __inline BOOL GfxHashByFile(D2CELCONTEXT *ptCelContext)
{
	return CELCONTEXT_ISOVERLAY(ptCelContext) ||
		ptCelContext->dwUnitType == UNITGFX_MISSILE || ptCelContext->dwUnitType == UNITGFX_ITEM;
}

static DWORD __fastcall GfxHashKey(D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData, DWORD *pdwDirBlock,
	DWORD *pdwBlockSize)
{
	DWORD dwHash;
	*pdwDirBlock = GfxHashDirBlock(ptCelContext, pdwBlockSize, ptCelData);
	if (!GfxHashByFile(ptCelContext))
	{
		dwHash = ptCelContext->tUnitInfo.dwWClassToken + ptCelContext->tUnitInfo.dwModeToken +
			ptCelContext->tUnitInfo.dwVariantToken + ptCelContext->tUnitInfo.dwComponentToken +
			ptCelContext->tUnitInfo.dwUnitToken + *pdwDirBlock;
	}
	else
	{
#line 44
		D2ASSERT(ptCelContext->tUnitInfo.pszFileName);
		dwHash = StormSStrHash(ptCelContext->tUnitInfo.pszFileName, 1, 0) + *pdwDirBlock;
	}
	return ((((dwHash & 536805376) ^ (dwHash >> 8)) >> 8 ^ (dwHash & 2096896)) >> 8) ^ (dwHash & 8191);
}

D2GFXELEMENT * __fastcall GfxHashFind(D2GFXHASH *ptHashTable, D2CELCONTEXT *ptCelContext, DWORD *pdwDirBlock,
	DWORD *pdwBlockSize, D2CELDATA *ptCelData)
{
	D2GFXELEMENT *ptElement;
	D2UNITINFO *ptUnitInfo;

	ptElement = ptHashTable->ptBuckets[GfxHashKey(ptCelContext, ptCelData, pdwDirBlock, pdwBlockSize)];
	ptUnitInfo = &ptCelContext->tUnitInfo;
	DWORD dwUnit = ptUnitInfo->dwUnitToken, dwComponent = ptUnitInfo->dwComponentToken, dwVariant = ptUnitInfo->dwVariantToken;
	for (; ptElement; ptElement = ptElement->ptNext)
	{
		if (CELCONTEXT_ISOVERLAY(ptCelContext))
		{
			if (dwUnit == ptElement->adwTokens[0] &&
				dwComponent == ptElement->adwTokens[1] &&
				dwVariant == ptElement->adwTokens[2] &&
				ptUnitInfo->dwModeToken == ptElement->adwTokens[3] &&
				ptUnitInfo->dwWClassToken == ptElement->adwTokens[4] &&
				ptCelContext->dwOverlayClass == ptElement->dwClass &&
				*pdwDirBlock == ptElement->dwDirBlock)
				return ptElement;
		}
		else if (ptCelContext->dwUnitType == UNITGFX_ITEM || ptCelContext->dwUnitType == UNITGFX_MISSILE)
		{
			if (dwUnit == ptElement->adwTokens[0] &&
				dwComponent == ptElement->adwTokens[1] &&
				dwVariant == ptElement->adwTokens[2] &&
				ptUnitInfo->dwModeToken == ptElement->adwTokens[3] &&
				ptUnitInfo->dwWClassToken == ptElement->adwTokens[4] &&
				ptCelContext->dwClassId == ptElement->dwClass &&
				*pdwDirBlock == ptElement->dwDirBlock)
				return ptElement;
		}
		else
		{
			if (dwUnit == ptElement->adwTokens[0] &&
				dwComponent == ptElement->adwTokens[1] &&
				dwVariant == ptElement->adwTokens[2] &&
				ptUnitInfo->dwModeToken == ptElement->adwTokens[3] &&
				ptUnitInfo->dwWClassToken == ptElement->adwTokens[4] &&
				*pdwDirBlock == ptElement->dwDirBlock)
				return ptElement;
		}
	}
	return NULL;
}

D2GFXELEMENT * __fastcall GfxHashAdd(void *pPool, D2GFXHASH *ptHashTable, D2GFXNODE *ptGfxNode,
	D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData)
{
	D2GFXELEMENT *ptElement;
	DWORD dwBlockSize;
	DWORD dwKey;

#line 234
	ptElement = (D2GFXELEMENT *)D2ALLOCPOOL(pPool, sizeof(D2GFXELEMENT));
	ptElement->ptGfxNode = ptGfxNode;
	dwBlockSize = 0;
	dwKey = GfxHashKey(ptCelContext, ptCelData, &ptElement->dwDirBlock, &dwBlockSize);
	ptElement->adwTokens[0] = ptCelContext->tUnitInfo.dwUnitToken;
	ptElement->adwTokens[1] = ptCelContext->tUnitInfo.dwComponentToken;
	ptElement->adwTokens[2] = ptCelContext->tUnitInfo.dwVariantToken;
	ptElement->adwTokens[3] = ptCelContext->tUnitInfo.dwModeToken;
	ptElement->adwTokens[4] = ptCelContext->tUnitInfo.dwWClassToken;
	if (CELCONTEXT_ISOVERLAY(ptCelContext))
		ptElement->dwClass = ptCelContext->dwOverlayClass;
	else if (ptCelContext->dwUnitType == UNITGFX_ITEM || ptCelContext->dwUnitType == UNITGFX_MISSILE)
		ptElement->dwClass = ptCelContext->dwClassId;
	ptElement->ptPrev = NULL;
	ptElement->ptNext = ptHashTable->ptBuckets[dwKey];
	if (ptElement->ptNext)
		ptElement->ptNext->ptPrev = ptElement;
	ptElement->dwKey = dwKey;
	ptHashTable->ptBuckets[dwKey] = ptElement;
	return ptElement;
}

static __inline void GfxHashFreeElement(void *pPool, D2GFXHASH *ptHashTable, D2GFXELEMENT *ptElement)
{
	D2GFXNODE *ptGfxNode;
	void *hAsyncData;
	void (__fastcall *pfnFreeHardware)(D2CELSLOT *ptSlot);
	BYTE *lpbData;
	D2CELSLOT *lpParent;
	D2CEL *pCel;
	DWORD dwFrame;
	DWORD dwOffset;

	if (ptHashTable->ptBuckets[ptElement->dwKey] == ptElement)
		ptHashTable->ptBuckets[ptElement->dwKey] = NULL;
	ptGfxNode = ptElement->ptGfxNode;
	hAsyncData = ptGfxNode->tGfxBlock.lpAsyncData;
	if (hAsyncData)
	{
#line 85
		D2ASSERTMSG(ptGfxNode->tGfxBlock.lpAsyncData, "hAsyncData");
		if (ptGfxNode->tGfxBlock.fHardwareCelData)
		{
			pfnFreeHardware = (void (__fastcall *)(D2CELSLOT *))(INT_PTR)CelGetHardwareFree();
			lpbData = FogAsyncDataGetBuffer(hAsyncData);
#line 92
			D2ASSERT(lpbData);
			dwOffset = 0;
			lpParent = ((D2CEL *)lpbData)->ptSlot;
#line 98
			D2ASSERT(lpParent);
			for (dwFrame = 0; dwFrame
				< (DWORD)ptGfxNode->tGfxBlock.bCelsPerDirection * ptGfxNode->tGfxBlock.bNumDirections;
				dwFrame++)
			{
				pCel = (D2CEL *)(lpbData + dwOffset);
				if (pCel->ptSlot)
				{
					if (pfnFreeHardware)
						pfnFreeHardware(pCel->ptSlot);
					pCel->ptSlot = NULL;
				}
				dwOffset += CELSIZE(pCel->dwDataSize + pCel->dwExtraSize);
			}
#line 126
			D2FREEPOOL(pPool, lpParent);
		}
		FogAsyncDataFree(hAsyncData);
	}
#line 137
	D2FREEPOOL(pPool, ptElement);
}

void __fastcall GfxHashRemove(void *pPool, D2GFXHASH *ptHashTable, D2GFXELEMENT *ptElement)
{
#line 291
	D2ASSERT(ptElement);
	if (ptElement->ptPrev)
		ptElement->ptPrev->ptNext = ptElement->ptNext;
	if (ptElement->ptNext)
		ptElement->ptNext->ptPrev = ptElement->ptPrev;
	GfxHashFreeElement(pPool, ptHashTable, ptElement);
}

void __fastcall GfxHashNothing(D2GFXHASH *ptHashTable)
{
}

DWORD __fastcall GfxHashDirBlock(D2CELCONTEXT *ptCelContext, DWORD *pdwBlockSize, D2CELDATA *ptCelData)
{
	DWORD dwDirection = 0;

	*pdwBlockSize = 0;
	switch (ptCelContext->dwUnitType)
	{
	case UNITGFX_PLAYER:
		*pdwBlockSize = 8;
		if (!ptCelContext->dwMode || ptCelContext->dwMode == 17 || (ptCelContext->dwFlags & 2))
			*pdwBlockSize = 1;
		if (!(ptCelData->bFlags & CELDATAFLAG_NOFILE))
			dwDirection = CelGetGameDirection(ptCelContext->dwDirection,
				ptCelData->tHeader[CELHDR_DIRS]);
		break;
	case UNITGFX_MONSTER:
		if (!(ptCelData->bFlags & CELDATAFLAG_NOFILE))
		{
			*pdwBlockSize = ptCelData->tHeader[CELHDR_DIRS] == 8 ? 4 : 1;
			dwDirection = CelGetGameDirection(ptCelContext->dwDirection,
				ptCelData->tHeader[CELHDR_DIRS]);
		}
		break;
	default:
		*pdwBlockSize = 1;
		if (ptCelContext->dwUnitType == UNITGFX_OBJECT)
			break;
		if (!(CELCONTEXT_ISOVERLAY(ptCelContext) || ptCelContext->dwUnitType == UNITGFX_ITEM ||
			ptCelContext->dwUnitType == UNITGFX_MISSILE))
			break;
		if (ptCelContext->dwUnitType == UNITGFX_ITEM)
			dwDirection = ptCelContext->dwDirection;
		else if (!(ptCelData->bFlags & CELDATAFLAG_NOFILE))
			dwDirection = CelGetGameDirection(ptCelContext->dwDirection,
				ptCelData->tHeader[CELHDR_DIRS]);
		break;
	}
	if (!*pdwBlockSize)
		return 0;
	return dwDirection / *pdwBlockSize;
}

void __fastcall GfxHashCheck(D2GFXHASH *ptHashTable)
{
}
