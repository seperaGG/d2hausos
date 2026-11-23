#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\PATH\\PathUtil.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "PathLocal.h"
#include "../DRLG/Drlg.h"
#include "FogImports.h"

static int __fastcall sGetDirectionIndex(int nX1, int nY1, int nX2, int nY2);
static int __stdcall sGetSquareDirectionIndex(struct TARGETCOORD tFrom, struct TARGETCOORD tTo);

static const int sgcanDirections[25][3] =
{
	{ 5, 4, 6 }, { 4, 5, 6 }, { 4, 3, 5 }, { 4, 3, 2 }, { 3, 4, 2 },
	{ 6, 5, 4 }, { 5, 4, 6 }, { 4, 3, 5 }, { 3, 4, 2 }, { 2, 3, 4 },
	{ 6, 7, 5 }, { 6, 7, 5 }, { 6, 7, 5 }, { 2, 1, 3 }, { 2, 1, 3 },
	{ 6, 7, 0 }, { 7, 0, 6 }, { 0, 1, 7 }, { 1, 0, 2 }, { 2, 1, 0 },
	{ 7, 0, 6 }, { 0, 7, 6 }, { 0, 1, 7 }, { 0, 1, 2 }, { 1, 0, 2 },
};

static const BYTE sgcabSquareDirections[25][3] =
{
	{ 4, 6, 255 }, { 4, 6, 255 }, { 4, 2, 6 }, { 4, 2, 255 }, { 4, 2, 255 },
	{ 6, 4, 255 }, { 4, 6, 255 }, { 4, 2, 6 }, { 4, 2, 255 }, { 2, 4, 255 },
	{ 6, 0, 4 }, { 6, 0, 4 }, { 6, 0, 4 }, { 2, 0, 4 }, { 2, 0, 4 },
	{ 6, 0, 255 }, { 0, 6, 255 }, { 0, 2, 6 }, { 2, 0, 255 }, { 2, 0, 255 },
	{ 0, 6, 255 }, { 0, 6, 255 }, { 0, 2, 6 }, { 0, 2, 255 }, { 0, 2, 255 },
};

int __fastcall PathUtilGetStepDistance(int nX1, int nY1, int nX2, int nY2)
{
	int nAbsX;
	int nAbsY;
	int nMin;

	nAbsX = nX1 - nX2;
	if (nAbsX < 0)
		nAbsX = -nAbsX;
	nAbsY = nY1 - nY2;
	if (nAbsY < 0)
		nAbsY = -nAbsY;
	nMin = nAbsX < nAbsY ? nAbsX : nAbsY;
	if (nAbsX > nAbsY)
		return nMin + nAbsX * 2;
	return nMin + nAbsY * 2;
}

int __stdcall PathUtilGetSquareStepDistance(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	int nAbsX;
	int nAbsY;
	int nMin;

	nAbsY = (int)tTo.wX - (int)tFrom.wX;
	if (nAbsY < 0)
		nAbsY = -nAbsY;
	nAbsX = nAbsY;
	nAbsY = (int)tTo.wY - (int)tFrom.wY;
	if (nAbsY < 0)
		nAbsY = -nAbsY;
	nMin = nAbsX < nAbsY ? nAbsX : nAbsY;
	if (nAbsX > nAbsY)
		return nMin + nAbsX * 2;
	return nMin + nAbsY * 2;
}

int __fastcall PathUtilGetFirstTryDirection(int nX1, int nY1, int nX2, int nY2)
{
	return sgcabSquareDirections[sGetDirectionIndex(nX1, nY1, nX2, nY2)][0];
}

static int __fastcall sGetDirectionIndex(int nX1, int nY1, int nX2, int nY2)
{
	int nAbsY;
	int nAbsX;
	int nDY;
	int nDX;

	nAbsX = nX2 - nX1;
	nAbsY = nY2 - nY1; nDX = nAbsX; nDY = nAbsY;
	if (nAbsX < 0)
		nAbsX = -nAbsX;
	if (nAbsY < 0)
		nAbsY = -nAbsY;
	if (nAbsX >= nAbsY * 2)
		nDY = nDY < 0 ? -1 : nDY & 1;
	else if (nAbsY >= nAbsX * 2)
		nDX = nDX < 0 ? -1 : nDX & 1;
	if (nDX < -1)
		nDX = -2;
	else if (nDX > 1)
		nDX = 2;
	if (nDY < -1)
		nDY = -2;
	else if (nDY > 1)
		nDY = 2;
	return nDX * 5 + nDY + 12;
}

