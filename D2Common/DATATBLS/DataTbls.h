#ifndef D2RE_DATATBLS_H
#define D2RE_DATATBLS_H

#include <windows.h>

struct HD2ARCHIVE__;
typedef struct HD2ARCHIVE__ D2ARCHIVE;
typedef D2ARCHIVE *HD2ARCHIVE;

struct LVLTYPESTXT
{
 	char		File[32][60];
 	BYTE		Act;
};
typedef struct LVLTYPESTXT LVLTYPESTXT, *PTLVLTYPESTXT;

struct EXCELCOLDESC
{
 	const char	*pszName;
 	int			nType;
 	int			nLength;
 	int			nOffset;
};
typedef struct EXCELCOLDESC EXCELCOLDESC, *PTEXCELCOLDESC;

#define EXCEL_TYPE_END		0
#define EXCEL_TYPE_STRING	1
#define EXCEL_TYPE_DWORD	2
#define EXCEL_TYPE_WORD		3
#define EXCEL_TYPE_BYTE		4
#define EXCEL_TYPE_BYTE_OR	5
#define EXCEL_TYPE_OPTBYTE	6
#define EXCEL_TYPE_OPTSTRING	7
#define EXCEL_TYPE_CODE		9

#define NUM_DIFFICULTY_LEVELS	3
#define NUM_WARP_TYPES			71
#define NUM_STATS				177

struct CHARSTATSTXT
{
 	WORD		wszClassName[16];
 	char		Class[16];
 	BYTE		str;
 	BYTE		dex;
 	BYTE		Int;
 	BYTE		vit;
 	BYTE		hpadd;
 	BYTE		PercentStr;
 	BYTE		PercentInt;
 	BYTE		PercentDex;
 	BYTE		PercentVit;
 	BYTE		ManaRegen;
 	BYTE		unobserved_3a[2];
 	int			ToHitFactor;
 	BYTE		WalkVelocity;
 	BYTE		RunVelocity;
 	BYTE		RunDrain;
 	BYTE		LifePerLevel;
 	BYTE		StaminaPerLevel;
 	BYTE		ManaPerLevel;
 	BYTE		LifePerVitality;
 	BYTE		StaminaPerVitality;
 	BYTE		ManaPerMagic;
 	BYTE		BlockFactor;
 	BYTE		unobserved_4a[2];
 	DWORD		basewclass;
 	int			StartSkill;
 	DWORD		item1;
 	BYTE		item1loc;
 	BYTE		item1count;
 	BYTE		unobserved_5a[2];
 	DWORD		item2;
 	BYTE		item2loc;
 	BYTE		item2count;
 	BYTE		unobserved_62[2];
 	DWORD		item3;
 	BYTE		item3loc;
 	BYTE		item3count;
 	BYTE		unobserved_6a[2];
 	DWORD		item4;
 	BYTE		item4loc;
 	BYTE		item4count;
 	BYTE		unobserved_72[2];
 	DWORD		item5;
 	BYTE		item5loc;
 	BYTE		item5count;
 	BYTE		unobserved_7a[2];
 	DWORD		item6;
 	BYTE		item6loc;
 	BYTE		item6count;
 	BYTE		unobserved_82[2];
 	DWORD		item7;
 	BYTE		item7loc;
 	BYTE		item7count;
 	BYTE		unobserved_8a[2];
 	DWORD		item8;
 	BYTE		item8loc;
 	BYTE		item8count;
 	BYTE		unobserved_92[2];
 	DWORD		item9;
 	BYTE		item9loc;
 	BYTE		item9count;
 	BYTE		unobserved_9a[2];
 	DWORD		item10;
 	BYTE		item10loc;
 	BYTE		item10count;
 	BYTE		unobserved_a2[2];
};
typedef struct CHARSTATSTXT CHARSTATSTXT, *PTCHARSTATSTXT;

#define NUM_PLAYERCLASS		5

struct CHARTEMPLATETXT
{
 	char		Name[30];
 	char		Class[4];
 	BYTE		unobserved_22[2];
 	int			nClassId;
 	BYTE		level;
 	BYTE		act;
 	BYTE		str;
 	BYTE		dex;
 	BYTE		Int;
 	BYTE		vit;
 	BYTE		Mana;
 	BYTE		Hitpoints;
 	BYTE		ManaRegenBonus;
 	BYTE		Velocity;
 	BYTE		AttackRate;
 	BYTE		OtherRate;
 	int			RightSkill;
 	int			Skill[9];
 	int			SkillLevel[9];
 	DWORD		item1;
 	BYTE		item1loc;
 	BYTE		item1count;
 	BYTE		unobserved_86[2];
 	DWORD		item2;
 	BYTE		item2loc;
 	BYTE		item2count;
 	BYTE		unobserved_8e[2];
 	DWORD		item3;
 	BYTE		item3loc;
 	BYTE		item3count;
 	BYTE		unobserved_96[2];
 	DWORD		item4;
 	BYTE		item4loc;
 	BYTE		item4count;
 	BYTE		unobserved_9e[2];
 	DWORD		item5;
 	BYTE		item5loc;
 	BYTE		item5count;
 	BYTE		unobserved_a6[2];
 	DWORD		item6;
 	BYTE		item6loc;
 	BYTE		item6count;
 	BYTE		unobserved_ae[2];
 	DWORD		item7;
 	BYTE		item7loc;
 	BYTE		item7count;
 	BYTE		unobserved_b6[2];
 	DWORD		item8;
 	BYTE		item8loc;
 	BYTE		item8count;
 	BYTE		unobserved_be[2];
 	DWORD		item9;
 	BYTE		item9loc;
 	BYTE		item9count;
 	BYTE		unobserved_c6[2];
 	DWORD		item10;
 	BYTE		item10loc;
 	BYTE		item10count;
 	BYTE		unobserved_ce[2];
 	DWORD		item11;
 	BYTE		item11loc;
 	BYTE		item11count;
 	BYTE		unobserved_d6[2];
 	DWORD		item12;
 	BYTE		item12loc;
 	BYTE		item12count;
 	BYTE		unobserved_de[2];
 	DWORD		item13;
 	BYTE		item13loc;
 	BYTE		item13count;
 	BYTE		unobserved_e6[2];
 	DWORD		item14;
 	BYTE		item14loc;
 	BYTE		item14count;
 	BYTE		unobserved_ee[2];
 	DWORD		item15;
 	BYTE		item15loc;
 	BYTE		item15count;
 	BYTE		unobserved_f6[2];
};
typedef struct CHARTEMPLATETXT CHARTEMPLATETXT, *PTCHARTEMPLATETXT;

