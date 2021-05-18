///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER
#pragma once
#endif _MSC_VER
#ifndef LUASTACKTABLEITERATOR_H
#define LUASTACKTABLEITERATOR_H

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

/**
	The LuaStackTableIterator class provides a far simpler, safer, and more natural
	looking table iteration method.  The iterator is not STL compliant,
	although it could easily be made so.
**/
class LuaStackTableIterator
{
public:
	/**
		\param tableObj The table to iterate the contents of.
		\param doReset If true, the Reset() function is called at constructor
			initialization time, allowing the iterator to be used immediately.
			If false, then Reset() must be called before iterating.
		\param autoStackManagement If true, then for every Next() pass through
			the iterator, the stack is cleared to the iteration position (as if
			a LuaAutoBlock was run every single time).
	**/
	inline LuaStackTableIterator( LuaStackObject tableObj, bool doReset = true,
			bool autoStackManagement = true ) :
		m_tableObj( tableObj ),
		m_startStackIndex( -1 ),
		m_autoStackManagement( autoStackManagement )
	{
		// If the user requested it, perform the automatic reset.
		if ( doReset )
			Reset();
	}


	/**
		The destructor does nothing.
	**/
	inline ~LuaStackTableIterator()
	{
	};


	/**
		Start iteration at the beginning of the table.
	**/
	inline void Reset()
	{
		// Start afresh...
		LuaState* state = m_tableObj;
		m_startStackIndex = state->GetTop();

		// Push the head stack entry.
		state->PushNil();

		// Start the iteration.  If the return value is 0, then the iterator
		// will be invalid.
		if ( state->Next( m_tableObj ) == 0 )
			m_startStackIndex = -1;
	}


	/**
		Invalidates the iterator.  Call this function if you early abort from
		an iteration loop (such as before it hits the end).
	**/
	inline void Invalidate()
	{
		// See if the iterator is already invalid.
		if ( !IsValid() )
			return;

		// This is a local helper variable so we don't waste space in the class
		// definition.
		LuaState* state = m_tableObj;

		if ( !m_autoStackManagement )
		{
			luaplus_assert( state->GetTop() <= m_startStackIndex + 1 );
		}

		// Set the stack back.
		state->SetTop( m_startStackIndex );

		// Invalidate the iterator.
		m_startStackIndex = -1;
	}

	/**
		Go to the next entry in the table.

		\return Returns true if the iteration is done.
	**/
	inline bool Next()
	{
		// This function is only active if Reset() has been called head.
		luaplus_assert( IsValid() );

		// This is a local helper variable so we don't waste space in the class
		// definition.
		LuaState* state = m_tableObj;

		// Do any stack management operations.
		if ( m_autoStackManagement )
		{
			state->SetTop( m_startStackIndex + 1 );
		}
		else
		{
			// If this luaplus_assert fires, then you left something on the stack.
			luaplus_assert( state->GetTop() == m_startStackIndex + 1 );
		}

		// Do the Lua table iteration.
		if ( state->Next( m_tableObj ) == 0 )
		{
			// Invalidate the iterator.
			m_startStackIndex = -1;
			return false;
		}

		// The iterator is still valid.
		return true;
	}


	/**
		\return Returns true if the iterator is valid (there is a current element).
	**/
	inline bool IsValid() const
	{
		return m_startStackIndex != -1;
	}


	/**
		We can easily allow a prefix operator++.  Postfix would be a stack
		management nightmare.
	**/
	inline LuaStackTableIterator& operator++()
	{
		Next();
		return *this;
	}


	/**
		\return Returns true if the iterator is valid (there is a current element).
	**/
	inline operator bool() const
	{
		// If the iterator is valid, then we're good.
		return IsValid();
	}

	
	/**
		\return Returns a LuaStackObject describing the current key.
	**/
	inline LuaStackObject GetKey()
	{
		// This function is only active if Reset() has been called head.
		luaplus_assert( IsValid() );

		return LuaStackObject( m_tableObj, m_startStackIndex + 1 );
	}

	
	/**
		\return Returns a LuaStackObject describing the current value.
	**/
	inline LuaStackObject GetValue()
	{
		// This function is only active if Reset() has been called head.
		luaplus_assert( IsValid() );

		return LuaStackObject( m_tableObj, m_startStackIndex + 2 );
	}

protected:

private:
	/**
		Don't allow copies.  The stack will get screwed.
	**/ 
	LuaPlus::LuaStackTableIterator& operator=( const LuaStackTableIterator& iter );
	LuaPlus::LuaStackTableIterator( const LuaStackTableIterator& iter );

	LuaPlus::LuaStackObject m_tableObj;				///< The table object being iterated.
	int m_startStackIndex;				///< The current starting stack index or -1 if the iterator is invalid.
	bool m_autoStackManagement;			///< Auto stack management enabled or not?
};


} // namespace LuaPlus

#endif LUASTACKTABLEITERATOR_H
