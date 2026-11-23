#ifndef D2RE_DRLG_H
#define D2RE_DRLG_H

#include "../D2Common.h"
#include "../DATATBLS/DataTbls.h"

void __fastcall SeedSet(D2SEED *ptSeed, DWORD dwSeed);
DWORD __fastcall SeedNext(D2SEED *ptSeed);
DWORD __fastcall SeedRand(D2SEED *ptSeed, DWORD nMax);

void *__fastcall DungeonActGetPool(struct ACT *ptAct);
DUNGEONROOM *__fastcall DungeonNewRoom(ACT *ptAct, ROOM *ptDrlgRoom, GAMERECT *ptRects,
	PTDRLGTILEARRAYS ptTiles, DWORD dwSeed, DWORD dwFlags);

void __stdcall EnvTileToSubtile(int *pnA, int *pnB);

BYTE __stdcall DrlgGetActNum(int nLevelId);

DUNGEONROOM *__fastcall DrlgActGetFirstActiveRoom(DRLGACTCTX *ptAct);
DUNGEONROOM *__fastcall DrlgActGetNextActiveRoom(DRLGACTCTX *ptAct, ROOM *ptRoom);
BOOL __fastcall DrlgRoomCanRelease(ROOM *ptRoom);
void __fastcall DrlgRoomSetHold(ROOM *ptRoom, BOOL fClear);
BYTE __fastcall DrlgRoomGetState(ROOM *ptRoom);
void __fastcall DrlgRoomChangeLevel(ROOM *ptFrom, ROOM *ptTo);
void __fastcall DrlgActFreeIdleLevels(DRLGACTCTX *ptAct);
int __fastcall DrlgActGetDrawnLevel(DRLGACTCTX *ptAct);
int *__fastcall DrlgRoomGetLevelWarpX(ROOM *ptRoom);
int __fastcall DrlgActCountLevelRooms(DRLGACTCTX *ptAct, int nLevelId);
BOOL __fastcall DrlgLevelIsNone(int nLevelId);
void __fastcall DrlgRoomAnimateNear(ROOM *ptRoom);
void __fastcall DrlgRoomSyncAnimFrames(ROOM *ptFrom, ROOM *ptTo);
void __fastcall DrlgAnimTiles(ROOM *ptRoom, int nStep, DRLGGRID *ptWalls, int nPairs, DRLGGRID *ptFloors,
	int nFloors, DRLGGRID *ptShadows);
int __fastcall DrlgRoomGetLogicValue(ROOM *ptRoom, int nX, int nY);
PTDRLGLOGICNODE __fastcall DrlgRoomGetLogicNode(ROOM *ptRoom, int nX, int nY);
PTDRLGLOGICNODE __fastcall DrlgRoomGetLogicList(ROOM *ptRoom);
void __fastcall DrlgLogicBuild(ROOM *ptRoom, DRLGGRID *ptOrient, DRLGGRID *ptFloors,
	DRLGGRID *ptWalls);
void __fastcall DrlgLogicBuildEmpty(ROOM *ptRoom);
void __fastcall DrlgRoomReleaseGameRoom(ROOM *ptRoom, BOOL fClient, DWORD dwFlags);
DWORD __fastcall DrlgRoomGetKindFlag(ROOM *ptRoom);
DWORD __fastcall DrlgRoomGetOutdoorFlag(ROOM *ptRoom);
DWORD __fastcall DrlgRoomGetLevelId(ROOM *ptRoom);
int __fastcall DrlgRoomGetVisLevel(ROOM *ptRoom, int nIndex);
DWORD __fastcall DrlgRoomGetLevelIdChecked(ROOM *ptRoom);
BOOL __fastcall DrlgRoomHasExitFlags(ROOM *ptRoom);
const char *__fastcall DrlgRoomGetFileName(ROOM *ptRoom);
int __fastcall DrlgRoomGetNearCount(ROOM *ptRoom);
int __fastcall DrlgRoomGetNearGameRooms(ROOM *ptRoom, DUNGEONROOM **pptOut);
void __fastcall DrlgRoomSetGameRoom(ROOM *ptRoom, DUNGEONROOM *ptGameRoom);
PTPRESETUNIT __fastcall DrlgRoomGetPresetUnits(ROOM *ptRoom);
void __fastcall DrlgRoomGetLight(ROOM *ptRoom, BYTE *pbIntensity, BYTE *pbRed, BYTE *pbGreen,
	BYTE *pbBlue);
