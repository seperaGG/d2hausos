#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\Codec.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#include "FogImports.h"
#line 15

#define DWORD_BITS			32

#define CODEC_SIGNATURE		116
#define CODEC_CELLS			256
#define CODEC_BLOCKS		5625
#define CODEC_VALUES		262144
#define CODEC_PIXELS		120004

#define CODECHDR_OPTIONAL	2
#define CODECHDR_LITERALS	1

#define CODEC_DESTERROR       \
	"Sprite Decompression Error -- EXP: (dest != &pCel->bControlData[pCel->dwControlSize])   " \
	"VALUE: dest == %x   &pCel->bControlData[pCel->dwControlSize] == %x   FILE: %s   LINE: %d"

static __inline DWORD CodecTestBit(const DWORD *pdwBits, int n)
{
	return pdwBits[n >> 5] & gdwBitMasks[n & 31];
}

#pragma pack(push, 1)
struct D2DCCHEADER
{
 	BYTE		bSignature;
 	BYTE		bVersion;
 	BYTE		spriteDirections;
 	DWORD		dwFramesPerDir;
 	DWORD		field_07;
 	DWORD		unobserved_0b;
};
#pragma pack(pop)

struct D2BITBUFFER
{
 	DWORD		*pdwData;
 	DWORD		*pdwCurr;
 	int			nLength;
 	int			nCurBit;
};

struct D2DCCSPRITE
{
 	DWORD		dwVersion;
 	DWORD		field_04;
 	int			directions;
 	int			nFramesPerDir;
 	DWORD		field_10;
};

struct D2DCCCELL
{
 	DWORD		field_00;
 	int			nWidth;
 	int			nHeight;
 	int			nOffsetX;
 	int			nOffsetY;
 	int			nExtraSize;
 	BYTE		*pbExtra;
 	DWORD		dwControlSize;
 	BYTE		*image;
 	int			nStride;
 	DWORD		dwTopDown;
};

struct D2DCCDIR
{
 	D2DCCSPRITE	*ptSprite;
 	D2DCCCELL	*ptCells;
 	int			nLeft;
 	int			nTop;
 	int			nWidth;
 	int			nHeight;
 	int			nDirection;
 	int			nNumCells;
 	int			nNumCellBlocks;
 	int			nNumBlocks;
 	int			nBlocksPerRow;
};

struct D2DCCWALK
{
 	int			nCell;
 	int			nWidth;
 	int			nHeight;
 	int			nLeft;
 	int			nTop;
 	BYTE		*pbPixels;
 	int			nStride;
 	int			nRowBlock;
 	int			nBlock;
 	int			nX;
 	int			nY;
 	int			nRowOffset;
 	int			nBlockOffset;
 	int			nBlockWidth;
 	int			nBlockHeight;
};

struct D2DCCBLOCK
{
 	BYTE		*pbValues;
 	BYTE		*pbPixels;
 	int			nStride;
 	int			nWidth;
 	int			nHeight;
};

struct D2DCCVALUES
{
 	BYTE		*pbValues;
 	int			nNumValues;
};

struct D2DCCMAP
{
 	BYTE		abEntries[256];
 	int			nNumEntries;
};

static const int sgcnBitWidths[16] =
{
	0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 26, 28, 30, 32
};

static const DWORD sgcdwBitValues[DWORD_BITS] =
{
	1, 2, 4, 8, 16, 32, 64, 128,
	256, 512, 1024, 2048, 4096, 8192, 16384, 32768,
	65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608,
	16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 0x80000000
};

static const DWORD sgcdwBitMasks[DWORD_BITS + 1] =
{
	0, 1, 3, 7, 15, 31, 63, 127,
	255, 511, 1023, 2047, 4095, 8191, 16383, 32767,
	65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607,
	16777215, 33554431, 67108863, 134217727, 268435455, 536870911, 1073741823, 2147483647,
	0xFFFFFFFF
};

static const DWORD sgcdwSignMasks[DWORD_BITS] =
{
	0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFC, 0xFFFFFFF8, 0xFFFFFFF0, 0xFFFFFFE0, 0xFFFFFFC0, 0xFFFFFF80,
	0xFFFFFF00, 0xFFFFFE00, 0xFFFFFC00, 0xFFFFF800, 0xFFFFF000, 0xFFFFE000, 0xFFFFC000, 0xFFFF8000,
	0xFFFF0000, 0xFFFE0000, 0xFFFC0000, 0xFFF80000, 0xFFF00000, 0xFFE00000, 0xFFC00000, 0xFF800000,
	0xFF000000, 0xFE000000, 0xFC000000, 0xF8000000, 0xF0000000, 0xE0000000, 0xC0000000, 0x80000000
};

