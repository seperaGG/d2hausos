#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\DataArrays.cpp"
#include "FogLocal.h"
#include <stdarg.h>
#include <stdio.h>
#include "DataArrays.h"
#include "D2Macros.h"

DATAARRAY * __fastcall DataArraysCreate(const char *pszName, int nCount, int nElementSize)
{
	DATAARRAY *lptDataArray;
	int i;

#line 51
	lptDataArray = (DATAARRAY *)D2ALLOC2(nCount * nElementSize + sizeof(DATAARRAY));
	if (lptDataArray) {
		for (i = 0; i < (int)(sizeof(DATAARRAY) / sizeof(DWORD)); i++)
			((DWORD *)lptDataArray)[i] = 0;
		strncpy(lptDataArray->szName, pszName, 256);
		lptDataArray->nElementSize = nElementSize;
		lptDataArray->nCount = nCount;
		lptDataArray->pbElements = (BYTE *)(lptDataArray + 1);
		lptDataArray->fValid = FALSE;
	}
	return lptDataArray;
}

void __fastcall DataArraysFree(DATAARRAY *lptDataArray)
{
	int i;

	for (i = 0; i < (int)(sizeof(DATAARRAY) / sizeof(DWORD)); i++)
		((DWORD *)lptDataArray)[i] = 0;
#line 75
	D2FREE2(lptDataArray);
}

void __fastcall DataArraysInit(DATAARRAY *lptDataArray)
{
	DATAARRAY *ptArray;
	int i;

	ptArray = lptDataArray;
	ptArray->fValid = TRUE;
	ptArray->nLastUsed = -1;
	ptArray->nNumUsed = 0;
	ptArray->nNextFree = 0;
	ptArray->wNextSerial = 32768;
	for (i = 0; i < ptArray->nCount; i++)
		*(WORD *)(ptArray->pbElements + ptArray->nElementSize * i) = 0;
}

void __fastcall DataArraysReset(DATAARRAY *lptDataArray)
{
	int i;

#line 103
	D2ASSERT(lptDataArray->fValid);
	lptDataArray->nLastUsed = -1;
	lptDataArray->nNumUsed = 0;
	lptDataArray->nNextFree = 0;
	lptDataArray->wNextSerial = 32768;
	for (i = 0; i < lptDataArray->nCount; i++)
		*(WORD *)(lptDataArray->pbElements + i * lptDataArray->nElementSize) = 0;
}

DWORD __fastcall DataArraysAlloc(DATAARRAY *lptDataArray)
{
	BYTE *pbElement;
	int i;
	DWORD dwHandle = (DWORD)-1;
#line 134
	D2ASSERT(lptDataArray->fValid);
	i = lptDataArray->nNextFree;
	pbElement = lptDataArray->pbElements + lptDataArray->nElementSize * i;
	for (; i < lptDataArray->nCount; i++, pbElement += lptDataArray->nElementSize)
	{
		if (*(WORD *)pbElement)
			continue;
		memset(pbElement, 0, lptDataArray->nElementSize);
		*(WORD *)pbElement = lptDataArray->wNextSerial;
		if (!++lptDataArray->wNextSerial)
			lptDataArray->wNextSerial = 32768;
		lptDataArray->nNumUsed++;
		lptDataArray->nNextFree = i + 1;
		if (lptDataArray->nLastUsed < i)
			lptDataArray->nLastUsed = i;
		dwHandle = ((DWORD)i << 16) | (*(short *)pbElement & 65535);
		break;
	}
	return dwHandle;
}

void __fastcall DataArraysFreeElement(DATAARRAY *lptDataArray, DWORD dwHandle)
{
	BYTE *pbElement;
	int nIndex;

	pbElement = DataArraysGetUsed(lptDataArray, dwHandle);
	*(WORD *)pbElement = 0;
	nIndex = (short)((int)dwHandle >> 16);
	if (nIndex < lptDataArray->nNextFree)
		lptDataArray->nNextFree = nIndex;
	if (nIndex == lptDataArray->nLastUsed)
	{
		do
		{
			pbElement -= lptDataArray->nElementSize;
			lptDataArray->nLastUsed--;
		} while (lptDataArray->nLastUsed >= 0 && !*(WORD *)pbElement);
	}
	lptDataArray->nNumUsed--;
}

BYTE * __fastcall DataArraysGetElement(DATAARRAY *lptDataArray, DWORD dwHandle)
{
	char szMessage[256];
	const char *pszMessage;
	short nIndex;
	BYTE *pbElement;
	BYTE *pbResult = NULL;

	if (!(dwHandle == (DWORD)-1))
	{
		nIndex = (short)((int)dwHandle >> 16);
		if (nIndex < 0 || nIndex >= lptDataArray->nCount)
		{
#line 217
			FogAssert((const char *)FogDataArraysSprintf(szMessage, "%s index #%d (0x%x) not in [0,#%d)", lptDataArray->szName, (int)nIndex, dwHandle, lptDataArray->nCount), D2_THIS_FILE, __LINE__);
			exit(-1); }
#line 220
		D2ASSERT(lptDataArray->fValid);
		pbElement = lptDataArray->pbElements + lptDataArray->nElementSize * nIndex;
		if (*(WORD *)pbElement && (!(WORD)dwHandle || *(WORD *)pbElement == (WORD)dwHandle))
			pbResult = pbElement; }
	return pbResult;
}

BYTE * __fastcall DataArraysGetUsed(DATAARRAY *lptDataArray, DWORD dwHandle)
{
	char szMessage[256];
	const char *pszMessage;
	BYTE *pbElement;

	pbElement = DataArraysGetElement(lptDataArray, dwHandle);
	if (!pbElement)
	{
#line 250
		FogAssert((const char *)FogDataArraysSprintf(szMessage, "%s index #%d (0x%x) (%p.#%d) is unused or changed", lptDataArray->szName, (short)((int)dwHandle >> 16), dwHandle, pbElement, lptDataArray->nElementSize), D2_THIS_FILE, __LINE__);
		exit(-1);
	}
	return pbElement;
}

char * __cdecl DataArraysSprintf(char *pszBuffer, const char *pszFormat, ...)
{
	va_list pArgs;

	va_start(pArgs, pszFormat);
	vsprintf(pszBuffer, pszFormat, pArgs);
	va_end(pArgs);
	return pszBuffer;
}
