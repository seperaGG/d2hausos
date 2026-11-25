#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\CACHE.CPP"
#include "Bnclient.h"
#include "D2Macros.h"
#include "StormImports.h"

#define BNCACHE_FILE			"\\bncache.dat"
#define BNCACHE_SIGNATURE		439569986
#define BNCACHE_OLDSIGNATURE	439504450
#define BNCACHE_MAXFILE			10485760
#define BNCACHE_MAXDATA			16777216
#define BNCACHE_TABLESIZE		1024
#define BNCACHE_MAXBLOCKS		1024
#define BNCACHE_FREE			0xFFFFFFFF
#define BNCACHE_DELETED			0xFFFFFFFE
#define BNCACHE_EXISTS			0xC0000000
#define BNCACHE_CRYPT_ENTRIES	1280
#define BNCACHE_HASH_OFFSET		0
#define BNCACHE_HASH_NAME1		1
#define BNCACHE_HASH_NAME2		2
#define BNCACHE_HASH_KEY		3
#define BNCACHE_CRYPT_ROW		1024
#define BNCACHE_TICKS			10000000

struct BNCACHEHEADER
{
 	DWORD		dwSignature;
 	DWORD		dwHeaderSize;
 	DWORD		dwFileSize;
 	DWORD		field_0C;
 	DWORD		dwHashTablePos;
 	DWORD		dwBlockTablePos;
 	DWORD		dwHashTableSize;
 	DWORD		dwBlockTableSize;
 	DWORD		dwHeaderCrc;
 	DWORD		dwHashTableCrc;
 	DWORD		dwBlockTableCrc;
};

struct BNCACHEHASH
{
 	DWORD		dwName1;
 	DWORD		dwName2;
 	DWORD		dwLocale;
 	DWORD		dwBlockIndex;
};

struct BNCACHEBLOCK
{
 	DWORD		dwFilePos;
 	DWORD		dwBlockSize;
 	DWORD		dwDataSize;
 	DWORD		dwFlags;
};

struct BNCACHEENTRY
{
 	DWORD		dwSize;
 	DWORD		field_04;
 	DWORD		field_08;
 	DWORD		dwHeaderCrc;
 	DWORD		dwDataCrc;
 	DWORD		field_14;
 	FILETIME	tAccess;
 	FILETIME	tFile;
 	FILETIME	tStored;
 	FILETIME	tKeep;
 	FILETIME	tExpire;
 	char		szName[MAX_PATH];
};

