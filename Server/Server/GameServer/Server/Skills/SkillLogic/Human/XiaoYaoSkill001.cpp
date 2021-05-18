#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYaoSkill001.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "XiaoYaoSkill001.h"
#include "StdImpact052.h"
#include "Obj_Special.h"
#include "GameTable.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		using namespace Combat_Module::Special_Obj_Module;

		//class XiaoYaoSkill001_T : public SkillLogic_T
		BOOL XiaoYaoSkill001_T::SpecificConditionCheck(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			SOT_XiaoYaoTraps_T Logic;
			INT nAllTypeTotal = 0;
			INT nThisTypeCount = 0;
			INT const nMaxTrapCount = Logic.MAX_TRAP_COUNT;
			INT nImpactDataIndex = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			INT nTrapDataIndex = rSkillInfo.GetDescriptorByIndex(1)->GetValue();
			INT const nCountUpBorder = rSkillInfo.GetDescriptorByIndex(2)->GetValue();
			SpecialObjData_T const* pData = g_SpecialObjDataMgr.GetInstanceByID(nTrapDataIndex);
			if(NULL==pData)
			{
				AssertEx(FALSE, "[SkillLogic_T::SpecificConditionCheck]: Illegal Trap data found!!");
				return FALSE;
			}
			INT nType = pData->GetClass();
			OWN_IMPACT* pImp = rMe.Impact_GetFirstImpactOfSpecificLogicID(Logic.ID);
			INT nTrapCount = 0;
			if(NULL!=pImp)
			{
				Logic.GetTrapCountOfSpecificType(*pImp, rMe, nType, nAllTypeTotal, nThisTypeCount);
				if(nMaxTrapCount<=nAllTypeTotal || nCountUpBorder<=nThisTypeCount)
				{
					rParams.SetErrCode(OR_TOO_MANY_TRAPS);
					return FALSE;
				}
			}
			rParams.SetErrCode(OR_OK);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

		BOOL XiaoYaoSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			INT nImpactDataIndex = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
			INT nTrapDataIndex = rSkillInfo.GetDescriptorByIndex(1)->GetValue();
			if(0>nImpactDataIndex || 0>nTrapDataIndex)
			{
				AssertEx(FALSE,"[XiaoYaoSkill001_T::EffectOnUnitOnce]: Illegal data index found!!");
				return FALSE;
			}
			Obj_Special* pTrap = (Obj_Special*)rMe.Skill_CreateObjSpecial(*(rMe.getWorldPos()), nTrapDataIndex);
			if(NULL==pTrap)
			{
				AssertEx(FALSE,"[SkillLogic_T::TakeEffectNow_ForXiaoYaoTraps]: Can't create Special Obj!");
				return FALSE;
			}
			// Refix trap
			pTrap->SetPowerRefixByRate(rSkillInfo.GetPowerRefixByRate());
			pTrap->SetPowerRefixByValue(rSkillInfo.GetPowerRefixByValue());
			INT nContinuance = pTrap->GetContinuance();
			nContinuance = nContinuance + Float2Int(rSkillInfo.GetTimeRefixByRate()*nContinuance/100.0f);
			nContinuance += rSkillInfo.GetTimeRefixByValue();
			pTrap->SetContinuance(nContinuance);
			// Refixing end
			SOT_XiaoYaoTraps_T Logic;
			OWN_IMPACT impact;
			OWN_IMPACT* pImp = rMe.Impact_GetFirstImpactOfSpecificLogicID(Logic.ID);
			if(NULL==pImp)
			{
				g_ImpactCore.InitImpactFromData(nImpactDataIndex, impact);
				if(Logic.ID != Impact_GetLogicID(impact))
				{
					AssertEx(FALSE, "[XiaoYaoSkill001_T::EffectOnUnitOnce]: Impact Data Error, Check it now..");
					return FALSE;					
				}
				if(FALSE==Logic.AddNewTrap(impact, rMe, pTrap->GetID()))
				{
					//前面检查过陷阱的数量,这里应该永远走不到
					AssertEx(FALSE, "[XiaoYaoSkill001_T::EffectOnUnitOnce]: Unknown Error, Check it now..");
					return FALSE;
				}
				//Register impact event
				RegisterImpactEvent(rMe, rMe, impact, 100, FALSE);
			}
			else
			{
				if(FALSE==Logic.AddNewTrap(*pImp, rMe, pTrap->GetID()))
				{
					return FALSE;
				}
			}
			//Register Active obj event
			RegisterActiveObj(*pTrap, rMe, 500);
			
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
