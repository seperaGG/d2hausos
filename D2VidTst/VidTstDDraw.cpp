#include "D2VidTst.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static VTDDRAWDEVICE sgatDDrawDevices[VIDTST_DISPLAYS] = { 0 };
static BOOL sgfDirectX3Only = 0;
static VTDDRAW *sgpDDraw = 0;
static VTDDRAW4 *sgpDDraw4 = 0;
static HWND sghDDrawWindow = 0;
static DWORD sgdwDDrawUnread = 0;
static int sgnDDrawDevices = 0;

static DWORD sgadwDDrawWidths[2] = { 640, 800 };
static DWORD sgadwDDrawHeights[2] = { 480, 600 };
static char sgszDDrawWindowName[] = "DirectDraw Enumeration";

BOOL __fastcall DDrawDetect(HINSTANCE hInstance)
{
	VIDTSTSYSINFO *ptInfo;
	HRESULT hResult;

	try
	{
		ptInfo = (VIDTSTSYSINFO *)FogErrorManagerGetSysInfo();
		if (ptInfo->dwOsType == 3 && !(ptInfo->fVersion >= 5.0f))
			sgfDirectX3Only = TRUE;
		if (sgfDirectX3Only)
			FogErrorManagerLog("Restricted to DirectX 3 support.");
		FogErrorManagerLog("Detecting boards which support DirectDraw...");
		sgnDDrawDevices = 0;
		SetLastError(0);
		sghDDrawWindow = CreateWindowExA(WS_EX_APPWINDOW, gszEnumClass, sgszDDrawWindowName,
			WS_POPUP | WS_MINIMIZE, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);
		if (!sghDDrawWindow)
		{
			FogErrorManagerLog("***** Failed to create window for DirectDraw enumeration= %d",
				GetLastError());
			return FALSE;
		}
		hResult = DirectDrawEnumerateA(sEnumerateDDrawDevices, NULL);
		if (hResult != 0)
			VidTstLogDDrawError("DirectDrawEnumerate", hResult);
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Caught a failure in DDrawDetect ** ");
	}
	if (sghDDrawWindow)
	{
		DestroyWindow(sghDDrawWindow);
		FogErrorManagerLog("Destroyed DDraw window.");
	}
	if (!sgnDDrawDevices)
	{
		FogErrorManagerLog("Found no valid DirectDraw devices to use!");
		return FALSE;
	}
	try
	{
		return TRUE;
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Caught a failure in DDrawDetect ** ");
	}
	return FALSE;
}

