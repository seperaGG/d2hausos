#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\PATH\\PathMisc.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "PathLocal.h"
#include "FogImports.h"

#define PATH_DIR_NULL	255

DWORD __fastcall SeedNext(D2SEED *ptSeed);
int __fastcall PathFindAStar(PTPATHINFO ptPathInfo);
int __stdcall PathUtilGetDistance(struct TARGETCOORD tFrom, struct TARGETCOORD tTo);

static BOOL __fastcall PathMiscTestDirections(PTPATHINFO ptPathInfo, struct TARGETCOORD tFrom, int *pnTestDir,
	int *pnDir);
static BOOL __fastcall PathMiscMoveToward(PTPATH ptPath, struct TARGETCOORD *ptGameCoord);
static BOOL __fastcall PathMiscLine(PTPATH ptPath, struct TARGETCOORD tStart, struct TARGETCOORD *ptEnd);

struct PATHDIROFFSET
{
	int			nX;
	int			nY;
};
static const struct PATHDIROFFSET sgcanPathMiscDirOffsets[8] =
{
	{ 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 },
};

struct STEPVELOCITY
{
 	int			nXV;
 	int			nYV;
};

static __inline int ABS(int nValue)	{ return nValue < 0 ? -nValue : nValue; }
static __inline int MAX(int nA, int nB)	{ return nA > nB ? nA : nB; }
#define GAME_FIXED_SHIFT	16

void __fastcall StepCalcSquareVector(struct TARGETCOORD tFrom, struct TARGETCOORD tTo, int *pnVector,
	int *pnDir);
void __fastcall PathUtilClampStep(int *pnX, int *pnY);

int __fastcall PathMiscWalkTurning(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	struct TARGETCOORD tCur;
	struct TARGETCOORD tTarget;
	struct TARGETCOORD tPrev;
	DWORD *pdwStep;
	int anDir[3];
	int nDir;
	int nPrevDir;
	int nTurn;
	int nMax;
	int nSteps;
	int i;
	BOOL fStored;

	ptPath = ptPathInfo->ptPath;
	nSteps = ptPath->dwNumSteps;
	nTurn = ptPath->adwSteps[0];
	tCur = ptPathInfo->tStartCoord;
	nPrevDir = PATH_DIR_NULL;
	nMax = ptPathInfo->dwDistMax;
	fStored = FALSE;
	tTarget = ptPathInfo->tTargetCoord;
	i = 0;
	if (nMax > 0)
	{
		pdwStep = &ptPath->adwSteps[nSteps + 1];
		do
		{
			if (tCur.wX == tTarget.wX && tCur.wY == tTarget.wY)
				break;
			fStored = FALSE;
			PathUtilGetSquareDirections(tCur, tTarget, anDir);
			anDir[0] = (anDir[0] + nTurn) & 7;
			anDir[1] = (anDir[1] + nTurn) & 7;
			anDir[2] = (anDir[2] + nTurn) & 7;
			if (!PathMiscTestDirections(ptPathInfo, tCur, anDir, &nDir) || ((nDir - 4) & 7) == nPrevDir)
				break;
			tPrev = tCur;
			tCur.wX += (WORD)sgcanPathMiscDirOffsets[nDir].nX;
			tCur.wY += (WORD)sgcanPathMiscDirOffsets[nDir].nY;
			if (nDir != nPrevDir)
			{
				nSteps++;
				*pdwStep++ = *(DWORD *)&tPrev;
				fStored = TRUE;
			}
			nPrevDir = nDir;
			i++;
		} while (i < nMax);
		if (!fStored && i != 0)
		{
			ptPath->adwSteps[nSteps + 1] = *(DWORD *)&tCur;
			nSteps++;
		}
	}
	ptPath->dwNumSteps = nSteps;
	return nSteps;
}

static BOOL __fastcall PathMiscTestDirections(PTPATHINFO ptPathInfo, struct TARGETCOORD tFrom, int *pnTestDir,
	int *pnDir)
{
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	int nX, nY;

#line 52
	D2ASSERT((UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPathInfo->ptPath->hUnit) == UNIT_MONSTER));
	hRoom = ptPathInfo->hRoomStart;
	dwPattern = ptPathInfo->dwCollisionPattern;
	dwMask = ptPathInfo->dwCollisionTestMask;
	nX = (WORD)(tFrom.wX + sgcanPathMiscDirOffsets[pnTestDir[0]].nX);
	nY = (WORD)(tFrom.wY + sgcanPathMiscDirOffsets[pnTestDir[0]].nY);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX, nY, dwPattern, dwMask))
	{
		*pnDir = pnTestDir[0];
		return TRUE;
	}
	nX = (WORD)(tFrom.wX + sgcanPathMiscDirOffsets[pnTestDir[1]].nX);
	nY = (WORD)(tFrom.wY + sgcanPathMiscDirOffsets[pnTestDir[1]].nY);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX, nY, dwPattern, dwMask))
	{
		*pnDir = pnTestDir[1];
		return TRUE;
	}
