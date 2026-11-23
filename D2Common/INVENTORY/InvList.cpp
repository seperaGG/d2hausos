#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\INVENTORY\\InvList.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"

struct INVLISTNODE
{
 	DWORD			field_00;
 	HUNIT			hItem;
 	DWORD			field_08;
 	DWORD			field_0c;
 	struct INVLISTNODE	*ptNext;
};
typedef struct INVLISTNODE INVLISTNODE, *PTINVLISTNODE;

PTINVLISTNODE __stdcall INVENTORYNodeGetNext(PTINVLISTNODE ptNode)
{
#line 22
	D2ASSERT(ptNode);
	return ptNode->ptNext;
}

HUNIT __stdcall INVENTORYNodeGetKeyA(PTINVLISTNODE ptNode)
{
#line 32
	D2ASSERT(ptNode);
	return ptNode->hItem;
}

DWORD __stdcall INVENTORYNodeGetItemId(PTINVLISTNODE ptNode)
{
#line 42
	D2ASSERT(ptNode);
	return ptNode->field_08;
}

DWORD __stdcall INVENTORYNodeGetCategory(PTINVLISTNODE ptNode)
{
#line 52
	D2ASSERT(ptNode);
	return ptNode->field_0c;
}

DWORD __stdcall INVENTORYNodeSetAcceptedIfWeapon(PTINVLISTNODE ptNode, DWORD dwValue)
{
	DWORD dwValid;

#line 63
	D2ASSERT(ptNode);
	dwValid = ITEMSIsWeapon(ptNode->hItem);
	if (dwValid)
		ptNode->field_00 = dwValue;
	return dwValid;
}

DWORD __stdcall INVENTORYNodeGetAccepted(PTINVLISTNODE ptNode)
{
#line 75
	D2ASSERT(ptNode);
	return ptNode->field_00;
}

PTINVLISTNODE __stdcall INVENTORYListFindNode(void *ptContainer, HUNIT hItem)
{
	PTINVLISTNODE ptNode;

	ptNode = (PTINVLISTNODE)D2CommonINVENTORYGetCategoryListHead(ptContainer);
	while (ptNode != NULL)
	{
		if (ptNode->hItem == hItem)
			return ptNode;
		ptNode = ptNode->ptNext;
	}
	return NULL;
}

HUNIT __stdcall INVENTORYNodeGetKeyB(PTINVLISTNODE ptNode)
{
#line 100
	D2ASSERT(ptNode);
	return ptNode->hItem;
}

DWORD __stdcall INVENTORYNodeGetDeltaItemId(PTINVLISTNODE ptNode)
{
#line 110
	D2ASSERT(ptNode);
	return ptNode->field_00;
}

DWORD __stdcall INVENTORYNodeGetTrackedNext(PTINVLISTNODE ptNode)
{
#line 120
	D2ASSERT(ptNode);
	return ptNode->field_0c;
}

HUNIT __stdcall INVENTORYNodeGetTrackedKey(PTINVLISTNODE ptNode)
{
#line 130
	D2ASSERT(ptNode);
	return ptNode->hItem;
}

DWORD __stdcall INVENTORYNodeGetTrackedData(PTINVLISTNODE ptNode)
{
#line 140
	D2ASSERT(ptNode);
	return ptNode->field_08;
}

DWORD __stdcall INVENTORYNodeGetTrackedFlag(PTINVLISTNODE ptNode)
{
#line 150
	D2ASSERT(ptNode);
	return ptNode->field_00;
}
