#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\MissileTbls.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

PTMISSILESTXT gptMissilesTxt;

PTMISSILESTXT __fastcall MissileTblsLoad(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Vel", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Vel) },
		{ "MaxVel", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, MaxVel) },
		{ "Accel", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, Accel) },
		{ "Range", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, Range) },
		{ "LevRange", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, LevRange) },
		{ "Light", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Light) },
		{ "Flicker", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Flicker) },
		{ "Red", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Red) },
		{ "Green", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Green) },
		{ "Blue", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Blue) },
		{ "InitSteps", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, InitSteps) },
		{ "Activate", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Activate) },
		{ "LoopAnim", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, LoopAnim) },
		{ "CelFile", EXCEL_TYPE_STRING, 64, offsetof(MISSILESTXT, CelFile) },
		{ "AnimLen", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, AnimLen) },
		{ "RandStart", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, RandStart) },
		{ "SubLoop", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, SubLoop) },
		{ "SubStart", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, SubStart) },
		{ "SubStop", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, SubStop) },
		{ "CollideType", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, CollideType) },
		{ "CollideKill", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, CollideKill) },
		{ "CollideFriend", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, CollideFriend) },
		{ "LastCollide", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, LastCollide) },
		{ "Collision", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Collision) },
		{ "ClientSend", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, ClientSend) },
		{ "NextHit", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, NextHit) },
		{ "NextDelay", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, NextDelay) },
		{ "Size", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Size) },
		{ "CanDestroy", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, CanDestroy) },
		{ "ToHit", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, ToHit) },
		{ "AlwaysExplode", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, AlwaysExplode) },
		{ "Explosion", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Explosion) },
		{ "NeverDup", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, NeverDup) },
		{ "ReturnFire", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, ReturnFire) },
		{ "GetHit", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, GetHit) },
		{ "KnockBack", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, KnockBack) },
		{ "Trans", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Trans) },
		{ "Qty", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Qty) },
		{ "Pierce", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Pierce) },
		{ "Param1", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, Param1) },
		{ "Param2", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, Param2) },
		{ "Open", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Open) },
		{ "Beta", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, Beta) },
		{ "SpecialSetup", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, SpecialSetup) },
		{ "Skill", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, Skill) },
		{ "HitShift", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, HitShift) },
		{ "SrcDamage", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, SrcDamage) },
		{ "MinDamage", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, nMinDamage) },
		{ "MaxDamage", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, nMaxDamage) },
		{ "LevDamage", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, LevDamage) },
		{ "EType", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, EType) },
		{ "EMin", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, EMin) },
		{ "EMax", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, EMax) },
		{ "ELevel", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, ELevel) },
		{ "ELen", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, ELen) },
		{ "ELevelLen", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, ELevelLen) },
		{ "HitClass", EXCEL_TYPE_DWORD, 0, offsetof(MISSILESTXT, HitClass) },
		{ "NumDirections", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, NumDirections) },
		{ "LocalBlood", EXCEL_TYPE_BYTE, 0, offsetof(MISSILESTXT, LocalBlood) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;

	gptMissilesTxt = (PTMISSILESTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(MISSILESTXT));
	return gptMissilesTxt;
}

void __fastcall MissileTblsFree(void)
{
	DataTblsFreeBlock(gptMissilesTxt);
}

PTMISSILESTXT __stdcall MissileTblsGetMissile(int eMissile)
{
#line 111
	D2ASSERT(eMissile < NUM_MISSILE_CLASSES);
	return &gptMissilesTxt[eMissile];
}

int __stdcall MissileTblsGetVel(int eMissile)
{
	PTMISSILESTXT ptMissile;

	ptMissile = &gptMissilesTxt[eMissile];
#line 122
	D2ASSERT(eMissile < NUM_MISSILE_CLASSES);
	return ptMissile->Vel;
}
