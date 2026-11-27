#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\FastCmp.cpp"
#include <math.h>
#include "D2Cmp.h"
#include "D2Cmp.h"
#include "D2Macros.h"
#include "CelList.h"
#line 14

static int sgnWarpStart[80];
static int sgnWarpEnd[80];
static int sgnWarpOffset[80 * 80];

void WINAPI DrawContextSet(D2DRAWCONTEXT *ptContext, BYTE bMask, DWORD dwValue0, DWORD dwValue1,
	DWORD dwValue2, DWORD dwValue3, DWORD dwValue4, DWORD dwValue5, DWORD dwValue6)
{
	if (bMask & 1)
		ptContext->field_04[0] = dwValue0;
	if (bMask & 2)
		ptContext->field_04[1] = dwValue1;
	if (bMask & 4)
		ptContext->field_04[2] = dwValue2;
	if (bMask & 8)
		ptContext->field_04[3] = dwValue3;
	if (bMask & 16)
		ptContext->field_04[4] = dwValue4;
	if (bMask & 32)
		ptContext->field_04[5] = dwValue5;
	if (bMask & 64)
		ptContext->field_04[6] = dwValue6;
}

static void __fastcall FastCmpEncode(const BYTE *pbPixels, DWORD dwWidth, DWORD dwHeight, DWORD dwFlags,
	BYTE **ppbOut, DWORD *pdwSize)
{
	const BYTE *pbRowEnd;
	const BYTE *pbNextRow;
	BYTE *pbOut;
	BYTE *pbStart;
	int nStep;
	DWORD nCount;
	int nAtEnd;
	int nRestSame;
	DWORD dwRow;

#line 37
	pbOut = (BYTE *)D2ALLOC2((2 * dwWidth + 1) * dwHeight);
	pbStart = pbOut;
	dwRow = 0;
	if (dwFlags & 1)
		nStep = dwWidth;
	else
	{
		pbPixels += (dwHeight - 1) * dwWidth;
		nStep = -(int)dwWidth;
	}
	pbNextRow = pbPixels + nStep;
	pbRowEnd = pbPixels + dwWidth;
	while (dwRow < dwHeight)
	{
		if (!*pbPixels)
		{
			nCount = CountSame(pbPixels, pbRowEnd, 127, &nAtEnd, &nRestSame);
			if (!nRestSame && !nAtEnd)
			{
				*pbOut++ = (BYTE)(nCount | 128);
				pbPixels += nCount;
			}
			else
			{
				*pbOut++ = 128;
				pbPixels = pbNextRow;
				pbNextRow += nStep;
				pbRowEnd += nStep;
				dwRow++;
			}
		}
		else
		{
			nCount = CountUntil(pbPixels, pbRowEnd, 0, 127, &nAtEnd, &nRestSame);
			*pbOut++ = (BYTE)nCount;
			for (; nCount > 0; nCount--)
				*pbOut++ = *pbPixels++;
			if (nAtEnd)
			{
				*pbOut++ = 128;
				pbPixels = pbNextRow;
				pbNextRow += nStep;
				pbRowEnd += nStep;
				dwRow++;
			}
		}
	}
	*ppbOut = pbStart;
	*pdwSize = pbOut - pbStart;
}

void __fastcall FastCmpBuildCelFile(CELLIST *pList, D2CELFILE *pFile)
{
	TCELENTRY *ptEntry;
	D2CEL *pCel;
	BYTE *pbEncoded;
	DWORD dwEncoded;
	DWORD dwFrames;
	DWORD dwDirection;
	DWORD i;

#line 114
	D2ASSERT(!( pList->dwFlags & CELFILE_24BIT ));
	i = 0;
	dwFrames = pList->nCels / pList->dwNumDirections;
	for (ptEntry = pList->tEntries.Head(); (INT_PTR)ptEntry > 0;
		i++, ptEntry = pList->tEntries.LinkNext(ptEntry))
	{
		FastCmpEncode(ptEntry->field_20, ptEntry->field_0c, ptEntry->field_10, ptEntry->field_08, &pbEncoded,
			&dwEncoded);
#line 130
		pCel = (D2CEL *)D2ALLOC2(CELSIZE(dwEncoded));
		pCel->dwFlags = ptEntry->field_08;
		pCel->dwWidth = ptEntry->field_0c;
		pCel->dwHeight = ptEntry->field_10;
		pCel->nOffsetX = ptEntry->field_18;
		pCel->nOffsetY = ptEntry->field_1c;
		pCel->dwExtraSize = 0;
		pCel->ptSlot = NULL;
		pCel->dwDataSize = dwEncoded;
		memcpy(pCel->abData, pbEncoded, dwEncoded);
#line 141
		D2FREE2(pbEncoded);
		if (pList->dwNumDirections > 1)
			dwDirection = i / dwFrames * 64 / pList->dwNumDirections;
		else
			dwDirection = 1;
		pFile->aptCels[CelGetGameDirection(dwDirection,
			pList->dwNumDirections) * dwFrames + i % dwFrames] = pCel;
	}
}

void __fastcall FastCmpCelRemapColors(D2CEL *pCel, const BYTE *pbMap)
{
	BYTE *pbData;
	DWORD dwRows;
	int nCount;
	BYTE bControl;

	dwRows = pCel->dwHeight;
	pbData = pCel->abData;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			if (bControl & 127)
				continue;
			if (!--dwRows)
				return;
			continue;
		}
		for (nCount = (signed char)bControl; nCount > 0; nCount--)
		{
			*pbData = pbMap[*pbData];
			pbData++;
		}
	}
}

