#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\PATH\\Path.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "PathLocal.h"
void __stdcall PathSetCollisionMask(PTPATH ptPath, DWORD dwValue);
void __stdcall PathSetType(PTPATH ptPath, DWORD eType);
void __stdcall PathSetFacingImmediate(PTPATH ptPath, BYTE bDir);
struct PATHDIROFFSET
{
	int			nX;
	int			nY;
};
static const struct PATHDIROFFSET sgcanPathDirOffsets[8] =
{
	{ 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 }, { 1, -1 },
};
static const struct TARGETCOORD sgctZeroGameCoord = { 0, 0 };
static const DWORD sgcadwPathField48[4] =
{
	0, 1, 1, 2,
};
static const int sgcanPathNudge[81] =
{
	-1, 0, 0, 0, 0, 0, 0, 0, 1,
	-1, -1, 0, 0, 0, 0, 0, 1, 1,
	-1, -1, -1, 0, 0, 0, 1, 1, 1,
	-1, -1, -1, -1, 0, 1, 1, 1, 1,
	-1, -1, -1, -1, 0, 1, 1, 1, 1,
	-1, -1, -1, -1, 0, 1, 1, 1, 1,
	-1, -1, -1, 0, 0, 0, 1, 1, 1,
	-1, -1, 0, 0, 0, 0, 0, 1, 1,
	-1, 0, 0, 0, 0, 0, 0, 0, 1,
};

typedef int (__fastcall *PFNPATHFUNCTION)(PTPATHINFO ptPathInfo);
int __fastcall IDAstarFindPath(PTPATHINFO ptPathInfo);
int __fastcall PathFindAStar(PTPATHINFO ptPathInfo);
int __fastcall PathMiscStraight(PTPATHINFO ptPathInfo);
int __fastcall PathMiscFindStraightFromStart(PTPATHINFO ptPathInfo);
int __fastcall PathMiscFindStraightOrSearch(PTPATHINFO ptPathInfo);
int __fastcall PathMiscOnKnockbackServer(PTPATHINFO ptPathInfo);
int __fastcall PathMiscFindCutBackStep(PTPATHINFO ptPathInfo);
int __fastcall PathMiscOnKnockbackClient(PTPATHINFO ptPathInfo);
int __fastcall PathMiscFindWalkTurningRetry(PTPATHINFO ptPathInfo);
int __fastcall PathWFFindLineDetour(PTPATHINFO ptPathInfo);
void __fastcall PathMiscMissileA(PTPATH ptPath, D2SEED *ptSeed);
void __fastcall PathMiscMissileE(PTPATH ptPath, D2SEED *ptSeed);

static PFNPATHFUNCTION const scpfnPathFunction[16] =
{
	IDAstarFindPath, PathFindAStar, PathMiscStraight, PathMiscFindStraightFromStart, NULL, PathMiscStraight,
	PathMiscStraight, PathMiscFindStraightOrSearch, PathMiscOnKnockbackServer, PathMiscFindCutBackStep, NULL,
	PathMiscOnKnockbackClient,
	PathMiscFindWalkTurningRetry, PathMiscStraight, NULL, PathWFFindLineDetour,
};

static const DWORD sgcadwPathTurnRate[64] =
{
	0, 1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4,
	4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8,
	0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8, 0xFFFFFFF8,
	0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC, 0xFFFFFFFC,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};
static const DWORD sgcadwPathTypeFlags[16] =
{
	137472, 6400, 0, 0, 393216, 0, 0, 137472,
	124416, 124928, 393216, 124420, 0, 256, 393216, 2048,
};

static const DWORD sgcadwPathTypeInit[16] =
{
	0, 0, 0, 0, 0, 2, 0xFFFFFFFE, 0,
	0, 0, 0, 0, 0xFFFFFFFC, 0, 0, 0,
};

void __fastcall PathTestStepsInRoom(PTPATH ptPath)
{
	GAMEROOM *hRoom;
	struct TARGETCOORD *ptSteps;
	int i;

	ptPath->dwFlags &= ~1;
	hRoom = (GAMEROOM *)ptPath->dwRoom;
	ptSteps = (struct TARGETCOORD *)&ptPath->adwSteps[1];
	for (i = 0; i < (int)ptPath->dwNumSteps; i++)
	{
		if (hRoom == NULL || ptSteps[i].wX < hRoom->tRect.nX || ptSteps[i].wX >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
			ptSteps[i].wY < hRoom->tRect.nY || ptSteps[i].wY >= hRoom->tRect.nY + hRoom->tRect.nHeight)
		{
			ptPath->dwFlags |= 1;
			return;
		}
	}
}

void __stdcall PathGetWaypointSquare(HUNIT hUnit, int *pnX, int *pnY)
{
	PTPATH ptPath;

	if (hUnit && D2CommonUnitTestModeGroup(hUnit))
	{
		ptPath = hUnit->ptPath;
		if (ptPath->dwNumSteps != 0)
		{
			*pnX = (int)ptPath->dwVelocityX;
			*pnY = (int)ptPath->dwVelocityY;
			*pnX = *pnX >> 11;
			*pnY = *pnY >> 11;
			D2CommonDungeonCalcScaledSubtileToScreen(pnX, pnY);
			return;
		}
		*pnX = 0; *pnY = 0; return;
	}
	*pnX = 0;
	*pnY = 0;
}

void __stdcall PathSetCollisionByType(HUNIT hUnit)
{
	switch ((int)hUnit->eType)
	{
	case 2:
		D2CommonCollisnSetArea(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), D2CommonUnitGetSizeX(hUnit), D2CommonUnitGetSizeY(hUnit), D2CommonUnitGetTypeFlags(hUnit));
		break;
	case 0:
	case 1:
	{
		DWORD dwCollisionPattern = hUnit->ptPath->dwCollisionPattern;

		D2CommonCollisnSetPattern(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), dwCollisionPattern, D2CommonUnitGetTypeFlags(hUnit));
		break;
	}
	default:
		D2CommonCollisnSetSize(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), D2CommonUnitGetSizeX(hUnit), D2CommonUnitGetTypeFlags(hUnit));
		break;
	}
}

