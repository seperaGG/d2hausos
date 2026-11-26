#ifndef D2RE_D2WIN_TYPES_H
#define D2RE_D2WIN_TYPES_H

#include <windows.h>
#include "D2WinLinkNames.h"
#include "D2WinImports.h"

struct Unicode
{
	WORD wChar;
	__declspec(dllimport) Unicode(WORD wCh = 0) { wChar = wCh; }
	operator WORD() const                           { return wChar; }
	__declspec(dllimport) static int __fastcall strlen(const Unicode *pwszStr);
	__declspec(dllimport) static Unicode * __fastcall strcpy(Unicode *pwszDest, const Unicode *pwszSrc);
	__declspec(dllimport) static Unicode * __fastcall strcat(Unicode *pwszDest, const Unicode *pwszSrc);
	__declspec(dllimport) static Unicode * __fastcall strncpy(Unicode *pwszDest, const Unicode *pwszSrc, int nCount);
	__declspec(dllimport) static int __fastcall strnicmp(const Unicode *pwszA, const Unicode *pwszB, unsigned int nCount);
	__declspec(dllimport) static Unicode * __fastcall toUnicode(Unicode *pwszDest, const char *pszSrc, int nCount);
	__declspec(dllimport) static char * __fastcall unicode2Win(char *pszDest, const Unicode *pwszSrc, int nCount);
	__declspec(dllimport) static Unicode * __fastcall win2Unicode(Unicode *pwszDest, const char *pszSrc, int nCount);
	__declspec(dllimport) static void __cdecl sprintf(int nSize, Unicode *pwszDest, const Unicode *pwszFormat, ...);
	static int strlen(const WORD *pwsz)             { return D2LangUnicodestrlen(pwsz); }
	static WORD *strcpy(WORD *pwszDest, const WORD *pwszSrc)
		{ return (WORD *)D2LangUnicodestrcpy(pwszDest, pwszSrc); }
	static WORD *strcat(WORD *pwszDest, const WORD *pwszSrc)
		{ return (WORD *)D2LangUnicodestrcat(pwszDest, pwszSrc); }
	static WORD *strncpy(WORD *pwszDest, const WORD *pwszSrc, int nCount)
		{ return (WORD *)D2LangUnicodestrncpy(pwszDest, pwszSrc, nCount); }
	static int strnicmp(const WORD *pwszA, const WORD *pwszB, DWORD dwCount)
		{ return D2LangUnicodestrnicmp(pwszA, pwszB, dwCount); }
	static const WORD *strstr(const WORD *pwszStr, const WORD *pwszSub)
		{ return (const WORD *)D2LangUnicodestrstr(pwszStr, pwszSub); }
	static WORD *toUnicode(WORD *pwszDest, const char *pszSrc, int nCount)
		{ return (WORD *)D2LangUnicodetoUnicode(pwszDest, pszSrc, nCount); }
	static char *unicode2Win(char *pszDest, const WORD *pwszSrc, int nCount)
		{ return D2LangUnicodeunicode2Win(pszDest, pwszSrc, nCount); }
	static WORD *win2Unicode(WORD *pwszDest, const char *pszSrc, int nCount)
		{ return (WORD *)D2LangUnicodewin2Unicode(pwszDest, pszSrc, nCount); }
	static char *toUtf(char *pszDest, const WORD *pwszSrc, int nCount)
		{ return D2LangUnicodetoUtf(pszDest, pwszSrc, nCount); }
	static unsigned int unicodenwidth(const char *pszSrc, int nBytes)
		{ return D2LangUnicodeunicodenwidth(pszSrc, nBytes); }
	static WORD *strcpy(WORD *pwszDest, const Unicode *pwszSrc)
		{ return (WORD *)D2LangUnicodestrcpy(pwszDest, pwszSrc); }
	static Unicode *strcpy(Unicode *pwszDest, const WORD *pwszSrc)
		{ return (Unicode *)D2LangUnicodestrcpy(pwszDest, pwszSrc); }
	static WORD *strcat(WORD *pwszDest, const Unicode *pwszSrc)
		{ return (WORD *)D2LangUnicodestrcat(pwszDest, pwszSrc); }
	static Unicode *strcat(Unicode *pwszDest, const WORD *pwszSrc)
		{ return (Unicode *)D2LangUnicodestrcat(pwszDest, pwszSrc); }
	static WORD *strncpy(WORD *pwszDest, const Unicode *pwszSrc, int nCount)
		{ return (WORD *)D2LangUnicodestrncpy(pwszDest, pwszSrc, nCount); }
	static Unicode *strncpy(Unicode *pwszDest, const WORD *pwszSrc, int nCount)
		{ return (Unicode *)D2LangUnicodestrncpy(pwszDest, pwszSrc, nCount); }
	static int strnicmp(const WORD *pwszA, const Unicode *pwszB, DWORD dwCount)
		{ return D2LangUnicodestrnicmp(pwszA, pwszB, dwCount); }
	static int strnicmp(const Unicode *pwszA, const WORD *pwszB, DWORD dwCount)
		{ return D2LangUnicodestrnicmp(pwszA, pwszB, dwCount); }
	static const WORD *strstr(const WORD *pwszStr, const Unicode *pwszSub)
		{ return (const WORD *)D2LangUnicodestrstr(pwszStr, pwszSub); }
	static const Unicode *strstr(const Unicode *pwszStr, const WORD *pwszSub)
		{ return (const Unicode *)D2LangUnicodestrstr(pwszStr, pwszSub); }
	static const Unicode *strstr(const Unicode *pwszStr, const Unicode *pwszSub)
		{ return (const Unicode *)D2LangUnicodestrstr(pwszStr, pwszSub); }
	static char *toUtf(char *pszDest, const Unicode *pwszSrc, int nCount)
		{ return D2LangUnicodetoUtf(pszDest, pwszSrc, nCount); }
};

