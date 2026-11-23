#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutEntry.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

struct OUTDOORSTEP
{
 	int			nTotal;
 	int			nEstimate;
 	int			nCost;
 	int			nX;
 	int			nY;
 	int			nTurnsTried;
 	const BYTE	*pbTurn;
 	BYTE		bDirection;
 	BYTE		unobserved_1d[3];
 	struct OUTDOORSTEP	*ptPrev;
 	struct OUTDOORSTEP	*ptNext;
};
typedef struct OUTDOORSTEP OUTDOORSTEP;

struct OUTDOORSTEPPOOL
{
	OUTDOORSTEP	*ptRoot;
	OUTDOORSTEP	atSteps[900];
	int			nUsed;
};
typedef struct OUTDOORSTEPPOOL OUTDOORSTEPPOOL;

static const BYTE sgcabStepTurns[4][4] =
{
	{ 0, 1, 2, 3 }, { 0, 1, 1, 1 }, { 3, 2, 1, 2 }, { 0, 3, 2, 1 },
};
static const char sgcanStepX[4] = { 1, 0, -1, 0 };
static const char sgcanStepY[4] = { 0, 1, 0, -1 };

static OUTDOORSTEP *OutdoorsStepAlloc(OUTDOORSTEPPOOL *ptPool)
{
	OUTDOORSTEP *ptStep;

	if (ptPool->nUsed == 900)
		return NULL;
	ptStep = &ptPool->atSteps[ptPool->nUsed++];
	memset(ptStep, 0, sizeof(OUTDOORSTEP));
	return ptStep;
}

static OUTDOORSTEP *OutdoorsStepNextTurn(OUTDOORSTEPPOOL *ptPool, OUTDOORSTEP *ptStep)
{
	if (ptStep->nTurnsTried < 4)
	{
		ptStep->pbTurn++;
		ptStep->bDirection = (ptStep->bDirection + *ptStep->pbTurn) & 3;
	}
	ptStep->nTurnsTried++;
	while (ptStep->nTurnsTried == 3)
	{
		if (ptStep == ptPool->ptRoot)
			return NULL;
		ptStep = ptStep->ptPrev;
		ptStep->pbTurn++;
		ptStep->bDirection = (ptStep->bDirection + *ptStep->pbTurn) & 3;
		ptStep->nTurnsTried++;
	}
	return ptStep;
}

static __inline int OutdoorsAbs(int nValue)
{
	return nValue < 0 ? -nValue : nValue;
}

static __inline int OutdoorsEstimate(int nFromX, int nFromY, int nToX, int nToY)
{
	int nDX, nDY;

	nDX = OutdoorsAbs(nFromX - nToX);
	nDY = OutdoorsAbs(nFromY - nToY);
	return (nDX < nDY ? nDX : nDY) + (nDX > nDY ? nDX : nDY) * 2;
}

static __inline BOOL OutdoorsCellFree(OUTDOORINFO *ptOutdoor, int nX, int nY)
{
	if (!DrlgRectContains(&ptOutdoor->tCellRect, nX, nY))
		return FALSE;
	if (DrlgGridGet(&ptOutdoor->tCellFlagGrid, nX, nY) & 512)
		return FALSE;
	return TRUE;
}

static __inline OUTDOORSTEP *OutdoorsSearch(OUTDOORSTEPPOOL *ptPool, DRLGLEVEL *ptLevel,
	const OUTDOORENTRY *ptTo, int nLimit)
{
	OUTDOORSTEP *ptStep;
	OUTDOORSTEP *ptNext;
	BYTE bTurn;
	int nStepX;
	int nStepY;
	int nStepCost;
	int nCost;
	int nEstimate;
	int nTotal;
	BOOL fOpen;

	ptStep = ptPool->ptRoot;
	while (ptStep->nX != ptTo->nX || ptStep->nY != ptTo->nY)
	{
		nStepX = ptStep->nX + sgcanStepX[ptStep->bDirection];
		nStepY = ptStep->nY + sgcanStepY[ptStep->bDirection];
		fOpen = TRUE;
		if (nStepX != ptTo->nX || nStepY != ptTo->nY)
		{
			fOpen = OutdoorsCellFree(ptLevel->field_38.pOutdoor, nStepX, nStepY);
			if (fOpen)
			{
				for (ptNext = ptStep; ptNext; ptNext = ptNext->ptPrev)
				{
					if (ptNext->nX == nStepX && ptNext->nY == nStepY)
					{
						fOpen = FALSE;
						break;
					}
				}
			}
		}
		if (fOpen)
		{
			if (ptStep->nX == nStepX || ptStep->nY == nStepY)
				nStepCost = 2;
			else
				nStepCost = 3;
			nCost = ptStep->nCost + nStepCost;
			nEstimate = OutdoorsEstimate(nStepX, nStepY, ptTo->nX, ptTo->nY);
			nTotal = nEstimate + nCost;
			if (nTotal > nLimit)
			{
				ptStep = OutdoorsStepNextTurn(ptPool, ptStep);
				if (!ptStep)
					return NULL;
				continue;
			}
		}
		if (!fOpen)
		{
			ptStep = OutdoorsStepNextTurn(ptPool, ptStep);
			if (!ptStep)
				return NULL;
			continue;
		}
		if (!ptStep->ptNext)
		{
			ptNext = OutdoorsStepAlloc(ptPool);
			ptStep->ptNext = ptNext;
			if (!ptNext)
				return NULL;
			ptNext->ptPrev = ptStep;
		}
		ptStep = ptStep->ptNext;
		ptStep->nCost = nCost;
		ptStep->nEstimate = nEstimate;
		ptStep->nTotal = nTotal;
		ptStep->nTurnsTried = 0;
		bTurn = PathUtilGetDirection(nStepX, nStepY, ptTo->nX, ptTo->nY) / 2;
		ptStep->pbTurn = sgcabStepTurns[(BYTE)(ptStep->ptPrev->bDirection - bTurn) & 3];
		ptStep->bDirection = (*ptStep->pbTurn + bTurn) & 3;
		ptStep->nX = nStepX;
		ptStep->nY = nStepY;
	}
	return ptStep;
}

