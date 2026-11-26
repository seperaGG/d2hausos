#define D2_THIS_FILE "C:\\D2\\Source\\D2Win\\D2WinAnimImage.cpp"
#include "D2Win.h"
#include "D2Macros.h"

static BOOL __fastcall D2WinAnimImageHitTest(D2WINANIMIMAGE *ptAnim);
BOOL __fastcall D2WinAnimImageDraw(D2WINANIMIMAGE *ptAnim);
static BOOL __stdcall D2WinAnimImageMouseDown(_PARAMS *ptMsg);

D2WINANIMIMAGE * __fastcall D2WinAnimImageCreate(int nX, int nY, int nWidth, int nHeight, HCELFILE hCelFile,
	int nSpeed, PFNWINMSG pfnCallback, D2WINANIMSTATE *ptStates, DWORD dwDrawMode, PFNWINMSG pfnHover)
{
	D2WINANIMIMAGE *ptAnim;

#line 28
	ptAnim = (D2WINANIMIMAGE *)D2ALLOC2(sizeof(D2WINANIMIMAGE));
	memset(ptAnim, 0, sizeof(D2WINANIMIMAGE));
	ptAnim->eType = D2WIN_IMAGE;
	ptAnim->dwFlags |= gdwBitMasks[0];
	ptAnim->dwFlags |= gdwBitMasks[2];
	ptAnim->hCelFile = hCelFile;
	ptAnim->nX = nX;
	ptAnim->nY = nY;
	ptAnim->nWidth = nWidth;
	ptAnim->nHeight = nHeight;
	ptAnim->pfnDraw = (PFNWINCONTROL)D2WinAnimImageDraw;
	ptAnim->ptNextChild = 0;
	ptAnim->pfnMouseDown = D2WinAnimImageMouseDown;
	ptAnim->pfnMouseUp = NULL;
	ptAnim->pfnChar = 0;
	ptAnim->pfnKey = NULL;
	ptAnim->pfnCallback = pfnCallback;
	ptAnim->ptStates = ptStates;
	ptAnim->nFrameDelay = nSpeed;
	ptAnim->nFrame = 0;
	ptAnim->dwStartTick = GetTickCount();
	ptAnim->eState = ANIMSTATE_IDLE;
	if (ptStates)
		ptAnim->pfnHitTest = (PFNWINCONTROL)D2WinAnimImageHitTest;
	if (!dwDrawMode)
		ptAnim->dwDrawMode = 5;
	else
		ptAnim->dwDrawMode = dwDrawMode;
	ptAnim->pfnHover = pfnHover;
	ptAnim->fAnimate = TRUE;
	D2WinMainAddChild(ptAnim);
	return ptAnim;
}

DWORD __fastcall D2WinAnimImageDestroy(D2WINANIMIMAGE *ptAnim)
{
	return D2WinMainFreeChild(ptAnim);
}

int __fastcall D2WinAnimImageGetState(D2WINANIMIMAGE *ptAnim)
{
#line 80
	D2ASSERT(ptAnim->eType == D2WIN_IMAGE);
	return ptAnim->eState;
}

BOOL __fastcall D2WinAnimImageDeselect(D2WINANIMIMAGE *ptAnim)
{
#line 92
	D2ASSERT(ptAnim->eType == D2WIN_IMAGE);
	if (ptAnim->eState == ANIMSTATE_SELECTED)
	{
		ptAnim->eState = ANIMSTATE_DESELECTING;
		ptAnim->nFrame = 0;
		ptAnim->dwStartTick = GetTickCount();
		return TRUE;
	}
	if (ptAnim->eState == ANIMSTATE_SELECTING || ptAnim->eState == ANIMSTATE_DESELECTING)
		return FALSE;
	return TRUE;
}

