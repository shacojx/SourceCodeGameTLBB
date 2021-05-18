#include "stdafx.h"
#include "WGCityCloseSuccess.h"

BOOL WGCityCloseSuccess::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerGuid), sizeof(GUID_t));
	iStream.Read( (CHAR*)(&m_ScenenID), sizeof(SceneID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGCityCloseSuccess::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerGuid), sizeof(GUID_t));
	oStream.Write( (CHAR*)(&m_ScenenID), sizeof(SceneID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGCityCloseSuccess::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGCityCloseSuccessHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


