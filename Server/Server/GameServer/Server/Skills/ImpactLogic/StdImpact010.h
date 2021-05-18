///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact010.h
// 功能说明：效果--间隔发作的驻留效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT010_H
#define STDIMPACT010_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact010_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_010,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_REFIX_RATE = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
						};
				};
				StdImpact010_T() {};
				~StdImpact010_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return TRUE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetSubImpactDataIndexByIndex(OWN_IMPACT const& rImp, INT nIndex) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), nIndex);
				}

			public: //message handler
				VOID OnIntervalOver(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		};
	};
};
#endif //STDIMPACT010_H
