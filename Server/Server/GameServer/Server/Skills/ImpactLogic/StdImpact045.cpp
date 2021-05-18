#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact045.cpp
// 功能说明：效果--在一段时间里爆击率增加，同时受到伤害将增加怒气
//			少林技能铁布衫用的
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact045.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact045_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			if(0!=GetRageRegenerationAdditional(rImp))
			{
				rRageRegeneration += GetRageRegenerationAdditional(rImp);
			}
			__LEAVE_FUNCTION
		}
	};
};