static D2DCCCELL sgtCells[CODEC_CELLS];
static D2DCCBLOCK sgtBlocks[CODEC_BLOCKS];
static BYTE sgabValues[CODEC_VALUES];
static BYTE sgabPixels[CODEC_PIXELS];
static CRITICAL_SECTION sgtCodecLock;
static BOOL sgfCodecInit;

static void __fastcall CodecSkipBits(D2BITBUFFER *ptBuffer, int nBits);
static void __fastcall CodecAlignBits(D2BITBUFFER *ptBuffer);
static DWORD __fastcall CodecReadBits(D2BITBUFFER *ptBuffer, int nBits);
static BOOL __fastcall CodecNextBlock(D2DCCDIR *ptDir, D2DCCWALK *ptWalk);
static BOOL __fastcall CodecFirstBlock(D2DCCDIR *ptDir, D2DCCWALK *ptWalk);
static void __fastcall CodecBuildColorMap(const BYTE *pbUsed, D2DCCMAP *ptMap, D2DCCMAP *ptColors);
static void __fastcall CodecBlockPixels(D2DCCBLOCK *ptBlock, D2BITBUFFER *ptBuffer, D2DCCBLOCK *ptPrev,
	D2BITBUFFER *ptSelect, BYTE **ppbValues);
static void __fastcall CodecBlockValues(D2DCCDIR *ptDir, D2DCCBLOCK *ptBlocks, D2BITBUFFER *ptKeep,
	D2BITBUFFER *ptCodes, D2BITBUFFER *ptLiteral, D2BITBUFFER *ptBytes, D2BITBUFFER *ptDeltas,
	D2DCCVALUES *ptValues, int *pnBits);

static __inline void __fastcall CodecSkipBits(D2BITBUFFER *ptBuffer, int nBits)
{
#line 468
	D2ASSERT(ptBuffer->nLength >= (ptBuffer->pdwCurr - ptBuffer->pdwData) * DWORD_BITS + ptBuffer->nCurBit + nBits);

	ptBuffer->pdwCurr += nBits / DWORD_BITS;
	ptBuffer->nCurBit += nBits & (DWORD_BITS - 1);
	if (ptBuffer->nCurBit >= DWORD_BITS)
	{
		ptBuffer->pdwCurr++;
		ptBuffer->nCurBit -= DWORD_BITS;
	}
}

static __inline void __fastcall CodecAlignBits(D2BITBUFFER *ptBuffer)
{
	if (ptBuffer->nCurBit & 7)
	{
		ptBuffer->nCurBit = (ptBuffer->nCurBit & ~7) + 8;
		if (ptBuffer->nCurBit >= DWORD_BITS)
		{
			ptBuffer->pdwCurr++;
			ptBuffer->nCurBit = 0;
		}
	}
}

static __inline DWORD __fastcall CodecReadBits(D2BITBUFFER *ptBuffer, int nBits)
{
	int nLeft;
	DWORD dwValue;
	DWORD dwHigh;

#line 616
	D2ASSERT((ptBuffer->pdwCurr - ptBuffer->pdwData) * DWORD_BITS + ptBuffer->nCurBit + nBits <= ptBuffer->nLength);

	if (!nBits)
		return 0;
	nLeft = DWORD_BITS - ptBuffer->nCurBit;
	if (nBits <= nLeft)
	{
		dwValue = sgcdwBitMasks[nBits] & (*ptBuffer->pdwCurr >> ptBuffer->nCurBit);
		ptBuffer->nCurBit += nBits;
		if (ptBuffer->nCurBit == DWORD_BITS)
		{
			ptBuffer->pdwCurr++;
			ptBuffer->nCurBit = 0;
		}
	}
	else
	{
		dwValue = sgcdwBitMasks[nLeft] & (*ptBuffer->pdwCurr >> ptBuffer->nCurBit);
		ptBuffer->pdwCurr++;
		ptBuffer->nCurBit = nBits - nLeft;
		dwHigh = sgcdwBitMasks[ptBuffer->nCurBit] & *ptBuffer->pdwCurr;
		dwHigh <<= nLeft;
		dwValue |= dwHigh;
	}
	return dwValue;
}

static __inline void CodecInitBits(D2BITBUFFER *ptBuffer, DWORD *pdwData, int nLength)
{
	ptBuffer->pdwData = pdwData;
	ptBuffer->pdwCurr = ptBuffer->pdwData;
	ptBuffer->nLength = nLength;
	ptBuffer->nCurBit = 0;
}