void __fastcall FastCmpConvert24Bit(D2CELFILE *pSrc, D2CELFILE *pDst,
	BYTE (WINAPI *pfnMatch)(BYTE bColor0, BYTE bColor1, BYTE bColor2), DWORD dwKeyColor)
{
	D2CEL *pNewCel;
	const BYTE *pbRgb;
	BYTE *pbIndexed;
	BYTE *pbOut;
	BYTE *pbEncoded;
	DWORD dwEncoded;
	DWORD dwPixels;
	DWORD i, k;
	BYTE bTemp;

#line 206
	D2ASSERT(pSrc->dwFlags & CELFILE_24BIT);
	for (i = 0; i < pSrc->dwNumDirections * pSrc->dwCelsPerDirection; i++)
	{
		dwPixels = pSrc->aptCels[i]->dwHeight * pSrc->aptCels[i]->dwWidth;
#line 212
		pbIndexed = (BYTE *)D2ALLOC2(dwPixels);
		pbRgb = pSrc->aptCels[i]->abData;
		pbOut = pbIndexed;
		for (k = 0; k < dwPixels; k++, pbRgb += 3)
		{
			if (pbRgb[0] == ((BYTE *)&dwKeyColor)[0] && pbRgb[1] == ((BYTE *)&dwKeyColor)[1] &&
				pbRgb[2] == ((BYTE *)&dwKeyColor)[2])
			{
				*pbOut++ = 0;
				continue;
			}
			bTemp = pfnMatch(pbRgb[0], pbRgb[1], pbRgb[2]);
#line 224
			D2ASSERT(bTemp);
			*pbOut++ = bTemp;
		}
		FastCmpEncode(pbIndexed, pSrc->aptCels[i]->dwWidth, pSrc->aptCels[i]->dwHeight,
			pSrc->aptCels[i]->dwFlags, &pbEncoded, &dwEncoded);
#line 240
		D2FREE2(pbIndexed);
#line 242
		pNewCel = (D2CEL *)D2ALLOC2(CELSIZE(dwEncoded));
		pNewCel->dwFlags = pSrc->aptCels[i]->dwFlags;
		pNewCel->dwWidth = pSrc->aptCels[i]->dwWidth;
		pNewCel->dwHeight = pSrc->aptCels[i]->dwHeight;
		pNewCel->nOffsetX = pSrc->aptCels[i]->nOffsetX;
		pNewCel->nOffsetY = pSrc->aptCels[i]->nOffsetY;
		pNewCel->dwExtraSize = 0;
		pNewCel->ptSlot = NULL;
		pNewCel->dwDataSize = dwEncoded;
		memcpy(pNewCel->abData, pbEncoded, dwEncoded);
#line 253
		D2FREE2(pbEncoded);
		pDst->aptCels[i] = pNewCel;
	}
}

static BOOL sgfDecodeGeneral;

const BYTE * __fastcall FastCmpSkipLines(const BYTE *pbData, UINT nLines)
{
	BYTE bControl;

	if (!nLines)
		return pbData;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			if (!(bControl & 127) && !--nLines)
				break;
		}
		else
			pbData += (signed char)bControl;
	}
	return pbData;
}

void __fastcall FastCmpDecodeCopy(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride)
{
	BYTE *pbNextRow;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		memcpy(pbDst, pbData, (signed char)bControl);
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodeCopyClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride, int nX,
	int nClipLeft, int nClipRight)
{
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		if (nRight - nLeft > 0)
			memcpy(pbDst + nLeft, pbData + nLeft, nRight - nLeft);
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodeMapBlend(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	const BYTE *pbColorMap, const BYTE *pbBlend)
{
	BYTE *pbNextRow;
	int nCount;
	int nGroups;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		nCount = (signed char)bControl;
		nGroups = nCount >> 3;
		switch (nCount & 7)
		{
		case 7:
			pbDst[6] = pbBlend[(pbDst[6] << 8) + pbColorMap[pbData[6]]];
		case 6:
			pbDst[5] = pbBlend[(pbDst[5] << 8) + pbColorMap[pbData[5]]];
		case 5:
			pbDst[4] = pbBlend[(pbDst[4] << 8) + pbColorMap[pbData[4]]];
		case 4:
			pbDst[3] = pbBlend[(pbDst[3] << 8) + pbColorMap[pbData[3]]];
		case 3:
			pbDst[2] = pbBlend[(pbDst[2] << 8) + pbColorMap[pbData[2]]];
		case 2:
			pbDst[1] = pbBlend[(pbDst[1] << 8) + pbColorMap[pbData[1]]];
		case 1:
			pbDst[0] = pbBlend[(pbDst[0] << 8) + pbColorMap[pbData[0]]];
		case 0:
			pbDst += nCount & 7;
			pbData += nCount & 7;
			for (; nGroups; nGroups--)
			{
				pbDst[7] = pbBlend[(pbDst[7] << 8) + pbColorMap[pbData[7]]];
				pbDst[6] = pbBlend[(pbDst[6] << 8) + pbColorMap[pbData[6]]];
				pbDst[5] = pbBlend[(pbDst[5] << 8) + pbColorMap[pbData[5]]];
				pbDst[4] = pbBlend[(pbDst[4] << 8) + pbColorMap[pbData[4]]];
				pbDst[3] = pbBlend[(pbDst[3] << 8) + pbColorMap[pbData[3]]];
				pbDst[2] = pbBlend[(pbDst[2] << 8) + pbColorMap[pbData[2]]];
				pbDst[1] = pbBlend[(pbDst[1] << 8) + pbColorMap[pbData[1]]];
				pbDst[0] = pbBlend[(pbDst[0] << 8) + pbColorMap[pbData[0]]];
				pbDst += 8;
				pbData += 8;
			}
		}
	}
}

void __fastcall FastCmpDecodeMapBlendClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	int nX, int nClipLeft, int nClipRight, const BYTE *pbColorMap, const BYTE *pbBlend)
{
	const BYTE *pbRowData;
	BYTE *pbRowDst;
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	int nGroups;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		nRight -= nLeft;
		if (nRight > 0)
		{
			nGroups = nRight >> 3;
			pbRowData = pbData;
			pbRowDst = pbDst;
			pbData += nLeft;
			pbDst += nLeft;
			switch (nRight & 7)
			{
			case 7:
				pbDst[6] = pbBlend[(pbDst[6] << 8) + pbColorMap[pbData[6]]];
			case 6:
				pbDst[5] = pbBlend[(pbDst[5] << 8) + pbColorMap[pbData[5]]];
			case 5:
				pbDst[4] = pbBlend[(pbDst[4] << 8) + pbColorMap[pbData[4]]];
			case 4:
				pbDst[3] = pbBlend[(pbDst[3] << 8) + pbColorMap[pbData[3]]];
			case 3:
				pbDst[2] = pbBlend[(pbDst[2] << 8) + pbColorMap[pbData[2]]];
			case 2:
				pbDst[1] = pbBlend[(pbDst[1] << 8) + pbColorMap[pbData[1]]];
			case 1:
				pbDst[0] = pbBlend[(pbDst[0] << 8) + pbColorMap[pbData[0]]];
			case 0:
				pbDst += nRight & 7;
				pbData += nRight & 7;
				while (nGroups--)
				{
					pbDst[7] = pbBlend[(pbDst[7] << 8) + pbColorMap[pbData[7]]];
					pbDst[6] = pbBlend[(pbDst[6] << 8) + pbColorMap[pbData[6]]];
					pbDst[5] = pbBlend[(pbDst[5] << 8) + pbColorMap[pbData[5]]];
					pbDst[4] = pbBlend[(pbDst[4] << 8) + pbColorMap[pbData[4]]];
					pbDst[3] = pbBlend[(pbDst[3] << 8) + pbColorMap[pbData[3]]];
					pbDst[2] = pbBlend[(pbDst[2] << 8) + pbColorMap[pbData[2]]];
					pbDst[1] = pbBlend[(pbDst[1] << 8) + pbColorMap[pbData[1]]];
					pbDst[0] = pbBlend[(pbDst[0] << 8) + pbColorMap[pbData[0]]];
					pbDst += 8;
					pbData += 8;
				}
			}
			nX += (signed char)bControl;
			pbData = pbRowData + (signed char)bControl;
			pbDst = pbRowDst + (signed char)bControl;
			continue;
		}
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodeMapPreMap(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	const BYTE *pbColorMap, const BYTE *pbPreMap)
{
	BYTE *pbNextRow;
	int nCount;
	int nGroups;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		nCount = (signed char)bControl;
		nGroups = nCount >> 3;
		switch (nCount & 7)
		{
		case 7:
			pbDst[6] = pbColorMap[pbPreMap[pbData[6]]];
		case 6:
			pbDst[5] = pbColorMap[pbPreMap[pbData[5]]];
		case 5:
			pbDst[4] = pbColorMap[pbPreMap[pbData[4]]];
		case 4:
			pbDst[3] = pbColorMap[pbPreMap[pbData[3]]];
		case 3:
			pbDst[2] = pbColorMap[pbPreMap[pbData[2]]];
		case 2:
			pbDst[1] = pbColorMap[pbPreMap[pbData[1]]];
		case 1:
			pbDst[0] = pbColorMap[pbPreMap[pbData[0]]];
		case 0:
			pbDst += nCount & 7;
			pbData += nCount & 7;
			for (; nGroups; nGroups--)
			{
				pbDst[7] = pbColorMap[pbPreMap[pbData[7]]];
				pbDst[6] = pbColorMap[pbPreMap[pbData[6]]];
				pbDst[5] = pbColorMap[pbPreMap[pbData[5]]];
				pbDst[4] = pbColorMap[pbPreMap[pbData[4]]];
				pbDst[3] = pbColorMap[pbPreMap[pbData[3]]];
				pbDst[2] = pbColorMap[pbPreMap[pbData[2]]];
				pbDst[1] = pbColorMap[pbPreMap[pbData[1]]];
				pbDst[0] = pbColorMap[pbPreMap[pbData[0]]];
				pbDst += 8;
				pbData += 8;
			}
		}
	}
}

void __fastcall FastCmpDecodeMapPreMapClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	int nX, int nClipLeft, int nClipRight, const BYTE *pbColorMap, const BYTE *pbPreMap)
{
	const BYTE *pbRowData;
	BYTE *pbRowDst;
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	int nGroups;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		nRight -= nLeft;
		if (nRight > 0)
		{
			nGroups = nRight >> 3;
			pbRowData = pbData;
			pbRowDst = pbDst;
			pbData += nLeft;
			pbDst += nLeft;
			switch (nRight & 7)
			{
			case 7:
				pbDst[6] = pbColorMap[pbPreMap[pbData[6]]];
			case 6:
				pbDst[5] = pbColorMap[pbPreMap[pbData[5]]];
			case 5:
				pbDst[4] = pbColorMap[pbPreMap[pbData[4]]];
			case 4:
				pbDst[3] = pbColorMap[pbPreMap[pbData[3]]];
			case 3:
				pbDst[2] = pbColorMap[pbPreMap[pbData[2]]];
			case 2:
				pbDst[1] = pbColorMap[pbPreMap[pbData[1]]];
			case 1:
				pbDst[0] = pbColorMap[pbPreMap[pbData[0]]];
			case 0:
				pbDst += nRight & 7;
				pbData += nRight & 7;
				for (; nGroups; nGroups--)
				{
					pbDst[7] = pbColorMap[pbPreMap[pbData[7]]];
					pbDst[6] = pbColorMap[pbPreMap[pbData[6]]];
					pbDst[5] = pbColorMap[pbPreMap[pbData[5]]];
					pbDst[4] = pbColorMap[pbPreMap[pbData[4]]];
					pbDst[3] = pbColorMap[pbPreMap[pbData[3]]];
					pbDst[2] = pbColorMap[pbPreMap[pbData[2]]];
					pbDst[1] = pbColorMap[pbPreMap[pbData[1]]];
					pbDst[0] = pbColorMap[pbPreMap[pbData[0]]];
					pbDst += 8;
					pbData += 8;
				}
			}
			nX += (signed char)bControl;
			pbData = pbRowData + (signed char)bControl;
			pbDst = pbRowDst + (signed char)bControl;
			continue;
		}
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodeMap(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	const BYTE *pbColorMap)
{
	BYTE *pbNextRow;
	int nCount;
	int nGroups;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		nCount = (signed char)bControl;
		nGroups = nCount >> 3;
		switch (nCount & 7)
		{
		case 7:
			pbDst[6] = pbColorMap[pbData[6]];
		case 6:
			pbDst[5] = pbColorMap[pbData[5]];
		case 5:
			pbDst[4] = pbColorMap[pbData[4]];
		case 4:
			pbDst[3] = pbColorMap[pbData[3]];
		case 3:
			pbDst[2] = pbColorMap[pbData[2]];
		case 2:
			pbDst[1] = pbColorMap[pbData[1]];
		case 1:
			pbDst[0] = pbColorMap[pbData[0]];
		case 0:
			pbDst += nCount & 7;
			pbData += nCount & 7;
			for (; nGroups; nGroups--)
			{
				pbDst[7] = pbColorMap[pbData[7]];
				pbDst[6] = pbColorMap[pbData[6]];
				pbDst[5] = pbColorMap[pbData[5]];
				pbDst[4] = pbColorMap[pbData[4]];
				pbDst[3] = pbColorMap[pbData[3]];
				pbDst[2] = pbColorMap[pbData[2]];
				pbDst[1] = pbColorMap[pbData[1]];
				pbDst[0] = pbColorMap[pbData[0]];
				pbDst += 8;
				pbData += 8;
			}
		}
	}
}

