#define _WIN32_WINNT 1280
#include <windows.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <commdlg.h>
#include <imagehlp.h>
#include "D2gfxMisc.h"

extern const char *const gpszHellNoName;

typedef BOOL (__stdcall *PFNSYMINITIALIZE)(HANDLE hProcess, LPSTR pszSearchPath, BOOL fInvade);
typedef BOOL (__stdcall *PFNSYMCLEANUP)(HANDLE hProcess);
typedef BOOL (__stdcall *PFNSTACKWALK)(DWORD dwMachine, HANDLE hProcess, HANDLE hThread, STACKFRAME *ptFrame,
	void *pContext, PREAD_PROCESS_MEMORY_ROUTINE pfnRead, PFUNCTION_TABLE_ACCESS_ROUTINE pfnTable,
	PGET_MODULE_BASE_ROUTINE pfnBase, PTRANSLATE_ADDRESS_ROUTINE pfnTranslate);
typedef BOOL (__stdcall *PFNSYMGETSYMFROMADDR)(HANDLE hProcess, DWORD dwAddress, DWORD *pdwDisplacement,
	IMAGEHLP_SYMBOL *ptSymbol);
typedef BOOL (__stdcall *PFNGETSAVEFILENAME)(OPENFILENAMEA *ptFile);

static LPTOP_LEVEL_EXCEPTION_FILTER sgpfnFaultPrevFilter = { 0 };
static PFNSYMINITIALIZE sgpfnSymInitialize = { 0 };
static PFNSYMCLEANUP sgpfnSymCleanup = { 0 };
static PFNSTACKWALK sgpfnStackWalk = { 0 };
static PFUNCTION_TABLE_ACCESS_ROUTINE sgpfnSymFunctionTableAccess = { 0 };
static PGET_MODULE_BASE_ROUTINE sgpfnSymGetModuleBase = { 0 };
static PFNSYMGETSYMFROMADDR sgpfnSymGetSymFromAddr = { 0 };
BOOL gfInDebugBreak = 0;
static HFONT sghFaultFont = { 0 };
static const char *const sgpszFaultImagehlp = "IMAGEHLP.DLL";
static const char *const sgpszFaultSymInitialize = "SymInitialize";
static const char *const sgpszFaultSymCleanup = "SymCleanup";
static const char *const sgpszFaultStackWalk = "StackWalk";
static const char *const sgpszFaultSymFunctionTableAccess = "SymFunctionTableAccess";
static const char *const sgpszFaultSymGetModuleBase = "SymGetModuleBase";
static const char *const sgpszFaultSymGetSymFromAddr = "SymGetSymFromAddr";
static char sgszFaultLogFile[260] = "D2Except.log";
static const WCHAR *const sgpwszFaultCaption = L"Diablo II Exception";
static const WCHAR *const sgpwszFaultSave = L"Write to File";
static const WCHAR *const sgpwszFaultCopy = L"Copy to Clipboard";
static const WCHAR *const sgpwszFaultContinue = L"Continue";
static const WCHAR *const sgpwszFaultAbort = L"Abort";
static const WCHAR *const sgpwszFaultFont = L"MS Sans Serif";
static const char *const sgpszFaultFontFace = "Lucida Console";
static const char *const sgpszFaultComDlg = "COMDLG32.DLL";
static const char *const sgpszFaultSaveTitle = "Save Exception Report";
static const char *const sgpszFaultFilter =
	"Text Files (*.txt)\0*.txt\0Log Files (*.log)\0*.log\0";
static const char *const sgpszFaultGetSaveFileName = "GetSaveFileNameA";

#define FAULT_REPORTSIZE	2048
#define FAULT_IDSAVE		100
#define FAULT_IDCOPY		101
#define FAULT_IDCONTINUE	102
#define FAULT_IDTEXT		200
#define OPCODE_INT3			204
#define FAULT_SYMNAME		512

#define FAULT_DLGSTYLE		(WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_CENTER | DS_MODALFRAME | DS_SETFONT)
#define FAULT_BUTTONSTYLE	(WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON)
#define FAULT_EDITSTYLE		(WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP | ES_MULTILINE \
	| ES_WANTRETURN)
#define FAULT_CLASSMARK		65535
#define FAULT_BUTTON		128
#define FAULT_EDIT			129

#define FAULT_ALIGN(pw) \
	if ((DWORD)(pw) & 3) \
		(pw) = (WORD *)(((DWORD)(pw) + 4) & ~3)

