#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：PetSkill001.cpp
// 功能说明：
//
// 效果描述部分的格式：|效果数据Index|
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "PetSkill001.h"
#include "Obj_Pet.h"
#include "Obj_Human.h"
#include "StdImpact022.h"
#include "StdImpact023.h"
#include "StdImpact024.h"
#include "StdImpact025.h"
#include "Impact_Core.h"

namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class PetSkill001_T : public SkillLogic_T
		BOOL PetSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(Obj::OBJ_TYPE_PET!=rMe.GetObjType())
			{
				return FALSE;
			}
			INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, SkillInfoDescriptorIndex_T::IDX_IMPACT_DATA_INDEX);

			OWN_IMPACT impact;
			g_ImpactCore.InitImpactFromData(nDataIndex, impact);

			// 根据AI规则进行修正生效值
			Obj_Pet& rPet = (Obj_Pet&)rMe;
			INT nActivateOdds = 0;
			switch (Impact_GetLogicID(impact))
			{
				case DS_PetRevenge_T::ID :
					{
						DS_PetRevenge_T const logic;
						nActivateOdds = logic.GetActivateOdds(impact);
						rPet.GetRateOfBuffTakeEffect(rParams.GetActivatedSkill(), nActivateOdds);			
						logic.SetActivateOdds(impact, nActivateOdds);
					}
					break;
				case DS_PetDefence_T::ID :
					{
						DS_PetDefence_T const logic;
						nActivateOdds = logic.GetActivateOdds(impact);
						rPet.GetRateOfBuffTakeEffect(rParams.GetActivatedSkill(), nActivateOdds);			
						logic.SetActivateOdds(impact, nActivateOdds);
					}
					break;
				case DS_FilterDebuff_T::ID :
					{
						DS_FilterDebuff_T const logic;
						nActivateOdds = logic.GetActivateOdds(impact);
						rPet.GetRateOfBuffTakeEffect(rParams.GetActivatedSkill(), nActivateOdds);			
						logic.SetActivateOdds(impact, nActivateOdds);
					}
					break;
				case DS_CounteractCriticalHit_T::ID :
					{
						DS_CounteractCriticalHit_T const logic;
						nActivateOdds = logic.GetActivateOdds(impact);
						rPet.GetRateOfBuffTakeEffect(rParams.GetActivatedSkill(), nActivateOdds);			
						logic.SetActivateOdds(impact, nActivateOdds);
					}
					break;
				default:
					AssertEx(FALSE, "[PetSkill001_T::EffectOnUnitOnce]: Impact Data wrong!!");
					return FALSE;
					break;
			}
			//send impact to petself
			RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);

			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
