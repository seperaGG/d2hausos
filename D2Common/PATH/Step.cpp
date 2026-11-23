#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\PATH\\Step.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "FogImports.h"

#define STEP_ARRAY_LENGTH	128
#define PATH_MISSILE_MASK	262144
#define FRACTIONAL_TO_GAMESQUARE(dwFixed)	((WORD)((dwFixed) >> 16))
#define COORD_TEST_EQUAL(tA, tB)	((tA).wX == (tB).wX && (tA).wY == (tB).wY)

static const struct GAMECOORD sgctZeroPathCoord = { 0, 0 };

static const struct TARGETCOORD sgctZeroGameCoord = { 0, 0 };

static const struct GAMECOORD sgctZeroStepVector = { 0, 0 };

static const int sgcanStepDirections[STEP_ARRAY_LENGTH * 3] =
{
	0, 4096, 0,  32, 4095, 0,  64, 4095, 0,  96, 4094, 0,  128, 4093, 0,  161, 4092, 0,
	193, 4091, 0,  225, 4089, 0,  257, 4087, 0,  289, 4085, 0,  321, 4083, 0,  353, 4080, 0,
	385, 4077, 0,  417, 4074, 1,  448, 4071, 1,  480, 4067, 1,  511, 4063, 1,  543, 4059, 1,
	574, 4055, 1,  606, 4050, 1,  637, 4046, 1,  668, 4041, 1,  699, 4035, 1,  729, 4030, 1,
	760, 4024, 1,  791, 4018, 1,  821, 4012, 2,  851, 4006, 2,  881, 3999, 2,  911, 3993, 2,
	941, 3986, 2,  971, 3979, 2,  1000, 3971, 2,  1030, 3964, 2,  1059, 3956, 2,  1088, 3948, 2,
	1117, 3940, 2,  1145, 3932, 2,  1174, 3924, 2,  1202, 3915, 3,  1230, 3906, 3,  1258, 3897, 3,
	1286, 3888, 3,  1313, 3879, 3,  1340, 3870, 3,  1368, 3860, 3,  1394, 3851, 3,  1421, 3841, 3,
	1448, 3831, 3,  1474, 3821, 3,  1500, 3811, 3,  1526, 3800, 3,  1552, 3790, 3,  1577, 3780, 4,
	1602, 3769, 4,  1627, 3758, 4,  1652, 3747, 4,  1677, 3736, 4,  1701, 3725, 4,  1725, 3714, 4,
	1749, 3703, 4,  1773, 3692, 4,  1796, 3680, 4,  1820, 3669, 4,  1843, 3657, 4,  1866, 3646, 4,
	1888, 3634, 4,  1911, 3622, 4,  1933, 3610, 5,  1955, 3599, 5,  1977, 3587, 5,  1998, 3575, 5,
	2020, 3563, 5,  2041, 3551, 5,  2062, 3539, 5,  2082, 3526, 5,  2103, 3514, 5,  2123, 3502, 5,
	2143, 3490, 5,  2163, 3478, 5,  2183, 3465, 5,  2202, 3453, 5,  2221, 3441, 5,  2240, 3428, 5,
	2259, 3416, 5,  2278, 3403, 6,  2296, 3391, 6,  2314, 3379, 6,  2332, 3366, 6,  2350, 3354, 6,
	2368, 3341, 6,  2385, 3329, 6,  2402, 3317, 6,  2419, 3304, 6,  2436, 3292, 6,  2453, 3279, 6,
	2469, 3267, 6,  2486, 3255, 6,  2502, 3242, 6,  2518, 3230, 6,  2533, 3218, 6,  2549, 3205, 6,
	2564, 3193, 6,  2580, 3181, 6,  2595, 3169, 6,  2609, 3156, 7,  2624, 3144, 7,  2639, 3132, 7,
	2653, 3120, 7,  2667, 3108, 7,  2681, 3096, 7,  2695, 3084, 7,  2709, 3072, 7,  2722, 3060, 7,
	2736, 3048, 7,  2749, 3036, 7,  2762, 3024, 7,  2775, 3012, 7,  2788, 3000, 7,  2800, 2988, 7,
	2813, 2977, 7,  2825, 2965, 7,  2837, 2953, 7,  2849, 2942, 7,  2861, 2930, 7,  2873, 2919, 7,
	2884, 2907, 7,  2896, 2896, 7,
};

static void __fastcall StepSetCoord(PTPATH ptPath, struct GAMECOORD tCoord, GAMEROOM *hRoomHint);
static void __fastcall StepSetRoom(PTPATH ptPath, GAMEROOM *hRoomDest);
static void __fastcall StepFindRoom(PTPATH ptPath, GAMEROOM *hRoomHint);
static BOOL __fastcall StepMove(PTPATH ptPath, struct GAMECOORD *ptNext, int *pnSaved);
void __stdcall StepSnapToSquare(PTPATH ptPath);

static __inline DWORD StepSquareCentre(DWORD dwSquare)
{
	return (((dwSquare << 16) + 32768) & 0xFFFF0000) + 32768;
}

static __inline void StepClearNumSteps(PTPATH ptPath)
{
	ptPath->dwNumSteps = 0;
}

