#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_DaLi012.cpp
// 程序员：高骐
// 功能说明：效果--大理技能12的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_Dali012.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "Scene.h"
#include "SOT_Root.h"
#include "GameTable.h"
#include "LogicManager.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID SOT_DaLi012_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetEffectedSkillCollection(rImp, INVALID_ID);
			SetActiveRate(rImp, 0);
			SetFaintImpactID(rImp, 0);
			SetFaintImpactContinuance(rImp, 0);
			__LEAVE_FUNCTION
		}

		VOID SOT_DaLi012_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==IsFadeOut(rImp))
			{
				return;
			}
			if(NULL==pAttacker)
			{
				return;
			}
			if(INVALID_ID==nSkillID)
			{
				return;
			}
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			if(NULL==pCollection)
			{
				return;
			}
			if(FALSE==pCollection->IsThisIDInCollection(nSkillID))
			{
				return;
			}
			OWN_IMPACT impact;
			SOT_Faint_T const logic;
			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			INT nRand = pScene->GetRand100();
			if(nRand>=GetActiveRate(rImp))
			{
				return;
			}
			logic.InitStruct(impact);
			logic.SetImpactID(impact, GetFaintImpactID(rImp));
			impact.m_nSkillID = MELEE_ATTACK;
			logic.SetContinuance(impact, GetFaintImpactContinuance(rImp));
			impact.m_nPRI = rImp.m_nPRI;
			pScene->GetEventCore().RegisterImpactEvent(rMe.GetID(), impact.m_nCasterID, impact, 500);
			__LEAVE_FUNCTION
		}
		VOID SOT_DaLi012_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
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
		BOOL SOT_DaLi012_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

