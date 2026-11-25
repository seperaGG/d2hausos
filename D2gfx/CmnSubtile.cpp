#define D2_THIS_FILE "C:\\D2\\Source\\D2Gfx\\SRC\\CmnSubtile.cpp"
#include <string.h>
#include "D2gfx.h"
#include "D2Macros.h"
#include "StormImports.h"

#define GOURAUD_SHIFT		24
#define SUBTILE_ROWS		15
#define BLOCK_ROWS			32

#include "CmnSubtile.h"

static int sgnBlockLeft = 0;
static int sgnPartInLeft = 0;
static int sgnClipTop = 0;
static int sgnPartInBottom = 0;
static int sgnPartRight = 0;
static int sgnClipRight = 0;
static int sgnBlockInLeft = 0;
static int sganRowOffsets[32 + 600] = { 0 };
static int sgnBlockRight = 0;
static BYTE sgabGouraud[32][32][32] = { 0 };
static int sgnClipLeft = 0;
static int sgnPartTop = 0;
static int sgnPartInTop = 0;
static int sgnPartInRight = 0;
static int sgnBlockTop = 0;
static int sgnBlockInRight = 0;
static int sgnBlockInTop = 0;
static int sgnPartBottom = 0;
static int sgnClipBottom = 0;
static int sgnPartLeft = 0;
static int sgnBlockInBottom = 0;
static int sgnBlockBottom = 0;
static BOOL sgfFlatLight = 0;

static int sgcanRowSkip[SUBTILE_ROWS] =
{
	14, 12, 10, 8, 6, 4, 2, 0, 2, 4, 6, 8, 10, 12, 14,
};
static int sgcanRowWidth[SUBTILE_ROWS] =
{
	4, 8, 12, 16, 20, 24, 28, 32, 28, 24, 20, 16, 12, 8, 4,
};
static int sgcanRowOffset[SUBTILE_ROWS] =
{
	0, 4, 12, 24, 40, 60, 84, 112, 144, 172, 196, 216, 232, 244, 252,
};

#define FLAT_PIXEL(n)		pbDst[n] = pbTable[pbSrc[n]]
#define GOURAUD_PIXEL(n)	pbDst[n] = gaabCmnLightTables[pbRun[n]][pbSrc[n]]
#define FLAT_PIXEL4(n)		FLAT_PIXEL(n + 3); FLAT_PIXEL(n + 2); FLAT_PIXEL(n + 1); FLAT_PIXEL(n)
#define GOURAUD_PIXEL4(n)	GOURAUD_PIXEL(n + 3); GOURAUD_PIXEL(n + 2); GOURAUD_PIXEL(n + 1); GOURAUD_PIXEL(n)

#define GOURAUD_ROW(nLeft, nRight, nShift) \
	((BYTE *)sgabGouraud + (((((nLeft) >> (nShift)) << 5) + ((nRight) >> (nShift))) << 5))

void __fastcall CmnSubtileDrawFlat(BYTE *pbData, BYTE *pbRow, BYTE *pbTable)
{
	BYTE *pbSrc, *pbDst, *pbLine;
	int nRow;

	pbLine = pbRow;
	for (nRow = 0; nRow < SUBTILE_ROWS; nRow++)
	{
		pbDst = pbLine + sgcanRowSkip[nRow];
		pbSrc = pbData + sgcanRowOffset[nRow];
		switch (sgcanRowWidth[nRow])
		{
		case 32: FLAT_PIXEL4(28);
		case 28: FLAT_PIXEL4(24);
		case 24: FLAT_PIXEL4(20);
		case 20: FLAT_PIXEL4(16);
		case 16: FLAT_PIXEL4(12);
		case 12: FLAT_PIXEL4(8);
		case 8: FLAT_PIXEL4(4);
		case 4: FLAT_PIXEL4(0);
		}
		pbLine += gtCmnInfo.nPitch;
	}
}

void __fastcall CmnSubtileDrawGouraud(BYTE *pbData, BYTE *pbRow, int nLeft, int nLeftStep, int nRight,
	int nRightStep)
{
	BYTE *pbSrc, *pbDst, *pbRun, *pbLine;
	int nRow, nIdx;

	pbLine = pbRow;
	for (nRow = 0; nRow < SUBTILE_ROWS; nRow++)
	{
		nIdx = nLeft >> 7;
		nIdx = (nIdx << 5) + (nRight >> 7);
		pbRun = (BYTE *)sgabGouraud + ((nIdx << 5) + sgcanRowSkip[nRow]);
		pbSrc = pbData + sgcanRowOffset[nRow];
		pbDst = pbLine + sgcanRowSkip[nRow];
		switch (sgcanRowWidth[nRow])
		{
		case 32: GOURAUD_PIXEL4(28);
		case 28: GOURAUD_PIXEL4(24);
		case 24: GOURAUD_PIXEL4(20);
		case 20: GOURAUD_PIXEL4(16);
		case 16: GOURAUD_PIXEL4(12);
		case 12: GOURAUD_PIXEL4(8);
		case 8: GOURAUD_PIXEL4(4);
		case 4: GOURAUD_PIXEL4(0);
		}
		pbLine += gtCmnInfo.nPitch;
		nLeft += nLeftStep;
		nRight += nRightStep;
	}
}

