#pragma once

#include <CEGUIScriptModule.h>

class tScriptSandbox;
namespace CEGUI
{
	
class CGameUIScript : public ScriptModule
{
public:
	//执行某个脚本文件
	virtual	void	executeScriptFile(const String& filename, const String& resourceGroup = "");
	//执行某个全局脚本函数
	virtual int		executeScriptGlobal(const String& function_name);
	//响应UI事件
	virtual	bool	executeScriptedEventHandler(const String& handler_name, const EventArgs& e);
	//执行某个脚本语句
    virtual void	executeString(const String& str);
	//开始脚本系统绑定
    virtual void	createBindings(void);
	//撤销脚本绑定
    virtual void	destroyBindings(void);

public:
	CGameUIScript();
	virtual ~CGameUIScript();
};

}