///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact019.h
// 功能说明：效果--增加某些效果的威力和持续时间时间
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT019_H
#define STDIMPACT019_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact019_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_019,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_TIMES = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_TIMES = 0,
							IDX_TARGET_IMPACT_COLLECTION,
							IDX_POWER_REFIX,
							IDX_CONTINUANCE_REFIX,
						};
				};
				StdImpact019_T() {};
				~StdImpact019_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetActivateTimes(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_TIMES);};
				VOID SetActivateTimes(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_TIMES, nValue);};

				INT GetActivateTimesInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ACTIVATE_TIMES);
				};
				INT GetTargetImpactCollection(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TARGET_IMPACT_COLLECTION);
				};
				INT GetPowerRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_POWER_REFIX);
				};
				INT GetContinuanceRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_CONTINUANCE_REFIX);
				};
			public: //message handler
				VOID RefixImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedRefix) const;
			protected:
			private:
		} SOT_ImpactRefix_T;
	};
};
#endif //STDIMPACT019_H
