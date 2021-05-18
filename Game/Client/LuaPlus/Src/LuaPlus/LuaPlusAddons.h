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
#ifndef LUAPLUSADDONS_H
#define LUAPLUSADDONS_H

#ifdef LUA_MTSUPPORT
	#define lua_lock(L)		if (L->l_G  &&  L->l_G->lockFunc) (*L->l_G->lockFunc)(L->l_G->lockData)
	#define lua_unlock(L)	if (L->l_G  &&  L->l_G->unlockFunc) (*L->l_G->unlockFunc)(L->l_G->lockData)
	LUA_API void lua_setlockfunctions(lua_State *L, void (*lockFunc)(void *), void (*unlockFunc)(void *), void *lockData);
#else !LUA_MTSUPPORT
	#define lua_lock(L)
	#define lua_unlock(L)
#endif LUA_MTSUPPORT

#define LUA_ALLOC_TEMP 1
typedef void* (*lua_ReallocFunction)(void* ptr, int oldsize, int size, void* data, const char* allocName, unsigned int flags);
typedef void (*lua_FreeFunction)(void* ptr, int oldsize, void* data);
LUA_API void lua_getdefaultmemoryfunctions(lua_ReallocFunction* reallocFunc, lua_FreeFunction* freeFunc, void** data);
LUA_API void lua_setdefaultmemoryfunctions(lua_ReallocFunction reallocFunc, lua_FreeFunction freeFunc, void* data);

LUA_API void lua_setusergcfunction(lua_State *L, void (*userGCFunction)(void*));

LUA_API void* lua_getglobaluserdata(lua_State *L);
LUA_API void lua_setglobaluserdata(lua_State *L, void* globalUserData);

LUA_API void* lua_getstateuserdata(lua_State *L);
LUA_API void lua_setstateuserdata(lua_State *L, void* stateUserData);

LUA_API void lua_setfatalerrorfunction(lua_State *L, void (*fatalErrorFunc)(void));
LUA_API void lua_setminimumstringtablesize(int numstrings);
LUA_API void lua_setminimumglobaltablesize(int numentries);
LUA_API void lua_setminimumauxspace(int size);

LUA_API int             lua_iswstring (lua_State *L, int index);
LUA_API const lua_WChar  *lua_towstring (lua_State *L, int index);
LUA_API void  lua_pushlwstring (lua_State *L, const lua_WChar *s, size_t len);
LUA_API void  lua_pushwstring (lua_State *L, const lua_WChar *s);

#define lua_pushwliteral(L, s)	\
	lua_pushlwstring(L, L"" s, (sizeof(s)/sizeof(lua_WChar))-1)

LUA_API void  lua_getdefaultmetatable (lua_State *L, int objindex);
LUA_API void  lua_setdefaultmetatable(lua_State *L, int type);

LUA_API void lua_newuserdatabox (lua_State *L, void *ptr);

LUA_API int lua_getn (lua_State *L, int index);

/* formats for Lua numbers */
#ifndef LUA_NUMBER_WSCAN
#define LUA_NUMBER_WSCAN	L"%lf"
#endif

#ifndef LUA_NUMBER_WFMT
#define LUA_NUMBER_WFMT		L"%.16g"
#endif

#define LUA_WSTRLIBNAME	"wstring"
LUALIB_API int luaopen_wstring (lua_State *L);

LUALIB_API int luaL_loadwbuffer (lua_State *L, const lua_WChar *buff, size_t size, const char *name);
LUALIB_API int lua_dowbuffer (lua_State *L, const lua_WChar *buff, size_t size, const char *name);
LUALIB_API int lua_dowstring (lua_State *L, const lua_WChar *str, const char *name);

#endif LUAPLUSADDONS_H
