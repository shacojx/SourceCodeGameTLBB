#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYaoSkill002.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "XiaoYaoSkill002.h"
#include "TrapLogic.h"
#include "Obj_Special.h"
#include "Skill_Core.h"
#include "StdImpact052.h"

using namespace Combat_Module;
using namespace Combat_Module::Impact_Module;
using namespace Combat_Module::Special_Obj_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		//class XiaoYaoSkill002_T : public SkillLogic_T
		BOOL XiaoYaoSkill002_T::EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag) const
		{
			__ENTER_FUNCTION
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			Obj* pObj = NULL;
			ObjID_t nObjID = 0;
			Obj_Special* pTrap = NULL;
			SpecialObjLogic_T const* pObjLogic = NULL;
			{
				OWN_IMPACT impact;
				INT nDataIndex = Skill_GetSkillInfoDescriptorValueByIndex(rSkillInfo, 0);
				if(INVALID_ID != nDataIndex)
				{
					g_ImpactCore.SendImpactToUnit(rMe, nDataIndex, rMe.GetID(), rParams.GetDelayTime(), bCriticalFlag);
				}
			} while(0);
			//Activate all traps
			{
				SOT_XiaoYaoTraps_T logic;
				OWN_IMPACT* pImp = rMe.Impact_GetFirstImpactOfSpecificLogicID(logic.ID);
				if(NULL==pImp)
				{
					return TRUE;
				}
				OWN_IMPACT& rImp = *pImp;
				for(INT nIdx=0;logic.MAX_TRAP_COUNT>=nIdx;++nIdx)
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
								pObjLogic = ObjSpecial_GetLogic(*pTrap);
								if(NULL!=pObjLogic)
								{
									if(TRAP_OBJ==pObjLogic->GetType())
									{
										TrapLogic_T const* pTrapLogic = (TrapLogic_T const*)pObjLogic;
										//Activate this trap
										pTrapLogic->ForceActivate(*pTrap);
									}
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
