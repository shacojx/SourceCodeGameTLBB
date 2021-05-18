///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2004 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://wwhiz.com/LuaPlus/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LUAOBJECT_INL
#define LUAOBJECT_INL

///////////////////////////////////////////////////////////////////////////////
// namespace LuaPlus
///////////////////////////////////////////////////////////////////////////////
namespace LuaPlus
{

/**
**/
inline bool LuaObject::operator==(const LuaObject& right) const
{
	luaplus_assert(m_state);
	return m_state->Equal(*this, right) != 0;
}

	
/**
**/
inline bool LuaObject::operator<(const LuaObject& right) const
{
	luaplus_assert(m_state);
	return m_state->LessThan(*this, right) != 0;
}

	
/**
**/
inline LuaState* LuaObject::GetState() const
{
	return m_state;
}


inline lua_State* LuaObject::GetCState() const
{
	return m_state->m_state;
}


inline LuaObject::operator lua_State*() const
{
	return m_state->m_state;
}


inline LuaObject::operator LuaState*() const
{
	return m_state;
}


inline LuaCall::LuaCall(LuaObject& functionObj) :
	m_functionObj(functionObj),
	m_numArgs(0)
{
	m_state = m_functionObj;
	m_state->GetGlobal_Stack("_ALERT");
	luaplus_assert(m_functionObj.IsFunction());
	m_startResults = m_state->GetTop() + 1;
	m_functionObj.PushStack();
}

inline LuaCall& operator<<(LuaCall& call, const LuaArgNil& /*value*/)
{
	call.m_state->PushNil();
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, float value)
{
	call.m_state->PushNumber(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, double value)
{
	call.m_state->PushNumber(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, int value)
{
	call.m_state->PushInteger(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, unsigned int value)
{
	call.m_state->PushInteger(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, const char* value)
{
	call.m_state->PushString(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, const lua_WChar* value)
{
	call.m_state->PushWString(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, lua_CFunction value)
{
	call.m_state->PushCClosure(value, 0);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, int (*value)(LuaState*))
{
	call.m_state->PushCClosure(value, 0);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, bool value)
{
	call.m_state->PushBoolean(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, void* value)
{
	call.m_state->PushLightUserData(value);
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, LuaStackObject& value)
{
	value.Push();
	call.m_numArgs++;
	return call;
}

inline LuaCall& operator<<(LuaCall& call, LuaObject& value)
{
	value.PushStack();
	call.m_numArgs++;
	return call;
}

inline LuaStackObject LuaCall::operator<<(const LuaRun& run)
{
	int alertStackPos = m_state->GetTop() - m_numArgs - 1;
	int err = m_state->PCall(m_numArgs, run.m_numResults, alertStackPos);
	if (err != 0)
	{
		LuaPlus::LuaStackObject errObj(m_state, -1);
		luaplus_assert(err == 0);
	}
	else
	{
		m_state->Remove(alertStackPos);		// Pop _ALERT.
	}
	return LuaStackObject(m_state, m_startResults - 1);
}

inline LuaCall& LuaCall::operator=(const LuaCall& src)
{
	m_state = src.m_state;
	m_functionObj = src.m_functionObj;
	m_numArgs = src.m_numArgs;
	m_startResults = src.m_startResults;

	return *this;
}


} // namespace LuaPlus

#endif LUAOBJECT_INL