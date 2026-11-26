#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WinTimer.cpp"
#include "D2Win.h"
#include "D2Macros.h"

static int sgnNextTick;

BOOL __fastcall TimerUpdate(D2WINTIMER *ptTimerData);
BOOL __fastcall TimerUpdatePeriodic(D2WINTIMER *ptTimerData);
static BOOL __stdcall TimerOnKey(_PARAMS *ptMsg);

D2WINTIMER * __fastcall TimerCreate(int nDelay, PFNWINMSG pfnCallback)
{
	D2WINTIMER *ptTimerData;

#line 28
	ptTimerData = (D2WINTIMER *)D2ALLOC2(sizeof(D2WINTIMER));
	memset(ptTimerData, 0, sizeof(D2WINTIMER));
	ptTimerData->eType = D2WIN_TIMER;
	ptTimerData->dwFlags |= gdwBitMasks[0];
	ptTimerData->dwFlags |= gdwBitMasks[2];
	ptTimerData->nInterval = nDelay;
	ptTimerData->dwStartTick = GetTickCount();
	ptTimerData->pfnUpdate = (PFNWINCONTROL)TimerUpdate;
	ptTimerData->ptNextChild = NULL;
	ptTimerData->pfnCallback = pfnCallback;
	ptTimerData->pfnKey = TimerOnKey;
	D2WinMainAddChild(ptTimerData);
	return ptTimerData;
}

DWORD __fastcall D2WinTimerDestroy(D2WINTIMER *ptTimerData)
{
	return D2WinMainFreeChild(ptTimerData);
}

void __fastcall D2WinTimerDisableKey(D2WINTIMER *ptTimerData)
{
#line 59
	D2ASSERT(ptTimerData->eType == D2WIN_TIMER);
	ptTimerData->pfnKey = NULL;
}

void __fastcall D2WinTimerSetPeriodic(D2WINTIMER *ptTimerData)
{
#line 69
	D2ASSERT(ptTimerData->eType == D2WIN_TIMER);
	ptTimerData->pfnUpdate = (PFNWINCONTROL)TimerUpdatePeriodic;
}

BOOL __fastcall TimerUpdate(D2WINTIMER *ptTimerData)
{
	_PARAMS tMsg;
	int nStart;
	int nTick;

#line 82
	D2ASSERT(ptTimerData->eType == D2WIN_TIMER);
	if (!(gdwBitMasks[2] & ptTimerData->dwFlags) || !(gdwBitMasks[0] & ptTimerData->dwFlags))
		return FALSE;
	nTick = (int)GetTickCount();
	nStart = (int)ptTimerData->dwStartTick / 1000;
	if (nTick / 1000 - nStart > ptTimerData->nInterval)
	{
		tMsg.window = ptTimerData;
		if (ptTimerData->pfnCallback)
			ptTimerData->pfnCallback(&tMsg);
	}
	return TRUE;
}

BOOL __fastcall TimerUpdatePeriodic(D2WINTIMER *ptTimerData)
{
	_PARAMS tMsg;
	int nTick;

#line 106
	D2ASSERT(ptTimerData->eType == D2WIN_TIMER);
	if (!(gdwBitMasks[2] & ptTimerData->dwFlags) || !(gdwBitMasks[0] & ptTimerData->dwFlags))
		return FALSE;
	nTick = (int)GetTickCount();
	if (nTick >= sgnNextTick)
	{
		tMsg.window = ptTimerData;
		if (ptTimerData->pfnCallback)
			ptTimerData->pfnCallback(&tMsg);
		sgnNextTick = ptTimerData->nInterval + nTick;
	}
	return TRUE;
}

static BOOL __stdcall TimerOnKey(_PARAMS *ptMsg)
{
	D2WINTIMER *ptTimerData;

	ptTimerData = (D2WINTIMER *)ptMsg->window;
#line 130
	D2ASSERT(ptTimerData->eType == D2WIN_TIMER);
	if (ptMsg->dwMessage == WM_CHAR || ptMsg->dwWParam != VK_SPACE)
		return FALSE;
	if (ptTimerData->pfnCallback)
		ptTimerData->pfnCallback(ptMsg);
	return TRUE;
}
