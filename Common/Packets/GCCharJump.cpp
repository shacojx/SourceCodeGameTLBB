#include "stdafx.h"

#include "GCCharJump.h"

BOOL GCCharJump::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharJump::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharJump::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharJumpHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