static __inline BYTE * CodecAtByte(D2BITBUFFER *ptBuffer)
{
	return (BYTE *)ptBuffer->pdwCurr + (ptBuffer->nCurBit + 7) / 8;
}

static __inline DWORD CodecReadField(D2BITBUFFER *ptBuffer, BOOL fSigned, int nWidth)
{
	int nBits;
	DWORD dwValue;

	nBits = sgcnBitWidths[nWidth];
	dwValue = CodecReadBits(ptBuffer, nBits);
	if (nBits && fSigned && (dwValue & sgcdwBitValues[nBits - 1]))
		dwValue |= sgcdwSignMasks[nBits];
	return dwValue;
}

static __inline void CodecMapColors(const DWORD *pdwColors, D2DCCMAP *ptMap, D2DCCMAP *ptColors)
{
	BYTE abUsed[256];
	int i;

	for (i = 0; i < 256; i++)
		abUsed[i] = CodecTestBit(pdwColors, i) != 0;
	CodecBuildColorMap(abUsed, ptMap, ptColors);
}

static __inline void CodecSetupDirection(D2DCCDIR *ptDir, D2DCCSPRITE *sprite, int dir)
{
	D2DCCWALK tWalk;
	D2DCCCELL *curcell;
	D2DCCCELL *ptEnd;
	int nLeft;
	int nTop;
	int nRight;
	int nBottom;
	int nX;
	int nY;

#line 780
	D2ASSERT(dir < sprite->directions);
	ptDir->ptSprite = sprite;
	ptDir->ptCells = sgtCells;
	ptDir->nDirection = dir;
	ptDir->nNumCells = sprite->nFramesPerDir;
	nLeft = 2147483647;
	nRight = (int)0x80000000;
	nTop = 2147483647;
	nBottom = (int)0x80000000;
	ptEnd = &sgtCells[sprite->nFramesPerDir];
	for (curcell = sgtCells; curcell < ptEnd; curcell++)
	{
		nX = curcell->nOffsetX;
		nY = curcell->nOffsetY - (curcell->dwTopDown ? 0 : curcell->nHeight);
		if (nX < nLeft)
			nLeft = nX;
		if (nX + curcell->nWidth > nRight)
			nRight = nX + curcell->nWidth;
		if (nY < nTop)
			nTop = nY;
		if (nY + curcell->nHeight > nBottom)
			nBottom = nY + curcell->nHeight;
	}
	ptDir->nLeft = nLeft;
	ptDir->nTop = nTop;
	ptDir->nWidth = nRight - nLeft;
	ptDir->nHeight = nBottom - nTop;
	ptDir->nBlocksPerRow = (ptDir->nWidth + 3) / 4;
	ptDir->nNumBlocks = (ptDir->nHeight + 3) / 4 * ptDir->nBlocksPerRow;
	ptDir->nNumCellBlocks = 0;
	if (CodecFirstBlock(ptDir, &tWalk))
	{
		do
		{
			ptDir->nNumCellBlocks++;
		} while (CodecNextBlock(ptDir, &tWalk));
	}
}

BOOL __fastcall CodecEncodeCel(D2DCCCELL *curcell, D2CEL *pCel, BYTE *pbData)
{
	BYTE *dest;
	int nStep;
	BYTE *pbRow;
	int nRow;
	int nX;
	int nStart;
	int nRun;

	dest = pbData;
	pbRow = curcell->image;
	if (curcell->dwTopDown)
		nStep = curcell->nStride;
	else
		nStep = -curcell->nStride;
	if (!curcell->dwTopDown)
		pbRow += (curcell->nHeight - 1) * curcell->nStride;
	for (nRow = 0; nRow < curcell->nHeight; nRow++, pbRow += nStep)
	{
		nX = 0;
		while (nX < curcell->nWidth)
		{
			nStart = nX;
			while (nX < curcell->nWidth && !pbRow[nX])
				nX++;
			if (nX >= curcell->nWidth)
				break;
			if (nX > nStart)
			{
				do
				{
					nRun = nX - nStart > 127 ? 127 : nX - nStart;
					*dest++ = (BYTE)(nRun | 128);
					nStart += nRun;
				} while (nStart < nX);
			}
			nStart = nX;
			while (nX < curcell->nWidth && pbRow[nX])
				nX++;
			if (nX > nStart)
			{
				do
				{
					nRun = nX - nStart > 127 ? 127 : nX - nStart;
					*dest++ = (BYTE)nRun;
					memcpy(dest, &pbRow[nStart], nRun);
					dest += nRun;
					nStart += nRun;
				} while (nStart < nX);
			}
		}
		*dest++ = 128;
	}
	if (dest != &pCel->abData[pCel->dwDataSize])
	{
#line 1142
		FogErrorManagerLog(CODEC_DESTERROR, dest, &pCel->abData[pCel->dwDataSize], D2_THIS_FILE, __LINE__);
		return FALSE;
	}
	return TRUE;
}

