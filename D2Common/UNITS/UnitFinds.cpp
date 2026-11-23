#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\UNITS\\UnitFinds.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "UnitsLocal.h"

struct UNITFINDSRECORD
{
 	void			*pPool;
 	void			*phUnitsArray;
};
typedef struct UNITFINDSRECORD UNITFINDSRECORD, *PTUNITFINDSRECORD;

void __stdcall UnitFindsNothingFirst(DWORD dwUnused1, DWORD dwUnused2, DWORD dwUnused3, DWORD dwUnused4)
{
}

void __stdcall UnitFindsNothingSecond(DWORD dwUnused1, DWORD dwUnused2, DWORD dwUnused3, DWORD dwUnused4)
{
}

#define UNIT_FIND_ARRAY_SIZE	15

typedef BOOL (__fastcall *PFNUNITTEST)(PTUNIT, DWORD);

struct UNITFINDARGS;

typedef BOOL (__fastcall *PFNUNITFIND)(PTUNIT, struct UNITFINDARGS *);

struct UNITFINDARGS
{
 	DWORD		dwFlags;
 	BYTE		unobserved_4[4];
 	PTUNIT		hExcluded;
 	int			nX;
 	int			nY;
 	int			nDistance;
 	int			nMaxMatches;
 	int			nMatches;
 	DWORD		*pFieldPair;
 	PFNUNITFIND	pfnFind;
};
typedef struct UNITFINDARGS UNITFINDARGS, *PTUNITFINDARGS;

struct UNITFINDDATA
{
 	void		*pPool;
 	PTUNIT		*phUnitsArray;
 	PTDUNGEONROOM	hRoom;
 	PFNUNITTEST	pfnUnitTest;
 	DWORD		dwData;
 	DWORD		dwFlags;
 	DWORD		nNumUnits;
 	DWORD		nMaxUnits;
 	int			nX;
 	int			nY;
 	int			nRadius;
};
typedef struct UNITFINDDATA UNITFINDDATA, *PTUNITFINDDATA;

BOOL __stdcall UnitFindsTestRoomsNear(PTUNIT hDestUnit, PTUNIT hSrcUnit)
{
	PTDUNGEONROOM hSrcRoom, hDestRoom;
	PTDUNGEONROOM *pppNear;
	DWORD dwNumNear;
	DWORD i;

#line 24
	D2ASSERT(hDestUnit);
#line 25
	D2ASSERT(hSrcUnit);

	hSrcRoom = (PTDUNGEONROOM)D2CommonUnitGetPathRoom(hSrcUnit);
	if (hSrcRoom == NULL)
		return FALSE;
	hDestRoom = (PTDUNGEONROOM)D2CommonUnitGetPathRoom(hDestUnit);
	if (hDestRoom == NULL)
		return FALSE;

	D2CommonDungeonRoomGetNear(hSrcRoom, &pppNear, &dwNumNear);
	for (i = 0; (int)i < (int)dwNumNear; i++)
	{
		if (pppNear[i] == hDestRoom)
			return TRUE;
	}
	return FALSE;
}

PTUNIT __stdcall UnitFindsTestUnitNearRoom(PTDUNGEONROOM hRoom, DWORD dwUnitType, DWORD dwClassId)
{
	PTDUNGEONROOM *pppNear;
	DWORD dwNumNear;
	DWORD i;

#line 49
	D2ASSERT(hRoom);

	D2CommonDungeonRoomGetNear(hRoom, &pppNear, &dwNumNear);
	for (i = 0; (int)i < (int)dwNumNear; i++)
	{
		PTUNIT ptUnit;

		for (ptUnit = pppNear[i]->ptUnitFirst; ptUnit != NULL; ptUnit = ptUnit->ptRoomNext)
		{
			if (ptUnit->eType == dwUnitType && ptUnit->dwClassId == dwClassId)
				return ptUnit;
		}
	}
	return NULL;
}

