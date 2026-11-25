#include "Bnclient.h"

#define GRID_KEYS			3
#define IDEA_KEYLEN			52
#define GRID_BLOCK			64

typedef WORD IDEAKEY[IDEA_KEYLEN];
typedef IDEAKEY GRIDKEYS[GRID_KEYS];

struct GRIDHASH
{
 	DWORD		adwState[5];
 	BYTE		abBlock[GRID_BLOCK];
};

#define GRIDROL(x, n)		(((x) << (n)) | ((x) >> (32 - (n))))
#define GRIDROLN(x, n)		(((x) << (n)) | ((x) >> ((32 - (n)) & 31)))

static GRIDKEYS sgaawKeys;

DWORD __fastcall IdeaMul(DWORD a, DWORD b)
{
	DWORD p;

	if (a) {
		if (b) {
			p = a * b;
			a = p & 65535;
			b = p >> 16;
			return a - b + (a < b); }
		return 1 - a; }
	return 1 - b;
}

DWORD __fastcall IdeaMulInv(DWORD x)
{
	DWORD t0;
	DWORD t1;
	DWORD q;
	DWORD y;

	if (x <= 1)
	{
		return x;
	}
	else
	{
		t1 = 65537 / x;
		y = 65537 % x;
		if (y == 1)
		{
			return (1 - t1) & 65535;
		}
		else
		{
			t0 = 1;
			do
			{
				q = x / y;
				x = x % y;
				t0 += q * t1;
				if (x == 1)
				{
					return t0;
				}
				else
				{
					q = y / x;
					y = y % x;
					t1 += q * t0;
				}
			} while (y != 1);
			return (1 - t1) & 65535;
		}
	}
}

void __fastcall IdeaExpandKey(const WORD *pwUserKey, WORD *pwKey)
{
	int i;
	int j;

	WORD *pw = pwKey;

	for (j = 0; j < 8; j++)
		*pw++ = *pwUserKey++;
	for (i = 0, j = 8; j < IDEA_KEYLEN; j++)
	{
		i++;
		pwKey[i + 7] = (WORD)(pwKey[i & 7] << 9 | pwKey[(i + 1) & 7] >> 7);
		pwKey += i & 8;
		i &= 7;
	}
}

void __fastcall IdeaInvertKey(const WORD *pwIn, WORD *pwOut)
{
	WORD awTemp[IDEA_KEYLEN];
	WORD *pw;
	DWORD t1;
	DWORD t2;
	DWORD t3;
	int i;

	pw = awTemp + IDEA_KEYLEN;
	t1 = IdeaMulInv(*pwIn++);
	t2 = -*pwIn++;
	t3 = -*pwIn++;
	*--pw = (WORD)IdeaMulInv(*pwIn++);
	*--pw = (WORD)t3;
	*--pw = (WORD)t2;
	*--pw = (WORD)t1;
	for (i = 0; i < 7; i++)
	{
		t1 = *pwIn++;
		*--pw = *pwIn++;
		*--pw = (WORD)t1;
		t1 = IdeaMulInv(*pwIn++);
		t2 = -*pwIn++;
		t3 = -*pwIn++;
		*--pw = (WORD)IdeaMulInv(*pwIn++);
		*--pw = (WORD)t2;
		*--pw = (WORD)t3;
		*--pw = (WORD)t1;
	}
	t1 = *pwIn++;
	*--pw = *pwIn++;
	*--pw = (WORD)t1;
	t1 = IdeaMulInv(*pwIn++);
	t2 = -*pwIn++;
	t3 = -*pwIn++;
	*--pw = (WORD)IdeaMulInv(*pwIn++);
	*--pw = (WORD)t3;
	*--pw = (WORD)t2;
	*--pw = (WORD)t1;
	for (i = 0, pw = awTemp; i < IDEA_KEYLEN; i++)
	{
		*pwOut++ = *pw;
		*pw++ = 0;
	}
}

void __fastcall IdeaCipher(const WORD *pwIn, WORD *pwOut, const WORD *pwKey)
{
	DWORD x1;
	DWORD x2;
	DWORD x3;
	DWORD x4;
	DWORD t1;
	DWORD t2;
	int nRounds;

	x1 = *pwIn++;
	x2 = *pwIn++;
	x3 = *pwIn++;
	x4 = *pwIn;
	nRounds = 8;
	do
	{
		x1 = IdeaMul(x1 & 65535, *pwKey++);
		x2 += *pwKey++;
		x3 += *pwKey++;
		x4 = IdeaMul(x4 & 65535, *pwKey++);
		t2 = IdeaMul((x1 ^ x3) & 65535, *pwKey++);
		t1 = IdeaMul(((x2 ^ x4) + t2) & 65535, *pwKey++);
		t2 += t1;
		x1 ^= t1;
		x4 ^= t2;
		t2 ^= x2;
		x2 = x3 ^ t1;
		x3 = t2;
	} while (--nRounds);
	*pwOut++ = (WORD)IdeaMul(x1 & 65535, *pwKey++);
	*pwOut++ = (WORD)(x3 + *pwKey++);
	*pwOut++ = (WORD)(x2 + *pwKey++);
	*pwOut = (WORD)IdeaMul(x4 & 65535, *pwKey);
}

