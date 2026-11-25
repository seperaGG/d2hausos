#include "FogLocal.h"
#include "FogLocalAsync.h"
#include <string.h>

unsigned __int64 __fastcall AsyncDataGetFreeDiskSpace(void)
{
	char szPath[1024];
	DWORD dwSectorsPerCluster;
	DWORD dwFreeClusters;
	DWORD dwBytesPerSector;
	DWORD dwTotalClusters;
	ULARGE_INTEGER tFree;
	DWORD i;

	SafesockGetSavePath(szPath, sizeof(szPath));
	for (i = 0; i < sizeof(szPath); i++)
	{
		if (szPath[i] == '\\')
		{
			szPath[i + 1] = 0;
			break;
		}
	}
	dwSectorsPerCluster = 0;
	dwBytesPerSector = 0;
	dwFreeClusters = 0;
	dwTotalClusters = 0;
	GetDiskFreeSpaceA(szPath, &dwSectorsPerCluster, &dwBytesPerSector, &dwFreeClusters, &dwTotalClusters);
	tFree.LowPart = dwFreeClusters * dwBytesPerSector * dwSectorsPerCluster;
	tFree.HighPart = 0;
	return tFree.QuadPart;
}

void __cdecl AsyncDataGetMemoryStatus(DWORD *pdwAvailPhys, DWORD *pdwTotalPhys, DWORD *pdwAvailPageFile,
	DWORD *pdwTotalPageFile)
{
	MEMORYSTATUS tStatus;

	tStatus.dwLength = sizeof(tStatus);
	GlobalMemoryStatus(&tStatus);
	*pdwAvailPhys = tStatus.dwAvailPhys;
	*pdwTotalPhys = tStatus.dwTotalPhys;
	*pdwAvailPageFile = tStatus.dwAvailPageFile;
	*pdwTotalPageFile = tStatus.dwTotalPageFile;
}
