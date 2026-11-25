#include "D2gfx.h"

static int sgnScaleTarget = 256;
static int sgnScale = 256;

void __fastcall ScaleApply(void)
{
	sgnScale = sgnScaleTarget;
	gpfnGraphics->field_54(sgnScale);
}

int __fastcall ScaleGet(void)
{
	return sgnScale;
}

void __fastcall ScaleReset(void)
{
	sgnScale = 256;
	gpfnGraphics->field_54(sgnScale);
}

void __fastcall ScaleStepUp(void)
{
	if (sgnScaleTarget < 512)
	{
		sgnScaleTarget += 32;
		if (sgnScaleTarget > 512)
			sgnScaleTarget = 512;
	}
}

void __fastcall ScaleStepDown(void)
{
	if (sgnScaleTarget > 256)
	{
		sgnScaleTarget -= 32;
		if (sgnScaleTarget < 256)
			sgnScaleTarget = 256;
	}
}
