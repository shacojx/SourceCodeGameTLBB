///////////////////////////////////////////////////////////////////////////////
// 文件名：Combat_Core.h
// 功能说明：战斗逻辑内核
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef COMBAT_CORE_H
#define COMBAT_CORE_H

#include "Type.h"
#include "Obj_Character.h"
#include "SkillInfo.h"

using namespace Combat_Module;
using namespace Impact_Module;

namespace Combat_Module
{
	typedef enum
	{
		DAMAGE_SPECIAL_EFFECT_PHYSICAL=0,
		DAMAGE_SPECIAL_EFFECT_MAGIC=1,
		DAMAGE_SPECIAL_EFFECT_COLD=2,
		DAMAGE_SPECIAL_EFFECT_FIRE=3,
		DAMAGE_SPECIAL_EFFECT_LIGHT=4,
		DAMAGE_SPECIAL_EFFECT_POISON=5
			
	} DamageSEID_T;
	
	class CombatCore_T
	{
		//注: 请在每次使用时声明一个实体，内含成员变量，非多线程支持
		public:
			CombatCore_T(){Reset();};
			~CombatCore_T(){};
			INT CalculateHitRate(INT nHit, INT nMiss);
			BOOL IsHit(INT nHitRate, INT nRand);
			BOOL IsCriticalHit(INT nCriticalRate, INT nRand);
			VOID Reset();
			BOOL GetResultImpact(Obj_Character& rAttacker, Obj_Character& rDefenceer, OWN_IMPACT& rImp);
		public:
			INT GetAdditionalAttackPhysics(VOID) const {return m_nAdditionalAttackPhysics;};
			VOID SetAdditionalAttackPhysics(INT nValue) {m_nAdditionalAttackPhysics=nValue;};
			INT GetAdditionalAttackMagic(VOID) const {return m_nAdditionalAttackMagic;};
			VOID SetAdditionalAttackMagic(INT nValue) {m_nAdditionalAttackMagic=nValue;};
			INT GetAdditionalAttackCold(VOID) const {return m_nAdditionalAttackCold;};
			VOID SetAdditionalAttackCold(INT nValue) {m_nAdditionalAttackCold=nValue;};
			INT GetAdditionalAttackFire(VOID) const {return m_nAdditionalAttackFire;};
			VOID SetAdditionalAttackFire(INT nValue) {m_nAdditionalAttackFire=nValue;};
			INT GetAdditionalAttackLight(VOID) const {return m_nAdditionalAttackLight;};
			VOID SetAdditionalAttackLight(INT nValue) {m_nAdditionalAttackLight=nValue;};
			INT GetAdditionalAttackPoison(VOID) const {return m_nAdditionalAttackPoison;};
			VOID SetAdditionalAttackPoison(INT nValue) {m_nAdditionalAttackPoison=nValue;};
		protected:
			INT PhysicalDamage(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack=0, INT nAdditionDefence=0);
			INT MagicalDamage(Obj_Character& rMe, Obj_Character& rTar, INT nAdditionAttack=0, INT nAdditionDefence=0);
			INT ColdDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			INT FireDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			INT LightDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
			INT PoisonDamage(Obj_Character& rMe,Obj_Character& rTar, INT nAdditionalAttack=0, INT nAdditionalResist=0);
		private:
			INT m_nAdditionalAttackPhysics;
			INT m_nAdditionalAttackMagic;
			INT m_nAdditionalAttackCold;
			INT m_nAdditionalAttackFire;
			INT m_nAdditionalAttackLight;
			INT m_nAdditionalAttackPoison;
			INT m_nAdditionalDefencePhysics;
			INT m_nAdditionalDefenceMagic;
			INT m_nAdditionalDefenceCold;
			INT m_nAdditionalDefenceFire;
			INT m_nAdditionalDefenceLight;
			INT m_nAdditionalDefencePoison;
	};
};
#endif //SKILL_CORE_H
