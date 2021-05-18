#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：GaiBangSkill014.cpp
// 功能说明：丐帮技能沛然有雨
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "Obj_Character.h"
#include "GaiBangSkill014.h"
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
		//class GaiBangSkill014_T : public SkillLogic_T
		//Skill Refix interfaces
		VOID GaiBangSkill014_T::RefixAttackPowerByRate(SkillInfo_T& rSkill, INT nRate) const
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
		BOOL GaiBangSkill014_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nDepleteStrikePointSegment = rParams.GetDepletedStrikePoints()/STRIKE_POINT_SEGMENT_SIZE;
			if(1>nDepleteStrikePointSegment)
			{
				AssertEx(FALSE,"GaiBangSkill004_T::TakeEffectNow");
				nDepleteStrikePointSegment=1;
			}
			else if(MAX_STRIKE_POINT_SEGMENT<nDepleteStrikePointSegment)
			{
				AssertEx(FALSE,"GaiBangSkill004_T::TakeEffectNow");
				nDepleteStrikePointSegment=MAX_STRIKE_POINT_SEGMENT;
			}
			OWN_IMPACT impact;
			DI_ModifyRage_T const logic;
			logic.InitStruct(impact);
			impact.m_nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			INT nDispelRage = rSkillInfo.GetDescriptorByIndex(nDepleteStrikePointSegment)->GetValue();
			logic.SetRageModification(impact, -nDispelRage);

			// register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};






