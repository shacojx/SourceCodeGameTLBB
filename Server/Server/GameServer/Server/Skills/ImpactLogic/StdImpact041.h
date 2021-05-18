///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact041.h
// 功能说明：效果--明教的五星连珠的狂暴效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT041_H
#define STDIMPACT041_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact041_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_041,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ATTACK_PHY_REFIX = 0,
							IDX_DEFENCE_PHY_REFIX,
							IDX_ATTACK_MAGIC_REFIX,
							IDX_DEFENCE_MAGIC_REFIX,
							IDX_RAGE_REGENERATE_REFIX,
						};
				};
				StdImpact041_T() {};
				~StdImpact041_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetAttackPhysicsRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_PHY_REFIX);
				};
				INT GetDefencePhysicsRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEFENCE_PHY_REFIX);
				};
				INT GetAttackMagicRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ATTACK_MAGIC_REFIX);
				};
				INT GetDefenceMagicRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DEFENCE_MAGIC_REFIX);
				};
				INT GetRageRegenerateRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RAGE_REGENERATE_REFIX);
				};
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const;
			protected:
			private:

		} SOT_MingJiao006_T;
	};
};
#endif //STDIMPACT041_H
