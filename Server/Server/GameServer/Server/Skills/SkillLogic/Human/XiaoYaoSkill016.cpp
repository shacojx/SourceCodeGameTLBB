#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYaoSkill016.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "XiaoYaoSkill016.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "DI_Damages.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Combat_Module::Impact_Module;
		//class XiaoYaoSkill016_T : public Skilllogic_T
		BOOL XiaoYaoSkill016_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Obj* pObj = NULL;
			ObjID_t nObjID = 0;
			Obj_Special* pTrap = NULL;
			{
				OWN_IMPACT impact;
				DI_JustSpecialEffect_T logic;
				logic.InitStruct(impact);
				impact.m_nImpactID = rSkillInfo.GetDescriptorByIndex(0)->GetValue();
				
				//register impact event
				RegisterImpactEvent(rTar, rMe, impact, rParams.GetDelayTime(), bCriticalFlag);
			} while(0)
			//all traps continuance++
			{
				SOT_XiaoYaoTraps_T logic;
				OWN_IMPACT* pImp = rMe.Impact_GetFirstImpactOfSpecificLogicID(logic.ID);
				if(NULL==pImp)
				{
					return TRUE;
				}
				OWN_IMPACT& rImp = *pImp;
				Time_t nContinuance;
				for(INT nIdx=0;logic.IDX_TRAP_END>=nIdx;++nIdx)
				{
					nObjID = logic.GetTrapByIndex(rImp, nIdx);
					if(INVALID_ID!=nObjID)
					{
						pObj = rMe.GetSpecificObjInSameSceneByID(nObjID);
						if(NULL!=pObj)
						{
							if(TRUE==IsSpecialObj(pObj->GetObjType()))
							{
								pTrap = (Obj_Special*)pObj;
								if(FALSE==pTrap->IsFadeOut())
								{
									nContinuance = rSkillInfo.GetDescriptorByIndex(1)->GetValue();
									nContinuance += pTrap->GetContinuance();
									pTrap->SetContinuance(nContinuance);
								}
							}
						}
					}
				}
			}while(0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
