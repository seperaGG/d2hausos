#ifndef D2RE_CELLIST_H
#define D2RE_CELLIST_H

#include <windows.h>
#include <stddef.h>
#include "StormImports.h"
#include "D2Cmp.h"

#define STORMLINE_TYPE		(-2)
#define LIST_UNKNOWNOFFSET	((int)0xCCCCCCCD)
#define LIST_UNSETOFFSET	((int)0xDDDDDDDD)
#define LINKOFFSET(T, m)	((int)((INT_PTR)&((T *)16)->m - 16))
#define LINK_AFTER			1
#define LINK_BEFORE			2

class type_info
{
public:
	virtual ~type_info();
	const char *raw_name() const;
private:
	void		*_m_data;
	char		_m_d_name[1];
};

#define STORM_TYPENAME(T)	(((const char *)&typeid(T)) + 8)

#define STORM_ALLOC(nSize, pszType, dwFlags) \
	(StormSMemAlloc(nSize, pszType, STORMLINE_TYPE, \
	dwFlags))
#define STORM_REALLOC(p, nSize, pszType, dwFlags) \
	(StormSMemReAlloc(p, nSize, pszType, \
	STORMLINE_TYPE, dwFlags))
#define STORM_FREE(p, pszType) \
	(DWORD)StormSMemFree(p, pszType, STORMLINE_TYPE, 0)

#ifndef D2RE_PLACEMENT_NEW
#define D2RE_PLACEMENT_NEW
inline void * __cdecl operator new(unsigned int nSize, void *pMemory) { return pMemory; }
inline void __cdecl operator delete(void *, void *) { }
#endif

struct CELLIST;
struct TCELENTRY;
class HASHKEY_NONE;

typedef struct HCELLIST__ { int unused; } *HCELLIST;
typedef struct HLOCKEDCELLIST__ { int unused; } *HLOCKEDCELLIST;

template <class T> class TSGetLink;
template <class T> class TSGetExplicitLink;
template <class T, class TGetLink> struct TSList;

template <class T> class TSLink
{
	friend struct TSList<T, TSGetLink<T> >;
	friend struct TSList<T, TSGetExplicitLink<T> >;

public:
	TSLink<T>	*m_prevlink;
	T			*m_next;

	TSLink()
	{
		m_prevlink = NULL;
		m_next = NULL;
	}
	~TSLink()
	{
		Unlink();
	}
	T *Next()
	{
		if ((INT_PTR)m_next > 0)
			return m_next;
		return NULL;
	}
private:
	TSLink<T> *NextLink(int nLinkOffset) const
	{
		if ((INT_PTR)m_next < 0)
			return (TSLink<T> *)~(INT_PTR)m_next;
		if (nLinkOffset < 0)
			nLinkOffset = (int)((BYTE *)this - (BYTE *)m_prevlink->m_next);
		return (TSLink<T> *)((BYTE *)m_next + nLinkOffset);
	}
public:
	void Unlink()
	{
		if (m_prevlink)
		{
			NextLink(-1)->m_prevlink = m_prevlink;
			m_prevlink->m_next = m_next;
			m_prevlink = NULL;
			m_next = NULL;
		}
	}
};

template <class T> class TSGetExplicitLink
{
public:
	static TSLink<T> *Link(const void *ptElement, const int nLinkOffset)
	{
		return (TSLink<T> *)((BYTE *)ptElement + nLinkOffset);
	}
};

template <class T> class TSGetLink
{
public:
	static TSLink<T> *Link(const void *ptElement, const int nLinkOffset)
	{
		return (TSLink<T> *)ptElement;
	}
};

