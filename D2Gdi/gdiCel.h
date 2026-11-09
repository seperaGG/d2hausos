#ifndef MATCH_D2GDI_GDICEL_H
#define MATCH_D2GDI_GDICEL_H

static BOOL __fastcall GdiCelIsVisible(D2CEL *ptCel, int nX, int nY, BOOL fShadow)
{
	int nWidth;
	int nHeight;

	nWidth = D2CMPCelGetWidth(ptCel);
	nHeight = D2CMPCelGetHeight(ptCel);
	if (fShadow)
	{
		nX -= nHeight;
		nWidth += nHeight;
		nHeight >>= 1;
	}
	nX += D2CMPCelGetOffsetX(ptCel);
	if (nX >= gnBufferWidth)
		return FALSE;
	nWidth += nX;
	if (nWidth < 0)
		return FALSE;
	nY += D2CMPCelGetOffsetY(ptCel);
	if (nY < 0)
		return FALSE;
	if (nY - nHeight >= gnBufferHeight)
		return FALSE;
	return TRUE;
}

#endif