static BOOL __fastcall D2WinAnimImageHitTest(D2WINANIMIMAGE *ptAnim)
{
	D2CELCONTEXT tCelContext;
	HCELFILE hCelFile;
	HCELFILE *phCelFile;
	D2CEL *pCel;
	int nMouseX, nMouseY;
	int nX, nY;
	int nWidth, nHeight;

#line 115
	D2ASSERT(ptAnim->eType == D2WIN_IMAGE);
	nMouseX = sgtWindowData.tMouse.x;
	nMouseY = sgtWindowData.tMouse.y;
	phCelFile = ptAnim->ptStates[ptAnim->eState].phCelFile;
	hCelFile = NULL;
	if (phCelFile)
		hCelFile = *phCelFile;
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.hCelFile = hCelFile;
	tCelContext.dwDirection = 0;
	tCelContext.dwFrame = 0;
	pCel = (D2CEL *)D2CMPCelContextGetCel(&tCelContext);
	nX = ptAnim->nX;
	nY = ptAnim->nY;
	nWidth = (int)D2CMPCelGetWidth(pCel);
	nHeight = (int)D2CMPCelGetHeight(pCel);
	nMouseX -= D2CMPCelGetOffsetX(pCel);
	nMouseY -= D2CMPCelGetOffsetY(pCel);
	if (nMouseX >= nX && nMouseY >= nY - nHeight && nMouseX < nWidth + nX && nMouseY < nY)
		return TRUE;
	return FALSE;
}

BOOL __fastcall D2WinAnimImageDraw(D2WINANIMIMAGE *ptAnim)
{
	D2CELCONTEXT tCelContext;
	_PARAMS tMsg;
	HCELFILE hCelFile, hCelFile2;
	HCELFILE *phCelFile, *phCelFile2;
	D2CEL *pCel;
	DWORD dwDrawMode, dwDrawMode2;
	int nX, nY;
	int eState;
	int ePrev;
	int nFrames;
	int nSpeed;
	int nDelay;
	int nFrame;

#line 249
	D2ASSERT(ptAnim->eType == D2WIN_IMAGE);
	memset(&tCelContext, 0, sizeof(tCelContext));
	tCelContext.dwDirection = 0;
	if (ptAnim->hCelFile || ptAnim->ptStates)
	{
		nX = ptAnim->nX;
		nY = ptAnim->nY;
		if (ptAnim->ptStates)
		{
			eState = ptAnim->eState;
			ePrev = eState;
			if (eState == ANIMSTATE_IDLE || eState == ANIMSTATE_OVER)
			{
				eState = D2WinAnimImageHitTest(ptAnim) != 0;
				ptAnim->eState = eState;
				if (ePrev != eState)
				{
					tMsg.window = ptAnim;
					tMsg.dwLParam = eState == ANIMSTATE_OVER;
					if (ptAnim->pfnHover)
						ptAnim->pfnHover(&tMsg);
				}
			}
			nFrames = 1;
			if (ptAnim->ptStates[ptAnim->eState].phCelFile && *ptAnim->ptStates[ptAnim->eState].phCelFile)
				nFrames = (int)D2CMPCelFileGetNumFrames(*ptAnim->ptStates[ptAnim->eState].phCelFile);
			eState = ptAnim->eState;
			switch (ptAnim->eState)
			{
			case ANIMSTATE_SELECTING:
				if (ptAnim->nFrame >= nFrames - 2)
				{
					ptAnim->nFrame = 0;
					ptAnim->eState = ANIMSTATE_SELECTED;
					ptAnim->dwStartTick = GetTickCount();
				}
				break;
			case ANIMSTATE_DESELECTING:
				if (ptAnim->nFrame >= nFrames - 2)
				{
					ptAnim->nFrame = 0;
					ptAnim->eState = ANIMSTATE_OVER;
					ptAnim->dwStartTick = GetTickCount();
				}
				break;
			}
			if (eState != ptAnim->eState)
			{
				if (ptAnim->ptStates[ptAnim->eState].phCelFile && *ptAnim->ptStates[ptAnim->eState].phCelFile)
					D2CMPCelFileGetNumFrames(*ptAnim->ptStates[ptAnim->eState].phCelFile);
			}
			if (ptAnim->nFrame > nFrames)
			{
				ptAnim->nFrame = 0;
				ptAnim->dwStartTick = GetTickCount();
			}
		}
		hCelFile = NULL;
		hCelFile2 = NULL;
		if (ptAnim->ptStates)
		{
			phCelFile = ptAnim->ptStates[ptAnim->eState].phCelFile;
			phCelFile2 = ptAnim->ptStates[ptAnim->eState].phCelFile2;
			if (phCelFile)
				hCelFile = *phCelFile;
			if (phCelFile2)
				hCelFile2 = *phCelFile2;
			dwDrawMode2 = ptAnim->ptStates[ptAnim->eState].dwDrawMode2;
			dwDrawMode = ptAnim->dwDrawMode;
			nFrames = (int)D2CMPCelFileGetNumFrames(hCelFile);
			nSpeed = ptAnim->ptStates[ptAnim->eState].nFrameDelay;
		}
		else
		{
			hCelFile = ptAnim->hCelFile;
			hCelFile2 = NULL;
			dwDrawMode = ptAnim->dwDrawMode;
			dwDrawMode2 = dwDrawMode;
			nFrames = (int)D2CMPCelFileGetNumFrames(hCelFile);
			nSpeed = ptAnim->nFrameDelay;
			tCelContext.hCelFile = hCelFile;
			tCelContext.dwDirection = 0;
			tCelContext.dwFrame = 0;
			pCel = (D2CEL *)D2CMPCelContextGetCel(&tCelContext);
			nY -= D2CMPCelGetOffsetY(pCel);
			nX -= D2CMPCelGetOffsetX(pCel);
		}
		if (hCelFile)
		{
			tCelContext.dwFrame = ptAnim->nFrame;
			tCelContext.hCelFile = hCelFile;
			D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, dwDrawMode, 0);
		}
		if (hCelFile2)
		{
			tCelContext.hCelFile = hCelFile2;
			tCelContext.dwFrame = ptAnim->nFrame;
			D2gfxScaleDrawCel(&tCelContext, nX, nY, -1, dwDrawMode2, 0);
		}
		if (nSpeed && nFrames > 1 && ptAnim->fAnimate)
		{
			nDelay = nSpeed * 1000 / 25;
			switch (ptAnim->eState)
			{
			case ANIMSTATE_SELECTING:
			case ANIMSTATE_DESELECTING:
				nFrame = (int)((GetTickCount() - ptAnim->dwStartTick) / (DWORD)nDelay);
				ptAnim->nFrame = nFrame;
				if (nFrame >= nFrames - 1)
					nFrame = nFrames - 1;
				ptAnim->nFrame = nFrame;
				break;
			default:
				ptAnim->nFrame = (int)((GetTickCount() - ptAnim->dwStartTick) / (DWORD)nDelay
					% (DWORD)(nFrames - 1));
				break;
			}
		}
	}
	return TRUE;
}

