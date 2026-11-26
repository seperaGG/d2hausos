#include "D2VidTst.h"
#include "D2VidTstHell.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define IDS_DXOLDCORRUPT	3
#define IDS_DXOLDER			4
#define IDS_WARNING			5
#define IDS_WARNINGCAPTION	6
#define IDS_NOMODES			7
#define IDS_RECOMMEND		11
#define IDS_COMPLETED		12
#define IDS_CANNOTRUN		13
#define IDS_DXWRONG			14
#define IDS_DXWRONGCAPTION	15
#define IDS_SELECTMODE		17
#define IDS_LOWCOLORCAPTION	18
#define IDS_LOWCOLOR		19
#define IDS_RUNNINGCAPTION	20
#define IDS_RUNNING			21
#define IDS_EXAMINING		22
#define IDS_TESTDDRAW		23
#define IDS_TESTD3D			24
#define IDS_TESTGLIDE		25
#define IDS_TWOLINES		29
#define IDS_ALREADYRUNNING	30
#define IDS_ALREADYCAPTION	31
#define IDS_CRASHCAPTION	32
#define IDS_CRASH			33
#define IDS_WIN9XHELP		34
#define IDS_NT4HELP			35
#define IDS_REBOOTHELP		36
#define IDS_MODES			37
#define IDS_NOMODESFOUND	38
#define IDS_SKIPHINT		39

#define IDD_WARNING			101
#define IDD_RESULT			102
#define IDD_RUNNING			104
#define IDD_CRASH			105

#define IDSKIP				4
#define IDC_WARNINGTEXT		1000
#define IDC_DDRAW0			1006
#define IDC_D3D0			1007
#define IDC_GLIDE			1008
#define IDC_DDRAW1			1009
#define IDC_D3D1			1010
#define IDC_INFO			1011
#define IDC_SUCCESS			1012
#define IDC_RUNNINGTEXT		1016
#define IDC_RETRYDDRAW		1017
#define IDC_RETRYD3D		1018
#define IDC_RETRYGLIDE		1019
#define IDC_CRASHTEXT		1020
#define IDC_MODES			1021

#define WM_ENDRUNNING		1225

extern const char *const gpszD2VidTstExe = "D2VIDTST.EXE";

const char *gpszVideoConfig =
	"SOFTWARE\\Blizzard Entertainment\\Diablo II\\VideoConfig";

char gszEnumClass[] = "Enumeration Class";

VIDEOSLOT gatVideoSlots[VIDTST_SLOTS] = { 0 };
static DWORD sgadwUnread415170[2] = { 0 };
static DWORD sgdwTestGlide = 0;
static DWORD sgdw41517c = 0;
static BYTE sgb415180 = 0;
DWORD gfGlideFound = 0;
static DWORD sgdwTestDDraw = 0;
static DWORD sgdw41518c = 0;
HINSTANCE ghInstance = 0;
static BYTE sgb415194 = 0;
static char sgszRebootHelp[VIDTST_TEXTMAX] = { 0 };
static int sgnDirectXState = 0;
DWORD gfD3D0Found = 0;
DWORD gfD3D1Found = 0;
static BYTE sgb4161a4 = 0;
static char sgszNt4Help[VIDTST_TEXTMAX] = { 0 };
static char sgszWin9xHelp[VIDTST_TEXTMAX] = { 0 };
HWND ghRunningDlg = 0;
static BYTE sgb4181ac = 0;
static DWORD sgdwTestD3D = 0;
DWORD gfDDraw0Found = 0;
DWORD gfDDraw1Found = 0;

static inline LONG VidTstRegKeyClose(HKEY *phKey)
{
	LONG lResult;

	lResult = ERROR_SUCCESS;
	if (*phKey)
	{
		lResult = RegCloseKey(*phKey);
		*phKey = NULL;
	}
	return lResult;
}

static inline LONG VidTstRegKeyCreate(HKEY *phKey, HKEY hParent, const char *pszName, REGSAM samDesired)
{
	HKEY hKey;
	DWORD dwDisposition;
	LONG lResult;

	hKey = NULL;
	lResult = RegCreateKeyExA(hParent, pszName, 0, NULL, REG_OPTION_NON_VOLATILE, samDesired, NULL, &hKey,
		&dwDisposition);
	if (lResult == ERROR_SUCCESS)
	{
		lResult = VidTstRegKeyClose(phKey);
		*phKey = hKey;
	}
	return lResult;
}

static inline LONG VidTstRegKeyQueryDword(HKEY hKey, DWORD *pdwValue, const char *pszValue)
{
	DWORD dwType;
	DWORD dwCount;

	dwType = 0;
	dwCount = sizeof(DWORD);
	return RegQueryValueExA(hKey, pszValue, NULL, &dwType, (BYTE *)pdwValue, &dwCount);
}

static inline LONG VidTstRegKeyQueryString(HKEY hKey, char *pszValue, const char *pszName, DWORD *pdwCount)
{
	DWORD dwType;

	dwType = 0;
	return RegQueryValueExA(hKey, pszName, NULL, &dwType, (BYTE *)pszValue, pdwCount);
}

static inline LONG VidTstRegKeySetDword(HKEY hKey, DWORD dwValue, const char *pszValue)
{
	return RegSetValueExA(hKey, pszValue, 0, REG_DWORD, (const BYTE *)&dwValue, sizeof(DWORD));
}

