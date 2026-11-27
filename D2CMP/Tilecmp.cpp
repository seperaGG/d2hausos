#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\Tilecmp.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 12

typedef struct D2TILELISTTILE D2TILELISTTILE;
typedef struct D2TILELISTCOMP D2TILELISTCOMP;

struct D2TILELIST
{
 	DWORD		field_00;
 	DWORD		dwFillColor;
 	DWORD		dwNumTiles;
 	D2TILELISTTILE	*ptTileHead;
};

struct D2TILELISTCOMP
{
 	WORD		wOffsetX;
 	WORD		wOffsetY;
 	BYTE		bColumn;
 	BYTE		bRow;
 	WORD		wFlags;
 	WORD		wCompLength;
 	WORD		unobserved_0a;
 	BYTE		*ptData;
 	D2TILELISTCOMP	*ptNext;
};

struct D2TILELISTTILE
{
 	int			nMinX;
 	int			nMaxY;
 	int			nMaxX;
 	int			nMinY;
 	DWORD		field_10;
 	WORD		field_14;
 	WORD		wFlags;
 	DWORD		unobserved_18[3];
 	DWORD		dwOrientation;
 	DWORD		dwMainIndex;
 	DWORD		dwSubIndex;
 	DWORD		dwWeight;
 	DWORD		dwFillColor;
 	BYTE		abCollisionMask[25];
 	BYTE		bFormatFlags;
 	WORD		unobserved_52;
 	DWORD		dwCompCount;
 	D2TILELISTCOMP	*ptComps;
 	D2TILELISTTILE	*ptNext;
};

struct D2SQUAREPOS
{
	int			nX;
	int			nY;
};

static D2SQUAREPOS sgatSquarePos[25] =
{
	{ 64, 0 }, { 80, 8 }, { 96, 16 }, { 112, 24 }, { 128, 32 },
	{ 48, 8 }, { 64, 16 }, { 80, 24 }, { 96, 32 }, { 112, 40 },
	{ 32, 16 }, { 48, 24 }, { 64, 32 }, { 80, 40 }, { 96, 48 },
	{ 16, 24 }, { 32, 32 }, { 48, 40 }, { 64, 48 }, { 80, 56 },
	{ 0, 32 }, { 16, 40 }, { 32, 48 }, { 48, 56 }, { 64, 64 },
};

#define TILEIMAGE_WIDTH		160
#define SQUARE_WIDTH		32
#define SQUARE_PIXELS		512

static __inline D2TILELISTCOMP *TileListCompNew(D2TILELISTTILE *ptTileAdd)
{
	D2TILELISTCOMP *ptComp;

#line 34
	ptComp = (D2TILELISTCOMP *)D2ALLOC2(sizeof(D2TILELISTCOMP));
#line 35
	D2ASSERT(ptComp);
	memset(ptComp, 0, sizeof(D2TILELISTCOMP));
	ptComp->ptNext = ptTileAdd->ptComps;
	ptTileAdd->ptComps = ptComp;
	ptTileAdd->dwCompCount++;
	return ptComp;
}

static __inline void TileListCompFree(D2TILELISTCOMP *ptComp)
{
	D2_OPAQUE(ptComp);
#line 48
	D2ASSERT(ptComp);
	if (ptComp->ptData)
	{
#line 52
		D2FREE2(ptComp->ptData);
	}
#line 55
	D2FREE2(ptComp);
}

static __inline D2TILELISTTILE *TileListTileCreate(void)
{
	D2TILELISTTILE *ptTile;

#line 68
	ptTile = (D2TILELISTTILE *)D2ALLOC2(sizeof(D2TILELISTTILE));
#line 69
	D2ASSERT(ptTile);
	memset(ptTile, 0, sizeof(D2TILELISTTILE));
	return ptTile;
}

static __inline void TileListTileFree(D2TILELISTTILE *ptTile)
{
	D2TILELISTCOMP *ptComp;
	D2TILELISTCOMP *ptNext;

	D2_OPAQUE(ptTile);
#line 83
	D2ASSERT(ptTile);
	for (ptComp = ptTile->ptComps; ptComp; ptComp = ptNext)
	{
		ptNext = ptComp->ptNext;
		TileListCompFree(ptComp);
	}
#line 95
	D2FREE2(ptTile);
}