template <class T, class TGetLink> struct TSList
{
	int			m_linkoffset;
	mutable TSLink<T>	m_terminator;

	TSList()
	{
		StartEmpty();
	}
	void StartEmpty()
	{
		m_linkoffset = 0;
		InitializeTerminator();
	}
	void StartAt(int nLinkOffset)
	{
		m_linkoffset = nLinkOffset;
		InitializeTerminator();
	}
	TSList(const TSList<T, TGetLink> &tList)
	{
		m_linkoffset = tList.m_linkoffset;
		InitializeTerminator();
	}
	~TSList()
	{
		UnlinkAll();
	}
private:
	void InitializeTerminator()
	{
		m_terminator.m_prevlink = &m_terminator;
		m_terminator.m_next = (T *)~(INT_PTR)&m_terminator;
	}
public:
	void ChangeLinkOffset(int nLinkOffset)
	{
		if (nLinkOffset == m_linkoffset)
			return;
		UnlinkAll();
		StartAt(nLinkOffset);
	}
	T *Head()
	{
		return m_terminator.Next();
	}
private:
	TSLink<T> *Link(const T *ptElement) const
	{
		if (ptElement)
			return TGetLink::Link(ptElement, m_linkoffset);
		return &m_terminator;
	}
public:
	T *LinkNext(const T *ptElement)
	{
		return Link(ptElement)->m_next;
	}
	void UnlinkAll()
	{
		T *ptElement;

		while ((ptElement = Head()))
			UnlinkNode(ptElement);
	}
	void UnlinkNode(T *ptElement) { Link(ptElement)->Unlink(); }
	void LinkNode(T *ptElement, int nWhere, T *ptRelative)
	{
		TSLink<T> *ptLink;
		TSLink<T> *ptRelativeLink;
		TSLink<T> *ptNeighbour;

		ptLink = ptElement ? Link(ptElement) : &m_terminator;
		if (ptLink->m_prevlink)
			ptLink->Unlink();
		if (ptRelative)
			ptRelativeLink = Link(ptRelative);
		else
			ptRelativeLink = &m_terminator;
		switch (nWhere)
		{
		case LINK_AFTER:
			ptLink->m_prevlink = ptRelativeLink;
			ptLink->m_next = ptRelativeLink->m_next;
			ptNeighbour = ptRelativeLink->NextLink(m_linkoffset);
			ptNeighbour->m_prevlink = ptLink;
			ptRelativeLink->m_next = ptElement;
			break;
		default:
			ptNeighbour = ptRelativeLink->m_prevlink;
			ptLink->m_prevlink = ptNeighbour;
			ptLink->m_next = ptNeighbour->m_next;
			ptNeighbour->m_next = ptElement;
			ptRelativeLink->m_prevlink = ptLink;
			break;
		}
	}
	T *NewNode(DWORD nWhere, DWORD dwExtraBytes, DWORD dwFlags)
	{
		T *ptElement;

		ptElement = new (STORM_ALLOC(sizeof(T) + dwExtraBytes, STORM_TYPENAME(T), dwFlags | 8)) T;
		if (nWhere)
			LinkNode(ptElement, nWhere, NULL);
		return ptElement;
	}
	T *FreeElement(T *ptElement)
	{
		T *ptNext = Link(ptElement)->Next();
		delete ptElement;
		return ptNext;
	}
	void DeleteAll()
	{
		T *ptElement;

		while ((ptElement = Head()))
		{
			ptElement->~T();
			STORM_FREE(ptElement, STORM_TYPENAME(T));
		}
	}
};

template <class T, int nExplicitOffset> class TSExplicitList : public TSList<T, TSGetExplicitLink<T> >
{
public:
	TSExplicitList()
	{
		this->StartAt(nExplicitOffset);
	}
};

template <class T> class TSCD
{
public:
	static void Construct(T *ptObject)
	{
		new (ptObject) T;
	}
	static void CopyConstruct(T *ptObject, const T *ptSource)
	{
		new (ptObject) T(*ptSource);
	}
};

template <class T> class TSArrayBase
{
public:
	DWORD		m_alloc;
	DWORD		m_count;
	T			*m_data;

	void Constructor() { m_alloc = 0; m_count = 0; m_data = NULL; }
	DWORD Count() { return m_count; }
	T *Ptr() { return m_data; }
	T &operator[](DWORD i) { return m_data[i]; }
	static void Destruct(T *ptObject) { ptObject->~T(); }
};

