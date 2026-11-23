#ifndef D2RE_CELLIST_H
#define D2RE_CELLIST_H

#include <windows.h>
#include <stddef.h>
#include "StormImports.h"
#include "D2Cmp.h"
#include "StormNew.h"

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

class HASHKEY_NONE;

template <class T> struct TSLink
{
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
	TSLink<T> *NextLink(int nLinkOffset)
	{
		if ((INT_PTR)m_next < 0)
			return (TSLink<T> *)~(INT_PTR)m_next;
		if (nLinkOffset < 0)
			nLinkOffset = (int)((BYTE *)this - (BYTE *)m_prevlink->m_next);
		return (TSLink<T> *)((BYTE *)m_next + nLinkOffset);
	}
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

template <class T> struct TSLinkPlace
{
	static TSLink<T> *At(T *ptElement, int nLinkOffset)
	{
		return (TSLink<T> *)((BYTE *)ptElement + nLinkOffset);
	}
};

template <class T, int nOffset> struct TSList
{
	int			m_linkoffset;
	TSLink<T>	m_terminator;

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
	TSList(const TSList<T, nOffset> &tList)
	{
		m_linkoffset = tList.m_linkoffset;
		InitializeTerminator();
	}
	~TSList()
	{
		UnlinkAll();
	}
	int LinkOffset() { return nOffset == LIST_UNKNOWNOFFSET ? m_linkoffset : nOffset; }
	void InitializeTerminator()
	{
		m_terminator.m_prevlink = &m_terminator;
		m_terminator.m_next = (T *)~(INT_PTR)&m_terminator;
	}
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
	TSLink<T> *Link(const T *ptElement)
	{
		if (ptElement)
			return TSLinkPlace<T>::At((T *)ptElement, m_linkoffset);
		return &m_terminator;
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
		TSLink<T> *ptPrevLink;

		ptLink = Link(ptElement);
		ptPrevLink = ptLink->m_prevlink;
		if (ptPrevLink)
		{
			ptLink->NextLink(-1)->m_prevlink = ptPrevLink;
			ptLink->m_prevlink->m_next = ptLink->m_next;
			ptLink->m_prevlink = NULL;
			ptLink->m_next = NULL;
		}
		ptRelativeLink = Link(ptRelative);
		switch (nWhere)
		{
		case LINK_AFTER:
			ptLink->m_prevlink = ptRelativeLink;
			ptLink->m_next = ptRelativeLink->m_next;
			ptRelativeLink->NextLink(m_linkoffset)->m_prevlink = ptLink;
			ptRelativeLink->m_next = ptElement;
			break;
		default:
			ptPrevLink = ptRelativeLink->m_prevlink;
			ptLink->m_prevlink = ptPrevLink;
			ptLink->m_next = ptPrevLink->m_next;
			ptPrevLink->m_next = ptElement;
			ptRelativeLink->m_prevlink = ptLink;
			break;
		}
	}
	T *NewNode(int nWhere, DWORD dwExtraBytes, DWORD dwFlags)
	{
		T *ptElement;

		ptElement = new (STORM_ALLOC(sizeof(T) + dwExtraBytes, STORM_TYPENAME(T), dwFlags | 8)) T;
		if (nWhere)
			LinkNode(ptElement, nWhere, NULL);
		return ptElement;
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

template <class T, int nExplicitOffset> class TSExplicitList : public TSList<T, LIST_UNKNOWNOFFSET>
{
public:
	TSExplicitList()
	{
		this->StartAt(nExplicitOffset);
	}
};

template <class T>
void SConstruct(T *ptObject)
{
	new (ptObject) T;
}

template <class T>
void SCopyConstruct(T *ptObject, const T &tSource)
{
	new (ptObject) T(tSource);
}

template <class T> struct TSArrayBase
{
	DWORD		m_alloc;
	DWORD		m_count;
	T			*m_data;

	void Constructor() { m_alloc = 0; m_count = 0; m_data = NULL; }
	DWORD Count() { return m_count; }
	T *Ptr() { return m_data; }
	T &operator[](DWORD i) { return m_data[i]; }
	static void Destruct(T *ptObject) { ptObject->~T(); }
};

template <class T> struct TSOwnedArray : TSArrayBase<T>
{
	~TSOwnedArray()
	{
		DWORD i;

		for (i = 0; i < Count(); i++)
			Destruct(&(*this)[i]);
		if (m_data)
			STORM_FREE(Ptr(), STORM_TYPENAME(T));
	}
	void Clear()
	{
		this->~TSOwnedArray<T>();
		Constructor();
	}
};

template <class T> struct TSGrowableArray : TSOwnedArray<T>
{
	DWORD		m_chunk;

	TSGrowableArray()
	{
		m_alloc = 0;
		m_count = 0;
		m_data = NULL;
		m_chunk = 0;
	}
	void SetCount(DWORD dwCount)
	{
		DWORD i;

		if (dwCount > m_count)
		{
			SGrowBy(this, dwCount - m_count, TRUE);
			for (i = m_count; i < dwCount; i++)
				SConstruct(&m_data[i]);
		}
		else if (dwCount < m_count)
		{
			for (i = dwCount; i < m_count; i++)
				m_data[i].~T();
		}
		m_count = dwCount;
	}
};

template <class T> inline void SReallocData(TSGrowableArray<T> *ptArray, DWORD dwAlloc)
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
				SCopyConstruct(&ptArray->m_data[i], ptOldData[i]);
				ptOldData[i].~T();
			}
			STORM_FREE(ptOldData, STORM_TYPENAME(T));
		}
	}
}