BOOL __stdcall PathClearCollisionByType(HUNIT hUnit, BOOL fFlag)
{
	DWORD dwCollisionPattern;

	switch (hUnit->eType)
	{
	case 1:
		if (hUnit->dwMode == 12 || hUnit->dwMode == 0)
		{
			if (!fFlag)
				break;
		}
		dwCollisionPattern = hUnit->ptPath->dwCollisionPattern;
		D2CommonCollisnClearPattern(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), dwCollisionPattern, D2CommonUnitGetTypeFlags(hUnit));
		return TRUE;
	case 0:
		if ((hUnit->dwMode != 17 && hUnit->dwMode != 0)
			|| fFlag)
		{
			dwCollisionPattern = hUnit->ptPath->dwCollisionPattern;
			D2CommonCollisnClearPattern(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), dwCollisionPattern, D2CommonUnitGetTypeFlags(hUnit));
			return TRUE;
		}
		break;
	case 2:
		if (D2CommonUnitTestObjectHasCollision(hUnit) || fFlag)
		{
			D2CommonCollisnClearArea(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), D2CommonUnitGetSizeX(hUnit), D2CommonUnitGetSizeY(hUnit), D2CommonUnitGetTypeFlags(hUnit));
			return TRUE;
		}
		break;
	default:
		D2CommonCollisnClearSize(D2CommonUnitGetPathRoom(hUnit), D2CommonUnitGetX(hUnit), D2CommonUnitGetY(hUnit), D2CommonUnitGetSizeX(hUnit), D2CommonUnitGetTypeFlags(hUnit));
		return TRUE;
	}
	return FALSE;
}

void __fastcall PathNothingUncalled(void)
{
}

void __fastcall PathNothingFirst(void)
{
}

DWORD __stdcall PathNothingZero(DWORD dwUnused1, DWORD dwUnused2)
{
	return 0;
}
static __inline int ABS(int nValue)	{ return nValue < 0 ? -nValue : nValue; }
#define PATH_MAX_MISSILE_RANGE	100
#define COORD_TEST_EQUAL(tA, tB)	((tA).wX == (tB).wX && (tA).wY == (tB).wY)
#define PATH_MISSILE_MASK	262144
#define UnitIsDoor(hUnit)	D2CommonUnitIsDoorRaw((DWORD)(hUnit))

static DWORD __fastcall PathMissileToward(PTPATH ptPath, HUNIT hUnit);
static void __fastcall PathExtendTarget(PTPATHINFO ptPathInfo);

static inline BOOL PathFindFreeTarget(PTPATHINFO ptPathInfo)
{
	WORD wStartX, wStartY, wTargetX, wTargetY;
	GAMEROOM *hRoom;
	DWORD dwPattern, dwTestPattern;
	DWORD eUnitType;
	struct TARGETCOORD atProbe[3];
	int anDir[3], anNewDir[3];
	PTPATH ptPath = ptPathInfo->ptPath;

#line 190
	D2WARN(!(ptPath->dwFlags & PATH_MISSILE_MASK));
	wStartX = ptPathInfo->tStartCoord.wX;
	wStartY = ptPathInfo->tStartCoord.wY;
	wTargetX = ptPathInfo->tTargetCoord.wX;
	wTargetY = ptPathInfo->tTargetCoord.wY;
	hRoom = ptPathInfo->hRoomStart;
	dwPattern = ptPathInfo->dwCollisionPattern;
	dwTestPattern = ptPathInfo->dwCollisionTestMask;
	eUnitType = ptPathInfo->ptPath->hUnit->eType;
	atProbe[0].wX = wTargetX;
	atProbe[0].wY = wTargetY;
	atProbe[1].wX = wTargetX;
	atProbe[1].wY = wTargetY;
	atProbe[2].wX = wTargetX;
	atProbe[2].wY = wTargetY;
	PathUtilGetDirections(ptPathInfo->tTargetCoord, ptPathInfo->tStartCoord, anDir);
	for (;;)
	{
		if (wStartX == atProbe[0].wX && wStartY == atProbe[0].wY)
			return FALSE;
		atProbe[0].wX += (WORD)sgcanPathDirOffsets[anDir[0]].nX;
		atProbe[0].wY += (WORD)sgcanPathDirOffsets[anDir[0]].nY;
		if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, atProbe[0].wX, atProbe[0].wY, dwPattern, dwTestPattern))
		{
			ptPathInfo->tTargetCoord.wX = atProbe[0].wX;
			ptPathInfo->tTargetCoord.wY = atProbe[0].wY;
			if (atProbe[0].wX == wStartX && atProbe[0].wY == wStartY)
				return FALSE;
			if (eUnitType == UNIT_PLAYER)
				PathExtendTarget(ptPathInfo);
			return TRUE;
		}
		atProbe[1].wX += (WORD)sgcanPathDirOffsets[anDir[1]].nX;
		atProbe[1].wY += (WORD)sgcanPathDirOffsets[anDir[1]].nY;
		if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, atProbe[1].wX, atProbe[1].wY, dwPattern, dwTestPattern))
		{
			ptPathInfo->tTargetCoord.wX = atProbe[1].wX;
			ptPathInfo->tTargetCoord.wY = atProbe[1].wY;
			if (atProbe[1].wX == wStartX && atProbe[1].wY == wStartY)
				return FALSE;
			if (eUnitType == UNIT_PLAYER)
				PathExtendTarget(ptPathInfo);
			return TRUE;
		}
		if (anDir[2] != 255)
		{
			atProbe[2].wX += (WORD)sgcanPathDirOffsets[anDir[2]].nX;
			atProbe[2].wY += (WORD)sgcanPathDirOffsets[anDir[2]].nY;
			if (!D2CommonCollisnTestPatternAnyRaw((DWORD)hRoom, atProbe[2].wX, atProbe[2].wY, dwPattern,
				dwTestPattern))
			{
				ptPathInfo->tTargetCoord.wX = atProbe[2].wX;
				ptPathInfo->tTargetCoord.wY = atProbe[2].wY;
				if (atProbe[2].wX == wStartX && atProbe[2].wY == wStartY)
					return FALSE;
				if (eUnitType == UNIT_PLAYER)
					PathExtendTarget(ptPathInfo);
				return TRUE;
			}
		}
		PathUtilGetDirections(atProbe[0], ptPathInfo->tStartCoord, anNewDir);
		anDir[0] = anNewDir[0];
	}
}