template <class T, class TCD> class TSFixedArray : public TSArrayBase<T>
{
public:
	~TSFixedArray()
	{
		DWORD i;

		for (i = 0; i < Count(); i++)
			Destruct(&(*this)[i]);
		if (m_data)
			STORM_FREE(Ptr(), STORM_TYPENAME(T));
	}
	void Clear()
	{
		this->~TSFixedArray<T, TCD>();
		Constructor();
	}
};

template <class T, class TCD = TSCD<T> > class TSGrowableArray : public TSFixedArray<T, TCD>
{
public:
	DWORD		m_chunk;

	TSGrowableArray()
	{
		m_alloc = 0;
		m_count = 0;
		m_data = NULL;
		m_chunk = 0;
	}
	void SetCount(UINT dwCount)
	{
		DWORD i;

		if (dwCount > m_count)
		{
			SGrowBy(this, dwCount - m_count, TRUE);
			for (i = m_count; i < dwCount; i++)
				TCD::Construct(&m_data[i]);
		}
		else if (dwCount < m_count)
		{
			for (i = dwCount; i < m_count; i++)
				m_data[i].~T();
		}
		m_count = dwCount;
	}
};

template <class T, class TCD> inline void SReallocData(TSGrowableArray<T, TCD> *ptArray, DWORD dwAlloc)
{
	T *ptOldData;
	DWORD dwMove;
	DWORD i;

	ptOldData = ptArray->m_data;
	for (i = dwAlloc; i < ptArray->m_count; i++)
		ptOldData[i].~T();
	ptArray->m_alloc = dwAlloc;
	ptArray->m_data = (T *)STORM_REALLOC(ptOldData, dwAlloc * sizeof(T), STORM_TYPENAME(T), 16);
	if (!ptArray->m_data)
	{
		ptArray->m_data = (T *)STORM_ALLOC(dwAlloc * sizeof(T), STORM_TYPENAME(T), 0);
		if (ptOldData)
		{
			dwMove = dwAlloc < ptArray->m_count ? dwAlloc : ptArray->m_count;
			for (i = 0; i < dwMove; i++)
			{
				TCD::CopyConstruct(&ptArray->m_data[i], &ptOldData[i]);
				ptOldData[i].~T();
			}
			STORM_FREE(ptOldData, STORM_TYPENAME(T));
		}
	}
}

template <class T, class TCD> inline DWORD SCalcChunkSize(TSGrowableArray<T, TCD> *ptArray, DWORD dwCount)
{
	if (dwCount < 256 / sizeof(T))
	{
		while (dwCount & (dwCount - 1))
			dwCount &= dwCount - 1;
		if (dwCount < 1)
			return 1;
	}
	else
	{
		ptArray->m_chunk = 256 / sizeof(T);
		dwCount = 256 / sizeof(T);
	}
	return dwCount;
}

inline DWORD SRoundUpTo(DWORD dwValue, DWORD dwChunk)
{
	DWORD dwRest;

	dwRest = dwValue % dwChunk;
	if (dwRest)
		dwValue += dwChunk - dwRest;
	return dwValue;
}

template <class T, class TCD> inline DWORD SWholeChunks(TSGrowableArray<T, TCD> *ptArray, DWORD dwCount)
{
	DWORD dwChunk;

	dwChunk = ptArray->m_chunk ? ptArray->m_chunk : SCalcChunkSize(ptArray, dwCount);
	return SRoundUpTo(dwCount, dwChunk);
}

template <class T, class TCD> inline void SGrowBy(TSGrowableArray<T, TCD> *ptArray, DWORD dwCount, BOOL bRound)
{
	dwCount += ptArray->m_count;
	if (dwCount > ptArray->m_alloc)
	{
		if (bRound)
			dwCount = SWholeChunks(ptArray, dwCount);
		SReallocData(ptArray, dwCount);
	}
}

