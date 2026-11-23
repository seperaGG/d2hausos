#define D2_THIS_FILE "C:\\D2\\Source\\D2Common\\Guilds\\Guilds.cpp"
#include "../D2Common.h"
#include "../D2CommonImports.h"
#include "D2Macros.h"
#include "FogImports.h"

static const char *sgpszGuildPlaceholderName = "???";

#define MAX_GUILD_MEMBERS	100

struct STRPACK
{
 	int			nPackedBits;
 	BYTE		unobserved_4[516 - 4];
};
typedef struct STRPACK STRPACK;

static STRPACK sgtGuildStrPack;

#pragma pack(push, 1)
struct GUILDSAVE
{
 	WORD		wSize;
 	DWORD		field_02;
 	DWORD		field_06;
 	BYTE		szTag[3];
 	BYTE		szName[27];
 	BYTE		abWebLink[218];
 	BYTE		abCharter[256];
 	BYTE		bMembers;
 	BYTE		bEmblemIcon;
 	BYTE		bEmblemColor1;
 	BYTE		bEmblemColor2;
 	BYTE		field_206;
 	WORD		wRank;
 	DWORD		dwFlags;
 	unsigned __int64	qwCounterA;
 	unsigned __int64	qwCounterB;
};

struct GUILDROSTERENTRY
{
 	BYTE		abName[12];
 	BYTE		bMemberId;
 	BYTE		field_0d;
 	WORD		field_0e;
 	WORD		field_10;
 	DWORD		field_12;
};

struct GUILDROSTER
{
 	WORD		wSize;
 	BYTE		bMembers;
 	struct GUILDROSTERENTRY	atMembers[MAX_GUILD_MEMBERS];
};
#pragma pack(pop)
typedef struct GUILDSAVE GUILDSAVE;
typedef struct GUILDROSTERENTRY GUILDROSTERENTRY;
typedef struct GUILDROSTER GUILDROSTER;

#define GUILD_NAME_ALPHABET	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

void __stdcall GuildsSetAllChanged(PTGUILDDATA hGuildData, BOOL fSet);

void __stdcall GuildsNothing(DWORD dwUnused1)
{
}

PTGUILDDATA __stdcall GuildsDataNew(void *pPool)
{
	PTGUILDDATA pData;
	DWORD *padwZero;
	int i;

#line 196
	pData = (PTGUILDDATA)D2ALLOCPOOL(pPool, sizeof(GUILDDATA));
	padwZero = (DWORD *)pData;
	for (i = 0; i < (int)(sizeof(GUILDDATA) / sizeof(DWORD)); i++)
		padwZero[i] = 0;
	pData->pPool = pPool;

	pData->dwVaultInventory = D2CommonINVENTORYCreate(pPool, 0, 2, 338, 0);
	pData->dwVaultUnitId = 0xFFFFFFFF;
	pData->dwTrophyCaseInventory = D2CommonINVENTORYCreate(pPool, 0, 2, 339, 0);
	pData->dwTrophyCaseUnitId = 0xFFFFFFFF;
	pData->dwMessageBoardUnitId = 0xFFFFFFFF;
	pData->abGuildPostBuffer[0] = 0;
	pData->ptMemberList = NULL;
	return pData;
}

void __stdcall GuildsDataFree(PTGUILDDATA pData)
{
	PTGUILDLISTNODE pNode;

	if (pData == NULL)
		return;

	if (pData->pbSaveData != NULL)
	{
#line 224
		D2FREEPOOL(pData->pPool, pData->pbSaveData);
	}
	if (pData->pbRosterData != NULL)
	{
#line 226
		D2FREEPOOL(pData->pPool, pData->pbRosterData);
	}
	if (pData->pVaultItemData != NULL)
	{
#line 228
		D2FREEPOOL(pData->pPool, pData->pVaultItemData);
	}
	if (pData->pTrophyCaseItemData != NULL)
	{
#line 230
		D2FREEPOOL(pData->pPool, pData->pTrophyCaseItemData);
	}

	if (pData->dwVaultInventory != 0)
		D2CommonINVENTORYDestroy(pData->dwVaultInventory);
	if (pData->dwTrophyCaseInventory != 0)
		D2CommonINVENTORYDestroy(pData->dwTrophyCaseInventory);

	pNode = pData->ptMemberList;
	while (pNode != NULL)
	{
		PTGUILDLISTNODE pNext;

		pNext = pNode->ptNext;
		pData->ptMemberList = pNext;
#line 243
		D2FREEPOOL(pData->pPool, pNode);
		pNode = pData->ptMemberList;
	}

#line 246
	D2FREEPOOL(pData->pPool, pData);
}

static __inline void GuildsCheckPostSpace(PTGUILDDATA ptGuild, int nSize)
{
#line 420
	D2ASSERT((int)sizeof(WORD) + ptGuild->nGuildPostCursor < nSize);
}

