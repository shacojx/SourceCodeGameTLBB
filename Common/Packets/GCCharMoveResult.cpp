#include "stdafx.h"
// GCCharMoveResult.cpp

#include "GCCharMoveResult.h"

BOOL GCCharMoveResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	iStream.Read( (CHAR*)(&m_nResult), sizeof(m_nResult));
	iStream.Read( (CHAR*)(&m_yNumTargetPos), sizeof(m_yNumTargetPos));
	if ( m_yNumTargetPos>0 && m_yNumTargetPos<=MAX_CHAR_PATH_NODE_NUMBER )
		iStream.Read( (CHAR*)(&m_aTargetPos), sizeof(WORLD_POS) * m_yNumTargetPos );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharMoveResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nHandleID), sizeof(m_nHandleID));
	oStream.Write( (CHAR*)(&m_nResult), sizeof(m_nResult));
	oStream.Write( (CHAR*)(&m_yNumTargetPos), sizeof(m_yNumTargetPos));
	if ( m_yNumTargetPos>0 && m_yNumTargetPos<=MAX_CHAR_PATH_NODE_NUMBER )
		oStream.Write( (CHAR*)(&m_aTargetPos), sizeof(WORLD_POS) * m_yNumTargetPos );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharMoveResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharMoveResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

