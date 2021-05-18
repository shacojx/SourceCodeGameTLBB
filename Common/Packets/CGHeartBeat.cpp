#include "stdafx.h"
#include "CGHeartBeat.h"


BOOL CGHeartBeat::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGHeartBeat::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGHeartBeat::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGHeartBeatHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


