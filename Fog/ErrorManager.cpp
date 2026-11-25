#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\ErrorManager.cpp"
#include "FogLocal.h"
#include <imagehlp.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <time.h>
#include <eh.h>
#include <stdlib.h>
#include <string.h>
#include "StormImports.h"
#include "FogLocalSafesock.h"
#include "Mem/mem.h"
#include "ErrorManager.h"
#include "D2Macros.h"

#define FogErrorManagerReport	ErrorManagerReport
#define FogErrorManagerLog	ErrorManagerLog

extern "C" const DWORD gdwBitMasks[32];
extern "C" const DWORD gdwInvBitMasks[32];

void __fastcall AsyncDataTimeInit(void);
void __fastcall AsyncDataTimeShutdown(void);
void __fastcall AsyncDataShutdown(void);
void __fastcall MemPerformanceInit(void);
void __fastcall CpuLogInfo(void);

typedef struct _MODULEINFO
{
	LPVOID lpBaseOfDll;
	DWORD SizeOfImage;
	LPVOID EntryPoint;
} MODULEINFO, *LPMODULEINFO;

void __cdecl ErrorManagerReport(int nLevel, const char *pszFile, int nLine, const char *pszFormat, ...);
void __cdecl ErrorManagerLog(const char *pszFormat, ...);

static const char *sgpszErrorLevels[10] =
{
	"No Error", "Unknown Error", "Out of Memory Error", "File Read Error", "File Write Error",
	"Assertion Failure", "Halt", "Warning", "Missing Dll", "Registry Error",
};
static char sgszAppName[4096] = "Diablo II";
static char sgszLogFile[4096] = "D2Debug.txt";
static char sgszLogFileRoot[4096] = "C:\\D2Debug.txt";
static char sgszLogFileNumbered[4096] = "D2Debug%d.txt";

ERRORMANAGERSYSINFO gtErrorManagerSysInfo = { 0 };
static void (__cdecl *sgpfnErrorCallback)(void) = 0;
static HMODULE sghImageHlp = 0;
static BOOL sgfInError = 0;
static BOOL sgfErrorManagerInit = 0;
static LONG sglExceptionDepth = -1;
static char sgszHexDumpDots[] = "\r\n\t\v\x1f";
static BOOL sgfPoolsEnabled = TRUE;

static BOOL (WINAPI *sgpfnSymCleanup)(HANDLE) = 0;
static PFUNCTION_TABLE_ACCESS_ROUTINE sgpfnSymFunctionTableAccess = 0;
static BOOL (WINAPI *sgpfnSymGetLineFromAddr)(HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE) = 0;
static PGET_MODULE_BASE_ROUTINE sgpfnSymGetModuleBase = 0;
static BOOL (WINAPI *sgpfnSymGetModuleInfo)(HANDLE, DWORD, PIMAGEHLP_MODULE) = 0;
static DWORD (WINAPI *sgpfnSymGetOptions)(void) = 0;
static BOOL (WINAPI *sgpfnSymGetSymFromAddr)(HANDLE, DWORD, PDWORD, PIMAGEHLP_SYMBOL) = 0;
static BOOL (WINAPI *sgpfnSymInitialize)(HANDLE, LPSTR, BOOL) = 0;
static DWORD (WINAPI *sgpfnSymLoadModule)(HANDLE, HANDLE, PSTR, PSTR, DWORD, DWORD) = 0;
static DWORD (WINAPI *sgpfnSymSetOptions)(DWORD) = 0;
static BOOL (WINAPI *sgpfnStackWalk)(DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE,
	PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE) = 0;
static FARPROC sgpfnUnDecorateSymbolName = 0;
static FARPROC sgpfnSplitSymbols = 0;
static BOOL sgfStackDumpReady = 0;
static HANDLE sghStackDumpEvent = 0;
static HANDLE sghStackDumpThread = 0;
static BOOL sgfStackDumpDone = 0;

struct ERRORMANAGERMODULE
{
 	char		szImagePath[260];
 	char		szModuleName[256];
 	DWORD		dwBaseAddress;
 	DWORD		dwImageSize;
 	ERRORMANAGERMODULE	*ptNext;
};

static BOOL __fastcall ErrorManagerCrashReport(HANDLE hThread, CONTEXT *ptContext);

static void __fastcall ErrorManagerLogWrite(const char *pszText, BOOL fTimestamp);
static LONG WINAPI ErrorManagerExceptionFilter(EXCEPTION_POINTERS *ptException);
static void __fastcall ErrorManagerHexDump(const BYTE *pbData, int nBytes);
static BOOL __fastcall ErrorManagerStartWatchdog(void);
static void __cdecl ErrorManagerShutdown(void);
static BOOL __fastcall ErrorManagerGetModuleDir(char *pszPath, DWORD dwSize);
char * __cdecl DataArraysSprintf(char *pszBuffer, const char *pszFormat, ...);

static __inline void sDumpStack(void)
{
	DWORD dwNoStackDump;

	dwNoStackDump = 0;
	StormSRegLoadValue("Diablo II", "NoStackDump", 0,
		&dwNoStackDump);
	if (dwNoStackDump)
	{
		FogErrorManagerLog("sDumpStack: Stack dump turned off in registry");
		return;
	}
	if (!sgfStackDumpReady)
	{
		FogErrorManagerLog("sDumpStack: imagehlp.dll not loaded");
		return;
	}
	FogErrorManagerLog("\nImageHelp Stack Crawl:");
	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &sghStackDumpThread, 0,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	SetEvent(sghStackDumpEvent);
	SetThreadPriority(sghStackDumpThread, THREAD_PRIORITY_IDLE);
	while (!sgfStackDumpDone)
		Sleep(10);
	SetThreadPriority(sghStackDumpThread, THREAD_PRIORITY_NORMAL);
	CloseHandle(sghStackDumpThread);
}

static __inline void ErrorManagerAssertInline(const char *pszExpression, const char *pszFile, int nLine)
{
	FogErrorManagerReport(5, pszFile, nLine, pszExpression ? pszExpression : "<no reason given>");
	sDumpStack();
}

#undef FogAssert
#define FogAssert(pszExpr, pszFile, nLine)	ErrorManagerAssertInline((pszExpr), (pszFile), (nLine))

