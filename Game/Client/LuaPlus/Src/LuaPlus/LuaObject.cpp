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
#include "lfunc.h"
#include "lobject.h"
#include "lstate.h"
#include "lstring.h"
#include "ltable.h"
#include "lvm.h"
} // extern "C"
#include "LuaPlus.h"
#include <string.h>
#include <malloc.h>

extern "C" TObject *negindex (lua_State *L, int index);
extern "C" TObject *luaA_index (lua_State *L, int idx);

// From the Loki reference implementation
template<int> struct CompileTimeError;
template<> struct CompileTimeError<true> {};

////////////////////////////////////////////////////////////////////////////////
// macro STATIC_CHECK
// Invocation: STATIC_CHECK(expr, id)
// where:
// expr is a compile-time integral or pointer expression
// id is a C++ identifier that does not need to be defined
// If expr is zero, id will appear in a compile-time error message.
////////////////////////////////////////////////////////////////////////////////
#define STATIC_CHECK(expr, msg) \
    { CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; }

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

static size_t lua_WChar_len(const lua_WChar* str)
{
	const lua_WChar* ptr = str;
	while (*ptr != 0)
	{
		ptr++;
	}

	return (size_t)(ptr - str);
}


void lua_pushtobject(lua_State *L, void* tobject);

#ifndef api_check
#define api_check(L, o)		luaplus_assert(o)
#endif

LuaObject::LuaObject() throw() :
	m_state(NULL),
	m_next(NULL),
	m_prev(NULL)
{
	// If this compile time assertion fires, change the line:
	//    	unsigned char m_object[12];
	// in LuaObject.h to match the size of lua_TObject.
	STATIC_CHECK(sizeof(lua_TObject) == TOBJECT_SIZE, lua_TObject_must_be_12_bytes);

	setnilvalue2n(&m_object);
}


LuaObject::LuaObject(LuaState* state) throw()
{
	// If this compile time assertion fires, change the line:
	//    	unsigned char m_object[12];
	// in LuaObject.h to match the size of lua_TObject.
	STATIC_CHECK(sizeof(lua_TObject) == TOBJECT_SIZE, lua_TObject_must_be_12_bytes);

	AddToUsedList(state);
	setnilvalue2n(&m_object);
}


LuaObject::LuaObject(LuaState* state, int stackIndex) throw()
{
	setnilvalue2n(&m_object);
	AddToUsedList(state, *luaA_index(state->m_state, stackIndex));
}


inline LuaObject::LuaObject(LuaState* state, const TObject* obj)
{
	luaplus_assert(obj);
	setnilvalue2n(&m_object);
	AddToUsedList(state, *obj);
}


LuaObject::LuaObject(const LuaObject& src) throw()
{
	setnilvalue2n(&m_object);
	AddToUsedList(src.m_state, src.m_object);
}


LuaObject::LuaObject(const LuaStackObject& src) throw()
{
	setnilvalue2n(&m_object);
	AddToUsedList(src.m_state, *luaA_index(src.m_state->m_state, src.m_stackIndex));
}


LuaObject& LuaObject::operator=(const LuaObject& src) throw()
{
	RemoveFromUsedList();
	AddToUsedList(src.m_state, src.m_object);
	return *this;
}


LuaObject& LuaObject::operator=(const LuaStackObject& src) throw()
{
	RemoveFromUsedList();
	AddToUsedList(src.m_state, *luaA_index(src.m_state->m_state, src.m_stackIndex));
	return *this;
}


LuaObject::~LuaObject()
{
	RemoveFromUsedList();
}


/**
	Resets the LuaObject by removing itself from the used GC list and setting the state to NULL.
**/
void LuaObject::Reset()
{
	RemoveFromUsedList();
	m_state = NULL;
}


// Mirrors lua_typename().
const char* LuaObject::TypeName() const
{
	int t = Type();
	return (t == LUA_TNONE) ? "no value" : luaT_typenames[t];
}