void __fastcall CmnSubtileDrawFlatRle(BYTE *pbSrc, BYTE *pbRow, BYTE *pbTable, int nRows)
{
	BYTE *pbDst;
	int nSkip, nRun;

	pbDst = pbRow;
	while (nRows)
	{
		nSkip = *pbSrc++;
		nRun = *pbSrc++;
		if (!nSkip && !nRun)
		{
			nRows--;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			continue;
		}
		pbDst += nSkip;
		switch (nRun)
		{
		case 32: FLAT_PIXEL(31);
		case 31: FLAT_PIXEL(30);
		case 30: FLAT_PIXEL(29);
		case 29: FLAT_PIXEL(28);
		case 28: FLAT_PIXEL(27);
		case 27: FLAT_PIXEL(26);
		case 26: FLAT_PIXEL(25);
		case 25: FLAT_PIXEL(24);
		case 24: FLAT_PIXEL(23);
		case 23: FLAT_PIXEL(22);
		case 22: FLAT_PIXEL(21);
		case 21: FLAT_PIXEL(20);
		case 20: FLAT_PIXEL(19);
		case 19: FLAT_PIXEL(18);
		case 18: FLAT_PIXEL(17);
		case 17: FLAT_PIXEL(16);
		case 16: FLAT_PIXEL(15);
		case 15: FLAT_PIXEL(14);
		case 14: FLAT_PIXEL(13);
		case 13: FLAT_PIXEL(12);
		case 12: FLAT_PIXEL(11);
		case 11: FLAT_PIXEL(10);
		case 10: FLAT_PIXEL(9);
		case 9: FLAT_PIXEL(8);
		case 8: FLAT_PIXEL(7);
		case 7: FLAT_PIXEL(6);
		case 6: FLAT_PIXEL(5);
		case 5: FLAT_PIXEL(4);
		case 4: FLAT_PIXEL(3);
		case 3: FLAT_PIXEL(2);
		case 2: FLAT_PIXEL(1);
		case 1: FLAT_PIXEL(0);
		}
		pbDst += nRun;
		pbSrc += nRun;
	}
}

void __fastcall CmnSubtileDrawGouraudRle(BYTE *pbSrc, BYTE *pbRow, int nLeft, int nLeftStep,
	int nRight, int nRightStep, int nRows, int nShift)
{
	BYTE *pbDst, *pbRun;
	int nSkip, nRun;

	pbDst = pbRow;
	pbRun = GOURAUD_ROW(nLeft, nRight, nShift);
	while (nRows)
	{
		nSkip = *pbSrc++;
		nRun = *pbSrc++;
		if (!nSkip && !nRun)
		{
			nRows--;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			nLeft += nLeftStep;
			nRight += nRightStep;
			pbRun = GOURAUD_ROW(nLeft, nRight, nShift);
			continue;
		}
		pbDst += nSkip;
		pbRun += nSkip;
		switch (nRun)
		{
		case 32: GOURAUD_PIXEL(31);
		case 31: GOURAUD_PIXEL(30);
		case 30: GOURAUD_PIXEL(29);
		case 29: GOURAUD_PIXEL(28);
		case 28: GOURAUD_PIXEL(27);
		case 27: GOURAUD_PIXEL(26);
		case 26: GOURAUD_PIXEL(25);
		case 25: GOURAUD_PIXEL(24);
		case 24: GOURAUD_PIXEL(23);
		case 23: GOURAUD_PIXEL(22);
		case 22: GOURAUD_PIXEL(21);
		case 21: GOURAUD_PIXEL(20);
		case 20: GOURAUD_PIXEL(19);
		case 19: GOURAUD_PIXEL(18);
		case 18: GOURAUD_PIXEL(17);
		case 17: GOURAUD_PIXEL(16);
		case 16: GOURAUD_PIXEL(15);
		case 15: GOURAUD_PIXEL(14);
		case 14: GOURAUD_PIXEL(13);
		case 13: GOURAUD_PIXEL(12);
		case 12: GOURAUD_PIXEL(11);
		case 11: GOURAUD_PIXEL(10);
		case 10: GOURAUD_PIXEL(9);
		case 9: GOURAUD_PIXEL(8);
		case 8: GOURAUD_PIXEL(7);
		case 7: GOURAUD_PIXEL(6);
		case 6: GOURAUD_PIXEL(5);
		case 5: GOURAUD_PIXEL(4);
		case 4: GOURAUD_PIXEL(3);
		case 3: GOURAUD_PIXEL(2);
		case 2: GOURAUD_PIXEL(1);
		case 1: GOURAUD_PIXEL(0);
		}
		pbDst += nRun;
		pbSrc += nRun;
		pbRun += nRun;
	}
}