char * __fastcall ErrorManagerEscapeString(char *destbuf, const char *src, int destsize, char bCQuote)
{
	char *dst;
	const char *srcp;
	int bytesleft;
	char c;

	srcp = src;
	dst = destbuf;
#line 143
	D2ASSERT(destbuf != NULL);
	if (!src)
	{
		StormSStrPrintf(destbuf, destsize, "<NULL>");
		return destbuf;
	}
	bytesleft = destsize - 1;
	if (!destsize)
		return destbuf;
	if (bCQuote && bytesleft > 0)
	{
		*dst++ = bCQuote;
		bytesleft--;
	}
	while (bytesleft > 0 && *srcp)
	{
		c = *srcp++;
		bytesleft--;
		if (c >= ' ')
		{
			*dst++ = c;
			continue;
		}
		*dst++ = '\\';
		if (!bytesleft)
			break;
		bytesleft--;
		if (c == '\r')
			*dst++ = 'r';
		else if (c == '\n')
			*dst++ = 'n';
		else if (c == '\t')
			*dst++ = 't';
		else
		{
			if (bytesleft < 2)
				break;
			*dst++ = 'x';
			*dst++ = (char)(((c >> 4) & 15) < 10 ? ((c >> 4) & 15) + '0' : ((c >> 4) & 15) + 'A' - 10);
			*dst++ = (char)((c & 15) < 10 ? (c & 15) + '0' : (c & 15) + 'A' - 10);
			bytesleft -= 2;
		}
	}
	if (bCQuote && bytesleft > 0)
	{
		*dst++ = bCQuote;
		bytesleft--;
	}
	*dst++ = 0;
#line 207
	D2ASSERT(bytesleft >= 0);
#line 208
	D2ASSERT(dst - destbuf <= (int) destsize);
	return destbuf;
}

static int __fastcall ErrorManagerOutput(DWORD dwFlags, const char *pszFormat, va_list pArgs)
{
	char szBuffer[4096];
	SYSTEMTIME tTime;
	int nLength;
	int nResult;

	szBuffer[0] = 0;
	if (dwFlags & 4)
	{
		GetLocalTime(&tTime);
		_snprintf(szBuffer, sizeof(szBuffer), "%02d:%02d:%02d.%03d    ", tTime.wHour, tTime.wMinute,
			tTime.wSecond,
			tTime.wMilliseconds);
	}
	nLength = strlen(szBuffer);
	nResult = _vsnprintf(szBuffer + nLength, sizeof(szBuffer) - nLength, pszFormat, pArgs);
	szBuffer[sizeof(szBuffer) - 1] = 0;
	if (!(dwFlags & 2))
		OutputDebugStringA(szBuffer);
	if (dwFlags & 1)
		ErrorManagerLogWrite(szBuffer, FALSE);
	if (dwFlags & 24)
		return GetKeyState(VK_CAPITAL) & 1;
}

int __cdecl ErrorManagerDebugPrint(const char *pszFormat, ...)
{
	va_list pArgs;

	va_start(pArgs, pszFormat);
	return ErrorManagerOutput(0, pszFormat, pArgs);
}

void __cdecl ErrorManagerDebugPrintCaps(const char *pszFormat, ...)
{
	va_list pArgs;

	if (GetKeyState(VK_CAPITAL) & 1)
	{
		va_start(pArgs, pszFormat);
		ErrorManagerOutput(0, pszFormat, pArgs);
	}
}

int __cdecl ErrorManagerDebugPrintFlags(DWORD dwFlags, const char *pszFormat, ...)
{
	va_list pArgs;

	va_start(pArgs, pszFormat);
	return ErrorManagerOutput(dwFlags, pszFormat, pArgs);
}

int __cdecl ErrorManagerDebugLog(const char *pszFormat, ...)
{
	va_list pArgs;

	va_start(pArgs, pszFormat);
	return ErrorManagerOutput(1, pszFormat, pArgs);
}

ERRORMANAGERSYSINFO * __fastcall ErrorManagerGetSysInfo(void)
{
	return gtErrorManagerSysInfo.fValid ? &gtErrorManagerSysInfo : NULL;
}

static __inline BOOL ErrorManagerModuleDirInline(char *pszPath, DWORD dwSize)
{
	int i;

	pszPath[0] = 0;
	if (!GetModuleFileNameA(NULL, pszPath, dwSize))
		return FALSE;
	for (i = (int)strlen(pszPath) - 1; i > 0; i--)
	{
		if (pszPath[i] == '\\' || pszPath[i] == '/' || pszPath[i] == ':')
			break;
	}
	if (i <= 0)
		return FALSE;
	if (pszPath[i] == ':')
		i++;
	pszPath[i] = 0;
	return TRUE;
}

void __fastcall ErrorManagerRotateLogs(void);

void __fastcall ErrorManagerSetLogName(const char *pszName)
{
	char szDir[MAX_PATH];
	FILE *pFile;

	if (ErrorManagerModuleDirInline(szDir, sizeof(szDir)))
		sprintf(sgszLogFile, "%s\\%s.txt", szDir, pszName);
	else
		strcpy(sgszLogFile, pszName);
	sprintf(sgszLogFileRoot, "C:\\%s", pszName);
	sprintf(sgszLogFileNumbered, "%s%%d.txt", pszName);
	ErrorManagerRotateLogs();
	pFile = fopen(sgszLogFile, "w");
	if (!pFile)
	{
		pFile = fopen(sgszLogFileRoot, "w");
		if (!pFile)
			exit(-1);
	}
	fclose(pFile);
}

void __fastcall ErrorManagerRotateLogs(void)
{
	SYSTEMTIME tTime;
	char szDir[MAX_PATH];
	char szNew[4096];
	char szMessage[1024];
	char szNewName[4096];
	char szOld[4096];
	char szOldName[4096];
	FILE *pFile;
	int i;

	if (ErrorManagerModuleDirInline(szDir, sizeof(szDir)))
		strcat(szDir, "\\");
	else
		szDir[0] = 0;
	GetLocalTime(&tTime);

	for (i = 4; i >= 1; i--)
	{
		sprintf(szOldName, sgszLogFileNumbered, i);
		sprintf(szOld, "%s%s", szDir, szOldName);
		pFile = fopen(szOld, "r");
		if (pFile)
		{
			fclose(pFile);
			sprintf(szNewName, sgszLogFileNumbered, i + 1);
			sprintf(szNew, "%s%s", szDir, szNewName);
			DeleteFileA(szNew);
			MoveFileA(szOld, szNew);
		}
	}
	sprintf(szNewName, sgszLogFileNumbered, 1);
	sprintf(szNew, "%s%s", szDir, szNewName);
	DeleteFileA(szNew);
	MoveFileA(sgszLogFile, szNew);
	sprintf(szMessage, "MoveFile: %s to %s\n", sgszLogFile, szNew);
	OutputDebugStringA(szMessage);
}

