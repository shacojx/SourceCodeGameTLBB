///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact044.h
// 功能说明：效果--在一定时间内，增加效果所有者的移动速度，并且下一次成功打击目标后有一定几率增加3点连击点
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT044_H
#define STDIMPACT044_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact044_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_044,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_MOVE_SPEED_REFIX =0,
							IDX_ACTIVATE_ODDS,
							IDX_MODIFY_STRIKE_POINT,
						};
				};
				StdImpact044_T() {};
				~StdImpact044_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};

				INT GetMoveSpeedRefix(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MOVE_SPEED_REFIX);
				};
				INT GetActivateOdds(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_ACTIVATE_ODDS);
				};
				INT GetModifyStrikePoint(OWN_IMPACT const& rImp) const
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MODIFY_STRIKE_POINT);
				};
			public: //message handler
				VOID MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const;
				BOOL GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const;
				VOID OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character& rTar) const;
			protected:
			private:
		} SOT_GaiBang007_T;
	};
};
#endif //STDIMPACT044_H