BOOL __fastcall CodecReadHeader(const char *pszFileName, D2CELDATA *ptCelData)
{
	DWORD hFile;
	DWORD dwRead;
	D2DCCHEADER header;
	DWORD dwSize;
	BOOL fRead;

	FogSafesockOpenStormFile(pszFileName, &hFile);
	if (!hFile)
		return FALSE;
	fRead = FogSafesockReadStormFile(hFile, &header, sizeof(header), &dwRead, 0, 0)
		&& dwRead == sizeof(header) && header.bSignature == CODEC_SIGNATURE;
	if (fRead)
	{
		ptCelData->tHeader[0] = header.bVersion;
		ptCelData->tHeader[1] = header.field_07;
		ptCelData->tHeader[2] = 0;
		ptCelData->tHeader[CELHDR_DIRS] = header.spriteDirections;
		ptCelData->tHeader[CELHDR_FRAMES] = header.dwFramesPerDir;
#line 1181
		D2ASSERT(header.spriteDirections <= 32);
		dwSize = header.spriteDirections * sizeof(DWORD);
		fRead = FogSafesockReadStormFile(hFile, ptCelData->adwDirOffsets, dwSize, &dwRead, 0, 0) && dwRead == dwSize;
	}
	if (!fRead)
	{
		FogSafesockCloseStormFile(hFile);
		return FALSE;
	}
	ptCelData->adwDirOffsets[header.spriteDirections] = FogSafesockGetStormFileSize(hFile, 0);
	FogSafesockCloseStormFile(hFile);
	ptCelData->bFlags |= CELDATAFLAG_DCC;
	return TRUE;
}

static __inline BYTE *CodecAllocCels(DWORD *pdwData, void *pPool, D2CELDATA *ptCelData, D2GFXBLOCK *ptGfxBlock,
	int nFirst, int nCount, DWORD *pdwSize)
{
	int nEnd;
	int dir;
	DWORD dwDirSize;

	*pdwSize = 0;
	nEnd = nFirst + nCount;
	for (dir = nFirst; dir < nEnd; dir++)
	{
		dwDirSize = *(DWORD *)&((BYTE *)pdwData - ptCelData->adwDirOffsets[nFirst])[ptCelData->adwDirOffsets[dir]];
		ptGfxBlock->nOffsets[dir] = *pdwSize;
		*pdwSize += dwDirSize;
	}
#line 1282
	return (BYTE *)D2ALLOCPOOL(pPool, *pdwSize);
}

