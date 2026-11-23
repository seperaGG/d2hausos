#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\COLLISN\\Collisn.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "StormImports.h"
#include "D2Macros.h"
#include "../DRLG/Drlg.h"

#define NUM_COLPAT		4
#define NUM_COLLISION_SIZES	4
#define COL_NUM_ROOM_RECTS	3

#define DungeonTestRoomGame(hRoom, x, y) ((hRoom) != NULL && \
	(x) >= (hRoom)->tRect.nX && (x) < (hRoom)->tRect.nX + (hRoom)->tRect.nWidth && \
	(y) >= (hRoom)->tRect.nY && (y) < (hRoom)->tRect.nY + (hRoom)->tRect.nHeight)
static void __fastcall CollisnStampTile(COL *ptCol, void *hTile, int nX, int nY);
static void __fastcall CollisnStampTiles(COL *ptCol, COL *ptSrcCol, PTDRLGTILE ptTiles, int nCount,
	BOOL fReplace);

void __cdecl operator delete(void *pMemory)
{
	if (pMemory)
		StormSMemFree(pMemory, "delete", -1, 0);
}

void * __cdecl operator new(unsigned int nSize)
{
	return StormSMemAlloc(nSize, "new", -1, 0);
}

static __inline int CollisnNotBelowZero(int nValue)
{
	if (nValue > 0)
		return nValue;
	return 0;
}

static void CollisnClearTile(COL *ptCol, void *hTile, int nX, int nY)
{
	BYTE *pbMask;
	BYTE *pbRow;
	BYTE *pbSrc;
	WORD *pwRow;
	int nStartX;
	int nStartY;
	int nEndX;
	int nEndY;
	int x;
	int y;

	pbMask = D2CMPTileGetCollisionMask(hTile);
	nStartY = CollisnNotBelowZero(nY);
	nEndY = nY + 5;
	nEndX = nX + 5;
	nStartX = CollisnNotBelowZero(nX);
	pbRow = &pbMask[(nY + 4 - nStartY) * 5 + nStartX - nX];
	for (y = nStartY; y < nEndY; y++)
	{
		pwRow = &ptCol->pwGrid[ptCol->pdwRowOffset[y]];
		pbSrc = pbRow;
		for (x = nStartX; x < nEndX; x++)
			pwRow[x] &= ~*pbSrc++;
		pbRow -= 5;
	}
}

void __fastcall CollisnReplaceTile(DUNGEONROOM *ptGameRoom, PTDRLGTILE ptTile, void *hNewTile)
{
	GAMEROOM *hRoom;
	GAMEROOM **pptNear;
	DWORD dwNumNear;
	COL *ptCol;
	int nX;
	int nY;
	DWORD i;

	hRoom = (GAMEROOM *)ptGameRoom;
	ptCol = DungeonRoomGetCollision(hRoom);
	nX = ptCol->tTileCoords.nMinX + ptTile->nX;
	nY = ptCol->tTileCoords.nMinY + ptTile->nY;
	EnvTileToSubtile(&nX, &nY);
	if (!DungeonTestRoomGame(hRoom, nX, nY))
	{
		DungeonRoomGetNear(hRoom, &pptNear, &dwNumNear);
		for (i = 0; i < dwNumNear; i++)
		{
			if (DungeonTestRoomGame(pptNear[i], nX, nY))
			{
				hRoom = pptNear[i];
				break;
			}
		}
		if (!DungeonTestRoomGame(hRoom, nX, nY))
			return;
		ptCol = DungeonRoomGetCollision(hRoom);
	}
	nX -= ptCol->tCoords.nMinX;
	nY -= ptCol->tCoords.nMinY;
	CollisnClearTile(ptCol, ptTile->pTile, nX, nY);
	if (hNewTile)
		CollisnStampTile(ptCol, hNewTile, nX, nY);
}

#pragma auto_inline(off)

static void __fastcall CollisnStampTile(COL *ptCol, void *hTile, int nX, int nY)
{
	BYTE *pbMask;
	BYTE *pbRow;
	BYTE *pbSrc;
	WORD *pwRow;
	int nStartX;
	int nStartY;
	int nEndX;
	int nEndY;
	int x;
	int y;

	pbMask = D2CMPTileGetCollisionMask(hTile);
	nStartY = CollisnNotBelowZero(nY);
	nEndY = nY + 5;
	nEndX = nX + 5;
	nStartX = CollisnNotBelowZero(nX);
	pbRow = &pbMask[(nY + 4 - nStartY) * 5 + nStartX - nX];
	for (y = nStartY; y < nEndY; y++)
	{
		pwRow = &ptCol->pwGrid[ptCol->pdwRowOffset[y]];
		pbSrc = pbRow;
		for (x = nStartX; x < nEndX; x++)
			pwRow[x] |= *pbSrc++;
		pbRow -= 5;
	}
}
#pragma auto_inline(on)

DWORD __fastcall CollisnGetZero(void)
{
	return 0;
}

void __fastcall CollisnNew(void *pPool, GAMEROOM *ptRoom)
{
	GAMERECT atRects[2];
	COL *ptCol;
	GAMEROOM **pptNear;
	DWORD dwNumNear;
	COL *ptNearCol;
	PTDRLGTILE ptTiles;
	DWORD dwCount;
	int nCells;
	int i;

	DungeonRoomCopyRects(ptRoom, atRects);
	nCells = atRects[0].nWidth * atRects[0].nHeight;
#line 323
	ptCol = (COL *)D2ALLOCPOOL(pPool, (nCells + atRects[0].nHeight * 2 + 20) * 2);
	memcpy(&ptCol->tCoords, atRects, sizeof(atRects));

	DungeonRoomSetCollision(ptRoom, ptCol);
	ptCol->pwGrid = (WORD *)(ptCol + 1);
	memset(ptCol->pwGrid, 0, nCells * 2);
	ptCol->pdwRowOffset = (DWORD *)((BYTE *)(ptCol + 1) + nCells * 2);
	ptCol->pdwRowOffset[0] = 0;
	for (i = 1; i < atRects[0].nHeight; i++)
		ptCol->pdwRowOffset[i] = ptCol->pdwRowOffset[i - 1] + atRects[0].nWidth;
	DungeonRoomGetNear(ptRoom, &pptNear, &dwNumNear);
	for (i = 0; i < (int)dwNumNear; i++)
	{
		ptNearCol = DungeonRoomGetCollision(pptNear[i]);
		ptTiles = (PTDRLGTILE)DungeonRoomGetSecondTileArray(pptNear[i], &dwCount);
		CollisnStampTiles(ptCol, ptNearCol, ptTiles, (int)dwCount, FALSE);
		ptTiles = (PTDRLGTILE)DungeonRoomGetFirstTileArray(pptNear[i], &dwCount);
		CollisnStampTiles(ptCol, ptNearCol, ptTiles, (int)dwCount, FALSE);
		ptTiles = (PTDRLGTILE)DungeonRoomGetThirdTileArray(pptNear[i], &dwCount);
		CollisnStampTiles(ptCol, ptNearCol, ptTiles, (int)dwCount, FALSE);
	}
}

