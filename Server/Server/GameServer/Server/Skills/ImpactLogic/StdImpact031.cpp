#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact031.cpp
// 功能说明：效果--武当的魔法盾:虎抱头
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact001.h"
#include "StdImpact003.h"
#include "StdImpact031.h"
#include "Scene.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact031_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetCollectedDamage(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact031_T::OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			Scene* pScene = rMe.getScene();
			INT nCollectedDamage = 0;
			if(NULL!=pScene)
			{
				INT nRand = pScene->GetRand100();
				if(nRand > GetAbsorbOdds(rImp))
				{
					nCollectedDamage = Float2Int((rDamage*GetReflectRate(rImp))/100.0f);
					rDamage = 0;
					nCollectedDamage += GetCollectedDamage(rImp);
					SetCollectedDamage(rImp, nCollectedDamage);
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact031_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			//Defences
			INT nRefixRate = 0;
			nRefixRate += 100;
			INT nValue = 0;
			switch(nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_DEFENCE_PHY:
					{
						if(0!=GetDefencePhysicsRefix(rImp))
						{
							nValue = Float2Int((GetDefencePhysicsRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DEFENCE_MAGIC:
					{
						if(0!=GetDefenceMagicRefix(rImp))
						{
							nValue = Float2Int((GetDefenceMagicRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_COLD:
					{
						if(0!=GetResistColdRefix(rImp))
						{
							nValue = Float2Int((GetResistColdRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_FIRE:
					{
						if(0!=GetResistFireRefix(rImp))
						{
							nValue = Float2Int((GetResistFireRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_LIGHT:
					{
						if(0!=GetResistLightRefix(rImp))
						{
							nValue = Float2Int((GetResistLightRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_POISON:
					{
						if(0!=GetResistPoisonRefix(rImp))
						{
							nValue = Float2Int((GetResistPoisonRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				default:
					break;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact031_T::MarkModifiedAttrDirty(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			// defences
			if(0!=GetDefencePhysicsRefix(rImp))
			{
				rMe.MarkDefencePhysicsRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicRefix(rImp))
			{
				rMe.MarkDefenceMagicRefixDirtyFlag();
			}
			if(0!=GetResistColdRefix(rImp))
			{
				rMe.MarkResistColdRefixDirtyFlag();
			}
			if(0!=GetResistFireRefix(rImp))
			{
				rMe.MarkResistFireRefixDirtyFlag();
			}
			if(0!=GetResistLightRefix(rImp))
			{
				rMe.MarkResistLightRefixDirtyFlag();
			}
			if(0!=GetResistPoisonRefix(rImp))
			{
				rMe.MarkResistPoisonRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact031_T::RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const
		{
			__ENTER_FUNCTION
			INT nDamage = 0;
			switch(Impact_GetLogicID(rImpactNeedRefix))
			{
				case DI_DamagesByValue_T::ID:
					{
						DI_DamagesByValue_T Logic;
						nDamage = Logic.GetDamageMagic(rImpactNeedRefix);
						nDamage += GetCollectedDamage(rImp);
						SetCollectedDamage(rImp, 0);
						Logic.SetDamageMagic(rImpactNeedRefix, nDamage);
					}
					break;
				case DI_DamageByValue_T::ID:
					{
						DI_DamageByValue_T Logic;
						nDamage = Logic.GetDamage(rImpactNeedRefix);
						nDamage += GetCollectedDamage(rImp);
						SetCollectedDamage(rImp, 0);
						Logic.SetDamage(rImpactNeedRefix, nDamage);
					}
					break;
				default:
					break;
			};
			__LEAVE_FUNCTION
		}
	};
};

