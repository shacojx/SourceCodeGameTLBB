#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact048.cpp
// 功能说明：效果--在一定时间内，增加效果所有者最大连击点数
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact048.h"
#include "Combat_Core.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact048_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj* pObj=Impact_GetCaster(rMe, rImp);
			if(NULL!=pObj)
			{
				if(IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rAttacker = (Obj_Character&)*pObj;
					OWN_IMPACT impact;
					CombatCore_T myCombatCore;
					myCombatCore.Reset();
					g_ImpactCore.InitImpactFromData(GetSubImpact(rImp), impact);
					myCombatCore.GetResultImpact(rAttacker, rMe, impact);
					Scene* pScene = rMe.getScene();
					if(NULL!=pScene)
					{
						pScene->GetEventCore().RegisterImpactEvent(rMe.GetID(), rAttacker.GetID(), impact, 500);
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