#line 76
	D2ASSERT(pnTestDir[2] != PATH_DIR_NULL);
	nX = (WORD)(tFrom.wX + sgcanPathMiscDirOffsets[pnTestDir[2]].nX);
	nY = (WORD)(tFrom.wY + sgcanPathMiscDirOffsets[pnTestDir[2]].nY);
	if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, nX, nY, dwPattern, dwMask))
	{
		*pnDir = pnTestDir[2];
		return TRUE;
	}
	return FALSE;
}

static __inline BOOL PathMiscVelocityLine(PTPATH ptPath, struct TARGETCOORD tStart, struct TARGETCOORD tEnd,
	struct STEPVELOCITY *ptVelocity)
{
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	int nX, nY;
	int nDX, nDY;
	int nNextX, nNextY;
	int nSmallXVel, nSmallYVel;
	struct TARGETCOORD tPrev;
	struct TARGETCOORD tTest;

	hRoom = (GAMEROOM *)ptPath->dwRoom;
	dwPattern = ptPath->dwCollisionPattern;
	dwMask = ptPath->dwCollisionTestMask;
	tPrev = tStart;
	nX = ((int)tStart.wX << 16) + 32768;
	nY = ((int)tStart.wY << 16) + 32768;
	nDX = (int)tEnd.wX - (int)(WORD)(nX >> 16);
	nDY = (int)tEnd.wY - (int)(WORD)(nY >> 16);
	nSmallXVel = ptVelocity->nXV;
	nSmallYVel = ptVelocity->nYV;
#line 110
	D2ASSERT(ABS(nSmallXVel) > 0 || ABS(nSmallYVel) > 0);
	PathUtilClampStep(&nSmallXVel, &nSmallYVel);
	if (ABS(nDX) > ABS(nDY))
	{
		if (tStart.wX != tEnd.wX)
		{
			do
			{
				nNextX = nX + nSmallXVel;
				nNextY = nY + nSmallYVel;
				tTest.wX = (WORD)(nNextX >> 16);
				tTest.wY = (WORD)(nNextY >> 16);
				if (tPrev.wX != tTest.wX || tPrev.wY != tTest.wY)
				{
					if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tTest.wX, tTest.wY, dwPattern, dwMask))
						return FALSE;
				}
				tPrev = tTest;
				nX = nNextX;
				nY = nNextY;
			} while (tTest.wX != tEnd.wX);
		}
	}
	else
	{
		if (tStart.wY != tEnd.wY)
		{
			do
			{
				nNextX = nX + nSmallXVel;
				nNextY = nY + nSmallYVel;
				tTest.wX = (WORD)(nNextX >> 16);
				tTest.wY = (WORD)(nNextY >> 16);
				if (tPrev.wX != tTest.wX || tPrev.wY != tTest.wY)
				{
					if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tTest.wX, tTest.wY, dwPattern, dwMask))
						return FALSE;
				}
				tPrev = tTest;
				nX = nNextX;
				nY = nNextY;
			} while (tTest.wY != tEnd.wY);
		}
	}
#line 171
	D2ASSERT(tEnd.wX != 0);
#line 172
	D2ASSERT(tEnd.wY != 0);
	return TRUE;
}

