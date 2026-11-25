#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\BitManip\\BitManip.cpp"
#include "../FogLocal.h"
#include "BitManip.h"
#include "D2Macros.h"

extern "C" const DWORD gdwBitMasks[32] =
{
	1, 2, 4, 8, 16, 32, 64, 128,
	256, 512, 1024, 2048, 4096, 8192, 16384, 32768,
	65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608,
	16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 0x80000000,
};

extern "C" const DWORD gdwInvBitMasks[32] =
{
	0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7, 0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F,
	0xFFFFFEFF, 0xFFFFFDFF, 0xFFFFFBFF, 0xFFFFF7FF, 0xFFFFEFFF, 0xFFFFDFFF, 0xFFFFBFFF, 0xFFFF7FFF,
	0xFFFEFFFF, 0xFFFDFFFF, 0xFFFBFFFF, 0xFFF7FFFF, 0xFFEFFFFF, 0xFFDFFFFF, 0xFFBFFFFF, 0xFF7FFFFF,
	0xFEFFFFFF, 0xFDFFFFFF, 0xFBFFFFFF, 0xF7FFFFFF, 0xEFFFFFFF, 0xDFFFFFFF, 0xBFFFFFFF, 2147483647,
};
static const DWORD sgcdwLowMasks[9] =
{
	0, 1, 3, 7, 15, 31, 63, 127,
	255,
};
static const BYTE sgcbHighMasks[8] =
{
	255, 254, 252, 248, 240, 224, 192, 128,
};

static DWORD BitManipMaskMemory(int nIndex)
{
	if (nIndex >= 1 && nIndex <= 32)
		return gdwBitMasks[nIndex - 1];
	if (nIndex >= 33 && nIndex <= 64)
		return gdwInvBitMasks[nIndex - 33];
	if (nIndex >= 65 && nIndex <= 73)
		return sgcdwLowMasks[nIndex - 65];
	if (nIndex == 74)
		return 0xF8FCFEFF;
	if (nIndex == 75)
		return 0x80C0E0F0;
	return 0;
}

void __stdcall BitManipSetBit(BYTE *pbBits, int nBit)
{
	pbBits[nBit >> 3] |= (BYTE)gdwBitMasks[nBit & 7];
}

DWORD __stdcall BitManipTestBit(const BYTE *pbBits, int nBit)
{
	return pbBits[nBit >> 3] & gdwBitMasks[nBit & 7];
}

void __stdcall BitManipClearBit(BYTE *pbBits, int nBit)
{
	pbBits[nBit >> 3] &= (BYTE)gdwInvBitMasks[nBit & 7];
}

void __stdcall BitManipToggleBit(BYTE *pbBits, int nBit)
{
	pbBits[nBit >> 3] ^= (BYTE)gdwBitMasks[nBit & 7];
}

void __stdcall BitManipAnd(const BYTE *pbSrc1, const BYTE *pbSrc2, BYTE *pbDst, int nBits)
{
	int nBytes = (nBits + 7) / 8;
	int nWhole = nBytes / 4 * 4;
	int i = 0;

	for (; i < nWhole; i += 4)
		*(DWORD *)(pbDst + i) = *(const DWORD *)(pbSrc1 + i) & *(const DWORD *)(pbSrc2 + i);
	switch (nBytes - nWhole)
	{
	case 3:
		pbDst[i + 2] = pbSrc1[i + 2] & pbSrc2[i + 2];
	case 2:
		pbDst[i + 1] = pbSrc1[i + 1] & pbSrc2[i + 1];
	case 1:
		pbDst[i] = pbSrc1[i] & pbSrc2[i];
	}
}

void __stdcall BitManipOr(const BYTE *pbSrc1, const BYTE *pbSrc2, BYTE *pbDst, int nBits)
{
	int nBytes = (nBits + 7) / 8;
	int nWhole = nBytes / 4 * 4;
	int i = 0;

	for (; i < nWhole; i += 4)
		*(DWORD *)(pbDst + i) = *(const DWORD *)(pbSrc1 + i) | *(const DWORD *)(pbSrc2 + i);
	switch (nBytes - nWhole)
	{
	case 3:
		pbDst[i + 2] = pbSrc1[i + 2] | pbSrc2[i + 2];
	case 2:
		pbDst[i + 1] = pbSrc1[i + 1] | pbSrc2[i + 1];
	case 1:
		pbDst[i] = pbSrc1[i] | pbSrc2[i];
	}
}

void __stdcall BitManipNot(const BYTE *pbSrc, BYTE *pbDst, int nBits)
{
	int nBytes = (nBits + 7) / 8;
	int nWhole = nBytes / 4 * 4;
	int i = 0;

	for (; i < nWhole; i += 4)
		*(DWORD *)(pbDst + i) = ~*(const DWORD *)(pbSrc + i);
	switch (nBytes - nWhole)
	{
	case 3:
		pbDst[i + 2] = ~pbSrc[i + 2];
	case 2:
		pbDst[i + 1] = ~pbSrc[i + 1];
	case 1:
		pbDst[i] = ~pbSrc[i];
	}
}

void __stdcall BitManipXor(const BYTE *pbSrc1, const BYTE *pbSrc2, BYTE *pbDst, int nBits)
{
	int nBytes = (nBits + 7) / 8;
	int nWhole = nBytes / 4 * 4;
	int i = 0;

	for (; i < nWhole; i += 4)
		*(DWORD *)(pbDst + i) = *(const DWORD *)(pbSrc1 + i) ^ *(const DWORD *)(pbSrc2 + i);
	switch (nBytes - nWhole)
	{
	case 3:
		pbDst[i + 2] = pbSrc1[i + 2] ^ pbSrc2[i + 2];
	case 2:
		pbDst[i + 1] = pbSrc1[i + 1] ^ pbSrc2[i + 1];
	case 1:
		pbDst[i] = pbSrc1[i] ^ pbSrc2[i];
	}
}

