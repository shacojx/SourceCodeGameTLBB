/*
** 
** Garbage Collector
** See Copyright Notice in lua.h
*/

#include <string.h>

#define lgc_c

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lfunc.h"
#include "lgc.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "ltm.h"



/*
** some userful bit tricks
*/
#define setbit(x,b)	((x) |= (1<<(b)))
#define resetbit(x,b)	((x) &= cast(lu_byte, ~(1<<(b))))
#define testbit(x,b)	((x) & (1<<(b)))

#define unmark(x)	resetbit((x)->gch.marked, 0)
#define ismarked(x)	((x)->gch.marked & ((1<<4)|1))

#define stringmark(s)	setbit((s)->tsv.marked, 0)


#define isfinalized(u)		(!testbit((u)->uv.marked, 1))
#define markfinalized(u)	resetbit((u)->uv.marked, 1)


#define KEYWEAKBIT    1
#define VALUEWEAKBIT  2
#define KEYWEAK         (1<<KEYWEAKBIT)
#define VALUEWEAK       (1<<VALUEWEAKBIT)



#define markobject(st,o) { checkconsistency(o); \
  if (iscollectable(o) && !ismarked(gcvalue(o))) reallymarkobject(st,gcvalue(o)); }

#define condmarkobject(st,o,c) { checkconsistency(o); \
  if (iscollectable(o) && !ismarked(gcvalue(o)) && (c)) \
    reallymarkobject(st,gcvalue(o)); }

#define markvalue(st,t) { if (!ismarked(valtogco(t))) \
		reallymarkobject(st, valtogco(t)); }



#if LUA_REFCOUNT

static void Unlink(GCObject* o)
{
  o->gch.prev->gch.next = o->gch.next;
  o->gch.next->gch.prev = o->gch.prev;
}

#endif LUA_REFCOUNT


void reallymarkobject (GCState *st, GCObject *o) {
  lua_assert(!ismarked(o));
  setbit(o->gch.marked, 0);  /* mark object */
  switch (o->gch.tt) {
    case LUA_TUSERDATA: {
      markvalue(st, gcotou(o)->uv.metatable);
      break;
    }
    case LUA_TFUNCTION: {
      gcotocl(o)->c.gclist = st->tmark;
      st->tmark = o;
      break;
    }
    case LUA_TTABLE: {
      gcotoh(o)->gclist = st->tmark;
      st->tmark = o;
      break;
    }
    case LUA_TTHREAD: {
#if LUA_REFCOUNT
      gcototh(o)->gclist_head.next = st->tmark;
#else !LUA_REFCOUNT
      gcototh(o)->gclist = st->tmark;
#endif LUA_REFCOUNT
      st->tmark = o;
      break;
    }
    case LUA_TPROTO: {
      gcotop(o)->gclist = st->tmark;
      st->tmark = o;
      break;
    }
    default: lua_assert(o->gch.tt == LUA_TSTRING || o->gch.tt == LUA_TWSTRING);
  }
}


