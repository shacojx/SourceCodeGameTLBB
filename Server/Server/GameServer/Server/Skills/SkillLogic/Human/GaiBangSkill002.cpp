#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：GaiBangSkill002.cpp
// 功能说明：丐帮技能沛然有雨
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "Obj_Character.h"
#include "GaiBangSkill002.h"
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
		//class GaiBangSkill002_T : public SkillLogic_T
		//Skill Refix interfaces
		VOID GaiBangSkill002_T::RefixAttackPowerByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			SkillInstanceData_T::Descriptor_T& rDescriptor0 = *((SkillInstanceData_T::Descriptor_T*)rSkill.GetDescriptorByIndex(0));
			SkillInstanceData_T::Descriptor_T& rDescriptor1 = *((SkillInstanceData_T::Descriptor_T*)rSkill.GetDescriptorByIndex(1));
			SkillInstanceData_T::Descriptor_T& rDescriptor2 = *((SkillInstanceData_T::Descriptor_T*)rSkill.GetDescriptorByIndex(2));
			rDescriptor0.SetValue(rDescriptor0.GetValue()*(1+nRate/100));
			rDescriptor1.SetValue(rDescriptor1.GetValue()*(1+nRate/100));
			rDescriptor2.SetValue(rDescriptor2.GetValue()*(1+nRate/100));
			__LEAVE_FUNCTION
		}
		BOOL GaiBangSkill002_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nDepleteStrikePointSegment = rParams.GetDepletedStrikePoints()/STRIKE_POINT_SEGMENT_SIZE;
			if(1>nDepleteStrikePointSegment)
			{
				AssertEx(FALSE,"[GaiBangSkill004_T::TakeEffectNow]: Depleted strike point segment Illegal!!");
				nDepleteStrikePointSegment=1;
			}
			else if(MAX_STRIKE_POINT_SEGMENT<nDepleteStrikePointSegment)
			{
				AssertEx(FALSE,"[GaiBangSkill004_T::TakeEffectNow]: Depleted strike point segment Illegal!!");
				nDepleteStrikePointSegment=MAX_STRIKE_POINT_SEGMENT;
			}
			INT nAdditionalAttackPower = rSkillInfo.GetDescriptorByIndex(nDepleteStrikePointSegment)->GetValue();
			// calculate damage
			OWN_IMPACT impact;

			CombatCore_T myCombatCore;
			myCombatCore.Reset();			
			myCombatCore.SetAdditionalAttackPhysics(nAdditionalAttackPower);
			myCombatCore.GetResultImpact(rMe, rTar, impact);

			// register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};





