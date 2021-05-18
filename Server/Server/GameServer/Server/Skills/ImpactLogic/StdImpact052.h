///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact052.h
// 功能说明：效果--逍遥纪录陷阱
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT052_H
#define STDIMPACT052_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact052_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_052,
					MAX_TRAP_COUNT = OWN_IMPACT::MAX_IMPACT_PARAM_NUMBER, 
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_TRAP_START =0,
							IDX_TRAP_END = IDX_TRAP_START + MAX_TRAP_COUNT -1,
						};
				};
				StdImpact052_T() {};
				~StdImpact052_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				ObjID_t	GetTrapByIndex(OWN_IMPACT& rImp, INT nIdx) const;
				VOID SetTrapByIndex(OWN_IMPACT& rImp, INT nIdx, ObjID_t nID) const;
				BOOL AddNewTrap(OWN_IMPACT& rImp, Obj_Character& rMe, ObjID_t nID) const;
				VOID GetTrapCountOfSpecificType(OWN_IMPACT& rImp, Obj_Character& rMe, INT nType, INT& rAllTypeTotal, INT& rThisTypeCount);
			public: //message handler
			protected:
				BOOL SpecialHeartBeatCheck(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			private:
		} SOT_XiaoYaoTraps_T;
	};
};
#endif //STDIMPACT052_H
