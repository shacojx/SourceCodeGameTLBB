///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact024.h
// 功能说明：效果--过滤不良效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT024_H
#define STDIMPACT024_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"
#include "Attributes.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact024_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_024,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ODDS,
						};
				};
				enum
				{
					COLLECTION_NUMBERS = 4,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ODDS = 0,
							IDX_IMMUNIZED_IMPACT_COLLECTION,
						};
				};
				StdImpact024_T() {};
				~StdImpact024_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetActivateOdds(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_ODDS);};
				VOID SetActivateOdds(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_ODDS, nValue);};

				INT GetActivateOddsInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ACTIVATE_ODDS);
				};
				INT GetImmunoImpactsByIndex(OWN_IMPACT const& rImp, INT nIndex) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_IMMUNIZED_IMPACT_COLLECTION+nIndex);
				};
			public: //message handler
				INT OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const;
			protected:
			private:
		} DS_FilterDebuff_T;
	};
};
#endif //STDIMPACT024_H


