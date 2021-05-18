#include "stdafx.h"


#include "WGRetChangeScene.h"



BOOL WGRetChangeScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_SceneID), sizeof(SceneID_t) ) ;
	iStream.Read( (CHAR*)(&m_uKey), sizeof(UINT) ) ;
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGRetChangeScene::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_SceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_uKey), sizeof(UINT) ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGRetChangeScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGRetChangeSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


