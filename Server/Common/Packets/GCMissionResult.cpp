#include "stdafx.h"
// GCMissionResult.cpp
// 
/////////////////////////////////////////////////////

#include "GCMissionResult.h"

using namespace Packets;


BOOL GCMissionResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_IsFinished), sizeof(BYTE) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMissionResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_IsFinished), sizeof(BYTE) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMissionResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMissionResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

