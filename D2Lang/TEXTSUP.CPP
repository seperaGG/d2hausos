#include "D2Lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Unicode::isLeftToRight(void) const
{
	if (wUnit < 1424 || wUnit > 1535)
	{
		if (wUnit < 1536 || wUnit > 1791)
			return 1;
	}
	return 0;
}

int __fastcall Unicode::isLineBreak(const Unicode *pszStr, unsigned int nIndex)
{

	if (nIndex)
	{
		if (!isspace(pszStr[nIndex]) && isspace(pszStr[nIndex - 1]))
			return 1;
		return 0;
	}
	return 1;
}

Unicode * __fastcall Unicode::win2Unicode(Unicode *pszDst, const char *pszSrc, int nCount)
{
	int i = 0;
	unsigned char bChar;

	if (nCount > 0)
	{
		while (1)
		{
			bChar = pszSrc[i];
			if (!bChar)
				break;
			pszDst[i].wUnit = bChar;
			i++;
			if (i >= nCount)
				break;
		}
	}

	if (pszSrc[i])
		pszDst[i - 1].wUnit = 0;
	else
		pszDst[i].wUnit = 0;
	return pszDst;
}

char * __fastcall Unicode::unicode2Win(char *pszDst, const Unicode *pszSrc, int nCount)
{
	char *pszCur = pszDst;

	while (nCount > 1 && pszSrc->wUnit)
	{
		*pszCur++ = (char)*pszSrc++;
		nCount--;
	}
	*pszCur = 0;
	return pszDst;
}

Unicode::Direction Unicode::directionality(void)
{
	if (wUnit >= 1424 && wUnit <= 2303)
		return DIRECTION_F1;
	if (wUnit >= 97 && wUnit <= 122)
		return DIRECTION_F0;
	if (wUnit >= 64 && wUnit <= 90)
		return DIRECTION_F0;
	if (wUnit >= 48 && wUnit <= 57)
		return DIRECTION_F0;
	if (wUnit <= 63)
		return wUnit == 38 ? DIRECTION_F0 : DIRECTION_F2;
	if (wUnit >= 12352 && wUnit <= 35839)
		return DIRECTION_F0;
	if (wUnit >= 192 && wUnit <= 1423 && wUnit != 215 && wUnit != 247)
		return DIRECTION_F0;
	if (wUnit >= 2304 && wUnit <= 4351)
		return DIRECTION_F0;
	if ((wUnit & 65024) == 8192)
	{
		if (wUnit >= 8544 && wUnit <= 8578)
			return DIRECTION_F0;
		if (wUnit >= 8400 && wUnit <= 8447)
			return DIRECTION_F0;
		if (wUnit == 8206)
			return DIRECTION_F0;
		if (wUnit == 8207)
			return DIRECTION_F1;
	}
	return DIRECTION_F2;
}

static unsigned short sgwCollateWeight[256] =
{
	0, 1, 2, 3, 4, 5, 6, 7,
	8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29, 30, 31,
	32, 33, 34, 35, 36, 37, 38, 39,
	40, 41, 42, 43, 44, 45, 46, 47,
	48, 49, 50, 51, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61, 62, 63,
	64, 256, 257, 258, 259, 260, 261, 262,
	263, 264, 265, 266, 267, 268, 269, 271,
	272, 273, 274, 275, 276, 277, 278, 279,
	280, 281, 282, 91, 92, 93, 94, 95,
	96, 256, 257, 258, 259, 260, 261, 262,
	263, 264, 265, 266, 267, 268, 269, 271,
	272, 273, 274, 275, 276, 277, 278, 279,
	280, 281, 282, 123, 124, 125, 126, 127,
	128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 275, 139, 288, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151,
	152, 153, 275, 155, 288, 157, 158, 281,
	160, 161, 162, 163, 164, 165, 166, 167,
	168, 169, 170, 171, 172, 173, 174, 175,
	176, 177, 178, 179, 180, 181, 182, 183,
	184, 185, 186, 187, 188, 189, 190, 191,
	256, 256, 256, 256, 286, 285, 283, 258,
	260, 260, 260, 260, 264, 264, 264, 264,
	289, 270, 271, 271, 271, 271, 287, 215,
	284, 277, 277, 277, 277, 281, 290, 275,
	256, 256, 256, 256, 286, 285, 283, 258,
	260, 260, 260, 260, 264, 264, 264, 264,
	289, 270, 271, 271, 271, 271, 287, 247,
	284, 277, 277, 277, 277, 281, 290, 281
};

int __fastcall Unicode::strncoll(const Unicode *pszStr1, const Unicode *pszStr2, int nCount)
{
	const Unicode *pszCur1 = pszStr1;
	const Unicode *pszCur2 = pszStr2;
	int nDiff;
	unsigned short wUnit1;

	if (pszCur1->wUnit)
	{
		while (1)
		{
			if (!pszCur2->wUnit || pszCur1 - pszStr1 >= nCount)
				break;

			wUnit1 = pszCur1->wUnit;
			if (wUnit1 < 256 && pszCur2->wUnit < 256)
				nDiff = sgwCollateWeight[wUnit1] - sgwCollateWeight[pszCur2->wUnit];
			else
				nDiff = wUnit1 - pszCur2->wUnit;
			if (nDiff)
				return nDiff;
			pszCur1++;
			pszCur2++;
			if (!pszCur1->wUnit)
				break;
		}
	}
	if (pszCur1 - pszStr1 < nCount && pszCur1->wUnit != pszCur2->wUnit)
		return pszCur1->wUnit ? 1 : -1;

	return strnicmp(pszStr1, pszStr2, nCount);
}