// Mirrors lua_type().
int LuaObject::Type() const
{
	luaplus_assert(m_state);
	return ttype(&m_object);
}


// Mirrors lua_isnil().
bool LuaObject::IsNil() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TNIL;
}


// Mirrors lua_istable().
bool LuaObject::IsTable() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TTABLE;
}


// Mirrors lua_isuserdata().
bool LuaObject::IsUserData() const
{
	luaplus_assert(m_state);
	return ttisuserdata(&m_object)  ||  ttislightuserdata(&m_object);
}


// Mirrors lua_iscfunction().
bool LuaObject::IsCFunction() const
{
	luaplus_assert(m_state);
	return iscfunction(&m_object);
}


// Behaves differently than lua_isinteger().  This function only tests for a value that is
// a real integer, not something that can be converted to a integer.
bool LuaObject::IsInteger() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TNUMBER;
}


// Behaves differently than lua_isnumber().  This function only tests for a value that is
// a real number, not something that can be converted to a number.
bool LuaObject::IsNumber() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TNUMBER;
}


// Behaves differently than lua_isstring().  This function only tests for a value that is
// a real string, not something that can be converted to a string.
bool LuaObject::IsString() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TSTRING;
}


bool LuaObject::IsWString() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TWSTRING;
}


// Mirrors lua_isinteger().
bool LuaObject::IsConvertibleToInteger() const
{
	luaplus_assert(m_state);
	lua_State *L = GetCState();		(void)L;
	const TObject* o = &m_object;
	TObject n;
	setnilvalue2n(&n);
	return tonumber(o, &n);
}


// Mirrors lua_isnumber().
bool LuaObject::IsConvertibleToNumber() const
{
	luaplus_assert(m_state);
	lua_State *L = GetCState();		(void)L;
	const TObject* o = &m_object;
	TObject n;
	setnilvalue2n(&n);
	return tonumber(o, &n);
}


// Mirrors lua_isstring().
bool LuaObject::IsConvertibleToString() const
{
	luaplus_assert(m_state);
	int t = Type();
	return (t == LUA_TSTRING || t == LUA_TNUMBER);
}


// Mirrors lua_iswstring().
bool LuaObject::IsConvertibleToWString() const
{
	luaplus_assert(m_state);
	int t = Type();
	return (t == LUA_TNUMBER || t == LUA_TWSTRING);
}


// Mirrors lua_isfunction().
bool LuaObject::IsFunction() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TFUNCTION;
}


// Mirrors lua_isnone().
bool LuaObject::IsNone() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TNONE;
}


// Mirrors lua_islightuserdata().
bool LuaObject::IsLightUserData() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TLIGHTUSERDATA;
}


// Mirrors lua_isboolean().
bool LuaObject::IsBoolean() const
{
	luaplus_assert(m_state);
	return ttype(&m_object) == LUA_TBOOLEAN;
}


// Mirrors lua_tointeger()
int LuaObject::ToInteger()
{
	const TObject* o = &m_object;
	lua_State *L = GetCState();		(void)L;
	TObject n;
	if (tonumber(o, &n))
		return (int)nvalue(&m_object);
	else
		return 0;
}


// Mirrors lua_tonumber()
lua_Number LuaObject::ToNumber()
{
	const TObject* o = &m_object;
	lua_State *L = GetCState();		(void)L;
	TObject n;
	if (tonumber(o, &n))
		return nvalue(&m_object);
	else
		return 0;
}


// Mirrors lua_tostring().
const char* LuaObject::ToString()
{
	if (ttisstring(&m_object))
		return svalue(&m_object);
	else
	{
		const char *s;
		lua_lock(m_state->m_state);  /* `luaV_tostring' may create a new string */
		s = (luaV_tostring(m_state->m_state, &m_object) ? svalue(&m_object) : NULL);
		lua_unlock(m_state->m_state);
		return s;
	}
}