int __fastcall PathMiscStraight(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	struct TARGETCOORD tCur;
	struct TARGETCOORD tPrev;
	struct TARGETCOORD tEnd;
	DWORD *pdwStep;
	int anDir[3];
	int nDir;
	int nPrevDir;
	int nMax;
	int nSteps;
	int i;
	BOOL fTurned;

	ptPath = ptPathInfo->ptPath;
	ptPath->dwStepIndex = 0;
	ptPath->dwNumSteps = 0;
	if (ptPath->adwSteps[0] != 0)
		return PathMiscWalkTurning(ptPathInfo);

	{
		tEnd = ptPathInfo->tTargetCoord;
		ptPath->adwSteps[1] = *(DWORD *)&tEnd;
		if (PathMiscMoveToward(ptPath, &tEnd))
		{
			ptPath->adwSteps[ptPath->dwNumSteps + 1] = *(DWORD *)&tEnd;
			ptPath->dwNumSteps++;
			return ptPath->dwNumSteps;
		}
		if (PathUtilGetDistance(tEnd, ptPathInfo->tTargetCoord) <= (int)ptPathInfo->bTargetRange)
		{
			ptPath->adwSteps[ptPath->dwNumSteps + 1] = *(DWORD *)&tEnd;
			ptPath->dwNumSteps++;
			return ptPath->dwNumSteps;
		}

		tCur = ptPathInfo->tStartCoord;
		if (tCur.wX != tEnd.wX || tCur.wY != tEnd.wY)
		{
			ptPath->adwSteps[ptPath->dwNumSteps + 1] = *(DWORD *)&tEnd;
			ptPath->dwNumSteps++;
			tCur = tEnd;
		}
	}
	nSteps = ptPath->dwNumSteps;
	nPrevDir = PATH_DIR_NULL;
	nMax = ptPathInfo->dwDistMax;
	fTurned = FALSE;
	tEnd = ptPathInfo->tTargetCoord;
	i = 0;
	if (nMax > 0)
	{
		pdwStep = &ptPath->adwSteps[nSteps + 1];
		do
		{
			if (tCur.wX == tEnd.wX && tCur.wY == tEnd.wY)
				break;
			fTurned = FALSE;
			PathUtilGetSquareDirections(tCur, tEnd, anDir);
			if (!PathMiscTestDirections(ptPathInfo, tCur, anDir, &nDir) || ((nDir - 4) & 7) == nPrevDir)
				break;
			tPrev = tCur;
			tCur.wX += (WORD)sgcanPathMiscDirOffsets[nDir].nX;
			tCur.wY += (WORD)sgcanPathMiscDirOffsets[nDir].nY;
			if (nDir != nPrevDir)
			{
				if (tPrev.wX != ptPathInfo->tStartCoord.wX || tPrev.wY != ptPathInfo->tStartCoord.wY)
				{
					nSteps++;
					*pdwStep++ = *(DWORD *)&tPrev;
				}
				fTurned = TRUE;
			}
			nPrevDir = nDir;
			i++;
		} while (i < nMax);
		if (!fTurned && i != 0)
		{
			ptPath->adwSteps[nSteps + 1] = *(DWORD *)&tCur;
			nSteps++;
		}
	}
	ptPath->dwNumSteps = nSteps;
	return nSteps;
}

static BOOL __fastcall PathMiscMoveToward(PTPATH ptPath, struct TARGETCOORD *ptGameCoord)
{
	struct TARGETCOORD tStart;

#line 324
	D2ASSERT((UnitGetType(ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPath->hUnit) == UNIT_MONSTER));
#line 325
	D2ASSERT(ptPath->eType != PATH_TYPE_MISSILE);
#line 326
	D2ASSERT(ptGameCoord->wX != 0);
#line 327
	D2ASSERT(ptGameCoord->wY != 0);
	if (!ptPath->dwVelocity)
	{
		ptGameCoord->wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
		ptGameCoord->wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
		return FALSE;
	}
	StepAimAtStep(ptPath, TRUE, TRUE);
	if (!ptPath->dwVelocity)
	{
		ptGameCoord->wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
		ptGameCoord->wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
		return FALSE;
	}
	tStart.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
	tStart.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
	return PathMiscLine(ptPath, tStart, ptGameCoord);
}