static inline LONG VidTstRegKeySetString(HKEY hKey, const char *pszText, const char *pszValue)
{
	return RegSetValueExA(hKey, pszValue, 0, REG_SZ, (const BYTE *)pszText, lstrlenA(pszText) + 1);
}

static inline BOOL VidTstIsWin9xHelp(const VIDTSTSYSINFO *ptInfo)
{
	if (ptInfo->dwOsType == 1 || ptInfo->dwOsType == 2 || ptInfo->dwOsType == 4)
		return TRUE;
	return ptInfo->dwOsType == 3 && ptInfo->fVersion >= 5.0f;
}

BOOL __fastcall VidTstOsDetect(HINSTANCE hInstance)
{
	char szCaption[VIDTST_CAPTIONMAX];
	char szVersion[VIDTST_NAMEMAX];
	char szText[VIDTST_TEXTMAX];
	char szMsg[VIDTST_TEXTMAX];
	VIDTSTSYSINFO *ptInfo;
	HKEY hKey;
	DWORD dwCount;
	int nMajor;
	int nMinor;
	int nBuild;
	int nRevision;
	BOOL fResult;

	ptInfo = (VIDTSTSYSINFO *)FogErrorManagerGetSysInfo();
	hKey = NULL;
	fResult = FALSE;
	FogErrorManagerLog("Detecting Operating System and DirectX...");
	sgnDirectXState = DXSTATE_UNKNOWN;
	FogErrorManagerLog("\tosDetect()");
	if (ptInfo->dwOsType == 3 && ptInfo->fVersion >= 4.0f && !(ptInfo->fVersion >= 5.0f))
	{
		FogErrorManagerLog("\tAutomatically passed NT 4 version %f.", (double)ptInfo->fVersion);
		fResult = TRUE;
		sgnDirectXState = DXSTATE_OK;
	}
	else if (ptInfo->dwOsType == 1 || ptInfo->dwOsType == 2 || ptInfo->dwOsType == 4
		|| (ptInfo->dwOsType == 3 && ptInfo->fVersion >= 5.0f))
	{
		if (VidTstRegKeyCreate(&hKey, HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\DirectX", KEY_QUERY_VALUE)
			== ERROR_SUCCESS)
		{
			nMajor = 0;
			nMinor = 0;
			nBuild = 0;
			nRevision = 0;
			dwCount = sizeof szVersion;
			if (VidTstRegKeyQueryString(hKey, szVersion, "Version", &dwCount) == ERROR_SUCCESS)
			{
				sscanf(szVersion, "%d.%d.%d.%d", &nMajor, &nMinor, &nBuild, &nRevision);
				if (nMajor > 4 || (nMajor == 4 && nMinor > 6) || (nMajor == 4 && nMinor == 6 && nBuild >= 2))
				{
					FogErrorManagerLog("\tValid version of DirectX found. (%d.%d.%d.%d)", nMajor, nMinor,
						nBuild, nRevision);
					fResult = TRUE;
					sgnDirectXState = DXSTATE_OK;
				}
				else
				{
					FogErrorManagerLog("\tWrong version of DirectX found! (%d.%d.%d.%d)", nMajor, nMinor,
						nBuild, nRevision);
					LoadStringA(hInstance, IDS_DXWRONG, szText, sizeof szText);
					LoadStringA(hInstance, IDS_DXWRONGCAPTION, szCaption, sizeof szCaption);
					sprintf(szMsg, szText, sgszWin9xHelp, sgszRebootHelp);
					if (MessageBoxA(NULL, szMsg, szCaption, MB_YESNO) == IDYES)
						fResult = TRUE;
					sgnDirectXState = DXSTATE_OLD;
				}
			}
			else
				FogErrorManagerLog("*** DirectX registry version not found!");
			VidTstRegKeyClose(&hKey);
		}
		else
			FogErrorManagerLog("*** DirectX registry key not found!");
	}
	else
	{
		FogErrorManagerLog("*** Unknown or unhandled OS type! (type= %d, version= %f)", ptInfo->dwOsType,
			(double)ptInfo->fVersion);
	}
	if (fResult)
		FogErrorManagerLog("...Done Detecting Operating System and DirectX: Success");
	else
		FogErrorManagerLog("...Done Detecting Operating System and DirectX: Failed");
	VidTstRegKeyClose(&hKey);
	return fResult;
}

void __fastcall VidTstWriteRender(DWORD dwRender, int nSlot)
{
	char szName[VIDTST_NAMEMAX];
	char szFlags[VIDTST_NAMEMAX];
	char szGuid[VIDTST_NAMEMAX];
	HKEY hKey;

	sprintf(szName, "DeviceName");
	sprintf(szFlags, "dwFlags");
	sprintf(szGuid, "GUID");
	hKey = NULL;
	if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_WRITE) == ERROR_SUCCESS)
	{
		VidTstRegKeySetDword(hKey, dwRender, "Render");
		VidTstRegKeySetString(hKey, gatVideoSlots[nSlot].szName, szName);
		VidTstRegKeySetDword(hKey, gatVideoSlots[nSlot].dwFlags, szFlags);
		RegSetValueExA(hKey, szGuid, 0, REG_BINARY, gatVideoSlots[nSlot].abGuid, sizeof gatVideoSlots[nSlot].abGuid);
		VidTstRegKeyClose(&hKey);
	}
	if (VidTstRegKeyCreate(&hKey, HKEY_LOCAL_MACHINE, gpszVideoConfig, KEY_WRITE) == ERROR_SUCCESS)
	{
		VidTstRegKeySetDword(hKey, dwRender, "Render");
		VidTstRegKeySetString(hKey, gatVideoSlots[nSlot].szName, szName);
		VidTstRegKeySetDword(hKey, gatVideoSlots[nSlot].dwFlags, szFlags);
		RegSetValueExA(hKey, szGuid, 0, REG_BINARY, gatVideoSlots[nSlot].abGuid, sizeof gatVideoSlots[nSlot].abGuid);
		VidTstRegKeyClose(&hKey);
	}
	VidTstRegKeyClose(&hKey);
}

