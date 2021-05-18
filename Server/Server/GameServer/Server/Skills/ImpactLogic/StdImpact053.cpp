#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact053.cpp
// 功能说明：效果--单独通知客户端显示特效用的
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "GameTable.h"
#include "StdImpact053.h"
#include "Obj_Human.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		VOID StdImpact053_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				Obj_Human& rHuman=(Obj_Human&)rMe;				
				const _SKILL_LIST& rSkillList = rHuman.Skill_GetSkillList();
				INT const nSkillCount = rSkillList.m_Count;
				for(INT nIndex=0;nSkillCount>nIndex;++nIndex)
				{
					SkillID_t nSkillID = rSkillList.m_aSkill[nIndex].m_nSkillID;
					if(INVALID_ID==nSkillID)
					{
						continue;
					}
					SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(nSkillID);
					if(NULL==pSkillTemplate)
					{
						continue;
					}
					CooldownID_t nCooldownID = pSkillTemplate->GetCooldownID();
					if(0<=nCooldownID)
					{
						if(nCooldownID!=GetCooldownExclude(rImp) && FALSE==rMe.IsCooldowned(nCooldownID))
						{
							rMe.SetCooldown(nCooldownID, 0);
						}
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};
