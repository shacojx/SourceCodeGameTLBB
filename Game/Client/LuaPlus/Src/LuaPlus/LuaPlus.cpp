///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#define BUILDING_LUAPLUS
#include "LuaLink.h"
extern "C" {
#include "lobject.h"
} // extern "C"
#include "LuaPlus.h"
#include <string.h>
#ifdef WIN32
#ifndef _WIN32_WCE
#include <windows.h>
#endif _WIN32_WCE
#undef GetObject
#endif WIN32
#include <new.h>
#include <malloc.h>
#include <stdio.h>
#include <exception>

#include "LuaPlusFunctions.h"

#ifdef _MSC_VER
#pragma warning(disable: 4100)
#endif _MSC_VER

//-----------------------------------------------------------------------------
extern "C" {
#include "lualib.h"
#include "lfunc.h"
#include "lgc.h"
#include "lstate.h"
#include "include/Lua.h"
#include "include/lauxlib.h"
} // extern "C"


namespace LuaPlus
{

static void LSLock(void* data);
static void LSUnlock(void* data);

void LuaPlusGCFunction(void*);

#ifndef api_check
#define api_check(L, o)		(luaplus_assert(o))
#endif

#define api_checknelems(L, n)	api_check(L, (n) <= (L->top - L->ci->base))

//#define api_incr_top(L)   (api_check(L, L->top<L->ci->top), L->top++)

#define api_incr_top(L) \
	{if (L->top >= L->ci->top) lua_checkstack(L, 1); L->top++;}

LUA_API void lua_pushtobject(lua_State *L, void* tobject)
{
	TObject* tobj = (TObject*)tobject;
	lua_lock(L);
	*L->top = *tobj;
	api_incr_top(L);
	lua_unlock(L);
}


/*static*/ LuaState* LuaState::Create( bool initStandardLibrary, bool multithreaded )
{
	lua_ReallocFunction reallocFunc;
	lua_FreeFunction freeFunc;
	void* data;
	lua_getdefaultmemoryfunctions(&reallocFunc, &freeFunc, &data);
	LuaState* state = (LuaState*)(*reallocFunc)(NULL, 0, sizeof(LuaState), data, "LuaState", 0);
	::new(state) LuaState(initStandardLibrary, multithreaded);
	return state;
}


/*static*/ LuaState* LuaState::CastState( lua_State* L )
{
	LuaState* state = (LuaState*)lua_getstateuserdata( L );
	return state;
}


/*static*/ void LuaState::Destroy( LuaState* state )
{
	lua_FreeFunction freeFunc = state->m_state->l_G->freeFunc;
	void* data = state->m_state->l_G->memData;
	state->~LuaState();
	(*freeFunc)(state, sizeof(LuaState), data);
}


LuaState* LuaState::CreateThread(LuaState* parentState)
{
	lua_ReallocFunction reallocFunc;
	lua_FreeFunction freeFunc;
	void* data;
	lua_getdefaultmemoryfunctions(&reallocFunc, &freeFunc, &data);
	LuaState* state = (LuaState*)(*reallocFunc)(NULL, 0, sizeof(LuaState), data, "LuaState", 0);
	::new(state) LuaState(parentState, false);
	return state;
}

	
/**
**/
LuaState::LuaState(bool initStandardLibrary, bool multithreaded)
{
	m_state = lua_open();
	m_ownState = true;

	lua_setusergcfunction(m_state, LuaPlusGCFunction);

	SetupStateEx();

	GetHeadObject()->m_prev = NULL;
	GetHeadObject()->m_next = GetTailObject();
	GetTailObject()->m_prev = GetHeadObject();
	GetTailObject()->m_next = NULL;

#ifdef LUA_MTSUPPORT
	if (multithreaded)
	{
#ifdef WIN32
		// What about clean up?
		CRITICAL_SECTION* cs = new CRITICAL_SECTION;
		::InitializeCriticalSection(cs);
		lua_setlockfunctions(m_state, LSLock, LSUnlock, cs);
#endif WIN32
	}
#endif LUA_MTSUPPORT
	
	Init(initStandardLibrary);
}


/**
**/
LuaState::LuaState(LuaState* parentState, bool initStandardLibrary)
{
	m_state = lua_newthread(*parentState);
	m_ownState = false;			// Threads are not closeable.

	GetHeadObject()->m_prev = NULL;
	GetHeadObject()->m_next = GetTailObject();
	GetTailObject()->m_prev = GetHeadObject();
	GetTailObject()->m_next = NULL;

	m_threadObj = LuaStackObject(parentState, parentState->GetTop());
	parentState->Pop();

	SetupStateEx();
}


/**
**/
LuaState::LuaState( lua_State* state )
{
	m_state = state;

	m_ownState = false;

	SetupStateEx();
}


/**
	The LuaState class assumes ownership of the lua_State pointer and
	destroys it when destroyed.
**/
LuaState::~LuaState()
{
	if ( m_state  &&  m_ownState )
	{
		lua_close( m_state );
	}
}


/**
**/
void LuaState::SetupStateEx()
{
	lua_setstateuserdata( m_state, this );
}


static int LS_LOG( LuaState* state )
{
	lua_State* L = *state;
	int n = lua_gettop(L);  /* number of arguments */
	int i;
	lua_getglobal(L, "towstring");
	lua_getglobal(L, "tostring");
	for (i=1; i<=n; i++) {
		const char *s = NULL;
		const lua_WChar *ws = NULL;
		if (lua_type(L, i) != LUA_TWSTRING)
		{
			lua_pushvalue(L, -1);  /* function to be called */
			lua_pushvalue(L, i);   /* value to print */
			lua_call(L, 1, 1);
			s = lua_tostring(L, -1);  /* get result */
			if (s == NULL)
				return luaL_error(L, "`tostring' must return a string to `print'");
		}
		else
		{
			lua_pushvalue(L, -2);  /* function to be called */
			lua_pushvalue(L, i);   /* value to print */
			lua_call(L, 1, 1);
			ws = lua_towstring(L, -1);  /* get result */
			if (ws == NULL)
				return luaL_error(L, "`tostring' must return a string to `print'");
		}
		if (i>1)
		{
#ifdef WIN32
//			OutputDebugStringA("\t");
#else
			fputs("\t", stdout);
#endif
		}
		if (s)
		{
#ifdef WIN32
			OutputDebugStringA(s);
			throw std::exception(s);
#else
			fputs(s, stdout);
#endif
		}
		else
		{
#ifdef WIN32
			OutputDebugStringW((LPCWSTR)ws);
#else
//			fputws(ws, stdout);
#endif
		}
		lua_pop(L, 1);  /* pop result */
	}

#ifdef WIN32
	OutputDebugStringA("\n");
#else
	fputs("\n", stdout);
#endif

	return 0;
}


/**
**/
static int FatalError( lua_State* state )
{
	const char* err = lua_tostring(state, 1);
#ifdef WIN32
	OutputDebugString(err);
#else !WIN32
	puts(err);
#endif WIN32

#ifndef _WIN32_WCE
	throw -1;
#endif _WIN32_WCE

	return -1;
}


/**
**/
static void LSLock(void* data)
{
#ifdef WIN32
	CRITICAL_SECTION* cs = (CRITICAL_SECTION*)data;
	::EnterCriticalSection(cs);
#endif WIN32
}


/**
**/
static void LSUnlock(void* data)
{
#ifdef WIN32
	CRITICAL_SECTION* cs = (CRITICAL_SECTION*)data;
	::LeaveCriticalSection(cs);
#endif WIN32
}


// LuaDumpObject(file, key, value, alphabetical, indentLevel, maxIndentLevel, writeAll)
static int LS_LuaDumpObject( LuaState* state )
{
	LuaStateOutFile file;

	LuaPlus::LuaStack args(state);
	LuaPlus::LuaStackObject fileObj = args[1];
	const char* fileName = NULL;
	if ( fileObj.IsUserData() )
	{
		FILE* stdioFile = (FILE *)fileObj.GetUserData();
		file.Assign( stdioFile );
	}
	else if ( fileObj.IsString() )
	{
		fileName = fileObj.GetString();
	}

	LuaPlus::LuaObject nameObj = args[2];
	LuaPlus::LuaObject valueObj = args[3];
	LuaPlus::LuaStackObject alphabeticalObj = args[4];
	LuaPlus::LuaStackObject indentLevelObj = args[5];
	LuaPlus::LuaStackObject maxIndentLevelObj = args[6];
	LuaPlus::LuaStackObject writeAllObj = args[7];
	bool writeAll = writeAllObj.IsBoolean() ? writeAllObj.GetBoolean() : false;
	bool alphabetical = alphabeticalObj.IsBoolean() ? alphabeticalObj.GetBoolean() : true;
	unsigned int maxIndentLevel = maxIndentLevelObj.IsInteger() ? (unsigned int)maxIndentLevelObj.GetInteger() : 0xFFFFFFFF;

	unsigned int flags = (alphabetical ? LuaState::DUMP_ALPHABETICAL : 0) | (writeAll ? LuaState::DUMP_WRITEALL : 0);

	if (fileName)
	{
		state->DumpObject(fileName, nameObj, valueObj, flags, (unsigned int)indentLevelObj.GetInteger(), maxIndentLevel);
	}
	else
	{
		state->DumpObject(file, nameObj, valueObj, flags, (unsigned int)indentLevelObj.GetInteger(), maxIndentLevel);
	}

	return 0;
}


// LuaDumpFile(file, key, value, alphabetical, indentLevel, maxIndentLevel, writeAll)
static int LS_LuaDumpFile( LuaState* state )
{
	return LS_LuaDumpObject(state);
}


// LuaDumpGlobals(file, alphabetical, maxIndentLevel, writeAll)
static int LS_LuaDumpGlobals(LuaState* state)
{
	LuaStateOutFile file;

	LuaPlus::LuaStack args(state);
	LuaPlus::LuaStackObject fileObj = args[1];
	const char* fileName = NULL;
	if ( fileObj.IsUserData() )
	{
		FILE* stdioFile = (FILE *)fileObj.GetUserData();
		file.Assign( stdioFile );
	}
	else if ( fileObj.IsString() )
	{
		fileName = fileObj.GetString();
	}

	LuaPlus::LuaStackObject alphabeticalObj = args[2];
	LuaPlus::LuaStackObject maxIndentLevelObj = args[3];
	LuaPlus::LuaStackObject writeAllObj = args[4];
	bool alphabetical = alphabeticalObj.IsBoolean() ? alphabeticalObj.GetBoolean() : true;
	unsigned int maxIndentLevel = maxIndentLevelObj.IsInteger() ? (unsigned int)maxIndentLevelObj.GetInteger() : 0xFFFFFFFF;
	bool writeAll = writeAllObj.IsBoolean() ? writeAllObj.GetBoolean() : false;

	unsigned int flags = (alphabetical ? LuaState::DUMP_ALPHABETICAL : 0) | (writeAll ? LuaState::DUMP_WRITEALL : 0);

	if (fileName)
	{
		state->DumpGlobals(fileName, flags, maxIndentLevel);
	}
	else
	{
		state->DumpGlobals(file, flags, maxIndentLevel);
	}

	return 0;
}


/**
**/
void LuaState::Init( bool initStandardLibrary )
{
	// Register some basic functions with Lua.
	if (initStandardLibrary)
	{
		// A "bug" in Lua 5.01 causes stack entries to be left behind.
		LuaAutoBlock autoBlock(this);
		luaopen_base(m_state);
		luaopen_table( m_state );
		luaopen_io(m_state);
		luaopen_string(m_state);
		luaopen_wstring(m_state);
		luaopen_math(m_state);
		luaopen_debug(m_state);
#ifndef _WIN32_WCE
		luaopen_loadlib(m_state);
#endif _WIN32_WCE

		ScriptFunctionsRegister( this );

		GetGlobals().Register("LuaDumpGlobals", LS_LuaDumpGlobals);
		GetGlobals().Register("LuaDumpObject", LS_LuaDumpObject);
		GetGlobals().Register("LuaDumpFile", LS_LuaDumpFile);
	}

	GetGlobals().Register("LOG", LS_LOG);
	GetGlobals().Register("_ALERT", LS_LOG);

	lua_atpanic( m_state, FatalError );
}


LuaObject LuaState::GetGlobals() throw()
{
	return LuaObject( this, gt(m_state) );
}

	
} // namespace LuaPlus


