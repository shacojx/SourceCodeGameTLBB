#include "stdafx.h"
#include "GWCityAskInitInfo.h"

BOOL GWCityAskInitInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWCityAskInitInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWCityAskInitInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCityAskInitInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


