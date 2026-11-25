#define D2_THIS_FILE "C:\\D2\\Source\\D2BNClient\\BnMessQueue.cpp"
#include "Bnclient.h"
#include "D2Macros.h"
#include "StormImports.h"

#define BN_MESSAGE_TEXT		288
struct BNMESSAGE
{
 	int			nKind;
 	char		szText[BN_MESSAGE_TEXT];
 	DWORD		dwBytes;
 	BNMESSAGE	*ptNext;
};

static CRITICAL_SECTION sgtQueueLock = { 0 };
static const char *sgpszErrInfo = 0;
static DWORD sgdwErrReason = 0;
BNMESSAGE *gptQueueHead = 0;
static BNMESSAGE *sgptQueueTail = 0;

BOOL __fastcall BnMessQueueInit(void)
{
	gptQueueHead = NULL;
	sgptQueueTail = NULL;
	sgdwErrReason = 0;
	InitializeCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueueAdd(int nKind, const BYTE *pbData, DWORD dwLength)
{
	BNMESSAGE *ptNew;

#line 47
	ptNew = (BNMESSAGE *)D2ALLOC2(sizeof(BNMESSAGE));
	if (dwLength > sizeof(ptNew->szText))
		dwLength = sizeof(ptNew->szText);
	if (dwLength)
		memcpy(ptNew->szText, pbData, dwLength);
	else
		ptNew->szText[0] = 0;
	ptNew->dwBytes = dwLength;
	ptNew->nKind = nKind;
	ptNew->ptNext = NULL;
	D2_OPAQUE(ptNew->dwBytes);
#line 60
	D2ASSERT(ptNew->dwBytes <= arrsize(ptNew->szText));
	EnterCriticalSection(&sgtQueueLock);
	if (!gptQueueHead)
		gptQueueHead = ptNew;
	else
		sgptQueueTail->ptNext = ptNew;
	sgptQueueTail = ptNew;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueueAddText(int nKind, const char *pszText)
{
	BNMESSAGE *ptNew;

#line 84
	ptNew = (BNMESSAGE *)D2ALLOC2(sizeof(BNMESSAGE));
	StormSStrCopy(ptNew->szText, pszText, sizeof(ptNew->szText));
	ptNew->dwBytes = strlen(ptNew->szText) + 1;
	ptNew->nKind = nKind;
	ptNew->ptNext = NULL;
#line 90
	D2ASSERT(ptNew->dwBytes <= arrsize(ptNew->szText));
	EnterCriticalSection(&sgtQueueLock);
	if (!gptQueueHead)
		gptQueueHead = ptNew;
	else
		sgptQueueTail->ptNext = ptNew;
	sgptQueueTail = ptNew;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

DWORD __fastcall BnMessQueuePop(int *pnKind, BYTE *pbData)
{
	BNMESSAGE *ptMessage;
	BNMESSAGE *ptNext;
	DWORD dwBytes;

	EnterCriticalSection(&sgtQueueLock);
	ptMessage = gptQueueHead;
	if (ptMessage)
	{
		memcpy(pbData, ptMessage->szText, ptMessage->dwBytes);
		*pnKind = ptMessage->nKind;
		dwBytes = ptMessage->dwBytes;
		ptNext = ptMessage->ptNext;
#line 126
		D2FREE2(gptQueueHead);
		gptQueueHead = ptNext;
	}
	else
		dwBytes = 0;
	LeaveCriticalSection(&sgtQueueLock);
	return dwBytes;
}

BOOL __fastcall BnMessQueueDrain(void)
{
	BNMESSAGE *ptNext;

	EnterCriticalSection(&sgtQueueLock);
	ptNext = gptQueueHead;
	while (gptQueueHead)
	{
		ptNext = ptNext->ptNext;
#line 151
		D2FREE2(gptQueueHead);
		gptQueueHead = ptNext;
	}
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueueSetReason(DWORD dwReason)
{
	EnterCriticalSection(&sgtQueueLock);
	if (!sgdwErrReason)
		sgdwErrReason = dwReason;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueueForceReason(DWORD dwReason)
{
	EnterCriticalSection(&sgtQueueLock);
	sgdwErrReason = dwReason;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueueSetInfo(const char *pszInfo)
{
	EnterCriticalSection(&sgtQueueLock);
	sgpszErrInfo = pszInfo;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueueGetError(BNERRINFO *errInfo)
{
#line 204
	D2ASSERT(errInfo != NULL);
	EnterCriticalSection(&sgtQueueLock);
	errInfo->dwReason = sgdwErrReason;
	errInfo->pszInfo = sgpszErrInfo;
	sgdwErrReason = 0;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}

BOOL __fastcall BnMessQueuePeekReason(BNERRINFO *errInfo)
{
	EnterCriticalSection(&sgtQueueLock);
	errInfo->dwReason = sgdwErrReason;
	errInfo->pszInfo = sgpszErrInfo;
	LeaveCriticalSection(&sgtQueueLock);
	return TRUE;
}
