/****************************************\
*										*
*       脚本系统管理器(LUA PLUS)		*
*										*
\****************************************/

#pragma once
#include "TDScriptSystem.h"
#include "LuaPlus.h"

class CScriptEnvironment : public tScriptEnvironment
{
public:
	//-----------------------------------------------------------------
	//得到名称
	virtual LPCTSTR GetName(VOID) const	{ return m_strName.c_str(); }
	//执行文件
	virtual	INT			DoFile(LPCTSTR szScriptFile)		;
	//执行命令
	virtual VOID		DoFunction(LPCTSTR szFunctionName, LPCTSTR szParam=NULL)	;
	//执行字符串
	virtual VOID		DoString(LPCTSTR szString);
	//得到环境对象
	virtual LuaPlus::LuaObject*	GetLuaObject(VOID) { return &m_theEnvObj; }

protected:
	//分析异常的脚本处理
	void DoString_WithCPPException(LPCTSTR szString);
	void DoString_WithGenException(LPCTSTR szString);

protected:
	//环境名
	STRING				m_strName;
	//在lua中环境obj的名字
	STRING				m_strNameInLua;
	//文件名
	STRING				m_strFileName;
	//lua对象
	LuaPlus::LuaObject	m_theEnvObj;

protected:
	CScriptEnvironment(LPCTSTR szName, LPCTSTR szFileName);
	virtual ~CScriptEnvironment();

	friend class CScriptSystem;
};

class CScriptSystem : public tScriptSystem
{
public:
	//----------------------------------------------------------
	//取得Lua状态
	virtual LuaPlus::LuaState*		GetLuaState(VOID) { return GetMe()->_GetLuaState(); }
	//创建新的脚本环境
	virtual	tScriptEnvironment*		NewEnvironment(LPCTSTR szName, LPCTSTR szFileName);
	//销毁环境
	virtual VOID					DestroyEnvironment(tScriptEnvironment* pEnv);
	//有Lua异常时是否抛出
	BOOL							IsEnableLuaCrash(void) const { return m_bEnableLuaCrash; }
	//设置当前Env
	void							SetActiveEnvironment(LPCTSTR szEnvName);
	//取得当前Env
	STRING							GetActiveEnvironment(void) const { return m_strEnvName; }

	//----------------------------------------------------------
	//任务列表
	//得到任务脚本文件名
	const STRING&					GetQuestFile(int nID);

	//----------------------------------------------------------
	//取得Lua状态(静态)
	static LuaPlus::LuaState*		_GetLuaState(void);

public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

protected:
	//加载任务数据
	void _loadQuestFile(void);

protected:
	//脚本环境链表
	std::list< CScriptEnvironment* >	m_listScriptEnv;
	//有Lua异常时是否抛出
	BOOL					m_bEnableLuaCrash;
	//解析任务脚本列表
	std::map< int, STRING >	m_mapQuestFile;
	//当前激活的Env名称
	STRING					m_strEnvName;

public:
	CScriptSystem();
	virtual ~CScriptSystem();
	static CScriptSystem* GetMe(VOID) { return s_pMe; }

protected:
	static CScriptSystem* s_pMe;
	WX_DECLARE_DYNAMIC(CScriptSystem);
};