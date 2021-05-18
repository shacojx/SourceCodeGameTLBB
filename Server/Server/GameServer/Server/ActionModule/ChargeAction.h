///////////////////////////////////////////////////////////////////////////////
// 文件名：ChargeAction.h
// 功能说明：聚气动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CHARGEACTION_H
#define CHARGEACTION_H
#include "Type.h"
#include "ActionLogic.h"

namespace Action_Module
{
	class ChargeActionLogic_T : public ActionLogic_T
	{
		public:
			enum
			{
				ID = ACTION_CHARGE,
			};
			ChargeActionLogic_T(VOID) {};
			~ChargeActionLogic_T(VOID) {};
			virtual INT GetLogicID(VOID) const {return ID;}
			virtual BOOL HeartBeat(ActionParams_T& rParams, Time_t nDeltaTime) const;
			virtual BOOL OnDisturb(ActionParams_T& rParams) const;
			virtual BOOL OnInterrupt(ActionParams_T& rParams) const;
		protected:
		private:
	};
	
}
#endif //CHARGEACTION_H
