#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\PATH\\AStar.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

#define ASTAR_MAX_NODES		200
#define ASTAR_HASH_SIZE		128

static const int sgcanAStarHashX[ASTAR_HASH_SIZE] =
{
	57, 92, 72, 94, 119, 80, 14, 84, 93, 17, 20, 37, 87, 70, 100, 108, 30, 91, 85, 60, 40, 3, 118, 78, 81,
	109, 43, 26, 67, 27, 49, 75, 69, 2, 58, 36, 55, 5, 122, 125, 11, 35, 77, 59, 29, 18, 28, 82, 63, 86, 83,
	105, 90, 48, 101, 38, 115, 9, 41, 89, 95, 19, 1, 61, 51, 7, 110, 34, 126, 47, 15, 6, 99, 22, 104, 4, 71,
	97, 121, 114, 0, 112, 62, 54, 23, 111, 74, 123, 45, 120, 44, 50, 31, 39, 42, 66, 33, 113, 53, 64, 16,
	65, 106, 96, 88, 24, 46, 56, 103, 68, 10, 117, 25, 32, 124, 127, 76, 8, 79, 21, 102, 52, 107, 73, 13,
	116, 98, 12,
};

static const int sgcanAStarHashY[ASTAR_HASH_SIZE] =
{
	114, 80, 68, 91, 97, 73, 127, 88, 18, 58, 101, 41, 19, 108, 6, 8, 24, 99, 83, 33, 77, 56, 74, 37, 43,
	107, 85, 12, 75, 30, 27, 23, 92, 69, 115, 2, 84, 7, 17, 20, 117, 67, 31, 121, 111, 105, 120, 96, 94,
	122, 47, 25, 4, 112, 34, 62, 10, 89, 1, 40, 76, 71, 49, 51, 90, 55, 118, 50, 116, 106, 109, 42, 45, 65,
	28, 60, 123, 36, 98, 48, 124, 22, 64, 38, 15, 102, 110, 79, 57, 125, 59, 21, 70, 63, 93, 86, 66, 35, 16,
	53, 61, 103, 29, 52, 119, 46, 100, 3, 0, 126, 32, 81, 87, 9, 26, 14, 95, 113, 13, 54, 104, 82, 44, 72,
	5, 78, 11, 39,
};

#define ASTAR_HASH(tCoord)	\
	((sgcanAStarHashX[(tCoord).wX & 127] + sgcanAStarHashY[(tCoord).wY & 127]) & 127)

struct ASTARNODE
{
 	struct TARGETCOORD	tCoord;
 	short		nEstimate;
 	short		nDistance;
 	short		nCost;
 	BYTE		unobserved_a[2];
 	struct ASTARNODE	*ptParent;
 	struct ASTARNODE	*aptChild[8];
 	struct ASTARNODE	*ptHashNext;
 	struct ASTARNODE	*ptOpenNext;
};
typedef struct ASTARNODE ASTARNODE, *PTASTARNODE;

struct ASTARCONTEXT
{
 	PTASTARNODE	aptOpen[ASTAR_HASH_SIZE];
 	PTASTARNODE	aptClosed[ASTAR_HASH_SIZE];
 	PTASTARNODE	ptOpenList;
 	ASTARNODE	atNodes[ASTAR_MAX_NODES];
 	int			nNodes;
 	PTASTARNODE	aptStack[ASTAR_MAX_NODES];
 	int			nStack;
};
typedef struct ASTARCONTEXT ASTARCONTEXT, *PTASTARCONTEXT;

static void __fastcall IDAstarAddClosed(PTASTARCONTEXT ptContext, PTASTARNODE ptNode);
static BOOL __fastcall IDAstarExpand(PTPATHINFO ptPathInfo, PTASTARCONTEXT ptContext, PTASTARNODE ptNode,
	struct TARGETCOORD tTarget);
static PTASTARNODE __fastcall IDAstarNewNode(PTASTARCONTEXT ptContext);
static inline void __fastcall IDAstarAddOpen(PTASTARCONTEXT ptContext, PTASTARNODE ptNode);
static inline BOOL __fastcall IDAstarVisit(PTPATHINFO ptPathInfo, PTASTARCONTEXT ptContext, PTASTARNODE ptNode,
	struct TARGETCOORD tNext, struct TARGETCOORD tTarget);
static int __stdcall IDAstarDistance(struct TARGETCOORD tFrom, struct TARGETCOORD tTo);
static int __fastcall IDAstarBuildPath(PTASTARNODE ptNode, PTPATHINFO ptPathInfo);

