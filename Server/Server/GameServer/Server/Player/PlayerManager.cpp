#include "stdafx.h"

#include "PlayerManager.h"
#include "PlayerPool.h"
#include "Log.h"


PlayerManager::PlayerManager( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	for( INT i=0; i<MAX_PLAYER; i++ )
	{
		m_pPlayers[i] = INVALID_ID ;
	}



//	m_SockTable.InitTable( MAX_PLAYER ) ;

__LEAVE_FUNCTION
}

PlayerManager::~PlayerManager( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

VOID PlayerManager::CleanUp( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	for( INT i=0; i<MAX_PLAYER; i++ )
	{
		m_pPlayers[i] = INVALID_ID ;
	}
//	m_SockTable.CleanUp() ;

__LEAVE_FUNCTION
}

BOOL PlayerManager::AddPlayer( Player* pPlayer )
{
__ENTER_FUNCTION
	
	Assert( pPlayer ) ;
	if( m_nPlayers>=MAX_PLAYER )
	{//管理器数量到达上限
		g_pLog->FastSaveLog( LOG_FILE_2, "ERROR: AddPlayer m_nPlayers>=MAX_PLAYER PID=%d",
			pPlayer->PlayerID() ) ;
		return FALSE ;
	}
	if( pPlayer->PlayerManagerID()!=INVALID_ID )
	{//已经处于某个管理器中
		g_pLog->FastSaveLog( LOG_FILE_2, "ERROR: AddPlayer pPlayer->PlayerManagerID()!=INVALID_ID PID=%d",
			pPlayer->PlayerID() ) ;
		return FALSE ;
	}

	if( m_pPlayers[m_nPlayers] == INVALID_ID )
	{
		m_pPlayers[m_nPlayers] = pPlayer->PlayerID() ;
		pPlayer->SetPlayerManagerID( m_nPlayers ) ;

		m_nPlayers ++ ;
		Assert( m_nPlayers<=MAX_PLAYER ) ;
	}
	else
	{
		g_pLog->FastSaveLog( LOG_FILE_2, "ERROR: AddPlayer() PlayerID=%d,m_nPlayers=%d",
			pPlayer->PlayerID(),m_nPlayers ) ;
		return FALSE ;
	}

//	SOCKET s = pPlayer->GetSocket()->getSOCKET() ;
//	Assert( s!=INVALID_SOCKET ) ;

//	return m_SockTable.Add( (uint)(s), pPlayer ) ;

	return TRUE ;
	
__LEAVE_FUNCTION

	return FALSE ;
}

BOOL PlayerManager::AddPlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	//未提供函数
	Assert( FALSE ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID PlayerManager::RemovePlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( m_nPlayers > 0 ) ;

	Player* pRemovePlayer = g_pPlayerPool->GetPlayer(pid) ;
	if( pRemovePlayer==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_2, "ERROR: pRemovePlayer==NULL PID=%d",
			pid ) ;
		return ;
	}

	ID_t PlayerManagerID = pRemovePlayer->PlayerManagerID() ;
	if( PlayerManagerID < 0 || (UINT)PlayerManagerID >= m_nPlayers )
	{
		g_pLog->FastSaveLog( LOG_FILE_2, "ERROR: RemovePlayer PMID=%d PID=%d",
			PlayerManagerID, pid ) ;
		return ;
	}

	pRemovePlayer->SetPlayerManagerID(INVALID_ID) ;
	if( pid == m_pPlayers[m_nPlayers-1] )
	{//删除最后一个player
		m_pPlayers[m_nPlayers-1] = INVALID_ID ;
	}
	else
	{
		Player* pPlayer = g_pPlayerPool->GetPlayer( m_pPlayers[m_nPlayers-1] ) ;
		if( pPlayer==NULL )
		{
			g_pLog->FastSaveLog( LOG_FILE_2, "ERROR: pPlayer==NULL PID=%d LastPID=%d",
				m_pPlayers[m_nPlayers-1],m_nPlayers-1 ) ;
			return ;
		}

		m_pPlayers[PlayerManagerID] = m_pPlayers[m_nPlayers-1] ;
		m_pPlayers[m_nPlayers-1] = INVALID_ID ;

		pPlayer->SetPlayerManagerID( PlayerManagerID ) ;
	}

	m_nPlayers -- ;
	Assert( m_nPlayers>=0 ) ;

//	m_SockTable.Remove( (uint)fd ) ;

	__LEAVE_FUNCTION
}

/*
Player* PlayerManager::GetPlayer( SOCKET fd )
{
__ENTER_FUNCTION

	Player* pPlayer = (Player*)(m_SockTable.Get((uint)fd)) ;
	return pPlayer ;

__LEAVE_FUNCTION

	return NULL ;
}
*/

BOOL PlayerManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

/*
	BOOL ret ;

	uint nPlayerCount = GetPlayerNumber() ;
	for( uint i=0; i<nPlayerCount; i++ )
	{
		if( m_pPlayers[i] == INVALID_ID )
			continue ;

		Player* pPlayer = g_pPlayerPool->GetPlayer(m_pPlayers[i]) ;
		if( pPlayer==NULL )
		{
			Assert(FALSE) ;
			return FALSE ;
		}
		ret = pPlayer->HeartBeat( ) ;
		Assert( ret ) ;
	}
*/

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}
