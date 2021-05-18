///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "LuaPlusAddons.h"


#ifdef _MSC_VER
#pragma warning(disable: 4213)
#endif _MSC_VER

/* macro to `unsign' a character */
#ifndef uchar
#define uchar(c)        ((unsigned char)(c))
#endif

const lua_WChar *wmemchr(const lua_WChar *S, lua_WChar C, size_t N)
        {for (; 0 < N; ++S, --N)
                if (*S == C)
                        return (S);
        return (0); }
int wmemcmp(const lua_WChar *S1, const lua_WChar *S2, size_t N)
        {for (; 0 < N; ++S1, ++S2, --N)
                if (*S1 != *S2)
                        return (*S1 < *S2 ? -1 : +1);
        return (0); }



typedef long sint32;	/* a signed version for size_t */


static int str_len (lua_State *L) {
  size_t l;
  luaL_checklwstring(L, 1, &l);
  lua_pushnumber(L, l);
  return 1;
}


static sint32 posrelat (sint32 pos, size_t len) {
  /* relative string position: negative means back from end */
  return (pos>=0) ? pos : (sint32)len+pos+1;
}


static int str_sub (lua_State *L) {
  size_t l;
  const lua_WChar *s = luaL_checklwstring(L, 1, &l);
  sint32 start = posrelat(luaL_checklong(L, 2), l);
  sint32 end = posrelat(luaL_optlong(L, 3, -1), l);
  if (start < 1) start = 1;
  if (end > (sint32)l) end = l;
  if (start <= end)
    lua_pushlwstring(L, s+start-1, end-start+1);
  else lua_pushwliteral(L, L"");
  return 1;
}


static int str_lower (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const lua_WChar *s = luaL_checklwstring(L, 1, &l);
  luaL_wbuffinit(L, &b);
  for (i=0; i<l; i++)
    luaL_putwchar(&b, towlower(s[i]));
  luaL_pushresult(&b);
  return 1;
}


static int str_upper (lua_State *L) {
  size_t l;
  size_t i;
  luaL_Buffer b;
  const lua_WChar *s = luaL_checklwstring(L, 1, &l);
  luaL_wbuffinit(L, &b);
  for (i=0; i<l; i++)
    luaL_putwchar(&b, toupper(s[i]));
  luaL_pushresult(&b);
  return 1;
}

static int str_rep (lua_State *L) {
  size_t l;
  luaL_Buffer b;
  const lua_WChar *s = luaL_checklwstring(L, 1, &l);
  int n = luaL_checkint(L, 2);
  luaL_wbuffinit(L, &b);
  while (n-- > 0)
    luaL_addlwstring(&b, s, l);
  luaL_pushresult(&b);
  return 1;
}


/*jj
static int str_concat (lua_State *L) {
  luaL_Buffer b;
  size_t lsep;
  const lua_WChar *sep = luaL_opt_lustr(L, 2, L"", &lsep);
  int n, i;
  luaL_check_type(L, 1, LUA_TTABLE);
  luaL_buffinit(L, &b, 1);
  n = lua_getn(L, 1);
  for (i=1; i<=n; i++) {
    lua_rawgeti(L, 1, i);
    luaL_addvalue(&b);
    if (i != n)
      luaL_addlwstring(&b, sep, lsep);
  }
  luaL_pushresult(&b);
  return 1;
}
*/

static int str_byte (lua_State *L) {
  size_t l;
  const lua_WChar *s = luaL_checklwstring(L, 1, &l);
  sint32 pos = posrelat(luaL_optlong(L, 2, 1), l);
  luaL_argcheck(L, 0 < pos && (size_t)(pos) <= l, 2,  "out of range");
  lua_pushnumber(L, s[pos-1]);
  return 1;
}


static int str_char (lua_State *L) {
  int n = lua_gettop(L);  /* number of arguments */
  int i;
  luaL_Buffer b;
  luaL_wbuffinit(L, &b);
  for (i=1; i<=n; i++) {
    int c = luaL_checkint(L, i);
    luaL_argcheck(L, (lua_WChar)(c) == c, i, "invalid value");
    luaL_putwchar(&b, (lua_WChar)(c));
  }
  luaL_pushresult(&b);
  return 1;
}



