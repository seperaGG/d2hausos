#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\DrlgGrid.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "Drlg.h"

static void __fastcall DrlgGridOpSet(int *pnCell, int nValue)
{
	*pnCell = nValue;
}

static void __fastcall DrlgGridOpOr(int *pnCell, int nValue)
{
	*pnCell |= nValue;
}

static void __fastcall DrlgGridOpAnd(int *pnCell, int nValue)
{
	*pnCell &= nValue;
}

static void __fastcall DrlgGridOpXor(int *pnCell, int nValue)
{
	*pnCell ^= nValue;
}

static void __fastcall DrlgGridOpSetIfZero(int *pnCell, int nValue)
{
	*pnCell = *pnCell ? *pnCell : nValue;
}

static void __fastcall DrlgGridOpClear(int *pnCell, int nValue)
{
	*pnCell &= ~nValue;
}

BOOL __fastcall DrlgGridIsSet(DRLGGRID *ptGrid)
{
	if (ptGrid && ptGrid->pnCells)
		return TRUE;
	return FALSE;
}

BOOL __fastcall DrlgGridInBounds(DRLGGRID *ptGrid, int nX, int nY)
{
	if (nX >= 0 && nX < ptGrid->nWidth && nY >= 0 && nY < ptGrid->nHeight)
		return TRUE;
	return FALSE;
}

typedef void (__fastcall *PFNDRLGGRIDOP)(int *pnCell, int nValue);
static PFNDRLGGRIDOP scpfnGridFunction[6] =
{
	DrlgGridOpOr, DrlgGridOpAnd, DrlgGridOpXor, DrlgGridOpSet, DrlgGridOpSetIfZero, DrlgGridOpClear,
};

void __fastcall DrlgGridApply(DRLGGRID *ptGrid, int nX, int nY, int nValue, int nOp)
{
	scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[nY] + nX], nValue);
}

void __fastcall DrlgGridApplyAtPoint(DRLGGRID *ptGrid, DRLGVERTEX *ptPoint, int nValue, int nOp)
{
	scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[ptPoint->nY] + ptPoint->nX],
		nValue);
}

int *__fastcall DrlgGridGetCell(DRLGGRID *ptGrid, int nX, int nY)
{
	return &ptGrid->pnCells[ptGrid->pnRowStarts[nY] + nX];
}

int __fastcall DrlgGridGet(DRLGGRID *ptGrid, int nX, int nY)
{
	return ptGrid->pnCells[ptGrid->pnRowStarts[nY] + nX];
}

void __fastcall DrlgGridApplyAll(DRLGGRID *ptGrid, int nValue, int nOp)
{
	int nY;
	int nX;
	int *pnCell;

	for (nY = 0; nY < ptGrid->nHeight; nY++)
	{
		pnCell = &ptGrid->pnCells[ptGrid->pnRowStarts[nY]];
		for (nX = 0; nX < ptGrid->nWidth; nX++)
		{
			scpfnGridFunction[nOp](pnCell, nValue);
			pnCell++;
		}
	}
}

void __fastcall DrlgGridApplyBorder(DRLGGRID *ptGrid, int nValue, int nOp)
{
	int *pnTop;
	int *pnBottom;
	int i;

	pnTop = &ptGrid->pnCells[ptGrid->pnRowStarts[0]];
	pnBottom = &ptGrid->pnCells[ptGrid->pnRowStarts[ptGrid->nHeight - 1]];
	for (i = 0; i < ptGrid->nWidth; i++)
	{
		scpfnGridFunction[nOp](pnTop, nValue);
		pnTop++;
		scpfnGridFunction[nOp](pnBottom, nValue);
		pnBottom++;
	}
	for (i = 1; i < ptGrid->nHeight; i++)
	{
		scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[i]], nValue);
		scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[i] + ptGrid->nWidth - 1], nValue);
	}
}

void __fastcall DrlgGridApplyFarEdges(DRLGGRID *ptGrid, int nValue, int nOp)
{
	int *pnCell;
	int i;

	pnCell = &ptGrid->pnCells[ptGrid->pnRowStarts[ptGrid->nHeight - 1]];
	for (i = 0; i < ptGrid->nWidth - 1; i++)
	{
		scpfnGridFunction[nOp](pnCell, nValue);
		pnCell++;
	}
	for (i = 0; i < ptGrid->nHeight; i++)
		scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[i] + ptGrid->nWidth - 1], nValue);
}

