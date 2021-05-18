#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactsToTargetAndSelf.cpp
// 功能说明：基础技能-只产生(特定效果)， 1对1，不是AE
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ImpactsToTargetAndSelf.h"
#include "Impact_Core.h"
#include "Combat_Core.h"
#include "StdImpact003.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class ImpactsToTargetAndSelf_T : public SkillLogic_T
		BOOL ImpactsToTargetAndSelf_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			nValue = GetActivateOnceImpactForSelf(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact);
				RegisterImpactEvent(rMe, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			nValue = GetActivateOnceImpactForTarget(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact);
				if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact))
				{
					DI_DamagesByValue_T logic;
					logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					myCombatCore.GetResultImpact(rMe, rTar, impact);
				}
				RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL ImpactsToTargetAndSelf_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			nValue = GetActivateEachTickImpactForSelf(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact);
				RegisterImpactEvent(rMe, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			nValue = GetActivateEachTickImpactForTarget(rSkillInfo);
			if(INVALID_ID != nValue)
			{
				g_ImpactCore.InitImpactFromData(nValue, impact);
				if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact))
				{
					DI_DamagesByValue_T logic;
					logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					myCombatCore.GetResultImpact(rMe, rTar, impact);
				}
				RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		// Activate once impacts
		INT ImpactsToTargetAndSelf_T::GetActivateOnceImpactForSelf(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_ONCE_IMPACT_FOR_SELF);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate each tick impacts
		INT ImpactsToTargetAndSelf_T::GetActivateEachTickImpactForSelf(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_SELF);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate once impacts
		INT ImpactsToTargetAndSelf_T::GetActivateOnceImpactForTarget(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_ONCE_IMPACT_FOR_TARGET);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate each tick impacts
		INT ImpactsToTargetAndSelf_T::GetActivateEachTickImpactForTarget(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_TARGET);
			__LEAVE_FUNCTION
			return 0;
		}
	};
};
