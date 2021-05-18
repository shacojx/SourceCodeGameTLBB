#include "stdafx.h"
// CGCharMove.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharMove.h"

BOOL CGCharMove::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read( (CHAR*)(&m_posWorld), sizeof(m_posWorld));
	iStream.Read( (CHAR*)(&m_yNumTargetPos), sizeof(m_yNumTargetPos));
	if ( m_yNumTargetPos>0 && m_yNumTargetPos<=MAX_CHAR_PATH_NODE_NUMBER )
		iStream.Read( (CHAR*)(&m_aTargetPos), sizeof(WORLD_POS) * m_yNumTargetPos );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL CGCharMove::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write( (CHAR*)(&m_posWorld), sizeof(m_posWorld));
	oStream.Write( (CHAR*)(&m_yNumTargetPos), sizeof(m_yNumTargetPos));
	if ( m_yNumTargetPos>0 && m_yNumTargetPos<=MAX_CHAR_PATH_NODE_NUMBER )
		oStream.Write( (CHAR*)(&m_aTargetPos), sizeof(WORLD_POS) * m_yNumTargetPos );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharMove::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCharMoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

