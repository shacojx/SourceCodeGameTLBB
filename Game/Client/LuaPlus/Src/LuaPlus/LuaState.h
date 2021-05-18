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
#ifndef LUASTATE_H
#define LUASTATE_H

// #include "LuaObject.h"
#include "LuaPlusCD.h"

namespace LuaPlus
{
	class LuaState;
}

namespace LPCD
{
	inline int LuaStateFunctionDispatcher(lua_State* L)
	{
		typedef int (*Functor)(LuaPlus::LuaState*);
		unsigned char* buffer = LPCD::GetFirstUpValueAsUserData(L);
		Functor& func = *(Functor*)(buffer);
		return (*func)((LuaPlus::LuaState*)lua_getstateuserdata(L));
	}

	template <typename Callee>
	inline int LuaStateMemberDispatcher(lua_State* L)
	{
		typedef int (Callee::*Functor)(LuaPlus::LuaState*);
 		unsigned char* buffer = LPCD::GetFirstUpValueAsUserData(L);
		Callee& callee = *(Callee*)buffer;
		Functor& func = *(Functor*)(buffer + sizeof(Callee));
		return (callee.*func)((LuaPlus::LuaState*)lua_getstateuserdata(L));
	}

	template <typename Callee>
	inline int Object_MemberDispatcher_to_LuaState(lua_State* L)
	{
		typedef int (Callee::*Functor)(LuaPlus::LuaState*);
 		unsigned char* buffer = GetFirstUpValueAsUserData(L);
		Functor& func = *(Functor*)(buffer);
		Callee& callee = *(Callee*)GetObjectUserData(L);
		return (callee.*func)(((LuaPlus::LuaState*)lua_getstateuserdata(L)));
	}
}

#include "LuaObject.h"

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

class LuaState;


/**
	A lightweight lua_State wrapper.
**/
class LUAPLUS_CLASS LuaState
{
public:
	enum ErrorCodes
	{
		/* error codes for `lua_load' and `lua_pcall' */
		ERRRUN = 1,
		ERRFILE = 2,
		ERRSYNTAX = 3,
		ERRMEM = 4,
		ERRERR = 5,
	};

	enum Types
	{
		TNONE			= (-1),
		TNIL			= 0,
		TBOOLEAN		= 1,
		TLIGHTUSERDATA	= 2,
		TNUMBER			= 3,
		TSTRING			= 4,
		TTABLE			= 5,
		TFUNCTION		= 6,
		TUSERDATA		= 7,
		TTHREAD			= 8,
		TWSTRING		= 9,
		NTYPES			= 11,
	};

	enum DumpObjectTypes
	{
		DUMP_ALPHABETICAL		= 0x00000001,
		DUMP_WRITEALL			= 0x00000002,
		DUMP_WRITETABLEPOINTERS = 0x00000004,
	};


	///////////////////////////////////////////////////////////////////////////
	static LuaState* Create(bool initStandardLibrary = true, bool multithreaded = false);
	static LuaState* CreateThread(LuaState* parentState);
	static LuaState* CastState(lua_State* L);
	static void Destroy(LuaState* state);

	lua_CFunction AtPanic(lua_CFunction panicf);

	// LuaObject
	LuaPlus::LuaObject GetGlobals() throw();
	LuaPlus::LuaObject GetGlobal(const char *name);
	LuaPlus::LuaObject GetRegistry();

	int Equal(const LuaObject& o1, const LuaObject& o2);
	int LessThan(const LuaObject& o1, const LuaObject& o2);

	// Stack functions.
	LuaPlus::LuaStackObject Stack(int index)				{  return LuaStackObject(this, index);  }
	int GetTop();
	void SetTop(int index);
	void PushValue(int index);
	void PushValue(LuaStackObject object);
	void Remove(int index);
	void Insert(int index);
	void Replace(int index);
	int CheckStack(int size);
	void XMove(LuaState* to, int n);
	int Equal(int index1, int index2);
	int RawEqual(int index1, int index2);
	int LessThan(int index1, int index2);