LONG __stdcall FaultExceptionFilter(EXCEPTION_POINTERS *ptException);
LONG __stdcall FaultReport(EXCEPTION_POINTERS *ptException);
const char *__fastcall FaultExceptionName(DWORD dwCode);
BOOL __fastcall FaultLogicalAddress(void *pAddress, char *pszModule, DWORD dwSize, DWORD *pdwSection,
	DWORD *pdwOffset);
LONG __fastcall FaultReportDialog(const char *pszReport);
void __fastcall FaultBuildTemplate(WORD *pwTemplate);
BOOL __stdcall FaultDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void __fastcall FaultSaveReport(HWND hDlg);
BOOL __fastcall FaultCopyReport(HWND hDlg);
BOOL __fastcall FaultInitDialog(HWND hDlg, const char *pszReport);
int __fastcall FaultCallStack(char *pszText, CONTEXT *ptContext);
int __fastcall FaultFrameWalk(char *pszText, CONTEXT *ptContext);
BOOL __cdecl FaultLoadImagehlp(void);
int __fastcall FaultStackWalk(char *pszText, CONTEXT *ptContext);

BOOL __fastcall FaultInstallFilter(void)
{
	sgpfnFaultPrevFilter = SetUnhandledExceptionFilter(FaultExceptionFilter);
	return TRUE;
}