static void CollisnSetTile(COL *ptCol, void *hTile, int nX, int nY)
{
	BYTE *pbMask;
	BYTE *pbRow;
	BYTE *pbSrc;
	WORD *pwRow;
	int nStartX;
	int nStartY;
	int nEndX;
	int nEndY;
	int x;
	int y;

	pbMask = D2CMPTileGetCollisionMask(hTile);
	nStartY = CollisnNotBelowZero(nY);
	nEndY = nY + 5;
	nEndX = nX + 5;
	nStartX = CollisnNotBelowZero(nX);
	pbRow = &pbMask[(nY + 4 - nStartY) * 5 + nStartX - nX];
	for (y = nStartY; y < nEndY; y++)
	{
		pwRow = &ptCol->pwGrid[ptCol->pdwRowOffset[y]];
		pbSrc = pbRow;
		for (x = nStartX; x < nEndX; x++)
			pwRow[x] = *pbSrc++;
		pbRow -= 5;
	}
}

static void CollisnSetFlags(COL *ptCol, int nX, int nY, WORD wFlags)
{
	WORD *pwRow;
	int nStartX;
	int nStartY;
	int nEndX;
	int nEndY;
	int x;
	int y;

	nStartY = CollisnNotBelowZero(nY);
	nEndY = nY + 5;
	nStartX = CollisnNotBelowZero(nX);
	nEndX = nX + 5;
	for (y = nStartY; y < nEndY; y++)
	{
		pwRow = &ptCol->pwGrid[ptCol->pdwRowOffset[y]];
		for (x = nStartX; x < nEndX; x++)
			pwRow[x] |= wFlags;
	}
}

static void __fastcall CollisnStampTiles(COL *ptCol, COL *ptSrcCol, PTDRLGTILE ptTiles, int nCount,
	BOOL fReplace)
{
	int nFlags;
	int nX;
	int nY;
	int i;

	for (i = 0; i < nCount; i++, ptTiles++)
	{
		nX = ptTiles->nX + ptSrcCol->tTileCoords.nMinX;
		nY = ptTiles->nY + ptSrcCol->tTileCoords.nMinY;
		EnvTileToSubtile(&nX, &nY);
		if (!DungeonCoordsContain(&ptCol->tCoords, nX, nY))
			continue;
		nX -= ptCol->tCoords.nMinX;
		nY -= ptCol->tCoords.nMinY;
		if (fReplace)
			CollisnSetTile(ptCol, ptTiles->pTile, nX, nY);
		else
			CollisnStampTile(ptCol, ptTiles->pTile, nX, nY);
		nFlags = 0;
		if (ptTiles->dwFlags & 2)
			nFlags = 16;
		if (ptTiles->dwFlags & 64)
			nFlags |= 1;
		if (ptTiles->dwFlags & 128)
			nFlags |= 4;
		if (nFlags)
			CollisnSetFlags(ptCol, nX, nY, nFlags);
	}
}

static WORD __fastcall CollisnTestRectAcrossRooms(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, DWORD dwPattern);
static int __fastcall CollisnClipRectToRoom(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, GAMERECTLTRB *paOut);
static WORD __fastcall CollisnTestRectLocal(COL *ptCol, GAMERECTLTRB *ptRect, DWORD dwPattern);
static BOOL __fastcall CollisnTestCrossFast(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern);
static void __fastcall CollisnSetRectAcrossRooms(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, DWORD dwPattern);
static void __fastcall CollisnClearRectAcrossRooms(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, DWORD dwPattern);
static void __fastcall CollisnMakeRect(GAMERECTLTRB *ptRect, int nX, int nY, int nWidth, int nHeight);
static void __fastcall CollisnSetCross(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern);
static void __fastcall CollisnSetBlock(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern);

struct COLPOINT
{
 	int		nX;
 	int		nY;
};
typedef struct COLPOINT COLPOINT, *PTCOLPOINT;

void __fastcall CollisnFree(void *pPool, GAMEROOM *ptRoom)
{
#line 379
	void *pCol = DungeonRoomGetCollision(ptRoom); D2FREEPOOL(pPool, pCol);
	DungeonRoomSetCollision(ptRoom, NULL);
}

static WORD __fastcall CollisnTestPointAnyRoom(GAMEROOM *hRoom, int nX, int nY, WORD dwPattern)
{
	GAMEROOM **pptNear;
	DWORD dwNumNear, i;
	GAMEROOM *ptFound;
	COL *ptCol;
	WORD *pRow;

	ptFound = hRoom;
	if (hRoom == NULL ||
		nX < hRoom->tRect.nX || nX >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		nY < hRoom->tRect.nY || nY >= hRoom->tRect.nY + hRoom->tRect.nHeight)
	{
		pptNear = NULL;
		dwNumNear = 0;
		DungeonRoomGetNear(hRoom, &pptNear, &dwNumNear);
		for (i = 0; ; i++)
		{
			if (i >= dwNumNear)
				return 39;
			if (pptNear[i] != NULL &&
				nX >= pptNear[i]->tRect.nX && nX < pptNear[i]->tRect.nX + pptNear[i]->tRect.nWidth &&
				nY >= pptNear[i]->tRect.nY && nY < pptNear[i]->tRect.nY + pptNear[i]->tRect.nHeight)
				break;
		}
		ptFound = pptNear[i];
	}
	ptCol = DungeonRoomGetCollision(ptFound);
	pRow = ptCol->pwGrid + ptCol->pdwRowOffset[nY - ptCol->tCoords.nMinY];
	return (WORD)(pRow[nX - ptCol->tCoords.nMinX] & dwPattern);
}

WORD __stdcall CollisnTestPoint(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	return CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
}

static void __fastcall CollisnSetPointAnyRoom(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	GAMEROOM *ptFound;
	COL *ptCol;
	WORD *pRow;

	if (hRoom == NULL ||
		nX < hRoom->tRect.nX || nX >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		nY < hRoom->tRect.nY || nY >= hRoom->tRect.nY + hRoom->tRect.nHeight)
		ptFound = DungeonFindRoomGame(hRoom, nX, nY);
	else
		ptFound = hRoom;
	if (ptFound != NULL)
	{
		ptCol = DungeonRoomGetCollision(ptFound);
		(ptCol->pwGrid + ptCol->pdwRowOffset[nY - ptCol->tCoords.nMinY])[nX - ptCol->tCoords.nMinX] |= (WORD)dwPattern;
	}
}

void __stdcall CollisnSetPoint(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	CollisnSetPointAnyRoom(hRoom, nX, nY, dwPattern);
}

static void __fastcall CollisnClearPointAnyRoom(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	GAMEROOM *ptFound;
	COL *ptCol;
	WORD *pRow;

	ptFound = hRoom;
	if (hRoom == NULL ||
		nX < hRoom->tRect.nX || nX >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		nY < hRoom->tRect.nY || nY >= hRoom->tRect.nY + hRoom->tRect.nHeight)
		ptFound = DungeonFindRoomGame(hRoom, nX, nY);
	if (ptFound != NULL)
	{
		ptCol = DungeonRoomGetCollision(ptFound);
		(ptCol->pwGrid + ptCol->pdwRowOffset[nY - ptCol->tCoords.nMinY])[nX - ptCol->tCoords.nMinX] &= (WORD)~dwPattern;
	}
}

void __stdcall CollisnClearPoint(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	CollisnClearPointAnyRoom(hRoom, nX, nY, dwPattern);
}

WORD __stdcall CollisnTestArea(GAMEROOM *hRoom, int nX, int nY, int nWidth, int nHeight, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

	if (nWidth == 1 && nHeight == 1)
		return CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);

	tRect.left = nX - (int)((DWORD)nWidth >> 1);
	tRect.right = tRect.left + nWidth - 1;
	tRect.top = nY - (int)((DWORD)nHeight >> 1);
	tRect.bottom = tRect.top + nHeight - 1;
	return CollisnTestRectAcrossRooms(hRoom, &tRect, dwPattern);
}

