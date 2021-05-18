#include "stdafx.h"
#include "WGCityError.h"

BOOL WGCityError::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ErrorID), sizeof(BYTE));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGCityError::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ErrorID), sizeof(BYTE));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGCityError::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGCityErrorHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


