#include "Bnclient.h"
#include "StormImports.h"

#define BNETLOG_FILE		"BnetLog.txt"

class BNLOG
{
public:
	DWORD	m_dwLog;
	BNLOG() { StormSLogCreate(BNETLOG_FILE, 0, &m_dwLog); }
	~BNLOG() { StormSLogClose(m_dwLog); }
};
static BNLOG sgtLog;
static BNLOCK sgtLogLock;
static HINSTANCE sghInstance = 0;

static BNCLIENTINTERFACE sgtInterface =
{
	BnConnectConnect,
	BnConnectLogin,
	BnConnectLoginResult,
	BnConnectDisconnect,
	BnSendChatLine,
	BnConnectGetName,
	BnSendJoinChannel,
	BnSendRequestChannelList,
	BnMessQueuePop,
	BnMessQueueGetError,
	BnDownloadRequestFile,
	BnSendAdvertisementRequest,
	BnSendCreateGame,
	BnSendRequestGameList,
	BnSendProductAndTwoTexts,
	BnSendRequestRealmList,
	BnSendRequestRealmLogon,
	BnDownloadApplyPatch,
	BnDownloadGetProgress,
	BnDownloadPatch,
	BnConnectEnterChat,
	BnConnectLeaveChannel,
	BnConnectZero,
	BnConnectCreateAccount,
	BnConnectCreateAccountResult,
	BnSendRequestCharacterList,
	BnSendCreateCharacter,
	BnSendSelectCharacter,
	BnConnectIsConnected,
	BnSendDwordAndFlag,
	BnDownloadCancel,
	BnSendWriteUserData,
	BnSendReadUserData,
	BattleReadUserData,
	BnConnectGetStage,
	BnclientLogPrintf,
	BnclientLogControl,
	BnSendGetCreateGameResult,
	BnSendBareMessage,
};

BOOL __cdecl BnclientLogControl(int nWhat, int nValue)
{
	BOOL fResult;

	fResult = FALSE;
	sgtLogLock.Enter();
	if (nWhat == 1)
	{
		StormSLogClose(sgtLog.m_dwLog);
		if (&sgtLog.m_dwLog)
			StormSLogCreate(BNETLOG_FILE, 0, &sgtLog.m_dwLog);
		fResult = TRUE;
	}
	else if (nWhat == 2)
		StormSLogAddIndent(sgtLog.m_dwLog, nValue);
	sgtLogLock.Leave();
	return fResult;
}

void __cdecl BnclientLogPrintf(const char *pszFormat, ...)
{
	char szLine[512];
	va_list pArgs;

	sgtLogLock.Enter();
	va_start(pArgs, pszFormat);
	_vsnprintf(szLine, sizeof(szLine), pszFormat, pArgs);
	va_end(pArgs);
	szLine[sizeof(szLine) - 1] = 0;
	StormSLogWriteRaw(sgtLog.m_dwLog, szLine);
	sgtLogLock.Leave();
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		BnMessQueueInit();
	}
	sghInstance = hInstance;
	return TRUE;

}

BNCLIENTINTERFACE * __cdecl QueryInterface(void)
{
	return &sgtInterface;
}
