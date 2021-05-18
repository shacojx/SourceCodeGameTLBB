#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact019.cpp
// 功能说明：效果--增加指定效果的威力和持续时间
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact019.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Skill_Module;
		BOOL StdImpact019_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateTimes(rImp, GetActivateTimesInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact019_T::RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			BOOL bRet = FALSE;
			if(0==GetActivateTimes(rImp)||TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(FALSE == Impact_IsImpactInCollection(rImpactNeedRefix, GetTargetImpactCollection(rImp)))
			{
				return;
			}
			ImpactLogic_T const* pLogic = Impact_GetLogic(rImpactNeedRefix);
			if(NULL==pLogic)
			{
				return;
			}
			INT nActivateTimes = GetActivateTimes(rImp);
			if(0!=GetPowerRefix(rImp))
			{
				bRet = bRet || pLogic->RefixPowerByRate(rImpactNeedRefix, GetPowerRefix(rImp));
			}
			if(0!=GetContinuanceRefix(rImp))
			{
				bRet = bRet || pLogic->RefixContinuanceByRate(rImpactNeedRefix, GetContinuanceRefix(rImp));					
			}
			if(TRUE==bRet)
			{
				if(0<nActivateTimes)
				{
					--nActivateTimes;
					if(0==nActivateTimes)
					{
						rMe.Impact_OnImpactFadeOut(rImp);
					}
					SetActivateTimes(rImp, nActivateTimes);
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