extern "C" {
#include "ltable.h"
} // extern "C"

namespace LuaPlus {

LUA_API bool LuaPlusH_next(LuaState* state, LuaObject* table, LuaObject* key, LuaObject* value)
{
	Table* t = hvalue(&table->m_object);
	int i = luaH_index(*state, t, &key->m_object);  /* find original element */
	for (i++; i < t->sizearray; i++)
	{  /* try head array part */
		if (!ttisnil(&t->array[i]))
		{  /* a non-nil value? */
			key->AssignInteger(state, i + 1);
			value->AssignTObject(state, &t->array[i]);
			return true;
		}
	}
	for (i -= t->sizearray; i < sizenode(t); i++)
	{  /* then hash part */
		if (!ttisnil(gval(gnode(t, i))))
		{  /* a non-nil value? */
			key->AssignTObject(state, gkey(gnode(t, i)));
			value->AssignTObject(state, gval(gnode(t, i)));
			return true;
		}
	}
	return false;  /* no more elements */
}


#define ismarked(x)	((x)->gch.marked & ((1<<4)|1))

#define markobject(st,o) { checkconsistency(o); \
  if (iscollectable(o) && !ismarked(gcvalue(o))) reallymarkobject(st,gcvalue(o)); }

void LuaPlusGCFunction(void* s)
{
	GCState* st = (GCState*)s;
	LuaState* state = (LuaState*)lua_getstateuserdata(st->L);
	if (!state)
		return;

	LuaPlus::LuaObject* curObj = state->GetHeadObject()->m_next;
	while (curObj != state->GetTailObject())
	{
		markobject(st, &curObj->m_object);
		curObj = curObj->m_next;		
	}
}


} // namespace LuaPlus