void __fastcall FastCmpDecodeMapClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	int nX, int nClipLeft, int nClipRight, const BYTE *pbColorMap)
{
	const BYTE *pbRowData;
	BYTE *pbRowDst;
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	int nGroups;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		nRight -= nLeft;
		if (nRight > 0)
		{
			nGroups = nRight >> 3;
			pbRowData = pbData;
			pbRowDst = pbDst;
			pbData += nLeft;
			pbDst += nLeft;
			switch (nRight & 7)
			{
			case 7:
				pbDst[6] = pbColorMap[pbData[6]];
			case 6:
				pbDst[5] = pbColorMap[pbData[5]];
			case 5:
				pbDst[4] = pbColorMap[pbData[4]];
			case 4:
				pbDst[3] = pbColorMap[pbData[3]];
			case 3:
				pbDst[2] = pbColorMap[pbData[2]];
			case 2:
				pbDst[1] = pbColorMap[pbData[1]];
			case 1:
				pbDst[0] = pbColorMap[pbData[0]];
			case 0:
				pbDst += nRight & 7;
				pbData += nRight & 7;
				for (; nGroups; nGroups--)
				{
					pbDst[7] = pbColorMap[pbData[7]];
					pbDst[6] = pbColorMap[pbData[6]];
					pbDst[5] = pbColorMap[pbData[5]];
					pbDst[4] = pbColorMap[pbData[4]];
					pbDst[3] = pbColorMap[pbData[3]];
					pbDst[2] = pbColorMap[pbData[2]];
					pbDst[1] = pbColorMap[pbData[1]];
					pbDst[0] = pbColorMap[pbData[0]];
					pbDst += 8;
					pbData += 8;
				}
			}
			nX += (signed char)bControl;
			pbData = pbRowData + (signed char)bControl;
			pbDst = pbRowDst + (signed char)bControl;
			continue;
		}
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodePreMapBlend(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	const BYTE *pbPreMap, const BYTE *pbBlend)
{
	BYTE *pbNextRow;
	int nCount;
	int nGroups;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		nCount = (signed char)bControl;
		nGroups = nCount >> 3;
		switch (nCount & 7)
		{
		case 7:
			pbDst[6] = pbBlend[(pbDst[6] << 8) + pbPreMap[pbData[6]]];
		case 6:
			pbDst[5] = pbBlend[(pbDst[5] << 8) + pbPreMap[pbData[5]]];
		case 5:
			pbDst[4] = pbBlend[(pbDst[4] << 8) + pbPreMap[pbData[4]]];
		case 4:
			pbDst[3] = pbBlend[(pbDst[3] << 8) + pbPreMap[pbData[3]]];
		case 3:
			pbDst[2] = pbBlend[(pbDst[2] << 8) + pbPreMap[pbData[2]]];
		case 2:
			pbDst[1] = pbBlend[(pbDst[1] << 8) + pbPreMap[pbData[1]]];
		case 1:
			pbDst[0] = pbBlend[(pbDst[0] << 8) + pbPreMap[pbData[0]]];
		case 0:
			pbDst += nCount & 7;
			pbData += nCount & 7;
			for (; nGroups; nGroups--)
			{
				pbDst[7] = pbBlend[(pbDst[7] << 8) + pbPreMap[pbData[7]]];
				pbDst[6] = pbBlend[(pbDst[6] << 8) + pbPreMap[pbData[6]]];
				pbDst[5] = pbBlend[(pbDst[5] << 8) + pbPreMap[pbData[5]]];
				pbDst[4] = pbBlend[(pbDst[4] << 8) + pbPreMap[pbData[4]]];
				pbDst[3] = pbBlend[(pbDst[3] << 8) + pbPreMap[pbData[3]]];
				pbDst[2] = pbBlend[(pbDst[2] << 8) + pbPreMap[pbData[2]]];
				pbDst[1] = pbBlend[(pbDst[1] << 8) + pbPreMap[pbData[1]]];
				pbDst[0] = pbBlend[(pbDst[0] << 8) + pbPreMap[pbData[0]]];
				pbDst += 8;
				pbData += 8;
			}
		}
	}
}

