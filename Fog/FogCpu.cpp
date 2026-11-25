#include "FogLocal.h"
#include <time.h>
#include "FogLocalAsync.h"
#include <string.h>
#include "ErrorManager.h"
#include "mem.h"

DWORD __fastcall CpuGetFeatures(DWORD dwLevel)
{
	DWORD dwFeatures;

	__asm
	{
		mov eax, dwLevel
		cpuid
		mov dwFeatures, edx
	}
	return dwFeatures;
}

static const char *sgpszCpuVendors[4] = { "Unknown", "Intel", "AMD", "Cyrix" };

static __inline int CpuGetVendor(void)
{
	char szVendorId[16];

	memset(szVendorId, 0, sizeof(szVendorId));
	__asm
	{
		mov eax, 0
		cpuid
		mov dword ptr szVendorId[0], ebx
		mov dword ptr szVendorId[4], edx
		mov dword ptr szVendorId[8], ecx
	}
	if (!strcmp(szVendorId, "GenuineIntel"))
		return 1;
	else if (!strcmp(szVendorId, "AuthenticAMD"))
		return 2;
	else
		return strcmp(szVendorId, "CyrixInstead") ? 0 : 3;
}

static __inline DWORD CpuGetCpuid1(void)
{
	DWORD nCpuid1;

	__asm
	{
		mov eax, 1
		cpuid
		mov nCpuid1, eax
	}
	return nCpuid1;
}

static __inline DWORD CpuGetSpeed(void)
{
	__int64 qwStart, qwEnd;
	DWORD dwTickStart, dwTickEnd;

	__asm
	{
		_emit 15
		_emit 49
		mov dword ptr qwStart[0], eax
		mov dword ptr qwStart[4], edx
	}
	dwTickStart = GetTickCount();
	Sleep(1000);
	__asm
	{
		_emit 15
		_emit 49
		mov dword ptr qwEnd[0], eax
		mov dword ptr qwEnd[4], edx
	}
	dwTickEnd = GetTickCount();
	return (DWORD)(qwEnd - qwStart) / (dwTickEnd - dwTickStart) / 1000;
}

static __inline void CpuGetName(char *pszName, int nVendor, DWORD nFamily, DWORD nModel, DWORD nType)
{
	strcpy(pszName, "{Unknown}");
	switch (nVendor)
	{
	case 1:
		if (nType == 0 && nFamily == 5 && nModel < 4)
			strcpy(pszName, "Pentium");
		else if (nType == 0 && nFamily == 5 && nModel >= 4)
			strcpy(pszName, "Pentium MMX");
		else if (nType == 1 && nFamily == 5 && nModel < 3)
			strcpy(pszName, "Pentium OD586");
		else if (nType == 1 && nFamily == 5 && nModel == 3)
			strcpy(pszName, "Pentium OD486");
		else if (nType == 1 && nFamily == 5 && nModel >= 4)
			strcpy(pszName, "Pentium MMX OD586");
		else if (nType == 0 && nFamily == 6 && nModel < 3)
			strcpy(pszName, "Pentium Pro");
		else if (nType == 0 && nFamily == 6 && nModel < 5)
			strcpy(pszName, "Pentium II");
		else if (nType == 0 && nFamily == 6 && nModel == 5)
			strcpy(pszName, "Pentium II or Celeron");
		else if (nType == 0 && nFamily == 6 && nModel == 6)
			strcpy(pszName, "Celeron");
		else if (nType == 0 && nFamily == 6 && nModel >= 7)
			strcpy(pszName, "Pentium III");
		else if (nType == 1 && nFamily == 6)
			strcpy(pszName, "Pentium II OD");
		break;
	case 2:
		if (nFamily == 4)
			strcpy(pszName, "Am486DX4 or Am5x86");
		else if (nFamily == 5 && nModel < 6)
			strcpy(pszName, "K5");
		else if (nFamily == 5 && nModel < 8)
			strcpy(pszName, "K6");
		else if (nFamily == 5 && nModel == 8)
			strcpy(pszName, "K6-II");
		else if (nFamily == 5 && nModel >= 9)
			strcpy(pszName, "K6-III");
		else if (nFamily == 6)
			strcpy(pszName, "Athlon");
		break;
	case 3:
		if (nType == 0 && nFamily == 4)
			strcpy(pszName, "MediaGX");
		else if (nType == 0 && nFamily == 5 && nModel < 4)
			strcpy(pszName, "6x86");
		else if (nType == 0 && nFamily == 5 && nModel >= 4)
			strcpy(pszName, "GXm");
		else if (nType == 0 && nFamily == 6)
			strcpy(pszName, "6x86MX");
		break;
	}
}

void __fastcall CpuLogInfo(void)
{
	char szName[256];
	DWORD nCpuid1;
	int nVendor;
	DWORD nType, nFamily, nModel, nStepping;

	switch (gtErrorManagerSysInfo.tSystemInfo.wProcessorLevel)
	{
	case 3:
		FogErrorManagerLog("Intel 386");
		return;
	case 4:
		FogErrorManagerLog("Intel 486");
		return;
	}
	nVendor = CpuGetVendor();
	nCpuid1 = CpuGetCpuid1();
	nType = (nCpuid1 >> 12) & 3;
	nFamily = (nCpuid1 >> 8) & 15;
	nModel = (nCpuid1 >> 4) & 15;
	nStepping = nCpuid1 & 15;
	CpuGetName(szName, nVendor, nFamily, nModel, nType);
	FogErrorManagerLog("Vendor:  %s", sgpszCpuVendors[nVendor]);
	FogErrorManagerLog("Version: %s (Type %X, Family %X, Model %X, Stepping %X)", szName, nType, nFamily,
		nModel, nStepping);
	FogErrorManagerLog("Speed: Approx. %d MHz", CpuGetSpeed());
}

BOOL __fastcall CpuIsSlow(void)
{
	DWORD nCpuid1;
	int nVendor;
	DWORD nType, nFamily, nModel;

	if (gtErrorManagerSysInfo.tSystemInfo.wProcessorLevel < 5)
		return TRUE;
	nVendor = CpuGetVendor();
	nCpuid1 = CpuGetCpuid1();
	nType = (nCpuid1 >> 12) & 3;
	nFamily = (nCpuid1 >> 8) & 15;
	nModel = (nCpuid1 >> 4) & 15;
	switch (nVendor)
	{
	case 1:
		if (nType == 0 && nFamily == 5 && nModel < 4)
			return TRUE;
		if (nType == 0 && nFamily == 5 && nModel >= 4)
			return TRUE;
		if (nType == 1 && nFamily == 5 && nModel < 3)
			return TRUE;
		if (nType == 1 && nFamily == 5 && nModel == 3)
			return TRUE;
		if (nType == 1 && nFamily == 5 && nModel >= 4)
			return TRUE;
		if (nType == 0 && nFamily == 6 && nModel < 3)
			return TRUE;
		break;
	case 2:
		if (nFamily == 4)
			return TRUE;
		if (nFamily == 5 && nModel < 6)
			return TRUE;
		if (nFamily == 5 && nModel < 8)
			return TRUE;
		if (nFamily == 5 && nModel == 8)
			return TRUE;
		break;
	case 3:
		return TRUE;
	}
	return FALSE;
}
