#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Text\\Text.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"

PTTEXTNODE __stdcall TextListNew(void *pPool)
{
	PTTEXTNODE ptList;

#line 43
	ptList = (PTTEXTNODE)D2ALLOCPOOL(pPool, sizeof(TEXTNODE));
	ptList->uHead.pPool = pPool;
	ptList->pNext = NULL;
	ptList->uCount.wCount = 0;
	return ptList;
}

void __stdcall TextListFree(PTTEXTNODE ptList)
{
	PTTEXTNODE ptNode;
	PTTEXTNODE ptNext;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		ptNext = ptNode->pNext;
#line 60
		D2FREEPOOL(ptList->uHead.pPool, ptNode);
		ptNode = ptNext;
	}
	ptList->uCount.wCount = 0;
#line 76
	D2FREEPOOL(ptList->uHead.pPool, ptList);
}

static void TextListFreeChain(PTTEXTNODE ptList)
{
	PTTEXTNODE ptNode;
	PTTEXTNODE ptNext;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		ptNext = ptNode->pNext;
#line 60
		D2FREEPOOL(ptList->uHead.pPool, ptNode);
		ptNode = ptNext;
	}
}

PTTEXTNODE __stdcall TextListInsert(PTTEXTNODE ptList, WORD wId, DWORD dwTag)
{
	PTTEXTNODE ptNode;
	PTTEXTNODE ptNext;

	ptNext = ptList->pNext;
#line 90
	ptNode = (PTTEXTNODE)D2ALLOCPOOL(ptList->uHead.pPool, sizeof(TEXTNODE));
	ptNode->pNext = ptNext;
	ptNode->uHead.wId = wId;
	ptNode->uCount.dwTag = dwTag;
	ptList->uCount.wCount++;
	ptList->pNext = ptNode;
	return ptNode;
}

void __stdcall TextListRemoveById(PTTEXTNODE ptList, WORD wId)
{
	PTTEXTNODE ptPrev = NULL;
	PTTEXTNODE ptNode;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		if (ptNode->uHead.wId == wId)
		{
			if (ptPrev != NULL)
				ptPrev->pNext = ptNode->pNext;
			else
				ptList->pNext = ptNode->pNext;
#line 123
			D2FREEPOOL(ptList->uHead.pPool, ptNode);
			ptList->uCount.wCount--;
			return;
		}
		ptPrev = ptNode;
		ptNode = ptNode->pNext;
	}
}

DWORD __stdcall TextListCountMenuLines(PTTEXTNODE ptList)
{
	DWORD dwCount = 0;
	PTTEXTNODE ptNode;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		if (ptNode->uCount.dwTag == 2)
			dwCount++;
		ptNode = ptNode->pNext;
	}
	return dwCount;
}

WORD __stdcall TextListFindMenuLineId(PTTEXTNODE ptList, DWORD nIndex)
{
	DWORD nCount = 0;
	PTTEXTNODE ptNode;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		if (ptNode->uCount.dwTag == 2)
		{
			if (nCount == nIndex)
				return ptNode->uHead.wId;
			nCount++;
		}
		ptNode = ptNode->pNext;
	}
	return 3724;
}

WORD __stdcall TextListFindOpeningTextId(PTTEXTNODE ptList, DWORD nIndex)
{
	DWORD nCount = 0;
	PTTEXTNODE ptNode;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		if (ptNode->uCount.dwTag == 0)
		{
			if (nCount == nIndex)
				return ptNode->uHead.wId;
			nCount++;
		}
		ptNode = ptNode->pNext;
	}
	return 65535;
}

WORD __stdcall TextListFindFallbackTextId(PTTEXTNODE ptList, DWORD nIndex)
{
	DWORD nCount = 0;
	PTTEXTNODE ptNode;

	ptNode = ptList->pNext;
	while (ptNode != NULL)
	{
		if (ptNode->uCount.dwTag == 1)
		{
			if (nCount == nIndex)
				return ptNode->uHead.wId;
			nCount++;
		}
		ptNode = ptNode->pNext;
	}
	return 65535;
}

