#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\COMPOSIT\\Composit.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "../DATATBLS/DataTbls.h"

#define NUM_COMPONENTS	16

struct COMPOSITMODETOKEN
{
	DWORD	dwToken;
	DWORD	nMode;
};
typedef struct COMPOSITMODETOKEN COMPOSITMODETOKEN;

static COMPOSITMODETOKEN sgtPlayerModeTokens[] =
{
	{ 538994791, 18 },
	{ 538994791, 19 },
};
static DWORD sgnPlayerModeTokens = arrsize(sgtPlayerModeTokens);

static COMPOSITMODETOKEN sgtMonsterModeTokens[] =
{
	{ 538994791, 13 },
};
static DWORD sgnMonsterModeTokens = arrsize(sgtMonsterModeTokens);

struct COMPOSITWEAPONCLASS
{
	DWORD	dwCode;
	DWORD	dwIndex;
};
typedef struct COMPOSITWEAPONCLASS COMPOSITWEAPONCLASS;

static COMPOSITWEAPONCLASS sgtWeaponClasses[] =
{
	{ 544698210, 1 },
	{ 544434225, 2 },
	{ 544499761, 3 },
	{ 543585395, 4 },
	{ 544434226, 5 },
	{ 544499762, 6 },
	{ 544694904, 7 },
	{ 544434737, 8 },
	{ 544500273, 9 },
	{ 544437041, 10 },
	{ 544502577, 11 },
};
static DWORD sgnWeaponClasses = arrsize(sgtWeaponClasses);

#define COMPOSIT_HTH	543716456

static __inline void COMPOSITTokenToString(char *psz, DWORD dwToken)
{
	BYTE *pb;

	pb = (BYTE *)&dwToken;
	psz[0] = pb[0] == ' ' ? 0 : pb[0];
	psz[1] = pb[1] == ' ' ? 0 : pb[1];
	psz[2] = pb[2] == ' ' ? 0 : pb[2];
	psz[3] = 0;
}

DWORD __stdcall COMPOSITGetWeaponClass(HUNIT hUnit, DWORD eTOU, DWORD nClass, DWORD nMode, DWORD dwInventory,
	DWORD *pdwIndex);

static __inline void COMPOSITFormatCofName(char *pszOut, DWORD dwClassToken, DWORD dwModeToken, DWORD *pdwWeaponClass,
	const char *pszDir, BOOL fPath, char *szWeapon, char *szClass, char *szMode)
{
	BYTE *pb;

	pb = (BYTE *)pdwWeaponClass;
	szWeapon[0] = pb[0] == ' ' ? 0 : pb[0];
	szWeapon[1] = pb[1] == ' ' ? 0 : pb[1];
	szWeapon[2] = pb[2] == ' ' ? 0 : pb[2];
	szWeapon[3] = 0;
	COMPOSITTokenToString(szClass, dwClassToken);
	COMPOSITTokenToString(szMode, dwModeToken);
	if (fPath)
		wsprintfA(pszOut, "%s\\%s\\COF\\%s%s%s.COF", pszDir, szClass, szClass, szMode, szWeapon);
	else
		wsprintfA(pszOut, "%s%s%s", szClass, szMode, szWeapon);
}

