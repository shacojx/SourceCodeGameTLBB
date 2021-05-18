///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact017.h
// 功能说明：效果--免疫特定的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT017_H
#define STDIMPACT017_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact017_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_017,
				};
				enum
				{
					COLLECTION_COUNT = 8,
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
							IDX_IMMUNO_LEVEL = 0,
							IDX_COLLECTION0,
							IDX_COLLECTION1,
							IDX_COLLECTION2,
							IDX_COLLECTION3,
							IDX_COLLECTION4,
							IDX_COLLECTION5,
							IDX_COLLECTION6,
							IDX_COLLECTION7,
						};
				};

				StdImpact017_T() {};
				~StdImpact017_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetRefixRate(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE);};
				VOID SetRefixRate(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_REFIX_RATE, nValue);};

				INT GetImmunoLevel(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_IMMUNO_LEVEL);
				};
				INT GetCollectionByIndex(OWN_IMPACT const& rImp, INT nIndex) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_COLLECTION0+nIndex);
				};
			public: //message handler
				INT  OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
				
		} SOT_ImmunoImpacts_T;
	};
};
#endif //STDIMPACT017_H
