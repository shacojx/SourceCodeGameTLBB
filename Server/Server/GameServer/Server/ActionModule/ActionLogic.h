///////////////////////////////////////////////////////////////////////////////
// 文件名：VirtualAction.h
// 功能说明：动作的基础类，声明动作的公共接口和常用功能
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef VIRTUALACTION_H
#define VIRTUALACTION_H
#include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "ActionCallBack.h"

namespace Action_Module
{
	enum
	{
		ACTION_INVALID = -1,
		ACTION_CHARGE = 0,
		ACTION_CHANNEL = 1,
		ACTION_INSTANT = 2,
	};
	class ActionLogic_T
	{
		public:
			enum
			{
				ID = ACTION_INVALID,
			};
			ActionLogic_T(VOID) {};
			~ActionLogic_T(VOID) {};
			virtual INT GetLogicID(VOID) const {return ID;}
			virtual BOOL IsPerformingSpecificSkill(ActionParams_T& rParams, SkillID_t nID) const;
			virtual BOOL IsPerformingSpecificScript(ActionParams_T& rParams, ScriptID_t nID) const;
			virtual BOOL HeartBeat(ActionParams_T& rParams, Time_t nDeltaTime) const{return FALSE;};
			virtual BOOL OnDisturb(ActionParams_T& rParams) const {return FALSE;};
			virtual BOOL OnInterrupt(ActionParams_T& rParams) const {return FALSE;};
		protected:
		private:
	};
	ActionLogic_T const& GetGlobalChargeActionLogic(VOID);
	ActionLogic_T const& GetGlobalChannelActionLogic(VOID);
	ActionLogic_T const& GetGlobalInstantActionLogic(VOID);
};
#endif //VRITUALACTION_H
