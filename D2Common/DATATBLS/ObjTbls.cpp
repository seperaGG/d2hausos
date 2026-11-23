#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\DATATBLS\\ObjTbls.cpp"
#include "../D2Common.h"
#include "D2Common.imports.h"
#include "D2Macros.h"
#include "DataTbls.h"
#include <stddef.h>

struct SHRINEDATATABLE
{
 	PTSHRINESTXT	ptShrinesTxt;
 	DWORD		nShrineCount;
};
typedef struct SHRINEDATATABLE SHRINEDATATABLE;

struct OBJECTDATATABLE
{
 	PTOBJECTSTXT	ptObjectsTxt;
 	DWORD		nObjectCount;
};
typedef struct OBJECTDATATABLE OBJECTDATATABLE;
static SHRINEDATATABLE sgtShrineDataTable = { 0 };
static OBJECTDATATABLE sgtObjectDataTable = { 0 };
PTOBJGROUPTXT gptObjGroupTxt = NULL;
int gnObjGroup = 0;

void __fastcall ObjTblsLoadObjects(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Name", EXCEL_TYPE_STRING, 63, offsetof(OBJECTSTXT, Name) },
		{ "Token", EXCEL_TYPE_STRING, 2, offsetof(OBJECTSTXT, Token) },
		{ "SpawnMax", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, SpawnMax) },
		{ "Selectable0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[0]) },
		{ "Selectable1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[1]) },
		{ "Selectable2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[2]) },
		{ "Selectable3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[3]) },
		{ "Selectable4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[4]) },
		{ "Selectable5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[5]) },
		{ "Selectable6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[6]) },
		{ "Selectable7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Selectable[7]) },
		{ "TrapProb", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, TrapProb) },
		{ "SizeX", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, SizeX) },
		{ "SizeY", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, SizeY) },
		{ "FrameCnt0", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[0]) },
		{ "FrameCnt1", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[1]) },
		{ "FrameCnt2", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[2]) },
		{ "FrameCnt3", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[3]) },
		{ "FrameCnt4", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[4]) },
		{ "FrameCnt5", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[5]) },
		{ "FrameCnt6", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[6]) },
		{ "FrameCnt7", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameCnt[7]) },
		{ "FrameDelta0", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[0]) },
		{ "FrameDelta1", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[1]) },
		{ "FrameDelta2", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[2]) },
		{ "FrameDelta3", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[3]) },
		{ "FrameDelta4", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[4]) },
		{ "FrameDelta5", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[5]) },
		{ "FrameDelta6", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[6]) },
		{ "FrameDelta7", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, FrameDelta[7]) },
		{ "CycleAnim0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[0]) },
		{ "CycleAnim1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[1]) },
		{ "CycleAnim2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[2]) },
		{ "CycleAnim3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[3]) },
		{ "CycleAnim4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[4]) },
		{ "CycleAnim5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[5]) },
		{ "CycleAnim6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[6]) },
		{ "CycleAnim7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CycleAnim[7]) },
		{ "Lit0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[0]) },
		{ "Lit1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[1]) },
		{ "Lit2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[2]) },
		{ "Lit3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[3]) },
		{ "Lit4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[4]) },
		{ "Lit5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[5]) },
		{ "Lit6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[6]) },
		{ "Lit7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lit[7]) },
		{ "BlocksLight0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[0]) },
		{ "BlocksLight1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[1]) },
		{ "BlocksLight2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[2]) },
		{ "BlocksLight3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[3]) },
		{ "BlocksLight4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[4]) },
		{ "BlocksLight5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[5]) },
		{ "BlocksLight6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[6]) },
		{ "BlocksLight7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksLight[7]) },
		{ "HasCollision0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[0]) },
		{ "HasCollision1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[1]) },
		{ "HasCollision2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[2]) },
		{ "HasCollision3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[3]) },
		{ "HasCollision4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[4]) },
		{ "HasCollision5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[5]) },
		{ "HasCollision6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[6]) },
		{ "HasCollision7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HasCollision[7]) },
		{ "IsAttackable0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, IsAttackable0) },
		{ "Start0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[0]) },
		{ "Start1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[1]) },
		{ "Start2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[2]) },
		{ "Start3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[3]) },
		{ "Start4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[4]) },
		{ "Start5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[5]) },
		{ "Start6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[6]) },
		{ "Start7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Start[7]) },
		{ "OrderFlag0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[0]) },
		{ "OrderFlag1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[1]) },
		{ "OrderFlag2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[2]) },
		{ "OrderFlag3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[3]) },
		{ "OrderFlag4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[4]) },
		{ "OrderFlag5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[5]) },
		{ "OrderFlag6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[6]) },
		{ "OrderFlag7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OrderFlag[7]) },
		{ "EnvEffect", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, EnvEffect) },
		{ "IsDoor", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, IsDoor) },
		{ "BlocksVis", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlocksVis) },
		{ "Orientation", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Orientation) },
		{ "Trans", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Trans) },
		{ "PreOperate", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, PreOperate) },
		{ "Mode0", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[0]) },
		{ "Mode1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[1]) },
		{ "Mode2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[2]) },
		{ "Mode3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[3]) },
		{ "Mode4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[4]) },
		{ "Mode5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[5]) },
		{ "Mode6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[6]) },
		{ "Mode7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Mode[7]) },
		{ "Xoffset", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Xoffset) },
		{ "Yoffset", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Yoffset) },
		{ "Draw", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Draw) },
		{ "HD", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, HD) },
		{ "TR", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, TR) },
		{ "LG", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, LG) },
		{ "RA", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, RA) },
		{ "LA", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, LA) },
		{ "RH", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, RH) },
		{ "LH", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, LH) },
		{ "SH", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, SH) },
		{ "S1", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[0]) },
		{ "S2", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[1]) },
		{ "S3", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[2]) },
		{ "S4", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[3]) },
		{ "S5", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[4]) },
		{ "S6", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[5]) },
		{ "S7", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[6]) },
		{ "S8", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, S[7]) },
		{ "TotalPieces", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, TotalPieces) },
		{ "XSpace", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, XSpace) },
		{ "YSpace", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, YSpace) },
		{ "Red", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Red) },
		{ "Green", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Green) },
		{ "Blue", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Blue) },
		{ "SubClass", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, SubClass) },
		{ "NameOffset", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, NameOffset) },
		{ "MonsterOK", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, MonsterOK) },
		{ "OperateRange", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OperateRange) },
		{ "ShrineFunction", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, ShrineFunction) },
		{ "Act", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Act) },
		{ "Restore", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Restore) },
		{ "RestoreVirgins", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, RestoreVirgins) },
		{ "Parm0", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[0]) },
		{ "Parm1", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[1]) },
		{ "Parm2", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[2]) },
		{ "Parm3", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[3]) },
		{ "Parm4", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[4]) },
		{ "Parm5", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[5]) },
		{ "Parm6", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[6]) },
		{ "Parm7", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Parm[7]) },
		{ "Lockable", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Lockable) },
		{ "Gore", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Gore) },
		{ "Sync", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Sync) },
		{ "Flicker", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Flicker) },
		{ "Beta", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Beta) },
		{ "nTgtFX", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, nTgtFX) },
		{ "nTgtFY", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, nTgtFY) },
		{ "nTgtBX", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, nTgtBX) },
		{ "nTgtBY", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, nTgtBY) },
		{ "Damage", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Damage) },
		{ "CollisionSubst", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, CollisionSubst) },
		{ "Left", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Left) },
		{ "Top", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Top) },
		{ "Width", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Width) },
		{ "Height", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, Height) },
		{ "OperateFn", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OperateFn) },
		{ "PopulateFn", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, PopulateFn) },
		{ "ClientFn", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, bClientProcessFunction) },
		{ "InitFn", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, InitFn) },
		{ "Overlay", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, Overlay) },
		{ "BlockMissile", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, BlockMissile) },
		{ "DrawUnder", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, DrawUnder) },
		{ "OpenWarp", EXCEL_TYPE_BYTE, 0, offsetof(OBJECTSTXT, OpenWarp) },
		{ "AutoMap", EXCEL_TYPE_DWORD, 0, offsetof(OBJECTSTXT, AutoMap) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	DWORD i;
	DWORD j;
	PTOBJECTSTXT ptObjects;

	ptObjects = (PTOBJECTSTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(OBJECTSTXT));
	sgtObjectDataTable.nObjectCount = nRows;
	sgtObjectDataTable.ptObjectsTxt = ptObjects;
	for (i = 0; i < (DWORD)nRows; i++)
	{
		PTOBJECTSTXT ptObject = &sgtObjectDataTable.ptObjectsTxt[i];
		memset(ptObject->wszName, 0, 128);
		(void)D2LangUnicodestrncpy(ptObject->wszName, D2LangStrGetStringByKey(ptObject->Name), 64);
		for (j = 0; j < arrsize(ptObject->FrameCnt); j++)
			ptObject->FrameCnt[j] <<= 8;
	}
}