int __stdcall GuildsWriteSave(PTGUILDDATA hGuildData, BYTE *pbBuffer, DWORD dwUnused, BOOL fItems)
{
	GUILDSAVE *ptSave;
	int nSize;
	int i;

#line 445
	D2ASSERT(hGuildData);
	ptSave = (GUILDSAVE *)pbBuffer;
	memset(pbBuffer, 0, sizeof(GUILDSAVE));
	ptSave->wSize = sizeof(GUILDSAVE);
	ptSave->field_02 = 43647642;
	ptSave->field_06 = 1;
	memcpy(ptSave->szTag, hGuildData->szTag, sizeof(ptSave->szTag));
	memcpy(ptSave->szName, hGuildData->szName, sizeof(ptSave->szName));
	memcpy(ptSave->abWebLink, hGuildData->szWebLink, sizeof(ptSave->abWebLink));
	memcpy(ptSave->abCharter, hGuildData->szCharter, sizeof(ptSave->abCharter));
	ptSave->bMembers = hGuildData->bMembers;
	ptSave->bEmblemIcon = hGuildData->bEmblemIcon;
	ptSave->bEmblemColor1 = hGuildData->bEmblemColor1;
	ptSave->bEmblemColor2 = hGuildData->bEmblemColor2;
	ptSave->field_206 = hGuildData->field_228;
	ptSave->wRank = hGuildData->wRank;
	ptSave->dwFlags = hGuildData->dwFlags;
	ptSave->qwCounterA = hGuildData->qwCounterA;
	ptSave->qwCounterB = hGuildData->qwCounterB;
	nSize = sizeof(GUILDSAVE);
	pbBuffer += nSize;
	if (fItems)
	{
		nSize += D2CommonItemSavePackList(hGuildData->dwVaultInventory, pbBuffer, 4096, 0);
		pbBuffer += nSize;
	}
	if (fItems)
	{
		nSize += D2CommonItemSavePackList(hGuildData->dwTrophyCaseInventory, pbBuffer, 4096, 0);
		GuildsCheckPostSpace(hGuildData, 4096);
	}
	ptSave->wSize = (WORD)nSize;
	return nSize;
}

int __stdcall GuildsWriteRoster(PTGUILDDATA hGuildData, BYTE *pbBuffer, int nSize)
{
	GUILDROSTER *ptGR;
	PTGUILDLISTNODE pNode;
	WORD wSize;
	int i;

#line 495
	D2ASSERT(hGuildData);
	ptGR = (GUILDROSTER *)pbBuffer;
	ptGR->bMembers = hGuildData->bMembers;
	if (!sgtGuildStrPack.nPackedBits)
		FogStringPackCreate(&sgtGuildStrPack, 6, GUILD_NAME_ALPHABET);
	wSize = (WORD)(hGuildData->bMembers * sizeof(GUILDROSTERENTRY) + 3);
	if ((int)wSize > nSize)
		return 0;
	ptGR->wSize = wSize;
	i = 0;
	for (pNode = hGuildData->ptMemberList; pNode != NULL && i < (int)hGuildData->bMembers; pNode
		= pNode->ptNext)
	{
		FogStringPackPack(&sgtGuildStrPack, pNode->szName, ptGR->atMembers[i].abName,
			sizeof(ptGR->atMembers[i].abName), FALSE);
		ptGR->atMembers[i].bMemberId = pNode->bMemberId;
		ptGR->atMembers[i].field_0d = pNode->field_11;
		ptGR->atMembers[i].field_0e = pNode->field_12;
		ptGR->atMembers[i].field_10 = pNode->wFlags;
		ptGR->atMembers[i].field_12 = pNode->dwDonated;
		i++;
	}
	return i * (int)sizeof(GUILDROSTERENTRY) + 3;
}

static __inline WORD GuildsSaveBytesLeft(PTGUILDDATA ptGuild, BYTE *pbCursor)
{
	return (WORD)(ptGuild->wSaveDataTotalSize - (pbCursor - ptGuild->pbSaveData));
}

static __inline void GuildsCopyString(BYTE *pszDst, const BYTE *pSrc, size_t nSize)
{
	memcpy(pszDst, pSrc, nSize);
	pszDst[nSize] = 0;
}

static __inline void GuildsUnpackSaveFixed(PTGUILDDATA ptGuild)
{
	GUILDSAVE *ptSave;

#line 326
	D2ASSERT(ptGuild->pbSaveData);
	ptSave = (GUILDSAVE *)ptGuild->pbSaveData;
	GuildsCopyString(ptGuild->szTag, ptSave->szTag, sizeof(ptSave->szTag));
	GuildsCopyString(ptGuild->szName, ptSave->szName, sizeof(ptSave->szName));
	GuildsCopyString(ptGuild->szWebLink, ptSave->abWebLink, sizeof(ptSave->abWebLink));
	GuildsCopyString(ptGuild->szCharter, ptSave->abCharter, sizeof(ptSave->abCharter));
	ptGuild->bMembers = ptSave->bMembers;
	ptGuild->bEmblemIcon = ptSave->bEmblemIcon;
	ptGuild->bEmblemColor1 = ptSave->bEmblemColor1;
	ptGuild->bEmblemColor2 = ptSave->bEmblemColor2;
	ptGuild->field_228 = ptSave->field_206;
	ptGuild->wRank = ptSave->wRank;
	ptGuild->dwFlags = ptSave->dwFlags;
	ptGuild->qwCounterA = ptSave->qwCounterA;
	ptGuild->qwCounterB = ptSave->qwCounterB;
	ptGuild->dwActive = 1;
	GuildsSetAllChanged(ptGuild, FALSE);
}

