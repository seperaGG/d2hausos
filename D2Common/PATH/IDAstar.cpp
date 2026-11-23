#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\PATH\\IDAstar.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

DWORD __fastcall SeedNext(D2SEED *ptSeed);
int __stdcall PathUtilGetSquareDirection(struct TARGETCOORD tFrom, struct TARGETCOORD tTo);

#define MAXPROOM	50000
#define IDASTAR_MAX_NODES	900

struct ROOMCOORDS
{
 	int			nMinX;
 	int			nMinY;
 	int			nSizeGameX;
 	int			nSizeGameY;
 	int			anRect2[4];
};
typedef struct ROOMCOORDS ROOMCOORDS;

static const int sgcanIDAstarTurns[8][8] =
{
	{ 0, 1, 6, 3, 4, 5, 2, 7 }, { 0, 1, 6, 3, 1, 3, 6, 1 }, { 0, 1, 1, 1, 1, 1, 2, 7 }, { 1, 1, 1, 1, 1, 3,
	6, 1 }, { 6, 4, 3, 6, 1, 3, 2, 7 }, { 7, 7, 7, 7, 7, 5, 2, 7 }, { 0, 7, 7, 7, 7, 5, 2, 7 }, { 0, 7, 2,
	5, 7, 5, 2, 7 },
};

static const int sgcanIDAstarRandomTurn[32] =
{
	-2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2, -2, -1, 0, 1, 2,
	-1, 1,
};

static const int sgcanIDAstarDirOffsets[8][2] =
{
	{ 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 },
};

int ganIDAstarUnread[9] = { 5, 1, 6, 2, 0, 3, 8, 4, 7 };

struct IDASTARNODE
{
 	short		nEstimate;
 	short		nDistance;
 	short		nCost;
 	short		nTries;
 	struct TARGETCOORD	tCoord;
 	const int	*pnTurn;
 	int			nDir;
 	struct IDASTARNODE	*ptParent;
 	struct IDASTARNODE	*ptChild;
};
typedef struct IDASTARNODE IDASTARNODE, *PTIDASTARNODE;

struct IDASTARCONTEXT
{
 	PTIDASTARNODE	ptRoot;
 	IDASTARNODE	atNodes[IDASTAR_MAX_NODES];
 	int			nNodes;
 	WORD		wMinX;
 	WORD		wMinY;
 	WORD		wMaxX;
 	WORD		wMaxY;
 	WORD		wSizeX;
 	WORD		wSizeY;
 	int			nStride;
 	int			nOffsetX;
 	int			nOffsetY;
 	int			anCost[MAXPROOM];
 	BOOL		fRandom;
 	D2SEED		*ptSeed;
};
typedef struct IDASTARCONTEXT IDASTARCONTEXT, *PTIDASTARCONTEXT;

static PTIDASTARNODE __fastcall IDAstarSearch(PTIDASTARCONTEXT ptContext, int nThreshold,
	PTPATHINFO ptPathInfo);
static void __fastcall IDAstarTurn(PTIDASTARNODE ptNode, PTIDASTARCONTEXT ptContext);

static __inline int ABS(int nValue)	{ return nValue < 0 ? -nValue : nValue; }

static __inline int IDAstarEstimate(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	int nDX, nDY;

	nDX = ABS((int)tTo.wX - (int)tFrom.wX);
	nDY = ABS((int)tTo.wY - (int)tFrom.wY);
	return (nDX < nDY ? nDX : nDY) + (nDX > nDY ? nDX : nDY) * 2;
}

static PTIDASTARNODE IDAstarRunPasses(PTIDASTARCONTEXT ptContext, PTIDASTARNODE ptRootInit, int nThreshold,
	PTPATHINFO ptPathInfo)
{
	PTIDASTARNODE ptFound;
	int nReach;

	nReach = nThreshold;
	if (ptPathInfo->ePathType == PATH_TYPE_IDASTAR)
		nReach = (short)(nThreshold > ptPathInfo->bSearchReach ? nThreshold : ptPathInfo->bSearchReach);

	do
	{
		*ptContext->ptRoot = *ptRootInit;
		ptFound = IDAstarSearch(ptContext, nThreshold, ptPathInfo);
		nThreshold += 5;
		if (ptFound == NULL && ptContext->nNodes == IDASTAR_MAX_NODES)
			break;
		ptContext->nNodes = 1;
	} while (ptFound == NULL && nThreshold < nReach);
	return ptFound;
}