/*
** {======================================================
** PATTERN MATCHING
** =======================================================
*/

#ifndef MAX_CAPTURES
#define MAX_CAPTURES 32  /* arbitrary limit */
#endif


#define CAP_UNFINISHED	(-1)
#define CAP_POSITION	(-2)

typedef struct MatchState {
  const lua_WChar *src_init;  /* init of source string */
  const lua_WChar *src_end;  /* end (`\0') of source string */
  lua_State *L;
  int level;  /* total number of captures (finished or unfinished) */
  struct {
    const lua_WChar *init;
    sint32 len;
  } capture[MAX_CAPTURES];
} MatchState;


#define ESC		((lua_WChar)('%'))
#define SPECIALS	L"^$*+?.([%-"


static int check_capture (MatchState *ms, int l) {
  l -= '1';
  if (l < 0 || l >= ms->level || ms->capture[l].len == CAP_UNFINISHED)
    return luaL_error(ms->L, "invalid capture index");
  return l;
}


static int capture_to_close (MatchState *ms) {
  int level = ms->level;
  for (level--; level>=0; level--)
    if (ms->capture[level].len == CAP_UNFINISHED) return level;
  return luaL_error(ms->L, "invalid pattern capture");
}


static const lua_WChar *luaI_classend (MatchState *ms, const lua_WChar *p) {
  switch (*p++) {
    case ESC:
      if (*p == '\0')
        luaL_error(ms->L, "malformed pattern (ends with `%')");
      return p+1;
    case '[':
      if (*p == '^') p++;
      do {  /* look for a `]' */
        if (*p == '\0')
          luaL_error(ms->L, "malformed pattern (missing `]')");
        if (*(p++) == ESC && *p != '\0')
          p++;  /* skip escapes (e.g. `%]') */
      } while (*p != ']');
      return p+1;
    default:
      return p;
  }
}


static int match_class (lua_WChar c, lua_WChar cl) {
  int res;
  switch (towlower(cl)) {
    case 'a' : res = iswalpha(c); break;
    case 'c' : res = iswcntrl(c); break;
    case 'd' : res = iswdigit(c); break;
    case 'l' : res = iswlower(c); break;
    case 'p' : res = iswpunct(c); break;
    case 's' : res = iswspace(c); break;
    case 'u' : res = iswupper(c); break;
    case 'w' : res = iswalnum(c); break;
    case 'x' : res = iswxdigit(c); break;
    case 'z' : res = (c == '\0'); break;
    default: return (cl == c);
  }
  return (iswlower(cl) ? res : !res);
}


static int matchbracketclass (lua_WChar c, const lua_WChar *p, const lua_WChar *ec) {
  int sig = 1;
  if (*(p+1) == '^') {
    sig = 0;
    p++;  /* skip the `^' */
  }
  while (++p < ec) {
    if (*p == ESC) {
      p++;
      if (match_class(c, *p))
        return sig;
    }
    else if ((*(p+1) == '-') && (p+2 < ec)) {
      p+=2;
      if (*(p-2) <= c && c <= *p)
        return sig;
    }
    else if (*p == c) return sig;
  }
  return !sig;
}


static int luaI_singlematch (lua_WChar c, const lua_WChar *p, const lua_WChar *ep) {
  switch (*p) {
    case '.':  /* matches any char */
      return 1;
    case ESC:
      return match_class(c, *(p+1));
    case '[':
      return matchbracketclass(c, p, ep-1);
    default:
      return (*p == c);
  }
}


static const lua_WChar *match (MatchState *ms, const lua_WChar *s, const lua_WChar *p);


static const lua_WChar *matchbalance (MatchState *ms, const lua_WChar *s,
                                   const lua_WChar *p) {
  if (*p == 0 || *(p+1) == 0)
    luaL_error(ms->L, "unbalanced pattern");
  if (*s != *p) return NULL;
  else {
    int b = *p;
    int e = *(p+1);
    int cont = 1;
    while (++s < ms->src_end) {
      if (*s == e) {
        if (--cont == 0) return s+1;
      }
      else if (*s == b) cont++;
    }
  }
  return NULL;  /* string ends out of balance */
}