DWORD __stdcall UnitFindsSearchRoom(PTDUNGEONROOM hRoom, PTUNIT *paUnits, PFNUNITTEST pfnUnitTest,
	DWORD dwContext)
{
	PTUNIT ptUnit;
	int nUnitIndex;

	ptUnit = hRoom->ptUnitFirst;
	nUnitIndex = 0;
#line 114
	D2ASSERTMSG(!IsBadCodePtr((FARPROC)pfnUnitTest), "pfnUnitTest");
	for (;;)
	{
		if (ptUnit == NULL)
			break;
		if (pfnUnitTest(ptUnit, dwContext))
		{
			paUnits[nUnitIndex] = ptUnit;
			nUnitIndex++;
			if (nUnitIndex >= UNIT_FIND_ARRAY_SIZE)
			{
#line 120
				D2ASSERT(nUnitIndex < UNIT_FIND_ARRAY_SIZE);
			}
		}
		ptUnit = ptUnit->ptRoomNext;
	}
	return nUnitIndex;
}

PTUNIT __stdcall UnitFindsFindNearestUnit(PTUNIT hUnit, int nX, int nY, int nMaxDist, PFNUNITTEST pfnUnitTest)
{
	PTDUNGEONROOM *pppNear;
	DWORD dwNumNear;
	PTUNIT ptBest;
	int nBestDist;
	int i;
	GAMERECT atCoords[2];
	PTUNIT ptUnit;
	int nDist;

	DWORD hRoom = D2CommonUnitGetPathRoom(hUnit);
	D2CommonDungeonRoomGetNear(hRoom, &pppNear, &dwNumNear);
	ptBest = NULL;
	nBestDist = 65535;
	if (nMaxDist == 0)
		nMaxDist = 65536;
#line 158
	D2ASSERTMSG(!IsBadCodePtr((FARPROC)pfnUnitTest), "pfnUnitTest");
	for (i = 0; i < (int)dwNumNear; i++)
	{
		D2CommonDungeonRoomCopyRects(pppNear[i], atCoords);
		if (nX + nMaxDist >= atCoords[0].nX)
		{
			if (nY + nMaxDist < atCoords[0].nY && nY - nMaxDist > atCoords[0].nY + atCoords[0].nHeight)
				continue;
		}
		else
		{
			if (nX - nMaxDist > atCoords[0].nX + atCoords[0].nWidth)
				continue;
			if (nY + nMaxDist < atCoords[0].nY && nY - nMaxDist > atCoords[0].nY + atCoords[0].nHeight)
				continue;
		}
		for (ptUnit = pppNear[i]->ptUnitFirst; ptUnit != NULL; ptUnit = ptUnit->ptRoomNext)
		{
			nDist = D2CommonUnitUtilGetDistance(ptUnit, nX, nY);
			if (nDist < nMaxDist && nDist < nBestDist && pfnUnitTest(ptUnit, (DWORD)hUnit))
			{
				ptBest = ptUnit;
				nBestDist = nDist;
			}
		}
	}
	return ptBest;
}

void __stdcall UnitFindsInit(void *pPool, PTUNITFINDDATA ufData, DWORD dwRoom, DWORD dwX, DWORD dwY,
	DWORD dwRadius, DWORD dwUnitTest, DWORD dwData)
{
#line 224
	D2ASSERT(ufData);
#line 226
	ufData->phUnitsArray = (PTUNIT *)D2ALLOCPOOL(pPool, UNIT_FIND_ARRAY_SIZE * sizeof(DWORD));
#line 227
	D2ASSERT(ufData->phUnitsArray);
	memset(ufData->phUnitsArray, 0, UNIT_FIND_ARRAY_SIZE * sizeof(DWORD));
	ufData->nNumUnits = 0;
	ufData->dwFlags = 0;
	ufData->hRoom = (PTDUNGEONROOM)dwRoom;
	ufData->nX = (int)dwX;
	ufData->nY = (int)dwY;
	ufData->pPool = pPool;
	ufData->nMaxUnits = UNIT_FIND_ARRAY_SIZE;
	ufData->nRadius = (int)dwRadius;
	ufData->pfnUnitTest = (PFNUNITTEST)dwUnitTest;
	ufData->dwData = dwData;
}