struct SKILLSTXT
{
 	DWORD		dwSkillId;
 	char		skill[31];
 	char		charclass[31];
 	BYTE		unobserved_42[2];
 	int			ClassReq;
 	BYTE		AttackRank;
 	BYTE		unobserved_49[3];
 	DWORD		itemclass[6];
 	char		anim[8];
 	char		monanim[8];
 	int			dwAnimMode;
 	int			dwMonAnimMode;
 	BYTE		seqnum;
 	BYTE		Durability;
 	BYTE		Shiver;
 	BYTE		unobserved_7f[1];
 	int			UseAttackRate;
 	BYTE		LineOfSight;
 	BYTE		unobserved_85[3];
 	int			TargetableOnly;
 	int			SearchEnemyXY;
 	int			SearchMonsterNear;
 	int			SelectDead;
 	int			SearchOpenXY;
 	BYTE		TargetPet;
 	BYTE		TargetAlly;
 	char		range[8];
 	BYTE		unobserved_a6[2];
 	int			dwRangeType;
 	int			AttackNoMana;
 	BYTE		reqlevel;
 	BYTE		reqstr;
 	BYTE		reqdex;
 	BYTE		reqint;
 	BYTE		reqvit;
 	char		reqskill1[31];
 	struct SKILLSTXT	*ptReqSkill1;
 	char		reqskill2[31];
 	BYTE		unobserved_f7[1];
 	struct SKILLSTXT	*ptReqSkill2;
 	char		reqskill3[31];
 	BYTE		unobserved_11b[1];
 	struct SKILLSTXT	*ptReqSkill3;
 	int			state1;
 	int			state2;
 	int			state3;
 	BYTE		skillpage;
 	BYTE		skillrow;
 	BYTE		skillcolumn;
 	BYTE		iconcel;
 	BYTE		leftskl;
 	BYTE		unobserved_131[3];
 	int			manashift;
 	int			mana;
 	int			lvlmana;
 	BYTE		Interrupt;
 	BYTE		InTown;
 	BYTE		Periodic;
 	BYTE		Passive;
 	int			Param[6];
 	int			InGame;
 	int			Open;
 	int			Beta;
 	int			dwNeedsShield;
 	int			dwNotBow;
 	BYTE		unobserved_170[4];
 	int			ToHit;
 	int			LevToHit;
 	BYTE		HitShift;
 	BYTE		SrcDam;
 	BYTE		unobserved_17e[2];
 	int			MinDam;
 	int			MaxDam;
 	int			LevDam;
 	BYTE		EType;
 	BYTE		unobserved_18d[3];
 	int			EMin;
 	int			EMax;
 	int			ELev;
 	int			ELen;
 	int			ELevLen;
};
typedef struct SKILLSTXT SKILLSTXT, *PTSKILLSTXT;

struct EXPERIENCETXT
{
 	int			Amazon;
 	int			Sorceress;
 	int			Necromancer;
 	int			Paladin;
 	int			Barbarian;
};
typedef struct EXPERIENCETXT EXPERIENCETXT, *PTEXPERIENCETXT;

struct DIFFICULTYLEVELSTXT
{
 	int			ResistPenalty;
 	int			DeathExpPenalty;
 	int			UberCodeOddsNormal;
 	int			UberCodeOddsGood;
 	int			MonsterSkillBonus;
 	int			MonsterFreezeDivisor;
 	int			MonsterColdDivisor;
 	int			AiCurseDivisor;
};
typedef struct DIFFICULTYLEVELSTXT DIFFICULTYLEVELSTXT, *PTDIFFICULTYLEVELSTXT;

struct ANIMDATAREC
{
 	char		szCofName[8];
 	DWORD		dwFrames;
 	DWORD		dwSpeed;
 	BYTE		abFrameEvents[144];
};
typedef struct ANIMDATAREC ANIMDATAREC, *PTANIMDATAREC;
struct ANIMDATABUCKET
{
 	DWORD		dwNumRecords;
 	ANIMDATAREC	atRecords[1];
};
typedef struct ANIMDATABUCKET ANIMDATABUCKET, *PTANIMDATABUCKET;

