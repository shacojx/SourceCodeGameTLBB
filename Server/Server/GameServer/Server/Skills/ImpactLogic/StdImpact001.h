///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact001.h
// 功能说明：效果--无类型的一次性伤害
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT001_H
#define STDIMPACT001_H

#include "ImpactLogic.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact001_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_001,
				};
				class OwnImpactParamIndex_T
				{
				public:
					enum
					{
						IDX_DAMAGE=0,
					};
				};
				class ImpactDataDescriptorIndex_T
				{
				public:
					enum
					{
						IDX_DAMAGE=0,
					};
				};
				StdImpact001_T() {};
				~StdImpact001_T() {};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				INT GetDamage(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE);};
				VOID SetDamage(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE,nDamage);};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID CriticalRefix(OWN_IMPACT& rImp) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DI_DamageByValue_T;
	};
};
#endif //DI_DAMAGE_H
