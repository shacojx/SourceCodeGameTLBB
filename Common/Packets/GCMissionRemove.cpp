#include "stdafx.h"
// GCMissionRemove.cpp
// 
/////////////////////////////////////////////////////

#include "GCMissionRemove.h"

using namespace Packets;

BOOL GCMissionRemove::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idMission), sizeof(m_idMission));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMissionRemove::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idMission), sizeof(m_idMission));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMissionRemove::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMissionRemoveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

