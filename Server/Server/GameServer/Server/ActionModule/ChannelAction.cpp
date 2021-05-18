#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ChannelAction.cpp
// 功能说明：引导动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "ChannelAction.h"
#include "ActionCallBack.h"

namespace Action_Module
{
	//class ChannelActionLogic_T : ActionLogic_T
	BOOL ChannelActionLogic_T::HeartBeat(ActionParams_T& rParams, Time_t nDeltaTime) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL==pCallBack)
		{
			return FALSE;
		}
		if(FALSE==pCallBack->CanDoThisActionInThisStatus(rParams))
		{
			return FALSE;
		}
		Time_t nIntervalElapsed = rParams.GetIntervalElapsed();
		Time_t nContinuance = rParams.GetContinuance();
		Time_t nInterval = rParams.GetInterval();
		BOOL bRet = TRUE;
		if(500>nInterval)
		{
			nInterval = 2000;
		}
		if(nDeltaTime>nContinuance)
		{
			nDeltaTime = nContinuance;
		}
		nIntervalElapsed += nDeltaTime;
		nContinuance -= nDeltaTime;
		if(0>nContinuance)
		{
			nContinuance = 0;
		}
		rParams.SetContinuance(nContinuance);
		while (nIntervalElapsed >= nInterval)
		{
			nIntervalElapsed-= nInterval;
			bRet = pCallBack->OnActivateEachTick(rParams);
			if(FALSE==bRet)
			{
				break;
			}
		}
		rParams.SetIntervalElapsed(nIntervalElapsed);
		return bRet;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ChannelActionLogic_T::OnDisturb(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			return pCallBack->OnDisturbForChanneling(rParams);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}
	BOOL ChannelActionLogic_T::OnInterrupt(ActionParams_T& rParams) const
	{
		__ENTER_FUNCTION
		ActionCallBackFunctor_T const* pCallBack = rParams.GetCallBackFunctor();
		if(NULL!=pCallBack)
		{
			return pCallBack->OnInterrupt(rParams);
		}
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	}

}