BOOL __fastcall OutdoorsFindEntryPath(DRLGLEVEL *ptLevel, int nEntry)
{
	OUTDOORSTEPPOOL tPool;
	OUTDOORSTEP tFirst;
	OUTDOORSTEP *ptStep;
	OUTDOORINFO *ptOutdoor;
	DRLGVERTEX *ptPrev;
	void *pPool;
	int nX;
	int nY;
	OUTDOORENTRY tTo;
	BYTE bTurn;
	int nLimit;
	int nLimitEnd;

	pPool = ptLevel->ptAct->pMemPool;
	ptOutdoor = ptLevel->field_38.pOutdoor;
	nX = (ptOutdoor->atEntryEdges[nEntry].nX - ptLevel->tCoords.nTileX) / 8;
	nY = (ptOutdoor->atEntryEdges[nEntry].nY - ptLevel->tCoords.nTileY) / 8;
	tTo.nX = (ptOutdoor->atMeetingEdges[nEntry].nX - ptLevel->tCoords.nTileX) / 8;
	tTo.nY = (ptOutdoor->atMeetingEdges[nEntry].nY - ptLevel->tCoords.nTileY) / 8;
	if (OutdoorsAbs(nX - tTo.nX) + OutdoorsAbs(nY - tTo.nY) < 2)
	{
		ptOutdoor->aptEntryPaths[nEntry] = DrlgVerNew(pPool, 0);
		ptOutdoor->aptEntryPaths[nEntry]->nX = nX;
		ptOutdoor->aptEntryPaths[nEntry]->nY = nY;
		ptOutdoor->aptEntryPaths[nEntry]->ptNext = DrlgVerNew(pPool, 0);
		ptOutdoor->aptEntryPaths[nEntry]->ptNext->nX = tTo.nX;
		ptOutdoor->aptEntryPaths[nEntry]->ptNext->nY = tTo.nY;
		return TRUE;
	}
	bTurn = PathUtilGetDirection(nX, nY, tTo.nX, tTo.nY) / 2;
	tFirst.nCost = 0;
	tFirst.nEstimate = OutdoorsEstimate(nX, nY, tTo.nX, tTo.nY);
	tFirst.nTotal = tFirst.nEstimate;
	tFirst.nX = nX;
	tFirst.nY = nY;
	tFirst.nTurnsTried = -1;
	tFirst.pbTurn = sgcabStepTurns[0];
	tFirst.bDirection = (*tFirst.pbTurn + bTurn) & 3;
	tFirst.ptNext = NULL;
	tFirst.ptPrev = NULL;
	tPool.nUsed = 1;
	memset(&tPool.atSteps[0], 0, sizeof(OUTDOORSTEP));
	tPool.ptRoot = &tPool.atSteps[0];
	nLimit = tFirst.nEstimate / 2 + tFirst.nEstimate;
	nLimitEnd = nLimit + 35;
	do
	{
		*tPool.ptRoot = tFirst;
		ptStep = OutdoorsSearch(&tPool, ptLevel, &tTo, nLimit);
		nLimit += 5;
		if (tPool.nUsed >= 900)
			return FALSE;
		tPool.nUsed = 1;
	}
	while (!ptStep && nLimit < nLimitEnd);
	if (!ptStep)
		return FALSE;
	ptPrev = NULL;
	for (; ptStep; ptStep = ptStep->ptPrev)
	{
		if (ptOutdoor->aptEntryPaths[nEntry])
		{
			ptPrev->ptNext = DrlgVerNew(pPool, 0);
			ptPrev = ptPrev->ptNext;
		}
		else
		{
			ptOutdoor->aptEntryPaths[nEntry] = DrlgVerNew(pPool, 0);
			ptPrev = ptOutdoor->aptEntryPaths[nEntry];
		}
		ptPrev->nX = ptStep->nX;
		ptPrev->nY = ptStep->nY;
	}
	return TRUE;
}
