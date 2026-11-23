#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\OverlayTbls.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

static PTOVERLAYTXT sgptOverlayStats = NULL;
static UINT sgnNumOverlayStats = 0;

void __fastcall OverlayTblsLoad(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Filename", EXCEL_TYPE_STRING, 64, offsetof(OVERLAYTXT, Filename) },
		{ "Frames", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Frames) },
		{ "PreDraw", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, PreDraw) },
		{ "1ofN", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, nOneOfN) },
		{ "Dir", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Dir) },
		{ "Open", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Open) },
		{ "Beta", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Beta) },
		{ "Xoffset", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Xoffset) },
		{ "Yoffset", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Yoffset) },
		{ "Height1", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Height1) },
		{ "Height2", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Height2) },
		{ "Height3", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Height3) },
		{ "Height4", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Height4) },
		{ "AnimRate", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, AnimRate) },
		{ "LoopWaitTime", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, LoopWaitTime) },
		{ "Trans", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Trans) },
		{ "InitRadius", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, InitRadius) },
		{ "Radius", EXCEL_TYPE_DWORD, 0, offsetof(OVERLAYTXT, Radius) },
		{ "Red", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Red) },
		{ "Green", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Green) },
		{ "Blue", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, Blue) },
		{ "NumDirections", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, NumDirections) },
		{ "LocalBlood", EXCEL_TYPE_BYTE, 0, offsetof(OVERLAYTXT, LocalBlood) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};

	sgptOverlayStats = (PTOVERLAYTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize,
		(int *)&sgnNumOverlayStats, sizeof(OVERLAYTXT));
#line 56
	D2WARN(sgnNumOverlayStats == NUM_OVERLAY_CLASSES);
}

void __fastcall OverlayTblsFree(void)
{
	DataTblsFreeBlock(sgptOverlayStats);
}

PTOVERLAYTXT __stdcall OverlayTblsGetOverlay(int eOverlay)
{
#line 77
	D2ASSERT(sgptOverlayStats);
#line 78
	D2ASSERT((UINT)eOverlay < sgnNumOverlayStats);
	return &sgptOverlayStats[eOverlay];
}

int __stdcall OverlayTblsGetFrames(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 88
	D2ASSERT(ptStats);
	return ptStats->Frames;
}

int __stdcall OverlayTblsGetPreDraw(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 99
	D2ASSERT(ptStats);
	return ptStats->PreDraw;
}

int __stdcall OverlayTblsGetXoffset(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 110
	D2ASSERT(ptStats);
	return ptStats->Xoffset;
}

int __stdcall OverlayTblsGetYoffset(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 121
	D2ASSERT(ptStats);
	return ptStats->Yoffset;
}

int __stdcall OverlayTblsGetTrans(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 132
	D2ASSERT(ptStats);
	return ptStats->Trans;
}

BOOL __stdcall OverlayTblsGetLight(int eOverlay, int *pnInitRadius, int *pnRadius, BYTE *pbRed, BYTE *pbGreen,
	BYTE *pbBlue)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 144
	D2ASSERT(ptStats);
	*pnInitRadius = ptStats->InitRadius;
	*pnRadius = ptStats->Radius;
	*pbRed = ptStats->Red;
	*pbGreen = ptStats->Green;
	*pbBlue = ptStats->Blue;
	return *pnRadius || *pnInitRadius;

}

int __stdcall OverlayTblsGet1ofN(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 161
	D2ASSERT(ptStats);
	return ptStats->nOneOfN;
}

int __stdcall OverlayTblsGetDir(int eOverlay)
{
	PTOVERLAYTXT ptStats;

	ptStats = &sgptOverlayStats[eOverlay];
#line 172
	D2ASSERT(ptStats);
	return ptStats->Dir;
}
