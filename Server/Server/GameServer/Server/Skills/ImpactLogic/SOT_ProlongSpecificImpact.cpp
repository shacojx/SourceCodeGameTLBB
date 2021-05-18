#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_ProlongSpecificImpact.cpp
// 程序员：高骐
// 功能说明：效果--增加指定效果的持续时间
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_ProlongSpecificImpact.h"
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
		VOID SOT_ProlongSpecificImpact_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetImproveRate(rImp, 0);
			SetTargetImpactCollection(rImp, INVALID_ID);
			SetActiveTimes(rImp, 0);
			__LEAVE_FUNCTION
		}
		VOID SOT_ProlongSpecificImpact_T::RefixImpact(OWN_IMPACT & rImp, OWN_IMPACT & rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			if(0==GetActiveTimes(rImp)||TRUE==IsFadeOut(rImp))
			{
				return;
			}
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetTargetImpactCollection(rImp));
			if(NULL==pCollection)
			{
				AssertEx(FALSE,"[SOT_ProlongSpecificImpact_T::RefixImpact] empty collection found!!");
				SetFadeOutFlag(rImp, TRUE);
				return;
			}
			if(FALSE == pCollection->IsThisIDInCollection(rImpactNeedRefix.m_nImpactID))
			{
				return;
			}
			ImpactLogic_T const* pLogic = g_ImpactLogicList.GetLogicById(rImpactNeedRefix.m_nLogicID);
			if(NULL==pLogic)
			{
				return;
			}
			INT nActiveTimes = GetActiveTimes(rImp);
			if(0<nActiveTimes)
			{
				if(TRUE==pLogic->RefixContinuanceByRate(rImpactNeedRefix, GetImproveRate(rImp)))
				{
					--nActiveTimes;
					if(0==nActiveTimes)
					{
						SetFadeOutFlag(rImp,TRUE);
					}
					SetActiveTimes(rImp, nActiveTimes);
				}
			}
			else
			{
				if(0>nActiveTimes)
				{
					pLogic->RefixContinuanceByRate(rImpactNeedRefix, GetImproveRate(rImp));
				}
			}
			__LEAVE_FUNCTION
		}
		VOID SOT_ProlongSpecificImpact_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
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
		BOOL SOT_ProlongSpecificImpact_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

