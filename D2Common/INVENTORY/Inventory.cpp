#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\INVENTORY\\Inventory.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "../DATATBLS/DataTbls.h"

struct INVFREECHAINA
{
 	DWORD			field_00;
 	HUNIT			hItem;
 	DWORD			dwItemId;
 	DWORD			dwCategory;
 	struct INVFREECHAINA	*ptNext;
};
struct INVFREECHAINB
{
 	DWORD			dwItemId;
 	struct INVFREECHAINB	*ptNext;
};
struct INVFREECHAINC
{
 	DWORD			dwKind;
 	DWORD			dwKey;
 	DWORD			dwHire;
 	struct INVFREECHAINC	*ptNext;
};
struct LOCINFO
{
 	HUNIT		ahSlot00[11];
 	HUNIT		ahSlot[16];
 	HUNIT		ahGrid[5][10][10];
};
typedef struct LOCINFO LOCINFO, *PTLOCINFO;

struct INVGRIDINFO
{
 	BYTE		gridRows;
 	BYTE		gridCols;
 	BYTE		unobserved_2[2];
 	int			gridLeft;
 	int			gridRight;
 	int			gridTop;
 	int			gridBottom;
 	BYTE		gridBoxWidth;
 	BYTE		gridBoxHeight;
 	BYTE		unobserved_16[2];
};
typedef struct INVGRIDINFO INVGRIDINFO;
struct SAVEHEADER
{
 	BYTE		unobserved_0[38];
 	BYTE		abComponentGfx[16];
 	BYTE		abComponentColors[16];
};
typedef struct SAVEHEADER SAVEHEADER, *PTSAVEHEADER;
struct INVENTORY
{
 	void			*pPool;
 	DWORD			field_04;
 	DWORD			field_08;
 	DWORD			field_0c;
 	DWORD			dwOwnerId;
 	BYTE			unobserved_14[24 - 20];
 	DWORD			dwCursorItem;
 	PTLOCINFO		ptLocInfo;
 	struct INVFREECHAINB	*ptFirstDeltaNode;
 	struct INVFREECHAINB	*ptLastDeltaNode;
 	struct INVFREECHAINA	*ptFirstItemNode;
 	struct INVFREECHAINA	*ptLastItemNode;
 	DWORD			nEntries;
 	struct INVFREECHAINC	*ptFirstTrackedNode;
 	struct INVFREECHAINC	*ptLastTrackedNode;
 	DWORD			dwNumCorpses;
};
typedef struct INVENTORY INVENTORY, *PTINVENTORY;
static DWORD sgcadwInvCatIds[3] = { 107, 108, 109 };
static DWORD sgcdwInvCatCount = 3;

PTINVENTORY __stdcall INVENTORYCreate(void *pPool, DWORD dwUnitId, DWORD dwUnitType, DWORD dwClassId,
	DWORD dwSkipLocInfo)
{
	PTINVENTORY ptInventory;
	int i;

#line 39
	ptInventory = (PTINVENTORY)D2ALLOCPOOL(pPool, sizeof(INVENTORY));
#line 40
	D2ASSERT(ptInventory);
	for (i = 0; i < (int)(sizeof(INVENTORY) / sizeof(DWORD)); i++)
		((DWORD *)ptInventory)[i] = 0;
	ptInventory->field_0c = dwUnitId;
	ptInventory->dwOwnerId = dwUnitId;
	ptInventory->pPool = pPool;
	ptInventory->field_04 = dwUnitType;
	ptInventory->field_08 = dwClassId;
	if (dwSkipLocInfo == 0)
	{
#line 50
		ptInventory->ptLocInfo = (PTLOCINFO)D2ALLOCPOOL(pPool, sizeof(LOCINFO));
#line 51
		D2ASSERT(ptInventory->ptLocInfo);
		memset(ptInventory->ptLocInfo, 0, ((int)(sizeof(LOCINFO) / sizeof(DWORD))) * sizeof(DWORD));
	}
	else
	{
		ptInventory->ptLocInfo = NULL;
	}
	return ptInventory;
}

void __stdcall INVENTORYDestroy(PTINVENTORY ptInventory)
{
	struct INVFREECHAINA *ptA, *ptANext;
	struct INVFREECHAINB *ptB, *ptBNext;
	struct INVFREECHAINC *ptC, *ptCNext;

	if (ptInventory == NULL)
		return;
	for (ptA = ptInventory->ptFirstItemNode; ptA != NULL; ptA = ptANext)
	{
		ptANext = ptA->ptNext;
#line 76
		D2FREEPOOL(ptInventory->pPool, ptA);
	}
	for (ptB = ptInventory->ptFirstDeltaNode; ptB != NULL; ptB = ptBNext)
	{
		ptBNext = ptB->ptNext;
#line 83
		D2FREEPOOL(ptInventory->pPool, ptB);
	}
	for (ptC = ptInventory->ptFirstTrackedNode; ptC != NULL; ptC = ptCNext)
	{
		ptCNext = ptC->ptNext;
#line 90
		D2FREEPOOL(ptInventory->pPool, ptC);
	}
	if (ptInventory->ptLocInfo != NULL)
	{
#line 96
		D2FREEPOOL(ptInventory->pPool, ptInventory->ptLocInfo);
	}
#line 98
	D2FREEPOOL(ptInventory->pPool, ptInventory);
}

BOOL __stdcall INVENTORYAddItem(PTINVENTORY ptInventory, HUNIT hItem, DWORD dwCategory)
{
	struct INVFREECHAINA *ptNewInvNode;
	struct INVFREECHAINA *ptOldHead;
	struct INVFREECHAINA *ptOldTail;
	int i;

	if (hItem == NULL)
		return FALSE;
#line 136
	D2ASSERT(UnitGetType(hItem) == UNIT_ITEM);
#line 139
	D2ASSERT(ptInventory);
	ptOldTail = ptInventory->ptLastItemNode;
#line 141
	ptNewInvNode = (struct INVFREECHAINA *)D2ALLOCPOOL(ptInventory->pPool, sizeof(struct INVFREECHAINA));
#line 142
	D2ASSERT(ptNewInvNode);
	memset(ptNewInvNode, 0, sizeof(struct INVFREECHAINA));
	ptNewInvNode->hItem = hItem;
	ptNewInvNode->dwItemId = hItem->dwId;
	ptNewInvNode->dwCategory = dwCategory;

	for (i = 0;; i++)
	{
		if ((DWORD)i < sgcdwInvCatCount)
		{
			if ((int)D2CommonStatsGetTotalValue(hItem, sgcadwInvCatIds[i]) > 0)
			{
				ptOldHead = ptInventory->ptFirstItemNode;
				ptNewInvNode->ptNext = ptOldHead;
				ptInventory->ptFirstItemNode = ptNewInvNode;
				if (ptInventory->ptLastItemNode == NULL)
					ptInventory->ptLastItemNode = ptNewInvNode;
				break;
			}
		}
		else
		{
			ptNewInvNode->ptNext = NULL;
			ptOldHead = ptInventory->ptFirstItemNode;
			ptInventory->ptLastItemNode = ptNewInvNode;
			if (ptOldHead == NULL)
				ptInventory->ptFirstItemNode = ptNewInvNode;
			else
				ptOldTail->ptNext = ptNewInvNode;
			break;
		}
	}
	ptInventory->nEntries++;
	return TRUE;
}

BOOL __stdcall INVENTORYTestHasItem(PTINVENTORY ptInventory, HUNIT hItemKey)
{
	struct INVFREECHAINA *ptNode;

#line 179
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstItemNode; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->hItem == hItemKey)
			return TRUE;
	}
	return FALSE;
}

