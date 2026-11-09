#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\gdiSmack.cpp"
#include "gdiCore.h"
#include "D2GdiImports.h"
#include "D2Macros.h"

struct SMACK
{
 	DWORD		unobserved_000;
 	DWORD		dwWidth;
 	DWORD		dwHeight;
 	DWORD		dwNumFrames;
 	BYTE		unobserved_010[104 - 16];
 	DWORD		dwNewPalette;
 	BYTE		abPalette[768];
 	BYTE		unobserved_36c[884 - 876];
 	DWORD		dwFrameNum;
};

struct GDISMACKVIDEO
{
 	SMACK		*ptSmack;
 	int			nX;
 	int			nY;
};

static BYTE sgabSmackColorMap[256] = { 0 };
static BYTE *sgpbSmackFrame = NULL;
static DWORD sgfSmackActive = FALSE;
static DWORD sgfSmackColorMap = FALSE;

void WINAPI GdiSmackNoop(void)
{
}

DWORD __fastcall GdiDrawTile(D2GDITILE *ptTile, DWORD dwVertices, int nX, DWORD dwY, DWORD dwTileX,
	DWORD dwTileY, DWORD dwAlpha, int nBoundaryMode, DWORD dwFilter)
{
	DWORD dwResult;

	dwResult = D2CMPTileCacheLoadTile(ptTile, 1);
	if (dwResult)
	{
		nX -= 80;
		if (nBoundaryMode == -160)
			nX -= 160;
		else if (nBoundaryMode == 160)
			nX += nBoundaryMode;
		dwResult = gptBlitTable->pfnDrawTile(ptTile, nX, dwY, dwVertices);
	}
	return dwResult;
}

void __fastcall GdiSmackClose(GDISMACKVIDEO *ptVideo);

void __fastcall GdiSmackOpen(const char *pszFileName, GDISMACKVIDEO *ptVideo)
{
	SMACK *ptSmack;
	SMACK *ptInfo;

	if (sgfSmackActive)
	{
		ptVideo->ptSmack = NULL;
		return;
	}
	if (ptVideo->ptSmack)
		return;
	ptSmack = (SMACK *)SmackOpen((DWORD)pszFileName, 1040384, (DWORD)-1);
	ptVideo->ptSmack = ptSmack;
	if (!ptSmack)
	{
		GdiSmackClose(ptVideo);
		return;
	}
	ptInfo = ptSmack;
#line 74
	sgpbSmackFrame = (BYTE *)D2ALLOC2(ptInfo->dwWidth * ptSmack->dwHeight);
	memset(sgpbSmackFrame, 0, ptInfo->dwWidth * ptSmack->dwHeight);
	SmackToBuffer((DWORD)ptVideo->ptSmack, 0, 0, ptSmack->dwWidth, ptSmack->dwHeight,
		(DWORD)sgpbSmackFrame,
		0);
	sgfSmackActive = TRUE;
	sgfSmackColorMap = FALSE;
}

void __fastcall GdiSmackUpdate(GDISMACKVIDEO *ptVideo)
{
	SMACK *ptSmack;
	BYTE *pbSrc;
	BYTE *pbDest;
	BYTE *pbS;
	BYTE *pbD;
	DWORD dwRows;
	DWORD x;
	int i;

	ptSmack = ptVideo->ptSmack;
	if (!ptSmack)
		return;
	if (!SmackWait((DWORD)ptSmack))
	{
		if (!ghWnd)
			return;
		if (ptSmack->dwNewPalette)
		{
			for (i = 0; i < 256; i++)
				sgabSmackColorMap[i] = (BYTE)D2CMPPaletteFindNearest(gatPaletteEntries, 256,
					ptSmack->abPalette[i * 3], ptSmack->abPalette[i * 3 + 1], ptSmack->abPalette[i * 3 + 2]);
			sgfSmackColorMap = TRUE;
		}
		SmackDoFrame((DWORD)ptSmack);
		if (ptSmack->dwFrameNum == ptSmack->dwNumFrames)
			GdiSmackClose(ptVideo);
		else
			SmackNextFrame((DWORD)ptSmack);
	}
	if (gfSuspended)
		return;
	if (!sgfSmackColorMap)
		return;
	ptSmack = ptVideo->ptSmack;
	pbSrc = sgpbSmackFrame;
	pbDest = gpbOffscreenBuffer + gnBufferWidth * (ptVideo->nY - (int)ptSmack->dwHeight) + ptVideo->nX;
	dwRows = ptSmack->dwHeight;
	while (dwRows--)
	{
		pbS = pbSrc;
		pbD = pbDest;
		x = ptSmack->dwWidth;
		while (x--)
			*pbD++ = sgabSmackColorMap[*pbS++];
		pbDest += gnBufferWidth;
		pbSrc += ptSmack->dwWidth;
	}
}

void __fastcall GdiSmackClose(GDISMACKVIDEO *ptVideo)
{
	if (sgpbSmackFrame)
	{
#line 156
		D2FREE2(sgpbSmackFrame);
		sgpbSmackFrame = NULL;
	}
	if (ptVideo->ptSmack)
	{
		SmackClose((DWORD)ptVideo->ptSmack);
		ptVideo->ptSmack = NULL;
	}
	sgfSmackActive = FALSE;
}