static __inline void StepClearSteps(PTPATH ptPath)
{
	ptPath->dwNumSteps = 0;
	ptPath->dwFlags &= ~32;
}

static __inline void StepClearVelocity(PTPATH ptPath)
{
	ptPath->dwVelocityX = sgctZeroStepVector.dwX;
	ptPath->dwVelocityY = sgctZeroStepVector.dwY;
}

static __inline void StepClearMotion(PTPATH ptPath)
{
	ptPath->dwStepIndex = 0;
	ptPath->dwVelocityX = sgctZeroStepVector.dwX;
	ptPath->dwVelocityY = sgctZeroStepVector.dwY;
}

static __inline BOOL StepIsWalking(PTPATH ptPath)
{
	return (ptPath->dwFlags & 32) && ptPath->dwNumSteps > 0 && ptPath->dwVelocity != 0;
}

static __inline BOOL StepIsStill(PTPATH ptPath)
{
	return ptPath->dwVelocityX == 0 && ptPath->dwVelocityY == 0;
}

static __inline void StepFinishMove(PTPATH ptPath)
{
	StepSnapToSquare(ptPath);
	ptPath->dwFlags &= ~32;
	ptPath->dwNumSteps = 0;
	ptPath->dwStepIndex = 0;
	ptPath->dwVelocityX = sgctZeroStepVector.dwX;
	ptPath->dwVelocityY = sgctZeroStepVector.dwY;
}

void __stdcall StepLeaveRoom(PTPATH ptPath)
{
	ptPath->tGameCoord = sgctZeroPathCoord;
	ptPath->tRoomTileCoord.dwX = sgctZeroPathCoord.dwX;
	ptPath->tRoomTileCoord.dwY = sgctZeroPathCoord.dwY;
	ptPath->dwNumSteps = 0;
	if (ptPath->dwRoom != 0)
	{
		ptPath->field_20 = ptPath->dwRoom;
		D2CommonUnitRoomRemoveUnit(ptPath->hUnit);
		ptPath->dwFlags |= 2;
	}
}

void __fastcall StepCalcVector(struct GAMECOORD tFrom, struct GAMECOORD tTo, int *pnVector,
	int *pnDir)
{
	DWORD dwXMin, dwXMax, dwYMin, dwYMax;
	DWORD dwSwap;
	BOOL fXGrows, fYGrows, fXMajor;
	int nIndex;
	int nRow;

	if (tFrom.dwX < tTo.dwX)
	{
		dwXMax = tTo.dwX;
		dwXMin = tFrom.dwX;
		fXGrows = TRUE;
	}
	else
	{
		dwXMax = tFrom.dwX;
		dwXMin = tTo.dwX;
		fXGrows = FALSE;
	}
	if (tFrom.dwY < tTo.dwY)
	{
		dwYMax = tTo.dwY;
		dwYMin = tFrom.dwY;
		fYGrows = TRUE;
	}
	else
	{
		dwYMax = tFrom.dwY;
		dwYMin = tTo.dwY;
		fYGrows = FALSE;
	}

	if ((int)(dwXMax - dwXMin) < (int)(dwYMax - dwYMin))
		fXMajor = FALSE;
	else
	{
		dwSwap = dwXMax;
		dwXMax = dwYMax;
		dwYMax = dwSwap;
		fXMajor = TRUE;
		dwSwap = dwXMin;
		dwXMin = dwYMin;
		dwYMin = dwSwap;
	}

	if ((int)(dwYMax - dwYMin) != 0)
	{
		nIndex = (int)((dwXMax - dwXMin) * 127) / (int)(dwYMax - dwYMin);
#line 341
		D2ASSERT(nIndex < STEP_ARRAY_LENGTH);
	}
	else
		nIndex = 0;

	nRow = nIndex * 3;
	*pnDir = sgcanStepDirections[nRow + 2];
	if (fXMajor)
	{
		pnVector[1] = sgcanStepDirections[nRow];
		pnVector[0] = sgcanStepDirections[nRow + 1];
		*pnDir = (-1 - *pnDir) & 15;
	}
	else
	{
		pnVector[0] = sgcanStepDirections[nRow];
		pnVector[1] = sgcanStepDirections[nRow + 1];
	}
	if (!fYGrows)
	{
		pnVector[1] = -pnVector[1];
		*pnDir = (-1 - *pnDir) & 31;
	}
	if (fXGrows)
		*pnDir = (-1 - *pnDir) & 63;
	else
		pnVector[0] = -pnVector[0];
	*pnDir = (*pnDir + 8) & 63;
}

void __fastcall StepCalcSquareVector(struct TARGETCOORD tFrom, struct TARGETCOORD tTo, int *pnVector,
	int *pnDir)
{
	struct GAMECOORD tFromPos, tToPos;

	tFromPos.dwX = ((DWORD)tFrom.wX << 16) + 32768;
	tFromPos.dwY = ((DWORD)tFrom.wY << 16) + 32768;
	tToPos.dwX = ((DWORD)tTo.wX << 16) + 32768;
	tToPos.dwY = ((DWORD)tTo.wY << 16) + 32768;
	StepCalcVector(tFromPos, tToPos, pnVector, pnDir);
}

