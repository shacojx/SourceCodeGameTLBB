/*
** 
** Global State
** See Copyright Notice in lua.h
*/


#include <stdlib.h>

#define lstate_c

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "llex.h"
#include "lmem.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"


/*
** macro to allow the inclusion of user information in Lua state
*/
#ifndef LUA_USERSTATE
#define EXTRASPACE	0
#else
union UEXTRASPACE {L_Umaxalign a; LUA_USERSTATE b;};
#define EXTRASPACE (sizeof(union UEXTRASPACE))
#endif



/*
** you can change this function through the official API:
** call `lua_setpanicf'
*/
static int default_panic (lua_State *L) {
  UNUSED(L);
  return 0;
}


static void* luaHelper_ReallocFunction(void* ptr, int oldsize, int size, void* data, const char* allocName, unsigned int allocFlags)
{
	UNUSED(oldsize);
	UNUSED(data);
	UNUSED(allocName);
	UNUSED(allocFlags);
	return realloc(ptr, size);
}


static void luaHelper_FreeFunction(void* ptr, int oldsize, void* data)
{
	UNUSED(data);
	UNUSED(oldsize);
	free(ptr);
}


static lua_ReallocFunction luaHelper_Realloc = luaHelper_ReallocFunction;
static lua_FreeFunction luaHelper_Free = luaHelper_FreeFunction;
static void* luaHelper_memData = NULL;


void lua_getdefaultmemoryfunctions(lua_ReallocFunction* reallocFunc, lua_FreeFunction* freeFunc, void** data)
{
	*reallocFunc = luaHelper_Realloc;
	*freeFunc = luaHelper_Free;
	*data = luaHelper_memData;
}


void lua_setdefaultmemoryfunctions(lua_ReallocFunction reallocFunc, lua_FreeFunction freeFunc, void* data)
{
	luaHelper_Realloc = reallocFunc ? reallocFunc : luaHelper_ReallocFunction;
	luaHelper_Free = freeFunc ? freeFunc : luaHelper_FreeFunction;
	luaHelper_memData = data;
}


static int lua_minimumnumstrings = NUM_RESERVED + NUM_TAGS;

void lua_setminimumstringtablesize(int numstrings)
{
	if (numstrings == 0)
		lua_minimumnumstrings = NUM_RESERVED + NUM_TAGS;
	else
		lua_minimumnumstrings = numstrings;
}

static int lua_minimumglobaltableentries = 10;

void lua_setminimumglobaltablesize(int numentries)
{
	if (numentries == 0)
		lua_minimumglobaltableentries = 10;
	else
		lua_minimumglobaltableentries = numentries;
}

static int lua_minimumauxspace = 0;

void lua_setminimumauxspace(int size)
{
	lua_minimumauxspace = size;
}

static void defaultFatalErrorFunc(void)
{
	exit(EXIT_FAILURE);
}


#ifdef LUA_MTSUPPORT
LUA_API void lua_setlockfunctions(lua_State *L, void (*lockFunc)(void *),
                                  void (*unlockFunc)(void *), void *lockData)
{
  G(L)->lockFunc = lockFunc;
  G(L)->unlockFunc = unlockFunc;
  G(L)->lockData = lockData;
}
#endif LUA_MTSUPPORT


LUA_API void lua_setfatalerrorfunction(lua_State *L, void (*fatalErrorFunc)(void))
{
	G(L)->fatalErrorFunc = fatalErrorFunc;
}


LUA_API void* lua_getglobaluserdata(lua_State *L)
{
  return G(L)->globalUserData;
}

								 
LUA_API void lua_setglobaluserdata(lua_State *L, void* globalUserData)
{
  G(L)->globalUserData = globalUserData;
}

								 
LUA_API void lua_setusergcfunction(lua_State *L, void (*userGCFunction)(void*))
{
  G(L)->userGCFunction = userGCFunction;
}

								 
LUA_API void* lua_getstateuserdata(lua_State *L)
{
  return L->stateUserData;
}

								 
LUA_API void lua_setstateuserdata(lua_State *L, void* stateUserData)
{
  L->stateUserData = stateUserData;
}

								 
static lua_State *mallocstate (lua_State *L) {
  lu_byte *block = (lu_byte *)luaM_malloc(L, sizeof(lua_State) + EXTRASPACE);
  if (block == NULL) return NULL;
  else {
    block += EXTRASPACE;
    return cast(lua_State *, block);
  }
}