struct ANIMDATATBL
{
 	void		*pBuffer;
 	PTANIMDATABUCKET	aptBuckets[256];
 	ANIMDATAREC	tDefaultRecord;
};
typedef struct ANIMDATATBL ANIMDATATBL, *PTANIMDATATBL;

struct TOKENTXT
{
 	char		name[32];
 	DWORD		token;
 	BYTE		DT_Dir;
 	BYTE		NU_Dir;
 	BYTE		WL_Dir;
 	BYTE		GH_Dir;
 	BYTE		A1_Dir;
 	BYTE		A2_Dir;
 	BYTE		BL_Dir;
 	BYTE		SC_Dir;
 	BYTE		S1_Dir;
 	BYTE		S2_Dir;
 	BYTE		S3_Dir;
 	BYTE		S4_Dir;
 	BYTE		DD_Dir;
 	BYTE		KB_Dir;
 	BYTE		SQ_Dir;
 	BYTE		RN_Dir;
};
typedef struct TOKENTXT TOKENTXT, *PTTOKENTXT;

struct MONITEMPERCENTTXT
{
 	BYTE		HeartPercent;
 	BYTE		BodyPartPercent;
 	BYTE		TreasureClassPercent;
 	BYTE		ComponentPercent;
};
typedef struct MONITEMPERCENTTXT MONITEMPERCENTTXT, *PTMONITEMPERCENTTXT;

struct LVLWARPTXT
{
 	int			SelectX;
 	int			SelectY;
 	int			SelectDX;
 	int			SelectDY;
 	int			ExitWalkX;
 	int			ExitWalkY;
 	int			OffsetX;
 	int			OffsetY;
 	int			LitVersion;
};
typedef struct LVLWARPTXT LVLWARPTXT, *PTLVLWARPTXT;

struct LVLMAZETXT
{
 	int			Level;
 	int			Rooms;
 	int			SizeX;
 	int			SizeY;
 	int			Merge;
};
typedef struct LVLMAZETXT LVLMAZETXT, *PTLVLMAZETXT;

struct ITEMRATIOTXT
{
 	int			Unique;
 	int			UniqueDivisor;
 	int			Rare;
 	int			RareDivisor;
 	int			Set;
 	int			SetDivisor;
 	int			Magic;
 	int			MagicDivisor;
 	int			HiQuality;
 	int			HiQualityDivisor;
 	int			Normal;
 	int			NormalDivisor;
};
typedef struct ITEMRATIOTXT ITEMRATIOTXT, *PTITEMRATIOTXT;

struct ITEMSTATCOSTTXT
{
 	int			divide;
 	int			multiply;
 	int			add;
 	int			ItemSpecific;
};
typedef struct ITEMSTATCOSTTXT ITEMSTATCOSTTXT, *PTITEMSTATCOSTTXT;

struct SHRINESTXT
{
 	BYTE		bCode;
 	BYTE		unobserved_1[3];
 	int			Arg0;
 	int			Arg1;
 	int			nDurationInFrames;
 	BYTE		bResetTimeInMinutes;
 	BYTE		rarity;
 	char		szViewName[32];
 	char		niftyphrase[128];
 	BYTE		effectclass;
 	BYTE		unobserved_b3[1];
 	int			LevelMin;
};
typedef struct SHRINESTXT SHRINESTXT, *PTSHRINESTXT;

struct ITEMMODTXT
{
 	char		code[8];
 	int			nModOffset;
 	int			min;
 	int			max;
};
typedef struct ITEMMODTXT ITEMMODTXT, *PTITEMMODTXT;

