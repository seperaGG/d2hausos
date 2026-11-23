#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Chat\\Chat.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"

PTCHATNODE __stdcall ChatNodeNew(void *pPool, const char *pszInput, DWORD dwFrame)
{
	int InputLen;
	int nClampedLen;
	PTCHATNODE hNode;

	InputLen = strlen(pszInput);
#line 38
	D2ASSERT(InputLen != 0);
	nClampedLen = (InputLen >= 255) ? 254 : InputLen;
#line 51
	hNode = (PTCHATNODE)D2ALLOCPOOL(pPool, sizeof(CHATNODE));
	hNode->dwDuration = nClampedLen * 8 + 125;
	hNode->dwEndFrame = hNode->dwDuration + dwFrame;
	hNode->dwTextBox = 0;
	strncpy(hNode->szBuf, pszInput, nClampedLen + 1);
	hNode->szBuf[nClampedLen + 1] = 0;
	return hNode;
}

void __stdcall ChatNodeFree(void *pPool, PTCHATNODE hNode)
{
	if (hNode == NULL)
		return;
#line 68
	D2FREEPOOL(pPool, hNode);
}

DWORD __stdcall ChatNodeGetDuration(PTCHATNODE hNode)
{
	return hNode->dwDuration;
}

DWORD __stdcall ChatNodeGetEndFrame(PTCHATNODE hNode)
{
	return hNode->dwEndFrame;
}

void __stdcall ChatNodeCopyString(PTCHATNODE hNode, char *pszDst)
{
	strcpy(pszDst, hNode->szBuf);
}

DWORD __stdcall ChatNodeGetTextBox(PTCHATNODE hNode)
{
	return hNode->dwTextBox;
}

void __stdcall ChatNodeSetTextBox(PTCHATNODE hNode, DWORD dwValue)
{
	hNode->dwTextBox = dwValue;
}

BYTE __stdcall ChatNodeGetMessageByte(PTCHATNODE hNode)
{
	return hNode->field_08;
}
