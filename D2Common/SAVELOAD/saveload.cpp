#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\SaveLoad\\saveload.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "FogImports.h"

#define ITEMSAVE_TAG		19786
#define ITEMSAVE_PACKED		25

#pragma pack(push, 1)
struct ITEMSAVE
{
 	BYTE		bLocation;
 	BYTE		bNumItemsAttached;
 	int			nLevelId;
 	DWORD		nId;
 	BYTE		bMode;
 	BYTE		bSpecMods;
        DWORD           nValue;
 	DWORD		dwDurability;
 	DWORD		dwMaxDurability;
 	DWORD		dwX;
 	DWORD		dwY;
 	int			nModClass;
 	DWORD		dwFlags;
 	DWORD		dwInitSeed;
 	DWORD		dwItemSeed;
 	BYTE		bPage;
 	BYTE		bEarLevel;
 	char		szPlayerName[16];
 	BYTE		unobserved_42[72 - 66];
};
#pragma pack(pop)
typedef struct ITEMSAVE ITEMSAVE, *PTITEMSAVE;

struct BITBUFFER
{
 	BYTE		*pbData;
 	int			nLength;
 	int			nCurByte;
 	int			nCurBit;
};
typedef struct BITBUFFER BITBUFFER;

static int __fastcall ItemSavePackItem(HUNIT hItem, BYTE *pbBuffer, DWORD nSize);
static BOOL __fastcall ItemSavePack(BYTE *pbBuffer, PTITEMSAVE ptItemSave, int nBytes);

PTITEMSAVE __stdcall ItemSaveGet(PTITEMSAVE ptItemSave, HUNIT hItem)
{
	ITEMSAVE tItemSave;
	DWORD *pdwClear;
	DWORD dwNode;
	DWORD dwInventory;
	int nQuantity;
	int i;

	pdwClear = (DWORD *)&tItemSave;
	for (i = 0; i < (int)(sizeof(ITEMSAVE) / sizeof(DWORD)); i++)
		pdwClear[i] = 0;
	tItemSave.bLocation = D2CommonITEMSGetCategory(hItem);
	if (D2CommonITEMSGetTypeHasInv(hItem))
	{
		dwInventory = hItem->ptInventory;
		dwNode = (DWORD)D2CommonINVENTORYGetCategoryListHead(dwInventory);
		if (dwNode)
		{
			do
			{
				tItemSave.bNumItemsAttached++;
				dwNode = D2CommonINVENTORYNodeGetNext(dwNode);
			} while (dwNode);
		}
	}
	tItemSave.nLevelId = (int)D2CommonITEMSGetLevelId(hItem);
	tItemSave.nId = hItem->dwClassId;
	tItemSave.bMode = (BYTE)hItem->dwMode;
	tItemSave.bSpecMods = (BYTE)D2CommonITEMSDataGetModType(hItem);
	if (D2CommonITEMSGetType(hItem) == 4)
		tItemSave.nValue = D2CommonStatsUnitGetValue(hItem, 14);
	else
	{
		nQuantity = D2CommonStatsUnitGetValue(hItem, 70);
#line 133
		D2ASSERT(nQuantity >= 0);
		tItemSave.nValue = nQuantity;
	}
	tItemSave.dwDurability = D2CommonStatsUnitGetValue(hItem, 72);
	tItemSave.dwMaxDurability = D2CommonStatsUnitGetBaseValue(hItem, 73);
	tItemSave.dwX = D2CommonUnitGetX(hItem);
	tItemSave.dwY = D2CommonUnitGetY(hItem);
	tItemSave.nModClass = (int)D2CommonITEMSDataGetUniqueOrSetRow(hItem);
	tItemSave.dwInitSeed = hItem->dwInitSeed;
	tItemSave.dwItemSeed = D2CommonITEMSGetItemSeedValue(hItem);
	tItemSave.dwFlags = D2CommonITEMSDataGetFlags(hItem);
	tItemSave.bPage = D2CommonITEMSDataGetPage(hItem);
	if (D2CommonITEMSGetType(hItem) == 7)
	{
		strcpy(tItemSave.szPlayerName, D2CommonITEMSGetNameBuffer(hItem));
		tItemSave.bEarLevel = D2CommonITEMSDataGetEarLevel(hItem);
	}
	*ptItemSave = tItemSave;
	return ptItemSave;
}

