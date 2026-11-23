#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Logging\\Logging.cpp"
#include <stdio.h>
#include <stdarg.h>
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"

void __stdcall LoggingUnpackDword(int nPacked, DWORD *pdwLow, int *pnHigh)
{
	*pdwLow = (DWORD)nPacked & 65535;
	*pnHigh = nPacked >> 16;
}

static CRITICAL_SECTION sgtLogLock;
static FILE *sghLogFile = NULL;
static DWORD sgfLogEnabled = 0;
static DWORD sgdwLogMode = 0;

void __fastcall LoggingToggle(DWORD fEnable)
{
	FILE *hOld = sghLogFile;

	sgfLogEnabled = fEnable;
	if (fEnable)
	{
		if (hOld)
			return;
		InitializeCriticalSection(&sgtLogLock);
		EnterCriticalSection(&sgtLogLock);
		sghLogFile = fopen("d2log.txt", "wt");
		LeaveCriticalSection(&sgtLogLock);
		if (sghLogFile)
		{
			LoggingWrite(1, 8, 2, 524288, "C:\\D2\\Source\\D2Common\\Logging\\Logging.cpp", 245,
				"Log Started");
			return;
		}
		DeleteCriticalSection(&sgtLogLock);
		return;
	}
	if (!hOld)
		return;
	EnterCriticalSection(&sgtLogLock);
	fclose(sghLogFile);
	LeaveCriticalSection(&sgtLogLock);
	DeleteCriticalSection(&sgtLogLock);
	sghLogFile = NULL;
}

void __fastcall LoggingSetMode(DWORD dwMode)
{
	sgdwLogMode = dwMode;
}

FILE * __fastcall LoggingGetFile(void)
{
	return sghLogFile;
}

int __fastcall LoggingFormatTime(char *pszBuf)
{
	SYSTEMTIME tTime;

	GetLocalTime(&tTime);
	return sprintf(pszBuf, "%02d:%02d:%02d.%03d\t", tTime.wHour, tTime.wMinute, tTime.wSecond,
		tTime.wMilliseconds);
}

void __cdecl LoggingWriteTo(FILE *fp, int nArg1, int nArg2, int nArg3, DWORD dwFlags, const char *pszFile,
	int nLine, const char *pszFormat, ...)
{
	SYSTEMTIME tTime;
	char szBuf[500];
	char *pszCursor;
	va_list tArgs;
	const char *pszFmt = pszFormat;

	if (!sgfLogEnabled)
		return;
	GetLocalTime(&tTime);
	sprintf(szBuf, "%02d:%02d:%02d.%03d\t", tTime.wHour, tTime.wMinute, tTime.wSecond, tTime.wMilliseconds);
	pszCursor = szBuf + strlen(szBuf);
	sprintf(pszCursor, "%01d\t%01d\t%01d\t%02d\t%.50s\t%.5d\t", nArg1, nArg2, nArg3, dwFlags, pszFile, nLine);
	pszCursor += strlen(pszCursor);
	va_start(tArgs, pszFormat);
	vsprintf(pszCursor, pszFmt, tArgs);
	va_end(tArgs);
	strcat(pszCursor, "\n");
	fwrite(szBuf, strlen(szBuf), 1, fp);
	fflush(fp);
}

void __cdecl LoggingWrite(int nArg1, int nArg2, int nArg3, DWORD dwFlags, const char *pszFile, int nLine,
	const char *pszFormat, ...)
{
	SYSTEMTIME tTime;
	char szBuf[500];
	char *pszCursor;
	va_list tArgs;
	FILE *pFile;
	const char *pszFmt;

	if (!sghLogFile)
		return;
	EnterCriticalSection(&sgtLogLock);
	pszFmt = pszFormat;
	pFile = sghLogFile;
	if (sgfLogEnabled)
	{
	GetLocalTime(&tTime);
	sprintf(szBuf, "%02d:%02d:%02d.%03d\t", tTime.wHour, tTime.wMinute, tTime.wSecond, tTime.wMilliseconds);
	pszCursor = szBuf + strlen(szBuf);
	sprintf(pszCursor, "%01d\t%01d\t%01d\t%02d\t%.50s\t%.5d\t", nArg1, nArg2, nArg3, dwFlags, pszFile, nLine);
	pszCursor += strlen(pszCursor);
	va_start(tArgs, pszFormat);
	vsprintf(pszCursor, pszFmt, tArgs);
	va_end(tArgs);
	strcat(pszCursor, "\n");
	fwrite(szBuf, strlen(szBuf), 1, pFile);
	fflush(pFile);
	}
	LeaveCriticalSection(&sgtLogLock);
}

void __cdecl LoggingWriteGameRecord(int nGame, int nFrame, int nClient, int nType, int nSize,
	const char *pszFormat, ...)
{
	char szBuf[500];
	char *pszCursor;
	va_list tArgs;

	if (!sghLogFile || !sgfLogEnabled)
		return;
	EnterCriticalSection(&sgtLogLock);
	sprintf(szBuf, "Game:%02d\tFrame:%d\tClient:%02d\t", nGame, nFrame, nClient);
	if (nType == 1)
		strcat(szBuf, "Type:System\t");
	else
		strcat(szBuf, "Type:Game\t");
	pszCursor = szBuf + strlen(szBuf);
	wsprintfA(pszCursor, "Size:%d\n", nSize);
	pszCursor += strlen(pszCursor);
	va_start(tArgs, pszFormat);
	vsprintf(pszCursor, pszFormat, tArgs);
	va_end(tArgs);
	strcat(pszCursor, "\n\n");
	fwrite(szBuf, strlen(szBuf), 1, sghLogFile);
	fflush(sghLogFile);
	LeaveCriticalSection(&sgtLogLock);
}

DWORD __fastcall LoggingOpenNamed(const char *pszFileName)
{
	if (sghLogFile)
		return 1;
	InitializeCriticalSection(&sgtLogLock);
	EnterCriticalSection(&sgtLogLock);
	sghLogFile = fopen(pszFileName, "wt");
	LeaveCriticalSection(&sgtLogLock);
	if (sghLogFile)
	{
		LoggingWrite(1, 8, 2, 524288, "C:\\D2\\Source\\D2Common\\Logging\\Logging.cpp", 245,
			"Log Started");
		return 1;
	}
	DeleteCriticalSection(&sgtLogLock);
	return 0;
}

DWORD __fastcall LoggingCloseNamed(void)
{
	if (!sghLogFile)
		return 1;
	EnterCriticalSection(&sgtLogLock);
	fclose(sghLogFile);
	LeaveCriticalSection(&sgtLogLock);
	DeleteCriticalSection(&sgtLogLock);
	sghLogFile = NULL;
	return 1;
}