PTOBJECTSTXT __stdcall ObjTblsGetObject(DWORD nIndex)
{
#line 233
	D2ASSERT(nIndex < sgtObjectDataTable.nObjectCount);
	return &sgtObjectDataTable.ptObjectsTxt[nIndex];
}

void __fastcall ObjTblsFreeObjects(void)
{
	DataTblsFreeBlock(sgtObjectDataTable.ptObjectsTxt);
	sgtObjectDataTable.ptObjectsTxt = NULL;
	sgtObjectDataTable.nObjectCount = 0;
}

void __fastcall ObjTblsFreeObjGroup(void)
{
	DataTblsFreeBlock(gptObjGroupTxt);
	gptObjGroupTxt = NULL;
	gnObjGroup = 0;
}

PTOBJGROUPTXT __fastcall ObjTblsLoadObjGroup(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "ID0", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[0]) },
		{ "DENSITY0", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[0]) },
		{ "PROB0", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[0]) },
		{ "ID1", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[1]) },
		{ "DENSITY1", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[1]) },
		{ "PROB1", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[1]) },
		{ "ID2", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[2]) },
		{ "DENSITY2", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[2]) },
		{ "PROB2", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[2]) },
		{ "ID3", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[3]) },
		{ "DENSITY3", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[3]) },
		{ "PROB3", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[3]) },
		{ "ID4", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[4]) },
		{ "DENSITY4", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[4]) },
		{ "PROB4", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[4]) },
		{ "ID5", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[5]) },
		{ "DENSITY5", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[5]) },
		{ "PROB5", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[5]) },
		{ "ID6", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[6]) },
		{ "DENSITY6", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[6]) },
		{ "PROB6", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[6]) },
		{ "ID7", EXCEL_TYPE_DWORD, 0, offsetof(OBJGROUPTXT, ID[7]) },
		{ "DENSITY7", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, DENSITY[7]) },
		{ "PROB7", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, PROB[7]) },
		{ "SHRINES", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, SHRINES) },
		{ "WELLS", EXCEL_TYPE_BYTE, 0, offsetof(OBJGROUPTXT, WELLS) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTOBJGROUPTXT ptObjGroup;

	ptObjGroup = (PTOBJGROUPTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(OBJGROUPTXT));
	gptObjGroupTxt = ptObjGroup;
	gnObjGroup = nRows;
	return ptObjGroup;
}

