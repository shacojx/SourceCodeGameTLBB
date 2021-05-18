#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：DaLiSkill007.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "DaLiSkill007.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_Damages.h"
#include "LogicManager.h"
#include "SOT_Dali007.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class DaLiSkill007_T : public SkillLogic_T
		BOOL DaLiSkill007_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			// Init impact
			OWN_IMPACT impact;
			SOT_DaLi007_T const logic;
			logic.InitStruct(impact);

			impact.m_nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			logic.SetContinuance(impact, rSkillInfo.GetDescriptorByIndex(1)->GetValue());
			logic.SetEffectedSkillCollection(impact, rSkillInfo.GetDescriptorByIndex(2)->GetValue());
			logic.SetAttackRefixRate(impact, rSkillInfo.GetDescriptorByIndex(3)->GetValue());
			logic.SetRageConvertRate(impact, rSkillInfo.GetDescriptorByIndex(4)->GetValue());
			//register impact on target
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
