///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact002.h
// 功能说明：效果--无类型的一次性百分率伤害
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT002_H
#define STDIMPACT002_H

#include "ImpactLogic.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact002_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_002,
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
				StdImpact002_T() {};
				~StdImpact002_T() {};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				INT GetDamage(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE);};
				VOID SetDamage(OWN_IMPACT& rImp, INT const nDamage) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_DAMAGE,nDamage);};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID CriticalRefix(OWN_IMPACT& rImp) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} DI_DamageByRate_T;
	};
};
#endif //DI_DAMAGE_H
