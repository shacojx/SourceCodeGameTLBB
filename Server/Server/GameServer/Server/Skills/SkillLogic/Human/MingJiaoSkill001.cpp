#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：MingJiaoSkill001.cpp
// 功能说明：明教技能迁怒于人
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////


#include "MingJiaoSkill001.h"
#include "StdImpact004.h"
#include "Impact_Core.h"
///
namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class MingJiaoSkill001_T : public SkillLogic_T
		BOOL MingJiaoSkill001_T::IsSpecificTargetWanted(Obj_Character& rMe, Obj_Character& rTar, SkillInfo_T const& rSkillInfo) const
		{
			__ENTER_FUNCTION
			if(rMe.GetID() != rTar.GetID())
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL MingJiaoSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
			// init impact
			OWN_IMPACT impact;
			g_ImpactCore.InitImpactFromData(nDataIndex, impact);
			ModifyHpMpRageStrikePointByValue_T const logic;

			INT nRage = rParams.GetDepletedRage();
			if(0<rParams.GetTargetCount())
			{
				nRage = nRage/rParams.GetTargetCount();
			}
			logic.SetRageModification(impact, nRage);

			// Register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