D2TILELIST * WINAPI TileListCreate(DWORD dwFirst, DWORD dwSecond)
{
	D2TILELIST *ptList;

#line 108
	ptList = (D2TILELIST *)D2ALLOC2(sizeof(D2TILELIST));
#line 109
	D2ASSERT(ptList);
	memset(ptList, 0, sizeof(D2TILELIST));
	ptList->field_00 = dwFirst;
	ptList->dwFillColor = dwSecond;
	return ptList;
}

void WINAPI TileListFree(D2TILELIST *ptList)
{
	D2TILELISTTILE *ptTile;
	D2TILELISTTILE *ptNext;

#line 127
	D2ASSERT(ptList);
	for (ptTile = ptList->ptTileHead; ptTile; ptTile = ptNext)
	{
		ptNext = ptTile->ptNext;
		TileListTileFree(ptTile);
	}
#line 139
	D2FREE2(ptList);
}

D2TILELISTTILE * WINAPI TileListAddTile(D2TILELIST *ptList, WORD wValue16, WORD wValue14, DWORD dwValue24,
	DWORD dwValue28, DWORD dwValue2C, DWORD dwValue30, DWORD dwValue10, const BYTE *pbValue38)
{
	D2TILELISTTILE *ptTile;

#line 161
	D2ASSERT(ptList);
	ptTile = TileListTileCreate();
	ptTile->ptNext = ptList->ptTileHead;
	ptList->ptTileHead = ptTile;
	ptList->dwNumTiles++;
	ptTile->wFlags = wValue16;
	ptTile->field_14 = wValue14;
	ptTile->dwOrientation = dwValue24;
	ptTile->dwMainIndex = dwValue28;
	ptTile->dwSubIndex = dwValue2C;
	ptTile->dwWeight = dwValue30;
	ptTile->field_10 = dwValue10;
	ptTile->dwFillColor = ptList->dwFillColor;
	ptTile->bFormatFlags = 2;
	memcpy(ptTile->abCollisionMask, pbValue38, sizeof(ptTile->abCollisionMask));
	return ptTile;
}

void WINAPI TileListTileAddBlock(D2TILELISTTILE *hTileAdd, const BYTE *ptImage, int nPitch, int nOffsetX,
	int nOffsetY)
{
	D2TILELISTCOMP *ptComp;
	BYTE *ptData;
	const BYTE *pbPixel;
	const BYTE *pbFill;
	int nSkip;
	DWORD dwColor;
	DWORD dwPixel;
	int x, y;

#line 255
	D2ASSERT(hTileAdd);
#line 256
	D2ASSERT(ptImage);
	pbPixel = ptImage;
	if (hTileAdd->bFormatFlags & 2)
	{
		nSkip = nPitch - 96;
		pbFill = (const BYTE *)&hTileAdd->dwFillColor;
		dwColor = ((((DWORD)pbFill[2] << 8) + pbFill[1]) << 8) + pbFill[0];
		for (y = 0; y < 32; y++)
		{
			for (x = 0; x < 32; x++)
			{
				dwPixel = *(const DWORD *)pbPixel & 16777215;
				pbPixel += 3;
				if (dwPixel != dwColor)
					goto add;
			}
			pbPixel += nSkip;
		}
	}
	if (!(hTileAdd->bFormatFlags & 1))
		return;
	nSkip = nPitch - 32;
	for (y = 0; y < 32; y++)
	{
		for (x = 0; x < 32; x++)
		{
			if (*pbPixel++)
				goto add;
		}
		pbPixel += nSkip;
	}
	return;
add:
	ptComp = TileListCompNew(hTileAdd);
#line 273
	D2ASSERT(nOffsetX < 0x00010000 && nOffsetY < 0x00010000);
	ptComp->wOffsetY = (WORD)nOffsetY;
	ptComp->wOffsetX = (WORD)nOffsetX;
#line 280
	ptData = (BYTE *)D2ALLOC2(32 * 96);
#line 281
	D2ASSERT(ptData);
	memset(ptData, 0, 32 * 96);
	ptComp->wCompLength = 32 * 96;
	ptComp->ptData = ptData;
	ptComp->wFlags = 4097;
	MemCopyRect(ptData, ptImage, 96, nPitch, 32, 96);
	if (nOffsetX < hTileAdd->nMinX)
		hTileAdd->nMinX = nOffsetX;
	if (nOffsetX + 32 > hTileAdd->nMaxX)
		hTileAdd->nMaxX = nOffsetX + 32;
	if (nOffsetY > hTileAdd->nMaxY)
		hTileAdd->nMaxY = nOffsetY;
	if (nOffsetY - 32 < hTileAdd->nMinY)
		hTileAdd->nMinY = nOffsetY - 32;
}

