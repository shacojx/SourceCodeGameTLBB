#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：MenPai.cpp
// 功能说明：门派:通用的接口实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "MenPai.h"
#include "GameTable.h"
namespace MenPai_Module
{
	SHORT MenPai_T::GetInitMaxHP(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_HP, GetID());
	}

	SHORT MenPai_T::GetMaxHPConRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_CON_HP, GetID());
	}

	SHORT MenPai_T::GetMaxHPLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_HP, GetID());		
	}

	//HP Regenerate
	SHORT MenPai_T::GetInitHPRegenerate(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_HPRESTORE, GetID());
	}

	SHORT MenPai_T::GetHPRegenerateConRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_CON_HPRESTORE, GetID());
	}

	SHORT MenPai_T::GetHPRegenerateLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_HPRESTORE, GetID());
	}

	//MaxMP
	SHORT MenPai_T::GetInitMaxMP(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_MP, GetID());
	}

	SHORT MenPai_T::GetMaxMPIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_MP, GetID());
	}

	SHORT MenPai_T::GetMaxMPLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_MP, GetID());
	}

	//MP Regenerate
	SHORT MenPai_T::GetInitMPRegenerate(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_MPRESTORE, GetID());
	}

	SHORT MenPai_T::GetMPRegenerateIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_MPRESTORE, GetID());
	}

	SHORT MenPai_T::GetMPRegenerateLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_MPRESTORE, GetID() ) ;
	}

	//Attr Level 1 Sstr
	SHORT MenPai_T::GetInitStr(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_STR,GetID(),0);
	}

	SHORT MenPai_T::GetStrLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_STR,GetID(),nLevel);
	}

	//Attr Level 1 Spr
	SHORT	MenPai_T::GetInitSpr(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_SPR,GetID(),0);
	}

	SHORT MenPai_T::GetSprLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_SPR,GetID(),nLevel);
	}

	//Attr Level 1 Con
	SHORT MenPai_T::GetInitCon(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_CON,GetID(),0);
	}

	SHORT MenPai_T::GetConLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_CON,GetID(),nLevel);
	}

	//Attr Level 1 Int
	SHORT MenPai_T::GetInitInt(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_INT,GetID(),0);
	}

	SHORT MenPai_T::GetIntLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_INT,GetID(),nLevel);
	}

	//Attr Level 1 Dex
	SHORT MenPai_T::GetInitDex(VOID) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_DEX,GetID(),0);
	}

	SHORT MenPai_T::GetDexLevelupRefix(SHORT const nLevel) const
	{
		return g_AttrLevelupTbl.Get(CATTR_LEVEL1_DEX,GetID(),nLevel);
	}

	//Attr Level 2 Attack Physics
	SHORT MenPai_T::GetInitAttackPhysics(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_P, GetID());
	}

	SHORT MenPai_T::GetAttackPhysicsStrRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_STR_ATTACK_P, GetID());
	}

	SHORT MenPai_T::GetAttackPhysicsLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_P, GetID());
	}

	//Attr Level 2 Defence Physics
	SHORT MenPai_T::GetInitDefencePhysics(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_P, GetID());
	}

	SHORT MenPai_T::GetDefencePhysicsConRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_CON_DEFENCE_P, GetID());
	}

	SHORT MenPai_T::GetDefencePhysicsLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_DEFENCE_P, GetID());
	}

	//Attr Level 2 Attack Magic
	SHORT MenPai_T::GetInitAttackMagic(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_ATTACK_M, GetID());
	}

	SHORT MenPai_T::GetAttackMagicSprRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_SPR_ATTACK_M, GetID());
	}

	SHORT MenPai_T::GetAttackMagicLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_ATTACK_M, GetID());
	}

	//Attr Level 2 Defence Magic
	SHORT MenPai_T::GetInitDefenceMagic(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_DEFENCE_M, GetID());
	}

	SHORT MenPai_T::GetDefenceMagicIntRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_INT_DEFENCE_M, GetID());
	}

	SHORT MenPai_T::GetDefenceMagicLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get( AINFOTYPE_LEVEL_DEFENCE_M,GetID());
	}

	//Attr Level 2 Hit
	SHORT MenPai_T::GetInitHit(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_HIT, GetID());
	}

	SHORT MenPai_T::GetHitDexRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_DEX_HIT, GetID());
	}

	SHORT MenPai_T::GetHitLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_HIT, GetID());
	}

	//Attr Level 2 Critical
	SHORT MenPai_T::GetInitCritical(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_CRITRATE, GetID());
	}

	SHORT MenPai_T::GetCriticalDexRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_DEX_CRITRATE, GetID());
	}

	SHORT MenPai_T::GetCriticalLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_CRITRATE, GetID());
	}

	//Attr Level 2 Miss
	SHORT MenPai_T::GetInitMiss(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_BASE_MISS, GetID());
	}

	SHORT MenPai_T::GetMissDexRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_DEX_MISS, GetID());
	}

	SHORT MenPai_T::GetMissLevelRefix(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_LEVEL_MISS, GetID());
	}
	//Attr Attack Fluctuation
	SHORT MenPai_T::GetAttackFluctuation(VOID) const
	{
		return g_BaseValueTbl.Get(AINFOTYPE_ATTACK_FLUCTUATION, GetID());
	}
	VOID MenPai_T::OnDamage(Obj_Human& rMe, INT nDamage) const
	{
		if(0>=nDamage)
		{
			return;
		}
		
		INT nRage = nDamage/40;
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage, NULL);
	}
	VOID MenPai_T::OnDamageTarget(Obj_Human& rMe,INT nDamage) const
	{
		if(0>=nDamage)
		{
			return;
		}
		
		INT nRage = nDamage/40;
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage, NULL);
	}

}

