#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact008.cpp
// 功能说明：效果--强制怪物改变攻击目标，这对非玩家有效
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "StdImpact008.h"
#include "Obj_Monster.h"
#include "AI_Monster.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact008_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			ObjID_t nTargetID = rImp.GetCasterObjID();
			if(Obj::OBJ_TYPE_MONSTER==rMe.GetObjType()&& TRUE==rMe.IsAlive())
			{
				Obj_Monster& rMonster = (Obj_Monster&)rMe;
				AI_Monster* pAI = (AI_Monster*)(rMonster.GetMonsterAI());
				if(NULL!=pAI)
				{
					if(TRUE==GetTargetMeFlag(rImp))
					{
						// Set impact caster as the primary target
						pAI->AddPrimaryEnemy(nTargetID);
					}
					else
					{
						// Set impact caster as the secondary target
						pAI->ChangePrimaryEnemy();
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