BOOL __stdcall sEnumerateDDrawDevices(GUID *ptGuid, LPSTR pszDescription, LPSTR pszName, LPVOID pContext)
{
	VTDDIDENTIFIER tId;
	VTDDCAPS tCaps = { sizeof(VTDDCAPS) };
	VTDDSURFACE *ptSurface;
	VTDDRAWDEVICE *ptInfoDev;
	HRESULT hResult;
	DWORD dwFlags;
	int i;
	BOOL f32;
	BOOL f24;
	BOOL f16;
	BOOL fUi;
	BOOL fGame;
	BOOL fFail;

	ptSurface = NULL;
	sgpDDraw = NULL;
	sgpDDraw4 = NULL;
	do
	{
		try
		{
			if (sgnDDrawDevices >= VIDTST_DISPLAYS)
			{
				FogErrorManagerLog("***** Exceeded Maximum Display Count (%d >= %d)", sgnDDrawDevices,
					VIDTST_DISPLAYS);
				return FALSE;
			}
			ptInfoDev = &sgatDDrawDevices[sgnDDrawDevices];
			memset(ptInfoDev, 0, sizeof(*ptInfoDev));
			if (!ptGuid)
				ptInfoDev->dwFlags |= gdwBitMasks[0];
			else
			{
				ptInfoDev->dwFlags &= gdwInvBitMasks[0];
				memcpy(ptInfoDev->abGuid, ptGuid, sizeof(ptInfoDev->abGuid));
			}
			hResult = DirectDrawCreate(ptGuid, &sgpDDraw, NULL);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Enumeration DirectDrawCreate failed", hResult);
				break;
			}
			if (!sgfDirectX3Only)
			{
				hResult = sgpDDraw->lpVtbl->field_00(sgpDDraw, &IID_IDirectDraw4, (void **)&sgpDDraw4);
				if (hResult != 0)
				{
					VidTstLogDDrawError("Enumeration failed to get DirectDraw4 interface", hResult);
					break;
				}
				hResult = sgpDDraw4->lpVtbl->field_6c(sgpDDraw4, &tId, 0);
				if (hResult == 0)
					strncpy(ptInfoDev->szName, tId.szName, 256);
				else
					strncpy(ptInfoDev->szName, pszName, 256);
				FogErrorManagerLog("\tChecking capabilities of %s...", ptInfoDev->szName);
			}
			else
			{
				strncpy(ptInfoDev->szName, pszName, 256);
				FogErrorManagerLog("\tChecking capabilities of %s...", ptInfoDev->szName);
			}
			hResult = sgpDDraw->lpVtbl->pfnSetCooperativeLevel(sgpDDraw, sghDDrawWindow, VIDTST_COOPERATIVE);
			if (hResult != 0)
			{
				VidTstLogDDrawError("SetCooperativeLevel failed", hResult);
				break;
			}
			hResult = sgpDDraw->lpVtbl->pfnEnumDisplayModes(sgpDDraw, 0, NULL, ptInfoDev,
				(PFNVTENUMMODES)VidTstEnumDDrawModes);
			if (hResult != 0)
			{
				VidTstLogDDrawError("EnumDisplayModes", hResult);
				break;
			}
			if (!ptInfoDev->nModes)
			{
				FogErrorManagerLog("\t\tCouldn't find any valid display modes.");
				break;
			}
			f32 = FALSE;
			f24 = FALSE;
			f16 = FALSE;
			fUi = FALSE;
			fGame = FALSE;
			fFail = FALSE;
			for (i = 0; i < ptInfoDev->nModes; i++)
			{
				dwFlags = ptInfoDev->atModes[i].dwFlags;
				if (dwFlags & gdwBitMasks[0])
					f32 = TRUE;
				if (dwFlags & gdwBitMasks[1])
					f24 = TRUE;
				if (dwFlags & gdwBitMasks[2])
					f16 = TRUE;
				if (dwFlags & gdwBitMasks[3])
					fUi = TRUE;
				if (dwFlags & gdwBitMasks[4])
					fGame = TRUE;
			}
			if (f32)
			{
				FogErrorManagerLog("\t\tSupports 32 bit cutscenes.");
				ptInfoDev->dwFlags |= gdwBitMasks[1];
			}
			else if (f24)
			{
				FogErrorManagerLog("\t\tSupports 24 bit cutscenes.");
				ptInfoDev->dwFlags |= gdwBitMasks[2];
			}
			else if (!f16)
			{
				FogErrorManagerLog("\t\tCouldn't find valid display mode for cutscenes.");
				fFail = TRUE;
			}
			if (!fUi)
			{
				FogErrorManagerLog("\t\tCouldn't find valid display mode for user interface screens.");
				fFail = TRUE;
			}
			if (!fGame)
			{
				FogErrorManagerLog("\t\tCouldn't find valid display mode for game screens.");
				break;
			}
			if (fFail)
				break;
			hResult = sgpDDraw->lpVtbl->field_2c(sgpDDraw, &tCaps, NULL);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Failed to get DD Caps", hResult);
				break;
			}
			if (!sgfDirectX3Only)
			{
				if (tCaps.field_08 & 131072)
					ptInfoDev->dwFlags |= 8;
				if (tCaps.field_08 & 1048576)
					ptInfoDev->dwFlags |= 16;
			}
			ptInfoDev->dwVidMemTotal = tCaps.dwVidMemTotal;
			ptInfoDev->dwVidMemFree = tCaps.dwVidMemFree;
			FogErrorManagerLog("\t\tVideo Memory Total= %2.3f MB, Free= %2.3f MB",
				ptInfoDev->dwVidMemTotal / VIDTST_MB, tCaps.dwVidMemFree / VIDTST_MB);

			if (sgnDDrawDevices == 0)
			{
				gatVideoSlots[0].dwFlags = ptInfoDev->dwFlags;
				memcpy(gatVideoSlots[0].abGuid, ptInfoDev->abGuid, sizeof(gatVideoSlots[0].abGuid));
				strcpy(gatVideoSlots[0].szName, ptInfoDev->szName);
				gatVideoSlots[0].dwRecommend = RECOMMEND_FOUND;
				gfDDraw0Found = RECOMMEND_FOUND;
			}
			else if (sgnDDrawDevices == 1)
			{
				gatVideoSlots[1].dwFlags = ptInfoDev->dwFlags;
				memcpy(gatVideoSlots[1].abGuid, ptInfoDev->abGuid, sizeof(gatVideoSlots[1].abGuid));
				strcpy(gatVideoSlots[1].szName, ptInfoDev->szName);
				gatVideoSlots[1].dwRecommend = RECOMMEND_FOUND;
				gfDDraw1Found = RECOMMEND_FOUND;
			}
			sgnDDrawDevices++;
			FogErrorManagerLog("\t\tPassed requirements for Diablo II.");
		}
		catch (...)
		{
			FogErrorManagerLog(" ** Caught a failure in sEnumerateDDrawDevices ** ");
			if (sgnDDrawDevices == 0)
				gatVideoSlots[0].dwRecommend = 0;
			else if (sgnDDrawDevices == 1)
				gatVideoSlots[1].dwRecommend = 0;
		}
	}
	while (0);

	try
	{
		if (ptSurface)
			ptSurface->lpVtbl->field_08(ptSurface);
		if (sgpDDraw4)
		{
			sgpDDraw4->lpVtbl->field_08(sgpDDraw4);
			sgpDDraw4 = NULL;
		}
		if (sgpDDraw)
		{
			sgpDDraw->lpVtbl->field_08(sgpDDraw);
			sgpDDraw = NULL;
		}
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Couldn't clean up sEnumerateDDrawDevices ** ");
	}
	return TRUE;
}