void __fastcall ErrorManagerInit(const char *pszAppName, void (__cdecl *pfnCallback)(void))
{
	MEMORYSTATUS tMemory;
	char szSystem[256];
	char szLine[4096];
	DWORD dwTrace;
	float flVersion;
	FILE *pFile;
	int i;

	dwTrace = 0;
	if (sgfErrorManagerInit)
		return;
	sgfErrorManagerInit = TRUE;
	AsyncDataTimeInit();
	StormSRegLoadValue("Diablo II", "TraceUnfreedMemory", 0,
		&dwTrace);
	gdwTraceUnfreedMemory = dwTrace;
	MemPoolSystemInit(NULL, 262144, 32768);
	SetUnhandledExceptionFilter(ErrorManagerExceptionFilter);
	MemPerformanceInit();
	for (i = 0; i < 32; i++)
	{
#line 543
		D2ASSERT(gdwBitMasks[i] == (DWORD)(1 << i));
#line 544
		D2ASSERT(gdwInvBitMasks[i] == ~(DWORD)(1 << i));
	}
	{
		OSVERSIONINFOA tVersion;

		tVersion.dwOSVersionInfoSize = sizeof(tVersion);
		if (GetVersionExA(&tVersion))
		{
			flVersion = (float)tVersion.dwMinorVersion;
			while (flVersion > 1.0f)
				flVersion *= 0.1f;
			flVersion *= 0.01f;
			gtErrorManagerSysInfo.flVersion = flVersion + (float)tVersion.dwMajorVersion;
			switch (tVersion.dwPlatformId)
			{
			case VER_PLATFORM_WIN32_NT:
				gtErrorManagerSysInfo.dwOsType = 3;
				if (tVersion.dwMajorVersion == 5)
					gtErrorManagerSysInfo.dwOsType = 4;
				break;
			case VER_PLATFORM_WIN32_WINDOWS:
				gtErrorManagerSysInfo.dwOsType = 1;
				if (tVersion.dwMinorVersion > 0)
					gtErrorManagerSysInfo.dwOsType = 2;
				break;
			default:
				gtErrorManagerSysInfo.dwOsType = 0;
			}
		}
	}
	GlobalMemoryStatus(&tMemory);
	gtErrorManagerSysInfo.dwTotalPhysMemory = tMemory.dwTotalPhys;
	GetSystemInfo(&gtErrorManagerSysInfo.tSystemInfo);
	gtErrorManagerSysInfo.fValid = TRUE;
	strcpy(sgszAppName, pszAppName);
	{
		char szDir[MAX_PATH];

		if (ErrorManagerGetModuleDir(szDir, sizeof(szDir)))
			sprintf(sgszLogFile, "%s\\%s.txt", szDir, "D2Debug");
		else
			strcpy(sgszLogFile, "D2Debug");
	}
	sprintf(sgszLogFileRoot, "C:\\%s", "D2Debug");
	sprintf(sgszLogFileNumbered, "%s%%d.txt", "D2Debug");
	ErrorManagerRotateLogs();
	pFile = fopen(sgszLogFile, "w");
	if (!pFile)
	{
		pFile = fopen(sgszLogFileRoot, "w");
		if (!pFile)
			exit(-1);
	}
	fclose(pFile);
	atexit(ErrorManagerShutdown);
	sgpfnErrorCallback = pfnCallback;
	switch (gtErrorManagerSysInfo.dwOsType)
	{
	case 1:
		strcpy(szSystem, "Windows 95");
		break;
	case 2:
		strcpy(szSystem, "Windows 98");
		break;
	case 3:
		strcpy(szSystem, "Windows NT");
		break;
	case 4:
		strcpy(szSystem, "Windows 2000");
		break;
	case 5:
		strcpy(szSystem, "MacOS");
		break;
	}
	sprintf(szLine, "%s running under %s (Version %.3f)\n", sgszAppName, szSystem,
		(double)gtErrorManagerSysInfo.flVersion);
	ErrorManagerLogWrite(szLine, TRUE);
	CpuLogInfo();
	ErrorManagerLogWrite(DataArraysSprintf(szLine, "Total Physical Memory: %.2f MB\n",
		gtErrorManagerSysInfo.dwTotalPhysMemory / 1048576.0f), FALSE);
	ErrorManagerLogWrite(DataArraysSprintf(szLine, "Application Path: %s\n", __argv[0]), FALSE);
	ErrorManagerStartWatchdog();
	ErrorManagerLogWrite("----------------------------------------------\n\n", FALSE);
}

void __fastcall ErrorManagerSetExceptionFilter(void)
{
	SetUnhandledExceptionFilter(ErrorManagerExceptionFilter);
}

static void __cdecl ErrorManagerShutdown(void)
{
	AsyncDataShutdown();
	MemPoolSystemDestroy(NULL);
	sgpfnErrorCallback = NULL;
	if (sghImageHlp)
	{
		FreeLibrary(sghImageHlp);
		sghImageHlp = NULL;
	}
	AsyncDataTimeShutdown();
}