static inline DWORD PathMissile(PTPATH ptPath, HUNIT hUnit)
{
	switch (ptPath->eType)
	{
	case PATH_TYPE_MISSILE:
		if (0)
			PathMissileToward(ptPath, hUnit);
		if (PathMissileToward(ptPath, hUnit))
		{
			ptPath->dwFlags |= 32;
			return ptPath->dwNumSteps;
		}
		ptPath->dwFlags &= ~32;
		return 0;
	case 10:
		PathMiscMissileA(ptPath, (D2SEED *)&hUnit->tSeed);
		ptPath->dwFlags |= 32;
		return ptPath->dwNumSteps;
	case 14:
		PathMiscMissileE(ptPath, (D2SEED *)&hUnit->tSeed);
		ptPath->dwFlags |= 32;
		return ptPath->dwNumSteps;
	}
#line 508
	D2WARNMSG("Not a missile path!");
	ptPath->dwFlags &= ~32;
	return 0;
}

static inline BYTE PathDoorTarget(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	HUNIT hTarget;

	ptPath = ptPathInfo->ptPath;
	hTarget = ptPath->hTarget;
#line 522
	D2ASSERT(UnitIsDoor( hTarget));
	if (UnitDoorOpensAlongY(hTarget))
	{
		if ((ptPath->tGameCoord.dwY >> 16) < D2CommonUnitGetY(hTarget))
			ptPath->tTargetCoord.wY -= 2;
		else
			ptPath->tTargetCoord.wY += 2;
	}
	else
	{
		if ((ptPath->tGameCoord.dwX >> 16) < D2CommonUnitGetX(hTarget))
			ptPath->tTargetCoord.wX -= 2;
		else
			ptPath->tTargetCoord.wX += 2;
	}
	return 2;
}

static inline BYTE PathAimAtTarget(PTPATHINFO ptPathInfo)
{
	PTPATH ptPath;
	HUNIT hTarget;

	ptPath = ptPathInfo->ptPath;
	hTarget = ptPath->hTarget;
	if (hTarget != NULL)
	{
		ptPath->tTargetCoord.wX = (WORD)D2CommonUnitGetX(hTarget);
		ptPath->tTargetCoord.wY = (WORD)D2CommonUnitGetY(hTarget);
		if (ptPath->tTargetCoord.wX == 0 && ptPath->tTargetCoord.wY == 0)
			return 0;
		switch (hTarget->eType)
		{
		case UNIT_ITEM:
			return 2;
		case UNIT_OBJECT:
			if (UnitIsDoor(hTarget))
				return PathDoorTarget(ptPathInfo);
			return 2;
		case UNIT_PLAYER:
		case UNIT_MONSTER:
			if (ptPath->bTargetOffset != 0)
				PathUtilOffsetTarget(ptPath);
			break;
		default:
			break;
		}
	}
	return 1;
}

DWORD __stdcall PathCompute(PTPATH ptPath, HUNIT hUnit)
{
	PATHINFO tPathInfo;
	struct TARGETCOORD tStart;
	HUNIT hTarget;
	int nDX, nDY;
	BOOL fTargetLifted;
	BOOL fFindPath;

#line 720
	D2ASSERT(ptPath);
#line 721
	D2ASSERT(ptPath->hUnit == hUnit);

	if (ptPath->dwFlags & PATH_MISSILE_MASK)
		return PathMissile(ptPath, hUnit);

	ptPath->wLastCollision = 0;
	hTarget = ptPath->hTarget;
	if (hTarget != NULL &&
		(D2CommonUnitGetX(hTarget) == 0 || D2CommonUnitGetY(hTarget) == 0))
		return 0;
	tStart.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
	tStart.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
	tPathInfo.tStartCoord = tStart;
	if (!COORD_TEST_EQUAL(tStart, sgctZeroGameCoord))
	{
		tPathInfo.dwUnitSize = ptPath->dwUnitSize;
		tPathInfo.dwCollisionPattern = ptPath->dwCollisionPattern;
		tPathInfo.ptPath = ptPath;
		tPathInfo.dwCollisionTestMask = ptPath->dwCollisionTestMask;
		tPathInfo.dwDistMax = ptPath->bDistMax;
		tPathInfo.bSearchReach = ptPath->bSearchReach;
		tPathInfo.bTargetRange = PathAimAtTarget(&tPathInfo);
		if (!COORD_TEST_EQUAL(ptPath->tTargetCoord, sgctZeroGameCoord))
		{
			tPathInfo.tTargetCoord = ptPath->tTargetCoord;
			if (!COORD_TEST_EQUAL(tPathInfo.tStartCoord, tPathInfo.tTargetCoord))
			{
				nDX = (int)tPathInfo.tTargetCoord.wX - (int)tPathInfo.tStartCoord.wX;
				nDY = (int)tPathInfo.tTargetCoord.wY - (int)tPathInfo.tStartCoord.wY;
				if (nDX <= 100 && -nDX <= 100 && nDY <= 100 && -nDY <= 100)
				{
					tPathInfo.hRoomStart = (GAMEROOM *)ptPath->dwRoom;
					tPathInfo.hRoomTarget = DungeonFindRoomGame(tPathInfo.hRoomStart, tPathInfo.tTargetCoord.wX,
						tPathInfo.tTargetCoord.wY);
#line 791
					D2ASSERT(tPathInfo.hRoomStart);
					if (hUnit->eType != UNIT_MONSTER || !D2CommonDungeonRoomIsTown(tPathInfo.hRoomTarget) ||
						MonsterRegionMayEnterTown(hUnit))
					{
						tPathInfo.ePathType = ptPath->eType;
						hTarget = ptPath->hTarget;
						fTargetLifted = FALSE;
						PathClearCollisionByType(hUnit, FALSE);
						if (hTarget != NULL && D2CommonUnitGetSizeX(hTarget) != 0 && (ptPath->dwFlags & 2048))
							fTargetLifted = PathClearCollisionByType(hTarget, FALSE);

						fFindPath = TRUE;
						if ((ptPath->dwFlags & 4096) &&
							D2CommonCollisnTestPatternAnyRaw((DWORD)tPathInfo.hRoomStart, tPathInfo.tTargetCoord.wX,
								tPathInfo.tTargetCoord.wY,
							ptPath->dwCollisionPattern, ptPath->dwCollisionTestMask))
							fFindPath = PathFindFreeTarget(&tPathInfo);
						ptPath->tEndCoord.wX = tPathInfo.tTargetCoord.wX;
						ptPath->dwStepIndex = 0;
						ptPath->tEndCoord.wY = tPathInfo.tTargetCoord.wY;
						ptPath->dwNumSteps = 0;
						if (fFindPath)
						{
							ptPath->dwStepIndex = 0;
#line 869
							D2ASSERTMSG(!IsBadCodePtr((FARPROC)scpfnPathFunction[tPathInfo.ePathType]), "scpfnPathFunction[tPathInfo.ePathType]");
							ptPath->dwNumSteps = scpfnPathFunction[tPathInfo.ePathType](&tPathInfo);
						}
						else
						{
							ptPath->dwStepIndex = 0;
							ptPath->dwNumSteps = 0;
						}
#line 880
						D2ASSERT(COORD_TEST_EQUAL(tPathInfo.tStartCoord, tStart));
						if (fTargetLifted)
							PathSetCollisionByType(ptPath->hTarget);
						PathSetCollisionByType(hUnit);
						if (ptPath->dwNumSteps == 0)
						{
							ptPath->dwFlags &= ~32;
							return 0;
						}

						StepAimAtStep(ptPath, TRUE, TRUE);
						if ((int)ptPath->dwStepIndex < (int)ptPath->dwNumSteps)
						{
							PathTestStepsInRoom(ptPath);
							ptPath->dwRecalcCounter = 0;
							ptPath->wKeptTargetX = ptPath->tTargetCoord.wX;
							ptPath->wKeptTargetY = ptPath->tTargetCoord.wY;
							if (!(ptPath->dwFlags & 16) && ptPath->hTarget == NULL)
							{
								ptPath->tTargetCoord.wX = ((struct TARGETCOORD *)ptPath->adwSteps)[ptPath->dwNumSteps].wX;
								ptPath->tTargetCoord.wY = ((struct TARGETCOORD *)ptPath->adwSteps)[ptPath->dwNumSteps].wY;
							}
							if (ptPath->dwNumSteps != 0)
								ptPath->dwFlags |= 32;
							else
								ptPath->dwFlags &= ~32;
							return ptPath->dwNumSteps;
						}
					}
				}
			}
		}
	}
	ptPath->dwStepIndex = 0;
	ptPath->dwNumSteps = 0;
	ptPath->dwFlags &= ~32;
	return 0;
}

