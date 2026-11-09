#include <windows.h>

extern const char *const gpszD2LangModuleName = "D2Lang.dll";

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return TRUE;
}
