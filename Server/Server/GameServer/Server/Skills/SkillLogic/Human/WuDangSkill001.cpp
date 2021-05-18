#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：WuDangSkill001.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "WuDangSkill001.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class WuDangSkill001_T : public SkillLogic_T
		BOOL WuDangSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			GUID_t guidTarget = rParams.GetTargetGuid();
			INT nDataIndex = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			Time_t nDuration = rSkillInfo.GetDescriptorByIndex(1)->GetValue();
			if(INVALID_ID!=nDataIndex)
			{
				g_ImpactCore.SendImpactToUnit(rMe, nDataIndex, rMe.GetID(), rParams.GetDelayTime(), bCriticalFlag);
			}
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				Obj_Human& rHuman = (Obj_Human&)rMe;
				rHuman.CallOfHuman(guidTarget, nDuration);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
