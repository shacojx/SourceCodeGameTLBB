///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact040.h
// 功能说明：效果--在一定时间内，增加效果所有者的移动速度，并且下一次打击驱散目标怒气
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT040_H
#define STDIMPACT040_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact040_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_040,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_TIMES = 0,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_TIMES = 0,
							IDX_MOVE_SPEED_REFIX,
							IDX_SUB_IMPACT_DATA_INDEX,
						};
				};
				StdImpact040_T() {};
				~StdImpact040_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetActivateTimes(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_TIMES);};
				VOID SetActivateTimes(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_ACTIVATE_TIMES, nValue);};

				INT GetActivateTimesInTable(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ACTIVATE_TIMES);
				};
				INT GetMoveSpeedRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MOVE_SPEED_REFIX);
				};
				INT GetSubImpact(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SUB_IMPACT_DATA_INDEX);
				};
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar) const;
			protected:
			private:
		} SOT_ShaoLin013_T;
	};
};
#endif //STDIMPACT040_H
