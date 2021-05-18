#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ScriptLogic.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ScriptLogic.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "LogicManager.h"
#include "GameTable.h"
#include "ActionDelegator.h"

namespace Script_Module
{
	using namespace Combat_Module;
	using namespace Action_Module;
	
	// class ScriptLogic_T
	BOOL ScriptLogic_T::StartCharging(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		long nMaxTime = 0;
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		Time_t nChargeTime = rSkillInfo.GetChargeTime();
		BOOL bRet = TRUE;
		if(FALSE==rParams.GetIgnoreConditionCheckFlag())
		{
			if(FALSE==Script_OnConditionCheck(rMe))
			{
				return FALSE;
			}
		}
		if(0<nChargeTime)
		{
			bRet = GetGlobalActionDelegator().RegisterChargeActionForScript(rMe, rParams.GetActivatedSkill(), nChargeTime);
		}
		else
		{
			return StartLaunching(rMe);
		}
		if(TRUE== bRet)
		{
			rParams.SetErrCode(OR_OK);
		}
		return bRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::StartChanneling(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		Time_t nChannelTime = rSkillInfo.GetChannelTime();
		Time_t nCooldown = rSkillInfo.GetCooldownTime();
		if(0<nChannelTime)
		{
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				if(FALSE==Script_OnConditionCheck(rMe))
				{
					return FALSE;
				}
				if(FALSE==Script_OnDeplete(rMe))
				{
					return FALSE;
				}
			}
			SetCooldown(rMe,(CooldownID_t)rSkillInfo.GetCooldownID(),nCooldown);
			if(TRUE==GetGlobalActionDelegator().RegisterChannelActionForScript(rMe, rParams.GetActivatedSkill(), nChannelTime, rSkillInfo.GetChargesOrInterval()))
			{
				rParams.SetErrCode(OR_OK);
				Script_OnActivateOnce(rMe);
				return TRUE;
			}
		}
		else
		{
			return StartLaunching(rMe);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::StartLaunching(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		return Action_ActivateOnceHandler(rMe);
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Item_IsSkillLikeScript(Obj_Character& rMe, ScriptID_t nScript) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Item_IsSkillLikeScript]: pScene == NULL!!!!Check it now!!");
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Item_IsSkillLikeScript]: pLuaInerface == NULL!!!!Check it now!!");
			return FALSE;
		}
		try
		{
			//Call script function "IsSkillLikeScript"
			nRet = pLuaInterface->ExeScript_DD(
						nScript, 							//script ID
						"IsSkillLikeScript", 				// function name
						pScene->SceneID(), 					// scene id
						rMe.GetID() 						// Obj ID of the skill user
						);
		}
		catch(...)
		{	
			nRet = 0;
		}
		return 0<nRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	
	BOOL ScriptLogic_T::Item_CallDefaultEvent(Obj_Character& rMe, ScriptID_t nScript, INT nBagIndex) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::CallDefaultEvent]: pScene == NULL!!!!Check it now!!");
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::CallDefaultEvent]: pLuaInerface == NULL!!!!Check it now!!");
			return FALSE;
		}
		try
		{
			//Call script function "OnDefaultEvent"
			nRet = pLuaInterface->ExeScript_DDD(
						nScript, 							//script ID
						"OnDefaultEvent", 					// function name
						pScene->SceneID(), 					// scene id
						rMe.GetID(), 						// Obj ID of the skill user
						nBagIndex							// Bag index of used item
						);
		}
		catch(...)
		{
			nRet=0;
		}
		return 0<nRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Item_CallSkillStudyEvent(Obj_Character& rMe,ScriptID_t nScript,INT NPCId, INT SkillIDandLevel) const
	{
		__ENTER_FUNCTION
			INT nRet = 0;
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::CallSkillStudyEvent]: pScene == NULL!!!!Check it now!!");
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::CallSkillStudyEvent]: pLuaInerface == NULL!!!!Check it now!!");
			return FALSE;
		}
		try
		{
			//Call script function "OnSkillStudyEvent"
			nRet = pLuaInterface->ExeScript_DDDD(
				nScript, 							//script ID
				"OnSkillStudyEvent", 				// function name
				pScene->SceneID(),					// scene id
				NPCId,								// NPCID
				rMe.GetID() ,						// Obj ID of the skill user
				SkillIDandLevel						// Skill ID of the skill user						
				);
		}
		catch(...)
		{
			nRet=0;
		}
		return 0<nRet;
		__LEAVE_FUNCTION
			return FALSE;
	}
	BOOL ScriptLogic_T::Action_ActivateOnceHandler(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		BOOL bRet = FALSE;
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		if(FALSE==rParams.GetIgnoreConditionCheckFlag())
		{
			if(FALSE==Script_OnConditionCheck(rMe))
			{
				return FALSE;
			}
			if(FALSE==Script_OnDeplete(rMe))
			{
				return FALSE;
			}
		}
		Time_t nPlayActionTime = rSkillInfo.GetPlayActionTime();
		Time_t nCooldown = rSkillInfo.GetCooldownTime();
		Time_t nDelayTime = rSkillInfo.GetDelayTime();

		if(FALSE==GetGlobalActionDelegator().RegisterInstantActionForScript(rMe, rParams.GetActivatedSkill(), nPlayActionTime))
		{
			return FALSE;
		}

		//Set Action time
		rMe.SetActionTime(nPlayActionTime);
		//技能冷却
		SetCooldown(rMe,(CooldownID_t)rSkillInfo.GetCooldownID(),nCooldown);
		rParams.SetDelayTime(rSkillInfo.GetDelayTime());
		return Script_OnActivateOnce(rMe);
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Action_ActivateEachTickHandler(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		return Script_OnActivateEachTick(rMe);
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	VOID ScriptLogic_T::SetCooldown(Obj_Character& rMe, CooldownID_t nID, INT nCooldownTime) const
	{
		__ENTER_FUNCTION
		rMe.SetCooldown((CooldownID_t)nID, nCooldownTime);
		__LEAVE_FUNCTION
	}
	BOOL ScriptLogic_T::RegisterImpactEvent(Obj_Character& rReceiver, Obj_Character& rSender, OWN_IMPACT& rImp, Time_t nDelayTime, BOOL bCriticalFlag) const
	{
		__ENTER_FUNCTION
		SkillInfo_T& rSkillInfo = rSender.GetSkillInfo();
		TargetingAndDepletingParams_T& rParams = rSender.GetTargetingAndDepletingParams();
		Scene* pScene = rReceiver.getScene();
		// Routine process
		rImp.SetSkillID(rSkillInfo.GetSkillID());
		if(TRUE==bCriticalFlag)
		{
			rImp.MarkCriticalFlag();
		}
		// register impact
		if(NULL!=pScene)
		{
			return pScene->GetEventCore().RegisterImpactEvent(rReceiver.GetID(), rSender.GetID(), rImp, nDelayTime);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::RegisterBeSkillEvent(Obj_Character& rReceiver, Obj_Character& rSender, INT nBehaviorType, Time_t nDelayTime) const
	{
		__ENTER_FUNCTION
		Scene* pScene = rReceiver.getScene();
		if(NULL!=pScene)
		{
			return pScene->GetEventCore().RegisterBeSkillEvent(rReceiver.GetID(), rSender.GetID(), nBehaviorType, nDelayTime);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Script_CancelImpacts(Obj_Character& rMe, ScriptID_t nScriptID) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		if(nScriptID == INVALID_ID)
		{
			return TRUE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE, "[ScriptLogic_T::Script_CancelImpacts]: pScene == NULL!!!!Check it now!!");
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE, "[ScriptLogic_T::Script_CancelImpacts]: pLuaInerface == NULL!!!!Check it now!!");
			return FALSE;
		}
		try
		{
			//Call script function "CancelImpacts"
			nRet = pLuaInterface->ExeScript_DD(
						nScriptID, 							// Script ID
						"CancelImpacts", 					// Function name
						pScene->SceneID(), 					// Scene id
						rMe.GetID() 						// Obj ID of the skill user
						);
		}
		catch(...)
		{
			nRet = 0;
		}
		return 0<nRet;
		__LEAVE_FUNCTION
		return FALSE;
	}

	BOOL ScriptLogic_T::Script_OnConditionCheck(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rScriptParams = rMe.GetTargetingAndDepletingParams();
		if(rScriptParams.GetActivatedScript() == INVALID_ID)
		{
			return TRUE;
		}
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rScriptParams.SetErrCode(OR_INVALID_SKILL);
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnConditionCheck]: pScene == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnConditionCheck]: pLuaInerface == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		try
		{
			//Call script function "OnConditionCheck"
			nRet = pLuaInterface->ExeScript_DD(
						rScriptParams.GetActivatedScript(), //script ID
						"OnConditionCheck", 				// function name
						pScene->SceneID(), 					// scene id
						rMe.GetID() 						// Obj ID of the skill user
						);
		}
		catch(...)
		{
			nRet = 0;
		}
		if(0<nRet)
		{
			rScriptParams.SetErrCode(OR_OK);
			return TRUE;
		}
		return 0<nRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Script_OnDeplete(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rScriptParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rScriptParams.SetErrCode(OR_INVALID_SKILL);
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnDeplete]: pScene == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnDeplete]: pLuaInerface == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		try
		{
			//Call script function "OnDeplete"
			nRet = pLuaInterface->ExeScript_DD(
						rScriptParams.GetActivatedScript(), //script ID
						"OnDeplete", 						// function name
						pScene->SceneID(), 					// scene id
						rMe.GetID() 						// Obj ID of the skill user
						);
		}
		catch(...)
		{
			nRet=0;
		}
		if(0<nRet)
		{
			rScriptParams.SetErrCode(OR_OK);
		}
		return nRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Script_OnActivateOnce(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rScriptParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rScriptParams.SetErrCode(OR_INVALID_SKILL);
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnActivateOnce]: pScene == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnActivateOnce]: pLuaInerface == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		try
		{
			//Call script function "OnActivateOnce"
			nRet = pLuaInterface->ExeScript_DD(
						rScriptParams.GetActivatedScript(), //script ID
						"OnActivateOnce", 					// function name
						pScene->SceneID(), 					// scene id
						rMe.GetID() 						// Obj ID of the skill user
						);
		}
		catch(...)
		{
			nRet = 0;
		}
		if(0<nRet)
		{
			rScriptParams.SetErrCode(OR_OK);
		}
		return 0<nRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ScriptLogic_T::Script_OnActivateEachTick(Obj_Character& rMe) const
	{
		__ENTER_FUNCTION
		INT nRet = 0;
		SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
		TargetingAndDepletingParams_T& rScriptParams = rMe.GetTargetingAndDepletingParams();
		if(Obj::OBJ_TYPE_HUMAN!=rMe.GetObjType())
		{
			rScriptParams.SetErrCode(OR_INVALID_SKILL);
			return FALSE;
		}
		Scene* pScene = rMe.getScene();
		if(NULL==pScene)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnActivateEachTick]: pScene == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		LuaInterface* pLuaInterface = pScene->GetLuaInterface();
		if(NULL==pLuaInterface)
		{
			AssertEx(FALSE,"[ScriptLogic_T::Script_OnActivateEachTick]: pLuaInerface == NULL!!!!Check it now!!");
			rScriptParams.SetErrCode(OR_ERROR);
			return FALSE;
		}
		try
		{
			//Call script function "OnActivateEachTick"
			nRet = pLuaInterface->ExeScript_DD(
						rScriptParams.GetActivatedScript(), //script ID
						"OnActivateEachTick", 				// function name
						pScene->SceneID(), 					// scene id
						rMe.GetID() 						// Obj ID of the skill user
						);
		}
		catch(...)
		{
			nRet = 0;
		}
		if(0<nRet)
		{
			rScriptParams.SetErrCode(OR_OK);
			return TRUE;
		}
		return (0<nRet);
		__LEAVE_FUNCTION
		return FALSE;
	}
	
	ScriptLogic_T g_ScriptLogic;
}