static __inline void TileListSquareCut(D2TILELISTCOMP *ptRecord, const BYTE *ptImage, int nX, int nY,
	DWORD dwColor)
{
	BYTE *ptSquare;
	const BYTE *pbSrc;
	BYTE *pbDst;
	int nStart;
	int nLength;
	int i;

#line 339
	ptSquare = (BYTE *)D2ALLOC2(SQUARE_PIXELS * 3);
#line 340
	D2ASSERT(ptSquare);
	memset(ptSquare, 0, SQUARE_PIXELS * 3);
	for (i = 0; i < SQUARE_PIXELS; i++)
	{
		ptSquare[3 * i] = ((const BYTE *)&dwColor)[0];
		ptSquare[3 * i + 1] = ((const BYTE *)&dwColor)[1];
		ptSquare[3 * i + 2] = ((const BYTE *)&dwColor)[2];
	}
	for (i = 0, pbSrc = ptImage + 3 * (nY * TILEIMAGE_WIDTH + nX), pbDst = ptSquare, nStart = 42, nLength = 12; i < 15;
		i++, pbDst += SQUARE_WIDTH * 3, pbSrc += TILEIMAGE_WIDTH * 3)
	{
		memcpy(pbDst + nStart, pbSrc + nStart, nLength);
		if (i < 7)
		{
			nStart -= 6;
			nLength += 12;
		}
		else
		{
			nStart += 6;
			nLength -= 12;
		}
	}
	for (i = 0, pbDst = ptSquare; i < SQUARE_PIXELS; pbDst += 3, i++)
	{
		if (pbDst[0] != ((const BYTE *)&dwColor)[0] || pbDst[1] != ((const BYTE *)&dwColor)[1] ||
			pbDst[2] != ((const BYTE *)&dwColor)[2])
		{
			ptRecord->wFlags |= 8193;
			ptRecord->wCompLength = SQUARE_PIXELS * 3;
			ptRecord->ptData = ptSquare;
			return;
		}
	}
#line 389
	D2FREE2(ptSquare);
}

static __inline D2TILELISTCOMP *TileListCompAdd(D2TILELISTTILE *ptTileAdd, const D2TILELISTCOMP *ptRecord)
{
	D2TILELISTCOMP *ptComp;

#line 422
	ptComp = (D2TILELISTCOMP *)D2ALLOC2(sizeof(D2TILELISTCOMP));
#line 423
	D2ASSERT(ptComp);
	memset(ptComp, 0, sizeof(D2TILELISTCOMP));
	*ptComp = *ptRecord;
	ptComp->ptNext = ptTileAdd->ptComps;
	ptTileAdd->dwCompCount++;
	ptTileAdd->ptComps = ptComp;
	return ptComp;
}

void WINAPI TileListTileCut(D2TILELISTTILE *ptTileAdd, const BYTE *ptImage)
{
	D2TILELISTCOMP tRecord;
	D2TILELISTCOMP *ptComp;
	int nSquare;

#line 450
	D2ASSERT(ptTileAdd);
#line 451
	D2ASSERT(ptImage);
	for (nSquare = 0; nSquare < 25; nSquare++)
	{
		memset(&tRecord, 0, sizeof(tRecord));
		TileListSquareCut(&tRecord, ptImage, sgatSquarePos[nSquare].nX, sgatSquarePos[nSquare].nY,
			ptTileAdd->dwFillColor);
		tRecord.bColumn = (BYTE)(nSquare % 5);
		tRecord.bRow = (BYTE)(nSquare / 5);
		if (tRecord.wFlags & 1)
		{
			ptComp = TileListCompAdd(ptTileAdd, &tRecord);
			ptComp->wOffsetX = (WORD)sgatSquarePos[nSquare].nX;
			ptComp->wOffsetY = (WORD)sgatSquarePos[nSquare].nY;
		}
	}
	ptTileAdd->nMaxY = 0;
	ptTileAdd->nMinY = -128;
	ptTileAdd->nMinX = 0;
	ptTileAdd->nMaxX = TILEIMAGE_WIDTH;
}

