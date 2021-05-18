#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact043.cpp
// 功能说明：效果--击中目标时增加自己的怒气，但在这一段时间内防御力降低
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact043.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact043_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetResistColdRefix(rImp))
			{
				rMe.MarkResistColdRefixDirtyFlag();
			}
			if(0!=GetResistFireRefix(rImp))
			{
				rMe.MarkResistFireRefixDirtyFlag();
			}
			if(0!=GetResistLightRefix(rImp))
			{
				rMe.MarkResistLightRefixDirtyFlag();
			}
			if(0!=GetResistPoisonRefix(rImp))
			{
				rMe.MarkResistPoisonRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact043_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration += GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION
		}
		BOOL StdImpact043_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_RESIST_COLD:
					if(0!=GetResistColdRefix(rImp))
					{
						rIntAttrRefix += GetResistColdRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_FIRE:
					if(0!=GetResistFireRefix(rImp))
					{
						rIntAttrRefix += GetResistFireRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_LIGHT:
					if(0!=GetResistLightRefix(rImp))
					{
						rIntAttrRefix += GetResistLightRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_POISON:
					if(0!=GetResistPoisonRefix(rImp))
					{
						rIntAttrRefix += GetResistPoisonRefix(rImp);
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
