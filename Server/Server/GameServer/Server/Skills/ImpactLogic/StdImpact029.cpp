#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact029.cpp
// 功能说明：效果--峨嵋技能虚冲养气的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact029.h"
#include "Skill_Core.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module;
	using namespace Combat_Module::Skill_Module;
	
	namespace Impact_Module
	{
		BOOL StdImpact029_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetShieldHp(rImp, GetShieldHpInTable(rImp));
			SetAbsorbRate(rImp, GetAbsorbRateInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact029_T::RefixSkill(OWN_IMPACT& rImp, SkillInfo_T& rSkill) const
		{
			__ENTER_FUNCTION
			if(TRUE == Skill_IsSkillInCollection(rSkill, GetTargetSkillCollectionForDepletionRefix(rImp)))
			{
				//Refix Mana deplettion
				rSkill.SetDepleteRefixByRate(GetDepletionRefix(rImp));
			}
			else if(TRUE == Skill_IsSkillInCollection(rSkill, GetTargetSkillCollectionForChargeTimeRefix(rImp)))
			{
				Time_t nTime = rSkill.GetChargeTime();
				if(0<nTime)
				{
					//Refix Charge Time
					INT nRate = GetChargeTimeRefix(rImp);
					nRate += 100;
					nTime = Float2Int((nTime*nRate)/100.0f);
					rSkill.SetChargeTime(nTime);
				}
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact029_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			INT nDamage = Float2Int((rDamage*GetAbsorbRate(rImp))/100.0f);
			INT nShieldHp = GetShieldHp(rImp);
			if(nDamage>nShieldHp)
			{
				nDamage=nShieldHp;
			}
			nShieldHp-=nDamage;
			SetShieldHp(rImp, nShieldHp);
			rDamage -=nDamage;
			if(0>=GetShieldHp(rImp))
			{
				rMe.Impact_OnImpactFadeOut(rImp);
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact029_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			// Get additional rage
			if(Obj::OBJ_TYPE_HUMAN == rMe.GetObjType())
			{
				Obj_Human& rHuman = (Obj_Human&)rMe;
				INT nRage = GetAdditionalRage(rImp);
				rHuman.RageIncrement(nRage,&rHuman);
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact029_T::RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			INT nValue = GetShieldHp(rImp);
			nRate+=100;
			nValue = Float2Int((nValue*nRate)/1000.0f);
			if(0>nValue)
			{
				nValue=0;
			}
			SetShieldHp	(rImp, nValue);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}

	};
};

