#include "stdafx.h"
// GCRemoveCanPickMissionItem.cpp

#include "GCRemoveCanPickMissionItem.h"

using namespace Packets;

BOOL GCRemoveCanPickMissionItem::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_uItemDataID), sizeof(m_uItemDataID) );
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCRemoveCanPickMissionItem::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_uItemDataID), sizeof(m_uItemDataID) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCRemoveCanPickMissionItem::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCRemoveCanPickMissionItemHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