void __fastcall VidTstWriteSlot(int nSlot)
{
	char szName[VIDTST_NAMEMAX];
	char szRecommend[VIDTST_NAMEMAX];
	char szFlags[VIDTST_NAMEMAX];
	char szGuid[VIDTST_NAMEMAX];
	HKEY hKey;

	sprintf(szName, "DeviceName%d", nSlot);
	sprintf(szFlags, "dwFlags%d", nSlot);
	sprintf(szGuid, "GUID%d", nSlot);
	sprintf(szRecommend, "Recommend%d", nSlot);
	hKey = NULL;
	if (VidTstRegKeyCreate(&hKey, HKEY_LOCAL_MACHINE, gpszVideoConfig, KEY_WRITE) == ERROR_SUCCESS)
	{
		VidTstRegKeySetString(hKey, gatVideoSlots[nSlot].szName, szName);
		VidTstRegKeySetDword(hKey, gatVideoSlots[nSlot].dwFlags, szFlags);
		VidTstRegKeySetDword(hKey, gatVideoSlots[nSlot].dwRecommend, szRecommend);
		RegSetValueExA(hKey, szGuid, 0, REG_BINARY, gatVideoSlots[nSlot].abGuid, sizeof gatVideoSlots[nSlot].abGuid);
		VidTstRegKeyClose(&hKey);
	}
	if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_WRITE) == ERROR_SUCCESS)
	{
		VidTstRegKeySetString(hKey, gatVideoSlots[nSlot].szName, szName);
		VidTstRegKeySetDword(hKey, gatVideoSlots[nSlot].dwFlags, szFlags);
		VidTstRegKeySetDword(hKey, gatVideoSlots[nSlot].dwRecommend, szRecommend);
		RegSetValueExA(hKey, szGuid, 0, REG_BINARY, gatVideoSlots[nSlot].abGuid, sizeof gatVideoSlots[nSlot].abGuid);
		VidTstRegKeyClose(&hKey);
	}
	VidTstRegKeyClose(&hKey);
}

BOOL __cdecl VidTstReadConfig(void)
{
	char szName[VIDTST_NAMEMAX];
	char szRecommend[VIDTST_NAMEMAX];
	char szFlags[VIDTST_NAMEMAX];
	char szGuid[VIDTST_NAMEMAX];
	HKEY hKey;
	BOOL fFound;
	int i;

	fFound = FALSE;
	for (i = 0; i < VIDTST_SLOTS; i++)
	{
		sprintf(szName, "DeviceName%d", i);
		sprintf(szFlags, "dwFlags%d", i);
		sprintf(szGuid, "GUID%d", i);
		sprintf(szRecommend, "Recommend%d", i);
		hKey = NULL;
		if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_READ) == ERROR_SUCCESS)
		{
			DWORD dwCount;
			DWORD dwType;

			dwCount = MAX_PATH;
			VidTstRegKeyQueryString(hKey, gatVideoSlots[i].szName, szName, &dwCount);
			dwCount = MAX_PATH;
			VidTstRegKeyQueryDword(hKey, &gatVideoSlots[i].dwFlags, szFlags);
			dwCount = MAX_PATH;
			VidTstRegKeyQueryDword(hKey, &gatVideoSlots[i].dwRecommend, szRecommend);
			if (gatVideoSlots[i].dwRecommend == RECOMMEND_FOUND)
				fFound = TRUE;
			dwType = 0;
			dwCount = MAX_PATH;
			RegQueryValueExA(hKey, szGuid, NULL, &dwType, gatVideoSlots[i].abGuid, &dwCount);
			VidTstRegKeyClose(&hKey);
		}
		if (!fFound && VidTstRegKeyCreate(&hKey, HKEY_LOCAL_MACHINE, gpszVideoConfig,
			KEY_READ) == ERROR_SUCCESS)
		{
			DWORD dwCount;
			DWORD dwType;

			dwCount = MAX_PATH;
			VidTstRegKeyQueryString(hKey, gatVideoSlots[i].szName, szName, &dwCount);
			dwCount = MAX_PATH;
			VidTstRegKeyQueryDword(hKey, &gatVideoSlots[i].dwFlags, szFlags);
			dwCount = MAX_PATH;
			VidTstRegKeyQueryDword(hKey, &gatVideoSlots[i].dwRecommend, szRecommend);
			if (gatVideoSlots[i].dwRecommend == RECOMMEND_FOUND)
				fFound = TRUE;
			dwType = 0;
			dwCount = MAX_PATH;
			RegQueryValueExA(hKey, szGuid, NULL, &dwType, gatVideoSlots[i].abGuid, &dwCount);
			VidTstRegKeyClose(&hKey);
		}
		VidTstRegKeyClose(&hKey);
	}
	return fFound;
}

