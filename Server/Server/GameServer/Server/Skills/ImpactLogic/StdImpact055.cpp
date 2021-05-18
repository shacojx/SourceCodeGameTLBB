#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact055.cpp
// 功能说明：效果--在一定时间内，效果持有者死亡后可以直接复活
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact055.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact055_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			INT nCollectionID0 = GetTransferCollection0(rImp);
			INT nCollectionID1 = GetTransferCollection1(rImp);
			INT nCount = GetTransferImpactCount(rImp);
			INT nTransfered = 0;
			OWN_IMPACT impact;
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			Obj* pObj = rMe.GetMyShadowGuard();
			if(NULL==pObj)
			{
				return;
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				return;
			}
			Obj_Character* pGuard = static_cast<Obj_Character*>(pObj);

			OWN_IMPACT* pTargetImp = NULL;
			do
			{
				if(NULL==pTargetImp)
				{
					if(INVALID_ID != nCollectionID0)
					{
						pTargetImp=rMe.Impact_GetFirstImpactInSpecificCollection(nCollectionID0);
					}
				}
				if(NULL==pTargetImp)
				{
					if(INVALID_ID != nCollectionID1)
					{
						pTargetImp=rMe.Impact_GetFirstImpactInSpecificCollection(nCollectionID1);					
					}
				}
				if(NULL!=pTargetImp)
				{
					impact = *pTargetImp;
					pScene->GetEventCore().RegisterImpactEvent(pGuard->GetID(), impact.GetCasterObjID(), impact, 0);
					rMe.Impact_OnImpactFadeOut(*pTargetImp);
					++nTransfered;
				}
			} while (NULL!=pTargetImp && nTransfered<nCount);
			__LEAVE_FUNCTION
		}
	};
};

