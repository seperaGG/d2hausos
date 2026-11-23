#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\TokTbls.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

struct TOKENDATATBLDESC
{
 	int			nSize;
 	PTTOKENTXT	ptTokenStats;
 	PTTOKENTXT	ptTypeStats;
 	PTTOKENTXT	ptModeStats;
};
typedef struct TOKENDATATBLDESC TOKENDATATBLDESC;

static PTTOKENTXT sgptArmorTokenStats;
static TOKENDATATBLDESC sgtMonsterTokenDataTblDesc;
static PTTOKENTXT sgptComponentTokenStats;
static TOKENDATATBLDESC sgtObjectTokenDataTblDesc;
static TOKENDATATBLDESC sgtPlayerTokenDataTblDesc;

void __stdcall TokTblsGetFrameData(BYTE *pBase, int nFrame, void **ppDataPtr, DWORD *pdwRate,
	DWORD *pdwField08, DWORD *pdwClass)
{
	PTANIMFRAMEREC ptRec;

	ptRec = (PTANIMFRAMEREC)(pBase + (nFrame >> 8) * sizeof(ANIMFRAMEREC));
	*ppDataPtr = ptRec->pDataPtr;
	*pdwRate = ptRec->dwRate;
	*pdwField08 = ptRec->field_08;
	*pdwClass = ptRec->dwClass;
}

void __stdcall TokTblsGetFrameClass(BYTE *pBase, int nFrame, DWORD *pdwClass)
{
	PTANIMFRAMEREC ptRec;

	ptRec = (PTANIMFRAMEREC)(pBase + (nFrame >> 8) * sizeof(ANIMFRAMEREC));
	*pdwClass = ptRec->dwClass;
}

void __fastcall TokTblsLoadPlrTypeModes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(TOKENTXT, name) },
		{ "token", EXCEL_TYPE_CODE, 0, offsetof(TOKENTXT, token) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nTypes;
	int nModes;
	int nTokens;
	PTTOKENTXT ptTypes;
	PTTOKENTXT ptModes;
	PTTOKENTXT ptTokenStats;
	PTTOKENTXT ptModeStats;

	ptTypes = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer1, nBufferSize1, &nTypes,
		sizeof(TOKENTXT));
	ptModes = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer2, nBufferSize2, &nModes,
		sizeof(TOKENTXT));
	nTokens = nTypes + nModes;
#line 71
	ptTokenStats = (PTTOKENTXT)D2ALLOCPOOL(NULL, nTokens * sizeof(TOKENTXT));
#line 72
	D2ASSERT(ptTokenStats);
	memcpy(ptTokenStats, ptTypes, nTypes * sizeof(TOKENTXT));
	ptModeStats = ptTokenStats + nTypes;
	memcpy(ptModeStats, ptModes, nModes * sizeof(TOKENTXT));
	DataTblsFreeBlock(ptTypes);
	DataTblsFreeBlock(ptModes);
	sgtPlayerTokenDataTblDesc.nSize = nTokens;
	sgtPlayerTokenDataTblDesc.ptTokenStats = ptTokenStats;
	sgtPlayerTokenDataTblDesc.ptTypeStats = ptTokenStats;
	sgtPlayerTokenDataTblDesc.ptModeStats = ptModeStats;
}

