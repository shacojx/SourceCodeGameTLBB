///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact047.h
// 功能说明：效果--大理技能12的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT047_H
#define STDIMPACT047_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact047_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_047,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_EFFECTED_SKILL_COLLECTION= 0,
							IDX_ACTIVATE_ODDS,
							IDX_SUB_IMPACT_DATA_INDEX,
						};
				};

				StdImpact047_T() {};
				~StdImpact047_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetEffectedSkillCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_EFFECTED_SKILL_COLLECTION);
				};
				INT GetActivateOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ACTIVATE_ODDS);
				};
				INT GetSubImpactDataIndex(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT_DATA_INDEX);
				};
			public: //message handler
				virtual VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& rDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} SOT_DaLi012_T;
	};
};
#endif //STDIMPACT047_H