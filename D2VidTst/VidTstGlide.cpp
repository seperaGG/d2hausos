#include "D2VidTst.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static char sgszGlideWindowName[] = "Glide Enumeration";

static int sgnGlideBoards = 0;
static DWORD sgdwGlideContext = 0;
static BOOL sgfGlideInit = 0;
static BOOL sgfGlideWindowOpen = 0;
static HWND sghGlideWindow = 0;
static void (__stdcall *sgpfnGrGlideShutdown)(void) = 0;
static BOOL (__stdcall *sgpfnGrSstWinClose)(DWORD dwContext) = 0;
static DWORD (__stdcall *sgpfnGrTexMaxAddress)(DWORD dwTmu) = 0;
static DWORD (__stdcall *sgpfnGrTexMinAddress)(DWORD dwTmu) = 0;
static DWORD (__stdcall *sgpfnGrSstWinOpen)(HWND hWnd, DWORD dwResolution, DWORD dwRefresh,
	DWORD dwColorFormat, DWORD dwOrigin, int nColorBuffers, int nAuxBuffers) = 0;
static const char *(__stdcall *sgpfnGrGetString)(DWORD dwName) = 0;
static void (__stdcall *sgpfnGrSstSelect)(int nBoard) = 0;
static void (__stdcall *sgpfnGrGlideInit)(void) = 0;
static DWORD (__stdcall *sgpfnGrGet)(DWORD dwName, DWORD cb, void *pValue) = 0;

int __fastcall GlideDetect(HINSTANCE hInstance)
{
	HMODULE hGlide;
	DWORD dwTotal;
	DWORD dwMin;
	float fTotal;
	int nTmus;
	int i;
	int nRecommend;
	BOOL fFound;

	try
	{
		FogErrorManagerLog("Detecting boards which support Glide...");
		hGlide = LoadLibraryA("glide3x.dll");
		if (!hGlide)
		{
			FogErrorManagerLog("Missing glide3x.dll.  Assuming no 3dfx boards present!");
			return 0;
		}
		sgpfnGrGlideInit = (void (__stdcall *)(void))GetProcAddress(hGlide, "_grGlideInit@0");
		fFound = sgpfnGrGlideInit != NULL;
		sgpfnGrGlideShutdown = (void (__stdcall *)(void))GetProcAddress(hGlide, "_grGlideShutdown@0");
		fFound = fFound && sgpfnGrGlideShutdown;
		sgpfnGrSstWinClose = (BOOL (__stdcall *)(DWORD))GetProcAddress(hGlide, "_grSstWinClose@4");
		fFound = fFound && sgpfnGrSstWinClose;
		sgpfnGrTexMaxAddress = (DWORD (__stdcall *)(DWORD))GetProcAddress(hGlide, "_grTexMaxAddress@4");
		fFound = fFound && sgpfnGrTexMaxAddress;
		sgpfnGrTexMinAddress = (DWORD (__stdcall *)(DWORD))GetProcAddress(hGlide, "_grTexMinAddress@4");
		fFound = fFound && sgpfnGrTexMinAddress;
		sgpfnGrSstWinOpen = (DWORD (__stdcall *)(HWND, DWORD, DWORD, DWORD, DWORD, int, int))
			GetProcAddress(hGlide, "_grSstWinOpen@28");
		fFound = fFound && sgpfnGrSstWinOpen;
		sgpfnGrGetString = (const char *(__stdcall *)(DWORD))GetProcAddress(hGlide, "_grGetString@4");
		fFound = fFound && sgpfnGrGetString;
		sgpfnGrGet = (DWORD (__stdcall *)(DWORD, DWORD, void *))GetProcAddress(hGlide, "_grGet@12");
		fFound = fFound && sgpfnGrGet;
		sgpfnGrSstSelect = (void (__stdcall *)(int))GetProcAddress(hGlide, "_grSstSelect@4");
		fFound = fFound && sgpfnGrSstSelect;
		if (!fFound)
		{
			FogErrorManagerLog("Missing or incomplete glide3x.dll.  Assuming no 3dfx boards present!");
			return 0;
		}
		sgpfnGrGet(15, 4, &sgnGlideBoards);
		if (sgnGlideBoards <= 0)
		{
			return 0;
		}
		dwTotal = 0;
		sgpfnGrGlideInit();
		sgfGlideInit = TRUE;
		sgpfnGrSstSelect(0);
		FogErrorManagerLog("\tHardware: %s", sgpfnGrGetString(161));
		FogErrorManagerLog("\tVendor: %s", sgpfnGrGetString(163));
		FogErrorManagerLog("\tRenderer: %s", sgpfnGrGetString(162));
		FogErrorManagerLog("\tVersion: %s", sgpfnGrGetString(164));
		SetLastError(0);
		for (;;)
		{
			sghGlideWindow = CreateWindowExA(WS_EX_APPWINDOW, gszEnumClass, sgszGlideWindowName, WS_POPUP,
				0, 0, 640, 480, NULL, NULL, hInstance, NULL);
			if (!sghGlideWindow)
				break;
			sgdwGlideContext = sgpfnGrSstWinOpen(sghGlideWindow, 7, 0, 2, 0, 2, 0);
			if (sgdwGlideContext)
			{
				sgfGlideWindowOpen = TRUE;
				sgpfnGrGet(19, 4, &nTmus);
				FogErrorManagerLog("\tTMU Count= %d", nTmus);
				if (nTmus < 1)
					break;
				for (i = 0; i < nTmus; i++)
				{
					dwMin = sgpfnGrTexMinAddress(i);
					dwTotal += sgpfnGrTexMaxAddress(i) - dwMin;
				}
				fTotal = (float)dwTotal;
				FogErrorManagerLog("\tTotal Texture Memory= %3.2f", fTotal / VIDTST_MB);
				if (fTotal < 3670016.0f)
				{
					FogErrorManagerLog("\t\tDoesn't have enough video memory (3.5 MB) to adequately run "
						"Diablo II!\n");
					break;
				}
				if (sgpfnGrSstWinClose(sgdwGlideContext) != TRUE)
					FogErrorManagerLog("grSstWinClose failed, context= %d", sgdwGlideContext);
				sgfGlideWindowOpen = FALSE;
			}
			sgpfnGrGlideShutdown();
			sgfGlideInit = FALSE;
			FogErrorManagerLog("\tPassed requirements for Diablo II.");
			if (sghGlideWindow)
			{
				DestroyWindow(sghGlideWindow);
				FogErrorManagerLog("Destroyed Glide window.");
			}
			strcpy(gatVideoSlots[4].szName, "3dfx Glide");
			nRecommend = (float)dwTotal > 5242880.0f ? 2 : 1;
			gatVideoSlots[4].dwRecommend = nRecommend;
			gfGlideFound = nRecommend;
			return nRecommend;
		}
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Caught a failure in GlideDetect ** ");
		gatVideoSlots[4].dwRecommend = 0;
	}

	try
	{
		if (sgfGlideWindowOpen)
		{
			if (sgpfnGrSstWinClose(sgdwGlideContext) != TRUE)
				FogErrorManagerLog("grSstWinClose failed, context= %d", sgdwGlideContext);
			sgfGlideWindowOpen = FALSE;
		}
		if (sgfGlideInit)
		{
			sgpfnGrGlideShutdown();
			sgfGlideInit = FALSE;
		}
		if (sghGlideWindow)
		{
			DestroyWindow(sghGlideWindow);
			FogErrorManagerLog("Destroyed bad Glide window.");
		}
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Couldn't clean up GlideDetect ** ");
	}
	return 0;
}