BOOL CALLBACK VidTstRunningDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szCaption[VIDTST_CAPTIONMAX];
	char szText[VIDTST_TEXTMAX];
	HWND hText;
	BOOL bRet = FALSE;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hText = GetDlgItem(hDlg, IDC_RUNNINGTEXT);
		LoadStringA(ghInstance, IDS_RUNNING, szText, sizeof szText);
		SetWindowTextA(hText, szText);
		ShowWindow(hText, SW_SHOWNORMAL);
		LoadStringA(ghInstance, IDS_RUNNINGCAPTION, szCaption, sizeof szCaption);
		SetWindowTextA(hDlg, szCaption);
		ShowWindow(hDlg, SW_SHOWNORMAL);
		bRet = TRUE;
		break;
	case WM_ENDRUNNING:
		EndDialog(hDlg, LOWORD(wParam));
		ghRunningDlg = NULL;
		break;
	default:
		bRet = FALSE;
		break;
	}
	return bRet;
}

DWORD WINAPI VidTstRunningThread(LPVOID pParam)
{
	MSG tMsg;

	ghRunningDlg = CreateDialogParamA(ghInstance, MAKEINTRESOURCEA(IDD_RUNNING), NULL, VidTstRunningDlgProc,
		0);
	ShowWindow(ghRunningDlg, SW_SHOWNORMAL);
	while (ghRunningDlg)
	{
		if (!GetMessageA(&tMsg, NULL, 0, 0) || !ghRunningDlg)
			break;
		if (!IsDialogMessageA(ghRunningDlg, &tMsg))
		{
			TranslateMessage(&tMsg);
			DispatchMessageA(&tMsg);
		}
	}
	return 0;
}

void __fastcall VidTstAppendRunningText(UINT uId)
{
	RUNNINGTEXTFRAME tFrame;
	HWND hText;

	if (!ghRunningDlg)
	{
		FogErrorManagerLog("No running dialog, can't display updatetext.");
		return;
	}
	hText = GetDlgItem(ghRunningDlg, IDC_RUNNINGTEXT);
	LoadStringA(ghInstance, uId, tFrame.szAdd, sizeof tFrame.szAdd);
	GetWindowTextA(hText, tFrame.szText, sizeof tFrame.szText);
	sprintf(tFrame.szText, "%s%s", tFrame.szText, tFrame.szAdd);
	SetWindowTextA(hText, tFrame.szText);
}

BOOL CALLBACK VidTstWarningDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WARNINGFRAME tFrame;
	HWND hText;
	HWND hSkip;
	BOOL bRet = FALSE;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		hText = GetDlgItem(hDlg, IDC_WARNINGTEXT);
		LoadStringA(ghInstance, IDS_WARNING, tFrame.szText, sizeof tFrame.szText);
		LoadStringA(ghInstance, IDS_SKIPHINT, tFrame.szSkip, sizeof tFrame.szSkip);
		if (VidTstReadConfig())
			strcat(tFrame.szText, tFrame.szSkip);
		else
		{
			hSkip = GetDlgItem(hDlg, IDSKIP);
			ShowWindow(hSkip, SW_HIDE);
		}
		SetWindowTextA(hText, tFrame.szText);
		LoadStringA(ghInstance, IDS_WARNINGCAPTION, tFrame.szCaption, sizeof tFrame.szCaption);
		SetWindowTextA(hDlg, tFrame.szCaption);
		bRet = TRUE;
		ShowWindow(hDlg, SW_SHOWNORMAL);
		ShowWindow(hDlg, SW_SHOW);
		ShowWindow(hDlg, SW_SHOWNORMAL);
		ShowWindow(hDlg, SW_SHOW);
		SetForegroundWindow(hDlg);
		SetActiveWindow(hDlg);
		SetFocus(hDlg);
		BringWindowToTop(hDlg);
		ShowWindow(hDlg, SW_SHOWNORMAL);
		break;
	case WM_COMMAND:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return bRet;
}