static __inline void GuildsUnpackSave(PTGUILDDATA ptGuild)
{
	BYTE *pbVault;
	BYTE *pbTrophyCase;
	BYTE *pbPost;
	WORD wLeft;
	WORD wLength;

	GuildsUnpackSaveFixed(ptGuild);
	ptGuild->wVaultItemDataSize = 0;
	ptGuild->wTrophyCaseItemDataSize = 0;
	if (ptGuild->pVaultItemData != NULL)
	{
#line 353
		D2FREEPOOL(ptGuild->pPool, ptGuild->pVaultItemData);
	}
	ptGuild->pVaultItemData = NULL;
	if (ptGuild->pTrophyCaseItemData != NULL)
	{
#line 356
		D2FREEPOOL(ptGuild->pPool, ptGuild->pTrophyCaseItemData);
	}
	ptGuild->pTrophyCaseItemData = NULL;

	pbVault = ptGuild->pbSaveData + sizeof(GUILDSAVE);
	wLeft = GuildsSaveBytesLeft(ptGuild, pbVault);
	if (wLeft <= 2)
		return;
	wLength = (WORD)(*(WORD *)pbVault * 25 + 2);
	if (wLeft < wLength)
		return;
	ptGuild->wVaultItemDataSize = wLength;
#line 378
	ptGuild->pVaultItemData = D2ALLOCPOOL(ptGuild->pPool, wLength);
	memcpy(ptGuild->pVaultItemData, pbVault, ptGuild->wVaultItemDataSize);
	pbTrophyCase = pbVault + wLength;

	wLeft = GuildsSaveBytesLeft(ptGuild, pbTrophyCase);
	if (wLeft <= 2)
		return;
	wLength = (WORD)(*(WORD *)pbTrophyCase * 25 + 2);
	if (wLeft < wLength)
		return;
	ptGuild->wTrophyCaseItemDataSize = wLength;
#line 394
	ptGuild->pTrophyCaseItemData = D2ALLOCPOOL(ptGuild->pPool, wLength);
	memcpy(ptGuild->pTrophyCaseItemData, pbTrophyCase, ptGuild->wTrophyCaseItemDataSize);
	pbPost = pbTrophyCase + wLength;

	wLeft = GuildsSaveBytesLeft(ptGuild, pbPost);
	if (wLeft <= 2)
		return;
	wLength = *(WORD *)pbPost;
	if ((DWORD)wLeft < (DWORD)wLength + 2)
		return;
	memcpy(ptGuild->abGuildPostBuffer, pbPost + 2, wLength);
	ptGuild->nGuildPostCursor = wLength;
}

BOOL __stdcall GuildsReceiveSave(PTGUILDDATA hGuildData, DWORD dwUnused, WORD wChunk, WORD wOffset,
	WORD wTotal, const BYTE *pbChunk)
{
#line 546
	D2ASSERT(hGuildData);
	if (!wTotal || !wChunk)
		return FALSE;
	if (hGuildData->wSaveDataTotalSize != wTotal || !wOffset || hGuildData->pbSaveData == NULL)
	{
		if (hGuildData->pbSaveData != NULL)
		{
#line 558
			D2FREEPOOL(hGuildData->pPool, hGuildData->pbSaveData);
		}
#line 559
		hGuildData->pbSaveData = (BYTE *)D2ALLOCPOOL(hGuildData->pPool, wTotal);
		if (hGuildData->pbSaveData == NULL)
			return FALSE;
		hGuildData->wSaveDataReceived = 0;
		hGuildData->wSaveDataTotalSize = wTotal;
	}
	memcpy(hGuildData->pbSaveData + hGuildData->wSaveDataReceived, pbChunk, wChunk);
	hGuildData->wSaveDataReceived += wChunk;
	if (hGuildData->wSaveDataReceived != wTotal)
		return TRUE;
	GuildsUnpackSave(hGuildData);
#line 576
	D2FREEPOOL(hGuildData->pPool, hGuildData->pbSaveData);
	hGuildData->pbSaveData = NULL;
	hGuildData->wSaveDataReceived = 0;
	hGuildData->wSaveDataTotalSize = 0;
	return TRUE;
}