static int IDAstarBuildSteps(PTIDASTARNODE ptFound, PTPATHINFO ptPathInfo)
{
	DWORD adwSteps[100];
	PTIDASTARNODE ptNode;
	PTIDASTARNODE ptParent;
	int nDX, nDY;
	int nPrevDX, nPrevDY;
	int nSteps;
	int nCount;
	int nResult;
	DWORD *pdwDest;

	nResult = 0;
	if (ptFound != NULL)
	{
		pdwDest = &ptPathInfo->ptPath->adwSteps[1];
		nSteps = 0;
		nPrevDX = -2;
		nPrevDY = -2;
		for (ptNode = ptFound; ptNode != NULL; ptNode = ptNode->ptParent)
		{
			if (nSteps >= 100)
				break;
			ptParent = ptNode->ptParent;
			if (ptParent == NULL)
				continue;
			nDX = (int)ptNode->tCoord.wX - (int)ptParent->tCoord.wX;
			nDY = (int)ptNode->tCoord.wY - (int)ptParent->tCoord.wY;
			if (nDX != nPrevDX || nDY != nPrevDY)
			{
				adwSteps[99 - nSteps] = *(DWORD *)&ptNode->tCoord;
				nSteps++;
				nPrevDX = nDX;
				nPrevDY = nDY;
			}
		}
		if (nSteps > 1 && nSteps < 100)
		{
			memcpy(pdwDest, &adwSteps[100 - nSteps], nSteps * sizeof(DWORD));
			nCount = nSteps;
		}
		else
			nCount = 0;
		nResult = nCount;
		if (nCount >= 100)
			nResult = 0;
	}
	return nResult;
}