// Mirrors lua_towstring().
const lua_WChar* LuaObject::ToWString()
{
	if (ttiswstring(&m_object))
		return wsvalue(&m_object);
	else
	{
		const lua_WChar *s;
		lua_lock(m_state->m_state);  /* `luaV_tostring' may create a new string */
		s = (luaV_towstring(m_state->m_state, &m_object) ? wsvalue(&m_object) : NULL);
		lua_unlock(m_state->m_state);
		return s;
	}
}


size_t LuaObject::ToStrLen()
{
	if (ttisstring(&m_object)  ||  ttiswstring(&m_object))
		return tsvalue(&m_object)->tsv.len;
	else
	{
		size_t l;
		lua_lock(m_state->m_state);  /* `luaV_tostring' may create a new string */
		l = (luaV_tostring(m_state->m_state, &m_object) ? tsvalue(&m_object)->tsv.len : 0);
		lua_unlock(m_state->m_state);
		return l;
	}
}


int LuaObject::GetInteger() const
{
	luaplus_assert(m_state  &&  IsInteger());
	return (int)nvalue(&m_object);
}


float LuaObject::GetFloat() const
{
	luaplus_assert(m_state  &&  IsNumber());
	return (float)nvalue(&m_object);
}


double LuaObject::GetDouble() const
{
	luaplus_assert(m_state  &&  IsNumber());
	return (double)nvalue(&m_object);
}


lua_Number LuaObject::GetNumber() const
{
	luaplus_assert(m_state  &&  IsNumber());
	return (lua_Number)nvalue(&m_object);
}


const char* LuaObject::GetString() const
{
	luaplus_assert(m_state  &&  IsString());
	return svalue(&m_object);
}


const lua_WChar* LuaObject::GetWString()const
{
	luaplus_assert(m_state  &&  IsWString());
	return wsvalue(&m_object);
}


int LuaObject::StrLen()
{
	luaplus_assert(m_state  &&  (IsString()  ||  IsWString()));
	return tsvalue(&m_object)->tsv.len;
}


lua_CFunction LuaObject::GetCFunction() const
{
	luaplus_assert(m_state  &&  IsCFunction());
	return (!iscfunction(&m_object)) ? NULL : clvalue(&m_object)->c.f;
}


// Mirrors lua_touserdata().
void* LuaObject::GetUserData()
{
	luaplus_assert(m_state  &&  IsUserData());

	StkId o = &m_object;
	switch (ttype(o))
	{
		case LUA_TUSERDATA:
		{
			if (uvalue(o)->uv.len & 4)
				return *(void**)(uvalue(o) + 1);
			else
				return (uvalue(o) + 1);
		}
		case LUA_TLIGHTUSERDATA: return pvalue(o);
		default: return NULL;
	}
}


// Mirrors lua_topointer.
const void* LuaObject::GetLuaPointer()
{
	luaplus_assert(m_state);
	StkId o = &m_object;
	switch (ttype(o))
	{
		case LUA_TTABLE: return hvalue(o);
		case LUA_TFUNCTION: return clvalue(o);
		case LUA_TUSERDATA:
		case LUA_TLIGHTUSERDATA:
			return GetUserData();
		default: return NULL;
	}
}


// No equivalent.
void* LuaObject::GetLightUserData() const
{
	luaplus_assert(m_state  &&  IsLightUserData());
	return pvalue(&m_object);
}


// Mirrors lua_toboolean().
bool LuaObject::GetBoolean() const
{
	luaplus_assert(m_state  &&  IsBoolean()  ||  IsNil());
	return !l_isfalse(&m_object);
}


/**
**/
LuaObject LuaObject::Clone()
{
	lua_State *L = GetCState();		(void)L;
	if (IsTable())
	{
		LuaPlus::LuaObject tableObj(m_state);
		sethvalue(&tableObj.m_object, luaH_new(m_state->m_state, hvalue(&m_object)->sizearray, hvalue(&m_object)->lsizenode));
		tableObj.SetMetaTable(GetMetaTable());

		for (LuaTableIterator it(*this); it; ++it)
		{
			if (it.GetValue().IsTable())
			{
				LuaPlus::LuaObject clonedChildTableObj = it.GetValue().Clone();
				tableObj.SetObject(it.GetKey(), clonedChildTableObj);
			}
			else
				tableObj.SetObject(it.GetKey(), it.GetValue());
		}

		return tableObj;
	}

	return LuaObject(m_state, &m_object);
}


