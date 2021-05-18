///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact028.h
// 功能说明：
//
// 效果描述部分的格式：|效果ID|持续时间|冰属性修正|火属性修正|电属性修正|毒属性修正
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT028_H
#define STDIMPACT028_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact028_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_028,
				};
				class OwnImpactParamIndex_T
				{
					public:
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_COLD_REFIX,
							IDX_FIRE_REFIX,
							IDX_LIGHT_REFIX,
							IDX_POISON_REFIX,
						};
				};

				StdImpact028_T() {};
				~StdImpact028_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				
				INT GetColdRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_COLD_REFIX);
				};
				INT GetFireRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_FIRE_REFIX);
				};
				INT GetLightRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_LIGHT_REFIX);
				};
				INT GetPoisonRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_POISON_REFIX);
				};
			public: //message handler
				virtual VOID OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const;
			protected:
			private:
		} DS_PetTrait_T;
	};
};
#endif //STDIMPACT028_H