DWORD __fastcall OutRoomGetCellFlagBit(ROOM *ptRoom);
DUNGEONROOM *__fastcall DrlgRoomGetWarpDest(ROOM *ptRoom, BYTE bExit, BYTE *pbExit,
	PTLVLWARPTXT *pptWarpDef);
void __fastcall DrlgRoomSetWarpsDefAllowed(ROOM *ptRoom, int nValue);
void __fastcall DrlgRoomShowWarpTiles(ROOM *ptRoom, int nExit);
void __fastcall DrlgRoomHideWarpTiles(ROOM *ptRoom, int nExit);
PTLVLWARPTXT __fastcall DrlgRoomGetUnitWarpDef(ROOM *ptRoom, struct UNIT *ptUnit);
BYTE *__fastcall PresetRoomGetFile(PTPRESETROOM pRoom);
void __fastcall PresetRoomUpdateDoors(ROOM *ptRoom, int nX, int nY, BOOL fReopen);
void __fastcall PresetRoomGetPoints(ROOM *ptRoom, struct GAMECOORD **pptPoints,
	int *pnCount);

DUNGEONROOM *__fastcall DrlgRoomBuild(ROOM *ptRoom);
void __fastcall DrlgRoomSwitch(ROOM *ptFrom, ROOM *ptTo);
void __fastcall DrlgRoomEnterClient(DRLGACTCTX *ptAct, DWORD nLevelId, int nX, int nY,
	ROOM *ptHint);
void __fastcall DrlgRoomLeaveClient(DRLGACTCTX *ptAct, DWORD nLevelId, int nX, int nY,
	ROOM *ptHint);
DUNGEONROOM *__fastcall DrlgLevelGetRoom(DRLGACTCTX *ptAct, int nX, int nY);
ROOM *__fastcall DrlgLevelFindRoom(DRLGLEVEL *ptDrlgLevel, int nX, int nY);
ROOM *__fastcall DrlgActFindRoom(int nX, int nY, DRLGACTCTX *ptAct, ROOM *ptHint,
	DRLGLEVEL *ptDrlgLevel);
BOOL __fastcall DrlgLevelIsTown(int nLevelId);
DRLGACTCTX *__fastcall DrlgNewAct(struct ACT *ptOwner, BYTE bAct, struct HD2ARCHIVE__ *hArchive, DWORD dwMapSeed,
	int nInitLevel, DWORD dwFlags, DWORD dwGame, PFNDRLGROOMBUILT pfnRoomBuilt,
	PFNDRLGTOWNPLACED pfnTownPlaced);
DRLGLEVEL *__fastcall DrlgNewLevel(DRLGACTCTX *ptAct, int nLevelId);
PTDRLGLEVEL __stdcall DrlgGetLevel(struct DRLGACTCTX *pDrlgAct, DWORD nLevelId);
void __stdcall DrlgGenerateLevel(DRLGLEVEL *ptLevel);
int __fastcall DrlgRectEdgeDir(DRLGCOORDS *ptA, DRLGCOORDS *ptB);
void __fastcall DrlgLevelGetRoomBounds(DRLGLEVEL *ptLevel, int *pnMinX, int *pnMinY, int *pnMaxX,
	int *pnMaxY);
void __fastcall DrlgLevelFitRooms(DRLGLEVEL *ptDrlgLevel);
void __fastcall DrlgLevelSetRect(DRLGACTCTX *ptAct, DRLGLEVEL *ptLevel);
PTVISINFO __fastcall DrlgActGetVisInfo(DRLGACTCTX *ptAct, int nLevelId);
PTVISINFO __fastcall DrlgVisInfoSetLevel(PTVISINFO ptVisInfo, int nLevelId, int nWarp,
	int nSlot);