LONG __stdcall FaultExceptionFilter(EXCEPTION_POINTERS *ptException)
{
	CONTEXT *ptContext;

	if (!ptException->ExceptionRecord)
		return EXCEPTION_CONTINUE_SEARCH;
	if (ptException->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT && gfInDebugBreak)
	{
		ptContext = ptException->ContextRecord;
		if (*(BYTE *)ptContext->Eip == OPCODE_INT3)
			ptContext->Eip++;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	FaultReport(ptException);
	if (sgpfnFaultPrevFilter)
		return sgpfnFaultPrevFilter(ptException);
	return EXCEPTION_CONTINUE_SEARCH;
}

LONG __stdcall FaultReport(EXCEPTION_POINTERS *ptException)
{
	char szText[FAULT_REPORTSIZE];
	char szModule[MAX_PATH];
	DWORD dwSection;
	DWORD dwOffset;
	EXCEPTION_RECORD *ptRecord;
	CONTEXT *ptContext;
	char *pszEnd;

	ptRecord = ptException->ExceptionRecord;
	pszEnd = szText + wsprintfA(szText, "Exception code:\t%s (%08X)\r\n",
		FaultExceptionName(ptRecord->ExceptionCode), ptRecord->ExceptionCode);
	if (FaultLogicalAddress(ptRecord->ExceptionAddress, szModule, sizeof szModule, &dwSection, &dwOffset))
		pszEnd += wsprintfA(pszEnd, "Fault address:\t%08X %02X:%08X (%s)\r\n",
			(DWORD)ptRecord->ExceptionAddress, dwSection, dwOffset, szModule);
	else
		pszEnd += wsprintfA(pszEnd, "Fault address:\t%08X (No logical address)\r\n\r\n",
			(DWORD)ptRecord->ExceptionAddress);
	ptContext = ptException->ContextRecord;
	pszEnd += wsprintfA(pszEnd, "Registers:\r\n");
	pszEnd += wsprintfA(pszEnd,
		"\tEAX:  %08X\tEBX:  %08X\r\n\tECX:  %08X\tEDX:  %08X\r\n\tESI:  %08X\tEDI:  %08X\r\n",
		ptContext->Eax, ptContext->Ebx, ptContext->Ecx, ptContext->Edx, ptContext->Esi, ptContext->Edi);
	pszEnd += wsprintfA(pszEnd, "\tCS:EIP:  %04X:%08X\r\n", ptContext->SegCs, ptContext->Eip);
	pszEnd += wsprintfA(pszEnd, "\tSS:ESP:  %04X:%08X\r\n", ptContext->SegSs, ptContext->Esp);
	pszEnd += wsprintfA(pszEnd, "\tSS:EBP:  %04X:%08X\r\n", ptContext->SegSs, ptContext->Ebp);
	pszEnd += wsprintfA(pszEnd, "\tDS:%04X  ES:%04X  FS:%04X  GS:%04X\r\n", ptContext->SegDs,
		ptContext->SegEs, ptContext->SegFs, ptContext->SegGs);
	pszEnd += wsprintfA(pszEnd, "\tFlags:%08X\r\n", ptContext->EFlags);
	pszEnd += FaultCallStack(pszEnd, ptContext);
	wsprintfA(pszEnd, "================================================\r\n\r\n");
	return FaultReportDialog(szText);
}

const char *__fastcall FaultExceptionName(DWORD dwCode)
{
	switch (dwCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return "ACCESS_VIOLATION";
	case EXCEPTION_SINGLE_STEP:
		return "SINGLE_STEP";
	case EXCEPTION_BREAKPOINT:
		return "BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "DATATYPE_MISALIGNMENT";
	case EXCEPTION_GUARD_PAGE:
		return "GUARD_PAGE";
	case EXCEPTION_IN_PAGE_ERROR:
		return "IN_PAGE_ERROR";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_INVALID_DISPOSITION:
		return "INVALID_DISPOSITION";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return "NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "ILLEGAL_INSTRUCTION";
	case EXCEPTION_INVALID_HANDLE:
		return "INVALID_HANDLE";
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:
		return "FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:
		return "FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:
		return "FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:
		return "FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:
		return "FLT_UNDERFLOW";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return "INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:
		return "INT_OVERFLOW";
	case EXCEPTION_PRIV_INSTRUCTION:
		return "PRIV_INSTRUCTION";
	case EXCEPTION_STACK_OVERFLOW:
		return "STACK_OVERFLOW";
	}
	return gpszHellNoName;
}

BOOL __fastcall FaultLogicalAddress(void *pAddress, char *pszModule, DWORD dwSize, DWORD *pdwSection,
	DWORD *pdwOffset)
{
	MEMORY_BASIC_INFORMATION tInfo;
	DWORD dwModule;
	IMAGE_NT_HEADERS *ptHeaders;
	IMAGE_SECTION_HEADER *ptSection;
	DWORD dwRva;
	DWORD dwStart;
	DWORD dwEnd;
	DWORD i;

	*pdwSection = 0;
	*pdwOffset = 0;
	*pszModule = '\0';
	if (!VirtualQuery(pAddress, &tInfo, sizeof tInfo))
		return FALSE;
	dwModule = (DWORD)tInfo.AllocationBase;
	if (!GetModuleFileNameA((HMODULE)dwModule, pszModule, dwSize))
		return FALSE;
	if (!dwModule)
		return FALSE;
	ptHeaders = (IMAGE_NT_HEADERS *)(dwModule + ((IMAGE_DOS_HEADER *)dwModule)->e_lfanew);
	ptSection = IMAGE_FIRST_SECTION(ptHeaders);
	dwRva = (DWORD)pAddress - dwModule;
	for (i = 0; i < ptHeaders->FileHeader.NumberOfSections; i++, ptSection++)
	{
		dwStart = ptSection->VirtualAddress;
		dwEnd = dwStart + (ptSection->SizeOfRawData > ptSection->Misc.VirtualSize ? ptSection->SizeOfRawData
			: ptSection->Misc.VirtualSize);
		if (dwRva >= dwStart && dwRva <= dwEnd)
		{
			*pdwSection = i + 1;
			*pdwOffset = dwRva - dwStart;
			return TRUE;
		}
	}
	return FALSE;
}

LONG __fastcall FaultReportDialog(const char *pszReport)
{
	DWORD adwTemplate[1024 / sizeof(DWORD)];

	FaultBuildTemplate((WORD *)adwTemplate);
	return DialogBoxIndirectParamA(GetModuleHandleA(NULL), (DLGTEMPLATE *)adwTemplate, NULL,
		(DLGPROC)FaultDialogProc, (LPARAM)pszReport) == FAULT_IDCONTINUE ? EXCEPTION_CONTINUE_EXECUTION
		: EXCEPTION_CONTINUE_SEARCH;
}

void __fastcall FaultBuildTemplate(WORD *pwTemplate)
{
	WORD *pw;

	pw = pwTemplate;
	*pw++ = LOWORD(FAULT_DLGSTYLE);
	*pw++ = HIWORD(FAULT_DLGSTYLE);
	*pw++ = 0;
	*pw++ = 0;
	*pw++ = 5;
	*pw++ = 0;
	*pw++ = 0;
	*pw++ = 291;
	*pw++ = 135;
	*pw++ = 0;
	*pw++ = 0;
	lstrcpyW((WCHAR *)pw, sgpwszFaultCaption);
	pw += lstrlenW(sgpwszFaultCaption) + 1;
	*pw++ = 8;
	lstrcpyW((WCHAR *)pw, sgpwszFaultFont);
	pw += lstrlenW(sgpwszFaultFont) + 1;
	FAULT_ALIGN(pw);
	*pw++ = LOWORD(FAULT_BUTTONSTYLE);
	*pw++ = HIWORD(FAULT_BUTTONSTYLE);
	*pw++ = 0;
	*pw++ = 0;
	*pw++ = 34;
	*pw++ = 114;
	*pw++ = 74;
	*pw++ = 14;
	*pw++ = FAULT_IDSAVE;
	*pw++ = FAULT_CLASSMARK;
	*pw++ = FAULT_BUTTON;
	lstrcpyW((WCHAR *)pw, sgpwszFaultSave);
	pw += lstrlenW(sgpwszFaultSave) + 1;
	*pw++ = 0;
	FAULT_ALIGN(pw);
	*pw++ = LOWORD(FAULT_BUTTONSTYLE);
	*pw++ = HIWORD(FAULT_BUTTONSTYLE);
	*pw++ = 0;
	*pw++ = 0;
	*pw++ = 127;
	*pw++ = 114;
	*pw++ = 74;
	*pw++ = 14;
	*pw++ = FAULT_IDCOPY;
	*pw++ = FAULT_CLASSMARK;
	*pw++ = FAULT_BUTTON;
	lstrcpyW((WCHAR *)pw, sgpwszFaultCopy);
	pw += lstrlenW(sgpwszFaultCopy) + 1;
	*pw++ = 0;
	FAULT_ALIGN(pw);
	*pw++ = LOWORD(FAULT_BUTTONSTYLE);
	*pw++ = HIWORD(FAULT_BUTTONSTYLE);
	*pw++ = 0;
	*pw++ = 0;
	*pw++ = 234;
	*pw++ = 28;
	*pw++ = 50;
	*pw++ = 14;
	*pw++ = FAULT_IDCONTINUE;
	*pw++ = FAULT_CLASSMARK;
	*pw++ = FAULT_BUTTON;
	lstrcpyW((WCHAR *)pw, sgpwszFaultContinue);
	pw += lstrlenW(sgpwszFaultContinue) + 1;
	*pw++ = 0;
	FAULT_ALIGN(pw);
	*pw++ = LOWORD(FAULT_BUTTONSTYLE | BS_DEFPUSHBUTTON);
	*pw++ = HIWORD(FAULT_BUTTONSTYLE | BS_DEFPUSHBUTTON);
	*pw++ = 0;
	*pw++ = 0;
	*pw++ = 234;
	*pw++ = 7;
	*pw++ = 50;
	*pw++ = 14;
	*pw++ = IDCANCEL;
	*pw++ = FAULT_CLASSMARK;
	*pw++ = FAULT_BUTTON;
	lstrcpyW((WCHAR *)pw, sgpwszFaultAbort);
	pw += lstrlenW(sgpwszFaultAbort) + 1;
	*pw++ = 0;
	FAULT_ALIGN(pw);
	*pw++ = LOWORD(FAULT_EDITSTYLE);
	*pw++ = HIWORD(FAULT_EDITSTYLE);
	*pw++ = LOWORD(WS_EX_CLIENTEDGE);
	*pw++ = HIWORD(WS_EX_CLIENTEDGE);
	*pw++ = 7;
	*pw++ = 7;
	*pw++ = 219;
	*pw++ = 100;
	*pw++ = FAULT_IDTEXT;
	*pw++ = FAULT_CLASSMARK;
	*pw++ = FAULT_EDIT;
	*pw++ = 0;
	*pw = 0;
}

BOOL __stdcall FaultDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		FaultInitDialog(hDlg, (const char *)lParam);
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == FAULT_IDCONTINUE)
		{
			if (sghFaultFont)
			{
				DeleteObject(sghFaultFont);
				sghFaultFont = NULL;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		if (LOWORD(wParam) == FAULT_IDSAVE)
		{
			FaultSaveReport(hDlg);
			return TRUE;
		}
		if (LOWORD(wParam) == FAULT_IDCOPY)
		{
			FaultCopyReport(hDlg);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void __fastcall FaultSaveReport(HWND hDlg)
{
	char szReport[FAULT_REPORTSIZE];
	OPENFILENAMEA tFile;
	DWORD dwWritten;
	HMODULE hComDlg;
	PFNGETSAVEFILENAME pfnGetSaveFileName;
	HANDLE hFile;

	hComDlg = LoadLibraryA(sgpszFaultComDlg);
	if (!hComDlg)
		return;
	pfnGetSaveFileName = (PFNGETSAVEFILENAME)GetProcAddress(hComDlg, sgpszFaultGetSaveFileName);
	if (!pfnGetSaveFileName)
		return;
	tFile.lStructSize = sizeof tFile;
	tFile.hwndOwner = hDlg;
	tFile.hInstance = NULL;
	tFile.lpstrFilter = sgpszFaultFilter;
	tFile.lpstrCustomFilter = NULL;
	tFile.nMaxCustFilter = 0;
	tFile.nFilterIndex = 2;
	tFile.lpstrFile = sgszFaultLogFile;
	tFile.nMaxFile = sizeof sgszFaultLogFile;
	tFile.lpstrFileTitle = NULL;
	tFile.nMaxFileTitle = 0;
	tFile.lpstrInitialDir = NULL;
	tFile.lpstrTitle = sgpszFaultSaveTitle;
	tFile.Flags = OFN_HIDEREADONLY;
	tFile.nFileOffset = 0;
	tFile.nFileExtension = 0;
	tFile.lpstrDefExt = NULL;
	tFile.lCustData = 0;
	tFile.lpfnHook = NULL;
	tFile.lpTemplateName = NULL;
	if (!pfnGetSaveFileName(&tFile))
		return;
	hFile = CreateFileA(sgszFaultLogFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,
		FILE_FLAG_WRITE_THROUGH, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;
	SetFilePointer(hFile, 0, NULL, FILE_END);
	GetDlgItemTextA(hDlg, FAULT_IDTEXT, szReport, sizeof szReport);
	WriteFile(hFile, szReport, lstrlenA(szReport), &dwWritten, NULL);
	CloseHandle(hFile);
}

BOOL __fastcall FaultCopyReport(HWND hDlg)
{
	HGLOBAL hMemory;
	char *pszReport;

	hMemory = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, FAULT_REPORTSIZE);
	pszReport = (char *)GlobalLock(hMemory);
	if (!pszReport)
		return FALSE;
	GetDlgItemTextA(hDlg, FAULT_IDTEXT, pszReport, FAULT_REPORTSIZE);
	GlobalUnlock(hMemory);
	if (!OpenClipboard(hDlg))
		return FALSE;
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMemory);
	CloseClipboard();
	return TRUE;
}

BOOL __fastcall FaultInitDialog(HWND hDlg, const char *pszReport)
{
	HWND hText;

	SetDlgItemTextA(hDlg, FAULT_IDTEXT, pszReport);
	if (!sghFaultFont)
	{
		sghFaultFont = CreateFontA(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, sgpszFaultFontFace);
		if (!sghFaultFont)
			return FALSE;
	}
	hText = GetDlgItem(hDlg, FAULT_IDTEXT);
	SendMessageA(hText, WM_SETFONT, (WPARAM)sghFaultFont, TRUE);
	return TRUE;
}

int __fastcall FaultCallStack(char *pszText, CONTEXT *ptContext)
{
	int nLength;

	if (!FaultLoadImagehlp())
		return FaultFrameWalk(pszText, ptContext);
	nLength = FaultStackWalk(pszText, ptContext);
	sgpfnSymCleanup(GetCurrentProcess());
	return nLength;
}

int __fastcall FaultFrameWalk(char *pszText, CONTEXT *ptContext)
{
	char *pszStart;
	DWORD dwPc;
	DWORD *pdwFrame;
	DWORD *pdwPrev;

	pszStart = pszText;
	pszText += wsprintfA(pszText, "\r\nCall stack:\r\nAddress   Frame     Logical Addr   Module\r\n");
	dwPc = ptContext->Eip;
	pdwFrame = (DWORD *)ptContext->Ebp;
	do
	{
		char szModule[MAX_PATH] = "";
		DWORD dwSection = 0;
		DWORD dwOffset = 0;

		FaultLogicalAddress((void *)dwPc, szModule, sizeof szModule, &dwSection, &dwOffset);
		pszText += wsprintfA(pszText, "%08X  %08X  %04X:%08X  %s\r\n", dwPc, (DWORD)pdwFrame, dwSection,
			dwOffset, szModule);
		dwPc = pdwFrame[1];
		pdwPrev = pdwFrame;
		pdwFrame = (DWORD *)pdwFrame[0];
		if ((DWORD)pdwFrame & 3)
			break;
		if (pdwFrame <= pdwPrev)
			break;
	}
	while (!IsBadWritePtr(pdwFrame, 8));
	return pszText - pszStart;
}

BOOL __cdecl FaultLoadImagehlp(void)
{
	HMODULE hImagehlp;

	hImagehlp = LoadLibraryA(sgpszFaultImagehlp);
	if (!hImagehlp)
		return FALSE;
	sgpfnSymInitialize = (PFNSYMINITIALIZE)GetProcAddress(hImagehlp, sgpszFaultSymInitialize);
	if (!sgpfnSymInitialize)
		return FALSE;
	sgpfnSymCleanup = (PFNSYMCLEANUP)GetProcAddress(hImagehlp, sgpszFaultSymCleanup);
	if (!sgpfnSymCleanup)
		return FALSE;
	sgpfnStackWalk = (PFNSTACKWALK)GetProcAddress(hImagehlp, sgpszFaultStackWalk);
	if (!sgpfnStackWalk)
		return FALSE;
	sgpfnSymFunctionTableAccess = (PFUNCTION_TABLE_ACCESS_ROUTINE)GetProcAddress(hImagehlp,
		sgpszFaultSymFunctionTableAccess);
	if (!sgpfnSymFunctionTableAccess)
		return FALSE;
	sgpfnSymGetModuleBase = (PGET_MODULE_BASE_ROUTINE)GetProcAddress(hImagehlp, sgpszFaultSymGetModuleBase);
	if (!sgpfnSymGetModuleBase)
		return FALSE;
	sgpfnSymGetSymFromAddr = (PFNSYMGETSYMFROMADDR)GetProcAddress(hImagehlp, sgpszFaultSymGetSymFromAddr);
	if (!sgpfnSymGetSymFromAddr)
		return FALSE;
	else
		return sgpfnSymInitialize(GetCurrentProcess(), NULL, TRUE) != FALSE;
}

int __fastcall FaultStackWalk(char *pszText, CONTEXT *ptContext)
{
	char *pszStart;
	STACKFRAME tFrame;
	char szModule[MAX_PATH];
	DWORD adwSymbol[(sizeof(IMAGEHLP_SYMBOL) + FAULT_SYMNAME) / sizeof(DWORD)];
	IMAGEHLP_SYMBOL *ptSymbol;
	DWORD dwDisplacement;
	DWORD dwSection;
	DWORD dwOffset;

	pszStart = pszText;
	pszText += wsprintfA(pszText, "\r\nCall stack:\r\nAddress  Frame\r\n");
	memset(&tFrame, 0, sizeof tFrame);
	tFrame.AddrPC.Offset = ptContext->Eip;
	tFrame.AddrPC.Mode = AddrModeFlat;
	tFrame.AddrStack.Offset = ptContext->Esp;
	tFrame.AddrStack.Mode = AddrModeFlat;
	tFrame.AddrFrame.Offset = ptContext->Ebp;
	tFrame.AddrFrame.Mode = AddrModeFlat;
	while (sgpfnStackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &tFrame,
		ptContext, NULL, sgpfnSymFunctionTableAccess, sgpfnSymGetModuleBase, NULL) && tFrame.AddrFrame.Offset)
	{
		pszText += wsprintfA(pszText, "%08X %08X", tFrame.AddrPC.Offset, tFrame.AddrFrame.Offset);
		ptSymbol = (IMAGEHLP_SYMBOL *)adwSymbol;
		dwDisplacement = 0;
		ptSymbol->SizeOfStruct = sizeof adwSymbol;
		ptSymbol->MaxNameLength = FAULT_SYMNAME;
		if (sgpfnSymGetSymFromAddr(GetCurrentProcess(), tFrame.AddrPC.Offset, &dwDisplacement, ptSymbol))
			pszText += wsprintfA(pszText, "  %hs+%X\r\n", ptSymbol->Name, dwDisplacement);
		else
		{
			dwSection = 0;
			dwOffset = 0;
			FaultLogicalAddress((void *)tFrame.AddrPC.Offset, szModule, sizeof szModule, &dwSection,
				&dwOffset);
			pszText += wsprintfA(pszText, "  %04X:%08X  %s\r\n", dwSection, dwOffset, szModule);
		}
	}
	return pszText - pszStart;
}

LPTOP_LEVEL_EXCEPTION_FILTER __cdecl FaultRemoveFilter(void)
{
	return SetUnhandledExceptionFilter(sgpfnFaultPrevFilter);
}
