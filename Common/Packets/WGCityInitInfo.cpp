#include "stdafx.h"
#include "WGCityInitInfo.h"

BOOL WGCityInitInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGCityInitInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGCityInitInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGCityInitInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