int __stdcall StepGetSquareDirection(DWORD nX1, DWORD nY1, DWORD nX2, DWORD nY2)
{
	int anVector[2];
	int nDir;
	struct GAMECOORD tFrom, tTo;

	tFrom.dwX = (nX1 << 16) + 32768;
	tFrom.dwY = (nY1 << 16) + 32768;
	tTo.dwX = (nX2 << 16) + 32768;
	tTo.dwY = (nY2 << 16) + 32768;
	StepCalcVector(tFrom, tTo, anVector, &nDir);
	return nDir;
}

int __stdcall StepGetDirection(struct GAMECOORD tFrom, struct GAMECOORD tTo)
{
	int anVector[2];
	int nDir;

	StepCalcVector(tFrom, tTo, anVector, &nDir);
	return nDir;
}

BOOL __fastcall StepAimAtStep(PTPATH ptPath, BOOL fFaceOnArrive, BOOL fFace)
{
	struct GAMECOORD tStep;
	int anVector[2];
	int nDir;
	BOOL fArrives;

	tStep.dwX = ((DWORD)((struct TARGETCOORD *)&ptPath->adwSteps[ptPath->dwStepIndex + 1])->wX << 16) + 32768;
	tStep.dwY = ((DWORD)((struct TARGETCOORD *)&ptPath->adwSteps[ptPath->dwStepIndex + 1])->wY << 16) + 32768;
	while (tStep.dwX == ptPath->tGameCoord.dwX && tStep.dwY == ptPath->tGameCoord.dwY)
	{
		if ((int)ptPath->dwStepIndex >= (int)ptPath->dwNumSteps - 1)
		{
			ptPath->nStepVectorX = sgctZeroStepVector.dwX;
			ptPath->nStepVectorY = sgctZeroStepVector.dwY;
			ptPath->dwVelocityX = sgctZeroStepVector.dwX;
			ptPath->dwVelocityY = sgctZeroStepVector.dwY;
			ptPath->dwVelocity = 0;
			ptPath->dwStepIndex = ptPath->dwNumSteps;
			return TRUE;
		}
		ptPath->dwStepIndex++;
		tStep.dwX = ((DWORD)((struct TARGETCOORD *)&ptPath->adwSteps[ptPath->dwStepIndex + 1])->wX << 16) + 32768;
		tStep.dwY = ((DWORD)((struct TARGETCOORD *)&ptPath->adwSteps[ptPath->dwStepIndex + 1])->wY << 16) + 32768;
	}

	StepCalcVector(ptPath->tGameCoord, tStep, anVector, &nDir);
	memcpy(&ptPath->nStepVectorX, anVector, sizeof(anVector));
	if (ptPath->dwFlags & 512)
		nDir = (nDir - 32) & 63;
	ptPath->dwVelocityX = (int)ptPath->dwVelocity * anVector[0] >> 8;
	ptPath->dwVelocityY = (int)ptPath->dwVelocity * anVector[1] >> 8;
	if (FRACTIONAL_TO_GAMESQUARE(tStep.dwX) == FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwX + ptPath->dwVelocityX) &&
		FRACTIONAL_TO_GAMESQUARE(tStep.dwY) == FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwY + ptPath->dwVelocityY))
		fArrives = TRUE;
	else
		fArrives = FALSE;
	if (fFace || (fArrives && fFaceOnArrive))
		PathSetFacing(ptPath, (BYTE)nDir);
	return fArrives;
}

PTPATH __fastcall StepUnitHalt(HUNIT hUnit)
{
	PTPATH ptPath;
	struct GAMECOORD tCoord;

	ptPath = hUnit->ptPath;
	if (ptPath == NULL)
		return NULL;
	ptPath->dwFlags &= ~8;
	if (ptPath->dwFlags & PATH_MISSILE_MASK)
		ptPath->wLastCollision = 0;
	if (!StepIsWalking(ptPath))
	{
		tCoord.dwX = (ptPath->tGameCoord.dwX & 0xFFFF0000) + 32768;
		tCoord.dwY = (ptPath->tGameCoord.dwY & 0xFFFF0000) + 32768;
		StepSetCoord(ptPath, tCoord, NULL);
		ptPath->dwFlags &= ~32;
		ptPath->dwNumSteps = 0;
		ptPath->dwStepIndex = 0;
		StepClearVelocity(ptPath);
		return NULL;
	}
	return ptPath;
}