static __inline void TileProjectTileFromList(D2TILE *ptTile, D2TILELISTTILE *ptTileAdd)
{
	D2TILELISTCOMP *ptComp;
	D2TILECOMP *ptTileComp;
	DWORD dwSize;
	int nTileComp;

	ptTile->field_00 = ptTileAdd->field_10;
	ptTile->wYOffset = ptTileAdd->field_14;
	ptTile->wFlags = ptTileAdd->wFlags;
	ptTile->dwWidth = ptTileAdd->nMaxX - ptTileAdd->nMinX;
	ptTile->dwHeight = ptTileAdd->nMinY - ptTileAdd->nMaxY;
	ptTile->dwOrientation = ptTileAdd->dwOrientation;
	ptTile->dwStyle = ptTileAdd->dwMainIndex;
	ptTile->dwSequence = ptTileAdd->dwSubIndex;
	ptTile->dwRarity = ptTileAdd->dwWeight;
	ptTile->dwKeyColor = ptTileAdd->dwFillColor;
	ptTile->nCompCount = ptTileAdd->dwCompCount;
	memcpy(ptTile->abCollisionMask, ptTileAdd->abCollisionMask, sizeof(ptTile->abCollisionMask));
	dwSize = ptTile->nCompCount * sizeof(D2TILECOMP);
	if (dwSize)
	{
#line 512
		ptTile->ptCompArray = (D2TILECOMP *)D2ALLOC2(dwSize);
#line 513
		D2ASSERT(ptTile->ptCompArray);
		memset(ptTile->ptCompArray, 0, dwSize);
	}
	else
		ptTile->ptCompArray = NULL;
	nTileComp = 0;
	for (ptComp = ptTileAdd->ptComps; ptComp; ptComp = ptComp->ptNext)
	{
		ptTileComp = &ptTile->ptCompArray[nTileComp++];
		ptTileComp->wOffsetX = ptComp->wOffsetX;
		ptTileComp->wOffsetY = ptComp->wOffsetY;
		ptTileComp->bColumn = ptComp->bColumn;
		ptTileComp->bRow = ptComp->bRow;
		ptTileComp->wFlags = ptComp->wFlags;
		ptTileComp->wCompLength = ptComp->wCompLength;
		ptTileComp->ptData = ptComp->ptData;
		ptComp->ptData = NULL;
	}
#line 530
	D2ASSERT(nTileComp == ptTile->nCompCount);
}

DWORD WINAPI TileListCreateProject(D2TILELIST *ptTileList, D2TILEPROJECT **pptProject)
{
	D2TILEPROJECT *ptProject;
	D2TILELISTTILE *ptTileAdd;
	int nTile;
	DWORD dwSize;

#line 547
	D2ASSERT(ptTileList);
	if (!ptTileList->dwNumTiles)
	{
		*pptProject = NULL;
		return 0x85200004;
	}
#line 555
	ptProject = (D2TILEPROJECT *)D2ALLOC2(sizeof(D2TILEPROJECT));
#line 556
	D2ASSERT(ptProject);
	memset(ptProject, 0, sizeof(D2TILEPROJECT));
	ptProject->dwVersion = TILEPROJECT_VERSION;
	if (ptTileList->field_00)
		ptProject->dwFlags = TILEPROJECT_FLAG_24BIT;
	ptProject->nNumTiles = ptTileList->dwNumTiles;
	dwSize = ptProject->nNumTiles * sizeof(D2TILE);
#line 567
	ptProject->ptTileArray = (D2TILE *)D2ALLOC2(dwSize);
#line 568
	D2ASSERT(ptProject->ptTileArray);
	memset(ptProject->ptTileArray, 0, dwSize);
	nTile = 0;
	for (ptTileAdd = ptTileList->ptTileHead; ptTileAdd; ptTileAdd = ptTileAdd->ptNext)
	{
		TileProjectTileFromList(&ptProject->ptTileArray[nTile], ptTileAdd);
		nTile++;
	}
#line 582
	D2ASSERT(nTile == ptProject->nNumTiles);
	*pptProject = ptProject;
	return 0;
}

