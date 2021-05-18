#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact002.cpp
// 功能说明：效果--无类型的一次性百分率伤害
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact002.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact002_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			INT nDamage = rData.GetDescriptorByIndex(ImpactDataDescriptorIndex_T::IDX_DAMAGE)->GetValue();
			SetDamage(rImp, nDamage);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact002_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			INT nDamage =GetDamage(rImp);
			nDamage = Float2Int((rMe.GetBaseMaxHP()*nDamage)/100.0f);
			rMe.OnDamage(nDamage, rImp.GetCasterObjID(), rImp.GetSkillID());
			__LEAVE_FUNCTION
		}
		VOID StdImpact002_T::CriticalRefix(OWN_IMPACT& rImp) const
		{
			__ENTER_FUNCTION
			SetDamage(rImp, GetDamage(rImp)*2);
			rImp.MarkCriticalFlag();
			__LEAVE_FUNCTION
		}
		BOOL StdImpact002_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += 100;
			SetDamage(rImp, Float2Int((GetDamage(rImp)*nRate)/100.0f));
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};
