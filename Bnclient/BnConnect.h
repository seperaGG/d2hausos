#ifndef BNCONNECT_H
#define BNCONNECT_H

typedef char BNNAME[128];
typedef char BNHOSTNAME[256];
typedef char BNSERVERLIST[512];
typedef char *BNADDRLIST[2];
typedef DWORD BNMESSAGEDWORDS[NUM_BNMESSAGES];

extern BNNAME sgszName;
extern BNHOSTNAME sgszGateway;
extern u_long sgdwHostAddress;
extern int sgnStage;
extern HOSTENT sgtHostEnt;
extern BNADDRLIST sgapszHostAddrs;
extern BNRESOLVE sgtResolve;
extern BNSERVERLIST sgszServers;
extern SAFESOCK *sgptConnection;
extern LONG sglPumps;
extern BOOL sgfConnected;
extern BOOL sgfAbort;
extern LONG sglConnectThreads;
extern BNMESSAGEDWORDS sgadwResult;
extern DWORD sgdwCDKeyReplyCode;
extern DWORD sgdwCreateAccountResult;
extern BNMESSAGEDWORDS sgafHandled;
extern BNNAME sgszCDKeyText;
extern BOOL sgfCancelled;
DWORD WINAPI BnConnectPump(LPVOID pArg);
BOOL __fastcall BnConnectSetName(const char *pszName);
BOOL __fastcall BnConnectOnLobbyNotification(BYTE *pbData, DWORD dwLength);
void __fastcall BnConnectSetStage(int nStage);
BOOL __fastcall BnConnectSetCancelled(void);
DWORD __fastcall BnConnectGetCreateAccountResult(void);
BOOL __fastcall BnConnectReset(void);
BOOL __fastcall BnConnectDispatch(BYTE bMessage, BYTE *pbData, DWORD dwLength);

#endif