void __stdcall COMPOSITGetCofName(HUNIT hUnit, DWORD nClass, DWORD nMode, DWORD eTOU, DWORD dwInventory,
	char *pszOut, DWORD *pdwWeaponClass, BOOL fPath, BOOL fResolve)
{
	DWORD dwClassToken;
	DWORD dwModeToken;
	char szWeapon[4];
	char szClass[4];
	char szMode[4];
	DWORD dwObjectIndex;
	DWORD i;

	switch (eTOU)
	{
	case UNIT_PLAYER:
	{
		char szPlrWeapon[4];
		char szPlrClass[4];
		char szPlrMode[4];

		dwClassToken = ((PTTOKENTXT)D2CommonTokTblsGetPlayerToken(nClass, 0))->token;
		dwModeToken = ((PTTOKENTXT)D2CommonTokTblsGetPlayerToken(nMode, 1))->token;
		if (fResolve)
		{
			if (nMode == 0 || nMode == 17)
				*pdwWeaponClass = COMPOSIT_HTH;
			else
			{
				DWORD dwIndex;

				*pdwWeaponClass = COMPOSITGetWeaponClass(hUnit, UNIT_PLAYER, nClass, nMode, dwInventory,
					&dwIndex);
			}
		}
		for (i = 0; i < sgnPlayerModeTokens; i++)
		{
			if (nMode == sgtPlayerModeTokens[i].nMode)
				dwModeToken = sgtPlayerModeTokens[i].dwToken;
		}
		if (nMode == 11 && *pdwWeaponClass != 544434225 && *pdwWeaponClass != 544499761
			&& *pdwWeaponClass != 544434737 && *pdwWeaponClass != 544500273
			&& *pdwWeaponClass != 544437041 && *pdwWeaponClass != 544502577)
			*pdwWeaponClass = COMPOSIT_HTH;
		COMPOSITFormatCofName(pszOut, dwClassToken, dwModeToken, pdwWeaponClass, "DATA\\GLOBAL\\CHARS",
			fPath, szPlrWeapon, szPlrClass, szPlrMode);
		return;
	}

	case UNIT_MONSTER:
	{
		char szMonWeapon[4];
		char szMonClass[4];
		char szMonMode[4];
		PTTOKENTXT ptToken;

		ptToken = (PTTOKENTXT)D2CommonTokTblsGetMonsterToken(nClass, 0);
		dwClassToken = ptToken->token;
		dwModeToken = ((PTTOKENTXT)D2CommonTokTblsGetMonsterToken(nMode, 1))->token;
		if (nMode != 0 && nMode != 12)
		{
			DWORD dwIndex;

			*pdwWeaponClass = COMPOSITGetWeaponClass(hUnit, UNIT_MONSTER, nClass, nMode, dwInventory, &dwIndex);
		}
		else if (((PTMONSTATS)D2CommonMonsterTblsGetStats(nClass))->CompositeDeath)
		{
			DWORD dwIndex;

			*pdwWeaponClass = COMPOSITGetWeaponClass(hUnit, UNIT_MONSTER, nClass, nMode, dwInventory, &dwIndex);
		}
		else
			*pdwWeaponClass = COMPOSIT_HTH;
		for (i = 0; i < sgnMonsterModeTokens; i++)
		{
			if (nMode == sgtMonsterModeTokens[i].nMode)
				dwModeToken = sgtMonsterModeTokens[i].dwToken;
		}
		COMPOSITFormatCofName(pszOut, dwClassToken, dwModeToken, pdwWeaponClass, "DATA\\GLOBAL\\MONSTERS",
			fPath, szMonWeapon, szMonClass, szMonMode);
		return;
	}

	default:
#line 264
		D2ASSERT(0);
	case UNIT_OBJECT:
		dwClassToken = ((PTTOKENTXT)D2CommonTokTblsGetObjectToken(nClass, 0))->token;
		dwModeToken = ((PTTOKENTXT)D2CommonTokTblsGetObjectToken(nMode, 1))->token;
		*pdwWeaponClass = COMPOSITGetWeaponClass(hUnit, UNIT_OBJECT, nClass, nMode, dwInventory, &dwObjectIndex);
		break;
	}
	COMPOSITFormatCofName(pszOut, dwClassToken, dwModeToken, pdwWeaponClass, "DATA\\GLOBAL\\OBJECTS",
		fPath, szWeapon, szClass, szMode);
}

void __stdcall COMPOSITGetCofNameForUnit(PTUNIT hUnit, DWORD dwCofNameOut, DWORD dwWeaponClassOut,
	DWORD dwFullPath,
	DWORD dwResolve,
	DWORD dwInventory, DWORD dwMode)
{
	DWORD dwFlag, dwOut, dwIndex, dwParam3;
#line 280
	D2ASSERT(hUnit);
	dwFlag = hUnit->eType;
	dwOut = hUnit->dwClassId;
	dwIndex = D2CommonUnitGetAnimMode(hUnit);
	D2CommonSkillsClearFlagIfMatched(hUnit, &dwFlag, &dwOut, &dwIndex);
	dwParam3 = (dwMode != (DWORD)-1) ? dwMode : dwIndex;
	D2CommonCOMPOSITGetCofName(hUnit, dwOut, dwParam3, dwFlag, dwInventory, dwCofNameOut,
		dwWeaponClassOut, dwFullPath,
		dwResolve);
}