void __fastcall CmnSubtileDrawFlatRleClip(int nX, int nY, BYTE *pbSrc, BYTE *pbRow, BYTE *pbTable,
	int nRows)
{
	BYTE *pbDst, *pbNextSrc, *pbNextDst;
	int nRow, nSkip, nRun, nPos, nLeft, nRight, nCount;

	pbDst = pbRow;
	nRow = 0;
	while (nRow < nRows)
	{
		nSkip = *pbSrc++;
		nRun = *pbSrc++;
		if (!nSkip && !nRun)
		{
			nRow++;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			continue;
		}
		if (nY + nRow >= sgnClipBottom)
			return;
		pbDst += nSkip;
		if (nY + nRow >= sgnClipTop)
		{
			pbNextSrc = pbSrc + nRun;
			pbNextDst = pbDst + nRun;
			nPos = pbDst - pbRow + nX;
			nLeft = sgnClipLeft - nPos;
			if (nLeft <= 0)
				nLeft = 0;
			nRight = sgnClipRight - nPos;
			nRight = nRight > 0 ? nRight : 0;
			if (nRight < nRun)
				nRun = nRight;
			nCount = nRun - nLeft;
			if (nCount > 0)
			{
				pbDst += nLeft;
				pbSrc += nLeft;
				switch (nCount)
				{
				case 32: FLAT_PIXEL(31);
				case 31: FLAT_PIXEL(30);
				case 30: FLAT_PIXEL(29);
				case 29: FLAT_PIXEL(28);
				case 28: FLAT_PIXEL(27);
				case 27: FLAT_PIXEL(26);
				case 26: FLAT_PIXEL(25);
				case 25: FLAT_PIXEL(24);
				case 24: FLAT_PIXEL(23);
				case 23: FLAT_PIXEL(22);
				case 22: FLAT_PIXEL(21);
				case 21: FLAT_PIXEL(20);
				case 20: FLAT_PIXEL(19);
				case 19: FLAT_PIXEL(18);
				case 18: FLAT_PIXEL(17);
				case 17: FLAT_PIXEL(16);
				case 16: FLAT_PIXEL(15);
				case 15: FLAT_PIXEL(14);
				case 14: FLAT_PIXEL(13);
				case 13: FLAT_PIXEL(12);
				case 12: FLAT_PIXEL(11);
				case 11: FLAT_PIXEL(10);
				case 10: FLAT_PIXEL(9);
				case 9: FLAT_PIXEL(8);
				case 8: FLAT_PIXEL(7);
				case 7: FLAT_PIXEL(6);
				case 6: FLAT_PIXEL(5);
				case 5: FLAT_PIXEL(4);
				case 4: FLAT_PIXEL(3);
				case 3: FLAT_PIXEL(2);
				case 2: FLAT_PIXEL(1);
				case 1: FLAT_PIXEL(0);
				}
			}
			pbSrc = pbNextSrc;
			pbDst = pbNextDst;
		}
		else
		{
			pbDst += nRun;
			pbSrc += nRun;
		}
	}
}

void __fastcall CmnSubtileDrawGouraudRleClip(int nX, int nY, BYTE *pbSrc, BYTE *pbRow, int nLeft,
	int nLeftStep, int nRight, int nRightStep, int nRows, int nShift)
{
	BYTE *pbDst, *pbRun, *pbNextSrc, *pbNextDst;
	int nRow, nSkip, nRun, nPos, nClipLeft, nClipRight;

	pbDst = pbRow;
	pbRun = GOURAUD_ROW(nLeft, nRight, nShift);
	nRow = 0;
	while (nRow < nRows)
	{
		nSkip = *pbSrc++;
		nRun = *pbSrc++;
		if (!nSkip && !nRun)
		{
			nRow++;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			nLeft += nLeftStep;
			nRight += nRightStep;
			pbRun = GOURAUD_ROW(nLeft, nRight, nShift);
			continue;
		}
		if (nY + nRow >= sgnClipBottom)
			return;
		pbDst += nSkip;
		pbRun += nSkip;
		if (nY + nRow >= sgnClipTop)
		{
			pbNextSrc = pbSrc + nRun;
			pbNextDst = pbDst + nRun;
			nPos = pbDst - pbRow + nX;
			nClipLeft = sgnClipLeft - nPos;
			if (nClipLeft <= 0)
				nClipLeft = 0;
			nClipRight = sgnClipRight - nPos;
			nClipRight = nClipRight > 0 ? nClipRight : 0;
			if (nClipRight >= nRun)
				nClipRight = nRun;
			nClipRight -= nClipLeft;
			if (nClipRight > 0)
			{
				pbDst += nClipLeft;
				pbSrc += nClipLeft;
				switch (nClipRight)
				{
				case 32: GOURAUD_PIXEL(31);
				case 31: GOURAUD_PIXEL(30);
				case 30: GOURAUD_PIXEL(29);
				case 29: GOURAUD_PIXEL(28);
				case 28: GOURAUD_PIXEL(27);
				case 27: GOURAUD_PIXEL(26);
				case 26: GOURAUD_PIXEL(25);
				case 25: GOURAUD_PIXEL(24);
				case 24: GOURAUD_PIXEL(23);
				case 23: GOURAUD_PIXEL(22);
				case 22: GOURAUD_PIXEL(21);
				case 21: GOURAUD_PIXEL(20);
				case 20: GOURAUD_PIXEL(19);
				case 19: GOURAUD_PIXEL(18);
				case 18: GOURAUD_PIXEL(17);
				case 17: GOURAUD_PIXEL(16);
				case 16: GOURAUD_PIXEL(15);
				case 15: GOURAUD_PIXEL(14);
				case 14: GOURAUD_PIXEL(13);
				case 13: GOURAUD_PIXEL(12);
				case 12: GOURAUD_PIXEL(11);
				case 11: GOURAUD_PIXEL(10);
				case 10: GOURAUD_PIXEL(9);
				case 9: GOURAUD_PIXEL(8);
				case 8: GOURAUD_PIXEL(7);
				case 7: GOURAUD_PIXEL(6);
				case 6: GOURAUD_PIXEL(5);
				case 5: GOURAUD_PIXEL(4);
				case 4: GOURAUD_PIXEL(3);
				case 3: GOURAUD_PIXEL(2);
				case 2: GOURAUD_PIXEL(1);
				case 1: GOURAUD_PIXEL(0);
				}
			}
			pbSrc = pbNextSrc;
			pbDst = pbNextDst;
			pbRun += nRun;
		}
		else
		{
			pbDst += nRun;
			pbRun += nRun;
			pbSrc += nRun;
		}
	}
}

