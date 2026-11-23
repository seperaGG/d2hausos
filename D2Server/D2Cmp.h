#ifndef D2RE_D2CMP_H
#define D2RE_D2CMP_H

#include <windows.h>
#include <stddef.h>
#include "Archive.h"

#define CELFILE_VERSION		6
#define CELFILE_SERIALIZED	1
#define CELFILE_POINTERS	2
#define CELFILE_24BIT		4
#define CELFILE_NUMGAMEDIRECTIONS	64

typedef struct D2CELSLOT D2CELSLOT;

struct D2CEL
{
 	DWORD		dwFlags;
 	DWORD		dwWidth;
 	DWORD		dwHeight;
 	int			nOffsetX;
 	int			nOffsetY;
 	DWORD		dwExtraSize;
 	D2CELSLOT	*ptSlot;
 	DWORD		dwDataSize;
 	BYTE		abData[1];
};

#define CELSIZE(dwDataSize)	((dwDataSize) + offsetof(D2CEL, abData) + 3)

struct D2CELSLOT
{
 	DWORD		unobserved_00[10];
 	BYTE		*pbExtraData;
};

struct D2CELFILE
{
 	DWORD		dwVersion;
 	DWORD		dwFlags;
 	DWORD		format;
 	DWORD		dwKeyColor;
 	DWORD		dwNumDirections;
 	DWORD		dwCelsPerDirection;
 	D2CEL		*aptCels[1];
};

struct D2CEL5
{
 	DWORD		unobserved_00;
 	DWORD		dwFlags;
 	DWORD		unobserved_08;
 	DWORD		dwWidth;
 	DWORD		dwHeight;
 	DWORD		dwOffsetX;
 	DWORD		dwOffsetY;
 	DWORD		dwExtraSize;
 	BYTE		*pbExtraData;
 	DWORD		dwDataSize;
 	BYTE		abData[1];
};

struct D2CELFILE5
{
 	DWORD		dwVersion;
 	DWORD		dwFlags;
 	DWORD		unobserved_08[2];
 	DWORD		dwNumDirections;
 	DWORD		dwCelsPerDirection;
 	DWORD		unobserved_18[7];
 	D2CEL5		*aptCels[1];
};

struct D2UNITINFO
{
 	DWORD		dwUnitToken;
 	DWORD		dwComponentToken;
 	DWORD		dwVariantToken;
 	DWORD		dwModeToken;
 	DWORD		dwWClassToken;
 	char		*pszFileName;
};

struct D2CELCONTEXT
{
 	D2CEL		*hCel;
 	D2CELFILE	*hCelFile;
 	DWORD		dwFrame;
 	DWORD		dwDirection;
 	DWORD		unobserved_10;
 	DWORD		dwCelsPerDirection;
 	DWORD		dwFlags;
 	BYTE		bUnitFlags;
 	BYTE		unobserved_1d[3];
 	DWORD		dwUnitType;
 	DWORD		dwClassId;
 	DWORD		dwMode;
 	DWORD		dwOverlayClass;
 	D2UNITINFO	tUnitInfo;
};

#define CELCONTEXT_DCC			1
#define CELCONTEXT_LOADED		4

#define CELCONTEXTUNIT_ITEMLIT	1
#define CELCONTEXTUNIT_OVERLAY	2
#define CELCONTEXTUNIT_CONVERT	4
#define CELCONTEXTUNIT_PLAYERSEP	8

#define UNITGFX_PLAYER		0
#define UNITGFX_MONSTER		1
#define UNITGFX_OBJECT		2
#define UNITGFX_MISSILE		3
#define UNITGFX_ITEM		4
#define UNITGFX_OVERLAY		6

#define CELCONTEXT_ISOVERLAY(p) \
	((p)->dwUnitType == UNITGFX_OVERLAY && ((p)->bUnitFlags & CELCONTEXTUNIT_OVERLAY))

typedef struct D2LRUBLOCK D2LRUBLOCK;
typedef struct D2SPRITEBLOCK D2SPRITEBLOCK;
typedef struct D2GFXELEMENT D2GFXELEMENT;
typedef struct D2CELDATAELEMENT D2CELDATAELEMENT;

#define MAX_DIROFFSETS		33
#define OFFSET_ARRAYSIZE	16384

#define CELDATAFLAG_NOFILE	1
#define CELDATAFLAG_DCC		2

struct D2CELDATA
{
 	BYTE		bFlags;
 	BYTE		unobserved_01[3];
 	DWORD		tHeader[6];
 	DWORD		adwDirOffsets[MAX_DIROFFSETS];
 	char		szFileName[32];
};

#define CELHDR_DIRS			4
#define CELHDR_FRAMES		5

