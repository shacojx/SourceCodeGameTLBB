///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact057.h
// 功能说明：效果--宠物生命转换成主人的HP,MP
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT057_H
#define STDIMPACT057_H

 
#include "ImpactLogic.h"
 
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact057_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_057,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_DAMAGE_RATE = 0,
							IDX_TRANSFER_RATE_FOR_HP,
							IDX_TRANSFER_RATE_FOR_MP,
							IDX_SUB_IMPACT_HEAL,
						};
				};
				StdImpact057_T() {};
				~StdImpact057_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};

				INT GetDamageRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DAMAGE_RATE);
				};
				INT GetTransferRateForHp(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TRANSFER_RATE_FOR_HP);
				};
				INT GetTransferRateForMp(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TRANSFER_RATE_FOR_MP);
				};
				INT GetSubImpactHeal(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT_HEAL);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		} DI_PetHealthToOwnerHpAndMp_T;
	};
};
#endif //STDIMPACT057_H
