#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\FieldTbls.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "DataTbls.h"

#pragma pack(push, 1)
struct EXPFIELDFILE
{
 	BYTE		unobserved_0[2];
 	int			nHeight;
 	int			nWidth;
 	BYTE		abField[1];
};
#pragma pack(pop)
typedef struct EXPFIELDFILE EXPFIELDFILE;

typedef int FIELDSTEPOFFSETS[9];
static FIELDSTEPOFFSETS sgnFieldStepOffsets = { 0 };
static BYTE *sgpbField = NULL;
static int sgnFieldWidth = 0;
static int sgnFieldHeight = 0;
static int sgnFieldSize = 0;
static int sgnFieldCenterX = 0;
static int sgnFieldCenterY = 0;

static int sgcnFieldStepX[9] =
{
	0, 1, 1, 1, 0, -1, -1, -1, 0,
};
static int sgcnFieldStepY[9] =
{
	-1, -1, 0, 1, 1, 1, 0, -1, 0,
};

BOOL __stdcall FieldTblsLoad(void *pBuffer, int nBufferSize)
{
	void *pFile;
	int nHeight;
	int nWidth;
	int nSize;
	BYTE *pbField;
	int i;

	pFile = pBuffer;
	pBuffer = (BYTE *)pBuffer + 2;
	nHeight = *(int *)pBuffer;
	pBuffer = (BYTE *)pBuffer + 4;
	nWidth = *(int *)pBuffer;
	pBuffer = (BYTE *)pBuffer + 4;
	nSize = nWidth * nHeight;
#line 46
	pbField = (BYTE *)D2ALLOCPOOL(NULL, nSize);
#line 48
	D2ASSERT(pbField);
	memcpy(pbField, pBuffer, nSize);
	sgpbField = pbField;
	sgnFieldHeight = nHeight;
	sgnFieldCenterX = nWidth >> 1;
	sgnFieldWidth = nWidth;
	sgnFieldSize = nSize;
	sgnFieldCenterY = nHeight >> 1;
	for (i = 0; i < 9; i++)
		sgnFieldStepOffsets[i] = sgcnFieldStepY[i] * nWidth + sgcnFieldStepX[i];
#line 62
	D2FREEPOOL(NULL, pFile);
	return TRUE;
}

BOOL __fastcall FieldTblsFree(void)
{
	if (sgpbField)
	{
#line 73
		D2FREEPOOL(NULL, sgpbField);
	}
	sgpbField = NULL;
	sgnFieldHeight = 0;
	sgnFieldWidth = 0;
	sgnFieldSize = 0;
	sgnFieldCenterX = 0;
	sgnFieldCenterY = 0;
	return TRUE;
}

void __stdcall FieldTblsGetCenter(int *pnX, int *pnY)
{
	*pnX = sgnFieldCenterX;
	*pnY = sgnFieldCenterY;
}

int __fastcall FieldTblsGetWidth(void)
{
	return sgnFieldWidth;
}

int __fastcall FieldTblsGetHeight(void)
{
	return sgnFieldHeight;
}

DWORD * __fastcall FieldTblsAllocPair(void)
{
	DWORD *tField;
	DWORD i;

#line 117
	tField = (DWORD *)D2ALLOCPOOL(NULL, 8);
#line 118
	D2ASSERT(tField);
	for (i = 0; i < 2; i++)
		tField[i] = 0;
	return tField;
}

void __stdcall FieldTblsFreePair(DWORD *tField)
{
#line 129
	D2ASSERT(tField);
#line 130
	D2FREEPOOL(NULL, tField);
}

void __stdcall FieldTblsSetPair(DWORD *tField, DWORD dwA, DWORD dwB)
{
#line 143
	D2ASSERT(tField);
	tField[0] = dwA;
	tField[1] = dwB;
}

int __stdcall FieldTblsGetAt(DWORD *tField, int nX, int nY)
{
#line 158
	D2ASSERT(tField);
	return sgpbField[((nY - tField[1]) << 8) - tField[0] + nX];
}

BOOL __stdcall FieldTblsStep(DWORD *tField, int *pnX, int *pnY)
{
	int nDir;

#line 173
	D2ASSERT(tField);
	nDir = *(BYTE *)(sgnFieldCenterX + (int)sgpbField + ((*pnY - (int)tField[1] + sgnFieldCenterY) << 8) - (int)tField[0] + *pnX);
	*pnX = sgcnFieldStepX[nDir] + *pnX;
	*pnY = sgcnFieldStepY[nDir] + *pnY;
	return *(BYTE *)(sgnFieldCenterX + (int)sgpbField + ((*pnY - (int)tField[1] + sgnFieldCenterY) << 8) - (int)tField[0] + *pnX) != 8;
}

BOOL __stdcall FieldTblsFindCandidate(DWORD dwField, DWORD dwCount, DWORD dwA, DWORD dwB, DWORD dwPattern)
{
	if (dwCount == 0)
		return FALSE;
	if (D2CommonCollisnTestPoint(dwCount, dwA, dwB, dwPattern) != 0)
		return TRUE;
	while (D2CommonFieldTblsStep(dwField, &dwA, &dwB))
	{
		if (D2CommonCollisnTestPoint(dwCount, dwA, dwB, dwPattern) != 0)
			return TRUE;
	}
	return FALSE;
}
