#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\SRC\\TileProjects.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 9

typedef struct D2TILEPROJECTRECORD D2TILEPROJECTRECORD;

struct D2TILEPROJECTRECORD
{
 	char		szName[MAX_PATH];
 	D2TILEPROJECT	*ptProject;
 	D2FINDTILEKEY	**pptFindTiles;
 	D2TILEPROJECTRECORD	*ptNext;
};

static D2TILEPROJECTRECORD *sgptTileProjects;

void __cdecl TileProjectsFreeAll(void)
{
	D2TILEPROJECTRECORD *ptRecord;
	D2TILEPROJECTRECORD *ptNext;

	for (ptRecord = sgptTileProjects; ptRecord; ptRecord = ptNext)
	{
		ptNext = ptRecord->ptNext;
		TileProjectFree(ptRecord->ptProject, FALSE);
		ptRecord->ptProject = NULL;
		FindTilesFree(ptRecord->pptFindTiles);
		ptRecord->pptFindTiles = NULL;
#line 41
		D2FREE2(ptRecord);
	}
	sgptTileProjects = NULL;
}

void WINAPI TileProjectsGet(D2FINDTILEKEY ***pptHash, const char *szName)
{
	D2TILEPROJECTRECORD *ptRecord;
	D2TILEPROJECT *ptProject = NULL;
	D2FINDTILEKEY **hFindTile;
	HANDLE hFile = NULL;
	BYTE *ptData;
	DWORD dwSize;

	ptRecord = sgptTileProjects;
	while (ptRecord && strcmp(ptRecord->szName, szName))
		ptRecord = ptRecord->ptNext;
	if (ptRecord)
	{
		*pptHash = ptRecord->pptFindTiles;
		return;
	}
	dwSize = TileCacheProjectSize(szName);
#line 94
	ptData = (BYTE *)D2ALLOC2(dwSize);
#line 95
	D2ASSERT(ptData);
	memset(ptData, 0, dwSize);
	ArchiveOpenFile(NULL, szName, &hFile, FALSE);
	ArchiveReadFile(NULL, hFile, ptData, dwSize);
	ArchiveCloseFile(NULL, hFile);
	TileProjectLoadTiles((D2TILEPROJECT *)ptData, &ptProject, szName);
	hFindTile = TileProjectFindTiles(ptProject);
#line 110
	ptRecord = (D2TILEPROJECTRECORD *)D2ALLOC2(sizeof(D2TILEPROJECTRECORD));
#line 111
	D2ASSERT(ptRecord);
	memset(ptRecord, 0, sizeof(D2TILEPROJECTRECORD));
	ptRecord->ptNext = sgptTileProjects;
	sgptTileProjects = ptRecord;
	ptRecord->ptProject = ptProject;
	ptRecord->pptFindTiles = hFindTile;
	strcpy(ptRecord->szName, szName);
	*pptHash = ptRecord->pptFindTiles;
}
