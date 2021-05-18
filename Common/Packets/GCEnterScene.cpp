#include "stdafx.h"

#include "GCEnterScene.h"

BOOL GCEnterScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_byRet), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_bIsCity), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nCityLevel), sizeof(BYTE));



	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCEnterScene::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_byRet), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(&m_nSceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(WORLD_POS) ) ;
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_bIsCity), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nCityLevel), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCEnterScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCEnterSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
