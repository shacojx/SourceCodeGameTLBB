#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_Unbreakable.cpp
// 程序员：frankwu
// 功能说明：效果--无敌状态
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_Unbreakable.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID SOT_Unbreakable_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			__LEAVE_FUNCTION
		}
		VOID SOT_Unbreakable_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			rMe.MarkUnbreakableDirtyFlag();
			__LEAVE_FUNCTION			
		}
		BOOL SOT_Unbreakable_T::GetBoolAttrRefix(OWN_IMPACT & rImp, CharBoolAttrs_T::Index_T nIdx, BOOL & rBoolAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharBoolAttrs_T::ATTR_UNBREAKABLE==nIdx)
			{
				rBoolAttrRefix = TRUE;
				return TRUE;
			}
			__LEAVE_FUNCTION						
			return FALSE;
		}
		VOID SOT_Unbreakable_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const
		{
			__ENTER_FUNCTION
			INT nContinuance = GetContinuance(rImp);
			BOOL& rFadeOut = rImp.m_bFadeOut;
			if(FALSE == rFadeOut)
			{
				if(TRUE == IsOverTimed())
				{
					if(-1==nContinuance)
					{
						return; //-1为持续时间无限长
					}
					if(0<nContinuance)
					{
						nContinuance -= nDeltaTime;
					}
					if(0>=nContinuance)
					{
						// prepare delete this impact
						rFadeOut=TRUE;
						nContinuance = 0;
					}
					SetContinuance(rImp, nContinuance);
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL SOT_Unbreakable_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};