class HASHKEY_NONE
{
public:
	BYTE		unobserved_00;

	BOOL operator==(const HASHKEY_NONE &tKey) const { return TRUE; }
};

template <class T, class K> struct TSHashObject
{
	DWORD		m_hashval;
	TSLink<T>	m_linktoslot;
	TSLink<T>	m_linktofull;
	K			m_key;
};

template <class T> class TSLinkedNode
{
public:
	TSLink<T>	m_link;

	~TSLinkedNode()
	{
		m_link.Unlink();
	}
};

struct TCELENTRY : TSLinkedNode<TCELENTRY>
{
	DWORD		field_08;
	DWORD		field_0c;
	DWORD		field_10;
	DWORD		field_14;
	DWORD		field_18;
	DWORD		field_1c;
	BYTE		*field_20;
};

#define CELLIST_24BIT		4

struct CELLIST : TSHashObject<CELLIST, HASHKEY_NONE>
{
	DWORD		dwFlags;
	DWORD		format;
	DWORD		dwKeyColor;
	DWORD		nCels;
	DWORD		dwNumDirections;
	TSList<TCELENTRY, TSGetLink<TCELENTRY> >	tEntries;
};

template <class T, class K> class TSHashObjectChunk
{
public:
	TSGrowableArray<T>	m_array;
	TSLink< TSHashObjectChunk<T, K> >	m_link;
};

