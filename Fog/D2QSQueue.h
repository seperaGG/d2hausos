#ifndef D2RE_D2QSQUEUE_H
#define D2RE_D2QSQUEUE_H

#include <windows.h>

struct D2QSMESSAGE
{
 	WORD		wDataSize;
 	BYTE		unobserved_1[2];
 	D2QSMESSAGE	*ptNext;
 	BYTE		abData[1];
};

struct D2QSQUEUE
{
 	BOOL		fOverflow;
 	CRITICAL_SECTION	tLock;
 	D2QSMESSAGE	*ptHead;
 	D2QSMESSAGE	*ptTail;
};

void __fastcall D2QSQueueInit(D2QSQUEUE *ptQueue);
void __fastcall D2QSQueueDestroy(D2QSQUEUE *ptQueue);
BOOL __fastcall D2QSQueueTestOverflow(D2QSQUEUE *ptQueue);
void __fastcall D2QSQueuePush(D2QSQUEUE *ptQueue, const BYTE *pbData, WORD wBytes);
DWORD __fastcall D2QSQueuePop(D2QSQUEUE *ptQueue, BYTE *pbBuffer, WORD wMax);

#endif
