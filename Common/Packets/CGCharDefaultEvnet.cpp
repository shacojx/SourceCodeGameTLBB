#include "stdafx.h"
#include "CGCharDefaultEvent.h"

using namespace Packets;

BOOL CGCharDefaultEvent::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCharDefaultEvent::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharDefaultEvent::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCharDefaultEventHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}