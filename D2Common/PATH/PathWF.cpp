#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Path\\PathWF.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

#define WF_MAX_STEPS	201

struct PATHDIROFFSET
{
	int			nX;
	int			nY;
};
static const struct PATHDIROFFSET sgcanWFDirOffsets[16] =
{
	{ 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 },
	{ 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 },
};

static const int sgcanWFDeltaDirection[9] =
{
	7, 0, 1, 6, -1, 2, 5, 4, 3,
};

static const int sgcanWFTurnA1[16] = { 6, 6, 0, 0, 2, 2, 4, 4, 6, 6, 0, 0, 2, 2, 4, 4 };
static const int sgcanWFTurnA2[16] = { 2, 2, 4, 4, 6, 6, 0, 0, 2, 2, 4, 4, 6, 6, 0, 0 };
static const int sgcanWFTurnB1[16] = { 2, 2, 4, 4, 6, 6, 0, 0, 2, 2, 4, 4, 6, 6, 0, 0 };
static const int sgcanWFTurnB2[16] = { 6, 0, 0, 2, 2, 4, 4, 6, 6, 0, 0, 2, 2, 4, 4, 6 };

struct WFPATH
{
 	int			nDir;
 	int			nLoopLen;
 	struct TARGETCOORD	tCur;
 	struct TARGETCOORD	tNext;
 	BOOL		fLastTry;
 	const int	*pnTurnFree;
 	const int	*pnTurnBlocked;
 	int			nMeet;
 	BOOL		fDone;
 	struct TARGETCOORD	atSteps[WF_MAX_STEPS];
};
typedef struct WFPATH WFPATH, *PTWFPATH;

static __inline BOOL PathWFTestNext(PTWFPATH ptPath, struct TARGETCOORD tCur, GAMEROOM *hRoom, DWORD dwPattern, DWORD dwMask)
{
	int nDir;

	nDir = ptPath->nDir;
	ptPath->tNext.wX = (WORD)(tCur.wX + sgcanWFDirOffsets[nDir].nX);
	ptPath->tNext.wY = (WORD)(tCur.wY + sgcanWFDirOffsets[nDir].nY);
	return D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, ptPath->tNext.wX, ptPath->tNext.wY, dwPattern,
		dwMask);
}

static __inline const int *PathWFBlockedTurns(PTWFPATH ptPath, int nHit)
{
	if (nHit > 0)
		return ptPath->pnTurnBlocked;
	return ptPath->pnTurnBlocked;
}

static __inline BOOL PathWFStep(PTPATHINFO ptPathInfo, PTWFPATH ptPath)
{
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	struct TARGETCOORD tCur;
	BOOL fBlocked;
	int nDir;

	hRoom = ptPathInfo->hRoomStart;
	dwPattern = ptPathInfo->dwCollisionPattern;
	dwMask = ptPathInfo->dwCollisionTestMask;
#line 290
	D2ASSERT((UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_MONSTER));

	ptPath->fLastTry = FALSE;
	tCur.wX = ptPath->tCur.wX;
	tCur.wY = ptPath->tCur.wY;
	fBlocked = PathWFTestNext(ptPath, tCur, hRoom, dwPattern, dwMask);
	if (!fBlocked)
		return TRUE;
	ptPath->nDir = PathWFBlockedTurns(ptPath, fBlocked)[ptPath->nDir];
	fBlocked = PathWFTestNext(ptPath, tCur, hRoom, dwPattern, dwMask);
	if (!fBlocked)
		return TRUE;
	nDir = ptPath->nDir;
	ptPath->nDir = PathWFBlockedTurns(ptPath, fBlocked)[nDir];
	fBlocked = PathWFTestNext(ptPath, tCur, hRoom, dwPattern, dwMask);
	if (!fBlocked)
		return TRUE;
	nDir = ptPath->nDir;
	ptPath->nDir = PathWFBlockedTurns(ptPath, fBlocked)[nDir];
	ptPath->fLastTry = TRUE;
	if (!PathWFTestNext(ptPath, tCur, hRoom, dwPattern, dwMask))
		return TRUE;
	return FALSE;
}

