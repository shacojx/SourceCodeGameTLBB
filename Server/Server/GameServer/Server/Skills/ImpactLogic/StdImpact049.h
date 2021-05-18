///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact049.h
// 功能说明：效果--逍遥神光离合
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT049_H
#define STDIMPACT049_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact049_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_049,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_EFFECTED_SKILL_COLLECTION = 0,
							IDX_ACTIVATE_ODDS,
							IDX_SUB_IMPACT,
						};
				};

				StdImpact049_T() {};
				~StdImpact049_T() {};
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
				INT GetSubImpact(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT);
				};
			public: //message handler
				VOID OnDamageTarget(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar, INT& rDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} SOT_TianShan011_T;
	};
};
#endif //STDIMPACT049_H
