#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：SkillLogic.cpp
// 功能说明：技能的基础类，定义了所有技能可能需要提供的接口和一些常用的固定逻辑
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SkillLogic.h"
#include "Obj_Human.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "Obj_Pet.h"
#include "Scene.h"
#include "LogicManager.h"
#include "ScanOperator_AESkill.h"
//#include "SOT_XiaoYaoTraps.h"
#include "SpecialObjLogic.h"
#include "ActionDelegator.h"
#include "GCTargetListAndHitFlags.h"
#include "GameTable.h"
#include "ConditionAndDeplete_Core.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Packets;
		using namespace Impact_Module;
		using namespace Combat_Module::Special_Obj_Module;
		using namespace Action_Module;
		
		// class SkillLogic_T
		// interface overwrite by sub class
		BOOL SkillLogic_T::SpecificOperationOnSkillStart(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			return TRUE; //缺省的是没有其他的特殊条件和操作
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL SkillLogic_T::IsPassive(VOID) const
		{
			__ENTER_FUNCTION
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL SkillLogic_T::OnInterrupt(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		BOOL SkillLogic_T::OnCancel(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetErrCode(OR_INVALID_SKILL);
			rParams.SetErrParam(rParams.GetActivatedSkill());
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		};
		
		BOOL SkillLogic_T::DepleteProcess(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE == CommonDeplete(rMe))
			{
				return FALSE;
			}
			if(FALSE == SpecificDeplete(rMe))
			{
				return FALSE;
			}
			return TRUE;//无特殊条件需要满足
			__LEAVE_FUNCTION		
			return FALSE;
		};
		BOOL SkillLogic_T::IsConditionSatisfied(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE == CommonConditionCheck(rMe))
			{
				return FALSE;
			}
			if(FALSE == SpecificConditionCheck(rMe))
			{
				return FALSE;
			}
			return TRUE;//无特殊条件需要满足
			__LEAVE_FUNCTION		
			return FALSE;
		};
		// interface common
		BOOL SkillLogic_T::StartCharging(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			long nMaxTime = 0;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			nMaxTime = rSkillInfo.GetChargeTime();
			BOOL bRet = TRUE;
			if(0>=nMaxTime)
			{
				bRet = Action_ActivateOnceHandler(rMe);
			}
			else
			{
				bRet = GetGlobalActionDelegator().RegisterChargeActionForSkill(rMe, rSkillInfo.GetSkillID(), nMaxTime);
			}
			if(TRUE== bRet)
			{
				rParams.SetErrCode(OR_OK);
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::StartChanneling(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			long nMaxTime = rSkillInfo.GetChannelTime();
			if(0 >= nMaxTime)
			{
				AssertEx(FALSE,"[SkillLogic_T::StartChanneling]: Zero channel time found!!");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			//技能消耗
			BOOL bRet = TRUE;
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				bRet=DepleteProcess(rMe);
			}
			if(TRUE == bRet)
			{
				//技能使用成功
				rMe.OnUseSkillSuccessfully(rSkillInfo);
				if(TRUE==GetGlobalActionDelegator().RegisterChannelActionForSkill(rMe, rSkillInfo.GetSkillID(), nMaxTime, rSkillInfo.GetChargesOrInterval()))
				{
					//冷却时间
					CooldownProcess(rMe);
					rParams.SetErrCode(OR_OK);
					ActivateOnce(rMe);
					return TRUE;
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::StartLaunching(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			return Action_ActivateOnceHandler(rMe);
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::Action_ActivateOnceHandler(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			//重新校验技能使用条件
			if(FALSE == IsConditionSatisfied(rMe))
			{
				return FALSE;
			}
			//技能消耗
			BOOL bRet = TRUE;
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				bRet=DepleteProcess(rMe);
			}
			if(TRUE == bRet)
			{
				//技能使用成功
				rMe.OnUseSkillSuccessfully(rSkillInfo);
				//动作演示时间
				Time_t nPlayActionTime = CalculateActionTime(rMe);
				if(FALSE==GetGlobalActionDelegator().RegisterInstantActionForSkill(rMe, rSkillInfo.GetSkillID(), nPlayActionTime))
				{
					return FALSE;
				}
				rMe.SetActionTime(nPlayActionTime);
				//冷却处理
				CooldownProcess(rMe);
				INT nActivateTimes = rSkillInfo.GetChargesOrInterval();
				if(0>=nActivateTimes)
				{
					nActivateTimes = 1;
				}
				for(INT nIdx=0; nActivateTimes>nIdx; ++nIdx)
				{
					rParams.SetDelayTime(rParams.GetDelayTime() + rSkillInfo.GetDelayTime());
					ActivateOnce(rMe);
				}
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::Action_ActivateEachTickHandler(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(TRUE==TargetCheckForEachTick(rMe))
			{
				rParams.SetDelayTime(rSkillInfo.GetDelayTime());
				ActivateEachTick(rMe);
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		//internal used
		Time_t SkillLogic_T::CalculateActionTime(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Time_t nPlayActionTime = rSkillInfo.GetPlayActionTime();
			if(MELEE_ATTACK==rParams.GetActivatedSkill())
			{
				if(Obj::OBJ_TYPE_MONSTER == rMe.GetObjType())
				{
					Obj_Monster& rMonster = static_cast<Obj_Monster&>(rMe);
					nPlayActionTime = rMonster.GetAttackAnimTime();
				}
				else if(Obj::OBJ_TYPE_PET == rMe.GetObjType())
				{
					Obj_Pet& rPet = static_cast<Obj_Pet&>(rMe);
				}
			}
			return nPlayActionTime;
			__LEAVE_FUNCTION
			return 1000; //1秒，安全值
		}

		VOID SkillLogic_T::CooldownProcess(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Time_t nPlayActionTime = rSkillInfo.GetPlayActionTime();
			Time_t nCooldown = rSkillInfo.GetCooldownTime();
			Time_t nDelayTime = rSkillInfo.GetDelayTime();
			if(MELEE_ATTACK==rParams.GetActivatedSkill())
			{
				if(Obj::OBJ_TYPE_MONSTER == rMe.GetObjType())
				{
					Obj_Monster& rMonster = static_cast<Obj_Monster&>(rMe);
					nCooldown = rMonster.GetAttackCooldownTime();
					nPlayActionTime = rMonster.GetAttackAnimTime();
				}
				else if(Obj::OBJ_TYPE_PET == rMe.GetObjType())
				{
					Obj_Pet& rPet = static_cast<Obj_Pet&>(rMe);
				}
			}
			if(nCooldown<nPlayActionTime) //冷却时间不能小于动作时间
			{
				nCooldown = nPlayActionTime;
			}
			//现在不用修正了，但是请保留这段代码已防止以后更改。
			/*
			if(TRUE==rSkillInfo.UseNormalAttackRate())
			{
				nCoolDown = RefixCooldownTimeWithAttackRate(nCoolDown,rMe.GetAttackSpeed()); 
			}
			*/
			//技能冷却
			if(TRUE==rSkillInfo.IsAutoShotSkill())
			{
				SetAutoRepeatCooldown(rMe, nCooldown);
			}
			else
			{
				SetCooldown(rMe,(CooldownID_t)rSkillInfo.GetCooldownID(),nCooldown);
			}
			__LEAVE_FUNCTION
		}
		BOOL SkillLogic_T::CommonConditionCheck(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			for(INT nIdx=0; SkillInstanceData_T::CONDITION_AND_DEPLETE_TERM_NUMBER> nIdx; ++nIdx)
			{
				ConDepTerm_T const* pConDepTerm = Skill_GetConDepTermByIndex(rSkillInfo, nIdx);
				if(NULL!=pConDepTerm)
				{
					if(FALSE==g_ConditionAndDepleteCore.ConditionCheck(rMe, *pConDepTerm))
					{
						return FALSE;
					}
				}
			}
			if(FALSE == TargetCheckForActivateOnce(rMe))
			{
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::CommonDeplete(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			for(INT nIdx=0; SkillInstanceData_T::CONDITION_AND_DEPLETE_TERM_NUMBER> nIdx; ++nIdx)
			{
				ConDepTerm_T const* pConDepTerm = Skill_GetConDepTermByIndex(rSkillInfo, nIdx);
				if(NULL!=pConDepTerm)
				{
					if(FALSE==g_ConditionAndDepleteCore.Deplete(rMe, *pConDepTerm))
					{
						return FALSE;
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL SkillLogic_T::TargetCheckForEachTick(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(1!=rSkillInfo.GetSelectType()) //技能必须点选目标否
			{
				return TRUE;
			}
			Obj* pObj = GetTargetObj(rMe);
			if(NULL!=pObj)
			{
				if(IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rTar = *(Obj_Character*)pObj;
					if(FALSE == rTar.IsCanViewMe(&rMe))
					{
						rParams.SetErrCode(OR_INVALID_TARGET);
						rParams.SetErrParam(0);
						return FALSE;
					}
					switch (rSkillInfo.GetTargetMustInSpecialState())
					{
						case 0: //目标必须是活的
							if(FALSE == rTar.IsAlive())
							{
								rParams.SetErrCode(OR_TARGET_DIE);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						case 1://目标必须是尸体
							if(TRUE == rTar.IsAliveInDeed())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						default:
							break;
					};
					if(TRUE==IsOutOfRange(rMe, rTar))
					{
						rParams.SetErrCode(OR_OUT_RANGE);
						rParams.SetErrParam(0);
						return FALSE;
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::TargetCheckForActivateOnce(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(1!=rSkillInfo.GetSelectType()) //技能必须点选目标否
			{
				if(2==rSkillInfo.GetSelectType()) //点选坐标是否出界
				{
					WORLD_POS TargetPosition = rParams.GetTargetPosition();
					if(TRUE == IsOutOfRange(rMe, TargetPosition))
					{
						return FALSE;
					}
				}
				return TRUE;
			}
			Obj* pObj = GetTargetObj(rMe);
			if(NULL!=pObj)
			{
				if(IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rTar = *(Obj_Character*)pObj;
					if(FALSE == rTar.IsCanViewMe(&rMe))
					{
						rParams.SetErrCode(OR_INVALID_TARGET);
						rParams.SetErrParam(0);
						return FALSE;
					}
					switch (rSkillInfo.GetTargetCheckByObjType())
					{
						case 0: //目标必须是玩家
							if(Obj::OBJ_TYPE_HUMAN!=rTar.GetObjType())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//条件不符
							};
							break;
						case 1://目标必须是宠物
							if(Obj::OBJ_TYPE_PET!=rTar.GetObjType())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//条件不符
							}
							break;
						case 2://目标必须是怪物
							if(Obj::OBJ_TYPE_MONSTER!=rTar.GetObjType())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//条件不符
							}
							break;
						case -1:
						default:
							break;
					};
					switch (rSkillInfo.GetTargetLogicByStand())
					{
						case 0: //目标和使用者应该是友好关系
							if(rMe.IsEnemy(&rTar) && rTar.IsEnemy(&rMe))
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//条件不符
							};
							break;
						case 1://目标和使用者应该是敌对关系
							if(rMe.IsFriend(&rTar) && rTar.IsFriend(&rMe))
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;//条件不符
							}
							break;
						case -1:
						default:
							break;
					};
					switch (rSkillInfo.GetTargetMustInSpecialState())
					{
						case 0: //目标必须是活的
							if(FALSE == rTar.IsAlive())
							{
								rParams.SetErrCode(OR_TARGET_DIE);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						case 1://目标必须是尸体
							if(TRUE == rTar.IsAliveInDeed())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);
								rParams.SetErrParam(0);
								return FALSE;
							}
							break;
						default:
							break;
					};
					if(TRUE==rSkillInfo.IsPartyOnly())
					{
						if(FALSE == rMe.IsPartner(&rTar)|| FALSE == rTar.IsPartner(&rMe))
						{
							rParams.SetErrCode(OR_INVALID_TARGET);
							rParams.SetErrParam(0);
							return FALSE;
						}
					}
					if(TRUE==IsOutOfRange(rMe, rTar))
					{
						rParams.SetErrCode(OR_OUT_RANGE);
						rParams.SetErrParam(0);
						return FALSE;
					}
					if(-1!=rSkillInfo.GetTargetLevel())
					{
						if(TRUE == rMe.IsFriend(&rTar))
						{
							if(rSkillInfo.GetTargetLevel()>rTar.GetLevel())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);							
								return FALSE;
							}
						}
						if(TRUE == rMe.IsEnemy(&rTar))
						{
							if(rSkillInfo.GetTargetLevel()<rTar.GetLevel())
							{
								rParams.SetErrCode(OR_INVALID_TARGET);							
								return FALSE;
							}
						}
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		//method for interval use
		BOOL SkillLogic_T::IsOutOfRange(Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if (rMe.GetObjType() == Obj::OBJ_TYPE_PET)
				return FALSE;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			FLOAT fAcceptableDistanceError = 0.0f;
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{	
				fAcceptableDistanceError = AcceptableDistanceError_NS::ADE_FOR_HUMAN/4.0;
			}
			else
			{
				fAcceptableDistanceError = AcceptableDistanceError_NS::ADE_FOR_NPC/4.0;
			}

			if(CHARACTER_LOGIC_MOVE == rTar.GetCharacterLogic())
			{
				fAcceptableDistanceError += 0.5; //目标移动中，误差再放大0.5米
			}
			FLOAT fRangeMinSq = rSkillInfo.GetOptimalRangeMin();
			FLOAT fRangeMaxSq = rSkillInfo.GetOptimalRangeMax();
			fRangeMinSq -= fAcceptableDistanceError;
			fRangeMaxSq += fAcceptableDistanceError;
			fRangeMinSq = fRangeMinSq>0?fRangeMinSq*fRangeMinSq:0;
			fRangeMaxSq = fRangeMaxSq>0?fRangeMaxSq*fRangeMaxSq:0;
			WORLD_POS const& posStart = *(rMe.getWorldPos());
			WORLD_POS const& posEnd = *(rTar.getWorldPos());
			FLOAT fDistToTargetSq	= DistanceSq(posStart, posEnd);
			
			if(fDistToTargetSq < fRangeMinSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMinSq));
				return TRUE;
			}
			if(fDistToTargetSq > fRangeMaxSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMaxSq));
				return TRUE;
			}
			rParams.SetErrCode(OR_OK);
			return FALSE;
			__LEAVE_FUNCTION
			return TRUE;
		}
		BOOL SkillLogic_T::IsOutOfRange(Obj_Character& rMe, WORLD_POS const& rPosTarget) const
		{
			__ENTER_FUNCTION
			if (rMe.GetObjType() == Obj::OBJ_TYPE_PET)
			{// 如果是宠物则没有距离的限制
                return FALSE;
			}
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			FLOAT nAcceptableDistanceError = 0.0f;
			FLOAT fRangeMinSq = rSkillInfo.GetOptimalRangeMin() - nAcceptableDistanceError;
			FLOAT fRangeMaxSq = rSkillInfo.GetOptimalRangeMax() + nAcceptableDistanceError;
			fRangeMinSq = fRangeMinSq>0?fRangeMinSq*fRangeMinSq:0;
			fRangeMaxSq = fRangeMaxSq>0?fRangeMaxSq*fRangeMaxSq:0;
			const WORLD_POS& posStart = *(rMe.getWorldPos());
			FLOAT fDistToTargetSq	= DistanceSq(posStart, rPosTarget);
			
			if(fDistToTargetSq < fRangeMinSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMinSq));
				return TRUE;
			}
			if(fDistToTargetSq > fRangeMaxSq)
			{
				rParams.SetErrCode(OR_OUT_RANGE);
				rParams.SetErrParam((INT)(fDistToTargetSq - fRangeMaxSq));
				return TRUE;
			}
			rParams.SetErrCode(OR_OK);
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		FLOAT SkillLogic_T::DistanceSq(WORLD_POS const& posStart, WORLD_POS const& posEnd) const
		{
			__ENTER_FUNCTION
			FLOAT fDistX, fDistZ;
			fDistX	= posEnd.m_fX - posStart.m_fX;
			fDistZ	= posEnd.m_fZ - posStart.m_fZ;
			return fDistX*fDistX+fDistZ*fDistZ; 
			__LEAVE_FUNCTION
			return 0;
		}
		VOID SkillLogic_T::SetCooldown(Obj_Character& rMe, CooldownID_t nID, INT nCooldownTime) const
		{
			__ENTER_FUNCTION
			rMe.SetCooldown((CooldownID_t)nID, nCooldownTime);
			__LEAVE_FUNCTION
		}
		VOID SkillLogic_T::SetAutoRepeatCooldown(Obj_Character& rMe, INT nCooldownTime) const
		{
			__ENTER_FUNCTION
			nCooldownTime += rMe.GetAutoRepeatCooldown();
			rMe.SetAutoRepeatCooldown(nCooldownTime);
			__LEAVE_FUNCTION
		}
		Obj* SkillLogic_T::GetTargetObj(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
		
			//Check Target require, return err when failure
			Obj_Character* pTar = (Obj_Character*)rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
			if(NULL==pTar)
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return NULL;
			}
			if(NULL==pTar || FALSE == pTar->IsActiveObj())
			{
				rParams.SetErrCode(OR_INVALID_TARGET);
				return NULL;
			}
			return (Obj*)pTar;
			__LEAVE_FUNCTION
			return NULL;
		}
		BOOL SkillLogic_T::IsTargetAlive(Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(rTar.IsAlive())
				return TRUE;
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;		
		}
		BOOL SkillLogic_T::IsHit(Obj_Character& rMe, Obj_Character& rTar, INT nAccuracy) const
		{
			__ENTER_FUNCTION
			CombatCore_T myCombatCore;
			if(-1 == nAccuracy)
			{
				nAccuracy = myCombatCore.CalculateHitRate(rMe.GetHit(), rTar.GetMiss());
			}
			INT nRand = 0;
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				nRand = pScene->GetRand100();
			}			
			return myCombatCore.IsHit(nAccuracy, nRand);
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::IsCriticalHit(Obj_Character& rMe, INT nCriticalRate) const
		{
			__ENTER_FUNCTION
			CombatCore_T myCombatCore;
			if(-1==nCriticalRate)
			{
				nCriticalRate = rMe.GetCritical();
			}
			INT nRand = 0;
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				nRand = pScene->GetRand100();
			}
			return myCombatCore.IsCriticalHit(nCriticalRate, nRand);
			__LEAVE_FUNCTION
			return FALSE;
		}
		Time_t SkillLogic_T::RefixPlayActionTimeWithAttackRate(Time_t nPlayActionTime, INT nAttackRate) const
		{
			return (nPlayActionTime * nAttackRate)/100;			
		}
		Time_t SkillLogic_T::RefixCooldownTimeWithAttackRate(Time_t nCooldownTime, INT nAttackRate) const
		{
			return (nCooldownTime * nAttackRate)/100;
			
		}
		VOID SkillLogic_T::RefixPowerByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			INT nDeplete = 0;
			if (TRUE == CanBeRefixed())
			{
				rSkill.SetPowerRefixByRate(nRate);
			}
			__LEAVE_FUNCTION
		}
		VOID SkillLogic_T::RefixDepleteByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			__ENTER_FUNCTION
			INT nDeplete = 0;
			if (TRUE == CanBeRefixed())
			{
				rSkill.SetDepleteRefixByRate(nRate);
			}
			__LEAVE_FUNCTION
			__LEAVE_FUNCTION
		}

		BOOL SkillLogic_T::ScanUnitForTarget(Obj_Character& rMe, FLOAT fX,FLOAT fZ, OBJLIST& rTargets)const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Scene* pScene = rMe.getScene();
			SCANOPERATOR_AESKILL_INIT ScanOperatorIniter;
			ScanOperatorIniter.m_pSkillInfo = &rSkillInfo;
			ScanOperatorIniter.m_pMe = &rMe;
			ScanOperatorIniter.m_pScene = rMe.getScene();
			ScanOperatorIniter.m_pTargets = &rTargets;
			ScanOperatorIniter.m_fRadius = rSkillInfo.GetRadius();
			ScanOperatorIniter.m_CentrePoint.m_fX = fX;
			ScanOperatorIniter.m_CentrePoint.m_fZ = fZ;

			if(-1==rSkillInfo.GetMaxTargetNumber()) //-1为不限数量
			{
				ScanOperatorIniter.m_nCount = rTargets.MAX_OBJ_LIST_SIZE-1;
			}
			else
			{
				ScanOperatorIniter.m_nCount= rSkillInfo.GetMaxTargetNumber();
			}
			ScanOperator_AESkill ScanOperator;
			ScanOperator.Init(&ScanOperatorIniter);
			if(NULL!=pScene)
			{
				if(FALSE==pScene->Scan(&ScanOperator))
				{
					rParams.SetErrCode(OR_ERROR);
					return FALSE;					
				}
			}
			else
			{
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		BOOL SkillLogic_T::ActivateOnce(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OBJLIST	Targets;
			HitFlagsForOBJLIST_T HitFlagList;
			if(FALSE == CalculateTargetList(rMe, Targets))
			{
				return FALSE;
			}
			HitFlagList.ClearAllFlags();
			INT nIdx = 0;
			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					Obj_Character* pChar = (Obj_Character*)pTarget;
					if(TRUE==HitThisTarget(rMe, *pChar))
					{
						HitFlagList.MarkFlagByIndex(nIdx); //Mark this Target Hitted
					}
					RegisterBeSkillEvent(*pChar, rMe, rSkillInfo.GetSkillID(), rSkillInfo.GetDelayTime());
				}
			}
			rParams.SetTargetCount(Targets.m_Count);

			//BroadCast The Target List message
			BroadcastTargetListMessage(rMe, Targets, HitFlagList);

			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					if(TRUE==HitFlagList.GetFlagByIndex(nIdx))
					{
						Obj_Character* pChar = (Obj_Character*)pTarget;
						BOOL bCriticalHit = CriticalHitThisTarget(rMe, *pChar);
						EffectOnUnitOnce(rMe, *pChar, bCriticalHit);
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::ActivateEachTick(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OBJLIST	Targets;
			HitFlagsForOBJLIST_T HitFlagList;
			if(FALSE == CalculateTargetList(rMe, Targets))
			{
				return FALSE;
			}
			HitFlagList.ClearAllFlags();
			INT nIdx = 0;
			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					Obj_Character* pChar = (Obj_Character*)pTarget;
					if(TRUE==HitThisTarget(rMe, *pChar))
					{
						HitFlagList.MarkFlagByIndex(nIdx); //Mark this Target Hitted
					}
				}
			}
			rParams.SetTargetCount(Targets.m_Count);

			//BroadCast The Target List message
			BroadcastTargetListMessage(rMe, Targets, HitFlagList);

			for(nIdx=0; Targets.m_Count>nIdx; ++nIdx)
			{
				Obj* pTarget = Targets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					if(TRUE==HitFlagList.GetFlagByIndex(nIdx))
					{
						Obj_Character* pChar = (Obj_Character*)pTarget;
						BOOL bCriticalHit = CriticalHitThisTarget(rMe, *pChar);
						EffectOnUnitEachTick(rMe, *pChar, bCriticalHit);
					}
				}
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			return TRUE;
		}
		BOOL SkillLogic_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			return TRUE;
		}
		BOOL SkillLogic_T::HitThisTarget(Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Scene* pScene = rMe.getScene();
			if(TRUE==rMe.IsFriend(&rTar))
			{
				//给友方使用的技能100%命中
				return TRUE;
			}
			//Hit Or Miss
			if(FALSE == IsHit(rMe, rTar, rSkillInfo.GetAccuracy()))
			{
				if(NULL!=pScene)
				{
					pScene->GetEventCore().RegisterSkillMissEvent(rTar.GetID(), rMe.GetID(), rSkillInfo.GetSkillID(), rSkillInfo.GetDelayTime());
				}
				return FALSE;
			}
			if(NULL!=pScene)
			{
				pScene->GetEventCore().RegisterSkillHitEvent(rTar.GetID(), rMe.GetID(), rSkillInfo.GetSkillID(), rSkillInfo.GetDelayTime());
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::CriticalHitThisTarget(Obj_Character & rMe, Obj_Character & rTar) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			if(TRUE==rMe.IsFriend(&rTar))
			{
				//攻击性技能才有会心一击，友好技能没有
				return FALSE;
			}
			// calculate critical hit
			if(TRUE == IsCriticalHit(rMe, rSkillInfo.GetCriticalRate()))
			{
				rMe.OnCriticalHitTarget(rSkillInfo.GetSkillID(), rTar);
				return TRUE;
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		
		BOOL SkillLogic_T::CalculateTargetList(Obj_Character & rMe, OBJLIST& rTargets) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rTargets.CleanUp();
			ID_t nTargetMode = rSkillInfo.GetTargetingLogic();
			BOOL bRet=FALSE;
			switch (nTargetMode)
			{
				case TARGET_SELF:
					{
						rTargets.m_aObj[0] = &rMe;
						++rTargets.m_Count;
						bRet = TRUE;
					}
					break;
				case TARGET_MY_PET:
					{
						Obj* pPet = rMe.GetMyPet();
						if(NULL!=pPet)
						{
							rTargets.m_aObj[0] = pPet;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_MY_SHADOW_GUARD:
					{
						Obj* pGuard = rMe.GetMyShadowGuard();
						if(NULL!=pGuard)
						{
							rTargets.m_aObj[0] = pGuard;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_MY_MASTER:
					{
						Obj* pMaster = rMe.GetMyMaster();
						if(NULL!=pMaster)
						{
							rTargets.m_aObj[0] = pMaster;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_AE_AROUND_SELF:
					{
						WORLD_POS const* pPos = rMe.getWorldPos();
						if(NULL!=pPos)
						{
							bRet = ScanUnitForTarget(rMe, pPos->m_fX,pPos->m_fZ,rTargets);
						}
					}
					break;
				case TARGET_SPECIFIC_UNIT:
					{
						Obj* pTarget = rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
						if(NULL!=pTarget)
						{
							rTargets.m_aObj[0] = pTarget;
							++rTargets.m_Count;
							bRet = TRUE;
						}
					}
					break;
				case TARGET_AE_AROUND_UNIT:
					{
						Obj* pTarget = rMe.GetSpecificObjInSameSceneByID(rParams.GetTargetObj());
						if(NULL!=pTarget)
						{
							WORLD_POS const* pPos = pTarget->getWorldPos();
							if(NULL!=pPos)
							{
								bRet = ScanUnitForTarget(rMe, pPos->m_fX, pPos->m_fZ, rTargets);
							}
						}
					}
					break;
				case TARGET_AE_AROUND_POSITION:
					{
						WORLD_POS const& rPos = rParams.GetTargetPosition();
						bRet = ScanUnitForTarget(rMe, rPos.m_fX, rPos.m_fZ, rTargets);
					}
					break;
				default:
					{
						bRet = FALSE;
					}
					break;
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterImpactEvent(Obj_Character& rReceiver, Obj_Character& rSender, OWN_IMPACT& rImp, Time_t nDelayTime, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rSender.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rSender.GetTargetingAndDepletingParams();
			Scene* pScene = rReceiver.getScene();
			// Routine process
			rImp.SetSkillID(rSkillInfo.GetSkillID());
			if(TRUE==bCriticalFlag)
			{
				rImp.MarkCriticalFlag();
			}
			// register impact
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterImpactEvent(rReceiver.GetID(), rSender.GetID(), rImp, nDelayTime, rImp.GetSkillID());
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterSkillMissEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const
		{
			__ENTER_FUNCTION
			Scene* pScene = rReceiver.getScene();
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterSkillMissEvent(rReceiver.GetID(), rSender.GetID(), nSkill, nDelayTime);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterBeSkillEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const
		{
			__ENTER_FUNCTION
			INT nBehaviorType = BEHAVIOR_TYPE_NEUTRALITY;
			SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkill);
			if(NULL!=pSkillTemplate)
			{
				if(0<pSkillTemplate->GetStandFlag())
				{
					nBehaviorType = BEHAVIOR_TYPE_AMITY;
				}
				else if (0>pSkillTemplate->GetStandFlag())
				{
					nBehaviorType = BEHAVIOR_TYPE_HOSTILITY;
				}
				else
				{
					nBehaviorType = BEHAVIOR_TYPE_NEUTRALITY;
				}
			}
			Scene* pScene = rReceiver.getScene();
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterBeSkillEvent(rReceiver.GetID(), rSender.GetID(), nBehaviorType, nDelayTime);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillLogic_T::RegisterActiveObj(Obj& rObj, Obj_Character& rSender, Time_t nDelayTime) const
		{
			__ENTER_FUNCTION
			Scene* pScene = rSender.getScene();
			if(NULL!=pScene)
			{
				return pScene->GetEventCore().RegisterActiveSpecialObjEvent(rObj.GetID(), nDelayTime);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID SkillLogic_T::BroadcastTargetListMessage(Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& rHitFlagsForObjList) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			GCTargetListAndHitFlags msg;
			GCTargetListAndHitFlags::HitFlagList_T& rHitFlags = msg.GetHitFlagList();
			msg.SetDataType(msg.UNIT_USE_SKILL);
			msg.SetObjID(rMe.GetID());
			msg.SetUserPos(*(rMe.getWorldPos()));
			msg.SetLogicCount(rMe.GetLogicCount());
			msg.SetSkillOrSpecialObjDataID(rSkillInfo.GetSkillID());
			msg.SetTargetID(rParams.GetTargetObj());
			msg.SetTargetPos(rParams.GetTargetPosition());
			msg.SetDir(rParams.GetTargetDirection());
			BYTE nNum = rTargets.m_Count;
			if(msg.MAX_TARGET_LIST_SIZE<nNum)
			{
				nNum = msg.MAX_TARGET_LIST_SIZE;
			}
			msg.SetTargetNum(nNum);
			INT nIdx=0;
			for(nIdx=0; nNum>nIdx; ++nIdx)
			{
				Obj* pTarget = rTargets.m_aObj[nIdx];
				if(NULL!=pTarget)
				{
					msg.SetTargetObjIDByIndex(nIdx, pTarget->GetID());
					if(TRUE == rHitFlagsForObjList.GetFlagByIndex(nIdx))
					{
						rHitFlags.MarkFlagByIndex(nIdx);
					}
				}
			}
			Scene* pScene = rMe.getScene();
			if(NULL!=pScene)
			{
				pScene->BroadCast(&msg, &rMe, TRUE);
			}
			__LEAVE_FUNCTION
		}
		BOOL SkillLogic_T::IsWantedTarget(Obj_Character& rMe, Obj_Character& rTar, SkillInfo_T const& rSkillInfo) const
		{
			__ENTER_FUNCTION
			switch (rSkillInfo.GetTargetCheckByObjType())
			{
				case 0: //目标必须是玩家
					if(Obj::OBJ_TYPE_HUMAN!=rTar.GetObjType())
					{
						return FALSE;//条件不符
					};
					break;
				case 1://目标必须是宠物
					if(Obj::OBJ_TYPE_PET!=rTar.GetObjType())
					{
						return FALSE;//条件不符
					}
					break;
				case 2://目标必须是怪物
					if(Obj::OBJ_TYPE_MONSTER!=rTar.GetObjType())
					{
						return FALSE;//条件不符
					}
					break;
				case -1:
				default:
					break;
			};
			if(0==rSkillInfo.GetTargetMustInSpecialState())
			{
				if(FALSE==rTar.IsAlive())
				{
					return FALSE;
				}
			}
			if(1==rSkillInfo.GetTargetMustInSpecialState())
			{
				if(FALSE==rTar.IsDie())
				{
					return FALSE;
				}
			}
			if(0==rSkillInfo.GetTargetLogicByStand())
			{
				if(FALSE==rMe.IsFriend(&rTar))
				{
					return FALSE;
				}
				else
				{
					if(TRUE == rSkillInfo.IsPartyOnly())
					{
						if(FALSE == rMe.IsPartner(&rTar))
						{
							return FALSE;
						}
					}
				}
			}
			if(1==rSkillInfo.GetTargetLogicByStand())
			{
				if(FALSE==rMe.IsEnemy(&rTar))
				{
					return FALSE;
				}
			}
			return IsSpecificTargetWanted(rMe, rTar, rSkillInfo);
			__LEAVE_FUNCTION
			return FALSE;
		}
	
		SkillLogic_T const* Skill_GetLogic(SkillInfo_T const& rSkill)
		{
		__ENTER_FUNCTION
			ID_t nLogicID = rSkill.GetLogicID();
			if(INVALID_ID==nLogicID)
			{
				return NULL;
			}

			return g_SkillLogicList.GetLogicById(nLogicID);
		__LEAVE_FUNCTION
			return NULL;
		}
	}
}
