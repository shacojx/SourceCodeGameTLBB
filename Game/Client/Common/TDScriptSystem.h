
#pragma once
#include "TDNode.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class tScriptEnvironment
{
public:
	//-----------------------------------------------------------------
	//供外部调用
	
	//得到名称
	virtual LPCTSTR GetName(VOID) const					= 0;
	//执行文件
	virtual	INT		DoFile(LPCTSTR szScriptFile)		= 0;
	//执行函数命令
	virtual VOID	DoFunction(LPCTSTR szFunctionName, LPCTSTR szParam=NULL)	= 0;
	//执行字符串
	virtual VOID	DoString(LPCTSTR szString)	= 0;
	//得到环境对象
	virtual LuaPlus::LuaObject*	GetLuaObject(VOID)		= 0;
};

class tScriptSystem : public tNode
{
public:
	//取得Lua状态
	virtual LuaPlus::LuaState*		GetLuaState(VOID)	= 0;
	//创建新的脚本环境
	virtual	tScriptEnvironment*		NewEnvironment(LPCTSTR szName, LPCTSTR szFileName)	= 0;
	//销毁环境
	virtual VOID					DestroyEnvironment(tScriptEnvironment* pEnv)= 0;

protected:
	WX_DECLARE_DYNAMIC( tScriptSystem );
};

