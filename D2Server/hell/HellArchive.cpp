#define D2_THIS_FILE "C:\\D2\\Source\\D2Hell\\SRC\\Archive.cpp"
#include <windows.h>
#include "Archive.h"
#include "D2Macros.h"
#include "StormImports.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FogImports.h"

static HANDLE __fastcall ArchiveOpenStorm(char *pszPath, const char *pszModule, const char *pszArchive,
	const char *pszKey, DWORD dwPriority);
static BOOL __fastcall ArchiveSearchCDs(char *pszPath, const char *pszArchive, DWORD dwPriority,
	HANDLE *phArchive);
BOOL __fastcall ArchiveOpenFile(HD2ARCHIVE hArchive, const char *pszFileName, HANDLE *phFile,
	BOOL fAllowMissing)
{
	if (FogSafesockOpenStormFileRaw((DWORD)pszFileName, (DWORD)phFile))
		return TRUE;
	if (fAllowMissing && GetLastError() == ERROR_FILE_NOT_FOUND)
		return FALSE;
#line 56
	FogErrorManagerError(3, pszFileName, D2_THIS_FILE, __LINE__);
	exit(-1);
	return FALSE;
}
DWORD __fastcall ArchiveCloseFile(HD2ARCHIVE hArchive, HANDLE hFile)
{
#line 70
	D2ASSERT(hFile);
	return FogSafesockCloseStormFileRaw((DWORD)hFile);
}
DWORD __fastcall ArchiveGetFileSize(HD2ARCHIVE hArchive, HANDLE hFile, DWORD *pdwFileSizeHigh)
{
	DWORD dwSize;
	char szFileName[MAX_PATH];

#line 86
	D2ASSERT(hFile);
	dwSize = FogSafesockGetStormFileSizeRaw((DWORD)hFile, (DWORD)pdwFileSizeHigh);
	if (!dwSize)
	{
		StormSFileGetFileName(hFile, szFileName, MAX_PATH);
#line 92
		FogErrorManagerError(3, szFileName, D2_THIS_FILE, __LINE__);
		exit(-1);
	}
	return dwSize;
}

DWORD __fastcall ArchiveSeekFile(HD2ARCHIVE hArchive, HANDLE hFile, LONG lDistance, PLONG plDistanceHigh,
	DWORD dwMoveMethod)
{
	DWORD dwPosition;
	char szFileName[MAX_PATH];

#line 109
	D2ASSERT(hFile);
	dwPosition = FogSafesockSetStormFilePointerRaw((DWORD)hFile, (DWORD)lDistance, (DWORD)plDistanceHigh, dwMoveMethod);
	if (dwPosition == (DWORD)-1)
	{
		StormSFileGetFileName(hFile, szFileName, MAX_PATH);
#line 118
		FogErrorManagerError(3, szFileName, D2_THIS_FILE, __LINE__);
		exit(-1);
	}
	return dwPosition;
}
BOOL __fastcall ArchiveReadFile(HD2ARCHIVE hArchive, HANDLE hFile, void *pBuffer, DWORD dwBytesToRead)
{
	DWORD dwBytes;
	BOOL fRead;
	char szFileName[MAX_PATH];

#line 132
	D2ASSERT(hFile);
	fRead = FogSafesockReadStormFileRaw((DWORD)hFile, (DWORD)pBuffer, dwBytesToRead, (DWORD)&dwBytes, 0, 0);
	if (!fRead)
	{
		StormSFileGetFileName(hFile, szFileName, MAX_PATH);
#line 140
		FogErrorManagerError(3, szFileName, D2_THIS_FILE, __LINE__);
		exit(-1);
	}
#line 143
	D2ASSERT(dwBytesToRead == dwBytes);
	return fRead;
}

DWORD __fastcall ArchiveReadFileInto(HD2ARCHIVE hArchive, const char *pszFileName, void *pBuffer,
	DWORD dwDestSize)
{
	HANDLE hFile;

	ArchiveOpenFile(hArchive, pszFileName, &hFile, FALSE);
#line 162
	D2ASSERT(dwDestSize >= ArchiveGetFileSize( hArchive, hFile, NULL ));
	ArchiveReadFile(hArchive, hFile, pBuffer, dwDestSize);
	ArchiveCloseFile(hArchive, hFile);
	return dwDestSize;
}
void * __fastcall ArchiveAllocBufferAndRead(HD2ARCHIVE hArchive, const char *pszFileName, DWORD *pdwSize,
	const char *pszFile, int nLine)
{
	HANDLE hFile;
	DWORD dwSizeHigh;
	DWORD dwSize;
	void *pBuffer;

	ArchiveOpenFile(hArchive, pszFileName, &hFile, FALSE);
	dwSize = ArchiveGetFileSize(hArchive, hFile, &dwSizeHigh);
	pBuffer = FogErrorManagerAlloc(dwSize + 800, pszFile, nLine, 0);
	if (!pBuffer)
		return NULL;
	ArchiveReadFile(hArchive, hFile, pBuffer, dwSize);
	ArchiveCloseFile(hArchive, hFile);
	if (pdwSize)
		*pdwSize = dwSize;
	return pBuffer;
}

