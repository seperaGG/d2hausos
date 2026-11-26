#include <windows.h>

char gszD2WinDll[] = "D2Win.DLL";
extern char *const gpszD2WinDll = gszD2WinDll;

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}
