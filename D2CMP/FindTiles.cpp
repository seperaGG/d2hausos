#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\FindTiles.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 8

#define FIND_TILE_HASH_SIZE	128

typedef struct D2FINDTILENODE D2FINDTILENODE;

struct D2FINDTILENODE
{
 	D2TILE		*ptTile;
 	D2FINDTILENODE	*ptNext;
};

struct D2FINDTILEKEY
{
 	DWORD		dwMainIndex;
 	DWORD		dwSubIndex;
 	DWORD		dwOrientation;
 	D2FINDTILENODE	*ptTileRefs;
 	D2FINDTILEKEY	*ptNext;
};

BYTE __fastcall FindTilesHashKey(DWORD dwValue3, DWORD dwValue2, DWORD dwValue1)
{
	BYTE tKey;

	tKey = (BYTE)((BYTE)(2 * dwValue3 - dwValue1 + dwValue2) % FIND_TILE_HASH_SIZE);
	D2_OPAQUE(tKey);
#line 30
	D2ASSERT(tKey < FIND_TILE_HASH_SIZE);
	return tKey;
}

D2FINDTILEKEY ** __fastcall FindTilesCreate(void)
{
	D2FINDTILEKEY **ptHash;

#line 42
	ptHash = (D2FINDTILEKEY **)D2ALLOC2(FIND_TILE_HASH_SIZE * sizeof(D2FINDTILEKEY *));
#line 43
	D2ASSERT(ptHash);
	memset(ptHash, 0, FIND_TILE_HASH_SIZE * sizeof(D2FINDTILEKEY *));
	return ptHash;
}

void __fastcall FindTilesFree(D2FINDTILEKEY **ptHash)
{
	D2FINDTILEKEY *ptKeyNode;
	D2FINDTILEKEY *ptNextKey;
	D2FINDTILENODE *ptRef;
	D2FINDTILENODE *ptNextRef;
	int nBucket;

	if (!ptHash)
		return;
	for (nBucket = 0; nBucket < FIND_TILE_HASH_SIZE; nBucket++)
	{
		for (ptKeyNode = ptHash[nBucket]; ptKeyNode; ptKeyNode = ptNextKey)
		{
			ptNextKey = ptKeyNode->ptNext;
			for (ptRef = ptKeyNode->ptTileRefs; ptRef; ptRef = ptNextRef)
			{
				ptNextRef = ptRef->ptNext;
#line 76
				D2FREE2(ptRef);
			}
			ptKeyNode->ptTileRefs = NULL;
#line 80
			D2FREE2(ptKeyNode);
		}
		ptHash[nBucket] = NULL;
	}
#line 87
	D2FREE2(ptHash);
}

static __inline void FindTilesAddRef(D2FINDTILEKEY *ptKeyNode, D2TILE *ptTile)
{
	D2FINDTILENODE *ptRef;

	for (ptRef = ptKeyNode->ptTileRefs; ptRef; ptRef = ptRef->ptNext)
	{
		if (ptRef->ptTile == ptTile)
			return;
	}
#line 110
	ptRef = (D2FINDTILENODE *)D2ALLOC2(sizeof(D2FINDTILENODE));
#line 111
	D2ASSERT(ptRef);
	memset(ptRef, 0, sizeof(D2FINDTILENODE));
	ptRef->ptTile = ptTile;
	ptRef->ptNext = ptKeyNode->ptTileRefs;
	ptKeyNode->ptTileRefs = ptRef;
}

static __inline void FindTilesAddKey(D2FINDTILEKEY **ppBucket, D2TILE *ptTile)
{
	D2FINDTILEKEY *ptKeyNode;
	D2FINDTILENODE *ptTileRef;

#line 130
	ptKeyNode = (D2FINDTILEKEY *)D2ALLOC2(sizeof(D2FINDTILEKEY));
#line 131
	D2ASSERT(ptKeyNode);
	memset(ptKeyNode, 0, sizeof(D2FINDTILEKEY));
	ptKeyNode->dwSubIndex = ptTile->dwSequence;
	ptKeyNode->dwMainIndex = ptTile->dwStyle;
	ptKeyNode->dwOrientation = ptTile->dwOrientation;
	ptKeyNode->ptNext = *ppBucket;
	*ppBucket = ptKeyNode;
#line 142
	ptTileRef = (D2FINDTILENODE *)D2ALLOC2(sizeof(D2FINDTILENODE));
#line 143
	D2ASSERT(ptTileRef);
	memset(ptTileRef, 0, sizeof(D2FINDTILENODE));
	ptTileRef->ptTile = ptTile;
	ptKeyNode->ptTileRefs = ptTileRef;
}

void __fastcall FindTilesAdd(D2FINDTILEKEY **ptHash, D2TILE *ptTile)
{
	D2FINDTILEKEY **ppBucket;
	D2FINDTILEKEY *ptKeyNode;
	BOOL bNew;

	bNew = TRUE;
#line 165
	D2ASSERT(ptHash);
#line 166
	D2ASSERT(ptTile);
	ppBucket = &ptHash[FindTilesHashKey(ptTile->dwSequence, ptTile->dwStyle, ptTile->dwOrientation)];
	ptKeyNode = *ppBucket;
	while (ptKeyNode && bNew)
	{
		if (ptKeyNode->dwSubIndex == ptTile->dwSequence && ptKeyNode->dwMainIndex == ptTile->dwStyle &&
			ptKeyNode->dwOrientation == ptTile->dwOrientation)
			bNew = FALSE;
		else
			ptKeyNode = ptKeyNode->ptNext;
	}
	if (bNew)
		FindTilesAddKey(ppBucket, ptTile);
	else
		FindTilesAddRef(ptKeyNode, ptTile);
}

int __fastcall FindTilesGet(D2FINDTILEKEY **ptHash, D2TILE **hTileList, DWORD dwMax, DWORD dwValue1,
	DWORD dwValue2, DWORD dwValue3)
{
	D2FINDTILEKEY *ptKey;
	D2FINDTILENODE *ptNode;
	DWORD dwCount;
	int nFound;
	BOOL bFound;

	nFound = 0;
	bFound = FALSE;
#line 207
	D2ASSERT(ptHash);
#line 208
	D2ASSERT(hTileList);
	if (!dwMax)
		return 0;
	ptKey = ptHash[FindTilesHashKey(dwValue3, dwValue2, dwValue1)];
	while (ptKey && !bFound)
	{
		if (ptKey->dwSubIndex == dwValue3 && ptKey->dwMainIndex == dwValue2 && ptKey->dwOrientation
			== dwValue1)
			bFound = TRUE;
		else
			ptKey = ptKey->ptNext;
	}
	if (bFound)
	{
		dwCount = 0;
		for (ptNode = ptKey->ptTileRefs; ptNode && dwCount < dwMax; ptNode = ptNode->ptNext)
		{
			hTileList[dwCount] = ptNode->ptTile;
			nFound++;
			dwCount++;
		}
	}
	return nFound;
}
