///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact048.h
// 功能说明：效果--大理技能15的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT048_H
#define STDIMPACT048_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact048_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_048,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SUB_IMPACT = 0,
						};
				};

				StdImpact048_T() {};
				~StdImpact048_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetSubImpact(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT);
				};
			public: //message handler
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		}SOT_DaLi015_T;
	};
};
#endif //STDIMPACT048_H
