#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\ENVIRONMENT\\Env.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include <math.h>
#include "D2Macros.h"
#include "../DRLG/Drlg.h"

void __stdcall EnvComputeTileBoundsPair(int nA, int nB, int *pnOutC, int *pnOutD)
{
	*pnOutC = 2 * nB + nA;
	*pnOutD = 2 * nB - nA;

	if (*pnOutC < 0)
		*pnOutC = *pnOutC / 32 - 1;
	else
		*pnOutC = *pnOutC / 32;

	if (*pnOutD < 0)
		*pnOutD = *pnOutD / 32 - 1;
	else
		*pnOutD = *pnOutD / 32;
}

void __stdcall EnvComputeScreenOriginPair(int nA, int nB, int *pnOutC, int *pnOutD)
{
	*pnOutC = (nA - nB) * 16;
	*pnOutD = (nA + nB) * 8;
	*pnOutC = *pnOutC - 16;
	*pnOutD = *pnOutD + 16;
}

void __stdcall EnvTileToSubtilePair(int nA, int nB, int *pnOutA, int *pnOutB)
{
	*pnOutA = nA * 5;
	*pnOutB = nB * 5;
}

void __stdcall EnvSubtileToTilePair(int nA, int nB, int *pnOutA, int *pnOutB)
{
	*pnOutA = nA / 5;
	if (nA < 0)
		*pnOutA -= 1;
	*pnOutB = nB / 5;
	if (nB < 0)
		*pnOutB -= 1;
}

void __stdcall EnvScreenToTilePair(int *pnA, int *pnB)
{
	int nA, nB;

	nA = *pnA;
	nB = *pnB;
	*pnA = (2 * nB + nA) >> 5;
	*pnB = (2 * nB - nA) >> 5;
}

void __stdcall EnvScreenToTilePairRaw(int *pnA, int *pnB)
{
	int nA, nB;

	nA = *pnA;
	nB = *pnB;
	*pnA = 2 * nB + nA;
	*pnB = 2 * nB - nA;
}

void __stdcall EnvTileToSubtile(int *pnA, int *pnB)
{
	*pnA *= 5;
	*pnB *= 5;
}

struct ENVCYCLE
{
	int			field_00;
	int			field_04;
	BYTE		field_08;
	BYTE		field_09;
	BYTE		field_0a;
	BYTE		field_0b;
};

static struct ENVCYCLE sgtEnvCycles[NUM_CYCLES] =
{
	{ 340, 3, 125, 144, 243, 0 }, { 350, 3, 208, 184, 131, 0 }, {   0, 0, 255, 255, 255, 0 },
	{ 180, 1, 255, 255, 255, 0 }, { 190, 1, 194, 152, 193, 0 }, { 200, 2, 125, 144, 243, 0 },
};
static struct ENVCYCLE sgtEnvCyclesAct4[NUM_CYCLES] =
{
	{ 340, 3, 243,  70, 243, 0 }, { 350, 3, 208, 184, 131, 0 }, {   0, 0, 255,  20,  20, 0 },
	{ 180, 1, 255, 255,  30, 0 }, { 190, 1,  20, 152, 193, 0 }, { 200, 2, 125, 144, 243, 0 },
};
static struct ENVCYCLE sgtEnvCyclesAlt[NUM_CYCLES] =
{
	{   0, 3,   0,  30, 243, 0 }, {  60, 0,  20, 230,  40, 0 }, { 120, 1,  20, 244, 244, 0 },
	{ 180, 2, 244,  24, 255, 0 }, { 240, 2, 255, 255,  70, 0 }, { 300, 2, 235, 244, 244, 0 },
};
static int sgnEnvCycleTimes[3] =
{
	128, 4, 8,
};

static void __fastcall EnvUpdateIntensity(PTENV ptEnv, int nLevelId, int nAct);
static void __fastcall EnvUpdateColor(PTENV ptEnv, int nAct);
static void __fastcall EnvResetAngle(PTENV ptEnv);

PTENV __fastcall EnvNew(void *pPool)
{
	PTENV ptEnv;

#line 228
	ptEnv = (PTENV)D2ALLOCPOOL(pPool, sizeof(ENV));
	memset(ptEnv, 0, sizeof(ENV));
	ptEnv->nCycleIndex = 2;
	ptEnv->nCycleTime = sgnEnvCycleTimes[ptEnv->nCycleTimeIndex];
	EnvResetAngle(ptEnv);
	EnvUpdateIntensity(ptEnv, LEVEL_ID_NONE, 0);
	EnvUpdateColor(ptEnv, 0);
	ptEnv->field_14 = 0;
	ptEnv->dwCreateTick = GetTickCount();
	ptEnv->fAltCycle = FALSE;
	return ptEnv;
}

