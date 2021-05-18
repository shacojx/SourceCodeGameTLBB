///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact036.h
// 功能说明：效果--探测陷阱
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT036_H
#define STDIMPACT036_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class StdImpact036_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_036,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_DETECT_LEVEL_REFIX= 0,
						};
				};
				StdImpact036_T() {};
				~StdImpact036_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetDetectLevelRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_DETECT_LEVEL_REFIX);
				};
			public: //message handler
				BOOL CanViewSpecialObj(OWN_IMPACT & rImp, Obj_Special & rObj) const;
			protected:
			private:
		};
	};
};
#endif //STDIMPACT036_H