BYTE * __fastcall CodecDecodeSprite(DWORD *pdwData, void *pPool, int nSize, D2CELDATA *ptCelData,
	D2GFXBLOCK *ptGfxBlock, int nFirst, int nCount, DWORD *pdwSize, const char *pszFileName)
{
	D2BITBUFFER tBuffer;
	D2DCCSPRITE sprite;
	int nEnd;
	int dir;
	BYTE *pbBuffer;
	D2CEL *pCel;
	DWORD dwFlags;
	DWORD dwWidths;
	int nFlagsWidth;
	int nWidthWidth;
	int nHeightWidth;
	int nOffsetXWidth;
	int nOffsetYWidth;
	int nExtraWidth;
	int nControlWidth;
	D2DCCCELL *ptEnd;
	D2DCCCELL *curcell;
	int nBits;
	int nExtraSize;
	int nExtra;
	int nCelsSize;
	BYTE *pbExtra;
	D2DCCDIR tDir;
	D2DCCWALK tWalk;
	int nKeepSize;
	int nCodesSize;
	int nLiteralSize;
	int nBytesSize;
	DWORD adwColors[8];
	D2DCCMAP tColors;
	D2DCCMAP tMap;
	D2BITBUFFER tKeep;
	D2BITBUFFER tCodes;
	D2BITBUFFER tLiteral;
	D2BITBUFFER tBytes;
	D2BITBUFFER tDeltas;
	D2BITBUFFER tKeep2;
	D2DCCVALUES tValues;
	int i;
	int j;
	int left;
	int top;
	D2BITBUFFER *ptSelect;
	int nCell;
	D2DCCBLOCK tBlock;
	BOOL fMore;
	int nSum;

	EnterCriticalSection(&sgtCodecLock);
	sprite.dwVersion = ptCelData->tHeader[0];
	sprite.directions = ptCelData->tHeader[CELHDR_DIRS];
	sprite.nFramesPerDir = ptCelData->tHeader[CELHDR_FRAMES];
	sprite.field_04 = ptCelData->tHeader[1];
	CodecInitBits(&tBuffer, pdwData, nSize * 8);
	pbBuffer = CodecAllocCels(pdwData, pPool, ptCelData, ptGfxBlock, nFirst, nCount, pdwSize);
	nEnd = nFirst + nCount;
	pCel = (D2CEL *)pbBuffer;
	for (dir = nFirst; dir < nEnd; dir++)
	{
		BYTE *pbValues;

		CodecReadBits(&tBuffer, 32);
		dwFlags = CodecReadBits(&tBuffer, 2);
		dwWidths = CodecReadBits(&tBuffer, 28);
		ptEnd = &sgtCells[sprite.nFramesPerDir];
		nFlagsWidth = dwWidths & 15;
		nWidthWidth = (dwWidths >> 4) & 15;
		nHeightWidth = (dwWidths >> 8) & 15;
		nOffsetXWidth = (dwWidths >> 12) & 15;
		nOffsetYWidth = (dwWidths >> 16) & 15;
		nExtraWidth = (dwWidths >> 20) & 15;
		nControlWidth = (dwWidths >> 24) & 15;
		nCelsSize = 0;
		nExtra = 0;
		for (curcell = sgtCells; curcell < ptEnd; curcell++)
		{
			curcell->field_00 = CodecReadField(&tBuffer, FALSE, nFlagsWidth);
			curcell->nWidth = CodecReadField(&tBuffer, FALSE, nWidthWidth);
			curcell->nHeight = CodecReadField(&tBuffer, FALSE, nHeightWidth);
			curcell->nOffsetX = CodecReadField(&tBuffer, TRUE, nOffsetXWidth);
			curcell->nOffsetY = CodecReadField(&tBuffer, TRUE, nOffsetYWidth);
			curcell->nExtraSize = CodecReadField(&tBuffer, FALSE, nExtraWidth);
			curcell->pbExtra = NULL;
			curcell->dwControlSize = CodecReadField(&tBuffer, FALSE, nControlWidth);
			curcell->image = NULL;
			curcell->nStride = 0;
			curcell->dwTopDown = CodecReadBits(&tBuffer, 1);
			nExtraSize = curcell->nExtraSize;
			nExtra += nExtraSize;
			nCelsSize += CELSIZE(curcell->dwControlSize + nExtraSize);
		}
		if (nExtra)
		{
			CodecAlignBits(&tBuffer);
			pbExtra = CodecAtByte(&tBuffer);
			for (curcell = sgtCells; curcell < ptEnd; curcell++)
			{
				if (curcell->nExtraSize)
				{
					curcell->pbExtra = pbExtra;
					pbExtra += curcell->nExtraSize;
				}
			}
			CodecSkipBits(&tBuffer, nExtra * 8);
		}
		CodecSetupDirection(&tDir, &sprite, dir);
		memset(sgtBlocks, 0, tDir.nNumBlocks * sizeof(D2DCCBLOCK));

		nKeepSize = 0;
		nLiteralSize = 0;
		nBytesSize = 0;
		if (dwFlags & CODECHDR_OPTIONAL)
			nKeepSize = CodecReadBits(&tBuffer, 20);
		nCodesSize = CodecReadBits(&tBuffer, 20);
		if (dwFlags & CODECHDR_LITERALS)
		{
			nLiteralSize = CodecReadBits(&tBuffer, 20);
			nBytesSize = CodecReadBits(&tBuffer, 20);
		}
		for (j = 0; j < 8; j++)
			adwColors[j] = CodecReadBits(&tBuffer, 32);
		CodecMapColors(adwColors, &tMap, &tColors);
		tKeep = tBuffer;
		CodecSkipBits(&tBuffer, nKeepSize);
		tCodes = tBuffer;
		CodecSkipBits(&tBuffer, nCodesSize);
		tLiteral = tBuffer;
		CodecSkipBits(&tBuffer, nLiteralSize);
		tBytes = tBuffer;
		CodecSkipBits(&tBuffer, nBytesSize);
		tDeltas = tBuffer;
		nBits = 0;
		tKeep2 = tKeep;
		tValues.pbValues = sgabValues;
		tValues.nNumValues = 0;
		CodecBlockValues(&tDir, sgtBlocks, nKeepSize ? &tKeep2 : NULL, &tCodes,
			nLiteralSize ? &tLiteral : NULL, nBytesSize ? &tBytes : NULL, &tDeltas, &tValues, &nBits);
		CodecSkipBits(&tBuffer, nBits);
		for (i = 0; i < tValues.nNumValues; i++)
			tValues.pbValues[i] = tColors.abEntries[tValues.pbValues[i]];

		for (curcell = sgtCells; curcell < ptEnd; curcell++)
		{
			left = curcell->nOffsetX - tDir.nLeft;
			top = curcell->nOffsetY - (curcell->dwTopDown ? 0 : curcell->nHeight) - tDir.nTop;
#line 1458
			D2ASSERT(left >= 0);
#line 1459
			D2ASSERT(top >= 0);
			curcell->nStride = tDir.nWidth;
			curcell->image = &sgabPixels[tDir.nWidth * top + left];
		}

		pbValues = tValues.pbValues;
		if (CodecFirstBlock(&tDir, &tWalk))
		{
			memset(sgtBlocks, 0, sizeof(sgtBlocks));
			nCell = 0;
			ptSelect = nKeepSize ? &tKeep : NULL;
			do
			{
				tBlock.pbValues = pbValues;
				tBlock.pbPixels = tWalk.pbPixels + tWalk.nBlockOffset;
				tBlock.nStride = tWalk.nStride;
				tBlock.nWidth = tWalk.nBlockWidth;
				tBlock.nHeight = tWalk.nBlockHeight;
				CodecBlockPixels(&tBlock, &tBuffer, &sgtBlocks[tWalk.nBlock], ptSelect, &pbValues);
				sgtBlocks[tWalk.nBlock] = tBlock;
				fMore = CodecNextBlock(&tDir, &tWalk);
				if (tWalk.nCell != nCell)
				{
					curcell = &sgtCells[nCell];
					pCel->dwFlags = curcell->field_00;
					pCel->dwWidth = curcell->nWidth;
					pCel->dwHeight = curcell->nHeight;
					pCel->nOffsetX = curcell->nOffsetX;
					pCel->nOffsetY = curcell->nOffsetY;
					pCel->dwDataSize = curcell->dwControlSize;
					pCel->dwExtraSize = 0;
					pCel->ptSlot = NULL;
#line 1505
					D2ASSERT(curcell->image);
					if (!CodecEncodeCel(curcell, pCel, pCel->abData))
					{
						FogErrorManagerLog("Sprite Decompression Error -- File:%s", pszFileName);
						FogErrorManagerLog("Dir:%d  Cell#:%d  Wid:%d  Ht:%d  OffX:%d  OffY:%d", dir,
							tWalk.nCell, pCel->dwWidth, pCel->dwHeight, pCel->nOffsetX, pCel->nOffsetY);
						FogErrorManagerLog("ControlSize:%d", pCel->dwDataSize);
						nSum = 0;
						for (i = 0; i < nSize; i++)
							nSum += ((BYTE *)pdwData)[i];
						FogErrorManagerLog("Buffer -- Len:%d  Checksum:%d", nSize, nSum);
#line 1519
						D2FATAL("Error decompressing sprite");
					}
					pCel = (D2CEL *)((BYTE *)pCel + CELSIZE(pCel->dwDataSize + pCel->dwExtraSize));
					nCell = tWalk.nCell;
				}
			} while (fMore);
		}
		CodecAlignBits(&tBuffer);
	}
	LeaveCriticalSection(&sgtCodecLock);
	return pbBuffer;
}