template <class T, class K> struct TSHashTable
{
	TSExplicitList<T, LIST_UNSETOFFSET>	m_fulllist;
	DWORD		m_fullness;
	TSGrowableArray< TSExplicitList<T, LIST_UNSETOFFSET> >	m_slotlistarray;
	DWORD		m_hashmask;

	void SetSlotCount(DWORD dwSlots) { m_slotlistarray.SetCount(dwSlots); }
	TSHashTable()
	{
		DWORD i;

		m_fullness = 0;
		m_fulllist.ChangeLinkOffset(LINKOFFSET(T, m_linktofull));
		m_hashmask = 3;
		SetSlotCount(4);
		for (i = 0; i <= m_hashmask; i++)
			m_slotlistarray.m_data[i].ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
	}
private:
	virtual void InternalDelete(T *ptElement)
	{
		ptElement->~T();
		STORM_FREE(ptElement, STORM_TYPENAME(T));
	}
	virtual T *InternalNew(TSExplicitList<T, LIST_UNSETOFFSET> *ptList, DWORD dwExtraBytes,
		DWORD dwFlags)
	{
		return ptList->NewNode(LINK_AFTER, dwExtraBytes, dwFlags);
	}
public:
	virtual ~TSHashTable()
	{
		EmptyLists(TRUE);
	}
	virtual void Destroy()
	{
		EmptyLists(TRUE);
		m_fullness = 0;
		m_hashmask = 0;
		m_slotlistarray.Clear();
	}
	T *New(UINT dwHashVal, const K &tKey, DWORD dwExtraBytes, DWORD dwFlags)
	{
		T *ptElement;
		DWORD dwSlot;

		dwSlot = dwHashVal & m_hashmask;
		if (m_hashmask < 1023)
		{
			if (m_fullness > 3)
				m_fullness -= 3;
			else
				m_fullness = 0;
			for (T *ptEntry = m_slotlistarray[dwSlot].Head(), *ptDelete = NULL; (INT_PTR)ptEntry > 0;
				ptDelete
					? (ptEntry = m_slotlistarray[dwSlot].FreeElement(ptEntry),
						ptEntry = (INT_PTR)ptDelete > 0 ? ptEntry : NULL,
						ptDelete = NULL,
						ptEntry)
					: ptEntry = m_slotlistarray[dwSlot].LinkNext(ptEntry))
			{
				if (++m_fullness > 13)
				{
					m_fullness = 0;
					Rehash();
					dwSlot = dwHashVal & m_hashmask;
					break;
				}
			}
		}
		ptElement = InternalNew(&m_slotlistarray[dwSlot], dwExtraBytes, dwFlags);
		m_fulllist.LinkNode(ptElement, LINK_BEFORE, NULL);
		ptElement->m_hashval = dwHashVal;
		ptElement->m_key = tKey;
		return ptElement;
	}
	T *Ptr(UINT dwHashVal, const K &tKey)
	{
		for (T *ptElement = m_slotlistarray[dwHashVal & m_hashmask].Head(), *ptDelete = NULL; (INT_PTR)ptElement > 0;
			ptDelete
				? (ptElement = m_slotlistarray[dwHashVal & m_hashmask].FreeElement(ptElement),
					ptElement = (INT_PTR)ptDelete > 0 ? ptElement : NULL,
					ptDelete = NULL,
					ptElement)
				: ptElement = m_slotlistarray[dwHashVal & m_hashmask].LinkNext(ptElement))
		{
			DWORD dwElementHash = ptElement->m_hashval;

			if (dwElementHash == dwHashVal && ptElement->m_key == tKey)
				return ptElement;
		}
		return NULL;
	}
	void Rehash() { GrowListArray(); }
	DWORD ComputeSlot(DWORD dwHashVal) { return dwHashVal & m_hashmask; }
private:
	void GrowListArray()
	{
		T *ptElement;
		DWORD dwOldSize;
		DWORD dwNewSize;
		DWORD i;
		DWORD dwSlot;

		dwOldSize = m_hashmask + 1;
		dwNewSize = 2 * dwOldSize;
		TSExplicitList<T, LIST_UNSETOFFSET> tList;
		tList.ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
		for (i = 0; i < dwOldSize; i++)
		{
			while ((ptElement = m_slotlistarray[i].m_terminator.Next()) != NULL)
				tList.LinkNode(ptElement, LINK_BEFORE, NULL);
		}
		SetSlotCount(dwNewSize);
		for (i = 0; i < dwNewSize; i++)
			m_slotlistarray[i].ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
		m_hashmask = dwNewSize - 1;
		while ((ptElement = tList.m_terminator.Next()) != NULL)
		{
			dwSlot = ComputeSlot(ptElement->m_hashval);
			m_slotlistarray[dwSlot].LinkNode(ptElement, LINK_BEFORE, NULL);
		}
	}
public:
	void EmptyLists(BOOL fUnlinkOnly)
	{
		T *ptElement;
		DWORD i;

		m_fullness = 0;
		m_fulllist.UnlinkAll();
		for (i = 0; i < m_slotlistarray.Count(); i++)
		{
			while ((ptElement = m_slotlistarray[i].Head()))
			{
				if (fUnlinkOnly)
					m_slotlistarray[i].UnlinkNode(ptElement);
				else
					InternalDelete(ptElement);
			}
		}
	}
	void UnlinkElement(T *ptElement)
	{
		if (ptElement->m_linktoslot.m_next)
		{
			ptElement->m_linktoslot.Unlink();
			ptElement->m_linktofull.Unlink();
		}
	}
	void Delete(T *ptElement)
	{
		UnlinkElement(ptElement);
		InternalDelete(ptElement);
	}
};

template <class T, class K, int N> struct TSHashTableReuse : TSHashTable<T, K>
{
	TSExplicitList<T, LIST_UNSETOFFSET>	m_reuselist;
	DWORD		m_chunksize;
	TSExplicitList<TSHashObjectChunk<T, K>, 16>	m_chunklist;

	TSHashTableReuse()
	{
		m_chunksize = 16;
		m_reuselist.ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
	}
private:
	virtual void InternalDelete(T *ptElement)
	{
		ptElement->~T();
		STORM_FREE(ptElement, STORM_TYPENAME(T));
	}
	virtual T *InternalNew(TSExplicitList<T, LIST_UNSETOFFSET> *ptList, DWORD dwExtraBytes,
		DWORD dwFlags)
	{
		if (N)
		{
			m_chunklist.NewNode(LINK_BEFORE, 0, 0);
			m_reuselist.LinkNode(NULL, LINK_BEFORE, NULL);
		}
		return ptList->NewNode(LINK_AFTER, dwExtraBytes, dwFlags);
	}
public:
	virtual ~TSHashTableReuse()
	{
		FreeReuse();
	}
	virtual void Destroy()
	{
		this->EmptyLists(FALSE);
		FreeReuse();
	}
	void FreeReuse()
	{
		m_chunklist.DeleteAll();
		m_reuselist.DeleteAll();
	}
};