void __fastcall DrlgGridApplyLine(DRLGGRID *ptGrid, DRLGVERTEX *ptVertex, int nValue, int nOp, BOOL fEnd)
{
	DRLGVERTEX *ptNext;
	int nX;
	int nY;
	int nEndX;
	int nEndY;
	BOOL fAlongX;

	ptNext = ptVertex->ptNext;
	if (ptVertex->nX == ptNext->nX && ptVertex->nY == ptNext->nY)
	{
		scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[ptVertex->nY] + ptVertex->nX],
			nValue);
		return;
	}
	if (ptVertex->nX == ptNext->nX)
	{
		nX = ptVertex->nX;
		nEndX = ptVertex->nX;
		fAlongX = FALSE;
		if (ptVertex->nY < ptNext->nY)
		{
			nY = ptVertex->nY;
			nEndY = ptNext->nY;
		}
		else
		{
			nY = ptNext->nY;
			nEndY = ptVertex->nY;
		}
	}
	else
	{
		fAlongX = TRUE;
		nY = ptVertex->nY;
		nEndY = ptVertex->nY;
		if (ptVertex->nX < ptNext->nX)
		{
			nX = ptVertex->nX;
			nEndX = ptNext->nX;
		}
		else
		{
			nX = ptNext->nX;
			nEndX = ptVertex->nX;
		}
	}
	if (fAlongX)
		nX++;
	else
		nY++;
	while (nX != nEndX || nY != nEndY)
	{
		scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[nY] + nX], nValue);
		if (fAlongX)
			nX++;
		else
			nY++;
	}
	scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[ptVertex->nY] + ptVertex->nX],
		nValue);
	if (fEnd)
	{
		ptNext = ptVertex->ptNext;
		scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[ptNext->nY] + ptNext->nX],
			nValue);
	}
}

void __fastcall DrlgGridOrPoints(DRLGGRID *ptGrid, DRLGVERTEX *ptPoint, int nValue)
{
	DRLGVERTEX *ptVertex;
	int nX;
	int nY;

	ptVertex = ptPoint;
	while (ptVertex)
	{
		nX = ptVertex->nX;
		nY = ptVertex->nY;
		ptVertex = ptVertex->ptNext;
		if (nX >= 0 && nX < ptGrid->nWidth && nY >= 0 && nY < ptGrid->nHeight)
			scpfnGridFunction[0](&ptGrid->pnCells[ptGrid->pnRowStarts[nY] + nX], nValue);
	}
}

void __fastcall DrlgGridApplyThickLine(DRLGGRID *ptGrid, DRLGVERTEX *ptVertex, DRLGCOORDS *ptRect, int nValue,
	int nOp, int nWidth)
{
	int nX;
	int nY;
	int nDX;
	int nDY;
	int nStepX;
	int nStepY;
	int nError;
	int nCol;
	int nRow;
	int n;
	int i;
	int j;

	nX = ptVertex->nX;
	nY = ptVertex->nY;
	nDX = ptVertex->ptNext->nX - nX;
	nDY = ptVertex->ptNext->nY - nY;
	if (nDX < 0)
	{
		nStepX = -1;
		nDX = -nDX;
	}
	else
		nStepX = 1;
	if (nDY < 0)
	{
		nStepY = -1;
		nDY = -nDY;
	}
	else
		nStepY = 1;
	nCol = nX - ptRect->nTileX;
	nRow = nY - ptRect->nTileY;
	nError = 0;
	if (nDX < nDY)
	{
		for (i = 0; i < nWidth; i++)
		{
			if (DrlgRectContains(ptRect, nX + i, nY))
				scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[nRow] + i + nCol], nValue);
		}
		for (n = nDY; n > 0; n--)
		{
			nY += nStepY;
			nError += nDX;
			if (nError > nDY)
			{
				nX += nStepX;
				nError -= nDY;
			}
			nCol = nX - ptRect->nTileX;
			nRow = nY - ptRect->nTileY;
			for (i = 0; i < nWidth; i++)
			{
				if (DrlgRectContains(ptRect, nX + i, nY))
					scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[nRow] + i + nCol], nValue);
			}
		}
	}
	else
	{
		for (j = 0; j < nWidth; j++)
		{
			if (DrlgRectContains(ptRect, nX, nY + j))
				scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[nRow + j] + nCol], nValue);
		}
		for (n = nDX; n > 0; n--)
		{
			nX += nStepX;
			nError += nDY;
			if (nError > nDX)
			{
				nY += nStepY;
				nError -= nDX;
			}
			nCol = nX - ptRect->nTileX;
			nRow = nY - ptRect->nTileY;
			for (j = 0; j < nWidth; j++)
			{
				if (DrlgRectContains(ptRect, nX, nY + j))
					scpfnGridFunction[nOp](&ptGrid->pnCells[ptGrid->pnRowStarts[nRow + j] + nCol], nValue);
			}
		}
	}
}

