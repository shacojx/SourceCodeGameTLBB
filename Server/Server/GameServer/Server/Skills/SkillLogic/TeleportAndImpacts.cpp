#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：TeleportAndImpacts.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "TeleportAndImpacts.h"
#include "StdImpact009.h"
#include "Scene.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class TeleportAndImpacts_T : public SkillLogic_T
		//Skill Refix interfaces		
		BOOL TeleportAndImpacts_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nValue = 0;
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return FALSE;
			}
			OWN_IMPACT impact;
			//Send Activate once impacts
			INT nImpact = INVALID_ID;
			if(INVALID_ID!=GetImpact1(rSkillInfo))
			{
				nImpact = GetImpact1(rSkillInfo);
				g_ImpactCore.SendImpactToUnit(rTar, nImpact, rMe.GetID(), rParams.GetDelayTime());
			}
			if(INVALID_ID!=GetImpact2(rSkillInfo))
			{
				nImpact = GetImpact2(rSkillInfo);
				g_ImpactCore.SendImpactToUnit(rTar, nImpact, rMe.GetID(), rParams.GetDelayTime());
			}
			if(INVALID_ID!=GetImpact3(rSkillInfo))
			{
				nImpact = GetImpact3(rSkillInfo);
				g_ImpactCore.SendImpactToUnit(rTar, nImpact, rMe.GetID(), rParams.GetDelayTime());
			}
			if(INVALID_ID!=GetImpactTeleport(rSkillInfo))
			{
				nImpact = GetImpactTeleport(rSkillInfo);
				g_ImpactCore.InitImpactFromData(nImpact, impact);
				DI_Teleport_T logic;
				logic.SetSceneID(impact, INVALID_ID);
				logic.SetPosition_X(impact, rParams.GetTargetPosition().m_fX);
				logic.SetPosition_Z(impact, rParams.GetTargetPosition().m_fZ);
				pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), rMe.GetID(), impact, rParams.GetDelayTime()+500, rSkillInfo.GetSkillID());
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		// impacts
		INT TeleportAndImpacts_T::GetImpact1(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_IMPACT_1);
			__LEAVE_FUNCTION
			return 0;
		}
		INT TeleportAndImpacts_T::GetImpact2(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_IMPACT_2);
			__LEAVE_FUNCTION
			return 0;
		}
		INT TeleportAndImpacts_T::GetImpact3(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_IMPACT_3);
			__LEAVE_FUNCTION
			return 0;
		}
		INT TeleportAndImpacts_T::GetImpactTeleport(SkillInfo_T const& rSkill) const
		{
			__ENTER_FUNCTION
			return Skill_GetSkillInfoDescriptorValueByIndex(rSkill, SkillInfoDescriptorIndex_T::IDX_IMPACT_TELEPORT);
			__LEAVE_FUNCTION
			return 0;
		}

	};
};
