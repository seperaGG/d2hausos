#ifndef D2RE_D2VIDTST_H
#define D2RE_D2VIDTST_H

#include <windows.h>

#define VIDTST_TEXTMAX		4096
#define VIDTST_CAPTIONMAX	256
#define VIDTST_NAMEMAX		260
#define VIDTST_SLOTS		5

struct VIDEOSLOT
{
 	DWORD		dwFlags;
 	BYTE		abGuid[16];
 	DWORD		dwVendorId;
 	DWORD		dwDeviceId;
 	char		szName[256];
 	DWORD		dwRecommend;
};

struct RUNNINGTEXTFRAME
{
 	char		szText[VIDTST_TEXTMAX];
 	char		szAdd[VIDTST_TEXTMAX];
};

struct WARNINGFRAME
{
 	char		szCaption[VIDTST_CAPTIONMAX];
 	char		szText[VIDTST_TEXTMAX];
 	char		szSkip[VIDTST_TEXTMAX];
};

struct RESULTFRAME
{
 	char		szCaption[VIDTST_CAPTIONMAX];
 	char		szText[VIDTST_TEXTMAX];
 	char		szMsg[VIDTST_TEXTMAX];
 	char		szSuccess[VIDTST_TEXTMAX];
 	char		szFormat[VIDTST_TEXTMAX];
 	char		szReason[VIDTST_TEXTMAX];
};

struct CRASHFRAME
{
 	char		szCaption[VIDTST_CAPTIONMAX];
 	char		szText[VIDTST_TEXTMAX];
 	char		szMsg[VIDTST_TEXTMAX];
};

struct VTDDSURFACE;
typedef BOOL (__stdcall *PFNVTENUMMODES)(void *ptDesc, void *pContext);

struct VTDDCAPS
{
 	DWORD		dwSize;
 	DWORD		field_04;
 	DWORD		field_08;
 	BYTE		unobserved_00c[48];
 	DWORD		dwVidMemTotal;
 	DWORD		dwVidMemFree;
 	BYTE		unobserved_044[312];
};

struct VTDDIDENTIFIER
{
 	BYTE		unobserved_000[512];
 	char		szName[512];
 	BYTE		unobserved_400[8];
 	DWORD		dwVendorId;
 	DWORD		dwDeviceId;
 	BYTE		unobserved_410[24];
};

struct VTSURFDESC
{
 	DWORD		dwSize;
 	DWORD		field_04;
 	DWORD		dwHeight;
 	DWORD		dwWidth;
 	DWORD		unobserved_010;
 	DWORD		dwNumBackBuffers;
 	DWORD		dwRate;
 	BYTE		unobserved_01c[56];
 	DWORD		dwBits;
 	BYTE		unobserved_058[16];
 	DWORD		field_68;
 	BYTE		unobserved_06c[16];
};
struct VTSURFDESC1
{
 	DWORD		dwSize;
 	DWORD		field_04;
 	DWORD		dwHeight;
 	DWORD		dwWidth;
 	DWORD		unobserved_010;
 	DWORD		dwNumBackBuffers;
 	DWORD		dwRate;
 	BYTE		unobserved_01c[56];
 	DWORD		dwBits;
 	BYTE		unobserved_058[16];
 	DWORD		field_68;
};

struct VTDDSCAPS
{
 	DWORD		field_00;
 	DWORD		unobserved_04[3];
};

struct VTDDRAW;
struct VTDDRAWVTBL
{
 	HRESULT		(__stdcall *field_00)(VTDDRAW *ptThis, const GUID *pIid, void **ppOut);
 	void		*unobserved_004;
 	ULONG		(__stdcall *field_08)(VTDDRAW *ptThis);
 	void		*unobserved_00c[3];
 	HRESULT		(__stdcall *pfnCreateSurface)(VTDDRAW *ptThis, VTSURFDESC1 *ptDesc,
					VTDDSURFACE **pptSurface, void *pUnused);
 	void		*unobserved_01c;
 	HRESULT		(__stdcall *pfnEnumDisplayModes)(VTDDRAW *ptThis, DWORD dwFlags,
					VTSURFDESC1 *ptDesc, void *pContext, PFNVTENUMMODES pfnCallback);
 	void		*unobserved_024[2];
 	HRESULT		(__stdcall *field_2c)(VTDDRAW *ptThis, VTDDCAPS *ptCaps, VTDDCAPS *ptOther);
 	void		*unobserved_030[8];
 	HRESULT		(__stdcall *pfnSetCooperativeLevel)(VTDDRAW *ptThis, HWND hWnd, DWORD dwFlags);
 	HRESULT		(__stdcall *pfnSetDisplayMode)(VTDDRAW *ptThis, DWORD dwWidth, DWORD dwHeight,
					DWORD dwBits);
};
struct VTDDRAW
{
 	VTDDRAWVTBL	*lpVtbl;
};