void __fastcall FastCmpDecodePreMapBlendClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	int nX, int nClipLeft, int nClipRight, const BYTE *pbPreMap, const BYTE *pbBlend)
{
	const BYTE *pbRowData;
	BYTE *pbRowDst;
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	int nGroups;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		nRight -= nLeft;
		if (nRight > 0)
		{
			nGroups = nRight >> 3;
			pbRowData = pbData;
			pbRowDst = pbDst;
			pbData += nLeft;
			pbDst += nLeft;
			switch (nRight & 7)
			{
			case 7:
				pbDst[6] = pbBlend[(pbDst[6] << 8) + pbPreMap[pbData[6]]];
			case 6:
				pbDst[5] = pbBlend[(pbDst[5] << 8) + pbPreMap[pbData[5]]];
			case 5:
				pbDst[4] = pbBlend[(pbDst[4] << 8) + pbPreMap[pbData[4]]];
			case 4:
				pbDst[3] = pbBlend[(pbDst[3] << 8) + pbPreMap[pbData[3]]];
			case 3:
				pbDst[2] = pbBlend[(pbDst[2] << 8) + pbPreMap[pbData[2]]];
			case 2:
				pbDst[1] = pbBlend[(pbDst[1] << 8) + pbPreMap[pbData[1]]];
			case 1:
				pbDst[0] = pbBlend[(pbDst[0] << 8) + pbPreMap[pbData[0]]];
			case 0:
				pbDst += nRight & 7;
				pbData += nRight & 7;
				for (; nGroups; nGroups--)
				{
					pbDst[7] = pbBlend[(pbDst[7] << 8) + pbPreMap[pbData[7]]];
					pbDst[6] = pbBlend[(pbDst[6] << 8) + pbPreMap[pbData[6]]];
					pbDst[5] = pbBlend[(pbDst[5] << 8) + pbPreMap[pbData[5]]];
					pbDst[4] = pbBlend[(pbDst[4] << 8) + pbPreMap[pbData[4]]];
					pbDst[3] = pbBlend[(pbDst[3] << 8) + pbPreMap[pbData[3]]];
					pbDst[2] = pbBlend[(pbDst[2] << 8) + pbPreMap[pbData[2]]];
					pbDst[1] = pbBlend[(pbDst[1] << 8) + pbPreMap[pbData[1]]];
					pbDst[0] = pbBlend[(pbDst[0] << 8) + pbPreMap[pbData[0]]];
					pbDst += 8;
					pbData += 8;
				}
			}
			nX += (signed char)bControl;
			pbData = pbRowData + (signed char)bControl;
			pbDst = pbRowDst + (signed char)bControl;
			continue;
		}
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodePreMap(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	const BYTE *pbPreMap)
{
	BYTE *pbNextRow;
	int nCount;
	int nGroups;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		nCount = (signed char)bControl;
		nGroups = nCount >> 3;
		switch (nCount & 7)
		{
		case 7:
			pbDst[6] = pbPreMap[pbData[6]];
		case 6:
			pbDst[5] = pbPreMap[pbData[5]];
		case 5:
			pbDst[4] = pbPreMap[pbData[4]];
		case 4:
			pbDst[3] = pbPreMap[pbData[3]];
		case 3:
			pbDst[2] = pbPreMap[pbData[2]];
		case 2:
			pbDst[1] = pbPreMap[pbData[1]];
		case 1:
			pbDst[0] = pbPreMap[pbData[0]];
		case 0:
			pbDst += nCount & 7;
			pbData += nCount & 7;
			for (; nGroups; nGroups--)
			{
				pbDst[7] = pbPreMap[pbData[7]];
				pbDst[6] = pbPreMap[pbData[6]];
				pbDst[5] = pbPreMap[pbData[5]];
				pbDst[4] = pbPreMap[pbData[4]];
				pbDst[3] = pbPreMap[pbData[3]];
				pbDst[2] = pbPreMap[pbData[2]];
				pbDst[1] = pbPreMap[pbData[1]];
				pbDst[0] = pbPreMap[pbData[0]];
				pbDst += 8;
				pbData += 8;
			}
		}
	}
}

void __fastcall FastCmpDecodePreMapClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	int nX, int nClipLeft, int nClipRight, const BYTE *pbPreMap)
{
	const BYTE *pbRowData;
	BYTE *pbRowDst;
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	int nGroups;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		nRight -= nLeft;
		if (nRight > 0)
		{
			nGroups = nRight >> 3;
			pbRowData = pbData;
			pbRowDst = pbDst;
			pbData += nLeft;
			pbDst += nLeft;
			switch (nRight & 7)
			{
			case 7:
				pbDst[6] = pbPreMap[pbData[6]];
			case 6:
				pbDst[5] = pbPreMap[pbData[5]];
			case 5:
				pbDst[4] = pbPreMap[pbData[4]];
			case 4:
				pbDst[3] = pbPreMap[pbData[3]];
			case 3:
				pbDst[2] = pbPreMap[pbData[2]];
			case 2:
				pbDst[1] = pbPreMap[pbData[1]];
			case 1:
				pbDst[0] = pbPreMap[pbData[0]];
			case 0:
				pbDst += nRight & 7;
				pbData += nRight & 7;
				for (; nGroups; nGroups--)
				{
					pbDst[7] = pbPreMap[pbData[7]];
					pbDst[6] = pbPreMap[pbData[6]];
					pbDst[5] = pbPreMap[pbData[5]];
					pbDst[4] = pbPreMap[pbData[4]];
					pbDst[3] = pbPreMap[pbData[3]];
					pbDst[2] = pbPreMap[pbData[2]];
					pbDst[1] = pbPreMap[pbData[1]];
					pbDst[0] = pbPreMap[pbData[0]];
					pbDst += 8;
					pbData += 8;
				}
			}
			nX += (signed char)bControl;
			pbData = pbRowData + (signed char)bControl;
			pbDst = pbRowDst + (signed char)bControl;
			continue;
		}
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpDecodeBlend(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	const BYTE *pbBlend)
{
	BYTE *pbNextRow;
	int nCount;
	int nGroups;
	BYTE bControl;

	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
				pbDst += (signed char)bControl;
			continue;
		}
		nCount = (signed char)bControl;
		nGroups = nCount >> 3;
		switch (nCount & 7)
		{
		case 7:
			pbDst[6] = pbBlend[(pbDst[6] << 8) + pbData[6]];
		case 6:
			pbDst[5] = pbBlend[(pbDst[5] << 8) + pbData[5]];
		case 5:
			pbDst[4] = pbBlend[(pbDst[4] << 8) + pbData[4]];
		case 4:
			pbDst[3] = pbBlend[(pbDst[3] << 8) + pbData[3]];
		case 3:
			pbDst[2] = pbBlend[(pbDst[2] << 8) + pbData[2]];
		case 2:
			pbDst[1] = pbBlend[(pbDst[1] << 8) + pbData[1]];
		case 1:
			pbDst[0] = pbBlend[(pbDst[0] << 8) + pbData[0]];
		case 0:
			pbDst += nCount & 7;
			pbData += nCount & 7;
			for (; nGroups; nGroups--)
			{
				pbDst[7] = pbBlend[(pbDst[7] << 8) + pbData[7]];
				pbDst[6] = pbBlend[(pbDst[6] << 8) + pbData[6]];
				pbDst[5] = pbBlend[(pbDst[5] << 8) + pbData[5]];
				pbDst[4] = pbBlend[(pbDst[4] << 8) + pbData[4]];
				pbDst[3] = pbBlend[(pbDst[3] << 8) + pbData[3]];
				pbDst[2] = pbBlend[(pbDst[2] << 8) + pbData[2]];
				pbDst[1] = pbBlend[(pbDst[1] << 8) + pbData[1]];
				pbDst[0] = pbBlend[(pbDst[0] << 8) + pbData[0]];
				pbDst += 8;
				pbData += 8;
			}
		}
	}
}

void __fastcall FastCmpDecodeBlendClip(const BYTE *pbData, BYTE *pbDst, UINT nLinesToDraw, int nStride,
	int nX, int nClipLeft, int nClipRight, const BYTE *pbBlend)
{
	const BYTE *pbRowData;
	BYTE *pbRowDst;
	BYTE *pbNextRow;
	int nRowX;
	int nLeft, nRight;
	int nGroups;
	BYTE bControl;

	nRowX = nX;
	pbNextRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				if (!--nLinesToDraw)
					return;
				nX = nRowX;
				pbDst = pbNextRow;
				pbNextRow += nStride;
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nLeft = nClipLeft - nX;
		if (nLeft < 0)
			nLeft = 0;
		nRight = nClipRight - nX;
		if (nRight > (signed char)bControl)
			nRight = (signed char)bControl;
		nRight -= nLeft;
		if (nRight > 0)
		{
			nGroups = nRight >> 3;
			pbRowData = pbData;
			pbRowDst = pbDst;
			pbData += nLeft;
			pbDst += nLeft;
			switch (nRight & 7)
			{
			case 7:
				pbDst[6] = pbBlend[(pbDst[6] << 8) + pbData[6]];
			case 6:
				pbDst[5] = pbBlend[(pbDst[5] << 8) + pbData[5]];
			case 5:
				pbDst[4] = pbBlend[(pbDst[4] << 8) + pbData[4]];
			case 4:
				pbDst[3] = pbBlend[(pbDst[3] << 8) + pbData[3]];
			case 3:
				pbDst[2] = pbBlend[(pbDst[2] << 8) + pbData[2]];
			case 2:
				pbDst[1] = pbBlend[(pbDst[1] << 8) + pbData[1]];
			case 1:
				pbDst[0] = pbBlend[(pbDst[0] << 8) + pbData[0]];
			case 0:
				if (nGroups)
				{
					pbDst += nRight & 7;
					pbData += nRight & 7;
					do
					{
						pbDst[7] = pbBlend[(pbDst[7] << 8) + pbData[7]];
						pbDst[6] = pbBlend[(pbDst[6] << 8) + pbData[6]];
						pbDst[5] = pbBlend[(pbDst[5] << 8) + pbData[5]];
						pbDst[4] = pbBlend[(pbDst[4] << 8) + pbData[4]];
						pbDst[3] = pbBlend[(pbDst[3] << 8) + pbData[3]];
						pbDst[2] = pbBlend[(pbDst[2] << 8) + pbData[2]];
						pbDst[1] = pbBlend[(pbDst[1] << 8) + pbData[1]];
						pbDst[0] = pbBlend[(pbDst[0] << 8) + pbData[0]];
						--nGroups;
						pbDst += 8;
						pbData += 8;
					} while (nGroups);
				}
			}
			nX += (signed char)bControl;
			pbData = pbRowData + (signed char)bControl;
			pbDst = pbRowDst + (signed char)bControl;
			continue;
		}
		nX += (signed char)bControl;
		pbData += (signed char)bControl;
		pbDst += (signed char)bControl;
	}
}

void __fastcall FastCmpToggleDecode(void)
{
	sgfDecodeGeneral = !sgfDecodeGeneral;
}

