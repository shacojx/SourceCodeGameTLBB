/*
** 
** String table (keeps all strings handled by Lua)
** See Copyright Notice in lua.h
*/


#include <string.h>

#define lstring_c

#include "lua.h"

#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"



void luaS_freeall (lua_State *L) {
  lua_assert(G(L)->strt.nuse==0);
  luaM_freearray(L, G(L)->strt.hash, G(L)->strt.size, TString *);
}


void luaS_resize (lua_State *L, int newsize) {
  GCObject **newhash;
  stringtable *tb;
  int i;
  luaM_setname(L, "Lua_StringTable");
  newhash = luaM_newvector(L, newsize, GCObject *);
  tb = &G(L)->strt;
  for (i=0; i<newsize; i++) newhash[i] = NULL;
  /* rehash */
  for (i=0; i<tb->size; i++) {
    GCObject *p = tb->hash[i];
    while (p) {  /* for each node in the list */
      GCObject *next = p->gch.next;  /* save next */
      lu_hash h = gcotots(p)->tsv.hash;
      int h1 = lmod(h, newsize);  /* new position */
      lua_assert(cast(int, h%newsize) == lmod(h, newsize));
      p->gch.next = newhash[h1];  /* chain it */
      newhash[h1] = p;
      p = next;
    }
  }
  luaM_freearray(L, tb->hash, tb->size, TString *);
  tb->size = newsize;
  tb->hash = newhash;
}


static TString *newlstr (lua_State *L, const char *str, size_t l, lu_hash h) {
  TString *ts;
  stringtable *tb;
  unsigned int stringbytes = sizestring(l);
  luaM_setnameif(L, "Lua_string");
  ts = cast(TString *, luaM_malloc(L, stringbytes));
  ts->tsv.len = l;
  ts->tsv.hash = h;
  ts->tsv.marked = 0;
  ts->tsv.tt = LUA_TSTRING;
  ts->tsv.reserved = 0;
  memcpy(ts+1, str, l*sizeof(char));
  ((char *)(ts+1))[l] = '\0';  /* ending 0 */
  tb = &G(L)->strt;
  h = lmod(h, tb->size);
  ts->tsv.next = tb->hash[h];  /* chain new entry */
#if LUA_REFCOUNT
  ts->tsv.ref = 0;
  ts->tsv.prev = NULL;
#endif LUA_REFCOUNT
  tb->hash[h] = valtogco(ts);
  tb->nuse++;
  if (tb->nuse > cast(ls_nstr, tb->size) && tb->size <= MAX_INT/2)
    luaS_resize(L, tb->size*2);  /* too crowded */
  return ts;
}


static TString *newlwstr (lua_State *L, const lua_WChar *str, size_t l, lu_hash h) {
  TString *ts;
  stringtable *tb;
  unsigned int stringbytes = sizewstring(l);
  luaM_setnameif(L, "Lua_string");
  ts = cast(TString *, luaM_malloc(L, stringbytes));
  ts->tsv.len = l;
  ts->tsv.hash = h;
  ts->tsv.marked = 0;
  ts->tsv.tt = LUA_TWSTRING;
  ts->tsv.reserved = 0;
  memcpy(ts+1, str, l*sizeof(lua_WChar));
  ((char *)(ts+1))[l * 2] = '\0';  /* ending 0 */
  ((char *)(ts+1))[l * 2 + 1] = '\0';  /* ending 0 */
  tb = &G(L)->strt;
  h = lmod(h, tb->size);
  ts->tsv.next = tb->hash[h];  /* chain new entry */
#if LUA_REFCOUNT
  ts->tsv.ref = 0;
  ts->tsv.prev = NULL;
#endif LUA_REFCOUNT
  tb->hash[h] = valtogco(ts);
  tb->nuse++;
  if (tb->nuse > cast(ls_nstr, tb->size) && tb->size <= MAX_INT/2)
    luaS_resize(L, tb->size*2);  /* too crowded */
  return ts;
}


TString *luaS_newlstr (lua_State *L, const char *str, size_t l) {
  GCObject *o;
  lu_hash h = (lu_hash)l;  /* seed */
  size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */
  size_t l1;
  for (l1=l; l1>=step; l1-=step)  /* compute hash */
    h = h ^ ((h<<5)+(h>>2)+(unsigned char)(str[l1-1]));
  for (o = G(L)->strt.hash[lmod(h, G(L)->strt.size)];
       o != NULL;
       o = o->gch.next) {
    TString *ts = gcotots(o);
    if (ts->tsv.tt == LUA_TSTRING && ts->tsv.len == l && (memcmp(str, getstr(ts), l) == 0))
      return ts;
  }
  return newlstr(L, str, l, h);  /* not found */
}


TString *luaS_newlwstr (lua_State *L, const lua_WChar *str, size_t l) {
  GCObject *o;
  lu_hash h = (lu_hash)l;  /* seed */
  size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */
  size_t l1;
  for (l1=l; l1>=step; l1-=step)  /* compute hash */
    h = h ^ ((h<<5)+(h>>2)+(lua_WChar)(str[l1-1]));
  for (o = G(L)->strt.hash[lmod(h, G(L)->strt.size)];
       o != NULL;
       o = o->gch.next) {
    TString *ts = gcotots(o);
    if (ts->tsv.tt == LUA_TWSTRING && ts->tsv.len == l && (memcmp(str, getstr(ts), l * 2) == 0))
      return ts;
  }
  return newlwstr(L, str, l, h);  /* not found */
}


Udata *luaS_newudata (lua_State *L, size_t s) {
  Udata *u;
  s = (s+7) & (~(size_t)7);  /* make sure size is multiple of 8 */
  u = cast(Udata *, luaM_malloc(L, sizeudata(s)));
  u->uv.marked = (1<<1);  /* is not finalized */
  u->uv.tt = LUA_TUSERDATA;
#if LUA_REFCOUNT
  u->uv.ref = 0;
#endif LUA_REFCOUNT
  u->uv.len = s;
  u->uv.metatable = hvalue(defaultmetatypes(L, LUA_TUSERDATA));
  __AddRefDirect(u->uv.metatable);
  /* chain it on udata list */
#if LUA_REFCOUNT
  u->uv.prev = (GCObject*)&G(L)->rootudata_head;
  u->uv.next = G(L)->rootudata_head.next;
  u->uv.next->uv.prev = valtogco(u);
  G(L)->rootudata_head.next = valtogco(u);
#else !LUA_REFCOUNT
  u->uv.next = G(L)->rootudata;
  G(L)->rootudata = valtogco(u);
#endif
  return u;
}