template <class T, class TExport, int N> struct TSExportTableSimple : TSHashTableReuse<T, HASHKEY_NONE, N>
{
	HASHKEY_NONE	m_key;
	DWORD		m_sequence;
	BOOL		m_wrapped;

	TSExportTableSimple()
	{
		m_sequence = ~(DWORD)(INT_PTR)this & 268435455;
		m_wrapped = FALSE;
	}
	DWORD GenerateUniqueHandle()
	{
		for (;;)
		{
			if (!++m_sequence)
			{
				m_wrapped = TRUE;
				continue;
			}
			if (!m_wrapped || !Ptr(m_sequence))
				break;
		}
		return m_sequence;
	}
	T *Ptr(DWORD dwHandle)
	{
		return TSHashTable<T, HASHKEY_NONE>::Ptr(dwHandle, m_key);
	}
	T *New(DWORD *pdwHandle)
	{
		*pdwHandle = GenerateUniqueHandle();
		return TSHashTable<T, HASHKEY_NONE>::New(*pdwHandle, m_key, 0, 0);
	}
	void Delete(T *ptElement)
	{
		TSHashTable<T, HASHKEY_NONE>::Delete(ptElement);
	}
};

class CCritSect
{
public:
	CRITICAL_SECTION	m_critsect;

	CCritSect() { InitializeCriticalSection(&m_critsect); }
	~CCritSect() { DeleteCriticalSection(&m_critsect); }
	void Enter(BOOL fForWriting) { EnterCriticalSection(&m_critsect); }
	void Leave(BOOL fForWriting) { LeaveCriticalSection(&m_critsect); }
};

template <class T, class TExport, class TLockedExport, class TLock, int N> struct TSExportTableSync
	: TSExportTableSimple<T, TExport, N>
{
	TLock		m_sync;

	TSExportTableSync() { }
	BOOL IsWriteLock(int nLocked) { return nLocked == 1; }
	void SyncEnterLock(int *pnLocked, BOOL fForWriting)
	{
		m_sync.Enter(fForWriting);
		*pnLocked = fForWriting ? 1 : -1;
	}
	void SyncLeaveLock(int nLocked)
	{
		if (nLocked)
			m_sync.Leave(IsWriteLock(nLocked));
	}
	T *Lock(DWORD dwHandle, int *pnLocked, BOOL fForWriting)
	{
		T *ptElement;

		SyncEnterLock(pnLocked, fForWriting);
		ptElement = this->Ptr(dwHandle);
		if (!ptElement)
		{
			SyncLeaveLock(*pnLocked);
			*pnLocked = 0;
		}
		return ptElement;
	}
	T *NewLock(DWORD *pdwHandle, int *pnLocked)
	{
		SyncEnterLock(pnLocked, TRUE);
		return this->New(pdwHandle);
	}
	void Unlock(int nLocked)
	{
		SyncLeaveLock(nLocked);
	}
	void DeleteLocked(T *ptElement, int nLocked)
	{
		this->Delete(ptElement);
		SyncLeaveLock(nLocked);
	}
};

void __fastcall FastCmpBuildCelFile(CELLIST *ptList, D2CELFILE *pFile);
void __fastcall RawBuildCelFile(CELLIST *ptList, D2CELFILE *pFile);

#endif
