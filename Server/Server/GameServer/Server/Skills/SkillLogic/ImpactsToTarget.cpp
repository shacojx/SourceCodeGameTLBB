#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactsToTarget.cpp
// 功能说明：基础技能-只产生(特定效果)， 不分敌我，AE时对任何人都有效
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ImpactsToTarget.h"
#include "Combat_Core.h"
#include "Impact_Core.h"
#include "StdImpact003.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		//class ImpactsToTarget_T : public SkillLogic_T
		BOOL ImpactsToTarget_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			//Send Activate once impacts
			for(INT nIdx=0; IMPACT_NUMBER>nIdx; ++nIdx)
			{
				nValue = GetActivateOnceImpactByIndex(rSkillInfo, nIdx);
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
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL ImpactsToTarget_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			OWN_IMPACT impact;
			//Send Activate once impacts
			for(INT nIdx=0; IMPACT_NUMBER>nIdx; ++nIdx)
			{
				nValue = GetActivateEachTickImpactByIndex(rSkillInfo, nIdx);
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
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		// Activate once impacts
		INT ImpactsToTarget_T::GetActivateOnceImpactByIndex(SkillInfo_T const& rSkill, INT nIndex) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_ONCE_IMPACT+2*nIndex);
			__LEAVE_FUNCTION
			return 0;
		}
		// Activate each tick impacts
		INT ImpactsToTarget_T::GetActivateEachTickImpactByIndex(SkillInfo_T const& rSkill, INT nIndex) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_ACTIVATE_EACH_TICK_IMPACT+2*nIndex);
			__LEAVE_FUNCTION
			return 0;
		}
	};
};
