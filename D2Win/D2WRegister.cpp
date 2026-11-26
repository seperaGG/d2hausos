#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\Src\\D2WRegister.cpp"
#include "D2Win.h"
#include "D2Macros.h"

void __fastcall D2WinRegisterMessage(HWND hWnd, DWORD eType, DWORD id, void *pfnCallback)
{
#line 19
	D2ASSERT(hWnd);
	switch (eType)
	{
	case D2WINMSG_MESSAGE:
#line 23
		D2ASSERT(id != WM_COMMAND);
		D2ASSERT(id != WM_KEYUP);
		D2ASSERT(id != WM_KEYDOWN);
		StormSMsgRegisterMessage(hWnd, id, pfnCallback);
		break;
	case D2WINMSG_COMMAND:
		StormSMsgRegisterCommand(hWnd, id, pfnCallback);
		break;
	case D2WINMSG_KEYUP:
		StormSMsgRegisterKeyUp(hWnd, id, pfnCallback);
		break;
	case D2WINMSG_KEYDOWN:
		StormSMsgRegisterKeyDown(hWnd, id, pfnCallback);
		break;
	}
}

void __fastcall D2WinUnregisterMessage(HWND hWnd, DWORD eType, DWORD id, void *pfnCallback)
{
#line 48
	D2ASSERT(hWnd);
	switch (eType)
	{
	case D2WINMSG_MESSAGE:
#line 52
		D2ASSERT(id != WM_COMMAND);
		D2ASSERT(id != WM_KEYUP);
		D2ASSERT(id != WM_KEYDOWN);
		StormSMsgUnregisterMessage(hWnd, id, pfnCallback);
		break;
	case D2WINMSG_COMMAND:
		StormSMsgUnregisterCommand(hWnd, id, pfnCallback);
		break;
	case D2WINMSG_KEYUP:
		StormSMsgUnregisterKeyUp(hWnd, id, pfnCallback);
		break;
	case D2WINMSG_KEYDOWN:
		StormSMsgUnregisterKeyDown(hWnd, id, pfnCallback);
		break;
	}
}

void __fastcall D2WinRegisterMessages(HWND hWnd, D2WINMSGENTRY *ptTable, DWORD dwCount)
{
	DWORD i;

#line 77
	D2ASSERT(hWnd);
	for (i = 0; i < dwCount; i++)
		D2WinRegisterMessage(hWnd, ptTable[i].eType, ptTable[i].id, ptTable[i].pfnCallback);
}

void __fastcall D2WinUnregisterMessages(HWND hWnd, D2WINMSGENTRY *ptTable, DWORD dwCount)
{
	DWORD i;

#line 90
	D2ASSERT(hWnd);
	for (i = 0; i < dwCount; i++)
		D2WinUnregisterMessage(hWnd, ptTable[i].eType, ptTable[i].id, ptTable[i].pfnCallback);
}
