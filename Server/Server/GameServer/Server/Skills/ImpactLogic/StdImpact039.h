///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact039.h
// 功能说明：效果--在一段时间里爆击率增加，同时受到伤害将增加怒气
//			少林技能铁布衫用
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT039_H
#define STDIMPACT039_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact039_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_039,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_CRITICAL_REFIX = 0,
							IDX_RAGE_REGENERATE_REFIX,
						};
				};
				StdImpact039_T() {};
				~StdImpact039_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetCriticalRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_CRITICAL_REFIX);
				};
				INT GetRageRegenerateRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RAGE_REGENERATE_REFIX);
				};
			public: //message handler
				VOID RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
			protected:
			private:
		} SOT_ShaoLin007_T;
	};
};
#endif //STDIMPACT039_H