DWORD __stdcall COMPOSITGetWeaponClassIndex(DWORD dwCode)
{
	DWORD i;

	for (i = 0; i < sgnWeaponClasses; i++)
	{
		if (dwCode == sgtWeaponClasses[i].dwCode)
			return sgtWeaponClasses[i].dwIndex;
	}
	return 0;
}

DWORD __stdcall COMPOSITGetWeaponClass(HUNIT hUnit, DWORD eTOU, DWORD nClass, DWORD nMode, DWORD dwInventory,
	DWORD *pdwIndex)
{
	HUNIT hWeapon;
	BYTE bComponent;
	PTCHARSTATSTXT ptCharStats;
	PTMONSTATS ptMonStats;
	HUNIT hRight;
	HUNIT hLeft;
	HUNIT hPrimary;
	DWORD dwPrimary;
	DWORD dwOther;
	DWORD dwCode;

	bComponent = 0;
	switch (eTOU)
	{
	case UNIT_PLAYER:
		hWeapon = NULL;
		if (dwInventory != 0)
		{
			hWeapon = (HUNIT)D2CommonINVENTORYGetLocSlot(dwInventory, 4);
			if (hWeapon == NULL || !D2CommonITEMSTestEffectsActive(hWeapon)
				|| ((bComponent = ITEMSGetTypeComponent(hWeapon)) != 5 && bComponent != 6))
			{
				hWeapon = (HUNIT)D2CommonINVENTORYGetLocSlot(dwInventory, 5);
				if (hWeapon != NULL)
				{
					if (!D2CommonITEMSTestEffectsActive(hWeapon))
						hWeapon = NULL;
					else
						bComponent = ITEMSGetTypeComponent(hWeapon);
				}
			}
		}
		ptCharStats = (PTCHARSTATSTXT)D2CommonDataTblsGetCharStats(nClass);
		if (nMode != 0 && nMode != 17)
		{
			if (hWeapon != NULL)
			{
				if (bComponent != 5 && bComponent != 6)
				{
					*pdwIndex = COMPOSITGetWeaponClassIndex(ptCharStats->basewclass);
					return ptCharStats->basewclass;
				}
				hRight = (HUNIT)D2CommonINVENTORYGetLocSlot(dwInventory, 4);
				if (!D2CommonITEMSTestEffectsActive(hRight))
					hRight = NULL;
				hLeft = (HUNIT)D2CommonINVENTORYGetLocSlot(dwInventory, 5);
				if (!D2CommonITEMSTestEffectsActive(hLeft))
					hLeft = NULL;
				if (nClass == 4 && hRight != NULL && D2CommonITEMSIsWeapon(hRight) && hLeft != NULL
					&& D2CommonITEMSIsWeapon(hLeft))
				{
					hPrimary = (HUNIT)D2CommonINVENTORYFindEquippedMatch(dwInventory);
#line 425
					D2ASSERT(hPrimary);
					dwPrimary = D2CommonITEMSGetWClassIndexRaw((DWORD)hPrimary) & 65535;
					if (hRight == hPrimary)
						dwOther = D2CommonITEMSGetWClassIndexRaw((DWORD)hLeft) & 65535;
					else
						dwOther = D2CommonITEMSGetWClassIndexRaw((DWORD)hRight) & 65535;
					if (dwPrimary == 2 && dwOther == 2)
					{
						*pdwIndex = COMPOSITGetWeaponClassIndex(544437041);
						return 544437041;
					}
					if (dwPrimary == 2 && dwOther == 3)
					{
						*pdwIndex = COMPOSITGetWeaponClassIndex(544434737);
						return 544434737;
					}
					if (dwPrimary == 3 && dwOther == 3)
					{
						*pdwIndex = COMPOSITGetWeaponClassIndex(544500273);
						return 544500273;
					}
					if (dwPrimary == 3 && dwOther == 2)
					{
						*pdwIndex = COMPOSITGetWeaponClassIndex(544502577);
						return 544502577;
					}
#line 461
					D2FATAL("Unknown weapon class for Barbarian!");
				}
				if (D2CommonINVENTORYTestCanEquipSlot(hUnit, dwInventory) == 2)
					dwCode = D2CommonITEMSGetTypeTwoHandedWClass(hWeapon);
				else
					dwCode = D2CommonITEMSGetTypeWClass(hWeapon);
				*pdwIndex = COMPOSITGetWeaponClassIndex(dwCode);
				return dwCode;
			}
			*pdwIndex = COMPOSITGetWeaponClassIndex(ptCharStats->basewclass);
			return ptCharStats->basewclass;
		}
		*pdwIndex = COMPOSITGetWeaponClassIndex(ptCharStats->basewclass);
		return ptCharStats->basewclass;

	default:
#line 516
		D2ASSERT(0);
	case UNIT_OBJECT:
		*pdwIndex = COMPOSITGetWeaponClassIndex(COMPOSIT_HTH);
		return COMPOSIT_HTH;

	case UNIT_MONSTER:
		ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(nClass);
		if ((nMode == 0 || nMode == 12) && !ptMonStats->CompositeDeath)
		{
			*pdwIndex = COMPOSITGetWeaponClassIndex(COMPOSIT_HTH);
			return COMPOSIT_HTH;
		}
		*pdwIndex = COMPOSITGetWeaponClassIndex(ptMonStats->BaseW);
		return ptMonStats->BaseW;
	}
}

