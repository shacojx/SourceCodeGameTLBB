#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：DS_EMei014.cpp
// 程序员：高骐
// 功能说明：效果--峨嵋技能易筋锻骨的护体效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "DS_Emei014.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "GameTable.h"
#include "Scene.h"
#include "ScanOperator_AEImpact.h"
#include "DI_ModifyRage.h"
#include "LogicManager.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		VOID DS_EMei014_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetAbsorbRate(rImp, 0);
			SetShieldHp(rImp, 0);
			SetEffectSkills(rImp, INVALID_ID);
			SetDamageRefix(rImp, 0);
			SetScanRadius(rImp, 0);
			SetEffectedObjNumber(rImp, 0);
			SetSubImpactID(rImp, 0);
			SetRageModification(rImp, 0);
			__LEAVE_FUNCTION
		}
		VOID DS_EMei014_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==IsFadeOut(rImp))
			{
				return;
			}
			INT nDamage = rDamage * GetAbsorbRate(rImp)/100;
			INT nShieldHp = GetShieldHp(rImp);
			nDamage>nShieldHp?nDamage=nShieldHp:NULL;
			nShieldHp-=nDamage;
			SetShieldHp(rImp, nShieldHp);
			rDamage -=nDamage;
			if(0>=GetShieldHp(rImp))
			{
				SetFadeOutFlag(rImp, TRUE);
			}
			__LEAVE_FUNCTION
		}
		VOID DS_EMei014_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj_Character* pTar=NULL;

			OBJLIST Targets;
			if(FALSE==ScanUnitForTarget(rImp, rMe, *(rMe.getWorldPos()), (FLOAT)GetScanRadius(rImp), GetEffectedObjNumber(rImp), Targets))
			{
				return;
			}
			// init impact
			OWN_IMPACT impact;
			DI_ModifyRage_T const* pImpactLogic = (DI_ModifyRage_T const*)g_ImpactLogicList.GetLogicById(DI_MODIFY_RAGE);
			if(NULL==pImpactLogic)
			{
				AssertEx(FALSE,"[SOT_EMei014_T::OnFadeOut]:Can't find the logic for DI_MODIFY_RAGE. check now.");
				return;
			}

			pImpactLogic->InitStruct(impact);	
			impact.m_nSkillID = rImp.m_nSkillID;
			impact.m_nCasterID = rMe.GetID();
			impact.m_nImpactID = GetSubImpactID(rImp);
			INT nRage = GetRageModification(rImp);
			0<nRage?nRage=-nRage:NULL;
			pImpactLogic->SetRageModification(impact, nRage);
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				AssertEx(FALSE,"[SOT_EMei014_T::OnFadeOut]:Empty Scene pointer. check now.");
				return;
			}
			// impacts init finished
			for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
			{
				pTar = (Obj_Character*)Targets.m_aObj[nIdx];
				if(NULL==pTar)
				{
					continue;
				}
				pScene->GetEventCore().RegisterBeSkillEvent(pTar->GetID(), rMe.GetID(), BEHAVIOR_TYPE_HOSTILITY, 500);
				pScene->GetEventCore().RegisterImpactEvent(pTar->GetID(), rMe.GetID(), impact, 500);
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL DS_EMei014_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(FALSE==rTar.IsAliveInDeed() || FALSE==rTar.IsActiveObj())
			{
				return FALSE;
			}
			if(rMe.IsEnemy(&rTar) || rTar.IsEnemy(&rMe))
			{
				return TRUE;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID DS_EMei014_T::RefixImpact(OWN_IMPACT& rImp, OWN_IMPACT& rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectSkills(rImp));
			if(NULL==pCollection)
			{
				return;
			}
			if(FALSE == pCollection->IsThisIDInCollection(rImpactNeedRefix.m_nSkillID))
			{
				return;
			}
			ImpactLogic_T const* pLogic = g_ImpactLogicList.GetLogicById(rImpactNeedRefix.m_nLogicID);
			if(NULL!=pLogic)
			{
				pLogic->RefixPowerByRate(rImpactNeedRefix, 100+GetDamageRefix(rImp));
			}
			__LEAVE_FUNCTION
		}
		VOID DS_EMei014_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
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
		BOOL DS_EMei014_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