void __fastcall CmnSubtileBuildGouraud(void)
{
	int i, j, k, nStep, nVal;

	for (i = 0; i < 32; i++)
	{
		for (j = 0; j < 32; j++)
		{
			nStep = ((j - i) << GOURAUD_SHIFT) >> 5;
			nVal = i << GOURAUD_SHIFT;
			for (k = 0; k < 32; k++)
			{
				if (nVal <= 0)
					nVal = 0;
				else if (nVal >= 520093696)
					nVal = 520093696;
				sgabGouraud[i][j][k] = (BYTE)(nVal >> GOURAUD_SHIFT);
				nVal += nStep;
			}
#line 641
			D2ASSERT( ( nVal >> GOURAUD_SHIFT ) == j );
		}
	}
}

void __fastcall CmnSubtileSetClip(void)
{
	int nRight, nLeft;

	nRight = 640;
	nLeft = 0;
	if (gdwBlitParam == (DWORD)-160)
		nRight = 320;
	else if (gdwBlitParam == 160)
		nLeft = 320;
	sgnPartTop = -16;
	sgnPartBottom = 433;
	sgnPartLeft = nLeft - 32;
	sgnPartInRight = nRight - 32;
	sgnBlockInRight = nRight - 32;
	sgnPartRight = nRight;
	sgnPartInTop = 0;
	sgnPartInBottom = 417;
	sgnPartInLeft = nLeft;
	sgnBlockTop = -32;
	sgnBlockBottom = 433;
	sgnBlockLeft = nLeft - 32;
	sgnBlockRight = nRight;
	sgnBlockInTop = 0;
	sgnBlockInBottom = 401;
	sgnBlockInLeft = nLeft;
	sgnClipTop = 0;
	sgnClipBottom = 433;
	sgnClipLeft = nLeft;
	sgnClipRight = nRight;
}

void __fastcall CmnSubtileBuildRowOffsets(void)
{
	int nPitch, nOffset, i;

	nPitch = gtCmnInfo.nPitch;
	nOffset = -nPitch << 5;
	for (i = 0; i < 32 + 600; i++)
	{
		sganRowOffsets[i] = nOffset;
		nOffset += nPitch;
	}
}

void __fastcall CmnSubtileBlitBlock(BYTE *pbData, int nX, int nY)
{
	BYTE *pbRow, *pbDst;
	int nRow, nSkip, nRun, nPos, nLeft, nRight, nYRow;

	if (nY < sgnBlockTop || nY >= sgnBlockBottom || nX < sgnBlockLeft || nX >= sgnBlockRight)
		return;
#line 732
	D2ASSERT(gtCmnInfo.pbBuffer);
	pbRow = sganRowOffsets[nY + 32] + gtCmnInfo.pbBuffer + nX;
	pbDst = pbRow;
	if (nY >= sgnBlockInTop && nY < sgnBlockInBottom && nX >= sgnBlockInLeft && nX < sgnBlockInRight)
	{
		nRow = BLOCK_ROWS;
		while (nRow)
		{
			nSkip = *pbData++;
			nRun = *pbData++;
			if (!nSkip && !nRun)
			{
				nRow--;
				pbRow += gtCmnInfo.nPitch;
				pbDst = pbRow;
				continue;
			}
			pbDst += nSkip;
			memcpy(pbDst, pbData, nRun);
			pbDst += nRun;
			pbData += nRun;
		}
		return;
	}
	nYRow = nY;
	nRow = 0;
	while (nRow < BLOCK_ROWS)
	{
		nSkip = *pbData++;
		nRun = *pbData++;
		if (!nSkip && !nRun)
		{
			nRow++;
			nYRow++;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			continue;
		}
		if (nYRow >= sgnClipBottom)
			return;
		pbDst += nSkip;
		if (nYRow >= sgnClipTop)
		{
			nPos = pbDst - pbRow + nX;
			nLeft = sgnClipLeft - nPos;
			if (nLeft <= 0)
				nLeft = 0;
			nRight = sgnClipRight - nPos;
			nRight = nRight > 0 ? nRight : 0;
			if (nRight >= nRun)
				nRight = nRun;
			nRight -= nLeft;
			if (nRight > 0)
				memcpy(pbDst + nLeft, pbData + nLeft, nRight);
			pbData += nRun;
			pbDst += nRun;
		}
		else
		{
			pbDst += nRun;
			pbData += nRun;
		}
	}
}

