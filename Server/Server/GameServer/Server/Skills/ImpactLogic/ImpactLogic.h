///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactLogic.h
// 功能说明：效果的基础类，声明了所有的公共接口和公共的固定逻辑接口
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef IMPACTLOGIC_H
#define IMPACTLOGIC_H

#include "Type.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "DataRecords.h"
#include "SkillInfo.h"
#include "Impact_Core.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Impact_Module
	{
		using namespace Packets;
		class ImpactLogic_T
		{
			public:
				// interface
				enum
				{
					ID = INVALID_ID,
				};
				ImpactLogic_T() {};
				~ImpactLogic_T() {};
				// initializer
				virtual BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const {return TRUE;};
				//flags
				virtual BOOL IsOverTimed(VOID) const {return FALSE;};
				virtual BOOL IsIntervaled(VOID) const {return FALSE;};
			public:
				//event handler
				VOID HeartBeat(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				virtual VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const {};
				virtual VOID OnBeCriticalHit(OWN_IMPACT& rImp, Obj_Character & rMe) const {};
				virtual VOID OnBeHeal(OWN_IMPACT& rImp, Obj_Character& rMe, INT& nHealedHP)const{};
				virtual VOID OnCriticalHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe) const {};
				virtual VOID OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const {};
				virtual VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const {};
				virtual VOID OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const {};
				virtual VOID OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const{};
				virtual VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const{};
				virtual VOID OnHealTarget(OWN_IMPACT & rImp, Obj_Character & rMe, INT& rHealedHP) const {};
				virtual VOID OnBeHit(OWN_IMPACT& rImp, Obj_Character&rMe, Obj_Character& rAttacker) const {};
				virtual VOID OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar) const{};
				virtual VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const {};
				virtual VOID OnUseSkillSuccessfully(OWN_IMPACT& rImp, Obj_Character& rMe, SkillInfo_T& rSkill) const {};
				virtual INT  OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const {return MissFlag_T::FLAG_NORMAL;};
				virtual BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const {return FALSE;};
				virtual BOOL GetBoolAttrRefix(OWN_IMPACT& rImp, Obj_Character& rMe, CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix) const {return FALSE;};
				virtual VOID RefixSkill(OWN_IMPACT& rImp, SkillInfo_T& rSkill) const {};
				virtual VOID RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const {};
				virtual VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const {};
				virtual BOOL CanViewSpecialObj(OWN_IMPACT& rImp, Obj_Special& rObj) const {return FALSE;}
			public:
				//public function
				BOOL IsSupportStillOn(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				virtual VOID CriticalRefix(OWN_IMPACT& rImp) const {};
				virtual BOOL IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const{return FALSE;};
				virtual VOID MarkModifiedAttrDirty(OWN_IMPACT& rImp, Obj_Character& rMe) const {};
				virtual BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const{return FALSE;};
				virtual BOOL RefixContinuanceByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
				BOOL ScanUnitForTarget(OWN_IMPACT& rImp, Obj_Character& rMe, WORLD_POS const& rPos, FLOAT fRadius, INT nListSize, OBJLIST& rTargets)const;
			protected:
				virtual BOOL SpecialHeartBeatCheck(OWN_IMPACT& rImp, Obj_Character& rMe) const {return TRUE;};
				virtual VOID ContinuanceCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const;
				virtual VOID IntervalCalc(OWN_IMPACT& rImp, Obj_Character& rMe, INT nDeltaTime) const;
			private:
		};
		typedef enum _ImpactLogicID_T
		{
			STD_IMPACT_000,
			STD_IMPACT_001,
			STD_IMPACT_002,
			STD_IMPACT_003,
			STD_IMPACT_004,
			STD_IMPACT_005,
			STD_IMPACT_006,
			STD_IMPACT_007,
			STD_IMPACT_008,
			STD_IMPACT_009,
			STD_IMPACT_010,
			STD_IMPACT_011,
			STD_IMPACT_012,
			STD_IMPACT_013,
			STD_IMPACT_014,
			STD_IMPACT_015,
			STD_IMPACT_016,
			STD_IMPACT_017,
			STD_IMPACT_018,
			STD_IMPACT_019,
			STD_IMPACT_020,
			STD_IMPACT_021,
			STD_IMPACT_022,
			STD_IMPACT_023,
			STD_IMPACT_024,
			STD_IMPACT_025,
			STD_IMPACT_026,
			STD_IMPACT_027,
			STD_IMPACT_028,
			STD_IMPACT_029,
			STD_IMPACT_030,
			STD_IMPACT_031,
			STD_IMPACT_032,
			STD_IMPACT_033,
			STD_IMPACT_034,
			STD_IMPACT_035,
			STD_IMPACT_036,
			STD_IMPACT_037,
			STD_IMPACT_038,
			STD_IMPACT_039,
			STD_IMPACT_040,
			STD_IMPACT_041,
			STD_IMPACT_042,
			STD_IMPACT_043,
			STD_IMPACT_044,
			STD_IMPACT_045,
			STD_IMPACT_046,
			STD_IMPACT_047,
			STD_IMPACT_048,
			STD_IMPACT_049,
			STD_IMPACT_050,
			STD_IMPACT_051,
			STD_IMPACT_052,
			STD_IMPACT_053,
			STD_IMPACT_054,
			STD_IMPACT_055,
			STD_IMPACT_056,
			STD_IMPACT_057,
			STD_IMPACT_058,
			STD_IMPACT_059,
			STD_IMPACT_060,
			STD_IMPACT_061,
			STD_IMPACT_062,
			STD_IMPACT_063,
			STD_IMPACT_064,
			STD_IMPACT_065,
			STD_IMPACT_066,
			STD_IMPACT_067,
			STD_IMPACT_068,
			STD_IMPACT_069,
			STD_IMPACT_070,
			STD_IMPACT_071,
			STD_IMPACT_072,
			STD_IMPACT_073,
			STD_IMPACT_074,
			STD_IMPACT_075,
			STD_IMPACT_076,
			STD_IMPACT_077,
			STD_IMPACT_078,
			STD_IMPACT_079,
			NUMBER_OF_IMPACT_LOGIC_IDS
		} ImpactLogicID_T;
		ImpactLogic_T const* Impact_GetLogic(OWN_IMPACT const& rImp);
	};
};
#endif //IMPACTLOGIC_H