int __fastcall IDAstarFindPath(PTPATHINFO ptPathInfo)
{
	IDASTARCONTEXT tContext;
	IDASTARNODE tRootInit;
	ROOMCOORDS tRoom;
	ROOMCOORDS tOther;
	ROOMCOORDS tArea;
	ROOMCOORDS *ptRoomCoords;
	PTIDASTARNODE ptFound;
	struct TARGETCOORD tStart;
	struct TARGETCOORD tTarget;
	int nCells;
	int nDir;
	int i;

	ptPathInfo->bTargetRange *= 2;
	D2CommonDungeonRoomCopyRects(ptPathInfo->hRoomStart, &tArea);
	tRoom = tArea;
	if (ptPathInfo->hRoomTarget != NULL && ptPathInfo->hRoomStart != ptPathInfo->hRoomTarget)
	{
		D2CommonDungeonRoomCopyRects(ptPathInfo->hRoomTarget, &tOther);
		if (tOther.nMinX < tRoom.nMinX)
		{
			tArea.nMinX = tOther.nMinX;
			tArea.nSizeGameX = tRoom.nSizeGameX - tOther.nMinX + tRoom.nMinX;
		}
		else
		{
			tArea.nMinX = tRoom.nMinX;
			tArea.nSizeGameX = tOther.nSizeGameX - tRoom.nMinX + tOther.nMinX;
		}
		if (tOther.nMinY < tRoom.nMinY)
		{
			tArea.nMinY = tOther.nMinY;
			tArea.nSizeGameY = tRoom.nSizeGameY - tOther.nMinY + tRoom.nMinY;
		}
		else
		{
			tArea.nMinY = tRoom.nMinY;
			tArea.nSizeGameY = tOther.nSizeGameY - tRoom.nMinY + tOther.nMinY;
		}
	}
	if (tArea.nSizeGameX == tRoom.nSizeGameX)
	{
		tArea.nMinX -= 10;
		tArea.nSizeGameX += 20;
	}
	if (tArea.nSizeGameY == tRoom.nSizeGameY)
	{
		tArea.nMinY -= 10;
		tArea.nSizeGameY += 20;
	}
	ptRoomCoords = &tArea;
#line 369
	D2ASSERT(ptRoomCoords->nSizeGameX * ptRoomCoords->nSizeGameY <= MAXPROOM);

	nCells = (tArea.nSizeGameY + 6) * (tArea.nSizeGameX + 6);
	memset(tContext.anCost, 0, nCells * sizeof(tContext.anCost[0]));
	tContext.wMinX = (WORD)tArea.nMinX;
	tContext.wSizeX = (WORD)tArea.nSizeGameX;
	tContext.wMinY = (WORD)tArea.nMinY;
	tContext.wSizeY = (WORD)tArea.nSizeGameY;
	tContext.wMaxX = (WORD)(tArea.nSizeGameX + tArea.nMinX);
	tContext.nStride = (WORD)tArea.nSizeGameX + 6;
	tContext.wMaxY = (WORD)(tArea.nSizeGameY + tArea.nMinY);
	tContext.nOffsetX = 3 - (WORD)tArea.nMinX;
	tContext.nOffsetY = 3 - (WORD)tArea.nMinY;
	tContext.fRandom = FALSE;
	tStart = ptPathInfo->tStartCoord;
	tTarget = ptPathInfo->tTargetCoord;

	tRootInit.nCost = 0;
	tRootInit.nDistance = (short)IDAstarEstimate(tStart, tTarget);
	tRootInit.nEstimate = tRootInit.nDistance;
	tRootInit.tCoord = tStart;
	tRootInit.nTries = -3;
	nDir = PathUtilGetSquareDirection(tStart, tTarget);
	tRootInit.nDir = (nDir + sgcanIDAstarTurns[0][0]) & 7;
	tRootInit.ptChild = NULL;
	tRootInit.ptParent = NULL;
	tContext.nNodes = 1;
	tRootInit.pnTurn = sgcanIDAstarTurns[0];
	for (i = 0; i < 7; i++)
		((DWORD *)&tContext.atNodes[0])[i] = 0;
	tContext.ptRoot = &tContext.atNodes[0];

	ptFound = IDAstarRunPasses(&tContext, &tRootInit, tRootInit.nDistance, ptPathInfo);

	return IDAstarBuildSteps(ptFound, ptPathInfo);
}

static void __fastcall IDAstarTurn(PTIDASTARNODE ptNode, PTIDASTARCONTEXT ptContext)
{
	if (ptContext->fRandom)
		ptNode->nDir = (sgcanIDAstarRandomTurn[SeedNext(ptContext->ptSeed) & 31] + *ptNode->pnTurn +
			ptNode->nDir) & 7;
	else
		ptNode->nDir = (*ptNode->pnTurn + ptNode->nDir) & 7;
}

static __inline PTIDASTARNODE IDAstarNextTry(PTIDASTARNODE ptNode, PTIDASTARCONTEXT ptContext)
{
	if (ptNode->nTries < 4)
	{
		ptNode->pnTurn++;
		IDAstarTurn(ptNode, ptContext);
	}
	ptNode->nTries++;
	while (ptNode->nTries == 5)
	{
		if (ptNode == ptContext->ptRoot)
			return NULL;
		ptNode = ptNode->ptParent;
		ptNode->pnTurn++;
		IDAstarTurn(ptNode, ptContext);
		ptNode->nTries++;
	}
	return ptNode;
}

static __inline BOOL IDAstarOutside(PTIDASTARCONTEXT ptContext, struct TARGETCOORD tCoord)
{
	return tCoord.wX < ptContext->wMinX || tCoord.wX > ptContext->wMaxX ||
		tCoord.wY < ptContext->wMinY || tCoord.wY > ptContext->wMaxY;
}

static __inline int IDAstarStepCost(PTIDASTARNODE ptNode, struct TARGETCOORD tNext)
{
	if (ptNode->tCoord.wX == tNext.wX || ptNode->tCoord.wY == tNext.wY)
		return 2;
	return 3;
}