static void marktmu (GCState *st) {
  GCObject *u;
#if LUA_REFCOUNT
  for (u = st->g->tmudata_head.next; u != (GCObject*)&st->g->tmudata_tail; u = u->gch.next) {
#else !LUA_REFCOUNT
  for (u = st->g->tmudata; u; u = u->gch.next) {
#endif LUA_REFCOUNT
    unmark(u);  /* may be marked, if left from previous GC */
    reallymarkobject(st, u);
  }
}


/* move `dead' udata that need finalization to list `tmudata' */
size_t luaC_separateudata (lua_State *L) {
  size_t deadmem = 0;
#if LUA_REFCOUNT
  GCObject **p = &G(L)->rootudata_head.next;
#else !LUA_REFCOUNT
  GCObject **p = &G(L)->rootudata;
#endif LUA_REFCOUNT
  GCObject *curr;
  GCObject *collected = NULL;  /* to collect udata with gc event */
#if !LUA_REFCOUNT
  GCObject **lastcollected = &collected;
  while ((curr = *p) != NULL) {
    lua_assert(curr->gch.tt == LUA_TUSERDATA);
#else LUA_REFCOUNT
  while ((curr = *p) != (GCObject*)&G(L)->rootudata_tail) {
#endif LUA_REFCOUNT
    if (ismarked(curr) || isfinalized(gcotou(curr)))
      p = &curr->gch.next;  /* don't bother with them */

    else if (fasttm(L, gcotou(curr)->uv.metatable, TM_GC) == NULL) {
      markfinalized(gcotou(curr));  /* don't need finalization */
      p = &curr->gch.next;
    }
    else {  /* must call its gc method */
      deadmem += sizeudata(gcotou(curr)->uv.len);
      *p = curr->gch.next;
#if LUA_REFCOUNT
	  Unlink(curr);
	  curr->gch.next = (GCObject*)&G(L)->tmudata_tail;  /* link `curr' at the end of `collected' list */
	  curr->gch.prev = G(L)->tmudata_tail.prev;
	  G(L)->tmudata_tail.prev->gch.next = curr;
      G(L)->tmudata_tail.prev = curr;
#else !LUA_REFCOUNT
	  curr->gch.next = NULL;  /* link `curr' at the end of `collected' list */
      *lastcollected = curr;
      lastcollected = &curr->gch.next;
#endif LUA_REFCOUNT
    }
  }
  /* insert collected udata with gc event into `tmudata' list */
#if LUA_REFCOUNT
//  *lastcollected = G(L)->tmudata_head.next;
//  G(L)->tmudata_head.next = collected;
#else !LUA_REFCOUNT
  *lastcollected = G(L)->tmudata;
  G(L)->tmudata = collected;
#endif LUA_REFCOUNT
  return deadmem;
}


static void removekey (lua_State *L, Node *n) {
  (void)L;
  setnilvalue(gval(n));  /* remove corresponding value ... */
  if (iscollectable(gkey(n)))
    setttype(gkey(n), LUA_TNONE);  /* dead key; remove it */
}


static void traversetable (GCState *st, Table *h) {
  int i;
  int weakkey = 0;
  int weakvalue = 0;
  const TObject *mode;
  markvalue(st, h->metatable);
  lua_assert(h->lsizenode || h->node == st->g->dummynode);
  mode = gfasttm(st->g, h->metatable, TM_MODE);
  if (mode && ttisstring(mode)) {  /* is there a weak mode? */
    weakkey = (strchr(svalue(mode), 'k') != NULL);
    weakvalue = (strchr(svalue(mode), 'v') != NULL);
    if (weakkey || weakvalue) {  /* is really weak? */
      GCObject **weaklist;
      h->marked &= ~(KEYWEAK | VALUEWEAK);  /* clear bits */
      h->marked |= cast(lu_byte, (weakkey << KEYWEAKBIT) |
                                 (weakvalue << VALUEWEAKBIT));
      weaklist = (weakkey && weakvalue) ? &st->wkv :
                              (weakkey) ? &st->wk :
                                          &st->wv;
      h->gclist = *weaklist;  /* must be cleared after GC, ... */
      *weaklist = valtogco(h);  /* ... so put in the appropriate list */
    }
  }
  if (!weakvalue) {
    i = h->sizearray;
    while (i--)
      markobject(st, &h->array[i]);
  }
  i = sizenode(h);
  while (i--) {
    Node *n = gnode(h, i);
    if (!ttisnil(gval(n))) {
      lua_assert(!ttisnil(gkey(n)));
      condmarkobject(st, gkey(n), !weakkey);
      condmarkobject(st, gval(n), !weakvalue);
    }
  }
}


static void traverseproto (GCState *st, Proto *f) {
  int i;
  stringmark(f->source);
  for (i=0; i<f->sizek; i++) {  /* mark literal strings */
    if (ttisstring(f->k+i) || ttiswstring(f->k+i))
      stringmark(tsvalue(f->k+i));
  }
  for (i=0; i<f->sizeupvalues; i++)  /* mark upvalue names */
    stringmark(f->upvalues[i]);
  for (i=0; i<f->sizep; i++)  /* mark nested protos */
    markvalue(st, f->p[i]);
  for (i=0; i<f->sizelocvars; i++)  /* mark local-variable names */
    stringmark(f->locvars[i].varname);
  lua_assert(luaG_checkcode(f));
}



static void traverseclosure (GCState *st, Closure *cl) {
  if (cl->c.isC) {
    int i;
    for (i=0; i<cl->c.nupvalues; i++)  /* mark its upvalues */
      markobject(st, &cl->c.upvalue[i]);
  }
  else {
    int i;
    lua_assert(cl->l.nupvalues == cl->l.p->nups);
    markvalue(st, hvalue(&cl->l.g));
    markvalue(st, cl->l.p);
    for (i=0; i<cl->l.nupvalues; i++) {  /* mark its upvalues */
      UpVal *u = cl->l.upvals[i];
      if (!u->marked) {
        markobject(st, &u->value);
        u->marked = 1;
      }
    }
  }
}


static void checkstacksizes (lua_State *L, StkId max) {
  int used = L->ci - L->base_ci;  /* number of `ci' in use */
  if (4*used < L->size_ci && 2*BASIC_CI_SIZE < L->size_ci)
    luaD_reallocCI(L, L->size_ci/2);  /* still big enough... */
  else condhardstacktests(luaD_reallocCI(L, L->size_ci));
  used = max - L->stack;  /* part of stack in use */
  if (4*used < L->stacksize && 2*(BASIC_STACK_SIZE+EXTRA_STACK) < L->stacksize)
    luaD_reallocstack(L, L->stacksize/2);  /* still big enough... */
  else condhardstacktests(luaD_reallocstack(L, L->stacksize));
}


static void traversestack (GCState *st, lua_State *L1) {
  StkId o, lim;
  CallInfo *ci;
  lua_State *L = st->L;
  (void)L;
  markobject(st, gt(L1));
  lim = L1->top;
  for (ci = L1->base_ci; ci <= L1->ci; ci++) {
    lua_assert(ci->top <= L1->stack_last);
    lua_assert(ci->state & (CI_C | CI_HASFRAME | CI_SAVEDPC));
    if (lim < ci->top)
      lim = ci->top;
  }
  for (o = L1->stack; o < L1->top; o++)
    markobject(st, o);
  for (; o <= lim; o++)
    setnilvalue(o);
  checkstacksizes(L1, lim);
}


static void propagatemarks (GCState *st) {
  while (st->tmark) {  /* traverse marked objects */
    switch (st->tmark->gch.tt) {
      case LUA_TTABLE: {
        Table *h = gcotoh(st->tmark);
        st->tmark = h->gclist;
        traversetable(st, h);
        break;
      }
      case LUA_TFUNCTION: {
        Closure *cl = gcotocl(st->tmark);
        st->tmark = cl->c.gclist;
        traverseclosure(st, cl);
        break;
      }
      case LUA_TTHREAD: {
        lua_State *th = gcototh(st->tmark);
#if LUA_REFCOUNT
        st->tmark = th->gclist_head.next;
#else !LUA_REFCOUNT
        st->tmark = th->gclist;
#endif LUA_REFCOUNT
        traversestack(st, th);
        if (G(th)->userGCFunction)
		{
		  st->L = th;
          G(th)->userGCFunction(st);
		}
        break;
      }
      case LUA_TPROTO: {
        Proto *p = gcotop(st->tmark);
        st->tmark = p->gclist;
        traverseproto(st, p);
        break;
      }
#if LUA_REFCOUNT
	  case LUA_TNIL: {
        st->tmark = NULL;
        break;
      }
#endif LUA_REFCOUNT
      default: lua_assert(0);
    }
  }
}


static int valismarked (const TObject *o) {
  if (ttisstring(o) || ttiswstring(o))
    stringmark(tsvalue(o));  /* strings are `values', so are never weak */
  return !iscollectable(o) || testbit(o->value.gc->gch.marked, 0);
}


/*
** clear collected keys from weaktables
*/
static void cleartablekeys (lua_State *L, GCObject *l) {
  while (l) {
    Table *h = gcotoh(l);
    int i = sizenode(h);
    lua_assert(h->marked & KEYWEAK);
    while (i--) {
      Node *n = gnode(h, i);
      if (!valismarked(gkey(n)))  /* key was collected? */
        removekey(L, n);  /* remove entry from table */
    }
    l = h->gclist;
  }
}


/*
** clear collected values from weaktables
*/
static void cleartablevalues (lua_State *L, GCObject *l) {
  while (l) {
    Table *h = gcotoh(l);
    int i = h->sizearray;
    lua_assert(h->marked & VALUEWEAK);
    while (i--) {
      TObject *o = &h->array[i];
      if (!valismarked(o))  /* value was collected? */
        setnilvalue(o);  /* remove value */
    }
    i = sizenode(h);
    while (i--) {
      Node *n = gnode(h, i);
      if (!valismarked(gval(n)))  /* value was collected? */
        removekey(L, n);  /* remove entry from table */
    }
    l = h->gclist;
  }
}


static void freeobj (lua_State *L, GCObject *o) {
  switch (o->gch.tt) {
    case LUA_TPROTO: luaF_freeproto(L, gcotop(o)); break;
    case LUA_TFUNCTION: luaF_freeclosure(L, gcotocl(o)); break;
    case LUA_TUPVAL: luaM_freelem(L, gcotouv(o)); break;
    case LUA_TTABLE: luaH_free(L, gcotoh(o)); break;
    case LUA_TTHREAD: {
      lua_assert(gcototh(o) != L && gcototh(o) != G(L)->mainthread);
      luaE_freethread(L, gcototh(o));
      break;
    }
    case LUA_TSTRING: {
      luaM_free(L, o, sizestring(gcotots(o)->tsv.len));
      break;
    }
    case LUA_TWSTRING: {
      luaM_free(L, o, sizewstring(gcototws(o)->tsv.len));
      break;
    }
    case LUA_TUSERDATA: {
      luaM_free(L, o, sizeudata(gcotou(o)->uv.len));
      break;
    }
#if LUA_REFCOUNT
	case LUA_TNIL: {
      break;  // Do nothing.
    }
#endif LUA_REFCOUNT
    default: lua_assert(0);
  }
}


static int sweeplist (lua_State *L, GCObject **p, GCObject *tail, int limit) {
#if 0
  GCObject **start = p;
#endif 0
  GCObject *curr;
  int count = 0;  /* number of collected items */
  while ((curr = *p) != tail) {
    if (curr->gch.marked > limit) {
      unmark(curr);
      p = &curr->gch.next;
    }
    else {
      count++;
      *p = curr->gch.next;
#if LUA_REFCOUNT
	  if (curr->gch.prev) {
        Unlink(curr);
        curr->gch.next->gch.prev = (GCObject*)p;
	  }
#endif LUA_REFCOUNT
      freeobj(L, curr);
    }
  }

#if 0
  while (1)
  {
	  curr = *p;
	  if (curr == NULL)
		  break;
	  if (curr->gch.next)
		  lua_assert(!curr->gch.next->gch.prev  ||  curr->gch.next->gch.prev == curr);
	  if (curr->gch.prev)
		  lua_assert(curr->gch.prev->gch.next == curr);
      start = &curr->gch.next;
  }
#endif 0
  return count;
}


static void sweepstrings (lua_State *L, int all) {
  int i;
  for (i=0; i<G(L)->strt.size; i++) {  /* for each list */
    G(L)->strt.nuse -= sweeplist(L, &G(L)->strt.hash[i], NULL, all);
  }
}


static void checkSizes (lua_State *L, size_t deadmem) {
  /* check size of string hash */
  if (G(L)->strt.nuse < cast(ls_nstr, G(L)->strt.size/4) &&
      G(L)->strt.size > MINSTRTABSIZE*2)
    luaS_resize(L, G(L)->strt.size/2);  /* table is too big */
  /* check size of buffer */
  if (luaZ_sizebuffer(&G(L)->buff) > LUA_MINBUFFER*2) {  /* buffer too big? */
    size_t newsize = luaZ_sizebuffer(&G(L)->buff) / 2;
    luaZ_resizebuffer(L, &G(L)->buff, newsize);
  }
  G(L)->GCthreshold = 2*G(L)->nblocks - deadmem;  /* new threshold */
}


static void do1gcTM (lua_State *L, Udata *udata) {
  const TObject *tm = fasttm(L, udata->uv.metatable, TM_GC);
  if (tm != NULL) {
    setobj2s(L->top, tm);
    setuvalue(L->top+1, udata);
    L->top += 2;
    luaD_call(L, L->top - 2, 0);
  }
}


void luaC_callGCTM (lua_State *L) {
  lu_byte oldah = L->allowhook;
  L->allowhook = 0;  /* stop debug hooks during GC tag methods */
  L->top++;  /* reserve space to keep udata while runs its gc method */
#if LUA_REFCOUNT
  while (G(L)->tmudata_head.next != (GCObject*)&G(L)->tmudata_tail) {
    GCObject *o = G(L)->tmudata_head.next;
    Udata *udata = gcotou(o);
    G(L)->tmudata_head.next = udata->uv.next;  /* remove udata from `tmudata' */
    udata->uv.prev = (GCObject*)&G(L)->rootudata_head;
    udata->uv.next = G(L)->rootudata_head.next;
	if (udata->uv.next)
      udata->uv.next->uv.prev = o;
    G(L)->rootudata_head.next = o;
#else !LUA_REFCOUNT
  while (G(L)->tmudata != NULL) {
    GCObject *o = G(L)->tmudata;
    Udata *udata = gcotou(o);
    G(L)->tmudata = udata->uv.next;  /* remove udata from `tmudata' */
    udata->uv.next = G(L)->rootudata;  /* return it to `root' list */
    G(L)->rootudata = o;
#endif LUA_REFCOUNT
    setuvalue(L->top - 1, udata);  /* keep a reference to it */
    unmark(o);
    markfinalized(udata);
    do1gcTM(L, udata);
  }
  L->top--;
  L->allowhook = oldah;  /* restore hooks */
}


void luaC_sweep (lua_State *L, int all) {
  if (all) all = 256;  /* larger than any mark */
#if LUA_REFCOUNT
  sweeplist(L, &G(L)->rootudata_head.next, (GCObject*)&G(L)->rootudata_tail, all);
#else !LUA_REFCOUNT
  sweeplist(L, &G(L)->rootudata, NULL, all);
#endif LUA_REFCOUNT
  sweepstrings(L, all);
#if LUA_REFCOUNT
  sweeplist(L, &G(L)->rootgc_head.next, (GCObject*)&G(L)->rootgc_tail, all);
#else !LUA_REFCOUNT
  sweeplist(L, &G(L)->rootgc, NULL, all);
#endif LUA_REFCOUNT
}


/* mark root set */
static void markroot (GCState *st, lua_State *L) {
  int i;
  global_State *g = st->g;
  markobject(st, defaultmeta(L));
  for (i = 0; i < LUA_NTYPES; i++)
  {
    markobject(st, defaultmetatypes(L, i));
  }
  markobject(st, registry(L));
  traversestack(st, g->mainthread);
  if (L != g->mainthread)  /* another thread is running? */
    markvalue(st, L);  /* cannot collect it */
  if (G(L)->userGCFunction)
  {
	st->L = g->mainthread;
    G(L)->userGCFunction(st);
  }
}


static size_t mark (lua_State *L) {
  size_t deadmem;
  GCState st;
  GCObject *wkv;
  st.L = L;
  st.g = G(L);
  st.tmark = NULL;
  st.wkv = st.wk = st.wv = NULL;
  markroot(&st, L);
  propagatemarks(&st);  /* mark all reachable objects */
  cleartablevalues(L, st.wkv);
  cleartablevalues(L, st.wv);
  wkv = st.wkv;  /* keys must be cleared after preserving udata */
  st.wkv = NULL;
  st.wv = NULL;
  deadmem = luaC_separateudata(L);  /* separate userdata to be preserved */
  marktmu(&st);  /* mark `preserved' userdata */
  propagatemarks(&st);  /* remark, to propagate `preserveness' */
  cleartablekeys(L, wkv);
  /* `propagatemarks' may resuscitate some weak tables; clear them too */
  cleartablekeys(L, st.wk);
  cleartablevalues(L, st.wv);
  cleartablekeys(L, st.wkv);
  cleartablevalues(L, st.wkv);
  return deadmem;
}


void luaC_collectgarbage (lua_State *L) {
  size_t deadmem = mark(L);
  luaC_sweep(L, 0);
  checkSizes(L, deadmem);
  luaC_callGCTM(L);
}


void luaC_link (lua_State *L, GCObject *o, lu_byte tt) {
#if LUA_REFCOUNT
  o->gch.prev = (GCObject*)&G(L)->rootgc_head;
  o->gch.next = G(L)->rootgc_head.next;
  o->gch.next->gch.prev = o;
  G(L)->rootgc_head.next = o;
#else !LUA_REFCOUNT
  o->gch.next = G(L)->rootgc;
  G(L)->rootgc = o;
#endif
  o->gch.marked = 0;
  o->gch.tt = tt;
}

