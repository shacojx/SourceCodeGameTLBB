#include "stdafx.h"

#include "GCLeaveScene.h"

BOOL GCLeaveScene::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_byLeaveCode), sizeof(BYTE));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCLeaveScene::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_byLeaveCode), sizeof(BYTE) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCLeaveScene::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCLeaveSceneHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}
