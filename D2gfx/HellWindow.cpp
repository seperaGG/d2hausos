#define D2_THIS_FILE "C:\\D2\\Source\\D2Hell\\SRC\\Window.cpp"
#include <windows.h>
#include "D2gfxLinkNames.h"
#include <stdlib.h>
#include "D2Macros.h"

BOOL __fastcall WindowCenter(HWND hwndSlave, HWND hwndMaster);

extern const char *const gpszHellNoName = "";

BOOL __fastcall WindowCenter(HWND hwndSlave, HWND hwndMaster)
{
	RECT tSlave;
	RECT tMaster;
	int nScreenWidth;
	int nScreenHeight;
	int nWidth;
	int nHeight;
	int nMasterWidth;
	int nMasterHeight;
	int nX;
	int nY;

#line 24
	D2ASSERT(hwndSlave);
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	GetWindowRect(hwndSlave, &tSlave);
	nWidth = tSlave.right - tSlave.left;
	nHeight = tSlave.bottom - tSlave.top;
	if (hwndMaster)
	{
		GetWindowRect(hwndMaster, &tMaster);
		nMasterWidth = tMaster.right - tMaster.left;
		nMasterHeight = tMaster.bottom - tMaster.top;
	}
	else
	{
		SetRect(&tMaster, 0, 0, nScreenWidth, nScreenHeight);
		nMasterWidth = nScreenWidth;
		nMasterHeight = nScreenHeight;
	}
	nX = (nMasterWidth - nWidth) / 2 + tMaster.left;
	if (nX < 0)
		nX = 0;
	else if (nX + nWidth > nScreenWidth)
		nX = nScreenWidth - nWidth;
	nY = (nMasterHeight - nHeight) / 2 + tMaster.top;
	if (nY < 0)
		nY = 0;
	else if (nY + nHeight > nScreenHeight)
		nY = nScreenHeight - nHeight;
	return SetWindowPos(hwndSlave, NULL, nX, nY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