static __inline void GuildsAddMember(PTGUILDDATA ptGuild, const char *pszName, BYTE bMemberId, BYTE bField11,
	WORD wField12, WORD wFlags, DWORD dwDonated)
{
	PTGUILDLISTNODE pNode;

#line 269
	pNode = (PTGUILDLISTNODE)D2ALLOCPOOL(ptGuild->pPool, sizeof(GUILDLISTNODE));
	if (pNode == NULL)
		return;
	memcpy(pNode->szName, pszName, sizeof(pNode->szName) - 1);
	pNode->szName[sizeof(pNode->szName) - 1] = 0;
	pNode->wFlags = wFlags;
	pNode->bMemberId = bMemberId;
	pNode->field_11 = bField11;
	pNode->field_12 = wField12;
	pNode->dwDonated = dwDonated;
	pNode->ptNext = ptGuild->ptMemberList;
	ptGuild->ptMemberList = pNode;
}

static __inline void GuildsUnpackRoster(PTGUILDDATA ptGuild)
{
	PTGUILDLISTNODE pNode;
	GUILDROSTER *ptGR;
	char szName[16];
	int i;

	while (ptGuild->ptMemberList != NULL)
	{
		pNode = ptGuild->ptMemberList;
		ptGuild->ptMemberList = pNode->ptNext;
#line 602
		D2FREEPOOL(ptGuild->pPool, pNode);
	}
	ptGR = (GUILDROSTER *)ptGuild->pbRosterData;
#line 606
	D2ASSERT(ptGR->bMembers <= MAX_GUILD_MEMBERS);
	ptGuild->bMembers = ptGR->bMembers;
	if (!sgtGuildStrPack.nPackedBits)
		FogStringPackCreate(&sgtGuildStrPack, 6, GUILD_NAME_ALPHABET);
	for (i = 0; i < (int)ptGuild->bMembers; i++)
	{
		FogStringPackUnpack(&sgtGuildStrPack, szName, ptGR->atMembers[i].abName,
			sizeof(ptGR->atMembers[i].abName), sizeof(szName));
		GuildsAddMember(ptGuild, szName, ptGR->atMembers[i].bMemberId, ptGR->atMembers[i].field_0d,
			ptGR->atMembers[i].field_0e, ptGR->atMembers[i].field_10, ptGR->atMembers[i].field_12);
	}
#line 632
	D2FREEPOOL(ptGuild->pPool, ptGuild->pbRosterData);
	ptGuild->pbRosterData = NULL;
	ptGuild->wRosterDataReceived = 0;
	ptGuild->wRosterDataTotalSize = 0;
}

BOOL __stdcall GuildsReceiveRoster(PTGUILDDATA hGuildData, DWORD dwUnused, WORD wChunk, WORD wOffset,
	WORD wTotal, const BYTE *pbChunk)
{
#line 652
	D2ASSERT(hGuildData);
	if (!wTotal || !wChunk)
		return FALSE;
	if (hGuildData->wRosterDataTotalSize != wTotal || !wOffset || hGuildData->pbRosterData == NULL)
	{
		if (hGuildData->pbRosterData != NULL)
		{
#line 664
			D2FREEPOOL(hGuildData->pPool, hGuildData->pbRosterData);
		}
#line 665
		hGuildData->pbRosterData = (BYTE *)D2ALLOCPOOL(hGuildData->pPool, wTotal);
		if (hGuildData->pbRosterData == NULL)
			return FALSE;
		hGuildData->wRosterDataReceived = 0;
		hGuildData->wRosterDataTotalSize = wTotal;
	}
	memcpy(hGuildData->pbRosterData + hGuildData->wRosterDataReceived, pbChunk, wChunk);
	hGuildData->wRosterDataReceived += wChunk;
	if (hGuildData->wRosterDataReceived != wTotal)
		return TRUE;
	if (hGuildData->pbRosterData != NULL && hGuildData->wRosterDataReceived
		== hGuildData->wRosterDataTotalSize)
		GuildsUnpackRoster(hGuildData);
	if (hGuildData->pbRosterData != NULL)
	{
#line 683
		D2FREEPOOL(hGuildData->pPool, hGuildData->pbRosterData);
	}
	hGuildData->pbRosterData = NULL;
	hGuildData->wRosterDataReceived = 0;
	hGuildData->wRosterDataTotalSize = 0;
	return TRUE;
}

DWORD __stdcall GuildsGetActive(PTGUILDDATA hGuildData)
{
#line 698
	D2ASSERT(hGuildData);
	return hGuildData->dwActive;
}

void __stdcall GuildsSetChanged(PTGUILDDATA hGuildData, BOOL fSet)
{
#line 711
	D2ASSERT(hGuildData);
	if (fSet)
		hGuildData->dwFlags |= 16;
	else
		hGuildData->dwFlags &= ~16;
}

DWORD __stdcall GuildsGetChanged(PTGUILDDATA hGuildData)
{
#line 728
	D2ASSERT(hGuildData);
	return hGuildData->dwFlags & 16;
}

void __stdcall GuildsSetAllChanged(PTGUILDDATA hGuildData, BOOL fSet)
{
#line 742
	D2ASSERT(hGuildData);
	if (fSet)
		hGuildData->dwFlags |= 64;
	else
		hGuildData->dwFlags &= ~64;
	GuildsSetChanged(hGuildData, fSet);

}

