///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact034.h
// 功能说明：效果--星宿技能恶贯满盈的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DS_XINGXIU017_H
#define DS_XINGXIU017_H


#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact034_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_034,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SCAN_RADIUS =0,
							IDX_EFFECTED_OBJ_COUNT,
							IDX_SUB_IMPACT_DATA_INDEX,
						};
				};
				enum
				{
				};
				StdImpact034_T() {};
				~StdImpact034_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetScanRadius(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SCAN_RADIUS);
				};
				INT GetEffectedObjCount(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_EFFECTED_OBJ_COUNT);
				};

				INT GetSubImpactDataIndex(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT_DATA_INDEX);
				};
			public: //message handler
				VOID OnDie(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL IsScanedTargetValid(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character& rTar) const;
			protected:
			private:
		} SOT_XingXiu017_T;
	};
};
#endif //DS_XINGXIU017_H
