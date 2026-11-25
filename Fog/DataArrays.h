#ifndef D2RE_DATAARRAYS_H
#define D2RE_DATAARRAYS_H

#include <windows.h>

typedef struct DATAARRAY DATAARRAY, *PTDATAARRAY;

struct DATAARRAY
{
 	char		szName[256];
 	int			nCount;
 	int			nElementSize;
 	BOOL		fValid;
 	int			nNextFree;
 	int			nLastUsed;
 	int			nNumUsed;
 	WORD		wNextSerial;
 	BYTE		unobserved_1[2];
 	BYTE		*pbElements;
};

DATAARRAY * __fastcall DataArraysCreate(const char *pszName, int nCount, int nElementSize);
void __fastcall DataArraysFree(DATAARRAY *lptDataArray);
void __fastcall DataArraysInit(DATAARRAY *lptDataArray);
void __fastcall DataArraysReset(DATAARRAY *lptDataArray);
DWORD __fastcall DataArraysAlloc(DATAARRAY *lptDataArray);
void __fastcall DataArraysFreeElement(DATAARRAY *lptDataArray, DWORD dwHandle);
BYTE * __fastcall DataArraysGetUsed(DATAARRAY *lptDataArray, DWORD dwHandle);
BYTE * __fastcall DataArraysGetElement(DATAARRAY *lptDataArray, DWORD dwHandle);

#endif
