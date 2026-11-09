#define D2_THIS_FILE "C:\\D2\\Source\\D2Gdi\\Src\\Memory.cpp"
#include <windows.h>
#include "StormImports.h"

void __cdecl operator delete(void *pMemory)
{
	if (pMemory)
		StormSMemFree(pMemory, "delete", -1, 0);
}

void * __cdecl operator new(unsigned int nSize)
{
	return StormSMemAlloc(nSize, "new", -1, 0);
}
