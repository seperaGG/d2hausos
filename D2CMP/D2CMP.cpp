#include <windows.h>

extern const char *const gpszD2CMPModuleName = "D2CMP.DLL";

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}