typedef struct HCELFILE__ *HCELFILE;
typedef struct D2CEL D2CEL;

struct D2UNITINFO
{
 	DWORD		dwUnitToken;
 	DWORD		dwComponentToken;
 	DWORD		dwVariantToken;
 	DWORD		dwModeToken;
 	DWORD		dwWClassToken;
 	const char	*pszFileName;
};

struct D2CELCONTEXT
{
 	D2CEL		*hCel;
 	HCELFILE	hCelFile;
 	DWORD		dwFrame;
 	DWORD		dwDirection;
 	DWORD		field_10;
 	DWORD		dwCelsPerDirection;
 	DWORD		dwFlags;
 	BYTE		bUnitFlags;
 	BYTE		bComponent;
 	BYTE		unobserved_1e[2];
 	DWORD		dwUnitType;
 	DWORD		dwClassId;
 	DWORD		dwMode;
 	DWORD		dwOverlayClass;
 	D2UNITINFO	tUnitInfo;
};

enum
{
	D2WIN_EDITBOX = 1,
	D2WIN_IMAGE = 2,
	D2WIN_ANIMIMAGE = 3,
	D2WIN_TEXTBOX = 4,
	D2WIN_SCROLLBAR = 5,
	D2WIN_BUTTON = 6,
	D2WIN_LIST = 7,
	D2WIN_TIMER = 8,
	D2WIN_SMACK = 9,
	D2WIN_PROGRESSBAR = 10,
	D2WIN_POPUP = 11,
	D2WIN_ACCOUNTLIST = 12
};

typedef struct D2WINCONTROL D2WINCONTROL;
typedef struct _PARAMS _PARAMS;

struct _PARAMS
{
 	D2WINCONTROL	*window;
 	DWORD		dwMessage;
 	DWORD		dwWParam;
 	DWORD		dwLParam;
 	DWORD		unobserved_10[2];
 	DWORD		dwHandled;
 	DWORD		dwResult;
};

typedef BOOL (__stdcall *PFNWINMSG)(_PARAMS *ptMsg);
typedef BOOL (__fastcall *PFNWINCONTROL)(D2WINCONTROL *ptControl);

struct D2WINCONTROL
{
 	int			eType;
 	HCELFILE	hCelFile;
 	DWORD		dwFlags;
 	int			nX;
 	int			nY;
 	int			nWidth;
 	int			nHeight;
 	PFNWINCONTROL	pfnDraw;
 	PFNWINCONTROL	pfnUpdate;
 	PFNWINMSG	pfnMouseDown;
 	PFNWINMSG	pfnMouseUp;
 	PFNWINMSG	pfnChar;
 	PFNWINMSG	pfnKey;
 	PFNWINMSG	pfnCallback;
 	PFNWINCONTROL	pfnHitTest;
 	D2WINCONTROL	*ptNextChild;
};

