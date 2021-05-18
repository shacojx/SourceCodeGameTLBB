#include "stdafx.h"


#include "GWAskChangeScene.h"



BOOL GWAskChangeScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Status), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	iStream.Read( (CHAR*)(&m_SourSceneID), sizeof(SceneID_t) ) ;
	iStream.Read( (CHAR*)(&m_DestSceneID), sizeof(SceneID_t) ) ;
	if( m_Status==CSS_DIFFSERVER )
		iStream.Read( (CHAR*)(&m_UserData), sizeof(FULLUSERDATA) ) ;
	iStream.Read( (CHAR*)(&m_uKey), sizeof(UINT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWAskChangeScene::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Status), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_SourSceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_DestSceneID), sizeof(SceneID_t) ) ;
	if( m_Status==CSS_DIFFSERVER )
		oStream.Write( (CHAR*)(&m_UserData), sizeof(FULLUSERDATA) ) ;
	oStream.Write( (CHAR*)(&m_uKey), sizeof(UINT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWAskChangeScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWAskChangeSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


