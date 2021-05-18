#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact049.cpp
// 功能说明：效果--逍遥技能11的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact049.h"
#include "Scene.h"
#include "GameTable.h"

namespace Combat_Module
{
	
	namespace Impact_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Skill_Module;

		VOID StdImpact049_T::OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			IDCollection_T const* pCollection = g_IDCollectionMgr.GetInstanceByID(GetEffectedSkillCollection(rImp));
			if(NULL!=pCollection)
			{
				if(TRUE==pCollection->IsThisIDInCollection(nSkillID))
				{
					Scene* pScene = rMe.getScene();
					if(NULL!=pScene)
					{
						INT nRand = pScene->GetRand100();
						if(nRand<=GetActivateOdds(rImp))
						{
							g_ImpactCore.SendImpactToUnit(rTar, GetSubImpact(rImp), rMe.GetID(), 0);
						}
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

