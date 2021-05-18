#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：CommonSkill003.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "CommonSkill003.h"
#include "Obj_Special.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		using namespace Combat_Module::Special_Obj_Module;

		//class CommonSkill003_T : public SkillLogic_T
		BOOL CommonSkill003_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			ID_t nTrapDataIndex = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			if(0<nTrapDataIndex)
			{
				Obj_Special* pTrap = (Obj_Special*)rMe.Skill_CreateObjSpecial(*(rMe.getWorldPos()), nTrapDataIndex);
				if(NULL==pTrap)
				{
					AssertEx(FALSE,"[CommonSkill003_T::EffectOnUnitOnce]: Can't create Special Obj!");
					return FALSE;
				}
				RegisterActiveObj(*pTrap, rMe, 500);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