static WORD __fastcall CollisnTestRectAcrossRooms(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, DWORD dwPattern)
{
	GAMEROOM *ptFound;
	COL *ptCol;
	GAMERECTLTRB atOut[COL_NUM_ROOM_RECTS];
	int nNumRoomRects, i;
	int nX, nY;
	WORD wResult;

	ptFound = hRoom;
	if (hRoom == NULL ||
		ptRect->left < hRoom->tRect.nX || ptRect->left >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		ptRect->top < hRoom->tRect.nY || ptRect->top >= hRoom->tRect.nY + hRoom->tRect.nHeight)
	{
		ptFound = DungeonFindRoomGame(hRoom, ptRect->left, ptRect->top);
	}
	if (ptFound == NULL)
	{
		wResult = 39;
		for (nY = ptRect->top; nY <= ptRect->bottom; nY++)
			for (nX = ptRect->left; nX <= ptRect->right; nX++)
			{
				GAMEROOM *ptCell = DungeonFindRoomGame(hRoom, nX, nY);

				if (ptCell != NULL)
					wResult |= CollisnTestPointAnyRoom(ptCell, nX, nY, dwPattern);
			}
		return wResult;
	}

	ptCol = DungeonRoomGetCollision(ptFound);
	nNumRoomRects = CollisnClipRectToRoom(ptFound, ptRect, atOut);
#line 697
	D2ASSERT(nNumRoomRects <= COL_NUM_ROOM_RECTS);
	if (nNumRoomRects == 0)
		return 39;

	wResult = CollisnTestRectLocal(ptCol, &atOut[0], dwPattern);
	for (i = 1; i < nNumRoomRects; i++)
		wResult |= CollisnTestRectAcrossRooms(ptFound, &atOut[i], dwPattern);
	return wResult;
}

static int __fastcall CollisnClipRectToRoom(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, GAMERECTLTRB *paOut)
{
	COL *ptCol;
	int nCount;

#line 500
	D2ASSERT(ptRect->top <= ptRect->bottom);
#line 501
	D2ASSERT(ptRect->left <= ptRect->right);

	ptCol = DungeonRoomGetCollision(hRoom);
	nCount = 1;
	paOut[0].top = ptRect->top;
	paOut[0].bottom = ptRect->bottom;
	paOut[0].left = ptRect->left;
	paOut[0].right = ptRect->right;

#line 514
	D2ASSERT(DungeonTestRoomGame(hRoom, ptRect->left, ptRect->top));

	if (ptRect->right >= ptCol->tCoords.nMinX + ptCol->tCoords.nSizeGameX)
	{
		paOut[0].right = ptCol->tCoords.nMinX + ptCol->tCoords.nSizeGameX - 1;
		paOut[nCount].left = paOut[0].right + 1;
		paOut[nCount].right = ptRect->right;
		paOut[nCount].top = paOut[0].top;
		paOut[nCount].bottom = paOut[0].bottom;
		nCount = 2;
	}
	if (ptRect->bottom >= ptCol->tCoords.nMinY + ptCol->tCoords.nSizeGameY)
	{
		paOut[0].bottom = ptCol->tCoords.nMinY + ptCol->tCoords.nSizeGameY - 1;
		paOut[nCount].left = paOut[0].left;
		paOut[nCount].right = paOut[0].right;
		paOut[nCount].top = paOut[0].bottom + 1;
		paOut[nCount].bottom = ptRect->bottom;
		nCount++;
	}
	return nCount;
}

static WORD __fastcall CollisnTestRectLocal(COL *ptCol, GAMERECTLTRB *ptRect, DWORD dwPattern)
{
	int nRow;
	WORD *pRowBase;
	WORD wResult = 0;

	ptRect->right -= ptCol->tCoords.nMinX;
	ptRect->left -= ptCol->tCoords.nMinX;
	ptRect->top -= ptCol->tCoords.nMinY;
	ptRect->bottom -= ptCol->tCoords.nMinY;

#line 561
	D2ASSERT(ptRect->top >= 0);
#line 562
	D2ASSERT(ptRect->left >= 0);
#line 563
	D2ASSERT(ptRect->bottom < ptCol->tCoords.nSizeGameY);
#line 564
	D2ASSERT(ptRect->right < ptCol->tCoords.nSizeGameX);

	pRowBase = ptCol->pwGrid + ptCol->pdwRowOffset[ptRect->top];
	for (nRow = ptRect->top; nRow <= ptRect->bottom; nRow++)
	{
		int nCol;

		for (nCol = ptRect->left; nCol <= ptRect->right; nCol++)
			wResult |= (WORD)(pRowBase[nCol] & dwPattern);
		pRowBase += ptCol->tCoords.nSizeGameX;
	}
	return wResult;
}

extern __inline WORD __stdcall CollisnTestPattern(GAMEROOM *hRoom, int nX, int nY, int ePattern, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

#line 1017
	D2ASSERT(ePattern < NUM_COLPAT);

	switch (ePattern)
	{
	case 0:
		return CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
	case 2:
		CollisnMakeRect(&tRect, nX, nY, 3, 3);
		return CollisnTestRectAcrossRooms(hRoom, &tRect, dwPattern);
	case 1:
		return CollisnTestPointAnyRoom(hRoom, nX, nY - 1, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX, nY + 1, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX - 1, nY, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX + 1, nY, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
	default:
		return CollisnTestPointAnyRoom(hRoom, nX, nY - 1, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX, nY + 1, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX - 1, nY, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX + 1, nY, dwPattern) |
			CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
	}
}

static inline BOOL CollisnTestPatternAnyBlock(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	GAMERECTLTRB tRect;
	GAMEROOM *ptFound;
	COL *ptCol;
	GAMERECTLTRB atOut[COL_NUM_ROOM_RECTS];
	int nNumRoomRects, i;

	CollisnMakeRect(&tRect, nX, nY, 3, 3);
	ptFound = hRoom;
	if (hRoom == NULL ||
		tRect.left < hRoom->tRect.nX || tRect.left >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		tRect.top < hRoom->tRect.nY || tRect.top >= hRoom->tRect.nY + hRoom->tRect.nHeight)
		ptFound = DungeonFindRoomGame(hRoom, tRect.left, tRect.top);
	if (ptFound == NULL)
		return 1;
	ptCol = DungeonRoomGetCollision(ptFound);
	nNumRoomRects = CollisnClipRectToRoom(ptFound, &tRect, atOut);
#line 742
	D2ASSERT(nNumRoomRects <= COL_NUM_ROOM_RECTS);
	if (nNumRoomRects == 0)
		return 1;
	if (CollisnTestRectLocal(ptCol, &atOut[0], dwPattern))
		return 1;
	else
	{
		for (i = 1; i < nNumRoomRects; i++)
			if (CollisnTestRectAcrossRooms(ptFound, &atOut[i], dwPattern))
				return 1;
		return 0;
	}
}

BOOL __stdcall CollisnTestPatternAny(GAMEROOM *hRoom, int nX, int nY, int ePattern, DWORD dwPattern)
{
#line 1040
	D2ASSERT(ePattern < NUM_COLPAT);

	switch (ePattern)
	{
	case 0:
		return CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
	case 2:
		return CollisnTestPatternAnyBlock(hRoom, nX, nY, dwPattern);
	case 1:
		return CollisnTestCrossFast(hRoom, nX, nY, dwPattern);
	default:
		return CollisnTestCrossFast(hRoom, nX, nY, dwPattern);
	}
}

static BOOL __fastcall CollisnTestCrossFast(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	WORD *pRow;
	GAMERECT *ptRect;
	COL *ptCol;
	WORD *pCenter;
	int nWidth;

	if (hRoom == NULL)
		return 1;

	ptRect = &hRoom->tRect;
	if (nX - 1 >= ptRect->nX && nX + 1 < ptRect->nX + ptRect->nWidth &&
		nY - 1 >= ptRect->nY && nY + 1 < ptRect->nY + ptRect->nHeight)
	{
		ptCol = DungeonRoomGetCollision(hRoom);
		nWidth = hRoom->tRect.nWidth;
		pRow = ptCol->pwGrid + ptCol->pdwRowOffset[nY - ptCol->tCoords.nMinY];
		pCenter = pRow - ptCol->tCoords.nMinX + nX;
		if ((WORD)(*pCenter & dwPattern) || (WORD)(*(pCenter - 1) & dwPattern) ||
			(WORD)(*(pCenter + 1) & dwPattern) || (WORD)(*(pCenter - nWidth) & dwPattern) ||
			(WORD)(*(pCenter + nWidth) & dwPattern))
			return 1;
	}

	if (CollisnTestPointAnyRoom(hRoom, nX - 1, nY, dwPattern) ||
		CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern) ||
		CollisnTestPointAnyRoom(hRoom, nX + 1, nY, dwPattern) ||
		CollisnTestPointAnyRoom(hRoom, nX, nY - 1, dwPattern) ||
		CollisnTestPointAnyRoom(hRoom, nX, nY + 1, dwPattern))
		return 1;
	return 0;
}