struct VTDDRAW4;
struct VTDDRAW4VTBL
{
 	HRESULT		(__stdcall *field_00)(VTDDRAW4 *ptThis, const GUID *pIid, void **ppOut);
 	void		*unobserved_004;
 	ULONG		(__stdcall *field_08)(VTDDRAW4 *ptThis);
 	void		*unobserved_00c[3];
 	HRESULT		(__stdcall *pfnCreateSurface)(VTDDRAW4 *ptThis, VTSURFDESC *ptDesc,
					VTDDSURFACE **pptSurface, void *pUnused);
 	void		*unobserved_01c;
 	HRESULT		(__stdcall *pfnEnumDisplayModes)(VTDDRAW4 *ptThis, DWORD dwFlags,
					VTSURFDESC *ptDesc, void *pContext, PFNVTENUMMODES pfnCallback);
 	void		*unobserved_024[2];
 	HRESULT		(__stdcall *field_2c)(VTDDRAW4 *ptThis, VTDDCAPS *ptCaps, VTDDCAPS *ptOther);
 	void		*unobserved_030[8];
 	HRESULT		(__stdcall *pfnSetCooperativeLevel)(VTDDRAW4 *ptThis, HWND hWnd, DWORD dwFlags);
 	HRESULT		(__stdcall *pfnSetDisplayMode)(VTDDRAW4 *ptThis, DWORD dwWidth, DWORD dwHeight,
					DWORD dwBits, DWORD dwRate, DWORD dwFlags);
 	void		*unobserved_058;
 	HRESULT		(__stdcall *field_5c)(VTDDRAW4 *ptThis, VTDDSCAPS *ptCaps, DWORD *pdwTotal,
					DWORD *pdwFree);
 	void		*unobserved_060[3];
 	HRESULT		(__stdcall *field_6c)(VTDDRAW4 *ptThis, VTDDIDENTIFIER *ptId, DWORD dwFlags);
};
struct VTDDRAW4
{
 	VTDDRAW4VTBL	*lpVtbl;
};

struct VTDDSURFACEVTBL
{
 	void		*unobserved_000[2];
 	ULONG		(__stdcall *field_08)(VTDDSURFACE *ptThis);
 	void		*unobserved_00c[9];
 	HRESULT		(__stdcall *pfnGetAttachedSurface)(VTDDSURFACE *ptThis, DWORD *pdwCaps,
					VTDDSURFACE **pptSurface);
};
struct VTDDSURFACE
{
 	VTDDSURFACEVTBL	*lpVtbl;
};

struct VTD3DDEVDESC
{
 	DWORD		dwSize;
 	DWORD		field_04;
 	DWORD		unobserved_008;
 	DWORD		dwTexMemFlags;
 	BYTE		unobserved_010[116];
 	DWORD		dwTextureFlags;
 	DWORD		field_88;
 	BYTE		unobserved_08c[112];
};

struct VTPIXELFORMAT
{
 	DWORD		field_00;
 	DWORD		field_04;
 	DWORD		field_08;
 	DWORD		field_0c;
 	DWORD		field_10;
 	DWORD		field_14;
 	DWORD		field_18;
 	DWORD		field_1c;
};

struct VTD3DDEVICE;
typedef HRESULT (__stdcall *PFNVTENUMTEXTURES)(VTPIXELFORMAT *ptFormat, void *pContext);
struct VTD3DDEVICEVTBL
{
 	void		*unobserved_000[2];
 	ULONG		(__stdcall *field_08)(VTD3DDEVICE *ptThis);
 	HRESULT		(__stdcall *field_0c)(VTD3DDEVICE *ptThis, VTD3DDEVDESC *ptHal,
					VTD3DDEVDESC *ptHel);
 	void		*unobserved_010[4];
 	HRESULT		(__stdcall *pfnEnumTextureFormats)(VTD3DDEVICE *ptThis,
					PFNVTENUMTEXTURES pfnCallback, void *pContext);
};
struct VTD3DDEVICE
{
 	VTD3DDEVICEVTBL	*lpVtbl;
};

struct VTD3D;
struct VTD3DVTBL
{
 	void		*unobserved_000[2];
 	ULONG		(__stdcall *field_08)(VTD3D *ptThis);
 	void		*unobserved_00c[5];
 	HRESULT		(__stdcall *field_20)(VTD3D *ptThis, const GUID *pIid, VTDDSURFACE *ptSurface,
					VTD3DDEVICE **pptDevice, void *pUnused);
};
struct VTD3D
{
 	VTD3DVTBL	*lpVtbl;
};

struct VTMODE
{
 	DWORD		dwFlags;
 	DWORD		dwBits;
 	DWORD		dwWidth;
 	DWORD		dwHeight;
 	DWORD		dwRate;
};

#define VIDTST_DISPLAYS		2
#define VIDTST_MODES		32

