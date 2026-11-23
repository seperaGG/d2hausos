#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\QuestRecord\\QuestRecord.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "FogImports.h"

void __stdcall QuestRecordSetBit(PTQUESTBITARRAY ptArray, DWORD nWord, DWORD nBit)
{
	FogBitManipSetBit(ptArray->pBits, nWord * 16 + nBit);
}

void __stdcall QuestRecordClearBit(PTQUESTBITARRAY ptArray, DWORD nWord, DWORD nBit)
{
	FogBitManipClearBit(ptArray->pBits, nWord * 16 + nBit);
}

void __stdcall QuestRecordClearMidBits(PTQUESTBITARRAY ptArray, DWORD nWord)
{
	DWORD nBit;
	int nLeft;

	nBit = nWord * 16 + 2;
	for (nLeft = 10; nLeft != 0; nLeft--, nBit++)
		FogBitManipClearBit(ptArray->pBits, nBit);
}

PTQUESTBITARRAY __stdcall QuestRecordNew(void *pPool)
{
	PTQUESTBITARRAY ptArray;
	BYTE *pBits;

#line 74
	ptArray = (PTQUESTBITARRAY)D2ALLOCPOOL(pPool, sizeof(QUESTBITARRAY));
#line 75
	pBits = (BYTE *)D2ALLOCPOOL(pPool, 96);
	memset(pBits, 0, 96);
	FogBitManipInitBuffer(ptArray, pBits, 96);
	return ptArray;
}

void __stdcall QuestRecordFree(void *pPool, PTQUESTBITARRAY ptArray)
{
	if (ptArray == NULL)
		return;
#line 90
	D2FREEPOOL(pPool, ptArray->pBits);
#line 91
	D2FREEPOOL(pPool, ptArray);
}

void __stdcall QuestRecordCopyAndSweep(PTQUESTBITARRAY ptArray, void *pSrc, WORD wBufSize, DWORD fSweep)
{
	int i;

#line 104
	D2ASSERT(wBufSize == NUM_QUEST_WORDS*sizeof(WORD));
	memcpy(ptArray->pBits, pSrc, NUM_QUEST_WORDS * sizeof(WORD));
	if (!fSweep)
		return;
	for (i = 14; i < 670; i += 16)
	{
		FogBitManipClearBit(ptArray->pBits, i - 1);
		FogBitManipClearBit(ptArray->pBits, i);
		if (FogBitManipTestBit(ptArray->pBits, i - 13) != 0)
			FogBitManipSetBit(ptArray->pBits, i + 1);
	}
}

void __stdcall QuestRecordCopyOut(PTQUESTBITARRAY ptArray, void *pDst, WORD wBufSize, DWORD dwUnused)
{
#line 126
	D2ASSERT(wBufSize == NUM_QUEST_WORDS*sizeof(WORD));
	memcpy(pDst, ptArray->pBits, NUM_QUEST_WORDS * sizeof(WORD));
}