static inline int InternalGetTop(LuaState* state)
{
  return (state->m_state->top - state->m_state->base);
}


//#define api_incr_top(L)   (api_check(L, L->top<L->ci->top), L->top++)

#define api_incr_top(L) \
	{if (L->top >= L->ci->top) lua_checkstack(L, 1); L->top++;}

static inline void InternalPushTObject(lua_State *L, void* tobject)
{
	TObject* tobj = (TObject*)tobject;
	lua_lock(L);
	setobj2s(L->top, tobj);
	api_incr_top(L);
	lua_unlock(L);
}


LuaStackObject LuaObject::PushStack()
{
	luaplus_assert(m_state);
	InternalPushTObject(m_state->m_state, &m_object);
	return LuaStackObject(m_state, InternalGetTop(m_state));
}


LuaObject LuaObject::GetMetaTable()
{
	luaplus_assert(m_state);
	Table *mt = luaT_getmetatable(m_state->m_state, &m_object);
	LuaPlus::LuaObject ret(m_state);
	sethvalue2n(&ret.m_object, mt);
	return ret;
}


void LuaObject::SetMetaTable(const LuaObject& valueObj)
{
	luaplus_assert(m_state);
	luaT_setmetatable(m_state->m_state, &m_object, hvalue(&valueObj.m_object));
}


int LuaObject::GetN()
{
	luaplus_assert(m_state);
	LuaAutoBlock autoBlock(GetCState());
	PushStack();
	return luaL_getn(m_state->m_state, -1);
}


void LuaObject::SetN(int n)
{
	luaplus_assert(m_state);
	LuaAutoBlock autoBlock(GetCState());
	PushStack();
	luaL_setn(m_state->m_state, -1, n);
}


void LuaObject::Insert(LuaObject& obj)
{
	luaplus_assert(m_state);
	LuaAutoBlock autoBlock(GetCState());
	LuaPlus::LuaObject tableObj = m_state->GetGlobal("table");
	LuaPlus::LuaObject funcObj = tableObj["insert"];
	luaplus_assert(funcObj.IsFunction());
    LuaCall callObj(funcObj);
	callObj << *this << obj << LuaRun();
}

	
void LuaObject::Insert(int index, LuaObject& obj)
{
	luaplus_assert(m_state);
	LuaAutoBlock autoBlock(GetCState());
	LuaPlus::LuaObject tableObj = m_state->GetGlobal("table");
	LuaPlus::LuaObject funcObj = tableObj["insert"];
	luaplus_assert(funcObj.IsFunction());
    LuaCall callObj(funcObj);
	callObj << *this << index << obj << LuaRun();
}

	
void LuaObject::Remove(int index)
{
	luaplus_assert(m_state);
	LuaAutoBlock autoBlock(GetCState());
	LuaPlus::LuaObject tableObj = m_state->GetGlobal("table");
	LuaPlus::LuaObject funcObj = tableObj["remove"];
	luaplus_assert(funcObj.IsFunction());
    LuaCall callObj(funcObj);
	callObj << *this << index << LuaRun();
}

	
void LuaObject::Sort()
{
	luaplus_assert(m_state);
	LuaAutoBlock autoBlock(GetCState());
	LuaPlus::LuaObject tableObj = m_state->GetGlobal("table");
	LuaPlus::LuaObject funcObj = tableObj["sort"];
	luaplus_assert(funcObj.IsFunction());
    LuaCall callObj(funcObj);
	callObj << *this << LuaRun();
}

	
int LuaObject::GetCount()
{
	luaplus_assert(m_state);
	PushStack();
	int count = lua_getn(m_state->m_state, m_state->GetTop());
	m_state->Pop();
	return count;
}


