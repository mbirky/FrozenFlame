//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkListBare.h

#ifndef _AKLISTBARE_H
#define _AKLISTBARE_H

// this one lets you define the structure
// only requirement is that T must have member pNextItem (if using the default AkListBareNextItem policy struct).
// client is responsible for allocation/deallocation of T.

// WATCH OUT !
// - remember that removeall/term can't delete the elements for you.
// - be sure to destroy elements AFTER removing them from the list, as remove will
// access members of the element.

/* Usage: List of AkMyType.

- With default AkMyType::pNextItem, no count (Length() is not available):
typedef AkListBare<AkMyType> AkMyList1;

- With custom AkMyType::pNextItemCustom, no count:
struct AkListBareNextItemCustom
{
	static AkForceInline AkMyType *& Get( AkMyType * in_pItem ) 
	{
		return in_pItem->pNextItemCustom;
	}
};
typedef AkListBare<AkMyType,AkListBareNextItemCustom> AkMyList2;

- With default AkMyType::pNextItem, WITH count (Length() is available):
typedef AkListBare<AkMyType,AkListBareNextItem,AkCountPolicyWithCount> AkMyList3;

*/

//
// List bare policy classes.
//

/// Next item name policy.
template <class T> struct AkListBareNextItem
{
	/// Default policy.
	static AkForceInline T *& Get( T * in_pItem ) 
	{
		return in_pItem->pNextItem;
	}
};

/// Item count policy. These policy classes must define protected methods 
/// ResetCount(), IncrementCount(), DecrementCount() and optionally, public unsigned int Length() const.
template <class T> 
class AkCountPolicyNoCount
{
protected:
	AkForceInline void ResetCount( T* ) {}
	AkForceInline void IncrementCount( T* ) {}
	AkForceInline void DecrementCount( T* ) {}
};

template <class T> 
class AkCountPolicyWithCount
{
public:
	/// Get list length.
	AkForceInline unsigned int Length() const
	{
		return m_ulNumListItems;
	}

protected:
	AkCountPolicyWithCount() :m_ulNumListItems( 0 ) {}

	AkForceInline void ResetCount( T* ) { m_ulNumListItems = 0; }
	AkForceInline void IncrementCount( T* ) { ++m_ulNumListItems; }
	AkForceInline void DecrementCount( T* ) { --m_ulNumListItems; }

private:
	unsigned int	m_ulNumListItems;			///< how many we have
};


