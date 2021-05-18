///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact011.h
// 功能说明：效果--State: refix all attack, Defence and MaxHP, MaxMP, MaxRage, MaxStrikePoint
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT011_H
#define STDIMPACT011_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact011_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_011,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ATTACK_PHY = 0,
							IDX_DEFENCE_PHY,
							IDX_ATTACK_MAGIC,
							IDX_DEFENCE_MAGIC,
							IDX_ATTACK_COLD,
							IDX_RESIST_COLD,
							IDX_ATTACK_FIRE,
							IDX_RESIST_FIRE,
							IDX_ATTACK_LIGHT,
							IDX_RESIST_LIGHT,
							IDX_ATTACK_POISON,
							IDX_RESIST_POISON,
							IDX_MAX_HP,
							IDX_MAX_MP,
							IDX_MAX_RAGE,
							IDX_MAX_STRIKE_POINT,
						};
				};
				StdImpact011_T() {};
				~StdImpact011_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetAttackPhysicsRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_PHY);
				};
				INT GetAttackMagicRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_MAGIC);
				};
				INT GetAttackColdRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_COLD);
				};
				INT GetAttackFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_FIRE);
				};
				INT GetAttackLightRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_LIGHT);
				};
				INT GetAttackPoisonRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_POISON);
				};

				INT GetDefencePhysicsRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEFENCE_PHY);
				};
				INT GetDefenceMagicRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC);
				};
				INT GetResistColdRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_COLD);
				};
				INT GetResistFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_FIRE);
				};
				INT GetResistLightRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_LIGHT);
				};
				INT GetResistPoisonRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RESIST_POISON);
				};

				INT GetMaxHpRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MAX_HP);
				};
				INT GetMaxMpRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MAX_MP);
				};
				INT GetMaxRageRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MAX_RAGE);
				};
				INT GetMaxStrikePointRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MAX_STRIKE_POINT);
				};

			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
				
		} RefixAttrLevel2ByValue_AttacksDefencesAndMaxs_T;
	};
};
#endif //STDIMPACT011_H