struct ITEMSTXT
{
 	BYTE		completed;
 	BYTE		rarity;
 	BYTE		level;
 	BYTE		mindam;
 	BYTE		maxdam;
 	BYTE		minmisdam;
 	BYTE		maxmisdam;
 	BYTE		bTwoHandMinDam;
 	BYTE		bTwoHandMaxDam;
 	BYTE		rangeadder;
 	BYTE		strbonus;
 	BYTE		dexbonus;
 	int			minac;
 	int			maxac;
 	BYTE		reqstr;
 	BYTE		reqdex;
 	BYTE		absorbs;
 	BYTE		invwidth;
 	BYTE		invheight;
 	BYTE		block;
 	BYTE		durability;
 	BYTE		nodurability;
 	BYTE		missile;
 	BYTE		replenish;
 	BYTE		special;
 	BYTE		component;
 	BYTE		bodyloc1;
 	BYTE		bodyloc2;
 	BYTE		rArm;
 	BYTE		lArm;
 	BYTE		torso;
 	BYTE		legs;
 	BYTE		rspad;
 	BYTE		lspad;
 	BYTE		bTwoHanded;
 	BYTE		useable;
 	BYTE		type;
 	BYTE		subtype;
 	BYTE		sound;
 	BYTE		unique;
 	BYTE		quest;
 	BYTE		transparent;
 	BYTE		transtbl;
 	BYTE		throwable;
 	BYTE		unobserved_32[1];
 	BYTE		lightradius;
 	BYTE		belt;
 	BYTE		autobelt;
 	BYTE		quivered;
 	BYTE		stackable;
 	BYTE		spawnable;
 	BYTE		unobserved_39[1];
 	WORD		missiletype;
 	BYTE		spellicon;
 	BYTE		durwarning;
 	BYTE		qntwarning;
 	BYTE		hasinv;
 	BYTE		gemsockets;
 	BYTE		Transmogrify;
 	BYTE		TMogMin;
 	BYTE		TMogMax;
 	BYTE		bWeaponGroup;
 	BYTE		bHitClass;
 	BYTE		bOneOrTwoHanded;
 	BYTE		Transform;
 	BYTE		InvTrans;
 	BYTE		weapongroupenum;
 	BYTE		weaponspeedenum;
 	BYTE		unobserved_4b[1];
 	int			speed;
 	int			bitfield1;
 	int			cost;
 	int			minstack;
 	int			maxstack;
 	int			spelloffset;
 	int			gemoffset;
 	DWORD		code;
 	DWORD		alternategfx;
 	DWORD		ubercode;
 	DWORD		wclass;
 	DWORD		dwTwoHandedWClass;
 	char		group[16];
 	DWORD		TMogType;
 	char		name[64];
 	WORD		wszName[64];
 	char		flippyfile[32];
 	char		invfile[32];
 	char		uniqueinvfile[32];
 	DWORD		BetterGem;
 	BYTE		SkipName;
 	BYTE		Min[14];
 	BYTE		Max[14];
 	BYTE		MagicMin[14];
 	BYTE		MagicMax[14];
 	BYTE		MagicLvl[14];
 	BYTE		NightmareUpgrade;
 	BYTE		HellUpgrade;
 	BYTE		unobserved_1fd[3];
};
typedef struct ITEMSTXT ITEMSTXT, *PTITEMSTXT;

struct QUALITYITEMSTXT
{
 	BYTE		armor;
 	BYTE		weapon;
 	BYTE		shield;
 	BYTE		scepter;
 	BYTE		wand;
 	BYTE		staff;
 	BYTE		bow;
 	BYTE		boots;
 	BYTE		gloves;
 	BYTE		belt;
 	BYTE		nummods;
 	BYTE		unobserved_b[1];
 	ITEMMODTXT	mod[2];
 	char		effect1[32];
 	char		effect2[32];
 	WORD		wEffect1String;
 	WORD		wEffect2String;
};
typedef struct QUALITYITEMSTXT QUALITYITEMSTXT, *PTQUALITYITEMSTXT;

struct GEMSTXT
{
 	char		name[32];
 	DWORD		code;
 	WORD		wCodeString;
 	BYTE		nummods;
 	BYTE		transform;
 	ITEMMODTXT	weaponmod[3];
 	ITEMMODTXT	helmmod[3];
 	ITEMMODTXT	shieldmod[3];
};
typedef struct GEMSTXT GEMSTXT, *PTGEMSTXT;

struct BOOKSTXT
{
 	BYTE		completed;
 	BYTE		spellicon;
 	BYTE		unobserved_2[2];
 	int			spelloffset;
 	int			skillscrolloffset;
 	int			skillbookoffset;
 	int			basecost;
 	int			costpercharge;
 	DWORD		scrollspellcode;
 	DWORD		bookspellcode;
 	char		name[32];
 	WORD		wNameString;
 	BYTE		unobserved_42[2];
};
typedef struct BOOKSTXT BOOKSTXT, *PTBOOKSTXT;

struct LOWQUALITYITEMSTXT
{
 	char		Name[32];
 	WORD		wszName[64];
};
typedef struct LOWQUALITYITEMSTXT LOWQUALITYITEMSTXT, *PTLOWQUALITYITEMSTXT;

struct GAMBLETXT
{
 	DWORD		code;
};
typedef struct GAMBLETXT GAMBLETXT, *PTGAMBLETXT;

struct BELTBOXTXT
{
 	int			left;
 	int			right;
 	int			top;
 	int			bottom;
};
typedef struct BELTBOXTXT BELTBOXTXT;
struct BELTSTXT
{
 	BYTE		unobserved_0[4];
 	BYTE		numboxes;
 	BYTE		unobserved_5[3];
 	BELTBOXTXT	box[16];
};
typedef struct BELTSTXT BELTSTXT, *PTBELTSTXT;

struct UNIQUENAMETXT
{
 	char		Name[60];
 	WORD		wszName[60];
 	BYTE		MonType[36];
 	BYTE		unobserved_d8[374];
};
typedef struct UNIQUENAMETXT UNIQUENAMETXT, *PTUNIQUENAMETXT;

