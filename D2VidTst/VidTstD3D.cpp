#include "D2VidTst.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static VTD3DDEVICEINFO sgatD3DDevices[VIDTST_DISPLAYS] = { 0 };
static VTDDRAW4 *sgpD3DDDraw4 = 0;
static VTD3D *sgpD3D = 0;
static HWND sghD3DWindow = 0;
static int sgnD3DDevices = 0;
static DWORD sgdw418950 = 0;

static DWORD sgadwD3DWidths[2] = { 640, 800 };
static DWORD sgadwD3DHeights[2] = { 480, 600 };
static VTPIXELFORMAT sgt1555Format = {
	32, 65, 0, 16, 31744, 992, 31, 32768
};
static char sgszD3DWindowName[] = "Direct3D Enumeration";

int __fastcall Direct3DDetect(HINSTANCE hInstance)
{
	VIDTSTSYSINFO *ptInfo;
	HRESULT hResult;
	int nResult;

	try
	{
		ptInfo = (VIDTSTSYSINFO *)FogErrorManagerGetSysInfo();
		if (ptInfo->dwOsType == 3 && !(ptInfo->fVersion >= 5.0f))
		{
			FogErrorManagerLog(
				"***** Windows NT versions less than 5.0 don't support Direct3D acceleration!");
			return 0;
		}
		FogErrorManagerLog("Detecting boards which support Direct3D...");
		sgnD3DDevices = 0;
		SetLastError(0);
		sghD3DWindow = CreateWindowExA(WS_EX_APPWINDOW, gszEnumClass, sgszD3DWindowName,
			WS_POPUP | WS_MINIMIZE, 0, 0, 640, 480, NULL, NULL, hInstance, NULL);
		if (!sghD3DWindow)
		{
			FogErrorManagerLog("***** Failed to create window for D3D enumeration= %d", GetLastError());
			return 0;
		}
		hResult = DirectDrawEnumerateA(sEnumerateD3DDevices, NULL);
		if (hResult != 0)
			VidTstLogDDrawError("DirectDrawEnumerate", hResult);
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Caught a failure in Direct3DDetect ** ");
	}
	if (sghD3DWindow)
	{
		DestroyWindow(sghD3DWindow);
		FogErrorManagerLog("Destroyed D3D window.");
	}
	if (!sgnD3DDevices)
	{
		FogErrorManagerLog("Found no valid Direct3D devices to use!");
		return 0;
	}
	try
	{
		if ((float)ptInfo->dwTotalPhysMemory < 66060288.0f)
			nResult = 1;
		else
			nResult = (float)sgdw418950 < 11534336.0f ? 1 : 2;
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Caught a failure in Direct3DDetect ** ");
	}
	return nResult;
}