struct D2WINANIMSTATE
{
 	HCELFILE	*phCelFile;
 	HCELFILE	*phCelFile2;
 	DWORD		dwDrawMode2;
 	int			nFrameDelay;
};

enum
{
	ANIMSTATE_IDLE,
	ANIMSTATE_OVER,
	ANIMSTATE_SELECTING,
	ANIMSTATE_SELECTED,
	ANIMSTATE_DESELECTING
};

struct D2WINANIMIMAGE : D2WINCONTROL
{
 	D2WINANIMSTATE	*ptStates;
 	int			nFrameDelay;
 	DWORD		dwStartTick;
 	int			nFrame;
 	int			eState;
 	DWORD		dwDrawMode;
 	PFNWINMSG	pfnHover;
 	BOOL		fAnimate;
};

struct D2WINBUTTON : D2WINCONTROL
{
 	DWORD		dwButtonFlags;
 	BOOL		fPressed;
 	BOOL		fMouseInside;
 	int			nClickFrames;
 	DWORD		dwKey;
 	int			nCelsPerState;
 	int			nFirstCel;
 	DWORD		dwFont;
 	WORD		wszText[256];
 	DWORD		dwColor;
 	PFNWINMSG	pfnHover;
 	BOOL		fLastHover;
};

struct D2WINFONTDESC
{
 	DWORD		dwFont;
 	DWORD		dwColor;
};

struct D2WINACCOUNT
{
 	char		szName[16];
 	D2WINACCOUNT	*ptNext;
};

struct D2WINACCOUNTLIST : D2WINCONTROL
{
 	D2WINACCOUNT	*ptFirst;
 	D2WINACCOUNT	*ptSelected;
 	int			nTopIndex;
 	DWORD		field_4c;
 	D2WINFONTDESC	tFont;
};

typedef LRESULT (__stdcall *PFNWINDOWPROC)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef void (__cdecl *PFNWINQUIT)(void);

struct D2WINDOWDATA
{
 	POINT		tMouse;
 	HWND		field_08;
 	HCELFILE	field_0c;
 	DWORD		field_10;
 	int			field_14;
 	D2WINCONTROL	*field_18;
 	D2WINCONTROL	*field_1c;
 	int			deadChildrenLocked;
 	D2WINCONTROL	*field_24;
 	D2WINCONTROL	*field_28;
 	D2WINCONTROL	*field_2c;
 	D2WINCONTROL	*field_30;
 	PFNWINDOWPROC	field_34;
 	BOOL		field_38;
 	BOOL		field_3c;
 	BOOL		field_40;
 	PFNWINQUIT	field_44;
};

extern D2WINDOWDATA sgtWindowData;

struct D2WIN_STRUCT
{
 	int			eType;
 	int			nX;
 	int			nY;
 	DWORD		dwWidth;
 	DWORD		dwHeight;
 	DWORD		field_14;
 	DWORD		field_18;
 	HCELFILE	*phCelFile;
 	DWORD		dwCallback;
 	DWORD		field_24;
 	DWORD		field_28;
 	DWORD		dwFlags;
};

enum
{
	D2WINMSG_MESSAGE,
	D2WINMSG_COMMAND,
	D2WINMSG_KEYUP,
	D2WINMSG_KEYDOWN
};

struct D2WINMSGENTRY
{
 	DWORD		eType;
 	DWORD		id;
 	void		*pfnCallback;
};

#define NUM_COMPONENTS			16
#define COF_VERSION				20
#define CELFILE_NUMGAMEDIRECTIONS	64
#define MAXIMUM_NUMBER_OF_EXTRA_DATA_STRINGS	7

struct TCOFHEADER
{
 	BYTE		bNumLayers;
 	BYTE		bNumFrames;
 	BYTE		bNumDirections;
 	BYTE		bVersion;
 	DWORD		unobserved_04[5];
 	DWORD		dwAnimRate;
};
typedef TCOFHEADER *PTCOFHEADER;

#pragma pack(push, 1)
struct TCOFLAYER
{
 	BYTE		bComponent;
 	BYTE		unobserved_01[4];
 	char		szWClass[4];
};
#pragma pack(pop)

struct TCOFNODE
{
 	int			nRefCount;
 	DWORD		dwMode;
 	DWORD		dwClassId;
 	DWORD		dwWClassToken;
 	DWORD		dwFileSize;
 	BYTE		*lpbCofFile;
 	TCOFNODE	*ptNext;
 	TCOFNODE	*ptPrev;
};

