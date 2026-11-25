#include <string.h>
#include "D2gfx.h"
#include "StormImports.h"
#include "CmnSubtile.h"

D2GFXENVINFO *gptCmnEnvInfo = 0;
D2GFXCMNINFO gtCmnInfo = { 0 };
static D2GFXENVINFO sgtEnvInfo = { 0 };
BYTE gaabCmnLightTables[49][256] = { 0 };
static int sgnLastPitch = 0;

static DWORD sgdwLastBlitParam = 0xFFFFFFFF;

void __fastcall CmnSubtileInit(void)
{
	CmnSubtileBuildGouraud();
	gtCmnInfo.pbBuffer = NULL;
}

void __fastcall CmnSubtileNothing(void)
{
}

void __fastcall CmnSubtileSetBuffer(BYTE *pbBuffer, int nPitch, DWORD dwHeight, DWORD dwArg2)
{
	gtCmnInfo.pbBuffer = pbBuffer;
	gtCmnInfo.dwHeight = dwHeight;
	gtCmnInfo.nPitch = nPitch;
	gtCmnInfo.field_0c = dwArg2;
	if (nPitch != sgnLastPitch)
	{
		gtCmnInfo.nPitch = nPitch;
		sgnLastPitch = nPitch;
		CmnSubtileBuildRowOffsets();
	}
	if (gdwBlitParam != sgdwLastBlitParam)
		CmnSubtileSetClip();
}

void __fastcall CmnSubtileSetEnv(D2GFXENVINFO *ptEnvInfo)
{
	int i;

	sgtEnvInfo = *ptEnvInfo;
	gptCmnEnvInfo = &sgtEnvInfo;
	for (i = 0; i < 49; i++)
		memcpy(gaabCmnLightTables[i], sgtEnvInfo.apbColorMaps[i], sizeof(gaabCmnLightTables[i]));
}

void __fastcall CmnSubtileResetEnv(void)
{
	ScaleSetEnvInfo(&sgtEnvInfo);
}

void __cdecl operator delete(void *pMemory)
{
	if (pMemory)
		StormSMemFree(pMemory, "delete", -1, 0);
}

void * __cdecl operator new(unsigned int nSize)
{
	return StormSMemAlloc(nSize, "new", -1, 0);
}
