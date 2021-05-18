#include "stdafx.h"
// CGMissionAccept.cpp
// 
/////////////////////////////////////////////////////

#include "CGMissionAccept.h"

using namespace Packets;

BOOL CGMissionAccept::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGMissionAccept::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGMissionAccept::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMissionAcceptHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