static int ItemSaveCountItems(PTINVENTORY hInventory)
{
	int nCount;
	DWORD dwNode;
	HUNIT hItem;

	nCount = 0;
	if (hInventory == NULL)
		return 0;
	hItem = (HUNIT)D2CommonINVENTORYGetCursorItem(hInventory);
	if (hItem != NULL && !(hItem->dwFlags & 32768))
		nCount = 1;
	for (dwNode = (DWORD)D2CommonINVENTORYGetCategoryListHead(
		hInventory); dwNode; dwNode = D2CommonINVENTORYNodeGetNext(dwNode))
	{
		hItem = (HUNIT)D2CommonINVENTORYNodeGetKeyA(dwNode);
#line 264
		D2ASSERT(hItem);
		if (!(hItem->dwFlags & 32768))
			nCount++;
	}
	return nCount;
}

int __stdcall ItemSavePackList(PTINVENTORY hInventory, BYTE *pbBuffer, DWORD nSize, DWORD dwUnused)
{
	int nCount;
	int nTotal;
	int nWritten;
	DWORD dwNode;
	HUNIT hItem;
	HUNIT hFirst;
	HUNIT hLeft;
	HUNIT hRight;
	BYTE bLocation;

	*(WORD *)pbBuffer = ITEMSAVE_TAG;

	pbBuffer += sizeof(WORD);
	nSize -= sizeof(WORD);
	nCount = ItemSaveCountItems(hInventory);

	*(WORD *)pbBuffer = (WORD)nCount;
	pbBuffer += sizeof(WORD);
	nSize -= sizeof(WORD);
	nTotal = 2 * sizeof(WORD);
	if (hInventory == NULL)
		return nTotal;

	dwNode = (DWORD)D2CommonINVENTORYGetCategoryListHead(hInventory);
	while (dwNode)
	{
		hItem = (HUNIT)D2CommonINVENTORYNodeGetKeyA(dwNode);
		bLocation = D2CommonITEMSGetCategory(hItem);
		if (D2CommonINVENTORYNodeGetCategory(dwNode) == 3 && (bLocation == 4 || bLocation == 5))
		{
			dwNode = D2CommonINVENTORYNodeGetNext(dwNode);
			continue;
		}
		nWritten = ItemSavePackItem(hItem, pbBuffer, nSize);
		pbBuffer += nWritten;
		nTotal += nWritten;
		nSize -= nWritten;
		dwNode = D2CommonINVENTORYNodeGetNext(dwNode);
	}

	hFirst = (HUNIT)D2CommonINVENTORYFindEquippedMatch(hInventory);
	hLeft = (HUNIT)D2CommonINVENTORYGetLocSlot(hInventory, 4);
	hRight = (HUNIT)D2CommonINVENTORYGetLocSlot(hInventory, 5);
	if (hFirst != NULL)
	{
		if (hLeft == hFirst)
		{
			nWritten = ItemSavePackItem(hLeft, pbBuffer, nSize);
			pbBuffer += nWritten;
			nTotal += nWritten;
			nSize -= nWritten;
			if (hRight != NULL)
			{
				nWritten = ItemSavePackItem(hRight, pbBuffer, nSize);
				pbBuffer += nWritten;
				nTotal += nWritten;
				nSize -= nWritten;
			}
		}
		else if (hRight == hFirst)
		{
			nWritten = ItemSavePackItem(hRight, pbBuffer, nSize);
			pbBuffer += nWritten;
			nTotal += nWritten;
			nSize -= nWritten;
			if (hLeft != NULL)
			{
				nWritten = ItemSavePackItem(hLeft, pbBuffer, nSize);
				pbBuffer += nWritten;
				nTotal += nWritten;
				nSize -= nWritten;
			}
		}
	}
	else
	{
		if (hLeft != NULL)
		{
			nWritten = ItemSavePackItem(hLeft, pbBuffer, nSize);
			pbBuffer += nWritten;
			nTotal += nWritten;
			nSize -= nWritten;
		}
		if (hRight != NULL)
		{
			nWritten = ItemSavePackItem(hRight, pbBuffer, nSize);
			pbBuffer += nWritten;
			nTotal += nWritten;
			nSize -= nWritten;
		}
	}

	hItem = (HUNIT)D2CommonINVENTORYGetCursorItem(hInventory);
	if (hItem != NULL)
		nTotal += ItemSavePackItem(hItem, pbBuffer, nSize);
	return nTotal;
}

