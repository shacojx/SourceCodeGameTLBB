///////////////////////////////////////////////////////////////////////////////
// 文件名：ActionDelegator.h
// 功能说明：动作的代理类 , 角色动作的公共接口和常用功能
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ACTIONDELEGATOR_H
#define ACTIONDELEGATOR_H
#include "Type.h"
#include "GameDefine.h"
#include "GameDefine2.h"

#include "ActionCallBack.h"

class Obj_Character;

namespace Action_Module
{
	using namespace Action_Module;
	class ActionDelegator_T
	{
		friend class ActionCallBackFunctorForScript_T;
		friend class ActionCallBackFunctorForSkill_T;
		friend class ActionCallBackFunctor_T;
		public:
			ActionDelegator_T(VOID) {};
			~ActionDelegator_T(VOID) {};
		// 动作管理和规则在这里实现
		public:
			//注:这里是设置action参数的接口，不是设置角色逻辑状态状态的接口
			//因此里面不负责设置服务器角色的逻辑状态
			BOOL IsPerformingTheSpecificSkill(Obj_Character& rActor, SkillID_t nSkillID) const;
			BOOL IsPerformingTheSpecificScript(Obj_Character& rActor, ScriptID_t nScriptID) const;
			BOOL IsChanneling(Obj_Character& rActor) const;
			BOOL RegisterChargeActionForSkill(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const;
			BOOL RegisterChargeActionForScript(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const;
			BOOL RegisterChannelActionForSkill(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance, Time_t nInterval) const;
			BOOL RegisterChannelActionForScript(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance, Time_t nInterval) const;
			BOOL RegisterInstantActionForSkill(Obj_Character& rActor, ActionID_t nAction, Time_t nActionTime) const;
			BOOL RegisterInstantActionForScript(Obj_Character& rActor, ActionID_t nAction, Time_t nActionTime) const;
			BOOL RegisterEmoteAction(Obj_Character& rActor, ActionID_t nAction) const;
			BOOL DisturbCurrentAction(Obj_Character& rActor) const;
			BOOL OnHeartBeat(Obj_Character& rActor, Time_t nDeltaTime) const;
			BOOL CanDoNextAction(Obj_Character& rActor) const;
			VOID InterruptCurrentAction(Obj_Character& rActor) const;
			VOID CurrentActionFinished(Obj_Character& rActor) const;
		protected:
			VOID BroadcastUnitStartChargeAction(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const;
			VOID BroadcastUnitStartChannelAction(Obj_Character& rActor, ActionID_t nAction, Time_t nContinuance) const;
			VOID BroadcastUnitStartInstantAction(Obj_Character& rActor, ActionID_t nAction, Time_t nActionTime) const;
			VOID BroadcastUnitChargeTimeChanged(Obj_Character& rActor, Time_t nDeltaTime) const;
			VOID BroadcastUnitChannelTimeChanged(Obj_Character& rActor, Time_t nDeltaTime) const;
		private:
			VOID ResetForNextAction(Obj_Character& rActor) const;
	};
	ActionDelegator_T const& GetGlobalActionDelegator(VOID);

};
#endif //ACTIONDELEGATOR_H
