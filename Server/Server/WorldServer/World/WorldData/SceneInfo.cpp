#include "stdafx.h"

#include "SceneInfo.h"
#include "Config.h"
#include "ServerManager.h"
#include "SSScenePlayerCount.h"
#include "OnlineUser.h"



SceneInfo* g_pSceneInfo=NULL ;

SceneInfo::SceneInfo( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_aScenePlayerCount[i]=0 ;
		m_aScenePlayerCount_Bak[i]=0 ;
	}

__LEAVE_FUNCTION
}

SceneInfo::~SceneInfo( )
{
}

BOOL SceneInfo::Init( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_SCENE; i++ )
	{
		m_aScenePlayerCount[i]=0 ;
		m_aScenePlayerCount_Bak[i]=0 ;
	}
	m_LoginTimer.CleanUp( ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID SceneInfo::CleanUp( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

BOOL SceneInfo::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	if( !m_LoginTimer.IsSetTimer() )
	{
		m_LoginTimer.BeginTimer( 2000, uTime ) ;
		return TRUE ;
	}
	if( !m_LoginTimer.CountingTimer(uTime) )
	{//时间没到
		return TRUE ;
	}

	SSScenePlayerCount Msg ;
	for( INT i=0; i<MAX_SCENE; i++ )
	{
		INT nIndex = g_Config.m_SceneInfo.m_HashScene[i] ;
		_SCENE_DATA* pData = &(g_Config.m_SceneInfo.m_pScene[nIndex]) ;
		if( pData->m_Type != SCENE_TYPE_GAMELOGIC )
			continue ;

		if( m_aScenePlayerCount[i] != m_aScenePlayerCount_Bak[i] )
		{
			m_aScenePlayerCount_Bak[i] = m_aScenePlayerCount[i] ;
			Msg.AddChangeScene( i, m_aScenePlayerCount[i] ) ;
		}
	}
	if( Msg.GetChangeCount() > 0 )
	{
		Msg.SetTotalPlayerCount( g_pOnlineUser->GetTotalCount() ) ;
		g_pServerManager->BroadCast( &Msg ) ;
	}


__LEAVE_FUNCTION

	return TRUE ;
}

