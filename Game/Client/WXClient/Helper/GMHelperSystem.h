/****************************************\
*										*
* 			程序外接帮助系统			*
*										*
\****************************************/

#pragma once
#include "TDHelperSystem.h"
#include "..\Interface\GMinterface_Lua.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CHelperSystem : public tHelperSystem, public LuaExport< CHelperSystem >
{
	//-----------------------------------------------------------
	//命令接口
	//-----------------------------------------------------------
public:
	// 跳转到指定帮助界面
	void	GotoAnchorHelp(const STRING& strAnchor);

	//-----------------------------------------------------------
	// 导出到Lua的函数
	//-----------------------------------------------------------
public:
	INT Lua_GotoHelper(LuaPlus::LuaState* state);

protected:
	//创建帮助进程
	void	CreateHelperProcess(const STRING& strAnchor);

	//-----------------------------------------------------------
	//核心继承
	//-----------------------------------------------------------
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);
	
protected:
	//帮助程序命令接受窗口名
	STRING	m_strCmdWnd;
	//帮助进程
	HANDLE	m_hHelperProcess;

public:
	CHelperSystem();
	virtual ~CHelperSystem();

protected:
	WX_DECLARE_DYNAMIC(CHelperSystem);
};