static BOOL __fastcall PathMiscLine(PTPATH ptPath, struct TARGETCOORD tStart, struct TARGETCOORD *ptEnd)
{
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	struct TARGETCOORD tEnd;
	struct TARGETCOORD tCur;
	struct TARGETCOORD tFree;
	int nDX, nDY;
	int nLenX, nLenY;
	int nStepX, nStepY;
	int nError;

	hRoom = (GAMEROOM *)ptPath->dwRoom;
	dwPattern = ptPath->dwCollisionPattern;
	dwMask = ptPath->dwCollisionTestMask;
	tEnd = *ptEnd;
	nDX = (int)tEnd.wX - (int)tStart.wX;
	nDY = (int)tEnd.wY - (int)tStart.wY;
	tFree = tStart;
	tCur = tStart;
	nLenX = (nDX < 0 ? -nDX : nDX) + 1;
	nLenY = (nDY < 0 ? -nDY : nDY) + 1;
	if (nLenX > nLenY)
	{
		if (nDX == 0)
		{
			nStepY = nDY >= 0 ? 1 : -1;
			while (tCur.wY != tEnd.wY)
			{
				tCur.wY += (WORD)nStepY;
				if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tStart.wX, tCur.wY, dwPattern, dwMask))
				{
					*ptEnd = tFree;
					return FALSE;
				}
				tFree = tCur;
			}
			return TRUE;
		}
		nStepX = nDX >= 0 ? 1 : -1;
		nStepY = nDY >= 0 ? 1 : -1;
		nError = nLenY;
		while (tCur.wX != tEnd.wX)
		{
			tCur.wX += (WORD)nStepX;
			if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tCur.wX, tCur.wY, dwPattern, dwMask))
			{
				*ptEnd = tFree;
				return FALSE;
			}
			nError += nLenY;
			if (nError >= nLenX)
			{
				tCur.wY += (WORD)nStepY;
				nError -= nLenX;
				if (nError > 0 && D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tCur.wX, tCur.wY, dwPattern,
					dwMask))
				{
					*ptEnd = tFree;
					return FALSE;
				}
			}
			tFree = tCur;
		}
		return TRUE;
	}
	if (nLenX < nLenY)
	{
		if (nDY == 0)
		{
			nStepX = nDX >= 0 ? 1 : -1;
			while (tCur.wX != tEnd.wX)
			{
				tCur.wX += (WORD)nStepX;
				if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tCur.wX, tStart.wY, dwPattern, dwMask))
				{
					*ptEnd = tFree;
					return FALSE;
				}
				tFree = tCur;
			}
			return TRUE;
		}
		nStepX = nDX >= 0 ? 1 : -1;
		nStepY = nDY >= 0 ? 1 : -1;
		nError = nLenX;
		while (tCur.wY != tEnd.wY)
		{
			tCur.wY += (WORD)nStepY;
			if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tCur.wX, tCur.wY, dwPattern, dwMask))
			{
				*ptEnd = tFree;
				return FALSE;
			}
			nError += nLenX;
			if (nError >= nLenY)
			{
				tCur.wX += (WORD)nStepX;
				nError -= nLenY;
				if (nError > 0 && D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tCur.wX, tCur.wY, dwPattern,
					dwMask))
				{
					*ptEnd = tFree;
					return FALSE;
				}
			}
			tFree = tCur;
		}
		return TRUE;
	}
	nStepX = nDX >= 0 ? 1 : -1;
	nStepY = nDY >= 0 ? 1 : -1;
	while (tCur.wX != tEnd.wX)
	{
		tCur.wX += (WORD)nStepX;
		tCur.wY += (WORD)nStepY;
		if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tCur.wX, tCur.wY, dwPattern, dwMask))
		{
			*ptEnd = tFree;
			return FALSE;
		}
		tFree = tCur;
	}
	return TRUE;
}

int __fastcall PathMiscFindStraightFromStart(PTPATHINFO ptPathInfo)
{
	ptPathInfo->ptPath->dwStepIndex = 0;
	return PathMiscStraight(ptPathInfo);
}

int __fastcall PathMiscFindWalkTurningRetry(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	int nFirst;
	int nSteps;

	ptPath = ptPathInfo->ptPath;
	ptPath->dwStepIndex = 0;
	ptPath->adwSteps[0] = (DWORD)-4;
	nFirst = PathMiscWalkTurning(ptPathInfo);
	if (nFirst == 0)
		return 0;
	ptPath->dwNumSteps = nFirst;
	ptPath->adwSteps[0] = (DWORD)-2;
	nSteps = PathMiscWalkTurning(ptPathInfo) - nFirst;
	if (nSteps == 0)
	{
		ptPath->adwSteps[0] = 2;
		nSteps = PathMiscWalkTurning(ptPathInfo) - nFirst;
	}
	ptPath->adwSteps[0] = (DWORD)-4;
	return nSteps + nFirst;
}