int LuaObject::GetTableCount()
{
	int count = 0;
	for (LuaTableIterator it(*this); it; ++it)
	{
		count++;
	}
	return count;
}


/**
	Creates a table called [name] within the current LuaObject.

	@param key The name of the table to create.
	@param size The size of the table.
	@return Returns the object representing the newly created table.
**/
LuaObject LuaObject::CreateTable(const char* key, int narray, int lnhash)
{
	luaplus_assert(m_state);
	LuaPlus::LuaObject ret = LuaObject(m_state);
	sethvalue2n(&ret.m_object, luaH_new(m_state->m_state, narray, lnhash));
	SetTableHelper(key, ret.m_object);
	return ret;
}


/**
	Creates a table called [key] within the current LuaStackObject.

	@param index The index of the table to create.
	@param size The size of the table.
	@return Returns the object representing the newly created table.
**/
LuaObject LuaObject::CreateTable(int key, int narray, int lnhash)
{
	luaplus_assert(m_state);
	LuaPlus::LuaObject ret = LuaObject(m_state);
	sethvalue2n(&ret.m_object, luaH_new(m_state->m_state, narray, lnhash));
	SetTableHelper(key, ret.m_object);
	return ret;
}
	

/**
	Creates a table called [key] within the current LuaStackObject.

	@param index The index of the table to create.
	@param size The size of the table.
	@return Returns the object representing the newly created table.
**/
LuaObject LuaObject::CreateTable(LuaObject& key, int narray, int lnhash)
{
	luaplus_assert(m_state);
	LuaPlus::LuaObject ret = LuaObject(m_state);
	sethvalue2n(&ret.m_object, luaH_new(m_state->m_state, narray, lnhash));
	SetTableHelper(key, ret.m_object);
	return ret;
}
	

LuaObject LuaObject::GetByName(const char* name)
{
	return (*this)[name];
}

LuaObject LuaObject::GetByIndex(int index)
{
	luaplus_assert(m_state);
	lua_State* L = m_state->m_state;
#ifdef _DEBUG
	StkId o = &m_object;	(void)o;
	api_check(L, ttistable(o));
#endif _DEBUG
	TObject obj;
	setnvalue2n(&obj, index);
	const TObject* v = luaV_gettable(L, &m_object, &obj, 0);
	return LuaObject(m_state, v);
}

LuaObject LuaObject::GetByObject(const LuaObject& obj)
{
	luaplus_assert(m_state);
	const TObject* v = luaV_gettable(m_state->m_state, &m_object, (TObject*)&obj.m_object, 0);
	return LuaObject(m_state, v);
}


LuaObject LuaObject::GetByObject(const LuaStackObject& obj)
{
	luaplus_assert(m_state);
	lua_State* L = *this;
	const TObject* v = luaV_gettable(m_state->m_state, &m_object, luaA_index(L, obj.m_stackIndex), 0);
	return LuaObject(m_state, v);
}


LuaObject LuaObject::operator[](const char* name)
{
	luaplus_assert(m_state);
	lua_State* L = m_state->m_state;
	TObject str;
	setnilvalue2n(&str);

	// It's safe to assume that if name is not in the hash table, this function can return nil.
	size_t l = strlen(name);
	GCObject *o;
	lu_hash h = (lu_hash)l;  /* seed */
	size_t step = (l>>5)+1;  /* if string is too long, don't hash all its chars */
	size_t l1;
	for (l1=l; l1>=step; l1-=step)  /* compute hash */
		h = h ^ ((h<<5)+(h>>2)+(unsigned char)(name[l1-1]));
	for (o = G(L)->strt.hash[lmod(h, G(L)->strt.size)];
		o != NULL;
		o = o->gch.next)
	{
		TString *ts = gcotots(o);
		if (ts->tsv.tt == LUA_TSTRING && ts->tsv.len == l && (memcmp(name, getstr(ts), l) == 0))
		{
			setsvalue2n(&str, ts);
			break;
		}
	}

	if (ttype(&str) == LUA_TNIL)
		return LuaObject(m_state);

//	setsvalue(&str, luaS_newlstr(L, name, strlen(name)));
	const TObject* v = luaH_get(hvalue(&m_object), &str);
	setnilvalue(&str);
	return LuaObject(m_state, v);
}