#define GFXBLOCK_NOFILE		1
#define GFXBLOCK_BLOCKS		2
#define GFXBLOCK_PENDING	4

struct D2GFXBLOCK
{
 	BYTE		bFlags;
 	BYTE		bNumDirections;
 	BYTE		bFirstDirection;
 	BYTE		field_03;
 	BYTE		bCelsPerDirection;
 	BYTE		unobserved_05[3];
 	BOOL		fHardwareCelData;
 	int			nOffsets[MAX_DIROFFSETS];
 	void		*lpAsyncData;
 	D2SPRITEBLOCK	*ptCacheBlock;
};

struct D2GFXNODE
{
 	DWORD		field_00;
 	DWORD		dwMode;
 	DWORD		dwClassId;
 	DWORD		adwTokens[5];
 	D2GFXBLOCK	tGfxBlock;
};

struct D2SPRITEBLOCK
{
 	void		*lpData;
 	DWORD		dwSize;
 	D2GFXELEMENT	*ptOwner;
 	BOOL		fLoad;
 	DWORD		dwUseTick;
 	D2SPRITEBLOCK	*ptNext;
 	D2SPRITEBLOCK	*ptPrev;
};

struct D2SPRITECACHE
{
 	void		*pMemPool;
 	DWORD		dwMaxAllocated;
 	DWORD		dwAllocated;
 	DWORD		dwNumUsed;
 	D2SPRITEBLOCK	*ptBlocks;
 	D2SPRITEBLOCK	*ptUsedHead;
 	D2SPRITEBLOCK	*ptUsedTail;
 	D2SPRITEBLOCK	*ptLoadHead;
 	D2SPRITEBLOCK	*ptLoadTail;
 	D2SPRITEBLOCK	*ptFreeHead;
 	D2SPRITEBLOCK	*ptFreeTail;
 	void		(__fastcall *field_2c)(D2GFXELEMENT *ptElement);
 	CRITICAL_SECTION	*pcs;
 	DWORD		dwNumAllocs;
 	DWORD		dwNumUses;
};

struct D2CELDATAELEMENT
{
 	DWORD		dwKey;
 	char		szFileName[32];
 	D2LRUBLOCK	*lpBlock;
 	D2CELDATAELEMENT	*ptPrev;
 	D2CELDATAELEMENT	*ptNext;
};

struct D2CELDATAHASH
{
 	DWORD		nElements[512];
 	D2CELDATAELEMENT	*ptBuckets[512];
};

struct D2GFXELEMENT
{
 	DWORD		dwKey;
 	DWORD		dwClass;
 	DWORD		adwTokens[5];
 	DWORD		dwDirBlock;
 	D2GFXNODE	*ptGfxNode;
 	D2SPRITEBLOCK	*ptCacheBlock;
 	D2GFXELEMENT	*ptPrev;
 	D2GFXELEMENT	*ptNext;
};

struct D2GFXHASH
{
 	DWORD		unobserved_0000[8192];
 	D2GFXELEMENT	*ptBuckets[8192];
};

typedef struct D2TILECOMP D2TILECOMP;
typedef struct D2LRUCACHE D2LRUCACHE;

struct D2LRUBLOCK
{
 	void		*lpData;
 	DWORD		dwSize;
 	void		*pOwner;
 	D2LRUBLOCK	*ptNext;
 	D2LRUBLOCK	*ptPrev;
};

struct D2LRUCACHE
{
 	void		*pMemPool;
 	DWORD		dwMaxAllocated;
 	DWORD		dwAllocated;
 	DWORD		dwNumUsedBlocks;
 	D2LRUBLOCK	*ptBlocks;
 	DWORD		dwMaxBlocks;
 	D2LRUBLOCK	*ptUsedHead;
 	D2LRUBLOCK	*ptUsedTail;
 	D2LRUBLOCK	*ptFreeHead;
 	D2LRUBLOCK	*ptFreeTail;
 	void		(__fastcall *field_28)(D2LRUBLOCK *ptBlock);
 	DWORD		dwNumAllocs;
 	DWORD		dwNumUses;
};

struct D2TILECOMP
{
 	WORD		wOffsetX;
 	WORD		wOffsetY;
 	WORD		unobserved_04;
 	BYTE		bColumn;
 	BYTE		bRow;
 	WORD		wFlags;
 	WORD		wCompLength;
 	DWORD		unobserved_0c;
 	BYTE		*ptData;
};

