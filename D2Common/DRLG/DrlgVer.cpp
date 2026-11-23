#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\DrlgVer.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "Drlg.h"

DRLGVERTEX *__fastcall DrlgVerNew(void *pPool, BYTE bFlag)
{
	DRLGVERTEX *ptVertex;
	DWORD i;

#line 18
	ptVertex = (DRLGVERTEX *)D2ALLOCPOOL(pPool, sizeof(DRLGVERTEX));
	for (i = 0; i < sizeof(DRLGVERTEX) / sizeof(DWORD); i++)
		((DWORD *)ptVertex)[i] = 0;
	ptVertex->bConcaveRun = bFlag;
	return ptVertex;
}

static inline void DrlgVerSplitEdges(void *pPool, DRLGVERTEX *ptBottomLeft, DRLGCOORDS *ptCoords, BYTE bFlag,
	DRLGLEVELLINK *ptLink)
{
	DRLGCOORDS *ptRect;
	DRLGVERTEX *ptTopLeft;
	DRLGVERTEX *ptTopRight;
	DRLGVERTEX *ptBottomRight;
	DRLGVERTEX *ptStart;
	DRLGVERTEX *ptEnd;
	DRLGVERTEX *ptNew;
	BYTE fAlongY;
	int nSign;
	int nBegin;
	int nEnd;
	int nFrom;
	int nTo;
	BOOL fPreset;

	fAlongY = FALSE;
	nSign = 0;
	ptStart = NULL;
	ptEnd = NULL;
	nBegin = 0;
	nEnd = 0;
	ptTopLeft = ptBottomLeft->ptNext;
	ptTopRight = ptBottomLeft->ptNext->ptNext;
	ptBottomRight = ptBottomLeft->ptNext->ptNext->ptNext;
	for (; ptLink; ptLink = ptLink->ptNext)
	{
		ptRect = ptLink->ptCoords;
		if (!ptRect)
			ptRect = ptCoords;
		switch (ptLink->nDirection)
		{
		case 0:
			fAlongY = TRUE;
			nSign = -1;
			ptStart = ptBottomLeft;
			ptEnd = ptTopLeft;
			nBegin = ptRect->nTileY + ptRect->nSizeTileY;
			nEnd = ptRect->nTileY;
			break;
		case 1:
			fAlongY = FALSE;
			nSign = 1;
			ptStart = ptTopLeft;
			ptEnd = ptTopRight;
			nBegin = ptRect->nTileX;
			nEnd = ptRect->nTileX + ptRect->nSizeTileX;
			break;
		case 2:
			fAlongY = TRUE;
			nSign = 1;
			ptStart = ptTopRight;
			ptEnd = ptBottomRight;
			nBegin = ptRect->nTileY;
			nEnd = ptRect->nTileY + ptRect->nSizeTileY;
			break;
		case 3:
			fAlongY = FALSE;
			nSign = -1;
			ptStart = ptBottomRight;
			ptEnd = ptBottomLeft;
			nBegin = ptRect->nTileX + ptRect->nSizeTileX;
			nEnd = ptRect->nTileX;
			break;
		default:
#line 108
			D2WARN(FALSE);
			break;
		}
		if (fAlongY)
		{
			nFrom = ptStart->nY;
			nTo = ptEnd->nY;
		}
		else
		{
			nFrom = ptStart->nX;
			nTo = ptEnd->nX;
		}
		if (nBegin * nSign <= nFrom * nSign)
		{
			if (nEnd * nSign < nFrom * nSign)
				continue;
		}
		else
		{
			if (nBegin * nSign > nTo * nSign)
				continue;
			ptNew = DrlgVerNew(pPool, bFlag);
			if (fAlongY)
			{
				ptNew->nX = ptStart->nX;
				ptNew->nY = nBegin;
			}
			else
			{
				ptNew->nY = ptStart->nY;
				ptNew->nX = nBegin;
			}
			ptNew->ptNext = ptStart->ptNext;
			ptStart->ptNext = ptNew;
			ptStart = ptNew;
		}
		fPreset = ptLink->nPresetNeighbour;
		ptStart->dwFlags |= 1;
		if (fPreset)
			ptStart->dwFlags |= 2;
		if (nEnd * nSign >= nTo * nSign)
			continue;
		ptNew = DrlgVerNew(pPool, bFlag);
		if (fAlongY)
		{
			ptNew->nX = ptStart->nX;
			ptNew->nY = nEnd;
		}
		else
		{
			ptNew->nY = ptStart->nY;
			ptNew->nX = nEnd;
		}
		ptNew->ptNext = ptStart->ptNext;
		ptStart->ptNext = ptNew;
	}
}