static DWORD __fastcall PathMissileToward(PTPATH ptPath, HUNIT hUnit)
{
	struct TARGETCOORD tStart;
	GAMEROOM *hRoom;

	tStart.wX = (WORD)(ptPath->tGameCoord.dwX >> 16);
	tStart.wY = (WORD)(ptPath->tGameCoord.dwY >> 16);
	ptPath->dwStepIndex = 0;
	if (ptPath->hTarget != NULL)
	{
		ptPath->tTargetCoord.wX = (WORD)D2CommonUnitGetX(ptPath->hTarget);
		ptPath->tTargetCoord.wY = (WORD)D2CommonUnitGetY(ptPath->hTarget);
	}
#line 93
	D2ASSERT(ABS( tStart.wX - ptPath->tTargetCoord.wX ) < PATH_MAX_MISSILE_RANGE);
#line 94
	D2ASSERT(ABS( tStart.wY - ptPath->tTargetCoord.wY ) < PATH_MAX_MISSILE_RANGE);
	if (ptPath->tTargetCoord.wX == 0 || ptPath->tTargetCoord.wY == 0)
		return 0;
	ptPath->adwSteps[1] = *(DWORD *)&ptPath->tTargetCoord;
	ptPath->dwNumSteps = 1;
	ptPath->dwRecalcCounter = 0;
	StepAimAtStep(ptPath, TRUE, TRUE);
	hRoom = (GAMEROOM *)ptPath->dwRoom;
	if (hRoom == NULL || ptPath->tTargetCoord.wX < hRoom->tRect.nX ||
		ptPath->tTargetCoord.wX >= hRoom->tRect.nX + hRoom->tRect.nWidth ||
		ptPath->tTargetCoord.wY < hRoom->tRect.nY ||
		ptPath->tTargetCoord.wY >= hRoom->tRect.nY + hRoom->tRect.nHeight)
		ptPath->dwFlags |= 1;
	return ptPath->dwNumSteps;
}

static void __fastcall PathExtendTarget(PTPATHINFO ptPathInfo)
{
	int nDX, nDY, nAbsX, nAbsY, nDist;
	int nStepX, nStepY;
	DWORD dwX, dwY;
	DWORD dwPattern, dwTestPattern;

	dwX = ptPathInfo->tTargetCoord.wX;
	nDX = (int)dwX - (int)ptPathInfo->tStartCoord.wX;
	dwY = ptPathInfo->tTargetCoord.wY;
	nDY = (int)dwY - (int)ptPathInfo->tStartCoord.wY;
	nAbsX = nDX < 0 ? -nDX : nDX;
	nAbsY = nDY < 0 ? -nDY : nDY;
	if (nAbsX >= 5 || nAbsY >= 5 || (nAbsX == 0 && nAbsY == 0))
		return;
	dwPattern = ptPathInfo->dwCollisionPattern;
	dwTestPattern = ptPathInfo->dwCollisionTestMask;
	nStepX = sgcanPathNudge[40 + nDX + nDY * 9];
	nStepY = sgcanPathNudge[40 + nDY + nDX * 9];
	nDist = nAbsX > nAbsY ? nAbsX : nAbsY;
	while (nDist < 5)
	{
		dwX += nStepX;
		dwY += nStepY;
		if (D2CommonCollisnTestPatternAnyRaw((DWORD)ptPathInfo->hRoomStart, dwX, dwY, dwPattern, dwTestPattern))
			return;
		nDist++;
		ptPathInfo->tTargetCoord.wX = (WORD)dwX;
		ptPathInfo->tTargetCoord.wY = (WORD)dwY;
	}
}

void __fastcall PathNothingThird(void)
{
}

void __fastcall PathNothingFourth(void)
{
}

void __stdcall PathFreePool(void *pPool, void *p)
{
#line 960
	D2FREEPOOL(pPool, p);
}

void __stdcall PathResetCollisionPattern(HUNIT hUnit)
{
	PTPATH ptPath;

	ptPath = hUnit->ptPath;
	if (ptPath == NULL)
		return;
	PathClearCollisionByType(hUnit, TRUE);
	hUnit->ptPath->dwCollisionPattern = sgcadwPathField48[ptPath->dwUnitSize];
	PathSetCollisionByType(hUnit);
}

