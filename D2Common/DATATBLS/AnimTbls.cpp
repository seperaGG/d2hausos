#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\AnimTbls.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "../hell/Archive.h"
#include "DataTbls.h"

DWORD __fastcall LoggingCloseNamed(void);

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_DETACH)
		LoggingCloseNamed();
	return TRUE;
}

PTANIMDATATBL __fastcall AnimTblsLoad(HD2ARCHIVE hArchive)
{
	char szPath[MAX_PATH];
	PTANIMDATATBL ptTbl;
	PTANIMDATABUCKET ptBucket;
	int i;

#line 127
	ptTbl = (PTANIMDATATBL)D2ALLOCPOOL(NULL, sizeof(ANIMDATATBL));
	for (i = 0; i < (int)(sizeof(ANIMDATATBL) / sizeof(DWORD)); i++)
		((DWORD *)ptTbl)[i] = 0;
	wsprintfA(szPath, "%s\\AnimData.d2", "DATA\\GLOBAL");
#line 138
	ptTbl->pBuffer = ArchiveAllocBufferAndRead(hArchive, szPath, NULL, D2_THIS_FILE, __LINE__);
	ptBucket = (PTANIMDATABUCKET)ptTbl->pBuffer;
	for (i = 0; i < 256; i++)
	{
		ptTbl->aptBuckets[i] = ptBucket;
		ptBucket = (PTANIMDATABUCKET)&ptBucket->atRecords[ptBucket->dwNumRecords];
	}
	ptTbl->tDefaultRecord.dwFrames = 2048;
	ptTbl->tDefaultRecord.dwSpeed = 256;
	return ptTbl;
}

void __fastcall AnimTblsFree(PTANIMDATATBL ptTbl)
{
	if (ptTbl)
	{
#line 188
		D2FREEPOOL(NULL, ptTbl->pBuffer);
#line 189
		D2FREEPOOL(NULL, ptTbl);
	}
}

static __inline BOOL AnimTblsCompareNames(DWORD *pdwName1, DWORD *pdwName2)
{
#line 215
	D2ASSERT(StringLength((LPCTSTR)pdwName1) <= 8);
#line 216
	D2ASSERT(StringLength((LPCTSTR)pdwName2) <= 8);
	return pdwName1[0] == pdwName2[0] && pdwName1[1] == pdwName2[1];
}

static PTANIMDATAREC AnimTblsFindInBucket(PTANIMDATABUCKET ptBucket, DWORD *dwName)
{
	PTANIMDATAREC ptRec;
	int nCount;

	ptRec = ptBucket->atRecords;
	for (nCount = (int)ptBucket->dwNumRecords; nCount > 0; nCount--)
	{
		if (AnimTblsCompareNames(dwName, (DWORD *)ptRec->szCofName))
			return ptRec;
		ptRec++;
	}
	return NULL;
}

PTANIMDATAREC __fastcall AnimTblsFindSequence(DWORD dwUnit, DWORD dwClass, DWORD dwMode, DWORD dwUnitType,
	DWORD dwInventory)
{
	DWORD dwUnused;
	DWORD dwName[2];
	char *psz;
	PTANIMDATATBL ptTbl;
	DWORD bHash;
	PTANIMDATABUCKET ptBucket;
	PTANIMDATAREC ptFound;

	dwUnused = 0;
	memset(dwName, 0, sizeof(dwName));

	D2CommonCOMPOSITGetCofName(dwUnit, dwClass, dwMode, dwUnitType, dwInventory, dwName, &dwUnused, 0, 1);
	for (psz = (char *)dwName; *psz; psz++)
	{
		if (*psz >= 'a' && *psz <= 'z')
			*psz -= 32;
	}
	ptTbl = DataTblsGetAnimData();
	bHash = 0;
	for (psz = (char *)dwName; *psz; psz++)
		bHash += (BYTE)*psz;
	ptBucket = ptTbl->aptBuckets[(BYTE)bHash];
	ptFound = AnimTblsFindInBucket(ptBucket, dwName);
	if (!ptFound)
		ptFound = &DataTblsGetAnimData()->tDefaultRecord;
	return ptFound;
}

void __stdcall AnimTblsSetUnitAnimSpeed(PTUNIT hUnit, DWORD dwSpeed)
{
	DWORD dwWeaponClass;
	DWORD dwName[2];
	char *psz;
	PTANIMDATATBL ptTbl;
	DWORD bHash;
	PTANIMDATABUCKET ptBucket;
	PTANIMDATAREC ptRec;
	PTANIMDATAREC ptFound;
	int nCount;

	dwWeaponClass = 0;
	memset(dwName, 0, sizeof(dwName));

	D2CommonCOMPOSITGetCofNameForUnit(hUnit, dwName, &dwWeaponClass, 0, 1, hUnit->ptInventory, -1);
	for (psz = (char *)dwName; *psz; psz++)
	{
		if (*psz >= 'a' && *psz <= 'z')
			*psz -= 32;
	}
	ptTbl = DataTblsGetAnimData();
	bHash = 0;
	for (psz = (char *)dwName; *psz; psz++)
		bHash += (BYTE)*psz;
	ptBucket = ptTbl->aptBuckets[(BYTE)bHash];
	ptRec = ptBucket->atRecords;
	ptFound = NULL;
	for (nCount = (int)ptBucket->dwNumRecords; nCount > 0; nCount--)
	{
		if (AnimTblsCompareNames(dwName, (DWORD *)ptRec->szCofName))
		{
			ptFound = ptRec;
			break;
		}
		ptRec++;
	}
	if (ptFound)
	{
		ptFound->dwSpeed = dwSpeed;
		return;
	}
#line 304
	D2FATAL("DataTablesUnitAnimInfoDebugSet");
}

void __stdcall AnimTblsGetAnimInfo(char *pszName, DWORD *pdwFrames, DWORD *pdwSpeed, DWORD *pdwFirstFrame)
{
	char *psz;
	PTANIMDATATBL ptTbl;
	DWORD bHash;
	PTANIMDATABUCKET ptBucket;
	PTANIMDATAREC ptRec;
	PTANIMDATAREC ptInfo;
	int nCount;
	DWORD i;

	for (psz = pszName; *psz; psz++)
	{
		if (*psz >= 'a' && *psz <= 'z')
			*psz -= 32;
	}
	ptTbl = DataTblsGetAnimData();
	bHash = 0;
	for (psz = pszName; *psz; psz++)
		bHash += (BYTE)*psz;
	ptBucket = ptTbl->aptBuckets[(BYTE)bHash];
	ptRec = ptBucket->atRecords;
	ptInfo = NULL;
	for (nCount = (int)ptBucket->dwNumRecords; nCount > 0; nCount--)
	{
		if (AnimTblsCompareNames((DWORD *)pszName, (DWORD *)ptRec->szCofName))
		{
			ptInfo = ptRec;
			break;
		}
		ptRec++;
	}
#line 345
	if (ptInfo) {
		*pdwFrames = ptInfo->dwFrames;
		*pdwSpeed = ptInfo->dwSpeed;
		for (i = 0; i < ptInfo->dwFrames; i++)
		{
			if (ptInfo->abFrameEvents[i])
				break;
		}
		*pdwFirstFrame = i; return; }
#line 345
	D2ASSERT(ptInfo);
}
#line 355

extern const char *const gcpszModuleFile = "D2Com.dll";
