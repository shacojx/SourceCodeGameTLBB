#include "stdafx.h"
#include "GWCityClose.h"

BOOL GWCityClose::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_CityGuid), sizeof(_CITY_GUID));
	iStream.Read( (CHAR*)(&m_PlayerGuid), sizeof(GUID_t));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWCityClose::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_CityGuid), sizeof(_CITY_GUID));
	oStream.Write( (CHAR*)(&m_PlayerGuid), sizeof(GUID_t));
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWCityClose::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCityCloseHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