HD2ARCHIVE __fastcall ArchiveOpen(const char *pszModule, const char *pszArchive, const char *pszKey,
	const char *pszProbeFile, BOOL (*pfnRetry)(void))
{
	HD2ARCHIVE hArchive;

#line 554
	hArchive = (HD2ARCHIVE)D2ALLOC2(sizeof(D2ARCHIVE));
	hArchive->hStormArchive = NULL;
	while (!hArchive->hStormArchive)
	{
		hArchive->hStormArchive = ArchiveOpenStorm(hArchive->szPath, pszModule, pszArchive, pszKey, 1000);
		if (hArchive->hStormArchive)
			break;
		if (!pfnRetry || !pfnRetry())
		{
#line 571
			D2FREE2(hArchive);
			return NULL;
		}
		Sleep(100);
	}
	return hArchive;
}
static HANDLE __fastcall ArchiveOpenStorm(char *pszPath, const char *pszModule, const char *pszArchive,
	const char *pszKey, DWORD dwPriority)
{
	HANDLE hArchive;
	char *pszSlash;

	lstrcpyA(pszPath, pszArchive);
	if (StormSFileOpenArchive(pszPath, dwPriority, 2, &hArchive))
		return hArchive;
	if (!GetModuleFileNameA(NULL, pszPath, MAX_PATH))
		*pszPath = '\0';
	pszSlash = strrchr(pszPath, '\\');
	if (pszSlash)
		pszSlash[1] = '\0';
	StormSStrAppend(pszPath, pszArchive, 2147483647);
	if (StormSFileOpenArchive(pszPath, dwPriority, 2, &hArchive))
		return hArchive;
	if (ArchiveSearchCDs(pszPath, pszArchive, dwPriority, &hArchive))
		return hArchive;
	return NULL;
}
static BOOL __fastcall ArchiveSearchCDs(char *pszPath, const char *pszArchive, DWORD dwPriority,
	HANDLE *phArchive)
{
	char szDrives[MAX_PATH];
	char *pszDrive;
	char *pszNext;
	DWORD dwLength;

	dwLength = GetLogicalDriveStringsA(MAX_PATH, szDrives);
	if (!dwLength || dwLength > MAX_PATH)
		return FALSE;
	while (*pszArchive == '\\')
		pszArchive++;
	pszNext = szDrives;
	while (*pszNext)
	{
		pszDrive = pszNext;
		while (*pszNext++)
			;
		if (GetDriveTypeA(pszDrive) == DRIVE_CDROM)
		{
			lstrcpyA(pszPath, pszDrive);
			lstrcatA(pszPath, pszArchive);
			if (StormSFileOpenArchive(pszPath, dwPriority, 2, phArchive))
				return TRUE;
		}
	}
	return FALSE;
}
void __fastcall ArchiveClose(HD2ARCHIVE hArchive)
{
	if (hArchive->hStormArchive)
		StormSFileCloseArchiveRaw((DWORD)hArchive->hStormArchive);
	hArchive->hStormArchive = NULL;
#line 630
	D2FREE2(hArchive);
}

#if 0

DWORD __fastcall StringLength(LPCTSTR pszString)
{
	DWORD dwLength;

	dwLength = 0;
	while (*pszString++)
		dwLength++;
	return dwLength;
}

BOOL __fastcall StringEqual(LPCTSTR pszString1, LPCTSTR pszString2)
{
	while (*pszString1 && *pszString2 && *pszString1 == *pszString2)
	{
		pszString1++;
		pszString2++;
	}
	return (BYTE)(*pszString1 - *pszString2) == 0;
}

int __cdecl ArchiveBitFindFirstSet(DWORD dwValue)
{
	int nBit;

	if (!dwValue)
		return 0;
	nBit = 0;
	while (!(dwValue & 1))
	{
		dwValue >>= 1;
		nBit++;
	}
	return nBit + 1;
}

#endif
