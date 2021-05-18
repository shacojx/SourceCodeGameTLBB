#include "stdafx.h"
#include "WGCityApplyNewCity.h"

BOOL WGCityApplyNewCity::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PortSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_PlayerGuid), sizeof(GUID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGCityApplyNewCity::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PortSceneID), sizeof(SceneID_t));
	oStream.Write( (CHAR*)(&m_PlayerGuid), sizeof(GUID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGCityApplyNewCity::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGCityApplyNewCityHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