void __fastcall CmnSubtileBlitBlockLit(BYTE *pbData, int nX, int nY, BYTE bLight)
{
	BYTE *pbTable, *pbDst;

	if (nY < sgnBlockTop || nY >= sgnBlockBottom || nX < sgnBlockLeft || nX >= sgnBlockRight)
		return;
	pbTable = gaabCmnLightTables[bLight >> 3];
	pbDst = sganRowOffsets[nY + 32] + gtCmnInfo.pbBuffer + nX;
#line 859
	D2ASSERT(gtCmnInfo.pbBuffer);
	if (nY >= sgnBlockInTop && nY < sgnBlockInBottom && nX >= sgnBlockInLeft && nX < sgnBlockInRight)
		CmnSubtileDrawFlatRle(pbData, pbDst, pbTable, BLOCK_ROWS);
	else
		CmnSubtileDrawFlatRleClip(nX, nY, pbData, pbDst, pbTable, BLOCK_ROWS);
}

void __fastcall CmnSubtileBlitBlockGouraud(BYTE *pbData, int nX, int nY, int *pnLight)
{
	BYTE *pbDst;
	int nLeft, nRight, nLeftStep, nRightStep, nDiff1, nDiff2, nDiff3, nT;
	BYTE bLight;

	if (sgfFlatLight)
	{
		bLight = (BYTE)pnLight[0];
		if ((bLight & 248) == 248)
			CmnSubtileBlitBlock(pbData, nX, nY);
		else
			CmnSubtileBlitBlockLit(pbData, nX, nY, bLight);
		return;
	}
	nLeft = pnLight[0] << 5;
	nLeftStep = pnLight[3] - pnLight[0];
	nRightStep = pnLight[2] - pnLight[1];
	nRight = pnLight[1] << 5;
	nT = pnLight[1] - pnLight[0];
	nDiff1 = nT < 0 ? -nT : nT;
	nDiff2 = nLeftStep;
	if (nDiff2 < 0)
		nDiff2 = -nDiff2;
	nDiff3 = nRightStep;
	if (nDiff3 < 0)
		nDiff3 = -nDiff3;
	if (gtGfxOptions.fFlatLighting || nDiff3 + nDiff2 + nDiff1 < 10)
	{
		if ((nLeft & ~255) == 7936)
			CmnSubtileBlitBlock(pbData, nX, nY);
		else
			CmnSubtileBlitBlockLit(pbData, nX, nY, (BYTE)pnLight[0]);
		return;
	}
	if (nY < sgnBlockTop || nY >= sgnBlockBottom || nX < sgnBlockLeft || nX >= sgnBlockRight)
		return;
#line 934
	D2ASSERT(gtCmnInfo.pbBuffer);
	pbDst = sganRowOffsets[nY + 32] + gtCmnInfo.pbBuffer + nX;
	if (nY >= sgnBlockInTop && nY < sgnBlockInBottom && nX >= sgnBlockInLeft && nX < sgnBlockInRight)
		CmnSubtileDrawGouraudRle(pbData, pbDst, nLeft, nLeftStep, nRight, nRightStep, BLOCK_ROWS, 8);
	else
		CmnSubtileDrawGouraudRleClip(nX, nY, pbData, pbDst, nLeft, nLeftStep, nRight, nRightStep, BLOCK_ROWS,
			8);
}

void __fastcall CmnSubtileBlitBlockAlpha(BYTE *pbData, int nX, int nY, BYTE bAlpha)
{
	BYTE *pbBlend, *pbRow, *pbDst, *pbEnd, *pbNextSrc, *pbNextDst;
	int nRow, nSkip, nRun, nPos, nLeft, nRight, nYRow, nCount;
	int nLines;

	if (bAlpha >= 192)
		pbBlend = gptCmnEnvInfo->pbBlendMode2;
	else if (bAlpha >= 128)
		pbBlend = gptCmnEnvInfo->pbBlendMode1;
	else if (bAlpha >= 64)
		pbBlend = gptCmnEnvInfo->pbBlendMode0;
	else
		return;
	if (nY < sgnBlockTop || nY >= sgnBlockBottom || nX < sgnBlockLeft || nX >= sgnBlockRight)
		return;
#line 992
	D2ASSERT(gtCmnInfo.pbBuffer);
	pbRow = sganRowOffsets[nY + 32] + gtCmnInfo.pbBuffer + nX;
	pbDst = pbRow;
	if (nY >= sgnBlockInTop && nY < sgnBlockInBottom && nX >= sgnBlockInLeft && nX < sgnBlockInRight)
	{
		nLines = BLOCK_ROWS;
		while (nLines)
		{
			nSkip = *pbData++;
			nRun = *pbData++;
			if (!nSkip && !nRun)
			{
				nLines--;
				pbRow += gtCmnInfo.nPitch;
				pbDst = pbRow;
				continue;
			}
			pbDst += nSkip;
			for (pbEnd = pbData + nRun; pbData < pbEnd; pbData++, pbDst++)
				*pbDst = pbBlend[(*pbDst << 8) + *pbData];
		}
		return;
	}
	nYRow = nY;
	nRow = 0;
	while (nRow < BLOCK_ROWS)
	{
		nSkip = *pbData++;
		nRun = *pbData++;
		if (!nSkip && !nRun)
		{
			nRow++;
			nYRow++;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			continue;
		}
		if (nYRow >= sgnClipBottom)
			return;
		pbDst += nSkip;
		if (nYRow >= sgnClipTop)
		{
			pbNextSrc = pbData + nRun;
			pbNextDst = pbDst + nRun;
			nPos = pbDst - pbRow + nX;
			nLeft = sgnClipLeft - nPos;
			if (nLeft <= 0)
				nLeft = 0;
			nRight = sgnClipRight - nPos;
			nRight = nRight > 0 ? nRight : 0;
			nCount = (nRight < nRun ? nRight : nRun) - nLeft;
			if (nCount > 0)
			{
				pbData += nLeft;
				pbDst += nLeft;
				for (pbEnd = pbData + nCount; pbData < pbEnd; pbData++, pbDst++)
					*pbDst = pbBlend[(*pbDst << 8) + *pbData];
			}
			pbData = pbNextSrc;
			pbDst = pbNextDst;
		}
		else
		{
			pbDst += nRun;
			pbData += nRun;
		}
	}
}

