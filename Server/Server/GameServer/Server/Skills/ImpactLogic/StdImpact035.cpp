#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact035.cpp
// 功能说明：效果--恐惧状态,不能施法
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact035.h"
#include "AI_Character.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact035_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			AI_Character* pAI = rMe.GetAIObj();
			if (pAI) {
				pAI->ChangeState(ESTATE_TERROR);
			}
			__LEAVE_FUNCTION			
		}

		VOID StdImpact035_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			AI_Character* pAI = rMe.GetAIObj();
			if (pAI) {
				pAI->ChangeState(ESTATE_IDLE);
			}
			__LEAVE_FUNCTION			
		}

		VOID StdImpact035_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			rMe.MarkCanAction1DirtyFlag();
			__LEAVE_FUNCTION
		}
		BOOL StdImpact035_T::GetBoolAttrRefix(OWN_IMPACT& rImp, Obj_Character& rMe, CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharBoolAttrs_T::ATTR_CAN_ACTION1==nIdx || CharBoolAttrs_T::ATTR_CAN_ACTION2==nIdx)
			{
				rBoolAttrRefix = FALSE;
				return TRUE;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};



