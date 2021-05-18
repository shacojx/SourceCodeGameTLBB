#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact027.cpp
// 功能说明：受击时将自己的一部分伤害转移到自己的宠物身上
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact027.h"
#include "Obj_Pet.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact027_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			
			if (rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN)
			{
				Obj_Pet* pPet = ((Obj_Human*)&rMe)->GetPet();
				if (NULL!=pPet)
				{
					INT nTransferRate = GetTransferRate(rImp);
					INT nTransferedDamage = Float2Int((rDamage * nTransferRate)/100.0f);
					if(TRUE==pPet->IsAlive() && TRUE==pPet->IsActiveObj())
					{
						pPet->HealthIncrement(-nTransferedDamage, pAttacker);
						rDamage -= nTransferedDamage;
						return;
					}
				}
			}
			rMe.Impact_OnImpactFadeOut(rImp);
			__LEAVE_FUNCTION
		}
	};
};