static void freestate (lua_State *L, lua_State *L1) {
  luaM_free(L, cast(lu_byte *, L1) - EXTRASPACE,
               sizeof(lua_State) + EXTRASPACE);
}


static void stack_init (lua_State *L1, lua_State *L) {
#if LUA_REFCOUNT
  StkId i;
#endif LUA_REFCOUNT
  luaM_setname(L, "Lua_Stack");
  L1->stack = luaM_newvector(L, BASIC_STACK_SIZE + EXTRA_STACK, TObject);
  L1->stacksize = BASIC_STACK_SIZE + EXTRA_STACK;
  L1->top = L1->stack;
  L1->stack_last = L1->stack+(L1->stacksize - EXTRA_STACK)-1;
#if LUA_REFCOUNT
  for (i = L1->stack; i != L1->stack_last; ++i)
    setnilvalue2n(i);
#endif LUA_REFCOUNT
  luaM_setname(L, "Lua_CallInfo");
  L1->base_ci = luaM_newvector(L, BASIC_CI_SIZE, CallInfo);
  L1->ci = L1->base_ci;
  L1->ci->state = CI_C;  /*  not a Lua function */
  setnilvalue2n(L1->top++);  /* `function' entry for this `ci' */
  L1->base = L1->ci->base = L1->top;
  L1->ci->top = L1->top + LUA_MINSTACK;
  L1->size_ci = BASIC_CI_SIZE;
  L1->end_ci = L1->base_ci + L1->size_ci;
}


static void freestack (lua_State *L, lua_State *L1) {
  luaM_freearray(L, L1->base_ci, L1->size_ci, CallInfo);
  luaM_freearray(L, L1->stack, L1->stacksize, TObject);
}


