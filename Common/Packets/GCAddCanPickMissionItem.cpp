#include "stdafx.h"
// GCAddCanPickMissionItem.cpp

#include "GCAddCanPickMissionItem.h"

using namespace Packets;

BOOL GCAddCanPickMissionItem::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_uItemDataID), sizeof(m_uItemDataID) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCAddCanPickMissionItem::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_uItemDataID), sizeof(m_uItemDataID) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCAddCanPickMissionItem::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAddCanPickMissionItemHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

