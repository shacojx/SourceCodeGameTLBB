///////////////////////////////////////////////////////////////////////////////
// 文件名：ActionCallBackForScript.h
// 功能说明：引导动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ACTIONCALLFORSCRIPT_H
#define ACTIONCALLFORSCRIPT_H
#include "Type.h"
#include "ActionCallBack.h"

namespace Action_Module
{
	class ActionCallBackFunctorForScript_T : public ActionCallBackFunctor_T
	{
		public:
			virtual BOOL IsPerformingSpecificScript(ActionParams_T& rParams, ScriptID_t nID) const;
			virtual BOOL CanDoThisActionInThisStatus(ActionParams_T&) const;
			virtual BOOL OnInterrupt(ActionParams_T& rParams) const;
			virtual BOOL OnActivateOnce(ActionParams_T& rParams) const;
			virtual BOOL OnActivateEachTick(ActionParams_T& rParams) const;
			virtual BOOL InterruptWhenDisturbed(ActionParams_T& rParams) const {return TRUE;};
			virtual BOOL OnDisturbForCharging(ActionParams_T& rParams) const;
			virtual BOOL OnDisturbForChanneling(ActionParams_T& rParams) const;
			virtual VOID ResetForNextAction(ActionParams_T& rParams) const;
		protected:
		private:
	};
}
#endif ACTIONCALLFORSCRIPT_H
