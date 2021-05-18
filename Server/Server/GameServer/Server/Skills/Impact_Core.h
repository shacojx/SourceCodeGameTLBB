///////////////////////////////////////////////////////////////////////////////
// 文件名：Impact_Core.h
// 功能说明：效果内核模块
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef IMPACT_CORE_H
#define IMPACT_CORE_H

#include "Type.h"
#include "Obj_Character.h"
#include "DataRecords.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		class ImpactCore_T
		{
			public:
				ImpactCore_T(){};
				~ImpactCore_T(){};
				BOOL InitImpactFromData(ID_t nDataIndex, OWN_IMPACT& rImp) const;
				BOOL SendImpactToUnit(Obj_Character& rTar, ID_t nDataIndex, ObjID_t nSender, Time_t nDelayTime=0, BOOL bCriticalFlag=FALSE, INT nRefixRate=0) const;
			private:
		};
		//impact
		ImpactData_T const* Impact_GetDataBlock(ID_t nDataIndex);
		BOOL Impact_IsImpactsABMutexed(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB);
		BOOL Impact_CanImpactAReplaceImpactB(OWN_IMPACT& rImpactA, OWN_IMPACT& rImpactB);
		Obj* Impact_GetCaster(Obj& rMe, OWN_IMPACT& rImp);
		ID_t Impact_GetLogicID(OWN_IMPACT const& rImp);
		BOOL Impact_IsOverTimed(OWN_IMPACT& rImp);
		ID_t Impact_GetImpactID(OWN_IMPACT& rImp);
		ID_t Impact_GetMutexID(OWN_IMPACT& rImp);
		BOOL Impact_GetMutexByCasterFlag(OWN_IMPACT& rImp);
		ID_t Impact_GetLevel(OWN_IMPACT& rImp);
		ID_t Impact_GetStandFlag(OWN_IMPACT& rImp);
		BOOL Impact_IsRemainOnCorpse(OWN_IMPACT& rImp);
		BOOL Impact_CanBeDispeled(OWN_IMPACT& rImp);
		BOOL Impact_CanBeCanceled(OWN_IMPACT& rImp);
		BOOL Impact_NeedChannelSupport(OWN_IMPACT& rImp);
		BOOL Impact_IsFadeOutWhenUnitOnDamage(OWN_IMPACT& rImp);
		BOOL Impact_IsFadeOutWhenUnitStartActions(OWN_IMPACT& rImp);
		BOOL Impact_IsFadeOutWhenUnitOffline(OWN_IMPACT& rImp);
		BOOL Impact_IsStillTimingWhenUnitOffline(OWN_IMPACT& rImp);
		ID_t Impact_IgnoreFliter(OWN_IMPACT& rImp);
		Time_t Impact_GetTableContinuance(OWN_IMPACT& rImp);
		Time_t Impact_GetInterval(OWN_IMPACT& rImp);
		INT	Impact_GetImpactDataDescriptorValueByIndex(INT nDataIndex, INT nIndex);
		BOOL Impact_IsImpactInCollection(OWN_IMPACT& rImp, INT nCollection);
	};
};
extern Combat_Module::Impact_Module::ImpactCore_T g_ImpactCore;
#endif //IMPACT_CORE_H