void __fastcall FastCmpDecode(D2CEL *pCel, BYTE *pbDst, int nX, int nClipLeft, int nClipRight, int dwStride,
	UINT nLinesToSkip, UINT nLinesToDraw, const BYTE *pbColorMap, const BYTE *pbBlend, const BYTE *pbPreMap)
{
	const BYTE *pbData;
	const BYTE *pbNextData;
	const BYTE *pbEnd;
	BYTE *pbNextRow;
	BYTE *pbNext;
	int nRowX;
	int nStride;
	int nCount;
	int nLeft, nRight, nDraw;
	int nVisible;
	BOOL fInside;
	char cControl;

	nRowX = nX;
#line 1514
	D2ASSERT(dwStride >= pCel->dwWidth);
#line 1515
	D2ASSERT(nLinesToSkip + nLinesToDraw <= (int)pCel->dwHeight);
	pbData = FastCmpSkipLines(pCel->abData, nLinesToSkip);
	nStride = pCel->dwFlags & 1 ? dwStride : -dwStride;
	if (sgfDecodeGeneral)
	{
		pbNextRow = pbDst + nStride;
		for (;;)
		{
			cControl = (char)*pbData++;
			if (cControl < 0)
			{
				cControl &= 127;
				if (!cControl)
				{
					nX = nRowX;
					pbDst = pbNextRow;
					pbNextRow += nStride;
					if (!--nLinesToDraw)
						return;
					continue;
				}
				nX += cControl;
				pbDst += cControl;
				continue;
			}
			nCount = cControl;
			pbNextData = pbData + nCount;
			pbNext = pbDst + nCount;
			nDraw = nCount;
			if (nClipLeft || nClipRight)
			{
				nLeft = nClipLeft - nX;
				if (nLeft <= 0)
					nLeft = 0;
				nRight = nClipRight - nX;
				nRight = nRight > 0 ? nRight : 0;
				if (nRight >= nCount)
					nRight = nCount;
				nVisible = nRight - nLeft;
				if (nVisible > 0)
					nDraw = nVisible;
				else
					nDraw = 0;
				if (nDraw > 0)
				{
					pbDst += nLeft;
					pbData += nLeft;
				}
			}
			pbEnd = pbData + nDraw;
			if (pbColorMap)
			{
				if (pbBlend)
				{
					for (; pbData < pbEnd; pbData++, pbDst++)
						*pbDst = pbBlend[(*pbDst << 8) + pbColorMap[*pbData]];
				}
				else if (pbPreMap)
				{
					for (; pbData < pbEnd; pbDst++, pbData++)
						*pbDst = pbColorMap[pbPreMap[*pbData]];
				}
				else
				{
					for (; pbData < pbEnd; pbDst++, pbData++)
						*pbDst = pbColorMap[*pbData];
				}
			}
			else if (pbPreMap)
			{
				if (pbBlend)
				{
					for (; pbData < pbEnd; pbData++, pbDst++)
						*pbDst = pbBlend[(*pbDst << 8) + pbPreMap[*pbData]];
				}
				else
				{
					for (; pbData < pbEnd; pbDst++, pbData++)
						*pbDst = pbPreMap[*pbData];
				}
			}
			else if (pbBlend)
			{
				for (; pbData < pbEnd; pbData++, pbDst++)
					*pbDst = pbBlend[(*pbDst << 8) + *pbData];
			}
			else
			{
				for (; pbData < pbEnd; pbDst++, pbData++)
					*pbDst = *pbData;
			}
			nX += nCount;
			pbData = pbNextData;
			pbDst = pbNext;
		}
	}
	fInside = (!nClipLeft && !nClipRight) || (nX >= nClipLeft && nX + (int)pCel->dwWidth < nClipRight);
	if (pbColorMap)
	{
		if (pbBlend)
		{
			if (fInside)
				FastCmpDecodeMapBlend(pbData, pbDst, nLinesToDraw, nStride, pbColorMap, pbBlend);
			else
				FastCmpDecodeMapBlendClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight,
					pbColorMap, pbBlend);
		}
		else if (pbPreMap)
		{
			if (fInside)
				FastCmpDecodeMapPreMap(pbData, pbDst, nLinesToDraw, nStride, pbColorMap, pbPreMap);
			else
				FastCmpDecodeMapPreMapClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight,
					pbColorMap, pbPreMap);
		}
		else
		{
			if (fInside)
				FastCmpDecodeMap(pbData, pbDst, nLinesToDraw, nStride, pbColorMap);
			else
				FastCmpDecodeMapClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight,
					pbColorMap);
		}
	}
	else if (pbPreMap)
	{
		if (pbBlend)
		{
			if (fInside)
				FastCmpDecodePreMapBlend(pbData, pbDst, nLinesToDraw, nStride, pbPreMap, pbBlend);
			else
				FastCmpDecodePreMapBlendClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight,
					pbPreMap, pbBlend);
		}
		else
		{
			if (fInside)
				FastCmpDecodePreMap(pbData, pbDst, nLinesToDraw, nStride, pbPreMap);
			else
				FastCmpDecodePreMapClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight,
					pbPreMap);
		}
	}
	else if (pbBlend)
	{
		if (fInside)
			FastCmpDecodeBlend(pbData, pbDst, nLinesToDraw, nStride, pbBlend);
		else
			FastCmpDecodeBlendClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight, pbBlend);
	}
	else
	{
		if (fInside)
			FastCmpDecodeCopy(pbData, pbDst, nLinesToDraw, nStride);
		else
			FastCmpDecodeCopyClip(pbData, pbDst, nLinesToDraw, nStride, nX, nClipLeft, nClipRight);
	}
}