BOOL __stdcall VidTstEnumDDrawModes(VTSURFDESC1 *ptDesc, VTDDRAWDEVICE *ptDevice)
{
	DWORD dwCaps;
	VTDDSURFACE *ptBack;
	VTDDSURFACE *ptPrimary;
	VTMODE *ptMode;
	DWORD dwFlags;
	HRESULT hResult;

	ptPrimary = NULL;
	ptBack = NULL;
	if (ptDevice)
	{
		if (!ptDesc || ptDesc->dwSize != sizeof(VTSURFDESC1))
			goto stop;
		if (ptDevice->nModes >= VIDTST_MODES)
		{
			FogErrorManagerLog("***** Exceeded Maximum Display Mode Count (%d >= %d)", ptDevice->nModes,
				VIDTST_MODES);
			goto stop;
		}
		if (ptDesc->dwBits == 32 && ptDesc->dwWidth == 640 && ptDesc->dwHeight == 480)
			dwFlags = gdwBitMasks[0];
		else if (ptDesc->dwBits == 24 && ptDesc->dwWidth == 640 && ptDesc->dwHeight == 480)
			dwFlags = gdwBitMasks[1];
		else if (ptDesc->dwBits == 16 && ptDesc->dwWidth == 640 && ptDesc->dwHeight == 480)
			dwFlags = gdwBitMasks[2];
		else if (ptDesc->dwBits == 8 && ptDesc->dwWidth == sgadwDDrawWidths[0]
			&& ptDesc->dwHeight == sgadwDDrawHeights[0])
			dwFlags = gdwBitMasks[4];
		else if (ptDesc->dwBits == 8 && ptDesc->dwWidth == sgadwDDrawWidths[1]
			&& ptDesc->dwHeight == sgadwDDrawHeights[1])
			dwFlags = gdwBitMasks[3];
		else
			return TRUE;
		for (;;)
		{
			hResult = sgpDDraw->lpVtbl->pfnSetCooperativeLevel(sgpDDraw, sghDDrawWindow, VIDTST_COOPERATIVE);
			if (hResult != 0 && hResult != (HRESULT)0x88760245)
			{
				VidTstLogDDrawError("SetCooperativeLevel (mode enumeration) failed!", hResult);
				break;
			}
			hResult = sgpDDraw->lpVtbl->pfnSetDisplayMode(sgpDDraw, ptDesc->dwWidth, ptDesc->dwHeight,
				ptDesc->dwBits);
			if (hResult != 0)
			{
				VidTstLogDDrawError("SetDisplayMode (mode enumeration) failed!", hResult);
				break;
			}
			if (!(gdwBitMasks[3] & dwFlags) && !(gdwBitMasks[4] & dwFlags))
			{
				VTSURFDESC1 tDesc = { sizeof(VTSURFDESC1) };

				tDesc.field_04 = 1;
				tDesc.field_68 = 16896;
				hResult = sgpDDraw->lpVtbl->pfnCreateSurface(sgpDDraw, &tDesc, &ptPrimary, NULL);
				if (hResult != 0)
				{
					VidTstLogDDrawError("Failed to create primary surface for cutscenes", hResult);
					break;
				}
			}
			else
			{
				VTSURFDESC1 tDesc;

				memset(&tDesc, 0, sizeof(tDesc));
				tDesc.dwSize = sizeof(tDesc);
				tDesc.field_04 = 33;
				tDesc.field_68 = 16920;
				tDesc.dwNumBackBuffers = 2;
				hResult = sgpDDraw->lpVtbl->pfnCreateSurface(sgpDDraw, &tDesc, &ptPrimary, NULL);
				if (hResult != 0)
				{
					FogErrorManagerLog("Failed to open window for triple buffering.  Trying double buffered...");
					memset(&tDesc, 0, sizeof(tDesc));
					tDesc.dwSize = sizeof(tDesc);
					tDesc.field_04 = 33;
					tDesc.field_68 = 16920;
					tDesc.dwNumBackBuffers = 1;
					hResult = sgpDDraw->lpVtbl->pfnCreateSurface(sgpDDraw, &tDesc, &ptPrimary, NULL);
				}
				if (hResult != 0)
				{
					FogErrorManagerLog("Failed to open window for double buffering.  Trying single buffered...");
					memset(&tDesc, 0, sizeof(tDesc));
					tDesc.dwSize = sizeof(tDesc);
					tDesc.field_04 = 1;
					tDesc.field_68 = 16896;
					hResult = sgpDDraw->lpVtbl->pfnCreateSurface(sgpDDraw, &tDesc, &ptPrimary, NULL);
					if (hResult != 0)
					{
						VidTstLogDDrawError("CreateSurface (primary single buffered) failed!", hResult);
						break;
					}
				}
				else
				{
					dwCaps = 4;
					hResult = ptPrimary->lpVtbl->pfnGetAttachedSurface(ptPrimary, &dwCaps, &ptBack);
					if (hResult != 0)
					{
						VidTstLogDDrawError("Failed to get back buffer (mode enumeration)!", hResult);
						break;
					}
				}
			}
			ptMode = &ptDevice->atModes[ptDevice->nModes];
			ptMode->dwFlags = dwFlags;
			ptMode->dwWidth = ptDesc->dwWidth;
			ptMode->dwHeight = ptDesc->dwHeight;
			ptMode->dwBits = ptDesc->dwBits;
			ptMode->dwRate = ptDesc->dwRate;
			ptDevice->nModes++;
			break;
		}
		if (ptBack)
			ptBack = NULL;
		if (ptPrimary)
			ptPrimary->lpVtbl->field_08(ptPrimary);
	}
	return TRUE;

stop:
	if (ptBack)
		ptBack = NULL;
	if (ptPrimary)
		ptPrimary->lpVtbl->field_08(ptPrimary);
	return FALSE;
}