template <class T> inline DWORD SCalcChunkSize(TSGrowableArray<T> *ptArray, DWORD dwCount)
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

template <class T> inline DWORD SWholeChunks(TSGrowableArray<T> *ptArray, DWORD dwCount)
{
	DWORD dwChunk;

	dwChunk = ptArray->m_chunk ? ptArray->m_chunk : SCalcChunkSize(ptArray, dwCount);
	return SRoundUpTo(dwCount, dwChunk);
}

template <class T> inline void SGrowBy(TSGrowableArray<T> *ptArray, DWORD dwCount, BOOL bRound)
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
	virtual void DestroyElement(T *ptElement)
	{
		ptElement->~T();
		STORM_FREE(ptElement, STORM_TYPENAME(T));
	}
	virtual T *CreateElement(TSExplicitList<T, LIST_UNSETOFFSET> *ptList, DWORD dwExtraBytes,
		DWORD dwFlags)
	{
		return ptList->NewNode(LINK_AFTER, dwExtraBytes, dwFlags);
	}
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
	T *Ptr(DWORD dwHashVal, const K *ptKey)
	{
		T *ptElement;

		ptElement = m_slotlistarray.m_data[dwHashVal & m_hashmask].Head();
		while ((INT_PTR)ptElement > 0)
		{
			if (ptElement->m_hashval == dwHashVal && ptElement->m_key == *ptKey)
				return ptElement;
			ptElement = m_slotlistarray.m_data[dwHashVal & m_hashmask].Link(ptElement)->m_next;
		}
		return NULL;
	}
	T *AddHashElement(DWORD dwHashVal, const K *ptKey, DWORD dwExtraBytes, DWORD dwFlags)
	{
		T *ptElement;
		DWORD dwSlot;

		dwSlot = dwHashVal & m_hashmask;
		if (m_hashmask < 1023)
		{
			m_fullness = m_fullness > 3 ? m_fullness - 3 : 0;
			ptElement = m_slotlistarray.m_data[dwSlot].m_terminator.Next();
			while ((INT_PTR)ptElement > 0)
			{
				if (++m_fullness > 13)
				{
					m_fullness = 0;
					GrowListArray();
					dwSlot = dwHashVal & m_hashmask;
					break;
				}
				ptElement = m_slotlistarray.m_data[dwSlot].Link(ptElement)->m_next;
			}
		}
		ptElement = CreateElement(&m_slotlistarray.m_data[dwSlot], dwExtraBytes, dwFlags);
		m_fulllist.LinkNode(ptElement, LINK_BEFORE, NULL);
		ptElement->m_hashval = dwHashVal;
		ptElement->m_key = *ptKey;
		return ptElement;
	}
	void GrowListArray()
	{
		TSExplicitList<T, LIST_UNSETOFFSET> tList;
		T *ptElement;
		DWORD dwOldSize;
		DWORD dwNewSize;
		DWORD i;

		dwOldSize = m_hashmask + 1;
		dwNewSize = 2 * dwOldSize;
		tList.ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
		for (i = 0; i < dwOldSize; i++)
		{
			while ((ptElement = m_slotlistarray.m_data[i].m_terminator.Next()) != NULL)
				tList.LinkNode(ptElement, LINK_BEFORE, NULL);
		}
		SetSlotCount(dwNewSize);
		for (i = 0; i < dwNewSize; i++)
			m_slotlistarray.m_data[i].ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
		m_hashmask = dwNewSize - 1;
		while ((ptElement = tList.m_terminator.Next()) != NULL)
			m_slotlistarray.m_data[ptElement->m_hashval & m_hashmask].LinkNode(ptElement, LINK_BEFORE, NULL);
	}
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
					DestroyElement(ptElement);
			}
		}
	}
	void Delete(T *ptElement)
	{
		if (ptElement->m_linktoslot.m_next)
		{
			ptElement->m_linktoslot.Unlink();
			ptElement->m_linktofull.Unlink();
		}
		DestroyElement(ptElement);
	}
};