DWORD __stdcall StepUnitRecalcPath(HUNIT ptUnit, DWORD dwHasTarget)
{
	PTPATH ptPath;
	DWORD dwResult;

	ptPath = ptUnit->ptPath;
	if (ptPath == NULL)
		return 0;

	if (!(ptPath->dwFlags & 16))
	{
		if (ptUnit->eType == UNIT_MONSTER)
		{
			if (D2CommonPathGetRecalcBudgetRaw((DWORD)ptPath) == 0)
				return 0;
		}
		D2CommonUnitRoomAddToChangedList(ptUnit);
		ptUnit->dwFlags0 |= 1;
		D2CommonPathAddRecalcBudget(ptPath, -(int)ptPath->dwStepIndex);
	}

	switch (ptPath->eType)
	{
	case 2:
	case 13:
	case 15:
		if (dwHasTarget != 0)
		{
			ptPath->eType = 13;
		}
		else
		{
			ptPath->eType = 2;
			ptPath->tTargetCoord = ptPath->tEndCoord;
		}

		dwResult = D2CommonPathCompute(ptPath, ptUnit);
		if (dwResult != 0)
			return dwResult;
		ptPath->eType = 15;
	}
	return D2CommonPathCompute(ptPath, ptUnit);
}

static __inline BOOL StepSetVelocity(PTPATH ptPath, int nSpeed)
{
	int nScale;
	int nRate;

	if (ptPath->dwStepIndex >= ptPath->dwNumSteps)
		return FALSE;

	nRate = nSpeed;
	if (nRate <= 0)
		nRate = 1024;
	if (ptPath->dwAccel != 0)
	{
		ptPath->nAccelCounter++;
		if (ptPath->nAccelCounter >= 5)
		{
			ptPath->dwVelocity += ptPath->dwAccel;
			if (ptPath->dwVelocity > ptPath->dwMaxVelocity)
			{
				ptPath->dwVelocity = ptPath->dwMaxVelocity;
				ptPath->dwAccel = 0;
			}
			else if (ptPath->dwVelocity < 0)
				ptPath->dwVelocity = 0;
			ptPath->nAccelCounter = 0;
		}
	}
	nScale = ptPath->dwVelocity * nRate >> 6;
	ptPath->dwVelocityX = ptPath->nStepVectorX * nScale >> 12;
	ptPath->dwVelocityY = ptPath->nStepVectorY * nScale >> 12;
	return TRUE;
}

static __inline BOOL StepWalk(HUNIT hUnit, int nSpeed)
{
	PTPATH ptPath;
	HUNIT hSelf;
	struct TARGETCOORD tCur;
	struct TARGETCOORD tTarget;
	struct GAMECOORD tNext;
	int nDX, nDY;
	BOOL fRecalc;
	BOOL fFound;
	int nSaved;

	ptPath = StepUnitHalt(hUnit);
	if (ptPath == NULL)
		return FALSE;

	for (;;)
	{
		if (!(ptPath->dwFlags & PATH_MISSILE_MASK) &&
			!(ptPath->eType >= 5 && ptPath->eType <= 6 &&
			ptPath->dwStepIndex >= ptPath->dwNumSteps))
		{
			hSelf = ptPath->hUnit;
			fFound = TRUE;
			if (ptPath->hTarget == NULL)
			{
				if (ptPath->dwStepIndex >= ptPath->dwNumSteps)
				{
					tCur.wX = FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwX);
					tCur.wY = FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwY);
					if (!COORD_TEST_EQUAL(tCur, ptPath->tEndCoord))
						fFound = StepUnitRecalcPath(hSelf, 0);
				}
			}
			else
			{
				if ((int)D2CommonUnitCalcEdgeDistance(ptPath->hTarget, hSelf) <= (int)ptPath->bStopDist)
					break;
				tCur.wX = FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwX);
				tCur.wY = FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwY);
				PathUtilGetTargetCoord(&tTarget, ptPath);
				fRecalc = FALSE;
				if ((int)ptPath->hTarget->eType >= UNIT_PLAYER && (int)ptPath->hTarget->eType <= UNIT_MONSTER)
				{
					nDX = ptPath->wKeptTargetX - tTarget.wX;
					nDY = ptPath->wKeptTargetY - tTarget.wY;
					if (nDX < 0)
						nDX = -nDX;
					if (nDX > 5)
						fRecalc = TRUE;
					else
					{
						if (nDY < 0)
							nDY = -nDY;
						if (nDY > 5)
							fRecalc = TRUE;
					}
				}
				if (fRecalc || (ptPath->dwStepIndex >= ptPath->dwNumSteps && !COORD_TEST_EQUAL(ptPath->tEndCoord, tCur)))
					fFound = StepUnitRecalcPath(hSelf, 1);
			}
			if (!fFound)
				break;
		}

		if (!StepSetVelocity(ptPath, nSpeed))
			break;
		if (StepIsStill(ptPath))
			break;

		StepMove(ptPath, &tNext, &nSaved);
		StepSetCoord(ptPath, tNext, NULL);
		if (ptPath->eType != PATH_TYPE_MISSILE && ptPath->dwStepIndex < ptPath->dwNumSteps)
			StepAimAtStep(ptPath, TRUE, TRUE);
		if (ptPath->dwStepIndex < ptPath->dwNumSteps)
			return TRUE;
		break;
	}

	StepSnapToSquare(ptPath);
	StepClearSteps(ptPath);
	ptPath->dwStepIndex = 0;
	StepClearVelocity(ptPath);
	return FALSE;
}

BOOL __stdcall StepUnitWalk(HUNIT hUnit, int nSpeed)
{
	return StepWalk(hUnit, nSpeed);
}

