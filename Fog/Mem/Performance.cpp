#include <windows.h>
#include "../FogLinkNames.h"

typedef struct PERFORMANCERECORD PERFORMANCERECORD;

struct PERFORMANCERECORD
{
 	DWORD			adwUnknown0[2];
 	LARGE_INTEGER	tAccumulated;
 	DWORD			adwUnknown16[2];
};

extern "C" DWORD PERFORMANCE_FREQUENCY_TICKS_PER_MILLISEC;
extern "C" DWORD PERFORMANCE_FREQUENCY_TICKS_PER_MICROSEC;
extern "C" PERFORMANCERECORD PERFORMANCE_ACCUMULATOR[4096];
extern "C" DWORD PERFORMANCE_ACCUMULATOR_INDEX;
static DWORD sgdwPerformanceResetTick = 0;
DWORD PERFORMANCE_FREQUENCY_TICKS_PER_MILLISEC = 0;
DWORD PERFORMANCE_FREQUENCY_TICKS_PER_MICROSEC = 0;
PERFORMANCERECORD PERFORMANCE_ACCUMULATOR[4096] = { 0 };
DWORD PERFORMANCE_ACCUMULATOR_INDEX = 0;

void __fastcall MemPerformanceInit(void)
{
	LARGE_INTEGER tFrequency;
	int i;

	QueryPerformanceFrequency(&tFrequency);
	PERFORMANCE_FREQUENCY_TICKS_PER_MILLISEC = (DWORD)(tFrequency.QuadPart / 1000);
	PERFORMANCE_FREQUENCY_TICKS_PER_MICROSEC = (DWORD)(tFrequency.QuadPart / 1000000);
	for (i = 0; i < 24576; i++)
		((DWORD *)PERFORMANCE_ACCUMULATOR)[i] = 0;
	sgdwPerformanceResetTick = GetTickCount();
}

void __fastcall MemPerformanceReset(void)
{
	DWORD *pdwRecord;
	DWORD i;

	for (i = 0; i < PERFORMANCE_ACCUMULATOR_INDEX; i++)
	{
		pdwRecord = &((DWORD *)PERFORMANCE_ACCUMULATOR)[i * 6 + 2];
		pdwRecord[0] = 0;
		pdwRecord[1] = 0;
	}
	sgdwPerformanceResetTick = GetTickCount();
}

DWORD __fastcall MemGetTickCount(void)
{
	return GetTickCount();
}