static LONG WINAPI ErrorManagerExceptionFilter(EXCEPTION_POINTERS *ptException)
{
	char szName[256];
	char szTitle[256];
	char szMessage[4096];
	EXCEPTION_RECORD *ptRecord;
	CONTEXT *ptContext;
	const char *pszName;
	HANDLE hThread;
	DWORD dwCode;
	BOOL fCopy;

	if (!InterlockedIncrement(&sglExceptionDepth))
	{

		sgfInError = TRUE;
		ptRecord = ptException->ExceptionRecord;
		dwCode = ptRecord->ExceptionCode;
		{
			char szUnknown[18] = "Unknown Exception";
			pszName = szUnknown;
			fCopy = TRUE;
			switch (dwCode)
			{
			case EXCEPTION_DATATYPE_MISALIGNMENT:	pszName = "DATATYPE_MISALIGNMENT";	break;
			case EXCEPTION_GUARD_PAGE:				pszName = "GUARD_PAGE";				break;
			case EXCEPTION_BREAKPOINT:				pszName = "BREAKPOINT";				break;
			case EXCEPTION_SINGLE_STEP:				pszName = "SINGLE_STEP";			break;
			case EXCEPTION_ACCESS_VIOLATION:		pszName = "ACCESS_VIOLATION";		break;
			case EXCEPTION_IN_PAGE_ERROR:			pszName = "IN_PAGE_ERROR";			break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:	pszName = "ARRAY_BOUNDS_EXCEEDED";	break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:		pszName = "ILLEGAL_INSTRUCTION";	break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:	pszName = "NONCONTINUABLE_EXCEPTION";	break;
			case EXCEPTION_INVALID_DISPOSITION:		pszName = "INVALID_DISPOSITION";	break;
			case EXCEPTION_INVALID_HANDLE:			pszName = "INVALID_HANDLE";			break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:	pszName = "FLT_DENORMAL_OPERAND";	break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:		pszName = "FLT_DIVIDE_BY_ZERO";		break;
			case EXCEPTION_FLT_INEXACT_RESULT:		pszName = "FLT_INEXACT_RESULT";		break;
			case EXCEPTION_FLT_INVALID_OPERATION:	pszName = "FLT_INVALID_OPERATION";	break;
			case EXCEPTION_FLT_OVERFLOW:			pszName = "FLT_OVERFLOW";			break;
			case EXCEPTION_FLT_STACK_CHECK:			pszName = "FLT_STACK_CHECK";		break;
			case EXCEPTION_FLT_UNDERFLOW:			pszName = "FLT_UNDERFLOW";			break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:		pszName = "INT_DIVIDE_BY_ZERO";		break;
			case EXCEPTION_INT_OVERFLOW:			pszName = "INT_OVERFLOW";			break;
			case EXCEPTION_PRIV_INSTRUCTION:		pszName = "PRIV_INSTRUCTION";		break;
			case EXCEPTION_STACK_OVERFLOW:			pszName = "STACK_OVERFLOW";			break;
			default:
				if (FormatMessageA(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
					GetModuleHandleA("NTDLL.DLL"),
					dwCode, 0, szName, sizeof(szName), NULL))
					fCopy = FALSE;
				break;
			}
			if (fCopy)
				strncpy(szName, pszName, sizeof(szName));
		}
		FogErrorManagerLog("----------------------------------------------\n", 0);
		FogErrorManagerLog("***** UNHANDLED EXCEPTION: %s (%08x)", szName, ptRecord->ExceptionCode);
		ptContext = ptException->ContextRecord;
		FogErrorManagerLog("eax:%08x ebx:%08x ecx:%08x edx:%08x esi:%08x", ptContext->Eax, ptContext->Ebx,
			ptContext->Ecx,
			ptContext->Edx, ptContext->Esi);
		FogErrorManagerLog("edi:%08x ebp:%08x esp:%08x eip:%08x flg:%08x", ptContext->Edi, ptContext->Ebp,
			ptContext->Esp,
			ptContext->Eip, ptContext->EFlags);
		FogErrorManagerLog("cs:%04x ds:%04x es:%04x ss:%04x fs:%04x gs:%04x\n", ptContext->SegCs,
			ptContext->SegDs,
			ptContext->SegEs, ptContext->SegSs, ptContext->SegFs, ptContext->SegGs);
		FogErrorManagerLog("Stack bytes:");
		ErrorManagerHexDump((const BYTE *)(INT_PTR)ptContext->Esp, 768);
		FogErrorManagerLog("Code bytes:");
		ErrorManagerHexDump((const BYTE *)(INT_PTR)ptContext->Eip, 16);
		DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &hThread, 0, FALSE,
			DUPLICATE_SAME_ACCESS);
		ErrorManagerCrashReport(hThread, ptException->ContextRecord);
		CloseHandle(hThread);
		if (sgpfnErrorCallback)
			sgpfnErrorCallback();
		sprintf(szTitle, "%s Exception", sgszAppName);
		sprintf(szMessage, "UNHANDLED EXCEPTION:\n%s (%08x)", szName, ptRecord->ExceptionCode);
		MessageBoxA(NULL, szMessage, szTitle, MB_ICONEXCLAMATION);
	}
	InterlockedDecrement(&sglExceptionDepth);
	return EXCEPTION_EXECUTE_HANDLER;
}

void __cdecl ErrorManagerAssert(const char *pszExpression, const char *pszFile, int nLine)
{
	ErrorManagerAssertInline(pszExpression, pszFile, nLine);
}

void __cdecl ErrorManagerHalt(const char *pszMessage, const char *pszFile, int nLine)
{
	FogErrorManagerReport(6, pszFile, nLine, pszMessage);
	sDumpStack();
}

void __cdecl ErrorManagerWarn(const char *pszMessage, const char *pszFile, int nLine)
{
	if (pszMessage)
		FogErrorManagerReport(7, pszFile, nLine, pszMessage);
}

void __cdecl ErrorManagerError(int nLevel, const char *pszMessage, const char *pszFile, int nLine)
{
	FogErrorManagerReport(nLevel, pszFile, nLine, pszMessage);
	sDumpStack();
}

void __cdecl ErrorManagerReport(int nLevel, const char *pszFile, int nLine, const char *pszFormat, ...)
{
	char szBox[4096];
	char szText[4096];
	char szFormatted[4096];
	char szTitle[256];
	const BYTE *pbCode;
	const BYTE *pbStack;
	va_list pArgs;
	BOOL fFatal;

	if (sgfInError)
	{
		ErrorManagerLogWrite("***** ENCOUNTERED ERROR DURING ERROR HANDLING!!!\n", FALSE);
		return;
	}
	fFatal = nLevel == 5 || nLevel == 6 || nLevel == 2;
	sgfInError = TRUE;
	if (pszFile)
	{
		sprintf(szFormatted, "--------  FILE: %s    LINE: %d  --------\n\n", pszFile, nLine);
		ErrorManagerLogWrite(szFormatted, TRUE);
	}
	if (pszFormat)
	{
		va_start(pArgs, pszFormat);
		vsprintf(szFormatted, pszFormat, pArgs);
		va_end(pArgs);
		if (pszFile)
			sprintf(szText, "%s\nLocation : %s, line #%d\nExpression : %s", sgpszErrorLevels[nLevel],
				pszFile, nLine,
				szFormatted);
		else
			strcpy(szText, szFormatted);
	}
	else
		sprintf(szText, "%s\nLocation : %s, line #%d", sgpszErrorLevels[nLevel], pszFile, nLine);
	sprintf(szTitle, "%s Error", sgszAppName);
	strcpy(szBox, szText);
	strcat(szText, "\n");
	ErrorManagerLogWrite(szText, FALSE);
	if (nLevel != 7)
	{
		__asm
		{
			call here
here:
			pop pbCode
			push esp
			pop pbStack
		}
		FogErrorManagerLog("Stack bytes:");
		ErrorManagerHexDump(pbStack, 768);
		FogErrorManagerLog("Code bytes:");
		ErrorManagerHexDump(pbCode, 16);
	}
	if (sgpfnErrorCallback)
		sgpfnErrorCallback();
	MessageBoxA(NULL, szBox, szTitle, MB_ICONEXCLAMATION);
	if (!fFatal)
		sgfInError = FALSE;
}

void __cdecl ErrorManagerLog(const char *pszFormat, ...)
{
	char szText[4096];
	char szLine[4096];
	va_list pArgs;

	if (!pszFormat)
		return;
	va_start(pArgs, pszFormat);
	vsprintf(szText, pszFormat, pArgs);
	va_end(pArgs);
	sprintf(szLine, "%s\n", szText);
	ErrorManagerLogWrite(szLine, FALSE);
}

void __cdecl ErrorManagerLogText(const char *pszFormat, ...)
{
	char szText[4096];
	va_list pArgs;

	if (!pszFormat)
		return;
	va_start(pArgs, pszFormat);
	_vsnprintf(szText, sizeof(szText), pszFormat, pArgs);
	va_end(pArgs);
	szText[sizeof(szText) - 1] = 0;
	ErrorManagerLogWrite(szText, FALSE);
}

BOOL __fastcall ErrorManagerIsInError(void)
{
	return sgfInError;
}