BOOL __stdcall sEnumerateD3DDevices(GUID *ptGuid, LPSTR pszDescription, LPSTR pszName, LPVOID pContext)
{
	VTDDIDENTIFIER tId;
	VTDDRAW *ptDDraw = NULL;
	VTDDSURFACE *ptPrimary = NULL;
	VTDDSURFACE *ptBack = NULL;
	VTD3DDEVICE *ptDevice = NULL;
	VTDDCAPS tCaps = { sizeof(VTDDCAPS) };
	VTD3DDEVDESC tHal;
	VTD3DDEVDESC tHel;
	VIDTSTSYSINFO *ptInfo;
	VTD3DDEVICEINFO *ptInfoDev;
	VTMODE *ptMode;
	HRESULT hResult;
	DWORD dwVendor;
	DWORD dwDevice;
	DWORD dwFlags;
	int nRecommend;
	int i;
	BOOL f32;
	BOOL f24;
	BOOL fUi;
	BOOL fGame;
	BOOL fFail;

	sgpD3DDDraw4 = NULL;
	sgpD3D = NULL;
	do
	{
		try
		{
			ptInfo = (VIDTSTSYSINFO *)FogErrorManagerGetSysInfo();
			if (sgnD3DDevices >= VIDTST_DISPLAYS)
			{
				FogErrorManagerLog("***** Exceeded Maximum Display Count (%d >= %d)", sgnD3DDevices,
					VIDTST_DISPLAYS);
				return FALSE;
			}
			ptInfoDev = &sgatD3DDevices[sgnD3DDevices];
			memset(ptInfoDev, 0, sizeof(*ptInfoDev));
			if (!ptGuid)
				ptInfoDev->dwFlags |= gdwBitMasks[0];
			else
			{
				ptInfoDev->dwFlags &= gdwInvBitMasks[0];
				memcpy(ptInfoDev->abGuid, ptGuid, sizeof(ptInfoDev->abGuid));
			}
			hResult = DirectDrawCreate(ptGuid, &ptDDraw, NULL);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Enumeration DirectDrawCreate failed", hResult);
				break;
			}
			hResult = ptDDraw->lpVtbl->field_00(ptDDraw, &IID_IDirectDraw4, (void **)&sgpD3DDDraw4);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Enumeration failed to get DirectDraw4 interface", hResult);
				break;
			}
			ptDDraw->lpVtbl->field_08(ptDDraw);
			ptDDraw = NULL;
			hResult = sgpD3DDDraw4->lpVtbl->field_6c(sgpD3DDDraw4, &tId, 0);
			if (hResult == 0)
				strncpy(ptInfoDev->szName, tId.szName, 256);
			else
				strncpy(ptInfoDev->szName, pszName, 256);
			FogErrorManagerLog("\tChecking capabilities of %s...", ptInfoDev->szName);
			if (hResult == 0)
			{
				if (tId.dwVendorId == 32902)
				{
					if (tId.dwDeviceId == 30720)
					{
						FogErrorManagerLog("\t\t%s is based on the Intel i740 chipset and has problems running "
							"Diablo II under Direct3D.", ptInfoDev->szName);
						break;
					}
				}
				else if (tId.dwVendorId == 4115)
				{
					if (tId.dwDeviceId == 214)
					{
						FogErrorManagerLog("\t\t%s is based on the Cirrus Logic 546X chipset and has problems "
							"running Diablo II under Direct3D.", ptInfoDev->szName);
						break;
					}
				}
				else if (tId.dwVendorId == 4153)
				{
					if (tId.dwDeviceId == 25350)
					{
						FogErrorManagerLog("\t\t%s is based on the SiS 530 chipset and has problems running "
							"Diablo II under Direct3D.", ptInfoDev->szName);
						break;
					}
				}
				else if (tId.dwVendorId == 4172)
				{
					if (tId.dwDeviceId == 15623)
					{
						FogErrorManagerLog(
							"\t\t%s is based on the FireGL 1000 Pro chipset and has problems running "
							"Diablo II under Direct3D.", ptInfoDev->szName);
						break;
					}
				}
				else if ((tId.dwVendorId == 4634 && (tId.dwDeviceId == 1 || tId.dwDeviceId == 2
					|| tId.dwDeviceId == 3 || tId.dwDeviceId == 5))
					|| (tId.dwVendorId == 4418 && tId.dwDeviceId == 25661)
					|| (tId.dwVendorId == 4313 && tId.dwDeviceId == 34342))
				{
					FogErrorManagerLog("\t\t%s is based on a 3Dfx chipset and has limited texturing capabilities "
						"under Direct3D.", ptInfoDev->szName);
					break;
				}
				dwVendor = tId.dwVendorId;
				dwDevice = tId.dwDeviceId;
				ptInfoDev->dwVendorId = dwVendor;
				ptInfoDev->dwDeviceId = dwDevice;
				FogErrorManagerLog("\t\tVendorID = 0x%08x, DeviceID = 0x%08x", dwVendor, dwDevice);
			}
			hResult = sgpD3DDDraw4->lpVtbl->field_00(sgpD3DDDraw4, &IID_IDirect3D3, (void **)&sgpD3D);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Failed to get D3D interface", hResult);
				break;
			}
			hResult = sgpD3DDDraw4->lpVtbl->pfnSetCooperativeLevel(sgpD3DDDraw4, sghD3DWindow,
				VIDTST_COOPERATIVE);
			if (hResult != 0)
			{
				VidTstLogDDrawError("SetCooperativeLevel failed", hResult);
				break;
			}
			hResult = sgpD3DDDraw4->lpVtbl->pfnEnumDisplayModes(sgpD3DDDraw4, 0, NULL, ptInfoDev,
				(PFNVTENUMMODES)VidTstEnumD3DModes);
			if (hResult != 0)
			{
				VidTstLogDDrawError("IDirectDraw_EnumDisplayModes", hResult);
				break;
			}
			if (!ptInfoDev->nModes)
			{
				FogErrorManagerLog("\t\tCouldn't find any valid display modes.");
				break;
			}
			f32 = FALSE;
			fUi = FALSE;
			fGame = FALSE;
			f24 = FALSE;
			fFail = FALSE;
			for (i = 0; i < ptInfoDev->nModes; i++)
			{
				dwFlags = ptInfoDev->atModes[i].dwFlags;
				if (dwFlags & gdwBitMasks[2])
					f32 = TRUE;
				if (dwFlags & gdwBitMasks[3])
					f24 = TRUE;
				if (dwFlags & gdwBitMasks[0])
					fUi = TRUE;
				if (dwFlags & gdwBitMasks[1])
					fGame = TRUE;
			}
			if (f32)
			{
				FogErrorManagerLog("\t\tSupports 32 bit cutscenes.");
				ptInfoDev->dwFlags |= gdwBitMasks[7];
			}
			if (f24)
			{
				FogErrorManagerLog("\t\tSupports 24 bit cutscenes.");
				ptInfoDev->dwFlags |= gdwBitMasks[8];
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

			ptMode = &ptInfoDev->atModes[ptInfoDev->nModes - 1];
			hResult = sgpD3DDDraw4->lpVtbl->pfnSetDisplayMode(sgpD3DDDraw4, ptMode->dwWidth, ptMode->dwHeight,
				ptMode->dwBits, ptMode->dwRate, 0);
			if (hResult != 0)
			{
				VidTstLogDDrawError("IDirectDraw4_SetDisplayMode", hResult);
				break;
			}
			VTSURFDESC tDesc = { sizeof(VTSURFDESC) };

			tDesc.field_04 = 33;
			tDesc.field_68 = 25112;
			tDesc.dwNumBackBuffers = 1;
			hResult = sgpD3DDDraw4->lpVtbl->pfnCreateSurface(sgpD3DDDraw4, &tDesc, &ptPrimary, NULL);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Failed to create primary surface", hResult);
				break;
			}
			{
				VTDDSCAPS tBackCaps;

				memset(&tBackCaps, 0, sizeof(tBackCaps));
				tBackCaps.field_00 = 4;
				hResult = ptPrimary->lpVtbl->pfnGetAttachedSurface(ptPrimary, &tBackCaps.field_00, &ptBack);
			}
			if (hResult != 0)
			{
				VidTstLogDDrawError("Failed to get backbuffer surface", hResult);
				break;
			}
			hResult = sgpD3DDDraw4->lpVtbl->field_2c(sgpD3DDDraw4, &tCaps, NULL);
			if (hResult != 0)
			{
				VidTstLogDDrawError("Failed to get DD4 Caps", hResult);
				break;
			}
			if (!(tCaps.field_04 & 1))
			{
				FogErrorManagerLog("\t\tDoesn't appear to support 3D.");
				break;
			}
			if (tCaps.field_08 & 131072)
				ptInfoDev->dwFlags |= gdwBitMasks[4];
			else
				FogErrorManagerLog("\t\tDoesn't support gamma control.");
			if (tCaps.field_08 & 1048576)
			{
				ptInfoDev->dwFlags |= gdwBitMasks[5];
				FogErrorManagerLog("\t\tSupports gamma calibration.");
			}

			sgpD3D->lpVtbl->field_20(sgpD3D, &IID_IDirect3DHALDevice, ptBack, &ptDevice, NULL);
			if (!ptDevice)
			{
				FogErrorManagerLog("\t\tDoesn't appear to have hardware 3D support");
				break;
			}
			tHal.dwSize = sizeof(tHal);
			tHel.dwSize = sizeof(tHel);
			hResult = ptDevice->lpVtbl->field_0c(ptDevice, &tHal, &tHel);
			if (hResult != 0)
			{
				VidTstLogD3DError("Failed to get D3D device caps", hResult);
				break;
			}
			if (!(tHal.dwTexMemFlags & 512))
			{
				FogErrorManagerLog("\t\tDoesn't support textures in video memory!");
				break;
			}
			if (tHal.dwTextureFlags & 256)
				FogErrorManagerLog("\t\tSupports non-power of two textures.");
			if (tHal.field_04 & 64)
			{
				if (!(tHal.dwTextureFlags & 1))
					FogErrorManagerLog("\t\tDoes not support perspective correct textures!");
				if (tHal.dwTextureFlags & 32)
				{
					FogErrorManagerLog("\t\tRequires square textures!");
					ptInfoDev->dwFlags |= gdwBitMasks[2];
					break;
				}
				if (tHal.field_88 & 32)
					ptInfoDev->dwFlags |= gdwBitMasks[3];
				else
					FogErrorManagerLog("\t\tDoesn't support mipmapping.");

				ptInfoDev->dwVidMemTotal = tCaps.dwVidMemTotal;
				ptInfoDev->dwVidMemFree = tCaps.dwVidMemFree;
				FogErrorManagerLog("\t\tVideo Memory Total= %2.3f MB, Free= %2.3f MB",
					ptInfoDev->dwVidMemTotal / VIDTST_MB, tCaps.dwVidMemFree / VIDTST_MB);
				VTDDSCAPS tMemCaps;

				memset(&tMemCaps, 0, sizeof(tMemCaps));
				tMemCaps.field_00 = 20480;
				hResult = sgpD3DDDraw4->lpVtbl->field_5c(sgpD3DDDraw4, &tMemCaps, &ptInfoDev->dwTexMemTotal,
					&ptInfoDev->dwTexMemFree);
				if (hResult != 0)
				{
					VidTstLogDDrawError("Failed to get available texture memory", hResult);
					break;
				}
				FogErrorManagerLog("\t\tTexture Memory Total= %2.3f MB, Free= %2.3f MB",
					ptInfoDev->dwTexMemTotal / VIDTST_MB, ptInfoDev->dwTexMemFree / VIDTST_MB);
				memset(&tMemCaps, 0, sizeof(tMemCaps));
				tMemCaps.field_00 = 268455936;
				hResult = sgpD3DDDraw4->lpVtbl->field_5c(sgpD3DDDraw4, &tMemCaps, &ptInfoDev->dwLocalTotal,
					&ptInfoDev->dwLocalFree);
				if (hResult != 0)
				{
					VidTstLogDDrawError("Failed to get available local texture memory", hResult);
					break;
				}
				FogErrorManagerLog("\t\t\tLocal Total= %2.3f MB, Free= %2.3f MB",
					ptInfoDev->dwLocalTotal / VIDTST_MB, ptInfoDev->dwLocalFree / VIDTST_MB);
				if ((tCaps.field_08 & 512) && (tHal.dwTexMemFlags & 4096))
				{
					ptInfoDev->dwFlags |= gdwBitMasks[1];
					memset(&tMemCaps, 0, sizeof(tMemCaps));
					tMemCaps.field_00 = 536891392;
					hResult = sgpD3DDDraw4->lpVtbl->field_5c(sgpD3DDDraw4, &tMemCaps, &ptInfoDev->dwAgpTotal,
						&ptInfoDev->dwAgpFree);
					if (hResult != 0)
						VidTstLogDDrawError("Failed to get available AGP texture memory", hResult);
					else
						FogErrorManagerLog("\t\t\tAGP Total= %2.3f MB, Free= %2.3f MB",
							ptInfoDev->dwAgpTotal / VIDTST_MB, ptInfoDev->dwAgpFree / VIDTST_MB);
				}
				else
					FogErrorManagerLog("\t\t\tDoes not support texturing from AGP memory.");
				if (ptInfoDev->dwTexMemFree < 6291456)
				{
					FogErrorManagerLog("\t\tDoesn't have enough free texture memory (%.1f MB) to adequately run "
						"Diablo II!", 6.0);
					break;
				}

				hResult = ptDevice->lpVtbl->pfnEnumTextureFormats(ptDevice, (PFNVTENUMTEXTURES)VidTstEnumD3DTextures,
					ptInfoDev);
				if (hResult != 0)
				{
					VidTstLogD3DError("EnumTextureFormats() failed", hResult);
					break;
				}
				if (ptInfoDev->dwFlags & gdwBitMasks[9])
					FogErrorManagerLog("\t\tSupports 1555 textures.");
				else
				{
					FogErrorManagerLog("\t\tDoes not support any of the necessary texture formats!");
					fFail = TRUE;
				}
				if (fFail)
					break;

				if (sgnD3DDevices == 0)
				{
					gatVideoSlots[2].dwFlags = ptInfoDev->dwFlags;
					memcpy(gatVideoSlots[2].abGuid, ptInfoDev->abGuid, sizeof(gatVideoSlots[2].abGuid));
					gatVideoSlots[2].dwVendorId = ptInfoDev->dwVendorId;
					gatVideoSlots[2].dwDeviceId = ptInfoDev->dwDeviceId;
					strcpy(gatVideoSlots[2].szName, ptInfoDev->szName);
					if ((float)ptInfoDev->dwLocalFree < 11534336.0f
						|| (float)ptInfo->dwTotalPhysMemory < 66060288.0f)
						nRecommend = 1;
					else
						nRecommend = 2;
					gatVideoSlots[2].dwRecommend = nRecommend;
					gfD3D0Found = nRecommend;
				}
				else if (sgnD3DDevices == 1)
				{
					gatVideoSlots[3].dwFlags = ptInfoDev->dwFlags;
					memcpy(gatVideoSlots[3].abGuid, ptInfoDev->abGuid, sizeof(gatVideoSlots[3].abGuid));
					gatVideoSlots[3].dwVendorId = ptInfoDev->dwVendorId;
					gatVideoSlots[3].dwDeviceId = ptInfoDev->dwDeviceId;
					strcpy(gatVideoSlots[3].szName, ptInfoDev->szName);
					if ((float)ptInfoDev->dwLocalFree < 11534336.0f
						|| (float)ptInfo->dwTotalPhysMemory < 66060288.0f)
						nRecommend = 1;
					else
						nRecommend = 2;
					gatVideoSlots[3].dwRecommend = nRecommend;
					gfD3D1Found = nRecommend;
				}
				sgnD3DDevices++;
				FogErrorManagerLog("\t\tPassed requirements for Diablo II.");
			}
			else
				FogErrorManagerLog("\t\tDoes not support triangles!");
		}
		catch (...)
		{
			FogErrorManagerLog(" ** Caught a failure in sEnumerateD3DDevices ** ");
			if (sgnD3DDevices == 0)
				gatVideoSlots[2].dwRecommend = 0;
			else if (sgnD3DDevices == 1)
				gatVideoSlots[3].dwRecommend = 0;
		}
	}
	while (0);

	try
	{
		if (ptDevice)
		{
			ptDevice->lpVtbl->field_08(ptDevice);
			ptDevice = NULL;
		}
		if (ptBack)
			ptBack = NULL;
		if (ptPrimary)
		{
			ptPrimary->lpVtbl->field_08(ptPrimary);
			ptPrimary = NULL;
		}
		if (sgpD3D)
		{
			sgpD3D->lpVtbl->field_08(sgpD3D);
			sgpD3D = NULL;
		}
		if (sgpD3DDDraw4)
		{
			sgpD3DDDraw4->lpVtbl->field_08(sgpD3DDDraw4);
			sgpD3DDDraw4 = NULL;
		}
	}
	catch (...)
	{
		FogErrorManagerLog(" ** Couldn't clean up sEnumerateD3DDevices ** ");
	}
	return TRUE;
}

