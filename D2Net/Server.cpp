#define D2_THIS_FILE "C:\\D2\\Source\\D2Net\\SRC\\Server.cpp"
#include "D2Net.h"
#include "FogImports.h"
#include "D2Macros.h"

extern BYTE *gpD2NetServer;
extern DWORD gdwD2NetLocalValue;

BOOL __fastcall D2NetServerLocalSend(void *pServer, const D2NETPKT *ptPkt, int nSize);
int __fastcall D2NetServerParse(const BYTE *pbData, int nBytes, int *pnPacket, int *pnHeader, int *pnType,
	int *pnFlood, SOCKET hSocket, DWORD dwOwner);
BOOL __fastcall D2NetServerAccept(void *pServer, SOCKET hSocket);
BOOL __fastcall D2NetServerClose(void *pServer, SOCKET hSocket);

DWORD WINAPI ServerWait(DWORD dwValue)
{
	return FogQServerWait(gpD2NetServer, dwValue);
}

BYTE * WINAPI ServerInit(int nMode, BOOL fForceSelect)
{
	gpD2NetServer = (BYTE *)(INT_PTR)FogQServerCreate(nMode, 2, 4000, fForceSelect,
		D2NetServerParse,
		D2NetServerAccept, D2NetServerClose,
			D2NetServerLocalSend);
	return gpD2NetServer;
}

DWORD WINAPI ServerSetMaxConnections(DWORD dwCount)
{
	return FogQServerSetAddressLimit(gpD2NetServer, dwCount);
}

DWORD WINAPI ServerSetBlocking(DWORD dwValue)
{
	return FogQServerSetBlocking(gpD2NetServer, dwValue);
}

DWORD __cdecl ServerShutdown(void)
{
	D2NETPKT tPkt;

	tPkt.tHdr.bMagic = D2NET_MAGIC;
	tPkt.tHdr.bQueue = 0;
	tPkt.abData[0] = 1;
	tPkt.tHdr.wSize = 5;
	return FogQServerDestroy(gpD2NetServer, &tPkt, 5);
}

int WINAPI ServerReceiveClientCommand(BYTE *pbBuffer, DWORD dwSize)
{
	return FogQServerGetMessage(gpD2NetServer, 1, pbBuffer, dwSize);
}

int WINAPI ServerReceiveSystemMessage(BYTE *pbBuffer, DWORD dwSize)
{
	return FogQServerGetMessage(gpD2NetServer, 0, pbBuffer, dwSize);
}

int WINAPI ServerNothing(DWORD dwUnused1, DWORD dwUnused2)
{
	return -1;
}

DWORD WINAPI ServerSend(BYTE bQueue, SOCKET hSocket, const BYTE *pbData, int nSize)
{
	D2NETPKT tPkt;

	tPkt.tHdr.bMagic = D2NET_MAGIC;
	tPkt.tHdr.bQueue = bQueue;
	tPkt.tHdr.wSize = (WORD)(nSize + 4);
#line 283
	D2ASSERT(tPkt.tHdr.wSize <= MAX_MSG_SIZE);
	memcpy(tPkt.abData, pbData, nSize);
	return FogQServerSend(gpD2NetServer, hSocket, &tPkt, nSize + 4);
}

DWORD WINAPI ServerGetAddress(SOCKET hSocket, char *pszAddress)
{
	return FogQServerGetAddressString(gpD2NetServer, hSocket, pszAddress);
}

DWORD WINAPI ServerDisconnect(DWORD dwValue)
{
	return FogQServerDisconnect(gpD2NetServer, dwValue);
}

DWORD WINAPI ServerClose(DWORD dwValue)
{
	return FogQServerClose(gpD2NetServer, dwValue);
}

DWORD WINAPI ServerSetBlockCallback(DWORD dwCallback)
{
	return FogQServerSetBlockHook(gpD2NetServer, dwCallback);
}

DWORD WINAPI ServerSetUnusedHook(DWORD dwValue)
{
	return FogQServerSetUnusedHook(gpD2NetServer, dwValue);
}

DWORD WINAPI ServerSetValue(SOCKET hSocket, DWORD dwValue)
{
	if (hSocket)
		return FogQServerSetClientValue(gpD2NetServer, hSocket, dwValue);
	gdwD2NetLocalValue = dwValue;
	return dwValue;
}

DWORD WINAPI ServerGetValue(SOCKET hSocket)
{
	if (hSocket)
		return FogQServerGetClientValue(gpD2NetServer, hSocket);
	return gdwD2NetLocalValue;
}

DWORD __fastcall ServerInject(const D2NETPKT *ptPkt, SOCKET hSocket)
{
	WORD packetSize;

	packetSize = ptPkt->tHdr.wSize;
#line 367
	D2ASSERT(packetSize <= MAX_MSG_SIZE);
	return FogQServerInject(gpD2NetServer, ptPkt, packetSize, hSocket);
}

BOOL __fastcall ServerLocalSend(const D2NETPKT *ptPkt)
{
	WORD packetSize;

	packetSize = ptPkt->tHdr.wSize;
#line 382
	D2ASSERT(packetSize <= MAX_MSG_SIZE);
	return FogQServerInject(gpD2NetServer, ptPkt, packetSize, 0) != 0;
}

DWORD WINAPI ServerService(DWORD dwTimeout)
{
	return FogQServerPump(gpD2NetServer, dwTimeout);
}