DWORD __stdcall GuildsGetEmblemChanged(PTGUILDDATA hGuildData)
{
#line 761
	D2ASSERT(hGuildData);
	return hGuildData->dwFlags & 64;
}

BYTE * __stdcall GuildsGetTag(PTGUILDDATA hGuildData)
{
#line 775
	D2ASSERT(hGuildData);
	return hGuildData->szTag;
}

BYTE * __stdcall GuildsGetName(PTGUILDDATA hGuildData)
{
#line 788
	D2ASSERT(hGuildData);
	return hGuildData->szName;
}

BYTE * __stdcall GuildsGetWebLink(PTGUILDDATA hGuildData)
{
#line 801
	D2ASSERT(hGuildData);
	return hGuildData->szWebLink;
}

void __stdcall GuildsSetWebLink(PTGUILDDATA hGuildData, const void *pSrc)
{
#line 814
	D2ASSERT(hGuildData);
	memcpy(hGuildData->szWebLink, pSrc, sizeof(hGuildData->szWebLink) - 1);
	hGuildData->szWebLink[sizeof(hGuildData->szWebLink) - 1] = 0;
	hGuildData->dwFlags |= 16;
}

BYTE * __stdcall GuildsGetCharter(PTGUILDDATA hGuildData)
{
#line 831
	D2ASSERT(hGuildData);
	return hGuildData->szCharter;
}

void __stdcall GuildsSetCharter(PTGUILDDATA hGuildData, const void *pSrc)
{
#line 844
	D2ASSERT(hGuildData);
	memcpy(hGuildData->szCharter, pSrc, sizeof(hGuildData->szCharter) - 1);
	hGuildData->szCharter[sizeof(hGuildData->szCharter) - 1] = 0;
	hGuildData->dwFlags |= 16;
}

DWORD __stdcall GuildsGetMembers(PTGUILDDATA hGuildData)
{
#line 861
	D2ASSERT(hGuildData);
	return hGuildData->bMembers;
}

void __stdcall GuildsGetEmblem(PTGUILDDATA hGuildData, BYTE *pbOut1, BYTE *pbOut2, BYTE *pbOut3)
{
#line 874
	D2ASSERT(hGuildData);
	*pbOut1 = hGuildData->bEmblemIcon;
	*pbOut2 = hGuildData->bEmblemColor1;
	*pbOut3 = hGuildData->bEmblemColor2;
}

void __stdcall GuildsSetEmblem(PTGUILDDATA hGuildData, BYTE bVal1, BYTE bVal2, BYTE bVal3)
{
#line 890
	D2ASSERT(hGuildData);
	hGuildData->bEmblemIcon = bVal1;
	hGuildData->bEmblemColor1 = bVal2;
	hGuildData->bEmblemColor2 = bVal3;
	hGuildData->dwFlags |= 80;
}

DWORD __stdcall GuildsGetByteAfterEmblem(PTGUILDDATA hGuildData)
{
#line 908
	D2ASSERT(hGuildData);
	return hGuildData->field_228;
}

DWORD __stdcall GuildsGetFlags(PTGUILDDATA hGuildData)
{
#line 921
	D2ASSERT(hGuildData);
	return hGuildData->dwFlags;
}

unsigned __int64 __stdcall GuildsGetCounterB(PTGUILDDATA hGuildData)
{
#line 935
	D2ASSERT(hGuildData);
	return hGuildData->qwCounterB;
}

void __stdcall GuildsSetCounterB(PTGUILDDATA hGuildData, unsigned __int64 qwVal)
{
#line 949
	D2ASSERT(hGuildData);
	hGuildData->qwCounterB = qwVal;
	hGuildData->dwFlags |= 16;
}

#define GUILD_RANK_COUNT	6

struct GUILDRANKENTRY
{
 	unsigned __int64	qwThreshold;
 	DWORD		dwLevelId;
 	DWORD		field_0c;
};
typedef struct GUILDRANKENTRY GUILDRANKENTRY;

static GUILDRANKENTRY sgtGuildRankTbl[GUILD_RANK_COUNT];

unsigned __int64 __stdcall GuildsGetNextRankThreshold(PTGUILDDATA hGuildData)
{
	unsigned __int64 qwCounter;
	int i;

#line 965
	D2ASSERT(hGuildData);
	qwCounter = hGuildData->qwCounterB;
	for (i = 0; i < GUILD_RANK_COUNT; i++)
	{
		if (sgtGuildRankTbl[i].qwThreshold > qwCounter)
			return sgtGuildRankTbl[i].qwThreshold;
	}
	return 0;
}

int __stdcall GuildsGetRankOf(unsigned __int64 qwValue)
{
	int i;

	for (i = 0; i < GUILD_RANK_COUNT; i++)
	{
		if (qwValue < sgtGuildRankTbl[i].qwThreshold)
			return i;
	}
	return GUILD_RANK_COUNT;
}