void __stdcall PathCreate(void *pPool, DWORD dwRoom, DWORD dwX, DWORD dwY, HUNIT hUnit, BOOL fFlag)
{
	PTPATH ptPath;
	PTMONSTATS ptMonStats;
	int nX, nY;
	int i;

#line 999
	ptPath = (PTPATH)D2ALLOCPOOL(pPool, sizeof(PATH));
	for (i = 0; i < (int)(sizeof(PATH) / sizeof(DWORD)); i++)
		((DWORD *)ptPath)[i] = 0;
	hUnit->ptPath = ptPath;
	ptPath->hUnit = hUnit;
	ptPath->dwUnitSize = D2CommonUnitGetSizeX(hUnit);
	ptPath->dwCollisionPattern = sgcadwPathField48[ptPath->dwUnitSize];
	ptPath->dwVelocity = 2048;
	ptPath->tGameCoord.dwX = (dwX << 16) + 32768;
	ptPath->dwRoom = dwRoom;
	ptPath->tGameCoord.dwY = (dwY << 16) + 32768;
	ptPath->bStopDist = 0;
	ptPath->nSteps = 1;
	ptPath->atSavedSteps[0].wX = (WORD)dwX;
	ptPath->atSavedSteps[0].wY = (WORD)dwY;

	switch (hUnit->eType)
	{
	case UNIT_PLAYER:
		ptPath->dwCollisionMask = 128;
		ptPath->dwCollisionTestMask = 7177;
		PathSetType(ptPath, 7);
		ptPath->bDistMax = 95;
		ptPath->bSearchReach = 70;
		break;

	case UNIT_MONSTER:
		ptPath->dwCollisionMask = 256;
		PathSetType(ptPath, PATH_TYPE_TOWARD);
		if (D2CommonMonsterUnitGetTypeLocalIndex(hUnit) != 38)
		{
			ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(hUnit->dwClassId);
			ptPath->dwCollisionTestMask = ptMonStats->bFlying ? 6148 : ptMonStats->bOpenDoors ? 5121 : 7169;
			ptPath->bDistMax = 14;
		}
		else
		{
			hUnit->ptPath->dwCollisionPattern = 3;
			ptPath->dwCollisionTestMask = 2052;
			ptPath->bDistMax = 14;
		}
		break;

	case UNIT_MISSILE:
		ptPath->dwCollisionMask = 0;
		ptPath->dwCollisionTestMask = 0;
		PathSetType(ptPath, PATH_TYPE_MISSILE);
		break;
	}

	if (dwRoom)
	{
		PathSetCollisionByType(hUnit);
		D2CommonAddUnitToRoom(hUnit, ptPath->dwRoom);
	}

	nX = ptPath->tGameCoord.dwX >> 11;
	nY = ptPath->tGameCoord.dwY >> 11;
	D2CommonDungeonCalcScaledSubtileToScreen(&nX, &nY);
	ptPath->tRoomTileCoord.dwX = nX;
	ptPath->tRoomTileCoord.dwY = nY;
	if (fFlag)
		ptPath->dwFlags |= 16;
}

void __fastcall PathSetFacing(PTPATH ptPath, BYTE bDir)
{
	bDir &= 63;
	switch (UnitGetType(ptPath->hUnit))
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
		ptPath->bFacing = bDir;
		return;
	case UNIT_MISSILE:
		if (!(ptPath->dwFlags & 64))
			ptPath->bFacing = bDir;
		return;
	default:
		if (bDir == ptPath->bTargetFacing)
			return;
		ptPath->bTargetFacing = bDir;
		ptPath->bTurnRate = (BYTE)sgcadwPathTurnRate[(BYTE)(bDir - ptPath->bFacing) & 63];
		return;
	}
}

void __stdcall PathStepFacing(PTPATH ptPath)
{
	BYTE bCur, bTarget, bRate, bNext, bDelta;

	bCur = ptPath->bFacing;
	bTarget = ptPath->bTargetFacing;
	if (bCur == bTarget)
		return;
	if (ptPath->dwFlags & 64)
		return;

	bRate = ptPath->bTurnRate;
	bNext = (BYTE)((bRate + bCur) & 63);
	ptPath->bFacing = bNext;
	bDelta = (BYTE)((bNext - bTarget) & 63);
	if (bDelta > 31 && bRate > 63)
		ptPath->bFacing = bTarget;
	else if (bDelta < 31 && bRate < 63)
		ptPath->bFacing = bTarget;
}

void __stdcall PathTurnToSquare(PTPATH ptPath, DWORD nX, DWORD nY, BOOL fImmediate)
{
	struct GAMECOORD tTo;
	BYTE bDir;

	tTo.dwX = (nX << 16) + 32768;
	tTo.dwY = (nY << 16) + 32768;
	bDir = (BYTE)StepGetDirection(ptPath->tGameCoord, tTo);
	if (fImmediate)
		PathSetFacingImmediate(ptPath, bDir);
	else
		PathSetFacing(ptPath, bDir);
}

void __stdcall PathClearIfExists(HUNIT hUnit)
{
	if (hUnit->ptPath != NULL)
		PathClearCollisionByType(hUnit, TRUE);
}

void __stdcall PathReplaceCollisionMask(HUNIT hUnit, BOOL fForce)
{
	PTPATH ptPath = hUnit->ptPath;

	if (!fForce && hUnit->eType == 1)
	{
		if (ptPath->dwCollisionMask != 32768)
		{
			if (ptPath != NULL)
				PathClearCollisionByType(hUnit, TRUE);
			hUnit->ptPath->dwCollisionPattern = 3;
			PathSetCollisionMask(ptPath, 32768);
			if (ptPath->dwUnitSize != 0)
				D2CommonCollisnSetArea(ptPath->dwRoom, ptPath->tGameCoord.dwX >> 16, ptPath->tGameCoord.dwY >> 16,
					3, 3, 32768);
		}
	}
	else
	{
		if (ptPath != NULL)
			PathClearCollisionByType(hUnit, TRUE);
		D2CommonCollisnSetArea(ptPath->dwRoom, ptPath->tGameCoord.dwX >> 16, ptPath->tGameCoord.dwY >> 16, 3, 3,
			32768);
	}

	D2CommonStepSnapToSquare(ptPath);
}