/// Implementation of List Bare.
template <class T, class U_NEXTITEM = AkListBareNextItem< T >, template <class> class COUNT_POLICY = AkCountPolicyNoCount > class AkListBare : public COUNT_POLICY<T>
{
public:
	/// Iterator.
	struct Iterator
	{
		T* pItem;	///< Next item.

		/// Operator ++.
		inline Iterator& operator++()
		{
			AKASSERT( pItem );
			pItem = U_NEXTITEM::Get( pItem );
			return *this;
		}

		/// Operator *.
		inline T * operator*() const
		{
			AKASSERT( pItem );
			return pItem;
		}

		/// Operator !=.
		bool operator !=( const Iterator& in_rOp ) const
		{
			return ( pItem != in_rOp.pItem );
		}
		
		/// Operator ==.
		bool operator ==( const Iterator& in_rOp ) const
		{
			return ( pItem == in_rOp.pItem );
		}
	};

	/// The IteratorEx iterator is intended for usage when a possible erase may occurs
	/// when simply iterating trough a list, use the simple Iterator, it is faster and lighter.
	struct IteratorEx : public Iterator
	{
		T* pPrevItem;	///< Previous item.

		/// Operator ++.
		IteratorEx& operator++()
		{
			AKASSERT( this->pItem );
			
			pPrevItem = this->pItem;
			this->pItem = U_NEXTITEM::Get( this->pItem );
			
			return *this;
		}
	};

	/// Erase item.
	IteratorEx Erase( const IteratorEx& in_rIter )
	{
		IteratorEx returnedIt;
		returnedIt.pItem = U_NEXTITEM::Get( in_rIter.pItem );
		returnedIt.pPrevItem = in_rIter.pPrevItem;

		RemoveItem( in_rIter.pItem, in_rIter.pPrevItem );

		return returnedIt;
	}

	/// Insert item.
    IteratorEx Insert( const IteratorEx& in_rIter,
                       T * in_pItem )
	{
        IteratorEx returnedIt;
		AddItem( in_pItem, in_rIter.pItem, in_rIter.pPrevItem );
        returnedIt = in_rIter;
        return returnedIt;
	}

	/// End condition.
	inline Iterator End()
	{
		Iterator returnedIt;
		returnedIt.pItem = NULL;
		return returnedIt;
	}

	/// Get IteratorEx at beginning.
	inline IteratorEx BeginEx()
	{
		IteratorEx returnedIt;
		
		returnedIt.pItem = m_pFirst;
		returnedIt.pPrevItem = NULL;
		
		return returnedIt;
	}

	/// Get Iterator at beginning.
	inline Iterator Begin()
	{
		Iterator returnedIt;
		
		returnedIt.pItem = m_pFirst;
		
		return returnedIt;
	}

	/// Get Iterator from item.
	inline IteratorEx FindEx( T *  in_pItem )
	{
		IteratorEx it = BeginEx();
		for ( ; it != End(); ++it )
		{
			if ( it.pItem == in_pItem )
				break;
		}

		return it;
	}

	/// Constructor.
	AkListBare()
	: m_pFirst( NULL )
	, m_pLast( NULL )
	{
	}

	/// Destructor.
	~AkListBare()
	{
	}

	/// Terminate.
	void Term()
	{
		RemoveAll();
	}

	/// Add element at the beginning of list.
	void AddFirst( T * in_pItem )
	{
		if ( m_pFirst == NULL )
		{
			m_pFirst = m_pLast = in_pItem;
			U_NEXTITEM::Get( in_pItem ) = NULL;
		}
		else
		{
			U_NEXTITEM::Get( in_pItem ) = m_pFirst;
			m_pFirst = in_pItem;
		}

		this->IncrementCount( in_pItem );
	}

	/// Add element at the end of list.
	void AddLast( T * in_pItem )
	{
		U_NEXTITEM::Get( in_pItem ) = NULL;

		if ( m_pLast == NULL )
		{
			m_pFirst = m_pLast = in_pItem;
		}
		else
		{
			U_NEXTITEM::Get( m_pLast ) = in_pItem;
			m_pLast = in_pItem;
		}

		this->IncrementCount( in_pItem );
	}

	/// Remove an element.
	AKRESULT Remove( T * in_pItem )
	{
		IteratorEx it = FindEx( in_pItem );
		if ( it != End() )
		{
			Erase( it );
			return AK_Success;
		}

		return AK_Fail;
	}

	/// Remove the first element.
	AKRESULT RemoveFirst()
	{
		if( m_pFirst == NULL )
			return AK_Fail;

		if ( U_NEXTITEM::Get( m_pFirst ) == NULL )
		{
			m_pFirst = NULL;
			m_pLast = NULL;
		}
		else
		{
			m_pFirst = U_NEXTITEM::Get( m_pFirst );
		}

		this->DecrementCount( m_pFirst );

		return AK_Success;
	}

	/// Remove all elements.
	AkForceInline void RemoveAll()
	{
		// Items being externally managed, all we need to do here is clear our members.
		m_pFirst = NULL;
		m_pLast = NULL;
		this->ResetCount( m_pFirst );
	}

	/// Get first element.
	AkForceInline T * First()
	{
		return m_pFirst;
	}

	/// Get last element.
	AkForceInline T * Last()
	{
		return m_pLast;
	}

	/// Empty condition.
	AkForceInline bool IsEmpty() const
	{
		return m_pFirst == NULL;
	}

	/// Remove an element.
	void RemoveItem( T * in_pItem, T * in_pPrevItem )
	{
		// Is it the first one ?

		if( in_pItem == m_pFirst )
		{
			// new first one is the next one
			m_pFirst = U_NEXTITEM::Get( in_pItem );
		}
		else
		{
			// take it out of the used space
			U_NEXTITEM::Get( in_pPrevItem ) = U_NEXTITEM::Get( in_pItem );
		}

		// Is it the last one ?

		if( in_pItem == m_pLast )
		{
			// new last one is the previous one
			m_pLast = in_pPrevItem;
		}

		this->DecrementCount( m_pFirst );
	}

	/// Add an element.
	void AddItem( T * in_pItem, T * in_pNextItem, T * in_pPrevItem )
	{
		U_NEXTITEM::Get( in_pItem ) = in_pNextItem;

		if ( in_pPrevItem == NULL )
            m_pFirst = in_pItem;
        else
            U_NEXTITEM::Get( in_pPrevItem ) = in_pItem;

        // Update tail.
        // Note: will never occur since it is used with iteratorEx (have EndEx?).
        if ( in_pNextItem == NULL )   
            m_pLast = in_pItem;

	    this->IncrementCount( in_pItem );
	}

protected:
	T *				m_pFirst;					///< top of list
	T *				m_pLast;					///< bottom of list
};

#endif // _AKLISTBARE_H