static __inline void PathWFSplice(DWORD *padwLine, int *pnIndex, int nAt, int *pnCount, PTWFPATH ptPath)
{
	int nShift;
	int nLast;

	nShift = *pnIndex - nAt + ptPath->nLoopLen - 1;
	nLast = nAt + nShift;
	memmove(&padwLine[nLast + 1], &padwLine[nAt + 1], (*pnCount - (nAt + 1)) * sizeof(DWORD));
	memcpy(&padwLine[*pnIndex], ptPath->atSteps, ptPath->nLoopLen * sizeof(DWORD));
	*pnCount += nShift;
	*pnIndex = nLast + 1;
}

static __inline int PathWFDistanceSq(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	int nDX, nDY;

	nDX = (int)tFrom.wX - (int)tTo.wX;
	nDY = (int)tFrom.wY - (int)tTo.wY;
	return nDY * nDY + nDX * nDX;
}

int __fastcall PathWFAroundObstacle(PTPATHINFO ptPathInfo, struct TARGETCOORD tFrom, DWORD *padwLine,
	int *pnIndex, int *pnCount, int nMaxLength, int nLineDir)
{
	PTWFPATH ptOther;
	PTWFPATH ptCurrPath;
	WFPATH tLeft = { 0, 0, { 0, 0 }, { 0, 0 }, FALSE, sgcanWFTurnA1, sgcanWFTurnB1, 0, FALSE, { 0 } };
	WFPATH tRight = { 0, 0, { 0, 0 }, { 0, 0 }, FALSE, sgcanWFTurnA2, sgcanWFTurnB2, 0, FALSE, { 0 } };
	PTWFPATH ptSwap;
	struct TARGETCOORD tBlocked;
	int nDir;
	int nAhead;
	int nStart;
	int i;
	int nDistLeft, nDistRight, nDistStart;

	ptOther = &tRight;
	ptCurrPath = &tLeft;
	tBlocked = *(struct TARGETCOORD *)&padwLine[*pnIndex];
	nDir = sgcanWFDeltaDirection[((int)tBlocked.wY - (int)tFrom.wY) * 3 - (int)tFrom.wX + (int)tBlocked.wX +
		4];
	tLeft.nDir = nDir + 1;
	tRight.nDir = nDir + 7;
	tRight.tCur = tFrom;
	tLeft.tCur = tFrom;

	do
	{
		if (!ptCurrPath->fDone)
		{
			if (PathWFStep(ptPathInfo, ptCurrPath))
			{
				if (ptCurrPath->nLoopLen > 0)
				{
					nAhead = 0;
					switch (nLineDir)
					{
					case 0:
						nAhead = (int)tFrom.wY - (int)ptCurrPath->tNext.wY;
						break;
					case 1:
						nAhead = (int)ptCurrPath->tNext.wX - (int)tFrom.wX;
						break;
					case 2:
						nAhead = (int)ptCurrPath->tNext.wY - (int)tFrom.wY;
						break;
					case 3:
						nAhead = (int)tFrom.wX - (int)ptCurrPath->tNext.wX;
						break;
					}
					if (nAhead > 0)
					{
						nAhead += *pnIndex - 1;
						if (nAhead < *pnCount && ptCurrPath->tNext.wX == ((struct TARGETCOORD *)padwLine)[nAhead].wX &&
							ptCurrPath->tNext.wY == ((struct TARGETCOORD *)padwLine)[nAhead].wY)
						{
#line 443
							D2ASSERT(ptCurrPath->nLoopLen < nMaxLength);
							ptCurrPath->atSteps[ptCurrPath->nLoopLen] = ptCurrPath->tNext;
							ptCurrPath->nLoopLen++;
							ptCurrPath->atSteps[ptCurrPath->nLoopLen].wX = 0;
							PathWFSplice(padwLine, pnIndex, nAhead, pnCount, ptCurrPath);
							return ptCurrPath->nLoopLen;
						}
					}
				}

				if (ptOther->nLoopLen > 1)
				{
					if (ptCurrPath->nMeet != 0)
					{
						if (ptCurrPath->tNext.wX == ptOther->atSteps[ptCurrPath->nMeet - 2].wX &&
							ptCurrPath->tNext.wY == ptOther->atSteps[ptCurrPath->nMeet - 2].wY)
						{
							((struct TARGETCOORD *)padwLine)[*pnIndex].wX = 0;
							*pnCount = *pnIndex;
							return 0;
						}
						ptCurrPath->nMeet = 0;
					}
					if (ptCurrPath->tNext.wX == ptOther->tCur.wX && ptCurrPath->tNext.wY == ptOther->tCur.wY)
						ptCurrPath->nMeet = ptOther->nLoopLen;
				}
				ptCurrPath->atSteps[ptCurrPath->nLoopLen] = ptCurrPath->tNext;
				ptCurrPath->nLoopLen++;
				ptCurrPath->tCur = ptCurrPath->tNext;
				ptCurrPath->nDir = ptCurrPath->pnTurnFree[ptCurrPath->nDir];
				if (ptCurrPath->nLoopLen >= nMaxLength - *pnCount - 1)
					ptCurrPath->fDone = TRUE;
			}
			else
				ptCurrPath->fDone = TRUE;
		}
		if (ptCurrPath->nMeet == 0)
		{
			ptSwap = ptCurrPath;
			ptCurrPath = ptOther;
			ptOther = ptSwap;
		}
	} while (!(tLeft.fDone || tRight.fDone));

	if (nMaxLength > 80)
	{
		((struct TARGETCOORD *)padwLine)[*pnIndex].wX = 0;
		*pnCount = *pnIndex;
		return 0;
	}
	nDistLeft = PathWFDistanceSq(tLeft.atSteps[tLeft.nLoopLen - 1], ptPathInfo->tTargetCoord);
	nDistRight = PathWFDistanceSq(tRight.atSteps[tRight.nLoopLen - 1], ptPathInfo->tTargetCoord);
	nDistStart = PathWFDistanceSq(ptPathInfo->tStartCoord, ptPathInfo->tTargetCoord);
	if (nDistRight <= nDistLeft)
	{
		if (nDistStart < nDistRight)
			return 0;
		nStart = *pnIndex;
		if (tRight.nLoopLen > 0)
		{
			for (i = 0; i < tRight.nLoopLen; i++)
				((struct TARGETCOORD *)padwLine)[nStart + i] = tRight.atSteps[i];
		}
		*pnIndex += tRight.nLoopLen;
	}
	else
	{
		if (nDistStart < nDistLeft)
			return 0;
		nStart = *pnIndex;
		if (tLeft.nLoopLen > 0)
		{
			for (i = 0; i < tLeft.nLoopLen; i++)
				((struct TARGETCOORD *)padwLine)[nStart + i] = tLeft.atSteps[i];
		}
		*pnIndex += tLeft.nLoopLen;
	}
	*pnCount = *pnIndex;
	((struct TARGETCOORD *)padwLine)[*pnIndex].wX = 0;
	return ptCurrPath->nLoopLen;
}