BOOL CALLBACK VidTstResultDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RESULTFRAME tFrame;
	HWND hItem;
	VIDTSTSYSINFO *ptInfo;
	int i;
	BOOL bRet = FALSE;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		LoadStringA(ghInstance, IDS_MODES, tFrame.szText, sizeof tFrame.szText);
		SetWindowTextA(GetDlgItem(hDlg, IDC_MODES), tFrame.szText);
		LoadStringA(ghInstance, IDS_COMPLETED, tFrame.szCaption, sizeof tFrame.szCaption);
		SetWindowTextA(hDlg, tFrame.szCaption);
		LoadStringA(ghInstance, IDS_RECOMMEND, tFrame.szFormat, sizeof tFrame.szFormat);

		hItem = GetDlgItem(hDlg, IDC_DDRAW0);
		sprintf(tFrame.szText, "DirectDraw (2D): %s", gatVideoSlots[0].szName);
		SetWindowTextA(hItem, tFrame.szText);
		if (!gatVideoSlots[0].dwRecommend)
			ShowWindow(hItem, SW_HIDE);
		if (gatVideoSlots[0].dwRecommend == RECOMMEND_FOUND)
		{
			sprintf(tFrame.szSuccess, tFrame.szFormat, tFrame.szText);
			PostMessageA(hItem, BM_CLICK, BST_CHECKED, 0);
			FogErrorManagerLog("Recommending ddraw0 in final dialog.");
			VidTstWriteRender(RENDER_DDRAW, 0);
		}
		sprintf(tFrame.szSuccess, tFrame.szFormat, tFrame.szText);

		hItem = GetDlgItem(hDlg, IDC_DDRAW1);
		sprintf(tFrame.szText, "DirectDraw (2D): %s", gatVideoSlots[1].szName);
		SetWindowTextA(hItem, tFrame.szText);
		if (!gatVideoSlots[1].dwRecommend)
			ShowWindow(hItem, SW_HIDE);
		if (gatVideoSlots[1].dwRecommend == RECOMMEND_FOUND)
		{
			sprintf(tFrame.szSuccess, tFrame.szFormat, tFrame.szText);
			PostMessageA(hItem, BM_CLICK, BST_CHECKED, 0);
			FogErrorManagerLog("Recommending ddraw1 in final dialog.");
			VidTstWriteRender(RENDER_DDRAW, 1);
		}

		hItem = GetDlgItem(hDlg, IDC_D3D0);
		sprintf(tFrame.szText, "Direct3D: %s", gatVideoSlots[2].szName);
		SetWindowTextA(hItem, tFrame.szText);
		if (!gatVideoSlots[2].dwRecommend)
			ShowWindow(hItem, SW_HIDE);
		if (gatVideoSlots[2].dwRecommend == RECOMMEND_FOUND)
		{
			sprintf(tFrame.szSuccess, tFrame.szFormat, tFrame.szText);
			PostMessageA(hItem, BM_CLICK, BST_CHECKED, 0);
			FogErrorManagerLog("Recommending d3d0 in final dialog.");
			VidTstWriteRender(RENDER_D3D, 2);
		}

		hItem = GetDlgItem(hDlg, IDC_D3D1);
		sprintf(tFrame.szText, "Direct3D: %s", gatVideoSlots[3].szName);
		SetWindowTextA(hItem, tFrame.szText);
		if (!gatVideoSlots[3].dwRecommend)
			ShowWindow(hItem, SW_HIDE);
		if (gatVideoSlots[3].dwRecommend == RECOMMEND_FOUND)
		{
			sprintf(tFrame.szSuccess, tFrame.szFormat, tFrame.szText);
			PostMessageA(hItem, BM_CLICK, BST_CHECKED, 0);
			FogErrorManagerLog("Recommending d3d1 in final dialog.");
			VidTstWriteRender(RENDER_D3D, 3);
		}

		hItem = GetDlgItem(hDlg, IDC_GLIDE);
		sprintf(tFrame.szText, "3dfx Glide: %s", gatVideoSlots[4].szName);
		SetWindowTextA(hItem, tFrame.szText);
		if (!gatVideoSlots[4].dwRecommend)
			ShowWindow(hItem, SW_HIDE);
		if (gatVideoSlots[4].dwRecommend == RECOMMEND_FOUND)
		{
			sprintf(tFrame.szSuccess, tFrame.szFormat, tFrame.szText);
			PostMessageA(hItem, BM_CLICK, BST_CHECKED, 0);
			FogErrorManagerLog("Recommending glide in final dialog.");
			VidTstWriteRender(RENDER_GLIDE, 4);
		}

		hItem = GetDlgItem(hDlg, IDC_SUCCESS);
		SetWindowTextA(hItem, tFrame.szSuccess);
		if (!gatVideoSlots[0].dwRecommend && !gatVideoSlots[1].dwRecommend && !gatVideoSlots[2].dwRecommend
			&& !gatVideoSlots[3].dwRecommend && !gatVideoSlots[4].dwRecommend)
		{
			FogErrorManagerLog("Displaying complete failure message in final dialog.");
			hItem = GetDlgItem(hDlg, IDC_SUCCESS);
			LoadStringA(ghInstance, IDS_CANNOTRUN, tFrame.szText, sizeof tFrame.szText);
			if (sgnDirectXState == DXSTATE_OLD)
			{
				LoadStringA(ghInstance, IDS_DXOLDER, tFrame.szReason, sizeof tFrame.szReason);
				sprintf(tFrame.szMsg, tFrame.szText, tFrame.szReason);
			}
			else if (sgnDirectXState == DXSTATE_UNKNOWN)
			{
				LoadStringA(ghInstance, IDS_DXOLDCORRUPT, tFrame.szReason, sizeof tFrame.szReason);
				sprintf(tFrame.szMsg, tFrame.szText, tFrame.szReason);
			}
			else
			{
				LoadStringA(ghInstance, IDS_NOMODES, tFrame.szReason, sizeof tFrame.szReason);
				sprintf(tFrame.szMsg, tFrame.szText, tFrame.szReason);
			}
			SetWindowTextA(hItem, tFrame.szMsg);
			hItem = GetDlgItem(hDlg, IDC_INFO);
			LoadStringA(ghInstance, IDS_TWOLINES, tFrame.szText, sizeof tFrame.szText);
			ptInfo = (VIDTSTSYSINFO *)FogErrorManagerGetSysInfo();
			if (!VidTstIsWin9xHelp(ptInfo))
				sprintf(tFrame.szMsg, tFrame.szText, sgszNt4Help, sgszRebootHelp);
			else
				sprintf(tFrame.szMsg, tFrame.szText, sgszWin9xHelp, sgszRebootHelp);
			SetWindowTextA(hItem, tFrame.szMsg);
			LoadStringA(ghInstance, IDS_NOMODESFOUND, tFrame.szText, sizeof tFrame.szText);
			SetWindowTextA(GetDlgItem(hDlg, IDC_MODES), tFrame.szText);
		}
		else
		{
			FogErrorManagerLog("Displaying info message in final dialog.");
			hItem = GetDlgItem(hDlg, IDC_INFO);
			LoadStringA(ghInstance, IDS_SELECTMODE, tFrame.szText, sizeof tFrame.szText);
			ptInfo = (VIDTSTSYSINFO *)FogErrorManagerGetSysInfo();
			if (!VidTstIsWin9xHelp(ptInfo))
				sprintf(tFrame.szMsg, tFrame.szText, sgszNt4Help, sgszRebootHelp);
			else
				sprintf(tFrame.szMsg, tFrame.szText, sgszWin9xHelp, sgszRebootHelp);
			SetWindowTextA(hItem, tFrame.szMsg);
		}
		FogErrorManagerLog("Show window in final dialog.");
		ShowWindow(hDlg, SW_SHOWNORMAL);
		ShowWindow(hDlg, SW_SHOW);
		SetForegroundWindow(hDlg);
		SetActiveWindow(hDlg);
		SetFocus(hDlg);
		BringWindowToTop(hDlg);
		ShowWindow(hDlg, SW_SHOWNORMAL);
		bRet = TRUE;
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) != IDOK)
			break;
		for (i = 0; i < VIDTST_SLOTS; i++)
		{
			if (gatVideoSlots[i].dwRecommend != RECOMMEND_FOUND)
				continue;
			switch (i)
			{
			case 0:
			case 1:
				VidTstWriteRender(RENDER_DDRAW, i);
				break;
			case 2:
			case 3:
				VidTstWriteRender(RENDER_D3D, i);
				break;
			case 4:
				VidTstWriteRender(RENDER_GLIDE, 4);
				break;
			}
		}
		hItem = GetDlgItem(hDlg, IDC_DDRAW0);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED)
			VidTstWriteRender(RENDER_DDRAW, 0);
		hItem = GetDlgItem(hDlg, IDC_DDRAW1);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED)
			VidTstWriteRender(RENDER_DDRAW, 1);
		hItem = GetDlgItem(hDlg, IDC_D3D0);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED)
			VidTstWriteRender(RENDER_D3D, 2);
		hItem = GetDlgItem(hDlg, IDC_D3D1);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED)
			VidTstWriteRender(RENDER_D3D, 3);
		hItem = GetDlgItem(hDlg, IDC_GLIDE);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED)
			VidTstWriteRender(RENDER_GLIDE, 4);
		bRet = TRUE;
		FogErrorManagerLog("Ending final dialog for IDOK.");
		EndDialog(hDlg, IDOK);
		break;
	}
	return bRet;
}

