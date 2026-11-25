#include "Bnclient.h"
#include "StormImports.h"
#include "BnConnect.h"

BNNAME sgszName;

BOOL __fastcall BnConnectSetName(const char *pszName)
{
	StormSStrCopy(sgszName, pszName, sizeof(sgszName));
	return TRUE;
}

char * __fastcall BnConnectGetName(void)
{
	return sgszName;
}

BOOL __fastcall BnConnectOnLobbyNotification(BYTE *pbData, DWORD dwLength)
{
	BnMessQueueAdd(0, pbData, dwLength);
	return TRUE;
}

void __cdecl operator delete(void *pv)
{
	if (pv)
		StormSMemFree(pv, "delete", -1, 0);
}

void * __cdecl operator new(unsigned int dwSize)
{
	return StormSMemAlloc(dwSize, "new", -1, 0);
}
