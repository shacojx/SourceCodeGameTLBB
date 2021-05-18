#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：Skill_Core.cpp
// 功能说明：技能内核模块
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file Skill_Core.cpp
*		\brief Skill Process module
*/
#include "Type.h"
#include "Skill_Core.h"
#include "Scene.h"
#include "LogicManager.h"
#include "Log.h"
#include "OResultDef.h"
#include "HumanItemLogic.h"
#include "GameTable.h"
#include "ActionDelegator.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Packets;
		using namespace Action_Module;
		//class SkillCore_T
		BOOL SkillCore_T::ProcessSkillRequest(Obj_Character& rMe, SkillID_t nSkillID, BYTE nLevel, ObjID_t nTargetID, WORLD_POS const& rTargetPos, FLOAT fTargetDir, GUID_t guidTarget) const
		{
			__ENTER_FUNCTION
			BOOL bRet = TRUE;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
			if(NULL==pSkillTemplate)
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				return FALSE;
			}

			if(INVALID_ID==nSkillID)
			{
				rParams.SetErrCode(OR_OK);
				return TRUE;
			}
			if(FALSE==rMe.IsAlive())
			{
				rParams.SetErrCode(OR_DIE);
				return FALSE;
			}
			//是否当前状态不允许使用这个技能
			if(FALSE==rMe.Skill_CanUseThisSkillInThisStatus(nSkillID))
			{
				rParams.SetErrCode(OR_LIMIT_USE_SKILL);
				return FALSE;
			}
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType()&&A_SKILL_FOR_PLAYER==pSkillTemplate->GetClassByUser())
			{
				if(FALSE == rMe.Skill_HaveSkill(nSkillID, nLevel)&&FALSE==rParams.GetIgnoreConditionCheckFlag())
				{	//角色不会这个技能
					rParams.SetErrCode(OR_INVALID_SKILL);
					return FALSE;
				}
			}
			if(FALSE==rMe.Skill_IsSkillCooldowned(nSkillID))
			{
				rParams.SetErrCode(OR_COOL_DOWNING);
				return FALSE;
			}
			if(FALSE==GetGlobalActionDelegator().CanDoNextAction(rMe))
			{
				rParams.SetErrCode(OR_BUSY);
				return FALSE;
			}
			if( pSkillTemplate->GetSkillClass() == INVALID_ID &&
				pSkillTemplate->GetMenPai() == INVALID_ID )
			{
				nLevel = 1;
			}
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				SkillID_t	nSkillInstance = pSkillTemplate->GetSkillInstance( nLevel-1 );
				Skill_Module::SkillInstanceData_T const* pSkillInstance = GetSkillInstanceByID(nSkillInstance);
				Obj_Human& rHuman = (Obj_Human&)rMe;
				if( pSkillInstance == NULL )
				{
					rParams.SetErrCode(OR_INVALID_SKILL);
					return FALSE;
				}
				if( rHuman.GetLevel() < pSkillInstance->GetStudyLevel() )
				{
					rParams.SetErrCode(OR_NEED_HIGH_LEVEL_XINFA);
					return FALSE;
				}
			}
			rParams.SetActivatedSkill(nSkillID);
			rParams.SetSkillLevel(nLevel);
			rParams.SetTargetObj(nTargetID);
			rParams.SetTargetPosition(rTargetPos);
			rParams.SetTargetDirection(fTargetDir);
			rParams.SetTargetGuid(guidTarget);
			bRet=ActiveSkillNow(rMe);
			if(FALSE==bRet)
			{
				g_SkillCore.OnException(rMe);
			}
			return bRet;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillCore_T::InstanceSkill(SkillInfo_T& rSkillInfoOut,Obj_Character& rMe, SkillID_t nSkill, BYTE nLevel) const
		{
			__ENTER_FUNCTION
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Skill_Module::SkillTemplateData_T const* pSkillTemplate = GetSkillTemplateByID(nSkill);
			if(NULL == pSkillTemplate)
			{
				AssertEx(FALSE,"[CombatCore_T::InstanceSkill]: Can't find skill date in the SkillTemplateDateMgr!");
				return FALSE;
			}
			//复制技能模板信息

			rSkillInfoOut= *pSkillTemplate; // Copy Skill data to output buffer
			//根据技能模板信息取心法信息
			//ID_t nSkillClass = rSkillInfoOut.GetSkillClass();
			//INT nXinFaParam = rSkillInfoOut.GetXinFaParam();
			//计算心法等级: 没有心法的,心法级别为0
			//INT nXinFaLvl = 0;
			//if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType())
			//{
			//	nXinFaLvl= rMe.Skill_GetXinFaLevel(nXinFaID);
			//	if(0<=nXinFaLvl)
			//	{
			//		nXinFaLvl = (nXinFaLvl+nXinFaParam)/10;
			//	}
			//	else
			//	{
			//		nXinFaLvl = 0;//暂时先这样
			//	}
			//}
			//if(0>nXinFaLvl)
			//{
			//	nXinFaLvl = 0;
			//}
			//if(MAX_XINFA_LEVEL_NUM/10 <= nXinFaLvl)
			//{
			//	nXinFaLvl = MAX_XINFA_LEVEL_NUM/10 -1;
			//}

			//保存相关的心法等级
			if( nLevel == 0 || nLevel > MAX_CHAR_SKILL_LEVEL )
			{
				GUID_t nGUID = 0;
				if( rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN )
				{
					Obj_Human* pTempHuman = (Obj_Human*)&rMe;
					nGUID = pTempHuman->GetGUID();
				}
				g_pLog->FastSaveLog( LOG_FILE_1, "[SkillCore_T::InstanceSkill]: Warning Skill Level outof Range. Skill=%d, Level=%d, GUID=%X", 
					nSkill, nLevel, nGUID) ;
				nLevel = 1;
			}
			rParams.SetSkillLevel(nLevel);
			//根据心法等级,取实际技能数据
			SkillID_t	nSkillInstance = pSkillTemplate->GetSkillInstance( rParams.GetSkillLevel()-1 );
			Skill_Module::SkillInstanceData_T const* pSkillInstance = GetSkillInstanceByID(nSkillInstance);
			if(NULL == pSkillInstance)
			{
				// Warning Skill data don't match the template data, plz check the data table.
				AssertEx(FALSE,"[SkillCore_T::InstanceSkill]: Can't find skill instance!");
				g_pLog->FastSaveLog( LOG_FILE_1, "[SkillCore_T::InstanceSkill]: Warning Skill Data don't match SkillTemplate Data TemplateID=%d, XinFaLvl=%d, SkillDataID=%d.", 
					nSkill, rParams.GetSkillLevel()-1, nSkillInstance) ;
				return FALSE;
			}
			rSkillInfoOut= *pSkillInstance; //transfer data from skilldata to runtime skill instance
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL SkillCore_T::ActiveSkillNow(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(FALSE==rMe.IsAlive()||FALSE==rMe.IsActiveObj())
			{
				rParams.SetErrCode(OR_DIE);
				return FALSE;
			}
			rSkillInfo.Init();
			if(FALSE==InstanceSkill(rSkillInfo, rMe, rParams.GetActivatedSkill(), rParams.GetSkillLevel()))
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				rParams.SetErrParam(rParams.GetActivatedSkill());
				return FALSE;
			}
			//refix skill instance
			rMe.RefixSkill(rSkillInfo);
			//perform checking
			INT nLogicID = rSkillInfo.GetLogicID();
			SkillLogic_T const* pLogic = g_SkillLogicList.GetLogicById(nLogicID);
			if(NULL==pLogic)
			{
				rParams.SetErrCode(OR_INVALID_SKILL);
				rParams.SetErrParam(rParams.GetActivatedSkill());
				return FALSE;
			}
			if(TRUE == pLogic->IsPassive())
			{
				AssertEx(FALSE,"[SkillCore_T::HeartBeat]: Found passive Skill!!!Data error!!");
				rParams.SetErrCode(OR_ERROR);
				return FALSE;
			}
			if(TRUE == pLogic->CancelSkillEffect(rMe))
			{
				rParams.SetErrCode(OR_OK);
				return TRUE;
			}
			//Prefix end
			if(FALSE==rMe.Skill_IsSkillCooldowned(rParams.GetActivatedSkill())&& FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				rParams.SetErrCode(OR_COOL_DOWNING);
				return FALSE;
			}
			if(TRUE == rSkillInfo.GetMustUseWeaponFlag())
			{
				if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
				{
					Obj_Human* pHuman = (Obj_Human*)&rMe;
					Item* pItem=HumanItemLogic::GetEquip(pHuman, HEQUIP_WEAPON);
					if(NULL==pItem)
					{
						rParams.SetErrCode(OR_NEED_A_WEAPON);
						return FALSE;
					}
					else if(TRUE==pItem->IsEmpty())
					{
						rParams.SetErrCode(OR_NEED_A_WEAPON);
						return FALSE;						
					}
				}
			}
			if(FALSE==rParams.GetIgnoreConditionCheckFlag())
			{
				if(FALSE == pLogic->IsConditionSatisfied(rMe))
				{
					return FALSE;
				}
				if(FALSE==pLogic->SpecificOperationOnSkillStart(rMe))
				{
					return FALSE;
				}
			}
			switch (rSkillInfo.GetSkillType())
			{
				case SKILL_INSTANT_LAUNCHING:
					pLogic->StartLaunching(rMe);
					break;
				case SKILL_NEED_CHARGING:
					pLogic->StartCharging(rMe);
					break;
				case SKILL_NEED_CHANNELING:
					pLogic->StartChanneling(rMe);
					break;
				default:
					AssertEx(FALSE,"[SkillCore_T::ActiveSkillNow]: Try to active a un-active skill!");
					break;
			}				
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE ;
		}
		VOID SkillCore_T::OnException(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType())
			{
				SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
				TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
				if (OR_FAILED(rParams.GetErrCode()))
				{
					Obj_Human& rHuman = static_cast<Obj_Human&>(rMe);
					rHuman.SendOperateResultMsg(rParams.GetErrCode());
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL Skill_IsSkillInCollection(SkillInfo_T const& rSkillInfo, INT nCollection)
		{
			__ENTER_FUNCTION
			INT nID=INVALID_ID;
			IDCollection_T const* pCollection = NULL;
			pCollection = g_IDCollectionMgr.GetInstanceByID(nCollection);
			if(NULL!=pCollection)
			{
				switch (pCollection->GetType())
				{
					case IDCollection_T::TYPE_SKILL_ID:
						nID = rSkillInfo.GetSkillID();
						break;
					case IDCollection_T::TYPE_BUFF_ID:
					case IDCollection_T::TYPE_IMPACT_LOGIC_ID:
					case IDCollection_T::TYPE_IMPACT_MUTEX_ID:
					case IDCollection_T::TYPE_DIRECT_IMPACT_ID:
					case IDCollection_T::TYPE_SKILL_LOGIC_ID:
					default:
						nID = INVALID_ID;
						break;
				}
				if(INVALID_ID != nID)
				{
					if(0<pCollection->GetCollectionSize())
					{
						return pCollection->IsThisIDInCollection(nID);
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		INT Skill_GetSkillInfoDescriptorValueByIndex(SkillInfo_T const& rSkillInfo, INT nIndex)
		{
			__ENTER_FUNCTION
			Descriptor_T const* pDescriptor = rSkillInfo.GetDescriptorByIndex(nIndex);
			if(NULL!=pDescriptor)
			{
				return pDescriptor->GetValue();
			}
			return 0;
			__LEAVE_FUNCTION
			return 0;
		}
		ConDepTerm_T const* Skill_GetConDepTermByIndex(SkillInfo_T const& rSkillInfo, INT nIndex)
		{
			__ENTER_FUNCTION
			return rSkillInfo.GetConDepTermByIndex(nIndex);
			__LEAVE_FUNCTION
			return NULL;
		}
	}
}
Combat_Module::Skill_Module::SkillCore_T	g_SkillCore;