static int __fastcall ItemSavePackItem(HUNIT hItem, BYTE *pbBuffer, DWORD nSize)
{
	ITEMSAVE tNodeResult;
	ITEMSAVE tResult;
	ITEMSAVE tPacked;
	ITEMSAVE tItemSave;
	ITEMSAVE tNodeSave;
	DWORD dwInventory;
	DWORD dwNode;
	int nTotal;

	if (hItem->dwFlags & 32768)
		return 0;
	if (nSize < sizeof(ITEMSAVE))
	{
#line 182
		D2FATAL("Out of memory in save buffer.\n");
	}
	tItemSave = *ItemSaveGet(&tResult, hItem);
	*(WORD *)pbBuffer = ITEMSAVE_TAG;
	pbBuffer += sizeof(WORD);
	tPacked = tItemSave;
	ItemSavePack(pbBuffer, &tPacked, ITEMSAVE_PACKED);
	pbBuffer += ITEMSAVE_PACKED;
	nTotal = sizeof(WORD) + ITEMSAVE_PACKED;
	nSize -= sizeof(WORD) + ITEMSAVE_PACKED;
	if (tPacked.bNumItemsAttached > 0)
	{
		dwInventory = hItem->ptInventory;
		for (dwNode = (DWORD)D2CommonINVENTORYGetCategoryListHead(
			dwInventory); dwNode; dwNode = D2CommonINVENTORYNodeGetNext(dwNode))
		{
			if (nSize < sizeof(ITEMSAVE))
			{
#line 211
				D2FATAL("Out of memory in save buffer.\n");
			}
			tNodeSave = *ItemSaveGet(&tNodeResult, (HUNIT)D2CommonINVENTORYNodeGetKeyA(dwNode));
			*(WORD *)pbBuffer = ITEMSAVE_TAG;
			pbBuffer += sizeof(WORD);
			tResult = tNodeSave;
			ItemSavePack(pbBuffer, &tResult, ITEMSAVE_PACKED);
			pbBuffer += ITEMSAVE_PACKED;
			nTotal += sizeof(WORD) + ITEMSAVE_PACKED;
			nSize -= sizeof(WORD) + ITEMSAVE_PACKED;
		}
	}
	return nTotal;
}

static BOOL __fastcall ItemSavePack(BYTE *pbBuffer, PTITEMSAVE ptItemSave, int nBytes)
{
	BITBUFFER tBuffer;
	int i;

	FogBitManipInitBuffer(&tBuffer, pbBuffer, ITEMSAVE_PACKED);
	FogBitManipWrite(&tBuffer, ptItemSave->dwFlags, 32);
	if (D2CommonITEMSMaskFlags(ptItemSave->dwFlags, 65536))
	{
		FogBitManipWrite(&tBuffer, ptItemSave->bLocation, 5);
#line 46
		D2ASSERT(ptItemSave->bLocation < 32);
		FogBitManipWrite(&tBuffer, ptItemSave->nId, 10);
#line 48
		D2ASSERT(ptItemSave->nId < 1024);
		FogBitManipWrite(&tBuffer, ptItemSave->bMode, 3);
#line 50
		D2ASSERT(ptItemSave->bMode < 8);
		if (ptItemSave->dwX >= 32)
			ptItemSave->dwX = 0;
		FogBitManipWrite(&tBuffer, ptItemSave->dwX, 5);
		if (ptItemSave->dwY >= 8)
			ptItemSave->dwX = 0;
		FogBitManipWrite(&tBuffer, ptItemSave->dwY, 3);
		FogBitManipWrite(&tBuffer, ptItemSave->nModClass, 10);
#line 59
		D2ASSERT((int)ptItemSave->nModClass < 512 && (int)ptItemSave->nModClass >= -512);
		FogBitManipWrite(&tBuffer, ptItemSave->bPage, 8);
		FogBitManipWrite(&tBuffer, ptItemSave->bEarLevel, 8);
		for (i = 0; i < sizeof(ptItemSave->szPlayerName); i++)
		{
#line 65
			D2ASSERT((int)ptItemSave->szPlayerName[i] < 128 && (int)ptItemSave->szPlayerName[i] >= -128);
			FogBitManipWrite(&tBuffer, ptItemSave->szPlayerName[i], 7);
		}
		FogBitManipWrite(&tBuffer, 0, 4);
		return TRUE;
	}
	FogBitManipWrite(&tBuffer, ptItemSave->bLocation, 5);
#line 74
	D2ASSERT(ptItemSave->bLocation < 32);
	FogBitManipWrite(&tBuffer, ptItemSave->bNumItemsAttached, 3);
#line 76
	D2ASSERT(ptItemSave->bNumItemsAttached < 8);
	FogBitManipWrite(&tBuffer, ptItemSave->nLevelId, 12);
#line 78
	D2ASSERT(ptItemSave->nLevelId < 4096);
	FogBitManipWrite(&tBuffer, ptItemSave->nId, 10);
#line 80
	D2ASSERT(ptItemSave->nId < 1024);
	FogBitManipWrite(&tBuffer, ptItemSave->bMode, 3);
#line 82
	D2ASSERT(ptItemSave->bMode < 8);
	FogBitManipWrite(&tBuffer, ptItemSave->bSpecMods, 4);
#line 84
	D2ASSERT(ptItemSave->bSpecMods < 16);
	FogBitManipWrite(&tBuffer, ptItemSave->nValue, 20);
#line 86
	D2ASSERT(ptItemSave->nValue < (1<<20));
	FogBitManipWrite(&tBuffer, ptItemSave->dwDurability, 8);
	FogBitManipWrite(&tBuffer, ptItemSave->dwMaxDurability, 8);
	if (ptItemSave->dwX >= 32)
		ptItemSave->dwX = 0;
	FogBitManipWrite(&tBuffer, ptItemSave->dwX, 5);
	if (ptItemSave->dwY >= 8)
		ptItemSave->dwY = 0;
	FogBitManipWrite(&tBuffer, ptItemSave->dwY, 3);
	FogBitManipWrite(&tBuffer, ptItemSave->nModClass, 10);
#line 97
	D2ASSERT((int)ptItemSave->nModClass < 512 && (int)ptItemSave->nModClass >= -512);
	FogBitManipWrite(&tBuffer, ptItemSave->dwInitSeed, 32);
	FogBitManipWrite(&tBuffer, ptItemSave->dwItemSeed, 32);
	FogBitManipWrite(&tBuffer, ptItemSave->bPage, 8);
	FogBitManipWrite(&tBuffer, 0, 5);
	return TRUE;
}

