#ifndef D2RE_ARCHIVE_H
#define D2RE_ARCHIVE_H

#include <windows.h>

typedef struct HD2ARCHIVE__ D2ARCHIVE;
typedef D2ARCHIVE *HD2ARCHIVE;

struct HD2ARCHIVE__
{
 	HANDLE		hStormArchive;
 	char		szPath[260];
};

BOOL __fastcall ArchiveOpenFile(HD2ARCHIVE hArchive, const char *pszFileName, HANDLE *phFile,
	BOOL fAllowMissing);
DWORD __fastcall ArchiveCloseFile(HD2ARCHIVE hArchive, HANDLE hFile);
DWORD __fastcall ArchiveGetFileSize(HD2ARCHIVE hArchive, HANDLE hFile, DWORD *pdwFileSizeHigh);
DWORD __fastcall ArchiveSeekFile(HD2ARCHIVE hArchive, HANDLE hFile, LONG lDistance, PLONG plDistanceHigh,
	DWORD dwMoveMethod);
DWORD __fastcall ArchiveReadFileInto(HD2ARCHIVE hArchive, const char *pszFileName, void *pBuffer,
	DWORD dwDestSize);
BOOL __fastcall ArchiveReadFile(HD2ARCHIVE hArchive, HANDLE hFile, void *pBuffer,
	DWORD dwBytesToRead);
void * __fastcall ArchiveAllocBufferAndRead(HD2ARCHIVE hArchive, const char *pszFileName, DWORD *pdwSize,
	const char *pszFile, int nLine);
HD2ARCHIVE __fastcall ArchiveOpen(const char *pszModule, const char *pszArchive, const char *pszKey,
	const char *pszProbeFile, BOOL (*pfnRetry)(void));
void __fastcall ArchiveClose(HD2ARCHIVE hArchive);

DWORD __fastcall StringLength(LPCTSTR pszString);

int __cdecl ArchiveBitFindFirstSet(DWORD dwValue);

#endif
