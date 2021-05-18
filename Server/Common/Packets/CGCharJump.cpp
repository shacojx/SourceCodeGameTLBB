#include "stdafx.h"
// CGCharJump.cpp
// 
/////////////////////////////////////////////////////

#include "CGCharJump.h"

BOOL CGCharJump::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCharJump::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCharJump::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCharJumpHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