static WORD __fastcall CollisnTestCross(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	return CollisnTestPointAnyRoom(hRoom, nX, nY - 1, dwPattern) |
		CollisnTestPointAnyRoom(hRoom, nX, nY + 1, dwPattern) |
		CollisnTestPointAnyRoom(hRoom, nX - 1, nY, dwPattern) |
		CollisnTestPointAnyRoom(hRoom, nX + 1, nY, dwPattern) |
		CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
}

static WORD __fastcall CollisnTestBlock(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

	CollisnMakeRect(&tRect, nX, nY, 3, 3);
	return CollisnTestRectAcrossRooms(hRoom, &tRect, dwPattern);
}

extern __inline WORD __stdcall CollisnTestSize(GAMEROOM *hRoom, int nX, int nY, int nSize, DWORD dwPattern)
{
	WORD wResult;

#line 1063
	D2ASSERT(nSize < NUM_COLLISION_SIZES);

	switch (nSize)
	{
	case 0:
		wResult = CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
		return wResult;
	case 1:
		wResult = CollisnTestPointAnyRoom(hRoom, nX, nY, dwPattern);
		return wResult;
	case 2:
		wResult = CollisnTestCross(hRoom, nX, nY, dwPattern);
		return wResult;
	default:
		wResult = CollisnTestBlock(hRoom, nX, nY, dwPattern);
		return wResult;
	}
}

extern __inline void __stdcall CollisnSetSize(GAMEROOM *hRoom, int nX, int nY, int nSize, DWORD dwPattern)
{
#line 1086
	D2ASSERT(nSize < NUM_COLLISION_SIZES);

	switch (nSize)
	{
	case 1:
		CollisnSetPointAnyRoom(hRoom, nX, nY, dwPattern);
		return;
	case 2:
		CollisnSetCross(hRoom, nX, nY, dwPattern);
		return;
	case 3:
		CollisnSetBlock(hRoom, nX, nY, dwPattern);
		return;
	}
}

static inline void CollisnSetRectLocal(COL *ptCol, GAMERECTLTRB *ptRect, DWORD dwPattern)
{
	WORD *pRowBase;
	int nRow;

	ptRect->right -= ptCol->tCoords.nMinX;
	ptRect->left -= ptCol->tCoords.nMinX;
	ptRect->top -= ptCol->tCoords.nMinY;
	ptRect->bottom -= ptCol->tCoords.nMinY;
#line 634
	D2ASSERT(ptRect->top >= 0);
#line 635
	D2ASSERT(ptRect->left >= 0);
#line 636
	D2ASSERT(ptRect->bottom < ptCol->tCoords.nSizeGameY);
#line 637
	D2ASSERT(ptRect->right < ptCol->tCoords.nSizeGameX);

	pRowBase = ptCol->pwGrid + ptCol->pdwRowOffset[ptRect->top];
	for (nRow = ptRect->top; nRow <= ptRect->bottom; nRow++)
	{
		int nCol;

		for (nCol = ptRect->left; nCol <= ptRect->right; nCol++)
			pRowBase[nCol] |= (WORD)dwPattern;
		pRowBase += ptCol->tCoords.nSizeGameX;
	}
}

static void __fastcall CollisnSetRectAcrossRooms(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, DWORD dwPattern)
{
	GAMEROOM *ptFound;
	COL *ptCol;
	GAMERECTLTRB atOut[COL_NUM_ROOM_RECTS];
	int nNumRoomRects, i;
	int nX, nY;

	ptFound = hRoom;
	if (hRoom == NULL ||
		ptRect->left < hRoom->tRect.nX || ptRect->left >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		ptRect->top < hRoom->tRect.nY || ptRect->top >= hRoom->tRect.nY + hRoom->tRect.nHeight)
	{
		ptFound = DungeonFindRoomGame(hRoom, ptRect->left, ptRect->top);
	}
	if (ptFound == NULL)
	{
		for (nY = ptRect->top; nY <= ptRect->bottom; nY++)
			for (nX = ptRect->left; nX <= ptRect->right; nX++)
			{
				GAMEROOM *ptCell = DungeonFindRoomGame(hRoom, nX, nY);

				if (ptCell != NULL)
					CollisnSetPointAnyRoom(ptCell, nX, nY, dwPattern);
			}
		return;
	}

	ptCol = DungeonRoomGetCollision(ptFound);
	nNumRoomRects = CollisnClipRectToRoom(ptFound, ptRect, atOut);
#line 865
	D2ASSERT(nNumRoomRects <= COL_NUM_ROOM_RECTS);
	if (nNumRoomRects == 0)
		return;
	CollisnSetRectLocal(ptCol, &atOut[0], dwPattern);

	for (i = 1; i < nNumRoomRects; i++)
		CollisnSetRectAcrossRooms(ptFound, &atOut[i], dwPattern);
}

void __stdcall CollisnSetPattern(GAMEROOM *hRoom, int nX, int nY, int ePattern, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

#line 1114
	D2ASSERT(ePattern < NUM_COLPAT);

	switch (ePattern)
	{
	case 2:
		CollisnMakeRect(&tRect, nX, nY, 3, 3);
		CollisnSetRectAcrossRooms(hRoom, &tRect, dwPattern);
		if (dwPattern != 0)
		{
			CollisnSetPointAnyRoom(hRoom, nX - 1, nY, 4096);
			CollisnSetPointAnyRoom(hRoom, nX, nY, 4096);
			CollisnSetPointAnyRoom(hRoom, nX + 1, nY, 4096);
			CollisnSetPointAnyRoom(hRoom, nX, nY - 1, 4096);
			CollisnSetPointAnyRoom(hRoom, nX, nY + 1, 4096);
		}
		return;
	case 1:
		CollisnSetPointAnyRoom(hRoom, nX - 1, nY, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX, nY, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX + 1, nY, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX, nY - 1, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX, nY + 1, dwPattern);
		if (dwPattern != 0)
			CollisnSetPointAnyRoom(hRoom, nX, nY, 4096);
		return;
	case 3:
		CollisnSetPointAnyRoom(hRoom, nX - 1, nY, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX, nY, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX + 1, nY, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX, nY - 1, dwPattern);
		CollisnSetPointAnyRoom(hRoom, nX, nY + 1, dwPattern);
		return;
	}
}