struct D2TILE
{
 	DWORD		field_00;
 	WORD		wYOffset;
 	WORD		wFlags;
 	DWORD		dwHeight;
 	DWORD		dwWidth;
 	DWORD		field_10;
 	DWORD		dwOrientation;
 	DWORD		dwStyle;
 	DWORD		dwSequence;
 	DWORD		dwRarity;
 	DWORD		dwKeyColor;
 	BYTE		abCollisionMask[25];
 	BYTE		unobserved_41[5];
 	BYTE		bTypeFlag;
 	BYTE		unobserved_47;
 	DWORD		dwCompOffset;
 	DWORD		dwCompSize;
 	int			nCompCount;
 	D2TILECOMP	*ptCompArray;
 	char		*pszProjectName;
 	D2LRUBLOCK	*ptLRUCache;
};

#define TILETYPEFLAG_GAMESQAURE24	8
#define TILETYPEFLAG_SQUISHED	16
#define TILECOMP_PRESENT		1
#define TILECOMP_BLOCK			4096
#define TILECOMP_GAMESQUARE		8192

#define TILEPROJECT_VERSION		7
#define TILEPROJECT_FLAG_24BIT	1
#define TILEPROJECT_SERIALIZED	2
#define TILEPROJECT_LOADED		4
#define TILEPROJECT_FEWBLOCKS	8

struct D2TILEPROJECT
{
 	DWORD		dwVersion;
 	DWORD		dwFlags;
 	char		szName[MAX_PATH];
 	int			nNumTiles;
 	D2TILE		*ptTileArray;
};

#define BITCOUNT(x) \
	(((((x) - (((x) >> 1) & 2004318071) - (((x) >> 2) & 858993459) - (((x) >> 3) & 286331153)) + \
	(((x) - (((x) >> 1) & 2004318071) - (((x) >> 2) & 858993459) - (((x) >> 3) & 286331153)) >> 4)) & \
	252645135) % 255)

typedef void (__fastcall *PFNFREEHARDWARE)(D2CELSLOT *ptSlot);

int __fastcall CountSame(const BYTE *pSrc, const BYTE *pEnd, int nMax, int *pnAtEnd, int *pnRestSame);
int __fastcall CountValue(BYTE bValue, const BYTE *pSrc, const BYTE *pEnd, int nMax, int *pnAtEnd,
	int *pnRestSame);
int __fastcall CountUntil(const BYTE *pSrc, const BYTE *pEnd, BYTE bValue, int nMax, int *pnAtEnd,
	int *pnRestSame);

BYTE * __fastcall SubTileEncodeBlock(const BYTE *pbPixels, int *pnSize);
void __fastcall SubTileConvertComp(D2TILECOMP *ptComp8, const D2TILECOMP *ptComp24, DWORD dwKeyColor,
	BYTE (WINAPI *pfnQuantize)(BYTE bColor0, BYTE bColor1, BYTE bColor2));
void __fastcall SubTileCompress(D2TILECOMP *ptComp);

void __fastcall FastCmpEncode(const BYTE *pbPixels, DWORD dwWidth, DWORD dwHeight, DWORD dwFlags,
	BYTE **ppbOut, DWORD *pdwSize);
void __fastcall FastCmpCelRemapColors(D2CEL *pCel, const BYTE *pbMap);
void __fastcall FastCmpConvert24Bit(D2CELFILE *pSrc, D2CELFILE *pDst,
	BYTE (WINAPI *pfnMatch)(BYTE bColor0, BYTE bColor1, BYTE bColor2), DWORD dwKeyColor);

void __fastcall FastCmpDrawShadow(D2CEL *pCel, BYTE *pbDst, int nX, int nClipLeft, int nClipRight,
	DWORD dwStride, DWORD dwSkipRows, DWORD dwRows, const BYTE *pbTable, DWORD dwColor);

void __fastcall FastCmpDecode(D2CEL *pCel, BYTE *pbDst, int nX, int nClipLeft, int nClipRight, DWORD dwStride,
	UINT nLinesToSkip, UINT nLinesToDraw, const BYTE *pbColorMap, const BYTE *pbBlend, const BYTE *pbPreMap);

void __fastcall RawCelForEachPixel(D2CEL *pCel, void (WINAPI *pfnPixel)(DWORD), DWORD dwKeyColor);

typedef struct D2FINDTILEKEY D2FINDTILEKEY;
BYTE __fastcall FindTilesHashKey(DWORD dwValue3, DWORD dwValue2, DWORD dwValue1);
D2FINDTILEKEY ** __cdecl FindTilesCreate(void);
void __fastcall FindTilesFree(D2FINDTILEKEY **ptHash);
void __fastcall FindTilesAdd(D2FINDTILEKEY **ptHash, D2TILE *ptTile);
int __fastcall FindTilesGet(D2FINDTILEKEY **ptHash, D2TILE **hTileList, DWORD dwMax, DWORD dwValue1,
	DWORD dwValue2, DWORD dwValue3);