static void __fastcall StepSetCoord(PTPATH ptPath, struct GAMECOORD tCoord, GAMEROOM *hRoomHint)
{
	int nX, nY;

	if ((ptPath->dwFlags & PATH_MISSILE_MASK) &&
		DungeonFindRoomGame((GAMEROOM *)ptPath->dwRoom, tCoord.dwX >> 16, tCoord.dwY >> 16) == NULL)
	{
		ptPath->dwNumSteps = 0;
		return;
	}
	ptPath->tGameCoord = tCoord;
	nX = tCoord.dwX >> 11;
	nY = tCoord.dwY >> 11;
	D2CommonDungeonCalcScaledSubtileToScreen(&nX, &nY);
	ptPath->tRoomTileCoord.dwX = nX;
	ptPath->tRoomTileCoord.dwY = nY;
	if (ptPath->hUnit != NULL && (ptPath->dwFlags & 1))
		StepFindRoom(ptPath, hRoomHint);
}

struct STEPVECTOR
{
	int nX;
	int nY;
};

static __inline BOOL StepWalkSquares(PTPATH ptPath, int nMoveX, int nMoveY, struct GAMECOORD *ptNext, int *pnSaved)
{
	struct STEPVECTOR tStep;
	struct GAMECOORD tCurPos;
	struct GAMECOORD tNextPos, tEndPos;
	struct TARGETCOORD tCur, tNext, tEnd;
	DWORD dwSaveSteps;
	DWORD dwMissile;
	DWORD dwTestPattern;

	ptPath->nSteps = 0;
	tStep.nX = nMoveX;
	tStep.nY = nMoveY;
	PathUtilClampStep(&tStep.nX, &tStep.nY);
	tCurPos = ptPath->tGameCoord;
	tCur.wX = (WORD)(tCurPos.dwX >> 16);
	tCur.wY = (WORD)(tCurPos.dwY >> 16);
	tEndPos.dwX = ptPath->tGameCoord.dwX + nMoveX;
	tEndPos.dwY = ptPath->tGameCoord.dwY + nMoveY;
	tEnd.wX = (WORD)(tEndPos.dwX >> 16);
	tEnd.wY = (WORD)(tEndPos.dwY >> 16);
	dwSaveSteps = ptPath->dwFlags & PATH_SAVE_STEPS_MASK;

	while (!COORD_TEST_EQUAL(tCur, tEnd))
	{
		tNextPos.dwX = tStep.nX + tCurPos.dwX;
		tNextPos.dwY = tStep.nY + tCurPos.dwY;
		tNext.wX = (WORD)(tNextPos.dwX >> 16);
		tNext.wY = (WORD)(tNextPos.dwY >> 16);
		if (!COORD_TEST_EQUAL(tCur, tNext))
		{
			if (ptPath->dwFlags & 4)
			{
#line 502
				D2WARN((UnitGetType(ptPath->hUnit) == UNIT_PLAYER) || (UnitGetType(ptPath->hUnit) == UNIT_MONSTER));

				D2CommonCollisnDualUpdate(ptPath->dwRoom, tCur.wX, tCur.wY, ptPath->dwRoom, tNext.wX,
					tNext.wY, ptPath->dwCollisionPattern, ptPath->dwCollisionMask);
			}
			else
			{
				dwMissile = ptPath->dwFlags & PATH_MISSILE_MASK;
				dwTestPattern = ptPath->dwCollisionTestMask;
				if (dwTestPattern == 5121)
					dwTestPattern = 7169;
				if (dwMissile)
					ptPath->wLastCollision |= D2CommonCollisnMoveSize(ptPath->dwRoom, tCur.wX, tCur.wY,
						tNext.wX, tNext.wY, ptPath->dwUnitSize, ptPath->dwCollisionMask, dwTestPattern);
				else
					ptPath->wLastCollision |= D2CommonCollisnMovePattern(ptPath->dwRoom, tCur.wX, tCur.wY,
						tNext.wX, tNext.wY, ptPath->dwCollisionPattern, ptPath->dwCollisionMask,
							dwTestPattern);
				if ((ptPath->wLastCollision && !dwMissile) || (dwMissile && (ptPath->wLastCollision & 5)))
				{
					ptNext->dwX = (tCurPos.dwX & 0xFFFF0000) + 32768;
					ptNext->dwY = (tCurPos.dwY & 0xFFFF0000) + 32768;
					*pnSaved = ptPath->nSteps;
					return TRUE;
				}
			}
			if (dwSaveSteps)
				ptPath->atSavedSteps[ptPath->nSteps] = tNext;
			ptPath->nSteps++;
#line 634
			D2ASSERT(ptPath->nSteps < MAX_STEP_LEN);
		}
		tCurPos = tNextPos;
		tCur = tNext;
	}

	if (ptPath->nSteps != 0)
		ptPath->dwFlags |= 8;
	*ptNext = tEndPos;
	*pnSaved = ptPath->nSteps;
	return FALSE;
}

