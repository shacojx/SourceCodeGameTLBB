#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact037.cpp
// 功能说明：效果--增加指定效果的持续时间
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "StdImpact029.h"
#include "StdImpact030.h"
#include "StdImpact037.h"

namespace Combat_Module
{
	
	namespace Impact_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Skill_Module;
		BOOL StdImpact037_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateTimes(rImp, GetActivateTimesInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact037_T::RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			BOOL bRet = FALSE;
			if(0==GetActivateTimes(rImp)||rImp.IsFadeOut())
			{
				return;
			}
			if(FALSE == Impact_IsImpactInCollection(rImpactNeedRefix, GetTargetImpactCollection(rImp)))
			{
				return;
			}
			INT nRate = GetShieldHpRefix(rImp);
			nRate +=100;
			INT nHp = 0;
			switch (Impact_GetLogicID(rImpactNeedRefix))
			{
				case DS_EMei009_T::ID:
					{
						DS_EMei009_T logic;
						nHp = logic.GetShieldHp(rImpactNeedRefix);
						nHp = Float2Int((nHp*nRate)/100.0f);
						if(0>nHp)
						{
							nHp=0;
						}
						logic.SetShieldHp(rImp, nHp);
						bRet = TRUE;
					}
					break;
				case DS_EMei014_T::ID:
					{
						DS_EMei014_T logic;
						nHp = logic.GetShieldHp(rImpactNeedRefix);
						nHp = Float2Int((nHp*nRate)/100.0f);
						if(0>nHp)
						{
							nHp=0;
						}
						logic.SetShieldHp(rImp, nHp);
						bRet = TRUE;
					}
					break;
				default:
					AssertEx(FALSE,"[StdImpact037_T::RefixImpact]: rImpactNeedRefix isn't Emei huti impact!!");
					return;
					break;
			}
			if(TRUE==bRet)
			{
				INT nActivateTimes = GetActivateTimes(rImp);
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

