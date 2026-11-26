#include <windows.h>
#include "Archive.h"

int __cdecl ArchiveBitFindFirstSet(DWORD dwValue)
{
	int nBit;

	if (!dwValue)
		return 0;
	__asm
	{
		bsf eax, dwValue
		inc eax
		mov nBit, eax
	}
	return nBit;
}