static const lua_WChar *max_expand (MatchState *ms, const lua_WChar *s,
                                 const lua_WChar *p, const lua_WChar *ep) {
  sint32 i = 0;  /* counts maximum expand for item */
  while ((s+i)<ms->src_end && luaI_singlematch(*(s+i), p, ep))
    i++;
  /* keeps trying to match with the maximum repetitions */
  while (i>=0) {
    const lua_WChar *res = match(ms, (s+i), ep+1);
    if (res) return res;
    i--;  /* else didn't match; reduce 1 repetition to try again */
  }
  return NULL;
}


static const lua_WChar *min_expand (MatchState *ms, const lua_WChar *s,
                                 const lua_WChar *p, const lua_WChar *ep) {
  for (;;) {
    const lua_WChar *res = match(ms, s, ep+1);
    if (res != NULL)
      return res;
    else if (s<ms->src_end && luaI_singlematch(*s, p, ep))
      s++;  /* try with one more repetition */
    else return NULL;
  }
}


static const lua_WChar *start_capture (MatchState *ms, const lua_WChar *s,
                                    const lua_WChar *p, int what) {
  const lua_WChar *res;
  int level = ms->level;
  if (level >= MAX_CAPTURES) luaL_error(ms->L, "too many captures");
  ms->capture[level].init = s;
  ms->capture[level].len = what;
  ms->level = level+1;
  if ((res=match(ms, s, p)) == NULL)  /* match failed? */
    ms->level--;  /* undo capture */
  return res;
}


static const lua_WChar *end_capture (MatchState *ms, const lua_WChar *s,
                                  const lua_WChar *p) {
  int l = capture_to_close(ms);
  const lua_WChar *res;
  ms->capture[l].len = s - ms->capture[l].init;  /* close capture */
  if ((res = match(ms, s, p)) == NULL)  /* match failed? */
    ms->capture[l].len = CAP_UNFINISHED;  /* undo capture */
  return res;
}


static const lua_WChar *match_capture (MatchState *ms, const lua_WChar *s, int l) {
  size_t len;
  l = check_capture(ms, l);
  len = ms->capture[l].len;
  if ((size_t)(ms->src_end-s) >= len &&
      memcmp(ms->capture[l].init, s, len) == 0)
    return s+len;
  else return NULL;
}


static const lua_WChar *match (MatchState *ms, const lua_WChar *s, const lua_WChar *p) {
  init: /* using goto's to optimize tail recursion */
  switch (*p) {
    case '(':  /* start capture */
      if (*(p+1) == ')')  /* position capture? */
        return start_capture(ms, s, p+2, CAP_POSITION);
      else
        return start_capture(ms, s, p+1, CAP_UNFINISHED);
    case ')':  /* end capture */
      return end_capture(ms, s, p+1);
    case ESC:  /* may be %[0-9] or %b */
      if (iswdigit(*(p+1))) {  /* capture? */
        s = match_capture(ms, s, *(p+1));
        if (s == NULL) return NULL;
        p+=2; goto init;  /* else return match(ms, s, p+2) */
      }
      else if (*(p+1) == 'b') {  /* balanced string? */
        s = matchbalance(ms, s, p+2);
        if (s == NULL) return NULL;
        p+=4; goto init;  /* else return match(ms, s, p+4); */
      }
      else goto dflt;  /* case default */
    case '\0':  /* end of pattern */
      return s;  /* match succeeded */
    case '$':
      if (*(p+1) == '\0')  /* is the `$' the last char in pattern? */
        return (s == ms->src_end) ? s : NULL;  /* check end of string */
      else goto dflt;
    default: dflt: {  /* it is a pattern item */
      const lua_WChar *ep = luaI_classend(ms, p);  /* points to what is next */
      int m = s<ms->src_end && luaI_singlematch(*s, p, ep);
      switch (*ep) {
        case '?': {  /* optional */
          const lua_WChar *res;
          if (m && ((res=match(ms, s+1, ep+1)) != NULL))
            return res;
          p=ep+1; goto init;  /* else return match(ms, s, ep+1); */
        }
        case '*':  /* 0 or more repetitions */
          return max_expand(ms, s, p, ep);
        case '+':  /* 1 or more repetitions */
          return (m ? max_expand(ms, s+1, p, ep) : NULL);
        case '-':  /* 0 or more repetitions (minimum) */
          return min_expand(ms, s, p, ep);
        default:
          if (!m) return NULL;
          s++; p=ep; goto init;  /* else return match(ms, s+1, ep); */
      }
    }
  }
}