static void __fastcall EnvUpdateIntensity(PTENV ptEnv, int nLevelId, int nAct)
{
	int nTarget;
	double dAngle;

	if (nAct == 3)
	{
		nTarget = 16;
		switch (nLevelId)
		{
		case 103:
			nTarget = 128;
			break;
		case 104:
			nTarget = 64;
			break;
		case 105:
			nTarget = 56;
			break;
		case 106:
			nTarget = 48;
			break;
		}
		if (ptEnv->nIntensity < nTarget)
			ptEnv->nIntensity++;
		if (ptEnv->nIntensity > nTarget)
			ptEnv->nIntensity--;
		if (!ptEnv->nIntensity)
			ptEnv->nIntensity = nTarget;
		return;
	}
	if (!ptEnv->fAltCycle)
	{
		dAngle = ((double)ptEnv->nAngle / ptEnv->nCycleTime * (1.0 / 180.0)) * (double)3.14159265f;
		if (ptEnv->nAngle < 180 * ptEnv->nCycleTime)
		{
			ptEnv->flSunX = (float)-cos(dAngle);
			ptEnv->flSunZ = (float)sin(dAngle);
		}
		else
		{
			ptEnv->flSunX = (float)-cos(dAngle);
			ptEnv->flSunZ = (float)(sin(dAngle) * 0.5);
		}
		ptEnv->flSunY = 0.0f;
		ptEnv->nIntensity = (int)(((ptEnv->flSunZ * 128.0f) + 128.0f) + 0.5);
		if (ptEnv->nIntensity < 0)
			ptEnv->nIntensity = 0;
		if (ptEnv->nIntensity > 255)
			ptEnv->nIntensity = 255;
		return;
	}
	if (ptEnv->nIntensity > 32)
		ptEnv->nIntensity -= 8;
	if (ptEnv->nIntensity < 32)
		ptEnv->nIntensity = 32;
}

static void __fastcall EnvUpdateColor(PTENV ptEnv, int nAct)
{
	const struct ENVCYCLE *ptCur;
	const struct ENVCYCLE *ptNext;
	int nNext;
	int nStart;
	int nEnd;
	int nRedDelta;
	int nGreenDelta;
	int nBlueDelta;
	double dFrac;
	double dSpan;
	double dNum;

	nNext = ptEnv->nCycleIndex + 1;
	if (nNext == NUM_CYCLES)
		nNext = 0;
	if (nAct == 3)
	{
		ptCur = &sgtEnvCyclesAct4[ptEnv->nCycleIndex];
		ptNext = &sgtEnvCyclesAct4[nNext];
	}
	else if (!ptEnv->fAltCycle)
	{
		ptCur = &sgtEnvCycles[ptEnv->nCycleIndex];
		ptNext = &sgtEnvCycles[nNext];
	}
	else
	{
		ptCur = &sgtEnvCyclesAlt[ptEnv->nCycleIndex];
		ptNext = &sgtEnvCyclesAlt[nNext];
	}
	nEnd = ptNext->field_00 * ptEnv->nCycleTime;
	if (!nEnd)
		nEnd = 360 * ptEnv->nCycleTime;
	nStart = ptCur->field_00 * ptEnv->nCycleTime;
	dNum = (double)(ptEnv->nAngle - nStart);
	dSpan = (double)(nEnd - nStart);
	dNum /= dSpan;
	dFrac = dNum;
	nGreenDelta = (int)((ptNext->field_09 - ptCur->field_09) * dFrac + 0.5);
	nBlueDelta = (int)((ptNext->field_0a - ptCur->field_0a) * dFrac + 0.5);
	nRedDelta = (int)((ptNext->field_08 - ptCur->field_08) * dFrac + 0.5);
	ptEnv->bRed = (BYTE)(nRedDelta + ptCur->field_08);
	ptEnv->bGreen = (BYTE)(ptCur->field_09 + nGreenDelta);
	ptEnv->bBlue = (BYTE)(ptCur->field_0a + nBlueDelta);
}

