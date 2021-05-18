#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_EMei016.cpp
// 程序员：高骐
// 功能说明：效果--增加指定效果的持续时间
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_Emei016.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogics.h"
#include "LogicManager.h"
#include "GameTable.h"

namespace Combat_Module
{
	
	namespace Impact_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Skill_Module;
		VOID SOT_EMei016_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetImproveRate(rImp, 0);
			SetTargetImpactMutexID(rImp, INVALID_ID);
			SetActiveTimes(rImp, 0);
			__LEAVE_FUNCTION
		}
		VOID SOT_EMei016_T::RefixImpact(OWN_IMPACT & rImp, OWN_IMPACT & rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			if(0==GetActiveTimes(rImp)||TRUE==IsFadeOut(rImp))
			{
				return;
			}
			if(GetTargetImpactMutexID(rImp) != rImpactNeedRefix.m_nMutexID)
			{
				return;
			}
			switch (rImpactNeedRefix.m_nLogicID)
			{
				case DS_EMEI009 :
					{
						DS_EMei009_T ImpactLogic;
						INT nHp = ImpactLogic.GetShieldHp(rImpactNeedRefix);
						nHp = nHp + (nHp*GetImproveRate(rImp)+50)/100;
						if(0>nHp)
						{
							nHp=0;
						}
						ImpactLogic.SetShieldHp(rImp, nHp);
					}
					break;
				case DS_EMEI014	:
					{
						DS_EMei014_T ImpactLogic;
						INT nHp = ImpactLogic.GetShieldHp(rImpactNeedRefix);
						nHp = nHp + (nHp*GetImproveRate(rImp)+50)/100;
						if(0>nHp)
						{
							nHp=0;
						}
						ImpactLogic.SetShieldHp(rImp, nHp);
					}
					break;
				default:
					AssertEx(FALSE,"[SOT_EMei016_T::RefixImpact]: rImpactNeedRefix isn't Emei huti impact!!");
					return;
					break;
			}

			INT nActiveTimes = GetActiveTimes(rImp);
			if(0<nActiveTimes)
			{
				--nActiveTimes;
				if(0==nActiveTimes)
				{
					SetFadeOutFlag(rImp,TRUE);
				}
				SetActiveTimes(rImp, nActiveTimes);
			}
			__LEAVE_FUNCTION
		}
		VOID SOT_EMei016_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
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
		BOOL SOT_EMei016_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