static void __fastcall CollisnClearCross(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	CollisnClearPointAnyRoom(hRoom, nX - 1, nY, dwPattern);
	CollisnClearPointAnyRoom(hRoom, nX, nY, dwPattern);
	CollisnClearPointAnyRoom(hRoom, nX + 1, nY, dwPattern);
	CollisnClearPointAnyRoom(hRoom, nX, nY - 1, dwPattern);
	CollisnClearPointAnyRoom(hRoom, nX, nY + 1, dwPattern);
}

static void __fastcall CollisnClearBlock(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

	CollisnMakeRect(&tRect, nX, nY, 3, 3);
	CollisnClearRectAcrossRooms(hRoom, &tRect, dwPattern);
}

extern __inline void __stdcall CollisnClearSize(GAMEROOM *hRoom, int nX, int nY, int nSize, DWORD dwPattern)
{
#line 1150
	D2ASSERT(nSize < NUM_COLLISION_SIZES);

	if (hRoom == NULL)
		return;

	switch (nSize)
	{
	case 1:
		CollisnClearPointAnyRoom(hRoom, nX, nY, dwPattern);
		return;
	case 2:
		CollisnClearCross(hRoom, nX, nY, dwPattern);
		return;
	case 3:
		CollisnClearBlock(hRoom, nX, nY, dwPattern);
		return;
	}
}

static inline void CollisnClearRectLocal(COL *ptCol, GAMERECTLTRB *ptRect, DWORD dwPattern)
{
	WORD *pRowBase;
	int nRow;

	ptRect->right -= ptCol->tCoords.nMinX;
	ptRect->left -= ptCol->tCoords.nMinX;
	ptRect->top -= ptCol->tCoords.nMinY;
	ptRect->bottom -= ptCol->tCoords.nMinY;
#line 597
	D2ASSERT(ptRect->top >= 0);
#line 598
	D2ASSERT(ptRect->left >= 0);
#line 599
	D2ASSERT(ptRect->bottom < ptCol->tCoords.nSizeGameY);
#line 600
	D2ASSERT(ptRect->right < ptCol->tCoords.nSizeGameX);

	pRowBase = ptCol->pwGrid + ptCol->pdwRowOffset[ptRect->top];
	for (nRow = ptRect->top; nRow <= ptRect->bottom; nRow++)
	{
		int nCol;

		for (nCol = ptRect->left; nCol <= ptRect->right; nCol++)
			pRowBase[nCol] &= (WORD)~dwPattern;
		pRowBase += ptCol->tCoords.nSizeGameX;
	}
}

static void __fastcall CollisnClearRectAcrossRooms(GAMEROOM *hRoom, GAMERECTLTRB *ptRect, DWORD dwPattern)
{
	GAMEROOM *ptFound;
	COL *ptCol;
	GAMERECTLTRB atOut[COL_NUM_ROOM_RECTS];
	int nNumRoomRects, i;
	int nX, nY;

	ptFound = hRoom;
	if (hRoom == NULL ||
		ptRect->left < hRoom->tRect.nX || ptRect->left >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		ptRect->top < hRoom->tRect.nY || ptRect->top >= hRoom->tRect.nY + hRoom->tRect.nHeight)
	{
		ptFound = DungeonFindRoomGame(hRoom, ptRect->left, ptRect->top);
	}
	if (ptFound == NULL)
	{
		for (nY = ptRect->top; nY <= ptRect->bottom; nY++)
			for (nX = ptRect->left; nX <= ptRect->right; nX++)
			{
				GAMEROOM *ptCell = DungeonFindRoomGame(hRoom, nX, nY);

				if (ptCell != NULL)
					CollisnClearPointAnyRoom(ptCell, nX, nY, dwPattern);
			}
		return;
	}

	ptCol = DungeonRoomGetCollision(ptFound);
	nNumRoomRects = CollisnClipRectToRoom(ptFound, ptRect, atOut);
#line 808
	D2ASSERT(nNumRoomRects <= COL_NUM_ROOM_RECTS);
	if (nNumRoomRects == 0)
		return;
	CollisnClearRectLocal(ptCol, &atOut[0], dwPattern);

	for (i = 1; i < nNumRoomRects; i++)
		CollisnClearRectAcrossRooms(ptFound, &atOut[i], dwPattern);
}

void __stdcall CollisnClearPattern(GAMEROOM *hRoom, int nX, int nY, int ePattern, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

#line 1181
	D2ASSERT(ePattern < NUM_COLPAT);

	if (hRoom == NULL)
		return;

	switch (ePattern)
	{
	case 2:
		CollisnMakeRect(&tRect, nX, nY, 3, 3);
		CollisnClearRectAcrossRooms(hRoom, &tRect, dwPattern);
		if (dwPattern != 0)
		{
			CollisnClearPointAnyRoom(hRoom, nX - 1, nY, 4096);
			CollisnClearPointAnyRoom(hRoom, nX, nY, 4096);
			CollisnClearPointAnyRoom(hRoom, nX + 1, nY, 4096);
			CollisnClearPointAnyRoom(hRoom, nX, nY - 1, 4096);
			CollisnClearPointAnyRoom(hRoom, nX, nY + 1, 4096);
		}
		return;
	case 1:
		CollisnClearPointAnyRoom(hRoom, nX - 1, nY, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX, nY, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX + 1, nY, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX, nY - 1, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX, nY + 1, dwPattern);
		if (dwPattern != 0)
			CollisnClearPointAnyRoom(hRoom, nX, nY, 4096);
		return;
	case 3:
		CollisnClearPointAnyRoom(hRoom, nX - 1, nY, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX, nY, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX + 1, nY, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX, nY - 1, dwPattern);
		CollisnClearPointAnyRoom(hRoom, nX, nY + 1, dwPattern);
		return;
	}
}

void __stdcall CollisnClearArea(GAMEROOM *hRoom, int nX, int nY, int nWidth, int nHeight, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

	if (nWidth == 0 || nHeight == 0)
		return;
	if (nWidth == 1 && nHeight == 1)
	{
		CollisnClearPointAnyRoom(hRoom, nX, nY, dwPattern);
		return;
	}
	tRect.left = nX - (int)((DWORD)nWidth >> 1);
	tRect.right = tRect.left + nWidth - 1;
	tRect.top = nY - (int)((DWORD)nHeight >> 1);
	tRect.bottom = tRect.top + nHeight - 1;
	CollisnClearRectAcrossRooms(hRoom, &tRect, dwPattern);
}

void __stdcall CollisnSetArea(GAMEROOM *hRoom, int nX, int nY, int nWidth, int nHeight, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

	if (nWidth == 1 && nHeight == 1)
	{
		CollisnSetPointAnyRoom(hRoom, nX, nY, dwPattern);
		return;
	}
	tRect.left = nX - (int)((DWORD)nWidth >> 1);
	tRect.right = tRect.left + nWidth - 1;
	tRect.top = nY - (int)((DWORD)nHeight >> 1);
	tRect.bottom = tRect.top + nHeight - 1;
	CollisnSetRectAcrossRooms(hRoom, &tRect, dwPattern);
}

WORD __fastcall CollisnMoveSize(GAMEROOM *hRoom, int nX, int nY, int nNewX, int nNewY, int nSize,
	DWORD dwPattern, DWORD dwTestPattern)
{
	WORD wResult;

	CollisnClearSize(hRoom, nX, nY, nSize, dwPattern);
	wResult = CollisnTestSize(hRoom, nNewX, nNewY, nSize, dwTestPattern);
	if (wResult & 5)
		CollisnSetSize(hRoom, nX, nY, nSize, dwPattern);
	else
		CollisnSetSize(hRoom, nNewX, nNewY, nSize, dwPattern);
	return wResult;
}