static void __fastcall ErrorManagerLogWrite(const char *pszText, BOOL fTimestamp)
{
	FILE *pFile;
	long tNow;
	struct tm *ptTime;
	struct tm tZero;

	pFile = fopen(sgszLogFile, "a");
	if (!pFile)
	{
		pFile = fopen(sgszLogFileRoot, "a");
		if (!pFile)
			return;
	}
	if (fTimestamp)
	{
		time(&tNow);
		ptTime = localtime(&tNow);
		if (!ptTime)
		{
			ptTime = &tZero;
			memset(ptTime, 0, sizeof(*ptTime));
		}
		ptTime->tm_year %= 100;
		fprintf(pFile, "%02d.%02d.%02d %02d:%02d:%02d  ", ptTime->tm_mon + 1, ptTime->tm_mday,
			ptTime->tm_year,
			ptTime->tm_hour, ptTime->tm_min, ptTime->tm_sec);
	}
	fprintf(pFile, pszText);
	fclose(pFile);
}

static void __fastcall ErrorManagerHexDump(const BYTE *pbData, int nBytes)
{
	FILE *pFile;
	UINT nLine;
	UINT i;
	char c;

	pFile = fopen(sgszLogFile, "a");
	if (!pFile)
	{
		pFile = fopen(sgszLogFileRoot, "a");
		if (!pFile)
			return;
	}
	while ((UINT)nBytes > 0)
	{
		nLine = nBytes;
		if ((int)nLine >= 16)
			nLine = 16;
		if (IsBadReadPtr(pbData, nLine))
			break;
		fprintf(pFile, "0x%08x: ", pbData);
		for (i = 0; i < 16; i++)
		{
			fprintf(pFile, i < nLine ? "%02x " : "   ", pbData[i]);
			if ((i & 3) == 3)
				fprintf(pFile, " ");
		}
		for (i = 0; i < nLine; i++)
		{
			c = (char)pbData[i];
			if (!isprint(c) || StormSStrChr(sgszHexDumpDots, (BYTE)c))
				c = '.';
			fprintf(pFile, "%c", c);
		}
		fprintf(pFile, "\n");
		pbData += nLine;
		nBytes -= nLine;
	}
	fprintf(pFile, "\n");
	fclose(pFile);
}

void __cdecl ErrorManagerSeTranslator(unsigned int nCode, EXCEPTION_POINTERS *ptException)
{
	HANDLE hThread;

	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &hThread, 0, FALSE,
		DUPLICATE_SAME_ACCESS);
	ErrorManagerCrashReport(hThread, ptException->ContextRecord);
	CloseHandle(hThread);
}

DWORD WINAPI ErrorManagerWatchdog(void *pParam)
{
	CONTEXT tContext;
	int i;

	WaitForSingleObject(sghStackDumpEvent, INFINITE);
	CloseHandle(sghStackDumpEvent);
	if (SuspendThread(sghStackDumpThread) == (DWORD)-1)
		printf("SuspendThread(): gle = %lu\n", GetLastError());
	else
	{
		FogErrorManagerLog("Suspended Victim Thread...");
		for (i = 0; i < (int)(sizeof(tContext) / 4); i++)
			((DWORD *)&tContext)[i] = 0;
		tContext.ContextFlags = CONTEXT_FULL;
		if (!GetThreadContext(sghStackDumpThread, &tContext))
			printf("GetThreadContext(): gle = %lu\n", GetLastError());
		else
			ErrorManagerCrashReport(sghStackDumpThread, &tContext);
	}
	sgfStackDumpDone++;
	Sleep(100);
	return 0;
}

static BOOL __fastcall ErrorManagerStartWatchdog(void)
{
	DWORD dwThreadId;

	if (sgfStackDumpReady)
		return TRUE;
	_set_se_translator(ErrorManagerSeTranslator);
	sghImageHlp = LoadLibraryA("imagehlp.dll");
	if (!sghImageHlp)
	{
		FogErrorManagerLog("Missing imagehlp.dll.  Can't generate better stack crawls!");
		return FALSE;
	}
	*(FARPROC *)&sgpfnSymCleanup = GetProcAddress(sghImageHlp, "SymCleanup");
	*(FARPROC *)&sgpfnSymFunctionTableAccess = GetProcAddress(sghImageHlp, "SymFunctionTableAccess");
	*(FARPROC *)&sgpfnSymGetLineFromAddr = GetProcAddress(sghImageHlp, "SymGetLineFromAddr");
	*(FARPROC *)&sgpfnSymGetModuleBase = GetProcAddress(sghImageHlp, "SymGetModuleBase");
	*(FARPROC *)&sgpfnSymGetModuleInfo = GetProcAddress(sghImageHlp, "SymGetModuleInfo");
	*(FARPROC *)&sgpfnSymGetOptions = GetProcAddress(sghImageHlp, "SymGetOptions");
	*(FARPROC *)&sgpfnSymGetSymFromAddr = GetProcAddress(sghImageHlp, "SymGetSymFromAddr");
	*(FARPROC *)&sgpfnSymInitialize = GetProcAddress(sghImageHlp, "SymInitialize");
	*(FARPROC *)&sgpfnSymSetOptions = GetProcAddress(sghImageHlp, "SymSetOptions");
	*(FARPROC *)&sgpfnSymLoadModule = GetProcAddress(sghImageHlp, "SymLoadModule");
	*(FARPROC *)&sgpfnStackWalk = GetProcAddress(sghImageHlp, "StackWalk");
	sgpfnUnDecorateSymbolName = GetProcAddress(sghImageHlp, "UnDecorateSymbolName");
	sgpfnSplitSymbols = GetProcAddress(sghImageHlp, "SplitSymbols");
	if (!sgpfnSymCleanup || !sgpfnSymFunctionTableAccess || !sgpfnSymGetModuleBase || !sgpfnSymGetModuleInfo
		|| !sgpfnSymGetOptions || !sgpfnSymGetSymFromAddr || !sgpfnSymInitialize || !sgpfnSymSetOptions
		|| !sgpfnStackWalk || !sgpfnUnDecorateSymbolName || !sgpfnSymLoadModule || !sgpfnSplitSymbols)
	{
		FogErrorManagerLog("Missing functions in imagehlp.dll.  Can't generate better stack crawls!");
		return FALSE;
	}
	sgfStackDumpReady = TRUE;
	FogErrorManagerLog("imagehlp.dll loaded for better stack crawls.");
	sghStackDumpEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
	CreateThread(NULL, 0, ErrorManagerWatchdog, NULL, 0, &dwThreadId);
	return TRUE;
}

static BOOL __fastcall ErrorManagerGetModuleDir(char *pszPath, DWORD dwSize)
{
	int i;

	pszPath[0] = 0;
	if (!GetModuleFileNameA(NULL, pszPath, dwSize))
		return FALSE;
	for (i = (int)strlen(pszPath) - 1; i > 0; i--)
	{
		if (pszPath[i] == '\\' || pszPath[i] == '/' || pszPath[i] == ':')
			break;
	}
	if (i <= 0)
		return FALSE;
	if (pszPath[i] == ':')
		i++;
	pszPath[i] = 0;
	return TRUE;
}

