#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\UNITS\\Missile.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

void __stdcall MissileCreateMissileData(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;
	int i;

	ptMissile = hMissile;
#line 23
	D2ASSERT(ptMissile);
#line 24
	D2ASSERTMSG(!ptMissile->uTypeData, "!ptMissile->uTypeData.hMissileData");
#line 25
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
#line 27
	ptMissileData = (PTMISSILEDATA)D2ALLOCPOOL((void *)ptMissile->dwMemPool, sizeof(MISSILEDATA));
#line 28
	D2ASSERT(ptMissileData);
	for (i = 0; i < (int)(sizeof(MISSILEDATA) / sizeof(DWORD)); i++)
		((DWORD *)ptMissileData)[i] = 0;
	ptMissileData->dwLastCollideType = 0;
	ptMissileData->dwLastCollideId = (DWORD)-1;
	ptMissile->uTypeData = (DWORD)ptMissileData;
}

void __stdcall MissileFreeMissileData(HUNIT hMissile)
{
	UNIT *ptUnit;
	PTMISSILEDATA ptMissileData;

	ptUnit = hMissile;
#line 45
	D2ASSERT(ptUnit);
#line 46
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptUnit->uTypeData;
	if (ptMissileData)
	{
		D2FREEPOOL((void *)ptUnit->dwMemPool, (void *)ptMissileData);
		ptUnit->uTypeData = 0;
	}
}

void __stdcall MissileSetLevel(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 64
	D2ASSERT(ptMissile);
#line 65
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	ptMissileData->dwLevel = dwValue;
}

DWORD __stdcall MissileGetLevel(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 79
	D2ASSERT(ptMissile);
#line 80
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	return ptMissileData->dwLevel;
}

void __stdcall MissileSetRange(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 93
	D2ASSERT(ptMissile);
#line 94
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	ptMissileData->dwRange = dwValue;
}

DWORD __stdcall MissileGetRange(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 108
	D2ASSERT(ptMissile);
#line 109
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	return ptMissileData->dwRange;
}

void __stdcall MissileSetRangeLeft(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 122
	D2ASSERT(ptMissile);
#line 123
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	ptMissileData->dwRangeLeft = dwValue;
}

DWORD __stdcall MissileGetRangeLeft(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 137
	D2ASSERT(ptMissile);
#line 138
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	return ptMissileData->dwRangeLeft;
}

DWORD __stdcall MissileCalcRangeUsed(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 151
	D2ASSERT(ptMissile);
#line 152
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
	return ptMissileData->dwRange - ptMissileData->dwRangeLeft;
}

void __stdcall MissileSetIndexedByte(HUNIT hMissile, DWORD dwIndex, BYTE bValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 167
	D2ASSERT(ptMissile);
#line 168
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 170
	D2ASSERT(ptMissileData);
	ptMissileData->field_30[dwIndex] = bValue;
}

DWORD __stdcall MissileGetIndexedByte(HUNIT hMissile, DWORD dwIndex)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 185
	D2ASSERT(ptMissile);
#line 186
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 188
	D2ASSERT(ptMissileData);
	return ptMissileData->field_30[dwIndex];
}

DWORD __stdcall MissileGetClassId(HUNIT hMissile)
{
	UNIT *ptMissile;

	ptMissile = hMissile;
#line 201
	D2ASSERT(ptMissile);
#line 202
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	return ptMissile->dwClassId;
}

void __stdcall MissileSetLastCollideUnit(HUNIT hMissile, HUNIT hUnit)
{
	UNIT *ptMissile;
	PTMISSILETBLREC ptStats;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 217
	D2ASSERT(ptMissile);
#line 218
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptStats = (PTMISSILETBLREC)D2CommonMissileTblsGetMissile(ptMissile->dwClassId);
#line 220
	D2ASSERT(ptStats);
	if (ptStats->LastCollide)
	{
		ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 226
		D2ASSERT(ptMissileData);
		ptMissileData->dwLastCollideType = hUnit->eType;
		ptMissileData->dwLastCollideId = hUnit->dwId;
	}
}

BOOL __stdcall MissileTestLastCollideUnit(HUNIT hMissile, HUNIT hUnit)
{
	UNIT *ptMissile;
	PTMISSILETBLREC ptStats;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 244
	D2ASSERT(ptMissile);
#line 245
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptStats = (PTMISSILETBLREC)D2CommonMissileTblsGetMissile(ptMissile->dwClassId);
#line 247
	D2ASSERT(ptStats);
	if (ptStats->LastCollide)
	{
		ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 253
		D2ASSERT(ptMissileData);
		if (ptMissileData->dwLastCollideType == hUnit->eType && ptMissileData->dwLastCollideId == hUnit->dwId)
			return TRUE;
	}
	return FALSE;
}