struct TUNITCOFNODE
{
 	TCOFNODE	*ptCofNode;
 	TUNITCOFNODE	*ptNext;
};

struct TGFXINFO
{
 	TUNITCOFNODE	*ptCofNode;
};

struct D2COMPOSITEUNIT
{
 	DWORD		dwFlags;
 	BYTE		bDirection;
 	BYTE		bNumDirections;
 	BYTE		unobserved_06[2];
 	int			nAnimPos;
 	int			nFrames;
 	int			nAnimRate;
 	int			nX;
 	int			nY;
 	DWORD		dwClassId;
 	DWORD		dwMode;
 	DWORD		dwWeaponClass;
 	BYTE		abColors[NUM_COMPONENTS];
 	DWORD		adwGearTokens[NUM_COMPONENTS];
 	DWORD		field_78[NUM_COMPONENTS];
 	TGFXINFO	*ptGfxInfo;
 	HCELFILE	hEmblemCelFile;
 	BYTE		bEmblemColorA;
 	BYTE		bEmblemColorB;
 	BYTE		unobserved_c2[2];
 	D2COMPOSITEUNIT	*ptNext;
 	D2COMPOSITEUNIT	*ptPrev;
 	WORD		wszAccountName[64];
 	WORD		wszCharacterName[64];
 	WORD		wszBracedText[16];
 	WORD		wszRealmName[32];
 	int			nLevel;
 	DWORD		dwProduct;
};

struct D2COMPPALETTE
{
 	BYTE		aabShifts[21][256];
 	BYTE		unobserved_1500[84][256];
};

typedef struct D2WINEDITBOX D2WINEDITBOX;
typedef BOOL (__stdcall *PFNEDITKEY)(D2WINEDITBOX *ptEditData, int nPos, char *pchKey);
typedef void (__stdcall *PFNEDITRETURN)(const WORD *pwszText);
typedef void (__stdcall *PFNEDITPOST)(DWORD dwZero);

enum
{
	EDITBOX_PASSWORD = 1,
	EDITBOX_KEEPFOCUS = 2,
	EDITBOX_TABFILTER = 4,
	EDITBOX_MULTILINE = 8,
	EDITBOX_CHAR0E = 16,
	EDITBOX_CHARFF = 32
};

struct D2WINEDITBOX : D2WINCONTROL
{
 	int			nTextMarginX;
 	int			nTextMarginY;
 	int			nMaxTextLength;
 	int			nFirstShown;
 	int			nLastShown;
 	int			nSelectionStart;
 	int			nSelectionEnd;
 	WORD		wszText[256];
 	int			nCaretPos;
 	DWORD		dwEditBoxFlags;
 	PFNEDITRETURN	pfnEditReturn;
 	PFNEDITKEY	pfnEditKey;
 	PFNEDITPOST	pfnEditPost;
 	D2WINFONTDESC	tFont;
 	D2WINEDITBOX	*ptNextBox;
 	D2WINEDITBOX	*ptPrevBox;
 	BOOL		fMouseDown;
};

#pragma pack(push, 1)
struct D2FONTGLYPH
{
 	WORD		wChar;
 	BYTE		unobserved_02;
 	BYTE		bWidth;
 	BYTE		unobserved_04[4];
 	WORD		wFrame;
 	BYTE		unobserved_0a[4];
};

struct D2FONTTABLE
{
 	BYTE		unobserved_00[8];
 	WORD		wNumGlyphs;
 	BYTE		bHeight;
 	BYTE		unobserved_0b;
};
#pragma pack(pop)

struct D2FONT
{
 	HCELFILE	hCelFile;
 	void		*pTableFile;
 	D2FONTTABLE	*ptTable;
 	D2FONTGLYPH	*ptGlyphs;
 	DWORD		dwLastSelectTime;
};

#define FONT_DWORDS_MAX			8
#define NUM_FONTS				14

struct D2WINHOTSPOT
{
 	int			nX;
 	int			nY;
 	int			nWidth;
 	int			nHeight;
 	PFNWINMSG	pfnCallback;
};

struct D2WINIMAGE : D2WINCONTROL
{
 	int			nCel;
 	DWORD		dwDrawMode;
 	D2WINHOTSPOT	*ptHotSpots;
};