static __inline void ErrorManagerFreeModules(ERRORMANAGERMODULE *ptModule)
{
	ERRORMANAGERMODULE *ptNext;
	void *pMemory;

	while (ptModule)
	{
		ptNext = ptModule->ptNext;
		pMemory = ptModule;
#line 1674
		MemPoolFree(NULL, &pMemory, D2_THIS_FILE, __LINE__);
		ptModule = ptNext;
	}
}

static ERRORMANAGERMODULE * __fastcall ErrorManagerListModulesToolhelp(DWORD dwProcessId);
static ERRORMANAGERMODULE * __fastcall ErrorManagerListModulesPsapi(DWORD dwProcessId, HANDLE hProcess);
static __inline ERRORMANAGERMODULE *ErrorManagerListModules(DWORD dwProcessId, HANDLE hProcess)
{
	ERRORMANAGERMODULE *ptModules;

	ptModules = ErrorManagerListModulesToolhelp(dwProcessId);
	if (ptModules)
		return ptModules;
	return ErrorManagerListModulesPsapi(dwProcessId, hProcess);
}
static DWORD WINAPI ErrorManagerGetModuleBase(HANDLE hProcess, DWORD dwAddress);
static int __fastcall ErrorManagerDescribeAddress(HANDLE hProcess, DWORD dwAddress, char *pszOut);

static BOOL __fastcall ErrorManagerCrashReport(HANDLE hThread, CONTEXT *ptContext)
{
	STACKFRAME tFrame;
	char szPath[512];
	char szModule[256];
	char szSymbolPath[4096];
	char *pszSearch;
	char szDescription[4096];
	ERRORMANAGERMODULE *ptModules;
	ERRORMANAGERMODULE *ptModule;
	ERRORMANAGERMODULE *ptNext;
	HANDLE hProcess;
	int nLength;
	int i;

	hProcess = GetCurrentProcess();
	szSymbolPath[0] = 0;
	if (GetCurrentDirectoryA(sizeof(szPath), szPath))
	{
		strcat(szSymbolPath, szPath);
		strcat(szSymbolPath, ";");
	}
	szPath[0] = 0;
	if (GetModuleFileNameA(NULL, szPath, sizeof(szPath)))
	{
		for (i = (int)strlen(szPath) - 1; i > 0; i--)
		{
			if (szPath[i] == '\\' || szPath[i] == '/' || szPath[i] == ':')
				break;
		}
		if (i > 0)
		{
			if (szPath[i] == ':')
				i++;
			szPath[i] = 0;
			strcat(szSymbolPath, szPath);
			strcat(szSymbolPath, ";");
		}
	}
	if (GetEnvironmentVariableA("_NT_SYMBOL_PATH", szPath, sizeof(szPath)))
	{
		strcat(szSymbolPath, szPath);
		strcat(szSymbolPath, ";");
	}
	if (GetEnvironmentVariableA("_NT_ALTERNATE_SYMBOL_PATH", szPath, sizeof(szPath)))
	{
		strcat(szSymbolPath, szPath);
		strcat(szSymbolPath, ";");
	}
	if (GetEnvironmentVariableA("SYSTEMROOT", szPath, sizeof(szPath)))
	{
		strcat(szSymbolPath, szPath);
		strcat(szSymbolPath, ";");
	}
	nLength = strlen(szSymbolPath);
	if (nLength)
	{
		szSymbolPath[nLength - 1] = 0;
		FogErrorManagerLog("User defined symbols path: %s", szSymbolPath);
	}
	else
		FogErrorManagerLog("Unable to generate symbols search path");
	pszSearch = szSymbolPath[0] ? szSymbolPath : NULL;
	if (!sgpfnSymInitialize(hProcess, pszSearch, FALSE))
		FogErrorManagerLog("SymInitialize(): gle = %lu", GetLastError());
	else
	{
		sgpfnSymSetOptions(sgpfnSymGetOptions() | SYMOPT_LOAD_LINES);
		i = GetCurrentProcessId();
		ptModules = ErrorManagerListModules(i, hProcess);
		FogErrorManagerLog("\nLoad symbols...");
		ptModule = ptModules;
		while (ptModule)
		{
			ptNext = ptModule->ptNext;
			strcpy(szPath, ptModule->szImagePath);
			strcpy(szModule, ptModule->szModuleName);
			if (!sgpfnSymLoadModule(hProcess, NULL, szPath, szModule, ptModule->dwBaseAddress,
				ptModule->dwImageSize))
				FogErrorManagerLog("Error (%d) loading symbols for '%s'", GetLastError(), szModule);
			else
				FogErrorManagerLog("Symbols Loaded for '%s'", szModule);
			ptModule = ptNext;
		}
		ErrorManagerFreeModules(ptModules);
		for (i = 0; i < (int)(sizeof(tFrame) / 4); i++)
			((DWORD *)&tFrame)[i] = 0;
		tFrame.AddrPC.Offset = ptContext->Eip;
		tFrame.AddrPC.Mode = AddrModeFlat;
		tFrame.AddrStack.Mode = AddrModeFlat;
		tFrame.AddrStack.Offset = ptContext->Esp;
		tFrame.AddrFrame.Offset = ptContext->Ebp;
		tFrame.AddrFrame.Mode = AddrModeFlat;
		FogErrorManagerLog("\nStack Crawl:");
		for (i = 0; i < 512; i++)
		{
			if (!sgpfnStackWalk(IMAGE_FILE_MACHINE_I386, hProcess, hThread, &tFrame, ptContext, NULL,
				sgpfnSymFunctionTableAccess, ErrorManagerGetModuleBase, NULL))
			{
				FogErrorManagerLog("Stack walk error: %d\n", GetLastError());
				break;
			}
			if (i > 1)
			{
				FogErrorManagerLog("%3d  %c%c  addr:%08lx  ret:%08lx  frame:%08lx  stack:%08lx", i - 1,
					tFrame.Far ? 'F' : '.', tFrame.Virtual ? 'V' : '.', tFrame.AddrPC.Offset,
						tFrame.AddrReturn.Offset,
					tFrame.AddrFrame.Offset, tFrame.AddrStack.Offset);
				if (tFrame.AddrPC.Offset && ErrorManagerDescribeAddress(hProcess, tFrame.AddrPC.Offset,
					szDescription))
					FogErrorManagerLog("%s", szDescription);
			}
		}
		FogErrorManagerLog("%d frames dumped", i);
	}
	ResumeThread(hThread);
	sgpfnSymCleanup(hProcess);
	return TRUE;
}