struct OBJECTSTXT
{
 	char		Name[64];
 	WORD		wszName[64];
 	char		Token[3];
 	BYTE		SpawnMax;
 	BYTE		Selectable[8];
 	BYTE		TrapProb;
 	BYTE		unobserved_cd[3];
 	int			SizeX;
 	int			SizeY;
 	int			FrameCnt[8];
 	int			FrameDelta[8];
 	BYTE		CycleAnim[8];
 	BYTE		Lit[8];
 	BYTE		BlocksLight[8];
 	BYTE		HasCollision[8];
 	BYTE		IsAttackable0;
 	BYTE		Start[8];
 	BYTE		OrderFlag[8];
 	BYTE		EnvEffect;
 	BYTE		IsDoor;
 	BYTE		BlocksVis;
 	BYTE		Orientation;
 	BYTE		PreOperate;
 	BYTE		Trans;
 	BYTE		Mode[8];
 	BYTE		unobserved_157[1];
 	int			Xoffset;
 	int			Yoffset;
 	BYTE		Draw;
 	BYTE		HD;
 	BYTE		TR;
 	BYTE		LG;
 	BYTE		RA;
 	BYTE		LA;
 	BYTE		RH;
 	BYTE		LH;
 	BYTE		SH;
 	BYTE		S[8];
 	BYTE		TotalPieces;
 	BYTE		XSpace;
 	BYTE		YSpace;
 	BYTE		Red;
 	BYTE		Green;
 	BYTE		Blue;
 	BYTE		SubClass;
 	int			NameOffset;
 	BYTE		unobserved_17c[1];
 	BYTE		MonsterOK;
 	BYTE		OperateRange;
 	BYTE		ShrineFunction;
 	BYTE		Act;
 	BYTE		Lockable;
 	BYTE		Gore;
 	BYTE		Restore;
 	BYTE		RestoreVirgins;
 	BYTE		Sync;
 	BYTE		Flicker;
 	BYTE		unobserved_187[1];
 	int			Parm[8];
 	BYTE		nTgtFX;
 	BYTE		nTgtFY;
 	BYTE		nTgtBX;
 	BYTE		nTgtBY;
 	BYTE		Damage;
 	BYTE		CollisionSubst;
 	BYTE		unobserved_1ae[2];
 	int			Left;
 	int			Top;
 	int			Width;
 	int			Height;
 	BYTE		Beta;
 	BYTE		InitFn;
 	BYTE		PopulateFn;
 	BYTE		OperateFn;
 	BYTE		bClientProcessFunction;
 	BYTE		Overlay;
 	BYTE		BlockMissile;
 	BYTE		DrawUnder;
 	BYTE		OpenWarp;
 	BYTE		unobserved_1c9[3];
 	int			AutoMap;
};
typedef struct OBJECTSTXT OBJECTSTXT, *PTOBJECTSTXT;

struct OBJGROUPTXT
{
 	int			ID[8];
 	BYTE		DENSITY[8];
 	BYTE		PROB[8];
 	BYTE		SHRINES;
 	BYTE		WELLS;
 	BYTE		unobserved_32[2];
};
typedef struct OBJGROUPTXT OBJGROUPTXT, *PTOBJGROUPTXT;

struct OVERLAYTXT
{
 	char		Filename[64];
 	int			Frames;
 	BYTE		PreDraw;
 	BYTE		unobserved_45[3];
 	int			nOneOfN;
 	BYTE		Dir;
 	BYTE		Open;
 	BYTE		Beta;
 	BYTE		unobserved_4f[1];
 	int			Xoffset;
 	int			Yoffset;
 	int			Height1;
 	int			Height2;
 	int			Height3;
 	int			Height4;
 	int			AnimRate;
 	int			InitRadius;
 	int			Radius;
 	int			LoopWaitTime;
 	BYTE		Trans;
 	BYTE		Red;
 	BYTE		Green;
 	BYTE		Blue;
 	BYTE		NumDirections;
 	BYTE		LocalBlood;
 	BYTE		unobserved_7e[2];
};
typedef struct OVERLAYTXT OVERLAYTXT, *PTOVERLAYTXT;

#define NUM_OVERLAY_CLASSES		190

struct LVLPRESTTXT
{
 	int			Def;
 	int			LevelId;
 	int			Populate;
 	int			Logicals;
 	int			Outdoors;
 	int			Animate;
 	int			KillEdge;
 	int			FillBlanks;
 	int			nAnimStep;
 	int			SizeX;
 	int			SizeY;
 	int			AutoMap;
 	int			Scan;
 	int			Pops;
 	int			PopPad;
 	int			Files;
 	char		File[6][60];
 	int			Dt1Mask;
};
typedef struct LVLPRESTTXT LVLPRESTTXT, *PTLVLPRESTTXT;

#define NUM_PRESET_DEFS		863

struct LVLSUBTXT
{
 	int			Type;
 	char		File[60];
 	int			CheckAll;
 	int			BordType;
 	int			Dt1Mask;
 	int			GridSize;
 	struct PRESETDS1	*ptFile;
 	struct DRLGGRID	atOrientations[4];
 	struct DRLGGRID	atWalls[4];
 	struct DRLGGRID	tFloor;
 	struct DRLGGRID	tShadows;
 	int			Prob[5];
 	int			Trials[5];
 	int			Max[5];
};
typedef struct LVLSUBTXT LVLSUBTXT, *PTLVLSUBTXT;

struct AUTOMAPTXT
{
 	char		LevelName[16];
 	char		TileName[8];
 	BYTE		Style;
 	BYTE		StartSequence;
 	BYTE		EndSequence;
 	BYTE		unobserved_1b[1];
 	int			Cel[4];
};
typedef struct AUTOMAPTXT AUTOMAPTXT, *PTAUTOMAPTXT;

struct AUTOMAPSUB
{
 	int			nLevelType;
 	int			nTile;
 	BYTE		bStyle;
 	BYTE		bStartSequence;
 	BYTE		bEndSequence;
 	BYTE		unobserved_b[1];
 	int			anCels[4];
 	int			nCelNum;
};
typedef struct AUTOMAPSUB AUTOMAPSUB, *PTAUTOMAPSUB;

