#include "stdafx.h"

#include "CGEnterScene.h"

BOOL CGEnterScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_byEnterType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGEnterScene::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_byEnterType), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_nSceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGEnterScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGEnterSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
