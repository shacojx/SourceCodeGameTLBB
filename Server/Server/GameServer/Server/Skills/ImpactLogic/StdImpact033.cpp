#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact033.cpp
// 功能说明：效果--策反
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact033.h"
#include "Obj_Monster.h"
#include "AI_Monster.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact033_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetAiTypeRefix(rImp, GetAiTypeRefixInTable(rImp));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact033_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			INT nValue = 0;
			if(Obj::OBJ_TYPE_MONSTER == rMe.GetObjType())
			{
				Obj_Monster& rMonster = (Obj_Monster&)rMe;
				INT nAI_Type = GetAiTypeRefix(rImp);
				SetAiTypeRefix(rImp, rMonster.GetAIType());
				rMonster.SetAIType(nAI_Type);
				AI_Monster * pAI = rMonster.GetMonsterAI();
				if(NULL!=pAI)
				{
					pAI->DelEnemy(rImp.GetCasterObjID());
				}
			}
		}
		VOID StdImpact033_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			INT nValue = 0;
			if(Obj::OBJ_TYPE_MONSTER == rMe.GetObjType())
			{
				Obj_Monster& rMonster = (Obj_Monster&)rMe;
				INT nAI_Type = GetAiTypeRefix(rImp);
				rMonster.SetAIType(nAI_Type);
				AI_Monster * pAI = rMonster.GetMonsterAI();
				if(NULL!=pAI)
				{
					Obj* pObj = Impact_GetCaster(rMe, rImp);
					if(NULL!=pObj)
					{					
						pAI->AddPrimaryEnemy(pObj->GetID());
					}
				}
			}
		}
		
		VOID StdImpact033_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			//rMe.MarkCampIDRefixDirtyFlag();
			__LEAVE_FUNCTION
		}
		BOOL StdImpact033_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharIntAttrRefixs_T::REFIX_CAMP==nIdx)
			{
				Obj* pObj = Impact_GetCaster(rMe, rImp);
				if(NULL!=pObj)
				{
					if(TRUE==IsCharacterObj(pObj->GetObjType()))
					{
						Obj_Character& rOwner = *(Obj_Character*)pObj;
						rIntAttrRefix = rOwner.GetCampID();
						return TRUE;
					}
				}
				//主人已不在, buff消失
				rMe.Impact_OnImpactFadeOut(rImp);
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

