#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\Raw.cpp"
#include "D2Cmp.h"
#include "D2Cmp.h"
#include "D2Macros.h"
#include "CelList.h"
#line 11

void __fastcall RawBuildCelFile(CELLIST *ptList, D2CELFILE *pFile)
{
	TCELENTRY *ptEntry;
	D2CEL *pCel;
	DWORD dwFrames;
	DWORD dwSize;
	DWORD dwTurn;
	DWORD i;
	TSList<TCELENTRY, TSGetLink<TCELENTRY> > *ptEntries;

	i = 0;
	ptEntries = &ptList->tEntries;
	dwFrames = ptList->nCels / ptList->dwNumDirections;
	for (ptEntry = ptEntries->Head(); (INT_PTR)ptEntry > 0;
		ptEntry = ptEntries->LinkNext(ptEntry))
	{
		dwSize = 3 * ptEntry->field_0c * ptEntry->field_10;
#line 34
		pCel = (D2CEL *)D2ALLOC2(CELSIZE(dwSize));
		pCel->dwFlags = ptEntry->field_08;
		pCel->dwWidth = ptEntry->field_0c;
		pCel->dwHeight = ptEntry->field_10;
		pCel->nOffsetX = ptEntry->field_18;
		pCel->nOffsetY = ptEntry->field_1c;
		pCel->dwExtraSize = 0;
		pCel->ptSlot = NULL;
		pCel->dwDataSize = dwSize;
		memcpy(pCel->abData, ptEntry->field_20, dwSize);
		if (ptList->dwNumDirections > 1)
			dwTurn = (i / dwFrames << 6) / ptList->dwNumDirections;
		else
			dwTurn = 1;
		pFile->aptCels[CelGetGameDirection(dwTurn,
			ptList->dwNumDirections) * dwFrames + i % dwFrames] = pCel;
		i++;
	}
}

void __fastcall RawCelForEachPixel(D2CEL *pCel, void (WINAPI *pfnPixel)(DWORD), DWORD dwKeyColor)
{
	const BYTE *pbPixel;
	DWORD dwKey;
	DWORD dwColor;
	DWORD i;

	dwKey = ((((BYTE *)&dwKeyColor)[2] << 8) + ((BYTE *)&dwKeyColor)[1]) << 8;
	dwKey += ((BYTE *)&dwKeyColor)[0];
	pbPixel = pCel->abData;
	for (i = 0; i < pCel->dwWidth * pCel->dwHeight; i++)
	{
		dwColor = (((DWORD)pbPixel[2] << 8) + pbPixel[1]) << 8;
		dwColor += pbPixel[0];
		pbPixel += 3;
		if (dwColor != dwKey)
			pfnPixel(dwColor);
	}
}
