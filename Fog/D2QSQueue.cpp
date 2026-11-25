#define D2_THIS_FILE "C:\\D2\\Source\\Fog\\Src\\D2QSQueue.cpp"
#include "FogLocal.h"
#include "D2QSQueue.h"
#include "D2Macros.h"
#include "StormImports.h"

void __fastcall D2QSQueueInit(D2QSQUEUE *ptQueue)
{
	ptQueue->ptHead = NULL;
	ptQueue->ptTail = NULL;
	ptQueue->fOverflow = FALSE;
	InitializeCriticalSection(&ptQueue->tLock);
}

void __fastcall D2QSQueueDestroy(D2QSQUEUE *ptQueue)
{
	D2QSMESSAGE *ptNext;

	EnterCriticalSection(&ptQueue->tLock);
	ptNext = ptQueue->ptHead;
	while (ptQueue->ptHead)
	{
		ptNext = ptNext->ptNext;
#line 40
		D2FREE2(ptQueue->ptHead);
		ptQueue->ptHead = ptNext;
	}
	ptQueue->ptTail = NULL;
	ptQueue->ptHead = NULL;
	DeleteCriticalSection(&ptQueue->tLock);
}

BOOL __fastcall D2QSQueueTestOverflow(D2QSQUEUE *ptQueue)
{
	BOOL fOverflowed;

	EnterCriticalSection(&ptQueue->tLock);
	fOverflowed = ptQueue->fOverflow;
	LeaveCriticalSection(&ptQueue->tLock);
	return fOverflowed;
}

void __fastcall D2QSQueuePush(D2QSQUEUE *ptQueue, const BYTE *pbData, WORD wBytes)
{
	D2QSMESSAGE *ptMessage;

#line 75
	D2ASSERTMSG(wBytes > 0, "Zero sized Message");
	EnterCriticalSection(&ptQueue->tLock);
#line 79
	ptMessage = (D2QSMESSAGE *)D2ALLOC2(wBytes + 8);
	ptMessage->wDataSize = wBytes;
	ptMessage->ptNext = NULL;
	memcpy(ptMessage->abData, pbData, wBytes);
	if (!ptQueue->ptHead)
	{
		ptQueue->ptHead = ptMessage;
		ptQueue->ptTail = ptMessage;
	}
	else
	{
		ptQueue->ptTail->ptNext = ptMessage;
		ptQueue->ptTail = ptMessage;
	}
	LeaveCriticalSection(&ptQueue->tLock);
}

DWORD __fastcall D2QSQueuePop(D2QSQUEUE *ptQueue, BYTE *pbBuffer, WORD wMax)
{
	D2QSMESSAGE *ptMessage;
	DWORD dwBytes;
	D2QSMESSAGE *ptNext;

	EnterCriticalSection(&ptQueue->tLock);
	ptMessage = ptQueue->ptHead;
	if (ptMessage)
	{
		if (ptMessage->wDataSize > wMax)
		{
			ptQueue->fOverflow = TRUE;
			dwBytes = 0;
		}
		else
		{
			memcpy(pbBuffer, ptMessage->abData, ptMessage->wDataSize);
			dwBytes = ptMessage->wDataSize;
		}
		ptNext = ptMessage->ptNext;
#line 121
		D2FREE2(ptQueue->ptHead);
		ptQueue->ptHead = ptNext;
		if (!ptNext)
			ptQueue->ptTail = NULL;
	}
	else
		dwBytes = 0;
	LeaveCriticalSection(&ptQueue->tLock);
	return dwBytes;
}