void __fastcall DrlgGridApply(DRLGGRID *ptGrid, int nX, int nY, int nValue, int nOp);
int __fastcall DrlgGridGet(DRLGGRID *ptGrid, int nX, int nY);
BOOL __fastcall DrlgGridInBounds(DRLGGRID *ptGrid, int nX, int nY);
void __fastcall DrlgGridApplyLine(DRLGGRID *ptGrid, DRLGVERTEX *ptVertex, int nValue, int nOp,
	BOOL fEnd);
void __fastcall DrlgGridOrPoints(DRLGGRID *ptGrid, DRLGVERTEX *ptPoint, int nValue);
void __fastcall DrlgGridApplyAll(DRLGGRID *ptGrid, int nValue, int nOp);

void __fastcall DrlgGridAlloc(void *pPool, DRLGGRID *ptGrid, int nX, int nY);
void __fastcall DrlgGridAllocView(void *pPool, DRLGGRID *ptGrid, int *pnCells, DRLGCOORDS *ptCoords,
	int nStride);
void __fastcall DrlgGridInit(DRLGGRID *ptGrid, int nX, int nY, int *pnCells,
	int *pnRowOffsets);
void __fastcall DrlgGridInitView(DRLGGRID *ptGrid, int *pnCells, DRLGCOORDS *ptCoords, int nStride,
	int *pnRowOffsets);
void __fastcall DrlgGridClear(DRLGGRID *ptGrid);

ROOM *__fastcall DrlgRoomNew(DRLGLEVEL *ptLevel, int nKind);
void __fastcall DrlgRoomAddLink(ROOM *ptRoom, ROOM *ptOther, int nDirection);
void __fastcall DrlgRoomLinkRooms(ROOM *ptRoomA, ROOM *ptRoomB, int nDirection);
void __fastcall DrlgLevelLinkNew(DRLGLEVELLINK **pptList, DRLGLEVEL *ptLevel, int nDirection,
	int nArg);
BOOL __fastcall DrlgRectTestApartAndGaps(DRLGCOORDS *ptA, DRLGCOORDS *ptB, int nThreshold, int *pnGapX,
	int *pnGapY);
BOOL __fastcall DrlgRectGapTest(DRLGCOORDS *ptA, DRLGCOORDS *ptB, int nThreshold);
BOOL __fastcall DrlgRectIsAdjacent(DRLGCOORDS *ptA, DRLGCOORDS *ptB, int nThreshold);
BOOL __fastcall DrlgRoomFitsInLevel(DRLGLEVEL *ptLevel, ROOM *ptRoom, ROOM *ptExclude,
	int nThreshold);
void __fastcall DrlgRoomLink(DRLGLEVEL *ptLevel, ROOM *ptRoom);
BOOL __fastcall DrlgRectContains(DRLGCOORDS *ptCoords, int nX, int nY);
void __fastcall DrlgRoomBuildNear(void *pPool, ROOM *ptRoom);
int *__fastcall DrlgRoomGetVis(DRLGACTCTX *ptAct, int nLevelId);

DRLGVERTEX *__fastcall DrlgVerNew(void *pPool, BYTE bFlag);
void __fastcall DrlgVerBuildOutline(void *pPool, DRLGVERTEX **pptRing, DRLGCOORDS *ptCoords, BYTE bFlag,
	DRLGLEVELLINK *ptLinks);
void __fastcall DrlgVerFreeRing(void *pPool, DRLGVERTEX **pptRing);
void __fastcall DrlgVerGetStep(DRLGVERTEX *ptVertex, int *pnDX, int *pnDY);

int *__fastcall DrlgWarpGetList(DRLGACTCTX *ptAct, int nLevelId);
DUNGEONROOM *__fastcall DrlgWarpGetRoom(DRLGACTCTX *ptAct, int nLevelId, int nEntry, int *pnX,
	int *pnY);
int __fastcall DrlgWarpGetWarp(DRLGLEVEL *ptLevel, BYTE bIndex);

