#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：Combat_Core.cpp
// 功能说明：战斗逻辑内核
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/**
*
*/
#include "Type.h"
#include "Obj_Character.h"
#include "Combat_Core.h"
#include "SkillInfo.h"
#include "SkillLogics.h"
#include "Skill_Core.h"
#include "LogicManager.h"
#include "Scene.h"
#include "StdImpact002.h"
#include "Impact_Core.h"
#include "StdImpact003.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;
using namespace MenPai_Module;

namespace Combat_Module
{
	static DI_DamagesByValue_T	logicDamages;
	enum
	{
		IMPACT_DATA_INDEX_OF_DEFAULT_DAMAGES = 0,
	};
	//class CombatCore_T
	VOID CombatCore_T::Reset()
	{
		m_nAdditionalAttackPhysics = 0;
		m_nAdditionalAttackMagic = 0;
		m_nAdditionalAttackCold = 0;
		m_nAdditionalAttackFire = 0;
		m_nAdditionalAttackLight = 0;
		m_nAdditionalAttackPoison = 0;
		m_nAdditionalDefencePhysics = 0;
		m_nAdditionalDefenceMagic = 0;
		m_nAdditionalDefenceCold = 0;
		m_nAdditionalDefenceFire = 0;
		m_nAdditionalDefenceLight = 0;
		m_nAdditionalDefencePoison = 0;
	}
	BOOL CombatCore_T::GetResultImpact(Obj_Character& rAttacker, Obj_Character& rDefencer, OWN_IMPACT& rImp)
	{
		__ENTER_FUNCTION
		INT nDamage = 0;
		INT nMaxDamage = 0;
		INT nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_PHYSICAL;
		//Init
		DI_DamagesByValue_T logic;
		if(INVALID_ID==rImp.GetDataIndex())
		{
			g_ImpactCore.InitImpactFromData(IMP_DAMAGES_OF_ATTACKS, rImp);
		}
		else if(logic.ID != Impact_GetLogicID(rImp))
		{
			g_ImpactCore.InitImpactFromData(IMP_DAMAGES_OF_ATTACKS, rImp);
		}
		m_nAdditionalAttackPhysics	+= logic.GetDamagePhy(rImp);
		m_nAdditionalAttackMagic	+= logic.GetDamageMagic(rImp);
		m_nAdditionalAttackCold		+= logic.GetDamageCold(rImp);
		m_nAdditionalAttackFire		+= logic.GetDamageFire(rImp);
		m_nAdditionalAttackLight	+= logic.GetDamageLight(rImp);
		m_nAdditionalAttackPoison	+= logic.GetDamagePoison(rImp);

		//Damage fluctuation
		INT nFluctuation = g_Config.m_ConfigInfo.m_nDefaultDamageFluctuation;
		
		MenPaiID_t nMenPai = 9;
		MenPai_Module::MenPai_T const* pMenPai = NULL;
		if(Obj::OBJ_TYPE_HUMAN==rAttacker.GetObjType())
		{
			Obj_Human& rHuman = (Obj_Human&)rAttacker;
			nMenPai = rHuman.GetMenPai();
			pMenPai = g_MenPaiLogicList.GetLogicById(nMenPai);
			if(NULL!=pMenPai)
			{
				nFluctuation=pMenPai->GetAttackFluctuation();
			}
		}
		Scene* pScene = rAttacker.getScene();
		// calculate the fluctuation
		INT nRand=50;
		if(NULL!=pScene)
		{
			nRand -= pScene->GetRand100();
		}
		nFluctuation = Float2Int((2*nFluctuation*nRand)/100.0f);
		
		//Calculate Damages
		//Phy
		nDamage = PhysicalDamage(rAttacker, rDefencer, m_nAdditionalAttackPhysics, m_nAdditionalDefencePhysics);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamagePhy(rImp, nDamage);
		nMaxDamage = nDamage;
		nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_PHYSICAL;
		//Magic
		nDamage = MagicalDamage(rAttacker, rDefencer, m_nAdditionalAttackMagic, m_nAdditionalDefenceMagic);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageMagic(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_MAGIC;
		}
		//Cold
		nDamage = ColdDamage(rAttacker, rDefencer, m_nAdditionalAttackCold, m_nAdditionalDefenceCold);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageCold(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_COLD;
		}
		//Fire
		nDamage = FireDamage(rAttacker, rDefencer, m_nAdditionalAttackFire, m_nAdditionalDefenceFire);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageFire(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_FIRE;
		}
		//Light
		nDamage = LightDamage(rAttacker, rDefencer, m_nAdditionalAttackLight, m_nAdditionalDefenceLight);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamageLight(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_LIGHT;
		}
		//Poison
		nDamage = PoisonDamage(rAttacker, rDefencer, m_nAdditionalAttackPoison, m_nAdditionalDefencePoison);
		// Refix damage with fluctuation
		nDamage	= nDamage + nDamage*nFluctuation/100;
		logicDamages.SetDamagePoison(rImp, nDamage);
		if(nMaxDamage<nDamage)
		{
			nMaxDamage = nDamage;
			nSpecialEffectID = DAMAGE_SPECIAL_EFFECT_POISON;
		}
		rImp.SetImpactID(nSpecialEffectID);

		return TRUE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	
	INT CombatCore_T::CalculateHitRate(INT nHit, INT nMiss)
	{
		__ENTER_FUNCTION
		nHit = Attr_VerifyHitMiss(nHit);
		nMiss = Attr_VerifyHitMiss(nMiss);
		if(0==(nHit+nMiss))
		{
			return 0;
		}
		return Float2Int(nHit*100/(nHit+nMiss));
		__LEAVE_FUNCTION
		return 0;
	}
	BOOL CombatCore_T::IsHit(INT nHitRate, INT nRand)
	{
		0>nHitRate?nHitRate=0:NULL;
		100<nHitRate?nHitRate=100:NULL;
		if(nRand<nHitRate)
		{
			return TRUE;
		}
		return FALSE;
	}
	BOOL CombatCore_T::IsCriticalHit(INT nCriticalRate, INT nRand)
	{
		__ENTER_FUNCTION
		nCriticalRate = Attr_VerifyCritical(nCriticalRate);
		if( nRand < nCriticalRate )
			return TRUE;//会心一击
		return FALSE;//普通一击
		__LEAVE_FUNCTION
		return FALSE;
	}
	INT CombatCore_T::PhysicalDamage(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack, INT nAdditionDefence)
	{
		__ENTER_FUNCTION
		
		INT	nDamage;
		INT nAttack = Attr_VerifyGeneralAttack(rMe.GetAttackPhysics()+nAdditionAttack);
		INT nDefence = Attr_VerifyDefence(rTar.GetDefencePhysics()+nAdditionDefence);
		INT nIgnoreRate = 0;
		// calculate damage
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= rTar.ItemValue(IATTRIBUTE_IMMUNITY_P).m_Value;
		}
		if(0==nAttack+nDefence)
		{
			//never enter here
			nDamage = 0;
		}
		else
		{
			nDamage = nAttack*nAttack/(nAttack+nDefence);
		}
		0>nIgnoreRate?nIgnoreRate=0:NULL;
		100<nIgnoreRate?nIgnoreRate=100:NULL;
		nDamage = nDamage - Float2Int((nDamage*nIgnoreRate)/100);
		0>nDamage?nDamage=0:NULL;
		return nDamage;
		__LEAVE_FUNCTION
		return 0;
	}
	INT CombatCore_T::MagicalDamage(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack, INT nAdditionDefence)
	{
		__ENTER_FUNCTION
		INT	nDamage;
		INT nAttack = Attr_VerifyGeneralAttack(rMe.GetAttackMagic()+nAdditionAttack);
		INT nDefence = Attr_VerifyDefence(rTar.GetDefenceMagic()+nAdditionDefence);
		INT nIgnoreRate = 0;
		// calculate damage
		if(Obj::OBJ_TYPE_HUMAN==rTar.GetObjType())
		{
			nIgnoreRate= rTar.ItemValue(IATTRIBUTE_IMMUNITY_P).m_Value;
		}
		if(0==nAttack+nDefence)
		{
			//never enter here
			nDamage = 0;
		}
		else
		{
			nDamage = nAttack*nAttack/(nAttack+nDefence);
		}
		0>nIgnoreRate?nIgnoreRate=0:NULL;
		100<nIgnoreRate?nIgnoreRate=100:NULL;
		nDamage = nDamage - Float2Int((nDamage*nIgnoreRate)/100.0f);
		0>nDamage?nDamage=0:NULL;
		return nDamage;
		__LEAVE_FUNCTION
		return 0;
	}
	INT CombatCore_T::ColdDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT	nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackCold()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceCold()+nAdditionalResist);
		if(MAX_EFFECTIVE_RESIST<nResist)
		{
			nResist = MAX_EFFECTIVE_RESIST;
		}
		if(-MAX_EFFECTIVE_RESIST>nResist)
		{
			nResist = -MAX_EFFECTIVE_RESIST;
		}
		nDamage = Float2Int(nAttack*(100-nResist)/100);
		0>nDamage?nDamage=0:NULL;
		return nDamage;
	}
	INT CombatCore_T::FireDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackFire()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceFire()+nAdditionalResist);
		if(MAX_EFFECTIVE_RESIST<nResist)
		{
			nResist = MAX_EFFECTIVE_RESIST;
		}
		if(-MAX_EFFECTIVE_RESIST>nResist)
		{
			nResist = -MAX_EFFECTIVE_RESIST;
		}
		nDamage = Float2Int(nAttack*(100-nResist)/100);
		0>nDamage?nDamage=0:NULL;
		return nDamage;
	}
	INT CombatCore_T::LightDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackLight()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefenceLight()+nAdditionalResist);
		if(MAX_EFFECTIVE_RESIST<nResist)
		{
			nResist = MAX_EFFECTIVE_RESIST;
		}
		if(-MAX_EFFECTIVE_RESIST>nResist)
		{
			nResist = -MAX_EFFECTIVE_RESIST;
		}
		nDamage = Float2Int(nAttack*(100-nResist)/100);
		0>nDamage?nDamage=0:NULL;
		return nDamage;
	}
	INT CombatCore_T::PoisonDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack, INT nAdditionalResist)
	{
		INT nDamage;
		INT nAttack = Attr_VerifyTraitAttack(rMe.GetAttackPoison()+nAdditionalAttack);
		INT nResist = Attr_VerifyResist(rTar.GetDefencePoison()+nAdditionalResist);
		if(MAX_EFFECTIVE_RESIST<nResist)
		{
			nResist = MAX_EFFECTIVE_RESIST;
		}
		if(-MAX_EFFECTIVE_RESIST>nResist)
		{
			nResist = -MAX_EFFECTIVE_RESIST;
		}
		nDamage = Float2Int(nAttack*(100-nResist)/100);
		0>nDamage?nDamage=0:NULL;
		return nDamage;
	}
}