static BOOL __fastcall CodecNextBlock(D2DCCDIR *ptDir, D2DCCWALK *ptWalk)
{
	D2DCCCELL *curcell;
	int nFirst;
	int nFirstRow;

	ptWalk->nX += ptWalk->nBlockWidth;
	if (ptWalk->nX < ptWalk->nWidth)
	{
		ptWalk->nBlockOffset += ptWalk->nBlockWidth;
		ptWalk->nBlock++;
		ptWalk->nBlockWidth = ptWalk->nX + 5 < ptWalk->nWidth ? 4 : ptWalk->nWidth - ptWalk->nX;
		return TRUE;
	}
	ptWalk->nY += ptWalk->nBlockHeight;
	if (ptWalk->nY < ptWalk->nHeight)
	{
		nFirst = -(ptWalk->nLeft % 4);
		ptWalk->nX = 0;
		ptWalk->nBlockWidth = nFirst + 5 < ptWalk->nWidth ? nFirst + 4 : ptWalk->nWidth;
		ptWalk->nRowOffset += ptWalk->nStride * ptWalk->nBlockHeight;
		ptWalk->nBlockOffset = ptWalk->nRowOffset;
		ptWalk->nRowBlock += ptDir->nBlocksPerRow;
		ptWalk->nBlock = ptWalk->nRowBlock;
		ptWalk->nBlockHeight = ptWalk->nY + 5 < ptWalk->nHeight ? 4 : ptWalk->nHeight - ptWalk->nY;
		return TRUE;
	}
	ptWalk->nCell++;
	curcell = &ptDir->ptCells[ptWalk->nCell];
	while (1)
	{
		if (ptWalk->nCell >= ptDir->nNumCells)
			return FALSE;
		if (curcell->nWidth && curcell->nHeight)
			break;
		ptWalk->nCell++;
		curcell++;
	}
	ptWalk->nWidth = curcell->nWidth;
	ptWalk->nHeight = curcell->nHeight;
	ptWalk->nLeft = curcell->nOffsetX - ptDir->nLeft;
	ptWalk->nTop = curcell->nOffsetY - ptDir->nTop - (curcell->dwTopDown ? 0 : curcell->nHeight);
	ptWalk->pbPixels = curcell->image;
	ptWalk->nStride = curcell->nStride;
	ptWalk->nRowBlock = ptWalk->nTop / 4 * ptDir->nBlocksPerRow + ptWalk->nLeft / 4;
	ptWalk->nBlock = ptWalk->nRowBlock;
	ptWalk->nX = 0;
	ptWalk->nY = 0;
	ptWalk->nRowOffset = 0;
	ptWalk->nBlockOffset = 0;
	nFirst = -(ptWalk->nLeft % 4);
	nFirstRow = -(ptWalk->nTop % 4);
	ptWalk->nBlockWidth = nFirst + 5 < ptWalk->nWidth ? nFirst + 4 : ptWalk->nWidth;
	ptWalk->nBlockHeight = nFirstRow + 5 < ptWalk->nHeight ? nFirstRow + 4 : ptWalk->nHeight;
	return TRUE;
}

