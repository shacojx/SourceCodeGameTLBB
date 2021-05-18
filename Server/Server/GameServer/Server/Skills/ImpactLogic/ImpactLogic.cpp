#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactLogic.cpp
// 功能说明：效果的基础类，实现了所有的公共接口和公共的固定逻辑接口
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "ImpactLogic.h"
#include "Scene.h"
#include "GameStruct_Impact.h"
#include "LogicManager.h"
#include "ScanOperator_AEImpact.h"
#include "ActionDelegator.h"

using namespace Action_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Impact_Module
	{
		//class ImpactLogic_T
		//event handler
		VOID ImpactLogic_T::HeartBeat(OWN_IMPACT& rImp, Obj_Character& rMe)const
		{
			__ENTER_FUNCTION
			BOOL bContinue = TRUE;
			if(TRUE==Impact_NeedChannelSupport(rImp))
			{
				if(FALSE == IsSupportStillOn(rImp, rMe))
				{
					bContinue = FALSE;
				}
			}
			if(FALSE == rMe.IsAliveInDeed())
			{
				if(FALSE==Impact_IsRemainOnCorpse(rImp))
				{
					bContinue = FALSE;
				}
			}
			if(FALSE == SpecialHeartBeatCheck(rImp, rMe))
			{
				bContinue = FALSE;
			}
			if(TRUE == bContinue)
			{
				INT nDeltaTime = rMe.GetLogicTime();
				if(TRUE==IsOverTimed())
				{
					if(TRUE==IsIntervaled())
					{
						IntervalCalc(rImp,rMe, nDeltaTime);
					}
					ContinuanceCalc(rImp, rMe, nDeltaTime);
				}
			}
			else
			{
				rMe.Impact_OnImpactFadeOut(rImp);
			}
			__LEAVE_FUNCTION
		}
		//public function
		BOOL ImpactLogic_T::ScanUnitForTarget(OWN_IMPACT& rImp, Obj_Character& rMe, WORLD_POS const& rPos, FLOAT fRadius, INT nListSize, OBJLIST& rTargets)const
		{
			Scene* pScene = rMe.getScene();
			SCANOPERATOR_AEIMPACT_INIT ScanOperatorIniter;
			ScanOperatorIniter.m_pLogic = this;
			ScanOperatorIniter.m_pMe = &rMe;
			ScanOperatorIniter.m_pScene = pScene;
			ScanOperatorIniter.m_pTargets = &rTargets;
			ScanOperatorIniter.m_fRadius = fRadius;

			if(-1==nListSize) //-1为不限数量
			{
				ScanOperatorIniter.m_nCount = rTargets.MAX_OBJ_LIST_SIZE-1;
			}
			else
			{
				ScanOperatorIniter.m_nCount = nListSize;
			}
			ScanOperatorIniter.m_Impact = rImp;
			ScanOperatorIniter.m_CentrePoint.m_fX = rPos.m_fX;
			ScanOperatorIniter.m_CentrePoint.m_fZ = rPos.m_fZ;
			ScanOperator_AEImpact ScanOperator;
			ScanOperator.Init(&ScanOperatorIniter);
			if(NULL!=pScene)
			{
				if(FALSE==pScene->Scan(&ScanOperator))
				{
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
			return TRUE;
		}
		BOOL ImpactLogic_T::IsSupportStillOn(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj* pObj = Impact_GetCaster(rMe, rImp);
			if(NULL==pObj)
			{
				return FALSE;
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				return FALSE;
			}
			Obj_Character& rCaster = *(Obj_Character*)pObj;
			if(FALSE == GetGlobalActionDelegator().IsChanneling(rCaster))
			{
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID ImpactLogic_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const
		{
			__ENTER_FUNCTION
			INT const nContinuance = rImp.GetContinuance();
			INT nContinuanceElapsed = rImp.GetContinuanceElapsed();
			if(FALSE == rImp.IsFadeOut())
			{
				if(TRUE == IsOverTimed())
				{
					if(-1==nContinuance)
					{
						return; //-1为持续时间无限长
					}
					if(nContinuanceElapsed<=nContinuance)
					{
						nContinuanceElapsed += nDeltaTime;
					}
					if(nContinuanceElapsed>nContinuance)
					{
						// prepare delete this impact
						rMe.Impact_OnImpactFadeOut(rImp);
					}
					rImp.SetContinuanceElapsed(nContinuanceElapsed);
				}
			}
			__LEAVE_FUNCTION
		}
		VOID ImpactLogic_T::IntervalCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
		{
			__ENTER_FUNCTION
			//process interval
			INT nElapsedInterval = rImp.GetIntervalElapsed();
			INT nContinuance = rImp.GetContinuance();
			if(nDeltaTime>nContinuance && 0<=nContinuance)
			{
				nDeltaTime = nContinuance;
			}
			nElapsedInterval += nDeltaTime;
			INT const nInterval = Impact_GetInterval(rImp);
			while (nElapsedInterval >= nInterval)
			{
				// Impact activited
				OnIntervalOver(rImp, rMe);
				nElapsedInterval -= nInterval;
			}
			rImp.SetIntervalElapsed(nElapsedInterval);
			__LEAVE_FUNCTION
		}
		BOOL ImpactLogic_T::RefixContinuanceByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			if(0<rImp.GetContinuance() && TRUE == Impact_IsOverTimed(rImp))
			{
				rImp.SetContinuance(Float2Int((rImp.GetContinuance()*nRate)/100.0f));
			}
			__LEAVE_FUNCTION
			return TRUE;
		}
		ImpactLogic_T const* Impact_GetLogic(OWN_IMPACT const& rImp)
		{
		__ENTER_FUNCTION
			ID_t nLogicID = Impact_GetLogicID(rImp);

			return g_ImpactLogicList.GetLogicById(nLogicID);
		__LEAVE_FUNCTION
			return NULL;
		}
	};
};
