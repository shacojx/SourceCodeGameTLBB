#include "stdafx.h"
#include "GWCityOpt.h"

BOOL GWCityOpt::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_CityGuid), sizeof(_CITY_GUID));
	iStream.Read( (CHAR*)(m_iParam), sizeof(INT)*MAX_IPARAM_NUM);
	iStream.Read( (CHAR*)(m_szParam), sizeof(CHAR)*MAX_SZPARAM_LENGTH);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWCityOpt::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION
	

	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_CityGuid), sizeof(_CITY_GUID));
	oStream.Write( (CHAR*)(m_iParam), sizeof(INT)*MAX_IPARAM_NUM);
	oStream.Write( (CHAR*)(m_szParam), sizeof(CHAR)*MAX_SZPARAM_LENGTH);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWCityOpt::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCityOptHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


