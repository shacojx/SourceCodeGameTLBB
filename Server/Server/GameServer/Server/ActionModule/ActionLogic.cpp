#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：VirtualAction.cpp
// 功能说明：动作接口类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "ActionLogic.h"
#include "ChargeAction.h"
#include "ChannelAction.h"
#include "InstantAction.h"

namespace Action_Module
{
	BOOL ActionLogic_T::IsPerformingSpecificSkill(ActionParams_T& rParams, SkillID_t nID) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			return pCallBack->IsPerformingSpecificSkill(rParams, nID);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionLogic_T::IsPerformingSpecificScript(ActionParams_T& rParams, ScriptID_t nID) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			return pCallBack->IsPerformingSpecificScript(rParams, nID);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}

	//全局唯一的动作逻辑实体
	ChargeActionLogic_T const g_ChargeActionLogic;
	ChannelActionLogic_T const g_ChannelActionLogic;
	InstantActionLogic_T const g_InstantActionLogic;
	
	ActionLogic_T const& GetGlobalChargeActionLogic(VOID)
	{
		return static_cast<ActionLogic_T const&>(g_ChargeActionLogic);
	}
	ActionLogic_T const& GetGlobalChannelActionLogic(VOID)
	{
		return static_cast<ActionLogic_T const&>(g_ChannelActionLogic);
	}	
	ActionLogic_T const& GetGlobalInstantActionLogic(VOID)
	{
		return static_cast<ActionLogic_T const&>(g_InstantActionLogic);
	}	
}