void __fastcall TokTblsLoadMonTypeModes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(TOKENTXT, name) },
		{ "token", EXCEL_TYPE_CODE, 0, offsetof(TOKENTXT, token) },
		{ "DT_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, DT_Dir) },
		{ "NU_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, NU_Dir) },
		{ "WL_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, WL_Dir) },
		{ "GH_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, GH_Dir) },
		{ "A1_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, A1_Dir) },
		{ "A2_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, A2_Dir) },
		{ "BL_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, BL_Dir) },
		{ "SC_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, SC_Dir) },
		{ "S1_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, S1_Dir) },
		{ "S2_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, S2_Dir) },
		{ "S3_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, S3_Dir) },
		{ "S4_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, S4_Dir) },
		{ "DD_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, DD_Dir) },
		{ "KB_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, KB_Dir) },
		{ "SQ_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, SQ_Dir) },
		{ "RN_Dir", EXCEL_TYPE_OPTBYTE, 0, offsetof(TOKENTXT, RN_Dir) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nTypes;
	int nModes;
	int nTokens;
	PTTOKENTXT ptTypes;
	PTTOKENTXT ptModes;
	PTTOKENTXT ptTokenStats;
	PTTOKENTXT ptModeStats;

	ptTypes = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer1, nBufferSize1, &nTypes,
		sizeof(TOKENTXT));
	ptModes = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer2, nBufferSize2, &nModes,
		sizeof(TOKENTXT));
	nTokens = nTypes + nModes;
#line 153
	ptTokenStats = (PTTOKENTXT)D2ALLOCPOOL(NULL, nTokens * sizeof(TOKENTXT));
#line 154
	D2ASSERT(ptTokenStats);
	memcpy(ptTokenStats, ptTypes, nTypes * sizeof(TOKENTXT));
	ptModeStats = ptTokenStats + nTypes;
	memcpy(ptModeStats, ptModes, nModes * sizeof(TOKENTXT));
	DataTblsFreeBlock(ptTypes);
	DataTblsFreeBlock(ptModes);
	sgtMonsterTokenDataTblDesc.nSize = nTokens;
	sgtMonsterTokenDataTblDesc.ptTokenStats = ptTokenStats;
	sgtMonsterTokenDataTblDesc.ptTypeStats = ptTokenStats;
	sgtMonsterTokenDataTblDesc.ptModeStats = ptModeStats;
}

void __fastcall TokTblsLoadObjTypeModes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(TOKENTXT, name) },
		{ "token", EXCEL_TYPE_CODE, 0, offsetof(TOKENTXT, token) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nTypes;
	int nModes;
	int nTokens;
	PTTOKENTXT ptTypes;
	PTTOKENTXT ptModes;
	PTTOKENTXT ptTokenStats;
	PTTOKENTXT ptModeStats;

	ptTypes = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer1, nBufferSize1, &nTypes,
		sizeof(TOKENTXT));
	ptModes = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer2, nBufferSize2, &nModes,
		sizeof(TOKENTXT));
	nTokens = nTypes + nModes;
#line 219
	ptTokenStats = (PTTOKENTXT)D2ALLOCPOOL(NULL, nTokens * sizeof(TOKENTXT));
#line 220
	D2ASSERT(ptTokenStats);
	memcpy(ptTokenStats, ptTypes, nTypes * sizeof(TOKENTXT));
	ptModeStats = ptTokenStats + nTypes;
	memcpy(ptModeStats, ptModes, nModes * sizeof(TOKENTXT));
	DataTblsFreeBlock(ptTypes);
	DataTblsFreeBlock(ptModes);
	sgtObjectTokenDataTblDesc.nSize = nTokens;
	sgtObjectTokenDataTblDesc.ptTokenStats = ptTokenStats;
	sgtObjectTokenDataTblDesc.ptTypeStats = ptTokenStats;
	sgtObjectTokenDataTblDesc.ptModeStats = ptModeStats;
}

PTTOKENTXT __fastcall TokTblsLoadComposit(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(TOKENTXT, name) },
		{ "token", EXCEL_TYPE_CODE, 0, offsetof(TOKENTXT, token) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;

	sgptComponentTokenStats = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize,
		&nRows, sizeof(TOKENTXT));
	return sgptComponentTokenStats;
}

PTTOKENTXT __fastcall TokTblsLoadArmType(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "name", EXCEL_TYPE_STRING, 31, offsetof(TOKENTXT, name) },
		{ "token", EXCEL_TYPE_CODE, 0, offsetof(TOKENTXT, token) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;

	sgptArmorTokenStats = (PTTOKENTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(TOKENTXT));
	return sgptArmorTokenStats;
}

void __fastcall TokTblsFree(void)
{
	if (sgtPlayerTokenDataTblDesc.ptTokenStats)
	{
#line 311
		D2FREEPOOL(NULL, sgtPlayerTokenDataTblDesc.ptTokenStats);
	}
	sgtPlayerTokenDataTblDesc.ptTokenStats = NULL;
	if (sgtMonsterTokenDataTblDesc.ptTokenStats)
	{
#line 316
		D2FREEPOOL(NULL, sgtMonsterTokenDataTblDesc.ptTokenStats);
	}
	sgtMonsterTokenDataTblDesc.ptTokenStats = NULL;
	if (sgtObjectTokenDataTblDesc.ptTokenStats)
	{
#line 321
		D2FREEPOOL(NULL, sgtObjectTokenDataTblDesc.ptTokenStats);
	}
	sgtObjectTokenDataTblDesc.ptTokenStats = NULL;
	if (sgptComponentTokenStats)
		DataTblsFreeBlock(sgptComponentTokenStats);
	sgptComponentTokenStats = NULL;
	if (sgptArmorTokenStats)
		DataTblsFreeBlock(sgptArmorTokenStats);
	sgptArmorTokenStats = NULL;
}

TOKENDATATBLDESC * __fastcall TokTblsGetPlayerDesc(void)
{
	return &sgtPlayerTokenDataTblDesc;
}

TOKENDATATBLDESC * __fastcall TokTblsGetMonsterDesc(void)
{
	return &sgtMonsterTokenDataTblDesc;
}

TOKENDATATBLDESC * __fastcall TokTblsGetObjectDesc(void)
{
	return &sgtObjectTokenDataTblDesc;
}

PTTOKENTXT __stdcall TokTblsGetPlayerToken(DWORD nIndex, int nType)
{
	if (nIndex < (DWORD)sgtPlayerTokenDataTblDesc.nSize)
	{
#line 367
		D2ASSERT(sgtPlayerTokenDataTblDesc.ptTokenStats);
		switch (nType)
		{
		case 0:
#line 370
			D2ASSERT((sgtPlayerTokenDataTblDesc.ptTypeStats + nIndex));
			return sgtPlayerTokenDataTblDesc.ptTypeStats + nIndex;
		case 1:
#line 373
			D2ASSERT((sgtPlayerTokenDataTblDesc.ptModeStats + nIndex));
			return sgtPlayerTokenDataTblDesc.ptModeStats + nIndex;
		}
	}
	return NULL;
}

PTTOKENTXT __stdcall TokTblsGetMonsterToken(DWORD nIndex, int nType)
{
	if (nIndex < (DWORD)sgtMonsterTokenDataTblDesc.nSize)
	{
		switch (nType)
		{
		case 0:
#line 389
			D2ASSERT((sgtMonsterTokenDataTblDesc.ptTypeStats + nIndex));
			return sgtMonsterTokenDataTblDesc.ptTypeStats + nIndex;
		case 1:
#line 392
			D2ASSERT((sgtMonsterTokenDataTblDesc.ptModeStats + nIndex));
			return sgtMonsterTokenDataTblDesc.ptModeStats + nIndex;
		}
	}
	return NULL;
}

PTTOKENTXT __stdcall TokTblsGetObjectToken(DWORD nIndex, int nType)
{
	if (nIndex < (DWORD)sgtObjectTokenDataTblDesc.nSize)
	{
		switch (nType)
		{
		case 0:
#line 408
			D2ASSERT((sgtObjectTokenDataTblDesc.ptTypeStats + nIndex));
			return sgtObjectTokenDataTblDesc.ptTypeStats + nIndex;
		case 1:
#line 411
			D2ASSERT((sgtObjectTokenDataTblDesc.ptModeStats + nIndex));
			return sgtObjectTokenDataTblDesc.ptModeStats + nIndex;
		}
	}
	return NULL;
}

PTTOKENTXT __stdcall TokTblsGetComponent(int eComponent)
{
#line 422
	D2ASSERT((sgptComponentTokenStats + eComponent));
	return sgptComponentTokenStats + eComponent;
}

PTTOKENTXT __stdcall TokTblsGetArmor(int nIndex)
{
#line 431
	D2ASSERT((sgptArmorTokenStats + nIndex));
	return sgptArmorTokenStats + nIndex;
}

void __fastcall TokTblsLoadNothing(HD2ARCHIVE hArchive)
{
}

void __fastcall TokTblsFreeNothing(void)
{
}