static void __fastcall CollisnMakeRect(GAMERECTLTRB *ptRect, int nX, int nY, int nWidth, int nHeight)
{
	ptRect->left = nX - (int)((DWORD)nWidth >> 1);
	ptRect->right = ptRect->left + nWidth - 1;
	ptRect->top = nY - (int)((DWORD)nHeight >> 1);
	ptRect->bottom = ptRect->top + nHeight - 1;
}

static void __fastcall CollisnSetCross(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	CollisnSetPointAnyRoom(hRoom, nX - 1, nY, dwPattern);
	CollisnSetPointAnyRoom(hRoom, nX, nY, dwPattern);
	CollisnSetPointAnyRoom(hRoom, nX + 1, nY, dwPattern);
	CollisnSetPointAnyRoom(hRoom, nX, nY - 1, dwPattern);
	CollisnSetPointAnyRoom(hRoom, nX, nY + 1, dwPattern);
}

static void __fastcall CollisnSetBlock(GAMEROOM *hRoom, int nX, int nY, DWORD dwPattern)
{
	GAMERECTLTRB tRect;

	CollisnMakeRect(&tRect, nX, nY, 3, 3);
	CollisnSetRectAcrossRooms(hRoom, &tRect, dwPattern);
}

WORD __fastcall CollisnMovePattern(GAMEROOM *hRoom, int nX, int nY, int nNewX, int nNewY, int ePattern,
	DWORD dwPattern, DWORD dwTestPattern)
{
	WORD wResult;

	CollisnClearPattern(hRoom, nX, nY, ePattern, dwPattern);
	wResult = CollisnTestPattern(hRoom, nNewX, nNewY, ePattern, dwTestPattern);
	if (wResult == 0)
		CollisnSetPattern(hRoom, nNewX, nNewY, ePattern, dwPattern);
	else
		CollisnSetPattern(hRoom, nX, nY, ePattern, dwPattern);
	return wResult;
}

DWORD __fastcall CollisnProbePattern(GAMEROOM *hRoom, int nX, int nY, int nNewX, int nNewY, int ePattern,
	DWORD dwPattern, DWORD dwTestPattern)
{
	DWORD wResult;

	CollisnClearPattern(hRoom, nX, nY, ePattern, dwPattern);
	wResult = CollisnTestPatternAny(hRoom, nNewX, nNewY, ePattern, dwTestPattern);
	CollisnSetPattern(hRoom, nX, nY, ePattern, dwPattern);
	return wResult;
}

WORD __fastcall CollisnMoveSizeRooms(GAMEROOM *hRoom, int nX, int nY, GAMEROOM *hRoomNew, int nNewX,
	int nNewY, int nSize, DWORD dwPattern, DWORD dwTestPattern)
{
	WORD wResult;

	if (hRoom != NULL)
		CollisnClearSize(hRoom, nX, nY, nSize, dwPattern);
	wResult = CollisnTestSize(hRoomNew, nNewX, nNewY, nSize, dwTestPattern);
	CollisnSetSize(hRoomNew, nNewX, nNewY, nSize, dwPattern);
	return wResult;
}

WORD __fastcall CollisnPlaceSizeRooms(GAMEROOM *hRoom, int nX, int nY, GAMEROOM *hRoomNew, int nNewX,
	int nNewY, int nSize, DWORD dwPattern)
{
	if (hRoom != NULL)
		CollisnClearSize(hRoom, nX, nY, nSize, dwPattern);
	CollisnSetSize(hRoomNew, nNewX, nNewY, nSize, dwPattern);
	return 0;
}

DWORD __fastcall CollisnMovePatternRooms(GAMEROOM *hRoom, int nX, int nY, GAMEROOM *hRoomNew, int nNewX,
	int nNewY, int ePattern, DWORD dwPattern, DWORD dwTestPattern)
{
	DWORD wResult;

	if (hRoom != NULL)
		CollisnClearPattern(hRoom, nX, nY, ePattern, dwPattern);
	wResult = CollisnTestPatternAny(hRoomNew, nNewX, nNewY, ePattern, dwTestPattern);
	if (wResult == 0)
		CollisnSetPattern(hRoomNew, nNewX, nNewY, ePattern, dwPattern);
	else if (hRoom != NULL)
		CollisnSetPattern(hRoom, nX, nY, ePattern, dwPattern);
	return wResult;
}

void __fastcall CollisnDualUpdate(DWORD dwFromRoom, DWORD dwFromX, DWORD dwFromY, DWORD dwToRoom, DWORD dwToX,
	DWORD dwToY, DWORD dwCollisionPattern, DWORD dwCollisionMask)
{
	if (dwFromRoom == 0)
		return;
	D2CommonCollisnClearPattern(dwFromRoom, dwFromX, dwFromY, dwCollisionPattern, dwCollisionMask);
	D2CommonCollisnSetPattern(dwToRoom, dwToX, dwToY, dwCollisionPattern, dwCollisionMask);
}

