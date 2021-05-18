#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_TianShan017.cpp
// 程序员：高骐
// 功能说明：效果--在一定时间内，改变效果所有者的移动速度
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_TianShan017.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "Scene.h"
#include "SOT_Stealth.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID SOT_TianShan017_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetStealthLevelRefix(rImp, 0);
			SetShowSelfWhenHitted(rImp, 0);
			__LEAVE_FUNCTION
		}
		VOID SOT_TianShan017_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			OWN_IMPACT* pTargetImp = rMe.Impact_GetFirstImpactOfSpecificLogicID(SOT_STEALTH);
			if(NULL==pTargetImp)
			{
				AssertEx(FALSE,"[TianShanSkill017_T::TakeEffectNow]:Can't find the Stealth buff. check now.");
				return;
			}
			SOT_Stealth_T const StealthLogic;
			INT nValue = StealthLogic.GetStealthLevelRefix(*pTargetImp) + GetStealthLevelRefix(rImp);
			StealthLogic.SetStealthLevelRefix(*pTargetImp, nValue);
			SetShowSelfWhenHitted(rImp, StealthLogic.GetShowSelfWhenHitted(*pTargetImp));
			StealthLogic.SetShowSelfWhenHitted(*pTargetImp, FALSE);
			__LEAVE_FUNCTION
		}
		VOID SOT_TianShan017_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			OWN_IMPACT* pTargetImp = rMe.Impact_GetFirstImpactOfSpecificLogicID(SOT_STEALTH);
			if(NULL==pTargetImp)
			{
				AssertEx(FALSE,"[TianShanSkill017_T::TakeEffectNow]:Can't find the Stealth buff. check now.");
				return;
			}
			SOT_Stealth_T const StealthLogic;
			INT nValue = StealthLogic.GetStealthLevelRefix(*pTargetImp) - GetStealthLevelRefix(rImp);
			StealthLogic.SetStealthLevelRefix(*pTargetImp, nValue);
			StealthLogic.SetShowSelfWhenHitted(*pTargetImp, GetShowSelfWhenHitted(rImp));
			__LEAVE_FUNCTION
		}
		VOID SOT_TianShan017_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			rMe.MarkStealthLevelRefixDirtyFlag();
			__LEAVE_FUNCTION
		}
		VOID SOT_TianShan017_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
		{
			__ENTER_FUNCTION
			INT nContinuance = GetContinuance(rImp);
			BOOL& rFadeOut = rImp.m_bFadeOut;
			if(FALSE == rFadeOut)
			{
				if(TRUE == IsOverTimed())
				{
					if(-1==nContinuance)
					{
						return; //-1为持续时间无限长
					}
					if(0<nContinuance)
					{
						nContinuance -= nDeltaTime;
					}
					if(0>=nContinuance)
					{
						// prepare delete this impact
						rFadeOut=TRUE;
						nContinuance = 0;
					}
					SetContinuance(rImp, nContinuance);
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL SOT_TianShan017_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

