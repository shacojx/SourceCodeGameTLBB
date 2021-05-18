///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact054.h
// 功能说明：效果--复活
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT054_H
#define STDIMPACT054_H

 
#include "ImpactLogic.h"
 
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact054_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_054,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_HP_RECOVER_RATE=0,
							IDX_MP_RECOVER_RATE,
							IDX_RAGE_RECOVER_RATE,
						};
				};
				StdImpact054_T() {};
				~StdImpact054_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetHpRecoverRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_HP_RECOVER_RATE);
				};
				INT GetMpRecoverRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MP_RECOVER_RATE);
				};
				INT GetRageRecoverRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RAGE_RECOVER_RATE);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DI_Resurrection_T;
	};
};
#endif //STDIMPACT054_H
