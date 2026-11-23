#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DRLG\\Preset.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "../hell/Archive.h"
#include "Drlg.h"

static int scanTagTable[4][135] =
{
		{
			12, 37, 39, 35, 36, 5, 17, 18, 19,
			20, 21, 22, 30, 70, 70, 69, 69, 29,
			31, 33, 34, 37, 61, 65, 66, 8, 26,
			28, 82, 2, 81, 84, 83, 78, 61, 103,
			108, 119, 417, 130, 159, 163, 169, 160, 161,
			162, 104, 105, 106, 107, 179, 180, 119, 157,
			247, 248, 155, 174, 175, 139, 140, 141, 144,
			6, 240, 241, 242, 54, 55, 56, 57, 58,
			171, 178, 239, 245, 250, 111, 138, 132, 164,
			165, 77, 85, 86, 262, 263, 264, 265, 50,
			51, 79, 53, 1, 3, 7, 46, 38, 256,
			257, 258, 129, 267, 268, 269, 418, 351, 352,
			353, 374, 385, 397, 321, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			74, 37, 192, 304, 305, 306, 101, 102, 78,
			103, 156, 417, 132, 129, 357, 153, 121, 122,
			229, 230, 196, 267, 261, 149, 269, 4, 9,
			52, 94, 95, 142, 143, 5, 6, 87, 88,
			146, 146, 147, 148, 240, 241, 242, 243, 176,
			177, 198, 246, 29, 160, 161, 162, 273, 283,
			85, 86, 109, 116, 134, 135, 136, 150, 151,
			172, 173, 279, 280, 281, 282, 166, 167, 113,
			137, 89, 104, 105, 106, 107, 154, 171, 178,
			270, 271, 272, 266, 274, 244, 284, 288, 298,
			289, 296, 297, 287, 286, 285, 290, 291, 292,
			293, 294, 295, 133, 303, 299, 300, 301, 302,
			418, 354, 419, 314, 315, 316, 317, 323, 322,
			110, 112, 114, 355, 356, 357, 351, 352, 353,
			152, 374, 387, 389, 390, 391, 388, 397, 402,
		},
		{
			117, 237, 417, 130, 102, 37, 160, 161, 162,
			104, 105, 106, 107, 194, 195, 193, 207, 211,
			210, 234, 214, 215, 213, 228, 216, 227, 217,
			235, 218, 219, 220, 221, 223, 224, 267, 269,
			418, 170, 325, 184, 190, 191, 197, 199, 200,
			201, 202, 206, 278, 120, 130, 326, 158, 271,
			272, 327, 328, 329, 330, 331, 332, 333, 334,
			335, 336, 5, 6, 176, 240, 241, 181, 183,
			246, 185, 186, 187, 188, 203, 204, 205, 208,
			209, 169, 323, 324, 196, 212, 225, 244, 351,
			352, 353, 360, 361, 362, 365, 251, 252, 208,
			283, 367, 366, 368, 341, 342, 343, 344, 374,
			370, 378, 379, 386, 397, 405, 407, 406, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
		{
			238, 417, 267, 269, 418, 410, 410, 410, 345,
			346, 347, 348, 349, 350, 351, 352, 353, 358,
			359, 363, 259, 373, 372, 374, 236, 249, 226,
			231, 232, 93, 97, 123, 124, 96, 225, 233,
			222, 125, 126, 127, 128, 375, 376, 254, 253,
			342, 255, 392, 393, 394, 395, 396, 398, 397,
			399, 401, 400, 380, 383, 384, 296, 297, 403,
			102, 408, 409, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0,
		},
};

int __stdcall PresetTagTableCountRow(BYTE bRow)
{
	int i;
	int nCount;

	nCount = 0;
	for (i = 0; i < 135; i++)
	{
		if (scanTagTable[bRow][i] == 0)
			break;
		nCount++;
	}
	return nCount;
}

int __stdcall PresetTagTableGetCell(BYTE bRow, int nCol)
{
	return scanTagTable[bRow][nCol];
}

static int sgcnPresetMonsterRemap[4][60] =
{
	{
		147, 146, 148, 149, 152, 150, 179, 155, 154, 156, 427, 428,
		415, 179, 185, 412, 411, 410, 410, 413, 266, 152, 272, 273,
		422, 424, 427, 428, 327, 328, 434, 435, 436, 437, 438, 439,
		440, 441, 442, 443, 444, 445, 446, 447, 458, 477, 478, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		175, 176, 177, 178, 410, 410, 410, 410, 198, 199, 202, 210,
		200, 201, 412, 411, 410, 250, 448, 211, 244, 413, 195, 196,
		203, 204, 205, 418, 320, 425, 426, 429, 430, 410, 273, 327,
		328, 434, 435, 436, 437, 371, 372, 410, 410, 410, 449, 450,
		451, 452, 453, 454, 455, 457, 377, 378, 228, 251, 4, 0,
	},
	{
		245, 413, 294, 296, 252, 253, 254, 255, 264, 419, 420, 421,
		423, 327, 328, 297, 431, 434, 435, 436, 437, 366, 242, 354,
		401, 359, 432, 433, 459, 460, 461, 462, 463, 464, 465, 466,
		467, 468, 469, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		413, 327, 328, 434, 435, 436, 437, 367, 393, 392, 405, 257,
		153, 256, 243, 470, 471, 472, 473, 474, 475, 476, 403, 404,
		309, 246, 408, 479, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
};

int __stdcall PresetMonsterRemapCountRow(BYTE bRow)
{
	int i;
	int nCount;

	nCount = 0;
	for (i = 0; i < 60; i++)
	{
		if (sgcnPresetMonsterRemap[bRow][i] == 0)
			break;
		nCount++;
	}
	return nCount;
}

int __stdcall PresetMonsterRemapGetCell(BYTE bRow, int nCol)
{
	return sgcnPresetMonsterRemap[bRow][nCol];
}

static const char *sgpszPresetItemCodes[] =
{
	"HDM",
};

int __fastcall PresetConvertOldCell(int nValue);

void __fastcall PresetReadFile(PTPRESETDS1 ptFile, struct HD2ARCHIVE__ *hArchive, const char *pszFileName)
{
	int *pdw;
	int nVersion;
	int nAct;
	int nCells;
	int nCount;
	int *pnCells;
	int nType;
	int nId;
	int nMode;
	int nX;
	int nY;
	DWORD dwFlags;
	PTPRESETUNIT ptUnit;
	int nNodes;
	int i;
	int k;

#line 853
	pdw = (int *)ArchiveAllocBufferAndRead(hArchive, pszFileName, NULL, D2_THIS_FILE, __LINE__);
	ptFile->pnBuffer = pdw;
	nAct = 0;
	nVersion = *pdw++;
	ptFile->nSizeX = *pdw++;
	ptFile->nSizeY = *pdw++;
	if (nVersion >= 8)
		nAct = *pdw++;
	ptFile->field_00 = 0;
	if (nVersion >= 10)
		ptFile->field_00 = *pdw++;
	if (nVersion >= 3)
	{
		nCount = *pdw++;
		for (i = 0; i < nCount; i++)
			pdw = (int *)((char *)pdw + StringLength((LPCTSTR)pdw) + 1);
	}
	nCells = (ptFile->nSizeY + 1) * (ptFile->nSizeX + 1);
	if (nVersion >= 9 && nVersion < 14)
		pdw += 2;
	if (nVersion >= 4)
	{
		ptFile->nWallLayers = *pdw++;
		if (nVersion >= 16)
			ptFile->nFloorLayers = *pdw++;
		else
			ptFile->nFloorLayers = 1;
		for (i = 0; i < ptFile->nWallLayers; i++)
		{
			ptFile->apnWalls[i] = pdw;
			pdw += nCells;
			ptFile->apnOrientations[i] = pdw;
			pdw += nCells;
		}
		for (i = 0; i < ptFile->nFloorLayers; i++)
		{
			ptFile->apnFloors[i] = pdw;
			pdw += nCells;
		}
	}
	else
	{
		ptFile->apnWalls[0] = pdw;
		pdw += nCells;
		ptFile->nWallLayers = 1;
		ptFile->apnFloors[0] = pdw;
		pdw += nCells;
		ptFile->apnOrientations[0] = pdw;
		pdw += nCells;
		ptFile->field_48 = pdw;
		pdw += nCells;
	}
	if (nVersion < 7)
	{
		for (i = 0; i < ptFile->nWallLayers; i++)
		{
			pnCells = ptFile->apnOrientations[i];
			for (k = 0; k < nCells; k++)
				pnCells[k] = PresetConvertOldCell(pnCells[k]);
		}
	}
	ptFile->pnShadows = pdw;
	pdw += nCells;
	if (ptFile->field_00 > 0 && ptFile->field_00 <= 2)
	{
		ptFile->field_48 = pdw;
		pdw += nCells;
	}

	if (nVersion > 1)
	{
		nCount = *pdw++;
		if (nCount)
		{
			for (i = 0; i < nCount; i++)
			{
				nType = *pdw++;
				nId = *pdw++;
				nMode = 0;
				switch (nType)
				{
				case 1:
					if (nVersion > 4)
					{
						nId = sgcnPresetMonsterRemap[nAct][nId];
						if (nAct == 2)
						{
							if (nId == 297)
							{
								nId = 382;
								nType = 2;
								break;
							}
							if (nId == 366)
							{
								nId = 404;
								nType = 2;
								break;
							}
						}
					}
					nMode = 1;
					if (nId == 410)
						nId = -1;
					break;
				case 2:
					if (nId == 410)
						nId = -1;
					else if (nVersion > 5)
					{
						if (nId < 135)
							nId = scanTagTable[nAct][nId];
						else
							nId = -1;
					}
					break;
				case 4:
					if (nVersion > 4)
					{
						char szCode[4];
						DWORD dwCode;
						BYTE *pbCode;

						strncpy(szCode, sgpszPresetItemCodes[nId], 4);
						szCode[3] = '\0';
						for (k = 0; k < 3; k++)
							szCode[k] = (char)tolower(szCode[k]);
						dwCode = 0;
						pbCode = (BYTE *)&dwCode;
						pbCode[0] = szCode[0];
						if (!szCode[0])
							pbCode[0] = ' ';
						pbCode[1] = szCode[1];
						if (!szCode[1])
							pbCode[1] = ' ';
						pbCode[2] = szCode[2];
						if (!szCode[2])
							pbCode[2] = ' ';
						pbCode[3] = szCode[3];
						if (!szCode[3])
							pbCode[3] = ' ';
						D2CommonItemTblsFindItemByCode(dwCode, &nId);
					}
					nMode = 3;
					break;
				}
				nX = *pdw++;
				nY = *pdw++;
				dwFlags = 0;
				if (nVersion > 5)
					dwFlags = *pdw++;
				if ((nType != 1 || nVersion > 4) && nId != -1)
				{
					ptUnit = DrlgRoomNewPresetUnit(NULL, NULL, nType, nId, nMode, nX, nY);
					ptUnit->ptNext = ptFile->ptUnits;
					ptFile->ptUnits = ptUnit;
					ptUnit->dwFlags |= dwFlags;
				}
			}
		}
	}

	if (nVersion >= 12 && ptFile->field_00 > 0 && ptFile->field_00 <= 2)
	{
		if (nVersion >= 18)
			pdw++;
		ptFile->nClusters = *pdw++;
#line 1187
		ptFile->ptClusters = (PTPRESETCLUSTER)D2ALLOCPOOL(NULL, ptFile->nClusters * sizeof(PRESETCLUSTER));
		for (i = 0; i < ptFile->nClusters; i++)
		{
			ptFile->ptClusters[i].nX = *pdw++;
			ptFile->ptClusters[i].nY = *pdw++;
			ptFile->ptClusters[i].nWidth = *pdw++;
			ptFile->ptClusters[i].nHeight = *pdw++;
			if (nVersion >= 13)
				ptFile->ptClusters[i].nAlternates = *pdw++;
		}
	}

	if (nVersion >= 14)
	{
		nCount = *pdw++;
		for (i = 0; i < nCount; i++)
		{
			nNodes = *pdw++;
			nX = *pdw++;
			nY = *pdw++;
			if (nNodes)
			{
				for (ptUnit = ptFile->ptUnits; ptUnit; ptUnit = ptUnit->ptNext)
				{
					if (ptUnit->nX == nX && ptUnit->nY == nY)
						break;
				}
#line 1238
				ptUnit->ptPath = (PTPRESETFILELIST)D2ALLOCPOOL(NULL, sizeof(PRESETFILELIST));
#line 1239
				ptUnit->ptPath->pArray = (BYTE *)D2ALLOCPOOL(NULL, nNodes * sizeof(PRESETPATHNODE));
				ptUnit->ptPath->nNodes = nNodes;
				for (k = 0; k < nNodes; k++)
				{
					((PTPRESETPATHNODE)ptUnit->ptPath->pArray)[k].nX = *pdw++;
					((PTPRESETPATHNODE)ptUnit->ptPath->pArray)[k].nY = *pdw++;
					if (nVersion >= 15)
						((PTPRESETPATHNODE)ptUnit->ptPath->pArray)[k].nAction = *pdw++;
					else
						((PTPRESETPATHNODE)ptUnit->ptPath->pArray)[k].nAction = 1;
				}
			}
		}
	}
}

struct PRESETCACHE
{
 	char		szFileName[MAX_PATH];
 	LONG		lRefCount;
 	PTPRESETDS1	ptFile;
 	struct PRESETCACHE	*ptNext;
};
typedef struct PRESETCACHE PRESETCACHE, *PTPRESETCACHE;

static PTPRESETCACHE sgptPresetCache;
LPCRITICAL_SECTION gptSubsFilesCS;

static void __fastcall sLockPresetCache(void)
{
	if (gptSubsFilesCS)
		EnterCriticalSection(gptSubsFilesCS);
}

static void __fastcall sUnlockPresetCache(void)
{
	if (gptSubsFilesCS)
		LeaveCriticalSection(gptSubsFilesCS);
}

void __fastcall PresetLoadFile(PTPRESETDS1 *pptFile, struct HD2ARCHIVE__ *hArchive, const char *pszFileName)
{
	PTPRESETCACHE ptNode;

	sLockPresetCache();
	for (ptNode = sgptPresetCache; ptNode; ptNode = ptNode->ptNext)
	{
		if (!strcmp(ptNode->szFileName, pszFileName))
		{
			InterlockedIncrement(&ptNode->lRefCount);
			*pptFile = ptNode->ptFile;
			sUnlockPresetCache();
			return;
		}
	}
#line 1288
	ptNode = (PTPRESETCACHE)D2ALLOCPOOL(NULL, sizeof(PRESETCACHE));
#line 1289
	*pptFile = (PTPRESETDS1)D2ALLOCPOOL(NULL, sizeof(PRESETDS1));
	memset(*pptFile, 0, sizeof(PRESETDS1));
	strcpy(ptNode->szFileName, pszFileName);
	ptNode->lRefCount = 1;
	ptNode->ptFile = *pptFile;
	ptNode->ptNext = sgptPresetCache;
	sgptPresetCache = ptNode;
	PresetReadFile(*pptFile, hArchive, pszFileName);
	sUnlockPresetCache();
}

static void __fastcall sFreeUnitList(void *pPool, PTPRESETUNIT ptNext)
{
	PTPRESETUNIT ptUnit;

	while (ptNext)
	{
		ptUnit = ptNext;
		ptNext = ptNext->ptNext;
		PresetUnitFree(pPool, ptUnit);
	}
}

void __fastcall PresetFreeFileData(PTPRESETDS1 ptFile)
{
	if (ptFile->pnBuffer)
	{
#line 1312
		D2FREE2(ptFile->pnBuffer);
	}
	if (ptFile->ptClusters)
	{
#line 1313
		D2FREEPOOL(NULL, ptFile->ptClusters);
	}
	sFreeUnitList(NULL, ptFile->ptUnits);
#line 1325
	D2FREEPOOL(NULL, ptFile);
}

void __fastcall PresetFreeFile(PTPRESETDS1 *pptFile)
{
	PTPRESETCACHE ptNode;
	PTPRESETCACHE ptPrev;

	if (!*pptFile)
		return;
	sLockPresetCache();
	ptPrev = NULL;
	for (ptNode = sgptPresetCache; ptNode && ptNode->ptFile != *pptFile; ptNode = ptNode->ptNext)
		ptPrev = ptNode;
	D2_OPAQUE(ptNode);
	InterlockedDecrement(&ptNode->lRefCount);
	if (ptNode && ptNode->lRefCount <= 0)
	{
		if (ptPrev)
			ptPrev->ptNext = ptNode->ptNext;
		else
			sgptPresetCache = ptNode->ptNext;
		sUnlockPresetCache();
		PresetFreeFileData(*pptFile);
#line 1376
		D2FREEPOOL(NULL, ptNode);
		*pptFile = NULL;
		return;
	}
	sUnlockPresetCache();
}

PTPRESETUNIT __fastcall PresetUnitClone(void *pPool, PTPRESETUNIT ptSource, int nX, int nY)
{
	PTPRESETUNIT ptUnit;
	DWORD i;

#line 1394
	ptUnit = (PTPRESETUNIT)D2ALLOCPOOL(pPool, sizeof(PRESETUNIT));
	for (i = 0; i < sizeof(PRESETUNIT) / sizeof(DWORD); i++)
		((DWORD *)ptUnit)[i] = 0;
	ptUnit->nUnitType = ptSource->nUnitType;
	ptUnit->nClassId = ptSource->nClassId;
	ptUnit->nMode = ptSource->nMode;
	ptUnit->nX = ptSource->nX + nX;
	ptUnit->nY = ptSource->nY + nY;
	ptUnit->dwFlags = ptSource->dwFlags;
	if (ptSource->ptPath)
	{
		ptUnit->ptPath = PresetFileListClone(pPool, ptSource->ptPath);
		for (i = 0; i < ptUnit->ptPath->nNodes; i++)
		{
			((PTPRESETPATHNODE)ptUnit->ptPath->pArray)[i].nX += nX;
			((PTPRESETPATHNODE)ptUnit->ptPath->pArray)[i].nY += nY;
		}
	}
	return ptUnit;
}

void __fastcall PresetUnitFree(void *pPool, PTPRESETUNIT ptUnit)
{
	if (ptUnit->ptPath)
		PresetFileListFree(pPool, ptUnit->ptPath);
#line 1427
	D2FREEPOOL(pPool, ptUnit);
}

PTPRESETFILELIST __fastcall PresetFileListClone(void *pPool, PTPRESETFILELIST pSource)
{
	PTPRESETFILELIST pNew;

#line 1436
	pNew = (PTPRESETFILELIST)D2ALLOCPOOL(pPool, sizeof(PRESETFILELIST));
	memset(pNew, 0, sizeof(PRESETFILELIST));
	pNew->nNodes = pSource->nNodes;
#line 1441
	pNew->pArray = (BYTE *)D2ALLOCPOOL(pPool, pNew->nNodes * sizeof(PRESETPATHNODE));
	memcpy(pNew->pArray, pSource->pArray, pNew->nNodes * sizeof(PRESETPATHNODE));
	return pNew;
}

DWORD __fastcall PresetInfoDetachFirstDword(PTPRESETINFO pInfo, DWORD *pdwOut)
{
	*pdwOut = pInfo->dwDef;
	pInfo->dwDef = 0;
	return *pdwOut;
}

void __fastcall PresetFileListFree(void *pPool, PTPRESETFILELIST pList)
{
#line 1471
	D2FREEPOOL(pPool, pList->pArray);
#line 1472
	D2FREEPOOL(pPool, pList);
}

void __fastcall PresetRegionAttachUnits(void *pPool, PTPRESETSUBINFO ptRegion, D2SEED *ptSeed)
{
	PTPRESETUNIT ptUnit;
	PTPRESETUNIT ptClone;
	int nX;
	int nY;

	ptUnit = ptRegion->ptFile->ptUnits;
	nX = ptRegion->tCoords.nTileX;
	nY = ptRegion->tCoords.nTileY;
	EnvTileToSubtile(&nX, &nY);
	for (; ptUnit; ptUnit = ptUnit->ptNext)
	{
		switch (ptUnit->nUnitType)
		{
		case 1:
			switch (ptUnit->nClassId)
			{
			case 204:
			case 205:
			case 371:
			case 372:
				if (SeedRand(ptSeed, 3))
					continue;
				break;
			case 434:
				if (!(SeedNext(ptSeed) & 3))
					continue;
				break;
			case 435:
				if (SeedNext(ptSeed) & 1)
					continue;
				break;
			case 436:
				if (SeedNext(ptSeed) & 3)
					continue;
				break;
			}
			break;
		case 2:
			switch (ptUnit->nClassId)
			{
			case 196:
			case 261:
				if (SeedNext(ptSeed) & 1)
					continue;
				break;
			case 418:
				if (!(SeedNext(ptSeed) & 3))
					continue;
				break;
			}
			break;
		}
		ptClone = PresetUnitClone(pPool, ptUnit, nX, nY);
		ptClone->ptNext = ptRegion->ptUnits;
		ptRegion->ptUnits = ptClone;
	}
}

static void __fastcall sPlaceRowUnits(PTPRESETSUBINFO ptRegion, void *pPool, int nIndex,
	DRLGGRID *ptGrid);

static void __fastcall sPlaceNaviUnit(PTPRESETSUBINFO ptRegion, void *pPool)
{
	PTPRESETUNIT ptUnit;
	int nX;
	int nY;

	nX = ptRegion->tCoords.nTileX + ptRegion->tCoords.nSizeTileX / 2;
	nY = ptRegion->tCoords.nTileY + ptRegion->tCoords.nSizeTileY / 2;
	EnvTileToSubtile(&nX, &nY);
	ptUnit = DrlgRoomNewPresetUnit(NULL, pPool, 1, 266, 1, nX, nY);
	ptUnit->ptNext = ptRegion->ptUnits;
	ptRegion->ptUnits = ptUnit;
}

static void __fastcall sAddRiverUnit(PTPRESETSUBINFO ptRegion, void *pPool, int nClass, int nX, int nY)
{
	PTPRESETUNIT ptUnit;

	ptUnit = DrlgRoomNewPresetUnit(NULL, pPool, 2, nClass, 0, nX, nY);
	ptUnit->dwFlags |= 1;
	ptUnit->ptNext = ptRegion->ptUnits;
	ptRegion->ptUnits = ptUnit;
}

static void __fastcall sPlaceColumnUnits(PTPRESETSUBINFO ptRegion, void *pPool, int nY, int nX)
{
	int nX2;
	int nY2;

	nX2 = 0;
	nY2 = ptRegion->ptFile->nSizeY + nY;
	EnvTileToSubtile(&nX, &nY);
	EnvTileToSubtile(&nX2, &nY2);
	for (; nY < nY2; nY += 40)
		sAddRiverUnit(ptRegion, pPool, 65, nX, nY);
}

static void __fastcall sPlaceRiverUnits(PTPRESETSUBINFO ptRegion, void *pPool)
{
	DRLGCOORDS tCoords;
	DRLGGRID tGrid;
	int anRowOffsets[256];
	int nTile;
	int i;

	tCoords.nTileX = 0;
	tCoords.nTileY = 0;
	tCoords.nSizeTileX = ptRegion->tCoords.nSizeTileX;
	tCoords.nSizeTileY = ptRegion->tCoords.nSizeTileY;
	DrlgGridInitView(&tGrid, ptRegion->ptFile->apnFloors[0], &tCoords,
		tCoords.nSizeTileX + 1, anRowOffsets);
	tCoords.nTileX = ptRegion->tCoords.nTileX;
	tCoords.nTileY = ptRegion->tCoords.nTileY;
	if (ptRegion->pRecord->Def == 27)
	{
		sPlaceColumnUnits(ptRegion, pPool, tCoords.nTileY, tCoords.nTileX);
		sPlaceRowUnits(ptRegion, pPool, -1, &tGrid);
	}
	else
	{
		for (i = 0; i < tCoords.nSizeTileX; i++)
		{
			nTile = DrlgGridGet(&tGrid, i, 0);
			if ((BYTE)(((DWORD)nTile >> 20) & 63) != 2 || (BYTE)((DWORD)nTile >> 8) != 24)
				continue;
			sPlaceColumnUnits(ptRegion, pPool, tCoords.nTileY, i + tCoords.nTileX + 1);
			sPlaceRowUnits(ptRegion, pPool, i, &tGrid);
			i += 4;
		}
	}
	DrlgGridClear(&tGrid);
}

void __fastcall PresetRoomLoadUnits(ROOM *ptRoom)
{
	PTPRESETSUBINFO ptRegion;
	void *pPool;

	ptRegion = ptRoom->field_20.ptPreset->pSubInfo;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	if (!ptRegion->ptFile)
	{
		PresetLoadFile(&ptRegion->ptFile, ptRoom->ptLevel->ptAct->hArchive,
			ptRegion->pRecord->File[ptRegion->nIndex]);
		PresetRegionAttachUnits(pPool, ptRegion, &ptRoom->tSeed);
	}
	if (ptRegion->dwDefUnitsPending == 1)
	{
		ptRegion->dwDefUnitsPending = 0;
		switch (ptRegion->nDef)
		{
		case 4:
		case 5:
		case 6:
		case 7:
			if (ptRoom->ptLevel->nLevelId == 2 && ptRegion->nIndex == 3)
				sPlaceNaviUnit(ptRegion, pPool);
			break;
		case 1:
		case 3:
		case 26:
		case 27:
		case 28:
		case 300:
			if (!(ptRoom->dwFlags & 262144))
				break;
			sPlaceRiverUnits(ptRegion, pPool);
			break;
		}
	}
	ptRoom->dwFlags |= 33554432;
}

static void __fastcall sPlaceRowUnits(PTPRESETSUBINFO ptRegion, void *pPool, int nIndex, DRLGGRID *ptGrid)
{
	int nRow;
	int nTile;
	int nX;
	int nY;

	for (nRow = 0; nRow < ptRegion->tCoords.nSizeTileY; nRow++)
	{
		nX = nIndex + ptRegion->tCoords.nTileX;
		nY = nRow + ptRegion->tCoords.nTileY;
		EnvTileToSubtile(&nX, &nY);
		nX -= 5;
		sAddRiverUnit(ptRegion, pPool, 40, nX, nY);
		sAddRiverUnit(ptRegion, pPool, 41, nX + 5, nY);
		sAddRiverUnit(ptRegion, pPool, 41, nX + 10, nY);
		sAddRiverUnit(ptRegion, pPool, 41, nX + 15, nY);
		sAddRiverUnit(ptRegion, pPool, 42, nX + 20, nY);
		nTile = DrlgGridGet(ptGrid, nIndex > 0 ? nIndex : 0, nRow);
		if ((BYTE)(((DWORD)nTile >> 20) & 63) == 4)
		{
			switch (((DWORD)nTile >> 8) & 255)
			{
			case 0:
			case 4:
			case 8:
			case 16:
			case 29:
			case 39:
				nRow += 3;
				break;
			}
		}
	}
}

static void __fastcall sFreeInfoGrids(void *pPool, ROOM *ptRoom);

void __fastcall PresetRoomFreeInfo(ROOM *ptRoom)
{
	PTPRESETINFO ptInfo;
	void *pPool;

	ptInfo = ptRoom->field_20.ptPreset;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	if (ptInfo)
	{
		sFreeInfoGrids(pPool, ptRoom);
		if (ptInfo->ptPoints)
		{
#line 1783
			D2FREEPOOL(pPool, ptInfo->ptPoints);
		}
#line 1786
		D2FREEPOOL(pPool, ptInfo);
		ptRoom->field_20.ptPreset = NULL;
	}
}

static void __fastcall sFreeInfoGrids(void *pPool, ROOM *ptRoom)
{
	PTPRESETINFO ptInfo;
	PTPRESETDS1 ptFile;
	int nPairs;
	int nFloors;
	int i;

	ptInfo = ptRoom->field_20.ptPreset;
	if (ptInfo && ptInfo->pSubInfo)
	{
		ptFile = ptInfo->pSubInfo->ptFile;
		if (ptFile)
		{
			nPairs = ptFile->nWallLayers;
			for (i = 0; i < nPairs; i++)
			{
				DrlgGridFree(pPool, &ptInfo->atOrientations[i]);
				DrlgGridFree(pPool, &ptInfo->atWalls[i]);
			}
			nFloors = ptFile->nFloorLayers;
			for (i = 0; i < nFloors; i++)
				DrlgGridFree(pPool, &ptInfo->atFloors[i]);
			DrlgGridFree(pPool, &ptInfo->tShadows);
		}
	}
}

void __fastcall PresetRoomFreeGrids(ROOM *ptRoom)
{
	if (ptRoom->field_20.ptPreset)
		sFreeInfoGrids(ptRoom->ptLevel->ptAct->pMemPool, ptRoom);
}

void __fastcall PresetRoomNewInfo(ROOM *ptRoom)
{
	PTPRESETINFO ptInfo;
	DWORD i;

#line 1807
	ptInfo = (PTPRESETINFO)D2ALLOCPOOL(ptRoom->ptLevel->ptAct->pMemPool, sizeof(PRESETINFO));
	for (i = 0; i < sizeof(PRESETINFO) / sizeof(DWORD); i++)
		((DWORD *)ptInfo)[i] = 0;
	ptRoom->field_20.ptPreset = ptInfo;
}

ROOM *__fastcall PresetRoomNew(DRLGLEVEL *ptLevel, PTPRESETSUBINFO ptRegion, DRLGCOORDS *ptCoords,
	DWORD dwDt1Mask, DWORD dwFlags, DWORD dwInfoFlags, PTPRESETLINK ptLink)
{
	ROOM *ptRoom;
	PTPRESETINFO ptInfo;
	struct LVLPRESTTXT *ptRecord;

	ptRoom = DrlgRoomNew(ptLevel, 2);
	ptRoom->dwDt1Mask = dwDt1Mask;
	ptRoom->dwFlags |= dwFlags;
	memcpy(&ptRoom->tCoords, ptCoords, sizeof(DRLGCOORDS));
	ptInfo = ptRoom->field_20.ptPreset;
	ptInfo->pSubInfo = ptRegion;
	ptRecord = ptRegion->pRecord;
	ptInfo->dwDef = (DWORD)ptRecord->Def;
	ptInfo->dwFlags = dwInfoFlags;
	ptInfo->ptLink = ptLink;
	if (ptLink != NULL)
	{
		ptLink->dwFlags |= 1;
	}
	if (ptInfo->pSubInfo->pRecord->Populate == 0)
	{
		ptRoom->dwFlags |= 8388608;
	}
	DrlgRoomLink(ptLevel, ptRoom);
	return ptRoom;
}

void __fastcall PresetRoomBuildTilesA(ROOM *ptRoom)
{
	PTPRESETINFO ptInfo;
	PTPRESETSUBINFO ptRegion;
	PTPRESETDS1 ptFile;
	PTPRESETUNIT ptUnit;
	PTPRESETUNIT ptPrev;
	DRLGCOORDS tRect;
	DRLGCOORDS tSub;
	void *pPool;
	int nStride;
	int nOffX;
	int nOffY;
	int i;

	ptInfo = ptRoom->field_20.ptPreset;
	ptRegion = ptInfo->pSubInfo;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	ptFile = ptRegion->ptFile;
	tRect.nTileX = ptRoom->tCoords.nTileX - ptRegion->tCoords.nTileX;
	tRect.nTileY = ptRoom->tCoords.nTileY - ptRegion->tCoords.nTileY;
	tRect.nSizeTileX = ptRoom->tCoords.nSizeTileX + 1;
	tRect.nSizeTileY = ptRoom->tCoords.nSizeTileY + 1;
	nStride = ptRegion->tCoords.nSizeTileX + 1;
	for (i = 0; i < ptFile->nWallLayers; i++)
	{
		DrlgGridAllocView(pPool, &ptInfo->atWalls[i], ptFile->apnWalls[i], &tRect, nStride);
		DrlgGridAllocView(pPool, &ptInfo->atOrientations[i], ptFile->apnOrientations[i], &tRect, nStride);
	}
	if (ptFile->nWallLayers)
		DrlgGridApplyBorder(&ptInfo->atWalls[0], 132, 0);
	for (i = 1; i < ptFile->nWallLayers; i++)
		DrlgGridApplyAll(&ptInfo->atWalls[i], i << 18, 0);
	for (i = 0; i < ptFile->nFloorLayers; i++)
	{
		DrlgGridAllocView(pPool, &ptInfo->atFloors[i], ptFile->apnFloors[i], &tRect, nStride);
		DrlgGridApplyAll(&ptInfo->atFloors[i], i << 18, 0);
	}
	DrlgGridAllocView(pPool, &ptInfo->tShadows, ptFile->pnShadows, &tRect, nStride);
	for (i = 0; i < ptFile->nFloorLayers; i++)
		DrlgGridApplyBorder(&ptInfo->atFloors[i], 132, 0);
	DrlgGridApplyBorder(&ptInfo->tShadows, 132, 0);
	ptRegion = ptRoom->field_20.ptPreset->pSubInfo;
	tSub = ptRoom->tCoords;
	EnvTileToSubtile(&tSub.nTileX, &tSub.nTileY);
	EnvTileToSubtile(&tSub.nSizeTileX, &tSub.nSizeTileY);
	ptUnit = ptRegion->ptUnits;
	ptPrev = NULL;
	nOffX = ptRoom->tCoords.nTileX * 5;
	nOffY = ptRoom->tCoords.nTileY * 5;
	while (ptUnit)
	{
		if (DrlgRectContains(&tSub, ptUnit->nX, ptUnit->nY))
		{
			ptUnit->nX -= nOffX;
			ptUnit->nY -= nOffY;
			if (ptPrev)
				ptPrev->ptNext = ptUnit->ptNext;
			else
				ptRegion->ptUnits = ptUnit->ptNext;
			ptUnit->ptNext = ptRoom->ptPresetUnits;
			ptRoom->ptPresetUnits = ptUnit;
			if (ptPrev)
				ptUnit = ptPrev->ptNext;
			else
				ptUnit = ptRegion->ptUnits;
		}
		else
		{
			ptPrev = ptUnit;
			ptUnit = ptUnit->ptNext;
		}
	}
}

void __fastcall PresetRoomGetPoints(ROOM *ptRoom, struct GAMECOORD **pptPoints, int *pnCount)
{
	PTPRESETINFO ptInfo;

	if (ptRoom->dwKind != 2)
	{
		*pptPoints = NULL;
		*pnCount = 0;
		return;
	}
	ptInfo = ptRoom->field_20.ptPreset;
	*pptPoints = ptInfo->ptPoints;
	*pnCount = ptInfo->nPoints;
}

static void __fastcall sPlaceBurialPoints(void *pPool, ROOM *ptRoom)
{
	PTPRESETINFO ptInfo;
	int nPoints;
	int nRow;
	int nCol;
	DWORD dwCell;
	BYTE bStyle;
	BYTE bSeq;

	ptInfo = ptRoom->field_20.ptPreset;
	nPoints = 0;
	if (ptInfo->ptPoints)
		return;
#line 1964
	ptInfo->ptPoints = (GAMECOORD *)D2ALLOCPOOL(pPool, 6 * sizeof(GAMECOORD));
	for (nRow = 0; nRow < ptRoom->tCoords.nSizeTileY; nRow++)
	{
		for (nCol = 0; nCol < ptRoom->tCoords.nSizeTileX; nCol++)
		{
			dwCell = (DWORD)DrlgGridGet(&ptInfo->atWalls[0], nCol, nRow);
			bStyle = (BYTE)((dwCell >> 20) & 63);
			bSeq = (BYTE)(dwCell >> 8);
			if (bStyle == 10 && bSeq >= 23 && bSeq <= 27)
			{
				if (nPoints < 6)
				{
					int nX;
					int nY;

					nX = ptRoom->tCoords.nTileX + nCol;
					nY = ptRoom->tCoords.nTileY + nRow;
					EnvTileToSubtile(&nX, &nY);
					nX += 2;
					nY += 2;
					ptInfo->ptPoints[nPoints].dwX = (DWORD)nX;
					ptInfo->ptPoints[nPoints].dwY = (DWORD)nY;
					nPoints++;
				}
			}
		}
	}
	ptInfo->nPoints = nPoints;
}

void __fastcall PresetRoomBuildTilesB(ROOM *ptRoom)
{
	PTPRESETINFO ptInfo;
	PTPRESETSUBINFO ptRegion;
	PTPRESETDS1 ptFile;
	PTDRLGTILEBLOCK ptTiles;
	PTDRLGTILEARRAYS ptArrays;
	DRLGGRID tWalls;
	DRLGGRID tOrient;
	int anOrientRows[256];
	int anOrientCells[1024];
	int anWallRows[256];
	int anWallCells[1024];
	void *pPool;
	BOOL fRight;
	BOOL fBottom;
	BOOL fTown;
	int nAnimate;
	int nFillBlanks;
	int nPairs;
	int nFloors;
	int i;

	RoomTileAllocBlock(ptRoom);
	ptInfo = ptRoom->field_20.ptPreset;
	fRight = FALSE;
	fBottom = FALSE;
	pPool = ptRoom->ptLevel->ptAct->pMemPool;
	ptRegion = ptInfo->pSubInfo;
	nAnimate = ptRegion->pRecord->Animate;
	nFillBlanks = ptRegion->pRecord->FillBlanks;
	if (ptRegion->pRecord->KillEdge)
	{
		fRight = ptRoom->tCoords.nTileX + ptRoom->tCoords.nSizeTileX ==
			ptRegion->tCoords.nTileX + ptRegion->tCoords.nSizeTileX;
		fBottom = ptRoom->tCoords.nTileY + ptRoom->tCoords.nSizeTileY ==
			ptRegion->tCoords.nTileY + ptRegion->tCoords.nSizeTileY;
	}
	fTown = FALSE;
	switch (ptInfo->dwDef)
	{
	case 1:
	case 108:
		fTown = TRUE;
		DrlgGridInit(&tOrient, ptRoom->tCoords.nSizeTileX + 1, ptRoom->tCoords.nSizeTileY + 1,
			anOrientCells, anOrientRows);
		DrlgGridInit(&tWalls, ptRoom->tCoords.nSizeTileX + 1, ptRoom->tCoords.nSizeTileY + 1, anWallCells,
			anWallRows);
		break;
	}
	ptFile = ptInfo->pSubInfo->ptFile;
	nPairs = ptFile->nWallLayers;
	nFloors = ptFile->nFloorLayers;
	for (i = 0; i < nFloors; i++)
	{
		RoomTileCountTiles(ptRoom, &ptInfo->atFloors[i], !i && nFillBlanks, fRight, fBottom);
		if (nAnimate)
			DrlgCountAnimTiles(ptRoom, &ptInfo->atFloors[i], NULL, 0, fRight, fBottom);
	}
	for (i = 0; i < nPairs; i++)
	{
		RoomTileCountWalls(ptRoom, &ptInfo->atWalls[i], &ptInfo->atOrientations[i], fRight, fBottom);
		RoomTileCountTiles(ptRoom, &ptInfo->atWalls[i], FALSE, fRight, fBottom);
		if (nAnimate)
			DrlgCountAnimTiles(ptRoom, &ptInfo->atWalls[i], &ptInfo->atOrientations[i], 0, fRight, fBottom);
	}
	RoomTileCountTiles(ptRoom, &ptInfo->tShadows, FALSE, fRight, fBottom);
	if (nAnimate)
		DrlgCountAnimTiles(ptRoom, &ptInfo->tShadows, NULL, 13, fRight, fBottom);
	if (fTown)
		RoomTileCountTiles(ptRoom, &tWalls, FALSE, fRight, fBottom);
	RoomTileAllocArrays(ptRoom);
	for (i = 0; i < nFloors; i++)
		RoomTileFill(ptRoom, &ptInfo->atFloors[i], NULL, !i && nFillBlanks, fRight, fBottom);
	for (i = 0; i < nPairs; i++)
		RoomTileFill(ptRoom, &ptInfo->atWalls[i], &ptInfo->atOrientations[i], FALSE, fRight, fBottom);
	RoomTileFill(ptRoom, &ptInfo->tShadows, NULL, FALSE, fRight, fBottom);
	if (fTown)
		RoomTileFill(ptRoom, &tWalls, &tOrient, FALSE, fRight, fBottom);
	if (nAnimate)
		DrlgAnimTiles(ptRoom, ptInfo->pSubInfo->pRecord->nAnimStep, ptInfo->atWalls, nPairs, ptInfo->atFloors,
			nFloors, &ptInfo->tShadows);
	ptTiles = ptRoom->ptDrlgTiles;
	ptArrays = &ptTiles->tTileArrays;
	ptArrays->nNumWalls = ptTiles->nWallCount;
	ptArrays->nNumFloors = ptTiles->nFloorCount;
	ptArrays->nNumShadows = ptTiles->dwShadowCount;
	if (ptInfo->pSubInfo->pRecord->Logicals)
		DrlgLogicBuild(ptRoom, ptInfo->atOrientations, ptInfo->atFloors, ptInfo->atWalls);
	else
		DrlgLogicBuildEmpty(ptRoom);
	if (fTown)
	{
		DrlgGridClear(&tOrient);
		DrlgGridClear(&tWalls);
	}
	if (ptRoom->ptLevel->nLevelId == 17)
		sPlaceBurialPoints(pPool, ptRoom);
}

ROOM *__fastcall PresetRegionToRooms(DRLGLEVEL *ptLevel, PTPRESETSUBINFO ptRegion, DWORD dwFlags, BOOL fWhole)
{
	DRLGGRID tGrid;
	int anRowOffsets[256];
	int anCells[1024];
	ROOM *ptRoom;
	DWORD dwDt1Mask;
	DRLGCOORDS tCoords;
	int nX;
	int nY;
	int nXEnd;
	int nYEnd;
	int nCol;
	int nRow;
	int nWidthLeft;
	int nCellFlags;
	PTPRESETLINK ptLink;
	DRLGCOORDS *ptArea;
	int nCellsX;
	int nCellsY;

	nCellsX = ptRegion->tCoords.nSizeTileX / 8 + 1;
	nCellsY = ptRegion->tCoords.nSizeTileY / 8 + 1;
	if (ptRegion->pRecord->Outdoors)
		dwFlags |= 524288;
	DrlgGridInit(&tGrid, nCellsX, nCellsY, anCells, anRowOffsets);
	PresetRegionFill(ptLevel, &tGrid, dwFlags, ptRegion, fWhole);
	dwDt1Mask = (DWORD)ptRegion->pRecord->Dt1Mask;
	ptRoom = NULL;
	ptArea = &ptRegion->tCoords;
	if (fWhole)
	{
		nCellFlags = DrlgGridGet(&tGrid, 0, 0);
		ptRoom = PresetRoomNew(ptLevel, ptRegion, ptArea, dwDt1Mask, (DWORD)nCellFlags, 1, NULL);
	}
	else
	{
		nXEnd = ptArea->nTileX + ptArea->nSizeTileX;
		nYEnd = ptArea->nTileY + ptArea->nSizeTileY;
		tCoords.nTileX = 0;
		tCoords.nTileY = 0;
		tCoords.nSizeTileX = 0;
		tCoords.nSizeTileY = 0;
		for (nY = ptArea->nTileY, nRow = 0; nY < nYEnd; nY += 8, nRow++)
		{
			for (nX = ptArea->nTileX, nCol = 0; nX < nXEnd; nX += 8, nCol++)
			{
				nWidthLeft = nXEnd - nX;
				tCoords.nTileX = nX;
				tCoords.nTileY = nY;
				if (nWidthLeft < 8)
					tCoords.nSizeTileX = nWidthLeft;
				else
					tCoords.nSizeTileX = 8;
				tCoords.nSizeTileY = nYEnd - nY < 8 ? nYEnd - nY : 8;
				nCellFlags = DrlgGridGet(&tGrid, nCol, nRow);
				ptLink = ptRegion->field_20 ? (PTPRESETLINK)DrlgGridGet(&ptRegion->tLinks, nCol, nRow)
					: NULL;
				if (tCoords.nSizeTileX && tCoords.nSizeTileY)
					ptRoom = PresetRoomNew(ptLevel, ptRegion, &tCoords, dwDt1Mask, (DWORD)nCellFlags, 0,
						ptLink);
			}
		}
	}
	DrlgGridClear(&tGrid);
	return ptRoom;
}

static void __fastcall PresetRegionFill(DRLGLEVEL *ptLevel, DRLGGRID *ptGrid, DWORD dwFlags,
	PTPRESETSUBINFO ptRegion, BOOL fWhole)
{
	DRLGACTCTX *ptAct;
	void *pPool;
	int *pnVis;
	int i;
	DWORD dwBit;
	int nLinks;
	int nScan;
	int nPops;
	PTPRESETDS1 ptFile;
	DRLGCOORDS tCoords;
	DRLGGRID tOrient;
	DRLGGRID tWalls;
	int anOrientRows[256];
	int anWallRows[256];
	int nLayer;
	int nX;
	int nY;
	int nOrient;
	int nWall;
	int nCode;
	int nSub;
	DWORD dwTop;
	int k;
	int nGridX;
	int nGridY;
	DRLGCOORDS *ptLink;
	int nMinX;
	int nMinY;
	int nMaxX;
	int nMaxY;
	PTPRESETUNIT ptUnit;

	ptAct = ptLevel->ptAct;
	pPool = ptAct->pMemPool;
	pnVis = DrlgRoomGetVis(ptAct, (int)ptLevel->nLevelId);
	dwBit = 16;
	for (i = 0; i < LEVEL_VIS_MAX; i++)
	{
		if (pnVis[i] && DrlgWarpGetWarp(ptLevel, (BYTE)i) == -1)
			dwFlags |= dwBit;
		dwBit <<= 1;
	}
	DrlgGridApplyAll(ptGrid, (int)dwFlags, 0);
	nLinks = 0;
	nScan = ptRegion->pRecord->Scan;
	nPops = ptRegion->pRecord->Pops;
	if (!nScan && !nPops)
		return;
	PresetLoadFile(&ptRegion->ptFile, ptAct->hArchive, ptRegion->pRecord->File[ptRegion->nIndex]);
	PresetRegionAttachUnits(pPool, ptRegion, &ptLevel->tSeed);
#line 2233
	D2WARN(ptRegion->ptFile->nSizeX == ptRegion->tCoords.nSizeTileX);
#line 2234
	D2WARN(ptRegion->ptFile->nSizeY == ptRegion->tCoords.nSizeTileY);
	if (nPops)
	{
#line 2239
		ptRegion->pnPopIds = (int *)D2ALLOCPOOL(pPool, nPops * sizeof(int));
#line 2240
		ptRegion->pnPopSubs = (int *)D2ALLOCPOOL(pPool, nPops * sizeof(int));
#line 2241
		ptRegion->pnPopTicks = (int *)D2ALLOCPOOL(pPool, nPops * sizeof(int));
#line 2242
		ptRegion->ptPopCoords = (DRLGCOORDS *)D2ALLOCPOOL(pPool, nPops * sizeof(DRLGCOORDS));
	}
	memset(ptRegion->pnPopIds, 0, nPops * sizeof(int));
	memset(ptRegion->pnPopSubs, 0, nPops * sizeof(int));
	memset(ptRegion->pnPopTicks, 0, nPops * sizeof(int));
	memset(ptRegion->ptPopCoords, 0, nPops * sizeof(DRLGCOORDS));
	ptFile = ptRegion->ptFile;
	memset(&tCoords, 0, sizeof(tCoords));
	memset(&tOrient, 0, sizeof(tOrient));
	memset(&tWalls, 0, sizeof(tWalls));
	tCoords.nSizeTileX = ptFile->nSizeX + 1;
	tCoords.nSizeTileY = ptFile->nSizeY + 1;
	for (nLayer = 0; nLayer < ptFile->nWallLayers; nLayer++)
	{
		DrlgGridInitView(&tOrient, ptFile->apnOrientations[nLayer], &tCoords, ptFile->nSizeX + 1,
			anOrientRows);
		DrlgGridInitView(&tWalls, ptFile->apnWalls[nLayer], &tCoords, ptFile->nSizeX + 1, anWallRows);
		for (nY = 0; nY < ptFile->nSizeY; nY++)
		{
			for (nX = 0; nX < ptFile->nSizeX; nX++)
			{
				nOrient = DrlgGridGet(&tOrient, nX, nY);
				nWall = DrlgGridGet(&tWalls, nX, nY);
				nSub = ((DWORD)nWall >> 8) & 255;
				nCode = ((DWORD)nWall >> 20) & 63;
				dwTop = (DWORD)nWall & 0x80000000;
				if (nOrient != 11 && nOrient != 10)
					continue;
				if (nScan && nCode >= 0 && nCode <= 7 && (nSub == 0 || nSub == 4 || dwTop))
				{
					nGridX = fWhole ? 0 : nX / 8;
					nGridY = fWhole ? 0 : nY / 8;
					DrlgGridApply(ptGrid, nGridX, nGridY, 1 << (nCode + 4), 0);
				}
				if (nPops && nCode >= 8 && nCode <= 29)
				{
					for (k = 0; k < nLinks; k++)
					{
						if (ptRegion->pnPopIds[k] == nCode)
						{
							ptRegion->ptPopCoords[k].nSizeTileX = nX;
							ptRegion->ptPopCoords[k].nSizeTileY = nY;
							break;
						}
					}
					if (k == nLinks)
					{
						ptRegion->pnPopIds[nLinks] = nCode;
						ptRegion->pnPopSubs[nLinks] = nSub;
						ptRegion->ptPopCoords[nLinks].nTileX = nX;
						ptRegion->ptPopCoords[nLinks].nTileY = nY;
						nLinks++;
					}
				}
				if (nScan && nCode >= 30 && nCode <= 33)
				{
					ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nX = ptRegion->tCoords.nTileX
						+ nX;
					ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nY = ptRegion->tCoords.nTileY
						+ nY;
					switch (nCode)
					{
					case 30:
						ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nKind = nSub;
						break;
					case 31:
						ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nKind = nSub + 5;
						break;
					case 32:
						ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nKind = 10;
						break;
					case 33:
						ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nKind = 11;
						break;
					case 34:
						ptLevel->atPresetMarks[ptLevel->nNumPresetMarks].nKind = 12;
						break;
					}
					ptLevel->nNumPresetMarks++;
				}
			}
		}
		DrlgGridClear(&tWalls);
		DrlgGridClear(&tOrient);
	}
	if (nPops)
	{
		ptRegion->nNumPops = nLinks;
		for (k = 0; k < nLinks; k++)
		{
			ptLink = &ptRegion->ptPopCoords[k];
			nMinX = ptLink->nTileX < ptLink->nSizeTileX ? ptLink->nTileX : ptLink->nSizeTileX;
			nMinY = ptLink->nTileY < ptLink->nSizeTileY ? ptLink->nTileY : ptLink->nSizeTileY;
			nMaxX = ptLink->nTileX > ptLink->nSizeTileX ? ptLink->nTileX : ptLink->nSizeTileX;
			nMaxY = ptLink->nTileY > ptLink->nSizeTileY ? ptLink->nTileY : ptLink->nSizeTileY;
			ptLink->nTileX = ptRegion->tCoords.nTileX + nMinX;
			ptLink->nTileY = ptRegion->tCoords.nTileY + nMinY;
			ptLink->nSizeTileX = nMaxX - nMinX + 1;
			ptLink->nSizeTileY = nMaxY - nMinY + 1;
			ptRegion->pnPopIds[k] = ptRegion->pnPopIds[k] / 4 - 1;
		}
	}
	if (nScan)
	{
		for (ptUnit = ptFile->ptUnits; ptUnit; ptUnit = ptUnit->ptNext)
		{
			if (ptUnit->nUnitType == 2 && (DWORD)ptUnit->nClassId < 410 &&
				((PTOBJECTSTXT)D2CommonObjTblsGetObject(ptUnit->nClassId))->SubClass & 64)
			{
				nGridX = fWhole ? 0 : ptUnit->nX / 5 / 8;
				nGridY = fWhole ? 0 : ptUnit->nY / 5 / 8;
				DrlgGridApply(ptGrid, nGridX, nGridY, 196608, 0);
			}
		}
	}
}
DWORD __fastcall PresetRegionGetFile(PTPRESETSUBINFO ptRegion)
{
	return ptRegion->nIndex;
}

DWORD __fastcall PresetRegionSetFile(PTPRESETSUBINFO ptRegion, DWORD nIndex)
{
	return ptRegion->nIndex = nIndex;
}

PTPRESETSUBINFO __fastcall PresetRegionNew(DRLGLEVEL *ptLevel, int nDef, DRLGCOORDS *ptCoords, D2SEED *ptSeed)
{
	PTPRESETSUBINFO ptRegion;
	PTLVLPRESTTXT ptRecord;
	DWORD i;

#line 2476
	ptRegion = (PTPRESETSUBINFO)D2ALLOCPOOL(ptLevel->ptAct->pMemPool, sizeof(PRESETSUBINFO));
	for (i = 0; i < sizeof(PRESETSUBINFO) / sizeof(DWORD); i++)
		((DWORD *)ptRegion)[i] = 0;
	ptRecord = (PTLVLPRESTTXT)D2CommonLvlTblsGetPrest(nDef);
	ptRegion->pRecord = ptRecord;
	ptRegion->nDef = nDef;
	ptRegion->nIndex = ptRecord->Files ? SeedRand(ptSeed, ptRecord->Files) : 0;
	ptRegion->tCoords.nTileX = ptCoords->nTileX;
	ptRegion->tCoords.nTileY = ptCoords->nTileY;
	if (ptRegion->pRecord->SizeX && ptRegion->pRecord->SizeY)
	{
		ptRegion->tCoords.nSizeTileX = ptRegion->pRecord->SizeX;
		ptRegion->tCoords.nSizeTileY = ptRegion->pRecord->SizeY;
	}
	else
	{
		ptRegion->tCoords.nSizeTileX = ptCoords->nSizeTileX;
		ptRegion->tCoords.nSizeTileY = ptCoords->nSizeTileY;
	}
	ptRegion->dwDefUnitsPending = 1;
	ptRegion->ptNext = ptLevel->ptRegionFirst;
	ptLevel->ptRegionFirst = ptRegion;
	return ptRegion;
}

int __fastcall PresetGetSizeX(int nDef)
{
	return ((PTLVLPRESTTXT)D2CommonLvlTblsGetPrest(nDef))->SizeX;
}

int __fastcall PresetGetSizeY(int nDef)
{
	return ((PTLVLPRESTTXT)D2CommonLvlTblsGetPrest(nDef))->SizeY;
}

void __fastcall PresetFreeRegions(void *pPool, PTPRESETSUBINFO ptFirst)
{
	PTPRESETSUBINFO ptRegion;
	PTPRESETSUBINFO ptNext;

	for (ptRegion = ptFirst; ptRegion; ptRegion = ptNext)
	{
		ptNext = ptRegion->ptNext;
		DrlgGridFree(pPool, &ptRegion->tLinks);
		DrlgVerFreeRing(pPool, &ptRegion->field_20);
		PresetFreeFile(&ptRegion->ptFile);
		sFreeUnitList(pPool, ptRegion->ptUnits);
		if (ptRegion->pnPopIds)
		{
#line 2551
			D2FREEPOOL(pPool, ptRegion->pnPopIds);
		}
		if (ptRegion->pnPopSubs)
		{
#line 2552
			D2FREEPOOL(pPool, ptRegion->pnPopSubs);
		}
		if (ptRegion->pnPopTicks)
		{
#line 2553
			D2FREEPOOL(pPool, ptRegion->pnPopTicks);
		}
		if (ptRegion->ptPopCoords)
		{
#line 2554
			D2FREEPOOL(pPool, ptRegion->ptPopCoords);
		}
#line 2556
		D2FREEPOOL(pPool, ptRegion);
	}
}

PTPRESETLINK __fastcall PresetRoomGetLink(ROOM *ptRoom)
{
	return ptRoom->field_20.ptPreset->ptLink;
}

DWORD __fastcall PresetRoomGetInfoFirstDword(PTPRESETROOM pRoom)
{
	return pRoom->pInfo->dwDef;
}

BYTE *__fastcall PresetRoomGetFile(PTPRESETROOM pRoom)
{
	PTPRESETINFO pInfo;

	pInfo = pRoom->pInfo;
	return (BYTE *)pInfo->pSubInfo->pRecord->File[pInfo->pSubInfo->nIndex];
}

static void __fastcall PresetRoomFadeDoor(ROOM *ptRoom, int nSub, DRLGCOORDS *ptDoor, DWORD dwTick,
	BOOL fOpen);

void __fastcall PresetRoomUpdateDoors(ROOM *ptRoom, int nX, int nY, BOOL fReopen)
{
	PTPRESETSUBINFO ptRegion;
	PTPRESETSUBINFO ptNearRegion;
	ROOM *ptNear;
	DWORD dwTick;
	BOOL fChanged;
	int nDoor;
	int nState;
	int i;
	int j;

	ptRegion = NULL;
	fChanged = FALSE;
	dwTick = GetTickCount() + 500;
	nDoor = 0;
	if (ptRoom->dwKind == 2)
	{
		ptRegion = ptRoom->field_20.ptPreset->pSubInfo;
		for (i = 0; i < ptRegion->nNumPops; i++)
		{
			DRLGCOORDS tRect = { 0 };

			tRect.nTileX = ptRegion->ptPopCoords[i].nTileX * 5;
			tRect.nTileY = ptRegion->ptPopCoords[i].nTileY * 5;
			tRect.nSizeTileX = ptRegion->ptPopCoords[i].nSizeTileX * 5 + ptRegion->pRecord->PopPad;
			tRect.nSizeTileY = ptRegion->ptPopCoords[i].nSizeTileY * 5 + ptRegion->pRecord->PopPad;
			if (DrlgRectContains(&tRect, nX, nY))
			{
				nDoor = ptRegion->pnPopIds[i];
				break;
			}
		}
	}
	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear->dwKind != 2 || !ptNear->ptGameRoom)
			continue;
		ptNearRegion = ptNear->field_20.ptPreset->pSubInfo;
		for (j = 0; j < ptNearRegion->nNumPops; j++)
		{
			nState = ptNearRegion->pnPopTicks[j];
			if (!(ptNearRegion == ptRegion && ptNearRegion->pnPopIds[j] == nDoor) && nState)
			{
				PresetRoomFadeDoor(ptNear, ptNearRegion->pnPopSubs[j], &ptNearRegion->ptPopCoords[j], dwTick,
					TRUE);
				fChanged = TRUE;
			}
			if (ptNearRegion == ptRegion && ptNearRegion->pnPopIds[j] == nDoor)
			{
				if (!nState)
				{
					PresetRoomFadeDoor(ptNear, ptNearRegion->pnPopSubs[j], &ptNearRegion->ptPopCoords[j],
						dwTick,
						FALSE);
					fChanged = TRUE;
				}
				else if (fReopen)
				{
					PresetRoomFadeDoor(ptNear, ptNearRegion->pnPopSubs[j], &ptNearRegion->ptPopCoords[j],
						(DWORD)nState, FALSE);
					fChanged = TRUE;
				}
			}
		}
	}
	if (!fChanged)
		return;
	for (i = 0; i < ptRoom->nNumNear; i++)
	{
		ptNear = ptRoom->pptNear[i];
		if (ptNear->dwKind != 2)
			continue;
		ptNearRegion = ptNear->field_20.ptPreset->pSubInfo;
		for (j = 0; j < ptNearRegion->nNumPops; j++)
		{
			if (!(ptNearRegion == ptRegion && ptNearRegion->pnPopIds[j] == nDoor))
				ptNearRegion->pnPopTicks[j] = 0;
			if (ptNearRegion == ptRegion && ptNearRegion->pnPopIds[j] == nDoor)
				ptNearRegion->pnPopTicks[j] = (int)dwTick;
		}
	}
}
static void __fastcall PresetRoomFadeDoor(ROOM *ptRoom, int nSub, DRLGCOORDS *ptDoor, DWORD dwTick,
	BOOL fOpen)
{
	PTDRLGTILE ptTile;
	int nCount;
	int nX;
	int nY;

#line 2601
	D2ASSERT(ptRoom->ptDrlgTiles);
	DRLGCOORDS tRect = { 0 };
	tRect.nTileX = ptDoor->nTileX - 1;
	tRect.nTileY = ptDoor->nTileY - 1;
	tRect.nSizeTileX = ptDoor->nSizeTileX + 2;
	tRect.nSizeTileY = ptDoor->nSizeTileY + 2;
	nCount = ptRoom->ptDrlgTiles->tTileArrays.nNumWalls;
	ptTile = ptRoom->ptDrlgTiles->tTileArrays.ptWalls;
	for (; nCount > 0; nCount--, ptTile++)
	{
		if (ptTile->dwFlags & 512)
		{
			nX = ptTile->nX + ptRoom->tCoords.nTileX;
			nY = ptTile->nY + ptRoom->tCoords.nTileY;
			if (DrlgRectContains(&tRect, nX, nY))
			{
				if (fOpen)
				{
					ptTile->dwFadeEndTick = dwTick;
					ptTile->dwFadeFlags |= 4;
				}
				else
				{
					ptTile->dwFadeFlags &= ~4;
					ptTile->dwFlags &= ~8;
				}
				continue;
			}
		}
		if (ptTile->dwFlags & 256)
			continue;
		if ((int)D2CMPTileGetStyle(ptTile->pTile) != nSub)
			continue;
		nX = ptTile->nX + ptRoom->tCoords.nTileX;
		nY = ptTile->nY + ptRoom->tCoords.nTileY;
		if (!DrlgRectContains(&tRect, nX, nY))
			continue;
		ptTile->dwFadeFlags |= 2;
		if (fOpen)
		{
			ptTile->bFadeFromAlpha = 0;
			ptTile->bFadeToAlpha = 255;
			if (!ptTile->bAlpha)
				ptTile->dwFadeEndTick = dwTick;
			else
				ptTile->dwFadeEndTick = (ptTile->bFadeFromAlpha - ptTile->bAlpha) * 500 /
					(ptTile->bFadeToAlpha - ptTile->bFadeFromAlpha) + dwTick;
		}
		else
		{
			ptTile->bFadeFromAlpha = 255;
			ptTile->bFadeToAlpha = 0;
			if (ptTile->bAlpha == 255)
				ptTile->dwFadeEndTick = dwTick;
			else
				ptTile->dwFadeEndTick = (ptTile->bAlpha - ptTile->bFadeFromAlpha) * 500 /
					(ptTile->bFadeFromAlpha - ptTile->bFadeToAlpha) + dwTick;
		}
	}
}

