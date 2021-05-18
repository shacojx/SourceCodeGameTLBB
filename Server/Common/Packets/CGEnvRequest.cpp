#include "stdafx.h"

#include "CGEnvRequest.h"

BOOL CGEnvRequest::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGEnvRequest::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGEnvRequest::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGEnvRequestHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