DWORD __fastcall EnvFree(void *pPool, PTENV ptEnv)
{
#line 249
	return D2FREEPOOL(pPool, ptEnv);
}

static __inline void sEnvAdvanceAngle(PTENV ptEnv, int nAct)
{
	const struct ENVCYCLE *ptCycle;
	int nNext;

	ptEnv->nAngle++;
	if (!ptEnv->fAltCycle)
	{
		if (nAct == 3)
			ptEnv->nAngle += 15;
		else if (sgtEnvCycles[ptEnv->nCycleIndex].field_04 == 2)
		{
			ptEnv->nAngle++;
			if (nAct == 2)
				ptEnv->nAngle += 8;
		}
	}
	if (ptEnv->nAngle >= 360 * ptEnv->nCycleTime)
		ptEnv->nAngle = 0;
	nNext = ptEnv->nCycleIndex + 1;
	if (nNext >= NUM_CYCLES)
		nNext = 0;
	if (!ptEnv->fAltCycle)
		ptCycle = &sgtEnvCycles[nNext];
	else if (nAct == 3)
		ptCycle = &sgtEnvCyclesAct4[nNext];
	else
		ptCycle = &sgtEnvCyclesAlt[nNext];
	if (ptEnv->nAngle == ptCycle->field_00 * ptEnv->nCycleTime)
	{
		if (++ptEnv->nCycleIndex >= NUM_CYCLES)
			ptEnv->nCycleIndex = 0;
		if (!ptEnv->fAltCycle)
			ptCycle = &sgtEnvCycles[ptEnv->nCycleIndex];
		else
			ptCycle = &sgtEnvCyclesAlt[ptEnv->nCycleIndex];
		ptEnv->nCycleValue = ptCycle->field_04;
		ptEnv->nAngle = ptCycle->field_00 * ptEnv->nCycleTime;
	}
}

BOOL __stdcall EnvTick(PTACT ptAct, struct DUNGEONROOM *ptRoom)
{
	PTENV ptEnv;
	int nOld;
	int nLevelId;
	int nAct;

	ptEnv = DungeonActGetEnv(ptAct);
	nLevelId = 0;
	nOld = ptEnv->nCycleValue;
	if (ptRoom)
		nLevelId = (int)DungeonRoomGetLevelId(ptRoom);
	nAct = DrlgGetActNum(nLevelId);
	sEnvAdvanceAngle(ptEnv, nAct);
	EnvUpdateIntensity(ptEnv, nLevelId, nAct);
	EnvUpdateColor(ptEnv, nAct);
	return nOld != ptEnv->nCycleValue;
}

void __stdcall EnvGetColor(PTACT ptAct, BYTE *pbRed, BYTE *pbGreen, BYTE *pbBlue)
{
	PTENV ptEnv;

	ptEnv = DungeonActGetEnv(ptAct);
	*pbRed = ptEnv->bRed;
	*pbGreen = ptEnv->bGreen;
	*pbBlue = ptEnv->bBlue;
}

int __stdcall EnvGetIntensity(PTACT ptAct)
{
	return DungeonActGetEnv(ptAct)->nIntensity;
}

int __stdcall EnvGetCycleValue(PTACT ptAct)
{
	return DungeonActGetEnv(ptAct)->nCycleValue;
}

DWORD __stdcall EnvGetZeroField(PTACT ptAct)
{
	return DungeonActGetEnv(ptAct)->field_14;
}

static void __fastcall EnvResetAngle(PTENV ptEnv)
{
	const struct ENVCYCLE *ptCycle;

	ptCycle = !ptEnv->fAltCycle ? &sgtEnvCycles[ptEnv->nCycleIndex] : &sgtEnvCyclesAlt[ptEnv->nCycleIndex];
	ptEnv->nCycleValue = ptCycle->field_04;
	ptEnv->nAngle = ptEnv->nCycleTime * ptCycle->field_00;
}

void __stdcall EnvNextCycle(PTACT ptAct, struct DUNGEONROOM *ptRoom)
{
	PTENV ptEnv;
	int nLevelId;
	int nAct;

	ptEnv = DungeonActGetEnv(ptAct);
	ptEnv->nCycleIndex++;
	if (ptEnv->nCycleIndex >= NUM_CYCLES)
		ptEnv->nCycleIndex = 0;
	EnvResetAngle(ptEnv);
	nLevelId = 0;
	if (ptRoom)
		nLevelId = (int)DungeonRoomGetLevelId(ptRoom);
	nAct = DrlgGetActNum(nLevelId);
	EnvUpdateIntensity(ptEnv, nLevelId, nAct);
	EnvUpdateColor(ptEnv, nAct);
}

