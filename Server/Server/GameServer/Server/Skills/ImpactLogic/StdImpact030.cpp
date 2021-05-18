#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact030.cpp
// 功能说明：效果--峨嵋技能易筋锻骨的护体效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact030.h"
#include "ScanOperator_AEImpact.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact030_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetShieldHp(rImp, GetShieldHpInTable(rImp));
			SetAbsorbRate(rImp, GetAbsorbRateInTable(rImp));			
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact030_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			INT nDamage = Float2Int((rDamage*GetAbsorbRate(rImp))/100.0f);
			INT nShieldHp = GetShieldHp(rImp);
			if(nDamage>nShieldHp)
			{
				nDamage = nShieldHp;
			}
			nShieldHp-=nDamage;
			SetShieldHp(rImp, nShieldHp);
			rDamage -=nDamage;
			if(0>=GetShieldHp(rImp))
			{
				rMe.Impact_OnImpactFadeOut(rImp);
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact030_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj_Character* pTar=NULL;
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				AssertEx(FALSE, "[StdImpact030_T::OnFadeOut]: Empty scene pointer found!!");
				return;
			}
			OBJLIST Targets;
			if(FALSE==ScanUnitForTarget(rImp, rMe, *(rMe.getWorldPos()), (FLOAT)GetScanRadius(rImp), GetEffectedObjCount(rImp), Targets))
			{
				return;
			}
			INT nImpact = GetSubImpactIndex(rImp);
			if(INVALID_ID==nImpact)
			{
				return;
			}
			// impacts init finished
			for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
			{
				pTar = (Obj_Character*)Targets.m_aObj[nIdx];
				if(NULL!=pTar)
				{
					pScene->GetEventCore().RegisterBeSkillEvent(pTar->GetID(), rMe.GetID(), BEHAVIOR_TYPE_HOSTILITY, 500);
					g_ImpactCore.SendImpactToUnit(*pTar, nImpact, rMe.GetID(), 500);
				}
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL StdImpact030_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
		{
			__ENTER_FUNCTION
			if(FALSE==rTar.IsAliveInDeed() || FALSE==rTar.IsActiveObj())
			{
				return FALSE;
			}
			if(rMe.IsEnemy(&rTar) || rTar.IsEnemy(&rMe))
			{
				return TRUE;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact030_T::RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			if(TRUE == Impact_IsImpactInCollection(rImpactNeedRefix, GetTargetImpactCollection(rImp)))
			{
				ImpactLogic_T const* pLogic = Impact_GetLogic(rImpactNeedRefix);
				if(NULL!=pLogic)
				{
					pLogic->RefixPowerByRate(rImpactNeedRefix, GetImpactRefixRate(rImp));
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact030_T::RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			INT nValue = GetShieldHp(rImp);
			nRate+=100;
			nValue = Float2Int((nValue*nRate)/100.0f);
			if(0>nValue)
			{
				nValue = 0;
			}
			SetShieldHp	(rImp, nValue);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

