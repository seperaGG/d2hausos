#define D2_THIS_FILE "C:\\D2\\Source\\D2Net\\SRC\\D2Net.cpp"
#include "D2Net.h"
#include "FogImports.h"
#include "D2Macros.h"

extern const char *const gpszD2NetDllName = "D2NET.DLL";

int gnD2NetMode;
BYTE *gpD2NetServer;
DWORD gdwD2NetLocalValue;

BOOL __cdecl D2NetIsLocal(void)
{
	if (gnD2NetMode == 1)
		return TRUE;
	return gnD2NetMode == 2;
}

BOOL __cdecl D2NetIsLocalHost(void)
{
	return gnD2NetMode == 2;
}

void __fastcall D2NetSetMode(int nMode)
{
	gnD2NetMode = nMode;
}

int __fastcall D2NetDequeue(CRITICAL_SECTION *ptLock, D2NETMSG **pptQueue, BYTE *pbBuffer, DWORD dwSize)
{
	D2NETMSG *ptMsg;
	int nSize;

	EnterCriticalSection(ptLock);
	ptMsg = *pptQueue;
	if (!ptMsg || (gnD2NetMode == 2 && GetTickCount() - ptMsg->dwQueuedTick < 500))
	{
		nSize = -1;
	}
	else
	{
		if (dwSize >= MAX_MSG_SIZE)
			dwSize = MAX_MSG_SIZE;
		memcpy(pbBuffer, ptMsg, dwSize);
		nSize = ptMsg->nSize;
		*pptQueue = ptMsg->ptNext;
#line 67
		D2FREE2(ptMsg);
	}
	LeaveCriticalSection(ptLock);
	return nSize;
}

BOOL __fastcall D2NetFlush(CRITICAL_SECTION *ptLock, D2NETMSG **pptQueue)
{
	D2NETMSG *ptMsg;

	EnterCriticalSection(ptLock);
	while ((ptMsg = *pptQueue) != NULL)
	{
		*pptQueue = ptMsg->ptNext;
#line 90
		D2FREE2(ptMsg);
	}
	LeaveCriticalSection(ptLock);
	return TRUE;
}

int WINAPI D2NetGetLastError(void)
{
	return WSAGetLastError();
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		gnD2NetMode = 0;
		break;
	}
	return TRUE;
}

BOOL __fastcall D2NetServerLocalSend(void *pServer, const D2NETPKT *ptPkt, int nSize)
{
	ClientLocalReceive(ptPkt);
	return TRUE;
}

int __fastcall D2NetServerParse(const BYTE *pbData, int nBytes, int *pnPacket, int *pnHeader, int *pnType,
	int *pnFlood, SOCKET hSocket, DWORD dwOwner)
{
	const D2NETHDR *ptHdr;
	BYTE bMagic;
	BYTE bQueue;
	WORD wSize;

	if ((DWORD)nBytes < sizeof(D2NETHDR))
		return 3;
	ptHdr = (const D2NETHDR *)pbData;
	bMagic = ptHdr->bMagic;
	wSize = ptHdr->wSize;
	if (bMagic != D2NET_MAGIC)
		return 4;
	bQueue = ptHdr->bQueue;
	if (bQueue != 0 && bQueue != 1)
		return 4;
	if (wSize > MAX_MSG_SIZE)
		return 4;
	if ((int)wSize > nBytes)
		return 3;
	*pnHeader = 4;
	*pnPacket = wSize;
	*pnType = ptHdr->bQueue;
	*pnFlood = 100;
	return ptHdr->bQueue ? 1 : 2;
}

BOOL __fastcall D2NetServerAccept(void *pServer, SOCKET hSocket)
{
	D2NETPKT tPkt;

	tPkt.tHdr.bQueue = 0;
	tPkt.abData[0] = 0;
	tPkt.tHdr.bMagic = D2NET_MAGIC;
	tPkt.tHdr.wSize = 5;
	FogQServerSend(gpD2NetServer, hSocket, &tPkt, 5);
	return TRUE;
}

BOOL __fastcall D2NetServerClose(void *pServer, SOCKET hSocket)
{
	D2NETPKT tPkt;

	tPkt.tHdr.bMagic = D2NET_MAGIC;
	tPkt.tHdr.bQueue = 0;
	tPkt.tHdr.wSize = 5;
	tPkt.abData[0] = 9;
	FogQServerInject(gpD2NetServer, &tPkt, 5, hSocket);
	return TRUE;
}

void __cdecl D2NetLocalAck(void)
{
	D2NETPKT tPkt;

	tPkt.tHdr.bMagic = D2NET_MAGIC;
	tPkt.tHdr.bQueue = 0;
	tPkt.tHdr.wSize = 5;
	tPkt.abData[0] = 0;
	FogQServerSend(gpD2NetServer, 0, &tPkt, 5);
}