LuaObject LuaObject::operator[](int index)
{
	luaplus_assert(m_state);
	StkId o = &m_object;
	api_check(L, ttistable(o));
	const TObject* v = luaH_getnum(hvalue(o), index);
	return LuaObject(m_state, v);
}

LuaObject LuaObject::operator[](const LuaObject& obj)
{
	luaplus_assert(m_state);
	const TObject* v = luaH_get(hvalue(&m_object), &obj.m_object);
	return LuaObject(m_state, v);
}

LuaObject LuaObject::operator[](const LuaStackObject& obj)
{
	luaplus_assert(m_state);
	lua_State* L = *this;
	const TObject* v = luaH_get(hvalue(&m_object), luaA_index(L, obj.m_stackIndex));
	return LuaObject(m_state, v);
}


LuaObject LuaObject::Lookup(const char* key)
{
	LuaPlus::LuaObject table = *this;

	size_t keyLen = strlen(key);
#if defined(_MSC_VER)
 	char* buf = (char*)_alloca(keyLen + 1);
#else !_MSC_VER
	char* buf(new char[keyLen + 1]);
#endif _MSC_VER
	strncpy(buf, key, keyLen);
	buf[keyLen] = 0;

	char* lastPos = (char*)key;

	while (true)
	{
		lua_Number num;

		char* curPos = strchr(lastPos, '.');
		if (!curPos)
		{
			if (luaO_str2d(lastPos, &num))
			{
#if !defined(_MSC_VER)
                delete [] buf;
#endif
				return table[(int)num];
			}

#if !defined(_MSC_VER)
            delete [] buf;
#endif
			return table[lastPos];
		}

		*curPos = 0;
		if (luaO_str2d(lastPos, &num))
		{
			table = table[(int)num];
		}
		else
		{
			table = table[lastPos];
		}

		if (table.IsNil())
        {
#if !defined(_MSC_VER)
            delete [] buf;
#endif
			return table;
        }

		lastPos = curPos + 1;
	}
    
#if !defined(_MSC_VER)
    delete [] buf;
#endif
}