/*
** open parts that may cause memory-allocation errors
*/
static void f_luaopen (lua_State *L, void *ud) {
  int i;
  global_State globalState;
  lua_State luaState;
  global_State *g;
#ifdef _DEBUG
  luaState.allocName = "Lua_global_State";
#endif _DEBUG
  luaState.l_G = &globalState;
  globalState.reallocFunc = luaHelper_Realloc;
  globalState.freeFunc = luaHelper_Free;
  globalState.memData = luaHelper_memData;
  globalState.nblocks = sizeof(lua_State) + sizeof(global_State);	// Bogus.
  /* create a new global state */
  g = luaM_new(&luaState, global_State);
  UNUSED(ud);
  if (g == NULL) luaD_throw(L, LUA_ERRMEM);
  L->l_G = g;
  g->mainthread = L;
  g->GCthreshold = 0;  /* mark it as unfinished state */
  g->strt.size = 0;
  g->strt.nuse = 0;
  g->strt.hash = NULL;
  setnilvalue2n(defaultmeta(L));
  setnilvalue2n(registry(L));
  luaZ_initbuffer(L, &g->buff);
  g->panic = default_panic;
#if !LUA_REFCOUNT
  g->rootgc = NULL;
  g->rootudata = NULL;
  g->tmudata = NULL;
#else LUA_REFCOUNT
  g->rootgc_head.next = (GCObject*)&g->rootgc_tail;
  g->rootgc_head.prev = NULL;
  g->rootgc_tail.next = NULL;
  g->rootgc_tail.prev = (GCObject*)&g->rootgc_head;
  g->rootgc_head.tt = LUA_TNIL;
  g->rootgc_head.marked = 0;
  g->rootgc_head.ref = 0;
  g->rootgc_tail.tt = LUA_TNIL;
  g->rootgc_tail.marked = 0;
  g->rootgc_tail.ref = 0;

  g->rootudata_head.next = (GCObject*)&g->rootudata_tail;
  g->rootudata_head.prev = NULL;
  g->rootudata_tail.next = NULL;
  g->rootudata_tail.prev = (GCObject*)&g->rootudata_head;
  g->rootudata_head.tt = LUA_TNIL;
  g->rootudata_head.marked = 0;
  g->rootudata_head.ref = 0;
  g->rootudata_tail.tt = LUA_TNIL;
  g->rootudata_tail.marked = 0;
  g->rootudata_tail.ref = 0;

  g->tmudata_head.next = (GCObject*)&g->tmudata_tail;
  g->tmudata_head.prev = NULL;
  g->tmudata_tail.next = NULL;
  g->tmudata_tail.prev = (GCObject*)&g->tmudata_head;
  g->tmudata_head.tt = LUA_TNIL;
  g->tmudata_head.marked = 0;
  g->tmudata_head.ref = 0;
  g->tmudata_tail.tt = LUA_TNIL;
  g->tmudata_tail.marked = 0;
  g->tmudata_tail.ref = 0;
#endif LUA_REFCOUNT
  setnilvalue2n(gkey(g->dummynode));
  setnilvalue2n(gval(g->dummynode));
  g->dummynode->next = NULL;
  g->nblocks = sizeof(lua_State) + sizeof(global_State);

  g->reallocFunc = luaHelper_Realloc;
  g->freeFunc = luaHelper_Free;
  g->memData = luaHelper_memData;
  g->fatalErrorFunc = defaultFatalErrorFunc;
#ifdef LUA_MTSUPPORT
  g->lockData = NULL;
  g->lockFunc = NULL;
  g->unlockFunc = NULL;
#endif LUA_MTSUPPORT
  g->userGCFunction = NULL;
  g->globalUserData = NULL;

  stack_init(L, L);  /* init stack */

  for (i = 0; i < LUA_NTYPES; i++)
  {
    defaultmetatypes(L, i)->value.gc = NULL;
  }

  /* create default meta table with a dummy table, and then close the loop */
  defaultmeta(L)->tt = LUA_TNUMBER;
  defaultmeta(L)->value.gc = NULL;
  sethvalue2n(defaultmeta(L), luaH_new(L, 0, 0));
  __AddRefDirect(hvalue(defaultmeta(L)));
  hvalue(defaultmeta(L))->metatable = hvalue(defaultmeta(L));
  __AddRefDirect(hvalue(defaultmeta(L))->metatable);

  /* build meta tables */
  for (i = 0; i < LUA_NTYPES; i++)
  {
    luaM_setname(L, "Lua_defaultMetaTypes");
    sethvalue2n(defaultmetatypes(L, i), luaH_new(L, 0, 0));
    hvalue(defaultmetatypes(L, i))->metatable = hvalue(defaultmeta(L));
  }

  luaM_setname(L, "Lua_Globals");
  sethvalue(gt(L), luaH_new(L, 0, 4));  /* table of globals */
  __AddRefDirect(hvalue(gt(L)));
  luaM_setname(L, "Lua_Registry");
  sethvalue(registry(L), luaH_new(L, 4, 4));  /* registry */
  __AddRef(registry(L));
  g->minimumstrings = lua_minimumnumstrings;
  luaS_resize(L, MINSTRTABSIZE);  /* initial size of string table */
  luaT_init(L);
  luaX_init(L);
  luaS_fix(luaS_newliteral(L, MEMERRMSG));
  g->GCthreshold = 4*G(L)->nblocks;

  luaZ_openspace(L, &g->buff, lua_minimumauxspace);
}


static void preinit_state (lua_State *L) {
  L->stack = NULL;
  L->stacksize = 0;
  L->errorJmp = NULL;
  L->hook = NULL;
  L->hookmask = L->hookinit = 0;
  L->basehookcount = 0;
  L->allowhook = 1;
  resethookcount(L);
  L->openupval = NULL;
  L->size_ci = 0;
  L->nCcalls = 0;
  L->base_ci = L->ci = NULL;
  L->errfunc = 0;
  L->stateUserData = NULL;	
  setnilvalue2n(gt(L));
}


static void close_state (lua_State *L) {
  global_State globalState;
  lua_State luaState;

  luaF_close(L, L->stack);  /* close all upvalues for this thread */

  luaState.l_G = &globalState;
  globalState.reallocFunc = G(L)->reallocFunc;
  globalState.freeFunc = G(L)->freeFunc;
  globalState.memData = G(L)->memData;

  if (G(L)) {  /* close global state */
    luaC_sweep(L, 1);  /* collect all elements */
#if !LUA_REFCOUNT
    lua_assert(G(L)->rootgc == NULL);
    lua_assert(G(L)->rootudata == NULL);
#endif LUA_REFCOUNT
    luaS_freeall(L);
    luaZ_freebuffer(L, &G(L)->buff);
  }
  freestack(L, L);
  if (G(L)) {
//jj This check doesn't work anymore, because LuaState variables are allocated through the
//jj memory interface, too.
//jj    lua_assert(G(L)->nblocks == sizeof(lua_State) + sizeof(global_State));
    luaM_freelem(&luaState, G(L));
  }
  freestate(&luaState, L);
}


