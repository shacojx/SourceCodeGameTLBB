///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact020.h
// 功能说明：效果--在一定时间内，效果所有者死亡后可以在原地复活。
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT020_H
#define STDIMPACT020_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact020_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_020,
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
				StdImpact020_T() {};
				~StdImpact020_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
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
				VOID OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} SOT_SelfResurrection_T;
	};
};
#endif //STDIMPACT020_H
