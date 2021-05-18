///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact033.h
// 功能说明：效果--策反
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT033_H
#define STDIMPACT033_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact033_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_033,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_AI_TYPE_REFIX =0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_AI_TYPE_REFIX = 0,
						};
				};
				StdImpact033_T() {};
				~StdImpact033_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetAiTypeRefix(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_AI_TYPE_REFIX);};
				VOID SetAiTypeRefix(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_AI_TYPE_REFIX, nValue);};

				INT GetAiTypeRefixInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_AI_TYPE_REFIX);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
			protected:
			private:
		} SOT_XingXiu011_T;
	};
};
#endif //STDIMPACT033_H
