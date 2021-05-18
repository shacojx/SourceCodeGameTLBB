#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact021.cpp
// 功能说明：效果--提升下n次技能的威力
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact021.h"
#include "SkillLogic.h"

namespace Combat_Module
{
	
	namespace Impact_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Skill_Module;
		BOOL StdImpact021_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateTimes(rImp, GetActivateTimesInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact021_T::RefixSkill(OWN_IMPACT & rImp, SkillInfo_T & rSkill) const
		{
			__ENTER_FUNCTION
			BOOL bFound = FALSE;
			INT nValue = 0;
			INT nActiveTimes=GetActivateTimes(rImp);
			if(0==nActiveTimes)
			{
				return;
			}
			if(FALSE == Skill_IsSkillInCollection(rSkill, GetTargetCollection(rImp)))
			{
				return;
			}
			SkillLogic_T const* pSkillLogic = Skill_GetLogic(rSkill);
			if(NULL==pSkillLogic)
			{
				return;
			}

			// Refix Hit
			INT nHitRefix = GetHitRefix(rImp);
			if(0!=nHitRefix)
			{
				rSkill.SetAccuracy(nHitRefix);
			}

			// Refix Critical
			INT nCriticalRefix = GetCriticalRefix(rImp);
			if(0!=nCriticalRefix)
			{
				rSkill.SetCriticalRate(nCriticalRefix);
			}

			// Refix Power
			INT nPowerRefixByValue = GetPowerRefixByValue(rImp);
			INT nPowerRefixByRate = GetPowerRefixByRate(rImp);
			if(0!=nPowerRefixByValue)
			{
				rSkill.SetPowerRefixByValue(nPowerRefixByValue);
			}
			if(0!=nPowerRefixByRate)
			{
				rSkill.SetPowerRefixByRate(nPowerRefixByRate);
			}

			// Refix Deplete
			INT nDepleteRefixByValue = GetDepleteRefixByValue(rImp);
			INT nDepleteRefixByRate = GetDepleteRefixByRate(rImp);
			if(0!=nDepleteRefixByValue)
			{
				rSkill.SetDepleteRefixByValue(nDepleteRefixByValue);
			}
			if(0!=nDepleteRefixByRate)
			{
				rSkill.SetDepleteRefixByRate(nDepleteRefixByRate);
			}

			// Refix Time
			INT nTimeRefixByValue = GetTimeRefixByValue(rImp);
			INT nTimeRefixByRate = GetTimeRefixByRate(rImp);
			if(0!=nTimeRefixByValue)
			{
				rSkill.SetTimeRefixByValue(nTimeRefixByValue);
			}
			if(0!=nTimeRefixByRate)
			{
				rSkill.SetTimeRefixByRate(nTimeRefixByRate);
			}

			// Refix Cooldown
			INT nCooldownRefixByValue = GetCooldownRefixByValue(rImp);
			INT nCooldownRefixByRate = GetCooldownRefixByRate(rImp);
			if(0!=nCooldownRefixByValue)
			{
				nValue = rSkill.GetCooldownTime();
				nValue += nCooldownRefixByValue;
				if(0>nValue)
				{
					nValue = 0;
				}
				rSkill.SetCooldownTime(nValue);
			}
			if(0!=nCooldownRefixByRate)
			{
				nValue = rSkill.GetCooldownTime();
				nValue += Float2Int(nValue*nCooldownRefixByRate/100.0f);
				if(0>nValue)
				{
					nValue = 0;
				}
				rSkill.SetCooldownTime(nValue);
			}

			// Refix Charge
			INT nChargeRefixByValue = GetChargeRefixByValue(rImp);
			INT nChargeRefixByRate = GetChargeRefixByRate(rImp);
			if(0!=nChargeRefixByValue)
			{
				nValue = rSkill.GetChargeTime();
				nValue += nChargeRefixByValue;
				if(0>nValue)
				{
					nValue = 0;
				}
				rSkill.SetChargeTime(nValue);
			}
			if(0!=nChargeRefixByRate)
			{
				nValue = rSkill.GetChargeTime();
				nValue += Float2Int(nValue*nChargeRefixByRate/100.0f);
				if(0>nValue)
				{
					nValue = 0;
				}
				rSkill.SetChargeTime(nValue);
			}

			// Refix Channel
			INT nChannelRefixByValue = GetChannelRefixByValue(rImp);
			INT nChannelRefixByRate = GetChannelRefixByRate(rImp);
			if(0!=nChannelRefixByValue)
			{
				nValue = rSkill.GetChannelTime();
				nValue += nChannelRefixByValue;
				if(0>nValue)
				{
					nValue = 0;
				}
				rSkill.SetChannelTime(nValue);
			}
			if(0!=nChannelRefixByRate)
			{
				nValue = rSkill.GetChannelTime();
				nValue += Float2Int(nValue*nChannelRefixByRate/100.0f);
				if(0>nValue)
				{
					nValue = 0;
				}
				rSkill.SetChannelTime(nValue);
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact021_T::OnUseSkillSuccessfully(OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill) const
		{
			__ENTER_FUNCTION
			BOOL bFound = FALSE;
			INT nActiveTimes=GetActivateTimes(rImp);
			if(0==nActiveTimes)
			{
				return;
			}
			if(FALSE == Skill_IsSkillInCollection(rSkill, GetTargetCollection(rImp)))
			{
				return;
			}
			SkillLogic_T const* pSkillLogic = Skill_GetLogic(rSkill);
			if(NULL==pSkillLogic)
			{
				return;
			}
			if(0<nActiveTimes)
			{
				--nActiveTimes;
				if(0==nActiveTimes)
				{
					rMe.Impact_OnImpactFadeOut(rImp);
				}
				SetActivateTimes(rImp, nActiveTimes);
				return;
			}
			__LEAVE_FUNCTION
		}
	};
};