	// push functions (C -> stack)
	LuaPlus::LuaStackObject PushNil();
	LuaPlus::LuaStackObject PushInteger(int n);
	LuaPlus::LuaStackObject PushNumber(lua_Number n);
	LuaPlus::LuaStackObject PushLString(const char *s, size_t len);
	LuaPlus::LuaStackObject PushLWString(const lua_WChar* s, size_t len);
	LuaPlus::LuaStackObject PushString(const char *s);
	LuaPlus::LuaStackObject PushWString(const lua_WChar* s);
	const char *PushVFString(const char *fmt, va_list argp);
	LuaPlus::LuaStackObject PushCClosure(lua_CFunction fn, int n);

	LuaPlus::LuaStackObject PushCClosure(int (*f)(LuaState*), int n)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(m_state, sizeof(f));
		memcpy(buffer, &f, sizeof(f));
		lua_pushcclosure(m_state, LPCD::LuaStateFunctionDispatcher, n + 1);
		return LuaStackObject(this, lua_gettop(m_state));
	}

	template <class Callee>
	LuaPlus::LuaStackObject PushCClosure(const Callee& callee, int (Callee::*f)(LuaState*), int n)
	{
		unsigned char* buffer = (unsigned char*)lua_newuserdata(m_state, sizeof(Callee) + sizeof(f));
		memcpy(buffer, &callee, sizeof(Callee));
		memcpy(buffer + sizeof(Callee), &f, sizeof(f));
		lua_pushcclosure(m_state, LPCD::LuaStateMemberDispatcher<Callee>, n + 1);
		return LuaStackObject(this, lua_gettop(m_state));
	}
	
	LuaPlus::LuaStackObject PushCFunction(lua_CFunction f);
	LuaPlus::LuaStackObject PushBoolean(bool value);
	LuaPlus::LuaStackObject PushLightUserData(void* p);

	// get functions (Lua -> stack)
	void GetTable(int index);
	void RawGet(int index);
	void RawGetI(int index, int n);
	LuaPlus::LuaStackObject NewTable(int size = 0);
	void* NewUserData(size_t size);
	LuaPlus::LuaStackObject GetMetaTable(int objindex);
	LuaPlus::LuaStackObject GetDefaultMetaTable(int type);
	LuaPlus::LuaStackObject GetGlobals_Stack();					// Backward compatible.
	LuaPlus::LuaStackObject GetGlobal_Stack(const char *name);
	LuaPlus::LuaStackObject GetRegistry_Stack();
	LuaPlus::LuaStackObject GetRef(int ref);


	// set functions(stack -> Lua)
	void SetTable(int index);
	void RawSet(int index);
	void RawSetI(int index, int n);
	void SetMetaTable(int index);
	void SetDefaultMetaTable(int type);
	void SetGlobals(int index);
	void SetGlobal(const char *name);
	int Ref(int lock);
	void Unref(int ref);

	// `load' and `do' functions (load and run Lua code)
	void Call(int nargs, int nresults);
	int PCall(int nargs, int nresults, int errf);
	int CPCall(lua_CFunction func, void* ud);

	int Dump(lua_Chunkwriter writer, void* data);

	int LoadFile(const char* filename);
	int DoFile(const char *filename);
	int DoString(const char *str);
	int DoWString(const lua_WChar* str, const char* name);
	int LoadBuffer(const char* buff, size_t size, const char* name);
	int DoBuffer(const char *buff, size_t size, const char *name);
	int LoadWBuffer(const lua_WChar* buff, size_t size, const char* name);
	int DoWBuffer(const lua_WChar* buff, size_t size, const char *name);

	// Coroutine functions
	int CoYield(int nresults);
	int CoResume(int narg);

	// Garbage-collection functions
	int GetGCThreshold();
	int GetGCCount();
	void SetGCThreshold(int newthreshold);

	// Miscellaneous functions
	int Error();

	int Next(int index);
	int GetN(int index);

	void Concat(int n);

	LuaPlus::LuaObject NewUserDataBox(void* u);
	LuaPlus::LuaStackObject NewUserDataBox_Stack(void* u);

	// Helper functions
	void Pop();
	void Pop(int amount);

	void CollectGarbage();

	bool DumpObject(const char* filename, const char* name, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);
	bool DumpObject(const char* filename, LuaObject& key, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);

	bool DumpObject(LuaStateOutFile& file, const char* name, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);
	bool DumpObject(LuaStateOutFile& file, LuaObject& key, LuaObject& value, unsigned int flags = DUMP_ALPHABETICAL,
					int indentLevel = 0, unsigned int maxIndentLevel = 0xffffffff);

	bool DumpGlobals(const char* filename, unsigned int flags = DUMP_ALPHABETICAL, unsigned int maxIndentLevel = 0xFFFFFFFF);
	bool DumpGlobals(LuaStateOutFile& file, unsigned int flags = DUMP_ALPHABETICAL, unsigned int maxIndentLevel = 0xFFFFFFFF);

	operator lua_State*()						{  return m_state;  }
	lua_State* GetCState()						{  return m_state;  }

	// Arg functions.
	int TypeError(int narg, const char* tname);
	int ArgError(int narg, const char* extramsg);
	const char* CheckLString(int numArg, size_t* len);
	const char* OptLString(int numArg, const char *def, size_t* len);
	lua_Number CheckNumber(int numArg);
	lua_Number OptNumber(int nArg, lua_Number def);
	void ArgCheck(bool condition, int numarg, const char* extramsg);
	const char* CheckString(int numArg);
	const char* OptString(int numArg, const char* def);
	int CheckInt(int numArg);
	long CheckLong(int numArg);
	int OptInt(int numArg, int def);
	long OptLong(int numArg, int def);
	void CheckStack(int sz, const char* msg);
	void CheckType(int narg, int t);
	void CheckAny(int narg);
	void* CheckUData(int ud, const char* tname);

	// Debug functions.
	int GetStack(int level, lua_Debug* ar);
	int GetInfo(const char* what, lua_Debug* ar);
	const char* GetLocal(const lua_Debug* ar, int n);
	const char* SetLocal(const lua_Debug* ar, int n);

	int SetHook(lua_Hook func, int mask, int count);
	lua_Hook GetHook();
	unsigned long GetHookMask();

	// Extra
	LuaPlus::LuaStackObject BoxPointer(void* u);
	void* UnBoxPointer(int stackIndex);

	lua_State* m_state;
	bool m_ownState;

	// We want the storage space, but we can't have the constructor run.
	LuaPlus::LuaObject* GetHeadObject()			{  return (LuaObject*)&m_headObject;  }
	LuaPlus::LuaObject* GetTailObject()			{  return (LuaObject*)&m_tailObject;  }

