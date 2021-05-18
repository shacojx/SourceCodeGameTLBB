#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：TianShanSkill013.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "TianShanSkill013.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_JustSpecialEffect.h"
#include "LogicManager.h"
#include "GameTable.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class TianShanSkill013_T : public SkillLogic_T
		BOOL TianShanSkill013_T::SpecificConditionCheck(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			ID_t nID = rSkillInfo.GetConDepTermByIndex(1)->GetParam0();
			if(TRUE==rMe.Impact_HaveImpactOfSpecificImpactID(nID))
			{
				return TRUE;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		BOOL TianShanSkill013_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			INT nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			INT nCollectionID = rSkillInfo.GetDescriptorByIndex(1)->GetValue();
			INT nCount = rSkillInfo.GetDescriptorByIndex(2)->GetValue();
			OWN_IMPACT impact;
			DI_JustSpecialEffect_T const logic;
			logic.InitStruct(impact);
			impact.m_nImpactID = nImpactID;
			//register impact event
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);

			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(nCollectionID);
			if(NULL==pCollection)
			{
				AssertEx(FALSE,"[TianShanSkill012_T::TakeEffectNow]: Can't find the specific ID Collection! Check now.");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			OWN_IMPACT* pTargetImp = NULL;
			for(INT nIdx=0;nCount>=nIdx;++nIdx)
			{
				pTargetImp=rMe.Impact_GetFirstImpactInSpecificCollection(nCollectionID);
				if(NULL == pTargetImp)
				{
					return TRUE;
				}
				// transfer impact to guard
				RegisterImpactEvent(rTar, rMe, *pTargetImp, rParams.GetDelayTime(), bCriticalFlag);				
				// remove my impact
				rMe.Impact_FadeOutImpact(*pTargetImp);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
