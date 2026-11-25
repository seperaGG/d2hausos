#include "D2gfx.h"

void __cdecl ScaleDrawText(DWORD dwX, DWORD dwY, const char *pszFormat, ...)
{
	va_list pArgs;

	va_start(pArgs, pszFormat);
	gpfnGraphics->field_c4(dwX, dwY, pszFormat, pArgs);
}

DWORD __stdcall ScaleSetDebugLightGrid(DWORD dwPalette, DWORD dwGrid, DWORD dwLightX, DWORD dwLightY)
{
	return gpfnGraphics->field_c8(dwPalette, dwGrid, dwLightX, dwLightY);
}
