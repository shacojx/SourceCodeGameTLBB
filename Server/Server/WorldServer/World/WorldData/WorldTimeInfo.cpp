#include "stdafx.h"
#include "WorldTimeInfo.h"
#include "WGWorldTime.h"
#include "ServerManager.h"


WorldTimeInfo*	g_pWorldTimeInfo = NULL;



WorldTimeInfo::WorldTimeInfo()
{

}

WorldTimeInfo::~WorldTimeInfo()
{

}


BOOL WorldTimeInfo::Init()
{
	__ENTER_FUNCTION

		m_WorldTime = WT_IND_1; //子时
		m_LoginTimer.CleanUp( ) ;

		return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}

VOID WorldTimeInfo::CleanUp()
{

}


BOOL WorldTimeInfo::HeartBeat(UINT uTime)
{
	__ENTER_FUNCTION

		if( !m_LoginTimer.IsSetTimer() )
		{
			m_LoginTimer.BeginTimer( g_Config.m_ConfigInfo.m_TimeChangeInterval, uTime ) ;
			return TRUE ;
		}

		if( !m_LoginTimer.CountingTimer(uTime) )
		{//时间没到
			return TRUE ;
		}

		if(m_WorldTime == WT_IND_12)
		{
			m_WorldTime = WT_IND_1;
		}
		else
		{
			m_WorldTime = (WORLD_TIME)(m_WorldTime+1);
		}
		
		WGWorldTime	Msg;
		Msg.SetWorldTime(m_WorldTime);
		g_pServerManager->BroadCastServer( &Msg ) ;
		
		return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}