DRLGVERTEX *__fastcall PresetLevelGetRegionRing(DRLGLEVEL *ptLevel)
{
	return ptLevel->field_38.ptPreset->ptRegion->field_20;
}

void __fastcall PresetLevelInit(DRLGLEVEL *ptLevel)
{
	PTPRESETLEVEL ptPreset;
	PTLVLPRESTTXT ptRecord;

#line 2791
	ptPreset = (PTPRESETLEVEL)D2ALLOCPOOL(ptLevel->ptAct->pMemPool, sizeof(PRESETLEVEL));
	ptRecord = LvlTblsGetPrestByLevel((int)ptLevel->nLevelId);
	ptLevel->field_38.ptPreset = ptPreset;
	memset(ptPreset, 0, sizeof(PRESETLEVEL));
	if (ptRecord->Files)
		ptPreset->nFile = (int)SeedRand(&ptLevel->tSeed, ptRecord->Files);
	else
		ptPreset->nFile = -1;
	DrlgLevelSetRect(ptLevel->ptAct, ptLevel);
}

void __fastcall PresetLevelGenerate(DRLGLEVEL *ptLevel)
{
	PTPRESETLEVEL ptPreset;
	PTLVLPRESTTXT ptRecord;
	PTPRESETSUBINFO ptRegion;
	int *pnVis;
	int i;
	DRLGLEVEL *ptVisLevel;
	ROOM *ptRoom;

	ptPreset = ptLevel->field_38.ptPreset;
	ptRecord = LvlTblsGetPrestByLevel((int)ptLevel->nLevelId);
	ptRegion = PresetRegionNew(ptLevel, ptRecord->Def, &ptLevel->tCoords, &ptLevel->tSeed);
	ptPreset->ptRegion = ptRegion;
	if (ptPreset->nFile == -1)
		ptPreset->nFile = (int)ptRegion->nIndex;
	else
		ptRegion->nIndex = (DWORD)ptPreset->nFile;
	PresetRegionToRooms(ptLevel, ptPreset->ptRegion, 0, FALSE);
	if (ptLevel->nLevelId == 40 || ptLevel->nLevelId == 103)
	{
		if (ptLevel->ptAct->pfnTownPlaced && ptRegion->pRecord->AutoMap)
			ptLevel->ptAct->pfnTownPlaced((int)ptLevel->nLevelId, (int)ptRegion->nIndex,
				ptLevel->tCoords.nTileX + ptLevel->tCoords.nSizeTileX / 2,
				ptLevel->tCoords.nTileY + ptLevel->tCoords.nSizeTileY / 2);
		return;
	}
	if (!ptLevel->ptAct->pfnRoomBuilt || !ptRegion->pRecord->AutoMap)
		return;
	pnVis = DrlgRoomGetVis(ptLevel->ptAct, (int)ptLevel->nLevelId);
	for (i = LEVEL_VIS_MAX; i; i--)
	{
		if (*pnVis)
		{
			ptVisLevel = DrlgGetLevel(ptLevel->ptAct, (DWORD)*pnVis);
			if (!ptVisLevel->ptRoomFirst)
				DrlgGenerateLevel(ptVisLevel);
		}
		pnVis++;
	}
	for (ptRoom = ptLevel->ptRoomFirst; ptRoom; ptRoom = ptRoom->ptLevelNext)
	{
		DrlgRoomBuild(ptRoom);
		ptLevel->ptAct->pfnRoomBuilt(ptRoom->ptGameRoom);
	}
}

void __fastcall PresetLevelFree(DRLGLEVEL *ptLevel, BOOL fKeep)
{
	ptLevel->field_38.ptPreset->ptRegion = NULL;
	if (!fKeep)
	{
#line 2884
		D2FREEPOOL(ptLevel->ptAct->pMemPool, ptLevel->field_38.ptPreset);
		ptLevel->field_38.ptPreset = NULL;
	}
}