void __stdcall UnitFindsFreeArray(PTUNITFINDSRECORD ptRec)
{
	if (ptRec == NULL)
		return;
	if (ptRec->phUnitsArray == NULL)
		return;
#line 253
	D2FREEPOOL(ptRec->pPool, ptRec->phUnitsArray);
	ptRec->phUnitsArray = NULL;
}

static inline void UnitFindsGrowArray(PTUNITFINDDATA ufData)
{
	DWORD dwNewMax;

	dwNewMax = ufData->nMaxUnits + UNIT_FIND_ARRAY_SIZE;
#line 196
	ufData->phUnitsArray = (PTUNIT *)D2REALLOCPOOL(ufData->pPool, ufData->phUnitsArray, dwNewMax * sizeof(PTUNIT));
	ufData->nMaxUnits = dwNewMax;
}

BOOL __stdcall UnitFindsTestUnit(PTUNIT hUnit, PTUNITFINDARGS ptData);

DWORD __stdcall UnitFindsFindUnits(PTUNITFINDDATA ufData)
{
	GAMERECT atCoords[2];
	PTDUNGEONROOM *pppRooms;
	DWORD dwNumRooms;
	DWORD dwCount;
	DWORD i;
	PTUNIT ptUnit;
	PTUNIT ptNext;
	BOOL fMatch;
	int nX;
	int nY;
	int nRadius;

#line 264
	D2ASSERT(ufData);
	nRadius = ufData->nRadius;
	nY = ufData->nY;
	nX = ufData->nX;
	D2CommonDungeonRoomCopyRects(ufData->hRoom, atCoords);
	if (nX - nRadius > atCoords[0].nX && nY - nRadius > atCoords[0].nY &&
		nX + nRadius < atCoords[0].nX + atCoords[0].nWidth && nY + nRadius < atCoords[0].nY + atCoords[0].nHeight)
	{
		pppRooms = &ufData->hRoom;
		dwNumRooms = 1;
	}
	else
		D2CommonDungeonRoomGetNear(ufData->hRoom, &pppRooms, &dwNumRooms);

	dwCount = 0;
	for (i = 0; i < dwNumRooms; i++)
	{
		if ((ufData->dwFlags & 8192) && D2CommonDungeonRoomIsTown(pppRooms[i]))
			continue;
		nRadius = ufData->nRadius;
		nY = ufData->nY;
		nX = ufData->nX;
		D2CommonDungeonRoomCopyRects(pppRooms[i], atCoords);
		if (nX + nRadius >= atCoords[0].nX)
		{
			if (nY + nRadius < atCoords[0].nY && nY - nRadius > atCoords[0].nY + atCoords[0].nHeight)
				continue;
		}
		else
		{
			if (nX - nRadius > atCoords[0].nX + atCoords[0].nWidth)
				continue;
			if (nY + nRadius < atCoords[0].nY && nY - nRadius > atCoords[0].nY + atCoords[0].nHeight)
				continue;
		}
		for (ptUnit = pppRooms[i]->ptUnitFirst; ptUnit != NULL; ptUnit = ptNext)
		{
			ptNext = ptUnit->ptRoomNext;
			if (ufData->pfnUnitTest != NULL)
			{
#line 300
				D2ASSERTMSG(!IsBadCodePtr((FARPROC)ufData->pfnUnitTest), "ufData->pfnUnitTest");
				fMatch = ufData->pfnUnitTest(ptUnit, ufData->dwData);
			}
			else
				fMatch = UnitFindsTestUnit(ptUnit, (PTUNITFINDARGS)ufData->dwData);
			if (fMatch)
			{
				ufData->phUnitsArray[dwCount] = ptUnit;
				dwCount++;
				if (dwCount == ufData->nMaxUnits)
					UnitFindsGrowArray(ufData);
			}
		}
	}
	ufData->nNumUnits = dwCount;
	return dwCount;
}