#define IDASTAR_ADD_CHILD(ptNode, ptChild) \
	if ((ptNode)->aptChild[0] == NULL) (ptNode)->aptChild[0] = (ptChild); \
	else if ((ptNode)->aptChild[1] == NULL) (ptNode)->aptChild[1] = (ptChild); \
	else if ((ptNode)->aptChild[2] == NULL) (ptNode)->aptChild[2] = (ptChild); \
	else if ((ptNode)->aptChild[3] == NULL) (ptNode)->aptChild[3] = (ptChild); \
	else if ((ptNode)->aptChild[4] == NULL) (ptNode)->aptChild[4] = (ptChild); \
	else if ((ptNode)->aptChild[5] == NULL) (ptNode)->aptChild[5] = (ptChild); \
	else if ((ptNode)->aptChild[6] == NULL) (ptNode)->aptChild[6] = (ptChild); \
	else if ((ptNode)->aptChild[7] == NULL) (ptNode)->aptChild[7] = (ptChild)

static PTASTARNODE IDAstarTakeFirst(PTASTARCONTEXT ptContext)
{
	PTASTARNODE ptNode;
	PTASTARNODE ptPrev;
	PTASTARNODE ptCur;
	int nHash;

	if (ptContext->ptOpenList == NULL)
		return NULL;
	ptNode = ptContext->ptOpenList;
	ptContext->ptOpenList = ptNode->ptOpenNext;
	nHash = ASTAR_HASH(ptNode->tCoord);
	ptPrev = NULL;
	for (ptCur = ptContext->aptOpen[nHash]; ptCur != ptNode; ptCur = ptCur->ptHashNext)
		ptPrev = ptCur;
	if (ptPrev == NULL)
		ptContext->aptOpen[nHash] = ptNode->ptHashNext;
	else
		ptPrev->ptHashNext = ptNode->ptHashNext;
	ptNode->ptHashNext = NULL;
	IDAstarAddClosed(ptContext, ptNode);
	return ptNode;
}

static int IDAstarRunSearch(PTPATHINFO ptPathInfo)
{
	ASTARCONTEXT tContext;
	PTASTARNODE ptNode;
	PTASTARNODE ptBest;
	struct TARGETCOORD tStart;
	struct TARGETCOORD tTarget;
	int nDistance;
	int nResult;
	int i;

	tContext.nNodes = 0;
	tContext.ptOpenList = NULL;
	tContext.nStack = 0;
	nResult = 0;
	for (i = 0; i < ASTAR_HASH_SIZE; i++)
		tContext.aptOpen[i] = NULL;
	for (i = 0; i < ASTAR_HASH_SIZE; i++)
		tContext.aptClosed[i] = NULL;
	tStart = ptPathInfo->tStartCoord;
	tTarget = ptPathInfo->tTargetCoord;

	ptNode = IDAstarNewNode(&tContext);
	ptNode->nCost = 0;
	nDistance = IDAstarDistance(tStart, tTarget);
	ptNode->nDistance = (short)nDistance;
	ptNode->nEstimate = (short)nDistance;
	ptNode->tCoord = tStart;
	IDAstarAddOpen(&tContext, ptNode);

	ptBest = NULL;
	while ((ptNode = IDAstarTakeFirst(&tContext)) != NULL)
	{
		if (ptBest == NULL || ptNode->nDistance < ptBest->nDistance ||
			(ptNode->nDistance == ptBest->nDistance && ptNode->nCost > ptBest->nCost + 5))
			ptBest = ptNode;
		if (ptNode->nDistance == 0)
			break;
		if (!IDAstarExpand(ptPathInfo, &tContext, ptNode, tTarget))
			break;
	}
	if (ptBest != NULL)
		nResult = IDAstarBuildPath(ptBest, ptPathInfo);
	return nResult;
}

int __fastcall PathFindAStar(PTPATHINFO ptPathInfo)
{
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	int nX, nY;

	if (ptPathInfo->ptPath->hTarget != NULL)
	{
		hRoom = ptPathInfo->hRoomStart;
		nY = ptPathInfo->tTargetCoord.wY;
		dwPattern = ptPathInfo->dwCollisionPattern;
		dwMask = ptPathInfo->dwCollisionTestMask;
		nX = ptPathInfo->tTargetCoord.wX;
		if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX - 2, nY - 2, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX - 2, nY + 2, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX + 2, nY - 2, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX + 2, nY + 2, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX - 2, nY, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX, nY - 2, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX + 2, nY, dwPattern, dwMask) &&
			D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX, nY + 2, dwPattern, dwMask))
			return 0;
	}
	return IDAstarRunSearch(ptPathInfo);
}

static void __fastcall IDAstarAddClosed(PTASTARCONTEXT ptContext, PTASTARNODE ptNode)
{
	int nHash;

	nHash = ASTAR_HASH(ptNode->tCoord);
	ptNode->ptHashNext = ptContext->aptClosed[nHash];
	ptContext->aptClosed[nHash] = ptNode;
}

