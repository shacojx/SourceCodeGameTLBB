///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact030.h
// 功能说明：效果--峨嵋技能易筋锻骨的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT030_H
#define STDIMPACT030_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact030_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_030,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_SHIELD_HP,
							IDX_ABSORB_RATE,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SHIELD_HP=0,
							IDX_ABSORB_RATE,
							IDX_TARGET_IMPACT_COLLECTION,
							IDX_IMPACT_REFIX_RATE,
							IDX_SCAN_RADIUS,
							IDX_EFFECTED_OBJ_COUNT,
							IDX_SUB_IMPACT_INDEX,
						};
				};
				StdImpact030_T() {};
				~StdImpact030_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetShieldHp(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_SHIELD_HP);};
				VOID SetShieldHp(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_SHIELD_HP, nValue);};

				INT GetAbsorbRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_ABSORB_RATE);};
				VOID SetAbsorbRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_ABSORB_RATE, nValue);};

				INT GetShieldHpInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SHIELD_HP);
				};
				INT GetAbsorbRateInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ABSORB_RATE);
				};
				INT GetTargetImpactCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TARGET_IMPACT_COLLECTION);
				};
				INT GetImpactRefixRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_IMPACT_REFIX_RATE);
				};
				INT GetScanRadius(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SCAN_RADIUS);
				};
				INT GetEffectedObjCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_EFFECTED_OBJ_COUNT);
				};
				INT GetSubImpactIndex(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT_INDEX);
				};
			public: //message handler
				VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& nDamage, SkillID_t nSkillID) const;
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const;
				VOID RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DS_EMei014_T;
	};
};
#endif //STDIMPACT030_H
