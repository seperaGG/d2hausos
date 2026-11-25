#ifndef D2RE_QSERVER_H
#define D2RE_QSERVER_H

#include <windows.h>
#include <winsock.h>

#define READ_BUFFER_SIZE	1460
#define MAX_MSG_SIZE		1460
#define QSERVER_CLIENT_HASH	1024
#define QSERVER_IP_HASH		2039
#define QSERVER_BLOCK_TICKS	120

struct QSERVER;

struct QSMSG
{
 	SOCKET		hSocket;
 	BYTE		abData[1456];
 	int			nDataSize;
 	DWORD		dwArrivalTick;
 	QSMSG		*ptNext;
};

struct QSSENDBUF
{
 	BYTE		abData[1460];
 	DWORD		dwDataSize;
 	QSSENDBUF	*ptNext;
};

struct QSCLIENT
{
 	SOCKET		hSocket;
 	DWORD		dwOwner;
 	DWORD		dwAddress;
 	DWORD		dwFloodTick;
 	DWORD		dwFloodCount;
 	CRITICAL_SECTION	tLock;
 	LONG		lRefCount;
 	BYTE		abMsgBuff[READ_BUFFER_SIZE];
 	int			nMsgBuffSize;
 	DWORD		dwBytesRead;
 	OVERLAPPED	tReadOverlapped;
 	OVERLAPPED	tWriteOverlapped;
 	QSSENDBUF	*ptSendQueue;
 	QSCLIENT	*ptNext;
 	BYTE		abLastPacket[READ_BUFFER_SIZE];
};

struct QSADDRESS
{
 	DWORD		dwAddress;
 	int			nNumConnections;
 	QSADDRESS	*ptNext;
};

struct QSBLOCK
{
 	DWORD		dwAddress;
 	DWORD		dwLifeSeconds;
 	DWORD		dwBlockSeconds;
 	QSBLOCK		*ptNext;
};

struct QSRATE
{
 	DWORD		dwAddress;
 	DWORD		dwWindowTick;
 	DWORD		dwNumConnects;
 	QSRATE		*ptNext;
};

typedef int (__fastcall *QSPARSE)(BYTE *pbBuffer, int nBytes, int *pnPacketSize, int *pnHeaderSize,
	int *pnMsgType, int *pnFlood, SOCKET hSocket, DWORD dwOwner);
typedef void (__fastcall *QSSOCKETHOOK)(QSERVER *ptServer, SOCKET hSocket);
typedef void (__fastcall *QSADDRESSHOOK)(DWORD dwAddress);
typedef int (__fastcall *QSLOCALSEND)(QSERVER *ptServer, const BYTE *pbData, int nBytes);

struct QSERVER
{
 	BOOL		fCompletionPort;
 	QSCLIENT	*aptClients[QSERVER_CLIENT_HASH];
 	QSADDRESS	*aptAddresses[QSERVER_IP_HASH];
 	QSRATE		*ptRateHead;
 	QSRATE		*ptRateTail;
 	CRITICAL_SECTION	tLock;
 	int			nMode;
 	QSMSG		**pptMsgHeads;
 	QSMSG		**pptMsgTails;
 	QSMSG		*ptFreeMsgs;
 	int			nNumMsgLists;
 	int			nNumClients;
 	int			nAddressLimit;
 	DWORD		dwLastAgeTick;
 	QSBLOCK		*ptBlocks;
 	SOCKET		hListenSocket;
 	QSADDRESSHOOK	pfnBlockHook;
 	QSADDRESSHOOK	field_302c;
 	QSPARSE		pfnParse;
 	QSSOCKETHOOK	pfnAccept;
 	QSSOCKETHOOK	pfnClose;
 	QSLOCALSEND	pfnLocalSend;
 	BOOL		fBlockAddresses;
 	HANDLE		hMsgEvent;
 	HANDLE		hThread;
 	DWORD		dwThreadId;
 	BOOL		fStopThread;
 	HANDLE		hPort;
 	DWORD		dwLastAcceptTick;
};

void __fastcall QServerAgeBlocks(QSERVER *ptServer);
QSCLIENT * __fastcall QServerFindClient(QSERVER *ptServer, SOCKET hSocket);
QSCLIENT * __fastcall QServerAddClient(QSERVER *ptServer, SOCKET hSocket, DWORD dwAddress);
void __fastcall QServerRemoveClient(QSERVER *ptServer, SOCKET hSocket);
void __fastcall QServerBlockAddress(QSERVER *ptServer, struct in_addr tAddress);
BOOL __fastcall QServerIsBlocked(QSERVER *ptServer, struct in_addr tAddress);
BOOL __fastcall QServerAddressFull(QSERVER *ptServer, DWORD *pdwAddress);
BOOL __fastcall QServerAllowConnect(QSERVER *ptServer, DWORD *pdwAddress);
BOOL __fastcall QServerDropAddress(QSERVER *ptServer, DWORD *pdwAddress);
void __fastcall QServerParse(QSERVER *ptServer, QSCLIENT *ptClient);
void __fastcall QServerClientAddRef(QSCLIENT *ptClient);
void __fastcall QServerClientRelease(QSCLIENT *ptClient);
void __stdcall QServerDisconnect(QSERVER *ptServer, SOCKET hSocket);
BOOL __stdcall QServerIsLocal(QSERVER *ptServer);

BOOL __fastcall QServer98Start(QSERVER *ptServer, u_short nPort);
void __fastcall QServer98Stop(QSERVER *ptServer);
BOOL __fastcall QServer98Send(QSERVER *ptServer, SOCKET hSocket, const BYTE *pbData, int nBytes);

SOCKET __fastcall QServerNTStart(QSERVER *ptServer, u_short nPort);
void __fastcall QServerNTStop(QSERVER *ptServer);
BOOL __fastcall QServerNTSend(QSERVER *ptServer, QSCLIENT *ptClient, const BYTE *pbData, DWORD dwBytes);
BOOL __fastcall QServerNTPump(QSERVER *ptServer, DWORD dwMilliseconds);

#endif