LuaObject& LuaObject::SetNil(const char* key)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnilvalue2n(&valueObj);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetNil(int key)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnilvalue2n(&valueObj);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetNil(LuaObject& key)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnilvalue2n(&valueObj);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetBoolean(const char* key, bool value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setbvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetBoolean(int key, bool value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setbvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetBoolean(LuaObject& key, bool value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setbvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetInteger(const char* key, int value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetInteger(int key, int value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetInteger(LuaObject& key, int value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetNumber(const char* key, lua_Number value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetNumber(int key, lua_Number value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetNumber(LuaObject& key, lua_Number value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setnvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetString(const char* key, const char* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	if (value == NULL)
	{
		setnilvalue2n(&valueObj);
	}
	else
	{
		setsvalue2n(&valueObj, luaS_newlstr(m_state->m_state, value, strlen(value)));
	}
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetString(int key, const char* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	if (value == NULL)
	{
		setnilvalue2n(&valueObj);
	}
	else
	{
		setsvalue2n(&valueObj, luaS_newlstr(m_state->m_state, value, strlen(value)));
	}
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetString(LuaObject& key, const char* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	if (value == NULL)
	{
		setnilvalue2n(&valueObj);
	}
	else
	{
		setsvalue2n(&valueObj, luaS_newlstr(m_state->m_state, value, strlen(value)));
	}
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetWString(const char* key, const lua_WChar* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	if (value == NULL)
	{
		setnilvalue2n(&valueObj);
	}
	else
	{
		setwsvalue2n(&valueObj, luaS_newlwstr(m_state->m_state, value, lua_WChar_len(value)));
	}
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetWString(int key, const lua_WChar* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	if (value == NULL)
	{
		setnilvalue2n(&valueObj);
	}
	else
	{
		setwsvalue2n(&valueObj, luaS_newlwstr(m_state->m_state, value, lua_WChar_len(value)));
	}
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetWString(LuaObject& key, const lua_WChar* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	if (value == NULL)
	{
		setnilvalue2n(&valueObj);
	}
	else
	{
		setwsvalue2n(&valueObj, luaS_newlwstr(m_state->m_state, value, lua_WChar_len(value)));
	}
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetUserData(const char* key, void* value)
{
	luaplus_assert(m_state  &&  IsTable());
	Udata* u = luaS_newudata(m_state->m_state, 4);
	u->uv.len = 4;  // user data box bit is set.
	*(void**)(u + 1) = value;

	TObject valueObj;
	setuvalue2n(&valueObj, u);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetUserData(int key, void* value)
{
	luaplus_assert(m_state  &&  IsTable());
	Udata* u = luaS_newudata(m_state->m_state, 4);
	u->uv.len = 4;  // user data box bit is set.
	*(void**)(u + 1) = value;

	TObject valueObj;
	setuvalue2n(&valueObj, u);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetUserData(LuaObject& key, void* value)
{
	luaplus_assert(m_state  &&  IsTable());
	Udata* u = luaS_newudata(m_state->m_state, 4);
	u->uv.len = 4;  // user data box bit is set.
	*(void**)(u + 1) = value;

	TObject valueObj;
	setuvalue2n(&valueObj, u);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetLightUserData(const char* key, void* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setpvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetLightUserData(int key, void* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setpvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetLightUserData(LuaObject& key, void* value)
{
	luaplus_assert(m_state  &&  IsTable());
	TObject valueObj;
	setpvalue2n(&valueObj, value);
	return SetTableHelper(key, valueObj);
}


LuaObject& LuaObject::SetObject(const char* key, LuaObject& value)
{
	luaplus_assert(m_state  &&  IsTable());
	return SetTableHelper(key, value.m_object);
}


LuaObject& LuaObject::SetObject(int key, LuaObject& value)
{
	luaplus_assert(m_state  &&  IsTable());
	return SetTableHelper(key, value.m_object);
}


LuaObject& LuaObject::SetObject(LuaObject& key, LuaObject& value)
{
	luaplus_assert(m_state  &&  IsTable());
	return SetTableHelper(key, value.m_object);
}


void LuaObject::AssignNil(LuaState* state)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	setnilvalue(&m_object);
}

	
void LuaObject::AssignBoolean(LuaState* state, bool value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	setbvalue(&m_object, value);
}

	
void LuaObject::AssignInteger(LuaState* state, int value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	setnvalue(&m_object, value);
}

	
void LuaObject::AssignNumber(LuaState* state, lua_Number value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	setnvalue(&m_object, value);
}

	
void LuaObject::AssignString(LuaState* state, const char* value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	if (value == NULL)
	{
		setnilvalue(&m_object);
	}
	else
	{
		setsvalue(&m_object, luaS_newlstr(m_state->m_state, value, strlen(value)));
	}
}

	
void LuaObject::AssignWString(LuaState* state, const lua_WChar* value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	if (value == NULL)
	{
		setnilvalue(&m_object);
	}
	else
	{
		setwsvalue2n(&m_object, luaS_newlwstr(m_state->m_state, value, lua_WChar_len(value)));
	}
}

	
void LuaObject::AssignUserData(LuaState* state, void* value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	Udata* u = luaS_newudata(m_state->m_state, 4);
	u->uv.len = 4;  // user data box bit is set.
	*(void**)(u + 1) = value;
	setuvalue(&m_object, u);
}

	
void LuaObject::AssignLightUserData(LuaState* state, void* value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	setpvalue(&m_object, value);
}

	
void LuaObject::AssignObject(LuaObject& value)
{
	if (value.m_state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(value.m_state);
	}
	lua_State* L = GetCState();	(void)L;
	setobj(&m_object, &value.m_object);
}

	
void LuaObject::AssignNewTable(LuaState* state, int narray, int numhash)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	int lnhash = luaO_log2(numhash) + 1;
	sethvalue(&m_object, luaH_new(m_state->m_state, narray, lnhash));
}

	
void LuaObject::AssignTObject(LuaState* state, TObject* value)
{
	if (state != m_state)
	{
		RemoveFromUsedList();
		AddToUsedList(state);
	}
	lua_State* L = GetCState();	(void)L;
	setobj(&m_object, value);
}

	
/**
	Assuming the current object is a table, registers a C function called 
	[funcName] within the table.

	@param funcName The name of the function to register.
	@param function A pointer to the C function to register.
**/
void LuaObject::Register(const char* funcName, lua_CFunction function, int nupvalues)
{
	luaplus_assert(m_state);
	Closure* cl = luaF_newCclosure(m_state->m_state, nupvalues);
	cl->c.f = function;

	lua_State *L = m_state->m_state;
	L->top -= nupvalues;
	while (nupvalues--)
	{
		setobj2n(&cl->c.upvalue[nupvalues], L->top+nupvalues);
		setnilvalue(L->top+nupvalues);
	}

	TObject valueObj;
	setclvalue2n(&valueObj, cl);
	SetTableHelper(funcName, valueObj);
	setnilvalue(&valueObj);
}


void LuaObject::Register(const char* funcName, int (*func)(LuaState*), int nupvalues)
{
	unsigned char* buffer = (unsigned char*)lua_newuserdata(GetCState(), sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	Register(funcName, LPCD::LuaStateFunctionDispatcher, nupvalues + 1);
}


/**
	Assuming the current object is a table, unregisters the function called
	[funcName].

	@param funcName The name of the function to unregister.
**/
void LuaObject::Unregister(const char* funcName)
{
	luaplus_assert(m_state);
	SetNil(funcName);
}

inline LuaObject& LuaObject::SetTableHelper(const char* key, TObject& valueObj)
{
	lua_State *L = m_state->m_state;	(void)L;
	TObject keyObj;
	setsvalue2n(&keyObj, luaS_newlstr(m_state->m_state, key, strlen(key)));
	luaV_settable(m_state->m_state, &m_object, &keyObj, &valueObj);
	setnilvalue(&keyObj);
	return *this;
}


inline LuaObject& LuaObject::SetTableHelper(int key, TObject& valueObj)
{
	TObject keyObj;
	setnvalue2n(&keyObj, key);
	luaV_settable(m_state->m_state, &m_object, &keyObj, &valueObj);
	return *this;
}


inline LuaObject& LuaObject::SetTableHelper(LuaObject& key, TObject& valueObj)
{
	luaV_settable(m_state->m_state, &m_object, &key.m_object, &valueObj);
	return *this;
}


inline void LuaObject::AddToUsedList(LuaState* state)
{
	luaplus_assert(state);
	m_state = state;
	LuaPlus::LuaObject& headObject = *m_state->GetHeadObject();
	m_next = headObject.m_next;
	headObject.m_next = this;
	m_next->m_prev = this;
	m_prev = &headObject;
}


inline void LuaObject::AddToUsedList(LuaState* state, const lua_TObject& obj)
{
	luaplus_assert(state);
	m_state = state;
	LuaPlus::LuaObject& headObject = *m_state->GetHeadObject();
	m_next = headObject.m_next;
	headObject.m_next = this;
	m_next->m_prev = this;
	m_prev = &headObject;
	lua_State* L = GetCState();	(void)L;
	setobj(&m_object, &obj);
}


inline void LuaObject::RemoveFromUsedList()
{
	if (m_state)
	{
		// remove pOldNode from list
		m_prev->m_next = m_next;
		m_next->m_prev = m_prev;

		lua_State* L = GetCState();	(void)L;
		setnilvalue(&m_object);
	}
}


} // namespace LuaPlus
