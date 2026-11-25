#include "Bnclient.h"

#define GRID_KEYS			3
#define GRID_BLOCK			64

struct GRIDSHA
{
 	int			anState[5];
 	int			nBitsLow;
 	int			nBitsHigh;
 	BYTE		abBlock[GRID_BLOCK];
};
typedef GRIDSHA GRIDSHAS[GRID_KEYS];

#define GRIDROL(x, n)		(((x) << (n)) | ((x) >> (32 - (n))))

static GRIDSHAS sgatSha;

static void __fastcall GridShaBlock(GRIDSHA *pt);

void __fastcall GridShaClear(void)
{
	int i;

	for (i = 0; i < (int)(sizeof(GRIDSHAS) / (sizeof(DWORD))); i++)
		((DWORD *)sgatSha)[i] = 0;
}

void __fastcall GridShaGet(int nIndex, BYTE *pbDigest)
{
	int i;

	if (pbDigest)
	{
		int *pnOut = (int *)pbDigest;

		for (i = 0; i < 5; i++)
			*pnOut++ = sgatSha[nIndex].anState[i];
	}
}

void __fastcall GridShaUpdate(int nIndex, const BYTE *pbData, BYTE *pbDigest)
{
	GRIDSHA *pt;
	int nBits;
	int i;

	pt = &sgatSha[nIndex];
	nBits = pt->nBitsLow + 512;
	if (nBits < pt->nBitsLow)
		pt->nBitsHigh++;
	pt->nBitsLow = nBits;
	memcpy(pt->abBlock, pbData, GRID_BLOCK);
	GridShaBlock(pt);
	if (pbDigest)
	{
		int *pnSrc = pt->anState;

		for (i = 0; i < 5; i++)
		{
			*(int *)pbDigest = *pnSrc++;
			pbDigest += sizeof(int);
		}
	}
}

static void __fastcall GridShaBlock(GRIDSHA *pt)
{
	int anW[80];
	int a;
	int b;
	int c;
	int d;
	int e;
	int t;
	int i;

	memcpy(anW, pt->abBlock, GRID_BLOCK);
	for (i = 16; i < 80; i++)
		anW[i] = anW[i - 3] ^ anW[i - 8] ^ anW[i - 14] ^ anW[i - 16];
	a = pt->anState[0];
	b = pt->anState[1];
	c = pt->anState[2];
	d = pt->anState[3];
	e = pt->anState[4];
	for (i = 0; i < 20; i++)
	{
		t = GRIDROL(a, 5) + ((~b & d) | (c & b)) + anW[i] + e + 1518500249;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	for (i = 20; 40 > i; i++)
	{
		t = GRIDROL(a, 5) + (d ^ c ^ b) + anW[i] + e + 1859775393;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	for (i = 40; i < 60; i++)
	{
		t = (((c | b) & d) | (c & b)) + GRIDROL(a, 5) + anW[i] + e + (int)0x8F1BBCDC;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	for (i = 60; i < 80; i++)
	{
		t = GRIDROL(a, 5) + (d ^ c ^ b) + e + anW[i] + (int)0xCA62C1D6;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	pt->anState[0] += a;
	pt->anState[1] += b;
	pt->anState[2] += c;
	pt->anState[3] += d;
	pt->anState[4] += e;
}

void __fastcall GridShaReset(int nIndex)
{
	GRIDSHA *pt;

	pt = &sgatSha[nIndex];
	pt->anState[0] = 1732584193;
	pt->anState[1] = (int)0xEFCDAB89;
	pt->anState[2] = (int)0x98BADCFE;
	pt->anState[3] = 271733878;
	pt->anState[4] = (int)0xC3D2E1F0;
	pt->nBitsLow = 0;
	pt->nBitsHigh = 0;
}
