#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ChannelAction.cpp
// 功能说明：引导动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "ChannelAction.h"
#include "Obj_Character.h"
#include "ActionCallBackForScript.h"

#include "ActionDelegator.h"
#include "LuaInterface.h"
#include "SkillInfo.h"
#include "TargetingAndDepletingParams.h"
#include "Scene.h"
#include "ScriptLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Script_Module;

namespace Action_Module
{
	//class ActionCallBackFunctorForScript_T : public ActionCallBackFunctor_T
	BOOL ActionCallBackFunctorForScript_T::IsPerformingSpecificScript(ActionParams_T& rParams, ScriptID_t nID) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			TargetingAndDepletingParams_T& rScriptParams = rActor.GetTargetingAndDepletingParams();
			if(nID == rScriptParams.GetActivatedScript())
			{
				return TRUE;
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForScript_T::CanDoThisActionInThisStatus(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			TargetingAndDepletingParams_T& rTargetingAndDepletingParams = rActor.GetTargetingAndDepletingParams();
			if(TRUE==rActor.Skill_CanUseThisSkillInThisStatus(rTargetingAndDepletingParams.GetActivatedSkill()))
			{
				return TRUE;
			}
			if(TRUE==rActor.CanActionFlag1() && TRUE==rActor.CanActionFlag2())
			{
				return TRUE;
			}
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForScript_T::OnInterrupt(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForScript_T::OnActivateOnce(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			return g_ScriptLogic.Action_ActivateOnceHandler(rActor);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForScript_T::OnActivateEachTick(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return FALSE;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			return g_ScriptLogic.Action_ActivateEachTickHandler(rActor);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForScript_T::OnDisturbForCharging(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		return FALSE; //直接打断
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ActionCallBackFunctorForScript_T::OnDisturbForChanneling(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		return FALSE; //直接打断
		__LEAVE_FUNCTION
		return FALSE;
	}
	VOID ActionCallBackFunctorForScript_T::ResetForNextAction(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		Obj* pObj = rParams.GetActor();
		if(NULL==pObj)
		{
			return;
		}
		if(TRUE==IsCharacterObj(pObj->GetObjType()))
		{
			Obj_Character& rActor = *(static_cast<Obj_Character*>(pObj));
			TargetingAndDepletingParams_T& rScriptParams = rActor.GetTargetingAndDepletingParams();
			rScriptParams.Reset();
		}
		__LEAVE_FUNCTION
	}
}

