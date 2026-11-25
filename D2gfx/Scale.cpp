#define D2_THIS_FILE "C:\\D2\\Source\\D2Gfx\\SRC\\Scale.cpp"
#include "D2gfx.h"
#include "D2Macros.h"

static int sgnPerspectiveX;
static int sgnPerspectiveY;

void __stdcall ScaleSetPerspectiveCenter(int nX, int nY)
{
#line 96
	D2ASSERT(gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE || gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL ||
		gtGfxSystem.eDispType == DISPLAYTYPE_RAVE || gtGfxSystem.eDispType == DISPLAYTYPE_DIRECT3D);
#line 98
	D2ASSERT(gtGfxOptions.fPerspective);
	sgnPerspectiveX = nX;
	sgnPerspectiveY = nY;
	gpfnGraphics->field_60(nX, nY);
}

BOOL __stdcall ScaleTestPerspectiveRange(int nX, int nY)
{
	int nDistance;

#line 115
	D2ASSERT(gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE || gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL ||
		gtGfxSystem.eDispType == DISPLAYTYPE_RAVE || gtGfxSystem.eDispType == DISPLAYTYPE_DIRECT3D);
#line 117
	D2ASSERT(gtGfxOptions.fPerspective);
	nDistance = nX - sgnPerspectiveX;
	if (nDistance < 0)
		nDistance = -nDistance;
	if (nDistance > 3276800)
		return FALSE;
	nDistance = nY - sgnPerspectiveY;
	if (nDistance < 0)
		nDistance = -nDistance;
	return nDistance <= 3276800;
}

void __stdcall ScaleProjectPoint(DWORD dwX, DWORD dwY, DWORD dwZ, DWORD dwScreenX, DWORD dwScreenY)
{
#line 141
	D2ASSERT(gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE || gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL ||
		gtGfxSystem.eDispType == DISPLAYTYPE_RAVE || gtGfxSystem.eDispType == DISPLAYTYPE_DIRECT3D);
#line 143
	D2ASSERT(gtGfxOptions.fPerspective);
	gpfnGraphics->field_64(dwX, dwY, dwZ, dwScreenX, dwScreenY);
}

void __stdcall ScaleProjectPointDepth(DWORD dwX, DWORD dwY, DWORD dwZ, DWORD dwScreenX, DWORD dwScreenY,
	DWORD dwFar)
{
#line 154
	D2ASSERT(gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE || gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL ||
		gtGfxSystem.eDispType == DISPLAYTYPE_RAVE || gtGfxSystem.eDispType == DISPLAYTYPE_DIRECT3D);
#line 156
	D2ASSERT(gtGfxOptions.fPerspective);
	gpfnGraphics->field_68(dwX, dwY, 0, dwScreenX, dwScreenY, dwFar);
}

DWORD __fastcall ScaleResetPerspectiveDepth(void)
{
#line 170
	D2ASSERT(gtGfxSystem.eDispType == DISPLAYTYPE_GLIDE || gtGfxSystem.eDispType == DISPLAYTYPE_OPENGL ||
		gtGfxSystem.eDispType == DISPLAYTYPE_RAVE || gtGfxSystem.eDispType == DISPLAYTYPE_DIRECT3D);
#line 172
	D2ASSERT(gtGfxOptions.fPerspective);
	return gpfnGraphics->field_6c();
}