int __stdcall PathUtilGetSquareTryDirection(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	return sgcabSquareDirections[sGetSquareDirectionIndex(tFrom, tTo)][0];
}

static int __stdcall sGetSquareDirectionIndex(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	int nAbsY;
	int nAbsX;
	int nDY;
	int nDX;

	nAbsX = (int)tTo.wX - (int)tFrom.wX;
	nAbsY = (int)tTo.wY - (int)tFrom.wY;
	nDX = nAbsX; nDY = nAbsY;
	if (nAbsX < 0) nAbsX = -nAbsX; if (nAbsY < 0) nAbsY = -nAbsY;
	if (nAbsX >= nAbsY * 2)
		nDY = nDY < 0 ? -1 : nDY & 1;
	else if (nAbsY >= nAbsX * 2)
		nDX = nDX < 0 ? -1 : nDX & 1;
	if (nDX < -1)
		nDX = -2;
	else if (nDX > 1)
		nDX = 2;
	if (nDY < -1)
		nDY = -2;
	else if (nDY > 1)
		nDY = 2;
	return nDX * 5 + nDY + 12;
}

int __fastcall PathUtilGetDirection(int nX1, int nY1, int nX2, int nY2)
{
	return sgcanDirections[sGetDirectionIndex(nX1, nY1, nX2, nY2)][0];
}

int __stdcall PathUtilGetSquareDirection(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	return sgcanDirections[sGetSquareDirectionIndex(tFrom, tTo)][0];
}

void __fastcall PathUtilGetDirectionsXY(int nX1, int nY1, int nX2, int nY2, int *pnOut)
{
	int nIndex;

	nIndex = sGetDirectionIndex(nX1, nY1, nX2, nY2);
	pnOut[0] = sgcanDirections[nIndex][0];
	pnOut[1] = sgcanDirections[nIndex][1];
	pnOut[2] = sgcanDirections[nIndex][2];
}

void __fastcall PathUtilGetSquareDirections(struct TARGETCOORD tFrom, struct TARGETCOORD tTo, int *pnOut)
{
	int nIndex;

	nIndex = sGetSquareDirectionIndex(tFrom, tTo);
	pnOut[0] = sgcanDirections[nIndex][0];
	pnOut[1] = sgcanDirections[nIndex][1];
	pnOut[2] = sgcanDirections[nIndex][2];
}

void __fastcall PathUtilGetTryDirectionsXY(int nX1, int nY1, int nX2, int nY2, int *pnOut)
{
	int nIndex;

	nIndex = sGetDirectionIndex(nX1, nY1, nX2, nY2);
	pnOut[0] = sgcabSquareDirections[nIndex][0];
	pnOut[1] = sgcabSquareDirections[nIndex][1];
	pnOut[2] = sgcabSquareDirections[nIndex][2];
}

void __fastcall PathUtilGetDirections(struct TARGETCOORD tFrom, struct TARGETCOORD tTo, int *pnOut)
{
	int nIndex;

	nIndex = sGetSquareDirectionIndex(tFrom, tTo);
	pnOut[0] = sgcabSquareDirections[nIndex][0];
	pnOut[1] = sgcabSquareDirections[nIndex][1];
	pnOut[2] = sgcabSquareDirections[nIndex][2];
}

struct PATHLINE
{
 	struct TARGETCOORD	tFrom;
 	struct TARGETCOORD	tTo;
 	int			anStepVector[2];
 	int			nVelocityX;
 	int			nVelocityY;
 	int			nVelocity;
 	BYTE		bFacing;
 	BYTE		unobserved_1d[3];
 	DWORD		dwFlags;
};

void __fastcall StepCalcSquareVector(struct TARGETCOORD tFrom, struct TARGETCOORD tTo, int *pnVector,
	int *pnDir);

