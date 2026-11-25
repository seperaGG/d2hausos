#ifndef D2RE_BITMANIP_H
#define D2RE_BITMANIP_H

#include <windows.h>

typedef struct BITBUFFER BITBUFFER, *PTBITBUFFER;

struct BITBUFFER
{
 	BYTE		*pbData;
 	int			nLength;
 	int			nCurByte;
 	int			nCurBit;
};

#endif