static const lua_WChar *lmemfind (const lua_WChar *s1, size_t l1,
                               const lua_WChar *s2, size_t l2) {
  if (l2 == 0) return s1;  /* empty strings are everywhere */
  else if (l2 > l1) return NULL;  /* avoids a negative `l1' */
  else {
    const lua_WChar *init;  /* to search for a `*s2' inside `s1' */
    l2--;  /* 1st char will be checked by `memchr' */
    l1 = l1-l2;  /* `s2' cannot be found after that */
    while (l1 > 0 && (init = (const lua_WChar *)wmemchr(s1, *s2, l1)) != NULL) {
      init++;   /* 1st char is already checked */
      if (wmemcmp(init, s2+1, l2) == 0)
        return init-1;
      else {  /* correct `l1' and `s1' to try again */
        l1 -= init-s1;
        s1 = init;
      }
    }
    return NULL;  /* not found */
  }
}


static void push_onecapture (MatchState *ms, int i) {
  int l = ms->capture[i].len;
  if (l == CAP_UNFINISHED) luaL_error(ms->L, "unfinished capture");
  if (l == CAP_POSITION)
    lua_pushnumber(ms->L, ms->capture[i].init - ms->src_init + 1);
  else
    lua_pushlwstring(ms->L, ms->capture[i].init, l);
}


static int push_captures (MatchState *ms, const lua_WChar *s, const lua_WChar *e) {
  int i;
  luaL_checkstack(ms->L, ms->level, "too many captures");
  if (ms->level == 0 && s) {  /* no explicit captures? */
    lua_pushlwstring(ms->L, s, e-s);  /* return whole match */
    return 1;
  }
  else {  /* return all captures */
    for (i=0; i<ms->level; i++)
      push_onecapture(ms, i);
    return ms->level;  /* number of strings pushed */
  }
}


static const lua_WChar* lua_WChar_pbrk(const lua_WChar* str, const lua_WChar* control)
{
	while (*str != 0)
	{
		const lua_WChar* set = control;
		while (*set != 0)
		{
			if (*set++ == *str)
				return str;
		}
		str++;
	}

	return NULL;
}


static const lua_WChar* lua_WChar_chr(const lua_WChar* str, lua_WChar ch)
{
	while (*str != 0)
	{
		if (*str == ch)
			return (str);
		str++;
	}

	return NULL;
}


static void lua_WChar_ncpy(lua_WChar* dest, const lua_WChar* src, size_t count)
{
	while (count != 0  &&  (*dest++ = *src++))
		--count;

	if (count != 0)
	{
		while (--count)
			*dest++ = 0;
	}
}


static size_t lua_WChar_len(const lua_WChar* str)
{
	const lua_WChar* ptr = str;
	while (*ptr != 0)
	{
		ptr++;
	}

	return (size_t)(ptr - str);
}


static int str_find (lua_State *L) {
  size_t l1, l2;
  const lua_WChar *s = luaL_checklwstring(L, 1, &l1);
  const lua_WChar *p = luaL_checklwstring(L, 2, &l2);
  sint32 init = posrelat(luaL_optlong(L, 3, 1), l1) - 1;
  luaL_argcheck(L, 0 <= init && (size_t)(init) <= l1, 3, "out of range");
  if (lua_toboolean(L, 4) ||  /* explicit request? */
      lua_WChar_pbrk(p, SPECIALS) == NULL) {  /* or no special characters? */
    /* do a plain search */
    const lua_WChar *s2 = lmemfind(s+init, l1-init, p, l2);
    if (s2) {
      lua_pushnumber(L, s2-s+1);
      lua_pushnumber(L, s2-s+l2);
      return 2;
    }
  }
  else {
    MatchState ms;
    int anchor = (*p == '^') ? (p++, 1) : 0;
    const lua_WChar *s1=s+init;
    ms.L = L;
    ms.src_init = s;
    ms.src_end = s+l1;
    do {
      const lua_WChar *res;
      ms.level = 0;
      if ((res=match(&ms, s1, p)) != NULL) {
        lua_pushnumber(L, s1-s+1);  /* start */
        lua_pushnumber(L, res-s);   /* end */
        return push_captures(&ms, NULL, 0) + 2;
      }
    } while (s1++<ms.src_end && !anchor);
  }
  lua_pushnil(L);  /* not found */
  return 1;
}


