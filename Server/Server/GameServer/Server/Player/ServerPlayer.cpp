#include "stdafx.h"


#include "ServerPlayer.h"
#include "PacketFactoryManager.h"


ServerPlayer::ServerPlayer( BOOL bIsServer ) : Player( bIsServer ) 
{
__ENTER_FUNCTION

	m_Status = 0 ;

__LEAVE_FUNCTION
}

ServerPlayer::~ServerPlayer( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL ServerPlayer::ProcessCommand( BOOL Option )
{
__ENTER_FUNCTION

	return Player::ProcessCommand( Option ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::ProcessInput( )
{
__ENTER_FUNCTION

	return Player::ProcessInput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::ProcessOutput( )
{
__ENTER_FUNCTION

	return Player::ProcessOutput( ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::SendPacket( Packet* pPacket )
{
__ENTER_FUNCTION

	return Player::SendPacket( pPacket ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ServerPlayer::IsValid( )
{
__ENTER_FUNCTION

	if( !Player::IsValid( ) )
		return FALSE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID ServerPlayer::CleanUp( )
{
__ENTER_FUNCTION

	Player::CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL ServerPlayer::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION


	return Player::HeartBeat(uTime ) ;

__LEAVE_FUNCTION

	return FALSE ;
}