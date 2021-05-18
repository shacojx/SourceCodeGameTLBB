#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact022.cpp
// 功能说明：宠物复仇技能统一效果
//
// 效果描述部分的格式：|效果ID|生效几率|伤害次数|返回伤害百分率|吸取伤害百分率|
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact022.h"
#include "StdImpact004.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact022_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateOdds(rImp, GetActivateOddsInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact022_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if (!pAttacker)
			{
				return;
			}
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			// 生效几率 
			INT nActivateOdds = GetActivateOdds(rImp);
			INT nRet = pScene->GetRand100();
			if (nRet > nActivateOdds)
			{// 没有生效则直接返回
				return;
			}
			// 多出几次伤害，但不发出招动作
            INT nDamageCount = GetDamageCount(rImp);
			for (INT i = 0; i < nDamageCount; ++i)
			{
				OWN_IMPACT impact;

				CombatCore_T myCombatCore;
				myCombatCore.Reset();			
				myCombatCore.GetResultImpact(rMe, *pAttacker, impact);

				pScene->GetEventCore().RegisterImpactEvent(pAttacker->GetID(), rMe.GetID(), impact, 500);
			}
			// 返回伤害百分率
			INT nReflectRate = GetReflectRate(rImp);
			if(0<nReflectRate)
			{
				OWN_IMPACT impact;
				INT nReflectDamage = (rDamage * nReflectRate) / 100;
				g_ImpactCore.InitImpactFromData(IMP_NOTYPE_DAMAGE, impact);
				ModifyHpMpRageStrikePointByValue_T logic;
				if(logic.ID != Impact_GetLogicID(impact))
				{
					AssertEx(FALSE, "[StdImpact022_T::OnDamage]: Error Data found!");
					return;	
				}
				logic.SetHpModification(impact, -nReflectDamage);
				pScene->GetEventCore().RegisterImpactEvent(pAttacker->GetID(), rMe.GetID(), impact, 500);
			}
		
			__LEAVE_FUNCTION
		}

		VOID StdImpact022_T::OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			// 吸取伤害百分率
			INT nAbsorbRate = GetAbsorbRate(rImp);
			INT nAbsorbHP = (rDamage * nAbsorbRate) / 100;
			// 当目标受到伤害时将伤害的一定比率转换成自己的HP			
			rMe.HealthIncrement(nAbsorbHP, &rMe);
			__LEAVE_FUNCTION
		}
	};
};