struct TREASURECLASSTXT
{
 	BYTE		NumCodes;
 	BYTE		unobserved_1[3];
 	DWORD		Code[30];
};
typedef struct TREASURECLASSTXT TREASURECLASSTXT, *PTTREASURECLASSTXT;

struct SUPERUNIQUESTXT
{
 	char		Name[60];
 	WORD		wszName[60];
 	int			Class;
 	int			Mod1;
 	int			Mod2;
 	int			Mod3;
 	int			MinGrp;
 	int			MaxGrp;
};
typedef struct SUPERUNIQUESTXT SUPERUNIQUESTXT, *PTSUPERUNIQUESTXT;

#define NUM_MISSILE_CLASSES		385
struct MISSILESTXT
{
 	BYTE		Vel;
 	BYTE		MaxVel;
 	BYTE		unobserved_2[2];
 	int			Accel;
 	int			Range;
 	int			LevRange;
 	BYTE		Light;
 	BYTE		Flicker;
 	BYTE		Red;
 	BYTE		Green;
 	BYTE		Blue;
 	BYTE		InitSteps;
 	BYTE		Activate;
 	BYTE		LoopAnim;
 	char		CelFile[64];
 	BYTE		AnimLen;
 	BYTE		unobserved_59[3];
 	int			RandStart;
 	BYTE		SubLoop;
 	BYTE		SubStart;
 	BYTE		SubStop;
 	BYTE		CollideType;
 	BYTE		Collision;
 	BYTE		CollideKill;
 	BYTE		CollideFriend;
 	BYTE		LastCollide;
 	BYTE		CanDestroy;
 	BYTE		ClientSend;
 	BYTE		NextHit;
 	BYTE		NextDelay;
 	BYTE		Size;
 	BYTE		ToHit;
 	BYTE		AlwaysExplode;
 	BYTE		Explosion;
 	BYTE		NeverDup;
 	BYTE		ReturnFire;
 	BYTE		GetHit;
 	BYTE		KnockBack;
 	BYTE		Trans;
 	BYTE		Qty;
 	BYTE		Pierce;
 	BYTE		unobserved_77[1];
 	int			Param1;
 	int			Param2;
 	BYTE		Open;
 	BYTE		Beta;
 	BYTE		unobserved_82[2];
 	int			SpecialSetup;
 	int			Skill;
 	BYTE		HitShift;
 	BYTE		unobserved_8d[3];
 	int			SrcDamage;
 	int			nMinDamage;
 	int			nMaxDamage;
 	int			LevDamage;
 	BYTE		EType;
 	BYTE		unobserved_a1[3];
 	int			EMin;
 	int			EMax;
 	int			ELevel;
 	int			ELen;
 	int			ELevelLen;
 	BYTE		unobserved_b8[4];
 	int			HitClass;
 	BYTE		NumDirections;
 	BYTE		LocalBlood;
 	BYTE		unobserved_c2[2];
};
typedef struct MISSILESTXT MISSILESTXT, *PTMISSILESTXT;

struct INVBODYLOCTXT
{
 	int			Left;
 	int			Right;
 	int			Top;
 	int			Bottom;
 	BYTE		Width;
 	BYTE		Height;
 	BYTE		unobserved_12[2];
};
typedef struct INVBODYLOCTXT INVBODYLOCTXT;
struct INVRECTTXT
{
 	int			Left;
 	int			Right;
 	int			Top;
 	int			Bottom;
};
typedef struct INVRECTTXT INVRECTTXT;
struct INVGRIDTXT
{
 	BYTE		Rows;
 	BYTE		Cols;
 	BYTE		unobserved_2[2];
 	int			Left;
 	int			Right;
 	int			Top;
 	int			Bottom;
 	BYTE		BoxWidth;
 	BYTE		BoxHeight;
 	BYTE		unobserved_16[2];
};
typedef struct INVGRIDTXT INVGRIDTXT;
struct INVENTORYTXT
{
 	INVRECTTXT	inv;
 	INVGRIDTXT	grid;
 	INVBODYLOCTXT	atBodyLocs[10];
};
typedef struct INVENTORYTXT INVENTORYTXT, *PTINVENTORYTXT;

struct MAGICAFFIXTXT
{
 	BYTE		nummods;
 	BYTE		unobserved_1[3];
 	ITEMMODTXT	mod[3];
 	BYTE		Armor;
 	BYTE		Shield;
 	BYTE		Weapon;
 	BYTE		Scepter;
 	BYTE		Wand;
 	BYTE		Staff;
 	BYTE		Bow;
 	BYTE		Boots;
 	BYTE		Gloves;
 	BYTE		Belt;
 	BYTE		Ring;
 	BYTE		Amulet;
 	BYTE		spawnable;
 	BYTE		transform;
 	BYTE		transformcolor;
 	BYTE		unobserved_4f[1];
 	int			level;
 	int			group;
 	int			divide;
 	int			multiply;
 	int			add;
 	char		name[32];
 	WORD		wNameString;
 	BYTE		unobserved_86[2];
};
typedef struct MAGICAFFIXTXT MAGICAFFIXTXT, *PTMAGICAFFIXTXT;

