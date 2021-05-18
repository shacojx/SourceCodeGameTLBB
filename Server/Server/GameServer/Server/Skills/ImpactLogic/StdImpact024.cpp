#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact024.cpp
// 功能说明：效果--过滤不良效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact024.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact024_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetActivateOdds(rImp, GetActivateOddsInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		INT StdImpact024_T::OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return FALSE;
			}
			// 生效几率 
			INT nRet = rand() % 100;
			if (nRet > GetActivateOdds(rImp))
			{// 没有生效则直接返回
				return FALSE;
			}
			for(INT nIndex=0; COLLECTION_NUMBERS>nIndex; ++nIndex)
			{
				if(TRUE==Impact_IsImpactInCollection(rImpactNeedCheck, GetImmunoImpactsByIndex(rImp, nIndex)))
				{
					return MissFlag_T::FLAG_ABSORB;
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};