static BOOL __fastcall StepMove(PTPATH ptPath, struct GAMECOORD *ptNext, int *pnSaved)
{
	HUNIT hUnit;
	BOOL fRecalcOnHit;
	BOOL fReachesStep;
	int nDX, nDY, nAbsDX, nAbsDY, nAbsVelY, nMax;
	struct TARGETCOORD tFrom, tTo;
	struct GAMECOORD tStep, tVel, tNew;
	struct GAMECOORD tPos = { 0 };

	hUnit = ptPath->hUnit;
	ptPath->wLastCollision = 0;
	*pnSaved = 0;
	fRecalcOnHit = (hUnit->eType == UNIT_MONSTER && (ptPath->dwFlags & 16)) ? TRUE : FALSE;

	if (ptPath->dwVelocityX == 0 && ptPath->dwVelocityY == 0)
	{
		ptPath->dwNumSteps = 0;
		ptPath->dwStepIndex = 0;
		ptNext->dwX = (ptPath->tGameCoord.dwX & 0xFFFF0000) + 32768;
		ptNext->dwY = (ptPath->tGameCoord.dwY & 0xFFFF0000) + 32768;
		return FALSE;
	}

	memcpy(&tVel, &ptPath->dwVelocityX, sizeof(tVel));
	if (ptPath->eType == PATH_TYPE_MISSILE)
		fReachesStep = FALSE;
	else
	{
		tStep.dwX = ((DWORD)((struct TARGETCOORD *)&ptPath->adwSteps[ptPath->dwStepIndex + 1])->wX << 16) + 32768;
		tStep.dwY = ((DWORD)((struct TARGETCOORD *)&ptPath->adwSteps[ptPath->dwStepIndex + 1])->wY << 16) + 32768;
		nDX = (int)(tStep.dwX - ptPath->tGameCoord.dwX);
		nAbsDX = nDX >= 0 ? nDX : -nDX;
		nDY = (int)(tStep.dwY - ptPath->tGameCoord.dwY);
		nAbsDY = nDY >= 0 ? nDY : -nDY;
		nAbsVelY = (int)tVel.dwY < 0 ? -(int)tVel.dwY : (int)tVel.dwY;
		nMax = (int)tVel.dwX < 0 ? -(int)tVel.dwX : (int)tVel.dwX;
		if (nMax <= nAbsVelY) nMax = nAbsVelY;
		if (nAbsDX <= nMax && nAbsDY <= nMax)
		{
			tVel.dwX = nDX;
			tVel.dwY = nDY;
			fReachesStep = TRUE;
		}
		else
			fReachesStep = FALSE;
	}

	tPos = ptPath->tGameCoord;
	tNew.dwX = tPos.dwX + tVel.dwX;
	tNew.dwY = tPos.dwY + tVel.dwY;
	tFrom.wX = (WORD)(tPos.dwX >> 16);
	tFrom.wY = (WORD)(tPos.dwY >> 16);
	tTo.wX = (WORD)(tNew.dwX >> 16);
	tTo.wY = (WORD)(tNew.dwY >> 16);
	if (!COORD_TEST_EQUAL(tFrom, tTo))
	{
		if (ptPath->bDistLeft != 0 && ptPath->eType != PATH_TYPE_KNOCKBACK_SERVER &&
			ptPath->eType != PATH_TYPE_KNOCKBACK_CLIENT)
			ptPath->bDistLeft--;
		if (StepWalkSquares(ptPath, tVel.dwX, tVel.dwY, ptNext, pnSaved))
		{
			if (fRecalcOnHit)
				return StepUnitRecalcPath(hUnit, ptPath->hTarget != NULL) == 0;
			ptPath->dwStepIndex = ptPath->dwNumSteps;
			return FALSE;
		}
	}

	*ptNext = tNew;
	if (!fReachesStep)
		return FALSE;
	ptPath->dwStepIndex++;
	return TRUE;
}

BOOL __stdcall StepUnitTestAtTarget(HUNIT ptUnit)
{
	PTPATH ptPath;
	DWORD dwDist;
	BYTE bStopDist;

	ptPath = ptUnit->ptPath;
	if (ptPath == NULL)
		return TRUE;

	if (ptPath->hTarget != NULL)
	{
		dwDist = D2CommonUnitCalcEdgeDistance(ptPath->hTarget, ptUnit);
		bStopDist = ptPath->bStopDist;
		if ((int)dwDist <= (int)bStopDist)
			return TRUE;
	}

	if ((WORD)(ptPath->tGameCoord.dwX >> 16) == ptPath->tTargetCoord.wX
		&& (WORD)(ptPath->tGameCoord.dwY >> 16) == ptPath->tTargetCoord.wY)
		return TRUE;

	return FALSE;
}

static BOOL __fastcall StepTeleport(PTPATH ptPath, GAMEROOM *hRoomDest, struct TARGETCOORD tDest);

BOOL __stdcall StepTeleportUnit(PTPATH ptPath, DWORD dwUnused, GAMEROOM *hRoomDest, DWORD nX, DWORD nY)
{
	struct TARGETCOORD tDest;

	tDest.wX = (WORD)nX;
	tDest.wY = (WORD)nY;
	if (!StepTeleport(ptPath, hRoomDest, tDest))
		return FALSE;
	ptPath->dwNumSteps = 0;
	return TRUE;
}

