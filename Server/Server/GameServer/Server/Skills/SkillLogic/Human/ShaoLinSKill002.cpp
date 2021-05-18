#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ShaoLinSkill002.cpp
// 功能说明：少林技能般若掌
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "ShaoLinSKill002.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_Damages.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class ShaoLinSkill002_T : public SkillLogic_T
		//Skill Refix interfaces
		VOID ShaoLinSkill002_T::RefixAttackPowerByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			SkillInstanceData_T::Descriptor_T& rDescriptor = *((SkillInstanceData_T::Descriptor_T*)rSkill.GetDescriptorByIndex(0));
			rDescriptor.SetValue(rDescriptor.GetValue()*(1+nRate/100));
			__LEAVE_FUNCTION
		}
		
		BOOL ShaoLinSkill002_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OWN_IMPACT impact;

			CombatCore_T myCombatCore;
			myCombatCore.Reset();			
			myCombatCore.GetResultImpact(rMe, rTar, impact);

			// Register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};