lua_State *luaE_newthread (lua_State *L) {
  lua_State *L1 = mallocstate(L);
  luaC_link(L, valtogco(L1), LUA_TTHREAD);
  preinit_state(L1);
  L1->l_G = L->l_G;
#if LUA_REFCOUNT
  L1->gclist_head.next = (GCObject*)&L->gclist_tail;
  L1->gclist_head.prev = NULL;
  L1->gclist_tail.next = NULL;
  L1->gclist_tail.prev = (GCObject*)&L->gclist_head;
  L1->gclist_head.tt = LUA_TNIL;
  L1->gclist_head.marked = 0;
  L1->gclist_head.ref = 0;
  L1->gclist_tail.tt = LUA_TNIL;
  L1->gclist_tail.marked = 0;
  L1->gclist_tail.ref = 0;
  L1->ref = 0;
#endif LUA_REFCOUNT
  stack_init(L1, L);  /* init stack */
  setobj2n(gt(L1), gt(L));  /* share table of globals */
  return L1;
}


void luaE_freethread (lua_State *L, lua_State *L1) {
  luaF_close(L1, L1->stack);  /* close all upvalues for this thread */
  lua_assert(L1->openupval == NULL);
  freestack(L, L1);
  freestate(L, L1);
}


LUA_API lua_State *lua_open (void) {
  lua_State *L;
  global_State globalState;
  lua_State luaState;
#ifdef _DEBUG
  luaState.allocName = "Lua_lua_State";
#endif _DEBUG
  luaState.l_G = &globalState;
  globalState.reallocFunc = luaHelper_Realloc;
  globalState.freeFunc = luaHelper_Free;
  globalState.memData = luaHelper_memData;
  globalState.nblocks = sizeof(lua_State) + sizeof(global_State);	// Bogus.
  L = mallocstate(&luaState);
  if (L) {  /* allocation OK? */
    L->tt = LUA_TTHREAD;
    L->marked = 0;
    L->next = NULL;
#if LUA_REFCOUNT
	L->prev = NULL;
	L->gclist_head.next = (GCObject*)&L->gclist_tail;
	L->gclist_head.prev = NULL;
	L->gclist_tail.next = NULL;
	L->gclist_tail.prev = (GCObject*)&L->gclist_head;
	L->gclist_head.tt = LUA_TNIL;
    L->gclist_head.marked = 0;
    L->gclist_head.ref = 0;
	L->gclist_tail.tt = LUA_TNIL;
    L->gclist_tail.marked = 0;
    L->gclist_tail.ref = 0;
	L->ref = 0;
#else !LUA_REFCOUNT
	L->gclist = NULL;
#endif LUA_REFCOUNT
    preinit_state(L);
    L->l_G = NULL;
    if (luaD_rawrunprotected(L, f_luaopen, NULL) != 0) {
      /* memory allocation error: free partial state */
      close_state(L);
      L = NULL;
    }
  }
  lua_userstateopen(L);
  return L;
}


static void callallgcTM (lua_State *L, void *ud) {
  UNUSED(ud);
  luaC_callGCTM(L);  /* call GC metamethods for all udata */
}


LUA_API void lua_close (lua_State *L) {
  lua_lock(L);
  L = G(L)->mainthread;  /* only the main thread can be closed */
  luaF_close(L, L->stack);  /* close all upvalues for this thread */
  luaC_separateudata(L);  /* separate udata that have GC metamethods */
  L->errfunc = 0;  /* no error function during GC metamethods */
  do {  /* repeat until no more errors */
    L->ci = L->base_ci;
    L->base = L->top = L->ci->base;
    L->nCcalls = 0;
  } while (luaD_rawrunprotected(L, callallgcTM, NULL) != 0);
#if !LUA_REFCOUNT
  lua_assert(G(L)->tmudata == NULL);
#endif LUA_REFCOUNT
  close_state(L);
}

