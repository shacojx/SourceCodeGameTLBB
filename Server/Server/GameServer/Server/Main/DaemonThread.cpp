#include "stdafx.h"

#include "DaemonThread.h"
#include "Server.h"
#include "TimeManager.h"
#include "CopySceneManager.h"
#include "DynamicSceneManager.h"
#include "ShareMemManager.h"
#include "PlayerPool.h"
#include "Performance.h"
#include "Log.h"


DaemonThread* g_pDaemonThread = NULL ;

DaemonThread::DaemonThread()
{
}

DaemonThread::~DaemonThread()
{
}

BOOL DaemonThread::Init( )
{
	return TRUE ;
}

VOID DaemonThread::CleanUp( )
{
}

BOOL DaemonThread::Loop( )
{
__ENTER_FUNCTION

	while( IsActive() )
	{
		BOOL ret = FALSE ;
		UINT uTime = g_pTimeManager->CurrentTime() ;//当前时间
		if( !m_WorkingTime.IsSetTimer() )
		{
			m_WorkingTime.BeginTimer( 1000, uTime ) ;
			m_FlushLogTimer.BeginTimer( 5000, uTime ) ;
		}
		if( !m_WorkingTime.CountingTimer(uTime) )
		{
			MySleep(100) ;
			continue ;
		}

		//日志刷入文件
		_MY_TRY
		{
			if( m_FlushLogTimer.CountingTimer(uTime) && g_pLog )
			{
				UINT daytime = g_pTimeManager->GetDayTime() ;
				if( daytime != g_pLog->GetDayTime() )
				{
					g_pLog->SetDayTime( daytime ) ;
				}
				g_pLog->FlushLog_All( ) ;
			}
		}
		_MY_CATCH
		{
		}

		//副本场景管理管理器的逻辑调用
		_MY_TRY
		{
			if( g_pCopySceneManager )
				g_pCopySceneManager->HeartBeat( uTime ) ;
		}
		_MY_CATCH
		{
		}

		//动态场景管理管理器的逻辑调用______________________________
		//_MY_TRY
		//{
		//	if( g_pDynamicSceneManager )
		//		g_pDynamicSceneManager->HeartBeat( uTime ) ;
		//}
		//_MY_CATCH
		//{
		//}

		//数据监控逻辑调用
		_MY_TRY
		{
			if( g_pPerformanceManager )
				g_pPerformanceManager->HeartBeat( uTime ) ;
	
			g_ShareMemNotifyer.HeartBeat(uTime);
		}
		_MY_CATCH
		{
		}
		
		//定时关闭服务器处理
		if( g_Server.m_TimeToQuit.IsSetTimer() )
		{
			if( g_Server.m_TimeToQuit.CountingTimer(uTime) )
			{
				g_Server.Stop( ) ;
			}
		}
	};

	Quit( ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID DaemonThread::Quit( )
{
}
