#include "D2MultiLAN.h"

struct HINSTANCE__ ghMultiLANInstance[5] = { { 0 } };

_STDINT gtD2MultiLANInt =
{
	StartChat,
};

extern const char * const gpszModuleName = "D2MultiLAN";

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	*(HINSTANCE *)ghMultiLANInstance = hInstance;
	return TRUE;
}

extern "C" _STDINT * __stdcall QueryInterface(void)
{
	return &gtD2MultiLANInt;
}
