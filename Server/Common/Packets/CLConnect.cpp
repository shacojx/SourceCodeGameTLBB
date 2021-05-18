#include "stdafx.h"

#include "CLConnect.h"

BOOL CLConnect::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CLConnect::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLConnect::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLConnectHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}