static DWORD WINAPI ErrorManagerGetModuleBase(HANDLE hProcess, DWORD dwAddress)
{
	MEMORY_BASIC_INFORMATION tMemory;
	char szFile[MAX_PATH];
	IMAGEHLP_MODULE tModule;
	DWORD dwLength;
	HANDLE hFile;

	if (!sgfStackDumpReady)
	{
		FogErrorManagerLog("sDumpStack: imagehlp.dll not loaded");
		return 0;
	}
	tModule.SizeOfStruct = sizeof(tModule);
	if (sgpfnSymGetModuleInfo(hProcess, dwAddress, &tModule))
		return tModule.BaseOfImage;
	if (!(!VirtualQueryEx(hProcess, (void *)(INT_PTR)dwAddress, &tMemory, sizeof(tMemory)))) {
		dwLength = GetModuleFileNameA((HMODULE)tMemory.AllocationBase, szFile, sizeof(szFile));
		hFile = NULL;
		if (dwLength)
			hFile = CreateFileA(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		sgpfnSymLoadModule(hProcess, hFile, dwLength ? szFile : NULL, NULL,
			(DWORD)(INT_PTR)tMemory.AllocationBase, 0);
		return (DWORD)(INT_PTR)tMemory.AllocationBase; }
	return 0;
}

static int __fastcall ErrorManagerDescribeAddress(HANDLE hProcess, DWORD dwAddress, char *pszOut)
{
	BYTE abSymbol[284];
	IMAGEHLP_SYMBOL *ptSymbol;
	IMAGEHLP_MODULE tModule;
	IMAGEHLP_LINE tLine;
	DWORD dwDisplacement;
	char *pszText;
	char *pszSlash;
	int i;

	ptSymbol = (IMAGEHLP_SYMBOL *)abSymbol;
	for (i = 0; i < 71; i++)
		((DWORD *)abSymbol)[i] = 0;
	ptSymbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
	for (i = 0; i < 143; i++)
		((DWORD *)&tModule)[i] = 0;
	ptSymbol->Address = dwAddress;
	ptSymbol->MaxNameLength = MAX_PATH;
	tModule.SizeOfStruct = sizeof(tModule);
	pszText = pszOut + wsprintfA(pszOut, "0x%08X ", dwAddress);
	if (sgpfnSymGetModuleInfo(hProcess, dwAddress, &tModule))
	{
		pszSlash = strrchr(tModule.ImageName, '\\');
		pszText += wsprintfA(pszText, "%s: ", pszSlash ? pszSlash + 1 : tModule.ImageName);
	}
	else
		pszText += wsprintfA(pszText, "<unknown module>: ");
	if (sgpfnSymGetSymFromAddr(hProcess, dwAddress, &dwDisplacement, ptSymbol))
	{
		if (!dwDisplacement)
			pszText += wsprintfA(pszText, "%s", ptSymbol->Name);
		else
			pszText += wsprintfA(pszText, "%s + %d bytes", ptSymbol->Name, dwDisplacement);
		memset(&tLine, 0, sizeof(tLine));
		tLine.SizeOfStruct = sizeof(tLine);
		if (!sgfStackDumpReady)
			FogErrorManagerLog("sDumpStack: imagehlp.dll not loaded");
		else if (sgpfnSymGetLineFromAddr)
		{
			i = 0;
			while (!sgpfnSymGetLineFromAddr(hProcess, dwAddress - i, &dwDisplacement, &tLine))
			{
				i++;
				if (i == 100)
					return pszText - pszOut;
			}
			if (i)
				dwDisplacement = i;
			pszText += wsprintfA(pszText, "\n\t\t%s, Line %d", tLine.FileName, tLine.LineNumber);
			if (dwDisplacement)
				pszText += wsprintfA(pszText, " + %d bytes", dwDisplacement);
		}
	}
	else
	{
		FogErrorManagerLog("GetLastError: %d", GetLastError());
		pszText += wsprintfA(pszText, "<unknown symbol>");
	}
	return pszText - pszOut;
}

static ERRORMANAGERMODULE * __fastcall ErrorManagerListModulesToolhelp(DWORD dwProcessId)
{
	HANDLE (WINAPI *pfnCreateSnapshot)(DWORD, DWORD);
	BOOL (WINAPI *pfnModuleFirst)(HANDLE, LPMODULEENTRY32);
	BOOL (WINAPI *pfnModuleNext)(HANDLE, LPMODULEENTRY32);
	MODULEENTRY32 tEntry;
	ERRORMANAGERMODULE *ptModules;
	ERRORMANAGERMODULE *ptModule;
	HMODULE hLibrary;
	HANDLE hSnapshot;
	int i;

	ptModules = NULL;
	pfnCreateSnapshot = NULL;
	pfnModuleFirst = NULL;
	pfnModuleNext = NULL;
	hLibrary = NULL;
	const char *apszLibraries[2] = { "kernel32.dll", "tlhelp32.dll" };
	for (i = 0; i < 2; i++)
	{
		hLibrary = LoadLibraryA(apszLibraries[i]);
		if (!hLibrary)
			continue;
		*(FARPROC *)&pfnCreateSnapshot = GetProcAddress(hLibrary, "CreateToolhelp32Snapshot");
		*(FARPROC *)&pfnModuleFirst = GetProcAddress(hLibrary, "Module32First");
		*(FARPROC *)&pfnModuleNext = GetProcAddress(hLibrary, "Module32Next");
		if (pfnCreateSnapshot && pfnModuleFirst && pfnModuleNext)
			break;
		FreeLibrary(hLibrary);
		hLibrary = NULL;
	}
	if (!hLibrary)
	{
		FogErrorManagerLog("Unable to load toolhelp library");
		return NULL;
	}
	hSnapshot = pfnCreateSnapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		FogErrorManagerLog("Unable to get toolhelp snapshot");
		return NULL;
	}
	FogErrorManagerLog("\nEnumerate modules...");
	if (pfnModuleFirst(hSnapshot, &tEntry))
	{
		do
		{
			FogErrorManagerLog("[Module Entry]  Base Addr:%08lXh  Base Size:%9lu   Name:%-15.15s  Path:%s",
				tEntry.modBaseAddr, tEntry.modBaseSize, tEntry.szModule, tEntry.szExePath);
#line 1765
			ptModule = (ERRORMANAGERMODULE *)MemPoolAlloc(NULL, 528, D2_THIS_FILE, __LINE__); if (!ptModule) FogErrorManagerReport(2, D2_THIS_FILE, __LINE__, "");

			for (i = 0; i < (int)(sizeof(ERRORMANAGERMODULE) / 4); i++)
				((DWORD *)ptModule)[i] = 0;
			strcpy(ptModule->szImagePath, tEntry.szExePath);
			strcpy(ptModule->szModuleName, tEntry.szModule);
			ptModule->dwBaseAddress = (DWORD)(INT_PTR)tEntry.modBaseAddr;
			ptModule->dwImageSize = tEntry.modBaseSize;
			ptModule->ptNext = ptModules;
			ptModules = ptModule;
		}
		while (pfnModuleNext(hSnapshot, &tEntry));
	}
	CloseHandle(hSnapshot);
	FreeLibrary(hLibrary);
	return ptModules;
}