BOOL CALLBACK VidTstCrashDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CRASHFRAME tFrame;
	HWND hItem;
	HKEY hKey;
	BOOL bRet = FALSE;

	switch (uMsg)
	{
	case WM_INITDIALOG:
		LoadStringA(ghInstance, IDS_CRASHCAPTION, tFrame.szCaption, sizeof tFrame.szCaption);
		SetWindowTextA(hDlg, tFrame.szCaption);
		hItem = GetDlgItem(hDlg, IDC_RETRYDDRAW);
		if (sgdwTestDDraw != TEST_RUNNING)
			ShowWindow(hItem, SW_HIDE);
		else
			FogErrorManagerLog("Showing ddr as an optional test.");
		hItem = GetDlgItem(hDlg, IDC_RETRYD3D);
		if (sgdwTestD3D != TEST_RUNNING)
			ShowWindow(hItem, SW_HIDE);
		else
			FogErrorManagerLog("Showing d3d as an optional test.");
		hItem = GetDlgItem(hDlg, IDC_RETRYGLIDE);
		if (sgdwTestGlide != TEST_RUNNING)
			ShowWindow(hItem, SW_HIDE);
		else
			FogErrorManagerLog("Showing gld as an optional test.");
		FogErrorManagerLog("Displaying info message in crash dialog.");
		hItem = GetDlgItem(hDlg, IDC_CRASHTEXT);
		LoadStringA(ghInstance, IDS_CRASH, tFrame.szText, sizeof tFrame.szText);
		if (!VidTstIsWin9xHelp((VIDTSTSYSINFO *)FogErrorManagerGetSysInfo()))
			sprintf(tFrame.szMsg, tFrame.szText, sgszNt4Help, sgszRebootHelp);
		else
			sprintf(tFrame.szMsg, tFrame.szText, sgszWin9xHelp, sgszRebootHelp);
		SetWindowTextA(hItem, tFrame.szMsg);
		FogErrorManagerLog("Show window in crash dialog.");
		ShowWindow(hDlg, SW_SHOWNORMAL);
		ShowWindow(hDlg, SW_SHOW);
		SetForegroundWindow(hDlg);
		SetActiveWindow(hDlg);
		SetFocus(hDlg);
		BringWindowToTop(hDlg);
		ShowWindow(hDlg, SW_SHOWNORMAL);
		bRet = TRUE;
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) != IDOK)
			break;
		hKey = NULL;
		hItem = GetDlgItem(hDlg, IDC_RETRYDDRAW);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED
			&& VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			VidTstRegKeySetDword(hKey, TEST_DONE, "TestDDraw");
			VidTstRegKeyClose(&hKey);
		}
		hItem = GetDlgItem(hDlg, IDC_RETRYD3D);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED
			&& VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			VidTstRegKeySetDword(hKey, TEST_DONE, "TestD3D");
			VidTstRegKeyClose(&hKey);
		}
		hItem = GetDlgItem(hDlg, IDC_RETRYGLIDE);
		if (SendMessageA(hItem, BM_GETSTATE, 0, 0) == BST_CHECKED
			&& VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			VidTstRegKeySetDword(hKey, TEST_DONE, "TestGLIDE");
			VidTstRegKeyClose(&hKey);
		}
		bRet = TRUE;
		FogErrorManagerLog("Ending final dialog for IDOK.");
		EndDialog(hDlg, IDOK);
		VidTstRegKeyClose(&hKey);
		break;
	}
	return bRet;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szText[VIDTST_TEXTMAX];
	char szCaption[VIDTST_CAPTIONMAX];
	DEVMODEA tMode;
	WNDCLASSA tClass;
	DWORD dwThreadId;
	HCURSOR hCursor;
	HKEY hKey;
	INT_PTR nResult;
	int nWait;

	if (!CreateEventA(NULL, TRUE, TRUE, "D2VideoTest") || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		FogErrorManagerLog("Already running Video Test");
		LoadStringA(hInstance, IDS_ALREADYRUNNING, szText, sizeof szText);
		LoadStringA(hInstance, IDS_ALREADYCAPTION, szCaption, sizeof szCaption);
		return MessageBoxA(NULL, szText, szCaption, MB_SYSTEMMODAL);
	}
	FogErrorManagerInit("D2VideoTest", 0);
	sgb4161a4 = 0;
	sgb415180 = 0;
	sgb415194 = 0;
	sgb4181ac = 0;
	ghInstance = hInstance;
	sgdw41517c = (DWORD)-1;
	gfGlideFound = 0;
	sgdw41518c = 0;
	gfD3D1Found = 0;
	gfD3D0Found = 0;
	gfDDraw1Found = 0;
	gfDDraw0Found = 0;
	gatVideoSlots[4].dwRecommend = 0;
	gatVideoSlots[3].dwRecommend = 0;
	gatVideoSlots[2].dwRecommend = 0;
	gatVideoSlots[1].dwRecommend = 0;
	gatVideoSlots[0].dwRecommend = 0;
	LoadStringA(hInstance, IDS_WIN9XHELP, sgszWin9xHelp, sizeof sgszWin9xHelp);
	LoadStringA(hInstance, IDS_REBOOTHELP, sgszRebootHelp, sizeof sgszRebootHelp);
	LoadStringA(hInstance, IDS_NT4HELP, sgszNt4Help, sizeof sgszNt4Help);
	hKey = NULL;
	if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
	{
		FogErrorManagerLog("Able to access user registry to delete flag");
		if (RegDeleteValueA(hKey, "VidTestRequired") == ERROR_SUCCESS)
			FogErrorManagerLog("Deleted vidtestrequiredflag");
		VidTstRegKeyClose(&hKey);
	}
	FogErrorManagerLog("... continuing Video Test");
	nResult = DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_WARNING), NULL, VidTstWarningDlgProc, 0);
	if (nResult == IDCANCEL)
	{
		VidTstRegKeyClose(&hKey);
		return -1;
	}
	if (nResult == IDSKIP)
	{
		FogErrorManagerLog("User selected configuration only.");
		VidTstReadConfig();
		nResult = DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_RESULT), NULL, VidTstResultDlgProc, 0);
		if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			FogErrorManagerLog("Able to access user registry to delete flag");
			if (RegDeleteValueA(hKey, "VidTestRequired") == ERROR_SUCCESS)
				FogErrorManagerLog("Deleted vidtestrequiredflag");
			VidTstRegKeyClose(&hKey);
		}
		VidTstRegKeyClose(&hKey);
		return (int)nResult;
	}
	sgdwTestDDraw = 0;
	sgdwTestD3D = 0;
	sgdwTestGlide = 0;
	if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
	{
		if (VidTstRegKeyQueryDword(hKey, &sgdwTestDDraw, "TestDDraw") != ERROR_SUCCESS)
			sgdwTestDDraw = 0;
		if (VidTstRegKeyQueryDword(hKey, &sgdwTestD3D, "TestD3D") != ERROR_SUCCESS)
			sgdwTestD3D = 0;
		if (VidTstRegKeyQueryDword(hKey, &sgdwTestGlide, "TestGLIDE") != ERROR_SUCCESS)
			sgdwTestGlide = 0;
		if (sgdwTestDDraw == TEST_RUNNING || sgdwTestD3D == TEST_RUNNING || sgdwTestGlide == TEST_RUNNING)
			DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_CRASH), NULL, VidTstCrashDlgProc, 0);
		VidTstRegKeyClose(&hKey);
	}
	FogErrorManagerLog("Enumerating display device");
	if (!EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &tMode) || tMode.dmBitsPerPel < 8)
	{
		LoadStringA(hInstance, IDS_LOWCOLOR, szText, sizeof szText);
		LoadStringA(hInstance, IDS_LOWCOLORCAPTION, szCaption, sizeof szCaption);
		if (MessageBoxA(NULL, szText, szCaption, MB_OKCANCEL) == IDCANCEL)
		{
			VidTstRegKeyClose(&hKey);
			return -1;
		}
	}
	FogErrorManagerLog("Creating running dialog");
	CreateThread(NULL, 0, VidTstRunningThread, NULL, 0, &dwThreadId);
	for (nWait = 0; !ghRunningDlg; )
	{
		if (++nWait > 10)
		{
			FogErrorManagerLog("Failed to display running dialog, trying to continue anyway.");
			break;
		}
		SleepEx(1000, TRUE);
	}
	if (ghRunningDlg)
		FogErrorManagerLog("Running dialog displayed, continuing ...");
	hCursor = SetCursor(LoadCursorA(NULL, IDC_WAIT));
	VidTstAppendRunningText(IDS_EXAMINING);
	SleepEx(1000, TRUE);
	FogErrorManagerLog("Ready to detect operating system");
	if (VidTstOsDetect(hInstance))
	{
		FogErrorManagerLog("OSDetect passed: Registering window class...");
		tClass.style = 0;
		tClass.lpfnWndProc = DefWindowProcA;
		tClass.cbClsExtra = 0;
		tClass.cbWndExtra = 0;
		tClass.hInstance = hInstance;
		tClass.hIcon = NULL;
		tClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
		tClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		tClass.lpszMenuName = NULL;
		tClass.lpszClassName = gszEnumClass;
		if (!RegisterClassA(&tClass))
		{
			if (FaultErrorBox(GetLastError()))
			{
				gfInDebugBreak = TRUE;
				__asm int 3
				gfInDebugBreak = FALSE;
			}
			VidTstRegKeyClose(&hKey);
			return -1;
		}
		FogErrorManagerLog("... Done Registering window class");

		FogErrorManagerLog("DDrawDetect...");
		if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			if (VidTstRegKeyQueryDword(hKey, &sgdwTestDDraw, "TestDDraw") != ERROR_SUCCESS)
				sgdwTestDDraw = 0;
			if (sgdwTestDDraw != TEST_RUNNING)
			{
				VidTstRegKeySetDword(hKey, TEST_RUNNING, "TestDDraw");
				VidTstAppendRunningText(IDS_TESTDDRAW);
				SleepEx(1000, TRUE);
				DDrawDetect(hInstance);
				VidTstRegKeySetDword(hKey, TEST_DONE, "TestDDraw");
			}
			VidTstRegKeyClose(&hKey);
		}
		SleepEx(2000, TRUE);
		if (gfDDraw0Found)
			VidTstWriteSlot(0);
		if (gfDDraw1Found)
			VidTstWriteSlot(1);

		FogErrorManagerLog("Direct3DDetect...");
		if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			if (VidTstRegKeyQueryDword(hKey, &sgdwTestD3D, "TestD3D") != ERROR_SUCCESS)
				sgdwTestD3D = 0;
			if (sgdwTestD3D != TEST_RUNNING)
			{
				VidTstRegKeySetDword(hKey, TEST_RUNNING, "TestD3D");
				VidTstAppendRunningText(IDS_TESTD3D);
				SleepEx(1000, TRUE);
				Direct3DDetect(hInstance);
				VidTstRegKeySetDword(hKey, TEST_DONE, "TestD3D");
			}
			VidTstRegKeyClose(&hKey);
		}
		SleepEx(2000, TRUE);
		if (gfD3D0Found)
			VidTstWriteSlot(2);
		if (gfD3D1Found)
			VidTstWriteSlot(3);

		FogErrorManagerLog("GlideDetect...");
		if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
		{
			if (VidTstRegKeyQueryDword(hKey, &sgdwTestGlide, "TestGLIDE") != ERROR_SUCCESS)
				sgdwTestGlide = 0;
			if (sgdwTestGlide != TEST_RUNNING)
			{
				VidTstRegKeySetDword(hKey, TEST_RUNNING, "TestGLIDE");
				VidTstAppendRunningText(IDS_TESTGLIDE);
				SleepEx(1000, TRUE);
				GlideDetect(hInstance);
				VidTstRegKeySetDword(hKey, TEST_DONE, "TestGLIDE");
			}
			VidTstRegKeyClose(&hKey);
		}
		SleepEx(2000, TRUE);
		if (gfGlideFound)
			VidTstWriteSlot(4);
	}
	VidTstAppendRunningText(IDS_COMPLETED);
	SleepEx(1000, TRUE);
	FogErrorManagerLog("Done with Detection phase...");
	if (ghRunningDlg)
	{
		PostMessageA(ghRunningDlg, WM_ENDRUNNING, 0, 0);
		DestroyWindow(ghRunningDlg);
	}
	SetCursor(hCursor);
	FogErrorManagerLog("Creating final dialog ...");
	DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_RESULT), NULL, VidTstResultDlgProc, 0);
	FogErrorManagerLog("Done with Interactive phase...");
	UnregisterClassA(gszEnumClass, hInstance);
	if (VidTstRegKeyCreate(&hKey, HKEY_CURRENT_USER, gpszVideoConfig, KEY_ALL_ACCESS) == ERROR_SUCCESS)
	{
		FogErrorManagerLog("Able to access user registry to delete flag");
		if (RegDeleteValueA(hKey, "VidTestRequired") == ERROR_SUCCESS)
			FogErrorManagerLog("Deleted vidtestrequiredflag");
		VidTstRegKeyClose(&hKey);
	}
	FogErrorManagerLog("Video Test application completed without error.");
	VidTstRegKeyClose(&hKey);
	return 0;
}

#include "StormImports.h"
void VidTstDeadStormMemory(void)
{
	void *pMemory = StormSMemAlloc(16, "D2VidTst", 0, 0);
	StormSMemFree(pMemory, "D2VidTst", 0, 0);
}