void __fastcall CmnSubtileBlitBlockGouraudAlpha(BYTE *pbData, int nX, int nY, int *pnLight, BYTE bAlpha)
{
	BYTE *pbBlend, *pbRow, *pbDst, *pbRun, *pbEnd, *pbNextSrc, *pbNextDst, *pbNextRun;
	int nLeft, nRight, nLeftStep, nRightStep, nRow, nPos, nClipLeft, nClipRight, nYRow;
	int nLines;

	if (bAlpha >= 192)
		pbBlend = gptCmnEnvInfo->pbBlendMode2;
	else if (bAlpha >= 128)
		pbBlend = gptCmnEnvInfo->pbBlendMode1;
	else if (bAlpha >= 64)
		pbBlend = gptCmnEnvInfo->pbBlendMode0;
	else
		return;
	nLeft = pnLight[0] << 5;
	nLeftStep = pnLight[3] - pnLight[0];
	nRight = pnLight[1] << 5;
	nRightStep = pnLight[2] - pnLight[1];
	if (nY < sgnBlockTop || nY >= sgnBlockBottom || nX < sgnBlockLeft || nX >= sgnBlockRight)
		return;
#line 1139
	D2ASSERT(gtCmnInfo.pbBuffer);
	pbRow = sganRowOffsets[nY + 32] + gtCmnInfo.pbBuffer + nX;
	pbDst = pbRow;
	pbRun = sgabGouraud[nLeft >> 8][nRight >> 8];
	if (nY >= sgnBlockInTop && nY < sgnBlockInBottom && nX >= sgnBlockInLeft && nX < sgnBlockInRight)
	{
		nLines = BLOCK_ROWS;
		while (nLines)
		{
			int nSkip, nRun;

			nSkip = *pbData++;
			nRun = *pbData++;
			if (!nSkip && !nRun)
			{
				nLines--;
				pbRow += gtCmnInfo.nPitch;
				pbDst = pbRow;
				nLeft += nLeftStep;
				nRight += nRightStep;
				pbRun = sgabGouraud[nLeft >> 8][nRight >> 8];
				continue;
			}
			pbDst += nSkip;
			pbRun += nSkip;
			for (pbEnd = pbData + nRun; pbData < pbEnd; pbDst++)
			{
				BYTE bLit = gaabCmnLightTables[*pbRun++][*pbData++];
				*pbDst = pbBlend[(bLit << 8) + *pbDst];
			}
		}
		return;
	}
	nRow = 0;
	nYRow = nY;
	while (nRow < BLOCK_ROWS)
	{
		int nSkip, nRun;

		nSkip = *pbData++;
		nRun = *pbData++;
		if (!nSkip && !nRun)
		{
			nRow++;
			nYRow++;
			pbRow += gtCmnInfo.nPitch;
			pbDst = pbRow;
			nLeft += nLeftStep;
			nRight += nRightStep;
			pbRun = sgabGouraud[nLeft >> 8][nRight >> 8];
			continue;
		}
		if (nYRow >= sgnClipBottom)
			return;
		pbDst += nSkip;
		pbRun += nSkip;
		if (nYRow >= sgnClipTop)
		{
			pbNextSrc = pbData + nRun;
			pbNextDst = pbDst + nRun;
			pbNextRun = pbRun + nRun;
			nPos = pbDst - pbRow + nX;
			nClipLeft = sgnClipLeft - nPos;
			if (nClipLeft <= 0)
				nClipLeft = 0;
			nClipRight = sgnClipRight - nPos;
			nClipRight = nClipRight > 0 ? nClipRight : 0;
			nRun = (nClipRight < nRun ? nClipRight : nRun) - nClipLeft;
			if (nRun > 0)
			{
				pbData += nClipLeft;
				pbDst += nClipLeft;
				for (pbEnd = pbData + nRun; pbData < pbEnd; pbDst++)
				{
					BYTE bLit = gaabCmnLightTables[*pbRun++][*pbData++];
					*pbDst = pbBlend[(bLit << 8) + *pbDst];
				}
			}
			pbData = pbNextSrc;
			pbDst = pbNextDst;
			pbRun = pbNextRun;
		}
		else
		{
			pbDst += nRun;
			pbRun += nRun;
			pbData += nRun;
		}
	}
}

