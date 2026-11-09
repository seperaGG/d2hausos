#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\DllMain.cpp"
#include <windows.h>

extern const char *const gpszModuleName = "D2GDI.DLL";

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}
