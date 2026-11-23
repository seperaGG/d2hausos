#ifndef MATCH_D2COMMON_IMPORTS_WRAPPERS_H
#define MATCH_D2COMMON_IMPORTS_WRAPPERS_H

#include "D2Common.imports.h"

#define D2CommonDrlgGetLevelType(nLevelId) \
	D2CommonOrdinal10000((DWORD)(nLevelId))

#define D2CommonDrlgGetActNum(dwUnused, nLevelId) \
	((BYTE)D2CommonOrdinal10001((DWORD)(nLevelId)))

#define D2CommonDrlgGetLevelLOSDraw(nLevelId) \
	D2CommonOrdinal10002((DWORD)(nLevelId))

#define D2CommonDrlgActUpdateRooms(ptAct) \
	D2CommonOrdinal10003((DWORD)(ptAct))

#define D2CommonDrlgGetLevelSaveMonsters(nLevelId) \
	D2CommonOrdinal10004((DWORD)(nLevelId))

#define D2CommonDrlgGetLevel(pDrlgAct, nLevelId) \
	D2CommonOrdinal10005((DWORD)(pDrlgAct), (DWORD)(nLevelId))

#define D2CommonDrlgGenerateLevel(ptLevel) \
	D2CommonOrdinal10006((DWORD)(ptLevel))

#define D2CommonDungeonRoomGetDrlgRoom(ptRoom) \
	((void *)D2CommonOrdinal10007((DWORD)(ptRoom)))

#define D2CommonPresetRoomGetInfoFirstDword(pRoom) \
	D2CommonOrdinal10008((DWORD)(pRoom))

#define D2CommonPresetRoomGetFile(pRoom) \
	((BYTE *)D2CommonOrdinal10009((DWORD)(pRoom)))

#define D2CommonLvlTblsGetLevelDef(eLevelID) \
	D2CommonOrdinal10010((DWORD)(eLevelID))

#define D2CommonLvlTblsGetAutomapCel(eLevelType, nTile, nStyle, nSequence) \
	((int)D2CommonOrdinal10011((DWORD)(eLevelType), (DWORD)(nTile), (DWORD)(nStyle), (DWORD)(nSequence)))

#define D2CommonDrlgFreeAct(ptAct) \
	D2CommonOrdinal10012((DWORD)(ptAct))

#define D2CommonDrlgNewLevel(ptAct, nLevelId) \
	((void *)D2CommonOrdinal10013((DWORD)(ptAct), (DWORD)(nLevelId)))

#define D2CommonDrlgNewAct(ptOwner, bAct, hArchive, dwMapSeed, nInitLevel, dwFlags, dwGame, pfnRoomBuilt, \
		pfnTownPlaced) \
	((void *)D2CommonOrdinal10014((DWORD)(ptOwner), (DWORD)(bAct), (DWORD)(hArchive), (DWORD)(dwMapSeed), \
		(DWORD)(nInitLevel), (DWORD)(dwFlags), (DWORD)(dwGame), (DWORD)(pfnRoomBuilt), \
		(DWORD)(pfnTownPlaced)))

#define D2CommonDrlgGetBuiltCounts(pdwClientBuilt, pdwClientCount, pdwServerBuilt, pdwServerCount) \
	D2CommonOrdinal10015((DWORD)(pdwClientBuilt), (DWORD)(pdwClientCount), (DWORD)(pdwServerBuilt), \
		(DWORD)(pdwServerCount))

#define D2CommonCollisnGetZero() \
	D2CommonOrdinal10016()

#define D2CommonRoomTileGetThirdArrayCount(ptRoom) \
	D2CommonOrdinal10017((DWORD)(ptRoom))

#define D2CommonRandomNothingZero() \
	D2CommonOrdinal10018()

#define D2CommonSkillsNothing(dwUnused1) \
	D2CommonOrdinal10019((DWORD)(dwUnused1))

#define D2CommonPresetFileListClone(pPool, pSource) \
	D2CommonOrdinal10020((DWORD)(pPool), (DWORD)(pSource))

#define D2CommonPresetInfoDetachFirstDword(pInfo, pdwOut) \
	D2CommonOrdinal10021((DWORD)(pInfo), (DWORD)(pdwOut))

#define D2CommonPresetFileListFree(pPool, pList) \
	D2CommonOrdinal10022((DWORD)(pPool), (DWORD)(pList))

#define D2CommonLvlTblsGetLevelTypeDef(nType) \
	D2CommonOrdinal10023((DWORD)(nType))

#define D2CommonLvlTblsGetPrest(nIndex) \
	D2CommonOrdinal10024((DWORD)(nIndex))

#define D2CommonDungeonRoomGetLevelWarpX(ptRoom) \
	((int *)D2CommonOrdinal10025((DWORD)(ptRoom)))

#define D2CommonDungeonRoomSetWarpsDefAllowed(ptRoom, nValue) \
	D2CommonOrdinal10026((DWORD)(ptRoom), (DWORD)(nValue))

#define D2CommonDungeonRoomGetWarpUnit(ptRoom, bExit, pptWarpDef) \
	((void *)D2CommonOrdinal10027((DWORD)(ptRoom), (DWORD)(bExit), (DWORD)(pptWarpDef)))

#define D2CommonDungeonRoomGetUnitWarpDef(ptRoom, ptUnit) \
	D2CommonOrdinal10028((DWORD)(ptRoom), (DWORD)(ptUnit))

#define D2CommonDungeonRoomGetUnitList(ptRoom) \
	D2CommonOrdinal10029((DWORD)(ptRoom))

#define D2CommonDungeonRoomGetSecondTileArray(ptRoom, pdwOut) \
	D2CommonOrdinal10030((DWORD)(ptRoom), (DWORD)(pdwOut))

#define D2CommonDungeonRoomGetFirstTileArray(ptRoom, pdwOut) \
	D2CommonOrdinal10031((DWORD)(ptRoom), (DWORD)(pdwOut))

#define D2CommonDungeonRoomGetThirdTileArray(ptRoom, pdwOut) \
	D2CommonOrdinal10032((DWORD)(ptRoom), (DWORD)(pdwOut))

#define D2CommonDungeonRoomCopyRects(ptRoom, ptCoords) \
	D2CommonOrdinal10033((DWORD)(ptRoom), (DWORD)(ptCoords))

#define D2CommonDungeonRoomGetNear(ptRoom, pppNear, pdwNumNear) \
	D2CommonOrdinal10034((DWORD)(ptRoom), (DWORD)(pppNear), (DWORD)(pdwNumNear))

#define D2CommonDungeonRoomGetPlayerUnits(ptRoom) \
	D2CommonOrdinal10035((DWORD)(ptRoom))

#define D2CommonDungeonActGetInitialLevelId(ptAct) \
	D2CommonOrdinal10036((DWORD)(ptAct))

#define D2CommonDungeonNewAct(bAct, dwMapSeed, dwClient, dwGame, pPool, nInitLevel, pfnRoomBuilt, \
		pfnTownPlaced) \
	D2CommonOrdinal10037((DWORD)(bAct), (DWORD)(dwMapSeed), (DWORD)(dwClient), (DWORD)(dwGame), \
		(DWORD)(pPool), (DWORD)(nInitLevel), (DWORD)(pfnRoomBuilt), (DWORD)(pfnTownPlaced))

#define D2CommonDungeonFreeAct(ptAct) \
	D2CommonOrdinal10038((DWORD)(ptAct))

#define D2CommonDungeonRoomsOverlap(ptFirst, ptSecond) \
	D2CommonOrdinal10039((DWORD)(ptFirst), (DWORD)(ptSecond))

