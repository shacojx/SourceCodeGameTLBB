///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact003.h
// 功能说明：效果--分类型的一次性伤害
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT003_H
#define STDIMPACT003_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact003_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_003,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_DAMAGE_PHY=0,
							IDX_DAMAGE_MAGIC,
							IDX_DAMAGE_COLD,
							IDX_DAMAGE_FIRE,
							IDX_DAMAGE_LIGHT,
							IDX_DAMAGE_POISON,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_DAMAGE_PHY=0,
							IDX_DAMAGE_MAGIC,
							IDX_DAMAGE_COLD,
							IDX_DAMAGE_FIRE,
							IDX_DAMAGE_LIGHT,
							IDX_DAMAGE_POISON,
						};
				};
				StdImpact003_T() {};
				~StdImpact003_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetDamagePhy(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_PHY);};
				VOID SetDamagePhy(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_PHY, nDamage);};

				INT GetDamageMagic(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC);};
				VOID SetDamageMagic(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_MAGIC, nDamage);};

				INT GetDamageCold(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_COLD);};
				VOID SetDamageCold(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_COLD, nDamage);};

				INT GetDamageFire(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FIRE);};
				VOID SetDamageFire(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_FIRE, nDamage);};

				INT GetDamageLight(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_LIGHT);};
				VOID SetDamageLight(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_LIGHT, nDamage);};

				INT GetDamagePoison(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_POISON);};
				VOID SetDamagePoison(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE_POISON, nDamage);};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID CriticalRefix(OWN_IMPACT& rImp) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DI_DamagesByValue_T;
	};
};
#endif //STDIMPACT003_H
