#include "D2gfx.h"

void __stdcall ScaleDrawCelPerspective(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor, DWORD dwMode,
	DWORD dwBoundary, DWORD dwPreMap)
{
	gpfnGraphics->field_80(dwCelContext, dwX, dwY, dwColor, dwMode, dwBoundary, dwPreMap);
}

DWORD __stdcall ScaleDrawCel(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor, DWORD dwMode,
	DWORD dwPreMap)
{
	return gpfnGraphics->field_84(dwCelContext, dwX, dwY, dwColor, dwMode, dwPreMap);
}

void __stdcall ScaleDrawCelPalette(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor, DWORD dwMode,
	DWORD dwPaletteIndex)
{
	gpfnGraphics->field_88(dwCelContext, dwX, dwY, dwColor, dwMode, dwPaletteIndex);
}

void __stdcall ScaleDrawSpriteRows(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwFromBottom, DWORD dwRows,
	DWORD dwMode)
{
	if (dwRows)
		gpfnGraphics->field_8c(dwCelContext, dwX, dwY, dwFromBottom, dwRows, dwMode);
}

void __stdcall ScaleDrawSpriteShadow(DWORD dwCelContext, int nX, DWORD dwY)
{
	gpfnGraphics->field_90(dwCelContext, nX + 2, dwY);
}

DWORD __stdcall WindowDrawCelOutline(DWORD dwCelContext, DWORD dwX, DWORD dwY, DWORD dwColor)
{
	return gpfnGraphics->field_94(dwCelContext, dwX, dwY, dwColor);
}

DWORD __stdcall WindowWarpBlock(DWORD dwX, DWORD dwY)
{
	return gpfnGraphics->field_cc(dwX, dwY);
}