void __stdcall BitManipInitBuffer(BITBUFFER *ptBuffer, BYTE *pbData, int nBytes)
{
#line 316
	D2ASSERT(ptBuffer);
	ptBuffer->nCurByte = 0;
	ptBuffer->pbData = pbData;
	ptBuffer->nLength = nBytes * 8;
	ptBuffer->nCurBit = 0;
}

int __stdcall BitManipGetBytesUsed(const BITBUFFER *ptBuffer)
{
	return ptBuffer->nCurBit ? ptBuffer->nCurByte + 1 : ptBuffer->nCurByte;
}

void __stdcall BitManipWrite(BITBUFFER *ptBuffer, int nValue, int nBits)
{
	int nChunk;
	DWORD dwMask;

#line 343
	D2ASSERT(ptBuffer->nCurByte * 8 + ptBuffer->nCurBit + nBits <= ptBuffer->nLength);
	if (!ptBuffer->nCurBit)
		*ptBuffer->pbData = 0;
	while (nBits > 0)
	{
		nChunk = 8 - ptBuffer->nCurBit;
		if (nChunk > nBits)
			nChunk = nBits;
		dwMask = sgcdwLowMasks[nChunk];
		*ptBuffer->pbData |= (dwMask & nValue) << ptBuffer->nCurBit;
		ptBuffer->nCurBit += nChunk;
		if (ptBuffer->nCurBit != 8)
			break;
		ptBuffer->pbData++;
		ptBuffer->nCurByte++;
		ptBuffer->nCurBit = 0;
		if (ptBuffer->nCurByte * 8 < ptBuffer->nLength)
			*ptBuffer->pbData = 0;
		nValue >>= nChunk;
		nBits -= nChunk;
	}
}

DWORD __stdcall BitManipReadUnsigned(BITBUFFER *ptBuffer, int nBits)
{
	DWORD dwResult;
	int nShift;
	int nChunk;
	DWORD dwBits;
	int nBit;

#line 387
	D2ASSERT(ptBuffer->nCurByte * 8 + ptBuffer->nCurBit + nBits < ptBuffer->nLength);
	dwResult = 0;
	nShift = 0;
	while (nBits > 0)
	{
		nChunk = 8 - ptBuffer->nCurBit;
		if (nChunk > nBits)
			nChunk = nBits;
		nBit = ptBuffer->nCurBit;
		dwBits = (DWORD)(sgcbHighMasks[nBit] & *ptBuffer->pbData) >> nBit;
		nBits -= nChunk;
		dwResult += (dwBits & sgcdwLowMasks[nChunk]) << nShift;
		nShift += nChunk;
		ptBuffer->nCurBit += nChunk;
		if (ptBuffer->nCurBit == 8)
		{
			ptBuffer->pbData++;
			ptBuffer->nCurBit = 0;
			ptBuffer->nCurByte++;
		}
	}
	return dwResult;
}

int __stdcall BitManipReadSigned(BITBUFFER *ptBuffer, int nBits)
{
	DWORD dwResult;
	int nShift;
	int nChunk;
	int nTotal;
	DWORD dwBits;
	int nBit;

#line 429
	D2ASSERT(ptBuffer->nCurByte * 8 + ptBuffer->nCurBit + nBits < ptBuffer->nLength);
	dwResult = 0;
	nShift = 0;
	nTotal = nBits;
	while (nBits > 0)
	{
		nChunk = 8 - ptBuffer->nCurBit;
		if (nChunk > nBits)
			nChunk = nBits;
		nBit = ptBuffer->nCurBit;
		dwBits = (DWORD)(sgcbHighMasks[nBit] & *ptBuffer->pbData) >> nBit;
		nBits -= nChunk;
		dwResult += (dwBits & sgcdwLowMasks[nChunk]) << nShift;
		nShift += nChunk;
		ptBuffer->nCurBit += nChunk;
		if (ptBuffer->nCurBit == 8)
		{
			ptBuffer->pbData++;
			ptBuffer->nCurBit = 0;
			ptBuffer->nCurByte++;
		}
	}
	if ((gdwBitMasks - 1)[nTotal] & dwResult)
		dwResult |= ~((gdwBitMasks - 1)[nTotal + 1] - 1);
	return (int)dwResult;
}

void __stdcall BitManipWriteCode(BITBUFFER *ptBuffer, const int *pnCodes, int nIndex)
{
	BitManipWrite(ptBuffer, pnCodes[nIndex * 2], pnCodes[nIndex * 2 + 1]);
}

int __stdcall BitManipDecode(BITBUFFER *ptBuffer, const int *pnTree)
{
	int nNode = 0;
	int nIndex;
	DWORD dwBit;
	for (;;)
	{
		dwBit = (gdwBitMasks[ptBuffer->nCurBit] & *ptBuffer->pbData) >> ptBuffer->nCurBit;
		ptBuffer->nCurBit++;
		if (ptBuffer->nCurBit == 8)
		{
			ptBuffer->pbData++;
			ptBuffer->nCurBit = 0;
			ptBuffer->nCurByte++;
		}
		nIndex = dwBit + nNode;
		if (pnTree[nIndex] >= 0)
			return pnTree[dwBit + nNode];
		nNode = nIndex * 2 + 2;
	}
}
