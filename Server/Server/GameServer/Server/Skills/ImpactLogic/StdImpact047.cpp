#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact047.cpp
// 功能说明：效果--大理技能12的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact047.h"
#include "GameTable.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact047_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			if(NULL==pAttacker)
			{
				return;
			}
			if(INVALID_ID==nSkillID)
			{
				return;
			}
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			if(NULL==pCollection)
			{
				return;
			}
			if(FALSE==pCollection->IsThisIDInCollection(nSkillID))
			{
				return;
			}
			Scene * pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			INT nRand = pScene->GetRand100();
			if(nRand>=GetActivateOdds(rImp))
			{
				return;
			}
			g_ImpactCore.SendImpactToUnit(rMe, GetSubImpactDataIndex(rImp), pAttacker->GetID(), 0);
			__LEAVE_FUNCTION
		}
	};
};