struct RAREAFFIXTXT
{
 	BYTE		Armor;
 	BYTE		Helm;
 	BYTE		Shield;
 	BYTE		Sword;
 	BYTE		Axe;
 	BYTE		Mace;
 	BYTE		Spear;
 	BYTE		Scepter;
 	BYTE		Wand;
 	BYTE		Staff;
 	BYTE		Bow;
 	BYTE		Boots;
 	BYTE		Gloves;
 	BYTE		Belt;
 	BYTE		Ring;
 	BYTE		Amulet;
 	int			add;
 	int			multiply;
 	int			divide;
 	char		name[32];
 	WORD		wNameString;
 	BYTE		unobserved_3e[2];
};
typedef struct RAREAFFIXTXT RAREAFFIXTXT, *PTRAREAFFIXTXT;

struct UNIQUEITEMSTXT
{
 	DWORD		code;
 	char		name[32];
 	WORD		wNameString;
 	BYTE		beta;
 	BYTE		transform;
 	BYTE		invtransform;
 	BYTE		transformcolor;
 	BYTE		nummods;
 	BYTE		unobserved_2b[1];
 	int			level;
 	ITEMMODTXT	itemmod[10];
};
typedef struct UNIQUEITEMSTXT UNIQUEITEMSTXT, *PTUNIQUEITEMSTXT;

struct SETITEMSTXT
{
 	char		name[96];
 	WORD		wNameString;
 	char		prefix[32];
 	WORD		wPrefixString;
 	BYTE		numitems;
 	BYTE		numproperties;
 	BYTE		transform;
 	BYTE		transformcolor;
 	int			level;
 	DWORD		item1;
 	WORD		wItem1SuffixString;
 	char		item1suffix[32];
 	BYTE		unobserved_b2[2];
 	DWORD		item2;
 	WORD		wItem2SuffixString;
 	char		item2suffix[32];
 	BYTE		unobserved_da[2];
 	DWORD		item3;
 	WORD		wItem3SuffixString;
 	char		item3suffix[32];
 	BYTE		unobserved_102[2];
 	DWORD		item4;
 	WORD		wItem4SuffixString;
 	char		item4suffix[32];
 	BYTE		unobserved_12a[2];
 	DWORD		item5;
 	WORD		wItem5SuffixString;
 	char		item5suffix[32];
 	BYTE		unobserved_152[2];
 	DWORD		item6;
 	WORD		wItem6SuffixString;
 	char		item6suffix[32];
 	BYTE		unobserved_17a[2];
 	ITEMMODTXT	propertymod[25];
};
typedef struct SETITEMSTXT SETITEMSTXT, *PTSETITEMSTXT;

struct SETITEMINFO
{
 	DWORD		item;
 	WORD		wItemSuffixString;
 	char		itemsuffix[32];
 	BYTE		unobserved_26[2];
};
typedef struct SETITEMINFO SETITEMINFO, *PTSETITEMINFO;

#define NUM_LEVEL_IDS		109

PTANIMDATATBL __fastcall AnimTblsLoad(HD2ARCHIVE hArchive);
PTANIMDATAREC __fastcall AnimTblsFindSequence(DWORD dwUnit, DWORD dwClass, DWORD dwMode, DWORD dwUnitType,
	DWORD dwInventory);
PTANIMDATATBL __fastcall DataTblsGetAnimData(void);
void __fastcall MonsterTblsLoadMonStats(void *pBuffer, int nBufferSize);
void __fastcall ArenaLoadCharTemplate(void *pBuffer, int nBufferSize);
PTBELTSTXT __fastcall BeltTblsLoad(void *pBuffer, int nBufferSize);
PTINVENTORYTXT __fastcall InvTblsLoad(void *pBuffer, int nBufferSize);
int __fastcall ItemTblsLoadItems(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2,
	void *pBuffer3, int nBufferSize3);
