#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact023.cpp
// 功能说明：宠物防御性DS
//
// 效果描述部分的格式：|效果ID|生效几率|受击时有效|命中时有效|受击时伤害修正
//					   |命中时吸取敌人MP百分率|命中时吸取敌人怒气百分率|
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact023.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact023_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateOdds(rImp, GetActivateOddsInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact023_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(0>=GetDamageIgnoreRate(rImp))
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
			// 对伤害进行修正
			rDamage = Float2Int((rDamage*GetDamageIgnoreRate(rImp))/100.0f);
			__LEAVE_FUNCTION
		}

		VOID StdImpact023_T::OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			INT nMpDamageRate = GetMpDamageRate(rImp);
			INT nRageDamageRate = GetRageDamageRate(rImp);
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(0>=nMpDamageRate && 0>= nRageDamageRate)
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
			// 命中时吸取敌人MP百分率
			if(0<nMpDamageRate)
			{
				INT nMP = Float2Int((nMpDamageRate*rDamage)/100.0f);
				rTar.ManaIncrement(-nMP, &rMe);
			}
			// 命中时吸取敌人怒气百分率
			if(0<nRageDamageRate)
			{
				INT nRage = rTar.GetRage();
				nRage = Float2Int((nRageDamageRate*nRage)/100.0f);
				rTar.RageIncrement(-nRage, &rMe); 
			}
			__LEAVE_FUNCTION
		}
	};
};

