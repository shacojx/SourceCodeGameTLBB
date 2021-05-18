///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact006.h
// 功能说明：效果--瞬发-恢复宠物生命和快乐值
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT006_H
#define STDIMPACT006_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact006_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_006,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_HP=0,
							IDX_HAPPINESS,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_HP=0,
							IDX_HAPPINESS,
						};
				};
				StdImpact006_T() {};
				~StdImpact006_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetHpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_HP);}
				VOID SetHpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_HP, nValue);}

				INT GetHappinessModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_HAPPINESS);}
				VOID SetHappinessModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_HAPPINESS, nValue);}
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DI_ModifyPetHpHappinessByValue_T;
	};
};
#endif //STDIMPACT006_H