DWORD __stdcall GuildsDonateGold(PTGUILDDATA hGuildData, const char *pszName, DWORD dwAmount)
{
	int nOldRank;
	int nNewRank;
	PTGUILDLISTNODE pNode;

#line 999
	D2ASSERT(hGuildData);
	nOldRank = GuildsGetRankOf(hGuildData->qwCounterB);
	hGuildData->qwCounterB += dwAmount;
	nNewRank = GuildsGetRankOf(hGuildData->qwCounterB);
	if (nOldRank != nNewRank)
		hGuildData->wRank = (WORD)nNewRank;
	if (nNewRank >= 0)
		hGuildData->dwFlags |= 2;
	hGuildData->dwFlags |= 16;

	for (pNode = hGuildData->ptMemberList; pNode != NULL; pNode = pNode->ptNext)
	{
		if (strnicmp(pNode->szName, pszName, 16) == 0)
			break;
	}
	if (pNode == NULL)
		return dwAmount;

	if (pNode->dwDonated + dwAmount < dwAmount)
		pNode->dwDonated = 0xFFFFFFFF;
	else
		pNode->dwDonated += dwAmount;
	return pNode->dwDonated;
}

unsigned __int64 __stdcall GuildsGetCounterA(PTGUILDDATA hGuildData)
{
#line 1032
	D2ASSERT(hGuildData);
	return hGuildData->qwCounterA;
}

void __stdcall GuildsSetCounterA(PTGUILDDATA hGuildData, unsigned __int64 qwVal)
{
#line 1046
	D2ASSERT(hGuildData);
	hGuildData->qwCounterA = qwVal;
	hGuildData->dwFlags |= 16;
}

void __stdcall GuildsAddCounterA(PTGUILDDATA hGuildData, DWORD dwAmount)
{
#line 1062
	D2ASSERT(hGuildData);
	hGuildData->qwCounterA += dwAmount;
	hGuildData->dwFlags |= 16;
}

DWORD __stdcall GuildsSubtractCounterA(PTGUILDDATA hGuildData, DWORD dwAmount)
{
	unsigned __int64 qwCur;
	DWORD dwSub;

#line 1078
	D2ASSERT(hGuildData);
	dwSub = dwAmount;
	if ((unsigned __int64)dwSub > hGuildData->qwCounterA)
		dwSub = (DWORD)hGuildData->qwCounterA;
	hGuildData->qwCounterA = hGuildData->qwCounterA - dwSub;
	hGuildData->dwFlags |= 16;
	return dwSub;
}

DWORD __stdcall GuildsGetVaultInventory(PTGUILDDATA hGuildData)
{
#line 1099
	D2ASSERT(hGuildData);
	return hGuildData->dwVaultInventory;
}

void __stdcall GuildsSetVaultUnitId(PTGUILDDATA hGuildData, DWORD dwVal)
{
#line 1113
	D2ASSERT(hGuildData);
	hGuildData->dwVaultUnitId = dwVal;
}

DWORD __stdcall GuildsGetVaultUnitId(PTGUILDDATA hGuildData)
{
#line 1127
	D2ASSERT(hGuildData);
	return hGuildData->dwVaultUnitId;
}

void __stdcall GuildsGetVaultItemData(PTGUILDDATA hGuildData, void **ppOut, WORD *pwOut)
{
#line 1141
	D2ASSERT(hGuildData);
	*ppOut = hGuildData->pVaultItemData;
	*pwOut = hGuildData->wVaultItemDataSize;
}

DWORD __stdcall GuildsGetTrophyCaseInventory(PTGUILDDATA hGuildData)
{
#line 1156
	D2ASSERT(hGuildData);
	return hGuildData->dwTrophyCaseInventory;
}

void __stdcall GuildsSetTrophyCaseUnitId(PTGUILDDATA hGuildData, DWORD dwVal)
{
#line 1170
	D2ASSERT(hGuildData);
	hGuildData->dwTrophyCaseUnitId = dwVal;
}

DWORD __stdcall GuildsGetTrophyCaseUnitId(PTGUILDDATA hGuildData)
{
#line 1184
	D2ASSERT(hGuildData);
	return hGuildData->dwTrophyCaseUnitId;
}

void __stdcall GuildsGetTrophyCaseItemData(PTGUILDDATA hGuildData, void **ppOut, WORD *pwOut)
{
#line 1198
	D2ASSERT(hGuildData);
	*ppOut = hGuildData->pTrophyCaseItemData;
	*pwOut = hGuildData->wTrophyCaseItemDataSize;
}

void __stdcall GuildsSetMessageBoardUnitId(PTGUILDDATA hGuildData, DWORD dwVal)
{
#line 1213
	D2ASSERT(hGuildData);
	hGuildData->dwMessageBoardUnitId = dwVal;
}

DWORD __stdcall GuildsGetMessageBoardUnitId(PTGUILDDATA hGuildData)
{
#line 1228
	D2ASSERT(hGuildData);
	return hGuildData->dwMessageBoardUnitId;
}