BOOL __fastcall PathUtilLineStart(struct PATHLINE *ptLine)
{
	int nDir;
	int nVX, nVY;
	int nDX, nDY;
	int nT;

	if (ptLine->tFrom.wX == ptLine->tTo.wX && ptLine->tFrom.wY == ptLine->tTo.wY)
	{
		ptLine->anStepVector[0] = 0;
		ptLine->anStepVector[1] = 0;
		ptLine->nVelocityX = 0;
		ptLine->nVelocityY = 0;
		ptLine->nVelocity = 0;
		return TRUE;
	}
	StepCalcSquareVector(ptLine->tFrom, ptLine->tTo, ptLine->anStepVector, &nDir);
	ptLine->bFacing = (BYTE)nDir;
	if (ptLine->dwFlags & 512)
		ptLine->bFacing = (BYTE)((BYTE)nDir - 32) & 63;
	ptLine->nVelocityX = ptLine->anStepVector[0] * ptLine->nVelocity >> 8;
	ptLine->nVelocityY = ptLine->anStepVector[1] * ptLine->nVelocity >> 8;
	nVX = ptLine->nVelocityX;
	if (nVX < 0)
		nVX = -nVX;
	nT = (int)ptLine->tFrom.wX - (int)ptLine->tTo.wX;
	if (nT < 0)
		nT = -nT;
	nDX = nT << 16;
	nVY = ptLine->nVelocityY;
	if (nVY < 0)
		nVY = -nVY;
	nT = (int)ptLine->tFrom.wY - (int)ptLine->tTo.wY;
	if (nT < 0)
		nT = -nT;
	nDY = nT;
	return nVX <= nDX
		|| nVY <= nDY << 16;
}

void __fastcall PathUtilClampStep(int *pnX, int *pnY)
{
	if (*pnX >= 0)
	{
		while (*pnX > 65536)
		{
			*pnX >>= 1;
			*pnY >>= 1;
		}
	}
	else
	{
		while (*pnX < -65536)
		{
			*pnX >>= 1;
			*pnY >>= 1;
		}
	}
	if (*pnY >= 0)
	{
		while (*pnY > 65536)
		{
			*pnX >>= 1;
			*pnY >>= 1;
		}
	}
	else
	{
		while (*pnY < -65536)
		{
			*pnX >>= 1;
			*pnY >>= 1;
		}
	}
}

BOOL __fastcall PathUtilLineIsClear(PTPATHINFO ptPathInfo, struct PATHLINE *ptLine)
{
	int nXV, nYV;
	int nAbsX, nAbsY;
	int nMax;
	int nDist;
	int nCurrX, nCurrY;
	int nEndX, nEndY;
	int nNextX, nNextY;
	GAMEROOM *hRoom;
	DWORD dwSize;
	DWORD dwPattern;

	if (!ptLine->nVelocity)
		return FALSE;
	PathUtilLineStart(ptLine);
	if (!ptLine->nVelocity)
		return FALSE;
	nXV = ptLine->nVelocityX;
	nYV = ptLine->nVelocityY;
	if (!nXV && !nYV)
		return FALSE;
	nCurrX = ((DWORD)ptLine->tFrom.wX << 16) + 32768;
	nCurrY = ((DWORD)ptLine->tFrom.wY << 16) + 32768;
	nEndX = ((DWORD)ptLine->tTo.wX << 16) + 32768;
	nEndY = ((DWORD)ptLine->tTo.wY << 16) + 32768;
	PathUtilClampStep(&nXV, &nYV);
	nAbsY = nYV < 0 ? -nYV : nYV;
	nAbsX = nXV < 0 ? -nXV : nXV;
	nMax = nAbsX > nAbsY ? nAbsX : nAbsY;
	hRoom = ptPathInfo->hRoomStart;
	dwSize = ptPathInfo->dwUnitSize;
	dwPattern = ptPathInfo->dwCollisionTestMask;
	for (;;)
	{
		nDist = nEndX - nCurrX;
		if ((nDist < 0 ? -nDist : nDist) < nMax)
		{
			nDist = nEndY - nCurrY;
			if ((nDist < 0 ? -nDist : nDist) < nMax)
				break;
		}
		nNextX = nCurrX + nXV;
		nNextY = nCurrY + nYV;
		if ((WORD)(nCurrX >> 16) != (WORD)(nNextX >> 16) || (WORD)(nCurrY >> 16) != (WORD)(nNextY >> 16))
		{
			if (D2CommonCollisnTestSize(hRoom, (WORD)(nNextX >> 16), (WORD)(nNextY >> 16), dwSize,
				dwPattern))
				return FALSE;
		}
		nCurrX = nNextX;
		nCurrY = nNextY;
	}
#line 479
	D2ASSERT(nCurrX != 0x80008000);
#line 480
	D2ASSERT(nCurrY != 0x80008000);
	return TRUE;
}

