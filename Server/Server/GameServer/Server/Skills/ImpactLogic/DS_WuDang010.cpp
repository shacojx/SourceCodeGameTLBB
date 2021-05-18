#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：DS_WuDang010.cpp
// 程序员：高骐
// 功能说明：效果--星宿技能恶贯满盈的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "DS_WuDang010.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "GameTable.h"
#include "Scene.h"
#include "LogicManager.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		using namespace Combat_Module::Skill_Module;
		VOID DS_WuDang010_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetDefenceMagicRefix(rImp, 0);
			SetSnareActivateOdds(rImp, 0);
			SetSnareImpactId(rImp, INVALID_ID);
			SetSnareContinuance(rImp, 0);
			SetSnareMoveSpeedRefix(rImp, 0);
			SetSnareCooldown(rImp, 0);
			SetScanRadius(rImp, 0);
			SetEffectedObjNumber(rImp, 0);
			SetFreezeImpactId(rImp, INVALID_ID);
			SetFreezeContinuance(rImp, 0);
			__LEAVE_FUNCTION
		}

		VOID DS_WuDang010_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			SkillTemplateData_T const* pData = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			if(NULL!=pData)
			{
				if(0==pData->GetRangedSkillFlag())
				{
					if(GetSnareCooldown(rImp)<=GetSnareCooldownElapsed(rImp))
					{
						Scene * pScene = rMe.getScene();
						if(NULL!=pScene)
						{
							INT nRand = pScene->GetRand100();
							if(nRand < GetSnareActivateOdds(rImp))
							{
								if(NULL!=pAttacker)
								{
									OWN_IMPACT impact;
									SOT_MoveSpeedRefix_T Logic;
									Logic.InitStruct(impact);
									impact.m_nSkillID = MELEE_ATTACK;
									impact.m_nCasterID = rImp.m_nCasterID;
									impact.m_nImpactID = GetSnareImpactId(rImp);
									Logic.SetContinuance(impact, GetSnareContinuance(rImp));
									impact.m_nPRI = rImp.m_nPRI;
									pScene->GetEventCore().RegisterImpactEvent(pAttacker->GetID(), impact.m_nCasterID, impact, 500);
									SetSnareCooldownElapsed(rImp, 0);
								}
							}
						}
					}
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL DS_WuDang010_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC)
			{
				rIntAttrRefix += GetDefenceMagicRefix(rImp);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID DS_WuDang010_T::MarkModifiedAttrDirty(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			rMe.MarkDefenceMagicRefixDirtyFlag();
			__LEAVE_FUNCTION
		}
		VOID DS_WuDang010_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
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
			SOT_Faint_T Logic;
			Logic.InitStruct(impact);	
			impact.m_nImpactID = GetFreezeImpactId(rImp);
			Logic.SetContinuance(impact, GetFreezeContinuance(rImp));
			impact.m_nPRI = rImp.m_nPRI;
			// impacts init finished
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
				{
					pTar = (Obj_Character*)Targets.m_aObj[nIdx];
					if(NULL!=pTar)
					{
						//send impact to Target
						pScene->GetEventCore().RegisterBeSkillEvent(pTar->GetID(), rMe.GetID(), BEHAVIOR_TYPE_HOSTILITY, 500);
						pScene->GetEventCore().RegisterImpactEvent(pTar->GetID(), rMe.GetID(), impact, 500);
					}
				}
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL DS_WuDang010_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
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
		VOID DS_WuDang010_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
		{
			__ENTER_FUNCTION
			INT nContinuance = GetContinuance(rImp);
			BOOL& rFadeOut = rImp.m_bFadeOut;
			if(FALSE == rFadeOut)
			{
				if(TRUE == IsOverTimed())
				{
					INT nCooldown = GetSnareCooldownElapsed(rImp);
					nCooldown += nDeltaTime;
					SetSnareCooldownElapsed(rImp, nCooldown);
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
		BOOL DS_WuDang010_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

