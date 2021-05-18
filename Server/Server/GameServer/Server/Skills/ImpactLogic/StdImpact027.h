///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact027.h
// 功能说明：受击时将自己的一部分伤害转移到自己的宠物身上
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef STDIMPACT027_H
#define STDIMPACT027_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact027_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_027,
				};
				class OwnImpactParamIndex_T
				{
					public:
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_TRANSFER_RATE,
						};
				};
				StdImpact027_T() {};
				~StdImpact027_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetTransferRate(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TRANSFER_RATE);
				};
			public: //message handler
				VOID OnDamage(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT& nDamage, SkillID_t nSkillID) const;
			protected:
			private:
		} DS_TransferDamageToPet_T;
	};
};
#endif //STDIMPACT027_H
