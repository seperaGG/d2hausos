#include <windows.h>
#include <stdarg.h>
#include <stddef.h>
#include "D2VidTstHell.h"
#include "StormImports.h"
#include "HellFault.h"

static CRITICAL_SECTION sgtMessageSourceLock;
static TSExplicitList<TMESSAGESOURCE, 0> sgtMessageSources;
static BOOL sgfMessageSourcesInit = 0;
static PFNASSERTHOOK sgpfnAssertHook = { 0 };

static const char * const sgpszAssertFormat = "An assertion has failed.\n\nFile:\t\t%s\nLine:\t\t%d\n"
	"Expression:\t%s\n\nAbort to exit, Retry to break, Ignore to continue";
static const char * const sgpszDefaultMessage = "D2Hell Fault.cpp error.";

void __cdecl FaultInitSourceLock(void);
void __cdecl FaultFreeSourcesAndLock(void);
void __fastcall FaultGetMessage(DWORD dwError, char *pszBuffer, int nSize);

PFNASSERTHOOK __fastcall FaultSetAssertHook(PFNASSERTHOOK pfnHook)
{
	PFNASSERTHOOK pfnPrev;

	pfnPrev = sgpfnAssertHook;
	sgpfnAssertHook = pfnHook;
	return pfnPrev;
}

void __fastcall FaultAddMessageSource(PFNMESSAGESOURCE pfnSource)
{
	if (!sgfMessageSourcesInit)
		FaultInitSourceLock();
	EnterCriticalSection(&sgtMessageSourceLock);
	sgtMessageSources.NewNode(LINK_AFTER, 0, 0)->pfnMessageSource = pfnSource;
	LeaveCriticalSection(&sgtMessageSourceLock);
}

void __cdecl FaultInitSourceLock(void)
{
	if (sgfMessageSourcesInit)
		return;
	InitializeCriticalSection(&sgtMessageSourceLock);
	atexit(FaultFreeSourcesAndLock);
	sgfMessageSourcesInit = TRUE;
}

void __cdecl FaultFreeSourcesAndLock(void)
{
	EnterCriticalSection(&sgtMessageSourceLock);
	sgtMessageSources.DeleteAll();
	LeaveCriticalSection(&sgtMessageSourceLock);
	DeleteCriticalSection(&sgtMessageSourceLock);
	sgfMessageSourcesInit = FALSE;
}

void __fastcall FaultGetMessage(DWORD dwError, char *pszBuffer, int nSize)
{
	TMESSAGESOURCE *ptSource;

	if (sgfMessageSourcesInit)
	{
		EnterCriticalSection(&sgtMessageSourceLock);
		for (ptSource = sgtMessageSources.Head(); (INT_PTR)ptSource > 0;
			ptSource = sgtMessageSources.Link(ptSource)->m_next)
		{
			if (ptSource->pfnMessageSource(dwError, pszBuffer, nSize))
			{
				LeaveCriticalSection(&sgtMessageSourceLock);
				return;
			}
		}
		LeaveCriticalSection(&sgtMessageSourceLock);
	}
	lstrcpynA(pszBuffer, sgpszDefaultMessage, nSize);
}

BOOL __cdecl FaultTerminate(void)
{
	return TerminateProcess(GetCurrentProcess(), (UINT)-1);
}

BOOL __cdecl FaultErrorRetryBox(DWORD dwError, ...)
{
	char szFormat[512];
	char szText[512];
	va_list pArgs;

	FaultGetMessage(dwError, szFormat, sizeof szFormat);
	va_start(pArgs, dwError);
	wvsprintfA(szText, szFormat, pArgs);
	va_end(pArgs);
	if (MessageBoxA(NULL, szText, gpszD2VidTstExe, MB_RETRYCANCEL | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL)
		== IDRETRY)
		return TRUE;
	TerminateProcess(GetCurrentProcess(), (UINT)-1);
	return FALSE;
}

BOOL __fastcall FaultAbortRetryBox(const char *pszText)
{
	int nResult;

	nResult = MessageBoxA(NULL, pszText, gpszD2VidTstExe,
		MB_ABORTRETRYIGNORE | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL);
	if (nResult == IDABORT)
	{
		TerminateProcess(GetCurrentProcess(), (UINT)-1);
		return TRUE;
	}
	return nResult == IDRETRY;
}

BOOL __cdecl FaultErrorAbortRetryBox(DWORD dwError, ...)
{
	char szFormat[512];
	char szText[512];
	va_list pArgs;

	FaultGetMessage(dwError, szFormat, sizeof szFormat);
	va_start(pArgs, dwError);
	wvsprintfA(szText, szFormat, pArgs);
	va_end(pArgs);
	return FaultAbortRetryBox(szText);
}

BOOL __fastcall FaultOkCancelBox(const char *pszMessage)
{
	return MessageBoxA(NULL, pszMessage, gpszD2VidTstExe, MB_OKCANCEL | MB_ICONHAND | MB_TASKMODAL)
		== IDCANCEL;
}

BOOL __cdecl FaultErrorBox(DWORD dwError, ...)
{
	char szFormat[512];
	char szText[512];
	va_list pArgs;

	FaultGetMessage(dwError, szFormat, sizeof szFormat);
	va_start(pArgs, dwError);
	wvsprintfA(szText, szFormat, pArgs);
	va_end(pArgs);
	return MessageBoxA(NULL, szText, gpszD2VidTstExe, MB_OKCANCEL | MB_ICONHAND | MB_TASKMODAL) == IDCANCEL;
}

BOOL __fastcall FaultAssertBox(const char *pszFile, int nLine, const char *pszExpression)
{
	char szText[512];
	int nResult;

	if (sgpfnAssertHook)
		sgpfnAssertHook(pszFile, nLine, pszExpression);
	wsprintfA(szText, sgpszAssertFormat, pszFile, nLine, pszExpression);
	nResult = MessageBoxA(NULL, szText, gpszD2VidTstExe,
		MB_ABORTRETRYIGNORE | MB_ICONHAND | MB_DEFBUTTON3 | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
	if (nResult == IDABORT)
	{
		TerminateProcess(GetCurrentProcess(), (UINT)-1);
		return TRUE;
	}
	return nResult == IDRETRY;
}
