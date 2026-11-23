#ifndef D2REALLOCPOOL
#define D2REALLOCPOOL(pPool, p, nSize)	((void *)FogErrorManagerPoolReallocRaw((DWORD)(pPool), (DWORD)(p), (DWORD)(nSize), (DWORD)D2_THIS_FILE, __LINE__, 0))
#endif
#ifndef LEVEL_ACT_1_TOWN
#define LEVEL_ACT_1_TOWN	1
#endif
#ifndef SKILL_HOLY_SHIELD
#define SKILL_HOLY_SHIELD	117
#endif
#ifndef MATCH_UNITSTHIS
#define MATCH_UNITSTHIS
struct UNITSTHIS
{
	void UnitInitPosition(PTUNIT ptUnit, DWORD dwRoom, DWORD dwTileX, DWORD dwTileY);
	DWORD UnitGetTargetType(PTUNIT ptUnit);
	DWORD UnitGetTargetId(PTUNIT ptUnit);
};
#endif