static HANDLE sghCacheFile = INVALID_HANDLE_VALUE;
static DWORD sgadwCrcTable[256] =
{
	0, 1996959894, 0xEE0E612C, 0x990951BA, 124634137, 1886057615,
	0xE963A535, 0x9E6495A3, 249268274, 2044508324, 0xE0D5E91E, 0x97D2D988,
	162941995, 2125561021, 0xE7B82D07, 0x90BF1D91, 498536548, 1789927666,
	0xF3B97148, 0x84BE41DE, 450548861, 1843258603, 0xF4D4B551, 0x83D385C7,
	325883990, 1684777152, 0xFD62F97A, 0x8A65C9EC, 335633487, 1661365465,
	0xFA0F3D63, 0x8D080DF5, 997073096, 1281953886, 0xD56041E4, 0xA2677172,
	1006888145, 1258607687, 0xD20D85FD, 0xA50AB56B, 901097722, 1119000684,
	0xDBBBC9D6, 0xACBCF940, 853044451, 1172266101, 0xDCD60DCF, 0xABD13D59,
	651767980, 1373503546, 0xC8D75180, 0xBFD06116, 565507253, 1454621731,
	0xCFBA9599, 0xB8BDA50F, 671266974, 1594198024, 0xC60CD9B2, 0xB10BE924,
	795835527, 1483230225, 0xC1611DAB, 0xB6662D3D, 1994146192, 31158534,
	0x98D220BC, 0xEFD5102A, 1907459465, 112637215, 0x9FBFE4A5, 0xE8B8D433,
	2013776290, 251722036, 0x9609A88E, 0xE10E9818, 2137656763, 141376813,
	0x91646C97, 0xE6635C01, 1802195444, 476864866, 0x856530D8, 0xF262004E,
	1812370925, 453092731, 0x8208F4C1, 0xF50FC457, 1706088902, 314042704,
	0x8BBEB8EA, 0xFCB9887C, 1658658271, 366619977, 0x8CD37CF3, 0xFBD44C65,
	1303535960, 984961486, 0xA3BC0074, 0xD4BB30E2, 1256170817, 1037604311,
	0xA4D1C46D, 0xD3D6F4FB, 1131014506, 879679996, 0xAD678846, 0xDA60B8D0,
	1141124467, 855842277, 0xAA0A4C5F, 0xDD0D7CC9, 1342533948, 654459306,
	0xBE0B1010, 0xC90C2086, 1466479909, 544179635, 0xB966D409, 0xCE61E49F,
	1591671054, 702138776, 0xB0D09822, 0xC7D7A8B4, 1504918807, 783551873,
	0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 62317068, 1957810842,
	0xEAD54739, 0x9DD277AF, 81470997, 1943803523, 0xE3630B12, 0x94643B84,
	225274430, 2053790376, 0xE40ECF0B, 0x9309FF9D, 167816743, 2097651377,
	0xF00F9344, 0x8708A3D2, 503444072, 1762050814, 0xF762575D, 0x806567CB,
	426522225, 1852507879, 0xFED41B76, 0x89D32BE0, 282753626, 1742555852,
	0xF9B9DF6F, 0x8EBEEFF9, 397917763, 1622183637, 0xD6D6A3E8, 0xA1D1937E,
	953729732, 1340076626, 0xD1BB67F1, 0xA6BC5767, 1068828381, 1219638859,
	0xD80D2BDA, 0xAF0A1B4C, 906185462, 1090812512, 0xDF60EFC3, 0xA867DF55,
	829329135, 1181335161, 0xCB61B38C, 0xBC66831A, 628085408, 1382605366,
	0xCC0C7795, 0xBB0B4703, 570562233, 1426400815, 0xC5BA3BBE, 0xB2BD0B28,
	733239954, 1555261956, 0xC2D7FFA7, 0xB5D0CF31, 752459403, 1541320221,
	0x9B64C2B0, 0xEC63F226, 1969922972, 40735498, 0x9C0906A9, 0xEB0E363F,
	1913087877, 83908371, 0x95BF4A82, 0xE2B87A14, 2075208622, 213261112,
	0x92D28E9B, 0xE5D5BE0D, 2094854071, 198958881, 0x86D3D2D4, 0xF1D4E242,
	1759359992, 534414190, 0x81BE16CD, 0xF6B9265B, 1873836001, 414664567,
	0x88085AE6, 0xFF0F6A70, 1711684554, 285281116, 0x8F659EFF, 0xF862AE69,
	1634467795, 376229701, 0xA00AE278, 0xD70DD2EE, 1308918612, 956543938,
	0xA7672661, 0xD06016F7, 1231636301, 1047427035, 0xAED16A4A, 0xD9D65ADC,
	1088359270, 936918000, 0xA9BCAE53, 0xDEBB9EC5, 1202900863, 817233897,
	0xBDBDF21C, 0xCABAC28A, 1404277552, 615818150, 0xBAD03605, 0xCDD70693,
	1423857449, 601450431, 0xB3667A2E, 0xC4614AB8, 1567103746, 711928724,
	0xB40BBE37, 0xC30C8EA1, 1510334235, 755167117,
};
static FILETIME sgtCacheTime;
static BNLOCK sgtCacheLock;
static BNCACHEHEADER *sgptCacheHeader = 0;
static BNCACHEBLOCK *sgptCacheBlocks = 0;
static DWORD *sgpdwCryptTable = 0;
static BNCACHEHASH *sgptCacheHashes = 0;
static BOOL sgfCacheOpen = 0;
static DWORD sgdwCacheTimeSeconds = 0;
static DWORD sgdwCacheTimeTick = 0;

static __inline DWORD __fastcall CacheHashString(const char *pszString, DWORD dwHashType);
static __inline int __fastcall CacheFindEntry(const char *pszFile, DWORD dwLocale);

