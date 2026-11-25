#include "D2gfx.h"

DWORD __stdcall WindowDrawWallOpaque(DWORD dwTile, DWORD dwX, DWORD dwY, DWORD dwLight, DWORD dwBoundaryMode)
{
	return gpfnGraphics->field_98(dwTile, dwX, dwY, dwLight, dwBoundaryMode);
}

DWORD __stdcall WindowDrawWallAlpha(DWORD dwTile, DWORD dwX, DWORD dwY, DWORD dwLight, DWORD dwBoundaryMode,
	DWORD dwAlpha)
{
	return gpfnGraphics->field_9c(dwTile, dwX, dwY, dwLight, dwBoundaryMode, dwAlpha);
}

DWORD __stdcall WindowDrawWallOneLight(DWORD dwTile, DWORD dwX, DWORD dwY, DWORD dwLight,
	DWORD dwBoundaryMode)
{
	return gpfnGraphics->field_a0(dwTile, dwX, dwY, dwLight, dwBoundaryMode);
}