void __fastcall GridClearKeys(void)
{
	int i;

	for (i = 0; i < (int)(sizeof(GRIDKEYS) / (sizeof(DWORD))); i++)
		((DWORD *)sgaawKeys)[i] = 0;
}

void __fastcall GridCipher(int nKey, const BYTE *pbIn, BYTE *pbOut)
{
	IdeaCipher((const WORD *)pbIn, (WORD *)pbOut, sgaawKeys[nKey]);
}

struct GRIDUSERKEY
{
	DWORD dwKey0;
	DWORD dwKey1;
	DWORD dwKey2;
	DWORD dwKey3;
};

static inline void GridExpandKey(WORD *pwKey, GRIDUSERKEY tUserKey)
{
	int i;
	int j;

	*(GRIDUSERKEY *)pwKey = tUserKey;
	for (i = 0, j = 8; j < IDEA_KEYLEN; j++)
	{
		i++;
		pwKey[i + 7] = (WORD)(pwKey[i & 7] << 9 | pwKey[(i + 1) & 7] >> 7);
		pwKey += i & 8;
		i &= 7;
	}
}

void __fastcall GridSetKey(int nKey, BOOL fEncrypt, const BYTE *pbKey)
{
	WORD awKey[IDEA_KEYLEN];
	DWORD adwBytes[4];
	GRIDUSERKEY tKey;

	memcpy(adwBytes, pbKey, sizeof(adwBytes));
	tKey.dwKey0 = adwBytes[0];
	tKey.dwKey1 = adwBytes[1];
	tKey.dwKey2 = adwBytes[2];
	tKey.dwKey3 = adwBytes[3];
	if (fEncrypt)
	{
		GridExpandKey(sgaawKeys[nKey], tKey);
	}
	else
	{
		GridExpandKey(awKey, tKey);
		IdeaInvertKey(awKey, sgaawKeys[nKey]);
	}
}

void __fastcall GridHashInit(GRIDHASH *pt)
{
	pt->adwState[0] = 1732584193;
	pt->adwState[1] = 0xEFCDAB89;
	pt->adwState[2] = 0x98BADCFE;
	pt->adwState[3] = 271733878;
	pt->adwState[4] = 0xC3D2E1F0;
}

void __fastcall GridHashBlock(GRIDHASH *pt)
{
	DWORD adwW[80];
	DWORD a;
	DWORD b;
	DWORD c;
	DWORD d;
	DWORD e;
	DWORD t;
	DWORD i;

	memcpy(adwW, pt->abBlock, GRID_BLOCK);
	for (i = 16; 80 > i; i++)
		adwW[i] = GRIDROL(1, (adwW[i - 3] ^ adwW[i - 8] ^ adwW[i - 14] ^ adwW[i - 16]) & 31);
	a = pt->adwState[0];
	b = pt->adwState[1];
	c = pt->adwState[2];
	d = pt->adwState[3];
	e = pt->adwState[4];
	for (i = 0; 20 > i; i++)
	{
		t = GRIDROL(a, 5) + ((~b & d) | (c & b)) + adwW[i] + e + 1518500249;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	for (; 40 > i; i++)
	{
		t = GRIDROL(a, 5) + (d ^ c ^ b) + adwW[i] + e + 1859775393;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	for (; 60 > i; i++)
	{
		t = (((c | b) & d) | (c & b)) + GRIDROL(a, 5) + adwW[i] + e + 0x8F1BBCDC;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	for (; 80 > i; i++)
	{
		t = e + GRIDROL(a, 5) + (d ^ c ^ b) + adwW[i] + 0xCA62C1D6;
		e = d;
		d = c;
		c = GRIDROL(b, 30);
		b = a;
		a = t;
	}
	pt->adwState[0] += a;
	pt->adwState[1] += b;
	pt->adwState[2] += c;
	pt->adwState[3] += d;
	pt->adwState[4] += e;
}

void __fastcall GridHash(BYTE *pbHash, const void *pData, DWORD dwLength)
{
	GRIDHASH tHash;
	DWORD dwDone;
	DWORD dwBytes;

	GridHashInit(&tHash);
	for (dwDone = 0; dwDone < dwLength; dwDone += GRID_BLOCK)
	{
		dwBytes = dwLength - dwDone;
		if (dwBytes > GRID_BLOCK)
			dwBytes = GRID_BLOCK;
		memcpy(tHash.abBlock, pData, dwBytes);
		if (dwBytes < GRID_BLOCK)
			memset(tHash.abBlock + dwBytes, 0, GRID_BLOCK - dwBytes);
		GridHashBlock(&tHash);
	}
	memcpy(pbHash, tHash.adwState, sizeof(tHash.adwState));
}