int __fastcall PathMiscFindStraightOrSearch(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	int nSteps;
	int nFound;
	int nDX, nDY, nT;
	struct TARGETCOORD tLast;

	ptPath = ptPathInfo->ptPath;
	ptPath->dwStepIndex = 0;
	ptPath->dwNumSteps = 0;
	nSteps = PathMiscStraight(ptPathInfo);
	if (nSteps != 0)
	{
		tLast = *(struct TARGETCOORD *)&ptPath->adwSteps[nSteps];
		if (PathUtilGetDistance(tLast, ptPathInfo->tTargetCoord) <= (int)ptPathInfo->bTargetRange)
			return nSteps;
	}
	nT = (int)ptPathInfo->tStartCoord.wX - (int)ptPathInfo->tTargetCoord.wX;
	nDX = nT < 0 ? -nT : nT;
	nT = (int)ptPathInfo->tStartCoord.wY - (int)ptPathInfo->tTargetCoord.wY;
	nDY = nT < 0 ? -nT : nT;
	if (nDY * nDY + nDX * nDX > 324)
		return nSteps;
	nFound = PathFindAStar(ptPathInfo);
	if (nFound)
		return nFound;
	if (nSteps)
		nSteps = PathMiscStraight(ptPathInfo);
	return nSteps;
}

int __fastcall PathMiscFindCutBackStep(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	struct TARGETCOORD tEnd;

	ptPath = ptPathInfo->ptPath;
	ptPath->dwStepIndex = 0;
	tEnd = ptPathInfo->tTargetCoord;
	ptPath->adwSteps[1] = *(DWORD *)&tEnd;
	PathMiscMoveToward(ptPath, &tEnd);
	if (ptPathInfo->tStartCoord.wX == tEnd.wX && ptPathInfo->tStartCoord.wY == tEnd.wY)
		return 0;
	ptPath->adwSteps[1] = *(DWORD *)&tEnd;
	ptPath->dwNumSteps = 1;
	return 1;
}

int __fastcall PathMiscOnKnockbackClient(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;

	ptPath = ptPathInfo->ptPath;
	ptPath->dwStepIndex = 0;
	ptPath->adwSteps[1] = *(DWORD *)&ptPathInfo->tTargetCoord;
	PathMiscMoveToward(ptPath, (struct TARGETCOORD *)&ptPath->adwSteps[1]);
	return 1;
}

int __fastcall PathMiscOnKnockbackServer(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	struct TARGETCOORD tEnd;
	int nDistance;
	int nDX = 0, nDY = 0;
	int nMax;
	int nAbsX;

	ptPath = ptPathInfo->ptPath;
	nDistance = (ptPath->bDistLeft >> 1) + 1;
	if (ptPath->hTarget != NULL)
	{
		tEnd = ptPathInfo->tStartCoord;
		nDX = D2CommonUnitGetX(ptPath->hTarget);
		nDY = D2CommonUnitGetY(ptPath->hTarget);
		nDX = (int)ptPathInfo->tStartCoord.wX - nDX;
		nDY = (int)ptPathInfo->tStartCoord.wY - nDY;
		nMax = nDY < 0 ? -nDY : nDY;
		nAbsX = nDX < 0 ? -nDX : nDX;
		if (nAbsX > nMax)
			nMax = nAbsX;
		if (nMax != 0)
		{
			nDX = nDX * nDistance / nMax;
			nDY = nDY * nDistance / nMax;
		}
		tEnd.wX += (WORD)nDX;
		tEnd.wY += (WORD)nDY;
	}
	else
		tEnd = ptPathInfo->tTargetCoord;
	ptPath->adwSteps[1] = *(DWORD *)&tEnd;
	PathMiscLine(ptPath, ptPathInfo->tStartCoord, &tEnd);
	D2CommonPathSetTargetCoord(ptPath, tEnd.wX, tEnd.wY);
	if (tEnd.wX == ptPathInfo->tStartCoord.wX && tEnd.wY == ptPathInfo->tStartCoord.wY)
		return 0;
	ptPath->adwSteps[1] = *(DWORD *)&tEnd;
	ptPath->dwNumSteps = 1;
	return 1;
}

typedef float (__stdcall *PFNFOGANGLE)(DWORD nAngle);
#define FogCosine	((PFNFOGANGLE)FogD2QSSocketCosineRaw)
#define FogSine		((PFNFOGANGLE)FogD2QSSocketSineRaw)