static int gfind_aux (lua_State *L) {
  MatchState ms;
  const lua_WChar *s = lua_towstring(L, lua_upvalueindex(1));
  size_t ls = lua_strlen(L, lua_upvalueindex(1));
  const lua_WChar *p = lua_towstring(L, lua_upvalueindex(2));
  const lua_WChar *src;
  ms.L = L;
  ms.src_init = s;
  ms.src_end = s+ls;
  for (src = s + (size_t)lua_tonumber(L, lua_upvalueindex(3));
       src <= ms.src_end;
       src++) {
    const lua_WChar *e;
    ms.level = 0;
    if ((e = match(&ms, src, p)) != NULL) {
      int newstart = e-s;
      if (e == src) newstart++;  /* empty match? go at least one position */
      lua_pushnumber(L, newstart);
      lua_replace(L, lua_upvalueindex(3));
      return push_captures(&ms, src, e);
    }
  }
  return 0;  /* not found */
}


static int gfind (lua_State *L) {
  luaL_checkwstring(L, 1);
  luaL_checkwstring(L, 2);
  lua_settop(L, 2);
  lua_pushnumber(L, 0);
  lua_pushcclosure(L, gfind_aux, 3);
  return 1;
}


static void add_s (MatchState *ms, luaL_Buffer *b,
                   const lua_WChar *s, const lua_WChar *e) {
  lua_State *L = ms->L;
  if (lua_iswstring(L, 3)) {
    const lua_WChar *news = lua_towstring(L, 3);
    size_t l = lua_strlen(L, 3);
    size_t i;
    for (i=0; i<l; i++) {
      if (news[i] != ESC)
        luaL_putwchar(b, news[i]);
      else {
        i++;  /* skip ESC */
        if (!iswdigit(news[i]))
          luaL_putwchar(b, news[i]);
        else {
          int level = check_capture(ms, news[i]);
          push_onecapture(ms, level);
          luaL_addvalue(b);  /* add capture to accumulated result */
        }
      }
    }
  }
  else {  /* is a function */
    int n;
    lua_pushvalue(L, 3);
    n = push_captures(ms, s, e);
    lua_call(L, n, 1);
    if (lua_iswstring(L, -1))
      luaL_addvalue(b);  /* add return to accumulated result */
    else
      lua_pop(L, 1);  /* function result is not a string: pop it */
  }
}


static int str_gsub (lua_State *L) {
  size_t srcl;
  const lua_WChar *src = luaL_checklwstring(L, 1, &srcl);
  const lua_WChar *p = luaL_checkwstring(L, 2);
  int max_s = luaL_optint(L, 4, srcl+1);
  int anchor = (*p == '^') ? (p++, 1) : 0;
  int n = 0;
  MatchState ms;
  luaL_Buffer b;
  luaL_argcheck(L,
    lua_gettop(L) >= 3 && (lua_iswstring(L, 3) || lua_isfunction(L, 3)),
    3, "string or function expected");
  luaL_wbuffinit(L, &b);
  ms.L = L;
  ms.src_init = src;
  ms.src_end = src+srcl;
  while (n < max_s) {
    const lua_WChar *e;
    ms.level = 0;
    e = match(&ms, src, p);
    if (e) {
      n++;
      add_s(&ms, &b, src, e);
    }
    if (e && e>src) /* non empty match? */
      src = e;  /* skip it */
    else if (src < ms.src_end)
      luaL_putwchar(&b, *src++);
    else break;
    if (anchor) break;
  }
  luaL_addlwstring(&b, src, ms.src_end-src);
  luaL_pushresult(&b);
  lua_pushnumber(L, n);  /* number of substitutions */
  return 2;
}

