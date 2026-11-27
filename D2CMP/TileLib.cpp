#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\Src\\TileLib.cpp"
#include "D2Cmp.h"

#include "D2Macros.h"
#line 12

#define MAX_TILEPROJECT		32

void WINAPI TileLibrarySetHardwareFree(void (__fastcall *pfnFree)(D2TILE *ptTile))
{
	TileCacheSetHardwareFree(pfnFree);
}

void WINAPI TileLibraryAddProject(D2FINDTILEKEY ***ptTileLibrary, const char *szName)
{
	int ii;

#line 27
	D2ASSERT(ptTileLibrary);
#line 28
	D2ASSERT(szName);
	for (ii = 0; ii < MAX_TILEPROJECT; ii++)
	{
		if (!ptTileLibrary[ii])
		{
			TileProjectsGet(&ptTileLibrary[ii], szName);
			break;
		}
	}
#line 39
	D2ASSERT(ii < MAX_TILEPROJECT);
}

int WINAPI TileLibraryFindTiles(D2FINDTILEKEY ***ptTileLibrary, DWORD dwValue1, DWORD dwValue2,
	DWORD dwValue3, D2TILE **pptTiles, int nMax)
{
	int nLeft;
	int nFound;
	int nCount;
	int ii;

#line 58
	D2ASSERT(ptTileLibrary);
	nLeft = nMax;
	nFound = 0;
	for (ii = 0; ii < MAX_TILEPROJECT; ii++)
	{
		if (!ptTileLibrary[ii])
			break;
		nCount = FindTilesGet(ptTileLibrary[ii], &pptTiles[nFound], nLeft, dwValue1, dwValue2, dwValue3);
		nLeft -= nCount;
		nFound += nCount;
		if (!nLeft)
			return nMax;
	}
	return nMax - nLeft;
}

DWORD WINAPI TileGetBaseTile(D2TILE *hTile)
{
#line 86
	D2WARN(hTile);
	return hTile->dwOrientation;
}

DWORD WINAPI TileGetStyle(D2TILE *hTile)
{
#line 87
	D2WARN(hTile);
	return hTile->dwStyle;
}

DWORD WINAPI TileGetFlags(D2TILE *hTile)
{
#line 88
	D2WARN(hTile);
	return hTile->wFlags;
}

DWORD WINAPI TileGetWidth(D2TILE *hTile)
{
#line 89
	D2WARN(hTile);
	return hTile->dwWidth;
}

DWORD WINAPI TileGetRarity(D2TILE *hTile)
{
#line 90
	D2WARN(hTile);
	return hTile->dwRarity;
}

DWORD WINAPI TileGetSequence(D2TILE *hTile)
{
#line 91
	D2WARN(hTile);
	return hTile->dwSequence;
}

DWORD WINAPI TileGetFirstDword(D2TILE *hTile)
{
#line 92
	D2WARN(hTile);
	return hTile->field_00;
}

DWORD WINAPI TileGetHeight(D2TILE *hTile)
{
#line 93
	D2WARN(hTile);
	return hTile->dwHeight;
}

BYTE * WINAPI TileGetCollisionMask(D2TILE *hTile)
{
#line 94
	D2WARN(hTile);
	return hTile->abCollisionMask;
}

DWORD WINAPI TileGetFifthDword(D2TILE *hTile)
{
#line 95
	D2WARN(hTile);
	return hTile->field_10;
}