void __stdcall PathRestoreCollisionMask(HUNIT hUnit, BOOL fSkip)
{
	PTPATH ptPath;
	DWORD dwCollisionPattern;

	if (fSkip)
		return;
#line 1224
	D2ASSERT(UnitGetType (hUnit) == UNIT_MONSTER);
	ptPath = hUnit->ptPath;
	if (ptPath->dwCollisionMask != 32768)
		return;
	dwCollisionPattern = sgcadwPathField48[D2CommonUnitGetSizeX(hUnit)];
	switch ((int)D2CommonMonsterUnitGetTypeLocalIndex(hUnit))
	{
	case 38:
	case 157:
	case 158:
	case 319:
		dwCollisionPattern = 3;
		break;
	}
	hUnit->ptPath->dwCollisionPattern = dwCollisionPattern;
	PathSetCollisionMask(ptPath, 256);
}

void __stdcall PathSetVelocity(PTPATH ptPath, DWORD dwValue)
{
	if (dwValue != ptPath->dwVelocity)
		ptPath->dwRecalcCounter = 15;
	ptPath->dwVelocity = dwValue;
	ptPath->dwMaxVelocity = dwValue;
}

DWORD __stdcall PathGetVelocity(PTPATH ptPath)
{
	return ptPath->dwVelocity;
}

void __stdcall PathSetMaxVelocity(PTPATH ptPath, DWORD dwValue)
{
	ptPath->dwMaxVelocity = dwValue;
}

DWORD __stdcall PathGetMaxVelocity(PTPATH ptPath)
{
	return ptPath->dwMaxVelocity;
}

void __stdcall PathSetAccel(PTPATH ptPath, DWORD dwValue)
{
	ptPath->dwAccel = dwValue;
}

DWORD __stdcall PathGetAccel(PTPATH ptPath)
{
	return ptPath->dwAccel;
}

void __stdcall PathClearSteps(PTPATH ptPath)
{
	ptPath->dwNumSteps = 0;
	ptPath->dwFlags &= ~32;
}

void __stdcall PathSetMoving(PTPATH ptPath, BOOL fSet)
{
	if (fSet)
		ptPath->dwFlags |= 32;
	else
		ptPath->dwFlags &= ~32;
}

DWORD __stdcall PathTestMoving(PTPATH ptPath)
{
	return ptPath->dwFlags & 32;
}

DWORD __stdcall PathGetStepCount(PTPATH ptPath)
{
	return ptPath->dwNumSteps;
}

DWORD __stdcall PathGetStepIndex(PTPATH ptPath)
{
	return ptPath->dwStepIndex;
}

DWORD __stdcall PathGetSteps(PTPATH ptPath, DWORD **ppSteps)
{
	*ppSteps = &ptPath->adwSteps[1];
	return ptPath->dwNumSteps;
}

BYTE __stdcall PathGetFacing(PTPATH ptPath)
{
	return ptPath->bFacing;
}

BYTE __stdcall PathGetTargetFacing(PTPATH ptPath)
{
	return ptPath->bTargetFacing;
}

void __stdcall PathSetFacingSmoothed(PTPATH ptPath, BYTE bDir)
{
	BYTE bDelta;

#line 1482
	D2ASSERT(bDir < 64);
	bDir &= 63;
	switch (UnitGetType(ptPath->hUnit))
	{
	case UNIT_OBJECT:
	case UNIT_ITEM:
		ptPath->bFacing = bDir;
		return;
	case UNIT_MISSILE:
		if (ptPath->dwFlags & 64)
			return;
		ptPath->bFacing = bDir;
		return;
	}

	if (bDir == ptPath->bTargetFacing)
		return;

	bDelta = (BYTE)((bDir - ptPath->bFacing) & 63);
	ptPath->bTargetFacing = bDir;
	ptPath->bTurnRate = (BYTE)sgcadwPathTurnRate[bDelta];
}

void __stdcall PathSetFacingImmediate(PTPATH ptPath, BYTE bDir)
{
#line 1494
	D2ASSERT(bDir < 64);

	ptPath->bFacing = bDir;
	ptPath->bTargetFacing = bDir;
}

DWORD __stdcall PathGetGameX(PTPATH ptPath)
{
	return ptPath->tGameCoord.dwX >> 16;
}

DWORD __stdcall PathGetGameY(PTPATH ptPath)
{
	return ptPath->tGameCoord.dwY >> 16;
}

DWORD __stdcall PathGetRawX(PTPATH ptPath)
{
	return ptPath->tGameCoord.dwX;
}

DWORD __stdcall PathGetRawY(PTPATH ptPath)
{
	return ptPath->tGameCoord.dwY;
}

void __stdcall PathSetRawX(PTPATH ptPath, DWORD dwValue)
{
	ptPath->tGameCoord.dwX = dwValue;
}

void __stdcall PathSetRawY(PTPATH ptPath, DWORD dwValue)
{
	ptPath->tGameCoord.dwY = dwValue;
}

DWORD __stdcall PathGetScreenX(PTPATH ptPath)
{
	return ptPath->tRoomTileCoord.dwX;
}

DWORD __stdcall PathGetScreenY(PTPATH ptPath)
{
	return ptPath->tRoomTileCoord.dwY;
}

void __fastcall PathSetScreenX(PTPATH ptPath, DWORD dwValue)
{
	ptPath->tRoomTileCoord.dwX = dwValue;
}

void __fastcall PathSetScreenY(PTPATH ptPath, DWORD dwValue)
{
	ptPath->tRoomTileCoord.dwY = dwValue;
}

DWORD __stdcall PathGetTargetX(PTPATH ptPath)
{
	return ptPath->tTargetCoord.wX;
}

DWORD __stdcall PathGetTargetY(PTPATH ptPath)
{
	return ptPath->tTargetCoord.wY;
}

DWORD __stdcall PathGetEndX(PTPATH ptPath)
{
	return ptPath->tEndCoord.wX;
}

DWORD __stdcall PathGetEndY(PTPATH ptPath)
{
	return ptPath->tEndCoord.wY;
}

DWORD __stdcall PathGetStepX(PTPATH ptPath)
{
	if (ptPath->dwNumSteps)
		return (WORD)ptPath->adwSteps[ptPath->dwNumSteps];
	return 0;
}

DWORD __stdcall PathGetStepY(PTPATH ptPath)
{
	if (!(ptPath->dwNumSteps == 0)) {
		return (DWORD)((WORD *)&ptPath->adwSteps[ptPath->dwNumSteps])[1]; }
	return (DWORD)0;
}

DWORD __stdcall PathGetRoom(PTPATH ptPath)
{
	return ptPath->dwRoom;
}

