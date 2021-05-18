#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact012.cpp
// 功能说明：效果--State: refix all attack, Defence and MaxHP, MaxMP, MaxRage, MaxStrikePoint
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact012.h"

namespace Combat_Module
{

	namespace Impact_Module
	{
		BOOL StdImpact012_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact012_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION

			// Attacks
			if(0!=GetAttackPhysicsRefix(rImp))
			{
				rMe.MarkAttackPhysicsRefixDirtyFlag();
			}
			if(0!=GetAttackMagicRefix(rImp))
			{
				rMe.MarkAttackMagicRefixDirtyFlag();
			}
			if(0!=GetAttackColdRefix(rImp))
			{
				rMe.MarkAttackColdRefixDirtyFlag();
			}
			if(0!=GetAttackFireRefix(rImp))
			{
				rMe.MarkAttackFireRefixDirtyFlag();
			}
			if(0!=GetAttackLightRefix(rImp))
			{
				rMe.MarkAttackLightRefixDirtyFlag();
			}
			if(0!=GetAttackPoisonRefix(rImp))
			{
				rMe.MarkAttackPoisonRefixDirtyFlag();
			}
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
			// maxs
			if(0!=GetMaxHpRefix(rImp))
			{
				rMe.MarkMaxHPRefixDirtyFlag();
			}
			if(0!=GetMaxMpRefix(rImp))
			{
				rMe.MarkMaxMPRefixDirtyFlag();
			}
			if(0!=GetMaxRageRefix(rImp))
			{
				rMe.MarkMaxRageRefixDirtyFlag();
			}
			if(0!=GetMaxStrikePointRefix(rImp))
			{
				rMe.MarkMaxStrikePointRefixDirtyFlag();
			}
			// end
			__LEAVE_FUNCTION
		}
		BOOL StdImpact012_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			INT nRefixRate = GetRefixRate(rImp);
			nRefixRate += 100;
			INT nValue = 0;
			switch (nIdx)
			{
				//Attacks
				case CharIntAttrRefixs_T::REFIX_ATTACK_PHY:
					{
						if(0!=GetAttackPhysicsRefix(rImp))
						{
							nValue = Float2Int((GetAttackPhysicsRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackPhysics()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_MAGIC:
					{
						if(0!=GetAttackMagicRefix(rImp))
						{
							nValue = Float2Int((GetAttackMagicRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackMagic()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_COLD:
					{
						if(0!=GetAttackColdRefix(rImp))
						{
							nValue = Float2Int((GetAttackColdRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackCold()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_FIRE:
					{
						if(0!=GetAttackFireRefix(rImp))
						{
							nValue = Float2Int((GetAttackFireRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackFire()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_LIGHT:
					{
						if(0!=GetAttackLightRefix(rImp))
						{
							nValue = Float2Int((GetAttackLightRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackLight()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_ATTACK_POISON:
					{
						if(0!=GetAttackPoisonRefix(rImp))
						{
							nValue = Float2Int((GetAttackPoisonRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseAttackPoison()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				//Defences
				case CharIntAttrRefixs_T::REFIX_DEFENCE_PHY:
					{
						if(0!=GetDefencePhysicsRefix(rImp))
						{
							nValue = Float2Int((GetDefencePhysicsRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseDefencePhysics()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefenceMagic()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefenceCold()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefenceFire()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefenceLight()*nValue)/100.0f);
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
							nValue = Float2Int((rMe.GetBaseDefencePoison()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;

				// Maxes
				case CharIntAttrRefixs_T::REFIX_MAX_HP:
					{
						if(0!=GetMaxHpRefix(rImp))
						{
							nValue = Float2Int((GetMaxHpRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseMaxHP()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MAX_MP:
					{
						if(0!=GetMaxMpRefix(rImp))
						{
							nValue = Float2Int((GetMaxMpRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseMaxMP()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MAX_RAGE:
					{
						if(0!=GetMaxRageRefix(rImp))
						{
							nValue = Float2Int((GetMaxRageRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseMaxRage()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MAX_STRIKE_POINT:
					{
						if(0!=GetMaxStrikePointRefix(rImp))
						{
							nValue = Float2Int((GetMaxStrikePointRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseMaxStrikePoint()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact012_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

