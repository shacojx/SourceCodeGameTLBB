#include "stdafx.h"
// GCMissionAdd.cpp
// 
/////////////////////////////////////////////////////

#include "GCMissionAdd.h"

using namespace Packets;

BOOL GCMissionAdd::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Mission), sizeof(m_Mission));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMissionAdd::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Mission), sizeof(m_Mission));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMissionAdd::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCMissionAddHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

