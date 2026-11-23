#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\InvTbls.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

int gnInventoryInitStats = 0;
static PTINVENTORYTXT sgptInventoryInitStats = NULL;

PTINVENTORYTXT __fastcall InvTblsLoad(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "invLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, inv.Left) },
		{ "invRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, inv.Right) },
		{ "invTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, inv.Top) },
		{ "invBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, inv.Bottom) },
		{ "gridRows", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, grid.Rows) },
		{ "gridCols", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, grid.Cols) },
		{ "gridLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, grid.Left) },
		{ "gridRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, grid.Right) },
		{ "gridTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, grid.Top) },
		{ "gridBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, grid.Bottom) },
		{ "gridBoxWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, grid.BoxWidth) },
		{ "gridBoxHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, grid.BoxHeight) },
		{ "rArmLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[0].Left) },
		{ "rArmRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[0].Right) },
		{ "rArmTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[0].Top) },
		{ "rArmBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[0].Bottom) },
		{ "rArmWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[0].Width) },
		{ "rArmHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[0].Height) },
		{ "torsoLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[1].Left) },
		{ "torsoRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[1].Right) },
		{ "torsoTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[1].Top) },
		{ "torsoBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[1].Bottom) },
		{ "torsoWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[1].Width) },
		{ "torsoHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[1].Height) },
		{ "lArmLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[2].Left) },
		{ "lArmRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[2].Right) },
		{ "lArmTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[2].Top) },
		{ "lArmBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[2].Bottom) },
		{ "lArmWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[2].Width) },
		{ "lArmHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[2].Height) },
		{ "headLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[3].Left) },
		{ "headRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[3].Right) },
		{ "headTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[3].Top) },
		{ "headBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[3].Bottom) },
		{ "headWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[3].Width) },
		{ "headHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[3].Height) },
		{ "neckLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[4].Left) },
		{ "neckRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[4].Right) },
		{ "neckTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[4].Top) },
		{ "neckBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[4].Bottom) },
		{ "neckWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[4].Width) },
		{ "neckHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[4].Height) },
		{ "rHandLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[5].Left) },
		{ "rHandRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[5].Right) },
		{ "rHandTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[5].Top) },
		{ "rHandBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[5].Bottom) },
		{ "rHandWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[5].Width) },
		{ "rHandHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[5].Height) },
		{ "lHandLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[6].Left) },
		{ "lHandRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[6].Right) },
		{ "lHandTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[6].Top) },
		{ "lHandBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[6].Bottom) },
		{ "lHandWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[6].Width) },
		{ "lHandHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[6].Height) },
		{ "beltLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[7].Left) },
		{ "beltRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[7].Right) },
		{ "beltTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[7].Top) },
		{ "beltBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[7].Bottom) },
		{ "beltWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[7].Width) },
		{ "beltHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[7].Height) },
		{ "feetLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[8].Left) },
		{ "feetRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[8].Right) },
		{ "feetTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[8].Top) },
		{ "feetBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[8].Bottom) },
		{ "feetWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[8].Width) },
		{ "feetHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[8].Height) },
		{ "glovesLeft", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[9].Left) },
		{ "glovesRight", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[9].Right) },
		{ "glovesTop", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[9].Top) },
		{ "glovesBottom", EXCEL_TYPE_DWORD, 0, offsetof(INVENTORYTXT, atBodyLocs[9].Bottom) },
		{ "glovesWidth", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[9].Width) },
		{ "glovesHeight", EXCEL_TYPE_BYTE, 0, offsetof(INVENTORYTXT, atBodyLocs[9].Height) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;

	sgptInventoryInitStats = (PTINVENTORYTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize,
		&nRows, sizeof(INVENTORYTXT));
	gnInventoryInitStats = nRows;
	return sgptInventoryInitStats;
}

void __fastcall InvTblsFree(void)
{
	DataTblsFreeBlock(sgptInventoryInitStats);
}

static __inline PTINVENTORYTXT InvTblsGetStats(DWORD nIndex)
{
#line 135
	D2ASSERT(sgptInventoryInitStats);
	if (nIndex > (DWORD)gnInventoryInitStats)
		return NULL;
	return &sgptInventoryInitStats[nIndex];
}

void __stdcall InvTblsGetInvRect(DWORD nIndex, INVRECTTXT *ptRect)
{
	PTINVENTORYTXT ptStats;

	ptStats = InvTblsGetStats(nIndex);
#line 148
	D2ASSERT(ptStats);
	*ptRect = ptStats->inv;
}

void __stdcall InvTblsGetGrid(DWORD nIndex, INVGRIDTXT *ptGrid)
{
	PTINVENTORYTXT ptStats;

	ptStats = InvTblsGetStats(nIndex);
#line 159
	D2ASSERT(ptStats);
	*ptGrid = ptStats->grid;
}

void __stdcall InvTblsGetBodyLoc(DWORD nIndex, INVBODYLOCTXT *ptBodyLoc, int nLoc)
{
	PTINVENTORYTXT ptStats;
	INVBODYLOCTXT *ptBodyStats;

	ptStats = InvTblsGetStats(nIndex);
#line 171
	D2ASSERT(ptStats);
	ptBodyStats = &ptStats->atBodyLocs[nLoc];
#line 173
	D2ASSERT(ptBodyStats);
	*ptBodyLoc = *ptBodyStats;
}