static BOOL __fastcall StepTeleport(PTPATH ptPath, GAMEROOM *hRoomDest, struct TARGETCOORD tDest)
{
	struct TARGETCOORD tCur;
	struct GAMECOORD tCoord;

	if (UnitGetType(ptPath->hUnit) == UNIT_MISSILE)
	{
		if (COORD_TEST_EQUAL(tDest, sgctZeroGameCoord))
		{
			D2CommonCollisnClearSize(ptPath->dwRoom, ptPath->tGameCoord.dwX >> 16,
				ptPath->tGameCoord.dwY >> 16, ptPath->dwUnitSize, ptPath->dwCollisionMask);
			ptPath->wLastCollision = 0;
		}
		else
		{
			tCur.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
			tCur.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
			if (COORD_TEST_EQUAL(tCur, tDest))
				ptPath->dwFlags &= ~8;
			else
				ptPath->dwFlags |= 8;
			ptPath->wLastCollision = CollisnMoveSizeRooms((GAMEROOM *)ptPath->dwRoom, tCur.wX, tCur.wY,
				hRoomDest,
				tDest.wX, tDest.wY, ptPath->dwUnitSize, ptPath->dwCollisionMask, ptPath->dwCollisionTestMask);
			ptPath->nSteps = 1;
			ptPath->atSavedSteps[0] = tDest;
		}
	}
	else
	{
		tCur.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
		tCur.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
		if (COORD_TEST_EQUAL(tDest, sgctZeroGameCoord))
			D2CommonCollisnClearPattern(ptPath->dwRoom, tCur.wX, tCur.wY, ptPath->dwCollisionPattern,
				ptPath->dwCollisionMask);
		else
			D2CommonCollisnDualUpdate(ptPath->dwRoom, tCur.wX, tCur.wY, hRoomDest, tDest.wX, tDest.wY,
				ptPath->dwCollisionPattern, ptPath->dwCollisionMask);
	}
	if ((GAMEROOM *)ptPath->dwRoom != hRoomDest)
		ptPath->dwFlags |= 1;
	tCoord.dwX = StepSquareCentre(tDest.wX);
	tCoord.dwY = StepSquareCentre(tDest.wY);
	StepSetCoord(ptPath, tCoord, hRoomDest);
#line 1120
	D2ASSERT(COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || DungeonFindRoomGame( hRoomDest, tDest.wX, tDest.wY ));
	StepFinishMove(ptPath);
	return TRUE;
}

static void __fastcall StepSetRoom(PTPATH ptPath, GAMEROOM *hRoomDest)
{
	HUNIT hUnit;

	hUnit = ptPath->hUnit;
	ptPath->field_20 = ptPath->dwRoom;
	if (ptPath->dwRoom != 0)
		D2CommonUnitRoomRemoveUnit(hUnit);
#line 184
	D2ASSERT(hRoomDest || (ptPath->dwFlags & PATH_MISSILE_MASK) || ( !FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwX) && !FRACTIONAL_TO_GAMESQUARE(ptPath->tGameCoord.dwY) ));

	ptPath->dwRoom = (DWORD)hRoomDest;
	ptPath->dwFlags |= 2;
	if (hRoomDest != NULL)
	{
		D2CommonAddUnitToRoom(hUnit, hRoomDest);
		D2CommonUnitRoomAddToChangedList(hUnit);
	}
}

BOOL __stdcall StepPlaceUnit(PTPATH ptPath, DWORD dwUnused, GAMEROOM *hRoomDest, DWORD nX, DWORD nY)
{
	struct TARGETCOORD tCur, tDest;
	struct GAMECOORD tCoord;

	tCur.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
	tCur.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
	tDest.wX = (WORD)nX;
	tDest.wY = (WORD)nY;
	if (UnitGetType(ptPath->hUnit) == UNIT_MISSILE)
	{
		if (COORD_TEST_EQUAL(tDest, sgctZeroGameCoord))
			D2CommonCollisnClearSize(ptPath->dwRoom, tCur.wX, tCur.wY, ptPath->dwUnitSize,
				ptPath->dwCollisionMask);
		else
			CollisnPlaceSizeRooms((GAMEROOM *)ptPath->dwRoom, tCur.wX, tCur.wY, hRoomDest, tDest.wX,
				tDest.wY, ptPath->dwUnitSize, ptPath->dwCollisionMask);
	}
	else
	{
		if (COORD_TEST_EQUAL(tDest, sgctZeroGameCoord))
			D2CommonCollisnClearPattern(ptPath->dwRoom, tCur.wX, tCur.wY, ptPath->dwCollisionPattern,
				ptPath->dwCollisionMask);
		else
			D2CommonCollisnDualUpdate(ptPath->dwRoom, tCur.wX, tCur.wY, hRoomDest, tDest.wX, tDest.wY,
				ptPath->dwCollisionPattern, ptPath->dwCollisionMask);
	}
	if ((GAMEROOM *)ptPath->dwRoom != hRoomDest)
		ptPath->dwFlags |= 1;
	tCoord.dwX = StepSquareCentre(tDest.wX);
	tCoord.dwY = StepSquareCentre(tDest.wY);
	StepSetCoord(ptPath, tCoord, hRoomDest);
#line 1187
	D2ASSERT(COORD_TEST_EQUAL(tDest, sgctZeroGameCoord) || DungeonFindRoomGame( hRoomDest, tDest.wX, tDest.wY ));
	StepSnapToSquare(ptPath);
	StepClearSteps(ptPath);
	StepClearMotion(ptPath);
	StepClearNumSteps(ptPath);
	return TRUE;
}