void __fastcall DrlgGridCopy(DRLGGRID *ptDest, DRLGGRID *ptSource)
{
	memcpy(ptDest->pnCells, ptSource->pnCells, ptSource->nHeight * ptSource->nWidth * sizeof(int));
}

void __fastcall DrlgGridAlloc(void *pPool, DRLGGRID *ptGrid, int nX, int nY)
{
	int i;
	int nOffset;
	int nSize;

	ptGrid->nWidth = nX;
	nSize = (nX + 1) * nY * sizeof(int);
	ptGrid->nHeight = nY;
#line 406
	ptGrid->pnRowStarts = (int *)D2ALLOCPOOL(pPool, nSize);
	memset(ptGrid->pnRowStarts, 0, nSize);
	ptGrid->pnCells = &ptGrid->pnRowStarts[nY];
	nOffset = 0;
	for (i = 0; i < nY; i++)
	{
		ptGrid->pnRowStarts[i] = nOffset;
		nOffset += nX;
	}
	ptGrid->dwIsView = 0;
}

void __fastcall DrlgGridInit(DRLGGRID *ptGrid, int nX, int nY, int *pnCells, int *pnRowOffsets)
{
	int i;
	int nOffset;

	ptGrid->nWidth = nX;
	ptGrid->nHeight = nY;
	ptGrid->pnCells = pnCells;
	memset(pnCells, 0, nX * nY * sizeof(int));
	ptGrid->pnRowStarts = pnRowOffsets;
	nOffset = 0;
	for (i = 0; i < nY; i++)
	{
		ptGrid->pnRowStarts[i] = nOffset;
		nOffset += nX;
	}
	ptGrid->dwIsView = 0;
}

void __fastcall DrlgGridAllocRows(void *pPool, DRLGGRID *ptGrid, int *pnCells, int nX, int nY)
{
	int nOffset;
	int i;

	ptGrid->nWidth = nX;
	ptGrid->nHeight = nY;
	ptGrid->pnCells = pnCells;
#line 463
	ptGrid->pnRowStarts = (int *)D2ALLOCPOOL(pPool, nY * sizeof(int));
	nOffset = 0;
	for (i = 0; i < nY; i++)
	{
		ptGrid->pnRowStarts[i] = nOffset;
		nOffset += nX;
	}
	ptGrid->dwIsView = 1;
}

void __fastcall DrlgGridAllocView(void *pPool, DRLGGRID *ptGrid, int *pnCells, DRLGCOORDS *ptCoords,
	int nStride)
{
	int i;
	int nOffset;

	ptGrid->nWidth = ptCoords->nSizeTileX;
	ptGrid->nHeight = ptCoords->nSizeTileY;
	ptGrid->pnCells = &pnCells[ptCoords->nTileY * nStride + ptCoords->nTileX];
#line 491
	ptGrid->pnRowStarts = (int *)D2ALLOCPOOL(pPool, ptCoords->nSizeTileY * sizeof(int));
	nOffset = 0;
	for (i = 0; i < ptCoords->nSizeTileY; i++)
	{
		ptGrid->pnRowStarts[i] = nOffset;
		nOffset += nStride;
	}
	ptGrid->dwIsView = 1;
}

void __fastcall DrlgGridInitView(DRLGGRID *ptGrid, int *pnCells, DRLGCOORDS *ptCoords, int nStride,
	int *pnRowOffsets)
{
	int i;
	int nOffset;

	ptGrid->nWidth = ptCoords->nSizeTileX;
	ptGrid->nHeight = ptCoords->nSizeTileY;
	ptGrid->pnCells = &pnCells[ptCoords->nTileY * nStride + ptCoords->nTileX];
	ptGrid->pnRowStarts = pnRowOffsets;
	nOffset = 0;
	for (i = 0; i < ptCoords->nSizeTileY; i++)
	{
		ptGrid->pnRowStarts[i] = nOffset;
		nOffset += nStride;
	}
	ptGrid->dwIsView = 1;
}

void __fastcall DrlgGridFree(void *pPool, DRLGGRID *ptGrid)
{
	if (ptGrid->pnRowStarts)
	{
#line 535
		D2FREEPOOL(pPool, ptGrid->pnRowStarts);
	}
	ptGrid->pnCells = NULL;
	ptGrid->pnRowStarts = NULL;
}

void __fastcall DrlgGridClear(DRLGGRID *ptGrid)
{
	ptGrid->pnCells = NULL;
	ptGrid->pnRowStarts = NULL;
}