BOOL __stdcall VidTstEnumD3DModes(VTSURFDESC *ptDesc, VTD3DDEVICEINFO *ptDevice)
{
	VTDDSCAPS tCaps;
	VTD3DDEVICE *ptD3DDevice;
	VTDDSURFACE *ptBack;
	VTDDSURFACE *ptPrimary;
	VTMODE *ptMode;
	DWORD dwFlags;
	HRESULT hResult;

	ptPrimary = NULL;
	ptBack = NULL;
	ptD3DDevice = NULL;
	if (ptDevice)
	{
		if (!ptDesc || ptDesc->dwSize != sizeof(VTSURFDESC))
			return FALSE;
		if (ptDevice->nModes >= VIDTST_MODES)
		{
			FogErrorManagerLog("***** Exceeded Maximum Display Mode Count (%d >= %d)", ptDevice->nModes,
				VIDTST_MODES);
			if (ptD3DDevice)
			{
				ptD3DDevice->lpVtbl->field_08(ptD3DDevice);
				ptD3DDevice = NULL;
			}
			if (ptBack)
				ptBack = NULL;
			if (ptPrimary)
				ptPrimary->lpVtbl->field_08(ptPrimary);
			return FALSE;
		}
		if (ptDesc->dwBits == 32 && ptDesc->dwWidth == 640 && ptDesc->dwHeight == 480)
			dwFlags = gdwBitMasks[2];
		else if (ptDesc->dwBits == 24 && ptDesc->dwWidth == 640 && ptDesc->dwHeight == 480)
			dwFlags = gdwBitMasks[3];
		else if (ptDesc->dwBits == 16 && ptDesc->dwWidth == sgadwD3DWidths[0]
			&& ptDesc->dwHeight == sgadwD3DHeights[0])
			dwFlags = gdwBitMasks[1];
		else if (ptDesc->dwBits == 16 && ptDesc->dwWidth == sgadwD3DWidths[1]
			&& ptDesc->dwHeight == sgadwD3DHeights[1])
			dwFlags = gdwBitMasks[0];
		else
			return TRUE;
		for (;;)
		{
			hResult = sgpD3DDDraw4->lpVtbl->pfnSetCooperativeLevel(sgpD3DDDraw4, sghD3DWindow, VIDTST_COOPERATIVE);
			if (hResult != 0 && hResult != (HRESULT)0x88760245)
			{
				VidTstLogDDrawError("SetCooperativeLevel (mode enumeration) failed!", hResult);
				break;
			}
			hResult = sgpD3DDDraw4->lpVtbl->pfnSetDisplayMode(sgpD3DDDraw4, ptDesc->dwWidth, ptDesc->dwHeight,
				ptDesc->dwBits, ptDesc->dwRate, 0);
			if (hResult != 0)
			{
				VidTstLogDDrawError("SetDisplayMode (mode enumeration) failed!", hResult);
				break;
			}
			if (!(gdwBitMasks[0] & dwFlags) && !(gdwBitMasks[1] & dwFlags))
			{
				VTSURFDESC tDesc = { sizeof(VTSURFDESC) };

				tDesc.field_04 = 1;
				tDesc.field_68 = 16896;
				hResult = sgpD3DDDraw4->lpVtbl->pfnCreateSurface(sgpD3DDDraw4, &tDesc, &ptPrimary, NULL);
				if (hResult != 0)
				{
					VidTstLogDDrawError("Failed to create primary surface for cutscenes", hResult);
					break;
				}
			}
			else
			{
				VTSURFDESC tDesc = { sizeof(VTSURFDESC) };

				tDesc.field_04 = 33;
				tDesc.field_68 = 25112;
				tDesc.dwNumBackBuffers = 1;
				hResult = sgpD3DDDraw4->lpVtbl->pfnCreateSurface(sgpD3DDDraw4, &tDesc, &ptPrimary, NULL);
				if (hResult != 0)
				{
					VidTstLogDDrawError("CreateSurface (primary, mode enumeration) failed!", hResult);
					break;
				}
				memset(&tCaps, 0, sizeof(tCaps));
				tCaps.field_00 = 4;
				hResult = ptPrimary->lpVtbl->pfnGetAttachedSurface(ptPrimary, &tCaps.field_00, &ptBack);
				if (hResult != 0)
				{
					VidTstLogDDrawError("GetAttachedSurface (mode enumeration) failed!", hResult);
					break;
				}
				sgpD3D->lpVtbl->field_20(sgpD3D, &IID_IDirect3DHALDevice, ptBack, &ptD3DDevice, NULL);
				if (!ptD3DDevice)
				{
					VidTstLogD3DError("Create 3D HAL Device (mode enumeration) failed!", 0);
					break;
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
		if (ptD3DDevice)
		{
			ptD3DDevice->lpVtbl->field_08(ptD3DDevice);
			ptD3DDevice = NULL;
		}
		if (ptBack)
			ptBack = NULL;
		if (ptPrimary)
			ptPrimary->lpVtbl->field_08(ptPrimary);
	}
	return TRUE;
}

HRESULT __stdcall VidTstEnumD3DTextures(VTPIXELFORMAT *ptFormat, VTD3DDEVICEINFO *ptDevice)
{
	if (!ptDevice)
		return 1;
	if (ptFormat->field_04 == sgt1555Format.field_04 && ptFormat->field_0c == sgt1555Format.field_0c
		&& ptFormat->field_10 == sgt1555Format.field_10 && ptFormat->field_14 == sgt1555Format.field_14
		&& ptFormat->field_18 == sgt1555Format.field_18 && ptFormat->field_1c == sgt1555Format.field_1c)
		ptDevice->dwFlags |= gdwBitMasks[9];
	return 1;
}

void __fastcall VidTstLogD3DError(const char *pszCall, HRESULT hResult)
{
	char szText[256];
	const char *pszName;
	BOOL fDirect3D;

	fDirect3D = TRUE;
	switch (hResult)
	{
	case 0:
		pszName = "D3D_OK";
		break;
	case 0x887602BC:
		pszName = "D3DERR_BADMAJORVERSION";
		break;
	case 0x887602BD:
		pszName = "D3DERR_BADMINORVERSION";
		break;
	case 0x887602C1:
		pszName = "D3DERR_INVALID_DEVICE";
		break;
	case 0x887602C2:
		pszName = "D3DERR_INITFAILED";
		break;
	case 0x887602C3:
		pszName = "D3DERR_DEVICEAGGREGATED";
		break;
	case 0x887602C6:
		pszName = "D3DERR_EXECUTE_CREATE_FAILED";
		break;
	case 0x887602C7:
		pszName = "D3DERR_EXECUTE_DESTROY_FAILED";
		break;
	case 0x887602C8:
		pszName = "D3DERR_EXECUTE_LOCK_FAILED";
		break;
	case 0x887602C9:
		pszName = "D3DERR_EXECUTE_UNLOCK_FAILED";
		break;
	case 0x887602CA:
		pszName = "D3DERR_EXECUTE_LOCKED";
		break;
	case 0x887602CB:
		pszName = "D3DERR_EXECUTE_NOT_LOCKED";
		break;
	case 0x887602CC:
		pszName = "D3DERR_EXECUTE_FAILED";
		break;
	case 0x887602CD:
		pszName = "D3DERR_EXECUTE_CLIPPED_FAILED";
		break;
	case 0x887602D0:
		pszName = "D3DERR_TEXTURE_NO_SUPPORT";
		break;
	case 0x887602D1:
		pszName = "D3DERR_TEXTURE_CREATE_FAILED";
		break;
	case 0x887602D2:
		pszName = "D3DERR_TEXTURE_DESTROY_FAILED";
		break;
	case 0x887602D3:
		pszName = "D3DERR_TEXTURE_LOCK_FAILED";
		break;
	case 0x887602D4:
		pszName = "D3DERR_TEXTURE_UNLOCK_FAILED";
		break;
	case 0x887602D5:
		pszName = "D3DERR_TEXTURE_LOAD_FAILED";
		break;
	case 0x887602D6:
		pszName = "D3DERR_TEXTURE_SWAP_FAILED";
		break;
	case 0x887602D7:
		pszName = "D3DERR_TEXTURE_LOCKED";
		break;
	case 0x887602D8:
		pszName = "D3DERR_TEXTURE_NOT_LOCKED";
		break;
	case 0x887602D9:
		pszName = "D3DERR_TEXTURE_GETSURF_FAILED";
		break;
	case 0x887602DA:
		pszName = "D3DERR_MATRIX_CREATE_FAILED";
		break;
	case 0x887602DB:
		pszName = "D3DERR_MATRIX_DESTROY_FAILED";
		break;
	case 0x887602DC:
		pszName = "D3DERR_MATRIX_SETDATA_FAILED";
		break;
	case 0x887602DD:
		pszName = "D3DERR_MATRIX_GETDATA_FAILED";
		break;
	case 0x887602DE:
		pszName = "D3DERR_SETVIEWPORTDATA_FAILED";
		break;
	case 0x887602DF:
		pszName = "D3DERR_INVALIDCURRENTVIEWPORT";
		break;
	case 0x887602E0:
		pszName = "D3DERR_INVALIDPRIMITIVETYPE";
		break;
	case 0x887602E1:
		pszName = "D3DERR_INVALIDVERTEXTYPE";
		break;
	case 0x887602E2:
		pszName = "D3DERR_TEXTURE_BADSIZE";
		break;
	case 0x887602E3:
		pszName = "D3DERR_INVALIDRAMPTEXTURE";
		break;
	case 0x887602E4:
		pszName = "D3DERR_MATERIAL_CREATE_FAILED";
		break;
	case 0x887602E5:
		pszName = "D3DERR_MATERIAL_DESTROY_FAILED";
		break;
	case 0x887602E6:
		pszName = "D3DERR_MATERIAL_SETDATA_FAILED";
		break;
	case 0x887602E7:
		pszName = "D3DERR_MATERIAL_GETDATA_FAILED";
		break;
	case 0x887602E8:
		pszName = "D3DERR_INVALIDPALETTE";
		break;
	case 0x887602E9:
		pszName = "D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY";
		break;
	case 0x887602EA:
		pszName = "D3DERR_ZBUFF_NEEDS_VIDEOMEMORY";
		break;
	case 0x887602EB:
		pszName = "D3DERR_SURFACENOTINVIDMEM";
		break;
	case 0x887602EE:
		pszName = "D3DERR_LIGHT_SET_FAILED";
		break;
	case 0x887602EF:
		pszName = "D3DERR_LIGHTHASVIEWPORT";
		break;
	case 0x887602F0:
		pszName = "D3DERR_LIGHTNOTINTHISVIEWPORT";
		break;
	case 0x887602F8:
		pszName = "D3DERR_SCENE_IN_SCENE";
		break;
	case 0x887602F9:
		pszName = "D3DERR_SCENE_NOT_IN_SCENE";
		break;
	case 0x887602FA:
		pszName = "D3DERR_SCENE_BEGIN_FAILED";
		break;
	case 0x887602FB:
		pszName = "D3DERR_SCENE_END_FAILED";
		break;
	case 0x88760302:
		pszName = "D3DERR_INBEGIN";
		break;
	case 0x88760303:
		pszName = "D3DERR_NOTINBEGIN";
		break;
	case 0x88760304:
		pszName = "D3DERR_NOVIEWPORTS";
		break;
	case 0x88760305:
		pszName = "D3DERR_VIEWPORTDATANOTSET";
		break;
	case 0x88760306:
		pszName = "D3DERR_VIEWPORTHASNODEVICE";
		break;
	case 0x88760307:
		pszName = "D3DERR_NOCURRENTVIEWPORT";
		break;
	default:
		fDirect3D = FALSE;
		break;
	}
	if (!fDirect3D)
	{
		VidTstLogDDrawError(pszCall, hResult);
		return;
	}
	sprintf(szText, "***** Direct3D error: %s - %s (0x%x)", pszCall, pszName, hResult);
	FogErrorManagerLog(szText);
}
