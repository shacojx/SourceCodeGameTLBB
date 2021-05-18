///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact029.h
// 功能说明：效果--峨嵋技能虚冲养气的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT029_H
#define STDIMPACT029_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact029_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_029,
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
							IDX_TARGET_SKILL_COLLECTION_1,
							IDX_DEPLETION_REFIX,
							IDX_TARGET_SKILL_COLLECTION_2,
							IDX_CHARGE_TIME_REFIX,
							IDX_ADDITIONAL_RAGE,
						};
				};
				StdImpact029_T() {};
				~StdImpact029_T() {};
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
				INT GetTargetSkillCollectionForDepletionRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TARGET_SKILL_COLLECTION_1);
				};
				INT GetDepletionRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEPLETION_REFIX);
				};
				INT GetTargetSkillCollectionForChargeTimeRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TARGET_SKILL_COLLECTION_2);
				};
				INT GetChargeTimeRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_CHARGE_TIME_REFIX);
				};
				INT GetAdditionalRage(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ADDITIONAL_RAGE);
				};
			public: //message handler
				VOID RefixSkill(OWN_IMPACT& rImp, SkillInfo_T& rSkill) const;
				VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& nDamage, SkillID_t nSkillID) const;
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DS_EMei009_T;
	};
};
#endif //STDIMPACT029_H