static ERRORMANAGERMODULE * __fastcall ErrorManagerListModulesPsapi(DWORD dwProcessId, HANDLE hProcess)
{
	BOOL (WINAPI *pfnEnumModules)(HANDLE, HMODULE *, DWORD, LPDWORD);
	DWORD (WINAPI *pfnGetFileNameEx)(HANDLE, HMODULE, LPSTR, DWORD);
	DWORD (WINAPI *pfnGetBaseName)(HANDLE, HMODULE, LPSTR, DWORD);
	BOOL (WINAPI *pfnGetInformation)(HANDLE, HMODULE, LPMODULEINFO, DWORD);
	HMODULE ahModules[256];
	MODULEINFO tInfo;
	char szPath[MAX_PATH];
	char szName[256];
	ERRORMANAGERMODULE *ptModules;
	ERRORMANAGERMODULE *ptModule;
	HMODULE hLibrary;
	DWORD dwNeeded;
	int nModules;
	int i;
	int k;

	ptModules = NULL;
	hLibrary = LoadLibraryA("psapi.dll");
	if (!hLibrary)
		return NULL;
	*(FARPROC *)&pfnEnumModules = GetProcAddress(hLibrary, "EnumProcessModules");
	*(FARPROC *)&pfnGetFileNameEx = GetProcAddress(hLibrary, "GetModuleFileNameExA");
	*(FARPROC *)&pfnGetBaseName = GetProcAddress(hLibrary, "GetModuleBaseNameA");
	*(FARPROC *)&pfnGetInformation = GetProcAddress(hLibrary, "GetModuleInformation");
	if (!pfnEnumModules || !pfnGetFileNameEx || !pfnGetBaseName || !pfnGetInformation)
	{
		FogErrorManagerLog("Unable to load psapi library");
		goto Failed;
	}
	if (!pfnEnumModules(hProcess, ahModules, sizeof(ahModules), &dwNeeded))
	{
		FogErrorManagerLog("Error enumerating modules using PSAPI:  error(%d)", GetLastError());
		goto Failed;
	}
	nModules = dwNeeded >> 2;
	if (nModules > 256)
	{
		FogErrorManagerLog("PSAPI EnumProcessModules returned invalid number of modules");
		goto Failed;
	}
	FogErrorManagerLog("\nEnumerate modules...");
	for (i = 0; i < nModules; i++)
	{
		if (!pfnGetInformation(hProcess, ahModules[i], &tInfo, sizeof(tInfo)))
			goto InformationFailed;
		if (!pfnGetFileNameEx(hProcess, ahModules[i], szPath, sizeof(szPath)))
			goto FileNameFailed;
		if (!pfnGetBaseName(hProcess, ahModules[i], szName, 255))
			goto BaseNameFailed;
		FogErrorManagerLog("[Module Entry]  Base Addr:%08lXh  Base Size:%9lu   Name:%-15.15s  Path:%s\n",
			tInfo.lpBaseOfDll, tInfo.SizeOfImage, szName, szPath);
#line 1873
		ptModule = (ERRORMANAGERMODULE *)MemPoolAlloc(NULL, 528, D2_THIS_FILE, __LINE__); if (!ptModule) FogErrorManagerReport(2, D2_THIS_FILE, __LINE__, "");
		for (k = 0; k < (int)(sizeof(ERRORMANAGERMODULE) / 4); k++)
			((DWORD *)ptModule)[k] = 0;
		strcpy(ptModule->szImagePath, szPath);
		strcpy(ptModule->szModuleName, szName);
		ptModule->dwBaseAddress = (DWORD)(INT_PTR)tInfo.lpBaseOfDll;
		ptModule->dwImageSize = tInfo.SizeOfImage;
		ptModule->ptNext = ptModules;
		ptModules = ptModule;
	}
	FreeLibrary(hLibrary);
	return ptModules;
InformationFailed:
	FogErrorManagerLog("ERROR: PSAPI GetModuleInformation (%d)", GetLastError());
	goto Failed;
FileNameFailed:
	FogErrorManagerLog("ERROR: PSAPI GetModuleFileNameExA (%d)", GetLastError());
	goto Failed;
BaseNameFailed:
	FogErrorManagerLog("ERROR: PSAPI GetModuleBaseNameA (%d)", GetLastError());
Failed:
	FreeLibrary(hLibrary);
	ErrorManagerFreeModules(ptModules);
	return NULL;
}

void __fastcall ErrorManagerSetPoolsEnabled(BOOL fEnabled)
{
	sgfPoolsEnabled = fEnabled;
}

void * __fastcall ErrorManagerAlloc(int nSize, const char *pszFile, int nLine, int nUnused)
{
	void *pMemory;

	pMemory = MemPoolAlloc(NULL, nSize, pszFile, nLine);
	if (!pMemory)
		FogErrorManagerReport(2, pszFile, nLine, "");
	return pMemory;
}

BOOL __fastcall ErrorManagerFree(void *pMemory, const char *pszFile, int nLine, int nUnused)
{
	MemPoolFree(NULL, &pMemory, pszFile, nLine);
	return TRUE;
}

void * __fastcall ErrorManagerRealloc(void *pMemory, int nSize, const char *pszFile, int nLine, int nUnused)
{
	void *pNew;

	pNew = MemPoolRealloc(NULL, pMemory, nSize, pszFile, nLine);
	if (!pNew)
		FogErrorManagerReport(2, pszFile, nLine, "");
	return pNew;
}

void * __fastcall ErrorManagerPoolAlloc(D2POOLSYSTEM *pPool, int nSize, const char *pszFile, int nLine,
	int nUnused)
{
	void *pMemory;

	pMemory = MemPoolAlloc(sgfPoolsEnabled ? pPool : NULL, nSize, pszFile, nLine);
	if (!pMemory)
		FogErrorManagerReport(2, pszFile, nLine, "");
	return pMemory;
}

BOOL __fastcall ErrorManagerPoolFree(D2POOLSYSTEM *pPool, void *pMemory, const char *pszFile, int nLine,
	int nUnused)
{
	MemPoolFree(sgfPoolsEnabled ? pPool : NULL, &pMemory, pszFile, nLine);
	return TRUE;
}

void * __fastcall ErrorManagerPoolRealloc(D2POOLSYSTEM *pPool, void *pMemory, int nSize, const char *pszFile,
	int nLine, int nUnused)
{
	void *pNew;

	pNew = MemPoolRealloc(sgfPoolsEnabled ? pPool : NULL, pMemory, nSize, pszFile, nLine);
	if (!pNew)
		FogErrorManagerReport(2, pszFile, nLine, "");
	return pNew;
}
