#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact056.cpp
// 功能说明：效果--在一定时间内修正、命中、闪避、生命上限、物理和魔法攻防
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact056.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact056_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact056_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetHitRefix(rImp))
			{
				rMe.MarkHitRefixDirtyFlag();
			}
			if(0!=GetMissRefix(rImp))
			{
				rMe.MarkMissRefixDirtyFlag();
			}
			if(0!=GetCriticalRefix(rImp))
			{
				rMe.MarkCriticalRefixDirtyFlag();
			}
			if(0!=GetMaxHpRefix(rImp))
			{
				rMe.MarkMaxHPRefixDirtyFlag();
			}
			if(0!=GetAttackPhysicsRefix(rImp))
			{
				rMe.MarkAttackPhysicsRefixDirtyFlag();
			}
			if(0!=GetDefencePhysicsRefix(rImp))
			{
				rMe.MarkDefencePhysicsRefixDirtyFlag();
			}
			if(0!=GetAttackMagicRefix(rImp))
			{
				rMe.MarkAttackMagicRefixDirtyFlag();
			}
			if(0!=GetDefenceMagicRefix(rImp))
			{
				rMe.MarkDefenceMagicRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact056_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			INT nRefixRate = GetRefixRate(rImp);
			nRefixRate += 100;
			INT nValue = 0;
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_HIT:
					{
						if(0!=GetHitRefix(rImp))
						{
							nValue = Float2Int((GetHitRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MISS:
					{
						if(0!=GetMissRefix(rImp))
						{
							nValue = Float2Int((GetMissRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_CRITICAL:
					{
						if(0!=GetCriticalRefix(rImp))
						{
							nValue = Float2Int((GetCriticalRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;				
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
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact056_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

