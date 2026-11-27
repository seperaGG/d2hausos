#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\SubTile.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 12

static void __fastcall SubTileEncodeDiamond(D2TILECOMP *ptComp, const BYTE *pbPixels);
static void __fastcall SubTileSquish(D2TILECOMP *ptComp, const BYTE *pbPixels);

BYTE * __fastcall SubTileEncodeBlock(const BYTE *pbPixels, int *pnSize)
{
	BYTE *pbStart;
	BYTE *pbOut;
	const BYTE *pbRowEnd;
	const BYTE *pbNextRow;
	DWORD dwRow;
	int nAtEnd;
	int nRestSame;
	int nSkip;
	DWORD dwCount;

#line 28
	pbStart = (BYTE *)D2ALLOC2(3104);
	pbOut = pbStart;
	pbNextRow = pbPixels + 32;
	dwRow = 0;
	pbRowEnd = pbPixels + 32;
	while (dwRow < 32)
	{
		nSkip = CountValue(0, pbPixels, pbRowEnd, 127, &nAtEnd, &nRestSame);
		pbPixels += nSkip;
		if (nAtEnd || nRestSame)
		{
			*pbOut++ = 0;
			pbPixels = pbNextRow;
			pbNextRow += 32;
			pbRowEnd += 32;
			*pbOut++ = 0;
			dwRow++;
		}
		else if (nSkip == 127)
			*pbOut++ = (BYTE)nSkip;
		else
		{
			dwCount = CountUntil(pbPixels, pbRowEnd, 0, 127, &nAtEnd, &nRestSame);
			*pbOut++ = (BYTE)nSkip;
			*pbOut++ = (BYTE)dwCount;
			while (dwCount > 0)
			{
				*pbOut++ = *pbPixels++;
				dwCount--;
			}
			if (nAtEnd || nRestSame)
			{
				*pbOut++ = 0;
				pbPixels = pbNextRow;
				pbNextRow += 32;
				pbRowEnd += 32;
				*pbOut++ = 0;
				dwRow++;
			}
		}
	}
	*pnSize = pbOut - pbStart;
	return pbStart;
}

D2TILECOMP * __fastcall SubTileMakeComp(const BYTE *pbPixels, BOOL fRaw)
{
	D2TILECOMP *ptComp;
	BYTE *pbData;
	int nCompLength;
	int nLength;

	if (fRaw)
	{
#line 102
		pbData = (BYTE *)D2ALLOC2(3072);
		memcpy(pbData, pbPixels, 3072);
		nCompLength = 3072;
	}
	else
		pbData = SubTileEncodeBlock(pbPixels, &nCompLength);
#line 110
	ptComp = (D2TILECOMP *)D2ALLOC2(sizeof(D2TILECOMP));
	memset(ptComp, 0, sizeof(D2TILECOMP));
	nLength = nCompLength;
#line 112
	D2ASSERT(nCompLength > 0x0000FFFF);
	ptComp->ptData = pbData;
	ptComp->wCompLength = nLength;
	return ptComp;
}

void __fastcall SubTileConvertComp(D2TILECOMP *ptComp8, const D2TILECOMP *ptComp24, DWORD dwKeyColor,
	BYTE (WINAPI *pfnQuantize)(BYTE bColor0, BYTE bColor1, BYTE bColor2))
{
	const BYTE *pbRgb;
	const BYTE *pbKey;
	BYTE *pbOut;
	int nSize;
	int i;
	BYTE bColor;

#line 130
	D2ASSERT(ptComp8);
#line 131
	D2ASSERT(ptComp24);
#line 132
	D2ASSERT(ptComp24->ptData);
	*ptComp8 = *ptComp24;
	ptComp8->wFlags &= ~TILECOMP_PRESENT;
	ptComp8->ptData = NULL;
	ptComp8->wCompLength = 0;
	nSize = ptComp24->wCompLength / 3;
#line 142
	D2ASSERT(nSize);
	D2_OPAQUE(nSize);
#line 143
	D2ASSERT(nSize < 0x00010000);
#line 144
	ptComp8->ptData = (BYTE *)D2ALLOC2(nSize);
#line 145
	D2ASSERT(ptComp8->ptData);
	memset(ptComp8->ptData, 0, nSize);
	ptComp8->wFlags |= TILECOMP_PRESENT;
	ptComp8->wCompLength = (WORD)nSize;
	pbRgb = ptComp24->ptData;
	pbOut = ptComp8->ptData;
	pbKey = (const BYTE *)&dwKeyColor;
	for (i = 0; i < nSize; pbRgb += 3, i++)
	{
		bColor = 0;
		if (pbRgb[0] != pbKey[0] || pbRgb[1] != pbKey[1] || pbRgb[2] != pbKey[2])
		{
			bColor = pfnQuantize(pbRgb[0], pbRgb[1], pbRgb[2]);
#line 161
			D2ASSERT(bColor);
		}
		*pbOut++ = bColor;
	}
}