void __stdcall PathSetRoom(PTPATH ptPath, DWORD hRoom)
{
	ptPath->dwRoom = hRoom;
}

DWORD __stdcall PathGetPrevRoom(PTPATH ptPath)
{
	return ptPath->field_20;
}

void __stdcall PathClearPrevRoom(PTPATH ptPath)
{
	ptPath->field_20 = 0;
}

void __stdcall PathSetTargetCoord(PTPATH ptPath, WORD wX, WORD wY)
{
	ptPath->tTargetCoord.wX = wX;
	ptPath->tTargetCoord.wY = wY;
	ptPath->hTarget = NULL;
}

void __stdcall PathSetTargetFromScreen(PTPATH ptPath, int nX, int nY)
{
	D2CommonEnvScreenToTilePairRaw(&nX, &nY);
	nX >>= 11;
	nY >>= 11;
	ptPath->tTargetCoord.wX = (WORD)nX;
	ptPath->tTargetCoord.wY = (WORD)nY;
}

DWORD __stdcall PathTestLeftRoom(PTPATH ptPath)
{
	return ptPath->dwFlags & 2;
}

void __stdcall PathSetLeftRoom(PTPATH ptPath, BOOL fSet)
{
	if (fSet)
		ptPath->dwFlags |= 2;
	else
		ptPath->dwFlags &= ~2;
}

void __stdcall PathSetCollisionTracking(PTPATH ptPath, BOOL fSet)
{
	if (fSet)
		ptPath->dwFlags |= 4;
	else
		ptPath->dwFlags &= ~4;
}

DWORD __stdcall PathTestCollisionTracking(PTPATH ptPath)
{
	return ptPath->dwFlags & 4;
}

void __stdcall PathSetTarget(PTPATH ptPath, HUNIT hTarget)
{
	ptPath->hTarget = hTarget;
	if (hTarget != NULL)
	{
		ptPath->dwTargetType = *(DWORD *)hTarget;
		ptPath->dwTargetId = *(DWORD *)((BYTE *)hTarget + 8);
	}
}

void __stdcall PathGetTargetSnapshot(PTPATH ptPath, DWORD *pdwTargetType, DWORD *pdwTargetId)
{
#line 1796
	D2ASSERT(ptPath->hTarget);

	*pdwTargetType = ptPath->dwTargetType;
	*pdwTargetId = ptPath->dwTargetId;
}

DWORD __stdcall PathGetTarget(PTPATH ptPath)
{
	return (DWORD)ptPath->hTarget;
}

DWORD __stdcall PathGetCollisionMask(PTPATH ptPath)
{
	return ptPath->dwCollisionMask;
}

void __stdcall PathSetCollisionMask(PTPATH ptPath, DWORD dwValue)
{
	struct TARGETCOORD tSquare;

	tSquare.wX = ptPath->tGameCoord.dwX >> 16;
	tSquare.wY = ptPath->tGameCoord.dwY >> 16;
	if (ptPath->dwRoom != 0)
	{
		if (ptPath->hUnit->eType == 3)
			D2CommonCollisnClearSize(ptPath->dwRoom, tSquare.wX, tSquare.wY, ptPath->dwUnitSize, ptPath->dwCollisionMask);
		else
			D2CommonCollisnClearPattern(ptPath->dwRoom, tSquare.wX, tSquare.wY, ptPath->dwCollisionPattern,
				ptPath->dwCollisionMask);
	}
	ptPath->dwCollisionMask = dwValue;

	if (ptPath->dwRoom != 0)
	{
		if (ptPath->hUnit->eType == 3)
			D2CommonCollisnSetSize(ptPath->dwRoom, tSquare.wX, tSquare.wY, ptPath->dwUnitSize, dwValue);
		else
			D2CommonCollisnSetPattern(ptPath->dwRoom, tSquare.wX, tSquare.wY, ptPath->dwCollisionPattern, dwValue);
	}
}

DWORD __stdcall PathGetCollisionTestMask(PTPATH ptPath)
{
	return ptPath->dwCollisionTestMask;
}

void __stdcall PathSetCollisionTestMask(PTPATH ptPath, DWORD dwValue)
{
	ptPath->dwCollisionTestMask = dwValue;
}

void __stdcall PathSetType(PTPATH ptPath, DWORD eType)
{
	DWORD dwTypeFlags;

#line 1952
	D2ASSERT(!(UnitGetType(ptPath->hUnit) == UNIT_PLAYER && eType == PATH_TYPE_TOWARD));

	dwTypeFlags = sgcadwPathTypeFlags[eType];

	if ((dwTypeFlags & 8192) && !(ptPath->dwFlags & 16384))
		ptPath->ePrevType = ptPath->eType;
	if ((dwTypeFlags & 32768) && !(ptPath->dwFlags & 65536))
		ptPath->dwSavedVelocity = ptPath->dwVelocity;

	ptPath->eType = eType;
	ptPath->dwFlags = (ptPath->dwFlags & 0xFFF800FF) | dwTypeFlags;
	ptPath->adwSteps[0] = sgcadwPathTypeInit[eType];

#line 1968
	D2ASSERT(ptPath->ePrevType != PATH_TYPE_KNOCKBACK_CLIENT);

#line 1969
	D2ASSERT(ptPath->ePrevType != PATH_TYPE_KNOCKBACK_SERVER);

#line 1970
	D2ASSERT((ptPath->eType != PATH_TYPE_MISSILE) || ( ptPath->bDistMax < MAXPATHLEN ));
}

void __stdcall PathRestoreType(PTPATH ptPath)
{
	HUNIT hUnit;

	hUnit = ptPath->hUnit;
#line 1981
	D2ASSERT(hUnit);

	if (ptPath->dwFlags & 32768)
		ptPath->dwVelocity = ptPath->dwSavedVelocity;

	if (UnitGetType(hUnit) == UNIT_PLAYER)
	{
		PathSetType(ptPath, 7);
		return;
	}

	if (ptPath->dwFlags & 8192)
		PathSetType(ptPath, ptPath->ePrevType);
}

DWORD __stdcall PathGetType(PTPATH ptPath)
{
	return ptPath->eType;
}

void __stdcall PathSetSquareCountdown(PTPATH ptPath, BYTE bValue)
{
	ptPath->bDistLeft = bValue;
}

BYTE __stdcall PathGetSquareCountdown(PTPATH ptPath)
{
	return ptPath->bDistLeft;
}