void WINAPI TileProjectFree(D2TILEPROJECT *ptProject, BOOL bKeep)
{
	D2TILE *ptTile;
	D2TILECOMP *ptTileComp;
	int nTile;
	int nTileComp;

	if (bKeep)
		return;
#line 601
	D2ASSERT(ptProject);
	TileCacheReleaseProject(ptProject);
	if (ptProject->dwFlags & TILEPROJECT_LOADED)
	{
#line 609
		D2FREE2(ptProject);
		return;
	}
#line 614
	D2ASSERT(!(ptProject->dwFlags & TILEPROJECT_FEWBLOCKS));
	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		for (nTileComp = 0; nTileComp < ptTile->nCompCount; nTileComp++)
		{
			ptTileComp = &ptTile->ptCompArray[nTileComp];
			if (ptTileComp->ptData)
			{
#line 627
				D2FREE2(ptTileComp->ptData);
			}
			ptTileComp->wCompLength = 0;
			ptTileComp->ptData = NULL;
		}
		if (ptTile->ptCompArray)
		{
#line 634
			D2FREE2(ptTile->ptCompArray);
		}
		ptTile->ptCompArray = NULL;
		ptTile->nCompCount = 0;
	}
#line 640
	D2FREE2(ptProject->ptTileArray);
	ptProject->ptTileArray = NULL;
	ptProject->nNumTiles = 0;
#line 644
	D2FREE2(ptProject);
}

void WINAPI TileProjectSerialize(D2TILEPROJECT *ptProject, BYTE **ptptData, DWORD *ptnSize)
{
	D2TILEPROJECT *ptSerialized;
	D2TILE *ptTile;
	D2TILE *ptNewTiles;
	D2TILE *ptNewTile;
	D2TILECOMP *ptTileComp;
	BYTE *ptData;
	BYTE *pbStart;
	BYTE *pbPixels;
	DWORD nSize;
	DWORD dwTileSize;
	int nTile;
	int nTileComp;

#line 728
	D2ASSERT(ptProject);
#line 729
	D2ASSERT(ptptData);
#line 730
	D2ASSERT(ptnSize);
#line 731
	D2ASSERT(ptProject->nNumTiles);
	nSize = ptProject->nNumTiles * sizeof(D2TILE) + sizeof(D2TILEPROJECT);
	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		nSize += ptTile->nCompCount * sizeof(D2TILECOMP);
		for (nTileComp = 0; nTileComp < ptTile->nCompCount; nTileComp++)
			nSize += ptTile->ptCompArray[nTileComp].wCompLength;
	}
#line 735
	D2ASSERT(nSize);
#line 738
	ptData = (BYTE *)D2ALLOC2(nSize);