static __inline BOOL SubTileDiamondHasHole(const BYTE *pbPixels)
{
	const BYTE *pbRow;
	int nStart;
	int nLength;
	int nRow;
	int i;

	pbRow = pbPixels;
	nRow = 0;
	nStart = 14;
	nLength = 4;
	for (; nRow < 15; nRow++)
	{
		for (i = 0; i < nLength; i++)
		{
			if (!pbRow[nStart + i])
				return TRUE;
		}
		pbRow += 32;
		if (nRow < 7)
		{
			nStart -= 2;
			nLength += 4;
		}
		else
		{
			nStart += 2;
			nLength -= 4;
		}
	}
	return FALSE;
}

void __fastcall SubTileCompress(D2TILECOMP *ptComp)
{
	BYTE *pbPixels;

	pbPixels = ptComp->ptData;
	if (SubTileDiamondHasHole(pbPixels))
		SubTileEncodeDiamond(ptComp, pbPixels);
	else
		SubTileSquish(ptComp, pbPixels);
#line 478
	D2FREE2(pbPixels);
}

static void __fastcall SubTileEncodeDiamond(D2TILECOMP *ptComp, const BYTE *pbPixels)
{
	BYTE *ptDestBuffer;
	BYTE *ptNewData;
	BYTE *pbOut;
	const BYTE *pbRowEnd;
	const BYTE *pbNextRow;
	DWORD dwRow;
	int nAtEnd;
	int nRestSame;
	int nSkip;
	int nCount;
	int nSize;

#line 296
	ptDestBuffer = (BYTE *)D2ALLOC2(1455);
#line 297
	D2ASSERT(ptDestBuffer);
	memset(ptDestBuffer, 0, 1455);
	pbOut = ptDestBuffer;
	pbNextRow = pbPixels + 32;
	dwRow = 0;
	pbRowEnd = pbPixels + 32;
	while (dwRow < 15)
	{
		nSkip = CountValue(0, pbPixels, pbRowEnd, 127, &nAtEnd, &nRestSame);
		pbPixels += nSkip;
		if (nAtEnd || nRestSame)
		{
			*pbOut++ = 0;
			pbPixels = pbNextRow;
			pbNextRow += 32;
			pbRowEnd += 32;
			*pbOut++ = 0;
			dwRow++;
		}
		else if (nSkip == 127)
			*pbOut++ = (BYTE)nSkip;
		else
		{
			nCount = CountUntil(pbPixels, pbRowEnd, 0, 127, &nAtEnd, &nRestSame);
			*pbOut++ = (BYTE)nSkip;
			*pbOut++ = (BYTE)nCount;
			while (nCount > 0)
			{
				*pbOut++ = *pbPixels++;
				nCount--;
			}
			if (nAtEnd || nRestSame)
			{
				*pbOut++ = 0;
				pbPixels = pbNextRow;
				pbNextRow += 32;
				pbRowEnd += 32;
				*pbOut++ = 0;
				dwRow++;
			}
		}
	}
	nSize = pbOut - ptDestBuffer;
#line 358
	D2ASSERT(nSize < 0x00010000);
#line 361
	ptNewData = (BYTE *)D2ALLOC2(nSize);
#line 362
	D2ASSERT(ptNewData);
	memset(ptNewData, 0, nSize);
	memcpy(ptNewData, ptDestBuffer, nSize);
	ptComp->wFlags |= 5;
	ptComp->ptData = ptNewData;
	ptComp->wCompLength = (WORD)nSize;
#line 374
	D2FREE2(ptDestBuffer);
}

static void __fastcall SubTileSquish(D2TILECOMP *ptComp, const BYTE *pbPixels)
{
	BYTE *ptSquished;
	BYTE *pbStart;
	int nStart;
	int nLength;
	int nRow;

#line 389
	ptSquished = (BYTE *)D2ALLOC2(256);
#line 390
	D2ASSERT(ptSquished);
	memset(ptSquished, 0, 256);
	pbStart = ptSquished;
	nRow = 0;
	nStart = 14;
	nLength = 4;
	for (; nRow < 15; nRow++)
	{
		memcpy(ptSquished, pbPixels + nStart, nLength);
		ptSquished += nLength;
		pbPixels += 32;
		if (nRow < 7)
		{
			nStart -= 2;
			nLength += 4;
		}
		else
		{
			nStart += 2;
			nLength -= 4;
		}
	}
	ptComp->wFlags = TILECOMP_PRESENT;
	ptComp->wCompLength = 256;
	ptComp->ptData = pbStart;
}