void __fastcall LRUCacheInit(D2LRUCACHE *ptCache, void *pPool, DWORD dwMaxAllocated,
	void (__fastcall *pfnFree)(D2LRUBLOCK *ptBlock), DWORD dwMaxBlocks);
void __fastcall LRUCacheDestroy(D2LRUCACHE *ptCache);
D2LRUBLOCK * __fastcall LRUCacheAlloc(D2LRUCACHE *ptCache, DWORD dwSize, void *pOwner);
void __fastcall LRUCacheFree(D2LRUCACHE *ptCache, D2LRUBLOCK *ptBlock);
void __fastcall LRUCacheTouch(D2LRUCACHE *ptCache, D2LRUBLOCK *ptBlock);

void __fastcall TileCacheReleaseProject(D2TILEPROJECT *ptProject);
DWORD __fastcall TileCacheProjectSize(const char *pszName);
void __fastcall TileCacheSetHardwareFree(void (__fastcall *pfnFree)(D2TILE *ptTile));

void WINAPI TileProjectFree(D2TILEPROJECT *ptProject, BOOL bKeep);
void WINAPI TileProjectLoadTiles(D2TILEPROJECT *ptProject, D2TILEPROJECT **pptProject, const char *szName);
D2FINDTILEKEY ** __fastcall TileProjectFindTiles(D2TILEPROJECT *hProject);
void WINAPI TileProjectTileParts(D2TILE *ptTile);

void __cdecl TileProjectsFreeAll(void);
void WINAPI TileProjectsGet(D2FINDTILEKEY ***pptHash, const char *szName);

void __fastcall CelDataHashInit(D2CELDATAHASH *ptHashTable);
D2CELDATAELEMENT * __fastcall CelDataHashFind(D2CELDATAHASH *ptHashTable, const char *pszFileName);
D2CELDATAELEMENT * __fastcall CelDataHashAdd(void *pPool, D2CELDATAHASH *ptHashTable, D2LRUBLOCK *lpBlock,
	const char *pszFileName);
void __fastcall CelDataHashRemove(void *pPool, D2CELDATAHASH *ptHashTable, D2CELDATAELEMENT *ptElement);
void __fastcall CelDataHashNothing(D2CELDATAHASH *ptHashTable);

void __fastcall GfxHashInit(D2GFXHASH *ptHashTable);
DWORD __fastcall GfxHashDirBlock(D2CELCONTEXT *ptCelContext, DWORD *pdwBlockSize, D2CELDATA *ptCelData);
D2GFXELEMENT * __fastcall GfxHashFind(D2GFXHASH *ptHashTable, D2CELCONTEXT *ptCelContext, DWORD *pdwDirBlock,
	DWORD *pdwBlockSize, D2CELDATA *ptCelData);
D2GFXELEMENT * __fastcall GfxHashAdd(void *pPool, D2GFXHASH *ptHashTable, D2GFXNODE *ptGfxNode,
	D2CELCONTEXT *ptCelContext, D2CELDATA *ptCelData);
void __fastcall GfxHashRemove(void *pPool, D2GFXHASH *ptHashTable, D2GFXELEMENT *ptElement);
void __fastcall GfxHashNothing(D2GFXHASH *ptHashTable);

void __cdecl CodecInit(void);
void __cdecl CodecShutdown(void);
BOOL __fastcall CodecReadHeader(const char *pszFileName, D2CELDATA *ptCelData);
BYTE * __fastcall CodecDecodeSprite(DWORD *pdwData, void *pPool, int nSize, D2CELDATA *ptCelData,
	D2GFXBLOCK *ptGfxBlock, int nFirst, int nCount, DWORD *pdwSize, const char *pszFileName);

DWORD __cdecl GetFirstBit(DWORD dwValue);
void * __fastcall MemVirtualAlloc(DWORD dwSize);
void __fastcall MemVirtualFree(void *pMemory);
void __fastcall MemCopyRect(BYTE *pbDst, const BYTE *pbSrc, int nDstPitch, int nSrcPitch, DWORD nRows,
	DWORD dwWidth);
BOOL __fastcall MemCompare(const BYTE *pbFirst, const BYTE *pbSecond, DWORD dwSize);

DWORD WINAPI CelGetGameDirection(DWORD nGameDirection, DWORD nLevelOfRotation);
D2CEL * WINAPI CelContextGetCel(D2CELCONTEXT *pCelContext);
int WINAPI CelConvertDirection(DWORD nDirection, DWORD nStartDirs, int nEndDirs, BOOL fAlternate);
int WINAPI CelGetDirectionConversion(int nFrom, int nTo);
BOOL WINAPI CelFileFreeHardware(D2CELFILE *ptFile);
PFNFREEHARDWARE __cdecl CelGetHardwareFree(void);

#endif