BOOL __stdcall CollisnTestLine(GAMEROOM *hRoom, PTCOLPOINT ptFrom, PTCOLPOINT ptTo, DWORD dwPattern)
{
	COL *ptCol;
	WORD *pwCell;
	WORD wCurX, wCurY;
	int nX, nY;
	int nX1, nY1;
	int nDX, nDY;
	int nStepX, nStepY;
	int nRoomX, nRoomY;
	int nRoomRight, nRoomBottom;
	int nRest;
	int nEdgeX, nEdgeY;
	int nNewDX, nNewDY;
	int nSideSteps;
	int nError;
	int nWidth;
	BOOL fBlocked = FALSE;

	if (!hRoom)
	{
		ptTo->nX = ptFrom->nX;
		ptTo->nY = ptFrom->nY;
		return FALSE;
	}
	wCurX = (WORD)ptFrom->nX;
	wCurY = (WORD)ptFrom->nY;
	nX1 = (WORD)ptTo->nX;
	nY1 = (WORD)ptTo->nY;
	nX = wCurX;
	nY = wCurY;
	nDX = nX1 - nX;
	nDY = nY1 - nY;
	if (nDX < 0)
	{
		nDX = -nDX;
		nStepX = -1;
	}
	else
		nStepX = 1;
	if (nDY < 0)
	{
		nDY = -nDY;
		nStepY = -1;
	}
	else
		nStepY = 1;

	nRoomX = hRoom->tRect.nX;
	nRoomY = hRoom->tRect.nY;
	nRoomRight = nRoomX + hRoom->tRect.nWidth;
	nRoomBottom = hRoom->tRect.nHeight + nRoomY;
	if (nX < nRoomX || nX >= nRoomRight || nY <= nRoomY || nY >= nRoomBottom)
	{
		hRoom = DungeonFindRoomGame(hRoom, nX, nY);
		if (!hRoom)
		{
			ptTo->nX = ptFrom->nX;
			ptTo->nY = ptFrom->nY;
			return FALSE;
		}
		nRoomX = hRoom->tRect.nX;
		nRoomRight = nRoomX + hRoom->tRect.nWidth;
		nRoomY = hRoom->tRect.nY;
		nRoomBottom = hRoom->tRect.nHeight + nRoomY;
	}

	if (nDX == 0)
	{
		if (nDY > 0)
		{
			for (;;)
			{
				nRest = 0;
				if (nStepY > 0)
				{
					if (nY1 >= nRoomBottom)
					{
						int nSpan = nRoomBottom - nY;

						nRest = nDY - nSpan;
						nDY = nSpan;
					}
				}
				else if (nY1 < nRoomY)
				{
					int nSpan = nY - nRoomY + 1;

					nRest = nDY - nSpan;
					nDY = nSpan;
				}
				ptCol = DungeonRoomGetCollision(hRoom);
				pwCell = &ptCol->pwGrid[ptCol->pdwRowOffset[nY - nRoomY] - nRoomX + nX];
				nWidth = hRoom->tRect.nWidth;
				fBlocked = FALSE;
				for (; nDY; nDY--)
				{
					wCurY += (WORD)nStepY;
					pwCell += nWidth;
					if (*pwCell & dwPattern)
					{
						fBlocked = TRUE;
						break;
					}
				}
				if (fBlocked)
					break;
				if (!nRest)
					break;
				nY = wCurY;
				hRoom = DungeonFindRoomGame(hRoom, nX, nY);
				if (!hRoom)
				{
					fBlocked = TRUE;
					break;
				}
				nRoomX = hRoom->tRect.nX;
				nRoomY = hRoom->tRect.nY;
				nRoomBottom = hRoom->tRect.nHeight + nRoomY;
				nDY = nRest;
			}
		}
	}
	else if (nDY == 0)
	{
		for (;;)
		{
			nNewDX = 0;
			if (nStepX > 0)
			{
				if (nX1 >= nRoomRight)
				{
					nEdgeX = nRoomRight - nX;
					nNewDX = nDX - nEdgeX;
					nDX = nEdgeX;
				}
			}
			else if (nX1 < nRoomX)
			{
				nEdgeX = nX - nRoomX + 1;
				nNewDX = nDX - nEdgeX;
				nDX = nEdgeX;
			}
			ptCol = DungeonRoomGetCollision(hRoom);
			pwCell = &ptCol->pwGrid[ptCol->pdwRowOffset[nY - nRoomY] - nRoomX + nX];
			nWidth = hRoom->tRect.nWidth;
			fBlocked = FALSE;
			for (; nDX; nDX--)
			{
				wCurX += (WORD)nStepX;
				pwCell++;
				if (*pwCell & dwPattern)
				{
					fBlocked = TRUE;
					break;
				}
			}
			if (fBlocked)
				break;
			if (!nNewDX)
				break;
			nX = wCurX;
			hRoom = DungeonFindRoomGame(hRoom, nX, nY);
			if (!hRoom)
			{
				fBlocked = TRUE;
				break;
			}
			nRoomX = hRoom->tRect.nX;
			nRoomY = hRoom->tRect.nY;
			nRoomRight = hRoom->tRect.nWidth + nRoomX;
			nDX = nNewDX;
		}
	}
	else if (nDX >= nDY)
	{
		int nSteps;

		for (;;)
		{
			nNewDX = 0;
			nNewDY = 0;
			if (nX1 < nRoomX || nX1 >= nRoomRight || nY1 <= nRoomY || nY1 >= nRoomBottom)
			{
				if (nStepX)
					nEdgeX = nRoomRight - nX;
				else
					nEdgeX = nX - nRoomX;
				if (nStepY)
					nEdgeY = nRoomBottom - nY;
				else
					nEdgeY = nY - nRoomY;
				if (nEdgeX * nDY > nEdgeY * nDX)
					nNewDY = nEdgeY;
				else
					nNewDX = nEdgeX;
			}
			nError = 0;
			nSteps = nNewDX ? nNewDX : nDX;
			nSideSteps = nNewDY ? nNewDY : nDY;
			ptCol = DungeonRoomGetCollision(hRoom);
			pwCell = &ptCol->pwGrid[ptCol->pdwRowOffset[nY - nRoomY] - nRoomX + nX];
			nWidth = hRoom->tRect.nWidth;
			fBlocked = FALSE;
			for (; nSteps; nSteps--)
			{
				wCurX += (WORD)nStepX;
				nError += nDY;
				if (nError >= nDX)
				{
					nError -= nDX;
					pwCell += nWidth;
					wCurY += (WORD)nStepY;
					if (--nSideSteps <= 0)
						break;
				}
				pwCell++;
				if (*pwCell & dwPattern)
				{
					fBlocked = TRUE;
					break;
				}
			}
			if (fBlocked)
				break;
			if (wCurX == nX1)
				break;
			nX = wCurX;
			hRoom = DungeonFindRoomGame(hRoom, nX, wCurY);
			nY = wCurY;
			if (!hRoom)
			{
				fBlocked = TRUE;
				break;
			}
			nRoomX = hRoom->tRect.nX;
			nRoomY = hRoom->tRect.nY;
			nRoomRight = nRoomX + hRoom->tRect.nWidth;
			nRoomBottom = hRoom->tRect.nHeight + nRoomY;
			if (nNewDX)
				nDX = nNewDX;
			if (nNewDY)
				nDY = nNewDY;
		}
	}
	else
	{
		int nSteps;

		for (;;)
		{
			nNewDX = 0;
			nNewDY = 0;
			if (nX1 < nRoomX || nX1 >= nRoomRight || nY1 <= nRoomY || nY1 >= nRoomBottom)
			{
				if (nStepX)
					nEdgeX = nRoomRight - nX;
				else
					nEdgeX = nX - nRoomX;
				if (nStepY)
					nEdgeY = nRoomBottom - nY;
				else
					nEdgeY = nY - nRoomY;
				if (nEdgeX * nDY > nEdgeY * nDX)
					nNewDY = nEdgeY;
				else
					nNewDX = nEdgeX;
			}
			nError = 0;
			nSideSteps = nNewDX ? nNewDX : nDX;
			nSteps = nNewDY ? nNewDY : nDY;
			ptCol = DungeonRoomGetCollision(hRoom);
			pwCell = &ptCol->pwGrid[ptCol->pdwRowOffset[nY - nRoomY] - nRoomX + nX];
			nWidth = hRoom->tRect.nWidth;
			fBlocked = FALSE;
			for (; nSteps; nSteps--)
			{
				wCurY += (WORD)nStepY;
				nError += nDX;
				if (nError >= nDY)
				{
					nError -= nDY;
					pwCell++;
					wCurX += (WORD)nStepX;
					if (--nSideSteps <= 0)
						break;
				}
				pwCell += nWidth;
				if (*pwCell & dwPattern)
				{
					fBlocked = TRUE;
					break;
				}
			}
			if (fBlocked)
				break;
			if (wCurY == nY1)
				break;
			nX = wCurX;
			hRoom = DungeonFindRoomGame(hRoom, nX, wCurY);
			nY = wCurY;
			if (!hRoom)
			{
				fBlocked = TRUE;
				break;
			}
			nRoomX = hRoom->tRect.nX;
			nRoomY = hRoom->tRect.nY;
			nRoomRight = nRoomX + hRoom->tRect.nWidth;
			nRoomBottom = hRoom->tRect.nHeight + nRoomY;
			if (nNewDX)
				nDX = nNewDX;
			if (nNewDY)
				nDY = nNewDY;
		}
	}

	if (fBlocked)
	{
		ptTo->nX = wCurX;
		ptTo->nY = wCurY;
		return FALSE;
	}
	return TRUE;
}

static __inline int CollisnAbs(int n)
{
	return n < 0 ? -n : n;
}

static GAMEROOM * __fastcall CollisnFindSpawnPoint(GAMEROOM *hRoom, PTCOLPOINT ptPoint,
	PTCOLPOINT ptField, int nSize, DWORD dwPattern, DWORD dwFieldPattern, BOOL fFallback, int nMaxRadius,
	int nStep);