template <class T, class K> struct TSHashTableReuse : TSHashTable<T, K>
{
	TSExplicitList<T, LIST_UNSETOFFSET>	m_reuselist;
	DWORD		m_chunksize;
	TSExplicitList<TSHashObjectChunk<T, K>, 16>	m_chunklist;

	TSHashTableReuse()
	{
		m_chunksize = 16;
		m_reuselist.ChangeLinkOffset(LINKOFFSET(T, m_linktoslot));
	}
	virtual void DestroyElement(T *ptElement)
	{
		ptElement->~T();
		STORM_FREE(ptElement, STORM_TYPENAME(T));
	}
	virtual T *CreateElement(TSExplicitList<T, LIST_UNSETOFFSET> *ptList, DWORD dwExtraBytes,
		DWORD dwFlags)
	{
		return ptList->NewNode(LINK_AFTER, dwExtraBytes, dwFlags);
		m_reuselist.LinkNode(NULL, LINK_BEFORE, NULL);
		m_chunklist.NewNode(LINK_BEFORE, 0, 0);
	}
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

template <class T, class K> struct TSExportTable : TSHashTableReuse<T, K>
{
	K			m_key;
	DWORD		m_sequence;
	BOOL		m_wrapped;

	TSExportTable()
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
		return TSHashTable<T, K>::Ptr(dwHandle, &m_key);
	}
	T *New(DWORD *pdwHandle)
	{
		*pdwHandle = GenerateUniqueHandle();
		return this->AddHashElement(*pdwHandle, &m_key, 0, 0);
	}
	void Delete(T *ptElement)
	{
		TSHashTable<T, K>::Delete(ptElement);
	}
};

class CCritSect
{
public:
	CRITICAL_SECTION	m_critsect;

	CCritSect() { InitializeCriticalSection(&m_critsect); }
	~CCritSect() { DeleteCriticalSection(&m_critsect); }
	void Enter() { EnterCriticalSection(&m_critsect); }
	void Leave() { LeaveCriticalSection(&m_critsect); }
};

template <class T, class K> struct TSExportTableSync : TSExportTable<T, K>
{
	CCritSect	m_sync;

	TSExportTableSync() { }
	void SyncEnterLock(int *pnLocked, BOOL fForWriting)
	{
		m_sync.Enter();
		*pnLocked = fForWriting ? 1 : -1;
	}
	void SyncLeaveLock(int nLocked)
	{
		if (nLocked)
			m_sync.Leave();
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
};

#endif