/* }====================================================== */


/* maximum size of each formatted item (> len(format('%99.99f', -1e308))) */
#define MAX_ITEM	512
/* maximum size of each format specification (such as '%-099.99d') */
#define MAX_FORMAT	20


static void luaI_addquoted (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  const lua_WChar *s = luaL_checklwstring(L, arg, &l);
  luaL_putwchar(b, '"');
  while (l--) {
    switch (*s) {
      case '"':  case '\\':  case '\n':
        luaL_putwchar(b, '\\');
        luaL_putwchar(b, *s);
        break;
      case '\0': luaL_addlwstring(b, L"\\000", 4); break;
      default: luaL_putwchar(b, *s);
    }
    s++;
  }
  luaL_putwchar(b, '"');
}


/*static void luaI_addquotedbinary (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  const lua_WChar *s = luaL_checklwstring(L, arg, &l);
  luaL_putwchar(b, '"');
  while (l--) {
    switch (*s) {
      case '"':  case '\\':
        luaL_putwchar(b, '\\');
        luaL_putwchar(b, *s);
        break;
      case '\0': luaL_addlwstring(b, L"\\000", 4); break;
      default:
		  if (iswprint(*s)  &&  *s != 13  &&  *s != 10)
			  luaL_putwchar(b, *s);
		  else
		  {
			  lua_WChar buf[5];
			  _snwprintf(buf, L"\\%03d", *s);
			  luaL_addlwstring(b, buf, 4);
		  }
    }
    s++;
  }
  luaL_putwchar(b, '"');
}
*/

static void luaI_addquotedbinary (lua_State *L, luaL_Buffer *b, int arg) {
  size_t l;
  if (lua_type(L, arg) == LUA_TWSTRING)
  {
	  const lua_WChar *s = luaL_checklwstring(L, arg, &l);
	  luaL_putwchar(b, 'L');
	  luaL_putwchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putwchar(b, '\\');
			luaL_putwchar(b, *s);
			break;
			case '\a':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'a');		break;
			case '\b':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'b');		break;
			case '\f':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'f');		break;
			case '\n':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'n');		break;
			case '\r':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'r');		break;
			case '\t':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 't');		break;
			case '\v':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'v');		break;
			default:
				if (*s < 256  &&  isprint((unsigned char)*s))
				{
					luaL_putwchar(b, *s);
				}
				else
				{
					lua_WChar str[10];
					_snwprintf(str, 10, L"\\x%04x", (unsigned long)*s);
					luaL_addwstring(b, str);
				}
		}
		s++;
	  }
	  luaL_putwchar(b, '"');
  }
  else
  {
	  const char *s = luaL_checklstring(L, arg, &l);
	  luaL_putwchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putwchar(b, '\\');
			luaL_putwchar(b, *s);
			break;
			case '\a':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'a');		break;
			case '\b':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'b');		break;
			case '\f':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'f');		break;
			case '\n':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'n');		break;
			case '\r':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'r');		break;
			case '\t':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 't');		break;
			case '\v':		luaL_putwchar(b, '\\');  luaL_putwchar(b, 'v');		break;
			default:
				if (isprint((unsigned char)*s))
				{
					luaL_putwchar(b, *s);
				}
				else
				{
					lua_WChar str[10];
					_snwprintf(str, 10, L"\\x%02x", (unsigned long)(unsigned char)*s);
					luaL_addwstring(b, str);
				}
		}
		s++;
	  }
	  luaL_putwchar(b, '"');

/*
	  const char *s = luaL_checklstring(L, arg, &l);
	  luaL_putchar(b, '"');
	  while (l--) {
		switch (*s) {
		  case '"':  case '\\':
			luaL_putchar(b, '\\');
			luaL_putchar(b, *s);
			break;
		  case '\0': luaL_addlstring(b, "\\000", 4); break;
		  default:
			  if (isprint(*s)  &&  *s != 13  &&  *s != 10)
				  luaL_putchar(b, *s);
			  else
			  {
				  char buf[5];
				  sprintf(buf, "\\%03d", *s);
				  luaL_addlstring(b, buf, 4);
			  }
		}
		s++;
	  }
	  luaL_putchar(b, '"');
*/
  }
}




