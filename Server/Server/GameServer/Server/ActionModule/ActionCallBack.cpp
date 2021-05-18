#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ActionCallBack.cpp
// 功能说明：回调函数类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "ActionCallBack.h"
#include "ActionCallBackForScript.h"
#include "ActionCallBackForSkill.h"
namespace Action_Module
{
	//全局唯一的动作回调逻辑实体
	ActionCallBackFunctorForSkill_T const g_ActionCallBackFunctorForSkill;
	ActionCallBackFunctorForScript_T const g_ActionCallBackFunctorForScript;
	
	ActionCallBackFunctor_T const& GetGlobalScriptCallBackFunctor(VOID)
	{
		return static_cast<ActionCallBackFunctor_T const&>(g_ActionCallBackFunctorForScript);
	}
	ActionCallBackFunctor_T const& GetGlobalSkillCallBackFunctor(VOID)
	{
		return static_cast<ActionCallBackFunctor_T const&>(g_ActionCallBackFunctorForSkill);
	}
	
}
