#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact034.cpp
// 功能说明：效果--星宿技能恶贯满盈的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact034.h"
#include "Combat_Core.h"
#include "Scene.h"

using namespace Combat_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		VOID StdImpact034_T::OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Obj_Character* pTar=NULL;
			OBJLIST Targets;
			if(FALSE==ScanUnitForTarget(rImp, rMe, *(rMe.getWorldPos()), (FLOAT)GetScanRadius(rImp), GetEffectedObjCount(rImp), Targets))
			{
				return;
			}
			// init impact
			OWN_IMPACT impact;
			CombatCore_T myCombatCore;
			// impacts init finished
			for(INT nIdx=0; Targets.m_Count>nIdx;++nIdx)
			{
				pTar = (Obj_Character*)Targets.m_aObj[nIdx];
				if(NULL!=pTar)
				{
					myCombatCore.Reset();
					g_ImpactCore.InitImpactFromData(GetSubImpactDataIndex(rImp), impact);
					myCombatCore.GetResultImpact(rMe, *pTar, impact);

					Scene* pScene = rMe.getScene();
					if(NULL!=pScene)
					{
						pScene->GetEventCore().RegisterBeSkillEvent(pTar->GetID(), rMe.GetID(), BEHAVIOR_TYPE_HOSTILITY, 500);
						pScene->GetEventCore().RegisterImpactEvent(pTar->GetID(), rMe.GetID(), impact, 500);
					}
				}
			}
			return;
			__LEAVE_FUNCTION
		}
		BOOL StdImpact034_T::IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const
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
	};
};