void __stdcall MissileSetSpawnClass(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 269
	D2ASSERT(ptMissile);
#line 270
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 272
	D2ASSERT(ptMissileData);
	ptMissileData->dwSpawnClass = dwValue;
}

DWORD __stdcall MissileGetSpawnClass(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 287
	D2ASSERT(ptMissile);
#line 288
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 290
	D2ASSERT(ptMissileData);
	return ptMissileData->dwSpawnClass;
}

void __stdcall MissileSetSpawnRadius(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 303
	D2ASSERT(ptMissile);
#line 304
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 306
	D2ASSERT(ptMissileData);
	ptMissileData->dwSpawnRadius = dwValue;
}

DWORD __stdcall MissileGetSpawnRadius(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 321
	D2ASSERT(ptMissile);
#line 322
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 324
	D2ASSERT(ptMissileData);
	return ptMissileData->dwSpawnRadius;
}

DWORD __stdcall MissileGetHitClass(HUNIT hMissile)
{
	PTMISSILETBLREC ptStats;
	DWORD dwClassId;

	dwClassId = hMissile->dwClassId;
	if (UnitGetType(hMissile) != UNIT_MISSILE)
		return 0;
	ptStats = (PTMISSILETBLREC)D2CommonMissileTblsGetMissile(dwClassId);
#line 342
	D2ASSERT(ptStats);
	return ptStats->HitClass;
}

void __stdcall MissileSetActivateAt(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 355
	D2ASSERT(ptMissile);
#line 356
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 358
	D2ASSERT(ptMissileData);
	ptMissileData->dwActivateAt = dwValue;
}

DWORD __stdcall MissileGetActivateAt(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 373
	D2ASSERT(ptMissile);
#line 374
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 376
	D2ASSERT(ptMissileData);
	return ptMissileData->dwActivateAt;
}

DWORD __stdcall MissileGetAnimRate(HUNIT hMissile)
{
	UNIT *ptMissile;

	ptMissile = hMissile;
#line 388
	D2ASSERT(ptMissile);
	return ptMissile->nAnimRate;
}

void __stdcall MissileSetAnimRate(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;

	ptMissile = hMissile;
#line 399
	D2ASSERT(ptMissile);
	ptMissile->nAnimRate = dwValue;
}

void __stdcall MissileSetScratchFirst(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 443
	D2ASSERT(ptMissile);
#line 444
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 446
	D2ASSERT(ptMissileData);
	ptMissileData->field_1c = dwValue;
}

DWORD __stdcall MissileGetScratchFirst(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 461
	D2ASSERT(ptMissile);
#line 462
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 464
	D2ASSERT(ptMissileData);
	return ptMissileData->field_1c;
}

void __stdcall MissileSetScratchSecond(HUNIT hMissile, DWORD dwValue)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 477
	D2ASSERT(ptMissile);
#line 478
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 480
	D2ASSERT(ptMissileData);
	ptMissileData->field_20 = dwValue;
}

DWORD __stdcall MissileGetScratchSecond(HUNIT hMissile)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 495
	D2ASSERT(ptMissile);
#line 496
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 498
	D2ASSERT(ptMissileData);
	return ptMissileData->field_20;
}

void __stdcall MissileSetStoredUnit(HUNIT hMissile, HUNIT hUnit)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 511
	D2ASSERT(ptMissile);
#line 512
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 514
	D2ASSERT(ptMissileData);
	if (hUnit)
	{
		ptMissileData->field_0c = hUnit->eType;
		ptMissileData->field_10 = hUnit->dwId;
	}
	else
	{
		ptMissileData->field_0c = 0;
		ptMissileData->field_10 = (DWORD)-1;
	}
}

void __stdcall MissileGetStoredUnit(HUNIT hMissile, DWORD *pdwField0C, DWORD *pdwField10)
{
	UNIT *ptMissile;
	PTMISSILEDATA ptMissileData;

	ptMissile = hMissile;
#line 536
	D2ASSERT(ptMissile);
#line 537
	D2ASSERT(UnitGetType (hMissile) == UNIT_MISSILE);
	ptMissileData = (PTMISSILEDATA)ptMissile->uTypeData;
#line 539
	D2ASSERT(ptMissileData);
	*pdwField0C = ptMissileData->field_0c;
	*pdwField10 = ptMissileData->field_10;
}

void __stdcall MissileNothingFirst(DWORD dwUnused1)
{
}

void __stdcall MissileNothingSecond(DWORD dwUnused1)
{
}