protected:
	LuaState(bool initStandardLibrary = true, bool multithreaded = false);
	LuaState(LuaState* script, bool initStandardLibrary = true);
	LuaState(lua_State* L);
	~LuaState();
	LuaState& operator=(LuaState& src);		// Not implemented.

	void Init(bool initStandardLibrary);

	bool CallFormatting(LuaObject& tableObj, LuaStateOutFile& file, int indentLevel,
			bool writeAll, bool alphabetical, bool writeTablePointers,
			unsigned int maxIndentLevel);

	void SetupStateEx();

	struct MiniLuaObject
	{
		LuaPlus::LuaObject* m_next;		   // only valid when in free list
		LuaPlus::LuaObject* m_prev;		   // only valid when in used list
	};

	MiniLuaObject m_headObject;
	MiniLuaObject m_tailObject;
	LuaPlus::LuaObject m_threadObj;
};


class LuaStateAuto
{
public:
    operator LuaState*()							{  return m_state;  }
    operator const LuaState*() const				{  return m_state;  }
    operator LuaState*() const						{  return m_state;  }
    LuaState& operator*()							{  return *m_state; }
    const LuaState& operator*() const				{  return *m_state; }
    LuaState* operator->()							{  return m_state;  }
    const LuaState* operator->() const				{  return m_state;  }
	LuaState* Get() const							{  return m_state;  }