static BOOL __fastcall CodecFirstBlock(D2DCCDIR *ptDir, D2DCCWALK *ptWalk)
{
	if (!ptDir->nNumCells)
		return FALSE;
	memset(ptWalk, 0, sizeof(D2DCCWALK));
	ptWalk->nCell = -1;
	return CodecNextBlock(ptDir, ptWalk);
}

static void __fastcall CodecBuildColorMap(const BYTE *pbUsed, D2DCCMAP *ptMap, D2DCCMAP *ptColors)
{
	int nCount;
	int i;

	for (i = 0, nCount = 0; i < 256; i++)
	{
		if (pbUsed[i])
		{
			ptMap->abEntries[i] = (BYTE)nCount;
			ptColors->abEntries[nCount] = (BYTE)i;
			nCount++;
		}
	}
	ptMap->nNumEntries = 256;
	ptColors->nNumEntries = nCount;
}

static __inline int CodecValueBits(const BYTE *pbValues)
{
	int nBits;

	nBits = 0;
	while ((1 << nBits) + 1 <= 4 && pbValues[1 << nBits] != pbValues[(1 << nBits) - 1])
		nBits++;
	return nBits;
}

static void __fastcall CodecBlockPixels(D2DCCBLOCK *ptBlock, D2BITBUFFER *ptBuffer, D2DCCBLOCK *ptPrev,
	D2BITBUFFER *ptSelect, BYTE **ppbValues)
{
	BYTE abCopy[5][5];
	int nRow;
	BYTE *pbSrc;
	BYTE *pbDst;
	BYTE *pbCopy;
	int nBits;
	int nValue;

	if (ptSelect && ptPrev->pbValues && CodecReadBits(ptSelect, 1))
	{
		if (ptBlock->nWidth == ptPrev->nWidth && ptBlock->nHeight == ptPrev->nHeight)
		{
			if (ptBlock->pbPixels != ptPrev->pbPixels)
			{
				pbSrc = ptPrev->pbPixels;
				pbCopy = abCopy[0];
				for (nRow = 0; nRow < ptBlock->nHeight; nRow++)
				{
					memcpy(pbCopy, pbSrc, ptBlock->nWidth);
					pbCopy += 5;
					pbSrc += ptPrev->nStride;
				}
				pbDst = ptBlock->pbPixels;
				pbCopy = abCopy[0];
				for (nRow = 0; nRow < ptBlock->nHeight; nRow++, pbDst += ptBlock->nStride, pbCopy += 5)
					memcpy(pbDst, pbCopy, ptBlock->nWidth);
			}
		}
		else
		{
			pbDst = ptBlock->pbPixels;
			for (nRow = 0; nRow < ptBlock->nHeight; nRow++, pbDst += ptBlock->nStride)
				memset(pbDst, 0, ptBlock->nWidth);
		}
		return;
	}
	pbDst = ptBlock->pbPixels;
	nBits = CodecValueBits(ptBlock->pbValues);
	if (!nBits)
	{
		nValue = (*ppbValues)[0];
		for (nRow = 0; nRow < ptBlock->nHeight; nRow++, pbDst += ptBlock->nStride)
			memset(pbDst, nValue, ptBlock->nWidth);
	}
	else
	{
		int nX, nY;

		for (nY = 0; nY < ptBlock->nHeight; nY++, pbDst += ptBlock->nStride)
		{
			for (nX = 0; nX < ptBlock->nWidth; nX++)
				pbDst[nX] = (*ppbValues)[CodecReadBits(ptBuffer, nBits)];
		}
	}
	*ppbValues += 4;
}

