#include "stdafx.h"



#include "PlayerPool.h"

PlayerPool* g_pPlayerPool=NULL ;

PlayerPool::PlayerPool( )
{
__ENTER_FUNCTION

	m_pPlayer = NULL ;
	m_Position = 0 ;
	m_PlayerCount = 0 ;
	m_MaxPlayerCount = 0 ;

__LEAVE_FUNCTION
}

PlayerPool::~PlayerPool( ) 
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( m_pPlayer ) ;


__LEAVE_FUNCTION
}

BOOL PlayerPool::Init( uint MaxPlayerCount )
{
__ENTER_FUNCTION

	m_pPlayer = new LoginPlayer[MaxPlayerCount] ;
	Assert( m_pPlayer ) ;

	for( UINT i=0; i<MaxPlayerCount; i++ )
	{
		m_pPlayer[i].SetPlayerID( (PlayerID_t)i ) ;
		m_pPlayer[i].SetEmpty(TRUE) ;
	}
	m_Position = 0 ;
	m_PlayerCount = MaxPlayerCount ;
	m_MaxPlayerCount = MaxPlayerCount ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}



LoginPlayer* PlayerPool::NewPlayer( )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	INT iRet = 0 ;
	for( UINT i=0; i<m_MaxPlayerCount; i++ )
	{
		if( m_pPlayer[m_Position].IsEmpty() )
		{
			iRet = m_Position ;
			m_pPlayer[m_Position].SetEmpty( FALSE ) ;

			m_Position ++ ;
			if( m_Position >= m_MaxPlayerCount ) 
				m_Position = 0 ;

			m_PlayerCount -- ;

			return &(m_pPlayer[iRet]) ;
		}

		m_Position ++ ;
		if( m_Position >= m_MaxPlayerCount ) 
			m_Position = 0 ;
	}

	return NULL ;

__LEAVE_FUNCTION

	return NULL ;
}

LoginPlayer* PlayerPool::NewPlayer( PlayerID_t PlayerID)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if(PlayerID>=(PlayerID_t)m_MaxPlayerCount)
	{
		return NULL;
	}


	if(m_pPlayer[PlayerID].IsEmpty())
	{
		LoginPlayer* pPlayer = &(m_pPlayer[PlayerID]);
		pPlayer->SetEmpty(FALSE);
		m_PlayerCount--;
		m_Position = PlayerID;
		m_Position ++ ;
		if( m_Position >= m_MaxPlayerCount ) 
			m_Position = 0 ;

		return pPlayer;

	}

	return NULL ;
	__LEAVE_FUNCTION
	return NULL ;
}

VOID PlayerPool::DelPlayer( PlayerID_t PlayerID )
{
__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( (uint)PlayerID >= m_MaxPlayerCount )
	{
		Assert( FALSE ) ;
		return ;
	}

	m_pPlayer[PlayerID].SetEmpty( TRUE ) ;
	

	m_PlayerCount ++ ;

	return ;

__LEAVE_FUNCTION

	return ;
}

