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
#ifndef LUAPLUS_H
#define LUAPLUS_H

#include <stdarg.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(disable: 4505)
#pragma warning(disable: 4127)
#endif _MSC_VER

#include "LuaLink.h"

struct lua_TObject;

extern "C" {
#include "include/Lua.h"
#include "include/lauxlib.h"
#include "LuaPlusAddons.h"
} // extern "C"

#define LUAPLUS_ENABLE_INLINES

#ifdef LUAPLUS_ENABLE_INLINES
#define LUAPLUS_INLINE inline
#else !LUAPLUS_ENABLE_INLINES
#define LUAPLUS_INLINE
#endif LUAPLUS_ENABLE_INLINES

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

//#define luaplus_assert(e) /* empty */

class LuaException
{
public:
	LuaException(const char* message) : m_message(message) {}
	~LuaException() {}

	const char* GetMessage() const			{  return m_message;  }

protected:
	const char* m_message;
};

#define luaplus_assert(e) if (!(e)) throw LuaException(#e)

class LuaStateOutFile;
class LuaState;
class LuaStackObject;

} // namespace LuaPlus

#include "LuaStackObject.h"
#include "LuaObject.h"
#include "LuaState.h"
#include "LuaTableIterator.h"
#include "LuaObject.inl"
#include "LuaStateOutFile.h"
#include "LuaHelper.h"
#include "LuaAutoBlock.h"
#include "LuaStackTableIterator.h"

#endif LUAPLUS_H
