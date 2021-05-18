#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact010.cpp
// 功能说明：效果--间隔发作的驻留效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact010.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact010_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact010_T::OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const 
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(rMe, rImp);
			for(INT nIdx=0; ImpactData_T::NUMBER_OF_PARAMS>nIdx; ++nIdx)
			{
				OWN_IMPACT impact;
				INT nDataIndex = GetSubImpactDataIndexByIndex(rImp, nIdx);
				if(0<nDataIndex)
				{
					g_ImpactCore.SendImpactToUnit(rMe, nDataIndex, rImp.GetCasterObjID(), 0, rImp.IsCriticalHit(), GetRefixRate(rImp));
				}
			}
			__LEAVE_FUNCTION
		}

		BOOL StdImpact010_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

