#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：TianShanSkill010.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "TianShanSkill010.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_Damages.h"
#include "LogicManager.h"
#include "GameTable.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class TianShanSkill010_T : public SkillLogic_T
		BOOL TianShanSkill010_T::SpecificConditionCheck(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			ID_t nID = rSkillInfo.GetConDepTermByIndex(1)->GetParam0();
			//不能在分身状态
			if(FALSE==rMe.Impact_HaveImpactOfSpecificImpactID(nID))
			{
				return TRUE;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		BOOL TianShanSkill010_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OWN_IMPACT impact;
			SOT_ShadowGuard_T const logic;
			logic.InitStruct(impact);

			impact.m_nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			logic.SetContinuance(impact, rSkillInfo.GetDescriptorByIndex(1)->GetValue());
			INT nDataID = rSkillInfo.GetDescriptorByIndex(2)->GetValue();
			//Call 分身接口,目前写个意向
			Obj_Character* pGuard = rMe.CreateGuard(nDataID);
			if(NULL==pGuard)
			{
				AssertEx(FALSE,"[TianShanSkill010_T::TakeEffectNow]:Can't Create Guard. check now.");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			logic.SetSummonerId(impact, rMe.GetID());
			logic.SetShadowGuardId(impact, pGuard->GetID());
			
			DispelMutexedImpact(rTar, impact.m_nImpactID);
			//register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			//send impact to guard
			RegisterImpactEvent(*pGuard, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
