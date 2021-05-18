#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：SOT_DaLi007.cpp
// 程序员：高骐
// 功能说明：效果--大理技能007的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "SOT_Dali007.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "Scene.h"
#include "GameTable.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID SOT_DaLi007_T::InitStruct(OWN_IMPACT & rImp) const
		{
			__ENTER_FUNCTION
			this->ImpactLogic_T::InitStruct(rImp);
			SetLogicID(rImp,ID);
			SetContinuance(rImp, 0);
			SetEffectedSkillCollection(rImp, -1);
			SetAttackRefixRate(rImp, 0);
			SetRageConvertRate(rImp, 0);
			__LEAVE_FUNCTION
		}
		VOID SOT_DaLi007_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==IsFadeOut(rImp))
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
			if(pAttacker->GetID()!=rImp.m_nCasterID)
			{
				return;
			}
			pDamageList[DAMAGE_PHYSICS] = pDamageList[DAMAGE_PHYSICS] + (pDamageList[DAMAGE_PHYSICS]* GetAttackRefixRate(rImp)+50)/100;
			INT nDamage = pDamageList[DAMAGE_PHYSICS]
						+ pDamageList[DAMAGE_MAGIC]
						+ pDamageList[DAMAGE_COLD]
						+ pDamageList[DAMAGE_FIRE]
						+ pDamageList[DAMAGE_LIGHT]
						+ pDamageList[DAMAGE_POISON];
			INT nRage = nDamage * GetRageConvertRate(rImp)/100;
			pAttacker->RageIncrement(nRage, pAttacker);
			__LEAVE_FUNCTION
		}
		VOID SOT_DaLi007_T::ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const 
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
		BOOL SOT_DaLi007_T::RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			SetContinuance(rImp, GetContinuance(rImp)*nRate/100);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