	LuaStateAuto() : m_state(NULL) {}
    LuaStateAuto(LuaState* newState) : m_state(newState) {}
	LuaStateAuto& operator=(LuaState* newState)
	{
		Assign(newState);
		return *this;
	}

	~LuaStateAuto()
	{
		Assign(NULL);
	}

    LuaState* m_state;

protected:
    LuaStateAuto(const LuaStateAuto&);					// Not implemented.
    LuaStateAuto& operator=(const LuaStateAuto&);		// Not implemented.

	void Assign(LuaState* state)
	{
		if (m_state)
			LuaState::Destroy(m_state);
		m_state = state;
	}
};


class LuaStateOwner : public LuaStateAuto
{
public:
    LuaStateOwner(bool initStandardLibrary = true)
	{
		m_state = LuaState::Create(initStandardLibrary);
	}

    LuaStateOwner(LuaState* newState) : LuaStateAuto(newState) {}
	LuaStateOwner& operator=(LuaState* newState)
	{
		Assign(newState);
		return *this;
	}

	~LuaStateOwner()
	{
		Assign(NULL);
	}

private:
    LuaStateOwner(const LuaStateOwner&);				// Not implemented.
    LuaStateOwner& operator=(const LuaStateOwner&);		// Not implemented.
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class LuaStack
{
public:
	LuaPlus::LuaStack(LuaState* state) : m_state(state) {}
	LuaPlus::LuaStack(lua_State* L) : m_state(LuaState::CastState(L)) {}
	~LuaStack() {}

	LuaPlus::LuaStackObject operator[](int index)
	{
		return LuaStackObject(m_state, index);
	}
 
protected:
	LuaState* m_state;
};

} // namespace LuaPlus

#include "LuaAutoBlock.h"

namespace LuaPlus {

#define LUAFUNCTION_PRECALL() \
		lua_State* L = m_functionObj.GetCState(); \
		LuaAutoBlock autoBlock(L); \
		m_functionObj.PushStack();

#define LUAFUNCTION_POSTCALL(numArgs) \
		if (lua_pcall(L, numArgs, 1, 0)) \
		{ \
			const char* errorString = lua_tostring(L, -1); errorString; \
			luaplus_assert(0); \
		} \
		return LPCD::Get(LPCD::TypeWrapper<RT>(), L, -1);


/**
**/
template <typename RT>
class LuaFunction
{
public:
	LuaFunction(LuaObject& functionObj) :
		m_functionObj(functionObj)
	{
		luaplus_assert(m_functionObj.IsFunction());
	}

	LuaFunction(LuaState* state, const char* functionName)
	{
		m_functionObj = state->GetGlobals()[functionName];
		luaplus_assert(m_functionObj.IsFunction());
	}

	RT operator()()
	{
		LUAFUNCTION_PRECALL();
		LUAFUNCTION_POSTCALL(0);
	}

	template <typename P1>
	RT operator()(P1 p1)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LUAFUNCTION_POSTCALL(1);
	}

	template <typename P1, typename P2>
	RT operator()(P1 p1, P2 p2)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LUAFUNCTION_POSTCALL(2);
	}

	template <typename P1, typename P2, typename P3>
	RT operator()(P1 p1, P2 p2, P3 p3)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LUAFUNCTION_POSTCALL(3);
	}

	template <typename P1, typename P2, typename P3, typename P4>
	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LUAFUNCTION_POSTCALL(4);
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LPCD::Push(L, p5);
		LUAFUNCTION_POSTCALL(5);
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LPCD::Push(L, p5);
		LPCD::Push(L, p6);
		LUAFUNCTION_POSTCALL(6);
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	RT operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		LUAFUNCTION_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LPCD::Push(L, p5);
		LPCD::Push(L, p6);
		LPCD::Push(L, p7);
		LUAFUNCTION_POSTCALL(7);
	}

protected:
	LuaPlus::LuaObject m_functionObj;
};


#define LUAFUNCTIONVOID_PRECALL() \
		lua_State* L = m_functionObj.GetCState(); \
		LuaAutoBlock autoBlock(L); \
		m_functionObj.PushStack();