BOOL __stdcall ItemSaveUnpack(PTITEMSAVE ptItemSave, BYTE *pbBuffer)
{
	BITBUFFER tBuffer;
	DWORD i;
	BYTE bChar;

	for (i = 0; i < sizeof(ITEMSAVE) / sizeof(DWORD); i++)
		((DWORD *)ptItemSave)[i] = 0;
	FogBitManipInitBuffer(&tBuffer, pbBuffer, ITEMSAVE_PACKED);
	ptItemSave->dwFlags = FogBitManipReadUnsigned(&tBuffer, 32);
	if (D2CommonITEMSMaskFlags(ptItemSave->dwFlags, 65536))
	{
		ptItemSave->bLocation = (BYTE)FogBitManipReadUnsigned(&tBuffer, 5);
		ptItemSave->nLevelId = 1;
		ptItemSave->nId = FogBitManipReadSigned(&tBuffer, 10);
		ptItemSave->bMode = (BYTE)FogBitManipReadUnsigned(&tBuffer, 3);
		ptItemSave->dwX = (BYTE)FogBitManipReadUnsigned(&tBuffer, 5);
		ptItemSave->dwY = (BYTE)FogBitManipReadUnsigned(&tBuffer, 3);
		ptItemSave->nModClass = FogBitManipReadSigned(&tBuffer, 10);
		ptItemSave->bPage = (BYTE)FogBitManipReadUnsigned(&tBuffer, 8);
		ptItemSave->bEarLevel = (BYTE)FogBitManipReadUnsigned(&tBuffer, 8);
		for (i = 0; i < 16; i++)
		{
			bChar = (BYTE)FogBitManipReadUnsigned(&tBuffer, 7);
			ptItemSave->szPlayerName[i] = bChar;
			if (!bChar)
				return TRUE;
		}
		return TRUE;
	}
	ptItemSave->bLocation = (BYTE)FogBitManipReadUnsigned(&tBuffer, 5);
	ptItemSave->bNumItemsAttached = (BYTE)FogBitManipReadUnsigned(&tBuffer, 3);
	ptItemSave->nLevelId = (BYTE)FogBitManipReadUnsigned(&tBuffer, 12);
	ptItemSave->nId = FogBitManipReadSigned(&tBuffer, 10);
	ptItemSave->bMode = (BYTE)FogBitManipReadUnsigned(&tBuffer, 3);
	ptItemSave->bSpecMods = (BYTE)FogBitManipReadUnsigned(&tBuffer, 4);
	ptItemSave->nValue = FogBitManipReadSigned(&tBuffer, 20);
	ptItemSave->dwDurability = (BYTE)FogBitManipReadUnsigned(&tBuffer, 8);
	ptItemSave->dwMaxDurability = (BYTE)FogBitManipReadUnsigned(&tBuffer, 8);
	ptItemSave->dwX = (BYTE)FogBitManipReadUnsigned(&tBuffer, 5);
	ptItemSave->dwY = (BYTE)FogBitManipReadUnsigned(&tBuffer, 3);
	ptItemSave->nModClass = FogBitManipReadSigned(&tBuffer, 10);
	ptItemSave->dwInitSeed = FogBitManipReadUnsigned(&tBuffer, 32);
	ptItemSave->dwItemSeed = FogBitManipReadUnsigned(&tBuffer, 32);
	ptItemSave->bPage = (BYTE)FogBitManipReadUnsigned(&tBuffer, 8);
	return TRUE;
}
