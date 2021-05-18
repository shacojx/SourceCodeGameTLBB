#include "stdafx.h"

#include "GCNotifyChangeScene.h"

BOOL GCNotifyChangeScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_CurrentSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_TargetSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_TargetPos), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_TargetDir), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_Flag), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNotifyChangeScene::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_CurrentSceneID), sizeof(SceneID_t));
	oStream.Write( (CHAR*)(&m_TargetSceneID), sizeof(SceneID_t));
	oStream.Write( (CHAR*)(&m_TargetPos), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_TargetDir), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_Flag), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNotifyChangeScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNotifyChangeSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

