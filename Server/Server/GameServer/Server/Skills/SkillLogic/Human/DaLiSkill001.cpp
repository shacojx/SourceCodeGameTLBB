#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：DaLiSkill001.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "DaLiSkill001.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class DaLiSkill001_T : public SkillLogic_T
		//Skill Refix interfaces
		BOOL DaLiSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
			
			Obj* pObj = rMe.Skill_CreateObjSpecial(rParams.GetTargetPosition(), nDataIndex);
			if(NULL==pObj)
			{
				AssertEx(FALSE,"[DaLiSkill001_T::EffectOnUnitOnce]: Can't create Special Obj!");
				return FALSE;
			}

			RegisterActiveObj(*pObj, rMe, 500);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
