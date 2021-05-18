#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：GaiBangSkill001.cpp
// 功能说明：丐帮技能见龙在田
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "GaiBangSkill001.h"
#include "StdImpact003.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class GaiBangSkill001_T : public SkillLogic_T
		BOOL GaiBangSkill001_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			OWN_IMPACT impact;
			INT nDepleteStrikePointSegment = rParams.GetDepletedStrikePoints()/STRIKE_POINT_SEGMENT_SIZE;
			if(1>nDepleteStrikePointSegment)
			{
				nDepleteStrikePointSegment=1;
			}
			else if((MAX_STRIKE_POINT_SEGMENT)<nDepleteStrikePointSegment)
			{
				nDepleteStrikePointSegment=MAX_STRIKE_POINT_SEGMENT;
			}
			INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, nDepleteStrikePointSegment-1);
			if(INVALID_ID!=nDataIndex)
			{
				g_ImpactCore.InitImpactFromData(nDataIndex, impact);
				if(DI_DamagesByValue_T::ID == Impact_GetLogicID(impact))
				{
					DI_DamagesByValue_T logic;
					logic.RefixPowerByRate(impact, rSkillInfo.GetPowerRefixByRate());
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					myCombatCore.GetResultImpact(rMe, rTar, impact);
				}
				// register impact event
				RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