void __fastcall ItemTblsLoadMagicAffixes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2);
void __fastcall ItemTblsLoadRareAffixes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2);
int __fastcall ItemTblsLoadUniqueItems(void *pBuffer, int nBufferSize);
PTSETITEMSTXT __fastcall ItemTblsLoadSetItems(void *pBuffer, int nBufferSize);
void __fastcall LvlTblsLoadLevels(void *pBuffer, int nBufferSize);
void __fastcall LvlTblsLoadLevelDefs(void *pBuffer, int nBufferSize);
void __fastcall LvlTblsLoadLvlTypes(void *pBuffer, int nBufferSize);
PTMISSILESTXT __fastcall MissileTblsLoad(void *pBuffer, int nBufferSize);
PTTREASURECLASSTXT __fastcall MonsterTblsLoadTreasureClass(void *pBuffer, int nBufferSize);
void __fastcall MonsterTblsLoadSuperUniques(void *pBuffer, int nBufferSize);
void __fastcall ItemTblsLoadQualityItems(void *pBuffer, int nBufferSize);
int __fastcall ItemTblsLoadGems(void *pBuffer, int nBufferSize);
int __fastcall ItemTblsLoadBooks(void *pBuffer, int nBufferSize);
void __fastcall ItemTblsLoadLowQualityItems(void *pBuffer, int nBufferSize);
PTITEMRATIOTXT __fastcall ItemTblsLoadItemRatio(void *pBuffer, int nBufferSize);
PTITEMSTATCOSTTXT __fastcall ItemTblsLoadItemStatCost(void *pBuffer, int nBufferSize);
void __fastcall ItemTblsLoadGamble(void *pBuffer, int nBufferSize);
void __fastcall ItemTblsFreeItems(void);
void __fastcall ItemTblsFreeMagicAffixes(void);
void __fastcall ItemTblsFreeRareAffixes(void);
void __fastcall ItemTblsFreeUniqueItems(void);
void __fastcall ItemTblsFreeSetItems(void);
void __fastcall ItemTblsFreeQualityItems(void);
void __fastcall ItemTblsFreeLowQualityItems(void);
void __fastcall ItemTblsFreeGems(void);
void __fastcall ItemTblsFreeBooks(void);
void __fastcall ItemTblsFreeItemRatio(void);
void __fastcall ItemTblsFreeItemStatCost(void);
void __fastcall ItemTblsFreeGamble(void);
void __fastcall MonsterTblsLoadUniqueNames(void *pBuffer, int nBufferSize, int nType);
void __fastcall ObjTblsLoadObjects(void *pBuffer, int nBufferSize);
PTOBJGROUPTXT __fastcall ObjTblsLoadObjGroup(void *pBuffer, int nBufferSize);
void __fastcall OverlayTblsLoad(void *pBuffer, int nBufferSize);
void __fastcall TokTblsLoadPlrTypeModes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2);
void __fastcall TokTblsLoadMonTypeModes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2);
void __fastcall TokTblsLoadObjTypeModes(void *pBuffer1, int nBufferSize1, void *pBuffer2, int nBufferSize2);
void __fastcall LvlTblsLoadLvlPrest(void *pBuffer, int nBufferSize, BOOL fLoadFiles);
void __fastcall LvlTblsLoadLvlSub(HD2ARCHIVE hArchive, void *pBuffer, int nBufferSize, BOOL fLoadFiles,
	BOOL fLoadFiles2);
void __fastcall LvlTblsLoadLvlTypeFiles(HD2ARCHIVE hArchive);
void __fastcall LvlTblsLoadAutomap(HD2ARCHIVE hArchive, void *pBuffer, int nBufferSize);
PTLVLWARPTXT __fastcall LvlTblsLoadLvlWarp(void *pBuffer, int nBufferSize);
PTLVLMAZETXT __fastcall LvlTblsLoadLvlMaze(void *pBuffer, int nBufferSize);
PTLVLPRESTTXT __fastcall LvlTblsGetPrestByLevel(int nLevelId);
PTLVLMAZETXT __fastcall LvlTblsGetMazeDef(int nLevelId);
PTLVLSUBTXT __fastcall LvlTblsGetTileSubs(int nType);
void __fastcall TileSubFreeRow(PTLVLSUBTXT ptTileSub);
PTMONITEMPERCENTTXT __fastcall MonsterTblsLoadMonItemPercent(void *pBuffer, int nBufferSize);
PTSHRINESTXT __fastcall ObjTblsLoadShrines(void *pBuffer, int nBufferSize);
PTTOKENTXT __fastcall TokTblsLoadComposit(void *pBuffer, int nBufferSize);
PTTOKENTXT __fastcall TokTblsLoadArmType(void *pBuffer, int nBufferSize);
void __fastcall TokTblsLoadNothing(HD2ARCHIVE hArchive);

BOOL __stdcall FieldTblsLoad(void *pBuffer, int nBufferSize);
BOOL __fastcall FieldTblsFree(void);
void __fastcall AnimTblsFree(PTANIMDATATBL ptTbl);
void __fastcall ArenaFreeCharTemplate(void);
void __fastcall BeltTblsFree(void);
void __fastcall InvTblsFree(void);
void __fastcall LvlTblsFreeLevels(void);
void __fastcall LvlTblsFreeLevelDefs(void);
void __fastcall LvlTblsFreeLvlTypes(void);
void __fastcall LvlTblsFreeLvlPrest(void);
void __fastcall LvlTblsFreeLvlWarp(void);
void __fastcall LvlTblsFreeLvlMaze(void);
void __fastcall LvlTblsFreeLvlSub(void);
void __fastcall LvlTblsFreeLvlTypeFiles(void);
void __fastcall LvlTblsFreeAutomap(void);
void __fastcall MissileTblsFree(void);
void __fastcall MonsterTblsFreeMonStats(void);
void __fastcall MonsterTblsFreeTreasureClass(void);
void __fastcall MonsterTblsFreeMonItemPercent(void);
void __fastcall ObjTblsFreeObjects(void);
void __fastcall ObjTblsFreeObjGroup(void);
void __fastcall ObjTblsFreeShrines(void);
void __fastcall OverlayTblsFree(void);
void __fastcall TokTblsFree(void);
void __fastcall TokTblsFreeNothing(void);

void * __fastcall ArchiveAllocBufferAndRead(HD2ARCHIVE hArchive, const char *pszFileName, DWORD *pdwSize,
	const char *pszFile, int nLine);

void * __stdcall DataTblsAllocAndTokenize(void *pDescriptors, char *pBuffer, int nSize, int *pnRows,
	int nStride);
void __stdcall DataTblsFreeBlock(void *p);

#endif
