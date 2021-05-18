#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact050.cpp
// 功能说明：效果--逍遥怒气增长修正
//			
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact050.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact050_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration *= GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION			
		}		
	};
};