static BOOL __fastcall IDAstarExpand(PTPATHINFO ptPathInfo, PTASTARCONTEXT ptContext, PTASTARNODE ptNode,
	struct TARGETCOORD tTarget)
{
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	struct TARGETCOORD tNext;

	hRoom = ptPathInfo->hRoomStart;
	dwPattern = ptPathInfo->dwCollisionPattern;
	dwMask = ptPathInfo->dwCollisionTestMask;
	tNext.wX = (WORD)(ptNode->tCoord.wX - 1);
	tNext.wY = (WORD)(ptNode->tCoord.wY - 1);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = (WORD)(ptNode->tCoord.wX - 1);
	tNext.wY = (WORD)(ptNode->tCoord.wY + 1);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = (WORD)(ptNode->tCoord.wX + 1);
	tNext.wY = (WORD)(ptNode->tCoord.wY - 1);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = (WORD)(ptNode->tCoord.wX + 1);
	tNext.wY = (WORD)(ptNode->tCoord.wY + 1);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = (WORD)(ptNode->tCoord.wX - 1);
	tNext.wY = ptNode->tCoord.wY;
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = ptNode->tCoord.wX;
	tNext.wY = (WORD)(ptNode->tCoord.wY - 1);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = (WORD)(ptNode->tCoord.wX + 1);
	tNext.wY = ptNode->tCoord.wY;
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	tNext.wX = ptNode->tCoord.wX;
	tNext.wY = (WORD)(ptNode->tCoord.wY + 1);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask) &&
		!IDAstarVisit(ptPathInfo, ptContext, ptNode, tNext, tTarget))
		return FALSE;
	return TRUE;
}

static int __stdcall IDAstarDistance(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	int nDX, nDY;

	nDX = (int)tFrom.wX - (int)tTo.wX;
	if (nDX < 0)
		nDX = -nDX;
	nDY = (int)tFrom.wY - (int)tTo.wY;
	if (nDY < 0)
		nDY = -nDY;
	if (nDX >= nDY)
		return nDY + nDX * 2;
	return nDX + nDY * 2;
}

static PTASTARNODE __fastcall IDAstarFindOpen(PTASTARCONTEXT ptContext,
	struct TARGETCOORD tCoord)
{
	PTASTARNODE ptNode;

	for (ptNode = ptContext->aptOpen[ASTAR_HASH(tCoord)]; ptNode != NULL; ptNode = ptNode->ptHashNext)
	{
		if (tCoord.wX == ptNode->tCoord.wX && tCoord.wY == ptNode->tCoord.wY)
			return ptNode;
	}
	return NULL;
}

static PTASTARNODE __fastcall IDAstarFindClosed(PTASTARCONTEXT ptContext,
	struct TARGETCOORD tCoord)
{
	PTASTARNODE ptNode;

	for (ptNode = ptContext->aptClosed[ASTAR_HASH(tCoord)]; ptNode != NULL; ptNode = ptNode->ptHashNext)
	{
		if (tCoord.wX == ptNode->tCoord.wX && tCoord.wY == ptNode->tCoord.wY)
			return ptNode;
	}
	return NULL;
}

static inline void __fastcall IDAstarAddOpen(PTASTARCONTEXT ptContext, PTASTARNODE ptNode)
{
	PTASTARNODE ptCur;
	PTASTARNODE ptPrev;
	int nHash;
	int nEstimate;

	ptCur = ptContext->ptOpenList;
	ptPrev = NULL;
	nHash = ASTAR_HASH(ptNode->tCoord);
	ptNode->ptHashNext = ptContext->aptOpen[nHash];
	ptContext->aptOpen[nHash] = ptNode;
	nEstimate = ptNode->nEstimate;
	for (; ptCur != NULL && ptCur->nEstimate < nEstimate; ptCur = ptCur->ptOpenNext)
		ptPrev = ptCur;
	if (ptPrev != NULL)
	{
		ptPrev->ptOpenNext = ptNode;
		ptNode->ptOpenNext = ptCur;
		return;
	}
	ptNode->ptOpenNext = ptContext->ptOpenList;
	ptContext->ptOpenList = ptNode;
}

