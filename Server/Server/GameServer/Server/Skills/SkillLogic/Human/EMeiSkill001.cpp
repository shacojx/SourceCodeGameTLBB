#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：EMeiSkill001.cpp
// 功能说明：峨嵋-太阿倒持
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "EMeiSkill001.h"
#include "StdImpact029.h"
#include "StdImpact030.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class EMeiSkill001_T : public SkillLogic_T
		BOOL EMeiSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			//INT nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
			if(0<=nDataIndex)
			{
				g_ImpactCore.SendImpactToUnit(rTar, nDataIndex, rMe.GetID(), rParams.GetDelayTime(), bCriticalFlag);
			}
			INT nRefixedAbsorbRate = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 1);
			OWN_IMPACT* pTargetImp = rTar.Impact_GetFirstImpactOfSpecificLogicID(DS_EMei009_T::ID);
			if(NULL!=pTargetImp)
			{
				DS_EMei009_T logic;
				logic.SetAbsorbRate(*pTargetImp, nRefixedAbsorbRate);
				return TRUE;
			}
			pTargetImp = rTar.Impact_GetFirstImpactOfSpecificLogicID(DS_EMei014_T::ID);
			if(NULL!=pTargetImp)
			{
				DS_EMei014_T logic;
				logic.SetAbsorbRate(*pTargetImp, nRefixedAbsorbRate);
				return TRUE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};




