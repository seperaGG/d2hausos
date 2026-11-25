#ifndef D2RE_STRINGPACK_H
#define D2RE_STRINGPACK_H

#include <windows.h>

typedef struct STRPACK STRPACK, *PTSTRPACK;

struct STRPACK
{
 	int			nPackedBits;
 	BYTE		abCodes[255];
 	BYTE		abChars[257];
};

#endif
