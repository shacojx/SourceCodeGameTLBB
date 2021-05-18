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
#ifndef LUATABLEITERATOR_H
#define LUATABLEITERATOR_H

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

bool LUA_API LuaPlusH_next(LuaState* state, LuaObject* table, LuaObject* key, LuaObject* value);

/**
	The LuaTableIterator class provides a far simpler, safer, and more natural
	looking table iteration method.  The iterator is not STL compliant,
	although it could easily be made so.
**/
class LuaTableIterator
{
public:
	/**
		\param tableObj The table to iterate the contents of.
		\param doReset If true, the Reset() function is called at constructor
			initialization time, allowing the iterator to be used immediately.
			If false, then Reset() must be called before iterating.
	**/
	inline LuaTableIterator( LuaObject& tableObj, bool doReset = true ) :
		m_isDone(false),
		m_tableObj(tableObj),
		m_keyObj(tableObj.m_state),
		m_valueObj(tableObj.m_state)
	{
		luaplus_assert(tableObj.IsTable());

		// If the user requested it, perform the automatic reset.
		if ( doReset )
			Reset();
	}


	/**
		The destructor does nothing.
	**/
	inline ~LuaTableIterator()
	{
	};


	/**
		Start iteration at the beginning of the table.
	**/
	inline void Reset()
	{
		// Start afresh...
		LuaState* state = m_tableObj;

		// Start at the beginning.
		m_keyObj.AssignNil(state);

		// Start the iteration.  If the return value is 0, then the iterator
		// will be invalid.
		if ( !LuaPlusH_next(state, &m_tableObj, &m_keyObj, &m_valueObj) )
			m_isDone = true;
  	}


	/**
		Invalidates the iterator.  Call this function if you early abort from
		an iteration loop (such as before it hits the end).
	**/
	inline void Invalidate()
	{
		// This is a local helper variable so we don't waste space in the class
		// definition.
		LuaState* state = m_tableObj;
		m_keyObj.AssignNil(state);
		m_valueObj.AssignNil(state);
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

		// Do the Lua table iteration.
		if ( !LuaPlusH_next(state, &m_tableObj, &m_keyObj, &m_valueObj) )
		{
			m_isDone = true;
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
		return !m_isDone;
	}


	/**
		We can easily allow a prefix operator++.  Postfix would be a stack
		management nightmare.
	**/
	inline LuaTableIterator& operator++()
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
		\return Returns a LuaObject describing the current key.
	**/
	inline LuaObject& GetKey()
	{
		// This function is only active if Reset() has been called head.
		luaplus_assert( IsValid() );

		return m_keyObj;
	}

	
	/**
		\return Returns a LuaObject describing the current value.
	**/
	inline LuaObject& GetValue()
	{
		// This function is only active if Reset() has been called head.
		luaplus_assert( IsValid() );

		return m_valueObj;
	}

protected:

private:
	/**
		Don't allow copies.
	**/ 
	LuaTableIterator& operator=( const LuaTableIterator& iter );
	LuaTableIterator( const LuaTableIterator& iter );

	LuaPlus::LuaObject& m_tableObj;				///< The table object being iterated.
	LuaPlus::LuaObject m_keyObj;
	LuaPlus::LuaObject m_valueObj;
	bool m_isDone;
};

} // namespace LuaPlus

#endif LUATABLEITERATOR_H
