#define D2_THIS_FILE "C:\\D2\\Source\\D2Cmp\\Src\\DrwCntxt.cpp"
#include "D2Cmp.h"
#include "D2Macros.h"
#line 8

DWORD WINAPI DrawContextIdentity(DWORD dwValue)
{
	return dwValue;
}

BOOL WINAPI DrawContextIsSet(D2DRAWCONTEXT *ptContext)
{
	if (!ptContext)
		return FALSE;
	if (!ptContext->field_04[0])
		return FALSE;
	if (!ptContext->field_04[1])
		return FALSE;
	return ptContext->field_04[2] != 0;
}

D2DRAWCONTEXT * __fastcall DrawContextCreate(void)
{
	D2DRAWCONTEXT *ptContext;

#line 62
	ptContext = (D2DRAWCONTEXT *)D2ALLOC2(sizeof(D2DRAWCONTEXT));
	ptContext->field_04[0] = 0;
	ptContext->field_04[1] = 0;
	ptContext->field_04[2] = 0;
	ptContext->field_04[4] = 0;
	ptContext->field_04[3] = 0;
	ptContext->field_04[5] = 0;
	ptContext->field_04[6] = 0;
	return ptContext;
}

void WINAPI DrawContextFree(D2DRAWCONTEXT *ptContext)
{
	if (ptContext)
	{
#line 87
		D2FREE2(ptContext);
	}
}