static __inline PTIDASTARNODE IDAstarNewChild(PTIDASTARCONTEXT ptContext)
{
	PTIDASTARNODE ptNode;

	if (ptContext->nNodes == IDASTAR_MAX_NODES)
		return NULL;
	ptNode = &ptContext->atNodes[ptContext->nNodes++];
	memset(ptNode, 0, sizeof(IDASTARNODE));
	return ptNode;
}

static __inline void IDAstarFirstTurn(PTIDASTARNODE ptNode, PTIDASTARCONTEXT ptContext)
{
	if (ptContext->fRandom)
		ptNode->nDir = (sgcanIDAstarRandomTurn[SeedNext(ptContext->ptSeed) & 31] + ptNode->ptParent->nDir +
			*ptNode->pnTurn) & 7;
	else
		ptNode->nDir = (ptNode->ptParent->nDir + *ptNode->pnTurn) & 7;
}

static PTIDASTARNODE __fastcall IDAstarSearch(PTIDASTARCONTEXT ptContext, int nThreshold,
	PTPATHINFO ptPathInfo)
{
	PTIDASTARNODE ptNode;
	PTIDASTARNODE ptChild;
	struct TARGETCOORD tTarget;
	struct TARGETCOORD tNext;
	int *pnCost;
	short nCost;
	short nEstimate;
	int nDistance;
	int nMoves;
	int nDir;

	tTarget = ptPathInfo->tTargetCoord;
	ptNode = ptContext->ptRoot;
	nMoves = 0;
	for (;;)
	{
		if (ptNode->tCoord.wX == tTarget.wX && ptNode->tCoord.wY == tTarget.wY)
			return ptNode;
		if (++nMoves > 10000)
			return NULL;
		if (IDAstarOutside(ptContext, ptNode->tCoord))
			return ptNode;
		tNext.wX = (WORD)(ptNode->tCoord.wX + sgcanIDAstarDirOffsets[ptNode->nDir][0]);
		tNext.wY = (WORD)(ptNode->tCoord.wY + sgcanIDAstarDirOffsets[ptNode->nDir][1]);
		pnCost = &ptContext->anCost[(tNext.wY + ptContext->nOffsetY) * ptContext->nStride +
			ptContext->nOffsetX + tNext.wX];
		if (*pnCost == 0 && D2CommonCollisnTestPatternAnyRaw((DWORD)ptPathInfo->hRoomStart, tNext.wX,
			tNext.wY, ptPathInfo->dwCollisionPattern, ptPathInfo->dwCollisionTestMask))
		{
			*pnCost = 1;
			ptNode = IDAstarNextTry(ptNode, ptContext);
			if (ptNode == NULL)
				return NULL;
			continue;
		}
		nCost = (short)(ptNode->nCost + IDAstarStepCost(ptNode, tNext));
		if (*pnCost != 0 && (DWORD)*pnCost < (DWORD)(int)nCost)
		{
			ptNode = IDAstarNextTry(ptNode, ptContext);
			if (ptNode == NULL)
				return NULL;
			continue;
		}
		*pnCost = nCost;
		nDistance = IDAstarEstimate(tNext, tTarget);
		nEstimate = (short)(nDistance + nCost);
		if (nEstimate > nThreshold)
		{
			ptNode = IDAstarNextTry(ptNode, ptContext);
			if (ptNode == NULL)
				return NULL;
			continue;
		}
		if (ptNode->ptChild == NULL)
		{
			ptChild = IDAstarNewChild(ptContext);
			ptNode->ptChild = ptChild;
			if (ptChild == NULL)
				return NULL;
			ptChild->ptParent = ptNode;
		}
		ptNode = ptNode->ptChild;
		ptNode->nDistance = (short)nDistance;
		ptNode->nCost = nCost;
		ptNode->nEstimate = nEstimate;
		ptNode->nTries = 0;
		nDir = PathUtilGetSquareDirection(tNext, tTarget);
		ptNode->pnTurn = sgcanIDAstarTurns[(nDir - ptNode->ptParent->nDir) & 7];
		IDAstarFirstTurn(ptNode, ptContext);
		ptNode->tCoord = tNext;
		if (ptNode->nDistance < (short)ptPathInfo->bTargetRange)
			return ptNode;
	}
}