void __stdcall TextListPack(PTTEXTNODE pTextHead, PTTEXTLISTMSG pMsgList)
{
	PTTEXTNODE ptNode;
	int Offset;

#line 237
	D2ASSERT(pTextHead != NULL);
#line 238
	D2ASSERT(pMsgList != NULL);
	memset(pMsgList, 0, sizeof(TEXTLISTMSG));
	ptNode = pTextHead->pNext;
	pMsgList->bCount = (BYTE)pTextHead->uCount.wCount;
	Offset = 0;
	while (ptNode != NULL)
	{
#line 248
		D2ASSERT(Offset < MAX_TEXT_LIST_NODES);
		pMsgList->aEntry[Offset].bTag = (BYTE)ptNode->uCount.dwTag;
		pMsgList->aEntry[Offset].wId = ptNode->uHead.wId;
		Offset++;
		ptNode = ptNode->pNext;
	}
}

void __stdcall TextListUnpack(PTTEXTNODE pTextHead, PTTEXTLISTMSG pMsgList)
{
	int Offset;

#line 267
	D2ASSERT(pTextHead != NULL);
#line 268
	D2ASSERT(pMsgList != NULL);
	TextListFreeChain(pTextHead);
	pTextHead->uCount.wCount = 0;
#line 274
	D2ASSERT(pMsgList->bCount < MAX_TEXT_LIST_NODES);
	for (Offset = 0; Offset < pMsgList->bCount; Offset++)
		TextListInsert(pTextHead, pMsgList->aEntry[Offset].wId, (DWORD)pMsgList->aEntry[Offset].bTag);
}

static int TextCompareIds(PTTEXTNODE ptA, PTTEXTNODE ptB)
{
	if (ptA->uHead.wId > ptB->uHead.wId)
		return 1;
	if (ptA->uHead.wId < ptB->uHead.wId)
		return -1;
	return 0;
}

static void TextSortPairs(PTTEXTNODE ptArray, WORD wCount)
{
	TEXTNODE tPrev;
	DWORD i, j;

	if (wCount <= 1)
		return;
	tPrev.uHead.wId = ptArray[0].uHead.wId;
	tPrev.uCount.dwTag = ptArray[0].uCount.dwTag;
	for (i = 1; i < wCount; i++)
	{
		TEXTNODE tKey;

		tKey.uHead.wId = ptArray[i].uHead.wId;
		tKey.uCount.dwTag = ptArray[i].uCount.dwTag;
		if (TextCompareIds(&tPrev, &tKey) > 0)
		{
			ptArray[i].uHead.wId = tPrev.uHead.wId;
			ptArray[i].uCount.dwTag = tPrev.uCount.dwTag;
			for (j = i - 1; j > 0; j--)
			{
				TEXTNODE tMoved;

				tMoved.uCount.dwTag = ptArray[j - 1].uCount.dwTag;
				tMoved.uHead.wId = ptArray[j - 1].uHead.wId;
				if (TextCompareIds(&tMoved, &tKey) <= 0)
					break;
				ptArray[j].uCount.dwTag = tMoved.uCount.dwTag;
				ptArray[j].uHead.wId = tMoved.uHead.wId;
			}
			ptArray[j].uCount.dwTag = tKey.uCount.dwTag;
			ptArray[j].uHead.wId = tKey.uHead.wId;
		}
		else
		{
			tPrev.uHead.wId = tKey.uHead.wId;
			tPrev.uCount.dwTag = tKey.uCount.dwTag;
		}
	}
}

void __stdcall TextListSort(PTTEXTNODE pTextHead)
{
	PTTEXTNODE ptNode;
	PTTEXTNODE ptArray;
	WORD wCount;
	int nPair;
	int i;

#line 357
	D2ASSERT(pTextHead != NULL);
	wCount = pTextHead->uCount.wCount;
	if (wCount <= 1)
		return;
	ptNode = pTextHead->pNext;
#line 364
	ptArray = (PTTEXTNODE)D2ALLOCPOOL(pTextHead->uHead.pPool, wCount * sizeof(TEXTNODE));
	for (nPair = 0; ptNode != NULL; nPair++)
	{
		ptArray[nPair].uHead.wId = ptNode->uHead.wId;
		ptArray[nPair].uCount.dwTag = ptNode->uCount.dwTag;
		ptNode = ptNode->pNext;
	}
	TextSortPairs(ptArray, pTextHead->uCount.wCount);
	ptNode = pTextHead->pNext;
	for (i = 0; i < pTextHead->uCount.wCount; i++)
	{
		ptNode->uHead.wId = ptArray[i].uHead.wId;
		ptNode->uCount.dwTag = ptArray[i].uCount.dwTag;
		ptNode = ptNode->pNext;
	}
#line 382
	D2FREEPOOL(pTextHead->uHead.pPool, ptArray);
}
