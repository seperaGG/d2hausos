#include "D2gfx.h"

DWORD __fastcall D2GfxBeginFrame(void)
{
	return gpfnGraphics->field_18();
}

BOOL __fastcall D2GfxEndFrameAndPresent(void)
{
	gpfnGraphics->field_1c();
	gpfnGraphics->field_20();
	return TRUE;
}

DWORD __stdcall D2GfxReadScreenRgb(DWORD dwArg)
{
	return gpfnGraphics->field_28(dwArg);
}

BOOL __fastcall D2GfxTestDisplayTypeGlideOrUp(void)
{
	return gtGfxSystem.eDispType >= DISPLAYTYPE_GLIDE;
}

void __stdcall ScaleDrawDiamond(DWORD dwRect, BYTE bColor)
{
	gpfnGraphics->field_a4(dwRect, bColor);
}

void __stdcall ScaleDrawFrame(DWORD dwRect, BYTE bColor)
{
	gpfnGraphics->field_a8(dwRect, bColor);
}

void __stdcall ScaleFillRect(DWORD dwRect, BYTE bColor)
{
	gpfnGraphics->field_ac(dwRect, bColor);
}

void __stdcall ScaleFillSquare(DWORD dwCenter, BYTE bSize, DWORD dwColor)
{
	gpfnGraphics->field_b0(dwCenter, bSize, dwColor);
}

void __stdcall ScaleFillSpan(DWORD dwX1, DWORD dwY1, DWORD dwX2, DWORD dwY2, DWORD dwColor,
	DWORD dwMode)
{
	gpfnGraphics->field_b4(dwX1, dwY1, dwX2, dwY2, dwColor, dwMode);
}

DWORD __stdcall ScaleFillSpanSelect(DWORD dwX1, DWORD dwY1, DWORD dwX2, DWORD dwY2, DWORD dwColor,
	DWORD dwAlpha)
{
	return gpfnGraphics->field_b8(dwX1, dwY1, dwX2, dwY2, dwColor, dwAlpha);
}

void __stdcall ScaleDrawLine(DWORD dwX1, DWORD dwY1, DWORD dwX2, DWORD dwY2, DWORD dwColor,
	DWORD dwAlpha)
{
	gpfnGraphics->field_bc(dwX1, dwY1, dwX2, dwY2, dwColor, dwAlpha);
}

void __stdcall ScaleClearBackBuffer(DWORD dwArg)
{
	gpfnGraphics->field_c0(dwArg);
}

void __stdcall ScaleDrawTile(DWORD dwTile, DWORD dwVertices, DWORD dwX, DWORD dwY, DWORD dwTileX,
	DWORD dwTileY, DWORD dwAlpha, DWORD dwBoundaryMode, DWORD dwFilter)
{
	gpfnGraphics->field_7c(dwTile, dwVertices, dwX, dwY, dwTileX, dwTileY, dwAlpha, dwBoundaryMode, dwFilter);
}
