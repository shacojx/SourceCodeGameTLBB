#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact046.cpp
// 功能说明：效果--大理技能007的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact046.h"
#include "GameTable.h"
namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact046_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
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
			if(pAttacker->GetUniqueID()!=rImp.GetCasterUniqueID())
			{
				return;
			}
			INT nRefixRate = GetPowerRefixRate(rImp);
			pDamageList[DAMAGE_PHYSICS] = pDamageList[DAMAGE_PHYSICS] + Float2Int((pDamageList[DAMAGE_PHYSICS]* nRefixRate)/100.0f);
			INT nDamage = pDamageList[DAMAGE_PHYSICS]
						+ pDamageList[DAMAGE_MAGIC]
						+ pDamageList[DAMAGE_COLD]
						+ pDamageList[DAMAGE_FIRE]
						+ pDamageList[DAMAGE_LIGHT]
						+ pDamageList[DAMAGE_POISON];
			INT nRage = Float2Int((nDamage * GetRageConvertRate(rImp))/100.0f);
			pAttacker->RageIncrement(nRage, pAttacker);
			__LEAVE_FUNCTION
		}
	};
};

