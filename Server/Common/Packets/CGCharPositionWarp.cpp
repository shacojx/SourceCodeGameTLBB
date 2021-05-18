#include "stdafx.h"
// CGCharPositionWarp.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharPositionWarp.h"

BOOL CGCharPositionWarp::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_posServer), sizeof(m_posServer));
	iStream.Read( (CHAR*)(&m_posClient), sizeof(m_posClient));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL CGCharPositionWarp::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_posServer), sizeof(m_posServer));
	oStream.Write( (CHAR*)(&m_posClient), sizeof(m_posClient));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharPositionWarp::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCharPositionWarpHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