HUNIT __stdcall INVENTORYRemoveItem(PTINVENTORY ptInventory, HUNIT hItemKey)
{
	struct INVFREECHAINA *ptNode, *ptPrev, *ptNext;
	HUNIT hFoundItem;

#line 197
	D2ASSERT(ptInventory);
	ptPrev = NULL;
	for (ptNode = ptInventory->ptFirstItemNode; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->hItem == hItemKey)
		{
			hFoundItem = ptNode->hItem;
			ptNext = ptNode->ptNext;
			if (ptPrev == NULL)
			{
				ptInventory->ptFirstItemNode = ptNext;
				if (ptNext == NULL)
					ptInventory->ptLastItemNode = ptNext;
			}
			else
			{
				ptPrev->ptNext = ptNext;
				if (ptNext == NULL)
					ptInventory->ptLastItemNode = ptPrev; }
#line 220
			D2FREEPOOL(ptInventory->pPool, ptNode);
			ptInventory->nEntries--;
			return hFoundItem;
		}
		ptPrev = ptNode;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYGetItemAt(PTINVENTORY ptInventory, int nX, int nY, int *pnItemX, int *pnItemY,
	int nRecord, BYTE bPage);
static BOOL __fastcall INVENTORYTestRectFree(PTINVENTORY ptInventory, BYTE bWidth, INVGRIDINFO tGrid,
	BYTE bHeight, int *pnX, int *pnY, BYTE bPage);
static void __fastcall INVENTORYFillItemRect(PTINVENTORY ptInventory, HUNIT hItem, int nX, int nY, BYTE bPage,
	int nRecord, DWORD dwArg);

static BOOL INVENTORYFindSpaceOneByOne(PTINVENTORY ptInventory, INVGRIDINFO tGrid, int *pnX, int *pnY,
	BYTE bPage)
{
	BOOL fFits;

#line 424
	D2ASSERT(ptInventory->ptLocInfo);
	fFits = FALSE;
	for (*pnX = tGrid.gridRows - 1; *pnX >= 0; (*pnX)--)
	{
		for (*pnY = tGrid.gridCols - 1; *pnY >= 0; (*pnY)--)
		{
			if (ptInventory->ptLocInfo->ahGrid[bPage][*pnY][*pnX] == NULL)
				fFits = INVENTORYTestRectFree(ptInventory, 1, tGrid, 1, pnX, pnY, bPage);
			else
				fFits = FALSE;
			if (fFits)
				break;
		}
		if (fFits)
			return TRUE;
	}
	return FALSE;
}

static BOOL INVENTORYFindSpaceTall(PTINVENTORY ptInventory, INVGRIDINFO tGrid, BYTE bWidth, BYTE bHeight,
	int *pnX, int *pnY, BYTE bPage)
{
	BOOL fFits;

#line 465
	D2ASSERT(ptInventory->ptLocInfo);
	fFits = FALSE;
	for (*pnX = 0; *pnX < tGrid.gridRows; (*pnX)++)
	{
		for (*pnY = 0; *pnY < tGrid.gridCols - 2; (*pnY)++)
		{
			if (ptInventory->ptLocInfo->ahGrid[bPage][*pnY][*pnX] == NULL)
				fFits = INVENTORYTestRectFree(ptInventory, bWidth, tGrid, bHeight, pnX, pnY, bPage);
			else
				fFits = FALSE;
			if (fFits)
				break;
		}
		if (fFits)
			return TRUE;
	}
	return FALSE;
}

static BOOL INVENTORYFindSpaceTwoByTwo(PTINVENTORY ptInventory, INVGRIDINFO tGrid, int *pnX, int *pnY,
	BYTE bPage)
{
	BOOL fFits;

#line 506
	D2ASSERT(ptInventory->ptLocInfo);
	fFits = FALSE;
	for (*pnY = tGrid.gridCols - 2; *pnY >= 0; (*pnY)--)
	{
		for (*pnX = tGrid.gridRows - 1; *pnX >= 0; (*pnX)--)
		{
			if (ptInventory->ptLocInfo->ahGrid[bPage][*pnY][*pnX] == NULL)
				fFits = INVENTORYTestRectFree(ptInventory, 2, tGrid, 2, pnX, pnY, bPage);
			else
				fFits = FALSE;
			if (fFits)
				break;
		}
		if (fFits)
			return TRUE;
	}
	return FALSE;
}

static BOOL INVENTORYFindSpaceAny(PTINVENTORY ptInventory, INVGRIDINFO tGrid, BYTE bWidth, BYTE bHeight,
	int *pnX, int *pnY, BYTE bPage)
{
	BOOL fFits;

#line 547
	D2ASSERT(ptInventory->ptLocInfo);
	fFits = FALSE;
	for (*pnX = 0; *pnX < tGrid.gridRows; (*pnX)++)
	{
		for (*pnY = 0; *pnY < tGrid.gridCols; (*pnY)++)
		{
			if (ptInventory->ptLocInfo->ahGrid[bPage][*pnY][*pnX] == NULL)
				fFits = INVENTORYTestRectFree(ptInventory, bWidth, tGrid, bHeight, pnX, pnY, bPage);
			else
				fFits = FALSE;
			if (fFits)
				break;
		}
		if (fFits)
			return TRUE;
	}
	return FALSE;
}

BOOL __stdcall INVENTORYFindFreeSpace(PTINVENTORY ptInventory, HUNIT hItem, int nRecord, int *pnX, int *pnY,
	BYTE bPage)
{
	BYTE bWidth, bHeight;
	INVGRIDINFO tGrid;

#line 589
	D2ASSERT(ptInventory);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	D2CommonInvTblsGetGrid(nRecord, &tGrid);
	if (bWidth == 1 && bHeight == 1)
		return INVENTORYFindSpaceOneByOne(ptInventory, tGrid, pnX, pnY, bPage);
	if (bWidth == 1 && bHeight == 3 || bWidth == 2 && bHeight == 3)
		return INVENTORYFindSpaceTall(ptInventory, tGrid, bWidth, bHeight, pnX, pnY, bPage);
	if (bWidth == 2 && bHeight == 2)
		return INVENTORYFindSpaceTwoByTwo(ptInventory, tGrid, pnX, pnY, bPage);
	return INVENTORYFindSpaceAny(ptInventory, tGrid, bWidth, bHeight, pnX, pnY, bPage);
}

static BOOL __fastcall INVENTORYTestRectFree(PTINVENTORY ptInventory, BYTE bWidth, INVGRIDINFO tGrid,
	BYTE bHeight, int *pnX, int *pnY, BYTE bPage)
{
	PTLOCINFO ptLocInfo;
	int nX, nY;
	int x, y;
	BOOL fFree;

	nX = *pnX;
	if (nX == -1)
		return FALSE;
	nY = *pnY;
	if (nY == -1)
		return FALSE;
#line 380
	D2ASSERTMSG(nX >= 0 && nX < tGrid.gridRows, "Outside inventories bounds. Please tell Jon Morin!\n");
#line 381
	D2ASSERTMSG(nY >= 0 && nY < tGrid.gridCols, "Outside inventories bounds. Please tell Jon Morin!\n");
#line 383
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	fFree = TRUE;
	for (y = nY; y < nY + bHeight; y++)
	{
		if (y >= tGrid.gridCols)
		{
			fFree = FALSE;
			break;
		}
		for (x = *pnX; x < *pnX + bWidth; x++)
		{
			if (x >= tGrid.gridRows || ptLocInfo->ahGrid[bPage][y][x] != NULL)
			{
				fFree = FALSE;
				break;
			}
		}
		if (!fFree)
			break;
	}
	return fFree;
}

BOOL __stdcall INVENTORYPlaceItem(PTINVENTORY ptInventory, HUNIT hItem, int nRecord, DWORD dwClient,
	BYTE bPage)
{
	int nX, nY;
	if (hItem == NULL)
		return FALSE;
	if (!INVENTORYFindFreeSpace(ptInventory, hItem, nRecord, &nX, &nY, bPage))
		return FALSE;
	INVENTORYFillItemRect(ptInventory, hItem, nX, nY, bPage, nRecord, dwClient);
	return TRUE;
}

static void __fastcall INVENTORYFillItemRect(PTINVENTORY ptInventory, HUNIT hItem, int nX, int nY, BYTE bPage,
	int nRecord, DWORD dwArg)
{
	BYTE bWidth, bHeight;
	int x, y;

#line 314
	D2ASSERT(hItem);
#line 316
	D2ASSERT(ptInventory);
#line 317
	D2ASSERT(ptInventory->ptLocInfo);
	UnitSetX(hItem, (DWORD)nX);
	UnitSetY(hItem, (DWORD)nY);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	for (y = nY; y < nY + bHeight; y++)
	{
		for (x = nX; x < nX + bWidth; x++)
			ptInventory->ptLocInfo->ahGrid[bPage][y][x] = (HUNIT)-1;
	}
	ptInventory->ptLocInfo->ahGrid[bPage][nY][nX] = hItem;
}

BOOL __stdcall INVENTORYTestPlaceAt(PTINVENTORY ptInventory, HUNIT hItem, int nX, int nY, int nRecord,
	HUNIT *phItemFound, int *pnCount, BYTE bPage)
{
	BYTE bWidth, bHeight;
	INVGRIDINFO tGrid;
	BOOL fFree;
	HUNIT hLast;
	HUNIT hCell;
	int nItemX, nItemY;
	int x, y;

	*pnCount = 0;
#line 644
	D2ASSERTMSG(bPage != 255, "This should not happen. Please tell Jon Morin!\n");
	if (hItem == NULL || nX == -1 || nY == -1)
		return FALSE;
#line 654
	D2ASSERT(ptInventory);
#line 655
	D2ASSERT(ptInventory->ptLocInfo);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	D2CommonInvTblsGetGrid(nRecord, &tGrid);
	fFree = TRUE;
	hLast = NULL;
	for (y = nY; y < nY + bHeight; y++)
	{
		if (y >= tGrid.gridCols)
		{
			fFree = FALSE;
			break;
		}
		for (x = nX; x < nX + bWidth; x++)
		{
			if (x >= tGrid.gridRows)
			{
				fFree = FALSE;
				break;
			}
			hCell = ptInventory->ptLocInfo->ahGrid[bPage][y][x];
			if (hCell == NULL)
				continue;
			fFree = FALSE;
			if (hCell == (HUNIT)-1)
				*phItemFound = INVENTORYGetItemAt(ptInventory, x, y, &nItemX, &nItemY, nRecord, bPage);
			else
				*phItemFound = hCell;
#line 680
			D2ASSERT(*phItemFound);
			if (*phItemFound != hLast)
				(*pnCount)++;
			hLast = *phItemFound;
		}
	}
	return fFree;
}

BOOL __stdcall INVENTORYTestSwapAt(PTINVENTORY ptInventory, HUNIT hItem, int nX, int nY, int nRecord,
	HUNIT *phItemFound, BYTE bPage)
{
	BYTE bWidth, bHeight;
	INVGRIDINFO tGrid;
	DWORD dwCount;
	HUNIT hLast;
	int x, y;

	if (hItem == NULL || nX == -1 || nY == -1)
		return FALSE;
	dwCount = 0;
	hLast = NULL;
#line 712
	D2ASSERT(ptInventory);
#line 713
	D2ASSERT(ptInventory->ptLocInfo);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	D2CommonInvTblsGetGrid(nRecord, &tGrid);
	for (y = nY; y < nY + bHeight; y++)
	{
		if (y >= tGrid.gridCols)
			return FALSE;
		for (x = nX; x < nX + bWidth; x++)
		{
			if (x >= tGrid.gridRows)
				return FALSE;
			if (ptInventory->ptLocInfo->ahGrid[bPage][y][x] != NULL)
			{
				int nItemX, nItemY;

				*phItemFound = INVENTORYGetItemAt(ptInventory, x, y, &nItemX, &nItemY, nRecord, bPage);
				if (*phItemFound != NULL && *phItemFound != hLast)
				{
					hLast = *phItemFound;
					dwCount++;
				}
			}
		}
	}
	if (dwCount > 1)
	{
		*phItemFound = NULL;
		return FALSE;
	}
	return TRUE;
}

BOOL __stdcall INVENTORYPlaceItemAt(PTINVENTORY ptInventory, HUNIT hItem, int nX, int nY, int nRecord,
	DWORD dwClient, BYTE bPage)
{
	HUNIT hItemFound;
	int nCount;

	if (hItem == NULL || nX == -1 || nY == -1)
		return FALSE;
	if (!INVENTORYTestPlaceAt(ptInventory, hItem, nX, nY, nRecord, &hItemFound, &nCount, bPage))
		return FALSE;
	INVENTORYFillItemRect(ptInventory, hItem, nX, nY, bPage, nRecord, dwClient);
	return TRUE;
}

static void INVENTORYClearItemRect(PTINVENTORY ptInventory, int nX, int nY, BYTE bWidth, BYTE bHeight,
	BYTE bPage)
{
	int x, y;

#line 349
	D2ASSERT(ptInventory);
#line 350
	D2ASSERT(ptInventory->ptLocInfo);
	for (y = nY; y < nY + bHeight; y++)
	{
		for (x = nX; x < nX + bWidth; x++)
			ptInventory->ptLocInfo->ahGrid[bPage][y][x] = NULL;
	}
}

void __stdcall INVENTORYRemoveItemAt(PTINVENTORY ptInventory, int nX, int nY, int nRecord, DWORD dwClient,
	BYTE bPage)
{
	HUNIT hItem;
	int nItemX, nItemY;
	BYTE bWidth, bHeight;

	hItem = INVENTORYGetItemAt(ptInventory, nX, nY, &nItemX, &nItemY, nRecord, bPage);
#line 788
	D2ASSERT(hItem);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	INVENTORYClearItemRect(ptInventory, nItemX, nItemY, bWidth, bHeight, bPage);
}

void __stdcall INVENTORYClearGrid(PTINVENTORY ptInventory)
{
	PTLOCINFO ptLocInfo;
	HUNIT *phCell;
	int i;

#line 801
	D2ASSERT(ptInventory);
#line 802
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	phCell = &ptLocInfo->ahGrid[0][0][0];
	memset(phCell, 0, ((int)(sizeof(ptLocInfo->ahGrid) / sizeof(HUNIT))) * sizeof(phCell[0]));
}

static __inline BOOL INVENTORYGetItemSize(HUNIT hItem, BYTE *pbWidth, BYTE *pbHeight)
{
	if (hItem == NULL)
		return FALSE;
	ITEMSGetTypeInvWidthHeight(hItem, pbWidth, pbHeight);
	return TRUE;
}

HUNIT __stdcall INVENTORYGetItemAt(PTINVENTORY ptInventory, int nX, int nY, int *pnItemX, int *pnItemY,
	int nRecord, BYTE bPage)
{
	INVGRIDINFO tGrid;
	HUNIT hCell;
	BYTE bWidth, bHeight;
	int x, y;

	D2CommonInvTblsGetGrid(nRecord, &tGrid);
#line 823
	D2ASSERTMSG(nX >= 0 && nX < tGrid.gridRows, "Outside inventories bounds. Please tell Jon Morin!\n");
#line 824
	D2ASSERTMSG(nY >= 0 && nY < tGrid.gridCols, "Outside inventories bounds. Please tell Jon Morin!\n");
	if (ptInventory)
	{
#line 829
		D2ASSERT(ptInventory->ptLocInfo);
		for (y = 0; y < tGrid.gridCols; y++)
		{
			for (x = 0; x < tGrid.gridRows; x++)
			{
				if (ptInventory->ptLocInfo->ahGrid[bPage][y][x] == (HUNIT)-1
					|| ptInventory->ptLocInfo->ahGrid[bPage][y][x] == NULL)
					continue;
				hCell = ptInventory->ptLocInfo->ahGrid[bPage][y][x];
				if (!INVENTORYGetItemSize(hCell, &bWidth, &bHeight))
					continue;
				if (nX >= x && nX < x + bWidth && nY >= y && nY < y + bHeight)
				{
					*pnItemX = x;
					*pnItemY = y;
					return hCell;
				}
			}
		}
	}
	return NULL;
}

BOOL __stdcall INVENTORYSetLocSlot(PTINVENTORY ptInventory, HUNIT hValue, int nIndex)
{
	PTLOCINFO ptLocInfo;

	if (hValue == NULL)
		return FALSE;
#line 871
	D2ASSERT(ptInventory);
#line 872
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	if (ptLocInfo->ahSlot00[nIndex] != NULL)
		return FALSE;
	ptLocInfo->ahSlot00[nIndex] = hValue;
	return TRUE;
}

void __stdcall INVENTORYClearLocSlot(PTINVENTORY ptInventory, int nIndex)
{
	PTLOCINFO ptLocInfo;

#line 888
	D2ASSERT(ptInventory);
#line 889
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	ptLocInfo->ahSlot00[nIndex] = NULL;
}

DWORD __stdcall INVENTORYGetEquipField(DWORD dwPlayer, PTINVENTORY ptInventory, DWORD *pdwOut,
	DWORD dwSlotSel)
{
	DWORD dwClass;
	PTLOCINFO ptLocInfo;
	DWORD dwSlotVal;
	dwClass = (D2CommonINVENTORYTestCanEquipSlot(dwPlayer, ptInventory) == 2) ? 1 : 0;
#line 903
	D2ASSERT(ptInventory);
#line 904
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	if (*pdwOut == 0)
	{
		if (dwSlotSel == 4)
		{
			if (dwClass)
			{
				dwSlotVal = (DWORD)ptLocInfo->ahSlot00[5];
				*pdwOut = dwSlotVal;
				return dwClass;
			}
		}
		else if (dwSlotSel == 5)
		{
			if (dwClass)
			{
				dwClass = (DWORD)ptLocInfo->ahSlot00[4];
				*pdwOut = dwClass;
			}
		}
	}
	return dwClass;
}

BOOL __stdcall INVENTORYTestEquipCompat(PTINVENTORY hInventory, DWORD dwWeaponClass)
{
	PTINVENTORY ptInventory;
	PTLOCINFO ptLocInfo;
	HUNIT hSlot;

#line 924
	D2ASSERT(hInventory);
	ptInventory = hInventory;
#line 951
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	hSlot = ptLocInfo->ahSlot00[5];
	if (D2CommonITEMSTestEffectsActive(hSlot) != 0
		&& D2CommonITEMSTestTypeWClassEqual(hSlot, dwWeaponClass) != 0)
		return TRUE;
#line 951
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	hSlot = ptLocInfo->ahSlot00[4];
	if (D2CommonITEMSTestEffectsActive(hSlot) != 0
		&& D2CommonITEMSTestTypeWClassEqual(hSlot, dwWeaponClass) != 0)
		return TRUE;
	return FALSE;
}

HUNIT __stdcall INVENTORYGetLocSlot(PTINVENTORY ptInventory, int nIndex)
{
	PTLOCINFO ptLocInfo;

#line 950
	D2ASSERT(ptInventory);
#line 951
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	return ptLocInfo->ahSlot00[nIndex];
}

static PTLOCINFO INVENTORYGetLocInfoChecked(PTINVENTORY ptInventory)
{
#line 951
	D2ASSERT(ptInventory->ptLocInfo);
	return ptInventory->ptLocInfo;
}

static HUNIT INVENTORYScanAcceptedSlot(PTINVENTORY ptInventory)
{
	PTLOCINFO ptLocInfo;
	HUNIT hSlot;
	PTINVLISTNODE hFound;
	int i;

#line 962
	D2ASSERT(ptInventory->ptLocInfo);
	for (i = 0; i < (int)arrsize(ptInventory->ptLocInfo->ahSlot00); i++)
	{
		ptLocInfo = ptInventory->ptLocInfo;
		hSlot = ptLocInfo->ahSlot00[i];
		if (hSlot == NULL)
			continue;
		hFound = INVENTORYListFindNode(ptInventory, hSlot);
		if (hFound == NULL)
			continue;
		if (D2CommonINVENTORYNodeGetAccepted(hFound) == 0)
			continue;
		return hSlot;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYFindEquippedMatch(PTINVENTORY ptInventory)
{
	HUNIT hSlot;
	PTINVLISTNODE hFound;
	int i;

#line 961
	D2ASSERT(ptInventory);
	D2ASSERT(ptInventory->ptLocInfo);
	for (i = 0; i < (int)arrsize(ptInventory->ptLocInfo->ahSlot00); i++)
	{
		hSlot = ptInventory->ptLocInfo->ahSlot00[i];
		if (hSlot == NULL)
			continue;
		hFound = INVENTORYListFindNode(ptInventory, hSlot);
		if (hFound == NULL)
			continue;
		if (D2CommonINVENTORYNodeGetAccepted(hFound) == 0)
			continue;
		return hSlot;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYGetBodyLocItem(PTINVENTORY ptInventory, int nLoc)
{
	HUNIT hFound;
	HUNIT hAccepted;
	int nClass;
	int i;

	hFound = NULL;
#line 985
	D2ASSERT(ptInventory);
#line 986
	D2ASSERT(ptInventory->ptLocInfo);
	hAccepted = INVENTORYScanAcceptedSlot(ptInventory);
	for (i = 0; i < (int)arrsize(ptInventory->ptLocInfo->ahSlot00); i++)
	{
		if (ptInventory->ptLocInfo->ahSlot00[i] == NULL)
			continue;
		nClass = D2CommonITEMSGetWClassIndex(ptInventory->ptLocInfo->ahSlot00[i]);
		if (nClass == 2 || nClass == 3)
		{
			if (nLoc == 6)
			{
				if (ptInventory->ptLocInfo->ahSlot00[i] != hAccepted)
				{
					hFound = ptInventory->ptLocInfo->ahSlot00[i];
					break;
				}
			}
			else if (nLoc == 5)
			{
				if (ptInventory->ptLocInfo->ahSlot00[i] == hAccepted)
				{
					hFound = ptInventory->ptLocInfo->ahSlot00[i];
					break;
				}
			}
		}
		else if ((int)ITEMSGetTypeComponent(ptInventory->ptLocInfo->ahSlot00[i]) == nLoc)
		{
			hFound = ptInventory->ptLocInfo->ahSlot00[i];
			break;
		}
	}
	return hFound;
}

int __stdcall INVENTORYFindSlotIndex(PTINVENTORY ptInventory, HUNIT hItem)
{
	PTLOCINFO ptLocInfo;
	int i;

#line 1017
	D2ASSERT(hItem);
#line 1019
	D2ASSERT(ptInventory);
#line 1020
	D2ASSERT(ptInventory->ptLocInfo);
	ptLocInfo = ptInventory->ptLocInfo;
	for (i = 0; i < (int)arrsize(ptLocInfo->ahSlot00); i++)
	{
		if (ptLocInfo->ahSlot00[i] == hItem)
			break;
	}
	return i;
}

void __stdcall INVENTORYSetCursorItem(PTINVENTORY ptInventory, DWORD dwValue)
{
#line 1037
	D2ASSERT(ptInventory);
	ptInventory->dwCursorItem = dwValue;
}

DWORD __stdcall INVENTORYGetCursorItem(PTINVENTORY ptInventory)
{
#line 1048
	D2ASSERT(ptInventory);
	return ptInventory->dwCursorItem;
}

HUNIT __stdcall INVENTORYFindGridStack(PTINVENTORY ptInventory, DWORD dwWantedKind, HUNIT hExclude)
{
	struct INVFREECHAINA *ptNode;
	BOOL fPastExclude;
	int nLevel, nMaxLevel;

	fPastExclude = FALSE;
#line 1062
	D2ASSERT(ptInventory);
	ptNode = ptInventory->ptFirstItemNode;
	while (ptNode != NULL)
	{
		if (hExclude == NULL)
		{
			fPastExclude = TRUE;
		}
		else if (ptNode->hItem == hExclude)
		{
			ptNode = ptNode->ptNext;
			fPastExclude = TRUE;
		}
		if (ptNode == NULL)
			return NULL;
		if (ptNode->dwCategory == 1 && fPastExclude)
		{
			if (D2CommonITEMSGetType(ptNode->hItem) == dwWantedKind)
			{
				if (D2CommonStatsUnitGetValue(ptNode->hItem, 70) < (int)D2CommonITEMSGetTypeMaxStack(ptNode->hItem))
				{
					if (!D2CommonITEMSDataGetPage(ptNode->hItem))
						return ptNode->hItem;
				}
			}
		}
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYFindEquippedStack(PTINVENTORY ptInventory, DWORD dwWantedKind, HUNIT hExclude)
{
	struct INVFREECHAINA *ptNode;
	BOOL fPastExclude;
	int nLevel, nMaxLevel;

	fPastExclude = FALSE;
#line 1094
	D2ASSERT(ptInventory);
	ptNode = ptInventory->ptFirstItemNode;
	while (ptNode != NULL)
	{
		if (hExclude == NULL)
		{
			fPastExclude = TRUE;
		}
		else if (ptNode->hItem == hExclude)
		{
			ptNode = ptNode->ptNext;
			fPastExclude = TRUE;
		}
		if (ptNode == NULL)
			return NULL;
		if (ptNode->dwCategory == 3 && fPastExclude)
		{
			if (D2CommonITEMSGetType(ptNode->hItem) == dwWantedKind)
			{
				if (D2CommonStatsUnitGetValue(ptNode->hItem, 70) < (int)D2CommonITEMSGetTypeMaxStack(ptNode->hItem))
					return ptNode->hItem;
			}
		}
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYFindGridBook(PTINVENTORY ptInventory, HUNIT hItem2, HUNIT hExclude)
{
	struct INVFREECHAINA *ptNode;
	BOOL fPastExclude;
	DWORD dwGot;
	int nLevel, nMaxLevel;

	fPastExclude = FALSE;
#line 1125
	D2ASSERT(ptInventory);
	ptNode = ptInventory->ptFirstItemNode;
	while (ptNode != NULL)
	{
		if (hExclude == NULL)
		{
			fPastExclude = TRUE;
		}
		else if (ptNode->hItem == hExclude)
		{
			ptNode = ptNode->ptNext;
			fPastExclude = TRUE;
		}
		if (ptNode == NULL)
			return NULL;
		if (ptNode->dwCategory == 1 && fPastExclude)
		{
			if (D2CommonITEMSGetType(ptNode->hItem) == 18)
			{

				if (D2CommonITEMSDataGetMagicSuffixAt(ptNode->hItem, 0) == D2CommonITEMSDataGetMagicSuffixAt(hItem2, 0))
				{
					if (D2CommonStatsUnitGetValue(ptNode->hItem, 70) < (int)D2CommonITEMSGetTypeMaxStack(ptNode->hItem))
					{
						if (!D2CommonITEMSDataGetPage(ptNode->hItem))
							return ptNode->hItem;
					}
				}
			}
		}
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

int __stdcall INVENTORYCountFreeCells(HUNIT hUnit)
{
	PTINVENTORY ptInventory;
	INVGRIDINFO tGrid;
	int nFree;
	int x, y;

	ptInventory = (PTINVENTORY)hUnit->ptInventory;
#line 1156
	D2ASSERT(ptInventory);
#line 1157
	D2ASSERT(ptInventory->ptLocInfo);
	nFree = 0;
	DWORD dwRecord = UnitGetInvRecordIndex(hUnit, 0);
	D2CommonInvTblsGetGrid(dwRecord, &tGrid);
	for (x = 0; x < tGrid.gridRows; x++)
	{
		for (y = 0; y < tGrid.gridCols; y++)
		{
			if (ptInventory->ptLocInfo->ahGrid[0][y][x] == NULL)
				nFree++;
		}
	}
	return nFree;
}

static void INVENTORYSetBeltSlot(PTINVENTORY ptInventory, HUNIT hItem, int nSlot)
{
#line 1186
	D2ASSERT(ptInventory);
#line 1187
	D2ASSERT(ptInventory->ptLocInfo);
	UnitSetX(hItem, (DWORD)nSlot);
	ptInventory->ptLocInfo->ahSlot[nSlot] = hItem;
}

BOOL __stdcall INVENTORYPlaceBeltItem(PTINVENTORY ptInventory, HUNIT hItem, int nSlot)
{
	BYTE bWidth, bHeight;

	if (hItem == NULL || nSlot == -1)
		return FALSE;
#line 1205
	D2ASSERT(ptInventory);
#line 1206
	D2ASSERT(ptInventory->ptLocInfo);
	if (!ITEMSGetTypeBelt(hItem))
		return FALSE;
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	if (ptInventory->ptLocInfo->ahSlot[nSlot] != NULL)
		return FALSE;
	if (bWidth != 1 || bHeight != 1)
		return FALSE;
	if (!ITEMSGetTypeBelt(hItem))
		return FALSE;
	INVENTORYSetBeltSlot(ptInventory, hItem, nSlot);
	return TRUE;
}

void __stdcall INVENTORYClearBeltSlot(PTINVENTORY ptInventory, int nSlot)
{
#line 1228
	D2ASSERT(ptInventory);
#line 1229
	D2ASSERT(ptInventory->ptLocInfo);
	ptInventory->ptLocInfo->ahSlot[nSlot] = NULL;
}

DWORD __stdcall INVENTORYTestItemFits(PTINVENTORY ptInventory, HUNIT hItem)
{
	PTITEMTYPEDATA ptStats;
	int i;

	if (ptInventory == NULL)
		return FALSE;
	ptStats = (PTITEMTYPEDATA)D2CommonItemTblsGetItemStats(hItem->dwClassId);
	if (ptStats->code == 543388521 || ptStats->code == 543388532)
		return FALSE;
#line 1263
	D2ASSERT(ptInventory->ptLocInfo);
	for (i = 0; i < 4; i++)
	{
		if (ptInventory->ptLocInfo->ahSlot[i] != NULL
			&& ITEMSTestTypeGroupMatch(hItem, ptInventory->ptLocInfo->ahSlot[i]))
			return TRUE;
	}
	return FALSE;
}

BOOL __stdcall INVENTORYFindBeltSlot(PTINVENTORY ptInventory, HUNIT hItem, int *pnSlot)
{
	BYTE bWidth, bHeight;
	HUNIT hBelt;
	int nBelt;
	BELTSTXT tBelt;
	PTITEMSTXT ptItemStats;
	int i, j;

	if (hItem == NULL ? TRUE : *pnSlot == -1)
		return FALSE;
#line 1295
	D2ASSERT(ptInventory);
#line 1296
	D2ASSERT(ptInventory->ptLocInfo);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	if (bWidth != 1 || bHeight != 1)
		return FALSE;
	hBelt = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[8];
	nBelt = 2;
	if (hBelt != NULL)
		nBelt = (int)UnitGetItemCodeValue(hBelt);
	D2CommonBeltTblsGetBelt(nBelt, &tBelt);
	for (i = 0; i < 4; i++)
	{
		if (ptInventory->ptLocInfo->ahSlot[i] == NULL)
			continue;
		if (!ITEMSTestTypeGroupMatch(hItem, ptInventory->ptLocInfo->ahSlot[i]))
			continue;
		for (j = i; j < tBelt.numboxes; j += 4)
		{
			if (ptInventory->ptLocInfo->ahSlot[j] == NULL)
			{
				*pnSlot = j;
				return TRUE;
			}
		}
	}
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
	if (ptItemStats == NULL || ptItemStats->autobelt == 0)
		return FALSE;
	for (i = 0; i < 4; i++)
	{
		if (ptInventory->ptLocInfo->ahSlot[i] == NULL)
		{
			*pnSlot = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL __stdcall INVENTORYAddBeltItem(PTINVENTORY ptInventory, HUNIT hItem)
{
	int nSlot;

	if (hItem == NULL)
		return FALSE;
	if (!ITEMSGetTypeBelt(hItem))
		return FALSE;
	nSlot = 0;
	if (!INVENTORYFindBeltSlot(ptInventory, hItem, &nSlot))
		return FALSE;
	INVENTORYSetBeltSlot(ptInventory, hItem, nSlot);
	return TRUE;
}

HUNIT __stdcall INVENTORYGetBeltSlot(PTINVENTORY ptInventory, int nSlot)
{
	HUNIT hSlot;

	if (!(ptInventory == NULL)) {
#line 1388
		D2ASSERT(ptInventory->ptLocInfo);
		hSlot = ptInventory->ptLocInfo->ahSlot[nSlot];
		return (hSlot != (HUNIT)-1) ? hSlot : NULL; }
	return NULL;
}

BOOL __stdcall INVENTORYGetBeltSlotFor(PTINVENTORY ptInventory, HUNIT hItem, int nSlot, HUNIT *phItem)
{
	PTITEMSTXT ptItemStats;
	BYTE bWidth, bHeight;

	if (hItem == NULL || nSlot == -1)
		return FALSE;
#line 1412
	D2ASSERT(ptInventory);
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
	if (ptItemStats->useable != 1 || ptItemStats->belt != 1)
		return FALSE;
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	if (bWidth != 1 || bHeight != 1)
		return FALSE;
	*phItem = INVENTORYGetBeltSlot(ptInventory, nSlot);
	return TRUE;
}

BOOL __stdcall INVENTORYFindShieldInHands(PTINVENTORY ptInventory, HUNIT *phItem)
{
	HUNIT hSlot;

#line 1435
	D2ASSERT(ptInventory);
	if (phItem != NULL)
		*phItem = NULL;
	hSlot = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[4];
	if (ITEMSTestEffectsActive(hSlot) && ITEMSGetType(hSlot) == 2)
	{
		if (phItem != NULL)
			*phItem = hSlot;
		return TRUE;
	}
	hSlot = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[5];
	if (ITEMSTestEffectsActive(hSlot) && ITEMSGetType(hSlot) == 2)
	{
		if (phItem != NULL)
			*phItem = hSlot;
		return TRUE;
	}
	return FALSE;
}

BOOL __stdcall INVENTORYGetWeapon(PTINVENTORY ptInventory, HUNIT *phItem, int *pnLoc, BOOL *pfPrimaryWeapon)
{
	HUNIT hAccepted;
	HUNIT hSlot;

#line 1469
	D2ASSERT(ptInventory);
	*pfPrimaryWeapon = FALSE;
	hAccepted = INVENTORYScanAcceptedSlot(ptInventory);
	hSlot = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[4];
	if (ITEMSTestEffectsActive(hSlot))
	{
		ITEMSGetType(hSlot);
		if (ITEMSIsWeapon(hSlot))
		{
			*phItem = hSlot;
			*pnLoc = 4;
			if (hSlot == hAccepted)
				*pfPrimaryWeapon = TRUE;
			return TRUE;
		}
	}
	hSlot = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[5];
	if (ITEMSTestEffectsActive(hSlot))
	{
		ITEMSGetType(hSlot);
		if (ITEMSIsWeapon(hSlot))
		{
			*phItem = hSlot;
			*pnLoc = 5;
			if (hSlot == hAccepted)
				*pfPrimaryWeapon = TRUE;
			return TRUE;
		}
	}
	*phItem = NULL;
	return FALSE;
}

BOOL __stdcall INVENTORYTestTorsoArmorWorn(PTINVENTORY ptInventory)
{
	HUNIT hSlot;

#line 1517
	D2ASSERT(ptInventory);
	hSlot = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[3];
	if (hSlot != NULL && ITEMSGetType(hSlot) == 3)
		return TRUE;
	return FALSE;
}

BOOL __stdcall INVENTORYTestSlotFits(PTINVENTORY ptInventory, DWORD dwItem, int nSlotIndex, DWORD dwRecord)
{
	HUNIT hSlotItem;
	DWORD dwScratch1, dwScratch2;

#line 950
	D2ASSERT(ptInventory);
#line 951
	D2ASSERT(ptInventory->ptLocInfo);
	hSlotItem = ptInventory->ptLocInfo->ahSlot00[nSlotIndex];
	if (!(hSlotItem == NULL)) {
		if (dwItem == 0)
			return FALSE;
		return (BOOL)(D2CommonINVENTORYFindFreeSpace(ptInventory, hSlotItem, dwRecord, &dwScratch2,
			&dwScratch1, 0) != 0); }
	return FALSE;
}

void * __stdcall INVENTORYGetCategoryListHead(PTINVENTORY ptInventory)
{
#line 1555
	D2ASSERT(ptInventory);
	return ptInventory->ptFirstItemNode;
}

void * __stdcall INVENTORYGetCategoryListHeadAlt(PTINVENTORY ptInventory)
{
#line 1565
	D2ASSERT(ptInventory);
	return ptInventory->ptFirstItemNode;
}

void __stdcall INVENTORYClearCategoryList(PTINVENTORY ptInventory)
{
	struct INVFREECHAINA *ptNode, *ptNext;

#line 1575
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstItemNode; ptNode != NULL; ptNode = ptNext)
	{
		ptNext = ptNode->ptNext;
#line 1579
		D2FREEPOOL(ptInventory->pPool, ptNode);
	}
	ptInventory->ptFirstItemNode = NULL;
	ptInventory->ptLastItemNode = NULL;
}

void * __stdcall INVENTORYGetDeltaListHead(PTINVENTORY ptInventory)
{
#line 1592
	D2ASSERT(ptInventory);
	return ptInventory->ptFirstDeltaNode;
}

void __stdcall INVENTORYClearDeltaList(PTINVENTORY ptInventory)
{
	struct INVFREECHAINB *ptNode, *ptNext;

#line 1602
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstDeltaNode; ptNode != NULL; ptNode = ptNext)
	{
		ptNext = ptNode->ptNext;
#line 1606
		D2FREEPOOL(ptInventory->pPool, ptNode);
	}
	ptInventory->ptFirstDeltaNode = NULL;
	ptInventory->ptLastDeltaNode = NULL;
}

BOOL __stdcall INVENTORYTestHasDeltaKey(PTINVENTORY ptInventory, DWORD dwItemId)
{
	struct INVFREECHAINB *ptNode;

#line 1620
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstDeltaNode; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->dwItemId == dwItemId)
			return TRUE;
	}
	return FALSE;
}

void __stdcall INVENTORYAddDeltaNode(PTINVENTORY ptInventory, HUNIT hItem)
{
	struct INVFREECHAINB *ptNode;
	struct INVFREECHAINB *ptInvDeltaNode;
	struct INVFREECHAINB *ptOldHead, *ptOldTail;
	DWORD dwItemId;

	if (hItem == NULL)
		return;
	dwItemId = hItem->dwId;
#line 1620
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstDeltaNode; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->dwItemId == dwItemId)
			return;
	}
	ptOldTail = ptInventory->ptLastDeltaNode;
#line 1654
	ptInvDeltaNode = (struct INVFREECHAINB *)D2ALLOCPOOL(ptInventory->pPool, sizeof(struct INVFREECHAINB));
#line 1655
	D2ASSERT(ptInvDeltaNode);
	memset(ptInvDeltaNode, 0, sizeof(struct INVFREECHAINB));
	ptInvDeltaNode->dwItemId = hItem->dwId;
	ptInvDeltaNode->ptNext = NULL;
	ptOldHead = ptInventory->ptFirstDeltaNode;
	ptInventory->ptLastDeltaNode = ptInvDeltaNode;
	if (ptOldHead == NULL)
		ptInventory->ptFirstDeltaNode = ptInvDeltaNode;
	else
		ptOldTail->ptNext = ptInvDeltaNode;
}
#line 1666

DWORD __stdcall INVENTORYGetCategoryListCount(PTINVENTORY ptInventory)
{
#line 1673
	D2ASSERT(ptInventory);
	return ptInventory->nEntries;
}

HUNIT __stdcall INVENTORYFindGridItemByType(PTINVENTORY ptInventory, DWORD dwItemType, HUNIT hExclude)
{
	struct INVFREECHAINA *ptNode;
	BOOL fPastExclude;

	fPastExclude = FALSE;
#line 1687
	D2ASSERT(ptInventory);
	ptNode = ptInventory->ptFirstItemNode;
	while (ptNode != NULL)
	{
		if (hExclude == NULL)
		{
			fPastExclude = TRUE;
		}
		else if (ptNode->hItem == hExclude)
		{
			ptNode = ptNode->ptNext;
			fPastExclude = TRUE;
		}
		if (ptNode == NULL)
			return NULL;
		if (ptNode->dwCategory == 1 && fPastExclude)
		{
			if (ITEMSGetType(ptNode->hItem) == dwItemType && ITEMSDataGetPage(ptNode->hItem) == 0)
				return ptNode->hItem;
		}
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYFindEquippedByType(PTINVENTORY ptInventory, DWORD dwItemType, HUNIT hExclude)
{
	struct INVFREECHAINA *ptNode;
	BOOL fPastExclude;

	fPastExclude = FALSE;
#line 1719
	D2ASSERT(ptInventory);
	ptNode = ptInventory->ptFirstItemNode;
	while (ptNode != NULL)
	{
		if (hExclude == NULL)
		{
			fPastExclude = TRUE;
		}
		else if (ptNode->hItem == hExclude)
		{
			ptNode = ptNode->ptNext;
			fPastExclude = TRUE;
		}
		if (ptNode == NULL)
			return NULL;
		if (ptNode->dwCategory == 3 && fPastExclude)
		{
			if (ITEMSGetType(ptNode->hItem) == dwItemType)
				return ptNode->hItem;
		}
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYFindEquippedByCode(PTINVENTORY ptInventory, DWORD dwCode, HUNIT hExclude)
{
	struct INVFREECHAINA *ptNode;
	BOOL fPastExclude;
	DWORD ptStats;
	DWORD dwOutVal;

	fPastExclude = FALSE;
#line 1751
	D2ASSERT(ptInventory);
	ptNode = ptInventory->ptFirstItemNode;
	while (ptNode != NULL)
	{
		if (hExclude == NULL)
		{
			fPastExclude = TRUE;
		}
		else if (ptNode->hItem == hExclude)
		{
			ptNode = ptNode->ptNext;
			fPastExclude = TRUE;
		}
		if (ptNode == NULL)
			return NULL;
		if (ptNode->dwCategory == 3 && fPastExclude)
		{
			ptStats = D2CommonItemTblsFindItemByCode(dwCode, &dwOutVal);
#line 1765
			D2ASSERT(ptStats);
			if (ptNode->hItem->dwClassId == dwOutVal)
				return ptNode->hItem;
		}
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

DWORD __stdcall INVENTORYCountEquippedSetItems(PTINVENTORY ptInventory, DWORD dwSetRow)
{
	struct INVFREECHAINA *ptNode;
	DWORD dwCount;

	dwCount = 0;
#line 1785
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstItemNode; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->dwCategory == 3 && ITEMSDataGetModType(ptNode->hItem) == 5
			&& ITEMSDataGetUniqueOrSetRow(ptNode->hItem) == dwSetRow)
			dwCount++;
	}
	return dwCount;
}

void __stdcall INVENTORYOfferAddDelta(PTINVENTORY hInventory, HUNIT hAddItem)
{
	PTINVLISTNODE hInvList;
	PTINVLISTNODE hFoundSlotNode;
	HUNIT hSlotItem;

#line 1807
	D2ASSERT(hInventory);
#line 1808
	D2ASSERT(hAddItem);
	if (ITEMSIsWeapon(hAddItem) == 0)
		return;
	hInvList = INVENTORYListFindNode(hInventory, hAddItem);
#line 1815
	D2ASSERT(hInvList);
	if (D2CommonITEMSTestEffectsActive(hAddItem) != 0)
	{
		hSlotItem = INVENTORYScanAcceptedSlot(hInventory);
		if (hSlotItem == NULL)
		{
			D2CommonINVENTORYNodeSetAcceptedIfWeapon(hInvList, 1);
			return;
		}
		if (ITEMSGetType(hSlotItem) == 38)
		{
			D2CommonINVENTORYNodeSetAcceptedIfWeapon(hInvList, 1);
			hFoundSlotNode = INVENTORYListFindNode(hInventory, hSlotItem);
			D2CommonINVENTORYNodeSetAcceptedIfWeapon(hFoundSlotNode, 0);
			return;
		}
	}
	D2CommonINVENTORYNodeSetAcceptedIfWeapon(hInvList, 0);
}

void __stdcall INVENTORYOfferRemoveDelta(PTINVENTORY hInventory, HUNIT hRemoveItem)
{
	PTINVLISTNODE hInvList;
	PTLOCINFO ptLocInfo;
	HUNIT hSlotItem;
	HUNIT hBrokeOnItem;
	int nCategory;

#line 1855
	D2ASSERT(hInventory);
#line 1856
	D2ASSERT(hRemoveItem);
	if (!(ITEMSIsWeapon(hRemoveItem) == 0))
	{
		hInvList = INVENTORYListFindNode(hInventory, hRemoveItem);
#line 1863
		D2ASSERT(hInvList);
		hBrokeOnItem = INVENTORYScanAcceptedSlot(hInventory);
		if (hRemoveItem == hBrokeOnItem)
			D2CommonINVENTORYNodeSetAcceptedIfWeapon(hInvList, 0);
		hSlotItem = NULL;
		nCategory = ITEMSGetCategory(hRemoveItem);
		if (nCategory == 4)
		{
			ptLocInfo = INVENTORYGetLocInfoChecked(hInventory);
			hSlotItem = ptLocInfo->ahSlot00[5];
		}
		else if (nCategory == 5)
		{
			ptLocInfo = INVENTORYGetLocInfoChecked(hInventory);
			hSlotItem = ptLocInfo->ahSlot00[4]; }
		if (D2CommonITEMSTestEffectsActive(hSlotItem) == 0)
			return;
		hInvList = INVENTORYListFindNode(hInventory, hSlotItem);
#line 1881
		D2ASSERT(hInvList);
		D2CommonINVENTORYNodeSetAcceptedIfWeapon(hInvList, 1); }
}

int __stdcall INVENTORYTestCanEquipSlot(HUNIT hPlayer, PTINVENTORY ptInventory)
{
	HUNIT hSlot5, hSlot4;

#line 950
	D2ASSERT(ptInventory);
#line 951
	D2ASSERT(ptInventory->ptLocInfo);
	hSlot5 = ptInventory->ptLocInfo->ahSlot00[5];
	hSlot4 = ptInventory->ptLocInfo->ahSlot00[4];
	if (hSlot5 != NULL)
	{
		if (hSlot4 != NULL)
		{
			if (ITEMSTestOneOrTwoHanded(hPlayer, hSlot5) || ITEMSTestOneOrTwoHanded(hPlayer, hSlot4))
				return 1;
		}
		else
		{
			if (ITEMSGetTypeTwoHanded(hSlot5) != 0)
				return 2;
			if (ITEMSTestOneOrTwoHanded(hPlayer, hSlot5))
				return 2;
		}
	}
	else if (hSlot4 != NULL)
	{
		if (ITEMSGetTypeTwoHanded(hSlot4) != 0)
			return 2;
		if (ITEMSTestOneOrTwoHanded(hPlayer, hSlot4))
			return 2;
	}
	return -1;
}

void __stdcall INVENTORYSetOwnerId(PTINVENTORY ptInventory, DWORD dwValue)
{
#line 1938
	D2ASSERT(ptInventory);
	ptInventory->dwOwnerId = dwValue;
}

DWORD __stdcall INVENTORYGetOwnerId(PTINVENTORY ptInventory)
{
#line 1948
	D2ASSERT(ptInventory);
	return ptInventory->dwOwnerId;
}

void __stdcall INVENTORYAppendTrackedNode(PTINVENTORY ptInventory, DWORD dwKey, DWORD dwHire, DWORD dwKind)
{
	struct INVFREECHAINC *ptNewNode;
	struct INVFREECHAINC *ptOldHead, *ptOldTail;

#line 1958
	D2ASSERT(ptInventory);
	ptOldTail = ptInventory->ptLastTrackedNode;
#line 1960
	ptNewNode = (struct INVFREECHAINC *)D2ALLOCPOOL(ptInventory->pPool, sizeof(struct INVFREECHAINC));
#line 1961
	D2ASSERT(ptNewNode);
	memset(ptNewNode, 0, sizeof(struct INVFREECHAINC));
	ptNewNode->dwKind = dwKind;
	ptNewNode->dwKey = dwKey;
	ptNewNode->dwHire = dwHire;
	ptNewNode->ptNext = NULL;
	ptInventory->ptLastTrackedNode = ptNewNode;
	if (ptInventory->ptFirstTrackedNode == NULL)
		ptInventory->ptFirstTrackedNode = ptNewNode;
	else
		ptOldTail->ptNext = ptNewNode;
	if (ptNewNode->dwKind != 0)
		ptInventory->dwNumCorpses++;
}

BOOL __stdcall INVENTORYRemoveTrackedNode(PTINVENTORY ptInventory, DWORD dwKey, DWORD dwKind)
{
	struct INVFREECHAINC *ptNode, *ptPrev, *ptNext;

#line 1988
	D2ASSERT(ptInventory);
	ptPrev = NULL;
	for (ptNode = ptInventory->ptFirstTrackedNode; ptNode != NULL; ptNode = ptNode->ptNext)
	{
		if (ptNode->dwKey == dwKey && ptNode->dwKind == dwKind)
		{
			ptNext = ptNode->ptNext;
			if (ptPrev == NULL)
			{
				ptInventory->ptFirstTrackedNode = ptNext;
				if (ptNext == NULL)
					ptInventory->ptLastTrackedNode = ptNext;
			}
			else
			{
				ptPrev->ptNext = ptNext;
				if (ptNext == NULL)
					ptInventory->ptLastTrackedNode = ptPrev; }
			if (ptNode->dwKind != 0)
				ptInventory->dwNumCorpses--;
#line 2015
			D2FREEPOOL(ptInventory->pPool, ptNode);
			return TRUE;
		}
		ptPrev = ptNode;
	}
	return FALSE;
}

void * __stdcall INVENTORYGetTrackedListHead(PTINVENTORY ptInventory)
{
#line 2031
	D2ASSERT(ptInventory);
	return ptInventory->ptFirstTrackedNode;
}

void __stdcall INVENTORYFreeTrackedList(PTINVENTORY ptInventory)
{
	struct INVFREECHAINC *ptNode;
	struct INVFREECHAINC *ptNext;

#line 2041
	D2ASSERT(ptInventory);
	for (ptNode = ptInventory->ptFirstTrackedNode; ptNode != NULL; ptNode = ptNext)
	{
		ptNext = ptNode->ptNext;
#line 2045
		D2FREEPOOL(ptInventory->pPool, ptNode);
	}
	ptInventory->ptFirstTrackedNode = NULL;
	ptInventory->ptLastTrackedNode = NULL;
}

DWORD __stdcall INVENTORYGetTrackedListCount(PTINVENTORY ptInventory)
{
#line 2058
	D2ASSERT(ptInventory);
	return ptInventory->dwNumCorpses;
}

static DWORD sgadwItemGfxCodes[283] =
{
	0, 544500076, 543450477, 544831080, 544235875, 544238451, 544042088, 543975526,
	544041063, 544109155, 543912813, 543518819, 543911011, 543321970, 543782257, 543253868,
	543255656, 544568435, 543649394, 543974259, 544106595, 544436834, 543977587, 544500848,
	543452262, 543716455, 543978854, 543389026, 543650412, 544500075, 544698228, 543975276,
	543975277, 543975272, 544498284, 544498285, 544498280, 543973996, 543973997, 543973992,
	544041058, 543716194, 543912051, 543452791, 544110457, 544110434, 543321187, 544760168,
	543454067, 544039795, 543383917, 543520865, 543386726, 543454060, 543255654, 544040040,
	544041079, 544436835, 544435302, 543977575, 544436327, 543977583, 544436335, 544368484,
	540226404, 540226407, 544368996, 544629098, 544631911, 543975792, 544042083, 543454055,
	544370803, 543781492, 543910256, 544370803, 544694899, 544694892, 543318643, 543318636,
	544502643, 544502636, 544502627, 544502626, 544760172, 544826227, 544762215, 544765026,
	543973736, 544563565, 544760176, 543973736, 543324264, 543324268, 544760180, 543387763,
	544367219, 544502637, 544236403, 544043111, 544043119, 543583092, 543452258, 543454050,
	544699234, 544434226, 544109154, 544694888, 544694883, 543324019, 543324012, 544502647,
	544760162, 544366946, 544760167, 543324274, 543324269, 544498298, 544366945, 544240748,
	543976819, 544437351, 544633185, 544633187, 543974006, 543974004, 543975286, 543975284,
	544498294, 544498292, 543321190, 543780973, 543973730, 543647084, 544500849, 544760114,
	544760183, 544110439, 543255911, 543454071, 544434535, 543452258, 543583074, 544240499,
	544240500, 543518835, 544567158, 543388535, 543781483, 544501875, 543388519, 544240503,
	544502632, 543585133, 543712872, 543254585, 544760121, 543240761, 544238905, 543258425,
	543255609, 543253049, 544498233, 543254329, 543778617, 544110393, 544700729, 544694841,
	544696121, 543974201, 543388473, 544436537, 544438073, 544240441, 543255865, 544501049,
	543974969, 543717177, 540634425, 544040761, 544437049, 544043833, 543322937, 543385145,
	544367417, 544432697, 544435257, 543455033, 543699513, 544039737, 544433977, 540631609,
	543319609, 543450937, 543646777, 543777849, 544369465, 543973945, 543913017, 543257657,
	543908409, 540566073, 543255097, 543780921, 540635961, 543975225, 544437305, 544371513,
	544371769, 544367161, 544502585, 540635193, 540500537, 544175673, 540570425, 543256633,
	540633145, 543389497, 544437048, 544435256, 544432952, 544432696, 544438072, 543322936,
	543320120, 543321144, 543318840, 540570424, 540568632, 544699192, 544697400, 544762936,
	544763192, 544761912, 544764472, 544235896, 544238456, 544042104, 543975544, 544041080,
	544109176, 543912824, 543782264, 543253880, 543255672, 544568440, 543649400, 543974264,
	544106616, 544436856, 543977592, 544500856, 543452280, 543716472, 543978872, 544366968,
	544240760, 543389048, 543976824, 543650424, 544500088, 544698232, 544437368, 543648888,
	543651448, 543649144, 543650936, 543647864, 543321208, 543323768, 543321464, 543323256,
	543320184, 543321210, 543323770, 543321466, 543323258, 543320186, 540633208, 543716216,
	543912056, 540108401, 540173937
};
static DWORD sgdwItemGfxCodeCount = 283;

static void __fastcall INVENTORYWriteSaveHeaderWeapon(HUNIT hUnit, PTSAVEHEADER ptSaveHeader, HUNIT hItem);
BYTE * __stdcall ITEMSGetColorTransform(HUNIT hUnit, HUNIT hItem, BYTE *pbTransform, BOOL fInventory);

static void INVENTORYGetItemArmTypes(HUNIT hItem, BYTE *pbArmTypes)
{
	PTITEMSTXT ptStats;
	BYTE *pbSrc;
	int i;

	if (hItem != NULL)
	{
		ptStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2403
		D2ASSERT(ptStats);
		pbSrc = &ptStats->rArm;
		for (i = 0; i < 6; i++)
			pbArmTypes[i] = pbSrc[i];
	}
	else
	{
		for (i = 0; i < 6; i++)
			pbArmTypes[i] = 0;
	}
}

static void INVENTORYWriteSaveHeaderBodyItem(HUNIT hUnit, PTSAVEHEADER ptSaveHeader, HUNIT hItem)
{
	PTINVENTORY ptInventory;
	HUNIT hLoc5, hLoc6;
	PTITEMSTXT ptItemStats;
	BYTE bSlot;
	DWORD dwCode;
	DWORD dwIndex;
	DWORD i;
	BYTE *pbByte;

	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	hLoc5 = INVENTORYGetBodyLocItem(ptInventory, 5);
	hLoc6 = INVENTORYGetBodyLocItem(ptInventory, 6);
	ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2511
	D2ASSERT(ptItemStats);
	bSlot = ITEMSGetTypeComponent(hItem);
	dwCode = (ptItemStats->alternategfx != 0) ? ptItemStats->alternategfx : ptItemStats->code;
	dwIndex = 0;
	for (i = 1; i < sgdwItemGfxCodeCount; i++)
	{
		if (sgadwItemGfxCodes[i] == dwCode)
		{
			dwIndex = i;
			break;
		}
	}
	if (!dwIndex)
	{
#line 2519
		D2ASSERT(0);
	}
	if (hItem == hLoc5)
		bSlot = 5;
	else if (hItem == hLoc6)
		bSlot = 6;
	else if (bSlot >= 16)
		return;
	ptSaveHeader->abComponentGfx[bSlot] = (BYTE)dwIndex;
	INVENTORYWriteSaveHeaderWeapon(hUnit, ptSaveHeader, hItem);
	pbByte = &ptSaveHeader->abComponentColors[bSlot];
	if (ITEMSGetColorTransform(hUnit, hItem, pbByte, 0) == NULL)
		*pbByte = 255;
	else
		(*pbByte)++;
}

void __stdcall INVENTORYWriteSaveHeaderItems(HUNIT hUnit, PTSAVEHEADER ptSaveHeader)
{
	PTINVLISTNODE ptNode;
	HUNIT hItem;
	PTITEMSTXT ptItemStats;
	BYTE bCategory;
	BYTE abArmTypes[6];
	DWORD i;
	PTTOKENTXT ptArmType;
	DWORD dwIndex;
	BYTE *pbByte;

	for (ptNode = (PTINVLISTNODE)INVENTORYGetCategoryListHead((PTINVENTORY)hUnit->ptInventory);
		ptNode != NULL; ptNode = INVENTORYNodeGetNext(ptNode))
	{
		hItem = INVENTORYNodeGetKeyA(ptNode);
		if (hItem->dwMode != 1)
			continue;
		ptItemStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hItem->dwClassId);
#line 2570
		D2ASSERT(ptItemStats);
		bCategory = ITEMSGetCategory(hItem);
		if (bCategory == 1 || bCategory == 4 || bCategory == 5)
		{
			INVENTORYWriteSaveHeaderBodyItem(hUnit, ptSaveHeader, hItem);
			continue;
		}
		if (bCategory != 3)
			continue;
		INVENTORYGetItemArmTypes(hItem, abArmTypes);
		for (i = 0; i < 16; i++)
		{
			if (!COMPOSITTestSlotMember((int)i))
				continue;
			ptArmType = (PTTOKENTXT)D2CommonTokTblsGetArmor(D2CommonCOMPOSITGetComponentByte(i, abArmTypes));
			for (dwIndex = 1; ; dwIndex++)
			{
				if (dwIndex >= sgdwItemGfxCodeCount)
				{
#line 2609
					D2ASSERT(0);
					break;
				}
				if (sgadwItemGfxCodes[dwIndex] == ptArmType->token)
					break;
			}
			ptSaveHeader->abComponentGfx[i] = (BYTE)dwIndex;
			pbByte = &ptSaveHeader->abComponentColors[i];
			if (ITEMSGetColorTransform(hUnit, hItem, pbByte, 0) == NULL)
				*pbByte = 255;
			else
				(*pbByte)++;
		}
	}
}

static void __fastcall INVENTORYWriteSaveHeaderWeapon(HUNIT hUnit, PTSAVEHEADER ptSaveHeader, HUNIT hItem)
{
	PTINVENTORY ptInventory;
	DWORD dwOut;
	DWORD dwCode;
	BYTE bCategory;
	BYTE bSlot;
	HUNIT hLoc5;
	PTITEMSTXT ptStats;
	DWORD dwLoc5Code;
	DWORD dwIndex;
	DWORD dwGfx;

#line 2438
	D2ASSERT(hUnit);
#line 2439
	D2ASSERT(hItem);
#line 2440
	D2ASSERT(ptSaveHeader);
	bCategory = ITEMSGetCategory(hItem);
	if (bCategory != 4 && bCategory != 5)
		return;
	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	dwCode = COMPOSITGetUnitWeaponClass(hUnit, (DWORD)ptInventory, (DWORD)&dwOut, (DWORD)-1, 1);
	bSlot = ITEMSGetTypeComponent(hItem);
	if (dwCode == 544694904)
	{
		if (bSlot != 5 && bSlot != 6)
			return;
		hLoc5 = INVENTORYGetBodyLocItem(ptInventory, 5);
		if (hLoc5 != NULL)
		{
			ptStats = (PTITEMSTXT)D2CommonItemTblsGetItemStats(hLoc5->dwClassId);
#line 2462
			D2ASSERT(ptStats);
			dwLoc5Code = (ptStats->alternategfx != 0) ? ptStats->alternategfx : ptStats->code;
			dwGfx = 0;
			for (dwIndex = 1; dwIndex < sgdwItemGfxCodeCount; dwIndex++)
			{
				if (sgadwItemGfxCodes[dwIndex] == dwLoc5Code)
				{
					dwGfx = dwIndex;
					break;
				}
			}
			if (dwGfx == 0)
			{
#line 2467
				D2ASSERT(0);
			}
			ptSaveHeader->abComponentGfx[6] = (BYTE)dwGfx;
		}
		else
		{
#line 2479
			D2ASSERT(0);
		}
	}
	else if (dwCode == 544698210 && bSlot == 6)
	{
		dwGfx = 0;
		for (dwIndex = 1; dwIndex < sgdwItemGfxCodeCount; dwIndex++)
		{
			if (sgadwItemGfxCodes[dwIndex] == 544500076)
			{
				dwGfx = dwIndex;
				break;
			}
		}
		if (dwGfx == 0)
		{
#line 2491
			D2ASSERT(0);
		}
		ptSaveHeader->abComponentGfx[5] = (BYTE)dwGfx;
	}
}

void __stdcall INVENTORYNothing(DWORD dwUnused1, DWORD dwUnused2)
{
}

typedef int (__fastcall *PFNBODYLOCTEST)(HUNIT hUnit, int nLoc, HUNIT hItem, BOOL fNoFitTest);

static BOOL __fastcall INVENTORYTestHandConflict(HUNIT hUnit, HUNIT hSlot4, HUNIT hSlot5, HUNIT hItem,
	HUNIT hReplaced);

static int __fastcall INVENTORYTestBodyLocNone(HUNIT hUnit, int nLoc, HUNIT hItem, BOOL fNoFitTest)
{
	return 0;
}

static int __fastcall INVENTORYTestBodyLoc(HUNIT hUnit, int nLoc, HUNIT hItem, BOOL fNoFitTest)
{
	PTINVENTORY ptInventory;
	HUNIT hCur;
	int nResult;

	nResult = 0;
	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	hCur = INVENTORYGetLocSlot(ptInventory, nLoc);
	if (hItem != NULL && !ITEMSTestTypeBodyLocEqual(hItem, (DWORD)nLoc))
		return 0;
	if (hCur == NULL)
	{
		if (hItem != NULL)
		{
			if (fNoFitTest || ITEMSTestFitsInventorySpace(hItem, hUnit, NULL, NULL, NULL))
				nResult = 1;
		}
	}
	else if (hItem == NULL)
		nResult = 3;
	else if (ITEMSTestFitsInventorySpace(hItem, hUnit, NULL, NULL, NULL))
		nResult = 5;
	return nResult;
}

static __inline BOOL INVENTORYTestOtherHand(HUNIT hOther, DWORD dwType)
{
	if (hOther == NULL)
		return TRUE;
	return ITEMSGetType(hOther) == dwType;
}

static int __fastcall INVENTORYCalcWeaponLocFirst(HUNIT hUnit, int nLoc, HUNIT hItem, BOOL fNoFitTest)
{
	PTINVENTORY ptInventory;
	HUNIT hCur, hOther;
	HUNIT hMove;
	int nResult;
	DWORD dwType;
	DWORD dwOtherType;
	int nRecord;

	nResult = 0;
	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	hCur = INVENTORYGetLocSlot(ptInventory, 4);
	hOther = INVENTORYGetLocSlot(ptInventory, 5);
	if (hItem != NULL && !ITEMSTestTypeBodyLocEqual(hItem, (DWORD)nLoc))
		return 0;
	if (hCur == NULL && hItem != NULL)
	{
		if (fNoFitTest || ITEMSTestFitsInventorySpace(hItem, hUnit, NULL, NULL, NULL))
		{
			dwType = ITEMSGetType(hItem);
			if (dwType == 27)
				nResult = INVENTORYTestOtherHand(hOther, 5) ? 1 : 2;
			else if (dwType == 35)
				nResult = INVENTORYTestOtherHand(hOther, 6) ? 1 : 2;
			else if (dwType == 5)
				nResult = INVENTORYTestOtherHand(hOther, 27) ? 1 : 2;
			else if (dwType == 6)
				nResult = INVENTORYTestOtherHand(hOther, 35) ? 1 : 2;
			else if (hOther == NULL)
				nResult = 1;
			else
			{
				dwOtherType = ITEMSGetType(hOther);
				if ((ITEMSGetTypeTwoHanded(hItem) && !ITEMSTestOneOrTwoHanded(hUnit, hItem))
					|| (ITEMSGetTypeTwoHanded(hOther) && !ITEMSTestOneOrTwoHanded(hUnit, hOther)))
					nResult = 2;
				else if (dwOtherType == 5 || dwOtherType == 6)
					nResult = 2;
				else if (ITEMSIsWeapon(hItem) && ITEMSIsWeapon(hOther))
				{
					if (hUnit->eType == UNIT_PLAYER && hUnit->dwClassId == 4)
						nResult = 1;
					else
						nResult = 2;
				}
				else if (dwOtherType == 2 && ITEMSGetType(hItem) == 2)
					nResult = 2;
				else if (dwOtherType == 2 && (ITEMSGetType(hItem) == 5 || ITEMSGetType(hItem) == 6))
					nResult = 2;
				else
					nResult = 1;
			}
		}
	}
	else if (hCur != NULL && hItem == NULL)
		nResult = 3;
	else if (hOther != NULL && hItem == NULL && ITEMSGetTypeTwoHanded(hOther))
		nResult = 4;
	else if (hCur != NULL && hItem != NULL)
	{
		if (D2CommonITEMSTestCanStack(hCur, hItem))
			return 6;
		if (fNoFitTest || ITEMSTestFitsInventorySpace(hItem, hUnit, NULL, NULL, NULL))
		{
			if (INVENTORYTestHandConflict(hUnit, hCur, hOther, hItem, hCur))
			{
				int nX, nY;
				nRecord = UnitGetInvRecordIndex(hUnit, 0);
				hMove = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[5];
				if (hMove == NULL || !INVENTORYFindFreeSpace(ptInventory, hMove, nRecord, &nX, &nY, 0))
					nResult = 0;
				else if (hUnit->eType != UNIT_PLAYER || hUnit->dwClassId != 4)
					nResult = 7;
				else if (!((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 2)
					|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 2)))
					nResult = 7;
				else if (hOther != NULL && (ITEMSGetType(hItem) == 2 || ITEMSGetType(hCur) == 2
					|| ITEMSGetType(hOther) == 2))
					nResult = 7;
				else
					nResult = 5;
			}
			else if (ITEMSGetTypeTwoHanded(hCur))
				nResult = 5;
			else if (ITEMSGetTypeTwoHanded(hItem) && hOther == NULL)
				nResult = 5;
			else if (ITEMSIsWeapon(hItem) && ITEMSIsWeapon(hCur))
				nResult = 5;
			else if (ITEMSGetType(hItem) == 2 && ITEMSGetType(hCur) == 2)
				nResult = 5;
			else if ((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 2)
				|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 2))
				nResult = 5;
			else if ((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 5)
				|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 5))
				nResult = 5;
			else if ((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 6)
				|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 6))
				nResult = 5;
			else if ((ITEMSGetType(hItem) == 2 && ITEMSGetType(hCur) == 5)
				|| (ITEMSGetType(hCur) != 0 && ITEMSGetType(hItem) == 5))
				nResult = 5;
			else if ((ITEMSGetType(hItem) == 2 && ITEMSGetType(hCur) == 6)
				|| (ITEMSGetType(hCur) == 2 && ITEMSGetType(hItem) == 6))
				nResult = 5;
		}
	}
	return nResult;
}

static BOOL __fastcall INVENTORYTestHandConflict(HUNIT hUnit, HUNIT hSlot4, HUNIT hSlot5, HUNIT hItem,
	HUNIT hReplaced)
{
	DWORD dwType4, dwType5, dwTypeNew, dwTypeRep;
	DWORD dwTT10Slot4, dwTT10Slot5, dwTT10New, dwTT10Rep;
	DWORD dwTwoHandNew, dwTwoHandRep;
	DWORD dwTT0CNew, dwTT0CRep;
	DWORD dwPairType;
	BOOL fRepPaired;

	if (hSlot4 == NULL || hSlot5 == NULL || hReplaced == NULL)
		return FALSE;
	dwType4 = ITEMSGetType(hSlot4);
	dwTT10Slot4 = ITEMSIsArmor(hSlot4);
	dwType5 = ITEMSGetType(hSlot5);
	dwTT10Slot5 = ITEMSIsArmor(hSlot5);
	dwTypeNew = ITEMSGetType(hItem);
	dwTwoHandNew = ITEMSGetTypeTwoHanded(hItem);
	dwTT0CNew = ITEMSIsWeapon(hItem);
	dwTT10New = ITEMSIsArmor(hItem);
	dwTypeRep = ITEMSGetType(hReplaced);
	dwTwoHandRep = ITEMSGetTypeTwoHanded(hReplaced);
	dwTT0CRep = ITEMSIsWeapon(hReplaced);
	dwTT10Rep = ITEMSIsArmor(hReplaced);
	dwPairType = 0;
	if (dwTypeNew == 27)
	{
		if (dwTypeRep == dwTypeNew)
			dwPairType = 5;
		else
			return TRUE;
	}
	else if (dwTypeNew == 35)
	{
		if (dwTypeRep == dwTypeNew)
			dwPairType = 6;
		else
			return TRUE;
	}
	if (dwPairType != 0)
	{
		if (dwType4 == dwPairType || dwType5 == dwPairType)
			return FALSE;
		return TRUE;
	}
	if (dwTypeNew == 5)
	{
		if (dwType4 == 27 && dwTypeRep == 5)
			return FALSE;
		if (dwType5 == 27 && dwTypeRep == 5)
			return FALSE;
		return TRUE;
	}
	if (dwTypeNew == 6)
	{
		if (dwType4 == 35 && dwTypeRep == 6)
			return FALSE;
		if (dwType5 == 35 && dwTypeRep == 6)
			return FALSE;
		return TRUE;
	}
	if (dwTypeNew != dwTypeRep && (dwTwoHandNew != 0 || dwTwoHandRep != 0))
		return TRUE;
	if (dwTT0CNew != 0 && dwTT10Rep != 0)
	{
		if (hUnit->eType == UNIT_PLAYER && hUnit->dwClassId == 4)
			return FALSE;
		return TRUE;
	}
	fRepPaired = dwTypeRep == 35 || dwTypeRep == 6 || dwTypeRep == 27 || dwTypeRep == 5;
	if (fRepPaired)
		return TRUE;
	if (dwTwoHandNew != 0)
		return TRUE;
	if (dwTT10New == 0)
		return FALSE;
	if (hReplaced == hSlot4 && dwTT0CRep != 0 && dwTT10Slot5 != 0)
		return TRUE;
	if (hReplaced == hSlot5 && dwTT0CRep != 0 && dwTT10Slot4 != 0)
		return TRUE;
	return FALSE;
}

static int __fastcall INVENTORYCalcWeaponLocSecond(HUNIT hUnit, int nLoc, HUNIT hItem, BOOL fNoFitTest)
{
	PTINVENTORY ptInventory;
	HUNIT hCur, hOther;
	HUNIT hMove;
	int nResult;
	DWORD dwType;
	DWORD dwOtherType;
	int nRecord;

	nResult = 0;
	ptInventory = (PTINVENTORY)hUnit->ptInventory;
	hOther = INVENTORYGetLocSlot(ptInventory, 4);
	hCur = INVENTORYGetLocSlot(ptInventory, 5);
	if (hItem != NULL && !ITEMSTestTypeBodyLocEqual(hItem, (DWORD)nLoc))
		return 0;
	if (hCur == NULL && hItem != NULL)
	{
		if (fNoFitTest || ITEMSTestFitsInventorySpace(hItem, hUnit, NULL, NULL, NULL))
		{
			dwType = ITEMSGetType(hItem);
			if (dwType == 27)
				nResult = INVENTORYTestOtherHand(hOther, 5) ? 1 : 2;
			else if (dwType == 35)
				nResult = INVENTORYTestOtherHand(hOther, 6) ? 1 : 2;
			else if (dwType == 5)
				nResult = INVENTORYTestOtherHand(hOther, 27) ? 1 : 2;
			else if (dwType == 6)
				nResult = INVENTORYTestOtherHand(hOther, 35) ? 1 : 2;
			else if (hOther == NULL)
				nResult = 1;
			else
			{
				dwOtherType = ITEMSGetType(hOther);
				if ((ITEMSGetTypeTwoHanded(hItem) && !ITEMSTestOneOrTwoHanded(hUnit, hItem))
					|| (ITEMSGetTypeTwoHanded(hOther) && !ITEMSTestOneOrTwoHanded(hUnit, hOther)))
					nResult = 2;
				else if (dwOtherType == 5 || dwOtherType == 6)
					nResult = 2;
				else if (ITEMSIsWeapon(hItem) && ITEMSIsWeapon(hOther))
				{
					if (hUnit->eType == UNIT_PLAYER && hUnit->dwClassId == 4)
						nResult = 1;
					else
						nResult = 2;
				}
				else if (dwOtherType == 2 && ITEMSGetType(hItem) == 2)
					nResult = 2;
				else if (dwOtherType == 2 && (ITEMSGetType(hItem) == 5 || ITEMSGetType(hItem) == 6))
					nResult = 2;
				else
					nResult = 1;
			}
		}
	}
	else if (hCur != NULL && hItem == NULL)
		nResult = 3;
	else if (hCur != NULL && hItem != NULL)
	{
		if (D2CommonITEMSTestCanStack(hCur, hItem))
			return 6;
		if (fNoFitTest || ITEMSTestFitsInventorySpace(hItem, hUnit, NULL, NULL, NULL))
		{
			if (INVENTORYTestHandConflict(hUnit, hOther, hCur, hItem, hCur))
			{
				int nX, nY;
				nRecord = UnitGetInvRecordIndex(hUnit, 0);
				hMove = INVENTORYGetLocInfoChecked(ptInventory)->ahSlot00[4];
				if (hMove == NULL || !INVENTORYFindFreeSpace(ptInventory, hMove, nRecord, &nX, &nY, 0))
					nResult = 0;
				else if (hUnit->eType != UNIT_PLAYER || hUnit->dwClassId != 4)
					nResult = 7;
				else if (!((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 2)
					|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 2)))
					nResult = 7;
				else if (hOther != NULL && (ITEMSGetType(hItem) == 2 || ITEMSGetType(hOther) == 2
					|| ITEMSGetType(hCur) == 2))
					nResult = 7;
				else
					nResult = 5;
			}
			else if (ITEMSGetTypeTwoHanded(hCur))
				nResult = 5;
			else if (ITEMSGetTypeTwoHanded(hItem) && hOther == NULL)
				nResult = 5;
			else if (ITEMSIsWeapon(hItem) && ITEMSIsWeapon(hCur))
				nResult = 5;
			else if (ITEMSGetType(hItem) == 2 && ITEMSGetType(hCur) == 2)
				nResult = 5;
			else if ((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 2)
				|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 2))
				nResult = 5;
			else if ((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 5)
				|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 5))
				nResult = 5;
			else if ((ITEMSIsWeapon(hItem) && ITEMSGetType(hCur) == 6)
				|| (ITEMSIsWeapon(hCur) && ITEMSGetType(hItem) == 6))
				nResult = 5;
			else if ((ITEMSGetType(hItem) == 2 && ITEMSGetType(hCur) == 5)
				|| (ITEMSGetType(hCur) == 2 && ITEMSGetType(hItem) == 5))
				nResult = 5;
			else if ((ITEMSGetType(hItem) == 2 && ITEMSGetType(hCur) == 6)
				|| (ITEMSGetType(hCur) == 2 && ITEMSGetType(hItem) == 6))
				nResult = 5;
		}
	}
	return nResult;
}

static PFNBODYLOCTEST scpfnBodyLocTest[11] =
{
	INVENTORYTestBodyLocNone,
	INVENTORYTestBodyLoc,
	INVENTORYTestBodyLoc,
	INVENTORYTestBodyLoc,
	INVENTORYCalcWeaponLocFirst,
	INVENTORYCalcWeaponLocSecond,
	INVENTORYTestBodyLoc,
	INVENTORYTestBodyLoc,
	INVENTORYTestBodyLoc,
	INVENTORYTestBodyLoc,
	INVENTORYTestBodyLoc
};

int __stdcall INVENTORYTestBodyLocPlace(HUNIT hUnit, int nLoc, HUNIT hItem, BOOL fNoFitTest)
{
	return scpfnBodyLocTest[nLoc](hUnit, nLoc, hItem, fNoFitTest);
}

void __stdcall INVENTORYSoftReset(PTINVENTORY ptInventory)
{
	struct INVFREECHAINA *ptNode, *ptNext;
	DWORD *pdwGrid;
	int i;

	if (ptInventory == NULL)
		return;
	for (ptNode = ptInventory->ptFirstItemNode; ptNode != NULL; ptNode = ptNext)
	{
		ptNext = ptNode->ptNext;
#line 3257
		D2FREEPOOL(ptInventory->pPool, ptNode);
		ptInventory->nEntries--;
	}
#line 3261
	D2ASSERTMSG(ptInventory->nEntries == 0, "ptInventory->tInventoryList.nEntries == 0");
	ptInventory->ptFirstItemNode = NULL;
	ptInventory->ptLastItemNode = NULL;
	pdwGrid = (DWORD *)ptInventory->ptLocInfo;
	for (i = 0; i < (int)(sizeof(LOCINFO) / sizeof(DWORD)); i++)
		pdwGrid[i] = 0;
	ptInventory->dwCursorItem = 0;
}

static void __fastcall INVENTORYMarkUnitItems(PTINVENTORY ptInventory, HUNIT hUnit);

static __inline BOOL INVENTORYMarkTradeItem(PTINVENTORY ptInventory, HUNIT hUnit, HUNIT hItem)
{
	int nRecord;
	INVGRIDINFO tGrid;
	int nX, nY;
	BYTE bWidth, bHeight;
	int x, y;

	nRecord = UnitGetInvRecordIndex(hUnit, 0);
	D2CommonInvTblsGetGrid(nRecord, &tGrid);
	if (!INVENTORYFindFreeSpace(ptInventory, hItem, nRecord, &nX, &nY, 0))
		return FALSE;
	if (nX < 0)
		return FALSE;
	if (nX >= tGrid.gridRows)
		return FALSE;
	if (nY < 0)
		return FALSE;
	if (nY >= tGrid.gridCols)
		return FALSE;
#line 3345
	D2ASSERT(ptInventory);
#line 3346
	D2ASSERT(ptInventory->ptLocInfo);
	ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
	for (y = nY; y < nY + bHeight; y++)
	{
		for (x = nX; x < nX + bWidth; x++)
			ptInventory->ptLocInfo->ahGrid[0][y][x] = (HUNIT)-1;
	}
	return TRUE;
}

static __inline BOOL INVENTORYTradeFits(PTINVENTORY ptInventory, HUNIT hUnit, HUNIT hFrom)
{
	PTINVLISTNODE ptNode, ptNext;
	HUNIT hItem;

	for (ptNode = (PTINVLISTNODE)INVENTORYGetCategoryListHead((PTINVENTORY)hFrom->ptInventory);
		ptNode != NULL; ptNode = ptNext)
	{
		hItem = INVENTORYNodeGetKeyA(ptNode);
		ptNext = INVENTORYNodeGetNext(ptNode);
		if (ITEMSDataGetPage(hItem) != 2)
			continue;
		if (!INVENTORYMarkTradeItem(ptInventory, hUnit, hItem))
			return FALSE;
	}
	return TRUE;
}

BOOL __stdcall INVENTORYTestTradeSpace(void *pPool, HUNIT hUnit1, HUNIT hUnit2, BOOL *pfFirstFits)
{
	PTINVENTORY ptInventory1, ptInventory2;

	ptInventory1 = INVENTORYCreate(pPool, hUnit1->dwId, 0, hUnit1->dwClassId, 0);
	ptInventory2 = INVENTORYCreate(pPool, hUnit2->dwId, 0, hUnit2->dwClassId, 0);
	INVENTORYMarkUnitItems(ptInventory1, hUnit1);
	INVENTORYMarkUnitItems(ptInventory2, hUnit2);
	*pfFirstFits = TRUE;
	if (!INVENTORYTradeFits(ptInventory2, hUnit2, hUnit1))
	{
		INVENTORYDestroy(ptInventory1);
		INVENTORYDestroy(ptInventory2);
		*pfFirstFits = FALSE;
		return FALSE;
	}
	if (!INVENTORYTradeFits(ptInventory1, hUnit1, hUnit2))
	{
		INVENTORYDestroy(ptInventory1);
		INVENTORYDestroy(ptInventory2);
		return FALSE;
	}
	INVENTORYDestroy(ptInventory1);
	INVENTORYDestroy(ptInventory2);
	return TRUE;
}

static void __fastcall INVENTORYMarkUnitItems(PTINVENTORY ptInventory, HUNIT hUnit)
{
	INVGRIDINFO tGrid;
	PTINVLISTNODE ptNode;
	HUNIT hItem;
	int nX, nY;
	BYTE bWidth, bHeight;
	int x, y;

	DWORD dwRecord = UnitGetInvRecordIndex(hUnit, 0);
	D2CommonInvTblsGetGrid(dwRecord, &tGrid);
	ptNode = (PTINVLISTNODE)INVENTORYGetCategoryListHead((PTINVENTORY)hUnit->ptInventory);
#line 3295
	D2ASSERT(ptInventory);
#line 3296
	D2ASSERT(ptInventory->ptLocInfo);
	for (; ptNode != NULL; ptNode = INVENTORYNodeGetNext(ptNode))
	{
		hItem = INVENTORYNodeGetKeyA(ptNode);
		if (ITEMSDataGetPage(hItem) != 0)
			continue;
		nX = (int)UnitGetX(hItem);
		nY = (int)UnitGetY(hItem);
		ITEMSGetTypeInvWidthHeight(hItem, &bWidth, &bHeight);
		for (y = nY; y < nY + bHeight; y++)
		{
			for (x = nX; x < nX + bWidth; x++)
				ptInventory->ptLocInfo->ahGrid[0][y][x] = (HUNIT)-1;
		}
	}
}
