#include "stdafx.h"
#include "GCCityAttr.h"

BOOL GCCityAttr::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_uFlags), sizeof(UINT));

	if ( m_uFlags & (1 << CITY_UPDATE_CITY_LEVEL) )
	{
		iStream.Read( (CHAR*)(&m_Level), sizeof(BYTE));
	}

	if ( m_uFlags & (1 << CITY_UPDATE_CITY_NAME) )
	{
		iStream.Read( (CHAR*)(m_szCityName), MAX_CITY_NAME_SIZE);
	}

	if ( m_uFlags & (1 << CITY_UPDATE_BUILDING) )
	{
		iStream.Read( (CHAR*)(&m_BuildingIndex), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_BuildingData), sizeof(CityBuilding_t));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCityAttr::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_uFlags), sizeof(UINT));

	if ( m_uFlags & (1 << CITY_UPDATE_CITY_LEVEL) )
	{
		oStream.Write( (CHAR*)(&m_Level), sizeof(BYTE));
	}

	if ( m_uFlags & (1 << CITY_UPDATE_CITY_NAME) )
	{
		oStream.Write( (CHAR*)(m_szCityName), MAX_CITY_NAME_SIZE);
	}

	if ( m_uFlags & (1 << CITY_UPDATE_BUILDING) )
	{
		oStream.Write( (CHAR*)(&m_BuildingIndex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_BuildingData), sizeof(CityBuilding_t));
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCityAttr::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCityAttrHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