int __fastcall PathWFTurningSquares(struct TARGETCOORD tStart, DWORD *padwSteps, DWORD *padwLine,
	int nCount)
{
	struct TARGETCOORD *ptLine;
	int nPrevDX, nPrevDY;
	int nDX, nDY;
	int nRun;
	int nOut;
	int i;

	if (nCount < 2)
	{
		if (nCount == 1)
		{
			*padwSteps = *padwLine;
			return 1;
		}
		return 0;
	}
	ptLine = (struct TARGETCOORD *)padwLine;
	i = 0;
	nOut = 0;
	nPrevDX = (int)ptLine[0].wX - (int)tStart.wX;
	nPrevDY = (int)ptLine[0].wY - (int)tStart.wY;
	nRun = 0;
	for (; i < nCount - 1; i++)
	{
		nDX = (int)ptLine[i + 1].wX - (int)ptLine[i].wX;
		nDY = (int)ptLine[i + 1].wY - (int)ptLine[i].wY;
		if (nDX == nPrevDX && nDY == nPrevDY)
			nRun++;
		else if (nRun > 0)
		{
			padwSteps[nOut++] = padwLine[i];
			nRun = 0;
		}
		else if (nPrevDX == nDX || nPrevDY == nDY)
		{
			padwSteps[nOut++] = padwLine[i];
			nRun = 0;
		}
		else
		{
			nRun = 1;
			nDX = -2;
		}
		nPrevDY = nDY;
		nPrevDX = nDX;
	}
	if (i < nCount)
		padwSteps[nOut++] = padwLine[i];
	return nOut;
}

static int PathWFBuildLine(struct TARGETCOORD tFrom, struct TARGETCOORD tTo, int nReach, int *pnLineDir,
	struct TARGETCOORD *ptLine)
{
	int nMax;
	int nDX, nDY;
	int nStepX, nStepY;
	WORD wX, wY;
	int nCount;
	int nError;

