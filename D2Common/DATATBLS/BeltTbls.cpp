#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\BeltTbls.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

static PTBELTSTXT sgptBeltInitStats;

PTBELTSTXT __fastcall BeltTblsLoad(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "numboxes", EXCEL_TYPE_BYTE, 0, offsetof(BELTSTXT, numboxes) },
		{ "box1left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[0].left) },
		{ "box1right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[0].right) },
		{ "box1top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[0].top) },
		{ "box1bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[0].bottom) },
		{ "box2left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[1].left) },
		{ "box2right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[1].right) },
		{ "box2top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[1].top) },
		{ "box2bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[1].bottom) },
		{ "box3left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[2].left) },
		{ "box3right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[2].right) },
		{ "box3top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[2].top) },
		{ "box3bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[2].bottom) },
		{ "box4left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[3].left) },
		{ "box4right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[3].right) },
		{ "box4top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[3].top) },
		{ "box4bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[3].bottom) },
		{ "box5left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[4].left) },
		{ "box5right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[4].right) },
		{ "box5top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[4].top) },
		{ "box5bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[4].bottom) },
		{ "box6left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[5].left) },
		{ "box6right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[5].right) },
		{ "box6top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[5].top) },
		{ "box6bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[5].bottom) },
		{ "box7left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[6].left) },
		{ "box7right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[6].right) },
		{ "box7top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[6].top) },
		{ "box7bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[6].bottom) },
		{ "box8left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[7].left) },
		{ "box8right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[7].right) },
		{ "box8top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[7].top) },
		{ "box8bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[7].bottom) },
		{ "box9left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[8].left) },
		{ "box9right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[8].right) },
		{ "box9top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[8].top) },
		{ "box9bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[8].bottom) },
		{ "box10left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[9].left) },
		{ "box10right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[9].right) },
		{ "box10top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[9].top) },
		{ "box10bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[9].bottom) },
		{ "box11left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[10].left) },
		{ "box11right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[10].right) },
		{ "box11top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[10].top) },
		{ "box11bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[10].bottom) },
		{ "box12left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[11].left) },
		{ "box12right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[11].right) },
		{ "box12top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[11].top) },
		{ "box12bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[11].bottom) },
		{ "box13left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[12].left) },
		{ "box13right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[12].right) },
		{ "box13top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[12].top) },
		{ "box13bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[12].bottom) },
		{ "box14left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[13].left) },
		{ "box14right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[13].right) },
		{ "box14top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[13].top) },
		{ "box14bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[13].bottom) },
		{ "box15left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[14].left) },
		{ "box15right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[14].right) },
		{ "box15top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[14].top) },
		{ "box15bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[14].bottom) },
		{ "box16left", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[15].left) },
		{ "box16right", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[15].right) },
		{ "box16top", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[15].top) },
		{ "box16bottom", EXCEL_TYPE_DWORD, 0, offsetof(BELTSTXT, box[15].bottom) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;

	sgptBeltInitStats = (PTBELTSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(BELTSTXT));
	return sgptBeltInitStats;
}

void __fastcall BeltTblsFree(void)
{
	DataTblsFreeBlock(sgptBeltInitStats);
}

static __inline PTBELTSTXT BeltTblsGetStats(int nIndex)
{
#line 126
	D2ASSERT(sgptBeltInitStats);
#line 127
	D2ASSERT(sgptBeltInitStats + nIndex);
	return sgptBeltInitStats + nIndex;
}

void __stdcall BeltTblsGetBelt(int nIndex, BELTSTXT *ptBelt)
{
#line 136
	D2ASSERT(sgptBeltInitStats + nIndex);
	*ptBelt = sgptBeltInitStats[nIndex];
}

void __stdcall BeltTblsGetBox(int nIndex, BELTBOXTXT *ptBox, int nBox)
{
	PTBELTSTXT ptStats;
	BELTBOXTXT *ptBoxStats;

	ptStats = BeltTblsGetStats(nIndex);
	ptBoxStats = &ptStats->box[nBox];
#line 148
	D2ASSERT(ptBoxStats);
	*ptBox = ptStats->box[nBox];
}
