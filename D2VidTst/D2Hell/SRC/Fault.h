#ifndef D2RE_D2VIDTST_FAULT_H
#define D2RE_D2VIDTST_FAULT_H

typedef BOOL (__stdcall *PFNMESSAGESOURCE)(DWORD dwError, char *pszBuffer, int nSize);
typedef void (__stdcall *PFNASSERTHOOK)(const char *pszFile, int nLine,
	const char *pszExpression);

inline void * __cdecl operator new(unsigned int nSize, void *pMemory) { return pMemory; }
inline void __cdecl operator delete(void *, void *) { }

#define STORMLINE_TYPE		(-2)
#define LINK_AFTER			1

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

#define STORM_FREE(p, pszType) \
	(DWORD)StormSMemFree(p, pszType, STORMLINE_TYPE, 0)

template <class T> struct TSLink
{
	TSLink<T>	*m_prevlink;
	T			*m_next;

	TSLink() { m_prevlink = NULL; m_next = NULL; }
	~TSLink() { Unlink(); }
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

template <class T> struct TSList
{
	int			m_linkoffset;
	TSLink<T>	m_terminator;

	TSList()
	{
		m_linkoffset = 0;
		InitializeTerminator();
	}
	void InitializeTerminator()
	{
		m_terminator.m_prevlink = &m_terminator;
		m_terminator.m_next = (T *)~(INT_PTR)&m_terminator;
	}
	void InitLinkOffset(int nLinkOffset)
	{
		m_linkoffset = nLinkOffset;
		InitializeTerminator();
	}
	~TSList()
	{
		UnlinkAll();
	}
	T *Head()
	{
		if ((INT_PTR)m_terminator.m_next > 0)
			return m_terminator.m_next;
		return NULL;
	}
	TSLink<T> *Link(const T *ptElement)
	{
		if (ptElement)
			return (TSLink<T> *)((BYTE *)ptElement + m_linkoffset);
		return &m_terminator;
	}
	void DeleteAll()
	{
		T *ptElement;

		while ((ptElement = Head()) != NULL)
		{
			ptElement->~T();
			STORM_FREE(ptElement, STORM_TYPENAME(T));
		}
	}
	void LinkNode(T *ptElement, int nWhere, T *ptRelative)
	{
		TSLink<T> *ptLink;
		TSLink<T> *ptRelativeLink;

		ptLink = Link(ptElement);
		if (ptLink->m_prevlink)
			ptLink->Unlink();
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
		{
			TSLink<T> *ptPrev = ptRelativeLink->m_prevlink;

			ptLink->m_prevlink = ptPrev;
			ptLink->m_next = ptPrev->m_next;
			ptPrev->m_next = ptElement;
			ptRelativeLink->m_prevlink = ptLink;
		}
		}
	}
	T *NewNode(int nWhere, DWORD dwExtraBytes, DWORD dwFlags)
	{
		T *ptElement;

		ptElement = new (StormSMemAlloc(sizeof(T) + dwExtraBytes, STORM_TYPENAME(T), STORMLINE_TYPE, dwFlags | 8)) T;
		if (nWhere)
			LinkNode(ptElement, nWhere, NULL);
		return ptElement;
	}
	void UnlinkAll()
	{
		T *ptElement;

		while ((ptElement = Head()) != NULL)
			Link(ptElement)->Unlink();
	}
};

template <class T, int nExplicitOffset> struct TSExplicitList : TSList<T>
{
	TSExplicitList()
	{
		InitLinkOffset(nExplicitOffset);
	}
};

struct TMESSAGESOURCE
{
 	TSLink<TMESSAGESOURCE>	m_link;
 	PFNMESSAGESOURCE	pfnMessageSource;
};

#endif
