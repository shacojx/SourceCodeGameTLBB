///////////////////////////////////////////////////////////////////////////////
// 文件名：ChannelAction.h
// 功能说明：引导动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CHANNELACTION_H
#define CHANNELACTION_H
#include "Type.h"
#include "ActionLogic.h"

namespace Action_Module
{
	class ChannelActionLogic_T : public ActionLogic_T
	{
		public:
			enum
			{
				ID = ACTION_CHANNEL,
			};
			ChannelActionLogic_T(VOID) {};
			~ChannelActionLogic_T(VOID) {};
			virtual INT GetLogicID(VOID) const {return ID;}
			virtual BOOL HeartBeat(ActionParams_T& rParams, Time_t nDeltaTime) const;
			virtual BOOL OnDisturb(ActionParams_T& rParams) const;
			virtual BOOL OnInterrupt(ActionParams_T& rParams) const;
		protected:
		private:
	};
}
#endif //CHANNELACTION_H