struct VTDDRAWDEVICE
{
 	DWORD		dwFlags;
 	BYTE		abGuid[16];
 	char		szName[256];
 	DWORD		dwVidMemTotal;
 	DWORD		dwVidMemFree;
 	VTMODE		atModes[VIDTST_MODES];
 	int			nModes;
};

struct VTD3DDEVICEINFO
{
 	DWORD		dwFlags;
 	BYTE		abGuid[16];
 	DWORD		dwVendorId;
 	DWORD		dwDeviceId;
 	char		szName[256];
 	DWORD		dwVidMemTotal;
 	DWORD		dwVidMemFree;
 	DWORD		dwTexMemTotal;
 	DWORD		dwTexMemFree;
 	DWORD		dwLocalTotal;
 	DWORD		dwLocalFree;
 	DWORD		dwAgpTotal;
 	DWORD		dwAgpFree;
 	VTMODE		atModes[VIDTST_MODES];
 	int			nModes;
};

extern "C" DWORD __fastcall FogOrdinal10017(DWORD, DWORD);
extern "C" void * __fastcall FogOrdinal10020(void);
extern "C" DWORD __cdecl FogOrdinal10026(...);
#define FogErrorManagerInit(pszAppName, pfnCallback) \
	FogOrdinal10017((DWORD)(pszAppName), (DWORD)(pfnCallback))
#define FogErrorManagerGetSysInfo() FogOrdinal10020()
#define FogErrorManagerLog FogOrdinal10026

extern "C" HRESULT __stdcall DirectDrawEnumerateA(BOOL (__stdcall *pfnCallback)(GUID *, LPSTR, LPSTR, LPVOID),
	LPVOID pContext);
extern "C" HRESULT __stdcall DirectDrawCreate(GUID *ptGuid, VTDDRAW **pptDDraw, void *pUnused);

extern "C" __declspec(dllimport) DWORD gdwBitMasks[];
extern "C" __declspec(dllimport) DWORD gdwInvBitMasks[];

#define VIDTST_MB			1048576.0f
#define VIDTST_COOPERATIVE	17

extern "C" const GUID IID_IDirect3DHALDevice;
extern "C" const GUID IID_IDirect3D3;
extern "C" const GUID IID_IDirectDraw4;

#define TEST_RUNNING		1
#define TEST_DONE			2
#define RECOMMEND_FOUND		2
#define RENDER_DDRAW		0
#define RENDER_D3D			1
#define RENDER_GLIDE		3
#define DXSTATE_OK			0
#define DXSTATE_OLD			1
#define DXSTATE_UNKNOWN		2

struct VIDTSTSYSINFO
{
 	BOOL		unobserved_00;
 	DWORD		dwOsType;
 	float		fVersion;
 	DWORD		dwTotalPhysMemory;
};

extern char gszEnumClass[];
extern HINSTANCE ghInstance;
extern HWND ghRunningDlg;
extern VIDEOSLOT gatVideoSlots[VIDTST_SLOTS];
extern DWORD gfGlideFound;
extern DWORD gfD3D0Found;
extern DWORD gfD3D1Found;
extern DWORD gfDDraw0Found;
extern DWORD gfDDraw1Found;

BOOL __fastcall VidTstOsDetect(HINSTANCE hInstance);
void __fastcall VidTstWriteRender(DWORD dwRender, int nSlot);
void __fastcall VidTstWriteSlot(int nSlot);
BOOL __cdecl VidTstReadConfig(void);
BOOL CALLBACK VidTstRunningDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI VidTstRunningThread(LPVOID pParam);
void __fastcall VidTstAppendRunningText(UINT uId);
BOOL CALLBACK VidTstWarningDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VidTstResultDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VidTstCrashDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int __fastcall Direct3DDetect(HINSTANCE hInstance);
BOOL __stdcall sEnumerateD3DDevices(GUID *ptGuid, LPSTR pszDescription, LPSTR pszName, LPVOID pContext);
BOOL __stdcall VidTstEnumD3DModes(VTSURFDESC *ptDesc, VTD3DDEVICEINFO *ptDevice);
HRESULT __stdcall VidTstEnumD3DTextures(VTPIXELFORMAT *ptFormat, VTD3DDEVICEINFO *ptDevice);
void __fastcall VidTstLogD3DError(const char *pszCall, HRESULT hResult);
BOOL __fastcall DDrawDetect(HINSTANCE hInstance);
BOOL __stdcall sEnumerateDDrawDevices(GUID *ptGuid, LPSTR pszDescription, LPSTR pszName, LPVOID pContext);
BOOL __stdcall VidTstEnumDDrawModes(VTSURFDESC1 *ptDesc, VTDDRAWDEVICE *ptDevice);
void __fastcall VidTstLogDDrawError(const char *pszCall, HRESULT hResult);
int __fastcall GlideDetect(HINSTANCE hInstance);
BOOL __cdecl FaultErrorBox(DWORD dwError, ...);

#endif