static const lua_WChar *scanformat (lua_State *L, const lua_WChar *strfrmt,
                                 lua_WChar *form, int *hasprecision) {
  const lua_WChar *p = strfrmt;
  while (lua_WChar_chr(L"-+ #0", *p)) p++;  /* skip flags */
  if (iswdigit(*p)) p++;  /* skip width */
  if (iswdigit(*p)) p++;  /* (2 digits at most) */
  if (*p == '.') {
    p++;
    *hasprecision = 1;
    if (iswdigit(*p)) p++;  /* skip precision */
    if (iswdigit(*p)) p++;  /* (2 digits at most) */
  }
  if (iswdigit(*p))
    luaL_error(L, "invalid format (width or precision too long)");
  if (p-strfrmt+2 > MAX_FORMAT)  /* +2 to include `%' and the specifier */
    luaL_error(L, "invalid format (too long)");
  form[0] = '%';
  lua_WChar_ncpy(form+1, strfrmt, p-strfrmt+1);
  form[p-strfrmt+2] = 0;
  return p;
}


int wstr_format_helper (luaL_Buffer* b, lua_State *L, int arg) {
  size_t sfl;
  const lua_WChar *strfrmt = luaL_checklwstring(L, arg, &sfl);
  const lua_WChar *strfrmt_end = strfrmt+sfl;
  luaL_wbuffinit(L, b);
  while (strfrmt < strfrmt_end) {
    if (*strfrmt != '%')
      luaL_putwchar(b, *strfrmt++);
    else if (*++strfrmt == '%')
      luaL_putwchar(b, *strfrmt++);  /* %% */
    else { /* format item */
      lua_WChar form[MAX_FORMAT];  /* to store the format (`%...') */
      lua_WChar buff[MAX_ITEM];  /* to store the formatted item */
      int hasprecision = 0;
      if (iswdigit(*strfrmt) && *(strfrmt+1) == '$')
        return luaL_error(L, "obsolete `format' option (d$)");
      arg++;
      strfrmt = scanformat(L, strfrmt, form, &hasprecision);
      switch (*strfrmt++) {
        case 'b':
		{
		  buff[1] = buff[2] = buff[3] = buff[4] = buff[5] = buff[6] = buff[7] = buff[8] = 0;
		  switch (*strfrmt++)
		  {
		  case 'b': {
            unsigned int num = (unsigned int)luaL_checkint(L, arg);
		    buff[0] = (unsigned char)num;
            luaL_addlwstring(b, buff, 1);
			break;
          }
		  case 'd': {
            unsigned int num = (unsigned int)luaL_checkint(L, arg);
		    *(unsigned int*)(&buff) = num;
            luaL_addlwstring(b, buff, 4);
			break;
          }
		  case 'w': {
            unsigned int num = (unsigned int)luaL_checkint(L, arg);
			*(unsigned short*)(&buff) = (unsigned short)num;
            luaL_addlwstring(b, buff, 2);
			break;
          }
		  case 'f': {
            float numF = (float)luaL_checknumber(L, arg);
			*(float*)(&buff) = numF;
            luaL_addlwstring(b, buff, 4);
			break;
          }
		  case 'F': {
            double numD = (double)luaL_checknumber(L, arg);
			*(double*)(&buff) = numD;
            luaL_addlwstring(b, buff, 8);
			break;
          }
			  
		  default:
			  break;
		  }
		  buff[0] = 0;
		  break;
		}
        case 'c':  case 'd':  case 'i':
          _snwprintf(buff, 512, form, luaL_checkint(L, arg));
          break;
        case 'o':  case 'u':  case 'x':  case 'X':
          //?? How should this be for integers?
          _snwprintf(buff, 512, form, (unsigned int)(luaL_checknumber(L, arg)));
          break;
        case 'e':  case 'E': case 'f':
        case 'g': case 'G':
          _snwprintf(buff, 512, form, luaL_checknumber(L, arg));
          break;
        case 'q':
          luaI_addquoted(L, b, arg);
          continue;  /* skip the `addsize' at the end */
        case 'Q':
          luaI_addquotedbinary(L, b, arg);
          continue;  /* skip the `addsize' at the end */
        case 's': {
          size_t l;
          const lua_WChar *s = luaL_checklwstring(L, arg, &l);
          if (!hasprecision && l >= 100) {
            /* no precision and string is too long to be formatted;
               keep original string */
            lua_pushvalue(L, arg);
            luaL_addvalue(b);
            continue;  /* skip the `addsize' at the end */
          }
          else {
            _snwprintf(buff, 512, form, s);
            break;
          }
        }
        default:  /* also treat cases `pnLlh' */
          luaL_error(L, "invalid option in `format'");
      }
      luaL_addlwstring(b, buff, lua_WChar_len(buff));
    }
  }

  return 1;
}


