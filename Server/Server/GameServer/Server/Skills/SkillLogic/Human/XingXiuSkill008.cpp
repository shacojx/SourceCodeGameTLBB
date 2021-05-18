#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：XingXiuSkill008.cpp
// 功能说明：移花接木
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "XingXiuSkill008.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_Damages.h"
#include "LogicManager.h"
#include "GameTable.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class XingXiuSkill008_T : public SkillLogic_T
		BOOL XingXiuSkill008_T::SpecificConditionCheck(Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			ID_t nID = rSkillInfo.GetConDepTermByIndex(1)->GetParam0();
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(nID);
			if(NULL==pCollection)
			{
				AssertEx(FALSE,"[XingXiuSkill008_T::SpecificConditionCheck]: Can not find ID collection!");
				return FALSE;
			}
			INT const nSize = pCollection->GetCollectionSize();
			Obj* pObj = GetTargetObj(rMe);
			if(TRUE == IsCharacterObj(pObj->GetObjType()))
			{
				Obj_Character* pCharacter = (Obj_Character*)pObj;
				for(INT nIdx=0;nSize>nIdx;++nIdx)
				{
					if(TRUE==pCharacter->Impact_HaveImpactOfSpecificImpactID(pCollection->GetIDByIndex(nIdx)))
					{
						return TRUE;
					}
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		//Skill Refix interfaces
		VOID XingXiuSkill008_T::RefixAttackPowerByRate(SkillInfo_T& rSkill, INT nRate) const
		{
			__ENTER_FUNCTION
			INT nValue = rSkill.GetDescriptorByIndex(1)->GetValue();
			nValue += (nValue*nRate + 50)/100;
			SkillInstanceData_T::Descriptor_T* pDescriptor = (SkillInstanceData_T::Descriptor_T*)rSkill.GetDescriptorByIndex(1);
			pDescriptor->SetValue(nValue);
			__LEAVE_FUNCTION
		}
		
		BOOL XingXiuSkill008_T::EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			INT nDamage = rSkillInfo.GetDescriptorByIndex(1)->GetValue();
			INT nTransferRate = rSkillInfo.GetDescriptorByIndex(2)->GetValue();
			INT nAbsorbed = (nDamage*nTransferRate+50)/100;

			OWN_IMPACT impact1;
			DI_Damage_T const logic1;
			logic1.InitStruct(impact1);
			logic1.SetDamage(impact1, nDamage);

			OWN_IMPACT impact2;
			DI_Heal_T const logic2;
			logic2.InitStruct(impact2);
			logic2.SetHealedHP(impact2, nAbsorbed);
			impact2.m_nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();

			//register impact event
			RegisterImpactEvent(rTar, rMe, impact1, rParams.GetDelayTime(), bCriticalFlag);
			RegisterImpactEvent(rMe, rMe, impact2, rParams.GetDelayTime(), bCriticalFlag);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