	nMax = nReach - 1;
	*pnLineDir = 0;
	nDX = (int)tTo.wX - (int)tFrom.wX;
	nDY = (int)tTo.wY - (int)tFrom.wY;
	nCount = 0;
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
	if (nDX == 0)
	{
		if (0 < nDY)
		{
			wY = tFrom.wY;
			*pnLineDir = nStepY > 0 ? 2 : 0;
			if (nDY > nMax)
			{
				ptLine[-1].wX = 0;
				return 0;
			}
			for (nCount = 0; nCount < nDY; nCount++)
			{
				wY += (WORD)nStepY;
				ptLine[nCount].wX = tFrom.wX;
				ptLine[nCount].wY = wY;
			}
		}
	}
	else
	{
		wX = tFrom.wX;
		wY = tFrom.wY;
		if (nDY == 0)
		{
			*pnLineDir = nStepX > 0 ? 1 : 3;
			if (nDX > nMax)
			{
				ptLine[-1].wX = 0;
				return 0;
			}
			for (nCount = 0; nCount != nDX; nCount++)
			{
				wX += (WORD)nStepX;
				ptLine[nCount].wX = wX;
				ptLine[nCount].wY = tFrom.wY;
			}
		}
		else if (nDX >= nDY)
		{
			*pnLineDir = nStepX > 0 ? 1 : 3;
			if (nDX > nMax)
			{
				ptLine[-1].wX = 0;
				return 0;
			}
			nError = 0;
			for (nCount = 0; nCount != nDX; nCount++)
			{
				wX += (WORD)nStepX;
				nError += nDY;
				if (nError >= nDX)
				{
					nError -= nDX;
					wY += (WORD)nStepY;
				}
				ptLine[nCount].wX = wX;
				ptLine[nCount].wY = wY;
			}
		}
		else
		{
			*pnLineDir = nStepY > 0 ? 2 : 0;
			if (nDY > nMax)
			{
				ptLine[-1].wX = 0;
				return 0;
			}
			nError = 0;
			for (nCount = 0; nCount != nDY; nCount++)
			{
				wY += (WORD)nStepY;
				nError += nDX;
				if (nError >= nDY)
				{
					nError -= nDY;
					wX += (WORD)nStepX;
				}
				ptLine[nCount].wX = wX;
				ptLine[nCount].wY = wY;
			}
		}
	}
	ptLine[nCount].wX = 0;
	return nCount;
}

int __fastcall PathWFFindLineDetour(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	DWORD adwLine[100];
	struct TARGETCOORD tStart;
	struct TARGETCOORD tFrom;
	struct TARGETCOORD tTarget;
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	int nReach;
	int nLineDir;
	int nCount;
	int i;

	ptPath = ptPathInfo->ptPath;
#line 685
	D2ASSERT((UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_MONSTER));

	hRoom = ptPathInfo->hRoomStart;
	dwPattern = ptPathInfo->dwCollisionPattern;
	dwMask = ptPathInfo->dwCollisionTestMask;
	i = 0;
	tTarget = ptPathInfo->tTargetCoord;
	tStart = ptPathInfo->tStartCoord;
	tFrom = tStart;
	nReach = ptPath->bDistMax;
	if (ptPath->hTarget != NULL && ptPath->bDistMax < 40)
		nReach = 40;
	nCount = PathWFBuildLine(tStart, tTarget, nReach, &nLineDir, (struct TARGETCOORD *)adwLine);
	if (nCount <= 2)
		return 0;
	for (; i < nCount; i++)
	{
		if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, ((struct TARGETCOORD *)adwLine)[i].wX,
			((struct TARGETCOORD *)adwLine)[i].wY, dwPattern, dwMask) &&
			!PathWFAroundObstacle(ptPathInfo, tFrom, adwLine, &i, &nCount, nReach - i, nLineDir))
			return PathWFTurningSquares(tStart, &ptPath->adwSteps[1], adwLine, i);
		tFrom = *(struct TARGETCOORD *)&adwLine[i];
	}
	if (nCount == 0)
		return 0;
	return PathWFTurningSquares(tStart, &ptPath->adwSteps[1], adwLine, nCount);
}