static void __fastcall CodecBlockValues(D2DCCDIR *ptDir, D2DCCBLOCK *ptBlocks, D2BITBUFFER *ptKeep,
	D2BITBUFFER *ptCodes, D2BITBUFFER *ptLiteral, D2BITBUFFER *ptBytes, D2BITBUFFER *ptDeltas,
	D2DCCVALUES *ptValues, int *pnBits)
{
	BYTE *pbValues;
	int nStart;
	D2DCCWALK tWalk;
	D2DCCWALK *ptWalk;
	BYTE *pbOld;
	DWORD adwCode[1];
	BOOL fLiteral;
	BYTE bLast;
	BYTE bValue;
	DWORD dwStep;
	int i;
	int nFrom;
	int nTo;

	pbValues = ptValues->pbValues;
	nStart = (ptDeltas->pdwCurr - ptDeltas->pdwData) * DWORD_BITS + ptDeltas->nCurBit;
	ptWalk = &tWalk;
	if (CodecFirstBlock(ptDir, ptWalk))
	{
		do
		{
			adwCode[0] = 15;
			pbOld = ptBlocks[ptWalk->nBlock].pbValues;
			if (pbOld)
			{
				if (ptKeep && CodecReadBits(ptKeep, 1))
					continue;
				adwCode[0] = CodecReadBits(ptCodes, 4);
			}
			fLiteral = FALSE;
			if (adwCode[0] && ptLiteral)
				fLiteral = CodecReadBits(ptLiteral, 1);
			adwCode[0] |= 0xFFFFFFF0;
			bLast = 0;
			i = 4;
			while (i-- != 0)
			{
				if (CodecTestBit(adwCode, i))
				{
					if (fLiteral)
					{
						bValue = (BYTE)CodecReadBits(ptBytes, 8);
						pbValues[i] = bValue;
					}
					else
					{
						bValue = bLast;
						do
						{
							dwStep = CodecReadBits(ptDeltas, 4);
							bValue += (BYTE)dwStep;
						} while (dwStep == 15);
						pbValues[i] = bValue;
					}
					if (bLast == bValue)
					{
						nFrom = i;
						nTo = -1;
						for (;;)
						{
							while (!CodecTestBit(adwCode, ++nFrom))
								;
							if (nFrom >= 4)
								break;
							while (!CodecTestBit(adwCode, ++nTo))
								;
							pbValues[nTo] = pbValues[nFrom];
						}
						for (;;)
						{
							while (!CodecTestBit(adwCode, ++nTo))
								;
							if (nTo >= 4)
								break;
							pbValues[nTo] = 0;
						}
						while (i-- != 0)
						{
							if (!CodecTestBit(adwCode, i))
								pbValues[i] = pbOld[i];
						}
						break;
					}
					bLast = bValue;
				}
				else
					pbValues[i] = pbOld[i];
			}
			ptBlocks[ptWalk->nBlock].pbValues = pbValues;
			pbValues += 4;
		} while (CodecNextBlock(ptDir, ptWalk));
	}
	*pnBits = (ptDeltas->pdwCurr - ptDeltas->pdwData) * DWORD_BITS + ptDeltas->nCurBit - nStart;
	ptValues->nNumValues = pbValues - ptValues->pbValues;
}

void __fastcall CodecInit(void)
{
	if (!sgfCodecInit)
	{
		InitializeCriticalSection(&sgtCodecLock);
		sgfCodecInit = TRUE;
	}
}

void __fastcall CodecShutdown(void)
{
	if (sgfCodecInit)
	{
		EnterCriticalSection(&sgtCodecLock);
		sgfCodecInit = FALSE;
		DeleteCriticalSection(&sgtCodecLock);
	}
}
