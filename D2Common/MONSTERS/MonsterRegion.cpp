#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Monsters\\MonsterRegion.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "MonsterRegionLocal.h"

void __fastcall MonsterRegionCreatePopulation(void *pPool, PTMONPOPRECORD *ppArray, void *pRandom,
	DWORD dwParam2)
{
	PTMONPOPRECORD *ppSlot;
	int nLevelId;
	PTMONPOPRECORD ptRec;
	BYTE *pLvlEntry;
	int nRemaining;
	int i;
	PTMONPOPSLOT pSlot;
	BYTE abSlotFlag[25];

	D2CommonSeedSet(pRandom, dwParam2);

	ppSlot = ppArray + 1;
	for (nLevelId = 1; nLevelId < 109; nLevelId++)
	{
#line 80
		ptRec = (PTMONPOPRECORD)D2ALLOCPOOL(pPool, sizeof(MONPOPRECORD));
		for (i = 0; i < (int)(sizeof(MONPOPRECORD) / sizeof(DWORD)); i++)
			((DWORD *)ptRec)[i] = 0;

		pLvlEntry = (BYTE *)D2CommonLvlTblsGetLevelData(nLevelId);

		nRemaining = 3;
		ptRec->bAct = pLvlEntry[2];
		ptRec->dwLevelSlot = (DWORD)nLevelId;
		ptRec->dwMonDen = *(DWORD *)(pLvlEntry + 16);
		ptRec->bMonUMin = pLvlEntry[20];
		ptRec->bMonUMax = pLvlEntry[21];
		ptRec->bMonWndr = pLvlEntry[22];
		ptRec->bQuest = pLvlEntry[24];
		ptRec->field_134 = 0xFFFFFFFF;
		ptRec->dwRooms = 0xFFFFFFFF;

		if (pLvlEntry[25] < 3)
			nRemaining = pLvlEntry[25];

		pSlot = ptRec->tSlots;
		for (i = 0; i < (int)sizeof(abSlotFlag); i++)
			abSlotFlag[i] = 0;

		while (nRemaining > 0)
		{
			int nSlotIndex;
			DWORD dwCols;
			PTMONSTATS ptMonStats;

			dwCols = pLvlEntry[25];
			if (dwCols != 0)
				nSlotIndex = (int)D2CommonSeedRand(pRandom, dwCols);
			else
				nSlotIndex = 0;

			ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(*(DWORD *)(pLvlEntry + 28 + nSlotIndex * 4));

			if (!ptMonStats->Spawned)
			{
				abSlotFlag[nSlotIndex] = 2;
				nRemaining--;
			}
			if (abSlotFlag[nSlotIndex] == 0)
			{
				abSlotFlag[nSlotIndex] = 1;
				pSlot->dwWeight = *(DWORD *)(pLvlEntry + 128 + nSlotIndex * 4);
				pSlot->nClass = *(DWORD *)(pLvlEntry + 28 + nSlotIndex * 4);
				pSlot->bRarity = ptMonStats->Rarity;
				ptRec->bRarity = (BYTE)(ptRec->bRarity + ptMonStats->Rarity);
				ptRec->bNumSlots++;
				ptRec->bNumLevelSlots++;
				pSlot++;
				nRemaining--;
			}
		}

		{
			int nSlot;

			pSlot = ptRec->tSlots;
			for (nSlot = 0; nSlot < ptRec->bNumLevelSlots; pSlot++, nSlot++)
			{
				D2CommonMonsterRandRollVariations(pRandom, pSlot, 3);
			}
		}

		*ppSlot = ptRec;
		ppSlot++;
	}
}

void __fastcall MonsterRegionFreeLevelPopulation(void *pPool, PTMONPOPRECORD *ppArray)
{
	int i;

	ppArray++;
	for (i = 0; i < 108; i++)
	{
		if (*ppArray != NULL)
		{
#line 133
			D2FREEPOOL(pPool, *ppArray);
		}
		ppArray++;
	}
}

DWORD __fastcall MonsterRegionArrayGetDword(DWORD *padwArray, DWORD dwIndex)
{
	return padwArray[dwIndex];
}

PTMONPOPSLOT __fastcall MonsterRegionFindOrAddSlot(PTMONPOPRECORD *ppArray, DWORD dwEdxParam, BYTE *pRecord)
{
	DWORD dwId;
	PTMONPOPRECORD ptRec;
	PTMONPOPSLOT ptSlot;
	struct D2SEED *phRand;
	int i;
	int nCount;
	PTMONSTATS ptMonStats;

	dwId = *(DWORD *)(pRecord + 4);
	if (dwId == 195 || dwId == 196 || dwId == 294 || dwId == 296)
		return NULL;

	i = (int)D2CommonDungeonRoomGetLevelId(dwEdxParam);
	ptRec = ppArray[i];
	if (ptRec == NULL)
		return NULL;

	nCount = ptRec->bNumSlots;
	ptSlot = ptRec->tSlots;
	for (i = 0; i < nCount; ptSlot++, i++)
	{
		if (ptSlot->nClass == (int)dwId)
			return ptSlot;
	}
	if (i >= 13)
		return NULL;

	ptMonStats = (PTMONSTATS)D2CommonMonsterTblsGetStats(dwId);
	if (!(ptMonStats->TotalPieces <= 2)) {

		ptSlot->nClass = dwId;
		ptRec->bNumSlots++;
		phRand = (struct D2SEED *)(pRecord + 40);
		D2CommonMonsterRandRollVariations(phRand, ptSlot, 3);
		return ptSlot; }
	return ptSlot;
}