static int str_format (lua_State *L) {
  luaL_Buffer b;
  int ret = wstr_format_helper(&b, L, 1);
  if (ret == 0)
	  return 0;
  luaL_pushresult(&b);
  return 1;
}


static int str_lualex(lua_State *L) {
  size_t l = 0;
  const lua_WChar *str = luaL_checklwstring(L, 1, &l);
  int isWide = luaL_checkint(L, 2) != 0;
  size_t i;
  luaL_Buffer b;
  luaL_wbuffinit(L, &b);

  for (i = 0; i < l; ++i)
  {
//	int needUnicodeZero = 1;

    switch (str[i])
	{
      case '\\':
		++i;
        switch (str[i]) {
          case 'a': luaL_putwchar(&b, '\a'); break;
          case 'b': luaL_putwchar(&b, '\b'); break;
          case 'f': luaL_putwchar(&b, '\f'); break;
          case 'n': luaL_putwchar(&b, '\n'); break;
          case 'r': luaL_putwchar(&b, '\r'); break;
          case 't': luaL_putwchar(&b, '\t'); break;
          case 'v': luaL_putwchar(&b, '\v'); break;
          case 'x': {
			  int ch;
			  ++i;
			  ch = tolower(str[i]);
              if (!isdigit(ch) && !(ch >= 'a' && ch <= 'f') )
			  {
				  --i;
				  luaL_putwchar(&b, 'x');
			  }
			  else {  /* \xxx */
				  int c = 0;
				  int i = 0;
				  int numDigits = isWide ? 4 : 2;
				  do {
					  ch = towlower(str[i]);
					  if (iswdigit((wint_t)ch))
					    c = 16*c + (ch-'0');
					  else if (ch >= 'a' && ch <= 'f')
						c = 16*c + (ch-'a') + 10;
					  ++i;
					  ch = towlower(str[i]);
				  } while (++i<numDigits && iswdigit((wint_t)ch) || (ch >= 'a' && ch <= 'f'));
				  luaL_putwchar(&b, c);
//				  needUnicodeZero = 0;
			  }
			  break;
          }
          default: {
            if (!iswdigit(str[i]))
				luaL_putwchar(&b, str[i]);
            else {  /* \xxx */
              int c = 0;
              int count = 0;
              do {
                c = 10*c + (str[i]-'0');
				++i;
              } while (++count<3 && iswdigit(str[i]));
              luaL_putwchar(&b, c);
            }
          }
        }
        break;
      default:
		  luaL_putwchar(&b, str[i]);
    }
	if (isWide) //  &&  needUnicodeZero)
	  luaL_putwchar(&b, str[i]);
  }

  luaL_pushresult(&b);
  return 1;
}



static const luaL_reg strlib[] = {
  {"len", str_len},
  {"sub", str_sub},
  {"lower", str_lower},
  {"upper", str_upper},
  {"char", str_char},
  {"rep", str_rep},
  {"byte", str_byte},
//  {"concat", str_concat},
  {"format", str_format},
  {"find", str_find},
  {"gfind", gfind},
  {"gsub", str_gsub},
  {"lualex", str_lualex},
  {NULL, NULL}
};


/*
** Open string library
*/
LUALIB_API int luaopen_wstring (lua_State *L) {
  luaL_openlib(L, LUA_WSTRLIBNAME, strlib, 0);
  lua_getglobal(L, LUA_WSTRLIBNAME);
  lua_setdefaultmetatable(L, LUA_TWSTRING);
  return 0;
}