DWORD __stdcall COMPOSITGetUnitWeaponClass(PTUNIT hUnit, DWORD dwInventory, DWORD dwIndexOut, DWORD dwMode,
	DWORD dwTranslateMode)
{
	DWORD dwFlag, dwOut, dwModeOrig;

#line 529
	D2ASSERT(hUnit);
	dwFlag = hUnit->eType;
	dwOut = hUnit->dwClassId;
	if (dwMode == (DWORD)-1)
		dwMode = D2CommonUnitGetAnimMode(hUnit);
	dwModeOrig = dwMode;
	D2CommonSkillsClearFlagIfMatched(hUnit, &dwFlag, &dwOut, &dwMode);
	if (dwTranslateMode == 0)
		dwMode = dwModeOrig;
	return D2CommonCOMPOSITGetWeaponClass(hUnit, dwFlag, dwOut, dwMode, dwInventory, dwIndexOut);
}

BOOL __stdcall COMPOSITTestSlotMember(int nSlot)
{
	if (nSlot == 1 || nSlot == 2 || nSlot == 3 || nSlot == 4 || nSlot == 8 || nSlot == 9)
		return TRUE;
	return FALSE;
}

BOOL __stdcall COMPOSITIsRangedAttack(PTUNIT hUnit)
{
	DWORD dwFlag, dwOut, dwCategory, dwInventory, dwIndex;

	dwIndex = (DWORD)-1;
	dwInventory = hUnit->ptInventory;
#line 529
	D2ASSERT(hUnit);
	dwFlag = hUnit->eType;
	dwOut = hUnit->dwClassId;
	dwIndex = D2CommonUnitGetAnimMode(hUnit);
	D2CommonSkillsClearFlagIfMatched(hUnit, &dwFlag, &dwOut, &dwIndex);
	D2CommonCOMPOSITGetWeaponClass(hUnit, dwFlag, dwOut, dwIndex, dwInventory, &dwCategory);
	return dwCategory == 1 || dwCategory == 7;
}

DWORD __stdcall COMPOSITGetComponentByte(int eComponent, BYTE *pbRecord)
{
#line 583
	D2ASSERT(eComponent < NUM_COMPONENTS);
	switch (eComponent)
	{
	case 1:
		return pbRecord[2];
	case 2:
		return pbRecord[3];
	case 3:
		return pbRecord[0];
	case 4:
		return pbRecord[1];
	case 8:
		return pbRecord[4];
	case 9:
		return pbRecord[5];
	}
#line 598
	D2FATAL(NULL);
}
