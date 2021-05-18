#include "stdafx.h"


#include "CGAskChangeScene.h"



BOOL CGAskChangeScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SourSceneID), sizeof(SceneID_t) ) ;
	iStream.Read( (CHAR*)(&m_DestSceneID), sizeof(SceneID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskChangeScene::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_SourSceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_DestSceneID), sizeof(SceneID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskChangeScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskChangeSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