void __fastcall VidTstLogDDrawError(const char *pszCall, HRESULT hResult)
{
	char szText[256];
	const char *pszName;

	switch (hResult)
	{
	case 0:
		pszName = "DD_OK";
		break;
	case 0x80004001:
		pszName = "DDERR_UNSUPPORTED";
		break;
	case 0x80004005:
		pszName = "DDERR_GENERIC";
		break;
	case 0x800401F0:
		pszName = "DDERR_NOTINITIALIZED";
		break;
	case 0x8007000E:
		pszName = "DDERR_OUTOFMEMORY";
		break;
	case 0x80070057:
		pszName = "DDERR_INVALIDPARAMS";
		break;
	case 0x88760005:
		pszName = "DDERR_ALREADYINITIALIZED";
		break;
	case 0x8876000A:
		pszName = "DDERR_CANNOTATTACHSURFACE";
		break;
	case 0x88760014:
		pszName = "DDERR_CANNOTDETACHSURFACE";
		break;
	case 0x88760028:
		pszName = "DDERR_CURRENTLYNOTAVAIL";
		break;
	case 0x88760037:
		pszName = "DDERR_EXCEPTION";
		break;
	case 0x8876005A:
		pszName = "DDERR_HEIGHTALIGN";
		break;
	case 0x8876005F:
		pszName = "DDERR_INCOMPATIBLEPRIMARY";
		break;
	case 0x88760064:
		pszName = "DDERR_INVALIDCAPS";
		break;
	case 0x8876006E:
		pszName = "DDERR_INVALIDCLIPLIST";
		break;
	case 0x88760078:
		pszName = "DDERR_INVALIDMODE";
		break;
	case 0x88760082:
		pszName = "DDERR_INVALIDOBJECT";
		break;
	case 0x88760091:
		pszName = "DDERR_INVALIDPIXELFORMAT";
		break;
	case 0x88760096:
		pszName = "DDERR_INVALIDRECT";
		break;
	case 0x887600A0:
		pszName = "DDERR_LOCKEDSURFACES";
		break;
	case 0x887600AA:
		pszName = "DDERR_NO3D";
		break;
	case 0x887600B4:
		pszName = "DDERR_NOALPHAHW";
		break;
	case 0x887600CD:
		pszName = "DDERR_NOCLIPLIST";
		break;
	case 0x887600D2:
		pszName = "DDERR_NOCOLORCONVHW";
		break;
	case 0x887600D4:
		pszName = "DDERR_NOCOOPERATIVELEVELSET";
		break;
	case 0x887600D7:
		pszName = "DDERR_NOCOLORKEY";
		break;
	case 0x887600DC:
		pszName = "DDERR_NOCOLORKEYHW";
		break;
	case 0x887600DE:
		pszName = "DDERR_NODIRECTDRAWSUPPORT";
		break;
	case 0x887600E1:
		pszName = "DDERR_NOEXCLUSIVEMODE";
		break;
	case 0x887600E6:
		pszName = "DDERR_NOFLIPHW";
		break;
	case 0x887600F0:
		pszName = "DDERR_NOGDI";
		break;
	case 0x887600FA:
		pszName = "DDERR_NOMIRRORHW";
		break;
	case 0x887600FF:
		pszName = "DDERR_NOTFOUND";
		break;
	case 0x88760104:
		pszName = "DDERR_NOOVERLAYHW";
		break;
	case 0x88760118:
		pszName = "DDERR_NORASTEROPHW";
		break;
	case 0x88760122:
		pszName = "DDERR_NOROTATIONHW";
		break;
	case 0x88760136:
		pszName = "DDERR_NOSTRETCHHW";
		break;
	case 0x8876013C:
		pszName = "DDERR_NOT4BITCOLOR";
		break;
	case 0x8876013D:
		pszName = "DDERR_NOT4BITCOLORINDEX";
		break;
	case 0x88760140:
		pszName = "DDERR_NOT8BITCOLOR";
		break;
	case 0x8876014A:
		pszName = "DDERR_NOTEXTUREHW";
		break;
	case 0x8876014F:
		pszName = "DDERR_NOVSYNCHW";
		break;
	case 0x88760154:
		pszName = "DDERR_NOZBUFFERHW";
		break;
	case 0x8876015E:
		pszName = "DDERR_NOZOVERLAYHW";
		break;
	case 0x88760168:
		pszName = "DDERR_OUTOFCAPS";
		break;
	case 0x8876017C:
		pszName = "DDERR_OUTOFVIDEOMEMORY";
		break;
	case 0x8876017E:
		pszName = "DDERR_OVERLAYCANTCLIP";
		break;
	case 0x88760180:
		pszName = "DDERR_OVERLAYCOLORKEYONLYONEACTIVE";
		break;
	case 0x88760183:
		pszName = "DDERR_PALETTEBUSY";
		break;
	case 0x88760190:
		pszName = "DDERR_COLORKEYNOTSET";
		break;
	case 0x8876019A:
		pszName = "DDERR_SURFACEALREADYATTACHED";
		break;
	case 0x887601A4:
		pszName = "DDERR_SURFACEALREADYDEPENDENT";
		break;
	case 0x887601AE:
		pszName = "DDERR_SURFACEBUSY";
		break;
	case 0x887601B3:
		pszName = "DDERR_CANTLOCKSURFACE";
		break;
	case 0x887601B8:
		pszName = "DDERR_SURFACEISOBSCURED";
		break;
	case 0x887601C2:
		pszName = "DDERR_SURFACELOST";
		break;
	case 0x887601CC:
		pszName = "DDERR_SURFACENOTATTACHED";
		break;
	case 0x887601D6:
		pszName = "DDERR_TOOBIGHEIGHT";
		break;
	case 0x887601E0:
		pszName = "DDERR_TOOBIGSIZE";
		break;
	case 0x887601EA:
		pszName = "DDERR_TOOBIGWIDTH";
		break;
	case 0x887601FE:
		pszName = "DDERR_UNSUPPORTEDFORMAT";
		break;
	case 0x88760208:
		pszName = "DDERR_UNSUPPORTEDMASK";
		break;
	case 0x88760219:
		pszName = "DDERR_VERTICALBLANKINPROGRESS";
		break;
	case 0x8876021C:
		pszName = "DDERR_WASSTILLDRAWING";
		break;
	case 0x88760230:
		pszName = "DDERR_XALIGN";
		break;
	case 0x88760231:
		pszName = "DDERR_INVALIDDIRECTDRAWGUID";
		break;
	case 0x88760232:
		pszName = "DDERR_DIRECTDRAWALREADYCREATED";
		break;
	case 0x88760233:
		pszName = "DDERR_NODIRECTDRAWHW";
		break;
	case 0x88760234:
		pszName = "DDERR_PRIMARYSURFACEALREADYEXISTS";
		break;
	case 0x88760235:
		pszName = "DDERR_NOEMULATION";
		break;
	case 0x88760236:
		pszName = "DDERR_REGIONTOOSMALL";
		break;
	case 0x88760237:
		pszName = "DDERR_CLIPPERISUSINGHWND";
		break;
	case 0x88760238:
		pszName = "DDERR_NOCLIPPERATTACHED";
		break;
	case 0x88760239:
		pszName = "DDERR_NOHWND";
		break;
	case 0x8876023A:
		pszName = "DDERR_HWNDSUBCLASSED";
		break;
	case 0x8876023B:
		pszName = "DDERR_HWNDALREADYSET";
		break;
	case 0x8876023C:
		pszName = "DDERR_NOPALETTEATTACHED";
		break;
	case 0x8876023D:
		pszName = "DDERR_NOPALETTEHW";
		break;
	case 0x8876023E:
		pszName = "DDERR_BLTFASTCANTCLIP";
		break;
	case 0x8876023F:
		pszName = "DDERR_NOBLTHW";
		break;
	case 0x88760240:
		pszName = "DDERR_NODDROPSHW";
		break;
	case 0x88760241:
		pszName = "DDERR_OVERLAYNOTVISIBLE";
		break;
	case 0x88760242:
		pszName = "DDERR_NOOVERLAYDEST";
		break;
	case 0x88760243:
		pszName = "DDERR_INVALIDPOSITION";
		break;
	case 0x88760244:
		pszName = "DDERR_NOTAOVERLAYSURFACE";
		break;
	case 0x88760245:
		pszName = "DDERR_EXCLUSIVEMODEALREADYSET";
		break;
	case 0x88760246:
		pszName = "DDERR_NOTFLIPPABLE";
		break;
	case 0x88760247:
		pszName = "DDERR_CANTDUPLICATE";
		break;
	case 0x88760248:
		pszName = "DDERR_NOTLOCKED";
		break;
	case 0x88760249:
		pszName = "DDERR_CANTCREATEDC";
		break;
	case 0x8876024A:
		pszName = "DDERR_NODC";
		break;
	case 0x8876024B:
		pszName = "DDERR_WRONGMODE";
		break;
	case 0x8876024C:
		pszName = "DDERR_IMPLICITLYCREATED";
		break;
	case 0x8876024D:
		pszName = "DDERR_NOTPALETTIZED";
		break;
	case 0x8876024E:
		pszName = "DDERR_UNSUPPORTEDMODE";
		break;
	case 0x8876024F:
		pszName = "DDERR_NOMIPMAPHW";
		break;
	case 0x88760250:
		pszName = "DDERR_INVALIDSURFACETYPE";
		break;
	case 0x88760258:
		pszName = "DDERR_NOOPTIMIZEHW";
		break;
	case 0x88760259:
		pszName = "DDERR_NOTLOADED";
		break;
	case 0x8876026C:
		pszName = "DDERR_DCALREADYCREATED";
		break;
	case 0x88760276:
		pszName = "DDERR_NONONLOCALVIDMEM";
		break;
	case 0x88760280:
		pszName = "DDERR_CANTPAGELOCK";
		break;
	case 0x88760294:
		pszName = "DDERR_CANTPAGEUNLOCK";
		break;
	case 0x887602A8:
		pszName = "DDERR_NOTPAGELOCKED";
		break;
	case 0x887602B2:
		pszName = "DDERR_MOREDATA";
		break;
	case 0x887602B7:
		pszName = "DDERR_VIDEONOTACTIVE";
		break;
	case 0x887602BB:
		pszName = "DDERR_DEVICEDOESNTOWNSURFACE";
		break;
	default:
		pszName = "< unknown >";
		break;
	}
	sprintf(szText, "***** DirectDraw error: %s - %s (0x%x)", pszCall, pszName, hResult);
	FogErrorManagerLog(szText);
}
