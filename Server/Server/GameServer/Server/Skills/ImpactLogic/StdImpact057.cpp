#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact057.cpp
// 功能说明：效果--宠物生命转换成主人的HP,MP
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact057.h"
#include "Scene.h"
#include "StdImpact004.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact057_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();

			if(Obj::OBJ_TYPE_PET!=rMe.GetObjType())
			{
				return;
			}
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			INT nDamageRate = GetDamageRate(rImp);
			INT nTransferRateForHp = GetTransferRateForHp(rImp);
			INT nTransferRateForMp = GetTransferRateForMp(rImp);
			INT nSubImpactForHeal = GetSubImpactHeal(rImp);
			
			INT nDamage = Float2Int(rMe.GetHP()*nDamageRate/100.0f);
			INT nTransferedHp = Float2Int(nDamage*nTransferRateForHp/100.0f);
			INT nTransferedMp = Float2Int(nDamage*nTransferRateForMp/100.0f);

			Obj* pObj = rMe.GetMyMaster();
			if(NULL!=pObj)
			{
				if(TRUE==IsCharacterObj(pObj->GetObjType()))
				{
					Obj_Character& rMaster = *static_cast<Obj_Character*>(pObj);
					rMe.HealthIncrement(-nDamage, &rMe);
					OWN_IMPACT impact;
					g_ImpactCore.InitImpactFromData(nSubImpactForHeal, impact);
					ModifyHpMpRageStrikePointByValue_T logic;
					if(0<nTransferRateForHp)
					{
						logic.SetHpModification(impact, nTransferedHp);
					}
					if(0<nTransferRateForMp)
					{
						logic.SetMpModification(impact, nTransferedMp);
					}
					pScene->GetEventCore().RegisterImpactEvent(rMaster.GetID(), rMe.GetID(), impact,0);
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

