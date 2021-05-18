#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：TrapLogic.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "TrapLogic.h"
#include "Obj_Special.h"
#include "ScanOpt_SObj_ScanChar.h"
#include "ScanOpt_SObj_ScanTrap.h"
#include "Impact_Core.h"

using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		VOID TrapLogic_T::ForceActivate(Obj_Special& rThis) const
		{
			if(FALSE==rThis.IsFadeOut())
			{
				Activate(rThis);
				ForceFadeOut(rThis);
			}
		}
		VOID TrapLogic_T::ForceFadeOut(Obj_Special& rThis) const
		{
			if(TRUE==ActivateWhenFadeOut(rThis))
			{
				Activate(rThis);
			}
			rThis.MarkFadeOutFlag();
		}
		VOID TrapLogic_T::OnTick(Obj_Special& rThisObj) const
		{
			if(TRUE==TriggerCheck(rThisObj))
			{
				Activate(rThisObj);
			}
		}
		VOID TrapLogic_T::OnTimeOver(Obj_Special& rThisObj) const
		{
			if(TRUE==rThisObj.IsFadeOut())
			{
				return;
			}
			if(TRUE==ActivateWhenFadeOut(rThisObj))
			{
				Activate(rThisObj);
			}
		}
		BOOL TrapLogic_T::TriggerCheck(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			OBJLIST Targets;
			Scene* pScene = rThis.getScene();
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				return FALSE;
			}
			SCANOPT_SOBJ_SCANCHAR_INIT ScanOperatorIniter;
			ScanOperatorIniter.m_pMe = &rThis;
			ScanOperatorIniter.m_pScene = pScene;
			ScanOperatorIniter.m_pTargets = &Targets;
			ScanOperatorIniter.m_fRadius = pData->GetTriggerRadius();
			ScanOperatorIniter.m_nCount= 1;
			ScanOperatorIniter.m_CentrePoint.m_fX = rThis.getWorldPos()->m_fX;
			ScanOperatorIniter.m_CentrePoint.m_fZ = rThis.getWorldPos()->m_fZ;

			ScanOpt_SObj_ScanChar ScanOperator;
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
			if(0<Targets.m_Count)
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID TrapLogic_T::Activate(Obj_Special& rThis) const
		{
			OBJLIST Targets;
			if(rThis.IsFadeOut())
			{
				return;
			}
			Scene* pScene = rThis.getScene();
			if(NULL==pScene)
			{
				return;
			}
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				return;
			}
			if(0==rThis.GetActiveTimes())
			{
				rThis.MarkFadeOutFlag();
				return;
			}
			if(0<rThis.GetActiveTimes())
			{
				rThis.SetActiveTimes(rThis.GetActiveTimes()-1);
			}
			{// scan Characters
				SCANOPT_SOBJ_SCANCHAR_INIT ScanOperatorIniter;
				ScanOperatorIniter.m_pMe = &rThis;
				ScanOperatorIniter.m_pScene = pScene;
				ScanOperatorIniter.m_pTargets = &Targets;
				ScanOperatorIniter.m_fRadius = pData->GetEffectRadius();
				if(-1==pData->GetEffectedObjNumber()) //-1为不限数量
				{
					ScanOperatorIniter.m_nCount = Targets.MAX_OBJ_LIST_SIZE-1;
				}
				else
				{
					ScanOperatorIniter.m_nCount= pData->GetEffectedObjNumber();
				}
				ScanOperatorIniter.m_CentrePoint.m_fX = rThis.getWorldPos()->m_fX;
				ScanOperatorIniter.m_CentrePoint.m_fZ = rThis.getWorldPos()->m_fZ;

				ScanOpt_SObj_ScanChar ScanOperator;
				ScanOperator.Init(&ScanOperatorIniter);
				if(FALSE==pScene->Scan(&ScanOperator))
				{
					return;
				}
				ObjID_List TargetIdList;
				TargetIdList.m_nCount = Targets.m_Count;
				for(INT idx=0;Targets.m_Count>idx;++idx)
				{
					Obj* pTar = Targets.m_aObj[idx];
					TargetIdList.m_aIDs[idx] = pTar->GetID();
				}
				rThis.OnTrigger(TargetIdList);
				for(INT idx=0;Targets.m_Count>idx;++idx)
				{
					Obj* pTar = Targets.m_aObj[idx];
					if(NULL!=pTar)
					{
						if(TRUE==IsCharacterObj(pTar->GetObjType()))
						{
							Obj_Character& rTar = (Obj_Character&)*pTar;
							EffectOnChar(rThis, rTar);
						}
					}
				}
			} while(0);
			Targets.CleanUp();
			{// scan Traps
				SCANOPT_SOBJ_SCANTRAP_INIT ScanOperatorIniter;
				ScanOperatorIniter.m_pMe = &rThis;
				ScanOperatorIniter.m_pScene = pScene;
				ScanOperatorIniter.m_pTargets = &Targets;
				ScanOperatorIniter.m_fRadius = pData->GetEffectRadius();
				ScanOperatorIniter.m_nCount= pData->GetEffectedObjNumber();
				ScanOperatorIniter.m_CentrePoint.m_fX = rThis.getWorldPos()->m_fX;
				ScanOperatorIniter.m_CentrePoint.m_fZ = rThis.getWorldPos()->m_fZ;

				ScanOpt_SObj_ScanTrap ScanOperator;
				ScanOperator.Init(&ScanOperatorIniter);
				if(FALSE==pScene->Scan(&ScanOperator))
				{
					return;
				}
				for(INT idx=0;Targets.m_Count>idx;++idx)
				{
					Obj* pTar = Targets.m_aObj[idx];
					if(NULL!=pTar)
					{
						if(rThis.GetID()!=pTar->GetID())
						{
							if(TRUE==IsSpecialObj(pTar->GetObjType()))
							{
								Obj_Special& rTar = (Obj_Special&)*pTar;
								EffectOnTrap(rThis, rTar);
							}
						}
					}
				}
			}while(0);
		}
		BOOL TrapLogic_T::CanTriggerBySpecificObj(Obj_Special& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(FALSE==rTar.IsAliveInDeed())
			{
				return FALSE;
			}
			Obj_Character* pOwner = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rMe.GetOwnerID());
			if(NULL==pOwner)
			{
				//Should Mark this trap fadeout now
				return FALSE;
			}
			if(pOwner->IsEnemy(&rTar) || rTar.IsEnemy(pOwner))
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::CanTriggerBySpecificObj(Obj_Special& rThis, Obj_Special& rOther) const
		{
			__ENTER_FUNCTION
			if(TRAP_OBJ!=rOther.GetClass())
			{
				return FALSE;
			}
			if(TRUE==CanTriggerByTrap(rThis))
			{
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::IsScanedTargetValid(Obj_Special& rThis, Obj_Character& rScaned) const
		{
			__ENTER_FUNCTION
			return CanTriggerBySpecificObj(rThis, rScaned);
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::IsScanedTargetValid(Obj_Special& rThis, Obj_Special& rScaned) const
		{
			__ENTER_FUNCTION
			if(rThis.GetID() != rScaned.GetID())
			{
				return CanTriggerBySpecificObj(rScaned, rThis);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::EffectOnChar(Obj_Special& rThis, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			INT nImpact = INVALID_ID;
			INT nOdds = 0;
			INT nRand = 0;
			Scene* pScene = rThis.getScene();
			if(NULL==pScene)
			{
				rThis.MarkFadeOutFlag();
				return FALSE;
			}
			pScene->GetEventCore().RegisterBeSkillEvent(rTar.GetID(), rThis.GetOwnerID(), BEHAVIOR_TYPE_HOSTILITY, 500);
			nImpact = GetImpact0(rThis);
			if(INVALID_ID!=nImpact)
			{
				g_ImpactCore.SendImpactToUnit(rTar, nImpact, rThis.GetOwnerID(), 500, FALSE, rThis.GetPowerRefixByRate());
			}
			nImpact = GetImpact1(rThis);
			if(INVALID_ID!=nImpact)
			{
				g_ImpactCore.SendImpactToUnit(rTar, nImpact, rThis.GetOwnerID(), 500, FALSE, rThis.GetPowerRefixByRate());
			}
			nRand = pScene->GetRand100();
			nOdds = GetImpact2ActivateOdds(rThis);
			if(nRand<nOdds)
			{
				nImpact = GetImpact2(rThis);
				if(INVALID_ID!=nImpact)
				{
					g_ImpactCore.SendImpactToUnit(rTar, nImpact, rThis.GetOwnerID(), 500, FALSE, rThis.GetPowerRefixByRate());
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::EffectOnTrap(Obj_Special& rThis, Obj_Special& rTar) const
		{
			__ENTER_FUNCTION
			if(TRAP_OBJ!=rTar.GetClass())
			{
				return FALSE;
			}
			TrapLogic_T const* pLogic = (TrapLogic_T const*)ObjSpecial_GetLogic(rTar);
			if(NULL==pLogic)
			{
				AssertEx(FALSE,"[TrapLogic_T::EffectOnTrap]: Can't find Logic Instance.");
				return FALSE;
			}
			if(TRUE==pLogic->CanTriggerBySpecificObj(rTar, rThis))
			{
				ForceActivate(rTar);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::CanTriggerByTrap(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL!=pData)
			{
				INT nFlag = pData->GetTrapUsedFlags();
				if(0<(nFlag & MASK_TRIGGER_BY_TRAP))
				{
					return TRUE;
				}
			}
			return FALSE;			
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::ActivateWhenFadeOut(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL!=pData)
			{
				INT nFlag = pData->GetTrapUsedFlags();
				if(0<(nFlag & MASK_ACTIVATE_WHEN_FADE_OUT))
				{
					return TRUE;
				}
			}
			return FALSE;			
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL TrapLogic_T::CanTriggerByCharacter(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL!=pData)
			{
				INT nFlag = pData->GetTrapUsedFlags();
				if(0<(nFlag & MASK_TRIGGER_BY_CHARACTER))
				{
					return TRUE;
				}
			}
			return FALSE;			
			__LEAVE_FUNCTION
			return FALSE;
		}		
		INT	TrapLogic_T::GetImpact0(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			return GetDescriptorValueByIndex(rThis, DescriptorIndex_T::IDX_IMPACT0);
			__LEAVE_FUNCTION
			return INVALID_ID;
		}
		INT	TrapLogic_T::GetImpact1(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			return GetDescriptorValueByIndex(rThis, DescriptorIndex_T::IDX_IMPACT1);
			__LEAVE_FUNCTION
			return INVALID_ID;
		}
		INT	TrapLogic_T::GetImpact2(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			return GetDescriptorValueByIndex(rThis, DescriptorIndex_T::IDX_IMPACT2);
			__LEAVE_FUNCTION
			return INVALID_ID;
		}
		INT	TrapLogic_T::GetImpact2ActivateOdds(Obj_Special& rThis) const
		{
			__ENTER_FUNCTION
			return GetDescriptorValueByIndex(rThis, DescriptorIndex_T::IDX_IMPACT2_ACTIVATE_ODDS);
			__LEAVE_FUNCTION
			return INVALID_ID;
		}

	}
}
