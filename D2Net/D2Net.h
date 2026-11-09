#ifndef D2RE_D2NET_H
#define D2RE_D2NET_H

#include <winsock.h>
#include <windows.h>
#include "D2NetLinkNames.h"

#define MAX_MSG_SIZE	516
#define D2NET_MAGIC		210

struct D2NETHDR
{
 	BYTE		bMagic;
 	BYTE		bQueue;

 	WORD		wSize;
};

struct D2NETPKT
{
 	D2NETHDR	tHdr;
 	BYTE		abData[MAX_MSG_SIZE - 4];
};

struct D2NETMSG
{
 	BYTE		abData[MAX_MSG_SIZE];
 	int			nSize;
 	DWORD		dwQueuedTick;

 	D2NETMSG	*ptNext;
};

extern int gnD2NetMode;
BOOL __cdecl D2NetIsLocal(void);
void __fastcall D2NetSetMode(int nMode);
int __fastcall D2NetDequeue(CRITICAL_SECTION *ptLock, D2NETMSG **pptQueue, BYTE *pbBuffer, DWORD dwSize);
BOOL __fastcall D2NetFlush(CRITICAL_SECTION *ptLock, D2NETMSG **pptQueue);
void __cdecl D2NetLocalAck(void);

void __fastcall ClientLocalReceive(const D2NETPKT *ptPkt);

BOOL __fastcall ServerLocalSend(const D2NETPKT *ptPkt);

#endif
