#include "stdafx.h"
// CGMissionAbandon.cpp
// 
/////////////////////////////////////////////////////

#include "CGMissionAbandon.h"

using namespace Packets;

BOOL CGMissionAbandon::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idMissionScript), sizeof(ScriptID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGMissionAbandon::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idMissionScript), sizeof(ScriptID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGMissionAbandon::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMissionAbandonHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

