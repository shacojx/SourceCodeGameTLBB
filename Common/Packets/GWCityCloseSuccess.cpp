#include "stdafx.h"
#include "GWCityCloseSuccess.h"

BOOL GWCityCloseSuccess::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_CityGuid), sizeof(_CITY_GUID));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWCityCloseSuccess::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_CityGuid), sizeof(_CITY_GUID));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWCityCloseSuccess::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCityCloseSuccessHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