static __inline void GuildsPostDropFirst(PTGUILDDATA ptGuild)
{
	int nLen;
	int nCursor;

#line 1280
	D2ASSERT(ptGuild->nGuildPostCursor);
	nLen = strlen((const char *)ptGuild->abGuildPostBuffer);
#line 1283
	D2ASSERT(nLen);
#line 1284
	D2ASSERT(ptGuild->nGuildPostCursor > nLen);
	nLen++;
	nCursor = ptGuild->nGuildPostCursor - nLen;
	ptGuild->nGuildPostCursor = nCursor;
	memmove(ptGuild->abGuildPostBuffer, ptGuild->abGuildPostBuffer + nLen, nCursor);
}

static __inline BYTE GuildsFindMemberId(PTGUILDDATA hGuildData, const char *pszName)
{
	PTGUILDLISTNODE pNode;

	for (pNode = hGuildData->ptMemberList; pNode != NULL; pNode = pNode->ptNext)
	{
		if (strnicmp(pNode->szName, pszName, 15) == 0)
		{
			if (pNode->bMemberId != 0)
				return pNode->bMemberId;
			return 255;
		}
	}
	return 255;
}

BOOL __stdcall GuildsPostAppend(PTGUILDDATA hGuildData, const char *pszName, char *pszText)
{
	PTGUILDLISTNODE pNode;
	BYTE bId;
	int nLen;

#line 1300
	D2ASSERT(hGuildData);
	if (hGuildData->wGuildPostTotalSize != 0)
		return FALSE;

	bId = GuildsFindMemberId(hGuildData, pszName);

	nLen = strlen(pszText);
	if (nLen >= 256)
		pszText[255] = 0;
	while (hGuildData->nGuildPostCursor + nLen + 2 >= GUILD_POST_BUFFER_SIZE)
		GuildsPostDropFirst(hGuildData);

	hGuildData->abGuildPostBuffer[hGuildData->nGuildPostCursor] = bId;
	hGuildData->nGuildPostCursor++;
	strcpy((char *)&hGuildData->abGuildPostBuffer[hGuildData->nGuildPostCursor], pszText);
	hGuildData->nGuildPostCursor += nLen + 1;
	hGuildData->dwFlags |= 16;
	return TRUE;
}

static __inline const char *GuildsFindMemberName(PTGUILDDATA hGuildData, BYTE bId)
{
	PTGUILDLISTNODE pNode;

	if (bId == 255 || bId == 0)
		return sgpszGuildPlaceholderName;
	for (pNode = hGuildData->ptMemberList; pNode != NULL; pNode = pNode->ptNext)
	{
		if (pNode->bMemberId == bId)
			return pNode->szName;
	}
	return sgpszGuildPlaceholderName;
}

DWORD __stdcall GuildsPostRead(PTGUILDDATA hGuildData, int nPos, char *pszNameOut, char *pszTextOut)
{
	BYTE bId;
	const char *pszName;
	BOOL fNoPost;

#line 1339
	D2ASSERT(hGuildData);
	fNoPost = nPos >= hGuildData->nGuildPostCursor || (nPos > 0 && hGuildData->abGuildPostBuffer[nPos - 1] != 0);
	if (fNoPost)
	{
		pszNameOut[0] = 0;
		pszTextOut[0] = 0;
		return 0;
	}

	bId = hGuildData->abGuildPostBuffer[nPos];
	pszName = GuildsFindMemberName(hGuildData, bId);
	strcpy(pszNameOut, pszName);
	nPos++;
	strcpy(pszTextOut, (const char *)&hGuildData->abGuildPostBuffer[nPos]);
	return strlen(pszTextOut) + nPos + 1;
}
#line 1363

DWORD __stdcall GuildsCopyPostBuffer(PTGUILDDATA hGuildData, void *pDst)
{
#line 1373
	D2ASSERT(hGuildData);
	if (hGuildData->nGuildPostCursor != 0)
		memcpy(pDst, hGuildData->abGuildPostBuffer, hGuildData->nGuildPostCursor);
	return (DWORD)hGuildData->nGuildPostCursor;
}

void __stdcall GuildsPostReceive(PTGUILDDATA hGuildData, WORD wSize, WORD wOffset, WORD wTotalSize,
	const void *pData)
{
#line 1393
	D2ASSERT(hGuildData);
#line 1397
	D2ASSERT(wTotalSize <= GUILD_POST_BUFFER_SIZE);

	if (hGuildData->wGuildPostTotalSize == 0 || hGuildData->wGuildPostTotalSize != wTotalSize)
	{
		hGuildData->wGuildPostReceived = 0;
		hGuildData->wGuildPostTotalSize = wTotalSize;
		hGuildData->nGuildPostCursor = 0;
	}
	if (hGuildData->wGuildPostReceived == wOffset)
	{
		memcpy(&hGuildData->abGuildPostBuffer[wOffset], pData, wSize);
		hGuildData->wGuildPostReceived += wSize;
		if (hGuildData->wGuildPostReceived == hGuildData->wGuildPostTotalSize)
			hGuildData->nGuildPostCursor = wTotalSize;
	}
}