#line 739
	D2ASSERT(ptData);
	memset(ptData, 0, nSize);
	*ptptData = ptData;
	*ptnSize = nSize;
	pbStart = ptData;
	ptSerialized = (D2TILEPROJECT *)ptData;
	memcpy(ptSerialized, ptProject, sizeof(D2TILEPROJECT));
	ptData += sizeof(D2TILEPROJECT);
	dwTileSize = ptProject->nNumTiles * sizeof(D2TILE);
	memcpy(ptData, ptProject->ptTileArray, dwTileSize);
	ptSerialized->ptTileArray = (D2TILE *)(ptData - pbStart);
	ptNewTiles = (D2TILE *)ptData;
	ptData += dwTileSize;
	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		ptNewTile = &ptNewTiles[nTile];
		ptNewTile->ptCompArray = NULL;
		dwTileSize = ptTile->nCompCount * sizeof(D2TILECOMP);
		memcpy(ptData, ptTile->ptCompArray, dwTileSize);
		pbPixels = ptData + dwTileSize;
		for (nTileComp = 0; nTileComp < ptTile->nCompCount; nTileComp++)
		{
			ptTileComp = &ptTile->ptCompArray[nTileComp];
			memcpy(pbPixels, ptTileComp->ptData, ptTileComp->wCompLength);
			((D2TILECOMP *)ptData)[nTileComp].ptData = (BYTE *)(pbPixels - ptData);
			pbPixels += ptTileComp->wCompLength;
			dwTileSize += ptTileComp->wCompLength;
		}
		ptNewTile->dwCompSize = dwTileSize;
		ptNewTile->dwCompOffset = ptData - pbStart;
		ptData += dwTileSize;
	}
	ptProject->dwFlags |= TILEPROJECT_SERIALIZED;
	ptSerialized->dwFlags |= TILEPROJECT_SERIALIZED | TILEPROJECT_LOADED;
}

void WINAPI TileProjectLoad(D2TILEPROJECT *ptLoaded, D2TILEPROJECT **pptProject, const char *pszName)
{
	D2TILEPROJECT *ptProject;
	D2TILE *ptTile;
	D2TILECOMP *ptTileComp;
	BYTE *pbNext;
	DWORD dwTileSize;
	int nTile;
	int nTileComp;

	ptProject = ptLoaded;
#line 823
	D2ASSERT(ptProject->dwVersion == TILEPROJECT_VERSION);
	if (!(ptProject->dwFlags & TILEPROJECT_FLAG_24BIT) && !(ptProject->dwFlags & TILEPROJECT_SERIALIZED))
		return;
	ptProject->ptTileArray = (D2TILE *)((BYTE *)ptProject + (DWORD)ptProject->ptTileArray);
	if (pszName)
		strcpy(ptProject->szName, pszName);
	pbNext = (BYTE *)ptProject + sizeof(D2TILEPROJECT) + ptProject->nNumTiles * sizeof(D2TILE);
	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		ptTile->pszProjectName = ptProject->szName;
		ptTile->ptCompArray = (D2TILECOMP *)((BYTE *)ptProject + ptTile->dwCompOffset);
		dwTileSize = ptTile->nCompCount * sizeof(D2TILECOMP);
		for (nTileComp = 0; nTileComp < ptTile->nCompCount; nTileComp++)
		{
			ptTileComp = &ptTile->ptCompArray[nTileComp];
			ptTileComp->ptData += (DWORD)pbNext;
			dwTileSize += ptTileComp->wCompLength;
		}
		pbNext += dwTileSize;
	}
	ptProject->dwFlags = (ptProject->dwFlags & ~TILEPROJECT_SERIALIZED) | TILEPROJECT_LOADED;
	*pptProject = ptProject;
}

void WINAPI TileProjectLoadTiles(D2TILEPROJECT *ptProject, D2TILEPROJECT **pptProject, const char *szName)
{
	D2TILE *ptTile;
	int nTile;

#line 878
	D2ASSERT(ptProject->dwVersion == TILEPROJECT_VERSION);
	if (!(ptProject->dwFlags & TILEPROJECT_SERIALIZED))
		return;
	ptProject->ptTileArray = (D2TILE *)((BYTE *)ptProject + (DWORD)ptProject->ptTileArray);
	strcpy(ptProject->szName, szName);
	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		ptProject->ptTileArray[nTile].pszProjectName = ptProject->szName;
#line 913
		D2ASSERT(ptTile->ptCompArray == NULL);
#line 914
		D2ASSERT(ptTile->ptLRUCache == NULL);
	}
	ptProject->dwFlags = (ptProject->dwFlags & ~TILEPROJECT_SERIALIZED) | TILEPROJECT_LOADED;
	*pptProject = ptProject;
}