static inline void __fastcall IDAstarPropagate(PTASTARCONTEXT ptContext, PTPATHINFO ptPathInfo, PTASTARNODE ptNode)
{
	PTASTARNODE ptChild;
	short nCost;
	int nStep;
	int i;

	ptContext->aptStack[ptContext->nStack] = ptNode;
	ptContext->nStack++;
	while (ptContext->nStack != 0)
	{
		ptContext->nStack--;
		ptNode = ptContext->aptStack[ptContext->nStack];
		for (i = 0; i < 8; i++)
		{
			ptChild = ptNode->aptChild[i];
			if (ptChild == NULL)
				break;
			if (ptNode->tCoord.wX == ptChild->tCoord.wX || ptNode->tCoord.wY == ptChild->tCoord.wY)
				nStep = 2;
			else
				nStep = 3;
			nCost = (short)(ptNode->nCost + nStep);
			if (nCost < ptChild->nCost)
			{
				ptChild->ptParent = ptNode;
				ptChild->nCost = nCost;
				ptChild->nEstimate = (short)(ptChild->nDistance + nCost);
				ptContext->aptStack[ptContext->nStack] = ptChild;
				ptContext->nStack++;
			}
		}
	}
}

static PTASTARNODE __fastcall IDAstarNewNode(PTASTARCONTEXT ptContext)
{
	PTASTARNODE ptNode;
	int i;

	if (ptContext->nNodes == ASTAR_MAX_NODES)
		return NULL;
	ptNode = &ptContext->atNodes[ptContext->nNodes++];
	for (i = 0; i < (int)(sizeof(ASTARNODE) / sizeof(DWORD)); i++)
		((DWORD *)ptNode)[i] = 0;
	return ptNode;
}

static inline BOOL __fastcall IDAstarVisit(PTPATHINFO ptPathInfo, PTASTARCONTEXT ptContext, PTASTARNODE ptNode,
	struct TARGETCOORD tNext, struct TARGETCOORD tTarget)
{
	PTASTARNODE ptFound;
	short nCost;
	int nStep;

	if (ptNode->tCoord.wX == tNext.wX || ptNode->tCoord.wY == tNext.wY)
		nStep = 2;
	else
		nStep = 3;
	nCost = (short)(ptNode->nCost + nStep);
	ptFound = IDAstarFindOpen(ptContext, tNext);

	if (ptFound != NULL)
	{
		IDASTAR_ADD_CHILD(ptNode, ptFound);
		if (nCost < ptFound->nCost)
		{
			ptFound->ptParent = ptNode;
			ptFound->nCost = nCost;
			ptFound->nEstimate = (short)(ptFound->nDistance + nCost);
		}
		return TRUE;
	}
	ptFound = IDAstarFindClosed(ptContext, tNext);
	if (ptFound != NULL)
	{
		IDASTAR_ADD_CHILD(ptNode, ptFound);
		if (nCost < ptFound->nCost)
		{
			ptFound->ptParent = ptNode;
			ptFound->nCost = nCost;
			ptFound->nEstimate = (short)(ptFound->nDistance + nCost);
			IDAstarPropagate(ptContext, ptPathInfo, ptFound);
		}
		return TRUE;
	}
	ptFound = IDAstarNewNode(ptContext);
	if (ptFound == NULL)
		return FALSE;
	ptFound->ptParent = ptNode;
	ptFound->nCost = nCost;
	ptFound->nDistance = (short)IDAstarDistance(tNext, tTarget);
	ptFound->nEstimate = (short)(ptFound->nDistance + nCost);
	ptFound->tCoord = tNext;
	IDAstarAddOpen(ptContext, ptFound);
	IDASTAR_ADD_CHILD(ptNode, ptFound);
	return TRUE;
}

static int __fastcall IDAstarBuildPath(PTASTARNODE ptNode, PTPATHINFO ptPathInfo)
{
	DWORD adwSteps[100];
	DWORD *pdwOut;
	int nPrevDX, nPrevDY;
	int nDX, nDY;
	int nSteps;

	pdwOut = &ptPathInfo->ptPath->adwSteps[1];
	nSteps = 0;
	nPrevDX = -2;
	nPrevDY = -2;
	if (ptNode == NULL)
		return 0;
	for (; ptNode != NULL && nSteps < 100; ptNode = ptNode->ptParent)
	{
		if (ptNode->ptParent == NULL)
			continue;
		nDX = (int)ptNode->tCoord.wX - (int)ptNode->ptParent->tCoord.wX;
		nDY = (int)ptNode->tCoord.wY - (int)ptNode->ptParent->tCoord.wY;
		if (nDX != nPrevDX || nDY != nPrevDY)
		{
			adwSteps[99 - nSteps] = *(DWORD *)&ptNode->tCoord;
			nSteps++;
			nPrevDX = nDX;
			nPrevDY = nDY;
		}
	}
	if (nSteps < 1 || nSteps >= 100)
		return 0;
	memcpy(pdwOut, &adwSteps[100 - nSteps], nSteps * sizeof(DWORD));
	return nSteps;
}
