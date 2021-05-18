#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact026.cpp
// 功能说明：效果--伤害盾，吸收、免疫、反弹
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact026.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact026_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetShieldHp(rImp, GetShieldHpInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact026_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			INT nDamage = 0;
			if(0!=GetAbsorbRate(rImp))
			{
				nDamage = Float2Int((rDamage*GetAbsorbRate(rImp))/100.0f);
				INT nShieldHp = GetShieldHp(rImp);
				if(nDamage>nShieldHp)
				{
					nDamage=nShieldHp;
				}
				nShieldHp -= nDamage;
				SetShieldHp(rImp, nShieldHp);
				rDamage -= nDamage;
				if(0>=nShieldHp)
				{
					rMe.Impact_OnImpactFadeOut(rImp);
				}
			}
			else if(0!=GetIgnoreRate(rImp))
			{
				nDamage = Float2Int((rDamage*GetIgnoreRate(rImp))/100.0f);
				rDamage -= nDamage;
			}
			else if(0!=GetReflectRate(rImp))
			{
				INT nRand = 0;
				Scene* pScene = rMe.getScene();
				if(NULL!=pScene)
				{
					nRand = pScene->GetRand100();
				}
				if(nRand<GetReflectActivateOdds(rImp))
				{
					nDamage = Float2Int((rDamage*GetReflectRate(rImp))/100.0f);
					if(NULL!=pAttacker)
					{
						pAttacker->HealthIncrement(-nDamage, &rMe);
					}
				}
			}			
			__LEAVE_FUNCTION
		}
	};
};