void __stdcall PathSetDistMax(PTPATH ptPath, BYTE bDist)
{
#line 2032
	D2ASSERT((ptPath->eType == PATH_TYPE_MISSILE) || (bDist < MAXPATHLEN));

	ptPath->bDistMax = bDist;
	ptPath->bDistLeft = bDist;
}

BYTE __stdcall PathGetDistMax(PTPATH ptPath)
{
	return ptPath->bDistMax;
}

WORD __stdcall PathGetLineOfSight(PTPATH ptPath)
{
	if (ptPath->dwVelocity == 0)
	{
		ptPath->wLastCollision = D2CommonCollisnTestSize(ptPath->dwRoom, ptPath->tGameCoord.dwX >> 16,
			ptPath->tGameCoord.dwY >> 16, ptPath->dwUnitSize, 32767);
	}
	return ptPath->wLastCollision;
}

WORD __stdcall PathGetCachedCollision(PTPATH ptPath)
{
	return ptPath->wLastCollision;
}

void __stdcall PathSetSearchReach(PTPATH ptPath, BYTE bValue)
{
#line 2080
	D2ASSERT(ptPath->eType == PATH_TYPE_IDASTAR);

	ptPath->bSearchReach = bValue;
}

DWORD __stdcall PathGetSavedSteps(PTPATH ptPath, struct TARGETCOORD **ppSteps2)
{
#line 2092
	D2ASSERT(PATH_TEST_FLAG(ptPath, PATH_SAVE_STEPS_MASK ));

	*ppSteps2 = &ptPath->atSavedSteps[0];
	return ptPath->nSteps;
}

DWORD __stdcall PathGetSavedStepX(PTPATH ptPath)
{
	DWORD nCount;

#line 2106
	D2ASSERT(PATH_TEST_FLAG(ptPath, PATH_SAVE_STEPS_MASK ));

	nCount = ptPath->nSteps;
	if (nCount)
		return ptPath->atSavedSteps[nCount - 1].wX;
	return 0;
}

DWORD __stdcall PathGetSavedStepY(PTPATH ptPath)
{
	DWORD nCount;

#line 2121
	D2ASSERT(PATH_TEST_FLAG(ptPath, PATH_SAVE_STEPS_MASK ));

	nCount = ptPath->nSteps;
	if (!(nCount == 0)) {
		return ptPath->atSavedSteps[nCount - 1].wY; }
	return 0;
}

void __stdcall PathSetFacingFree(PTPATH ptPath, BOOL fFree)
{
	if (!fFree)
		ptPath->dwFlags |= 64;
	else
		ptPath->dwFlags &= ~64;
}

void __stdcall PathClearKeptTarget(PTPATH ptPath)
{
	ptPath->wKeptTargetX = 0;
	ptPath->wKeptTargetY = 0;
}

void __stdcall PathSetStopDistance(PTPATH ptPath, BYTE bValue)
{
	if (bValue > 0)
		ptPath->bStopDist = (BYTE)(bValue - 1);
	else
		ptPath->bStopDist = 0;
}

BYTE __stdcall PathGetStopDistance(PTPATH ptPath)
{
	return (BYTE)(ptPath->bStopDist + 1);
}

void __stdcall PathSetTargetOffset(PTPATH ptPath, BYTE bValue67, BYTE bValue68)
{
	ptPath->field_67 = bValue67;
	ptPath->bTargetOffset = bValue68;
}

void __stdcall PathSetRecalcBudget(PTPATH ptPath, int nDist)
{
#line 2191
	D2ASSERT(nDist >= 0 && nDist <= 255);

	ptPath->bRecalcDist = (BYTE)nDist;
}

DWORD __stdcall PathGetRecalcBudget(PTPATH ptPath)
{
	return ptPath->bRecalcDist;
}

void __stdcall PathAddRecalcBudget(PTPATH ptPath, int nDelta)
{
	int nNew = (int)ptPath->bRecalcDist + nDelta;

	if (nNew < 0)
		ptPath->bRecalcDist = 0;
	else if (nNew > 255)
		ptPath->bRecalcDist = 255;
	else
		ptPath->bRecalcDist = (BYTE)nNew;
}

DWORD __stdcall PathGetCollisionPatternByUnit(HUNIT hUnit)
{
	return hUnit->ptPath->dwCollisionPattern;
}

void __stdcall PathSetCollisionPatternByUnit(HUNIT hUnit, DWORD dwValue)
{
	hUnit->ptPath->dwCollisionPattern = dwValue;
}

void __stdcall PathUnitSetPeriodicRecalc(HUNIT hUnit, BOOL fSet)
{
	PTPATH ptPath;

	ptPath = hUnit->ptPath;
#line 2252
	D2ASSERT(ptPath);

	if (fSet)
		hUnit->dwFlags |= 8192;
	else
		hUnit->dwFlags &= 0xFFFFDFFF;

	ptPath->dwRecalcCounter = 0;
}

void __stdcall PathUnitPeriodicRecalc(HUNIT hUnit)
{
	PTPATH ptPath;
	HUNIT hTarget;

	if (hUnit->dwFlags0 & 2097152)
		return;
	if (!(hUnit->dwFlags & 8192))
		return;

	ptPath = hUnit->ptPath;
#line 2271
	D2ASSERT(ptPath);

	ptPath->dwRecalcCounter++;
	if ((int)ptPath->dwRecalcCounter <= 1)
		return;

	ptPath->dwRecalcCounter = 0;
	if (D2CommonStepUnitTestAtTarget(hUnit))
		return;

	hTarget = hUnit->ptPath->hTarget;

	if (D2CommonStepUnitRecalcPath(hUnit, hTarget != NULL))
		D2CommonUnitSetMode(hUnit, 2);
}

int __stdcall PathDistanceSquared(int nX1, int nY1, int nX2, int nY2)
{
	int nDX = nX2 - nX1;
	int nDY = nY2 - nY1;

	return nDX * nDX + nDY * nDY;
}

void __stdcall PathFreeIfSet(HUNIT hUnit)
{
	if (hUnit != NULL)
		PathSetCollisionByType(hUnit);
}

BOOL __stdcall PathTestHasMoreSteps(HUNIT hUnit)
{
	PTPATH ptPath;

	if (hUnit == NULL)
		return FALSE;
	ptPath = hUnit->ptPath;
	if (ptPath == NULL)
		return FALSE;
	return (int)ptPath->dwStepIndex < (int)ptPath->dwNumSteps;
}
