#include "stdafx.h"
#include "PlayerManager.h"
#include "PlayerPool.h"


PlayerManager::PlayerManager( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	for( int i=0; i<MAX_PLAYER; i++ )
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

void PlayerManager::CleanUp( )
{
__ENTER_FUNCTION

	m_nPlayers = 0 ;
	for( int i=0; i<MAX_PLAYER; i++ )
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

	if( m_pPlayers[m_nPlayers] == INVALID_ID )
	{
		m_pPlayers[m_nPlayers] = pPlayer->PlayerID() ;
		pPlayer->SetPlayerManagerID( m_nPlayers ) ;

		m_nPlayers ++ ;
		Assert( m_nPlayers<=MAX_PLAYER ) ;
	}
	else
	{
		Assert(FALSE) ;
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

	//Î´Ìá¹©º¯Êý
	Assert( FALSE ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

/*
void PlayerManager::RemovePlayer( SOCKET fd )
{
__ENTER_FUNCTION

	Assert( m_nPlayers > 0 ) ;

	Player* pPlayer = GetPlayer(fd) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	ID_t PlayerManagerID = pPlayer->PlayerManagerID() ;
	if( PlayerManagerID < 0 || PlayerManagerID >= MAX_PLAYER )
	{
		Assert( FALSE ) ;
		return ;
	}

	pPlayer = g_pPlayerPool->GetPlayer( m_pPlayers[m_nPlayers-1] ) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	m_pPlayers[PlayerManagerID] = m_pPlayers[m_nPlayers-1] ;
	m_pPlayers[m_nPlayers-1] = INVALID_ID ;

	pPlayer->SetPlayerManagerID( PlayerManagerID ) ;


	m_nPlayers -- ;
	Assert( m_nPlayers>=0 ) ;

	m_SockTable.Remove( (uint)fd ) ;


__LEAVE_FUNCTION
}
*/

void PlayerManager::RemovePlayer( PlayerID_t pid )
{
__ENTER_FUNCTION

	Assert( m_nPlayers > 0 ) ;

	Player* pPlayer = g_pPlayerPool->GetPlayer(pid) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	ID_t PlayerManagerID = pPlayer->PlayerManagerID() ;
	if( PlayerManagerID < 0 || PlayerManagerID >= MAX_PLAYER )
	{
		Assert( FALSE ) ;
		return ;
	}

	pPlayer = g_pPlayerPool->GetPlayer( m_pPlayers[m_nPlayers-1] ) ;
	if( pPlayer==NULL )
	{
		Assert( FALSE ) ;
		return ;
	}

	m_pPlayers[PlayerManagerID] = m_pPlayers[m_nPlayers-1] ;
	m_pPlayers[m_nPlayers-1] = INVALID_ID ;

	pPlayer->SetPlayerManagerID( PlayerManagerID ) ;


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

BOOL PlayerManager::HeartBeat( DWORD dwTime )
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