BOOL __stdcall UnitFindsTestUnit(PTUNIT hUnit, PTUNITFINDARGS ptData)
{
	int nDX;
	int nDY;
	int nX;
	int nY;
	int nDist;
	PTMONSTATS ptMonStats;
	DWORD dwRoom;
	DWORD eType;
	DWORD dwMode;

	eType = hUnit->eType;
	nX = (int)D2CommonUnitGetX(hUnit);
	nY = (int)D2CommonUnitGetY(hUnit);
	nDX = nX - ptData->nX;
	nDY = nY - ptData->nY;
#line 342
	D2ASSERT(ptData);
	if ((ptData->dwFlags & 64) && ptData->nMatches >= ptData->nMaxMatches)
		return FALSE;
	if (nDX < 0)
		nDX = -nDX;
	if (nDY < 0)
		nDY = -nDY;
	if (nDY < nDX)
	{
		nDist = (nDX * 2 - nDY) >> 1;
		if (nDist >= ptData->nDistance)
			return FALSE;
	}
	else
	{
		nDist = (nDY * 2 - nDX) >> 1;
		if (nDist >= ptData->nDistance)
			return FALSE;
	}

	switch (eType)
	{
	case UNIT_PLAYER:
		if (!(ptData->dwFlags & 1))
			return FALSE;
		dwMode = hUnit->dwMode;
		if (ptData->dwFlags & 4096)
		{
			if (dwMode != 17)
				return FALSE;
		}
		else if (dwMode == 17 || dwMode == 0)
			return FALSE;
		if (hUnit == ptData->hExcluded)
			return FALSE;
		break;
	case UNIT_MONSTER:
		if (!(ptData->dwFlags & 2))
			return FALSE;
		dwMode = hUnit->dwMode;
		if (ptData->dwFlags & 4096)
		{
			if (dwMode != 12)
				return FALSE;
		}
		else if (dwMode == 12 || dwMode == 0)
			return FALSE;
		if (ptData->dwFlags & 4)
		{
			ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(hUnit->dwClassId);
			if (!ptMonStats->bLUndead && !ptMonStats->bHUndead)
				return FALSE;
		}
		break;
	case UNIT_OBJECT:
		if (!(ptData->dwFlags & 16))
			return FALSE;
		break;
	case UNIT_MISSILE:
		if (!(ptData->dwFlags & 8))
			return FALSE;
		if (((PTMISSILETBLREC)D2CommonMissileTblsGetMissile(hUnit->dwClassId))->Explosion)
			return FALSE;
		break;
	case UNIT_ITEM:
		if (!(ptData->dwFlags & 32))
			return FALSE;
		break;
	default:
		return FALSE;
	}

	if ((ptData->dwFlags & 128) && !(hUnit->dwFlags0 & 4))
		return FALSE;
	if ((ptData->dwFlags & 1024) && !(hUnit->dwFlags0 & 8))
		return FALSE;
	if ((ptData->dwFlags & 256) && D2CommonDungeonRoomIsTown(D2CommonUnitGetPathRoom(hUnit)))
		return FALSE;
	if (ptData->dwFlags & 512)
	{
		nX = (int)D2CommonUnitGetX(hUnit);
		nY = (int)D2CommonUnitGetY(hUnit);
		dwRoom = D2CommonUnitGetPathRoom(hUnit);
		if (dwRoom)
		{
			while (D2CommonFieldTblsStep(ptData->pFieldPair, &nX, &nY))
			{
				if (D2CommonCollisnTestPoint(dwRoom, nX, nY, 4) == 4)
					return FALSE;
			}
		}
	}
	if (ptData->dwFlags & 2048)
	{
#line 466
		D2ASSERTMSG(!IsBadCodePtr((FARPROC)ptData->pfnFind), "ptData->pfnFind");
		if (ptData->pfnFind(hUnit, ptData))
			return FALSE;
	}
	ptData->nMatches++;
	return TRUE;
}
