#include "stdafx.h"

#include "CGLeaveScene.h"

BOOL CGLeaveScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGLeaveScene::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGLeaveScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGLeaveSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
