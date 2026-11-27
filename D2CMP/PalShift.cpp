#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\PalShift.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 9

#define NUM_ICLASSES		9
#define NUM_ITRANSFORMS		21

static BYTE sgabItemTransforms[NUM_ICLASSES][5 * NUM_ITRANSFORMS][256];

static void __fastcall PalShiftLoadClass(const char *pszName, int eClass);

void __fastcall PalShiftLoad(void)
{
	PalShiftLoadClass("grey", 1);
	PalShiftLoadClass("grey2", 2);
	PalShiftLoadClass("gold", 3);
	PalShiftLoadClass("brown", 4);
	PalShiftLoadClass("greybrown", 5);
	PalShiftLoadClass("invgrey", 6);
	PalShiftLoadClass("invgrey2", 7);
	PalShiftLoadClass("invgreybrown", 8);
}

static void __fastcall PalShiftLoadClass(const char *pszName, int eClass)
{
	char szPath[256];
	HANDLE hFile;

	D2ASSERT(sizeof(sgabItemTransforms[0]) >= NUM_ITRANSFORMS * 256);
	strcpy(szPath, "Data\\Global\\Items\\Palette\\");
	strcat(szPath, pszName);
	strcat(szPath, ".dat");
	if (!ArchiveOpenFile(NULL, szPath, &hFile, TRUE))
	{
#line 56
		D2WARN(0);
		memset(sgabItemTransforms[eClass], 0, NUM_ITRANSFORMS * 256);
		return;
	}
	ArchiveReadFile(NULL, hFile, sgabItemTransforms[eClass], NUM_ITRANSFORMS * 256);
	ArchiveCloseFile(NULL, hFile);
}

static int sgnPalShiftLevel = 1;

void WINAPI PalShiftSetLevel(int nLevel)
{
	if (nLevel > 0 && nLevel <= 5)
		sgnPalShiftLevel = nLevel;
}

BYTE * WINAPI PalShiftGetTransform(int eClass, int eTransform)
{
#line 100
	D2ASSERT(eClass < NUM_ICLASSES && eTransform < NUM_ITRANSFORMS);
	if (!eClass || (eClass > 2 && eClass <= 4))
		return NULL;
	return sgabItemTransforms[eClass][eTransform];
}
