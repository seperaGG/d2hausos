#include "D2gfx.h"

static PALETTEENTRY sgatPalette[256];

DWORD __stdcall ScaleSetPalette(PALETTEENTRY *ptEntries)
{
	if (ptEntries)
	{
		memcpy(sgatPalette, ptEntries, sizeof(sgatPalette));
		return gpfnGraphics->field_70(ptEntries);
	}
	return gpfnGraphics->field_70(sgatPalette);
}

void __stdcall ScaleSetEnvInfo(D2GFXENVINFO *ptEnvInfo)
{
	gpfnGraphics->field_74(ptEnvInfo);
	CmnSubtileSetEnv(ptEnvInfo);
}

void __stdcall ScaleSetVertexColor(DWORD dwRed, DWORD dwGreen, DWORD dwBlue)
{
	gpfnGraphics->field_78(dwRed, dwGreen, dwBlue);
}