void __fastcall DrlgVerBuildOutline(void *pPool, DRLGVERTEX **pptRing, DRLGCOORDS *ptCoords, BYTE bFlag,
	DRLGLEVELLINK *ptLinks)
{
	DRLGLEVELLINK *ptLink;
	DRLGVERTEX *ptBottomLeft;
	DRLGVERTEX *ptTopLeft;
	DRLGVERTEX *ptTopRight;
	DRLGVERTEX *ptBottomRight;
	DRLGVERTEX *ptVertex;

	ptCoords->nSizeTileX--;
	ptCoords->nSizeTileY--;
	for (ptLink = ptLinks; ptLink; ptLink = ptLink->ptNext)
	{
		ptLink->ptCoords->nSizeTileX--;
		ptLink->ptCoords->nSizeTileY--;
	}
	*pptRing = DrlgVerNew(pPool, bFlag);
	ptBottomLeft = *pptRing;
	ptBottomLeft->nX = ptCoords->nTileX;
	ptBottomLeft->nY = ptCoords->nTileY + ptCoords->nSizeTileY;
	ptTopLeft = DrlgVerNew(pPool, bFlag);
	ptBottomLeft->ptNext = ptTopLeft;
	ptTopLeft->nX = ptCoords->nTileX;
	ptTopLeft->nY = ptCoords->nTileY;
	ptTopRight = DrlgVerNew(pPool, bFlag);
	ptTopLeft->ptNext = ptTopRight;
	ptTopRight->nX = ptCoords->nTileX + ptCoords->nSizeTileX;
	ptTopRight->nY = ptCoords->nTileY;
	ptBottomRight = DrlgVerNew(pPool, bFlag);
	ptTopRight->ptNext = ptBottomRight;
	ptBottomRight->nX = ptCoords->nTileX + ptCoords->nSizeTileX;
	ptBottomRight->nY = ptCoords->nTileY + ptCoords->nSizeTileY;
	ptBottomRight->ptNext = *pptRing;
	DrlgVerSplitEdges(pPool, *pptRing, ptCoords, bFlag, ptLinks);
	ptBottomLeft = *pptRing;
	ptVertex = ptBottomLeft;
	do
	{
		ptVertex->nX -= ptCoords->nTileX;
		ptVertex->nY -= ptCoords->nTileY;
		ptVertex = ptVertex->ptNext;
	}
	while (ptVertex != ptBottomLeft);
	ptCoords->nSizeTileX++;
	ptCoords->nSizeTileY++;
	for (ptLink = ptLinks; ptLink; ptLink = ptLink->ptNext)
	{
		ptLink->ptCoords->nSizeTileX++;
		ptLink->ptCoords->nSizeTileY++;
	}
}

void __fastcall DrlgVerFreeRing(void *pPool, DRLGVERTEX **pptRing)
{
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptFree;

	if (*pptRing == NULL)
		return;
	ptVertex = (*pptRing)->ptNext;
	if (ptVertex)
	{
		do
		{
			ptFree = ptVertex;
			ptVertex = ptVertex->ptNext;
#line 275
			D2FREEPOOL(pPool, ptFree);
			if (ptVertex == *pptRing)
			{
#line 278
				D2FREEPOOL(pPool, ptVertex);
				break;
			}
		}
		while (ptVertex);
	}
	if (ptVertex == NULL)
	{
#line 286
		D2FREEPOOL(pPool, *pptRing);
	}
	*pptRing = NULL;
}

void __fastcall DrlgVerReverse(DRLGVERTEX **pptList)
{
	DRLGVERTEX *ptVertex;
	DRLGVERTEX *ptPrev;
	DRLGVERTEX *ptNext;

	ptPrev = NULL;
	for (ptVertex = *pptList; ptVertex; ptVertex = ptNext)
	{
		ptNext = ptVertex->ptNext;
		ptVertex->ptNext = ptPrev;
		ptPrev = ptVertex;
	}
	*pptList = ptPrev;
}

void __fastcall DrlgVerGetStep(DRLGVERTEX *ptVertex, int *pnDX, int *pnDY)
{
	*pnDX = ptVertex->ptNext->nX - ptVertex->nX;
	*pnDY = ptVertex->ptNext->nY - ptVertex->nY;
	if (*pnDX < 0)
		*pnDX = -1;
	else if (*pnDX > 0)
		*pnDX = 1;
	if (*pnDY < 0)
		*pnDY = -1;
	else if (*pnDY > 0)
		*pnDY = 1;
}

DUNGEONROOM *__fastcall DrlgRoomGetWarpDest(ROOM *ptRoom, BYTE bExit, BYTE *pbExit, PTLVLWARPTXT *pptWarpDef)
{
	PTDRLGROOMWARP ptWarp;
	ROOM *ptDest;

	for (ptWarp = ptRoom->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
	{
		if (ptWarp->bExit == bExit)
			break;
	}
	if (ptWarp)
	{
		ptDest = ptWarp->ptDestRoom;
		for (ptWarp = ptDest->ptWarps; ptWarp; ptWarp = ptWarp->ptNext)
		{
			if (ptWarp->ptDestRoom == ptRoom)
				break; }
		if (!ptWarp)
			return NULL;
		*pbExit = ptWarp->bExit;
		*pptWarpDef = ptWarp->nWarpId == -1 ? NULL : LvlTblsGetWarpDef(ptWarp->nWarpId);
		if (!ptDest->ptGameRoom)
			DrlgRoomBuild(ptDest);
		return ptDest->ptGameRoom; }
	return NULL;
}
