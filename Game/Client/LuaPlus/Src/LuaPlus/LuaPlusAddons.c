///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "lua.h"
#include "lobject.h"
#include "lstring.h"
#include "ltable.h"
#include "ldo.h"

#define luaplus_assert(e) /* empty */

// lvm.c

#ifndef lua_number2wstr
#include <stdio.h>
#define lua_number2wstr(s,l,n)    _snwprintf((s), (l), LUA_NUMBER_WFMT, (n))
#endif

int luaV_towstring (lua_State *L, TObject *obj) {
  if (ttype(obj) != LUA_TNUMBER)
    return 0;
  else {
    lua_WChar s[32];  /* 16 digits, sign, point and \0  (+ some extra...) */
    lua_number2wstr((s), 32, nvalue(obj));  /* convert `s' to number */
    setwsvalue2s(obj, luaWS_new(L, s));
    return 1;
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// lapi.c
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifndef api_check
#define api_check(L, o)		/*{ assert(o); }*/
#endif

#define api_checknelems(L, n)	api_check(L, (n) <= (L->top - L->ci->base))

#define api_incr_top(L) \
	{if (L->top >= L->ci->top) luaD_checkstack(L, 1); L->top++;}
//#define api_incr_top(L)   (api_check(L, L->top<L->ci->top), L->top++)

TObject *luaA_index (lua_State *L, int idx);

extern TObject *luaA_indexAcceptable (lua_State *L, int index);
extern TObject *negindex (lua_State *L, int index);


LUA_API int lua_iswstring (lua_State *L, int index) {
  int t = lua_type(L, index);
  return (t == LUA_TWSTRING || t == LUA_TNUMBER);
}


LUA_API const lua_WChar *lua_towstring (lua_State *L, int index) {
  StkId o = luaA_indexAcceptable(L, index);
  if (o == NULL)
    return NULL;
  else if (ttype(o) == LUA_TWSTRING)
    return wsvalue(o);
  else {
    const lua_WChar *s;
    lua_lock(L);  /* `luaV_tostring' may create a new string */
    s = (luaV_towstring(L, o) == 0) ? wsvalue(o) : NULL;
    lua_unlock(L);
    return s;
  }
}


LUA_API void lua_pushlwstring (lua_State *L, const lua_WChar *s, size_t len) {
  lua_lock(L);
  setwsvalue(L->top, luaS_newlwstr(L, s, len));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_pushwstring (lua_State *L, const lua_WChar *s) {
  if (s == NULL)
    lua_pushnil(L);
  else
    lua_pushlwstring(L, s, wcslen(s));
}


LUA_API void lua_getdefaultmetatable(lua_State *L, int type)
{
  lua_lock(L);
  sethvalue(L->top, hvalue(defaultmetatypes(L, type)));
  api_incr_top(L);
  lua_unlock(L);
}


LUA_API void lua_setdefaultmetatable(lua_State *L, int type)
{
  StkId t;
  lua_lock(L);
  api_checknelems(L, 1);
  t = L->top - 1;
  if (ttype(t) == LUA_TTABLE) {  /* `t' is a table? */
	  sethvalue(defaultmetatypes(L, type), hvalue(t));
  }
  L->top -= 1;
  lua_unlock(L);
}


#define lua_number2int(i,n)	((i)=(int)(n))

LUA_API int lua_getn (lua_State *L, int index) {
  StkId t;
  const TObject *value;
  int n;
  lua_lock(L);
  t = luaA_index(L, index);
  api_check(L, ttype(t) == LUA_TTABLE);
  value = luaH_getstr(hvalue(t), luaS_newliteral(L, "n"));  /* = t.n */
  if (ttype(value) == LUA_TNUMBER)
    lua_number2int(n, nvalue(value));
  else {
    Node *nd;
    Table *a = hvalue(t);
    lua_Number max = 0;
    int i;
    i = a->sizearray;
    while (i--) {
      if (ttype(&a->array[i]) != LUA_TNIL)
        break;
    }
    max = i+1;
    i = sizenode(a);
    nd = a->node;
    while (i--) {
      if (ttype(gkey(nd)) == LUA_TNUMBER &&
          ttype(gval(nd)) != LUA_TNIL &&
          nvalue(gkey(nd)) > max)
        max = nvalue(gkey(nd));
      nd++;
    }
    lua_number2int(n, max);
  }
  lua_unlock(L);
  return n;
}


LUA_API void lua_newuserdatabox (lua_State *L, void *ptr) {
  Udata *u;
  lua_lock(L);
  u = luaS_newudata(L, 4);
  u->uv.len = 4;  // user data box bit is set.
  *(void**)(u + 1) = ptr;
  setuvalue(L->top, u);
  api_incr_top(L);
  lua_unlock(L);
}

#ifdef _MSC_VER
#pragma warning(disable : 4055)
#pragma warning(disable : 4152)

typedef int (__stdcall *lua_stdcallCFunction) (lua_State *L);

static int stdcall_closure(lua_State *L) {
  lua_stdcallCFunction fn = (lua_stdcallCFunction)lua_touserdata(L, lua_upvalueindex(1));
  return fn(L);
}


LUA_API void lua_pushstdcallcfunction(lua_State *L,lua_stdcallCFunction fn) {
  lua_pushlightuserdata(L, fn);
  lua_pushcclosure(L, stdcall_closure, 1);
}

LUA_API void lua_safetostring(lua_State *L,int index,char* buffer) {
  const char* tmp;
  tmp=lua_tostring(L,index);
  memcpy(buffer,tmp,lua_strlen(L,index)+1);
}

#endif _MSC_VER