void __fastcall MazeLevelInit(DRLGLEVEL *ptLevel);
void __fastcall MazeLevelGenerate(DRLGLEVEL *ptLevel);

int __fastcall OutdoorsGetEdgeExits(DRLGLEVEL *ptLevel, DRLGVERTEX *ptVertex);
void __fastcall OutDesrGenerate(DRLGLEVEL *ptLevel);

void __fastcall OutdoorsCellClear(DRLGLEVEL *ptLevel, int nX, int nY);
void __fastcall OutdoorsCellBlock(DRLGLEVEL *ptLevel, int nX, int nY);
BOOL __fastcall OutdoorsCellTestOpen(DRLGLEVEL *ptLevel, int nX, int nY);
BOOL __fastcall OutdoorsCellIsFree(DRLGLEVEL *ptLevel, int nX, int nY);
BOOL __fastcall OutdoorsTryPlacePreset(DRLGLEVEL *ptLevel, int nX, int nY, int nDef, int nGrow,
	int bSides);
void __fastcall OutdoorsStampPreset(DRLGLEVEL *ptLevel, int nX, int nY, int nDef, int nFile,
	BOOL fBorder);
BOOL __fastcall OutdoorsPlaceFarthest(DRLGLEVEL *ptLevel, DRLGCOORDS *ptFrom, int nDef, int nFile, int nGrow,
	int bSides);
BOOL __fastcall OutdoorsPlacePreset(DRLGLEVEL *ptLevel, int nDef, int nFile, int nGrow,
	int bSides);
BOOL __fastcall OutdoorsPlacePresetNear(DRLGLEVEL *ptLevel, int nDef, int nFile);
void __fastcall OutdoorsPlaceWaypoint(DRLGLEVEL *ptLevel);
void __fastcall OutdoorsPlaceShrines(DRLGLEVEL *ptLevel, int nCount);
void __fastcall OutdoorsRunLvlSubPass(DRLGLEVEL *ptLevel, int nType, int nDefBase);
void __fastcall OutdoorsBuildEntries(DRLGLEVEL *ptLevel);
void __fastcall OutdoorsLevelInit(DRLGLEVEL *ptLevel);
void __fastcall OutdoorsLevelGenerate(DRLGLEVEL *ptLevel);
void __fastcall OutdoorsLevelFree(DRLGLEVEL *ptLevel, BOOL fKeep);

void __fastcall OutdoorsLayoutAct(DRLGACTCTX *ptAct, BYTE bAct);

ROOM *__fastcall OutRoomNew(DRLGLEVEL *ptLevel, int nX, int nY, int nSizeX, int nSizeY, DWORD dwExits,
	DWORD dwCellFlags, int nCellValue, DWORD dwFlags);
void __fastcall OutRoomFreeInfo(ROOM *ptRoom);
void __fastcall OutRoomFreeGrids(ROOM *ptRoom);
void __fastcall OutRoomNewInfo(ROOM *ptRoom);
void __fastcall OutRoomFindTownEdge(DRLGLEVEL *ptLevel, int *pnX, int *pnY);
void __fastcall OutRoomBuildTilesA(ROOM *ptRoom);
void __fastcall OutRoomBuildTilesB(ROOM *ptRoom);

BOOL __fastcall OutdoorsFindEntryPath(DRLGLEVEL *ptLevel, int nEntry);
int __fastcall OutdoorsGetEdgeDef(int nDX, int nDY, int nColumn);
int __fastcall OutdoorsGetCornerDef(int nDX, int nDY, int nNextDX, int nNextDY,
	int nColumn);
void __fastcall OutdoorsMarkOutside(DRLGLEVEL *ptLevel);
int __fastcall OutdoorsGetBorderColumn(DRLGLEVEL *ptLevel, BYTE bConcave);
void __fastcall OutdoorsMarkOutline(DRLGLEVEL *ptLevel);
void __fastcall OutdoorsMarkInterior(DRLGLEVEL *ptLevel);
void __fastcall OutJungGenerate(DRLGLEVEL *ptLevel);
void __fastcall OutJungGenerateMesa(DRLGLEVEL *ptLevel);

