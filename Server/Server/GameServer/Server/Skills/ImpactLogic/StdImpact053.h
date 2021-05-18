///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact053.h
// 功能说明：效果--所用技能cooldown,除了指定的cooldownid
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT053_H
#define STDIMPACT053_H

#include "ImpactLogic.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact053_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_053,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_COOLDOWN_ID_EXCLUDE = 0,
						};
				};
				StdImpact053_T() {};
				~StdImpact053_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetCooldownExclude(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_COOLDOWN_ID_EXCLUDE);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		} DI_AllSkillCooldownExcept_T;
	};
};
#endif //STDIMPACT053_H
