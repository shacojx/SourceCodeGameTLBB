///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact026.h
// 功能说明：效果--伤害盾，吸收、免疫、反弹
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT026_H
#define STDIMPACT026_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact026_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_026,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_SHIELD_HP = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SHIELD_HP = 0,
							IDX_ABSORB_RATE,
							IDX_IGNORE_RATE,
							IDX_REFLECT_RATE,
							IDX_REFLECT_ACTIVATE_ODDS,
						};
				};
				StdImpact026_T() {};
				~StdImpact026_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetShieldHp(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_SHIELD_HP);};
				VOID SetShieldHp(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_SHIELD_HP, nValue);};

				INT GetShieldHpInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SHIELD_HP);
				};
				INT GetAbsorbRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ABSORB_RATE);
				};
				INT GetIgnoreRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_IGNORE_RATE);
				};
				INT GetReflectRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_REFLECT_RATE);
				};
				INT GetReflectActivateOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_REFLECT_ACTIVATE_ODDS);
				};
			public: //message handler
				VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& nDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} DS_AbsorbIgnoreReflect_T;
	};
};
#endif //STDIMPACT026_H