#define LUAFUNCTIONVOID_POSTCALL(numArgs) \
		if (lua_pcall(L, numArgs, 1, 0)) \
		{ \
			const char* errorString = lua_tostring(L, -1); (void)errorString;\
			luaplus_assert(0); \
		}


/**
**/
class LuaFunctionVoid
{
public:
	LuaFunctionVoid(LuaObject& functionObj) :
		m_functionObj(functionObj)
	{
		luaplus_assert(m_functionObj.IsFunction());
	}

	LuaFunctionVoid(LuaState* state, const char* functionName)
	{
		m_functionObj = state->GetGlobals()[functionName];
		luaplus_assert(m_functionObj.IsFunction());
	}

	void operator()()
	{
		LUAFUNCTIONVOID_PRECALL();
		LUAFUNCTIONVOID_POSTCALL(0);
	}

	template <typename P1>
	void operator()(P1 p1)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LUAFUNCTIONVOID_POSTCALL(1);
	}

	template <typename P1, typename P2>
	void operator()(P1 p1, P2 p2)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LUAFUNCTIONVOID_POSTCALL(2);
	}

	template <typename P1, typename P2, typename P3>
	void operator()(P1 p1, P2 p2, P3 p3)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LUAFUNCTIONVOID_POSTCALL(3);
	}

	template <typename P1, typename P2, typename P3, typename P4>
	void operator()(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LUAFUNCTIONVOID_POSTCALL(4);
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LPCD::Push(L, p5);
		LUAFUNCTIONVOID_POSTCALL(5);
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LPCD::Push(L, p5);
		LPCD::Push(L, p6);
		LUAFUNCTIONVOID_POSTCALL(6);
	}

	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	void operator()(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		LUAFUNCTIONVOID_PRECALL();
		LPCD::Push(L, p1);
		LPCD::Push(L, p2);
		LPCD::Push(L, p3);
		LPCD::Push(L, p4);
		LPCD::Push(L, p5);
		LPCD::Push(L, p6);
		LPCD::Push(L, p7);
		LUAFUNCTIONVOID_POSTCALL(7);
	}

protected:
	LuaPlus::LuaObject m_functionObj;
};


} // namespace LuaPlus

 

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

namespace LPCD
{
	using namespace LuaPlus;

	inline void Push(lua_State* L, const lua_WChar* value)			{  lua_pushwstring(L, value);  }
	inline void Push(lua_State* L, LuaStackObject& value)			{  (void)L; value.Push();  }
	inline void Push(lua_State* L, LuaObject& value)				{  (void)L; value.PushStack();  }
	inline void Push(lua_State* L, const LPCD::LuaUserData& value)	{  LuaState* state = LuaState::CastState(L);  state->BoxPointer((void*)value.m_value);  }
	inline void Push(lua_State* L, int (*value)(LuaState*))			{  LuaState* state = LuaState::CastState(L);  state->PushCClosure(value, 0);  }

	inline bool	Match(TypeWrapper<const lua_WChar*>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TWSTRING;  }
	inline bool	Match(TypeWrapper<LuaStackObject>, lua_State* L, int idx)
		{  (void)L, (void)idx;  return true;  }
	inline bool	Match(TypeWrapper<LuaObject>, lua_State* L, int idx)
		{  (void)L, (void)idx;  return true;  }

	inline const lua_WChar*	Get(TypeWrapper<const lua_WChar*>, lua_State* L, int idx)
		{  return static_cast<const lua_WChar*>(lua_towstring(L, idx));  }
	inline LuaStackObject	Get(TypeWrapper<LuaStackObject>, lua_State* L, int idx)
		{  return LuaStackObject(LuaState::CastState(L), idx);  }
	inline LuaObject		Get(TypeWrapper<LuaObject>, lua_State* L, int idx)
		{  return LuaObject(LuaState::CastState(L), idx);  }
} // namespace LPCD



#ifdef LUAPLUS_ENABLE_INLINES
#include "LuaState.inl"
#endif LUAPLUS_ENABLE_INLINES

#endif LUASTATE_H