int __fastcall PathUtilGetDirection(int nX1, int nY1, int nX2, int nY2);

void __fastcall OutWildGenerate(DRLGLEVEL *ptLevel);

PTPRESETUNIT __fastcall PresetUnitClone(void *pPool, PTPRESETUNIT ptSource, int nX,
	int nY);
PTPRESETFILELIST __fastcall PresetFileListClone(void *pPool, PTPRESETFILELIST pSource);
void __fastcall PresetRegionAttachUnits(void *pPool, PTPRESETSUBINFO ptRegion,
	D2SEED *ptSeed);
void __fastcall PresetRoomNewInfo(ROOM *ptRoom);
ROOM *__fastcall PresetRoomNew(DRLGLEVEL *ptLevel, PTPRESETSUBINFO ptRegion, DRLGCOORDS *ptCoords,
	DWORD dwDt1Mask, DWORD dwFlags, DWORD dwInfoFlags, PTPRESETLINK ptLink);
ROOM *__fastcall PresetRegionToRooms(DRLGLEVEL *ptLevel, PTPRESETSUBINFO ptRegion, DWORD dwFlags,
	BOOL fWhole);
void __fastcall PresetRegionFill(DRLGLEVEL *ptLevel, DRLGGRID *ptGrid, DWORD dwFlags,
	PTPRESETSUBINFO ptRegion, BOOL fWhole);
PTPRESETSUBINFO __fastcall PresetRegionNew(DRLGLEVEL *ptLevel, int nDef, DRLGCOORDS *ptCoords,
	D2SEED *ptSeed);
void __fastcall PresetLevelInit(DRLGLEVEL *ptLevel);
void __fastcall PresetLevelGenerate(DRLGLEVEL *ptLevel);
DWORD __fastcall PresetRegionSetFile(PTPRESETSUBINFO ptRegion, DWORD nIndex);
int __fastcall PresetGetSizeX(int nDef);
int __fastcall PresetGetSizeY(int nDef);

void __fastcall TileSubRun(PTTILESUBCTX ptCtx);
void __fastcall TileSubLoadFile(struct HD2ARCHIVE__ *hArchive, PTLVLSUBTXT ptTileSub);
DWORD __fastcall TileSubRoomPick(ROOM *ptRoom, int nType, int nTheme);

struct TILESUBROOM
{
 	ROOM		*ptRoom;
 	DRLGGRID	*aptOrientationGrids[4];
 	DRLGGRID	*aptWallGrids[4];
 	DRLGGRID	*ptFloorGrid;
 	int			field_28;
 	int			nGrids;
 	int			nType;
 	int			nSubTheme;
 	DWORD		dwPicks;
};
typedef struct TILESUBROOM TILESUBROOM, *PTTILESUBROOM;
void __fastcall TileSubRoomRun(PTTILESUBROOM ptCtx);
PTDRLGTILE __fastcall RoomTileAddShadowAt(ROOM *ptRoom, int nX, int nY, DWORD dwCell);
void __fastcall RoomTileGrowShadows(void *pPool, PTDRLGTILEBLOCK ptBlock, int nExtra);
void __fastcall DrlgGridApplyThickLine(DRLGGRID *ptGrid, DRLGVERTEX *ptVertex, DRLGCOORDS *ptRect, int nValue,
	int nOp, int nWidth);
void __fastcall OutdoorsMarkRoomPaths(DRLGLEVEL *ptLevel, ROOM *ptRoom);

void __stdcall DungeonCalcTileToScreen(int *pnA, int *pnB);
PTLVLWARPTXT __fastcall LvlTblsGetWarpDef(int nWarp);
PTLVLWARPTXT __fastcall DrlgWarpGetWarpDef(DRLGLEVEL *ptLevel, BYTE bExit);
void __fastcall DrlgCountAnimTiles(ROOM *ptRoom, DRLGGRID *ptGrid, DRLGGRID *ptOrient, int nOrient,
	BOOL fRight, BOOL fBottom);