D2FINDTILEKEY ** __fastcall TileProjectFindTiles(D2TILEPROJECT *hProject)
{
	D2FINDTILEKEY **hFindTile;
	int nTile;

	hFindTile = FindTilesCreate();
#line 936
	D2ASSERT(hFindTile);
#line 937
	D2ASSERT(hProject);
	for (nTile = 0; nTile < hProject->nNumTiles; nTile++)
		FindTilesAdd(hFindTile, &hProject->ptTileArray[nTile]);
	return hFindTile;
}

void WINAPI TileProjectTileParts(D2TILE *ptCached)
{
	D2TILE *ptTile;
	BYTE *pbBase;
	D2TILECOMP *ptTileComp;
	DWORD dwTileSize;
	int nTileComp;

	ptTile = ptCached;
#line 952
	D2ASSERT(ptTile);
	pbBase = (BYTE *)ptTile->ptCompArray;
	dwTileSize = ptTile->nCompCount * sizeof(D2TILECOMP);
	for (nTileComp = 0; nTileComp < ptTile->nCompCount; nTileComp++)
	{
		ptTileComp = (D2TILECOMP *)(nTileComp * sizeof(D2TILECOMP) + (DWORD)ptTile->ptCompArray);
		ptTileComp->ptData += (DWORD)pbBase;
		dwTileSize += ptTileComp->wCompLength;
	}
	(void)dwTileSize;
}

void WINAPI TileProjectForEachPixel(D2TILEPROJECT *ptProject, void (WINAPI *pfnPixel)(DWORD))
{
	D2TILE *ptTile;
	D2TILECOMP *ptTileComp;
	const BYTE *pbPixel;
	int nLength;
	DWORD dwPixels;
	int nTile;
	int nTileComp;

#line 985
	D2ASSERT(ptProject);
#line 986
	D2ASSERT(ptProject->dwFlags & TILEPROJECT_FLAG_24BIT);
	for (nTile = 0; nTile < ptProject->nNumTiles; nTile++)
	{
		ptTile = &ptProject->ptTileArray[nTile];
		for (nTileComp = 0; nTileComp < ptTile->nCompCount; nTileComp++)
		{
			ptTileComp = &ptTile->ptCompArray[nTileComp];
			pbPixel = ptTileComp->ptData;
			nLength = ptTileComp->wCompLength;
			if (nLength > 0)
			{
				dwPixels = (DWORD)(nLength + 2) / 3;
				do
				{
					pfnPixel(((((DWORD)pbPixel[2] << 8) + pbPixel[1]) << 8) + pbPixel[0]);
					pbPixel += 3;
				} while (--dwPixels);
			}
		}
	}
}

static __inline BOOL TileCompIsEmpty(const BYTE *pbData, int nSize)
{
	int i;

	for (i = 0; i < nSize; pbData++, i++)
	{
		if (*pbData)
			return FALSE;
	}
	return TRUE;
}

static __inline void TileProjectConvertComp(D2TILECOMP *ptComp8, const D2TILECOMP *ptComp24,
	DWORD dwKeyColor, BYTE (WINAPI *pfnQuantize)(BYTE, BYTE, BYTE))
{
	BYTE *ptData;
	int nSize;

	if (!ptComp24->ptData)
	{
		ptComp8->wFlags &= ~TILECOMP_PRESENT;
		ptComp8->wCompLength = 0;
		ptComp8->ptData = NULL;
		return;
	}
	SubTileConvertComp(ptComp8, ptComp24, dwKeyColor, pfnQuantize);
	if (TileCompIsEmpty(ptComp8->ptData, ptComp8->wCompLength))
	{
#line 1048
		D2FREE2(ptComp8->ptData);
		ptComp8->wFlags &= ~TILECOMP_PRESENT;
		ptComp8->wCompLength = 0;
		ptComp8->ptData = NULL;
		return;
	}
	if (ptComp8->wFlags & TILECOMP_BLOCK)
	{
		ptData = SubTileEncodeBlock(ptComp8->ptData, &nSize);
#line 1059
		D2ASSERT(ptData);
#line 1060
		D2ASSERT(nSize < 0x00010000);
#line 1061
		D2FREE2(ptComp8->ptData);
		ptComp8->ptData = ptData;
		ptComp8->wCompLength = (WORD)nSize;
		return;
	}
#line 1066
	D2ASSERT(ptComp8->wFlags & TILECOMP_GAMESQUARE);
	SubTileCompress(ptComp8);
}

