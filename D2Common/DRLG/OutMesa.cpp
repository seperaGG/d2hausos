#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\OutMesa.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "Drlg.h"

static const int sgcnMesaFirstDefs[3] =
{
	812, 817, 823,
};
static const int sgcnMesaSecondDefs[3] =
{
	828, 832, 832,
};

static const int sgcnChaosDefs[25] =
{
	836, 836, 836, 836, 836,
	836, 836, 861, 836, 836,
	836, 858, 862, 859, 836,
	836, 836, 860, 836, 836,
	836, 836, 857, 836, 836,
};

void __fastcall OutJungGenerateMesa(DRLGLEVEL *ptLevel)
{
	int nFirst;
	int nSecond;
	int i;
	struct OUTDOORINFO *pOut;

	if (ptLevel->nLevelId == 108)
		OutdoorsMarkOutline(ptLevel);
	else
	{
		OutdoorsMarkOutline(ptLevel);
		OutdoorsMarkInterior(ptLevel);
	}
	if ((int)ptLevel->nLevelId < 104)
		return;
	if ((int)ptLevel->nLevelId > 106)
	{
		if (ptLevel->nLevelId == 108)
		{
			for (i = 0; i < 25; i++)
				OutdoorsStampPreset(ptLevel, i % 5 * 3, i / 5 * 3, sgcnChaosDefs[i], -1, FALSE);
		}
		return;
	}
	{
		pOut = ptLevel->field_38.pOutdoor;
		if (pOut->dwFlags & 4194304)
			OutdoorsStampPreset(ptLevel, 0, 1, 798, -1, FALSE);
		if (pOut->dwFlags & 8388608)
			OutdoorsStampPreset(ptLevel, 0, 4, 798, -1, FALSE);
		OutdoorsRunLvlSubPass(ptLevel, 1, 799);
		OutdoorsRunLvlSubPass(ptLevel, 2, 799);
		OutdoorsRunLvlSubPass(ptLevel, 3, 799);
		if (ptLevel->nLevelId == 106)
			OutdoorsPlacePreset(ptLevel, 811, -1, 0, 15);
		nFirst = sgcnMesaFirstDefs[ptLevel->nLevelId - 104];
		nSecond = sgcnMesaSecondDefs[ptLevel->nLevelId - 104];
		OutdoorsPlacePreset(ptLevel, nFirst, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 1, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 1, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 2, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 2, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 3, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 3, -1, 0, 15);
		if (ptLevel->nLevelId == 105)
			OutdoorsPlacePreset(ptLevel, 822, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 4, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 4, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 4, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nFirst + 4, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 1, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 1, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 2, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 2, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 3, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 3, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 3, -1, 0, 15);
		OutdoorsPlacePreset(ptLevel, nSecond + 3, -1, 0, 15);
	}
}