static BOOL __stdcall D2WinAnimImageMouseDown(_PARAMS *ptMsg)
{
	D2WINANIMIMAGE *ptAnim;
	BOOL fResult;

	ptAnim = (D2WINANIMIMAGE *)ptMsg->window;
#line 346
	D2ASSERT(ptAnim->eType == D2WIN_IMAGE);
	fResult = ptAnim->pfnCallback ? ptAnim->pfnCallback(ptMsg) : FALSE;
	if (fResult)
	{
		if (ptAnim->eState == ANIMSTATE_SELECTED)
		{
			ptAnim->eState = ANIMSTATE_DESELECTING;
			ptAnim->nFrame = 0;
			ptAnim->dwStartTick = GetTickCount();
		}
		else if (ptAnim->eState == ANIMSTATE_OVER)
		{
			ptAnim->eState = ANIMSTATE_SELECTING;
			ptAnim->nFrame = 0;
			ptAnim->dwStartTick = GetTickCount();
		}
	}
	return fResult;
}

BOOL __stdcall D2WinAnimImageSetAnimate(D2WINANIMIMAGE *ptAnim, BOOL fAnimate)
{
#line 387
	D2ASSERT(ptAnim->eType == D2WIN_IMAGE);
	ptAnim->fAnimate = fAnimate;
	return TRUE;
}
