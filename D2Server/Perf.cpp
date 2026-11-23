#include "D2Server.h"

#define IDD_PERFORMANCE		103
#define IDC_PERFGRAPH		1010
#define IDC_PERFTEXT1		1009
#define IDC_PERFTEXT2		1008

typedef char PERFPATH[256];

static HWND sghPerfDialog = NULL;
static HDC sghPerfDC = NULL;
static HBITMAP sghPerfBitmap = NULL;
static BOOL sgfPerfLogging = 0;
static int sgnPerfX = 0;

static PERFPATH sgszPerfPath = "C:\\D2\\Performace.txt";

HWND __fastcall PerfGetDialog(void)
{
	return sghPerfDialog;
}

BOOL __fastcall PerfCreateDialog(HWND hWndParent)
{
	HINSTANCE hInstance;
	HWND hGraph;
	RECT tRect;
	HDC hDC;
	HPEN hPen;
	HGDIOBJ hOldPen;
	HBRUSH hBrush;
	HGDIOBJ hOldBrush;

	hInstance = (HINSTANCE)GetWindowLongA(hWndParent, GWL_HINSTANCE);
	if (sghPerfDialog)
		return FALSE;
	sghPerfDialog = CreateDialogParamA(hInstance, MAKEINTRESOURCEA(IDD_PERFORMANCE), hWndParent, PerfDlgProc,
		0);
	if (!sghPerfDialog)
		return FALSE;
	hGraph = GetDlgItem(sghPerfDialog, IDC_PERFGRAPH);
	GetClientRect(hGraph, &tRect);
	hDC = GetDC(sghPerfDialog);
	sghPerfDC = CreateCompatibleDC(hDC);
	sghPerfBitmap = CreateCompatibleBitmap(hDC, tRect.right - tRect.left, tRect.bottom - tRect.top);
	ReleaseDC(sghPerfDialog, hDC);
	SelectObject(sghPerfDC, sghPerfBitmap);
	hPen = CreatePen(PS_SOLID, 1, 0);
	hOldPen = SelectObject(sghPerfDC, hPen);
	hBrush = CreateSolidBrush(0);
	hOldBrush = SelectObject(sghPerfDC, hBrush);
	Rectangle(sghPerfDC, tRect.left, tRect.top, tRect.right, tRect.bottom);
	SelectObject(sghPerfDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(sghPerfDC, hOldBrush);
	DeleteObject(hBrush);
	return TRUE;
}

LRESULT __fastcall PerfCloseDialog(void)
{
	return SendMessageA(sghPerfDialog, WM_CLOSE, 0, 0);
}

BOOL CALLBACK PerfDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT tPaint;
	RECT tRect;
	HWND hItem;
	HDC hDC;

	switch (uMsg)
	{
	case WM_CLOSE:
		sghPerfDialog = NULL;
		DeleteObject(sghPerfBitmap);
		sghPerfBitmap = NULL;
		DeleteObject(sghPerfDC);
		sghPerfDC = NULL;
		EndDialog(hDlg, 0);
		return TRUE;

	case WM_PAINT:
		BeginPaint(hDlg, &tPaint);
		hItem = GetDlgItem(hDlg, IDC_PERFGRAPH);
		hDC = GetDC(hItem);
		GetClientRect(hItem, &tRect);
		BitBlt(hDC, tRect.left, tRect.top, tRect.right - tRect.left, tRect.bottom - tRect.top, sghPerfDC,
			0, 0, SRCCOPY);
		ReleaseDC(hItem, hDC);
		EndPaint(hDlg, &tPaint);
		return TRUE;

	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDCANCEL:
			sghPerfDialog = NULL;
			EndDialog(hDlg, 0);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

void __fastcall PerfSetFirstLine(const char *pszText)
{
	if (sghPerfDialog)
		SendDlgItemMessageA(sghPerfDialog, IDC_PERFTEXT1, WM_SETTEXT, 0, (LPARAM)pszText);
}

void __fastcall PerfSetSecondLine(const char *pszText)
{
	if (sghPerfDialog)
		SendDlgItemMessageA(sghPerfDialog, IDC_PERFTEXT2, WM_SETTEXT, 0, (LPARAM)pszText);
}

void __fastcall PerfDraw(int nBusy, int nGames)
{
	HPEN hPenBlack;
	HPEN hPenGreen;
	HPEN hPenRed;
	HBRUSH hBrush;
	HWND hItem;
	RECT tRect;
	POINT tPoint;
	HGDIOBJ hOldBrush;
	HGDIOBJ hOldPen;
	HDC hDC;
	int nHeight;
	int nX;
	int nY;

	if (!sghPerfDialog)
		return;
	hPenBlack = CreatePen(PS_SOLID, 1, 0);
	hPenGreen = CreatePen(PS_SOLID, 1, 65280);
	hPenRed = CreatePen(PS_SOLID, 1, 255);
	hBrush = CreateSolidBrush(0);
	hItem = GetDlgItem(sghPerfDialog, IDC_PERFGRAPH);
	GetClientRect(hItem, &tRect);
	nHeight = tRect.bottom - tRect.top;
	nX = sgnPerfX + tRect.left;
	hOldBrush = SelectObject(sghPerfDC, hBrush);
	hOldPen = SelectObject(sghPerfDC, hPenBlack);
	Rectangle(sghPerfDC, nX, tRect.top, nX + 2, tRect.bottom);
	nY = nHeight * nBusy / 100;
	SelectObject(sghPerfDC, hPenGreen);
	MoveToEx(sghPerfDC, nX, tRect.bottom - nY, &tPoint);
	LineTo(sghPerfDC, nX + 2, tRect.bottom - nY);
	nY = nHeight * nGames / 600;
	SelectObject(sghPerfDC, hPenRed);
	MoveToEx(sghPerfDC, nX, tRect.bottom - nY, &tPoint);
	LineTo(sghPerfDC, nX + 2, tRect.bottom - nY);
	sgnPerfX += 2;
	if (sgnPerfX >= tRect.right - tRect.left - 2)
		sgnPerfX = 0;
	SelectObject(sghPerfDC, hOldPen);
	SelectObject(sghPerfDC, hOldBrush);
	DeleteObject(hPenBlack);
	DeleteObject(hPenGreen);
	DeleteObject(hPenRed);
	DeleteObject(hBrush);
	hDC = GetDC(hItem);
	GetClientRect(hItem, &tRect);
	BitBlt(hDC, tRect.left, tRect.top, tRect.right - tRect.left, tRect.bottom - tRect.top, sghPerfDC, 0, 0,
		SRCCOPY);
	ReleaseDC(hItem, hDC);
}

BOOL __fastcall PerfGetLogging(void)
{
	return sgfPerfLogging;
}

void __fastcall PerfSetLogging(BOOL fLogging)
{
	HANDLE hFile;
	DWORD dwWritten;
	char chEnd;

	sgfPerfLogging = fLogging;
	if (!fLogging)
		return;
	chEnd = '\r';
	hFile = CreateFileA(sgszPerfPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile)
	{
		MessageBoxA((HWND)hFile, "Error creating performance logfile!", "Error", MB_ICONHAND);
		sgfPerfLogging = (BOOL)hFile;
		return;
	}
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, "Busy\tGames\tPlayers\tActiveMonsters\tInActiveMonsters",
		strlen("Busy\tGames\tPlayers\tActiveMonsters\tInActiveMonsters"), &dwWritten, NULL);
	WriteFile(hFile, &chEnd, 1, &dwWritten, NULL);
	CloseHandle(hFile);
}

BOOL __fastcall PerfGetLogPath(char *pszPath)
{
	strcpy(pszPath, sgszPerfPath);
	return sgfPerfLogging;
}

void __fastcall PerfSetLogPath(const char *pszPath)
{
	strcpy(sgszPerfPath, pszPath);
}

void __fastcall PerfLogValues(int nBusy, const DWORD *pdwValues)
{
	HANDLE hFile;
	DWORD dwWritten;
	char chEnd;
	char szLine[256];

	sprintf(szLine, "%d\t%d\t%d\t%d\t%d", nBusy, pdwValues[0], pdwValues[3], pdwValues[1], pdwValues[2]);
	chEnd = '\r';
	if (!sgfPerfLogging)
		return;
	hFile = CreateFileA(sgszPerfPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile)
	{
		MessageBoxA((HWND)hFile, "Error creating performance logfile!", "Error", MB_ICONHAND);
		sgfPerfLogging = (BOOL)hFile;
		return;
	}
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, szLine, strlen(szLine), &dwWritten, NULL);
	WriteFile(hFile, &chEnd, 1, &dwWritten, NULL);
	CloseHandle(hFile);
}

void __fastcall PerfLogText(const char *pszText)
{
	HANDLE hFile;
	DWORD dwWritten;
	char chEnd;

	chEnd = '\r';
	if (!sgfPerfLogging)
		return;
	hFile = CreateFileA(sgszPerfPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (!hFile)
	{
		MessageBoxA((HWND)hFile, "Error creating performance logfile!", "Error", MB_ICONHAND);
		sgfPerfLogging = (BOOL)hFile;
		return;
	}
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, pszText, strlen(pszText), &dwWritten, NULL);
	WriteFile(hFile, &chEnd, 1, &dwWritten, NULL);
	CloseHandle(hFile);
}
