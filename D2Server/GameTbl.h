#ifndef D2RE_D2SERVER_GAMETBL_H
#define D2RE_D2SERVER_GAMETBL_H

#include "CelList.h"

#define TAG_SGAMEDATA				".?AUSGAMEDATA@@"
#define TAG_SGAMEDATA_CHUNK			".?AV?$TSHashObjectChunk@USGAMEDATA@@VHASHKEY_NONE@@@@"
#define TAG_SGAMEDATA_LIST			".?AV?$TSExplicitList@USGAMEDATA@@$0?CCCCCCCD@@@"

struct SGAMEDATA : TSHashObject<SGAMEDATA, HASHKEY_NONE>
{
 	DWORD		unobserved_18[12360 / 4];
};

inline const char *StormTypeTag(SGAMEDATA *) { return TAG_SGAMEDATA; }
inline const char *StormTypeTag(TSHashObjectChunk<SGAMEDATA, HASHKEY_NONE> *) { return TAG_SGAMEDATA_CHUNK; }
inline const char *StormTypeTag(TSExplicitList<SGAMEDATA, (int)0xCCCCCCCD> *) { return TAG_SGAMEDATA_LIST; }

typedef TSExportTableSync<SGAMEDATA, HASHKEY_NONE> GAMEDATATBL;

struct SERVERGAMEDATATBL
{
 	BYTE		unobserved_00[104];
};
typedef char SERVERGAMEDATATBLFITS[sizeof(GAMEDATATBL) == sizeof(SERVERGAMEDATATBL) ? 1 : -1];

inline void GameDataTblConstruct(SERVERGAMEDATATBL *ptStorage) { new (ptStorage) GAMEDATATBL; }
inline void GameDataTblDestruct(SERVERGAMEDATATBL *ptStorage)
{
	((GAMEDATATBL *)ptStorage)->GAMEDATATBL::~GAMEDATATBL();
}

#endif
