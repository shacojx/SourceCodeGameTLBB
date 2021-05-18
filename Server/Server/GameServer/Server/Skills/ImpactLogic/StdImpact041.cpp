#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact041.cpp
// 功能说明：效果--明教的五星连珠的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact041.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact041_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetAttackPhysicsRefix(rImp))
			{
				rMe.MarkAttackPhysicsRefixDirtyFlag();
			}
			if(0!=GetDefencePhysicsRefix(rImp))
			{
				rMe.MarkDefencePhysicsRefixDirtyFlag();
			}
			if(0!=GetAttackMagicRefix(rImp))
			{
				rMe.MarkAttackMagicRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicRefix(rImp))
			{
				rMe.MarkDefenceMagicRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact041_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration *= GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION
		}
		BOOL StdImpact041_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_ATTACK_PHY:
					if(0!=GetAttackPhysicsRefix(rImp))
					{
						rIntAttrRefix += GetAttackPhysicsRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_PHY:
					if(0!=GetDefencePhysicsRefix(rImp))
					{
						rIntAttrRefix += GetDefencePhysicsRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC:
					if(0!=GetAttackMagicRefix(rImp))
					{
						rIntAttrRefix += GetAttackMagicRefix(rImp);
						return TRUE;
					}

					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC:
					if(0!=GetDefenceMagicRefix(rImp))
					{
						rIntAttrRefix += GetDefenceMagicRefix(rImp);
						return TRUE;
					}
					break;
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