void WINAPI TileProjectConvert24Bit(D2TILEPROJECT *ptSource, D2TILEPROJECT **pptProject8,
	BYTE (WINAPI *pfnQuantize)(BYTE, BYTE, BYTE))
{
	D2TILEPROJECT *ptProject24;
	D2TILEPROJECT *ptProject8;
	D2TILE *ptTile24;
	D2TILE *ptTile8;
	D2TILECOMP *ptComp8;
	D2TILECOMP *ptComp24;
	DWORD nSize;
	DWORD nArraySize;
	int nTile;
	int nTileComp;

	ptProject24 = ptSource;
#line 1086
	D2ASSERT(ptProject24);
#line 1087
	D2ASSERT(ptProject24->dwFlags & TILEPROJECT_FLAG_24BIT);
#line 1090
	ptProject8 = (D2TILEPROJECT *)D2ALLOC2(sizeof(D2TILEPROJECT));
#line 1091
	D2ASSERT(ptProject8);
	memset(ptProject8, 0, sizeof(D2TILEPROJECT));
	memcpy(ptProject8, ptProject24, sizeof(D2TILEPROJECT));
	ptProject8->dwFlags &= ~TILEPROJECT_FLAG_24BIT;
	ptProject8->ptTileArray = NULL;
	nArraySize = ptProject8->nNumTiles * sizeof(D2TILE);
#line 1101
	ptProject8->ptTileArray = (D2TILE *)D2ALLOC2(nArraySize);
#line 1102
	D2ASSERT(ptProject8->ptTileArray);
	memset(ptProject8->ptTileArray, 0, nArraySize);
	for (nTile = 0; nTile < ptProject24->nNumTiles; nTile++)
	{
		ptTile8 = &ptProject8->ptTileArray[nTile];
		ptTile24 = &ptProject24->ptTileArray[nTile];
		memcpy(ptTile8, ptTile24, sizeof(D2TILE));
		ptTile8->ptCompArray = NULL;
		if (!ptTile8->nCompCount)
			continue;
		nSize = ptTile8->nCompCount * sizeof(D2TILECOMP);
#line 1119
		D2ASSERT(nSize);
#line 1121
		ptTile8->ptCompArray = (D2TILECOMP *)D2ALLOC2(nSize);
#line 1122
		D2ASSERT(ptTile8->ptCompArray);
		memset(ptTile8->ptCompArray, 0, nSize);
		for (nTileComp = 0; nTileComp < ptTile24->nCompCount; nTileComp++)
		{
			ptComp8 = &ptTile8->ptCompArray[nTileComp];
			ptComp24 = &ptTile24->ptCompArray[nTileComp];
			TileProjectConvertComp(ptComp8, ptComp24, ptTile24->dwKeyColor, pfnQuantize);
		}
	}
	*pptProject8 = ptProject8;
}

void WINAPI TileProjectCopyHeader(D2TILEPROJECT *ptProject, D2TILEPROJECT **pptNewProject, DWORD *pdwSize)
{
	D2TILEPROJECT *ptNewProject;
	D2TILE *ptTile;
	DWORD dwSize;
	int nTile;

#line 1154
	D2ASSERT(ptProject);
	dwSize = sizeof(D2TILEPROJECT) + ptProject->nNumTiles * sizeof(D2TILE);
#line 1161
	ptNewProject = (D2TILEPROJECT *)D2ALLOC2(dwSize);
#line 1162
	D2ASSERT(ptNewProject);
	memset(ptNewProject, 0, dwSize);
	memcpy(ptNewProject, ptProject, dwSize);
	ptNewProject->ptTileArray = (D2TILE *)(ptNewProject + 1);
	ptTile = ptNewProject->ptTileArray;
	for (nTile = 0; nTile < ptNewProject->nNumTiles; nTile++, ptTile++)
	{
		ptTile->ptCompArray = NULL;
		ptTile->nCompCount = 0;
	}
	ptNewProject->dwFlags &= TILEPROJECT_LOADED;
	*pptNewProject = ptNewProject;
	*pdwSize = dwSize;
}
