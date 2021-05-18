#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ShaoLinSkill007.cpp
// 功能说明：少林技能铁布衫
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file ShaoLinSkill007.cpp
*
*/

#include "Type.h"
#include "Obj_Character.h"
#include "ShaoLinSKill007.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "SOT_ShaoLin007.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class ShaoLinSkill007_T : public SkillLogic_T
		BOOL ShaoLinSkill007_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OWN_IMPACT impact;
			SOT_ShaoLin007_T const logic;
			logic.InitStruct(impact);

			logic.SetContinuance(impact, -1);
			logic.SetRageRegenerateRate(impact, rSkillInfo.GetDescriptorByIndex(1)->GetValue());
			logic.SetCriticalRate(impact, rSkillInfo.GetDescriptorByIndex(0)->GetValue());
			impact.m_nImpactID= rSkillInfo.GetDescriptorByIndex(2)->GetValue();
			// Cancel Mutexed Buff
			DispelMutexedImpact(rTar, impact.m_nImpactID);
			//register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};