int __stdcall EnvGetCycle(PTACT ptAct)
{
	return DungeonActGetEnv(ptAct)->nCycleIndex;
}

void __stdcall EnvSetCycle(PTACT ptAct, struct DUNGEONROOM *ptRoom, int nIndex, int nAngle, BOOL fAlt)
{
	PTENV ptEnv;
	const struct ENVCYCLE *ptCycle;
	int nLevelId;
	int nAct;

	ptEnv = DungeonActGetEnv(ptAct);
#line 407
	D2ASSERT(nIndex >= 0);
#line 408
	D2ASSERT(nIndex < NUM_CYCLES);
#line 409
	D2ASSERT(nAngle >= 0);
#line 410
	D2ASSERT(nAngle < ( 360 * ptEnv->nCycleTime ));
	ptEnv->nCycleIndex = nIndex;
	ptEnv->nAngle = nAngle;
	ptCycle = !fAlt ? &sgtEnvCycles[nIndex] : &sgtEnvCyclesAlt[nIndex];
	ptEnv->nCycleValue = ptCycle->field_04;
	nLevelId = 0;
	if (ptRoom)
		nLevelId = (int)DungeonRoomGetLevelId(ptRoom);
	nAct = DrlgGetActNum(nLevelId);
	EnvUpdateIntensity(ptEnv, nLevelId, nAct);
	ptEnv->fAltCycle = fAlt;
	if (fAlt)
	{
		EnvResetAngle(ptEnv);
		EnvUpdateIntensity(ptEnv, nLevelId, nAct);
		EnvUpdateColor(ptEnv, nAct);
	}
}

void __stdcall EnvGetState(PTACT ptAct, int *pnCycle, int *pnAngle, BOOL *pfAlt)
{
	PTENV ptEnv;

	ptEnv = DungeonActGetEnv(ptAct);
	*pnCycle = ptEnv->nCycleIndex;
	*pnAngle = ptEnv->nAngle;
	*pfAlt = ptEnv->fAltCycle;
}

void __stdcall EnvGetSun(PTACT ptAct, float *pfX, float *pfY, float *pfZ, DWORD *pdwZero)
{
	PTENV ptEnv;

	ptEnv = DungeonActGetEnv(ptAct);
	*pfX = ptEnv->flSunX;
	*pfY = ptEnv->flSunY;
	*pfZ = ptEnv->flSunZ;
	*pdwZero = 0;
}

int __stdcall EnvGetCycleTime(PTACT ptAct)
{
	return DungeonActGetEnv(ptAct)->nCycleTime;
}

void __stdcall EnvNextCycleTime(PTACT ptAct, struct DUNGEONROOM *ptRoom)
{
	PTENV ptEnv;
	int nLevelId;
	int nAct;

	ptEnv = DungeonActGetEnv(ptAct);
	ptEnv->nCycleTimeIndex++;
	if (ptEnv->nCycleTimeIndex >= 3)
		ptEnv->nCycleTimeIndex = 0;
	ptEnv->nCycleTime = sgnEnvCycleTimes[ptEnv->nCycleTimeIndex];
	EnvResetAngle(ptEnv);
	nLevelId = 0;
	if (ptRoom)
		nLevelId = (int)DungeonRoomGetLevelId(ptRoom);
	nAct = DrlgGetActNum(nLevelId);
	EnvUpdateIntensity(ptEnv, nLevelId, nAct);
	EnvUpdateColor(ptEnv, nAct);
}

void __stdcall EnvSetAltCycle(PTACT ptAct)
{
	PTENV ptEnv;

	ptEnv = DungeonActGetEnv(ptAct);
	ptEnv->nCycleTimeIndex = 1;
	ptEnv->nCycleTime = sgnEnvCycleTimes[1];
	EnvSetCycle(ptAct, NULL, 0, 0, TRUE);
}

void __stdcall EnvSetUsualCycle(PTACT ptAct)
{
	PTENV ptEnv;

	ptEnv = DungeonActGetEnv(ptAct);
	ptEnv->nCycleTimeIndex = 0;
	ptEnv->nCycleTime = sgnEnvCycleTimes[0];
	EnvSetCycle(ptAct, NULL, 2, 0, FALSE);
}