static void __fastcall StepFindRoom(PTPATH ptPath, GAMEROOM *hRoomHint)
{
	GAMEROOM *hRoom;
	GAMEROOM *hFound;
	WORD nX, nY;

	nX = (WORD)(ptPath->tGameCoord.dwX >> 16);
	nY = (WORD)(ptPath->tGameCoord.dwY >> 16);
	hRoom = (GAMEROOM *)ptPath->dwRoom;
	if (hRoom != NULL && nX >= hRoom->tRect.nX && nX < hRoom->tRect.nX + hRoom->tRect.nWidth &&
		nY >= hRoom->tRect.nY && nY < hRoom->tRect.nY + hRoom->tRect.nHeight)
		return;

	hFound = DungeonFindRoomGame(hRoom, nX, nY);
	if (hFound == NULL && hRoomHint != NULL)
		hFound = DungeonFindRoomGame(hRoomHint, nX, nY);
	if (hFound == NULL && (ptPath->dwFlags & PATH_MISSILE_MASK))
	{
		ptPath->dwNumSteps = 0;
		return;
	}
	StepSetRoom(ptPath, hFound);
}

BOOL __stdcall StepTeleportUnitForceRecalc(PTPATH ptPath, DWORD dwUnused, GAMEROOM *hRoomDest, DWORD nX,
	DWORD nY)
{
	struct TARGETCOORD tDest;

	tDest.wX = (WORD)nX;
	tDest.wY = (WORD)nY;
	if (!StepTeleport(ptPath, hRoomDest, tDest))
		return FALSE;
	ptPath->dwRecalcCounter = 15;
	return TRUE;
}

BOOL __stdcall StepMoveUnit(PTPATH ptPath, HUNIT hUnit, GAMEROOM *hRoomDest, DWORD nX, DWORD nY)
{
	struct GAMECOORD tCoord;

#line 1252
	D2WARN(UnitGetType(hUnit) == UNIT_PLAYER || UnitGetType(hUnit) == UNIT_MONSTER);
	if (CollisnMovePatternRooms((GAMEROOM *)ptPath->dwRoom, ptPath->tGameCoord.dwX >> 16,
		ptPath->tGameCoord.dwY >> 16, hRoomDest, nX, nY, ptPath->dwCollisionPattern, ptPath->dwCollisionMask,
		ptPath->dwCollisionTestMask))
		return FALSE;

	if ((GAMEROOM *)ptPath->dwRoom != hRoomDest)
		ptPath->dwFlags |= 1;
	tCoord.dwX = StepSquareCentre(nX);
	tCoord.dwY = StepSquareCentre(nY);
	StepSetCoord(ptPath, tCoord, hRoomDest);
#line 1275
	D2ASSERT(DungeonFindRoomGame( hRoomDest, nX, nY ));
	StepFinishMove(ptPath);
	return TRUE;
}
#line 1285

void __stdcall StepSnapToSquare(PTPATH ptPath)
{
	struct GAMECOORD tCoord;

	tCoord.dwX = (ptPath->tGameCoord.dwX & 0xFFFF0000) + 32768;
	tCoord.dwY = (ptPath->tGameCoord.dwY & 0xFFFF0000) + 32768;
	StepSetCoord(ptPath, tCoord, NULL);
}

DWORD __stdcall StepTestSquareCrossed(PTPATH ptPath)
{
	return ptPath->dwFlags & 8;
}

void __stdcall StepRideHorse(HUNIT hRiderUnit, HUNIT hHorseUnit)
{
	PTPATH ptRider, ptHorse;

#line 1310
	D2ASSERT(hRiderUnit);
#line 1311
	D2ASSERT(hHorseUnit);
	ptRider = hRiderUnit->ptPath;
	ptHorse = hHorseUnit->ptPath;
	if (FRACTIONAL_TO_GAMESQUARE(ptRider->tGameCoord.dwX) == FRACTIONAL_TO_GAMESQUARE(ptHorse->tGameCoord.dwX) &&
		FRACTIONAL_TO_GAMESQUARE(ptRider->tGameCoord.dwY) == FRACTIONAL_TO_GAMESQUARE(ptHorse->tGameCoord.dwY))
		ptRider->dwFlags &= ~8;
	else
		ptRider->dwFlags |= 8;
	StepSetCoord(ptRider, ptHorse->tGameCoord, (GAMEROOM *)ptHorse->dwRoom);
}

BOOL __stdcall StepTestGridBit(void **phGrid, DWORD nRow, DWORD nCol)
{
	return FogBitManipTestBit(*phGrid, nRow * 16 + nCol) != 0;
}