GAMEROOM * __stdcall CollisnFindSpawnPointRadius(GAMEROOM *hRoom, PTCOLPOINT ptSpawnPoint, int nSize,
	DWORD dwPattern, BOOL fFallback, int nMaxRadius)
{
	return CollisnFindSpawnPoint(hRoom, ptSpawnPoint, NULL, nSize, dwPattern, dwPattern, fFallback,
		nMaxRadius, 1);
}

static GAMEROOM * __fastcall CollisnFindSpawnPoint(GAMEROOM *hRoom, PTCOLPOINT ptPoint,
	PTCOLPOINT ptField, int nSize, DWORD dwPattern, DWORD dwFieldPattern, BOOL fFallback, int nMaxRadius,
	int nStep)
{
	PTCOLPOINT ptSpawnPoint = ptPoint;
	COLPOINT tStart;
	DWORD *hField;
	GAMEROOM *ptRoom;
	int nBest;
	int nRadius;

#line 1474
	D2ASSERT(ptSpawnPoint);
	tStart = *ptSpawnPoint;
	hField = NULL;
	if (ptField)
	{
		hField = FieldTblsAllocPair();
#line 1484
		D2ASSERT(hField);
		FieldTblsSetPair(hField, (DWORD)ptField->nX, (DWORD)ptField->nY);
	}

	ptRoom = DungeonFindRoomGame(hRoom, ptSpawnPoint->nX, ptSpawnPoint->nY);
	if (ptRoom && !CollisnTestSize(ptRoom, ptSpawnPoint->nX, ptSpawnPoint->nY, nSize, dwPattern) &&
		(!ptField || !FieldTblsFindCandidate((DWORD)hField, (DWORD)ptRoom, (DWORD)ptSpawnPoint->nX,
		(DWORD)ptSpawnPoint->nY, dwFieldPattern)))
	{
		if (hField)
			FieldTblsFreePair(hField);
		return ptRoom;
	}

	nBest = -1;
	for (nRadius = 1; nRadius < nMaxRadius; nRadius += nStep)
	{
		nBest = -1;
		{
			GAMEROOM *ptFound;
			int nX;
			int nY;
			int nDist;

			for (nY = tStart.nY - nRadius; nY <= tStart.nY + nRadius; nY += nStep)
			{
				for (nX = tStart.nX - nRadius; nX <= tStart.nX + nRadius; nX += nRadius * 2)
				{
					ptFound = DungeonFindRoomGame(ptRoom, nX, nY);
					if (!ptFound)
						continue;
					ptRoom = ptFound;
					if (CollisnTestSize(ptRoom, nX, nY, nSize, dwPattern))
						continue;
					if (ptField && FieldTblsFindCandidate((DWORD)hField, (DWORD)ptRoom, (DWORD)nX, (DWORD)nY,
						dwFieldPattern))
						continue;
					nDist = CollisnAbs(nX - tStart.nX) + CollisnAbs(nY - tStart.nY);
					if (nBest == -1 || nDist < nBest)
					{
						ptSpawnPoint->nX = nX;
						ptSpawnPoint->nY = nY;
						nBest = nDist;
					}
				}
			}
		}
		{
			GAMEROOM *ptFound;
			int nX;
			int nY;
			int nDist;

			for (nX = tStart.nX - nRadius + 1; nX <= tStart.nX + nRadius - 1; nX += nStep)
			{
				for (nY = tStart.nY - nRadius; nY <= tStart.nY + nRadius; nY += nRadius * 2)
				{
					ptFound = DungeonFindRoomGame(ptRoom, nX, nY);
					if (!ptFound)
						continue;
					ptRoom = ptFound;
					if (CollisnTestSize(ptRoom, nX, nY, nSize, dwPattern))
						continue;
					if (ptField && FieldTblsFindCandidate((DWORD)hField, (DWORD)ptRoom, (DWORD)nX, (DWORD)nY,
						dwFieldPattern))
						continue;
					nDist = CollisnAbs(nX - tStart.nX) + CollisnAbs(nY - tStart.nY);
					if (nBest == -1 || nDist < nBest)
					{
						ptSpawnPoint->nX = nX;
						ptSpawnPoint->nY = nY;
						nBest = nDist;
					}
				}
			}
		}
		if (nBest != -1)
			break;
	}

	if (hField)
		FieldTblsFreePair(hField);
	if (nBest != -1)
		return DungeonFindRoomGame(ptRoom, ptSpawnPoint->nX, ptSpawnPoint->nY);
	if (fFallback)
		return DungeonFindRoomGame(ptRoom, ptSpawnPoint->nX, ptSpawnPoint->nY);
	return NULL;
}

GAMEROOM * __stdcall CollisnFindSpawnPointNear(GAMEROOM *hRoom, PTCOLPOINT ptSpawnPoint, int nSize,
	DWORD dwPattern, BOOL fFallback)
{
	return CollisnFindSpawnPoint(hRoom, ptSpawnPoint, NULL, nSize, dwPattern, dwPattern, fFallback, 50, 1);
}

GAMEROOM * __stdcall CollisnFindSpawnPointStep(GAMEROOM *hRoom, PTCOLPOINT ptSpawnPoint, int nSize,
	DWORD dwPattern, int nStep)
{
	return CollisnFindSpawnPoint(hRoom, ptSpawnPoint, NULL, nSize, dwPattern, dwPattern, FALSE, 50, nStep);
}

GAMEROOM * __stdcall CollisnFindSpawnPointField(GAMEROOM *hRoom, PTCOLPOINT ptSpawnPoint, PTCOLPOINT ptField,
	int nSize, DWORD dwPattern, DWORD dwFieldPattern, BOOL fFallback)
{
	return CollisnFindSpawnPoint(hRoom, ptSpawnPoint, ptField, nSize, dwPattern, dwFieldPattern, fFallback,
		50, 1);
}

void __fastcall CollisnFindFreeArea(GAMEROOM *hRoom, PTCOLPOINT ptCoord, int nSize, DWORD dwPattern,
	GAMEROOM **pptRoom)
{
	GAMERECT atCoords[2];
	COLPOINT tStart;
	int n;
	int i;
	int j;
	GAMEROOM *ptRow;
	GAMEROOM *ptCell;

	tStart = *ptCoord;
	DungeonRoomCopyRects(hRoom, atCoords);
	for (n = 1; n < 50; n++)
	{
		for (i = -n; i < n; i += 2)
		{
			ptRow = hRoom;
			ptCoord->nY = tStart.nY + i;
			if (ptCoord->nY < atCoords[0].nY || ptCoord->nY >= atCoords[0].nY + atCoords[0].nHeight)
				ptRow = DungeonFindRoomGame(hRoom, ptCoord->nX, ptCoord->nY);
			if (!ptRow)
				continue;
			for (j = -n; j < n; j += 2)
			{
				ptCell = ptRow;
				DungeonRoomCopyRects(ptRow, atCoords);
				ptCoord->nX = tStart.nX + j;
				if (ptCoord->nX < atCoords[0].nX || ptCoord->nX >= atCoords[0].nX + atCoords[0].nWidth)
					ptCell = DungeonFindRoomGame(ptRow, ptCoord->nX, ptCoord->nY);
				if (ptCell &&
					!CollisnTestArea(ptCell, ptCoord->nX, ptCoord->nY, nSize + 2, nSize + 2, dwPattern))
				{
					*pptRoom = ptCell;
					return;
				}
			}
		}
	}
	*pptRoom = NULL;
}