#define MAX_LIST_STRCHARS		256

struct D2WINLISTENTRY
{
 	DWORD		dwStringId;
 	BYTE		bChar;
 	BYTE		unobserved_05[3];
 	DWORD		dwKey;
 	PFNWINMSG	pfnCallback;
};

struct D2WINLISTITEM
{
 	WORD		wszText[MAX_LIST_STRCHARS];
 	int			nTextWidth;
 	PFNWINMSG	pfnCallback;
 	int			nLeft;
 	int			nRight;
 	int			nTop;
 	int			nBottom;
 	BYTE		bChar;
 	BYTE		unobserved_219[3];
 	DWORD		dwKey;
 	D2WINLISTITEM	*ptNext;
};

struct D2WINLIST : D2WINCONTROL
{
 	DWORD		dwFont;
 	int			nLineHeight;
 	int			nLinePadding;
 	int			nTopIndex;
 	int			nNumItems;
 	int			nSelectedIndex;
 	int			nMaxLineOffset;
 	int			nPentagramFrame;
 	int			nMaxTextWidth;
 	D2WINLISTITEM	*ptFirst;
 	D2WINLISTITEM	*ptSelected;
};

typedef void (__cdecl *PFNPOPUPCHOICE)(void);

struct D2WINPOPUPENTRY
{
 	WORD		wszText[64];
 	PFNPOPUPCHOICE	pfnCallback;
};

#define NUM_POPUP_ENTRIES		8

struct D2WINPOPUP : D2WINCONTROL
{
 	int			nNumEntries;
 	D2WINPOPUPENTRY	atEntries[NUM_POPUP_ENTRIES];
 	int			nHoverIndex;
};

struct D2WINPROGRESSBAR : D2WINCONTROL
{
 	float		flFilledPart;
};

struct D2WINSCROLLBAR : D2WINCONTROL
{
 	BOOL		fUpHeld;
 	BOOL		fDownHeld;
 	int			nMaxValue;
 	int			nValue;
 	D2WINCONTROL	*ptTextBox;
 	BOOL		fDragging;
};

struct D2WINSMACKFILE
{
 	BYTE		unobserved_00[884];
 	DWORD		dwFrameNum;
};

struct D2WINSMACKVIEW
{
 	D2WINSMACKFILE	*ptSmack;
 	int			nX;
 	int			nY;
};

struct D2WINSMACK : D2WINCONTROL
{
 	char		szFile[256];
 	D2WINSMACKVIEW	tView;
};

#define MAX_TEXTBOX_COLUMNS		5

struct D2WINTEXTLINE
{
 	WORD		*szText[MAX_TEXTBOX_COLUMNS];
 	DWORD		dwColor;
 	BYTE		bBroken;
 	BYTE		unobserved_19[3];
 	D2WINTEXTLINE	*ptNext;
};

enum
{
	TEXTBOX_SELECT = 1,
	TEXTBOX_CENTER = 2,
	TEXTBOX_SCROLLBAR = 4,
	TEXTBOX_KEEPSELECTED = 8,
	TEXTBOX_RIGHT = 16,
	TEXTBOX_NOWRAP = 32,
	TEXTBOX_MARQUEE = 64,
	TEXTBOX_ROLL = 128
};

struct D2WINTEXTBOX : D2WINCONTROL
{
 	int			nTextMarginX;
 	int			nTextMarginY;
 	D2WINTEXTLINE	*ptFirstLine;
 	D2WINTEXTLINE	*ptLastLine;
 	D2WINTEXTLINE	*ptTopLine;
 	int			nNumLines;
 	int			nTopLine;
 	DWORD		dwTextBoxFlags;
 	int			nSelectedNum;
 	int			nNumColumns;
 	int			anColumnX[MAX_TEXTBOX_COLUMNS];
 	DWORD		adwColumnAlign[MAX_TEXTBOX_COLUMNS];
 	D2WINSCROLLBAR	*ptScrollbar;
 	D2WINFONTDESC	tFont;
 	int			nScrollOffset;
 	int			nBrokenLineSpacing;
 	int			nLineSpacing;
};

struct D2WINTEXTBOXBAR
{
 	int			nX;
 	int			nY;
 	int			nHeight;
};

struct D2WINTIMER : D2WINCONTROL
{
 	int			nInterval;
 	DWORD		dwStartTick;
};

#endif