static DWORD CacheCrc(const void *buffer, DWORD len)
{
	int nValid = -1;

	nValid &= (buffer || !len) ? -1 : 0;
#line 139
	D2ASSERT(buffer || !len);
	if (!nValid)
	{
		StormSErrSetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	const BYTE *pbData = (const BYTE *)buffer;
	DWORD dwCrc = 0xFFFFFFFF;

	while (len--)
		dwCrc = (dwCrc >> 8) ^ sgadwCrcTable[(dwCrc & 255) ^ *pbData++];
	return ~dwCrc;
}

static void CacheDecrypt(DWORD *pdwData, DWORD dwBytes, DWORD dwKey)
{
	DWORD dwSeed = 0xEEEEEEEE;
	DWORD dwCount = dwBytes >> 2;

	while (dwCount--)
	{
		dwSeed += sgpdwCryptTable[BNCACHE_CRYPT_ROW + (dwKey & 255)];
		dwSeed += (*pdwData++ ^= dwSeed + dwKey) + (dwSeed << 5) + 3;
		dwKey = (dwKey >> 11) | ((dwKey << 21) ^ 0xFFE00000) + 286331153;
	}
}

static void CacheEncrypt(DWORD *pdwData, DWORD dwBytes, DWORD dwKey)
{
	DWORD dwSeed = 0xEEEEEEEE;
	DWORD dwCount = dwBytes >> 2;

	while (dwCount--)
	{
		DWORD dwValue = *pdwData;

		dwSeed += sgpdwCryptTable[BNCACHE_CRYPT_ROW + (dwKey & 255)];
		*pdwData++ = dwValue ^ (dwSeed + dwKey);
		dwSeed += dwValue + (dwSeed << 5) + 3;
		dwKey = (dwKey >> 11) | ((dwKey << 21) ^ 0xFFE00000) + 286331153;
	}
}

static void CacheGetPath(char *pszPath)
{
	char *pszSlash;

	pszPath[0] = 0;
	GetModuleFileNameA(GetModuleHandleA(NULL), pszPath, MAX_PATH);
	pszSlash = strchr(pszPath, '\\');
	while (pszSlash && strchr(pszSlash + 1, '\\'))
		pszSlash = strchr(pszSlash + 1, '\\');
	if (pszSlash)
		*pszSlash = 0;
	strcat(pszPath, BNCACHE_FILE);
}

static void CacheDeleteFile(void)
{
	char szPath[MAX_PATH];

	CacheGetPath(szPath);
	DeleteFileA(szPath);
}

static const FILETIME *CacheGetTime(DWORD dwSeconds)
{
	SYSTEMTIME tSystem;
	__int64 nTicks;

	if (dwSeconds != sgdwCacheTimeSeconds || GetTickCount() != sgdwCacheTimeTick)
	{
		GetSystemTime(&tSystem);
		SystemTimeToFileTime(&tSystem, &sgtCacheTime);
		nTicks = 10000000ul;
		nTicks *= (__int64)dwSeconds;
		*(__int64 *)&sgtCacheTime += nTicks;
		sgdwCacheTimeSeconds = dwSeconds;
		sgdwCacheTimeTick = GetTickCount();
	}
	return &sgtCacheTime;
}

static void CacheDeleteEntries(const char *pszFile)
{
	int nHash;
	DWORD dwBlock;

	while ((nHash = CacheFindEntry(pszFile, 0)) != -1)
	{
		dwBlock = sgptCacheHashes[nHash].dwBlockIndex;
		if (dwBlock < sgptCacheHeader->dwBlockTableSize)
		{
			sgptCacheBlocks[dwBlock].dwDataSize = 0;
			sgptCacheBlocks[dwBlock].dwFlags = 0;
		}
		sgptCacheHashes[nHash].dwBlockIndex = BNCACHE_DELETED;
	}
}

static void CacheReplace(int nError)
{
	char szPath[MAX_PATH];

	BnclientLogPrintf("bncache.dat is corrupt (%d) - rebuilding archive", nError);
	if (sgfCacheOpen)
	{
		CacheClose();
		if (sghCacheFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(sghCacheFile);
			sghCacheFile = INVALID_HANDLE_VALUE;
		}
		CacheDeleteFile();
	}
	CacheGetPath(szPath);
	DeleteFileA(szPath);
	CacheOpen();
}

void __fastcall CacheClose(void)
{
	sgtCacheLock.Enter();
	sgfCacheOpen = FALSE;
	if (sgptCacheBlocks)
	{
#line 318
		StormSMemFree(sgptCacheBlocks, D2_THIS_FILE, __LINE__, 0);
		sgptCacheBlocks = NULL;
	}
	if (sgptCacheHashes)
	{
#line 322
		StormSMemFree(sgptCacheHashes, D2_THIS_FILE, __LINE__, 0);
		sgptCacheHashes = NULL;
	}
	if (sgptCacheHeader)
	{
#line 326
		StormSMemFree(sgptCacheHeader, D2_THIS_FILE, __LINE__, 0);
		sgptCacheHeader = NULL;
	}
	if (sghCacheFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(sghCacheFile);
		sghCacheFile = INVALID_HANDLE_VALUE;
	}
	if (sgpdwCryptTable)
	{
#line 334
		StormSMemFree(sgpdwCryptTable, D2_THIS_FILE, __LINE__, 0);
		sgpdwCryptTable = NULL;
	}
	sgtCacheLock.Leave();
}

BOOL __fastcall CacheFree(BYTE *pbData, DWORD dwBytes)
{
	if (!pbData)
		return FALSE;
#line 344
	StormSMemFree(pbData, D2_THIS_FILE, __LINE__, 0);
	return TRUE;
}

BOOL __fastcall CacheOpen(void)
{
	char szPath[MAX_PATH];
	DWORD dwFileSize;
	DWORD dwCrc;
	DWORD dwSeed;
	DWORD dwHigh;
	DWORD dwLow;
	int nError;
	int i;
	int j;

	sgtCacheLock.Enter();
	if (sgfCacheOpen)
		CacheClose();
	CacheGetPath(szPath);
	sghCacheFile = CreateFileA(szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (sghCacheFile == INVALID_HANDLE_VALUE)
		goto failed;
#line 380
	sgpdwCryptTable = (DWORD *)StormSMemAlloc(BNCACHE_CRYPT_ENTRIES * sizeof(DWORD), D2_THIS_FILE, __LINE__, 0);
	if (sgpdwCryptTable)
	{
		dwSeed = 1048577;
		for (i = 0; i < 256; i++)
		{
			for (j = 0; j < 5; j++)
			{
				dwSeed = (dwSeed * 125 + 3) % 2796203;
				dwHigh = dwSeed & 65535;
				dwSeed = (dwSeed * 125 + 3) % 2796203;
				dwLow = dwSeed & 65535;
				sgpdwCryptTable[(j << 8) + i] = (dwHigh << 16) | dwLow;
			}
		}
	}
#line 384
	sgptCacheHeader = (BNCACHEHEADER *)StormSMemAlloc(sizeof(BNCACHEHEADER), D2_THIS_FILE, __LINE__, 8);
	dwFileSize = GetFileSize(sghCacheFile, NULL);
	if (!dwFileSize || dwFileSize > BNCACHE_MAXFILE)
		goto replace;
	if (dwFileSize < sizeof(BNCACHEHEADER))
	{
		nError = 1;
		goto invalid;
	}
	{
		DWORD dwDone;

		ReadFile(sghCacheFile, sgptCacheHeader, sizeof(BNCACHEHEADER), &dwDone, NULL);
		if (dwDone < sizeof(BNCACHEHEADER))
		{
			nError = 2;
			goto invalid;
		}
	}
	if (sgptCacheHeader->dwSignature == BNCACHE_OLDSIGNATURE)
		goto replace;
	if (sgptCacheHeader->dwSignature != BNCACHE_SIGNATURE)
	{
		nError = 3;
		goto invalid;
	}
	dwCrc = sgptCacheHeader->dwHeaderCrc;
	sgptCacheHeader->dwHeaderCrc = 0;
	if (dwCrc != CacheCrc(sgptCacheHeader, sizeof(BNCACHEHEADER)))
	{
		nError = 4;
		goto invalid;
	}
	sgptCacheHeader->dwHeaderCrc = dwCrc;
	if (!sgptCacheHeader->dwHashTablePos || !sgptCacheHeader->dwHashTableSize)
	{
		nError = 5;
		goto invalid;
	}
	if (!sgptCacheHeader->dwBlockTablePos || !sgptCacheHeader->dwBlockTableSize)
	{
		nError = 6;
		goto invalid;
	}
	{
		DWORD dwBytes = max(BNCACHE_TABLESIZE, sgptCacheHeader->dwHashTableSize) * sizeof(BNCACHEHASH);
		DWORD dwDone;

		if (dwBytes + sgptCacheHeader->dwHashTablePos > dwFileSize)
		{
			nError = 7;
			goto invalid;
		}
#line 437
		sgptCacheHashes = (BNCACHEHASH *)StormSMemAlloc(dwBytes, D2_THIS_FILE, __LINE__, 8);
		SetFilePointer(sghCacheFile, sgptCacheHeader->dwHashTablePos, NULL, FILE_BEGIN);
		ReadFile(sghCacheFile, sgptCacheHashes, dwBytes, &dwDone, NULL);
		if (dwDone != dwBytes)
		{
			nError = 8;
			goto invalid;
		}
		if (sgptCacheHeader->dwHashTableCrc != CacheCrc(sgptCacheHashes, dwBytes))
		{
			nError = 9;
			goto invalid;
		}
		CacheDecrypt((DWORD *)sgptCacheHashes, dwBytes, CacheHashString("(hash table)", BNCACHE_HASH_KEY));
	}
	{
		DWORD dwBytes = max(BNCACHE_TABLESIZE, sgptCacheHeader->dwBlockTableSize) * sizeof(BNCACHEBLOCK);
		DWORD dwDone;

		if (sgptCacheHeader->dwBlockTablePos + dwBytes > dwFileSize)
		{
			nError = 10;
			goto invalid;
		}
#line 459
		sgptCacheBlocks = (BNCACHEBLOCK *)StormSMemAlloc(dwBytes, D2_THIS_FILE, __LINE__, 8);
		SetFilePointer(sghCacheFile, sgptCacheHeader->dwBlockTablePos, NULL, FILE_BEGIN);
		ReadFile(sghCacheFile, sgptCacheBlocks, dwBytes, &dwDone, NULL);
		if (dwDone != dwBytes)
		{
			nError = 11;
			goto invalid;
		}
		if (sgptCacheHeader->dwBlockTableCrc != CacheCrc(sgptCacheBlocks, dwBytes))
		{
			nError = 12;
			goto invalid;
		}
		CacheDecrypt((DWORD *)sgptCacheBlocks, dwBytes, CacheHashString("(block table)", BNCACHE_HASH_KEY));
	}
	sgfCacheOpen = TRUE;
	sgtCacheLock.Leave();
	return TRUE;

invalid:
	BnclientLogPrintf("WARNING- cache file is invalid. Replacing with new cache file", nError);
replace:
	if (!sgfCacheOpen)
	{
		if (sgptCacheHashes)
		{
#line 485
			StormSMemFree(sgptCacheHashes, D2_THIS_FILE, __LINE__, 0);
		}
		if (sgptCacheBlocks)
		{
#line 487
			StormSMemFree(sgptCacheBlocks, D2_THIS_FILE, __LINE__, 0);
		}
		if (sghCacheFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(sghCacheFile);
			sghCacheFile = INVALID_HANDLE_VALUE;
		}
		CacheDeleteFile();
		sghCacheFile = CreateFileA(szPath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL);
		if (sghCacheFile == INVALID_HANDLE_VALUE)
			goto failed;
		ZeroMemory(sgptCacheHeader, sizeof(BNCACHEHEADER));
		sgptCacheHeader->dwSignature = BNCACHE_SIGNATURE;
		sgptCacheHeader->dwHeaderSize = sizeof(BNCACHEHEADER);
#line 517
		sgptCacheHashes = (BNCACHEHASH *)StormSMemAlloc(BNCACHE_TABLESIZE * sizeof(BNCACHEHASH), D2_THIS_FILE, __LINE__, 8);
		sgptCacheHeader->dwHashTablePos = sizeof(BNCACHEHEADER);
		sgptCacheHeader->dwHashTableSize = BNCACHE_TABLESIZE;
		sgptCacheHeader->dwFileSize = max(sgptCacheHeader->dwFileSize,
			sgptCacheHeader->dwHashTablePos + BNCACHE_TABLESIZE * sizeof(BNCACHEHASH));
		for (i = 0; i < BNCACHE_TABLESIZE; i++)
			sgptCacheHashes[i].dwBlockIndex = BNCACHE_FREE;
#line 530
		sgptCacheBlocks = (BNCACHEBLOCK *)StormSMemAlloc(BNCACHE_TABLESIZE * sizeof(BNCACHEBLOCK), D2_THIS_FILE, __LINE__, 8);
		sgptCacheHeader->dwBlockTablePos = sizeof(BNCACHEHEADER) + BNCACHE_TABLESIZE * sizeof(BNCACHEHASH);
		sgptCacheHeader->dwFileSize = max(sgptCacheHeader->dwFileSize,
			sgptCacheHeader->dwBlockTablePos + BNCACHE_TABLESIZE * sizeof(BNCACHEBLOCK));
		sgfCacheOpen = TRUE;
	}
	sgtCacheLock.Leave();
	return TRUE;

failed:
	CacheClose();
	sgtCacheLock.Leave();
	return FALSE;
}

BOOL __fastcall CacheFind(const char *pszFile, FILETIME *ptTime, BYTE **ppbData, DWORD *pdwBytes)
{
	BNCACHEENTRY tEntry;
	DWORD dwBlock;
	DWORD dwCrc;
	int nHash;
	int nCorrupt;
	int nDeleted;

	if (ptTime)
		ZeroMemory(ptTime, sizeof(FILETIME));
	if (ppbData)
		*ppbData = NULL;
	if (pdwBytes)
		*pdwBytes = 0;
	if (!pszFile || !*pszFile || !ppbData || !pdwBytes || !sgfCacheOpen)
		return FALSE;
	nCorrupt = 0;
	nDeleted = 0;
	sgtCacheLock.Enter();
	nHash = CacheFindEntry(pszFile, 0);
	if (nHash == -1)
	{
		sgtCacheLock.Leave();
		return FALSE;
	}
	dwBlock = sgptCacheHashes[nHash].dwBlockIndex;
	if (dwBlock >= sgptCacheHeader->dwBlockTableSize)
	{
		nCorrupt = 1;
		goto done;
	}
	ZeroMemory(&tEntry, sizeof(BNCACHEENTRY));
	if (sgptCacheBlocks[dwBlock].dwFlags & BNCACHE_EXISTS)
	{
		DWORD dwDone;

		SetFilePointer(sghCacheFile, sgptCacheBlocks[dwBlock].dwFilePos, NULL, FILE_BEGIN);
		ReadFile(sghCacheFile, &tEntry, sizeof(DWORD), &dwDone, NULL);
		if (dwDone != sizeof(DWORD))
		{
			nCorrupt = 2;
			goto done;
		}
		if (tEntry.dwSize != sizeof(BNCACHEENTRY))
		{
			nCorrupt = 3;
			goto done;
		}
		ReadFile(sghCacheFile, &tEntry.field_04, sizeof(BNCACHEENTRY) - sizeof(DWORD), &dwDone, NULL);
		if (dwDone != sizeof(BNCACHEENTRY) - sizeof(DWORD))
		{
			nCorrupt = 4;
			goto done;
		}
		dwCrc = tEntry.dwHeaderCrc;
		tEntry.dwHeaderCrc = 0;
		if (dwCrc != CacheCrc(&tEntry, sizeof(BNCACHEENTRY)))
		{
			nDeleted = 1;
			goto done;
		}
		tEntry.dwHeaderCrc = dwCrc;
	}
	if (_stricmp(pszFile, tEntry.szName))
	{
		nDeleted = 2;
		goto done;
	}
	if (CompareFileTime(CacheGetTime(0), &tEntry.tExpire) > 0)
	{
		nDeleted = 3;
		goto done;
	}
	if (sgptCacheBlocks[dwBlock].dwDataSize > BNCACHE_MAXDATA)
	{
		nCorrupt = 5;
		goto done;
	}
	if (ptTime)
		*ptTime = tEntry.tFile;
	tEntry.tAccess = *CacheGetTime(0);
	SetFilePointer(sghCacheFile, sgptCacheBlocks[dwBlock].dwFilePos, NULL, FILE_BEGIN);
	tEntry.dwHeaderCrc = 0;
	tEntry.dwHeaderCrc = CacheCrc(&tEntry, sizeof(BNCACHEENTRY));
	{
		DWORD dwBytes = min(sizeof(BNCACHEENTRY), tEntry.dwSize);
		DWORD dwDone;

		WriteFile(sghCacheFile, &tEntry, dwBytes, &dwDone, NULL);
		if (dwDone != dwBytes)
		{
			nCorrupt = 6;
			goto done;
		}
	}
#line 667
	*ppbData = (BYTE *)StormSMemAlloc(sgptCacheBlocks[dwBlock].dwDataSize, D2_THIS_FILE, __LINE__, 0);
	SetFilePointer(sghCacheFile, sgptCacheBlocks[dwBlock].dwFilePos + tEntry.dwSize, NULL, FILE_BEGIN);
	ReadFile(sghCacheFile, *ppbData, sgptCacheBlocks[dwBlock].dwDataSize, pdwBytes, NULL);
	if (*pdwBytes != sgptCacheBlocks[dwBlock].dwDataSize)
	{
		nCorrupt = 7;
		goto done;
	}
	if (tEntry.dwDataCrc != CacheCrc(*ppbData, *pdwBytes))
	{
		nDeleted = 4;
		goto done;
	}
done:
	if (nCorrupt)
		CacheReplace(nCorrupt);
	if (nDeleted)
	{
		BnclientLogPrintf("File: '%s' in bncache.dat is corrupt (%d) - deleting from cache", pszFile,
			nDeleted);
		CacheDeleteEntries(pszFile);
	}
	if (nCorrupt || nDeleted)
	{
		if (*ppbData)
		{
#line 708
			StormSMemFree(*ppbData, D2_THIS_FILE, __LINE__, 0);
			*ppbData = NULL;
		}
		*pdwBytes = 0;
	}
	sgtCacheLock.Leave();
	return !nDeleted && !nCorrupt;
}

static __inline DWORD __fastcall CacheHashString(const char *pszString, DWORD dwHashType)
{
	DWORD dwSeed1 = 2146271213;
	DWORD dwSeed2 = 0xEEEEEEEE;

	while (pszString && *pszString)
	{
		char chUpper = toupper(*pszString++);

		dwSeed1 = (dwSeed1 + dwSeed2) ^ sgpdwCryptTable[(dwHashType << 8) + chUpper];
		dwSeed2 += chUpper + dwSeed1 + (dwSeed2 << 5) + 3;
	}
	return dwSeed1;
}

BOOL __fastcall CacheStore(const char *pszFile, const BYTE *pbData, DWORD dwBytes, const FILETIME *ptTime,
	DWORD dwKeepSeconds, DWORD dwExpireSeconds)
{
	BNCACHEENTRY tEntry;
	BYTE *pbTable;
	DWORD dwNeed;
	DWORD dwBlock;
	DWORD dwHashBytes;
	DWORD dwBlockBytes;
	DWORD j;
	int nError;

	if (!pszFile || !*pszFile || !pbData || !dwBytes || !ptTime || !dwExpireSeconds || !sgfCacheOpen
		|| dwBytes > BNCACHE_MAXDATA)
		return FALSE;
	nError = 0;
	sgtCacheLock.Enter();
	CacheDeleteEntries(pszFile);
	dwNeed = dwBytes + sizeof(BNCACHEENTRY);
	dwBlock = 0;
	while (dwBlock < sgptCacheHeader->dwBlockTableSize
		&& (sgptCacheBlocks[dwBlock].dwFlags || sgptCacheBlocks[dwBlock].dwBlockSize < dwNeed))
		++dwBlock;
	if (dwBlock >= sgptCacheHeader->dwBlockTableSize)
	{
		dwBlock = 0;
		while (dwBlock < sgptCacheHeader->dwBlockTableSize)
		{
			if (sgptCacheBlocks[dwBlock].dwBlockSize >= dwNeed)
			{
				SetFilePointer(sghCacheFile, sgptCacheBlocks[dwBlock].dwFilePos, NULL, FILE_BEGIN);

				BNCACHEENTRY tOld;
				DWORD dwDone;

				ZeroMemory(&tOld, sizeof(BNCACHEENTRY));
				ReadFile(sghCacheFile, &tOld, sizeof(DWORD), &dwDone, NULL);
				if (dwDone != sizeof(DWORD) || tOld.dwSize != sizeof(BNCACHEENTRY))
					break;
				ReadFile(sghCacheFile, &tOld.field_04, sizeof(BNCACHEENTRY) - sizeof(DWORD), &dwDone, NULL);
				if (dwDone != sizeof(BNCACHEENTRY) - sizeof(DWORD))
					break;
				if (CompareFileTime(CacheGetTime(0), &tOld.tKeep) > 0)
					break;
			}
			++dwBlock;
		}
	}
	if (dwBlock < sgptCacheHeader->dwBlockTableSize)
	{
		for (j = 0; j < sgptCacheHeader->dwHashTableSize; j++)
			if (sgptCacheHashes[j].dwBlockIndex == dwBlock)
				sgptCacheHashes[j].dwBlockIndex = BNCACHE_DELETED;
	}
	else
	{
		if (dwBlock >= BNCACHE_MAXBLOCKS)
		{
			nError = 11;
			goto done;
		}
		sgptCacheBlocks[dwBlock].dwFilePos = sgptCacheHeader->dwFileSize;
		sgptCacheBlocks[dwBlock].dwBlockSize = dwNeed;
		sgptCacheHeader->dwFileSize += dwNeed;
		sgptCacheHeader->dwBlockTableSize = dwBlock + 1;
	}
	{
		DWORD dwHash = CacheHashString(pszFile, BNCACHE_HASH_OFFSET);
		DWORD dwName1 = CacheHashString(pszFile, BNCACHE_HASH_NAME1);
		DWORD dwName2 = CacheHashString(pszFile, BNCACHE_HASH_NAME2);
		DWORD dwSlot = dwHash & (sgptCacheHeader->dwHashTableSize - 1);
		DWORD dwStart = dwSlot;

		while (sgptCacheHashes[dwSlot].dwBlockIndex != BNCACHE_FREE
			&& sgptCacheHashes[dwSlot].dwBlockIndex != BNCACHE_DELETED)
		{
			dwSlot = (dwSlot + 1) & (sgptCacheHeader->dwHashTableSize - 1);
			if (dwSlot == dwStart)
				break;
		}
		if (sgptCacheHashes[dwSlot].dwBlockIndex != BNCACHE_FREE
			&& sgptCacheHashes[dwSlot].dwBlockIndex != BNCACHE_DELETED)
		{
			nError = 12;
			goto done;
		}
		sgptCacheHashes[dwSlot].dwName1 = dwName1;
		sgptCacheHashes[dwSlot].dwName2 = dwName2;
		sgptCacheHashes[dwSlot].dwLocale = 0;
		sgptCacheHashes[dwSlot].dwBlockIndex = dwBlock;
	}
	sgptCacheBlocks[dwBlock].dwDataSize = dwBytes;
	sgptCacheBlocks[dwBlock].dwFlags = BNCACHE_EXISTS;
	ZeroMemory(&tEntry, sizeof(BNCACHEENTRY));
	tEntry.dwSize = sizeof(BNCACHEENTRY);
	CopyMemory(&tEntry.tStored, CacheGetTime(0), sizeof(FILETIME));
	CopyMemory(&tEntry.tFile, ptTime, sizeof(FILETIME));
	CopyMemory(&tEntry.tKeep, CacheGetTime(dwKeepSeconds), sizeof(FILETIME));
	CopyMemory(&tEntry.tExpire, CacheGetTime(dwExpireSeconds), sizeof(FILETIME));
	CopyMemory(&tEntry.tAccess, CacheGetTime(0), sizeof(FILETIME));
	strcpy(tEntry.szName, pszFile);
	tEntry.dwDataCrc = CacheCrc(pbData, dwBytes);
	tEntry.dwHeaderCrc = CacheCrc(&tEntry, sizeof(BNCACHEENTRY));
	SetFilePointer(sghCacheFile, sgptCacheBlocks[dwBlock].dwFilePos, NULL, FILE_BEGIN);
	{
		DWORD dwDone;

		WriteFile(sghCacheFile, &tEntry, sizeof(BNCACHEENTRY), &dwDone, NULL);
		if (dwDone != sizeof(BNCACHEENTRY))
		{
			nError = 13;
			goto done;
		}
		WriteFile(sghCacheFile, pbData, dwBytes, &dwDone, NULL);
		if (dwDone != dwBytes)
		{
			nError = 14;
			goto done;
		}
	}
	{
		DWORD dwDone;

		dwHashBytes = max(BNCACHE_TABLESIZE, sgptCacheHeader->dwHashTableSize) * sizeof(BNCACHEHASH);
		dwBlockBytes = max(BNCACHE_TABLESIZE, sgptCacheHeader->dwBlockTableSize) * sizeof(BNCACHEBLOCK);
#line 887
		pbTable = (BYTE *)StormSMemAlloc(max(dwHashBytes, dwBlockBytes), D2_THIS_FILE, __LINE__, 0);
		SetFilePointer(sghCacheFile, sgptCacheHeader->dwHashTablePos, NULL, FILE_BEGIN);
		memcpy(pbTable, sgptCacheHashes, dwHashBytes);
		CacheEncrypt((DWORD *)pbTable, dwHashBytes, CacheHashString("(hash table)", BNCACHE_HASH_KEY));
		sgptCacheHeader->dwHashTableCrc = CacheCrc(pbTable, dwHashBytes);
		WriteFile(sghCacheFile, pbTable, dwHashBytes, &dwDone, NULL);
		if (dwDone != dwHashBytes)
		{
#line 899
			StormSMemFree(pbTable, D2_THIS_FILE, __LINE__, 0);
			nError = 15;
			goto done;
		}
		SetFilePointer(sghCacheFile, sgptCacheHeader->dwBlockTablePos, NULL, FILE_BEGIN);
		memcpy(pbTable, sgptCacheBlocks, dwBlockBytes);
		CacheEncrypt((DWORD *)pbTable, dwBlockBytes, CacheHashString("(block table)", BNCACHE_HASH_KEY));
		sgptCacheHeader->dwBlockTableCrc = CacheCrc(pbTable, dwBlockBytes);
		WriteFile(sghCacheFile, pbTable, dwBlockBytes, &dwDone, NULL);
#line 913
		StormSMemFree(pbTable, D2_THIS_FILE, __LINE__, 0);
		if (dwDone != dwBlockBytes)
		{
			nError = 16;
			goto done;
		}
		sgptCacheHeader->dwHeaderCrc = 0;
		sgptCacheHeader->dwHeaderCrc = CacheCrc(sgptCacheHeader, sizeof(BNCACHEHEADER));
		SetFilePointer(sghCacheFile, 0, NULL, FILE_BEGIN);
		WriteFile(sghCacheFile, sgptCacheHeader, sizeof(BNCACHEHEADER), &dwDone, NULL);
		if (dwDone != sizeof(BNCACHEHEADER))
			nError = 17;
	}
done:
	if (nError)
		CacheReplace(nError);
	sgtCacheLock.Leave();
	return nError == 0;
}

static __inline int __fastcall CacheFindEntry(const char *pszFile, DWORD dwLocale)
{
	DWORD dwHash = CacheHashString(pszFile, BNCACHE_HASH_OFFSET);
	DWORD dwName1 = CacheHashString(pszFile, BNCACHE_HASH_NAME1);
	DWORD dwName2 = CacheHashString(pszFile, BNCACHE_HASH_NAME2);
	DWORD dwSlot = dwHash & (sgptCacheHeader->dwHashTableSize - 1);
	DWORD dwStart = dwSlot;
	DWORD dwResult = 0xFFFFFFFF;

	while (sgptCacheHashes[dwSlot].dwBlockIndex != BNCACHE_FREE)
	{
		if (sgptCacheHashes[dwSlot].dwName1 == dwName1 && sgptCacheHashes[dwSlot].dwName2 == dwName2
			&& sgptCacheHashes[dwSlot].dwBlockIndex != BNCACHE_DELETED)
			if (sgptCacheHashes[dwSlot].dwLocale == dwLocale)
				return dwSlot;
			else if (sgptCacheHashes[dwSlot].dwLocale == 0)
				dwResult = dwSlot;
		dwSlot = (dwSlot + 1) & (sgptCacheHeader->dwHashTableSize - 1);
		if (dwSlot == dwStart)
			break;
	}
	return dwResult;
}
