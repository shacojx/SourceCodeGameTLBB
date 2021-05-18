#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：DaLiSkill014.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "DaLiSkill014.h"
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
		//class DaLiSkill014_T : public SkillLogic_T
		//Skill Refix interfaces
		VOID DaLiSkill014_T::RefixAttackPowerByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			__LEAVE_FUNCTION
		}
		BOOL DaLiSkill014_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			INT nDataIndex = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			
			Obj* pObj = rMe.Skill_CreateObjSpecial(rParams.GetTargetPosition(), nDataIndex);
			if(NULL==pObj)
			{
				AssertEx(FALSE,"[DaLiSkill014_T::EffectOnUnitOnce]: Can't create Special Obj!");
				return FALSE;
			}

			RegisterActiveObj(*pObj, rMe, 500);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
