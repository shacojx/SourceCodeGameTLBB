///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact031.h
// 功能说明：效果--武当技能006的伤害过滤盾
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT031_H
#define STDIMPACT031_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact031_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_031,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_COLLECTED_DAMAGE =0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ABSORB_ODDS = 0,
							IDX_REFLECT_RATE,
							IDX_DEFENCE_PHYSICS_REFIX,
							IDX_DEFENCE_MAGIC_REFIX,
							IDX_RESIST_COLD_REFIX,
							IDX_RESIST_FIRE_REFIX,
							IDX_RESIST_LIGHT_REFIX,
							IDX_RESIST_POISON_REFIX,
						};
				};
				StdImpact031_T() {};
				~StdImpact031_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetCollectedDamage(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_COLLECTED_DAMAGE);};
				VOID SetCollectedDamage(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_COLLECTED_DAMAGE, nValue);};

				INT GetAbsorbOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ABSORB_ODDS);
				};
				INT GetReflectRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_REFLECT_RATE);
				};
				INT GetDefencePhysicsRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEFENCE_PHYSICS_REFIX);
				};
				INT GetDefenceMagicRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_REFIX);
				};
				INT GetResistColdRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_COLD_REFIX);
				};
				INT GetResistFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_FIRE_REFIX);
				};
				INT GetResistLightRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_LIGHT_REFIX);
				};
				INT GetResistPoisonRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_POISON_REFIX);
				};
			public: //message handler
				VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const;
			protected:
			private:
		} DS_WuDang006_T;
	};
};
#endif //STDIMPACT031_H