void __fastcall DrlgFreeAct(DRLGACTCTX *ptAct);
void __fastcall DrlgRoomNewGameRoom(DRLGACTCTX *ptAct, ROOM *ptRoom);
BOOL __fastcall DrlgGridIsSet(DRLGGRID *ptGrid);
int *__fastcall DrlgGridGetCell(DRLGGRID *ptGrid, int nX, int nY);
void __fastcall DrlgGridApplyBorder(DRLGGRID *ptGrid, int nValue, int nOp);
void __fastcall DrlgGridFree(void *pPool, DRLGGRID *ptGrid);
void __fastcall DrlgLogicFree(ROOM *ptRoom);
void __fastcall DrlgRoomFreeWarps(void *pPool, ROOM *ptRoom);
void __fastcall DrlgRoomFree(ROOM *ptRoom);
void __fastcall DrlgRoomFreeLinks(void *pPool, PTDRLGROOMLINK ptLink);
void __fastcall DrlgRoomRemoveLink(void *pPool, ROOM *ptRoom, ROOM *ptOther);
void __fastcall DrlgRoomUnlink(void *pPool, ROOM *ptRoomA, ROOM *ptRoomB);
BOOL __fastcall DrlgRectContainsInclusive(DRLGCOORDS *ptRect, int nX, int nY);
DWORD __fastcall DrlgRoomGetExitFlags(ROOM *ptRoom);
void __fastcall MazeLevelFree(DRLGLEVEL *ptLevel, BOOL fKeep);
void __fastcall PresetUnitFree(void *pPool, PTPRESETUNIT ptUnit);
void __fastcall PresetRoomLoadUnits(ROOM *ptRoom);
void __fastcall PresetRoomBuildTilesA(ROOM *ptRoom);
void __fastcall PresetRoomBuildTilesB(ROOM *ptRoom);
void __fastcall PresetFileListFree(void *pPool, PTPRESETFILELIST pList);
void __fastcall PresetRoomFreeInfo(ROOM *ptRoom);
void __fastcall PresetRoomFreeGrids(ROOM *ptRoom);
void __fastcall PresetLevelFree(DRLGLEVEL *ptLevel, BOOL fKeep);
void __fastcall PresetFreeRegions(void *pPool, PTPRESETSUBINFO ptRegion);
void *__fastcall RoomTilePick(ROOM *ptRoom, DWORD dwOrient, DWORD dwCell);
PTDRLGTILE __fastcall RoomTileAddWall(ROOM *ptRoom, PTDRLGTILE *ppChain, int nX, int nY, DWORD dwCell,
	void *hTile, int nOrient);
PTDRLGTILE __fastcall RoomTileAddFloor(ROOM *ptRoom, PTDRLGTILE *ppChain, int nX, int nY, DWORD dwCell,
	void *hTile);
PTDRLGTILE __fastcall RoomTileAddShadow(ROOM *ptRoom, PTDRLGTILE *ppChain, int nX, int nY, DWORD dwCell,
	void *hTile);
void __fastcall RoomTileFill(ROOM *ptRoom, DRLGGRID *ptWalls, DRLGGRID *ptOrient, BOOL fFill, BOOL fRight,
	BOOL fBottom);
void __fastcall RoomTileCountTiles(ROOM *ptRoom, DRLGGRID *ptGrid, BOOL fFill, BOOL fRight,
	BOOL fBottom);
void __fastcall RoomTileCountWalls(ROOM *ptRoom, DRLGGRID *ptWalls, DRLGGRID *ptOrient, BOOL fRight,
	BOOL fBottom);
void __fastcall RoomTileAllocBlock(ROOM *ptRoom);
void __fastcall RoomTileAllocArrays(ROOM *ptRoom);
void __fastcall RoomTileBuildA(ROOM *ptRoom);
void __fastcall RoomTileBuildB(ROOM *ptRoom);
void __fastcall RoomTileFree(ROOM *ptRoom);
void __fastcall RoomTileUnload(ROOM *ptRoom, BOOL fKeep);
void __fastcall RoomTileInit(ROOM *ptRoom);

#endif
