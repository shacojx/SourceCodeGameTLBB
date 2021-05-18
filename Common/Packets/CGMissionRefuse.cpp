#include "stdafx.h"
// CGMissionRefuse.cpp
// 
/////////////////////////////////////////////////////

#include "CGMissionRefuse.h"

using namespace Packets;

BOOL CGMissionRefuse::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGMissionRefuse::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC) ) ;
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGMissionRefuse::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMissionRefuseHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

