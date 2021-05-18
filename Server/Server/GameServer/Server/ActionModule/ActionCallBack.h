///////////////////////////////////////////////////////////////////////////////
// 文件名：ActionCallBack.h
// 功能说明：回调函数类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ACTIONCALLBACK_H
#define ACTIONCALLBACK_H
#include "Type.h"

class Obj_Character;
namespace Action_Module
{
	class ActionCallBackFunctor_T;
	class ActionParams_T
	{
		public:
			//ctor & dtor
			ActionParams_T(VOID) : 
				m_nContinuance(0), 
				m_nInterval(0), 
				m_nIntervalElapsed(0), 
				m_pActor(NULL),
				m_pCallBack(NULL)
			{}
			~ActionParams_T(VOID) {}
			ActionParams_T(ActionParams_T const& rhs) :
				m_nContinuance(rhs.m_nContinuance),
				m_nInterval(rhs.m_nInterval),
				m_nIntervalElapsed(rhs.m_nIntervalElapsed),
				m_pActor(rhs.m_pActor),
				m_pCallBack(rhs.m_pCallBack)
			{}
			ActionParams_T& operator=(ActionParams_T const& rhs)
			{
				m_nContinuance = rhs.GetContinuance();
				m_nInterval = rhs.GetInterval();
				m_nIntervalElapsed = rhs.GetIntervalElapsed();
				m_pActor = rhs.GetActor();
				m_pCallBack = rhs.GetCallBackFunctor();
			}
			VOID Reset(VOID)
			{
				m_nContinuance = 0;
				m_nInterval = 0;
				m_nIntervalElapsed = 0;
				m_pActor = NULL;
				m_pCallBack = NULL;
			}
			//Access function
			Time_t GetContinuance(VOID) const {return m_nContinuance;};
			VOID SetContinuance(Time_t nTime) {m_nContinuance=nTime;};
			Time_t GetInterval(VOID) const {return m_nInterval;};
			VOID SetInterval(Time_t nTime) {m_nInterval=nTime;};
			Time_t GetIntervalElapsed(VOID) const {return m_nIntervalElapsed;};
			VOID SetIntervalElapsed(Time_t nTime) {m_nIntervalElapsed=nTime;};
			Obj_Character* GetActor(VOID) const {return m_pActor;};
			VOID SetActor(Obj_Character* pActor) {m_pActor=pActor;};
			ActionCallBackFunctor_T const* GetCallBackFunctor(VOID) const {return m_pCallBack;}
			VOID SetCallBackFunctor(ActionCallBackFunctor_T const& rCallBack) {m_pCallBack=&rCallBack;};
		protected:
		private:
			Time_t m_nContinuance;
			Time_t m_nInterval;
			Time_t m_nIntervalElapsed;
			Obj_Character* m_pActor;
			ActionCallBackFunctor_T const* m_pCallBack;
	};
	
	class ActionCallBackFunctor_T
	{
		public:
			ActionCallBackFunctor_T(VOID) {};
			~ActionCallBackFunctor_T(VOID) {};
			virtual BOOL IsPerformingSpecificSkill(ActionParams_T& rParams, SkillID_t nID) const {return FALSE;};
			virtual BOOL IsPerformingSpecificScript(ActionParams_T& rParams, ScriptID_t nID) const {return FALSE;};
			virtual BOOL CanDoThisActionInThisStatus(ActionParams_T&) const {return TRUE;}
			virtual BOOL OnInterrupt(ActionParams_T& rParams) const {return FALSE;};
			virtual BOOL OnActivateOnce(ActionParams_T& rParams) const {return FALSE;};
			virtual BOOL OnActivateEachTick(ActionParams_T& rParams) const {return FALSE;};
			virtual BOOL InterruptWhenDisturbed(ActionParams_T& rParams) const {return FALSE;};
			virtual BOOL OnDisturbForCharging(ActionParams_T& rParams) const {return FALSE;};
			virtual BOOL OnDisturbForChanneling(ActionParams_T& rParams) const {return FALSE;};
			virtual VOID ResetForNextAction(ActionParams_T& rParams) const {};
		protected:
		private:
	};

	ActionCallBackFunctor_T const& GetGlobalScriptCallBackFunctor(VOID);
	ActionCallBackFunctor_T const& GetGlobalSkillCallBackFunctor(VOID);
};
#endif //ACTIONCALLBACK_H