PTGUILDLISTNODE __stdcall GuildsListGetNext(PTGUILDDATA hGuildData, PTGUILDLISTNODE ptCur)
{
#line 1424
	D2ASSERT(hGuildData);
	if (ptCur == NULL)
		return hGuildData->ptMemberList;
	return ptCur->ptNext;
}

PTGUILDLISTNODE __stdcall GuildsListFindByName(PTGUILDDATA hGuildData, const char *pszName)
{
	PTGUILDLISTNODE pNode;

#line 1443
	D2ASSERT(hGuildData);
	for (pNode = hGuildData->ptMemberList; pNode != NULL; pNode = pNode->ptNext)
	{
		if (strnicmp(pNode->szName, pszName, 16) == 0)
			return pNode;
	}
	return NULL;
}

DWORD __stdcall GuildsUnpackField(DWORD dwPacked)
{
	return (dwPacked >> 5) & 7;
}

DWORD __stdcall GuildsGetMemberRank(PTGUILDLISTNODE hMember)
{
#line 1473
	D2ASSERT(hMember);
	return (hMember->wFlags >> 5) & 7;
}

void __stdcall GuildsSetMemberRank(PTGUILDLISTNODE hMember, int nRank)
{
#line 1487
	D2ASSERT(hMember);
	if (nRank >= 0 && nRank <= 2)
		hMember->wFlags = (hMember->wFlags & 65311) | (WORD)(nRank << 5);
}

static __inline void GuildsListNodeFree(PTGUILDDATA hGuildData, PTGUILDLISTNODE pNode)
{
#line 307
	D2FREEPOOL(hGuildData->pPool, pNode);
}

void __stdcall GuildsListRemoveByName(PTGUILDDATA hGuildData, const char *pszName)
{
	PTGUILDLISTNODE pNode;
	PTGUILDLISTNODE pPrev;

#line 1505
	D2ASSERT(hGuildData);
	pPrev = NULL;
	for (pNode = hGuildData->ptMemberList; pNode != NULL; pNode = pNode->ptNext)
	{
		if (strcmpi(pNode->szName, pszName) == 0)
		{
			if (pPrev != NULL)
				pPrev->ptNext = pNode->ptNext;
			else
				hGuildData->ptMemberList = pNode->ptNext;
			GuildsListNodeFree(hGuildData, pNode);
			return;
		}
		pPrev = pNode;
	}
}

DWORD __stdcall GuildsGetMemberClass(PTGUILDLISTNODE hMember)
{
#line 1518
	D2ASSERT(hMember);
	return hMember->wFlags & 15;
}

void __stdcall GuildsMemberSnapshot(PTGUILDLISTNODE hMember, void *pDst)
{
	BYTE *pbDst;

#line 1532
	D2ASSERT(hMember);
	pbDst = (BYTE *)pDst;
	*(PTGUILDLISTNODE *)pbDst = hMember;
	*(BYTE *)(pbDst + 4) = hMember->bMemberId;
	*(BYTE *)(pbDst + 5) = hMember->field_11;
	*(WORD *)(pbDst + 6) = hMember->field_12;
	*(WORD *)(pbDst + 8) = hMember->wFlags;
	*(DWORD *)(pbDst + 12) = hMember->dwDonated;
}

void __stdcall GuildsListNodeSetFromRecord(PTGUILDLISTNODE hMember, const void *pRecord)
{
	const BYTE *pbSrc;
	const char *pszName;

#line 1551
	D2ASSERT(hMember);
	pbSrc = (const BYTE *)pRecord;
	pszName = *(const char * const *)pbSrc;

	if (strlen(pszName) + 1 < 16)
		strcpy(hMember->szName, pszName);
	hMember->field_11 = pbSrc[5];
	hMember->field_12 = *(const WORD *)(pbSrc + 6);
	hMember->wFlags = *(const WORD *)(pbSrc + 8);
	hMember->dwDonated = *(const DWORD *)(pbSrc + 12);
}

DWORD __stdcall GuildsMemberAddDonated(PTGUILDLISTNODE hMember, DWORD dwAmount)
{
	DWORD dwSum;

#line 1571
	D2ASSERT(hMember);
	dwSum = hMember->dwDonated + dwAmount;
	if (dwSum < dwAmount)
		hMember->dwDonated = 0xFFFFFFFF;
	else
		hMember->dwDonated = dwSum;
	return hMember->dwDonated;
}

DWORD __stdcall GuildsGetRank(PTGUILDDATA hGuildData)
{
#line 1590
	D2ASSERT(hGuildData);
	return hGuildData->wRank;
}

DWORD __stdcall GuildsGetRankLevelId(PTGUILDDATA hGuildData)
{
	int nRank;

#line 1606
	D2ASSERT(hGuildData);
	nRank = hGuildData->wRank;
	if (nRank >= GUILD_RANK_COUNT)
		nRank = GUILD_RANK_COUNT - 1;
	return sgtGuildRankTbl[nRank].dwLevelId;
}
