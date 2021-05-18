#include "stdafx.h"
#include "GWCityApplyNewCity.h"

BOOL GWCityApplyNewCity::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PortSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_ApplicantGuid), sizeof(GUID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWCityApplyNewCity::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PortSceneID), sizeof(SceneID_t));
	oStream.Write( (CHAR*)(&m_ApplicantGuid), sizeof(GUID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWCityApplyNewCity::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCityApplyNewCityHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