typedef float (__stdcall *PFNFOGANGLE)(DWORD nAngle);
#define FogCosine	((PFNFOGANGLE)FogD2QSSocketCosineRaw)
#define FogSine		((PFNFOGANGLE)FogD2QSSocketSineRaw)

void __fastcall PathUtilOffsetTarget(PTPATH ptPath)
{
	int nAngle;
	float fDistance;

#line 490
	D2ASSERT(ptPath);
#line 491
	D2ASSERT(ptPath->hTarget);
	nAngle = (int)D2CommonPathGetFacing(ptPath->hTarget->ptPath) << 3;
	fDistance = (float)(int)ptPath->bTargetOffset;
	nAngle = (nAngle - ((int)ptPath->bTargetOffset << 8)) & 511;
	ptPath->tTargetCoord.wX += (WORD)(int)((double)FogCosine(nAngle) * fDistance);
	ptPath->tTargetCoord.wY += (WORD)(int)((double)FogSine(nAngle) * fDistance);
}

void __fastcall PathUtilGetTargetCoord(struct TARGETCOORD *ptCoord, PTPATH ptPath)
{
	int nAngle;
	float fDistance;

#line 513
	D2ASSERT(ptPath);
#line 514
	D2ASSERT(ptPath->hTarget);
	ptCoord->wX = (WORD)D2CommonUnitGetX(ptPath->hTarget);
	ptCoord->wY = (WORD)D2CommonUnitGetY(ptPath->hTarget);
	if (ptPath->bTargetOffset != 0)
	{
		nAngle = (int)D2CommonPathGetFacing(ptPath->hTarget->ptPath) << 3;
		fDistance = (float)(int)ptPath->bTargetOffset;
		nAngle = (nAngle - ((int)ptPath->bTargetOffset << 8)) & 511;
		ptCoord->wX += (WORD)(int)((double)FogCosine(nAngle) * fDistance);
		ptCoord->wY += (WORD)(int)((double)FogSine(nAngle) * fDistance);
	}
}

static const int sgcanPathUtilNearDistance[8][8] =
{
	{ -1, -1, -1, 0, 2, 4, 6, 8 },
	{ -1, -1, 0, 1, 2, 4, 6, 8 },
	{ -1, 0, 0, 2, 3, 5, 7, 8 },
	{ 0, 1, 2, 2, 4, 5, 7, 8 },
	{ 2, 2, 3, 4, 5, 6, 7, 9 },
	{ 4, 4, 5, 5, 6, 7, 8, 9 },
	{ 6, 6, 7, 7, 7, 8, 10, 10 },
	{ 8, 8, 8, 8, 9, 9, 10, 11 },
};

int __stdcall PathUtilGetDistance(struct TARGETCOORD tFrom, struct TARGETCOORD tTo)
{
	int nDX;
	int nDY;
	int nDistance;

	nDY = (int)tFrom.wX - (int)tTo.wX;
	if (nDY < 0)
		nDY = -nDY;
	nDX = nDY;
	nDY = (int)tFrom.wY - (int)tTo.wY;
	if (nDY < 0)
		nDY = -nDY;
	if (nDX < 8 && nDY < 8)
	{
		nDistance = sgcanPathUtilNearDistance[nDY][nDX];
		if (nDistance < 0)
			return 0;
		return nDistance + 1;
	}
	if (nDX > nDY)
		return nDY + nDX * 2;
	return nDX + nDY * 2;
}