PTOBJGROUPTXT __stdcall ObjTblsGetObjGroup(DWORD nIndex)
{
	if ((DWORD)gnObjGroup > nIndex)
		return &gptObjGroupTxt[nIndex];
	return NULL;
}

PTSHRINESTXT __fastcall ObjTblsLoadShrines(void *pBuffer, int nBufferSize)
{
	EXCELCOLDESC tDesc[] =
	{
		{ "Code", EXCEL_TYPE_BYTE, 0, offsetof(SHRINESTXT, bCode) },
		{ "Arg0", EXCEL_TYPE_DWORD, 0, offsetof(SHRINESTXT, Arg0) },
		{ "Arg1", EXCEL_TYPE_DWORD, 0, offsetof(SHRINESTXT, Arg1) },
		{ "Duration in frames", EXCEL_TYPE_DWORD, 0, offsetof(SHRINESTXT, nDurationInFrames) },
		{ "reset time in minutes", EXCEL_TYPE_BYTE, 0, offsetof(SHRINESTXT, bResetTimeInMinutes) },
		{ "rarity", EXCEL_TYPE_BYTE, 0, offsetof(SHRINESTXT, rarity) },
		{ "view name", EXCEL_TYPE_STRING, 31, offsetof(SHRINESTXT, szViewName) },
		{ "niftyphrase", EXCEL_TYPE_STRING, 127, offsetof(SHRINESTXT, niftyphrase) },
		{ "effectclass", EXCEL_TYPE_BYTE, 0, offsetof(SHRINESTXT, effectclass) },
		{ "LevelMin", EXCEL_TYPE_DWORD, 0, offsetof(SHRINESTXT, LevelMin) },
		{ "end", EXCEL_TYPE_END, 0, 0 },
	};
	int nRows;
	PTSHRINESTXT ptShrines;

	ptShrines = (PTSHRINESTXT)DataTblsAllocAndTokenize(tDesc, (char *)pBuffer, nBufferSize, &nRows,
		sizeof(SHRINESTXT));
	sgtShrineDataTable.ptShrinesTxt = ptShrines;
	sgtShrineDataTable.nShrineCount = nRows;
	return ptShrines;
}

PTSHRINESTXT __stdcall ObjTblsGetShrine(DWORD nIndex)
{
#line 389
	D2ASSERT(nIndex < sgtShrineDataTable.nShrineCount);
	return &sgtShrineDataTable.ptShrinesTxt[nIndex];
}

int __fastcall ObjTblsGetShrineCount(void)
{
	return sgtShrineDataTable.nShrineCount;
}

void __fastcall ObjTblsFreeShrines(void)
{
	DataTblsFreeBlock(sgtShrineDataTable.ptShrinesTxt);
	sgtShrineDataTable.ptShrinesTxt = NULL;
	sgtShrineDataTable.nShrineCount = 0;
}