void __fastcall CmnSubtileDrawDiamondFlat(D2GFXTILEPART *ptPart, BYTE *pbDst, BYTE bLight)
{
	BYTE *pbTable;

	pbTable = gaabCmnLightTables[bLight >> 3];
	if (ptPart->wFlags & 4)
		CmnSubtileDrawFlatRle(ptPart->ptData, pbDst, pbTable, SUBTILE_ROWS);
	else
		CmnSubtileDrawFlat(ptPart->ptData, pbDst, pbTable);
}

void __fastcall CmnSubtileDrawDiamondGouraud(D2GFXTILEPART *ptPart, BYTE *pbDst, int *pnLight)
{
	int nLeft = pnLight[0] << 4;
	int nLeftStep = pnLight[3] - pnLight[0];
	int nRight = pnLight[1] << 4;
	int nRightStep = pnLight[2] - pnLight[1];

	if (ptPart->wFlags & 4)
		CmnSubtileDrawGouraudRle(ptPart->ptData, pbDst, nLeft, nLeftStep, nRight, nRightStep, SUBTILE_ROWS, 7);
	else
		CmnSubtileDrawGouraud(ptPart->ptData, pbDst, nLeft, nLeftStep, nRight, nRightStep);
}

void __fastcall CmnSubtileDrawDiamondFlatClip(D2GFXTILEPART *ptPart, int nX, int nY, BYTE *pbDst,
	BYTE bLight)
{
	BYTE *pbTable, *pbRow, *pbOut, *pbSrc, *pbEnd;
	int nRow, nFirst, nEnd, nPos, nLeft, nRight;

	pbTable = gaabCmnLightTables[bLight >> 3];
	if (ptPart->wFlags & 4)
	{
		CmnSubtileDrawFlatRleClip(nX, nY, ptPart->ptData, pbDst, pbTable, SUBTILE_ROWS);
		return;
	}
	nFirst = sgnClipTop - nY;
	nFirst = nFirst > 0 ? nFirst : 0;
	nEnd = sgnClipBottom - nY < SUBTILE_ROWS ? sgnClipBottom - nY : SUBTILE_ROWS;
	pbRow = gtCmnInfo.nPitch * nFirst + pbDst;
	for (nRow = nFirst; nRow < nEnd; nRow++)
	{
		pbOut = pbRow + sgcanRowSkip[nRow];
		nPos = sgcanRowSkip[nRow] + nX;
		pbSrc = ptPart->ptData + sgcanRowOffset[nRow];
		nLeft = sgnClipLeft - nPos;
		if (nLeft <= 0)
			nLeft = 0;
		nRight = sgnClipRight - nPos;
		nRight = nRight > 0 ? nRight : 0;
		if (nRight >= sgcanRowWidth[nRow])
			nRight = sgcanRowWidth[nRow];
		nRight -= nLeft;
		if (nRight > 0)
		{
			pbSrc += nLeft;
			pbOut += nLeft;
			for (pbEnd = pbSrc + nRight; pbSrc < pbEnd; pbOut++, pbSrc++)
				*pbOut = pbTable[*pbSrc];
		}
		pbRow += gtCmnInfo.nPitch;
	}
}

void __fastcall CmnSubtileDrawDiamondGouraudClip(D2GFXTILEPART *ptPart, int nX, int nY, BYTE *pbDst,
	int *pnLight)
{
	BYTE *pbRow, *pbOut, *pbSrc, *pbRun, *pbEnd;
	int nLeft, nRight, nLeftStep, nRightStep, nRow, nFirst, nEnd, nPos, nClipLeft, nClipRight;

	nLeft = pnLight[0] << 4;
	nLeftStep = pnLight[3] - pnLight[0];
	nRight = pnLight[1] << 4;
	nRightStep = pnLight[2] - pnLight[1];
	if (ptPart->wFlags & 4)
	{
		CmnSubtileDrawGouraudRleClip(nX, nY, ptPart->ptData, pbDst, nLeft, nLeftStep, nRight, nRightStep,
			SUBTILE_ROWS, 7);
		return;
	}
	nFirst = sgnClipTop - nY;
	nFirst = nFirst > 0 ? nFirst : 0;
	nEnd = sgnClipBottom - nY < SUBTILE_ROWS ? sgnClipBottom - nY : SUBTILE_ROWS;
	pbRow = gtCmnInfo.nPitch * nFirst + pbDst;
	for (nRow = nFirst; nRow < nEnd; nRow++)
	{
		nPos = nX + sgcanRowSkip[nRow];
		pbOut = pbRow + sgcanRowSkip[nRow];
		pbRun = &sgabGouraud[nLeft >> 7][nRight >> 7][sgcanRowSkip[nRow]];
		pbSrc = ptPart->ptData + sgcanRowOffset[nRow];
		nClipLeft = sgnClipLeft - nPos;
		if (nClipLeft <= 0)
			nClipLeft = 0;
		nClipRight = sgnClipRight - nPos;
		nClipRight = nClipRight > 0 ? nClipRight : 0;
		if (nClipRight >= sgcanRowWidth[nRow])
			nClipRight = sgcanRowWidth[nRow];
		nClipRight -= nClipLeft;
		if (nClipRight > 0)
		{
			pbSrc += nClipLeft;
			pbOut += nClipLeft;
			for (pbEnd = pbSrc + nClipRight; pbSrc < pbEnd; pbOut++, pbSrc++, pbRun++)
				*pbOut = gaabCmnLightTables[*pbRun][*pbSrc];
		}
		pbRow += gtCmnInfo.nPitch;
		nLeft += nLeftStep;
		nRight += nRightStep;
	}
}

