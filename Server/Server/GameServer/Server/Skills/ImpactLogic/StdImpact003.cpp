#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact003.cpp
// 功能说明：效果--分类型的一次性伤害
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact003.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact003_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetDamagePhy(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_PHY));
			SetDamageMagic(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_MAGIC));
			SetDamageCold(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_COLD));
			SetDamageFire(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_FIRE));
			SetDamageLight(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_LIGHT));
			SetDamagePoison(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_POISON));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact003_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			INT nDeltaHP = rMe.GetHP();
			//Calculate Total damage for display
			INT nDamage	=	GetDamagePhy(rImp)
						+	GetDamageMagic(rImp)
						+	GetDamageCold(rImp)
						+	GetDamageFire(rImp)
						+	GetDamageLight(rImp)
						+	GetDamagePoison(rImp);
			
			INT vDamage[DAMAGE_TYPE_NUMBER];
			vDamage[DAMAGE_TYPE_P]=GetDamagePhy(rImp);
			vDamage[DAMAGE_TYPE_M]=GetDamageMagic(rImp);
			vDamage[DAMAGE_TYPE_COLD]=GetDamageCold(rImp);
			vDamage[DAMAGE_TYPE_FIRE]=GetDamageFire(rImp);
			vDamage[DAMAGE_TYPE_LIGHT]=GetDamageLight(rImp);
			vDamage[DAMAGE_TYPE_POISON]=GetDamagePoison(rImp);
			rMe.OnDamages(vDamage, rImp.GetCasterObjID(), rImp.IsCriticalHit(), rImp.GetSkillID());
			__LEAVE_FUNCTION
		}
		VOID StdImpact003_T::CriticalRefix(OWN_IMPACT& rImp) const
		{
			__ENTER_FUNCTION
			SetDamagePhy(rImp, GetDamagePhy(rImp)*2);
			SetDamageMagic(rImp, GetDamageMagic(rImp)*2);
			SetDamageCold(rImp, GetDamageCold(rImp)*2);
			SetDamageFire(rImp, GetDamageFire(rImp)*2);
			SetDamageLight(rImp, GetDamageLight(rImp)*2);
			SetDamagePoison(rImp, GetDamagePoison(rImp)*2);
			rImp.MarkCriticalFlag();
			__LEAVE_FUNCTION
		}
		BOOL StdImpact003_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += 100;
			SetDamagePhy(rImp, Float2Int((GetDamagePhy(rImp)*nRate)/100.0f));
			SetDamageMagic(rImp, Float2Int((GetDamageMagic(rImp)*nRate)/100.0f));
			SetDamageCold(rImp, Float2Int((GetDamageCold(rImp)*nRate)/100.0f));
			SetDamageFire(rImp, Float2Int((GetDamageFire(rImp)*nRate)/100.0f));
			SetDamageLight(rImp, Float2Int((GetDamageLight(rImp)*nRate)/100.0f));
			SetDamagePoison(rImp, Float2Int((GetDamagePoison(rImp)*nRate)/100.0f));
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};