static __inline BYTE *SubTileQuantize(const D2TILECOMP *ptComp24, const BYTE *pbRgb, DWORD dwKeyColor,
	BYTE (WINAPI *pfnQuantize)(BYTE bColor0, BYTE bColor1, BYTE bColor2), int *pnSize)
{
	const BYTE *pbKey;
	BYTE *ptData8;
	BYTE *pbOut;
	int nSize;
	int i;
	BYTE bTemp;

	nSize = (int)ptComp24->wCompLength / 3;
#line 216
	ptData8 = (BYTE *)D2ALLOC2(nSize);
#line 217
	D2ASSERT(ptData8);
	memset(ptData8, 0, nSize);
	pbKey = (const BYTE *)&dwKeyColor;
	pbOut = ptData8;
	for (i = 0; i < nSize; pbRgb += 3, i++)
	{
		if (pbRgb[0] == pbKey[0] && pbRgb[1] == pbKey[1] && pbRgb[2] == pbKey[2])
			bTemp = 0;
		else
		{
			bTemp = pfnQuantize(pbRgb[0], pbRgb[1], pbRgb[2]);
#line 228
			D2ASSERT(bTemp);
		}
		*pbOut++ = bTemp;
	}
	*pnSize = nSize;
	return ptData8;
}

static __inline BOOL SubTileIsEmpty(const BYTE *pbData, int nSize)
{
	int i;

	for (i = 0; i < nSize; pbData++, i++)
	{
		if (*pbData)
			return FALSE;
	}
	return TRUE;
}

static __inline void SubTileSetPixels(D2TILECOMP *ptComp8, BYTE *ptData8)
{
	if (SubTileIsEmpty(ptData8, 512))
	{
		ptComp8->wFlags = 0;
		ptComp8->wCompLength = 0;
		ptComp8->ptData = NULL;
#line 440
		D2FREE2(ptData8);
		return;
	}
	if (SubTileDiamondHasHole(ptData8))
		SubTileEncodeDiamond(ptComp8, ptData8);
	else
		SubTileSquish(ptComp8, ptData8);
#line 455
	D2FREE2(ptData8);
}

void __fastcall SubTileConvertTile(D2TILE *ptTile24, D2TILE *ptTile8, DWORD dwKeyColor,
	BYTE (WINAPI *pfnQuantize)(BYTE bColor0, BYTE bColor1, BYTE bColor2))
{
	D2TILECOMP *ptComp8;
	const D2TILECOMP *ptComp24;
	const BYTE *pbRgb;
	BYTE *ptData8;
	int nSize;
	int i;

#line 492
	D2ASSERT(ptTile24);
#line 493
	D2ASSERT(ptTile8);
#line 494
	D2ASSERT(pfnQuantize);
#line 495
	D2ASSERT(ptTile24->bTypeFlag & TILETYPEFLAG_GAMESQAURE24);
	for (i = 0; i < ptTile24->nCompCount; i++)
	{
		ptComp8 = &ptTile8->ptCompArray[i];
		ptComp24 = &ptTile24->ptCompArray[i];
		*ptComp8 = *ptComp24;
		ptComp8->ptData = NULL;
		pbRgb = ptComp24->ptData;
		if (!pbRgb)
			continue;
		ptData8 = SubTileQuantize(ptComp24, pbRgb, dwKeyColor, pfnQuantize, &nSize);
		SubTileSetPixels(ptComp8, ptData8);
		if (ptComp8->wFlags & 4)
			ptTile8->bTypeFlag |= TILETYPEFLAG_SQUISHED;
	}
}

void __fastcall SubTileForEachPixel(const D2TILECOMP *ptComp, void (WINAPI *pfnPixel)(DWORD))
{
	const BYTE *pbPixel;
	int i;

	pbPixel = ptComp->ptData;
	for (i = 1024; i; i--)
	{
		pfnPixel(((((DWORD)pbPixel[2] << 8) + pbPixel[1]) << 8) + pbPixel[0]);
		pbPixel += 3;
	}
}