void __fastcall FastCmpBuildWarp(int nPitch)
{
	int anInside[80][80];
	int nRow;
	int nCol;
	int nDRow, nDCol;
	int nDist, nNewDist;
	int nToRow, nToCol;
	int nSrcCol, nClamp;

	for (nRow = 0; nRow < 80; nRow++)
	{
		for (nCol = 0; nCol < 80; nCol++)
		{
			nDCol = nCol - 40;
			nToCol = nDCol;
			nDRow = nRow - 40;
			nDist = nDCol * nDCol + nDRow * nDRow;
			anInside[nCol][nRow] = nDist < 1600;
			if (anInside[nCol][nRow])
			{
				nNewDist = nDist * nDist / 1600;
				nDist = (int)sqrt((double)nDist);
				nNewDist = (int)sqrt((double)nNewDist);
				if (nNewDist < nDist / 2)
					nNewDist = nDist / 2;
				if (nDist)
				{
					nToCol = nDCol * nNewDist / nDist;
					nDRow = nNewDist * nDRow / nDist;
				}
				nSrcCol = nToCol + 40;
				nDRow += 40;
				if (nSrcCol <= 0)
					nSrcCol = 0;
				nClamp = nSrcCol < 79 ? nSrcCol : 79;
				if (nDRow > 0)
					nToRow = nDRow;
				else
					nToRow = 0;
				if (nToRow >= 79)
					nToRow = 79;
				sgnWarpOffset[nRow * 80 + nCol] = (nToRow - nRow) * nPitch - nCol + nClamp;
			}
		}
	}
	for (nRow = 0; nRow < 80; nRow++)
	{
		sgnWarpStart[nRow] = 0;
		sgnWarpEnd[nRow] = 0;
		for (nCol = 0; nCol < 80; nCol++)
		{
			if (anInside[nCol][nRow])
				break;
		}
		if (nCol < 80)
		{
			sgnWarpStart[nRow] = nCol;
			while (anInside[nCol][nRow])
				nCol++;
			sgnWarpEnd[nRow] = nCol;
		}
	}
}

void WINAPI FastCmpWarp(BYTE *pbSurface, int nPitch)
{
	BYTE *pbRow;
	BYTE *pbPixel;
	BYTE *pbLimit;
	int nBase;
	int nRow;
	int nOffset;

	pbRow = pbSurface;
	nBase = 0;
	for (nRow = 0; 40 > nRow; nRow++)
	{
		nOffset = sgnWarpStart[nRow] + nBase;
		pbPixel = pbRow + sgnWarpStart[nRow];
		pbLimit = pbRow + (sgnWarpEnd[nRow] < 41 ? sgnWarpEnd[nRow] : 41);
		for (; pbPixel < pbLimit; pbPixel++, nOffset++)
			*pbPixel = pbPixel[sgnWarpOffset[nOffset]];
		nOffset = sgnWarpEnd[nRow] + nBase;
		pbPixel = pbRow + sgnWarpEnd[nRow];
		pbLimit = pbRow + (sgnWarpStart[nRow] >= 40 ? sgnWarpStart[nRow] : 40);
		for (; pbPixel > pbLimit; pbPixel--, nOffset--)
			*pbPixel = pbPixel[sgnWarpOffset[nOffset]];
		nBase += 80;
		pbRow += nPitch;
	}
	pbRow = pbSurface + 79 * nPitch;
	nBase = 79 * 80;
	for (nRow = 79; nRow >= 40; nRow--)
	{
		nOffset = sgnWarpStart[nRow] + nBase;
		pbPixel = pbRow + sgnWarpStart[nRow];
		pbLimit = pbRow + (sgnWarpEnd[nRow] < 41 ? sgnWarpEnd[nRow] : 41);
		for (; pbPixel < pbLimit; pbPixel++, nOffset++)
			*pbPixel = pbPixel[sgnWarpOffset[nOffset]];
		nOffset = sgnWarpEnd[nRow] + nBase;
		pbPixel = pbRow + sgnWarpEnd[nRow];
		pbLimit = pbRow + (sgnWarpStart[nRow] >= 40 ? sgnWarpStart[nRow] : 40);
		for (; pbPixel > pbLimit; pbPixel--, nOffset--)
			*pbPixel = pbPixel[sgnWarpOffset[nOffset]];
		nBase -= 80;
		pbRow -= nPitch;
	}
}

void __fastcall FastCmpDrawShadow(D2CEL *pCel, BYTE *pbDst, int nX, int nClipLeft, int nClipRight,
	DWORD dwStride, DWORD dwSkipRows, DWORD dwRows, const BYTE *pbTable, DWORD dwColor)
{
	const BYTE *pbData;
	BYTE *pbRow;
	BYTE *pbNext;
	BYTE *pbEnd;
	int nRowX;
	int nStride;
	int nCount;
	int nLeft, nRight, nDraw;
	BYTE bControl;

	nRowX = nX;
#line 1871
	D2ASSERT(dwStride >= pCel->dwWidth);
	pbData = FastCmpSkipLines(pCel->abData, dwSkipRows * 2);
	nStride = -(int)dwStride;
	pbRow = pbDst + nStride;
	for (;;)
	{
		bControl = *pbData++;
		if ((signed char)bControl < 0)
		{
			bControl &= 127;
			if (!bControl)
			{
				pbDst = pbRow - 1;
				pbRow = pbRow + nStride - 1;
				nX = --nRowX;
				if (!--dwRows)
					return;
				do
				{
					bControl = *pbData++;
					if ((signed char)bControl > 0)
						pbData += (signed char)bControl;
					else
						bControl &= 127;
				} while (bControl);
			}
			else
			{
				nX += (signed char)bControl;
				pbDst += (signed char)bControl;
			}
			continue;
		}
		nCount = (signed char)bControl;
		pbNext = pbDst + nCount;
		nDraw = nCount;
		do
		{
			if (nClipLeft || nClipRight)
			{
				nLeft = nClipLeft - nX;
				if (nLeft <= 0)
					nLeft = 0;
				nRight = nClipRight - nX;
				nRight = nRight > 0 ? nRight : 0;
				if (nRight >= nCount)
					nRight = nCount;
				if (nRight - nLeft <= 0)
					break;
				nDraw = nRight - nLeft;
				pbDst += nLeft;
			}
			if (nDraw)
			{
				if (pbTable)
				{
					for (pbEnd = pbDst + nDraw; pbDst < pbEnd; pbDst++)
						*pbDst = pbTable[(*pbDst << 8) + (BYTE)dwColor];
				}
				else
					memset(pbDst, (BYTE)dwColor, nDraw);
			}
		} while (0);
		nX += nCount;
		pbDst = pbNext;
		pbData += nCount;
	}
}