static __inline int Abs(int n)
{
	return n < 0 ? -n : n;
}

void __fastcall CmnSubtileDrawTile(D2GFXTILE *ptTile, int nX, int nY, D2GFXLIGHT (*ptLight)[8])
{
	D2GFXTILEPART *ptPart;
	D2GFXLIGHT (*ptCell)[8];
	BYTE *pbOrigin, *pbDst, *pbTable;
	int i, nPartX, nPartY, nDiff1, nDiff2, nDiff3;
	int nLight01, nLight02, nLight10, nLight11, nLight12, nLight13, nLight20, nLight21, nLight22, nLight23;
	int nLight31, nLight32;
	int nLeft, nLeftStep, nRight, nRightStep;
	D2GFXLIGHT *pt;
	int *pnLight;
	BOOL fFlat;
	BYTE bLight;

	pbOrigin = gtCmnInfo.pbBuffer + gtCmnInfo.nPitch * nY + nX;
	for (i = 0; i < ptTile->nCompCount; i++)
	{
		ptPart = &ptTile->ptCompArray[i];
		pbDst = pbOrigin + ptPart->sOffsetY * gtCmnInfo.nPitch + ptPart->sOffsetX;
		if (ptPart->wFlags & 1)
		{
			nPartX = ptPart->sOffsetX + nX;
			nPartY = ptPart->sOffsetY + nY;
			if (nPartY >= sgnPartTop && nPartY < sgnPartBottom && nPartX >= sgnPartLeft
				&& nPartX < sgnPartRight)
			{
				int anCorner[4];

				fFlat = FALSE;
				bLight = 0;
				if (sgfFlatLight)
				{
					fFlat = TRUE;
					bLight = (BYTE)ptLight[ptPart->bRow + 1][ptPart->bColumn + 1].nLight;
				}
				else
				{
					ptCell = (D2GFXLIGHT (*)[8])&ptLight[ptPart->bRow][ptPart->bColumn];
					nLight10 = ptCell[1][0].nLight;
					pt = &ptCell[1][1];
					nLight11 = pt->nLight;
					pt++;
					nLight12 = pt[0].nLight;
					nLight13 = pt[1].nLight;
					nLight20 = ptCell[2][0].nLight;
					pt = &ptCell[2][1];
					nLight21 = pt->nLight;
					pt++;
					nLight22 = pt[0].nLight;
					nLight23 = pt[1].nLight;
					nDiff1 = Abs(nLight12 - nLight11);
					nDiff2 = Abs(nLight21 - nLight11);
					nDiff3 = Abs(nLight22 - nLight12);
					if (gtGfxOptions.fFlatLighting || nDiff3 + nDiff2 + nDiff1 < 10)
					{
						fFlat = TRUE;
						bLight = (BYTE)nLight11;
					}
					else
					{
						pt = &ptCell[0][1];
						nLight01 = pt[0].nLight;
						nLight02 = pt[1].nLight;
						pt = &ptCell[3][1];
						nLight31 = pt[0].nLight;
						nLight32 = pt[1].nLight;
						anCorner[0] = (nLight10 + nLight11 + nLight20 + nLight21) >> 2;
						anCorner[1] = (nLight11 + nLight12 + nLight02 + nLight01) >> 2;
						anCorner[2] = (nLight12 + nLight13 + nLight21 + nLight23) >> 2;
						anCorner[3] = (nLight21 + nLight22 + nLight32 + nLight31) >> 2;
					}
				}
				if (nPartY >= sgnPartInTop && nPartY < sgnPartInBottom && nPartX >= sgnPartInLeft
					&& nPartX < sgnPartInRight)
				{
					if (fFlat)
					{
						pbTable = gaabCmnLightTables[bLight >> 3];
						if (ptPart->wFlags & 4)
							CmnSubtileDrawFlatRle(ptPart->ptData, pbDst, pbTable, SUBTILE_ROWS);
						else
							CmnSubtileDrawFlat(ptPart->ptData, pbDst, pbTable);
					}
					else
					{
						pnLight = anCorner;
						nLeft = pnLight[0] << 4;
						nLeftStep = pnLight[3] - pnLight[0];
						nRight = pnLight[1] << 4;
						nRightStep = pnLight[2] - pnLight[1];
						if (ptPart->wFlags & 4)
							CmnSubtileDrawGouraudRle(ptPart->ptData, pbDst, nLeft, nLeftStep, nRight, nRightStep,
								SUBTILE_ROWS, 7);
						else
							CmnSubtileDrawGouraud(ptPart->ptData, pbDst, nLeft, nLeftStep, nRight, nRightStep);
					}
				}
				else if (fFlat)
					CmnSubtileDrawDiamondFlatClip(ptPart, nPartX, nPartY, pbDst, bLight);
				else
					CmnSubtileDrawDiamondGouraudClip(ptPart, nPartX, nPartY, pbDst, anCorner);
			}
		}
	}
}