#define D2CommonDungeonActFindRoomByPoint(hAct, nX, nY) \
	((void *)D2CommonOrdinal10040((DWORD)(hAct), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonDungeonFindRoomGame(hRoomDest, nX, nY) \
	((void *)D2CommonOrdinal10041((DWORD)(hRoomDest), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonDungeonRoomTestPointInSecondRect(hRoom, nX, nY) \
	D2CommonOrdinal10042((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY))

#define D2CommonDungeonRoomGetKindFlag(ptRoom) \
	D2CommonOrdinal10043((DWORD)(ptRoom))

#define D2CommonDungeonGetActStartRoom(ptAct, nLevelId, dwArg, pnX, pnY, dwArg6) \
	((void *)D2CommonOrdinal10044((DWORD)(ptAct), (DWORD)(nLevelId), (DWORD)(dwArg), (DWORD)(pnX), \
		(DWORD)(pnY), (DWORD)(dwArg6)))

#define D2CommonDungeonGetWarpRoom(ptAct, nLevelId, dwArg, pnX, pnY) \
	((void *)D2CommonOrdinal10045((DWORD)(ptAct), (DWORD)(nLevelId), (DWORD)(dwArg), (DWORD)(pnX), \
		(DWORD)(pnY)))

#define D2CommonDungeonActGetDrawnLevel(ptAct) \
	((int)D2CommonOrdinal10046((DWORD)(ptAct)))

#define D2CommonDungeonRoomGetSecondRectShape(pPrimary, dwUnused) \
	D2CommonOrdinal10047((DWORD)(pPrimary), (DWORD)(dwUnused))

#define D2CommonDungeonRoomForEachNear(ptRoom, pfnRoomCallback, pContext) \
	D2CommonOrdinal10048((DWORD)(ptRoom), (DWORD)(pfnRoomCallback), (DWORD)(pContext))

#define D2CommonDungeonRoomFindNearByPoint(ptRoom, nX, nY) \
	((void *)D2CommonOrdinal10049((DWORD)(ptRoom), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonDungeonTestRoomGame(hRoom, nX, nY) \
	((void *)D2CommonOrdinal10050((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonDungeonRoomGetScaledBounds(pRoom, prc) \
	D2CommonOrdinal10051((DWORD)(pRoom), (DWORD)(prc))

#define D2CommonDungeonRoomGetRectLTRB(pRoom, prc) \
	D2CommonOrdinal10052((DWORD)(pRoom), (DWORD)(prc))

#define D2CommonDungeonRoomGetLight(ptRoom, pbIntensity, pbRed, pbGreen, pbBlue) \
	D2CommonOrdinal10053((DWORD)(ptRoom), (DWORD)(pbIntensity), (DWORD)(pbRed), (DWORD)(pbGreen), \
		(DWORD)(pbBlue))

#define D2CommonDungeonRoomGetPresetUnits(ptRoom) \
	D2CommonOrdinal10054((DWORD)(ptRoom))

#define D2CommonDungeonActGetFirstRoom(ptAct) \
	((void *)D2CommonOrdinal10055((DWORD)(ptAct)))

#define D2CommonDungeonRoomGetLevelId(ptRoom) \
	D2CommonOrdinal10056((DWORD)(ptRoom))

#define D2CommonDungeonRoomGetVisLevel(ptRoom, nIndex) \
	((int)D2CommonOrdinal10057((DWORD)(ptRoom), (DWORD)(nIndex)))

#define D2CommonDungeonRoomGetLevelIdChecked(ptRoom) \
	D2CommonOrdinal10058((DWORD)(ptRoom))

#define D2CommonDungeonRoomHasExitFlags(ptRoom) \
	((BOOL)D2CommonOrdinal10059((DWORD)(ptRoom)))

#define D2CommonDungeonRoomGetFileName(ptRoom) \
	((const char *)D2CommonOrdinal10060((DWORD)(ptRoom)))

#define D2CommonDungeonRoomSwitch(ptFrom, ptTo) \
	D2CommonOrdinal10061(0, 0, (DWORD)(ptFrom), (DWORD)(ptTo))

#define D2CommonDungeonActFindRoomA(ptAct, dwLevelId, dwX, dwY, ptRoom) \
	D2CommonOrdinal10062((DWORD)(ptAct), (DWORD)(dwLevelId), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(ptRoom))

#define D2CommonDungeonActFindRoomB(ptAct, dwLevelId, dwX, dwY, ptRoom) \
	D2CommonOrdinal10063((DWORD)(ptAct), (DWORD)(dwLevelId), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(ptRoom))

#define D2CommonDungeonActGetLevelRoom(ptAct, dwX, dwY) \
	D2CommonOrdinal10064((DWORD)(ptAct), (DWORD)(dwX), (DWORD)(dwY))

#define D2CommonDungeonRoomAddNode(ptRoom, dwType, dwUnitId) \
	D2CommonOrdinal10065((DWORD)(ptRoom), (DWORD)(dwType), (DWORD)(dwUnitId))

#define D2CommonDungeonRoomFreeNodes(ptRoom) \
	D2CommonOrdinal10066((DWORD)(ptRoom))

#define D2CommonDungeonRoomGetNodes(ptRoom) \
	D2CommonOrdinal10067((DWORD)(ptRoom))

#define D2CommonDungeonActGetFirstActiveRoom(ptAct) \
	((void *)D2CommonOrdinal10068((DWORD)(ptAct)))

#define D2CommonDungeonActGetNextActiveRoom(ptAct, ptRoom) \
	((void *)D2CommonOrdinal10069((DWORD)(ptAct), (DWORD)(ptRoom)))

#define D2CommonDungeonRoomCanRelease(ptAct, ptRoom) \
	((BOOL)D2CommonOrdinal10070((DWORD)(ptAct), (DWORD)(ptRoom)))

#define D2CommonDungeonRoomIsIdle(ptRoom) \
	((BOOL)D2CommonOrdinal10071((DWORD)(ptRoom)))

#define D2CommonDungeonRoomNearAllFlagged(ptRoom) \
	((BOOL)D2CommonOrdinal10072((DWORD)(ptRoom)))

#define D2CommonDungeonRoomTestPopulated(ptRoom) \
	D2CommonOrdinal10073((DWORD)(ptRoom))

#define D2CommonDungeonRoomSetPopulated(ptRoom, fSet) \
	D2CommonOrdinal10074((DWORD)(ptRoom), (DWORD)(fSet))

#define D2CommonDungeonActRemoveRoom(ptAct, ptRoom) \
	D2CommonOrdinal10075((DWORD)(ptAct), (DWORD)(ptRoom))

#define D2CommonDungeonRoomChangeLevel(ptFrom, ptTo) \
	D2CommonOrdinal10076((DWORD)(ptFrom), (DWORD)(ptTo))

#define D2CommonDungeonActFreeIdleLevels(ptAct) \
	D2CommonOrdinal10077((DWORD)(ptAct))

#define D2CommonDungeonRoomAddClient(ptRoom, hClient) \
	D2CommonOrdinal10078((DWORD)(ptRoom), (DWORD)(hClient))

#define D2CommonDungeonRoomRemoveClient(ptRoom, hClient) \
	D2CommonOrdinal10079((DWORD)(ptRoom), (DWORD)(hClient))

#define D2CommonDungeonRoomTickEmptyCounter(ptRoom) \
	D2CommonOrdinal10080((DWORD)(ptRoom))

#define D2CommonDungeonRoomIsTown(ptRoom) \
	((BOOL)D2CommonOrdinal10081((DWORD)(ptRoom)))

#define D2CommonDungeonRoomIsNone(ptRoom) \
	((BOOL)D2CommonOrdinal10082((DWORD)(ptRoom)))

#define D2CommonDungeonRoomTestSkipsChangedList(ptRoom) \
	D2CommonOrdinal10083((DWORD)(ptRoom))

#define D2CommonDungeonActGetTownLevelId(bAct) \
	D2CommonOrdinal10084((DWORD)(bAct))

#define D2CommonDungeonLevelIsTown(nLevelId) \
	((BOOL)D2CommonOrdinal10085((DWORD)(nLevelId)))

#define D2CommonDungeonRoomGetOutdoorFlag(ptRoom) \
	D2CommonOrdinal10086((DWORD)(ptRoom))

#define D2CommonDungeonActGetDrlgAct(ptAct) \
	D2CommonOrdinal10087((DWORD)(ptAct))

#define D2CommonDungeonActGetMapSeed(ptAct) \
	D2CommonOrdinal10088((DWORD)(ptAct))

#define D2CommonDungeonActCountLevelRooms(ptAct, nLevelId) \
	((int)D2CommonOrdinal10089((DWORD)(ptAct), (DWORD)(nLevelId)))

#define D2CommonDungeonRoomShowWarpTiles(ptRoom, nExit) \
	D2CommonOrdinal10090((DWORD)(ptRoom), (DWORD)(nExit))

#define D2CommonDungeonRoomHideWarpTiles(ptRoom, nExit) \
	D2CommonOrdinal10091((DWORD)(ptRoom), (DWORD)(nExit))

#define D2CommonDungeonRoomUpdateDoors(ptRoom, nX, nY, fReopen) \
	D2CommonOrdinal10092((DWORD)(ptRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(fReopen))

#define D2CommonDungeonRoomGetPoints(ptRoom, pptPoints, pnCount) \
	D2CommonOrdinal10093((DWORD)(ptRoom), (DWORD)(pptPoints), (DWORD)(pnCount))

#define D2CommonDungeonGetLogicValue(ptRoom, nX, nY) \
	((int)D2CommonOrdinal10094((DWORD)(ptRoom), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonDungeonRoomGetLogicNode(ptRoom, nX, nY) \
	D2CommonOrdinal10095((DWORD)(ptRoom), (DWORD)(nX), (DWORD)(nY))

#define D2CommonDungeonRoomGetLogicList(ptRoom) \
	D2CommonOrdinal10096((DWORD)(ptRoom))

#define D2CommonDungeonGetPortalLevels(pPool, dwMask, ppnLevels, pnCount) \
	D2CommonOrdinal10097((DWORD)(pPool), (DWORD)(dwMask), (DWORD)(ppnLevels), (DWORD)(pnCount))

#define D2CommonDungeonGetPortalLevelBit(nLevelId) \
	D2CommonOrdinal10098((DWORD)(nLevelId))

#define D2CommonDungeonRoomIncPlayerUnits(ptRoom) \
	D2CommonOrdinal10099((DWORD)(ptRoom))

#define D2CommonDungeonRoomDecPlayerUnits(ptRoom) \
	D2CommonOrdinal10100((DWORD)(ptRoom))

#define D2CommonDungeonRoomSetHold(ptRoom, fClear) \
	D2CommonOrdinal10101((DWORD)(ptRoom), (DWORD)(fClear))

#define D2CommonDungeonActSetRoomMadeCallback(ptAct, dwValue) \
	D2CommonOrdinal10102((DWORD)(ptAct), (DWORD)(dwValue))

#define D2CommonDungeonRoomAnimateNear(ptRoom) \
	D2CommonOrdinal10103((DWORD)(ptRoom))

#define D2CommonDungeonActSyncAnimFrames(ptAct, ptFrom, ptTo) \
	D2CommonOrdinal10104(0, 0, (DWORD)(ptAct), (DWORD)(ptFrom), (DWORD)(ptTo))

#define D2CommonDungeonRoomRingBufferPush(hRoom, dwValue) \
	D2CommonOrdinal10105((DWORD)(hRoom), (DWORD)(dwValue))

#define D2CommonDungeonCalcScreenToTile(pnA, pnB) \
	D2CommonOrdinal10106((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonEnvScreenToTilePair(pnA, pnB) \
	D2CommonOrdinal10107((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonEnvScreenToTilePairRaw(pnA, pnB) \
	D2CommonOrdinal10108((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonDungeonCalcTileToScreen(pnA, pnB) \
	D2CommonOrdinal10109((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonDungeonCalcSubtileToScreen(pnA, pnB) \
	D2CommonOrdinal10110((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonDungeonCalcScaledSubtileToScreen(pnA, pnB) \
	D2CommonOrdinal10111((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonEnvTileToSubtile(pnA, pnB) \
	D2CommonOrdinal10112((DWORD)(pnA), (DWORD)(pnB))

#define D2CommonDungeonCalcScreenToTileFloor(nA, nB, pnOut1, pnOut2) \
	D2CommonOrdinal10113((DWORD)(nA), (DWORD)(nB), (DWORD)(pnOut1), (DWORD)(pnOut2))

#define D2CommonDungeonCalcTileToScreenOffset(nA, nB, pnOut1, pnOut2) \
	D2CommonOrdinal10114((DWORD)(nA), (DWORD)(nB), (DWORD)(pnOut1), (DWORD)(pnOut2))

#define D2CommonEnvComputeTileBoundsPair(nA, nB, pnOutC, pnOutD) \
	D2CommonOrdinal10115((DWORD)(nA), (DWORD)(nB), (DWORD)(pnOutC), (DWORD)(pnOutD))

#define D2CommonEnvComputeScreenOriginPair(nA, nB, pnOutC, pnOutD) \
	D2CommonOrdinal10116((DWORD)(nA), (DWORD)(nB), (DWORD)(pnOutC), (DWORD)(pnOutD))

#define D2CommonCollisnTestPoint(hRoom, nX, nY, dwPattern) \
	((WORD)D2CommonOrdinal10117((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(dwPattern)))

#define D2CommonCollisnTestSize(hRoom, nX, nY, nSize, dwPattern) \
	((WORD)D2CommonOrdinal10118((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nSize), \
		(DWORD)(dwPattern)))

#define D2CommonCollisnTestArea(hRoom, nX, nY, nWidth, nHeight, dwPattern) \
	((WORD)D2CommonOrdinal10119((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nWidth), (DWORD)(nHeight), \
		(DWORD)(dwPattern)))

#define D2CommonCollisnTestPattern(hRoom, nX, nY, ePattern, dwPattern) \
	((WORD)D2CommonOrdinal10120((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(ePattern), \
		(DWORD)(dwPattern)))

#define D2CommonCollisnTestPatternAny(hRoom, nX, nY, ePattern, dwPattern) \
	((WORD)D2CommonOrdinal10121((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(ePattern), \
		(DWORD)(dwPattern)))

#define D2CommonCollisnClearPoint(hRoom, nX, nY, dwPattern) \
	D2CommonOrdinal10122((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(dwPattern))

#define D2CommonCollisnClearSize(hRoom, nX, nY, nSize, dwPattern) \
	D2CommonOrdinal10123((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nSize), (DWORD)(dwPattern))

#define D2CommonCollisnClearArea(hRoom, nX, nY, nWidth, nHeight, dwPattern) \
	D2CommonOrdinal10124((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nWidth), (DWORD)(nHeight), \
		(DWORD)(dwPattern))

#define D2CommonCollisnClearPattern(hRoom, nX, nY, ePattern, dwPattern) \
	D2CommonOrdinal10125((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(ePattern), (DWORD)(dwPattern))

#define D2CommonCollisnSetPoint(hRoom, nX, nY, dwPattern) \
	D2CommonOrdinal10126((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(dwPattern))

#define D2CommonCollisnSetSize(hRoom, nX, nY, nSize, dwPattern) \
	D2CommonOrdinal10127((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nSize), (DWORD)(dwPattern))

#define D2CommonCollisnSetArea(hRoom, nX, nY, nWidth, nHeight, dwPattern) \
	D2CommonOrdinal10128((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nWidth), (DWORD)(nHeight), \
		(DWORD)(dwPattern))

#define D2CommonCollisnSetPattern(hRoom, nX, nY, ePattern, dwPattern) \
	D2CommonOrdinal10129((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(ePattern), (DWORD)(dwPattern))

#define D2CommonCollisnMoveSize(hRoom, nX, nY, nNewX, nNewY, nSize, dwPattern, dwTestPattern) \
	((WORD)D2CommonOrdinal10130((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nNewX), (DWORD)(nNewY), \
		(DWORD)(nSize), (DWORD)(dwPattern), (DWORD)(dwTestPattern)))

#define D2CommonCollisnMovePattern(hRoom, nX, nY, nNewX, nNewY, ePattern, dwPattern, dwTestPattern) \
	((WORD)D2CommonOrdinal10131((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(nNewX), (DWORD)(nNewY), \
		(DWORD)(ePattern), (DWORD)(dwPattern), (DWORD)(dwTestPattern)))

#define D2CommonCollisnDualUpdate(dwFromRoom, dwFromX, dwFromY, dwToRoom, dwToX, dwToY, dwCollisionPattern, \
		dwCollisionMask) \
	D2CommonOrdinal10132((DWORD)(dwFromRoom), (DWORD)(dwFromX), (DWORD)(dwFromY), (DWORD)(dwToRoom), \
		(DWORD)(dwToX), (DWORD)(dwToY), (DWORD)(dwCollisionPattern), (DWORD)(dwCollisionMask))

#define D2CommonCollisnFindSpawnPointNear(hRoom, ptSpawnPoint, nSize, dwPattern, fFallback) \
	((void *)D2CommonOrdinal10133((DWORD)(hRoom), (DWORD)(ptSpawnPoint), (DWORD)(nSize), (DWORD)(dwPattern), \
		(DWORD)(fFallback)))

#define D2CommonCollisnFindSpawnPointRadius(hRoom, ptSpawnPoint, nSize, dwPattern, fFallback, nMaxRadius) \
	((void *)D2CommonOrdinal10134((DWORD)(hRoom), (DWORD)(ptSpawnPoint), (DWORD)(nSize), (DWORD)(dwPattern), \
		(DWORD)(fFallback), (DWORD)(nMaxRadius)))

#define D2CommonCollisnFindFreeArea(hRoom, ptCoord, nSize, dwPattern, pptRoom) \
	D2CommonOrdinal10135((DWORD)(hRoom), (DWORD)(ptCoord), (DWORD)(nSize), (DWORD)(dwPattern), \
		(DWORD)(pptRoom))

#define D2CommonCollisnFindSpawnPointStep(hRoom, ptSpawnPoint, nSize, dwPattern, nStep) \
	((void *)D2CommonOrdinal10136((DWORD)(hRoom), (DWORD)(ptSpawnPoint), (DWORD)(nSize), (DWORD)(dwPattern), \
		(DWORD)(nStep)))

#define D2CommonCollisnFindSpawnPointField(hRoom, ptSpawnPoint, ptField, nSize, dwPattern, dwFieldPattern, \
		fFallback) \
	((void *)D2CommonOrdinal10137((DWORD)(hRoom), (DWORD)(ptSpawnPoint), (DWORD)(ptField), (DWORD)(nSize), \
		(DWORD)(dwPattern), (DWORD)(dwFieldPattern), (DWORD)(fFallback)))

#define D2CommonPathNothingThird() \
	D2CommonOrdinal10138()

#define D2CommonPathNothingFourth() \
	D2CommonOrdinal10139()

#define D2CommonPathGetWaypointSquare(hUnit, pnX, pnY) \
	D2CommonOrdinal10140((DWORD)(hUnit), (DWORD)(pnX), (DWORD)(pnY))

#define D2CommonPathCompute(ptPath, hUnit) \
	D2CommonOrdinal10141((DWORD)(ptPath), (DWORD)(hUnit))

#define D2CommonPathReplaceCollisionMask(hUnit, fForce) \
	D2CommonOrdinal10142((DWORD)(hUnit), (DWORD)(fForce))

#define D2CommonPathSetCollisionTracking(ptPath, fSet) \
	D2CommonOrdinal10143((DWORD)(ptPath), (DWORD)(fSet))

#define D2CommonPathSetVelocity(ptPath, dwValue) \
	D2CommonOrdinal10144((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathGetVelocity(ptPath) \
	D2CommonOrdinal10145((DWORD)(ptPath))

#define D2CommonPathSetMaxVelocity(ptPath, dwValue) \
	D2CommonOrdinal10146((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathGetMaxVelocity(ptPath) \
	D2CommonOrdinal10147((DWORD)(ptPath))

#define D2CommonPathSetAccel(ptPath, dwValue) \
	D2CommonOrdinal10148((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathGetAccel(ptPath) \
	D2CommonOrdinal10149((DWORD)(ptPath))

#define D2CommonPathCreate(pPool, dwRoom, dwX, dwY, hUnit, fFlag) \
	D2CommonOrdinal10150((DWORD)(pPool), (DWORD)(dwRoom), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(hUnit), \
		(DWORD)(fFlag))

#define D2CommonPathClearSteps(ptPath) \
	D2CommonOrdinal10151((DWORD)(ptPath))

#define D2CommonPathGetStepCount(ptPath) \
	D2CommonOrdinal10152((DWORD)(ptPath))

#define D2CommonPathGetStepIndex(ptPath) \
	D2CommonOrdinal10153((DWORD)(ptPath))

#define D2CommonPathFreePool(pPool, p) \
	D2CommonOrdinal10154((DWORD)(pPool), (DWORD)(p))

#define D2CommonPathGetSteps(ptPath, ppSteps) \
	D2CommonOrdinal10155((DWORD)(ptPath), (DWORD)(ppSteps))

#define D2CommonPathGetFacing(ptPath) \
	((BYTE)D2CommonOrdinal10156((DWORD)(ptPath)))

#define D2CommonPathGetTargetFacing(ptPath) \
	((BYTE)D2CommonOrdinal10157((DWORD)(ptPath)))

#define D2CommonPathSetFacingSmoothed(ptPath, bDir) \
	D2CommonOrdinal10158((DWORD)(ptPath), (DWORD)(bDir))

#define D2CommonPathSetFacingImmediate(ptPath, bDir) \
	D2CommonOrdinal10159((DWORD)(ptPath), (DWORD)(bDir))

#define D2CommonPathGetGameX(ptPath) \
	D2CommonOrdinal10160((DWORD)(ptPath))

#define D2CommonPathGetGameY(ptPath) \
	D2CommonOrdinal10161((DWORD)(ptPath))

#define D2CommonPathGetScreenX(ptPath) \
	D2CommonOrdinal10162((DWORD)(ptPath))

#define D2CommonPathGetScreenY(ptPath) \
	D2CommonOrdinal10163((DWORD)(ptPath))

#define D2CommonPathGetRoom(ptPath) \
	D2CommonOrdinal10164((DWORD)(ptPath))

#define D2CommonPathSetRoom(ptPath, hRoom) \
	D2CommonOrdinal10165((DWORD)(ptPath), (DWORD)(hRoom))

#define D2CommonPathGetPrevRoom(ptPath) \
	D2CommonOrdinal10166((DWORD)(ptPath))

#define D2CommonPathClearPrevRoom(ptPath) \
	D2CommonOrdinal10167((DWORD)(ptPath))

#define D2CommonPathSetTargetCoord(ptPath, wX, wY) \
	D2CommonOrdinal10168((DWORD)(ptPath), (DWORD)(wX), (DWORD)(wY))

#define D2CommonPathGetTargetSnapshot(ptPath, pdwTargetType, pdwTargetId) \
	D2CommonOrdinal10169((DWORD)(ptPath), (DWORD)(pdwTargetType), (DWORD)(pdwTargetId))

#define D2CommonPathTestLeftRoom(ptPath) \
	D2CommonOrdinal10170((DWORD)(ptPath))

#define D2CommonPathSetLeftRoom(ptPath, fSet) \
	D2CommonOrdinal10171((DWORD)(ptPath), (DWORD)(fSet))

#define D2CommonPathTestCollisionTracking(ptPath) \
	D2CommonOrdinal10172((DWORD)(ptPath))

#define D2CommonPathGetTargetX(ptPath) \
	((WORD)D2CommonOrdinal10173((DWORD)(ptPath)))

#define D2CommonPathGetTargetY(ptPath) \
	((WORD)D2CommonOrdinal10174((DWORD)(ptPath)))

#define D2CommonPathGetStepX(ptPath) \
	((WORD)D2CommonOrdinal10175((DWORD)(ptPath)))

#define D2CommonPathGetStepY(ptPath) \
	((WORD)D2CommonOrdinal10176((DWORD)(ptPath)))

#define D2CommonPathSetTarget(ptPath, hTarget) \
	D2CommonOrdinal10177((DWORD)(ptPath), (DWORD)(hTarget))

#define D2CommonPathGetTarget(ptPath) \
	D2CommonOrdinal10178((DWORD)(ptPath))

#define D2CommonPathGetCollisionMask(ptPath) \
	D2CommonOrdinal10179((DWORD)(ptPath))

#define D2CommonPathSetCollisionMask(ptPath, dwValue) \
	D2CommonOrdinal10180((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathSetCollisionTestMask(ptPath, dwValue) \
	D2CommonOrdinal10181((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathSetType(ptPath, eType) \
	D2CommonOrdinal10182((DWORD)(ptPath), (DWORD)(eType))

#define D2CommonPathRestoreType(ptPath) \
	D2CommonOrdinal10183((DWORD)(ptPath))

#define D2CommonPathGetType(ptPath) \
	D2CommonOrdinal10184((DWORD)(ptPath))

#define D2CommonPathSetDistMax(ptPath, bDist) \
	D2CommonOrdinal10185((DWORD)(ptPath), (DWORD)(bDist))

#define D2CommonPathGetDistMax(ptPath) \
	((BYTE)D2CommonOrdinal10186((DWORD)(ptPath)))

#define D2CommonPathSetSquareCountdown(ptPath, bValue) \
	D2CommonOrdinal10187((DWORD)(ptPath), (DWORD)(bValue))

#define D2CommonPathGetSquareCountdown(ptPath) \
	((BYTE)D2CommonOrdinal10188((DWORD)(ptPath)))

#define D2CommonPathSetSearchReach(ptPath, bValue) \
	D2CommonOrdinal10189((DWORD)(ptPath), (DWORD)(bValue))

#define D2CommonPathStepFacing(ptPath) \
	D2CommonOrdinal10190((DWORD)(ptPath))

#define D2CommonPathGetRawX(ptPath) \
	D2CommonOrdinal10191((DWORD)(ptPath))

#define D2CommonPathGetRawY(ptPath) \
	D2CommonOrdinal10192((DWORD)(ptPath))

#define D2CommonPathSetRawX(ptPath, dwValue) \
	D2CommonOrdinal10193((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathSetRawY(ptPath, dwValue) \
	D2CommonOrdinal10194((DWORD)(ptPath), (DWORD)(dwValue))

#define D2CommonPathGetSavedSteps(ptPath, ppSteps2) \
	D2CommonOrdinal10195((DWORD)(ptPath), (DWORD)(ppSteps2))

#define D2CommonPathGetSavedStepX(ptPath) \
	((WORD)D2CommonOrdinal10196((DWORD)(ptPath)))

#define D2CommonPathGetSavedStepY(ptPath) \
	((WORD)D2CommonOrdinal10197((DWORD)(ptPath)))

#define D2CommonPathGetLineOfSight(ptPath) \
	((WORD)D2CommonOrdinal10198((DWORD)(ptPath)))

#define D2CommonPathGetCachedCollision(ptPath) \
	((WORD)D2CommonOrdinal10199((DWORD)(ptPath)))

#define D2CommonPathSetFacingFree(ptPath, fFree) \
	D2CommonOrdinal10200((DWORD)(ptPath), (DWORD)(fFree))

#define D2CommonPathClearKeptTarget(ptPath) \
	D2CommonOrdinal10201((DWORD)(ptPath))

#define D2CommonPathSetStopDistance(ptPath, bValue) \
	D2CommonOrdinal10202((DWORD)(ptPath), (DWORD)(bValue))

#define D2CommonPathGetStopDistance(ptPath) \
	((BYTE)D2CommonOrdinal10203((DWORD)(ptPath)))

#define D2CommonPathSetTargetOffset(ptPath, bValue67, bValue68) \
	D2CommonOrdinal10204((DWORD)(ptPath), (DWORD)(bValue67), (DWORD)(bValue68))

#define D2CommonPathSetMoving(ptPath, fSet) \
	D2CommonOrdinal10205((DWORD)(ptPath), (DWORD)(fSet))

#define D2CommonPathTestMoving(ptPath) \
	D2CommonOrdinal10206((DWORD)(ptPath))

#define D2CommonPathGetCollisionPatternByUnit(hUnit) \
	D2CommonOrdinal10207((DWORD)(hUnit))

#define D2CommonPathSetCollisionPatternByUnit(hUnit, dwValue) \
	D2CommonOrdinal10208((DWORD)(hUnit), (DWORD)(dwValue))

#define D2CommonPathUnitSetPeriodicRecalc(hUnit, fSet) \
	D2CommonOrdinal10209((DWORD)(hUnit), (DWORD)(fSet))

#define D2CommonPathUnitPeriodicRecalc(hUnit) \
	D2CommonOrdinal10210((DWORD)(hUnit))

#define D2CommonPathResetCollisionPattern(hUnit) \
	D2CommonOrdinal10211((DWORD)(hUnit))

#define D2CommonStepGetSquareDirection(nX1, nY1, nX2, nY2) \
	((int)D2CommonOrdinal10212((DWORD)(nX1), (DWORD)(nY1), (DWORD)(nX2), (DWORD)(nY2)))

#define D2CommonPathTurnToSquare(ptPath, nX, nY, fImmediate) \
	D2CommonOrdinal10213((DWORD)(ptPath), (DWORD)(nX), (DWORD)(nY), (DWORD)(fImmediate))

#define D2CommonPathSetRecalcBudget(ptPath, nDist) \
	D2CommonOrdinal10214((DWORD)(ptPath), (DWORD)(nDist))

#define D2CommonPathGetRecalcBudget(ptPath) \
	((BYTE)D2CommonOrdinal10215((DWORD)(ptPath)))

#define D2CommonPathAddRecalcBudget(ptPath, nDelta) \
	D2CommonOrdinal10216((DWORD)(ptPath), (DWORD)(nDelta))

#define D2CommonPathDistanceSquared(nX1, nY1, nX2, nY2) \
	((int)D2CommonOrdinal10217((DWORD)(nX1), (DWORD)(nY1), (DWORD)(nX2), (DWORD)(nY2)))

#define D2CommonPathFreeIfSet(hUnit) \
	D2CommonOrdinal10218((DWORD)(hUnit))

#define D2CommonPathSetCollisionByType(hUnit) \
	D2CommonOrdinal10219((DWORD)(hUnit))

#define D2CommonPathClearCollisionByType(hUnit, fFlag) \
	((BOOL)D2CommonOrdinal10220((DWORD)(hUnit), (DWORD)(fFlag)))

#define D2CommonPathGetEndX(ptPath) \
	((WORD)D2CommonOrdinal10221((DWORD)(ptPath)))

#define D2CommonPathGetEndY(ptPath) \
	((WORD)D2CommonOrdinal10222((DWORD)(ptPath)))

#define D2CommonStepUnitWalk(hUnit, nSpeed) \
	((BOOL)D2CommonOrdinal10223((DWORD)(hUnit), (DWORD)(nSpeed)))

#define D2CommonStepUnitTestAtTarget(ptUnit) \
	((BOOL)D2CommonOrdinal10224((DWORD)(ptUnit)))

#define D2CommonPathClearIfExists(hUnit) \
	D2CommonOrdinal10225((DWORD)(hUnit))

#define D2CommonStepTeleportUnit(ptPath, dwUnused, hRoomDest, nX, nY) \
	((BOOL)D2CommonOrdinal10226((DWORD)(ptPath), (DWORD)(dwUnused), (DWORD)(hRoomDest), (DWORD)(nX), \
		(DWORD)(nY)))

#define D2CommonStepTeleportUnitForceRecalc(ptPath, dwUnused, hRoomDest, nX, nY) \
	((BOOL)D2CommonOrdinal10227((DWORD)(ptPath), (DWORD)(dwUnused), (DWORD)(hRoomDest), (DWORD)(nX), \
		(DWORD)(nY)))

#define D2CommonStepPlaceUnit(ptPath, dwUnused, hRoomDest, nX, nY) \
	((BOOL)D2CommonOrdinal10228((DWORD)(ptPath), (DWORD)(dwUnused), (DWORD)(hRoomDest), (DWORD)(nX), \
		(DWORD)(nY)))

#define D2CommonStepMoveUnit(ptPath, hUnit, hRoomDest, nX, nY) \
	((BOOL)D2CommonOrdinal10229((DWORD)(ptPath), (DWORD)(hUnit), (DWORD)(hRoomDest), (DWORD)(nX), \
		(DWORD)(nY)))

#define D2CommonStepSnapToSquare(ptPath) \
	D2CommonOrdinal10230((DWORD)(ptPath))

#define D2CommonStepTestSquareCrossed(ptPath) \
	D2CommonOrdinal10231((DWORD)(ptPath))

#define D2CommonStepRideHorse(hRiderUnit, hHorseUnit) \
	((DWORD (__stdcall *)(void *, void *))D2CommonOrdinal10232)((void *)(hRiderUnit), (void *)(hHorseUnit))

#define D2CommonStepUnitRecalcPath(ptUnit, dwHasTarget) \
	D2CommonOrdinal10233((DWORD)(ptUnit), (DWORD)(dwHasTarget))

#define D2CommonPathTestHasMoreSteps(hUnit) \
	((BOOL)D2CommonOrdinal10234((DWORD)(hUnit)))

#define D2CommonPathNothingFirst() \
	D2CommonOrdinal10235()

#define D2CommonPathNothingZero(dwUnused1, dwUnused2) \
	D2CommonOrdinal10236((DWORD)(dwUnused1), (DWORD)(dwUnused2))

#define D2CommonINVENTORYCreate(pPool, dwUnitId, dwUnitType, dwClassId, dwSkipLocInfo) \
	D2CommonOrdinal10237((DWORD)(pPool), (DWORD)(dwUnitId), (DWORD)(dwUnitType), (DWORD)(dwClassId), \
		(DWORD)(dwSkipLocInfo))

#define D2CommonINVENTORYDestroy(ptInventory) \
	D2CommonOrdinal10238((DWORD)(ptInventory))

#define D2CommonINVENTORYAddItem(ptInventory, hItem, dwCategory) \
	((BOOL)D2CommonOrdinal10239((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(dwCategory)))

#define D2CommonINVENTORYRemoveItem(ptInventory, hItemKey) \
	D2CommonOrdinal10240((DWORD)(ptInventory), (DWORD)(hItemKey))

#define D2CommonINVENTORYTestHasItem(ptInventory, hItemKey) \
	((BOOL)D2CommonOrdinal10241((DWORD)(ptInventory), (DWORD)(hItemKey)))

#define D2CommonINVENTORYFindFreeSpace(ptInventory, hItem, nRecord, pnX, pnY, bPage) \
	((BOOL)D2CommonOrdinal10242((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nRecord), (DWORD)(pnX), \
		(DWORD)(pnY), (DWORD)(bPage)))

#define D2CommonINVENTORYPlaceItem(ptInventory, hItem, nRecord, dwClient, bPage) \
	((BOOL)D2CommonOrdinal10243((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nRecord), (DWORD)(dwClient), \
		(DWORD)(bPage)))

#define D2CommonINVENTORYTestPlaceAt(ptInventory, hItem, nX, nY, nRecord, phItemFound, pnCount, bPage) \
	((BOOL)D2CommonOrdinal10244((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nX), (DWORD)(nY), \
		(DWORD)(nRecord), (DWORD)(phItemFound), (DWORD)(pnCount), (DWORD)(bPage)))

#define D2CommonINVENTORYTestSwapAt(ptInventory, hItem, nX, nY, nRecord, phItemFound, bPage) \
	((BOOL)D2CommonOrdinal10245((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nX), (DWORD)(nY), \
		(DWORD)(nRecord), (DWORD)(phItemFound), (DWORD)(bPage)))

#define D2CommonINVENTORYPlaceItemAt(ptInventory, hItem, nX, nY, nRecord, dwClient, bPage) \
	((BOOL)D2CommonOrdinal10246((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nX), (DWORD)(nY), \
		(DWORD)(nRecord), (DWORD)(dwClient), (DWORD)(bPage)))

#define D2CommonINVENTORYRemoveItemAt(ptInventory, nX, nY, nRecord, dwClient, bPage) \
	D2CommonOrdinal10247((DWORD)(ptInventory), (DWORD)(nX), (DWORD)(nY), (DWORD)(nRecord), \
		(DWORD)(dwClient), (DWORD)(bPage))

#define D2CommonINVENTORYClearGrid(ptInventory) \
	D2CommonOrdinal10248((DWORD)(ptInventory))

#define D2CommonINVENTORYGetItemAt(ptInventory, nX, nY, pnItemX, pnItemY, nRecord, bPage) \
	D2CommonOrdinal10249((DWORD)(ptInventory), (DWORD)(nX), (DWORD)(nY), (DWORD)(pnItemX), (DWORD)(pnItemY), \
		(DWORD)(nRecord), (DWORD)(bPage))

#define D2CommonINVENTORYSetLocSlot(ptInventory, hValue, nIndex) \
	((BOOL)D2CommonOrdinal10250((DWORD)(ptInventory), (DWORD)(hValue), (DWORD)(nIndex)))

#define D2CommonINVENTORYClearLocSlot(ptInventory, nIndex) \
	D2CommonOrdinal10251((DWORD)(ptInventory), (DWORD)(nIndex))

#define D2CommonINVENTORYGetEquipField(dwPlayer, ptInventory, pdwOut, dwSlotSel) \
	D2CommonOrdinal10252((DWORD)(dwPlayer), (DWORD)(ptInventory), (DWORD)(pdwOut), (DWORD)(dwSlotSel))

#define D2CommonINVENTORYTestEquipCompat(hInventory, dwWeaponClass) \
	((BOOL)D2CommonOrdinal10253((DWORD)(hInventory), (DWORD)(dwWeaponClass)))

#define D2CommonINVENTORYGetLocSlot(ptInventory, nIndex) \
	D2CommonOrdinal10254((DWORD)(ptInventory), (DWORD)(nIndex))

#define D2CommonINVENTORYFindEquippedMatch(ptInventory) \
	D2CommonOrdinal10255((DWORD)(ptInventory))

#define D2CommonINVENTORYGetBodyLocItem(ptInventory, nLoc) \
	D2CommonOrdinal10256((DWORD)(ptInventory), (DWORD)(nLoc))

#define D2CommonINVENTORYFindSlotIndex(ptInventory, hItem) \
	((int)D2CommonOrdinal10257((DWORD)(ptInventory), (DWORD)(hItem)))

#define D2CommonINVENTORYSetCursorItem(ptInventory, dwValue) \
	D2CommonOrdinal10258((DWORD)(ptInventory), (DWORD)(dwValue))

#define D2CommonINVENTORYGetCursorItem(ptInventory) \
	D2CommonOrdinal10259((DWORD)(ptInventory))

#define D2CommonINVENTORYFindGridStack(ptInventory, dwWantedKind, hExclude) \
	D2CommonOrdinal10260((DWORD)(ptInventory), (DWORD)(dwWantedKind), (DWORD)(hExclude))

#define D2CommonINVENTORYFindEquippedStack(ptInventory, dwWantedKind, hExclude) \
	D2CommonOrdinal10261((DWORD)(ptInventory), (DWORD)(dwWantedKind), (DWORD)(hExclude))

#define D2CommonINVENTORYFindGridBook(ptInventory, hItem2, hExclude) \
	D2CommonOrdinal10262((DWORD)(ptInventory), (DWORD)(hItem2), (DWORD)(hExclude))

#define D2CommonINVENTORYPlaceBeltItem(ptInventory, hItem, nSlot) \
	((BOOL)D2CommonOrdinal10263((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nSlot)))

#define D2CommonINVENTORYClearBeltSlot(ptInventory, nSlot) \
	D2CommonOrdinal10264((DWORD)(ptInventory), (DWORD)(nSlot))

#define D2CommonINVENTORYTestItemFits(ptInventory, hItem) \
	D2CommonOrdinal10265((DWORD)(ptInventory), (DWORD)(hItem))

#define D2CommonINVENTORYFindBeltSlot(ptInventory, hItem, pnSlot) \
	((BOOL)D2CommonOrdinal10266((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(pnSlot)))

#define D2CommonINVENTORYAddBeltItem(ptInventory, hItem) \
	((BOOL)D2CommonOrdinal10267((DWORD)(ptInventory), (DWORD)(hItem)))

#define D2CommonINVENTORYGetBeltSlot(ptInventory, nSlot) \
	D2CommonOrdinal10268((DWORD)(ptInventory), (DWORD)(nSlot))

#define D2CommonINVENTORYGetBeltSlotFor(ptInventory, hItem, nSlot, phItem) \
	((BOOL)D2CommonOrdinal10269((DWORD)(ptInventory), (DWORD)(hItem), (DWORD)(nSlot), (DWORD)(phItem)))

#define D2CommonINVENTORYFindShieldInHands(ptInventory, phItem) \
	((BOOL)D2CommonOrdinal10270((DWORD)(ptInventory), (DWORD)(phItem)))

#define D2CommonINVENTORYGetWeapon(ptInventory, phItem, pnLoc, pfPrimaryWeapon) \
	((BOOL)D2CommonOrdinal10271((DWORD)(ptInventory), (DWORD)(phItem), (DWORD)(pnLoc), \
		(DWORD)(pfPrimaryWeapon)))

#define D2CommonINVENTORYTestTorsoArmorWorn(ptInventory) \
	((BOOL)D2CommonOrdinal10272((DWORD)(ptInventory)))

#define D2CommonINVENTORYTestSlotFits(ptInventory, dwItem, nSlotIndex, dwRecord) \
	((BOOL)D2CommonOrdinal10273((DWORD)(ptInventory), (DWORD)(dwItem), (DWORD)(nSlotIndex), \
		(DWORD)(dwRecord)))

#define D2CommonINVENTORYGetCategoryListHead(ptInventory) \
	((void *)D2CommonOrdinal10274((DWORD)(ptInventory)))

#define D2CommonINVENTORYGetCategoryListHeadAlt(ptInventory) \
	((void *)D2CommonOrdinal10275((DWORD)(ptInventory)))

#define D2CommonINVENTORYClearCategoryList(ptInventory) \
	D2CommonOrdinal10276((DWORD)(ptInventory))

#define D2CommonINVENTORYGetDeltaListHead(ptInventory) \
	((void *)D2CommonOrdinal10277((DWORD)(ptInventory)))

#define D2CommonINVENTORYClearDeltaList(ptInventory) \
	D2CommonOrdinal10278((DWORD)(ptInventory))

#define D2CommonINVENTORYAddDeltaNode(ptInventory, hItem) \
	((void *)D2CommonOrdinal10279((DWORD)(ptInventory), (DWORD)(hItem)))

#define D2CommonINVENTORYGetCategoryListCount(ptInventory) \
	D2CommonOrdinal10280((DWORD)(ptInventory))

#define D2CommonINVENTORYFindGridItemByType(ptInventory, dwItemType, hExclude) \
	D2CommonOrdinal10281((DWORD)(ptInventory), (DWORD)(dwItemType), (DWORD)(hExclude))

#define D2CommonINVENTORYFindEquippedByType(ptInventory, dwItemType, hExclude) \
	D2CommonOrdinal10282((DWORD)(ptInventory), (DWORD)(dwItemType), (DWORD)(hExclude))

#define D2CommonINVENTORYFindEquippedByCode(ptInventory, dwCode, hExclude) \
	D2CommonOrdinal10283((DWORD)(ptInventory), (DWORD)(dwCode), (DWORD)(hExclude))

#define D2CommonINVENTORYCountEquippedSetItems(ptInventory, dwSetRow) \
	D2CommonOrdinal10284((DWORD)(ptInventory), (DWORD)(dwSetRow))

#define D2CommonINVENTORYOfferAddDelta(hInventory, hAddItem) \
	D2CommonOrdinal10285((DWORD)(hInventory), (DWORD)(hAddItem))

#define D2CommonINVENTORYOfferRemoveDelta(hInventory, hRemoveItem) \
	D2CommonOrdinal10286((DWORD)(hInventory), (DWORD)(hRemoveItem))

#define D2CommonINVENTORYTestCanEquipSlot(hPlayer, ptInventory) \
	((int)D2CommonOrdinal10287((DWORD)(hPlayer), (DWORD)(ptInventory)))

#define D2CommonINVENTORYSetOwnerId(ptInventory, dwValue) \
	D2CommonOrdinal10288((DWORD)(ptInventory), (DWORD)(dwValue))

#define D2CommonINVENTORYGetOwnerId(ptInventory) \
	D2CommonOrdinal10289((DWORD)(ptInventory))

#define D2CommonINVENTORYAppendTrackedNode(ptInventory, dwKey, dwHire, dwKind) \
	D2CommonOrdinal10290((DWORD)(ptInventory), (DWORD)(dwKey), (DWORD)(dwHire), (DWORD)(dwKind))

#define D2CommonINVENTORYRemoveTrackedNode(ptInventory, dwKey, dwKind) \
	((BOOL)D2CommonOrdinal10291((DWORD)(ptInventory), (DWORD)(dwKey), (DWORD)(dwKind)))

#define D2CommonINVENTORYGetTrackedListHead(ptInventory) \
	((void *)D2CommonOrdinal10292((DWORD)(ptInventory)))

#define D2CommonINVENTORYGetTrackedListCount(ptInventory) \
	D2CommonOrdinal10293((DWORD)(ptInventory))

#define D2CommonINVENTORYWriteSaveHeaderItems(hUnit, ptSaveHeader) \
	D2CommonOrdinal10294((DWORD)(hUnit), (DWORD)(ptSaveHeader))

#define D2CommonINVENTORYTestBodyLocPlace(hUnit, nLoc, hItem, fNoFitTest) \
	((int)D2CommonOrdinal10295((DWORD)(hUnit), (DWORD)(nLoc), (DWORD)(hItem), (DWORD)(fNoFitTest)))

#define D2CommonINVENTORYCountFreeCells(hUnit) \
	((int)D2CommonOrdinal10296((DWORD)(hUnit)))

#define D2CommonINVENTORYTestTradeSpace(pPool, hUnit1, hUnit2, pfFirstFits) \
	((BOOL)D2CommonOrdinal10297((DWORD)(pPool), (DWORD)(hUnit1), (DWORD)(hUnit2), (DWORD)(pfFirstFits)))

#define D2CommonINVENTORYNothing(dwUnused1, dwUnused2) \
	D2CommonOrdinal10298((DWORD)(dwUnused1), (DWORD)(dwUnused2))

#define D2CommonINVENTORYNodeGetNext(ptNode) \
	D2CommonOrdinal10299((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetKeyA(ptNode) \
	D2CommonOrdinal10300((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetItemId(ptNode) \
	D2CommonOrdinal10301((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetCategory(ptNode) \
	D2CommonOrdinal10302((DWORD)(ptNode))

#define D2CommonINVENTORYNodeSetAcceptedIfWeapon(ptNode, dwValue) \
	D2CommonOrdinal10303((DWORD)(ptNode), (DWORD)(dwValue))

#define D2CommonINVENTORYNodeGetAccepted(ptNode) \
	D2CommonOrdinal10304((DWORD)(ptNode))

#define D2CommonINVENTORYListFindNode(ptContainer, hItem) \
	D2CommonOrdinal10305((DWORD)(ptContainer), (DWORD)(hItem))

#define D2CommonINVENTORYNodeGetKeyB(ptNode) \
	D2CommonOrdinal10306((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetDeltaItemId(ptNode) \
	D2CommonOrdinal10307((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetTrackedNext(ptNode) \
	D2CommonOrdinal10308((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetTrackedKey(ptNode) \
	D2CommonOrdinal10309((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetTrackedData(ptNode) \
	D2CommonOrdinal10310((DWORD)(ptNode))

#define D2CommonINVENTORYNodeGetTrackedFlag(ptNode) \
	D2CommonOrdinal10311((DWORD)(ptNode))

#define D2CommonUnitTestMonsterCombinedFlag(hUnit) \
	D2CommonOrdinal10312((DWORD)(hUnit))

#define D2CommonUnitTestModeGroup(hUnit) \
	((BOOL)D2CommonOrdinal10313((DWORD)(hUnit)))

#define D2CommonUnitGetThreeIfInModeGroup(hUnit) \
	D2CommonOrdinal10314((DWORD)(hUnit))

#define D2CommonUnitGetFirstSkill(hUnit) \
	D2CommonOrdinal10315((DWORD)(hUnit))

#define D2CommonUnitGetLeftSkill(hUnit) \
	D2CommonOrdinal10316((DWORD)(hUnit))

#define D2CommonUnitGetRightSkill(hUnit) \
	D2CommonOrdinal10317((DWORD)(hUnit))

#define D2CommonUnitGetSkillThirdCached(hUnit) \
	D2CommonOrdinal10318((DWORD)(hUnit))

#define D2CommonUnitSetSkillThirdCached(hUnit, ptSkill) \
	D2CommonOrdinal10319((DWORD)(hUnit), (DWORD)(ptSkill))

#define D2CommonUnitSetAnimRecord(ptUnit, eType, nClass, nMode) \
	D2CommonOrdinal10320((DWORD)(ptUnit), (DWORD)(eType), (DWORD)(nClass), (DWORD)(nMode))

#define D2CommonUnitGetX(ptUnit) \
	D2CommonOrdinal10321((DWORD)(ptUnit))

#define D2CommonUnitGetXFixed(ptUnit) \
	D2CommonOrdinal10322((DWORD)(ptUnit))

#define D2CommonUnitSetX(ptUnit, dwX) \
	D2CommonOrdinal10323((DWORD)(ptUnit), (DWORD)(dwX))

#define D2CommonUnitGetY(ptUnit) \
	D2CommonOrdinal10324((DWORD)(ptUnit))

#define D2CommonUnitGetYFixed(ptUnit) \
	D2CommonOrdinal10325((DWORD)(ptUnit))

#define D2CommonUnitSetY(ptUnit, dwY) \
	D2CommonOrdinal10326((DWORD)(ptUnit), (DWORD)(dwY))

#define D2CommonUnitGetPosition(ptUnit, ptOut) \
	D2CommonOrdinal10327((DWORD)(ptUnit), (DWORD)(ptOut))

#define D2CommonUnitGetScreenX(ptUnit) \
	D2CommonOrdinal10328((DWORD)(ptUnit))

#define D2CommonUnitGetScreenY(ptUnit) \
	D2CommonOrdinal10329((DWORD)(ptUnit))

#define D2CommonUnitGetScreenPosition(ptUnit, ptOut) \
	D2CommonOrdinal10330((DWORD)(ptUnit), (DWORD)(ptOut))

#define D2CommonUnitGetSizeX(ptUnit) \
	D2CommonOrdinal10331((DWORD)(ptUnit))

#define D2CommonUnitGetSizeY(ptUnit) \
	D2CommonOrdinal10332((DWORD)(ptUnit))

#define D2CommonUnitGetTypeFlags(ptUnit) \
	D2CommonOrdinal10333((DWORD)(ptUnit))

#define D2CommonUnitCalcGoldBankLimit(dwArg) \
	D2CommonOrdinal10334((DWORD)(dwArg))

#define D2CommonUnitSetScreenX(ptUnit, dwValue) \
	D2CommonOrdinal10335((DWORD)(ptUnit), (DWORD)(dwValue))

#define D2CommonUnitSetScreenY(ptUnit, dwValue) \
	D2CommonOrdinal10336((DWORD)(ptUnit), (DWORD)(dwValue))

#define D2CommonUnitGetPathRoom(ptUnit) \
	D2CommonOrdinal10337((DWORD)(ptUnit))

#define D2CommonUnitClearPathRoom(hUnit) \
	D2CommonOrdinal10338((DWORD)(hUnit))

#define D2CommonUnitSetTarget(ptUnit, hTarget) \
	D2CommonOrdinal10339((DWORD)(ptUnit), (DWORD)(hTarget))

#define D2CommonUnitGetTargetType(dwUnused, ptUnit) \
	D2CommonOrdinal10340((DWORD)(ptUnit))

#define D2CommonUnitGetTargetId(dwUnused, ptUnit) \
	D2CommonOrdinal10341((DWORD)(ptUnit))

#define D2CommonUnitSetTargetStrict(ptUnit, hTarget) \
	D2CommonOrdinal10342((DWORD)(ptUnit), (DWORD)(hTarget))

#define D2CommonUnitSetMode(ptUnit, dwNewMode) \
	((BOOL)D2CommonOrdinal10343((DWORD)(ptUnit), (DWORD)(dwNewMode)))

#define D2CommonUnitInitModeAnim(ptUnit) \
	((BOOL)D2CommonOrdinal10344((DWORD)(ptUnit)))

#define D2CommonUnitInitPosition(dwUnused, ptUnit, dwRoom, dwTileX, dwTileY) \
	D2CommonOrdinal10345((DWORD)(ptUnit), (DWORD)(dwRoom), \
		(DWORD)(dwTileX), (DWORD)(dwTileY))

#define D2CommonUnitSetCollision(hUnit, hRoom, nX, nY) \
	D2CommonOrdinal10346((DWORD)(hUnit), (DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY))

#define D2CommonUnitClearCollision(ptUnit) \
	D2CommonOrdinal10347((DWORD)(ptUnit))

#define D2CommonUnitGetObjectNameOffset(ptUnit) \
	D2CommonOrdinal10348((DWORD)(ptUnit))

#define D2CommonUnitGetLookupBytes(dwArg1Unused, dwIndex, pdwWalkVelocity, pdwRunVelocity) \
	D2CommonOrdinal10349((DWORD)(dwArg1Unused), (DWORD)(dwIndex), (DWORD)(pdwWalkVelocity), \
		(DWORD)(pdwRunVelocity))

#define D2CommonUnitGetRoomChanged(ptUnit) \
	D2CommonOrdinal10350((DWORD)(ptUnit))

#define D2CommonUnitSetRoomChanged(ptUnit, dwValue) \
	D2CommonOrdinal10351((DWORD)(ptUnit), (DWORD)(dwValue))

#define D2CommonUnitSetChanged(hUnit, fEnable) \
	D2CommonOrdinal10352((DWORD)(hUnit), (DWORD)(fEnable))

#define D2CommonUnitFindSkillByType(hUnit, nSkillId) \
	D2CommonOrdinal10353((DWORD)(hUnit), (DWORD)(nSkillId))

#define D2CommonUnitTestMonsterSoft(hUnit) \
	((BOOL)D2CommonOrdinal10354((DWORD)(hUnit)))

#define D2CommonUnitGetWeaponNumber(hUnit) \
	D2CommonOrdinal10355((DWORD)(hUnit))

#define D2CommonUnitTestPositionClear(hUnitA, hUnitB, dwDistanceAdjust) \
	((BOOL)D2CommonOrdinal10356((DWORD)(hUnitA), (DWORD)(hUnitB), (DWORD)(dwDistanceAdjust)))

#define D2CommonUnitTestLineBlocked(hUnit1, hUnit2, dwMask) \
	((BOOL)D2CommonOrdinal10357((DWORD)(hUnit1), (DWORD)(hUnit2), (DWORD)(dwMask)))

#define D2CommonUnitTestLineBlockedIgnoring(hUnit1, hUnit2, dwMask) \
	((BOOL)D2CommonOrdinal10358((DWORD)(hUnit1), (DWORD)(hUnit2), (DWORD)(dwMask)))

#define D2CommonUnitTestLineBlockedToPoint(hUnit, dwX, dwY, dwMask) \
	((BOOL)D2CommonOrdinal10359((DWORD)(hUnit), (DWORD)(dwX), (DWORD)(dwY), (DWORD)(dwMask)))

#define D2CommonUnitGetStateByte(hUnit) \
	D2CommonOrdinal10360((DWORD)(hUnit))

#define D2CommonUnitGetItemFlagsWord(hUnit) \
	D2CommonOrdinal10361((DWORD)(hUnit))

#define D2CommonUnitGetItemCodeValue(hItem) \
	D2CommonOrdinal10362((DWORD)(hItem))

#define D2CommonUnitCalcLifePercent(hUnit) \
	((int)D2CommonOrdinal10363((DWORD)(hUnit)))

#define D2CommonUnitGetAnimMode(ptUnit) \
	D2CommonOrdinal10364((DWORD)(ptUnit))

#define D2CommonUnitSetAnimMode(ptUnit, dwValue) \
	D2CommonOrdinal10365((DWORD)(ptUnit), (DWORD)(dwValue))

#define D2CommonUnitInitAnimState(ptUnit) \
	D2CommonOrdinal10366((DWORD)(ptUnit))

#define D2CommonUnitSetAnimSequencePos(ptUnit, dwFrame) \
	D2CommonOrdinal10367((DWORD)(ptUnit), (DWORD)(dwFrame))

#define D2CommonUnitStepAnimState(ptUnit) \
	D2CommonOrdinal10368((DWORD)(ptUnit))

#define D2CommonUnitStepAnimPos(ptUnit) \
	D2CommonOrdinal10369((DWORD)(ptUnit))

#define D2CommonUnitStepAnimPosTo(ptUnit, dwLoopFrame, dwFrameCount) \
	D2CommonOrdinal10370((DWORD)(ptUnit), (DWORD)(dwLoopFrame), (DWORD)(dwFrameCount))

#define D2CommonUnitUpdateAnimRate(ptUnit) \
	D2CommonOrdinal10371((DWORD)(ptUnit))

#define D2CommonUnitSetMissileAnimRate(hUnit, dwValue) \
	D2CommonOrdinal10372((DWORD)(hUnit), (DWORD)(dwValue))

#define D2CommonUnitTestAnimComplete(ptUnit) \
	((BOOL)D2CommonOrdinal10373((DWORD)(ptUnit)))

#define D2CommonUnitGetAnimFrameAndCount(ptUnit, pdwFrame, pdwCount) \
	D2CommonOrdinal10374((DWORD)(ptUnit), (DWORD)(pdwFrame), (DWORD)(pdwCount))

#define D2CommonUnitGetAnimPosAndLength(hUnit, pdwPos, pdwLength) \
	D2CommonOrdinal10375((DWORD)(hUnit), (DWORD)(pdwPos), (DWORD)(pdwLength))

#define D2CommonUnitSetAnimEventFlagByte(ptUnit, dwIndex) \
	D2CommonOrdinal10376((DWORD)(ptUnit), (DWORD)(dwIndex))

#define D2CommonUnitGetAnimFrameByte(ptUnit, nFrame) \
	D2CommonOrdinal10377((DWORD)(ptUnit), (DWORD)(nFrame))

#define D2CommonUnitTakeLight(ptUnit) \
	D2CommonOrdinal10378((DWORD)(ptUnit))

#define D2CommonAddUnitToRoom(ptUnit, hRoom) \
	D2CommonOrdinal10379((DWORD)(ptUnit), (DWORD)(hRoom))

#define D2CommonUnitRoomAddToChangedList(hUnit) \
	D2CommonOrdinal10380((DWORD)(hUnit))

#define D2CommonUnitRoomRemoveUnit(hUnit) \
	D2CommonOrdinal10381((DWORD)(hUnit))

#define D2CommonUnitRoomRemoveFromChangedList(hUnit) \
	D2CommonOrdinal10382((DWORD)(hUnit))

#define D2CommonUnitRoomSortByTileY(ptRoom) \
	D2CommonOrdinal10383((DWORD)(ptRoom))

#define D2CommonUnitRoomTestUnitInRoom(ptRoom, hUnit) \
	((BOOL)D2CommonOrdinal10384((DWORD)(ptRoom), (DWORD)(hUnit)))

#define D2CommonUnitRoomLeave(ptUnit) \
	D2CommonOrdinal10385((DWORD)(ptUnit))

#define D2CommonUnitRoomClearChangedList(ptRoom) \
	D2CommonOrdinal10386((DWORD)(ptRoom))

#define D2CommonUnitIsDoor(ptUnit) \
	((BYTE)D2CommonOrdinal10387((DWORD)(ptUnit)))

#define D2CommonUnitTestObjectSubclassBitZero(hUnit) \
	D2CommonOrdinal10388((DWORD)(hUnit))

#define D2CommonUnitGetObjectClassInfoPtr(ptUnit) \
	D2CommonOrdinal10389((DWORD)(ptUnit))

#define D2CommonUnitGetObjectShrine(ptUnit) \
	D2CommonOrdinal10390((DWORD)(ptUnit))

#define D2CommonUnitSetObjectShrine(ptUnit, dwValue) \
	D2CommonOrdinal10391((DWORD)(ptUnit), (DWORD)(dwValue))

#define D2CommonUnitGetProximityMetric(hUnitA, hUnitB) \
	D2CommonOrdinal10392((DWORD)(hUnitA), (DWORD)(hUnitB))

#define D2CommonUnitUtilGetDistance(hUnit, nX, nY) \
	((int)D2CommonOrdinal10393((DWORD)(hUnit), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonUnitCalcEdgeDistance(hUnitA, hUnitB) \
	D2CommonOrdinal10394((DWORD)(hUnitA), (DWORD)(hUnitB))

#define D2CommonUnitUtilIsWithinRange(hUnit, ptCoord, dwRange) \
	((BOOL)D2CommonOrdinal10395((DWORD)(hUnit), (DWORD)(ptCoord), (DWORD)(dwRange)))

#define D2CommonUnitFindsInit(pPool, ufData, dwRoom, dwX, dwY, dwRadius, dwUnitTest, dwData) \
	D2CommonOrdinal10396((DWORD)(pPool), (DWORD)(ufData), (DWORD)(dwRoom), (DWORD)(dwX), (DWORD)(dwY), \
		(DWORD)(dwRadius), (DWORD)(dwUnitTest), (DWORD)(dwData))

#define D2CommonUnitFindsFreeArray(ptRec) \
	D2CommonOrdinal10397((DWORD)(ptRec))

#define D2CommonUnitFindsFindUnits(ufData) \
	D2CommonOrdinal10398((DWORD)(ufData))

#define D2CommonUnitFindsSearchRoom(hRoom, paUnits, pfnUnitTest, dwContext) \
	D2CommonOrdinal10400((DWORD)(hRoom), (DWORD)(paUnits), (DWORD)(pfnUnitTest), (DWORD)(dwContext))

#define D2CommonUnitUtilFindOverlappingUnit(hUnit, pfnTest) \
	D2CommonOrdinal10401((DWORD)(hUnit), (DWORD)(pfnTest))

#define D2CommonUnitUtilFindUnitAtPoint(hRoom, nX, nY, pfnTest, nSize) \
	D2CommonOrdinal10402((DWORD)(hRoom), (DWORD)(nX), (DWORD)(nY), (DWORD)(pfnTest), (DWORD)(nSize))

#define D2CommonUnitFindsTestRoomsNear(hDestUnit, hSrcUnit) \
	((BOOL)D2CommonOrdinal10403((DWORD)(hDestUnit), (DWORD)(hSrcUnit)))

#define D2CommonUnitGetInvRecordIndex(hUnit, bPage) \
	((int)D2CommonOrdinal10404((DWORD)(hUnit), (DWORD)(bPage)))

#define D2CommonUnitTestObjectHasCollision(hUnit) \
	D2CommonOrdinal10405((DWORD)(hUnit))

#define D2CommonUnitGetDistanceX(hUnitA, hUnitB) \
	D2CommonOrdinal10406((DWORD)(hUnitA), (DWORD)(hUnitB))

#define D2CommonUnitGetDistanceY(hUnitA, hUnitB) \
	D2CommonOrdinal10407((DWORD)(hUnitA), (DWORD)(hUnitB))

#define D2CommonUnitPathSetExtra(hUnit, dwX, dwY) \
	D2CommonOrdinal10408((DWORD)(hUnit), (DWORD)(dwX), (DWORD)(dwY))

#define D2CommonUnitSyncPosition(hUnit) \
	D2CommonOrdinal10409((DWORD)(hUnit))

#define D2CommonUnitSetLinkedUnit(ptUnit, pdwPair) \
	D2CommonOrdinal10410((DWORD)(ptUnit), (DWORD)(pdwPair))

#define D2CommonUnitSetLinkedUnitIds(ptUnit, dwB4, dwB8) \
	D2CommonOrdinal10411((DWORD)(ptUnit), (DWORD)(dwB4), (DWORD)(dwB8))

#define D2CommonUnitSetSecondLinkedUnit(ptUnit, pdwPair) \
	D2CommonOrdinal10412((DWORD)(ptUnit), (DWORD)(pdwPair))

#define D2CommonUnitSetPositionExtra(ptUnit, dwTargetX, dwTargetY) \
	D2CommonOrdinal10413((DWORD)(ptUnit), (DWORD)(dwTargetX), (DWORD)(dwTargetY))

#define D2CommonUnitUtilSetTestCallback(ptUnit, pfnTest, dwKeptTargetType, dwKeptTargetId) \
	D2CommonOrdinal10414((DWORD)(ptUnit), (DWORD)(pfnTest), (DWORD)(dwKeptTargetType), \
		(DWORD)(dwKeptTargetId))

#define D2CommonUnitCreatePlrInfo(ptUnit) \
	D2CommonOrdinal10415((DWORD)(ptUnit))

#define D2CommonUnitFreePlrInfo(dwPool, ptUnit) \
	D2CommonOrdinal10416((DWORD)(dwPool), (DWORD)(ptUnit))

#define D2CommonUnitSetPlayerName(ptUnit, pszName) \
	D2CommonOrdinal10417((DWORD)(ptUnit), (DWORD)(pszName))

#define D2CommonUnitGetPlayerName(ptUnit) \
	((char *)D2CommonOrdinal10418((DWORD)(ptUnit)))

#define D2CommonUnitGetPlrInfo(ptUnit) \
	D2CommonOrdinal10419((DWORD)(ptUnit))

#define D2CommonUnitSetPlayerLevelBits(ptUnit, dwValue) \
	D2CommonOrdinal10420((DWORD)(ptUnit), (DWORD)(dwValue))

#define D2CommonUnitGetPlayerLevelBits(ptUnit) \
	D2CommonOrdinal10421((DWORD)(ptUnit))

#define D2CommonUnitGetObjectFlags(ptUnit) \
	((BYTE)D2CommonOrdinal10422((DWORD)(ptUnit)))

#define D2CommonUnitSetObjectFlags(ptUnit, bValue) \
	D2CommonOrdinal10423((DWORD)(ptUnit), (DWORD)(bValue))

#define D2CommonUnitTestObjectFlags(ptUnit, bMask) \
	((BOOL)D2CommonOrdinal10424((DWORD)(ptUnit), (DWORD)(bMask)))

#define D2CommonUnitGetOverlayHeightClass(hUnit) \
	D2CommonOrdinal10425((DWORD)(hUnit))

#define D2CommonUnitCalcArmorClassWithBonus(hUnit) \
	((int)D2CommonOrdinal10426((DWORD)(hUnit)))

#define D2CommonUnitGetAttackerIndexedValue(hAttacker) \
	D2CommonOrdinal10427((DWORD)(hAttacker))

#define D2CommonUnitGetActiveWeapon(hUnit) \
	D2CommonOrdinal10428((DWORD)(hUnit))

#define D2CommonUnitGetWClassModeValue(ptUnit) \
	D2CommonOrdinal10429((DWORD)(ptUnit))

#define D2CommonUnitAddAnimOverlay(ptUnit, eOverlay, nFrames) \
	D2CommonOrdinal10430((DWORD)(ptUnit), (DWORD)(eOverlay), (DWORD)(nFrames))

#define D2CommonUnitGetCombinedStatDelta(dwArg) \
	D2CommonOrdinal10431((DWORD)(dwArg))

#define D2CommonUnitCalcGoldLimit(dwArg) \
	D2CommonOrdinal10432((DWORD)(dwArg))

#define D2CommonUnitApplyPrimaryWeaponSwap(hUnit) \
	D2CommonOrdinal10433((DWORD)(hUnit))

#define D2CommonUnitApplyEquipStatSwap(hUnit) \
	D2CommonOrdinal10434((DWORD)(hUnit))

#define D2CommonUnitTestEquipSlotConflict(hUnit, dwItemArg) \
	((BYTE)D2CommonOrdinal10435((DWORD)(hUnit), (DWORD)(dwItemArg)))

#define D2CommonUnitSyncPlayerRightSkill(ptUnit) \
	D2CommonOrdinal10436((DWORD)(ptUnit))

#define D2CommonUnitSyncPlayerLeftSkill(ptUnit) \
	D2CommonOrdinal10437((DWORD)(ptUnit))

#define D2CommonUnitGetPlayerRightSkill(ptUnit) \
	D2CommonOrdinal10438((DWORD)(ptUnit))

#define D2CommonUnitGetPlayerLeftSkill(ptUnit) \
	D2CommonOrdinal10439((DWORD)(ptUnit))

#define D2CommonUnitSetPlayerWeaponId(ptUnit, hWeapon) \
	D2CommonOrdinal10440((DWORD)(ptUnit), (DWORD)(hWeapon))

#define D2CommonUnitGetPlayerWeaponId(ptUnit) \
	D2CommonOrdinal10441((DWORD)(ptUnit))

#define D2CommonUnitGetPathFlagByte(ptUnit) \
	((BYTE)D2CommonOrdinal10442((DWORD)(ptUnit)))

#define D2CommonUnitSetObjectLinks(hUnit, dwValue) \
	D2CommonOrdinal10443((DWORD)(hUnit), (DWORD)(dwValue))

#define D2CommonUnitGetObjectLinks(hUnit) \
	D2CommonOrdinal10444((DWORD)(hUnit))

#define D2CommonUnitAllocAuxBlock(hUnit) \
	D2CommonOrdinal10445((DWORD)(hUnit))

#define D2CommonUnitFreeAuxBlock(hUnit) \
	D2CommonOrdinal10446((DWORD)(hUnit))

#define D2CommonStatsListSetValueA(ptStatsList, eStat, nValue) \
	D2CommonOrdinal10447((DWORD)(ptStatsList), (DWORD)(eStat), (DWORD)(nValue))

#define D2CommonStatsListAddValue(ptStatsList, eStat, nValue) \
	D2CommonOrdinal10448((DWORD)(ptStatsList), (DWORD)(eStat), (DWORD)(nValue))

#define D2CommonStatsListSetValueB(ptStatsList, eStat, nValue) \
	D2CommonOrdinal10449((DWORD)(ptStatsList), (DWORD)(eStat), (DWORD)(nValue))

#define D2CommonStatsListGetValueA(ptStatsList, nStat) \
	D2CommonOrdinal10450((DWORD)(ptStatsList), (DWORD)(nStat))

#define D2CommonStatsContainerGetEntry(ptContainer, nIndex) \
	D2CommonOrdinal10451((DWORD)(ptContainer), (DWORD)(nIndex))

#define D2CommonStatsListReset(ptList) \
	D2CommonOrdinal10452((DWORD)(ptList))

#define D2CommonStatsUnitFreeLists(hUnit) \
	D2CommonOrdinal10453((DWORD)(hUnit))

#define D2CommonStatsAllocList(dwPool, dwFlags, dwExpiry, dwSourceType, dwSourceId) \
	D2CommonOrdinal10454((DWORD)(dwPool), (DWORD)(dwFlags), (DWORD)(dwExpiry), (DWORD)(dwSourceType), \
		(DWORD)(dwSourceId))

#define D2CommonStatsGetListSourceType(ptList) \
	D2CommonOrdinal10455((DWORD)(ptList))

#define D2CommonStatsGetListSourceId(ptList) \
	D2CommonOrdinal10456((DWORD)(ptList))

#define D2CommonStatsGetListEndFrame(ptList) \
	D2CommonOrdinal10457((DWORD)(ptList))

#define D2CommonStatsUnitUnlinkList(hUnit, ptListVictim) \
	D2CommonOrdinal10458((DWORD)(hUnit), (DWORD)(ptListVictim))

#define D2CommonStatsUnitLinkList(hUnit, ptList) \
	D2CommonOrdinal10459((DWORD)(hUnit), (DWORD)(ptList))

#define D2CommonStatsSetListEndFrame(ptStatsList, dwValue) \
	D2CommonOrdinal10460((DWORD)(ptStatsList), (DWORD)(dwValue))

#define D2CommonStatsSetListRemoveCallback(ptStatsList, dwValue) \
	D2CommonOrdinal10461((DWORD)(ptStatsList), (DWORD)(dwValue))

#define D2CommonStatsSetListState(ptList, dwValue) \
	D2CommonOrdinal10462((DWORD)(ptList), (DWORD)(dwValue))

#define D2CommonStatsGetListState(ptList) \
	D2CommonOrdinal10463((DWORD)(ptList))

#define D2CommonStatsFindListByState(hUnit, dwKey) \
	D2CommonOrdinal10464((DWORD)(hUnit), (DWORD)(dwKey))

#define D2CommonStatsFindListByMask(hUnit, dwMask) \
	D2CommonOrdinal10465((DWORD)(hUnit), (DWORD)(dwMask))

#define D2CommonStatsListFree(ptList) \
	D2CommonOrdinal10466((DWORD)(ptList))

#define D2CommonStatsUnitSetActiveStateBit(hUnit, nShift, fSet) \
	D2CommonOrdinal10467((DWORD)(hUnit), (DWORD)(nShift), (DWORD)(fSet))

#define D2CommonStatsGetState(hUnit, nShift) \
	D2CommonOrdinal10468((DWORD)(hUnit), (DWORD)(nShift))

#define D2CommonStatsUnitSetActiveStateValue(hUnit, nShift, fSet) \
	D2CommonOrdinal10469((DWORD)(hUnit), (DWORD)(nShift), (DWORD)(fSet))

#define D2CommonStatsUnitTestActiveStateBit(hUnit, nShift) \
	D2CommonOrdinal10470((DWORD)(hUnit), (DWORD)(nShift))

#define D2CommonStatsUnitTestAnyActiveState(hUnit) \
	((BOOL)D2CommonOrdinal10471((DWORD)(hUnit)))

#define D2CommonStatsUnitClearActiveStates(hUnit) \
	D2CommonOrdinal10472((DWORD)(hUnit))

#define D2CommonStatsUnitCommitPendingFlags(hUnit, dwBoss) \
	D2CommonOrdinal10473((DWORD)(hUnit), (DWORD)(dwBoss))

#define D2CommonStatsUnitGetPendingFlags(hUnit) \
	((DWORD *)D2CommonOrdinal10474((DWORD)(hUnit)))

#define D2CommonStatsGetFive() \
	D2CommonOrdinal10475()

#define D2CommonStatsTestStateInOverlayGroup(nShift) \
	((BOOL)D2CommonOrdinal10476((DWORD)(nShift)))

#define D2CommonStatsTestStateSharedList(nShift) \
	((BOOL)D2CommonOrdinal10477((DWORD)(nShift)))

#define D2CommonStatsTestStateSharedListBase(nShift) \
	((BOOL)D2CommonOrdinal10478((DWORD)(nShift)))

#define D2CommonStatsTestStateStoresStat(nShift) \
	((BOOL)D2CommonOrdinal10479((DWORD)(nShift)))

#define D2CommonStatsUnitTestHasAction(hUnit) \
	((BOOL)D2CommonOrdinal10480((DWORD)(hUnit)))

#define D2CommonStatsUnitTestOverlayGroup(hUnit) \
	((BOOL)D2CommonOrdinal10481((DWORD)(hUnit)))

#define D2CommonStatsTestStatePassesOn(nShift) \
	((BOOL)D2CommonOrdinal10482((DWORD)(nShift)))

#define D2CommonStatsTestStateSendsValue(nShift) \
	((BOOL)D2CommonOrdinal10483((DWORD)(nShift)))

#define D2CommonStatsTestStateSendsTwoValues(nShift) \
	((BOOL)D2CommonOrdinal10484((DWORD)(nShift)))

#define D2CommonStatsTestStateSetAll(nShift) \
	((BOOL)D2CommonOrdinal10485((DWORD)(nShift)))

#define D2CommonStatsTestStateSetNoneA(nShift) \
	((BOOL)D2CommonOrdinal10486((DWORD)(nShift)))

#define D2CommonStatsTestStateSetNoneB(nShift) \
	((BOOL)D2CommonOrdinal10487((DWORD)(nShift)))

#define D2CommonStatsUnitGetZero(hUnit) \
	D2CommonOrdinal10488((DWORD)(hUnit))

#define D2CommonStatsUnitClearFieldEc(hUnit) \
	D2CommonOrdinal10489((DWORD)(hUnit))

#define D2CommonStatsUnitCallChanged(hUnit, dwArg, eStat, pfnChangeFunction) \
	D2CommonOrdinal10490((DWORD)(hUnit), (DWORD)(dwArg), (DWORD)(eStat), (DWORD)(pfnChangeFunction))

#define D2CommonStatsUnitCallChangedAll(hUnit, dwArg, pfnChangeFunction) \
	D2CommonOrdinal10491((DWORD)(hUnit), (DWORD)(dwArg), (DWORD)(pfnChangeFunction))

#define D2CommonStatsUnitClampToMax(hUnit) \
	D2CommonOrdinal10492((DWORD)(hUnit))

#define D2CommonStatsUnitFreeStateLists(hUnit) \
	D2CommonOrdinal10493((DWORD)(hUnit))

#define D2CommonStatsUnitExpireLists(hUnit, nFrame) \
	D2CommonOrdinal10494((DWORD)(hUnit), (DWORD)(nFrame))

#define D2CommonStatsUnitSetValue(hUnit, eStat, nValue) \
	D2CommonOrdinal10495((DWORD)(hUnit), (DWORD)(eStat), (DWORD)(nValue))

#define D2CommonStatsUnitAddValue(hUnit, eStat, nValue) \
	D2CommonOrdinal10496((DWORD)(hUnit), (DWORD)(eStat), (DWORD)(nValue))

#define D2CommonStatsUnitGetValue(hUnit, eStat) \
	((int)D2CommonOrdinal10497((DWORD)(hUnit), (DWORD)(eStat)))

#define D2CommonStatsGetTotalValue(hUnit, nStat) \
	D2CommonOrdinal10498((DWORD)(hUnit), (DWORD)(nStat))

#define D2CommonStatsUnitGetBaseValue(hUnit, eStat) \
	((int)D2CommonOrdinal10499((DWORD)(hUnit), (DWORD)(eStat)))

#define D2CommonStatsUnitGetBonusValue(hUnit, eStat) \
	((int)D2CommonOrdinal10500((DWORD)(hUnit), (DWORD)(eStat)))

#define D2CommonStatsUnitApplyDye(hCloth, hDye, nMode) \
	D2CommonOrdinal10501((DWORD)(hCloth), (DWORD)(hDye), (DWORD)(nMode))

#define D2CommonStatsUnitRemoveDye(hCloth, hDye) \
	D2CommonOrdinal10502((DWORD)(hCloth), (DWORD)(hDye))

#define D2CommonStatsUnitRebuildExport(hUnit) \
	D2CommonOrdinal10503((DWORD)(hUnit))

#define D2CommonStatsUnitAllocStats(hUnit, dwFlags) \
	D2CommonOrdinal10504((DWORD)(hUnit), (DWORD)(dwFlags))

#define D2CommonStatsUnitFreeStats(hUnit) \
	D2CommonOrdinal10505((DWORD)(hUnit))

#define D2CommonStatsSetListEndFrameSecond(ptStatsList, dwValue) \
	D2CommonOrdinal10506((DWORD)(ptStatsList), (DWORD)(dwValue))

#define D2CommonStatsGetGlobalListEndFrame(ptStatsList) \
	D2CommonOrdinal10507((DWORD)(ptStatsList))

#define D2CommonStatsTestHasOverlay(hUnit) \
	((BOOL)D2CommonOrdinal10508((DWORD)(hUnit)))

#define D2CommonStatsListSetValuePacked(ptStatsList, eStat, nLow, nHigh) \
	D2CommonOrdinal10509((DWORD)(ptStatsList), (DWORD)(eStat), (DWORD)(nLow), (DWORD)(nHigh))

#define D2CommonStatsListGetValueB(ptStatsList, nStat) \
	D2CommonOrdinal10510((DWORD)(ptStatsList), (DWORD)(nStat))

#define D2CommonStatsUnitClearHasOverlay(hUnit) \
	D2CommonOrdinal10511((DWORD)(hUnit))

#define D2CommonStatsUnitSetHasOverlay(hUnit) \
	D2CommonOrdinal10512((DWORD)(hUnit))

#define D2CommonStatsTestModifying(hUnit) \
	((BOOL)D2CommonOrdinal10513((DWORD)(hUnit)))

#define D2CommonStatsUnitTestLightColor(hUnit) \
	((BOOL)D2CommonOrdinal10514((DWORD)(hUnit)))

#define D2CommonStatsUnitTestToHitRaised(hUnit) \
	((BOOL)D2CommonOrdinal10515((DWORD)(hUnit)))

#define D2CommonStatsUnitTestDamageRaised(hUnit) \
	((BOOL)D2CommonOrdinal10516((DWORD)(hUnit)))

#define D2CommonStatsUnitTestArmorClassRaised(hUnit) \
	((BOOL)D2CommonOrdinal10517((DWORD)(hUnit)))

#define D2CommonStatsUnitTestFireResistRaised(hUnit) \
	((BOOL)D2CommonOrdinal10518((DWORD)(hUnit)))

#define D2CommonStatsUnitTestColdResistRaised(hUnit) \
	((BOOL)D2CommonOrdinal10519((DWORD)(hUnit)))

#define D2CommonStatsUnitTestLightResistRaised(hUnit) \
	((BOOL)D2CommonOrdinal10520((DWORD)(hUnit)))

#define D2CommonStatsUnitTestPoisonResistRaised(hUnit) \
	((BOOL)D2CommonOrdinal10521((DWORD)(hUnit)))

#define D2CommonStatsUnitTestToHitLowered(hUnit) \
	((BOOL)D2CommonOrdinal10522((DWORD)(hUnit)))

#define D2CommonStatsUnitTestDamageLowered(hUnit) \
	((BOOL)D2CommonOrdinal10523((DWORD)(hUnit)))

#define D2CommonStatsUnitTestArmorClassLowered(hUnit) \
	((BOOL)D2CommonOrdinal10524((DWORD)(hUnit)))

#define D2CommonStatsUnitTestFireResistLowered(hUnit) \
	((BOOL)D2CommonOrdinal10525((DWORD)(hUnit)))

#define D2CommonStatsUnitTestColdResistLowered(hUnit) \
	((BOOL)D2CommonOrdinal10526((DWORD)(hUnit)))

#define D2CommonStatsUnitTestLightResistLowered(hUnit) \
	((BOOL)D2CommonOrdinal10527((DWORD)(hUnit)))

#define D2CommonStatsUnitTestPoisonResistLowered(hUnit) \
	((BOOL)D2CommonOrdinal10528((DWORD)(hUnit)))

#define D2CommonStatsUnitTestMatchState(hUnit) \
	((BOOL)D2CommonOrdinal10529((DWORD)(hUnit)))

#define D2CommonStatsUnitTestOwnedState(hUnit) \
	((BOOL)D2CommonOrdinal10530((DWORD)(hUnit)))

#define D2CommonStatsTestStateOwned(nShift) \
	((BOOL)D2CommonOrdinal10531((DWORD)(nShift)))

#define D2CommonStatsUnitTestStateKept(hUnit, nShift) \
	((BOOL)D2CommonOrdinal10532((DWORD)(hUnit), (DWORD)(nShift)))

#define D2CommonStatsUnitTestStateGroupA(hUnit) \
	((BOOL)D2CommonOrdinal10533((DWORD)(hUnit)))

#define D2CommonStatsTestStateGroupA(nShift) \
	((BOOL)D2CommonOrdinal10534((DWORD)(nShift)))

#define D2CommonStatsUnitTestStateGroupB(hUnit) \
	((BOOL)D2CommonOrdinal10535((DWORD)(hUnit)))

#define D2CommonStatsTestStateGroupB(nShift) \
	((BOOL)D2CommonOrdinal10536((DWORD)(nShift)))

#define D2CommonStatsUnitTestRegenBaseOnly(hUnit) \
	((BOOL)D2CommonOrdinal10537((DWORD)(hUnit)))

#define D2CommonStatsUnitTestStateGroupC(hUnit) \
	((BOOL)D2CommonOrdinal10538((DWORD)(hUnit)))

#define D2CommonStatsUnitTestNoColors(hUnit) \
	((BOOL)D2CommonOrdinal10539((DWORD)(hUnit)))

#define D2CommonStatsUnitGetAlignment(hUnit) \
	D2CommonOrdinal10540((DWORD)(hUnit))

#define D2CommonStatsUnitsTestSameAlignment(hUnitA, hUnitB) \
	((BOOL)D2CommonOrdinal10541((DWORD)(hUnitA), (DWORD)(hUnitB)))

#define D2CommonStatsUnitCalcMaxHp(hUnit) \
	((int)D2CommonOrdinal10542((DWORD)(hUnit)))

#define D2CommonStatsUnitCalcMaxMana(hUnit) \
	((int)D2CommonOrdinal10543((DWORD)(hUnit)))

#define D2CommonStatsUnitCalcMaxStamina(hUnit) \
	((int)D2CommonOrdinal10544((DWORD)(hUnit)))

#define D2CommonStatsUnitCalcMaxDurability(hUnit) \
	((int)D2CommonOrdinal10545((DWORD)(hUnit)))

#define D2CommonStatsUnitCalcMaxDamage(hUnit) \
	((int)D2CommonOrdinal10546((DWORD)(hUnit)))

#define D2CommonStatsUnitCalcMinDamage(hUnit) \
	((int)D2CommonOrdinal10547((DWORD)(hUnit)))

#define D2CommonStatsUnitCalcArmorClass(hUnit) \
	((int)D2CommonOrdinal10548((DWORD)(hUnit)))

#define D2CommonExcelParseRecords(file, ptDataTable, pRecords, nMemHgt, nMemSpan) \
	D2CommonOrdinal10549((DWORD)(file), (DWORD)(ptDataTable), (DWORD)(pRecords), (DWORD)(nMemHgt), \
		(DWORD)(nMemSpan))

#define D2CommonExcelOpen(pBuffer, nSize) \
	D2CommonOrdinal10550((DWORD)(pBuffer), (DWORD)(nSize))

#define D2CommonExcelFileFree(ptFile) \
	D2CommonOrdinal10551((DWORD)(ptFile))

#define D2CommonExcelFileGetRowCount(ptFile) \
	D2CommonOrdinal10552((DWORD)(ptFile))

#define D2CommonDataTblsFreeAll() \
	D2CommonOrdinal10553()

#define D2CommonDataTblsLoadAll(hArchive, dwLoadFiles, dwLoadSubFiles) \
	D2CommonOrdinal10554((DWORD)(hArchive), (DWORD)(dwLoadFiles), (DWORD)(dwLoadSubFiles))

#define D2CommonDataTblsLoadExcelFile(hArchive, pszName, pnSize) \
	((void *)D2CommonOrdinal10555((DWORD)(hArchive), (DWORD)(pszName), (DWORD)(pnSize)))

#define D2CommonDataTblsAllocAndTokenize(pDescriptors, pBuffer, nSize, pnRows, nStride) \
	((void *)D2CommonOrdinal10556((DWORD)(pDescriptors), (DWORD)(pBuffer), (DWORD)(nSize), (DWORD)(pnRows), \
		(DWORD)(nStride)))

#define D2CommonDataTblsFreeBlock(p) \
	D2CommonOrdinal10557((DWORD)(p))

#define D2CommonDataTblsNothing(dwUnused1, dwUnused2, dwUnused3, dwUnused4) \
	D2CommonOrdinal10558((DWORD)(dwUnused1), (DWORD)(dwUnused2), (DWORD)(dwUnused3), (DWORD)(dwUnused4))

#define D2CommonDataTblsGetCharStats(nClass) \
	D2CommonOrdinal10559((DWORD)(nClass))

#define D2CommonMonsterTblsGetStats(nClass) \
	D2CommonOrdinal10560((DWORD)(nClass))

#define D2CommonMissileTblsGetMissile(eMissile) \
	D2CommonOrdinal10561((DWORD)(eMissile))

#define D2CommonMissileTblsGetVel(eMissile) \
	((int)D2CommonOrdinal10562((DWORD)(eMissile)))

#define D2CommonOverlayTblsGetOverlay(eOverlay) \
	D2CommonOrdinal10563((DWORD)(eOverlay))

#define D2CommonMonsterTblsGetRun(nClass, pnRunCount, pnRunIndex) \
	((BOOL)D2CommonOrdinal10564((DWORD)(nClass), (DWORD)(pnRunCount), (DWORD)(pnRunIndex)))

#define D2CommonDataTblsGetCharStatsTxt() \
	D2CommonOrdinal10565()

#define D2CommonDataTblsGetSkill(nSkill) \
	D2CommonOrdinal10566((DWORD)(nSkill))

#define D2CommonDataTblsGetSkillCount() \
	((int)D2CommonOrdinal10567())

#define D2CommonArenaGetZero() \
	D2CommonOrdinal10568()

#define D2CommonItemTblsGetItemStatsDesc() \
	((void *)D2CommonOrdinal10569())

#define D2CommonItemTblsGetItemStats(nIndex) \
	D2CommonOrdinal10570((DWORD)(nIndex))

#define D2CommonItemTblsFindItemByCode(dwCode, pnIndex) \
	D2CommonOrdinal10571((DWORD)(dwCode), (DWORD)(pnIndex))

#define D2CommonItemTblsFindItemIndexByCode(dwCode) \
	((int)D2CommonOrdinal10572((DWORD)(dwCode)))

#define D2CommonItemTblsGetMagicAffixDesc() \
	((void *)D2CommonOrdinal10573())

#define D2CommonItemTblsGetMagicAffix(nIndex) \
	D2CommonOrdinal10574((DWORD)(nIndex))

#define D2CommonItemTblsGetRareAffixDesc() \
	((void *)D2CommonOrdinal10575())

#define D2CommonItemTblsGetRareAffix(nIndex) \
	D2CommonOrdinal10576((DWORD)(nIndex))

#define D2CommonItemTblsGetUniqueItemDesc() \
	((void *)D2CommonOrdinal10577())

#define D2CommonItemTblsGetUniqueItem(nIndex) \
	D2CommonOrdinal10578((DWORD)(nIndex))

#define D2CommonItemTblsGetSetItemDesc() \
	((void *)D2CommonOrdinal10579())

#define D2CommonItemTblsGetSetItem(nIndex) \
	D2CommonOrdinal10580((DWORD)(nIndex))

#define D2CommonItemTblsGetQualityItemDesc() \
	((void *)D2CommonOrdinal10581())

#define D2CommonItemTblsGetQualityItem(nIndex) \
	D2CommonOrdinal10582((DWORD)(nIndex))

#define D2CommonItemTblsGetLowQualityItemDesc() \
	((void *)D2CommonOrdinal10583())

#define D2CommonItemTblsGetLowQualityItem(nIndex) \
	D2CommonOrdinal10584((DWORD)(nIndex))

#define D2CommonItemTblsGetGemDesc() \
	((void *)D2CommonOrdinal10585())

#define D2CommonItemTblsGetGem(nIndex) \
	D2CommonOrdinal10586((DWORD)(nIndex))

#define D2CommonItemTblsGetBookDesc() \
	((void *)D2CommonOrdinal10587())

#define D2CommonItemTblsGetBook(nIndex) \
	D2CommonOrdinal10588((DWORD)(nIndex))

#define D2CommonItemTblsGetItemRatioDesc() \
	((void *)D2CommonOrdinal10589())

#define D2CommonItemTblsGetItemRatio(dwItemClassIndex, dwUnused) \
	D2CommonOrdinal10590((DWORD)(dwItemClassIndex), (DWORD)(dwUnused))

#define D2CommonObjTblsGetShrine(nIndex) \
	D2CommonOrdinal10591((DWORD)(nIndex))

#define D2CommonObjTblsGetShrineCount() \
	((int)D2CommonOrdinal10592())

#define D2CommonObjTblsGetObject(nIndex) \
	D2CommonOrdinal10593((DWORD)(nIndex))

#define D2CommonObjTblsGetObjGroup(nIndex) \
	D2CommonOrdinal10594((DWORD)(nIndex))

#define D2CommonDataTblsGetExperience(nClass, nLevel) \
	D2CommonOrdinal10595((DWORD)(nClass), (DWORD)(nLevel))

#define D2CommonDataTblsGetMaxLevel(nClass) \
	((int)D2CommonOrdinal10596((DWORD)(nClass)))

#define D2CommonDataTblsGetLevelFromExperience(nClass, dwExperience) \
	((int)D2CommonOrdinal10597((DWORD)(nClass), (DWORD)(dwExperience)))

#define D2CommonLvlTblsGetLevelData(eLevelID) \
	D2CommonOrdinal10598((DWORD)(eLevelID))

#define D2CommonLvlTblsGetRain(eLevelID) \
	((BYTE)D2CommonOrdinal10599((DWORD)(eLevelID)))

#define D2CommonLvlTblsGetMud(eLevelID) \
	((BYTE)D2CommonOrdinal10600((DWORD)(eLevelID)))

#define D2CommonLvlTblsGetNoPer(eLevelID) \
	((BYTE)D2CommonOrdinal10601((DWORD)(eLevelID)))

#define D2CommonInvTblsGetInvRect(nIndex, ptRect) \
	D2CommonOrdinal10602((DWORD)(nIndex), (DWORD)(ptRect))

#define D2CommonInvTblsGetGrid(nIndex, ptGrid) \
	D2CommonOrdinal10603((DWORD)(nIndex), (DWORD)(ptGrid))

#define D2CommonInvTblsGetBodyLoc(nIndex, ptBodyLoc, nLoc) \
	D2CommonOrdinal10604((DWORD)(nIndex), (DWORD)(ptBodyLoc), (DWORD)(nLoc))

#define D2CommonBeltTblsGetBelt(nIndex, ptBelt) \
	D2CommonOrdinal10605((DWORD)(nIndex), (DWORD)(ptBelt))

#define D2CommonBeltTblsGetBox(nIndex, ptBox, nBox) \
	D2CommonOrdinal10606((DWORD)(nIndex), (DWORD)(ptBox), (DWORD)(nBox))

#define D2CommonAnimTblsSetUnitAnimSpeed(hUnit, dwSpeed) \
	D2CommonOrdinal10607((DWORD)(hUnit), (DWORD)(dwSpeed))

#define D2CommonAnimTblsGetAnimInfo(pszName, pdwFrames, pdwSpeed, pdwFirstFrame) \
	D2CommonOrdinal10608((DWORD)(pszName), (DWORD)(pdwFrames), (DWORD)(pdwSpeed), (DWORD)(pdwFirstFrame))

#define D2CommonMonsterTblsSetVelocity(nClass, nVelocity) \
	D2CommonOrdinal10609((DWORD)(nClass), (DWORD)(nVelocity))

#define D2CommonTokTblsGetPlayerDesc() \
	((void *)D2CommonOrdinal10610())

#define D2CommonTokTblsGetMonsterDesc() \
	((void *)D2CommonOrdinal10611())

#define D2CommonTokTblsGetObjectDesc() \
	((void *)D2CommonOrdinal10612())

#define D2CommonTokTblsGetPlayerToken(nIndex, nType) \
	D2CommonOrdinal10613((DWORD)(nIndex), (DWORD)(nType))

#define D2CommonTokTblsGetMonsterToken(nIndex, nType) \
	D2CommonOrdinal10614((DWORD)(nIndex), (DWORD)(nType))

#define D2CommonTokTblsGetObjectToken(nIndex, nType) \
	D2CommonOrdinal10615((DWORD)(nIndex), (DWORD)(nType))

#define D2CommonTokTblsGetComponent(eComponent) \
	D2CommonOrdinal10616((DWORD)(eComponent))

#define D2CommonTokTblsGetArmor(nIndex) \
	D2CommonOrdinal10617((DWORD)(nIndex))

#define D2CommonMonsterTblsGetRandomUniqueTitle(hUnit) \
	((WORD *)D2CommonOrdinal10618((DWORD)(hUnit)))

#define D2CommonMonsterTblsGetRandomUniquePrefix(hUnit) \
	((WORD *)D2CommonOrdinal10619((DWORD)(hUnit)))

#define D2CommonMonsterTblsRollUniqueSuffix(hUnit) \
	((WORD *)D2CommonOrdinal10620((DWORD)(hUnit)))

#define D2CommonMonsterTblsRollUniqueAppellation(hUnit) \
	((WORD *)D2CommonOrdinal10621((DWORD)(hUnit)))

#define D2CommonMonsterTblsGetTreasureClassDesc() \
	((void *)D2CommonOrdinal10622())

#define D2CommonMonsterTblsGetTreasureClass(nIndex) \
	D2CommonOrdinal10623((DWORD)(nIndex))

#define D2CommonMonsterTblsGetMonItemPercentDesc() \
	((void *)D2CommonOrdinal10624())

#define D2CommonMonsterTblsGetMonItemPercent(nIndex) \
	D2CommonOrdinal10625((DWORD)(nIndex))

#define D2CommonArenaGetCharTemplateCount() \
	((int)D2CommonOrdinal10626())

#define D2CommonArenaGetCharTemplate(nTemplate, nLevel) \
	D2CommonOrdinal10627((DWORD)(nTemplate), (DWORD)(nLevel))

#define D2CommonArenaGetCharTemplateClass(nTemplate, nLevel) \
	((int)D2CommonOrdinal10628((DWORD)(nTemplate), (DWORD)(nLevel)))

#define D2CommonTransformTblsWarnStub(dwUnused1, dwUnused2) \
	D2CommonOrdinal10629((DWORD)(dwUnused1), (DWORD)(dwUnused2))

#define D2CommonMonsterTblsGetSuperUnique(nIndex) \
	D2CommonOrdinal10630((DWORD)(nIndex))

#define D2CommonMonsterTblsTestSecondIsSel(hMonster) \
	((BOOL)D2CommonOrdinal10631((DWORD)(hMonster)))

#define D2CommonMonsterTblsTestNeverSel(hMonster) \
	((BOOL)D2CommonOrdinal10632((DWORD)(hMonster)))

#define D2CommonItemTblsGetGambleItems() \
	((int *)D2CommonOrdinal10633())

#define D2CommonOverlayTblsGetFrames(eOverlay) \
	((int)D2CommonOrdinal10634((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetPreDraw(eOverlay) \
	((int)D2CommonOrdinal10635((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetXoffset(eOverlay) \
	((int)D2CommonOrdinal10636((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetYoffset(eOverlay) \
	((int)D2CommonOrdinal10637((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetTrans(eOverlay) \
	((int)D2CommonOrdinal10638((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetLight(eOverlay, pnInitRadius, pnRadius, pbRed, pbGreen, pbBlue) \
	((BOOL)D2CommonOrdinal10639((DWORD)(eOverlay), (DWORD)(pnInitRadius), (DWORD)(pnRadius), (DWORD)(pbRed), \
		(DWORD)(pbGreen), (DWORD)(pbBlue)))

#define D2CommonOverlayTblsGet1ofN(eOverlay) \
	((int)D2CommonOrdinal10640((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetDir(eOverlay) \
	((int)D2CommonOrdinal10641((DWORD)(eOverlay)))

#define D2CommonOverlayTblsGetAnimSequence(hUnit) \
	D2CommonOrdinal10642((DWORD)(hUnit))

#define D2CommonSequenceGetLengthFixed(hUnit) \
	D2CommonOrdinal10643((DWORD)(hUnit))

#define D2CommonSequenceGetFrameCount(hUnit) \
	D2CommonOrdinal10644((DWORD)(hUnit))

#define D2CommonTokTblsGetFrameData(pBase, nFrame, ppDataPtr, pdwRate, pdwField08, pdwClass) \
	D2CommonOrdinal10645((DWORD)(pBase), (DWORD)(nFrame), (DWORD)(ppDataPtr), (DWORD)(pdwRate), \
		(DWORD)(pdwField08), (DWORD)(pdwClass))

#define D2CommonTokTblsGetFrameClass(pBase, nFrame, pdwClass) \
	D2CommonOrdinal10646((DWORD)(pBase), (DWORD)(nFrame), (DWORD)(pdwClass))

#define D2CommonITEMSCreateItemData(dwPool, hItem) \
	D2CommonOrdinal10647((DWORD)(dwPool), (DWORD)(hItem))

#define D2CommonITEMSFreeItemData(dwPool, hItem) \
	D2CommonOrdinal10648((DWORD)(dwPool), (DWORD)(hItem))

#define D2CommonITEMSGetCategory(hItem) \
	((BYTE)D2CommonOrdinal10649((DWORD)(hItem)))

#define D2CommonITEMSSetCategory(hItem, bCategory) \
	D2CommonOrdinal10650((DWORD)(hItem), (DWORD)(bCategory))

#define D2CommonITEMSGetItemSeed(hItem) \
	((void *)D2CommonOrdinal10651((DWORD)(hItem)))

#define D2CommonITEMSAdvanceItemSeed(hItem) \
	D2CommonOrdinal10652((DWORD)(hItem))

#define D2CommonITEMSGetItemSeedValue(hItem) \
	D2CommonOrdinal10653((DWORD)(hItem))

#define D2CommonITEMSSetItemSeedValue(hItem, dwSeedValue) \
	D2CommonOrdinal10654((DWORD)(hItem), (DWORD)(dwSeedValue))

#define D2CommonITEMSDataGetModType(hItem) \
	D2CommonOrdinal10655((DWORD)(hItem))

#define D2CommonITEMSDataSetModType(hItem, dwValue) \
	D2CommonOrdinal10656((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataGetMagicPrefixAt(hItem, nIndex) \
	D2CommonOrdinal10657((DWORD)(hItem), (DWORD)(nIndex))

#define D2CommonITEMSDataSetMagicPrefixAt(hItem, dwValue, nIndex) \
	D2CommonOrdinal10658((DWORD)(hItem), (DWORD)(dwValue), (DWORD)(nIndex))

#define D2CommonITEMSDataGetMagicSuffixAt(hItem, nIndex) \
	D2CommonOrdinal10659((DWORD)(hItem), (DWORD)(nIndex))

#define D2CommonITEMSDataSetMagicSuffixAt(hItem, dwValue, nIndex) \
	D2CommonOrdinal10660((DWORD)(hItem), (DWORD)(dwValue), (DWORD)(nIndex))

#define D2CommonITEMSDataGetRarePrefix(hItem) \
	D2CommonOrdinal10661((DWORD)(hItem))

#define D2CommonITEMSDataSetRarePrefix(hItem, dwValue) \
	D2CommonOrdinal10662((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataGetRareSuffix(hItem) \
	D2CommonOrdinal10663((DWORD)(hItem))

#define D2CommonITEMSDataSetRareSuffix(hItem, dwValue) \
	D2CommonOrdinal10664((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataTestFlags(hItem, dwMask) \
	D2CommonOrdinal10665((DWORD)(hItem), (DWORD)(dwMask))

#define D2CommonITEMSDataSetFlags(hItem, dwMask, fSet) \
	D2CommonOrdinal10666((DWORD)(hItem), (DWORD)(dwMask), (DWORD)(fSet))

#define D2CommonITEMSDataGetFlags(hItem) \
	D2CommonOrdinal10667((DWORD)(hItem))

#define D2CommonITEMSDataTestCmdFlags(hItem, dwMask) \
	D2CommonOrdinal10668((DWORD)(hItem), (DWORD)(dwMask))

#define D2CommonITEMSDataSetCmdFlags(hItem, dwMask, fSet) \
	D2CommonOrdinal10669((DWORD)(hItem), (DWORD)(dwMask), (DWORD)(fSet))

#define D2CommonITEMSDataGetCmdFlags(hItem) \
	D2CommonOrdinal10670((DWORD)(hItem))

#define D2CommonITEMSDataSetMagicPrefix(hItem, dwValue) \
	D2CommonOrdinal10671((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataGetMagicPrefix(hItem) \
	D2CommonOrdinal10672((DWORD)(hItem))

#define D2CommonITEMSDataSetMagicSuffix(hItem, dwValue) \
	D2CommonOrdinal10673((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataGetMagicSuffix(hItem) \
	D2CommonOrdinal10674((DWORD)(hItem))

#define D2CommonITEMSGetLevelId(hItem) \
	D2CommonOrdinal10675((DWORD)(hItem))

#define D2CommonITEMSSetLevelId(hItem, nLevelId) \
	D2CommonOrdinal10676((DWORD)(hItem), (DWORD)(nLevelId))

#define D2CommonITEMSDataGetPage(hItem) \
	((BYTE)D2CommonOrdinal10677((DWORD)(hItem)))

#define D2CommonITEMSDataSetPage(hItem, bValue) \
	D2CommonOrdinal10678((DWORD)(hItem), (DWORD)(bValue))

#define D2CommonITEMSDataGetSwapPage(hItem) \
	((BYTE)D2CommonOrdinal10679((DWORD)(hItem)))

#define D2CommonITEMSDataSetSwapPage(hItem, bValue) \
	D2CommonOrdinal10680((DWORD)(hItem), (DWORD)(bValue))

#define D2CommonITEMSGetNameBuffer(hItem) \
	((char *)D2CommonOrdinal10681((DWORD)(hItem)))

#define D2CommonITEMSSetNameBuffer(hItem, pszSrc) \
	D2CommonOrdinal10682((DWORD)(hItem), (DWORD)(pszSrc))

#define D2CommonITEMSDataGetEarLevel(hItem) \
	((BYTE)D2CommonOrdinal10683((DWORD)(hItem)))

#define D2CommonITEMSDataSetEarLevel(hItem, bValue) \
	D2CommonOrdinal10684((DWORD)(hItem), (DWORD)(bValue))

#define D2CommonITEMSDataGetGfxIndex(hItem) \
	((BYTE)D2CommonOrdinal10685((DWORD)(hItem)))

#define D2CommonITEMSDataSetGfxIndex(hItem, bValue) \
	D2CommonOrdinal10686((DWORD)(hItem), (DWORD)(bValue))

#define D2CommonITEMSIsWeapon(hItem) \
	D2CommonOrdinal10687((DWORD)(hItem))

#define D2CommonITEMSIsWeaponNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10688((DWORD)(dwItemClassIndex))

#define D2CommonITEMSDataGetUniqueOrSetRow(hItem) \
	D2CommonOrdinal10689((DWORD)(hItem))

#define D2CommonITEMSDataSetUniqueOrSetRow(hItem, dwValue) \
	D2CommonOrdinal10690((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataSetOwnerId(hItem, dwValue) \
	D2CommonOrdinal10691((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSDataGetOwnerId(hItem) \
	D2CommonOrdinal10692((DWORD)(hItem))

#define D2CommonITEMSIsArmor(hItem) \
	D2CommonOrdinal10693((DWORD)(hItem))

#define D2CommonITEMSIsArmorNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10694((DWORD)(dwItemClassIndex))

#define D2CommonITEMSTestHasBodyLoc(hItem) \
	D2CommonOrdinal10695((DWORD)(hItem))

#define D2CommonITEMSIsMiscellaneousNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10696((DWORD)(dwItemClassIndex))

#define D2CommonITEMSIsMiscellaneous(hItem) \
	D2CommonOrdinal10697((DWORD)(hItem))

#define D2CommonITEMSGetZeroA(dwUnused1) \
	D2CommonOrdinal10698((DWORD)(dwUnused1))

#define D2CommonITEMSGetZeroB(dwUnused1) \
	D2CommonOrdinal10699((DWORD)(dwUnused1))

#define D2CommonITEMSGetConstantFive(dwUnused1) \
	D2CommonOrdinal10700((DWORD)(dwUnused1))

#define D2CommonITEMSTestSellable(hItem) \
	((BOOL)D2CommonOrdinal10701((DWORD)(hItem)))

#define D2CommonITEMSGetTypeWClass(hItem) \
	D2CommonOrdinal10702((DWORD)(hItem))

#define D2CommonITEMSGetTypeTwoHandedWClass(hItem) \
	D2CommonOrdinal10703((DWORD)(hItem))

#define D2CommonITEMSGetTypeCode(hItem) \
	D2CommonOrdinal10704((DWORD)(hItem))

#define D2CommonITEMSGetTypeAlternateGfxOrCode(hItem) \
	D2CommonOrdinal10705((DWORD)(hItem))

#define D2CommonITEMSGetTypeComponent(hItem) \
	((BYTE)D2CommonOrdinal10706((DWORD)(hItem)))

#define D2CommonITEMSGetTypeInvWidthHeight(hItem, pbInvWidth, pbInvHeight) \
	D2CommonOrdinal10707((DWORD)(hItem), (DWORD)(pbInvWidth), (DWORD)(pbInvHeight))

#define D2CommonITEMSGetTypeBodyLocs(hItem, pbBodyLoc1, pbBodyLoc2) \
	D2CommonOrdinal10708((DWORD)(hItem), (DWORD)(pbBodyLoc1), (DWORD)(pbBodyLoc2))

#define D2CommonITEMSGetType(hItem) \
	D2CommonOrdinal10709((DWORD)(hItem))

#define D2CommonITEMSGetTypeNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10710((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetTypeLevel(hItem) \
	((BYTE)D2CommonOrdinal10711((DWORD)(hItem)))

#define D2CommonITEMSMaskFlags(dwFlagsA, dwFlagsB) \
	D2CommonOrdinal10712((DWORD)(dwFlagsA), (DWORD)(dwFlagsB))

#define D2CommonITEMSSetOrClearFlags(pdwFlags, dwMask, fSet) \
	D2CommonOrdinal10713((DWORD)(pdwFlags), (DWORD)(dwMask), (DWORD)(fSet))

#define D2CommonITEMSTestFitsInventorySpace(hItem, hOwner, pnFitsWidth, pnFitsHeight, pnFitsCount) \
	D2CommonOrdinal10714((DWORD)(hItem), (DWORD)(hOwner), (DWORD)(pnFitsWidth), (DWORD)(pnFitsHeight), \
		(DWORD)(pnFitsCount))

#define D2CommonITEMSGetRequiredLevel(hItem) \
	((int)D2CommonOrdinal10715((DWORD)(hItem)))

#define D2CommonITEMSTestTypeBodyLocEqual(hItem, dwValue) \
	D2CommonOrdinal10716((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSGetTypeThrowable(hItem) \
	D2CommonOrdinal10717((DWORD)(hItem))

#define D2CommonITEMSGetTypeMissileType(hUnit) \
	D2CommonOrdinal10718((DWORD)(hUnit))

#define D2CommonITEMSGetTypeRangeAdder(hUnit) \
	((BYTE)D2CommonOrdinal10719((DWORD)(hUnit)))

#define D2CommonITEMSGetTypeThrowableNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10720((DWORD)(dwItemClassIndex))

#define D2CommonITEMSTestTypeWClassEqualNoCheck(dwItemClassIndex, dwValue) \
	D2CommonOrdinal10721((DWORD)(dwItemClassIndex), (DWORD)(dwValue))

#define D2CommonITEMSTestTypeWClassEqual(hItem, dwValue) \
	D2CommonOrdinal10722((DWORD)(hItem), (DWORD)(dwValue))

#define D2CommonITEMSGetTypeQuivered(hItem) \
	D2CommonOrdinal10723((DWORD)(hItem))

#define D2CommonITEMSGetTypeQuiveredNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10724((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetTypeTwoHanded(hItem) \
	D2CommonOrdinal10725((DWORD)(hItem))

#define D2CommonITEMSGetTypeTwoHandedNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10726((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetTypeStackable(hItem) \
	D2CommonOrdinal10727((DWORD)(hItem))

#define D2CommonITEMSGetTypeBelt(hItem) \
	D2CommonOrdinal10728((DWORD)(hItem))

#define D2CommonITEMSTestTypeGroupMatch(hItem1, hItem2) \
	D2CommonOrdinal10729((DWORD)(hItem1), (DWORD)(hItem2))

#define D2CommonITEMSTestAutobeltOrFits(ptInventory, hItem) \
	D2CommonOrdinal10730((DWORD)(ptInventory), (DWORD)(hItem))

#define D2CommonITEMSGetTypeUseable(hItem) \
	D2CommonOrdinal10731((DWORD)(hItem))

#define D2CommonITEMSGetTypeUnique(hItem) \
	D2CommonOrdinal10732((DWORD)(hItem))

#define D2CommonITEMSGetTypeQuest(hItem) \
	D2CommonOrdinal10733((DWORD)(hItem))

#define D2CommonITEMSTestTypeSellableNoCheck(dwItemClassIndex) \
	((BOOL)D2CommonOrdinal10734((DWORD)(dwItemClassIndex)))

#define D2CommonITEMSGetPrice(hItem, dwCapFlag, dwQuestArg, nVendor, nMode) \
	D2CommonOrdinal10735((DWORD)(hItem), (DWORD)(dwCapFlag), (DWORD)(dwQuestArg), (DWORD)(nVendor), \
		(DWORD)(nMode))

#define D2CommonITEMSCountUnidentified(hUnit) \
	D2CommonOrdinal10736((DWORD)(hUnit))

#define D2CommonITEMSTestRepairable(hItem) \
	D2CommonOrdinal10737((DWORD)(hItem))

#define D2CommonITEMSTestTypeIdentifiable(eType) \
	D2CommonOrdinal10738((DWORD)(eType))

#define D2CommonITEMSGetTranslateTblFirst(eType) \
	D2CommonOrdinal10739((DWORD)(eType))

#define D2CommonITEMSGetTypeMaxStackNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10740((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetTypeMaxStack(hItem) \
	D2CommonOrdinal10741((DWORD)(hItem))

#define D2CommonITEMSGetTypeMinStackNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10742((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetTypeMinStack(hItem) \
	D2CommonOrdinal10743((DWORD)(hItem))

#define D2CommonITEMSTestTypeBitfieldFourthBit(hItem) \
	D2CommonOrdinal10744((DWORD)(hItem))

#define D2CommonITEMSTestTypeBitfieldThirdBit(dwItemClassIndex) \
	D2CommonOrdinal10745((DWORD)(dwItemClassIndex))

#define D2CommonITEMSTestTypeBitfieldSecondBit(dwItemClassIndex) \
	D2CommonOrdinal10746((DWORD)(dwItemClassIndex))

#define D2CommonITEMSTestTypeBitfieldFirstBit(dwItemClassIndex) \
	D2CommonOrdinal10747((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetSpellIcon(hItem) \
	((BYTE)D2CommonOrdinal10748((DWORD)(hItem)))

#define D2CommonITEMSGetTypeDurWarning(hItem) \
	((BYTE)D2CommonOrdinal10749((DWORD)(hItem)))

#define D2CommonITEMSGetTypeQntWarning(hItem) \
	((BYTE)D2CommonOrdinal10750((DWORD)(hItem)))

#define D2CommonITEMSGetTypeStrBonus(hItem) \
	D2CommonOrdinal10751((DWORD)(hItem))

#define D2CommonITEMSGetTypeDexBonus(hItem) \
	D2CommonOrdinal10752((DWORD)(hItem))

#define D2CommonITEMSGetTypeHasInvNoCheck(dwItemClassIndex) \
	D2CommonOrdinal10753((DWORD)(dwItemClassIndex))

#define D2CommonITEMSGetTypeHasInv(hItem) \
	D2CommonOrdinal10754((DWORD)(hItem))

#define D2CommonITEMSTestTypeHasDurability(hItem) \
	((BOOL)D2CommonOrdinal10755((DWORD)(hItem)))

#define D2CommonITEMSTestKeepsRolledQuality(hItem) \
	D2CommonOrdinal10756((DWORD)(hItem))

#define D2CommonITEMSGetTypeGemSocketsNoCheck(dwItemClassIndex) \
	((BYTE)D2CommonOrdinal10757((DWORD)(dwItemClassIndex)))

#define D2CommonITEMSGetTypeGemSockets(hItem) \
	((BYTE)D2CommonOrdinal10758((DWORD)(hItem)))

#define D2CommonITEMSGetWClassIndex(hItem) \
	((WORD)D2CommonOrdinal10759((DWORD)(hItem)))

#define D2CommonITEMSTestTypeTransmogrifyNoCheck(dwItemClassIndex) \
	((BOOL)D2CommonOrdinal10760((DWORD)(dwItemClassIndex)))

#define D2CommonITEMSTestTypeTransmogrify(hItem) \
	((BOOL)D2CommonOrdinal10761((DWORD)(hItem)))

#define D2CommonITEMSTestModTypeMagicOrAbove(hItem) \
	((BOOL)D2CommonOrdinal10762((DWORD)(hItem)))

#define D2CommonITEMSGetTypeHitClass(hItem) \
	((BYTE)D2CommonOrdinal10763((DWORD)(hItem)))

#define D2CommonITEMSTestOneOrTwoHanded(hPlayer, hItem) \
	((BOOL)D2CommonOrdinal10764((DWORD)(hPlayer), (DWORD)(hItem)))

#define D2CommonITEMSGetColorTransform(hUnit, hItem, pbTransform, fInventory) \
	((BYTE *)D2CommonOrdinal10765(0, 0, (DWORD)(hUnit), (DWORD)(hItem), (DWORD)(pbTransform), \
		(DWORD)(fInventory)))

#define D2CommonITEMSTestModifiable(hItem) \
	((BOOL)D2CommonOrdinal10766((DWORD)(hItem)))

#define D2CommonITEMSGetWeaponGroupEnum(hItem) \
	((BYTE)D2CommonOrdinal10767((DWORD)(hItem)))

#define D2CommonITEMSTestCanStack(hItem1, hItem2) \
	((BOOL)D2CommonOrdinal10768((DWORD)(hItem1), (DWORD)(hItem2)))

#define D2CommonITEMSValidateQuantity(hUnit) \
	((BOOL)D2CommonOrdinal10769((DWORD)(hUnit)))

#define D2CommonITEMSTestSetComplete(hUnit, hItem) \
	((BOOL)D2CommonOrdinal10770((DWORD)(hUnit), (DWORD)(hItem)))

#define D2CommonITEMSTestEffectsActive(hItem) \
	((BOOL)D2CommonOrdinal10771((DWORD)(hItem)))

#define D2CommonITEMSScaleValueByPlayerClass(hUnit, dwValue) \
	D2CommonOrdinal10772((DWORD)(hUnit), (DWORD)(dwValue))

#define D2CommonITEMSScaleValueByPlayerClassAlt(hUnit, dwValue) \
	D2CommonOrdinal10773((DWORD)(hUnit), (DWORD)(dwValue))

#define D2CommonLoggingUnpackDword(nPacked, pdwLow, pnHigh) \
	D2CommonOrdinal10774((DWORD)(nPacked), (DWORD)(pdwLow), (DWORD)(pnHigh))

#define D2CommonITEMSDataGetPreviousPage(hItem) \
	((BYTE)D2CommonOrdinal10775((DWORD)(hItem)))

#define D2CommonITEMSDataSetPreviousPage(hItem, bValue) \
	D2CommonOrdinal10776((DWORD)(hItem), (DWORD)(bValue))

#define D2CommonITEMSModsApply(eMods, hItem, pMods, nType, fRemove) \
	D2CommonOrdinal10777((DWORD)(eMods), (DWORD)(hItem), (DWORD)(pMods), (DWORD)(nType), (DWORD)(fRemove))

#define D2CommonITEMSModsApplySetProperties(hOwner, hItem, ptSetMods) \
	((DWORD (__stdcall *)(DWORD, DWORD, DWORD))D2CommonOrdinal10778)((DWORD)(hOwner), (DWORD)(hItem), \
		(DWORD)(ptSetMods))

#define D2CommonITEMSModsGetStatId(nOffset) \
	((int)D2CommonOrdinal10779((DWORD)(nOffset)))

#define D2CommonITEMSModsTestMagicFits(dwItem, ptRec) \
	((BOOL)D2CommonOrdinal10780((DWORD)(dwItem), (DWORD)(ptRec)))

#define D2CommonITEMSModsTestRareFits(dwItem, ptRec) \
	((BOOL)D2CommonOrdinal10781((DWORD)(dwItem), (DWORD)(ptRec)))

#define D2CommonITEMSModsTestQualityFits(dwItem, ptRec) \
	((BOOL)D2CommonOrdinal10782((DWORD)(dwItem), (DWORD)(ptRec)))

#define D2CommonITEMSRollElixirKind(hItem) \
	D2CommonOrdinal10783((DWORD)(hItem))

#define D2CommonITEMSAddRandomSkillStatsByType(hItem, nLevel) \
	D2CommonOrdinal10784((DWORD)(hItem), (DWORD)(nLevel))

#define D2CommonCOMPOSITGetCofName(hUnit, nClass, nMode, eTOU, dwInventory, pszOut, pdwWeaponClass, fPath, \
		fResolve) \
	D2CommonOrdinal10785((DWORD)(hUnit), (DWORD)(nClass), (DWORD)(nMode), (DWORD)(eTOU), \
		(DWORD)(dwInventory), (DWORD)(pszOut), (DWORD)(pdwWeaponClass), (DWORD)(fPath), (DWORD)(fResolve))

#define D2CommonCOMPOSITGetCofNameForUnit(hUnit, dwCofNameOut, dwWeaponClassOut, dwFullPath, dwResolve, \
		dwInventory, dwMode) \
	D2CommonOrdinal10786((DWORD)(hUnit), (DWORD)(dwCofNameOut), (DWORD)(dwWeaponClassOut), \
		(DWORD)(dwFullPath), (DWORD)(dwResolve), (DWORD)(dwInventory), (DWORD)(dwMode))

#define D2CommonCOMPOSITGetWeaponClassIndex(dwCode) \
	D2CommonOrdinal10787((DWORD)(dwCode))

#define D2CommonCOMPOSITGetWeaponClass(hUnit, eTOU, nClass, nMode, dwInventory, pdwIndex) \
	D2CommonOrdinal10788((DWORD)(hUnit), (DWORD)(eTOU), (DWORD)(nClass), (DWORD)(nMode), \
		(DWORD)(dwInventory), (DWORD)(pdwIndex))

#define D2CommonCOMPOSITGetUnitWeaponClass(hUnit, dwInventory, dwIndexOut, dwMode, dwTranslateMode) \
	((DWORD (__stdcall *)(void *, DWORD, DWORD, DWORD, DWORD))D2CommonOrdinal10789)((void *)(hUnit), \
		(DWORD)(dwInventory), (DWORD)(dwIndexOut), (DWORD)(dwMode), (DWORD)(dwTranslateMode))

#define D2CommonCOMPOSITTestSlotMember(nSlot) \
	((BOOL)D2CommonOrdinal10790((DWORD)(nSlot)))

#define D2CommonCOMPOSITIsRangedAttack(hUnit) \
	((BOOL)D2CommonOrdinal10791((DWORD)(hUnit)))

#define D2CommonCOMPOSITGetComponentByte(eComponent, pbRecord) \
	D2CommonOrdinal10792((DWORD)(eComponent), (DWORD)(pbRecord))

#define D2CommonChatNodeNew(pPool, pszInput, dwFrame) \
	D2CommonOrdinal10793((DWORD)(pPool), (DWORD)(pszInput), (DWORD)(dwFrame))

#define D2CommonChatNodeFree(pPool, hNode) \
	D2CommonOrdinal10794((DWORD)(pPool), (DWORD)(hNode))

#define D2CommonChatNodeGetDuration(hNode) \
	D2CommonOrdinal10795((DWORD)(hNode))

#define D2CommonChatNodeGetEndFrame(hNode) \
	D2CommonOrdinal10796((DWORD)(hNode))

#define D2CommonChatNodeCopyString(hNode, pszDst) \
	D2CommonOrdinal10797((DWORD)(hNode), (DWORD)(pszDst))

#define D2CommonChatNodeGetTextBox(hNode) \
	D2CommonOrdinal10798((DWORD)(hNode))

#define D2CommonChatNodeSetTextBox(hNode, dwValue) \
	D2CommonOrdinal10799((DWORD)(hNode), (DWORD)(dwValue))

#define D2CommonChatNodeGetMessageByte(hNode) \
	((BYTE)D2CommonOrdinal10800((DWORD)(hNode)))

#define D2CommonWaypointSetChatNodeMessageByte(pRecord, bVal) \
	D2CommonOrdinal10801((DWORD)(pRecord), (DWORD)(bVal))

#define D2CommonTextListNew(pPool) \
	D2CommonOrdinal10802((DWORD)(pPool))

#define D2CommonTextListFree(ptList) \
	D2CommonOrdinal10803((DWORD)(ptList))

#define D2CommonTextListInsert(ptList, wId, dwTag) \
	D2CommonOrdinal10804((DWORD)(ptList), (DWORD)(wId), (DWORD)(dwTag))

#define D2CommonTextListRemoveById(ptList, wId) \
	D2CommonOrdinal10805((DWORD)(ptList), (DWORD)(wId))

#define D2CommonTextListCountMenuLines(ptList) \
	D2CommonOrdinal10806((DWORD)(ptList))

#define D2CommonTextListFindMenuLineId(ptList, nIndex) \
	((WORD)D2CommonOrdinal10807((DWORD)(ptList), (DWORD)(nIndex)))

#define D2CommonTextListFindOpeningTextId(ptList, nIndex) \
	((WORD)D2CommonOrdinal10808((DWORD)(ptList), (DWORD)(nIndex)))

#define D2CommonTextListFindFallbackTextId(ptList, nIndex) \
	((WORD)D2CommonOrdinal10809((DWORD)(ptList), (DWORD)(nIndex)))

#define D2CommonTextListPack(pTextHead, pMsgList) \
	D2CommonOrdinal10810((DWORD)(pTextHead), (DWORD)(pMsgList))

#define D2CommonTextListUnpack(pTextHead, pMsgList) \
	D2CommonOrdinal10811((DWORD)(pTextHead), (DWORD)(pMsgList))

#define D2CommonTextListSort(pTextHead) \
	D2CommonOrdinal10812((DWORD)(pTextHead))

#define D2CommonSeedInit(ptSeed) \
	D2CommonOrdinal10813((DWORD)(ptSeed))

#define D2CommonSeedSet(ptSeed, dwSeed) \
	D2CommonOrdinal10814((DWORD)(ptSeed), (DWORD)(dwSeed))

#define D2CommonSeedGetLow(ptSeed) \
	D2CommonOrdinal10815((DWORD)(ptSeed))

#define D2CommonSeedGetHigh(ptSeed) \
	D2CommonOrdinal10816((DWORD)(ptSeed))

#define D2CommonSeedNothing() \
	D2CommonOrdinal10817()

#define D2CommonSeedNext(ptSeed) \
	D2CommonOrdinal10818((DWORD)(ptSeed), 0)

#define D2CommonSeedRand(ptSeed, nMax) \
	D2CommonOrdinal10819((DWORD)(ptSeed), (DWORD)(nMax))

#define D2CommonSeedBetween(ptSeed, nLowBound, nHighBound) \
	((int)D2CommonOrdinal10820((DWORD)(ptSeed), (DWORD)(nLowBound), (DWORD)(nHighBound)))

#define D2CommonSeedInvent(nSalt) \
	D2CommonOrdinal10821((DWORD)(nSalt))

#define D2CommonSeedSetState(ptSeed, dwLow, dwHigh) \
	D2CommonOrdinal10822((DWORD)(ptSeed), (DWORD)(dwLow), (DWORD)(dwHigh))

#define D2CommonSeedGetState(ptSeed, pdwLow, pdwHigh) \
	D2CommonOrdinal10823((DWORD)(ptSeed), (DWORD)(pdwLow), (DWORD)(pdwHigh))

#define D2CommonEnvTick(ptAct, ptRoom) \
	((BOOL)D2CommonOrdinal10824((DWORD)(ptAct), (DWORD)(ptRoom)))

#define D2CommonEnvGetSun(ptAct, pfX, pfY, pfZ, pdwZero) \
	D2CommonOrdinal10825((DWORD)(ptAct), (DWORD)(pfX), (DWORD)(pfY), (DWORD)(pfZ), (DWORD)(pdwZero))

#define D2CommonEnvGetIntensity(ptAct) \
	((int)D2CommonOrdinal10826((DWORD)(ptAct)))

#define D2CommonEnvGetColor(ptAct, pbRed, pbGreen, pbBlue) \
	D2CommonOrdinal10827((DWORD)(ptAct), (DWORD)(pbRed), (DWORD)(pbGreen), (DWORD)(pbBlue))

#define D2CommonEnvGetZeroField(ptAct) \
	D2CommonOrdinal10828((DWORD)(ptAct))

#define D2CommonEnvNextCycle(ptAct, ptRoom) \
	D2CommonOrdinal10829((DWORD)(ptAct), (DWORD)(ptRoom))

#define D2CommonEnvGetCycle(ptAct) \
	((int)D2CommonOrdinal10830((DWORD)(ptAct)))

#define D2CommonEnvGetState(ptAct, pnCycle, pnAngle, pfAlt) \
	D2CommonOrdinal10831((DWORD)(ptAct), (DWORD)(pnCycle), (DWORD)(pnAngle), (DWORD)(pfAlt))

#define D2CommonEnvSetCycle(ptAct, ptRoom, nIndex, nAngle, fAlt) \
	D2CommonOrdinal10832((DWORD)(ptAct), (DWORD)(ptRoom), (DWORD)(nIndex), (DWORD)(nAngle), (DWORD)(fAlt))

#define D2CommonEnvGetCycleValue(ptAct) \
	((int)D2CommonOrdinal10833((DWORD)(ptAct)))

#define D2CommonEnvGetCycleTime(ptAct) \
	((int)D2CommonOrdinal10834((DWORD)(ptAct)))

#define D2CommonEnvNextCycleTime(ptAct, ptRoom) \
	D2CommonOrdinal10835((DWORD)(ptAct), (DWORD)(ptRoom))

#define D2CommonEnvSetAltCycle(ptAct) \
	D2CommonOrdinal10836((DWORD)(ptAct))

#define D2CommonEnvSetUsualCycle(ptAct) \
	D2CommonOrdinal10837((DWORD)(ptAct))

#define D2CommonSkillsLookupKeyAFromKeyB(dwKeyB) \
	D2CommonOrdinal10838((DWORD)(dwKeyB))

#define D2CommonSkillsLookupKeyBFromKeyA(dwKeyA) \
	D2CommonOrdinal10839((DWORD)(dwKeyA))

#define D2CommonSkillsUpdatePassive(hUnit, nSkillId) \
	D2CommonOrdinal10840((DWORD)(hUnit), (DWORD)(nSkillId))

#define D2CommonSkillsRefreshPassives(hUnit) \
	((int)D2CommonOrdinal10841((DWORD)(hUnit)))

#define D2CommonSkillsGetNodeModeIfAny(dwUnit, ptSkill) \
	D2CommonOrdinal10842((DWORD)(dwUnit), (DWORD)(ptSkill))

#define D2CommonSkillsGetNodeShiver(hSkill) \
	((BYTE)D2CommonOrdinal10843((DWORD)(hSkill)))

#define D2CommonSkillsGetNodeRange(hSkill) \
	D2CommonOrdinal10844((DWORD)(hSkill))

#define D2CommonSkillsAllocNode(pPool) \
	D2CommonOrdinal10845((DWORD)(pPool))

#define D2CommonSkillsRebuildCaches(hUnit) \
	D2CommonOrdinal10846((DWORD)(hUnit))

#define D2CommonSkillsNodeGetNext(ptSkill) \
	D2CommonOrdinal10847((DWORD)(ptSkill))

#define D2CommonSkillsHeadGetFirst(ptInfo) \
	D2CommonOrdinal10848((DWORD)(ptInfo))

#define D2CommonSkillsFindByKey(hUnit, dwKey) \
	D2CommonOrdinal10849((DWORD)(hUnit), (DWORD)(dwKey))

#define D2CommonSkillsDecrementSkill(hUnit, nSkillId) \
	D2CommonOrdinal10850((DWORD)(hUnit), (DWORD)(nSkillId))

#define D2CommonSkillsAddSkill(hUnit, nSkillId) \
	D2CommonOrdinal10851((DWORD)(hUnit), (DWORD)(nSkillId))

#define D2CommonSkillsSetSkillLevel(hUnit, nSkillId, nLevel, fRemove) \
	D2CommonOrdinal10852((DWORD)(hUnit), (DWORD)(nSkillId), (DWORD)(nLevel), (DWORD)(fRemove))

#define D2CommonSkillsExchangeSelected(hUnit, ptNewSelected) \
	D2CommonOrdinal10853((DWORD)(hUnit), (DWORD)(ptNewSelected))

#define D2CommonSkillsGetSelectedUnsafe(hUnit) \
	D2CommonOrdinal10854((DWORD)(hUnit))

#define D2CommonSkillsFreeList(hUnit) \
	D2CommonOrdinal10855((DWORD)(hUnit))

#define D2CommonSkillsCacheLeftSkill(dwUnused, hUnit, nSkillId) \
	D2CommonOrdinal10856((void *)(hUnit), (DWORD)(nSkillId))

#define D2CommonSkillsCacheRightSkill(dwUnused, hUnit, nSkillId) \
	D2CommonOrdinal10857((void *)(hUnit), (DWORD)(nSkillId))

#define D2CommonSkillsGetNodeType(hSkill) \
	D2CommonOrdinal10858((DWORD)(hSkill))

#define D2CommonSkillsTestUsable(hUnit, ptSkill) \
	((int)D2CommonOrdinal10859((DWORD)(hUnit), (DWORD)(ptSkill)))

#define D2CommonSkillsGetNodeSlot(hUnit, ptSkill) \
	((BYTE)D2CommonOrdinal10860((DWORD)(hUnit), (DWORD)(ptSkill)))

#define D2CommonSkillsGetNodeTypeRecord(ptSkill) \
	D2CommonOrdinal10861((DWORD)(ptSkill))

#define D2CommonSkillsGetComboFlags(hUnit) \
	D2CommonOrdinal10862((DWORD)(hUnit))

#define D2CommonSkillsCalcNodeLevel(hUnit, hSkill, fCombine) \
	D2CommonOrdinal10863((DWORD)(hUnit), (DWORD)(hSkill), (DWORD)(fCombine))

#define D2CommonSkillsGetNodeScratchA(ptSkill) \
	D2CommonOrdinal10864((DWORD)(ptSkill))

#define D2CommonSkillsSetNodeScratchA(ptSkill, dwValue) \
	D2CommonOrdinal10865((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsGetNodeScratchB(ptSkill) \
	D2CommonOrdinal10866((DWORD)(ptSkill))

#define D2CommonSkillsSetNodeScratchB(ptSkill, dwValue) \
	D2CommonOrdinal10867((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsResetNode(ptSkill) \
	D2CommonOrdinal10868((DWORD)(ptSkill))

#define D2CommonSkillsSetNodeMode(ptSkill, dwValue) \
	D2CommonOrdinal10869((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsGetNodeMode(ptSkill) \
	D2CommonOrdinal10870((DWORD)(ptSkill))

#define D2CommonSkillsGetNodeScratchC(ptSkill) \
	D2CommonOrdinal10871((DWORD)(ptSkill))

#define D2CommonSkillsGetNodeScratchD(ptSkill) \
	D2CommonOrdinal10872((DWORD)(ptSkill))

#define D2CommonSkillsGetNodeScratchE(ptSkill) \
	D2CommonOrdinal10873((DWORD)(ptSkill))

#define D2CommonSkillsGetNodeScratchF(ptSkill) \
	D2CommonOrdinal10874((DWORD)(ptSkill))

#define D2CommonSkillsSetNodeScratchC(ptSkill, dwValue) \
	D2CommonOrdinal10875((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsSetNodeScratchD(ptSkill, dwValue) \
	D2CommonOrdinal10876((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsSetNodeScratchE(ptSkill, dwValue) \
	D2CommonOrdinal10877((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsSetNodeScratchF(ptSkill, dwValue) \
	D2CommonOrdinal10878((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsSetNodeFlags(ptSkill, dwValue) \
	D2CommonOrdinal10879((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsGetNodeFlags(hSkill) \
	D2CommonOrdinal10880((DWORD)(hSkill))

#define D2CommonSkillsGetTypeReqLevel(nSkillId) \
	((BYTE)D2CommonOrdinal10881((DWORD)(nSkillId)))

#define D2CommonSkillsCalcReqLevel(hUnit, nSkillId) \
	D2CommonOrdinal10882((DWORD)(hUnit), (DWORD)(nSkillId))

#define D2CommonSkillsTestPrerequisites(hUnit, ptType, nSkillId) \
	((BOOL)D2CommonOrdinal10883((DWORD)(hUnit), (DWORD)(ptType), (DWORD)(nSkillId)))

#define D2CommonSkillsTestStatRequirements(hUnit, ptType, nSkillId) \
	((BOOL)D2CommonOrdinal10884((DWORD)(hUnit), (DWORD)(ptType), (DWORD)(nSkillId)))

#define D2CommonSkillsGetTypeParamOne(nSkillId) \
	D2CommonOrdinal10885((DWORD)(nSkillId))

#define D2CommonSkillsGetTypeParamTwo(nSkillId) \
	D2CommonOrdinal10886((DWORD)(nSkillId))

#define D2CommonSkillsGetTypeParamThree(nSkillId) \
	D2CommonOrdinal10887((DWORD)(nSkillId))

#define D2CommonSkillsGetTypeParamFour(nSkillId) \
	D2CommonOrdinal10888((DWORD)(nSkillId))

#define D2CommonSkillsGetTypeParamFive(nSkillId) \
	D2CommonOrdinal10889((DWORD)(nSkillId))

#define D2CommonSkillsGetTypeParamSix(nSkillId) \
	D2CommonOrdinal10890((DWORD)(nSkillId))

#define D2CommonSkillsCalcParamOneByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10891((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcParamThreeByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10892((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcParamFiveByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10893((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcToHitByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10894((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsSumNodeBonus(hUnit, dwKey) \
	D2CommonOrdinal10895((DWORD)(hUnit), (DWORD)(dwKey))

#define D2CommonSkillsCalcManaCost(nSkillId, nLevel) \
	((int)D2CommonOrdinal10896((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcValueA(nSkillId, hUnit, nLevel) \
	((int)D2CommonOrdinal10897((DWORD)(nSkillId), (DWORD)(hUnit), (DWORD)(nLevel)))

#define D2CommonSkillsCalcValueB(nSkillId, hUnit, nLevel) \
	((int)D2CommonOrdinal10898((DWORD)(nSkillId), (DWORD)(hUnit), (DWORD)(nLevel)))

#define D2CommonSkillsCalcEMinByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10899((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcEMaxByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10900((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcELenByLevel(nSkillId, nLevel) \
	((int)D2CommonOrdinal10901((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsGetTypeHitShift(nSkillId) \
	((BYTE)D2CommonOrdinal10902((DWORD)(nSkillId)))

#define D2CommonSkillsGetTypeCharClass(nSkillId) \
	((BYTE *)D2CommonOrdinal10903((DWORD)(nSkillId)))

#define D2CommonSkillsGetNodeCount(ptSkill) \
	D2CommonOrdinal10904((DWORD)(ptSkill))

#define D2CommonSkillsSetNodeCount(ptSkill, dwValue) \
	D2CommonOrdinal10905((DWORD)(ptSkill), (DWORD)(dwValue))

#define D2CommonSkillsClearFlagIfMatched(hUnit, pdwFlag, pdwOut, pdwIndex) \
	D2CommonOrdinal10906((DWORD)(hUnit), (DWORD)(pdwFlag), (DWORD)(pdwOut), (DWORD)(pdwIndex))

#define D2CommonSkillsUpdateNodeSeed(hUnit, dwMode, dwKey) \
	D2CommonOrdinal10907((DWORD)(hUnit), (DWORD)(dwMode), (DWORD)(dwKey))

#define D2CommonSkillsGetNodeRangeForUnit(dwUnit, hSkill) \
	D2CommonOrdinal10908((DWORD)(dwUnit), (DWORD)(hSkill))

#define D2CommonSkillsUnitTestGuardF(hUnit, dwGate) \
	((BOOL)D2CommonOrdinal10909((DWORD)(hUnit), (DWORD)(dwGate)))

#define D2CommonSkillsUnitTestGuardD(hUnit) \
	((BOOL)D2CommonOrdinal10910((DWORD)(hUnit)))

#define D2CommonSkillsUnitTestGuardA(hUnit) \
	((BOOL)D2CommonOrdinal10911((DWORD)(hUnit)))

#define D2CommonSkillsUnitTestGuardE(hUnit) \
	((BOOL)D2CommonOrdinal10912((DWORD)(hUnit)))

#define D2CommonSkillsUnitTestGuardB(hUnit) \
	((BOOL)D2CommonOrdinal10913((DWORD)(hUnit)))

#define D2CommonSkillsUnitTestGuardC(hUnit) \
	((BOOL)D2CommonOrdinal10914((DWORD)(hUnit)))

#define D2CommonSkillsGetItemSkillValueAny(hUnit, nBonus, hItem, pnOut, hSkill, dwToHitColumn) \
	((int)D2CommonOrdinal10915((DWORD)(hUnit), (DWORD)(nBonus), (DWORD)(hItem), (DWORD)(pnOut), \
		(DWORD)(hSkill), (DWORD)(dwToHitColumn)))

#define D2CommonSkillsGetItemSkillValue(hUnit, nBonus, hItem, hSkill, dwToHitColumn) \
	((int)D2CommonOrdinal10916((DWORD)(hUnit), (DWORD)(nBonus), (DWORD)(hItem), (DWORD)(hSkill), \
		(DWORD)(dwToHitColumn)))

#define D2CommonSkillsAddFieldA(dwUnitX, dwUnitY, dwTargetX, dwTargetY, dwRoom, dwPattern) \
	((BOOL)D2CommonOrdinal10917((DWORD)(dwUnitX), (DWORD)(dwUnitY), (DWORD)(dwTargetX), (DWORD)(dwTargetY), \
		(DWORD)(dwRoom), (DWORD)(dwPattern)))

#define D2CommonSkillsAddFieldB(dwUnitX, dwUnitY, dwTarget, dwPattern) \
	((BOOL)D2CommonOrdinal10918((DWORD)(dwUnitX), (DWORD)(dwUnitY), (DWORD)(dwTarget), (DWORD)(dwPattern)))

#define D2CommonSkillsCalcManaPoints(nSkillId, nLevel) \
	((int)D2CommonOrdinal10919((DWORD)(nSkillId), (DWORD)(nLevel)))

#define D2CommonSkillsCalcCountPercent(nCount) \
	((int)D2CommonOrdinal10920((DWORD)(nCount)))

#define D2CommonSkillsGetBonusByKey(hUnit, dwKey) \
	D2CommonOrdinal10921((DWORD)(hUnit), (DWORD)(dwKey))

#define D2CommonSkillsSetSkillBonus(hUnit, nSkillId, nBonus) \
	D2CommonOrdinal10922((DWORD)(hUnit), (DWORD)(nSkillId), (DWORD)(nBonus))

#define D2CommonSkillsAdjustBonusByKey(hUnit, dwKey, nDelta) \
	D2CommonOrdinal10923((DWORD)(hUnit), (DWORD)(dwKey), (DWORD)(nDelta))

#define D2CommonSkillsGetDifficultyTableEntry(nLevel, dwKey, nToHitColumn) \
	((int)D2CommonOrdinal10924((DWORD)(nLevel), (DWORD)(dwKey), (DWORD)(nToHitColumn)))

#define D2CommonSkillsCalcCurveValue(nLevel, nMin, nMax) \
	((int)D2CommonOrdinal10925((DWORD)(nLevel), (DWORD)(nMin), (DWORD)(nMax)))

#define D2CommonSkillsCalcCurveParamOne(nLevel, nSkillId) \
	((int)D2CommonOrdinal10926((DWORD)(nLevel), (DWORD)(nSkillId)))

#define D2CommonSkillsCalcCurveParamThree(nLevel, nSkillId) \
	((int)D2CommonOrdinal10927((DWORD)(nLevel), (DWORD)(nSkillId)))

#define D2CommonSkillsCalcCurveParamFive(nLevel, nSkillId) \
	((int)D2CommonOrdinal10928((DWORD)(nLevel), (DWORD)(nSkillId)))

#define D2CommonSkillsGetFieldPastTarget(hUnit, hTarget, pnX, pnY) \
	((BOOL)D2CommonOrdinal10929((DWORD)(hUnit), (DWORD)(hTarget), (DWORD)(pnX), (DWORD)(pnY)))

#define D2CommonSkillsAddListRange(hUnit, pnMin, pnMax, fThird) \
	((BOOL)D2CommonOrdinal10930((DWORD)(hUnit), (DWORD)(pnMin), (DWORD)(pnMax), (DWORD)(fThird)))

#define D2CommonSkillsGetRangedAttackCost(hUnit) \
	D2CommonOrdinal10931((DWORD)(hUnit))

#define D2CommonSkillsTestIdFlag(nSkillId) \
	((BOOL)D2CommonOrdinal10932((DWORD)(nSkillId)))

#define D2CommonMonsterRegionGetMissileField(ptUnit, dwPosition) \
	D2CommonOrdinal10933((DWORD)(ptUnit), (DWORD)(dwPosition))

#define D2CommonMonsterUnitGetBestSlotRank(dwUnused, ptUnit) \
	((BYTE)D2CommonOrdinal10934((DWORD)(ptUnit)))

#define D2CommonMonsterRegionLookupTableA(dwIndex) \
	((BYTE)D2CommonOrdinal10935((DWORD)(dwIndex)))

#define D2CommonMonsterRegionLookupTableB(dwIndex) \
	((BYTE)D2CommonOrdinal10936((DWORD)(dwIndex)))

#define D2CommonMonsterRegionLookupTableC(dwIndex) \
	((BYTE)D2CommonOrdinal10937((DWORD)(dwIndex)))

#define D2CommonMonsterRegionGetOffsetPair(dwIndex, pnOffsetA, pnOffsetB) \
	D2CommonOrdinal10938((DWORD)(dwIndex), (DWORD)(pnOffsetA), (DWORD)(pnOffsetB))

#define D2CommonMonsterUnitGetTypeLocalIndex(ptUnit) \
	D2CommonOrdinal10939((DWORD)(ptUnit))

#define D2CommonMonsterUtilTestModeMove(dwParam0, dwParam1, dwParam2) \
	((BOOL)D2CommonOrdinal10940((DWORD)(dwParam1), (DWORD)(dwParam2)))

#define D2CommonMonsterRegionUnitIsDemon(ptUnit) \
	((BYTE)D2CommonOrdinal10941((DWORD)(ptUnit)))

#define D2CommonMonsterRegionUnitIsUndead(ptUnit) \
	((BOOL)D2CommonOrdinal10942((DWORD)(ptUnit)))

#define D2CommonMonsterRegionUnitIsBoss(dwOverride, ptUnit) \
	((BYTE)D2CommonOrdinal10943((DWORD)(dwOverride), (DWORD)(ptUnit)))

#define D2CommonMonsterGetSpecialPosition(hUnit, pnClass, pnX, pnY, pnMode, dwUnused) \
	D2CommonOrdinal10944((DWORD)(hUnit), (DWORD)(pnClass), (DWORD)(pnX), (DWORD)(pnY), (DWORD)(pnMode), \
		(DWORD)(dwUnused))

#define D2CommonMonsterUnitApplyChartEffects(dwUnused, ptUnit) \
	D2CommonOrdinal10945((DWORD)(ptUnit))

#define D2CommonMonsterResolvePopulationIndex(dwAreaRef, dwMonsterTypeId) \
	D2CommonOrdinal10946((DWORD)(dwAreaRef), (DWORD)(dwMonsterTypeId))

#define D2CommonMonsterUnitTestFieldC(ptUnit) \
	((BOOL)D2CommonOrdinal10947((DWORD)(ptUnit)))

#define D2CommonMonsterUnitTestBoss(ptUnit) \
	((BOOL)D2CommonOrdinal10948((DWORD)(ptUnit)))

#define D2CommonMonsterRandGetComponentTableByte(ptUnit, nComponent) \
	((BYTE)D2CommonOrdinal10949((DWORD)(ptUnit), (DWORD)(nComponent)))

#define D2CommonMonsterRandGetComponentCode(hUnit, nComponent, nVariation) \
	D2CommonOrdinal10950((DWORD)(hUnit), (DWORD)(nComponent), (DWORD)(nVariation))

#define D2CommonMonsterRandGetClassComponentCode(nClass, nComponent, nVariation) \
	D2CommonOrdinal10951((DWORD)(nClass), (DWORD)(nComponent), (DWORD)(nVariation))

#define D2CommonMonsterRandRollVariations(hRand, ptMonsterNode, nMax) \
	D2CommonOrdinal10952((DWORD)(hRand), (DWORD)(ptMonsterNode), (DWORD)(nMax))

#define D2CommonMonsterRegionCreatePopulation(pPool, ppArray, pRandom, dwParam2) \
	D2CommonOrdinal10953((DWORD)(pPool), (DWORD)(ppArray), (DWORD)(pRandom), (DWORD)(dwParam2))

#define D2CommonMonsterRegionFreeLevelPopulation(pPool, ppArray) \
	D2CommonOrdinal10954((DWORD)(pPool), (DWORD)(ppArray))

#define D2CommonMonsterRegionArrayGetDword(padwArray, dwIndex) \
	D2CommonOrdinal10955((DWORD)(padwArray), (DWORD)(dwIndex))

#define D2CommonMonsterRegionFindOrAddSlot(ppArray, dwEdxParam, pRecord) \
	D2CommonOrdinal10956((DWORD)(ppArray), (DWORD)(dwEdxParam), (DWORD)(pRecord))

#define D2CommonMonsterKillsLogSetFlags(dwPlayer, eType) \
	D2CommonOrdinal10957((DWORD)(dwPlayer), (DWORD)(eType))

#define D2CommonMonsterKillsLogIncrement(dwPlayer, eType) \
	D2CommonOrdinal10958((DWORD)(dwPlayer), (DWORD)(eType))

#define D2CommonMonsterKillsLogGetCount(dwPlayer, eType) \
	D2CommonOrdinal10959((DWORD)(dwPlayer), (DWORD)(eType))

#define D2CommonMonsterKillsLogTestFlags(dwPlayer, eType) \
	((BOOL)D2CommonOrdinal10960((DWORD)(dwPlayer), (DWORD)(eType)))

#define D2CommonMonsterKillsLogClearRecord(dwPlayer) \
	D2CommonOrdinal10961((DWORD)(dwPlayer))

#define D2CommonMonsterKillsLogRollRecord(dwSeed, ptRecord, bDifficulty) \
	((BOOL)D2CommonOrdinal10962((DWORD)(dwSeed), (DWORD)(ptRecord), (DWORD)(bDifficulty)))

#define D2CommonMonsterKillsLogGetString(nIndex) \
	((void *)D2CommonOrdinal10963((DWORD)(nIndex)))

#define D2CommonMonsterKillsLogClassify(dwClassId, wNameString) \
	((int)D2CommonOrdinal10964((DWORD)(dwClassId), (DWORD)(wNameString)))

#define D2CommonUnitFindsTestUnitNearRoom(hRoom, dwUnitType, dwClassId) \
	D2CommonOrdinal10965((DWORD)(hRoom), (DWORD)(dwUnitType), (DWORD)(dwClassId))

#define D2CommonUnitFindsFindNearestUnit(hUnit, nX, nY, nMaxDist, pfnUnitTest) \
	D2CommonOrdinal10966((DWORD)(hUnit), (DWORD)(nX), (DWORD)(nY), (DWORD)(nMaxDist), (DWORD)(pfnUnitTest))

#define D2CommonFieldTblsLoad(pBuffer, nBufferSize) \
	((BOOL)D2CommonOrdinal10967((DWORD)(pBuffer), (DWORD)(nBufferSize)))

#define D2CommonFieldTblsFree() \
	((BOOL)D2CommonOrdinal10968())

#define D2CommonFieldTblsGetCenter(pnX, pnY) \
	D2CommonOrdinal10969((DWORD)(pnX), (DWORD)(pnY))

#define D2CommonFieldTblsGetWidth() \
	((int)D2CommonOrdinal10970())

#define D2CommonFieldTblsGetHeight() \
	((int)D2CommonOrdinal10971())

#define D2CommonFieldTblsAllocPair() \
	((DWORD *)D2CommonOrdinal10972())

#define D2CommonFieldTblsFreePair(tField) \
	D2CommonOrdinal10973((DWORD)(tField))

#define D2CommonFieldTblsSetPair(tField, dwA, dwB) \
	D2CommonOrdinal10974((DWORD)(tField), (DWORD)(dwA), (DWORD)(dwB))

#define D2CommonFieldTblsGetAt(tField, nX, nY) \
	((int)D2CommonOrdinal10975((DWORD)(tField), (DWORD)(nX), (DWORD)(nY)))

#define D2CommonFieldTblsStep(tField, pnX, pnY) \
	((BOOL)D2CommonOrdinal10976((DWORD)(tField), (DWORD)(pnX), (DWORD)(pnY)))

#define D2CommonFieldTblsFindCandidate(dwField, dwCount, dwA, dwB, dwPattern) \
	((BOOL)D2CommonOrdinal10977((DWORD)(dwField), (DWORD)(dwCount), (DWORD)(dwA), (DWORD)(dwB), \
		(DWORD)(dwPattern)))

#define D2CommonLoggingWrite \
	D2CommonOrdinal10978

#define D2CommonLoggingWriteGameRecord \
	D2CommonOrdinal10979

#define D2CommonLoggingToggle(fEnable) \
	D2CommonOrdinal10980((DWORD)(fEnable))

#define D2CommonLoggingSetMode(dwMode) \
	D2CommonOrdinal10981((DWORD)(dwMode))

#define D2CommonProfDump() \
	D2CommonOrdinal10983()

#define D2CommonProfDumpAndReset() \
	D2CommonOrdinal10984()

#define D2CommonStepTestGridBit(phGrid, nRow, nCol) \
	((BOOL)D2CommonOrdinal10985((DWORD)(phGrid), (DWORD)(nRow), (DWORD)(nCol)))

#define D2CommonQuestRecordSetBit(ptArray, nWord, nBit) \
	D2CommonOrdinal10986((DWORD)(ptArray), (DWORD)(nWord), (DWORD)(nBit))

#define D2CommonQuestRecordClearBit(ptArray, nWord, nBit) \
	D2CommonOrdinal10987((DWORD)(ptArray), (DWORD)(nWord), (DWORD)(nBit))

#define D2CommonQuestRecordClearMidBits(ptArray, nWord) \
	D2CommonOrdinal10988((DWORD)(ptArray), (DWORD)(nWord))

#define D2CommonQuestRecordNew(pPool) \
	D2CommonOrdinal10989((DWORD)(pPool))

#define D2CommonQuestRecordFree(pPool, ptArray) \
	D2CommonOrdinal10990((DWORD)(pPool), (DWORD)(ptArray))

#define D2CommonQuestRecordCopyAndSweep(ptArray, pSrc, wBufSize, fSweep) \
	D2CommonOrdinal10991((DWORD)(ptArray), (DWORD)(pSrc), (DWORD)(wBufSize), (DWORD)(fSweep))

#define D2CommonQuestRecordCopyOut(ptArray, pDst, wBufSize, dwUnused) \
	D2CommonOrdinal10992((DWORD)(ptArray), (DWORD)(pDst), (DWORD)(wBufSize), (DWORD)(dwUnused))

#define D2CommonMissileCreateMissileData(hMissile) \
	D2CommonOrdinal10993((DWORD)(hMissile))

#define D2CommonMissileFreeMissileData(hMissile) \
	D2CommonOrdinal10994((DWORD)(hMissile))

#define D2CommonMissileSetLevel(hMissile, dwValue) \
	D2CommonOrdinal10995((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetLevel(hMissile) \
	D2CommonOrdinal10996((DWORD)(hMissile))

#define D2CommonMissileSetRange(hMissile, dwValue) \
	D2CommonOrdinal10997((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetRange(hMissile) \
	D2CommonOrdinal10998((DWORD)(hMissile))

#define D2CommonMissileSetRangeLeft(hMissile, dwValue) \
	D2CommonOrdinal10999((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetRangeLeft(hMissile) \
	D2CommonOrdinal11000((DWORD)(hMissile))

#define D2CommonMissileCalcRangeUsed(hMissile) \
	D2CommonOrdinal11001((DWORD)(hMissile))

#define D2CommonMissileSetIndexedByte(hMissile, dwIndex, bValue) \
	D2CommonOrdinal11002((DWORD)(hMissile), (DWORD)(dwIndex), (DWORD)(bValue))

#define D2CommonMissileGetIndexedByte(hMissile, dwIndex) \
	((BYTE)D2CommonOrdinal11003((DWORD)(hMissile), (DWORD)(dwIndex)))

#define D2CommonMissileGetClassId(hMissile) \
	D2CommonOrdinal11004((DWORD)(hMissile))

#define D2CommonMissileSetLastCollideUnit(hMissile, hUnit) \
	D2CommonOrdinal11005((DWORD)(hMissile), (DWORD)(hUnit))

#define D2CommonMissileTestLastCollideUnit(hMissile, hUnit) \
	((BOOL)D2CommonOrdinal11006((DWORD)(hMissile), (DWORD)(hUnit)))

#define D2CommonMissileSetSpawnClass(hMissile, dwValue) \
	D2CommonOrdinal11007((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetSpawnClass(hMissile) \
	D2CommonOrdinal11008((DWORD)(hMissile))

#define D2CommonMissileSetSpawnRadius(hMissile, dwValue) \
	D2CommonOrdinal11009((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetSpawnRadius(hMissile) \
	D2CommonOrdinal11010((DWORD)(hMissile))

#define D2CommonMissileGetHitClass(hMissile) \
	D2CommonOrdinal11011((DWORD)(hMissile))

#define D2CommonMissileSetActivateAt(hMissile, dwValue) \
	D2CommonOrdinal11012((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetActivateAt(hMissile) \
	D2CommonOrdinal11013((DWORD)(hMissile))

#define D2CommonMissileGetAnimRate(hMissile) \
	D2CommonOrdinal11014((DWORD)(hMissile))

#define D2CommonMissileSetAnimRate(hMissile, dwValue) \
	D2CommonOrdinal11015((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileSetScratchFirst(hMissile, dwValue) \
	D2CommonOrdinal11016((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetScratchFirst(hMissile) \
	D2CommonOrdinal11017((DWORD)(hMissile))

#define D2CommonMissileSetScratchSecond(hMissile, dwValue) \
	D2CommonOrdinal11018((DWORD)(hMissile), (DWORD)(dwValue))

#define D2CommonMissileGetScratchSecond(hMissile) \
	D2CommonOrdinal11019((DWORD)(hMissile))

#define D2CommonMissileSetStoredUnit(hMissile, hUnit) \
	D2CommonOrdinal11020((DWORD)(hMissile), (DWORD)(hUnit))

#define D2CommonMissileGetStoredUnit(hMissile, pdwField0C, pdwField10) \
	D2CommonOrdinal11021((DWORD)(hMissile), (DWORD)(pdwField0C), (DWORD)(pdwField10))

#define D2CommonWaypointTestField(pHistory, wField) \
	D2CommonOrdinal11022((DWORD)(pHistory), (DWORD)(wField))

#define D2CommonWaypointSetField(pHistory, wField) \
	D2CommonOrdinal11023((DWORD)(pHistory), (DWORD)(wField))

#define D2CommonWaypointNewHistory(pPool) \
	((WORD *)D2CommonOrdinal11024((DWORD)(pPool)))

#define D2CommonWaypointFreeHistory(pPool, pHistory) \
	D2CommonOrdinal11025((DWORD)(pPool), (DWORD)(pHistory))

#define D2CommonWaypointCopyAndResetHistory(pHistory, pSrc) \
	((WORD *)D2CommonOrdinal11026((DWORD)(pHistory), (DWORD)(pSrc)))

#define D2CommonWaypointExportHistory(pHistory, pOut) \
	D2CommonOrdinal11027((DWORD)(pHistory), (DWORD)(pOut))

#define D2CommonWaypointGetLevelIndex(nLevelId, pwOut) \
	((BOOL)D2CommonOrdinal11028((DWORD)(nLevelId), (DWORD)(pwOut)))

#define D2CommonWaypointFindLevelByIndex(dwIndex, pdwOutLevelId) \
	((BOOL)D2CommonOrdinal11029((DWORD)(dwIndex), (DWORD)(pdwOutLevelId)))

#define D2CommonGuildsNothing(dwUnused1) \
	D2CommonOrdinal11030((DWORD)(dwUnused1))

#define D2CommonGuildsDataNew(pPool) \
	D2CommonOrdinal11031((DWORD)(pPool))

#define D2CommonGuildsDataFree(pData) \
	D2CommonOrdinal11032((DWORD)(pData))

#define D2CommonGuildsReceiveSave(hGuildData, dwUnused, wChunk, wOffset, wTotal, pbChunk) \
	((BOOL)D2CommonOrdinal11033((DWORD)(hGuildData), (DWORD)(dwUnused), (DWORD)(wChunk), (DWORD)(wOffset), \
		(DWORD)(wTotal), (DWORD)(pbChunk)))

#define D2CommonGuildsReceiveRoster(hGuildData, dwUnused, wChunk, wOffset, wTotal, pbChunk) \
	((BOOL)D2CommonOrdinal11034((DWORD)(hGuildData), (DWORD)(dwUnused), (DWORD)(wChunk), (DWORD)(wOffset), \
		(DWORD)(wTotal), (DWORD)(pbChunk)))

#define D2CommonGuildsWriteSave(hGuildData, pbBuffer, dwUnused, fItems) \
	((int)D2CommonOrdinal11035((DWORD)(hGuildData), (DWORD)(pbBuffer), (DWORD)(dwUnused), (DWORD)(fItems)))

#define D2CommonGuildsWriteRoster(hGuildData, pbBuffer, nSize) \
	((int)D2CommonOrdinal11036((DWORD)(hGuildData), (DWORD)(pbBuffer), (DWORD)(nSize)))

#define D2CommonGuildsGetActive(hGuildData) \
	D2CommonOrdinal11037((DWORD)(hGuildData))

#define D2CommonGuildsGetTag(hGuildData) \
	((BYTE *)D2CommonOrdinal11038((DWORD)(hGuildData)))

#define D2CommonGuildsGetName(hGuildData) \
	((BYTE *)D2CommonOrdinal11039((DWORD)(hGuildData)))

#define D2CommonGuildsGetWebLink(hGuildData) \
	((BYTE *)D2CommonOrdinal11040((DWORD)(hGuildData)))

#define D2CommonGuildsSetWebLink(hGuildData, pSrc) \
	D2CommonOrdinal11041((DWORD)(hGuildData), (DWORD)(pSrc))

#define D2CommonGuildsGetCharter(hGuildData) \
	((BYTE *)D2CommonOrdinal11042((DWORD)(hGuildData)))

#define D2CommonGuildsSetCharter(hGuildData, pSrc) \
	D2CommonOrdinal11043((DWORD)(hGuildData), (DWORD)(pSrc))

#define D2CommonGuildsGetMembers(hGuildData) \
	D2CommonOrdinal11044((DWORD)(hGuildData))

#define D2CommonGuildsGetEmblem(hGuildData, pbOut1, pbOut2, pbOut3) \
	D2CommonOrdinal11045((DWORD)(hGuildData), (DWORD)(pbOut1), (DWORD)(pbOut2), (DWORD)(pbOut3))

#define D2CommonGuildsSetEmblem(hGuildData, bVal1, bVal2, bVal3) \
	D2CommonOrdinal11046((DWORD)(hGuildData), (DWORD)(bVal1), (DWORD)(bVal2), (DWORD)(bVal3))

#define D2CommonGuildsGetByteAfterEmblem(hGuildData) \
	D2CommonOrdinal11047((DWORD)(hGuildData))

#define D2CommonGuildsGetFlags(hGuildData) \
	D2CommonOrdinal11048((DWORD)(hGuildData))

#define D2CommonGuildsGetCounterB(hGuildData) \
	D2CommonOrdinal11049((DWORD)(hGuildData))

#define D2CommonGuildsDonateGold(hGuildData, pszName, dwAmount) \
	D2CommonOrdinal11051((DWORD)(hGuildData), (DWORD)(pszName), (DWORD)(dwAmount))

#define D2CommonGuildsGetNextRankThreshold(hGuildData) \
	D2CommonOrdinal11052((DWORD)(hGuildData))

#define D2CommonGuildsGetCounterA(hGuildData) \
	D2CommonOrdinal11053((DWORD)(hGuildData))

#define D2CommonGuildsAddCounterA(hGuildData, dwAmount) \
	D2CommonOrdinal11055((DWORD)(hGuildData), (DWORD)(dwAmount))

#define D2CommonGuildsSubtractCounterA(hGuildData, dwAmount) \
	D2CommonOrdinal11056((DWORD)(hGuildData), (DWORD)(dwAmount))

#define D2CommonGuildsGetVaultInventory(hGuildData) \
	D2CommonOrdinal11057((DWORD)(hGuildData))

#define D2CommonGuildsSetVaultUnitId(hGuildData, dwVal) \
	D2CommonOrdinal11058((DWORD)(hGuildData), (DWORD)(dwVal))

#define D2CommonGuildsGetVaultUnitId(hGuildData) \
	D2CommonOrdinal11059((DWORD)(hGuildData))

#define D2CommonGuildsGetVaultItemData(hGuildData, ppOut, pwOut) \
	D2CommonOrdinal11060((DWORD)(hGuildData), (DWORD)(ppOut), (DWORD)(pwOut))

#define D2CommonGuildsGetTrophyCaseInventory(hGuildData) \
	D2CommonOrdinal11061((DWORD)(hGuildData))

#define D2CommonGuildsSetTrophyCaseUnitId(hGuildData, dwVal) \
	D2CommonOrdinal11062((DWORD)(hGuildData), (DWORD)(dwVal))

#define D2CommonGuildsGetTrophyCaseUnitId(hGuildData) \
	D2CommonOrdinal11063((DWORD)(hGuildData))

#define D2CommonGuildsGetTrophyCaseItemData(hGuildData, ppOut, pwOut) \
	D2CommonOrdinal11064((DWORD)(hGuildData), (DWORD)(ppOut), (DWORD)(pwOut))

#define D2CommonGuildsSetMessageBoardUnitId(hGuildData, dwVal) \
	D2CommonOrdinal11065((DWORD)(hGuildData), (DWORD)(dwVal))

#define D2CommonGuildsGetMessageBoardUnitId(hGuildData) \
	D2CommonOrdinal11066((DWORD)(hGuildData))

#define D2CommonGuildsPostAppend(hGuildData, pszName, pszText) \
	((BOOL)D2CommonOrdinal11067((DWORD)(hGuildData), (DWORD)(pszName), (DWORD)(pszText)))

#define D2CommonGuildsPostRead(hGuildData, nPos, pszNameOut, pszTextOut) \
	D2CommonOrdinal11068((DWORD)(hGuildData), (DWORD)(nPos), (DWORD)(pszNameOut), (DWORD)(pszTextOut))

#define D2CommonGuildsCopyPostBuffer(hGuildData, pDst) \
	D2CommonOrdinal11069((DWORD)(hGuildData), (DWORD)(pDst))

#define D2CommonGuildsPostReceive(hGuildData, wSize, wOffset, wTotalSize, pData) \
	D2CommonOrdinal11070((DWORD)(hGuildData), (DWORD)(wSize), (DWORD)(wOffset), (DWORD)(wTotalSize), \
		(DWORD)(pData))

#define D2CommonGuildsListGetNext(hGuildData, ptCur) \
	D2CommonOrdinal11071((DWORD)(hGuildData), (DWORD)(ptCur))

#define D2CommonGuildsListFindByName(hGuildData, pszName) \
	D2CommonOrdinal11072((DWORD)(hGuildData), (DWORD)(pszName))

#define D2CommonGuildsMemberSnapshot(hMember, pDst) \
	D2CommonOrdinal11073((DWORD)(hMember), (DWORD)(pDst))

#define D2CommonGuildsListNodeSetFromRecord(hMember, pRecord) \
	D2CommonOrdinal11074((DWORD)(hMember), (DWORD)(pRecord))

#define D2CommonGuildsGetMemberClass(hMember) \
	D2CommonOrdinal11075((DWORD)(hMember))

#define D2CommonGuildsGetMemberRank(hMember) \
	D2CommonOrdinal11076((DWORD)(hMember))

#define D2CommonGuildsSetMemberRank(hMember, nRank) \
	D2CommonOrdinal11077((DWORD)(hMember), (DWORD)(nRank))

#define D2CommonGuildsListRemoveByName(hGuildData, pszName) \
	D2CommonOrdinal11078((DWORD)(hGuildData), (DWORD)(pszName))

#define D2CommonGuildsUnpackField(dwPacked) \
	D2CommonOrdinal11079((DWORD)(dwPacked))

#define D2CommonGuildsGetRank(hGuildData) \
	D2CommonOrdinal11080((DWORD)(hGuildData))

#define D2CommonGuildsGetRankLevelId(hGuildData) \
	D2CommonOrdinal11081((DWORD)(hGuildData))

#define D2CommonGuildsSetChanged(hGuildData, fSet) \
	D2CommonOrdinal11082((DWORD)(hGuildData), (DWORD)(fSet))

#define D2CommonGuildsGetChanged(hGuildData) \
	D2CommonOrdinal11083((DWORD)(hGuildData))

#define D2CommonGuildsSetAllChanged(hGuildData, fSet) \
	D2CommonOrdinal11084((DWORD)(hGuildData), (DWORD)(fSet))

#define D2CommonGuildsGetEmblemChanged(hGuildData) \
	D2CommonOrdinal11085((DWORD)(hGuildData))

#define D2CommonItemSavePackList(hInventory, pbBuffer, nSize, dwUnused) \
	((int)D2CommonOrdinal11086((DWORD)(hInventory), (DWORD)(pbBuffer), (DWORD)(nSize), (DWORD)(dwUnused)))

#define D2CommonItemSaveUnpack(ptItemSave, pbBuffer) \
	((BOOL)D2CommonOrdinal11087((DWORD)(ptItemSave), (DWORD)(pbBuffer)))

#define D2CommonMissileNothingFirst(dwUnused1) \
	D2CommonOrdinal11088((DWORD)(dwUnused1))

#define D2CommonMissileNothingSecond(dwUnused1) \
	D2CommonOrdinal11089((DWORD)(dwUnused1))

#define D2CommonUnitFindsNothingFirst(dwUnused1, dwUnused2, dwUnused3, dwUnused4) \
	D2CommonOrdinal11090((DWORD)(dwUnused1), (DWORD)(dwUnused2), (DWORD)(dwUnused3), (DWORD)(dwUnused4))

#define D2CommonUnitFindsNothingSecond(dwUnused1, dwUnused2, dwUnused3, dwUnused4) \
	D2CommonOrdinal11091((DWORD)(dwUnused1), (DWORD)(dwUnused2), (DWORD)(dwUnused3), (DWORD)(dwUnused4))

#define D2CommonPresetTagTableCountRow(bRow) \
	((int)D2CommonOrdinal11092((DWORD)(bRow)))

#define D2CommonPresetTagTableGetCell(bRow, nCol) \
	((int)D2CommonOrdinal11093((DWORD)(bRow), (DWORD)(nCol)))

#define D2CommonPresetMonsterRemapCountRow(bRow) \
	((int)D2CommonOrdinal11094((DWORD)(bRow)))

#define D2CommonPresetMonsterRemapGetCell(bRow, nCol) \
	((int)D2CommonOrdinal11095((DWORD)(bRow), (DWORD)(nCol)))

#define D2CommonLvlTblsTestLevelTypeAct(nType, bAct) \
	((BOOL)D2CommonOrdinal11096((DWORD)(nType), (DWORD)(bAct)))

#define D2CommonLvlTblsGetLevelTypeFile(nType, nFile, pszFile) \
	D2CommonOrdinal11097((DWORD)(nType), (DWORD)(nFile), (DWORD)(pszFile))

#define D2CommonDataTblsGetDifficultyLevel(nDifficultyLevel) \
	D2CommonOrdinal11098((DWORD)(nDifficultyLevel))

#define D2CommonPathGetCollisionTestMask(ptPath) \
	D2CommonOrdinal11099((DWORD)(ptPath))

#define D2CommonITEMSTestUbercodeUnsetNoCheck(dwItemClassIndex) \
	((BOOL)D2CommonOrdinal11100((DWORD)(dwItemClassIndex)))

#define D2CommonITEMSTestUbercodeUnset(hItem) \
	((BOOL)D2CommonOrdinal11101((DWORD)(hItem)))

#define D2CommonINVENTORYTestHasDeltaKey(ptInventory, dwItemId) \
	((BOOL)D2CommonOrdinal11102((DWORD)(ptInventory), (DWORD)(dwItemId)))

#define D2CommonPathRestoreCollisionMask(hUnit, fSkip) \
	D2CommonOrdinal11103((DWORD)(hUnit), (DWORD)(fSkip))

#define D2CommonItemSaveGet(ptItemSave, hItem) \
	D2CommonOrdinal11104((DWORD)(ptItemSave), (DWORD)(hItem))

#define D2CommonINVENTORYSoftReset(ptInventory) \
	D2CommonOrdinal11105((DWORD)(ptInventory))

#define D2CommonDrlgGetLevelTypeRaw D2CommonOrdinal10000
#define D2CommonDrlgGetActNumRaw(ecx, a1) \
	(D2CommonOrdinal10001(a1))
#define D2CommonDrlgGetLevelLOSDrawRaw D2CommonOrdinal10002
#define D2CommonDrlgActUpdateRoomsRaw D2CommonOrdinal10003
#define D2CommonDrlgGetLevelSaveMonstersRaw D2CommonOrdinal10004
#define D2CommonDrlgGetLevelRaw D2CommonOrdinal10005
#define D2CommonDrlgGenerateLevelRaw D2CommonOrdinal10006
#define D2CommonDungeonRoomGetDrlgRoomRaw D2CommonOrdinal10007
#define D2CommonPresetRoomGetInfoFirstDwordRaw D2CommonOrdinal10008
#define D2CommonPresetRoomGetFileRaw D2CommonOrdinal10009
#define D2CommonLvlTblsGetLevelDefRaw D2CommonOrdinal10010
#define D2CommonLvlTblsGetAutomapCelRaw D2CommonOrdinal10011
#define D2CommonDrlgFreeActRaw D2CommonOrdinal10012
#define D2CommonDrlgNewLevelRaw D2CommonOrdinal10013
#define D2CommonDrlgNewActRaw D2CommonOrdinal10014
#define D2CommonDrlgGetBuiltCountsRaw D2CommonOrdinal10015
#define D2CommonCollisnGetZeroRaw D2CommonOrdinal10016
#define D2CommonRoomTileGetThirdArrayCountRaw D2CommonOrdinal10017
#define D2CommonRandomNothingZeroRaw D2CommonOrdinal10018
#define D2CommonSkillsNothingRaw D2CommonOrdinal10019
#define D2CommonPresetFileListCloneRaw D2CommonOrdinal10020
#define D2CommonPresetInfoDetachFirstDwordRaw D2CommonOrdinal10021
#define D2CommonPresetFileListFreeRaw D2CommonOrdinal10022
#define D2CommonLvlTblsGetLevelTypeDefRaw D2CommonOrdinal10023
#define D2CommonLvlTblsGetPrestRaw D2CommonOrdinal10024
#define D2CommonDungeonRoomGetLevelWarpXRaw D2CommonOrdinal10025
#define D2CommonDungeonRoomSetWarpsDefAllowedRaw D2CommonOrdinal10026
#define D2CommonDungeonRoomGetWarpUnitRaw D2CommonOrdinal10027
#define D2CommonDungeonRoomGetUnitWarpDefRaw D2CommonOrdinal10028
#define D2CommonDungeonRoomGetUnitListRaw D2CommonOrdinal10029
#define D2CommonDungeonRoomGetSecondTileArrayRaw D2CommonOrdinal10030
#define D2CommonDungeonRoomGetFirstTileArrayRaw D2CommonOrdinal10031
#define D2CommonDungeonRoomGetThirdTileArrayRaw D2CommonOrdinal10032
#define D2CommonDungeonRoomCopyRectsRaw D2CommonOrdinal10033
#define D2CommonDungeonRoomGetNearRaw D2CommonOrdinal10034
#define D2CommonDungeonRoomGetPlayerUnitsRaw D2CommonOrdinal10035
#define D2CommonDungeonActGetInitialLevelIdRaw D2CommonOrdinal10036
#define D2CommonDungeonNewActRaw D2CommonOrdinal10037
#define D2CommonDungeonFreeActRaw D2CommonOrdinal10038
#define D2CommonDungeonRoomsOverlapRaw D2CommonOrdinal10039
#define D2CommonDungeonActFindRoomByPointRaw D2CommonOrdinal10040
#define D2CommonDungeonFindRoomGameRaw D2CommonOrdinal10041
#define D2CommonDungeonRoomTestPointInSecondRectRaw D2CommonOrdinal10042
#define D2CommonDungeonRoomGetKindFlagRaw D2CommonOrdinal10043
#define D2CommonDungeonGetActStartRoomRaw D2CommonOrdinal10044
#define D2CommonDungeonGetWarpRoomRaw D2CommonOrdinal10045
#define D2CommonDungeonActGetDrawnLevelRaw D2CommonOrdinal10046
#define D2CommonDungeonRoomGetSecondRectShapeRaw D2CommonOrdinal10047
#define D2CommonDungeonRoomForEachNearRaw D2CommonOrdinal10048
#define D2CommonDungeonRoomFindNearByPointRaw D2CommonOrdinal10049
#define D2CommonDungeonTestRoomGameRaw D2CommonOrdinal10050
#define D2CommonDungeonRoomGetScaledBoundsRaw D2CommonOrdinal10051
#define D2CommonDungeonRoomGetRectLTRBRaw D2CommonOrdinal10052
#define D2CommonDungeonRoomGetLightRaw D2CommonOrdinal10053
#define D2CommonDungeonRoomGetPresetUnitsRaw D2CommonOrdinal10054
#define D2CommonDungeonActGetFirstRoomRaw D2CommonOrdinal10055
#define D2CommonDungeonRoomGetLevelIdRaw D2CommonOrdinal10056
#define D2CommonDungeonRoomGetVisLevelRaw D2CommonOrdinal10057
#define D2CommonDungeonRoomGetLevelIdCheckedRaw D2CommonOrdinal10058
#define D2CommonDungeonRoomHasExitFlagsRaw D2CommonOrdinal10059
#define D2CommonDungeonRoomGetFileNameRaw D2CommonOrdinal10060
#define D2CommonDungeonRoomSwitchRaw D2CommonOrdinal10061
#define D2CommonDungeonActFindRoomARaw D2CommonOrdinal10062
#define D2CommonDungeonActFindRoomBRaw D2CommonOrdinal10063
#define D2CommonDungeonActGetLevelRoomRaw D2CommonOrdinal10064
#define D2CommonDungeonRoomAddNodeRaw D2CommonOrdinal10065
#define D2CommonDungeonRoomFreeNodesRaw D2CommonOrdinal10066
#define D2CommonDungeonRoomGetNodesRaw D2CommonOrdinal10067
#define D2CommonDungeonActGetFirstActiveRoomRaw D2CommonOrdinal10068
#define D2CommonDungeonActGetNextActiveRoomRaw D2CommonOrdinal10069
#define D2CommonDungeonRoomCanReleaseRaw D2CommonOrdinal10070
#define D2CommonDungeonRoomIsIdleRaw D2CommonOrdinal10071
#define D2CommonDungeonRoomNearAllFlaggedRaw D2CommonOrdinal10072
#define D2CommonDungeonRoomTestPopulatedRaw D2CommonOrdinal10073
#define D2CommonDungeonRoomSetPopulatedRaw D2CommonOrdinal10074
#define D2CommonDungeonActRemoveRoomRaw D2CommonOrdinal10075
#define D2CommonDungeonRoomChangeLevelRaw D2CommonOrdinal10076
#define D2CommonDungeonActFreeIdleLevelsRaw D2CommonOrdinal10077
#define D2CommonDungeonRoomAddClientRaw D2CommonOrdinal10078
#define D2CommonDungeonRoomRemoveClientRaw D2CommonOrdinal10079
#define D2CommonDungeonRoomTickEmptyCounterRaw D2CommonOrdinal10080
#define D2CommonDungeonRoomIsTownRaw D2CommonOrdinal10081
#define D2CommonDungeonRoomIsNoneRaw D2CommonOrdinal10082
#define D2CommonDungeonRoomTestSkipsChangedListRaw D2CommonOrdinal10083
#define D2CommonDungeonActGetTownLevelIdRaw D2CommonOrdinal10084
#define D2CommonDungeonLevelIsTownRaw D2CommonOrdinal10085
#define D2CommonDungeonRoomGetOutdoorFlagRaw D2CommonOrdinal10086
#define D2CommonDungeonActGetDrlgActRaw D2CommonOrdinal10087
#define D2CommonDungeonActGetMapSeedRaw D2CommonOrdinal10088
#define D2CommonDungeonActCountLevelRoomsRaw D2CommonOrdinal10089
#define D2CommonDungeonRoomShowWarpTilesRaw D2CommonOrdinal10090
#define D2CommonDungeonRoomHideWarpTilesRaw D2CommonOrdinal10091
#define D2CommonDungeonRoomUpdateDoorsRaw D2CommonOrdinal10092
#define D2CommonDungeonRoomGetPointsRaw D2CommonOrdinal10093
#define D2CommonDungeonGetLogicValueRaw D2CommonOrdinal10094
#define D2CommonDungeonRoomGetLogicNodeRaw D2CommonOrdinal10095
#define D2CommonDungeonRoomGetLogicListRaw D2CommonOrdinal10096
#define D2CommonDungeonGetPortalLevelsRaw D2CommonOrdinal10097
#define D2CommonDungeonGetPortalLevelBitRaw(ecx, a1) \
	(D2CommonOrdinal10098(a1))
#define D2CommonDungeonRoomIncPlayerUnitsRaw D2CommonOrdinal10099
#define D2CommonDungeonRoomDecPlayerUnitsRaw D2CommonOrdinal10100
#define D2CommonDungeonRoomSetHoldRaw D2CommonOrdinal10101
#define D2CommonDungeonActSetRoomMadeCallbackRaw D2CommonOrdinal10102
#define D2CommonDungeonRoomAnimateNearRaw D2CommonOrdinal10103
#define D2CommonDungeonActSyncAnimFramesRaw D2CommonOrdinal10104
#define D2CommonDungeonRoomRingBufferPushRaw D2CommonOrdinal10105
#define D2CommonDungeonCalcScreenToTileRaw D2CommonOrdinal10106
#define D2CommonEnvScreenToTilePairRawRaw D2CommonOrdinal10108
#define D2CommonDungeonCalcTileToScreenRaw D2CommonOrdinal10109
#define D2CommonDungeonCalcSubtileToScreenRaw D2CommonOrdinal10110
#define D2CommonDungeonCalcScaledSubtileToScreenRaw D2CommonOrdinal10111
#define D2CommonEnvTileToSubtileRaw D2CommonOrdinal10112
#define D2CommonDungeonCalcScreenToTileFloorRaw D2CommonOrdinal10113
#define D2CommonDungeonCalcTileToScreenOffsetRaw D2CommonOrdinal10114
#define D2CommonEnvComputeTileBoundsPairRaw D2CommonOrdinal10115
#define D2CommonEnvComputeScreenOriginPairRaw D2CommonOrdinal10116
#define D2CommonCollisnTestPointRaw D2CommonOrdinal10117
#define D2CommonCollisnTestSizeRaw D2CommonOrdinal10118
#define D2CommonCollisnTestAreaRaw D2CommonOrdinal10119
#define D2CommonCollisnTestPatternRaw D2CommonOrdinal10120
#define D2CommonCollisnTestPatternAnyRaw D2CommonOrdinal10121
#define D2CommonCollisnClearPointRaw D2CommonOrdinal10122
#define D2CommonCollisnClearSizeRaw D2CommonOrdinal10123
#define D2CommonCollisnClearAreaRaw D2CommonOrdinal10124
#define D2CommonCollisnClearPatternRaw D2CommonOrdinal10125
#define D2CommonCollisnSetPointRaw D2CommonOrdinal10126
#define D2CommonCollisnSetSizeRaw D2CommonOrdinal10127
#define D2CommonCollisnSetAreaRaw D2CommonOrdinal10128
#define D2CommonCollisnSetPatternRaw D2CommonOrdinal10129
#define D2CommonCollisnMoveSizeRaw D2CommonOrdinal10130
#define D2CommonCollisnMovePatternRaw D2CommonOrdinal10131
#define D2CommonCollisnDualUpdateRaw D2CommonOrdinal10132
#define D2CommonCollisnFindSpawnPointNearRaw D2CommonOrdinal10133
#define D2CommonCollisnFindSpawnPointRadiusRaw D2CommonOrdinal10134
#define D2CommonCollisnFindFreeAreaRaw D2CommonOrdinal10135
#define D2CommonCollisnFindSpawnPointStepRaw D2CommonOrdinal10136
#define D2CommonCollisnFindSpawnPointFieldRaw D2CommonOrdinal10137
#define D2CommonPathNothingThirdRaw D2CommonOrdinal10138
#define D2CommonPathNothingFourthRaw D2CommonOrdinal10139
#define D2CommonPathGetWaypointSquareRaw D2CommonOrdinal10140
#define D2CommonPathComputeRaw D2CommonOrdinal10141
#define D2CommonPathReplaceCollisionMaskRaw D2CommonOrdinal10142
#define D2CommonPathSetCollisionTrackingRaw D2CommonOrdinal10143
#define D2CommonPathSetVelocityRaw D2CommonOrdinal10144
#define D2CommonPathGetVelocityRaw D2CommonOrdinal10145
#define D2CommonPathSetMaxVelocityRaw D2CommonOrdinal10146
#define D2CommonPathGetMaxVelocityRaw D2CommonOrdinal10147
#define D2CommonPathSetAccelRaw D2CommonOrdinal10148
#define D2CommonPathGetAccelRaw D2CommonOrdinal10149
#define D2CommonPathCreateRaw D2CommonOrdinal10150
#define D2CommonPathClearStepsRaw D2CommonOrdinal10151
#define D2CommonPathGetStepCountRaw D2CommonOrdinal10152
#define D2CommonPathGetStepIndexRaw D2CommonOrdinal10153
#define D2CommonPathFreePoolRaw D2CommonOrdinal10154
#define D2CommonPathGetStepsRaw D2CommonOrdinal10155
#define D2CommonPathGetFacingRaw D2CommonOrdinal10156
#define D2CommonPathGetTargetFacingRaw D2CommonOrdinal10157
#define D2CommonPathSetFacingSmoothedRaw D2CommonOrdinal10158
#define D2CommonPathSetFacingImmediateRaw D2CommonOrdinal10159
#define D2CommonPathGetGameXRaw D2CommonOrdinal10160
#define D2CommonPathGetGameYRaw D2CommonOrdinal10161
#define D2CommonPathGetScreenXRaw D2CommonOrdinal10162
#define D2CommonPathGetScreenYRaw D2CommonOrdinal10163
#define D2CommonPathGetRoomRaw D2CommonOrdinal10164
#define D2CommonPathSetRoomRaw D2CommonOrdinal10165
#define D2CommonPathGetPrevRoomRaw D2CommonOrdinal10166
#define D2CommonPathClearPrevRoomRaw D2CommonOrdinal10167
#define D2CommonPathSetTargetCoordRaw D2CommonOrdinal10168
#define D2CommonPathGetTargetSnapshotRaw D2CommonOrdinal10169
#define D2CommonPathTestLeftRoomRaw D2CommonOrdinal10170
#define D2CommonPathSetLeftRoomRaw D2CommonOrdinal10171
#define D2CommonPathTestCollisionTrackingRaw D2CommonOrdinal10172
#define D2CommonPathGetTargetXRaw D2CommonOrdinal10173
#define D2CommonPathGetTargetYRaw D2CommonOrdinal10174
#define D2CommonPathGetStepXRaw D2CommonOrdinal10175
#define D2CommonPathGetStepYRaw D2CommonOrdinal10176
#define D2CommonPathSetTargetRaw D2CommonOrdinal10177
#define D2CommonPathGetTargetRaw D2CommonOrdinal10178
#define D2CommonPathGetCollisionMaskRaw D2CommonOrdinal10179
#define D2CommonPathSetCollisionMaskRaw D2CommonOrdinal10180
#define D2CommonPathSetCollisionTestMaskRaw D2CommonOrdinal10181
#define D2CommonPathSetTypeRaw D2CommonOrdinal10182
#define D2CommonPathRestoreTypeRaw D2CommonOrdinal10183
#define D2CommonPathGetTypeRaw D2CommonOrdinal10184
#define D2CommonPathSetDistMaxRaw D2CommonOrdinal10185
#define D2CommonPathGetDistMaxRaw D2CommonOrdinal10186
#define D2CommonPathSetSquareCountdownRaw D2CommonOrdinal10187
#define D2CommonPathGetSquareCountdownRaw D2CommonOrdinal10188
#define D2CommonPathSetSearchReachRaw D2CommonOrdinal10189
#define D2CommonPathStepFacingRaw D2CommonOrdinal10190
#define D2CommonPathGetRawXRaw D2CommonOrdinal10191
#define D2CommonPathGetRawYRaw D2CommonOrdinal10192
#define D2CommonPathSetRawXRaw D2CommonOrdinal10193
#define D2CommonPathSetRawYRaw D2CommonOrdinal10194
#define D2CommonPathGetSavedStepsRaw D2CommonOrdinal10195
#define D2CommonPathGetSavedStepXRaw D2CommonOrdinal10196
#define D2CommonPathGetSavedStepYRaw D2CommonOrdinal10197
#define D2CommonPathGetLineOfSightRaw D2CommonOrdinal10198
#define D2CommonPathGetCachedCollisionRaw D2CommonOrdinal10199
#define D2CommonPathSetFacingFreeRaw D2CommonOrdinal10200
#define D2CommonPathClearKeptTargetRaw D2CommonOrdinal10201
#define D2CommonPathSetStopDistanceRaw D2CommonOrdinal10202
#define D2CommonPathGetStopDistanceRaw D2CommonOrdinal10203
#define D2CommonPathSetTargetOffsetRaw D2CommonOrdinal10204
#define D2CommonPathSetMovingRaw D2CommonOrdinal10205
#define D2CommonPathTestMovingRaw D2CommonOrdinal10206
#define D2CommonPathGetCollisionPatternByUnitRaw D2CommonOrdinal10207
#define D2CommonPathSetCollisionPatternByUnitRaw D2CommonOrdinal10208
#define D2CommonPathUnitSetPeriodicRecalcRaw D2CommonOrdinal10209
#define D2CommonPathUnitPeriodicRecalcRaw D2CommonOrdinal10210
#define D2CommonPathResetCollisionPatternRaw D2CommonOrdinal10211
#define D2CommonStepGetSquareDirectionRaw D2CommonOrdinal10212
#define D2CommonPathTurnToSquareRaw D2CommonOrdinal10213
#define D2CommonPathSetRecalcBudgetRaw D2CommonOrdinal10214
#define D2CommonPathGetRecalcBudgetRaw D2CommonOrdinal10215
#define D2CommonPathAddRecalcBudgetRaw D2CommonOrdinal10216
#define D2CommonPathDistanceSquaredRaw D2CommonOrdinal10217
#define D2CommonPathFreeIfSetRaw D2CommonOrdinal10218
#define D2CommonPathSetCollisionByTypeRaw D2CommonOrdinal10219
#define D2CommonPathClearCollisionByTypeRaw D2CommonOrdinal10220
#define D2CommonPathGetEndXRaw D2CommonOrdinal10221
#define D2CommonPathGetEndYRaw D2CommonOrdinal10222
#define D2CommonStepUnitWalkRaw D2CommonOrdinal10223
#define D2CommonStepUnitTestAtTargetRaw D2CommonOrdinal10224
#define D2CommonPathClearIfExistsRaw D2CommonOrdinal10225
#define D2CommonStepTeleportUnitRaw D2CommonOrdinal10226
#define D2CommonStepTeleportUnitForceRecalcRaw D2CommonOrdinal10227
#define D2CommonStepPlaceUnitRaw D2CommonOrdinal10228
#define D2CommonStepMoveUnitRaw D2CommonOrdinal10229
#define D2CommonStepSnapToSquareRaw D2CommonOrdinal10230
#define D2CommonStepTestSquareCrossedRaw D2CommonOrdinal10231
#define D2CommonStepRideHorseRaw D2CommonOrdinal10232
#define D2CommonStepUnitRecalcPathRaw D2CommonOrdinal10233
#define D2CommonPathTestHasMoreStepsRaw D2CommonOrdinal10234
#define D2CommonPathNothingFirstRaw D2CommonOrdinal10235
#define D2CommonPathNothingZeroRaw D2CommonOrdinal10236
#define D2CommonINVENTORYCreateRaw D2CommonOrdinal10237
#define D2CommonINVENTORYDestroyRaw D2CommonOrdinal10238
#define D2CommonINVENTORYAddItemRaw D2CommonOrdinal10239
#define D2CommonINVENTORYRemoveItemRaw D2CommonOrdinal10240
#define D2CommonINVENTORYTestHasItemRaw D2CommonOrdinal10241
#define D2CommonINVENTORYFindFreeSpaceRaw D2CommonOrdinal10242
#define D2CommonINVENTORYPlaceItemRaw D2CommonOrdinal10243
#define D2CommonINVENTORYTestPlaceAtRaw D2CommonOrdinal10244
#define D2CommonINVENTORYTestSwapAtRaw D2CommonOrdinal10245
#define D2CommonINVENTORYPlaceItemAtRaw D2CommonOrdinal10246
#define D2CommonINVENTORYRemoveItemAtRaw D2CommonOrdinal10247
#define D2CommonINVENTORYClearGridRaw D2CommonOrdinal10248
#define D2CommonINVENTORYGetItemAtRaw D2CommonOrdinal10249
#define D2CommonINVENTORYSetLocSlotRaw D2CommonOrdinal10250
#define D2CommonINVENTORYClearLocSlotRaw D2CommonOrdinal10251
#define D2CommonINVENTORYGetEquipFieldRaw D2CommonOrdinal10252
#define D2CommonINVENTORYTestEquipCompatRaw D2CommonOrdinal10253
#define D2CommonINVENTORYGetLocSlotRaw D2CommonOrdinal10254
#define D2CommonINVENTORYFindEquippedMatchRaw D2CommonOrdinal10255
#define D2CommonINVENTORYGetBodyLocItemRaw D2CommonOrdinal10256
#define D2CommonINVENTORYFindSlotIndexRaw D2CommonOrdinal10257
#define D2CommonINVENTORYSetCursorItemRaw D2CommonOrdinal10258
#define D2CommonINVENTORYGetCursorItemRaw D2CommonOrdinal10259
#define D2CommonINVENTORYFindGridStackRaw D2CommonOrdinal10260
#define D2CommonINVENTORYFindEquippedStackRaw D2CommonOrdinal10261
#define D2CommonINVENTORYFindGridBookRaw D2CommonOrdinal10262
#define D2CommonINVENTORYPlaceBeltItemRaw D2CommonOrdinal10263
#define D2CommonINVENTORYClearBeltSlotRaw D2CommonOrdinal10264
#define D2CommonINVENTORYTestItemFitsRaw D2CommonOrdinal10265
#define D2CommonINVENTORYFindBeltSlotRaw D2CommonOrdinal10266
#define D2CommonINVENTORYAddBeltItemRaw D2CommonOrdinal10267
#define D2CommonINVENTORYGetBeltSlotRaw D2CommonOrdinal10268
#define D2CommonINVENTORYGetBeltSlotForRaw D2CommonOrdinal10269
#define D2CommonINVENTORYFindShieldInHandsRaw D2CommonOrdinal10270
#define D2CommonINVENTORYGetWeaponRaw D2CommonOrdinal10271
#define D2CommonINVENTORYTestTorsoArmorWornRaw D2CommonOrdinal10272
#define D2CommonINVENTORYTestSlotFitsRaw D2CommonOrdinal10273
#define D2CommonINVENTORYGetCategoryListHeadRaw D2CommonOrdinal10274
#define D2CommonINVENTORYGetCategoryListHeadAltRaw D2CommonOrdinal10275
#define D2CommonINVENTORYClearCategoryListRaw D2CommonOrdinal10276
#define D2CommonINVENTORYGetDeltaListHeadRaw D2CommonOrdinal10277
#define D2CommonINVENTORYClearDeltaListRaw D2CommonOrdinal10278
#define D2CommonINVENTORYAddDeltaNodeRaw D2CommonOrdinal10279
#define D2CommonINVENTORYGetCategoryListCountRaw D2CommonOrdinal10280
#define D2CommonINVENTORYFindGridItemByTypeRaw D2CommonOrdinal10281
#define D2CommonINVENTORYFindEquippedByTypeRaw D2CommonOrdinal10282
#define D2CommonINVENTORYFindEquippedByCodeRaw D2CommonOrdinal10283
#define D2CommonINVENTORYCountEquippedSetItemsRaw D2CommonOrdinal10284
#define D2CommonINVENTORYOfferAddDeltaRaw D2CommonOrdinal10285
#define D2CommonINVENTORYOfferRemoveDeltaRaw D2CommonOrdinal10286
#define D2CommonINVENTORYTestCanEquipSlotRaw D2CommonOrdinal10287
#define D2CommonINVENTORYSetOwnerIdRaw D2CommonOrdinal10288
#define D2CommonINVENTORYGetOwnerIdRaw D2CommonOrdinal10289
#define D2CommonINVENTORYAppendTrackedNodeRaw D2CommonOrdinal10290
#define D2CommonINVENTORYRemoveTrackedNodeRaw D2CommonOrdinal10291
#define D2CommonINVENTORYGetTrackedListHeadRaw D2CommonOrdinal10292
#define D2CommonINVENTORYGetTrackedListCountRaw D2CommonOrdinal10293
#define D2CommonINVENTORYWriteSaveHeaderItemsRaw D2CommonOrdinal10294
#define D2CommonINVENTORYTestBodyLocPlaceRaw D2CommonOrdinal10295
#define D2CommonINVENTORYCountFreeCellsRaw D2CommonOrdinal10296
#define D2CommonINVENTORYTestTradeSpaceRaw D2CommonOrdinal10297
#define D2CommonINVENTORYNothingRaw D2CommonOrdinal10298
#define D2CommonINVENTORYNodeGetNextRaw D2CommonOrdinal10299
#define D2CommonINVENTORYNodeGetKeyARaw D2CommonOrdinal10300
#define D2CommonINVENTORYNodeGetItemIdRaw D2CommonOrdinal10301
#define D2CommonINVENTORYNodeGetCategoryRaw D2CommonOrdinal10302
#define D2CommonINVENTORYNodeSetAcceptedIfWeaponRaw D2CommonOrdinal10303
#define D2CommonINVENTORYNodeGetAcceptedRaw D2CommonOrdinal10304
#define D2CommonINVENTORYListFindNodeRaw D2CommonOrdinal10305
#define D2CommonINVENTORYNodeGetKeyBRaw D2CommonOrdinal10306
#define D2CommonINVENTORYNodeGetDeltaItemIdRaw D2CommonOrdinal10307
#define D2CommonINVENTORYNodeGetTrackedNextRaw D2CommonOrdinal10308
#define D2CommonINVENTORYNodeGetTrackedKeyRaw D2CommonOrdinal10309
#define D2CommonINVENTORYNodeGetTrackedDataRaw D2CommonOrdinal10310
#define D2CommonINVENTORYNodeGetTrackedFlagRaw D2CommonOrdinal10311
#define D2CommonUnitTestMonsterCombinedFlagRaw D2CommonOrdinal10312
#define D2CommonUnitTestModeGroupRaw D2CommonOrdinal10313
#define D2CommonUnitGetThreeIfInModeGroupRaw D2CommonOrdinal10314
#define D2CommonUnitGetFirstSkillRaw D2CommonOrdinal10315
#define D2CommonUnitGetLeftSkillRaw D2CommonOrdinal10316
#define D2CommonUnitGetRightSkillRaw D2CommonOrdinal10317
#define D2CommonUnitGetSkillThirdCachedRaw D2CommonOrdinal10318
#define D2CommonUnitSetSkillThirdCachedRaw D2CommonOrdinal10319
#define D2CommonUnitSetAnimRecordRaw D2CommonOrdinal10320
#define D2CommonUnitGetXRaw D2CommonOrdinal10321
#define D2CommonUnitGetXFixedRaw D2CommonOrdinal10322
#define D2CommonUnitSetXRaw D2CommonOrdinal10323
#define D2CommonUnitGetYRaw D2CommonOrdinal10324
#define D2CommonUnitGetYFixedRaw D2CommonOrdinal10325
#define D2CommonUnitSetYRaw D2CommonOrdinal10326
#define D2CommonUnitGetPositionRaw D2CommonOrdinal10327
#define D2CommonUnitGetScreenXRaw D2CommonOrdinal10328
#define D2CommonUnitGetScreenYRaw D2CommonOrdinal10329
#define D2CommonUnitGetScreenPositionRaw D2CommonOrdinal10330
#define D2CommonUnitGetSizeXRaw D2CommonOrdinal10331
#define D2CommonUnitGetSizeYRaw D2CommonOrdinal10332
#define D2CommonUnitGetTypeFlagsRaw D2CommonOrdinal10333
#define D2CommonUnitCalcGoldBankLimitRaw D2CommonOrdinal10334
#define D2CommonUnitSetScreenXRaw D2CommonOrdinal10335
#define D2CommonUnitSetScreenYRaw D2CommonOrdinal10336
#define D2CommonUnitGetPathRoomRaw D2CommonOrdinal10337
#define D2CommonUnitClearPathRoomRaw D2CommonOrdinal10338
#define D2CommonUnitSetTargetRaw D2CommonOrdinal10339
#define D2CommonUnitGetTargetTypeRaw(ecx, a1) \
	(D2CommonOrdinal10340(a1))
#define D2CommonUnitGetTargetIdRaw(ecx, a1) \
	(D2CommonOrdinal10341(a1))
#define D2CommonUnitSetTargetStrictRaw D2CommonOrdinal10342
#define D2CommonUnitSetModeRaw D2CommonOrdinal10343
#define D2CommonUnitInitModeAnimRaw D2CommonOrdinal10344
#define D2CommonUnitInitPositionRaw(ecx, a1, a2, a3, a4) \
	(D2CommonOrdinal10345(a1, a2, a3, a4))
#define D2CommonUnitSetCollisionRaw D2CommonOrdinal10346
#define D2CommonUnitClearCollisionRaw D2CommonOrdinal10347
#define D2CommonUnitGetObjectNameOffsetRaw D2CommonOrdinal10348
#define D2CommonUnitGetLookupBytesRaw D2CommonOrdinal10349
#define D2CommonUnitGetRoomChangedRaw D2CommonOrdinal10350
#define D2CommonUnitSetRoomChangedRaw D2CommonOrdinal10351
#define D2CommonUnitSetChangedRaw D2CommonOrdinal10352
#define D2CommonUnitFindSkillByTypeRaw D2CommonOrdinal10353
#define D2CommonUnitTestMonsterSoftRaw D2CommonOrdinal10354
#define D2CommonUnitGetWeaponNumberRaw D2CommonOrdinal10355
#define D2CommonUnitTestPositionClearRaw D2CommonOrdinal10356
#define D2CommonUnitTestLineBlockedRaw D2CommonOrdinal10357
#define D2CommonUnitTestLineBlockedIgnoringRaw D2CommonOrdinal10358
#define D2CommonUnitTestLineBlockedToPointRaw D2CommonOrdinal10359
#define D2CommonUnitGetStateByteRaw D2CommonOrdinal10360
#define D2CommonUnitGetItemFlagsWordRaw D2CommonOrdinal10361
#define D2CommonUnitGetItemCodeValueRaw D2CommonOrdinal10362
#define D2CommonUnitCalcLifePercentRaw D2CommonOrdinal10363
#define D2CommonUnitGetAnimModeRaw D2CommonOrdinal10364
#define D2CommonUnitSetAnimModeRaw D2CommonOrdinal10365
#define D2CommonUnitInitAnimStateRaw D2CommonOrdinal10366
#define D2CommonUnitSetAnimSequencePosRaw D2CommonOrdinal10367
#define D2CommonUnitStepAnimStateRaw D2CommonOrdinal10368
#define D2CommonUnitStepAnimPosRaw D2CommonOrdinal10369
#define D2CommonUnitStepAnimPosToRaw D2CommonOrdinal10370
#define D2CommonUnitUpdateAnimRateRaw D2CommonOrdinal10371
#define D2CommonUnitSetMissileAnimRateRaw D2CommonOrdinal10372
#define D2CommonUnitTestAnimCompleteRaw D2CommonOrdinal10373
#define D2CommonUnitGetAnimFrameAndCountRaw D2CommonOrdinal10374
#define D2CommonUnitGetAnimPosAndLengthRaw D2CommonOrdinal10375
#define D2CommonUnitSetAnimEventFlagByteRaw D2CommonOrdinal10376
#define D2CommonUnitGetAnimFrameByteRaw D2CommonOrdinal10377
#define D2CommonUnitTakeLightRaw D2CommonOrdinal10378
#define D2CommonAddUnitToRoomRaw D2CommonOrdinal10379
#define D2CommonUnitRoomAddToChangedListRaw D2CommonOrdinal10380
#define D2CommonUnitRoomRemoveUnitRaw D2CommonOrdinal10381
#define D2CommonUnitRoomRemoveFromChangedListRaw D2CommonOrdinal10382
#define D2CommonUnitRoomSortByTileYRaw D2CommonOrdinal10383
#define D2CommonUnitRoomTestUnitInRoomRaw D2CommonOrdinal10384
#define D2CommonUnitRoomLeaveRaw D2CommonOrdinal10385
#define D2CommonUnitRoomClearChangedListRaw D2CommonOrdinal10386
#define D2CommonUnitIsDoorRaw D2CommonOrdinal10387
#define D2CommonUnitTestObjectSubclassBitZeroRaw D2CommonOrdinal10388
#define D2CommonUnitGetObjectClassInfoPtrRaw D2CommonOrdinal10389
#define D2CommonUnitGetObjectShrineRaw D2CommonOrdinal10390
#define D2CommonUnitSetObjectShrineRaw D2CommonOrdinal10391
#define D2CommonUnitGetProximityMetricRaw D2CommonOrdinal10392
#define D2CommonUnitUtilGetDistanceRaw D2CommonOrdinal10393
#define D2CommonUnitCalcEdgeDistanceRaw D2CommonOrdinal10394
#define D2CommonUnitUtilIsWithinRangeRaw D2CommonOrdinal10395
#define D2CommonUnitFindsInitRaw D2CommonOrdinal10396
#define D2CommonUnitFindsFreeArrayRaw D2CommonOrdinal10397
#define D2CommonUnitFindsFindUnitsRaw D2CommonOrdinal10398
#define D2CommonUnitFindsTestUnitRaw D2CommonOrdinal10399
#define D2CommonUnitFindsSearchRoomRaw D2CommonOrdinal10400
#define D2CommonUnitUtilFindOverlappingUnitRaw D2CommonOrdinal10401
#define D2CommonUnitUtilFindUnitAtPointRaw D2CommonOrdinal10402
#define D2CommonUnitFindsTestRoomsNearRaw D2CommonOrdinal10403
#define D2CommonUnitGetInvRecordIndexRaw D2CommonOrdinal10404
#define D2CommonUnitTestObjectHasCollisionRaw D2CommonOrdinal10405
#define D2CommonUnitGetDistanceXRaw D2CommonOrdinal10406
#define D2CommonUnitGetDistanceYRaw D2CommonOrdinal10407
#define D2CommonUnitPathSetExtraRaw D2CommonOrdinal10408
#define D2CommonUnitSyncPositionRaw D2CommonOrdinal10409
#define D2CommonUnitSetLinkedUnitRaw D2CommonOrdinal10410
#define D2CommonUnitSetLinkedUnitIdsRaw D2CommonOrdinal10411
#define D2CommonUnitSetSecondLinkedUnitRaw D2CommonOrdinal10412
#define D2CommonUnitSetPositionExtraRaw D2CommonOrdinal10413
#define D2CommonUnitUtilSetTestCallbackRaw D2CommonOrdinal10414
#define D2CommonUnitCreatePlrInfoRaw D2CommonOrdinal10415
#define D2CommonUnitFreePlrInfoRaw D2CommonOrdinal10416
#define D2CommonUnitSetPlayerNameRaw D2CommonOrdinal10417
#define D2CommonUnitGetPlayerNameRaw D2CommonOrdinal10418
#define D2CommonUnitGetPlrInfoRaw D2CommonOrdinal10419
#define D2CommonUnitSetPlayerLevelBitsRaw D2CommonOrdinal10420
#define D2CommonUnitGetPlayerLevelBitsRaw D2CommonOrdinal10421
#define D2CommonUnitGetObjectFlagsRaw D2CommonOrdinal10422
#define D2CommonUnitSetObjectFlagsRaw D2CommonOrdinal10423
#define D2CommonUnitTestObjectFlagsRaw D2CommonOrdinal10424
#define D2CommonUnitGetOverlayHeightClassRaw D2CommonOrdinal10425
#define D2CommonUnitCalcArmorClassWithBonusRaw D2CommonOrdinal10426
#define D2CommonUnitGetAttackerIndexedValueRaw D2CommonOrdinal10427
#define D2CommonUnitGetActiveWeaponRaw D2CommonOrdinal10428
#define D2CommonUnitGetWClassModeValueRaw D2CommonOrdinal10429
#define D2CommonUnitAddAnimOverlayRaw D2CommonOrdinal10430
#define D2CommonUnitGetCombinedStatDeltaRaw D2CommonOrdinal10431
#define D2CommonUnitCalcGoldLimitRaw D2CommonOrdinal10432
#define D2CommonUnitApplyPrimaryWeaponSwapRaw D2CommonOrdinal10433
#define D2CommonUnitApplyEquipStatSwapRaw D2CommonOrdinal10434
#define D2CommonUnitTestEquipSlotConflictRaw D2CommonOrdinal10435
#define D2CommonUnitSyncPlayerRightSkillRaw D2CommonOrdinal10436
#define D2CommonUnitSyncPlayerLeftSkillRaw D2CommonOrdinal10437
#define D2CommonUnitGetPlayerRightSkillRaw D2CommonOrdinal10438
#define D2CommonUnitGetPlayerLeftSkillRaw D2CommonOrdinal10439
#define D2CommonUnitSetPlayerWeaponIdRaw D2CommonOrdinal10440
#define D2CommonUnitGetPlayerWeaponIdRaw D2CommonOrdinal10441
#define D2CommonUnitGetPathFlagByteRaw D2CommonOrdinal10442
#define D2CommonUnitSetObjectLinksRaw D2CommonOrdinal10443
#define D2CommonUnitGetObjectLinksRaw D2CommonOrdinal10444
#define D2CommonUnitAllocAuxBlockRaw D2CommonOrdinal10445
#define D2CommonUnitFreeAuxBlockRaw D2CommonOrdinal10446
#define D2CommonStatsListSetValueARaw D2CommonOrdinal10447
#define D2CommonStatsListAddValueRaw D2CommonOrdinal10448
#define D2CommonStatsListSetValueBRaw D2CommonOrdinal10449
#define D2CommonStatsListGetValueARaw D2CommonOrdinal10450
#define D2CommonStatsContainerGetEntryRaw D2CommonOrdinal10451
#define D2CommonStatsListResetRaw D2CommonOrdinal10452
#define D2CommonStatsUnitFreeListsRaw D2CommonOrdinal10453
#define D2CommonStatsAllocListRaw D2CommonOrdinal10454
#define D2CommonStatsGetListSourceTypeRaw D2CommonOrdinal10455
#define D2CommonStatsGetListSourceIdRaw D2CommonOrdinal10456
#define D2CommonStatsGetListEndFrameRaw D2CommonOrdinal10457
#define D2CommonStatsUnitUnlinkListRaw D2CommonOrdinal10458
#define D2CommonStatsUnitLinkListRaw D2CommonOrdinal10459
#define D2CommonStatsSetListEndFrameRaw D2CommonOrdinal10460
#define D2CommonStatsSetListRemoveCallbackRaw D2CommonOrdinal10461
#define D2CommonStatsSetListStateRaw D2CommonOrdinal10462
#define D2CommonStatsGetListStateRaw D2CommonOrdinal10463
#define D2CommonStatsFindListByStateRaw D2CommonOrdinal10464
#define D2CommonStatsFindListByMaskRaw D2CommonOrdinal10465
#define D2CommonStatsListFreeRaw D2CommonOrdinal10466
#define D2CommonStatsUnitSetActiveStateBitRaw D2CommonOrdinal10467
#define D2CommonStatsGetStateRaw D2CommonOrdinal10468
#define D2CommonStatsUnitSetActiveStateValueRaw D2CommonOrdinal10469
#define D2CommonStatsUnitTestActiveStateBitRaw D2CommonOrdinal10470
#define D2CommonStatsUnitTestAnyActiveStateRaw D2CommonOrdinal10471
#define D2CommonStatsUnitClearActiveStatesRaw D2CommonOrdinal10472
#define D2CommonStatsUnitCommitPendingFlagsRaw D2CommonOrdinal10473
#define D2CommonStatsUnitGetPendingFlagsRaw D2CommonOrdinal10474
#define D2CommonStatsGetFiveRaw D2CommonOrdinal10475
#define D2CommonStatsTestStateInOverlayGroupRaw D2CommonOrdinal10476
#define D2CommonStatsTestStateSharedListRaw D2CommonOrdinal10477
#define D2CommonStatsTestStateSharedListBaseRaw D2CommonOrdinal10478
#define D2CommonStatsTestStateStoresStatRaw D2CommonOrdinal10479
#define D2CommonStatsUnitTestHasActionRaw D2CommonOrdinal10480
#define D2CommonStatsUnitTestOverlayGroupRaw D2CommonOrdinal10481
#define D2CommonStatsTestStatePassesOnRaw D2CommonOrdinal10482
#define D2CommonStatsTestStateSendsValueRaw D2CommonOrdinal10483
#define D2CommonStatsTestStateSendsTwoValuesRaw D2CommonOrdinal10484
#define D2CommonStatsTestStateSetAllRaw D2CommonOrdinal10485
#define D2CommonStatsTestStateSetNoneARaw D2CommonOrdinal10486
#define D2CommonStatsTestStateSetNoneBRaw D2CommonOrdinal10487
#define D2CommonStatsUnitGetZeroRaw D2CommonOrdinal10488
#define D2CommonStatsUnitClearFieldEcRaw D2CommonOrdinal10489
#define D2CommonStatsUnitCallChangedRaw D2CommonOrdinal10490
#define D2CommonStatsUnitCallChangedAllRaw D2CommonOrdinal10491
#define D2CommonStatsUnitClampToMaxRaw D2CommonOrdinal10492
#define D2CommonStatsUnitFreeStateListsRaw D2CommonOrdinal10493
#define D2CommonStatsUnitExpireListsRaw D2CommonOrdinal10494
#define D2CommonStatsUnitSetValueRaw D2CommonOrdinal10495
#define D2CommonStatsUnitAddValueRaw D2CommonOrdinal10496
#define D2CommonStatsUnitGetValueRaw D2CommonOrdinal10497
#define D2CommonStatsGetTotalValueRaw D2CommonOrdinal10498
#define D2CommonStatsUnitGetBaseValueRaw D2CommonOrdinal10499
#define D2CommonStatsUnitGetBonusValueRaw D2CommonOrdinal10500
#define D2CommonStatsUnitApplyDyeRaw D2CommonOrdinal10501
#define D2CommonStatsUnitRemoveDyeRaw D2CommonOrdinal10502
#define D2CommonStatsUnitRebuildExportRaw D2CommonOrdinal10503
#define D2CommonStatsUnitAllocStatsRaw D2CommonOrdinal10504
#define D2CommonStatsUnitFreeStatsRaw D2CommonOrdinal10505
#define D2CommonStatsSetListEndFrameSecondRaw D2CommonOrdinal10506
#define D2CommonStatsGetGlobalListEndFrameRaw D2CommonOrdinal10507
#define D2CommonStatsTestHasOverlayRaw D2CommonOrdinal10508
#define D2CommonStatsListSetValuePackedRaw D2CommonOrdinal10509
#define D2CommonStatsListGetValueBRaw D2CommonOrdinal10510
#define D2CommonStatsUnitClearHasOverlayRaw D2CommonOrdinal10511
#define D2CommonStatsUnitSetHasOverlayRaw D2CommonOrdinal10512
#define D2CommonStatsTestModifyingRaw D2CommonOrdinal10513
#define D2CommonStatsUnitTestLightColorRaw D2CommonOrdinal10514
#define D2CommonStatsUnitTestToHitRaisedRaw D2CommonOrdinal10515
#define D2CommonStatsUnitTestDamageRaisedRaw D2CommonOrdinal10516
#define D2CommonStatsUnitTestArmorClassRaisedRaw D2CommonOrdinal10517
#define D2CommonStatsUnitTestFireResistRaisedRaw D2CommonOrdinal10518
#define D2CommonStatsUnitTestColdResistRaisedRaw D2CommonOrdinal10519
#define D2CommonStatsUnitTestLightResistRaisedRaw D2CommonOrdinal10520
#define D2CommonStatsUnitTestPoisonResistRaisedRaw D2CommonOrdinal10521
#define D2CommonStatsUnitTestToHitLoweredRaw D2CommonOrdinal10522
#define D2CommonStatsUnitTestDamageLoweredRaw D2CommonOrdinal10523
#define D2CommonStatsUnitTestArmorClassLoweredRaw D2CommonOrdinal10524
#define D2CommonStatsUnitTestFireResistLoweredRaw D2CommonOrdinal10525
#define D2CommonStatsUnitTestColdResistLoweredRaw D2CommonOrdinal10526
#define D2CommonStatsUnitTestLightResistLoweredRaw D2CommonOrdinal10527
#define D2CommonStatsUnitTestPoisonResistLoweredRaw D2CommonOrdinal10528
#define D2CommonStatsUnitTestMatchStateRaw D2CommonOrdinal10529
#define D2CommonStatsUnitTestOwnedStateRaw D2CommonOrdinal10530
#define D2CommonStatsTestStateOwnedRaw D2CommonOrdinal10531
#define D2CommonStatsUnitTestStateKeptRaw D2CommonOrdinal10532
#define D2CommonStatsUnitTestStateGroupARaw D2CommonOrdinal10533
#define D2CommonStatsTestStateGroupARaw D2CommonOrdinal10534
#define D2CommonStatsUnitTestStateGroupBRaw D2CommonOrdinal10535
#define D2CommonStatsTestStateGroupBRaw D2CommonOrdinal10536
#define D2CommonStatsUnitTestRegenBaseOnlyRaw D2CommonOrdinal10537
#define D2CommonStatsUnitTestStateGroupCRaw D2CommonOrdinal10538
#define D2CommonStatsUnitTestNoColorsRaw D2CommonOrdinal10539
#define D2CommonStatsUnitGetAlignmentRaw D2CommonOrdinal10540
#define D2CommonStatsUnitsTestSameAlignmentRaw D2CommonOrdinal10541
#define D2CommonStatsUnitCalcMaxHpRaw D2CommonOrdinal10542
#define D2CommonStatsUnitCalcMaxManaRaw D2CommonOrdinal10543
#define D2CommonStatsUnitCalcMaxStaminaRaw D2CommonOrdinal10544
#define D2CommonStatsUnitCalcMaxDurabilityRaw D2CommonOrdinal10545
#define D2CommonStatsUnitCalcMaxDamageRaw D2CommonOrdinal10546
#define D2CommonStatsUnitCalcMinDamageRaw D2CommonOrdinal10547
#define D2CommonStatsUnitCalcArmorClassRaw D2CommonOrdinal10548
#define D2CommonExcelParseRecordsRaw D2CommonOrdinal10549
#define D2CommonExcelOpenRaw D2CommonOrdinal10550
#define D2CommonExcelFileFreeRaw D2CommonOrdinal10551
#define D2CommonExcelFileGetRowCountRaw D2CommonOrdinal10552
#define D2CommonDataTblsFreeAllRaw D2CommonOrdinal10553
#define D2CommonDataTblsLoadAllRaw D2CommonOrdinal10554
#define D2CommonDataTblsLoadExcelFileRaw D2CommonOrdinal10555
#define D2CommonDataTblsAllocAndTokenizeRaw D2CommonOrdinal10556
#define D2CommonDataTblsFreeBlockRaw D2CommonOrdinal10557
#define D2CommonDataTblsNothingRaw D2CommonOrdinal10558
#define D2CommonDataTblsGetCharStatsRaw D2CommonOrdinal10559
#define D2CommonMonsterTblsGetStatsRaw D2CommonOrdinal10560
#define D2CommonMissileTblsGetMissileRaw D2CommonOrdinal10561
#define D2CommonMissileTblsGetVelRaw D2CommonOrdinal10562
#define D2CommonOverlayTblsGetOverlayRaw D2CommonOrdinal10563
#define D2CommonMonsterTblsGetRunRaw D2CommonOrdinal10564
#define D2CommonDataTblsGetCharStatsTxtRaw D2CommonOrdinal10565
#define D2CommonDataTblsGetSkillRaw D2CommonOrdinal10566
#define D2CommonDataTblsGetSkillCountRaw D2CommonOrdinal10567
#define D2CommonArenaGetZeroRaw D2CommonOrdinal10568
#define D2CommonItemTblsGetItemStatsDescRaw D2CommonOrdinal10569
#define D2CommonItemTblsGetItemStatsRaw D2CommonOrdinal10570
#define D2CommonItemTblsFindItemByCodeRaw D2CommonOrdinal10571
#define D2CommonItemTblsFindItemIndexByCodeRaw D2CommonOrdinal10572
#define D2CommonItemTblsGetMagicAffixDescRaw D2CommonOrdinal10573
#define D2CommonItemTblsGetMagicAffixRaw D2CommonOrdinal10574
#define D2CommonItemTblsGetRareAffixDescRaw D2CommonOrdinal10575
#define D2CommonItemTblsGetRareAffixRaw D2CommonOrdinal10576
#define D2CommonItemTblsGetUniqueItemDescRaw D2CommonOrdinal10577
#define D2CommonItemTblsGetUniqueItemRaw D2CommonOrdinal10578
#define D2CommonItemTblsGetSetItemDescRaw D2CommonOrdinal10579
#define D2CommonItemTblsGetSetItemRaw D2CommonOrdinal10580
#define D2CommonItemTblsGetQualityItemDescRaw D2CommonOrdinal10581
#define D2CommonItemTblsGetQualityItemRaw D2CommonOrdinal10582
#define D2CommonItemTblsGetLowQualityItemDescRaw D2CommonOrdinal10583
#define D2CommonItemTblsGetLowQualityItemRaw D2CommonOrdinal10584
#define D2CommonItemTblsGetGemDescRaw D2CommonOrdinal10585
#define D2CommonItemTblsGetGemRaw D2CommonOrdinal10586
#define D2CommonItemTblsGetBookDescRaw D2CommonOrdinal10587
#define D2CommonItemTblsGetBookRaw D2CommonOrdinal10588
#define D2CommonItemTblsGetItemRatioDescRaw D2CommonOrdinal10589
#define D2CommonItemTblsGetItemRatioRaw D2CommonOrdinal10590
#define D2CommonObjTblsGetShrineRaw D2CommonOrdinal10591
#define D2CommonObjTblsGetShrineCountRaw D2CommonOrdinal10592
#define D2CommonObjTblsGetObjectRaw D2CommonOrdinal10593
#define D2CommonObjTblsGetObjGroupRaw D2CommonOrdinal10594
#define D2CommonDataTblsGetExperienceRaw D2CommonOrdinal10595
#define D2CommonDataTblsGetMaxLevelRaw D2CommonOrdinal10596
#define D2CommonDataTblsGetLevelFromExperienceRaw D2CommonOrdinal10597
#define D2CommonLvlTblsGetLevelDataRaw D2CommonOrdinal10598
#define D2CommonLvlTblsGetRainRaw D2CommonOrdinal10599
#define D2CommonLvlTblsGetMudRaw D2CommonOrdinal10600
#define D2CommonLvlTblsGetNoPerRaw D2CommonOrdinal10601
#define D2CommonInvTblsGetInvRectRaw D2CommonOrdinal10602
#define D2CommonInvTblsGetGridRaw D2CommonOrdinal10603
#define D2CommonInvTblsGetBodyLocRaw D2CommonOrdinal10604
#define D2CommonBeltTblsGetBeltRaw D2CommonOrdinal10605
#define D2CommonBeltTblsGetBoxRaw D2CommonOrdinal10606
#define D2CommonAnimTblsSetUnitAnimSpeedRaw D2CommonOrdinal10607
#define D2CommonAnimTblsGetAnimInfoRaw D2CommonOrdinal10608
#define D2CommonMonsterTblsSetVelocityRaw D2CommonOrdinal10609
#define D2CommonTokTblsGetPlayerDescRaw D2CommonOrdinal10610
#define D2CommonTokTblsGetMonsterDescRaw D2CommonOrdinal10611
#define D2CommonTokTblsGetObjectDescRaw D2CommonOrdinal10612
#define D2CommonTokTblsGetPlayerTokenRaw D2CommonOrdinal10613
#define D2CommonTokTblsGetMonsterTokenRaw D2CommonOrdinal10614
#define D2CommonTokTblsGetObjectTokenRaw D2CommonOrdinal10615
#define D2CommonTokTblsGetComponentRaw D2CommonOrdinal10616
#define D2CommonTokTblsGetArmorRaw D2CommonOrdinal10617
#define D2CommonMonsterTblsGetRandomUniqueTitleRaw D2CommonOrdinal10618
#define D2CommonMonsterTblsGetRandomUniquePrefixRaw D2CommonOrdinal10619
#define D2CommonMonsterTblsRollUniqueSuffixRaw D2CommonOrdinal10620
#define D2CommonMonsterTblsRollUniqueAppellationRaw D2CommonOrdinal10621
#define D2CommonMonsterTblsGetTreasureClassDescRaw D2CommonOrdinal10622
#define D2CommonMonsterTblsGetTreasureClassRaw D2CommonOrdinal10623
#define D2CommonMonsterTblsGetMonItemPercentDescRaw D2CommonOrdinal10624
#define D2CommonMonsterTblsGetMonItemPercentRaw D2CommonOrdinal10625
#define D2CommonArenaGetCharTemplateCountRaw D2CommonOrdinal10626
#define D2CommonArenaGetCharTemplateRaw D2CommonOrdinal10627
#define D2CommonArenaGetCharTemplateClassRaw D2CommonOrdinal10628
#define D2CommonTransformTblsWarnStubRaw D2CommonOrdinal10629
#define D2CommonMonsterTblsGetSuperUniqueRaw D2CommonOrdinal10630
#define D2CommonMonsterTblsTestSecondIsSelRaw D2CommonOrdinal10631
#define D2CommonMonsterTblsTestNeverSelRaw D2CommonOrdinal10632
#define D2CommonItemTblsGetGambleItemsRaw D2CommonOrdinal10633
#define D2CommonOverlayTblsGetFramesRaw D2CommonOrdinal10634
#define D2CommonOverlayTblsGetPreDrawRaw D2CommonOrdinal10635
#define D2CommonOverlayTblsGetXoffsetRaw D2CommonOrdinal10636
#define D2CommonOverlayTblsGetYoffsetRaw D2CommonOrdinal10637
#define D2CommonOverlayTblsGetTransRaw D2CommonOrdinal10638
#define D2CommonOverlayTblsGetLightRaw D2CommonOrdinal10639
#define D2CommonOverlayTblsGet1ofNRaw D2CommonOrdinal10640
#define D2CommonOverlayTblsGetDirRaw D2CommonOrdinal10641
#define D2CommonOverlayTblsGetAnimSequenceRaw D2CommonOrdinal10642
#define D2CommonSequenceGetLengthFixedRaw D2CommonOrdinal10643
#define D2CommonSequenceGetFrameCountRaw D2CommonOrdinal10644
#define D2CommonTokTblsGetFrameDataRaw D2CommonOrdinal10645
#define D2CommonTokTblsGetFrameClassRaw D2CommonOrdinal10646
#define D2CommonITEMSCreateItemDataRaw D2CommonOrdinal10647
#define D2CommonITEMSFreeItemDataRaw D2CommonOrdinal10648
#define D2CommonITEMSGetCategoryRaw D2CommonOrdinal10649
#define D2CommonITEMSSetCategoryRaw D2CommonOrdinal10650
#define D2CommonITEMSGetItemSeedRaw D2CommonOrdinal10651
#define D2CommonITEMSAdvanceItemSeedRaw D2CommonOrdinal10652
#define D2CommonITEMSGetItemSeedValueRaw D2CommonOrdinal10653
#define D2CommonITEMSSetItemSeedValueRaw D2CommonOrdinal10654
#define D2CommonITEMSDataGetModTypeRaw D2CommonOrdinal10655
#define D2CommonITEMSDataSetModTypeRaw D2CommonOrdinal10656
#define D2CommonITEMSDataGetMagicPrefixAtRaw D2CommonOrdinal10657
#define D2CommonITEMSDataSetMagicPrefixAtRaw D2CommonOrdinal10658
#define D2CommonITEMSDataGetMagicSuffixAtRaw D2CommonOrdinal10659
#define D2CommonITEMSDataSetMagicSuffixAtRaw D2CommonOrdinal10660
#define D2CommonITEMSDataGetRarePrefixRaw D2CommonOrdinal10661
#define D2CommonITEMSDataSetRarePrefixRaw D2CommonOrdinal10662
#define D2CommonITEMSDataGetRareSuffixRaw D2CommonOrdinal10663
#define D2CommonITEMSDataSetRareSuffixRaw D2CommonOrdinal10664
#define D2CommonITEMSDataTestFlagsRaw D2CommonOrdinal10665
#define D2CommonITEMSDataSetFlagsRaw D2CommonOrdinal10666
#define D2CommonITEMSDataGetFlagsRaw D2CommonOrdinal10667
#define D2CommonITEMSDataTestCmdFlagsRaw D2CommonOrdinal10668
#define D2CommonITEMSDataSetCmdFlagsRaw D2CommonOrdinal10669
#define D2CommonITEMSDataGetCmdFlagsRaw D2CommonOrdinal10670
#define D2CommonITEMSDataSetMagicPrefixRaw D2CommonOrdinal10671
#define D2CommonITEMSDataGetMagicPrefixRaw D2CommonOrdinal10672
#define D2CommonITEMSDataSetMagicSuffixRaw D2CommonOrdinal10673
#define D2CommonITEMSDataGetMagicSuffixRaw D2CommonOrdinal10674
#define D2CommonITEMSGetLevelIdRaw D2CommonOrdinal10675
#define D2CommonITEMSSetLevelIdRaw D2CommonOrdinal10676
#define D2CommonITEMSDataGetPageRaw D2CommonOrdinal10677
#define D2CommonITEMSDataSetPageRaw D2CommonOrdinal10678
#define D2CommonITEMSDataGetSwapPageRaw D2CommonOrdinal10679
#define D2CommonITEMSDataSetSwapPageRaw D2CommonOrdinal10680
#define D2CommonITEMSGetNameBufferRaw D2CommonOrdinal10681
#define D2CommonITEMSSetNameBufferRaw D2CommonOrdinal10682
#define D2CommonITEMSDataGetEarLevelRaw D2CommonOrdinal10683
#define D2CommonITEMSDataSetEarLevelRaw D2CommonOrdinal10684
#define D2CommonITEMSDataGetGfxIndexRaw D2CommonOrdinal10685
#define D2CommonITEMSDataSetGfxIndexRaw D2CommonOrdinal10686
#define D2CommonITEMSIsWeaponRaw D2CommonOrdinal10687
#define D2CommonITEMSIsWeaponNoCheckRaw D2CommonOrdinal10688
#define D2CommonITEMSDataGetUniqueOrSetRowRaw D2CommonOrdinal10689
#define D2CommonITEMSDataSetUniqueOrSetRowRaw D2CommonOrdinal10690
#define D2CommonITEMSDataSetOwnerIdRaw D2CommonOrdinal10691
#define D2CommonITEMSDataGetOwnerIdRaw D2CommonOrdinal10692
#define D2CommonITEMSIsArmorRaw D2CommonOrdinal10693
#define D2CommonITEMSIsArmorNoCheckRaw D2CommonOrdinal10694
#define D2CommonITEMSTestHasBodyLocRaw D2CommonOrdinal10695
#define D2CommonITEMSIsMiscellaneousNoCheckRaw D2CommonOrdinal10696
#define D2CommonITEMSIsMiscellaneousRaw D2CommonOrdinal10697
#define D2CommonITEMSGetZeroARaw D2CommonOrdinal10698
#define D2CommonITEMSGetZeroBRaw D2CommonOrdinal10699
#define D2CommonITEMSGetConstantFiveRaw D2CommonOrdinal10700
#define D2CommonITEMSTestSellableRaw D2CommonOrdinal10701
#define D2CommonITEMSGetTypeWClassRaw D2CommonOrdinal10702
#define D2CommonITEMSGetTypeTwoHandedWClassRaw D2CommonOrdinal10703
#define D2CommonITEMSGetTypeCodeRaw D2CommonOrdinal10704
#define D2CommonITEMSGetTypeAlternateGfxOrCodeRaw D2CommonOrdinal10705
#define D2CommonITEMSGetTypeComponentRaw D2CommonOrdinal10706
#define D2CommonITEMSGetTypeInvWidthHeightRaw D2CommonOrdinal10707
#define D2CommonITEMSGetTypeBodyLocsRaw D2CommonOrdinal10708
#define D2CommonITEMSGetTypeRaw D2CommonOrdinal10709
#define D2CommonITEMSGetTypeNoCheckRaw D2CommonOrdinal10710
#define D2CommonITEMSGetTypeLevelRaw D2CommonOrdinal10711
#define D2CommonITEMSMaskFlagsRaw D2CommonOrdinal10712
#define D2CommonITEMSSetOrClearFlagsRaw D2CommonOrdinal10713
#define D2CommonITEMSTestFitsInventorySpaceRaw D2CommonOrdinal10714
#define D2CommonITEMSGetRequiredLevelRaw D2CommonOrdinal10715
#define D2CommonITEMSTestTypeBodyLocEqualRaw D2CommonOrdinal10716
#define D2CommonITEMSGetTypeThrowableRaw D2CommonOrdinal10717
#define D2CommonITEMSGetTypeMissileTypeRaw D2CommonOrdinal10718
#define D2CommonITEMSGetTypeRangeAdderRaw D2CommonOrdinal10719
#define D2CommonITEMSGetTypeThrowableNoCheckRaw D2CommonOrdinal10720
#define D2CommonITEMSTestTypeWClassEqualNoCheckRaw D2CommonOrdinal10721
#define D2CommonITEMSTestTypeWClassEqualRaw D2CommonOrdinal10722
#define D2CommonITEMSGetTypeQuiveredRaw D2CommonOrdinal10723
#define D2CommonITEMSGetTypeQuiveredNoCheckRaw D2CommonOrdinal10724
#define D2CommonITEMSGetTypeTwoHandedRaw D2CommonOrdinal10725
#define D2CommonITEMSGetTypeTwoHandedNoCheckRaw D2CommonOrdinal10726
#define D2CommonITEMSGetTypeStackableRaw D2CommonOrdinal10727
#define D2CommonITEMSGetTypeBeltRaw D2CommonOrdinal10728
#define D2CommonITEMSTestTypeGroupMatchRaw D2CommonOrdinal10729
#define D2CommonITEMSTestAutobeltOrFitsRaw D2CommonOrdinal10730
#define D2CommonITEMSGetTypeUseableRaw D2CommonOrdinal10731
#define D2CommonITEMSGetTypeUniqueRaw D2CommonOrdinal10732
#define D2CommonITEMSGetTypeQuestRaw D2CommonOrdinal10733
#define D2CommonITEMSTestTypeSellableNoCheckRaw D2CommonOrdinal10734
#define D2CommonITEMSGetPriceRaw D2CommonOrdinal10735
#define D2CommonITEMSCountUnidentifiedRaw D2CommonOrdinal10736
#define D2CommonITEMSTestRepairableRaw D2CommonOrdinal10737
#define D2CommonITEMSTestTypeIdentifiableRaw D2CommonOrdinal10738
#define D2CommonITEMSGetTranslateTblFirstRaw D2CommonOrdinal10739
#define D2CommonITEMSGetTypeMaxStackNoCheckRaw D2CommonOrdinal10740
#define D2CommonITEMSGetTypeMaxStackRaw D2CommonOrdinal10741
#define D2CommonITEMSGetTypeMinStackNoCheckRaw D2CommonOrdinal10742
#define D2CommonITEMSGetTypeMinStackRaw D2CommonOrdinal10743
#define D2CommonITEMSTestTypeBitfieldFourthBitRaw D2CommonOrdinal10744
#define D2CommonITEMSTestTypeBitfieldThirdBitRaw D2CommonOrdinal10745
#define D2CommonITEMSTestTypeBitfieldSecondBitRaw D2CommonOrdinal10746
#define D2CommonITEMSTestTypeBitfieldFirstBitRaw D2CommonOrdinal10747
#define D2CommonITEMSGetSpellIconRaw D2CommonOrdinal10748
#define D2CommonITEMSGetTypeDurWarningRaw D2CommonOrdinal10749
#define D2CommonITEMSGetTypeQntWarningRaw D2CommonOrdinal10750
#define D2CommonITEMSGetTypeStrBonusRaw D2CommonOrdinal10751
#define D2CommonITEMSGetTypeDexBonusRaw D2CommonOrdinal10752
#define D2CommonITEMSGetTypeHasInvNoCheckRaw D2CommonOrdinal10753
#define D2CommonITEMSGetTypeHasInvRaw D2CommonOrdinal10754
#define D2CommonITEMSTestTypeHasDurabilityRaw D2CommonOrdinal10755
#define D2CommonITEMSTestKeepsRolledQualityRaw D2CommonOrdinal10756
#define D2CommonITEMSGetTypeGemSocketsNoCheckRaw D2CommonOrdinal10757
#define D2CommonITEMSGetTypeGemSocketsRaw D2CommonOrdinal10758
#define D2CommonITEMSGetWClassIndexRaw D2CommonOrdinal10759
#define D2CommonITEMSTestTypeTransmogrifyNoCheckRaw D2CommonOrdinal10760
#define D2CommonITEMSTestTypeTransmogrifyRaw D2CommonOrdinal10761
#define D2CommonITEMSTestModTypeMagicOrAboveRaw D2CommonOrdinal10762
#define D2CommonITEMSGetTypeHitClassRaw D2CommonOrdinal10763
#define D2CommonITEMSTestOneOrTwoHandedRaw D2CommonOrdinal10764
#define D2CommonITEMSGetColorTransformRaw D2CommonOrdinal10765
#define D2CommonITEMSTestModifiableRaw D2CommonOrdinal10766
#define D2CommonITEMSGetWeaponGroupEnumRaw D2CommonOrdinal10767
#define D2CommonITEMSTestCanStackRaw D2CommonOrdinal10768
#define D2CommonITEMSValidateQuantityRaw D2CommonOrdinal10769
#define D2CommonITEMSTestSetCompleteRaw D2CommonOrdinal10770
#define D2CommonITEMSTestEffectsActiveRaw D2CommonOrdinal10771
#define D2CommonITEMSScaleValueByPlayerClassRaw D2CommonOrdinal10772
#define D2CommonITEMSScaleValueByPlayerClassAltRaw D2CommonOrdinal10773
#define D2CommonLoggingUnpackDwordRaw D2CommonOrdinal10774
#define D2CommonITEMSDataGetPreviousPageRaw D2CommonOrdinal10775
#define D2CommonITEMSDataSetPreviousPageRaw D2CommonOrdinal10776
#define D2CommonITEMSModsApplyRaw D2CommonOrdinal10777
#define D2CommonITEMSModsApplySetPropertiesRaw D2CommonOrdinal10778
#define D2CommonITEMSModsGetStatIdRaw D2CommonOrdinal10779
#define D2CommonITEMSModsTestMagicFitsRaw D2CommonOrdinal10780
#define D2CommonITEMSModsTestRareFitsRaw D2CommonOrdinal10781
#define D2CommonITEMSModsTestQualityFitsRaw D2CommonOrdinal10782
#define D2CommonITEMSRollElixirKindRaw D2CommonOrdinal10783
#define D2CommonITEMSAddRandomSkillStatsByTypeRaw D2CommonOrdinal10784
#define D2CommonCOMPOSITGetCofNameRaw D2CommonOrdinal10785
#define D2CommonCOMPOSITGetCofNameForUnitRaw D2CommonOrdinal10786
#define D2CommonCOMPOSITGetWeaponClassIndexRaw D2CommonOrdinal10787
#define D2CommonCOMPOSITGetWeaponClassRaw D2CommonOrdinal10788
#define D2CommonCOMPOSITGetUnitWeaponClassRaw D2CommonOrdinal10789
#define D2CommonCOMPOSITTestSlotMemberRaw D2CommonOrdinal10790
#define D2CommonCOMPOSITIsRangedAttackRaw D2CommonOrdinal10791
#define D2CommonCOMPOSITGetComponentByteRaw D2CommonOrdinal10792
#define D2CommonChatNodeNewRaw D2CommonOrdinal10793
#define D2CommonChatNodeFreeRaw D2CommonOrdinal10794
#define D2CommonChatNodeGetDurationRaw D2CommonOrdinal10795
#define D2CommonChatNodeGetEndFrameRaw D2CommonOrdinal10796
#define D2CommonChatNodeCopyStringRaw D2CommonOrdinal10797
#define D2CommonChatNodeGetTextBoxRaw D2CommonOrdinal10798
#define D2CommonChatNodeSetTextBoxRaw D2CommonOrdinal10799
#define D2CommonChatNodeGetMessageByteRaw D2CommonOrdinal10800
#define D2CommonWaypointSetChatNodeMessageByteRaw D2CommonOrdinal10801
#define D2CommonTextListNewRaw D2CommonOrdinal10802
#define D2CommonTextListFreeRaw D2CommonOrdinal10803
#define D2CommonTextListInsertRaw D2CommonOrdinal10804
#define D2CommonTextListRemoveByIdRaw D2CommonOrdinal10805
#define D2CommonTextListCountMenuLinesRaw D2CommonOrdinal10806
#define D2CommonTextListFindMenuLineIdRaw D2CommonOrdinal10807
#define D2CommonTextListFindOpeningTextIdRaw D2CommonOrdinal10808
#define D2CommonTextListFindFallbackTextIdRaw D2CommonOrdinal10809
#define D2CommonTextListPackRaw D2CommonOrdinal10810
#define D2CommonTextListUnpackRaw D2CommonOrdinal10811
#define D2CommonTextListSortRaw D2CommonOrdinal10812
#define D2CommonSeedInitRaw D2CommonOrdinal10813
#define D2CommonSeedSetRaw D2CommonOrdinal10814
#define D2CommonSeedGetLowRaw D2CommonOrdinal10815
#define D2CommonSeedGetHighRaw D2CommonOrdinal10816
#define D2CommonSeedNothingRaw D2CommonOrdinal10817
#define D2CommonSeedNextRaw D2CommonOrdinal10818
#define D2CommonSeedRandRaw D2CommonOrdinal10819
#define D2CommonSeedBetweenRaw D2CommonOrdinal10820
#define D2CommonSeedInventRaw D2CommonOrdinal10821
#define D2CommonSeedSetStateRaw D2CommonOrdinal10822
#define D2CommonSeedGetStateRaw D2CommonOrdinal10823
#define D2CommonEnvTickRaw D2CommonOrdinal10824
#define D2CommonEnvGetSunRaw D2CommonOrdinal10825
#define D2CommonEnvGetIntensityRaw D2CommonOrdinal10826
#define D2CommonEnvGetColorRaw D2CommonOrdinal10827
#define D2CommonEnvGetZeroFieldRaw D2CommonOrdinal10828
#define D2CommonEnvNextCycleRaw D2CommonOrdinal10829
#define D2CommonEnvGetCycleRaw D2CommonOrdinal10830
#define D2CommonEnvGetStateRaw D2CommonOrdinal10831
#define D2CommonEnvSetCycleRaw D2CommonOrdinal10832
#define D2CommonEnvGetCycleValueRaw D2CommonOrdinal10833
#define D2CommonEnvGetCycleTimeRaw D2CommonOrdinal10834
#define D2CommonEnvNextCycleTimeRaw D2CommonOrdinal10835
#define D2CommonEnvSetAltCycleRaw D2CommonOrdinal10836
#define D2CommonEnvSetUsualCycleRaw D2CommonOrdinal10837
#define D2CommonSkillsLookupKeyAFromKeyBRaw D2CommonOrdinal10838
#define D2CommonSkillsLookupKeyBFromKeyARaw D2CommonOrdinal10839
#define D2CommonSkillsUpdatePassiveRaw D2CommonOrdinal10840
#define D2CommonSkillsRefreshPassivesRaw D2CommonOrdinal10841
#define D2CommonSkillsGetNodeModeIfAnyRaw D2CommonOrdinal10842
#define D2CommonSkillsGetNodeShiverRaw D2CommonOrdinal10843
#define D2CommonSkillsGetNodeRangeRaw D2CommonOrdinal10844
#define D2CommonSkillsAllocNodeRaw D2CommonOrdinal10845
#define D2CommonSkillsRebuildCachesRaw D2CommonOrdinal10846
#define D2CommonSkillsNodeGetNextRaw D2CommonOrdinal10847
#define D2CommonSkillsHeadGetFirstRaw D2CommonOrdinal10848
#define D2CommonSkillsFindByKeyRaw D2CommonOrdinal10849
#define D2CommonSkillsDecrementSkillRaw D2CommonOrdinal10850
#define D2CommonSkillsAddSkillRaw D2CommonOrdinal10851
#define D2CommonSkillsSetSkillLevelRaw D2CommonOrdinal10852
#define D2CommonSkillsExchangeSelectedRaw D2CommonOrdinal10853
#define D2CommonSkillsGetSelectedUnsafeRaw D2CommonOrdinal10854
#define D2CommonSkillsFreeListRaw D2CommonOrdinal10855
#define D2CommonSkillsCacheLeftSkillRaw(ecx, a1, a2) \
	(D2CommonOrdinal10856(a1, a2))
#define D2CommonSkillsCacheRightSkillRaw(ecx, a1, a2) \
	(D2CommonOrdinal10857(a1, a2))
#define D2CommonSkillsGetNodeTypeRaw D2CommonOrdinal10858
#define D2CommonSkillsTestUsableRaw D2CommonOrdinal10859
#define D2CommonSkillsGetNodeSlotRaw D2CommonOrdinal10860
#define D2CommonSkillsGetNodeTypeRecordRaw D2CommonOrdinal10861
#define D2CommonSkillsGetComboFlagsRaw D2CommonOrdinal10862
#define D2CommonSkillsCalcNodeLevelRaw D2CommonOrdinal10863
#define D2CommonSkillsGetNodeScratchARaw D2CommonOrdinal10864
#define D2CommonSkillsSetNodeScratchARaw D2CommonOrdinal10865
#define D2CommonSkillsGetNodeScratchBRaw D2CommonOrdinal10866
#define D2CommonSkillsSetNodeScratchBRaw D2CommonOrdinal10867
#define D2CommonSkillsResetNodeRaw D2CommonOrdinal10868
#define D2CommonSkillsSetNodeModeRaw D2CommonOrdinal10869
#define D2CommonSkillsGetNodeModeRaw D2CommonOrdinal10870
#define D2CommonSkillsGetNodeScratchCRaw D2CommonOrdinal10871
#define D2CommonSkillsGetNodeScratchDRaw D2CommonOrdinal10872
#define D2CommonSkillsGetNodeScratchERaw D2CommonOrdinal10873
#define D2CommonSkillsGetNodeScratchFRaw D2CommonOrdinal10874
#define D2CommonSkillsSetNodeScratchCRaw D2CommonOrdinal10875
#define D2CommonSkillsSetNodeScratchDRaw D2CommonOrdinal10876
#define D2CommonSkillsSetNodeScratchERaw D2CommonOrdinal10877
#define D2CommonSkillsSetNodeScratchFRaw D2CommonOrdinal10878
#define D2CommonSkillsSetNodeFlagsRaw D2CommonOrdinal10879
#define D2CommonSkillsGetNodeFlagsRaw D2CommonOrdinal10880
#define D2CommonSkillsGetTypeReqLevelRaw D2CommonOrdinal10881
#define D2CommonSkillsCalcReqLevelRaw D2CommonOrdinal10882
#define D2CommonSkillsTestPrerequisitesRaw D2CommonOrdinal10883
#define D2CommonSkillsTestStatRequirementsRaw D2CommonOrdinal10884
#define D2CommonSkillsGetTypeParamOneRaw D2CommonOrdinal10885
#define D2CommonSkillsGetTypeParamTwoRaw D2CommonOrdinal10886
#define D2CommonSkillsGetTypeParamThreeRaw D2CommonOrdinal10887
#define D2CommonSkillsGetTypeParamFourRaw D2CommonOrdinal10888
#define D2CommonSkillsGetTypeParamFiveRaw D2CommonOrdinal10889
#define D2CommonSkillsGetTypeParamSixRaw D2CommonOrdinal10890
#define D2CommonSkillsCalcParamOneByLevelRaw D2CommonOrdinal10891
#define D2CommonSkillsCalcParamThreeByLevelRaw D2CommonOrdinal10892
#define D2CommonSkillsCalcParamFiveByLevelRaw D2CommonOrdinal10893
#define D2CommonSkillsCalcToHitByLevelRaw D2CommonOrdinal10894
#define D2CommonSkillsSumNodeBonusRaw D2CommonOrdinal10895
#define D2CommonSkillsCalcManaCostRaw D2CommonOrdinal10896
#define D2CommonSkillsCalcValueARaw D2CommonOrdinal10897
#define D2CommonSkillsCalcValueBRaw D2CommonOrdinal10898
#define D2CommonSkillsCalcEMinByLevelRaw D2CommonOrdinal10899
#define D2CommonSkillsCalcEMaxByLevelRaw D2CommonOrdinal10900
#define D2CommonSkillsCalcELenByLevelRaw D2CommonOrdinal10901
#define D2CommonSkillsGetTypeHitShiftRaw D2CommonOrdinal10902
#define D2CommonSkillsGetTypeCharClassRaw D2CommonOrdinal10903
#define D2CommonSkillsGetNodeCountRaw D2CommonOrdinal10904
#define D2CommonSkillsSetNodeCountRaw D2CommonOrdinal10905
#define D2CommonSkillsClearFlagIfMatchedRaw D2CommonOrdinal10906
#define D2CommonSkillsUpdateNodeSeedRaw D2CommonOrdinal10907
#define D2CommonSkillsGetNodeRangeForUnitRaw D2CommonOrdinal10908
#define D2CommonSkillsUnitTestGuardFRaw D2CommonOrdinal10909
#define D2CommonSkillsUnitTestGuardDRaw D2CommonOrdinal10910
#define D2CommonSkillsUnitTestGuardARaw D2CommonOrdinal10911
#define D2CommonSkillsUnitTestGuardERaw D2CommonOrdinal10912
#define D2CommonSkillsUnitTestGuardBRaw D2CommonOrdinal10913
#define D2CommonSkillsUnitTestGuardCRaw D2CommonOrdinal10914
#define D2CommonSkillsGetItemSkillValueAnyRaw D2CommonOrdinal10915
#define D2CommonSkillsGetItemSkillValueRaw D2CommonOrdinal10916
#define D2CommonSkillsAddFieldARaw D2CommonOrdinal10917
#define D2CommonSkillsAddFieldBRaw D2CommonOrdinal10918
#define D2CommonSkillsCalcManaPointsRaw D2CommonOrdinal10919
#define D2CommonSkillsCalcCountPercentRaw D2CommonOrdinal10920
#define D2CommonSkillsGetBonusByKeyRaw D2CommonOrdinal10921
#define D2CommonSkillsSetSkillBonusRaw D2CommonOrdinal10922
#define D2CommonSkillsAdjustBonusByKeyRaw D2CommonOrdinal10923
#define D2CommonSkillsGetDifficultyTableEntryRaw D2CommonOrdinal10924
#define D2CommonSkillsCalcCurveValueRaw D2CommonOrdinal10925
#define D2CommonSkillsCalcCurveParamOneRaw D2CommonOrdinal10926
#define D2CommonSkillsCalcCurveParamThreeRaw D2CommonOrdinal10927
#define D2CommonSkillsCalcCurveParamFiveRaw D2CommonOrdinal10928
#define D2CommonSkillsGetFieldPastTargetRaw D2CommonOrdinal10929
#define D2CommonSkillsAddListRangeRaw D2CommonOrdinal10930
#define D2CommonSkillsGetRangedAttackCostRaw D2CommonOrdinal10931
#define D2CommonSkillsTestIdFlagRaw D2CommonOrdinal10932
#define D2CommonMonsterRegionGetMissileFieldRaw D2CommonOrdinal10933
#define D2CommonMonsterUnitGetBestSlotRankRaw(ecx, a1) \
	(D2CommonOrdinal10934(a1))
#define D2CommonMonsterRegionLookupTableARaw D2CommonOrdinal10935
#define D2CommonMonsterRegionLookupTableBRaw D2CommonOrdinal10936
#define D2CommonMonsterRegionLookupTableCRaw D2CommonOrdinal10937
#define D2CommonMonsterRegionGetOffsetPairRaw D2CommonOrdinal10938
#define D2CommonMonsterUnitGetTypeLocalIndexRaw D2CommonOrdinal10939
#define D2CommonMonsterUtilTestModeMoveRaw(ecx, a1, a2) \
	(D2CommonOrdinal10940(a1, a2))
#define D2CommonMonsterRegionUnitIsDemonRaw D2CommonOrdinal10941
#define D2CommonMonsterRegionUnitIsUndeadRaw D2CommonOrdinal10942
#define D2CommonMonsterRegionUnitIsBossRaw D2CommonOrdinal10943
#define D2CommonMonsterGetSpecialPositionRaw D2CommonOrdinal10944
#define D2CommonMonsterUnitApplyChartEffectsRaw(ecx, a1) \
	(D2CommonOrdinal10945(a1))
#define D2CommonMonsterResolvePopulationIndexRaw D2CommonOrdinal10946
#define D2CommonMonsterUnitTestFieldCRaw D2CommonOrdinal10947
#define D2CommonMonsterUnitTestBossRaw D2CommonOrdinal10948
#define D2CommonMonsterRandGetComponentTableByteRaw D2CommonOrdinal10949
#define D2CommonMonsterRandGetComponentCodeRaw D2CommonOrdinal10950
#define D2CommonMonsterRandGetClassComponentCodeRaw D2CommonOrdinal10951
#define D2CommonMonsterRandRollVariationsRaw D2CommonOrdinal10952
#define D2CommonMonsterRegionCreatePopulationRaw D2CommonOrdinal10953
#define D2CommonMonsterRegionFreeLevelPopulationRaw D2CommonOrdinal10954
#define D2CommonMonsterRegionArrayGetDwordRaw D2CommonOrdinal10955
#define D2CommonMonsterRegionFindOrAddSlotRaw D2CommonOrdinal10956
#define D2CommonMonsterKillsLogSetFlagsRaw D2CommonOrdinal10957
#define D2CommonMonsterKillsLogIncrementRaw D2CommonOrdinal10958
#define D2CommonMonsterKillsLogGetCountRaw D2CommonOrdinal10959
#define D2CommonMonsterKillsLogTestFlagsRaw D2CommonOrdinal10960
#define D2CommonMonsterKillsLogClearRecordRaw D2CommonOrdinal10961
#define D2CommonMonsterKillsLogRollRecordRaw D2CommonOrdinal10962
#define D2CommonMonsterKillsLogGetStringRaw D2CommonOrdinal10963
#define D2CommonMonsterKillsLogClassifyRaw D2CommonOrdinal10964
#define D2CommonUnitFindsTestUnitNearRoomRaw D2CommonOrdinal10965
#define D2CommonUnitFindsFindNearestUnitRaw D2CommonOrdinal10966
#define D2CommonFieldTblsLoadRaw D2CommonOrdinal10967
#define D2CommonFieldTblsFreeRaw D2CommonOrdinal10968
#define D2CommonFieldTblsGetCenterRaw D2CommonOrdinal10969
#define D2CommonFieldTblsGetWidthRaw D2CommonOrdinal10970
#define D2CommonFieldTblsGetHeightRaw D2CommonOrdinal10971
#define D2CommonFieldTblsAllocPairRaw D2CommonOrdinal10972
#define D2CommonFieldTblsFreePairRaw D2CommonOrdinal10973
#define D2CommonFieldTblsSetPairRaw D2CommonOrdinal10974
#define D2CommonFieldTblsGetAtRaw D2CommonOrdinal10975
#define D2CommonFieldTblsStepRaw D2CommonOrdinal10976
#define D2CommonFieldTblsFindCandidateRaw D2CommonOrdinal10977
#define D2CommonLoggingWriteRaw D2CommonOrdinal10978
#define D2CommonLoggingWriteGameRecordRaw D2CommonOrdinal10979
#define D2CommonLoggingToggleRaw D2CommonOrdinal10980
#define D2CommonLoggingSetModeRaw D2CommonOrdinal10981
#define D2CommonProfZoneAccumulateRaw D2CommonOrdinal10982
#define D2CommonProfDumpRaw D2CommonOrdinal10983
#define D2CommonProfDumpAndResetRaw D2CommonOrdinal10984
#define D2CommonStepTestGridBitRaw D2CommonOrdinal10985
#define D2CommonQuestRecordSetBitRaw D2CommonOrdinal10986
#define D2CommonQuestRecordClearBitRaw D2CommonOrdinal10987
#define D2CommonQuestRecordClearMidBitsRaw D2CommonOrdinal10988
#define D2CommonQuestRecordNewRaw D2CommonOrdinal10989
#define D2CommonQuestRecordFreeRaw D2CommonOrdinal10990
#define D2CommonQuestRecordCopyAndSweepRaw D2CommonOrdinal10991
#define D2CommonQuestRecordCopyOutRaw D2CommonOrdinal10992
#define D2CommonMissileCreateMissileDataRaw D2CommonOrdinal10993
#define D2CommonMissileFreeMissileDataRaw D2CommonOrdinal10994
#define D2CommonMissileSetLevelRaw D2CommonOrdinal10995
#define D2CommonMissileGetLevelRaw D2CommonOrdinal10996
#define D2CommonMissileSetRangeRaw D2CommonOrdinal10997
#define D2CommonMissileGetRangeRaw D2CommonOrdinal10998
#define D2CommonMissileSetRangeLeftRaw D2CommonOrdinal10999
#define D2CommonMissileGetRangeLeftRaw D2CommonOrdinal11000
#define D2CommonMissileCalcRangeUsedRaw D2CommonOrdinal11001
#define D2CommonMissileSetIndexedByteRaw D2CommonOrdinal11002
#define D2CommonMissileGetIndexedByteRaw D2CommonOrdinal11003
#define D2CommonMissileGetClassIdRaw D2CommonOrdinal11004
#define D2CommonMissileSetLastCollideUnitRaw D2CommonOrdinal11005
#define D2CommonMissileTestLastCollideUnitRaw D2CommonOrdinal11006
#define D2CommonMissileSetSpawnClassRaw D2CommonOrdinal11007
#define D2CommonMissileGetSpawnClassRaw D2CommonOrdinal11008
#define D2CommonMissileSetSpawnRadiusRaw D2CommonOrdinal11009
#define D2CommonMissileGetSpawnRadiusRaw D2CommonOrdinal11010
#define D2CommonMissileGetHitClassRaw D2CommonOrdinal11011
#define D2CommonMissileSetActivateAtRaw D2CommonOrdinal11012
#define D2CommonMissileGetActivateAtRaw D2CommonOrdinal11013
#define D2CommonMissileGetAnimRateRaw D2CommonOrdinal11014
#define D2CommonMissileSetAnimRateRaw D2CommonOrdinal11015
#define D2CommonMissileSetScratchFirstRaw D2CommonOrdinal11016
#define D2CommonMissileGetScratchFirstRaw D2CommonOrdinal11017
#define D2CommonMissileSetScratchSecondRaw D2CommonOrdinal11018
#define D2CommonMissileGetScratchSecondRaw D2CommonOrdinal11019
#define D2CommonMissileSetStoredUnitRaw D2CommonOrdinal11020
#define D2CommonMissileGetStoredUnitRaw D2CommonOrdinal11021
#define D2CommonWaypointTestFieldRaw D2CommonOrdinal11022
#define D2CommonWaypointSetFieldRaw D2CommonOrdinal11023
#define D2CommonWaypointNewHistoryRaw D2CommonOrdinal11024
#define D2CommonWaypointFreeHistoryRaw D2CommonOrdinal11025
#define D2CommonWaypointCopyAndResetHistoryRaw D2CommonOrdinal11026
#define D2CommonWaypointExportHistoryRaw D2CommonOrdinal11027
#define D2CommonWaypointGetLevelIndexRaw D2CommonOrdinal11028
#define D2CommonWaypointFindLevelByIndexRaw D2CommonOrdinal11029
#define D2CommonGuildsNothingRaw D2CommonOrdinal11030
#define D2CommonGuildsDataNewRaw D2CommonOrdinal11031
#define D2CommonGuildsDataFreeRaw D2CommonOrdinal11032
#define D2CommonGuildsReceiveSaveRaw D2CommonOrdinal11033
#define D2CommonGuildsReceiveRosterRaw D2CommonOrdinal11034
#define D2CommonGuildsWriteSaveRaw D2CommonOrdinal11035
#define D2CommonGuildsWriteRosterRaw D2CommonOrdinal11036
#define D2CommonGuildsGetActiveRaw D2CommonOrdinal11037
#define D2CommonGuildsGetTagRaw D2CommonOrdinal11038
#define D2CommonGuildsGetNameRaw D2CommonOrdinal11039
#define D2CommonGuildsGetWebLinkRaw D2CommonOrdinal11040
#define D2CommonGuildsSetWebLinkRaw D2CommonOrdinal11041
#define D2CommonGuildsGetCharterRaw D2CommonOrdinal11042
#define D2CommonGuildsSetCharterRaw D2CommonOrdinal11043
#define D2CommonGuildsGetMembersRaw D2CommonOrdinal11044
#define D2CommonGuildsGetEmblemRaw D2CommonOrdinal11045
#define D2CommonGuildsSetEmblemRaw D2CommonOrdinal11046
#define D2CommonGuildsGetByteAfterEmblemRaw D2CommonOrdinal11047
#define D2CommonGuildsGetFlagsRaw D2CommonOrdinal11048
#define D2CommonGuildsGetCounterBRaw D2CommonOrdinal11049
#define D2CommonGuildsSetCounterBRaw D2CommonOrdinal11050
#define D2CommonGuildsDonateGoldRaw D2CommonOrdinal11051
#define D2CommonGuildsGetNextRankThresholdRaw D2CommonOrdinal11052
#define D2CommonGuildsGetCounterARaw D2CommonOrdinal11053
#define D2CommonGuildsSetCounterARaw D2CommonOrdinal11054
#define D2CommonGuildsAddCounterARaw D2CommonOrdinal11055
#define D2CommonGuildsSubtractCounterARaw D2CommonOrdinal11056
#define D2CommonGuildsGetVaultInventoryRaw D2CommonOrdinal11057
#define D2CommonGuildsSetVaultUnitIdRaw D2CommonOrdinal11058
#define D2CommonGuildsGetVaultUnitIdRaw D2CommonOrdinal11059
#define D2CommonGuildsGetVaultItemDataRaw D2CommonOrdinal11060
#define D2CommonGuildsGetTrophyCaseInventoryRaw D2CommonOrdinal11061
#define D2CommonGuildsSetTrophyCaseUnitIdRaw D2CommonOrdinal11062
#define D2CommonGuildsGetTrophyCaseUnitIdRaw D2CommonOrdinal11063
#define D2CommonGuildsGetTrophyCaseItemDataRaw D2CommonOrdinal11064
#define D2CommonGuildsSetMessageBoardUnitIdRaw D2CommonOrdinal11065
#define D2CommonGuildsGetMessageBoardUnitIdRaw D2CommonOrdinal11066
#define D2CommonGuildsPostAppendRaw D2CommonOrdinal11067
#define D2CommonGuildsPostReadRaw D2CommonOrdinal11068
#define D2CommonGuildsCopyPostBufferRaw D2CommonOrdinal11069
#define D2CommonGuildsPostReceiveRaw D2CommonOrdinal11070
#define D2CommonGuildsListGetNextRaw D2CommonOrdinal11071
#define D2CommonGuildsListFindByNameRaw D2CommonOrdinal11072
#define D2CommonGuildsMemberSnapshotRaw D2CommonOrdinal11073
#define D2CommonGuildsListNodeSetFromRecordRaw D2CommonOrdinal11074
#define D2CommonGuildsGetMemberClassRaw D2CommonOrdinal11075
#define D2CommonGuildsGetMemberRankRaw D2CommonOrdinal11076
#define D2CommonGuildsSetMemberRankRaw D2CommonOrdinal11077
#define D2CommonGuildsListRemoveByNameRaw D2CommonOrdinal11078
#define D2CommonGuildsUnpackFieldRaw D2CommonOrdinal11079
#define D2CommonGuildsGetRankRaw D2CommonOrdinal11080
#define D2CommonGuildsGetRankLevelIdRaw D2CommonOrdinal11081
#define D2CommonGuildsSetChangedRaw D2CommonOrdinal11082
#define D2CommonGuildsGetChangedRaw D2CommonOrdinal11083
#define D2CommonGuildsSetAllChangedRaw D2CommonOrdinal11084
#define D2CommonGuildsGetEmblemChangedRaw D2CommonOrdinal11085
#define D2CommonItemSavePackListRaw D2CommonOrdinal11086
#define D2CommonItemSaveUnpackRaw D2CommonOrdinal11087
#define D2CommonMissileNothingFirstRaw D2CommonOrdinal11088
#define D2CommonMissileNothingSecondRaw D2CommonOrdinal11089
#define D2CommonUnitFindsNothingFirstRaw D2CommonOrdinal11090
#define D2CommonUnitFindsNothingSecondRaw D2CommonOrdinal11091
#define D2CommonPresetTagTableCountRowRaw D2CommonOrdinal11092
#define D2CommonPresetTagTableGetCellRaw D2CommonOrdinal11093
#define D2CommonPresetMonsterRemapCountRowRaw D2CommonOrdinal11094
#define D2CommonPresetMonsterRemapGetCellRaw D2CommonOrdinal11095
#define D2CommonLvlTblsTestLevelTypeActRaw D2CommonOrdinal11096
#define D2CommonLvlTblsGetLevelTypeFileRaw D2CommonOrdinal11097
#define D2CommonDataTblsGetDifficultyLevelRaw D2CommonOrdinal11098
#define D2CommonPathGetCollisionTestMaskRaw D2CommonOrdinal11099
#define D2CommonITEMSTestUbercodeUnsetNoCheckRaw D2CommonOrdinal11100
#define D2CommonITEMSTestUbercodeUnsetRaw D2CommonOrdinal11101
#define D2CommonINVENTORYTestHasDeltaKeyRaw D2CommonOrdinal11102
#define D2CommonPathRestoreCollisionMaskRaw D2CommonOrdinal11103
#define D2CommonItemSaveGetRaw D2CommonOrdinal11104
#define D2CommonINVENTORYSoftResetRaw D2CommonOrdinal11105

#endif
