#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_XingXiu017.cpp
// 程序员：高骐
// 功能说明：效果--星宿技能恶贯满盈的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_XingXiu017.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "GameTable.h"
#include "Scene.h"
#include "DI_Damages.h"
#include "LogicManager.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		VOID SOT_XingXiu017_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetScanRadius(rImp, 0);
			SetEffectedObjNumber(rImp, 0);
			SetSubImpactID(rImp, 0);
			SetAttackAdditional(rImp, 0);
			__LEAVE_FUNCTION
		}

		VOID SOT_XingXiu017_T::OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const
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
			CombatCore_T myCombatCore;
			// impacts init finished
			for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
			{
				pTar = (Obj_Character*)Targets.m_aObj[nIdx];
				if(NULL!=pTar)
				{
					myCombatCore.Reset();			
					INT nAdditionalAttack = GetAttackAdditional(rImp);
					myCombatCore.SetAdditionalAttackMagic(nAdditionalAttack);
					myCombatCore.GetResultImpact(rMe, *pTar, impact);

					impact.m_nSkillID = INVALID_ID;
					impact.m_nCasterID = rMe.GetID();
					impact.m_nImpactID = GetSubImpactID(rImp);
					
					Scene* pScene = rMe.getScene();
					if(NULL!=pScene)
					{
						pScene->GetEventCore().RegisterBeSkillEvent(pTar->GetID(), rMe.GetID(), BEHAVIOR_TYPE_HOSTILITY, 500);
						pScene->GetEventCore().RegisterImpactEvent(pTar->GetID(), rMe.GetID(), impact, 500);
					}
				}
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL SOT_XingXiu017_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
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
		VOID SOT_XingXiu017_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
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
		BOOL SOT_XingXiu017_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