void __fastcall PathMiscMissileE(PTPATH ptPath, D2SEED *ptSeed)
{
	struct GAMECOORD tBase;
	struct GAMECOORD tPrev;
	DWORD dwX, dwY;
	int nAngle;
	int nRadius;
	int nCount;
	float fRadius;

	tBase = ptPath->tGameCoord;
	tPrev = tBase;
	nAngle = 0;
	nRadius = 0;
	nCount = 0;
	do
	{
		nRadius += 9600;
		nAngle += 16;
		fRadius = (float)nRadius;
		dwX = tBase.dwX + (int)((double)FogCosine(nAngle) * fRadius);
		dwY = tBase.dwY + (int)((double)FogSine(nAngle) * fRadius);
		if (HIWORD(dwX) != HIWORD(tPrev.dwX) || HIWORD(dwY) != HIWORD(tPrev.dwY))
		{
			tPrev.dwX = dwX;
			tPrev.dwY = dwY;
			((struct TARGETCOORD *)&ptPath->adwSteps[nCount + 1])->wX = (WORD)(dwX >> 16);
			((struct TARGETCOORD *)&ptPath->adwSteps[nCount + 1])->wY = (WORD)(dwY >> 16);
			nCount++;
		}
	} while (nCount < 80);
	ptPath->dwNumSteps = nCount;
}

void __fastcall PathMiscMissileA(PTPATH ptPath, D2SEED *ptSeed)
{
	int anTurn[32] =
	{
		-1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1, -1, 0, 1,
		-1, 1,
	};
	int anDir[3];
	struct TARGETCOORD tCur;
	int nMoves;
	int nSteps;
	int nTurn;
	int nDir;

	nMoves = ptPath->bDistMax >> 1;
	tCur.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
	tCur.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
	nSteps = 0;
	PathUtilGetSquareDirections(tCur, ptPath->tTargetCoord, anDir);
	for (; nSteps < nMoves; nSteps++)
	{
		nTurn = anTurn[SeedNext(ptSeed) & 31];
		nDir = (nTurn + anDir[0]) & 7;
		ptPath->adwSteps[nSteps + 1] = *(DWORD *)&tCur;
		tCur.wX += (WORD)(sgcanPathMiscDirOffsets[nDir].nX << 1);
		tCur.wY += (WORD)(sgcanPathMiscDirOffsets[nDir].nY << 1);
	}
	ptPath->adwSteps[nSteps + 1] = *(DWORD *)&tCur;
	ptPath->dwNumSteps = nSteps + 1;
}

void __fastcall PathMiscStepVelocity(int nSpeed, struct STEPVELOCITY *ptStep, struct TARGETCOORD tFrom,
	struct TARGETCOORD tTo)
{
	int nDir;

	StepCalcSquareVector(tFrom, tTo, &ptStep->nXV, &nDir);
	ptStep->nXV = ptStep->nXV * nSpeed >> 8;
	ptStep->nYV = nSpeed * ptStep->nYV >> 8;
#line 814
	D2ASSERT(MAX( ABS(ptStep->nXV), ABS(ptStep->nYV) ) < (32 << GAME_FIXED_SHIFT));
}

BOOL __fastcall PathMiscCheckSteps(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	GAMEROOM *hRoom;
	DWORD dwPattern;
	DWORD dwMask;
	struct TARGETCOORD tCur;
	struct TARGETCOORD tNext;
	struct STEPVELOCITY tVelocity;
	DWORD *padwSteps;
	int nCount;
	int i;

	ptPath = ptPathInfo->ptPath;
	padwSteps = &ptPath->adwSteps[1];
	tCur = ptPathInfo->tStartCoord;
	hRoom = (GAMEROOM *)ptPath->dwRoom;
	nCount = (int)ptPath->dwNumSteps;
	dwPattern = ptPath->dwCollisionPattern;
	dwMask = ptPath->dwCollisionTestMask;
	for (i = 0; i < nCount; i++)
	{
		tNext = *(struct TARGETCOORD *)&padwSteps[i];
		if (ABS((int)tNext.wX - (int)tCur.wX) <= 1 && ABS((int)tNext.wY - (int)tCur.wY) <= 1)
		{
			if (D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, tNext.wX, tNext.wY, dwPattern, dwMask))
				return FALSE;
		}
		else
		{
			if (!ptPath->dwVelocity)
				return FALSE;
			PathMiscStepVelocity((int)ptPath->dwVelocity, &tVelocity, tCur, tNext);
#line 831
			D2ASSERT(ABS(tVelocity.nXV) > 0 || ABS(tVelocity.nYV) > 0);
			PathUtilClampStep(&tVelocity.nXV, &tVelocity.nYV);
			if (!PathMiscVelocityLine(ptPath, tCur, tNext, &tVelocity))
				return FALSE;
		}
		tCur = tNext;
	}
	return TRUE;
}
