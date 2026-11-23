#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Logging\\ProfCore.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"

PROFZONE sgatProfZones[NUM_PROF_ZONES] =
{
	{ "Sample" },
	{ "Server Loop" },
	{ "Server Display" },
	{ "Srv Process All Games" },
	{ "-Srv Process Game" },
	{ "-Srv Update Clients" },
	{ "--Srv Process Units" },
	{ "---Srv Player Do Mode" },
	{ "---Srv Monster Do Mode" },
	{ "----Srv Monster Awake" },
	{ "----Srv Monster Run AI" },
	{ "---Com Stats Calc All Outer" },
	{ "----Com Stats Calc All Inner" },
	{ "----Com Stats Calc All Test" },
	{ "Srv Send All Msgs" },
	{ "-Srv Send Client Packets" },
	{ "Srv Process Msgs" },
	{ "-Process Sys Message" },
	{ "--Srv Join Game" },
	{ "-Process Client Message" },
	{ "--Srv Player Process Msg" },
	{ "Com Find Path" },
	{ "Com Path Step" },
};

#pragma auto_inline(off)
void __fastcall ProfZoneAccumulate(int nZoneIndex, unsigned __int64 qwStart)
{
	LARGE_INTEGER tNow;

	QueryPerformanceCounter(&tNow);
	sgatProfZones[nZoneIndex].qwTime += tNow.QuadPart - (__int64)qwStart;
	sgatProfZones[nZoneIndex].nCount++;
}
#pragma auto_inline(on)

void __fastcall ProfDump(void)
{
	LARGE_INTEGER tFreq;
	int i;

	QueryPerformanceFrequency(&tFreq);
	LoggingWrite(1, 2, 1, 524288, "C:\\D2\\Source\\D2Common\\Logging\\ProfCore.cpp", 78,
		"PROFILING DUMP FREQUENCY %d:\n", (int)tFreq.LowPart, (int)tFreq.HighPart);
	for (i = 0; i < NUM_PROF_ZONES; i++)
	{
		LoggingWrite(1, 2, 1, 524288, "C:\\D2\\Source\\D2Common\\Logging\\ProfCore.cpp", 84,
			"FUNCTION: %30s, TIME %12d, COUNT %5d",
			sgatProfZones[i].szName, (int)sgatProfZones[i].qwTime, sgatProfZones[i].nCount);
	}
}

void __fastcall ProfDumpAndReset(void)
{
	LARGE_INTEGER tFreq;
	int i;

	QueryPerformanceFrequency(&tFreq);
	LoggingWrite(1, 2, 1, 524288, "C:\\D2\\Source\\D2Common\\Logging\\ProfCore.cpp", 78,
		"PROFILING DUMP FREQUENCY %d:\n", (int)tFreq.LowPart, (int)tFreq.HighPart);
	for (i = 0; i < NUM_PROF_ZONES; i++)
	{
		LoggingWrite(1, 2, 1, 524288, "C:\\D2\\Source\\D2Common\\Logging\\ProfCore.cpp", 84,
			"FUNCTION: %30s, TIME %12d, COUNT %5d",
			sgatProfZones[i].szName, (int)sgatProfZones[i].qwTime, sgatProfZones[i].nCount);
	}
	for (i = 0; i < NUM_PROF_ZONES; i++)
	{
		sgatProfZones[i].qwTime = 0;
		sgatProfZones[i].nCount = 0;
	}
}

struct PROFACCUMARGS
{
	LARGE_INTEGER tStart;
	DWORD dwZoneIndex;
	struct PROFACCUMARGS *ProfZoneStart(DWORD dwZoneIndex);
};

struct PROFACCUMARGS *PROFACCUMARGS::ProfZoneStart(DWORD dwZoneIndex)
{
	struct PROFACCUMARGS *ptArgs = this;

	QueryPerformanceCounter(&ptArgs->tStart);
	ptArgs->dwZoneIndex = dwZoneIndex;
	return ptArgs;
}

void __fastcall ProfZoneAccumulateFromStruct(struct PROFACCUMARGS *ptArgs)
{
	ProfZoneAccumulate((int)ptArgs->dwZoneIndex, (unsigned __int64)ptArgs->tStart.QuadPart);
}
