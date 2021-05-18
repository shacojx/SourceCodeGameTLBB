#include "stdafx.h"
// CGAskDetailAttrib.cpp
// 
/////////////////////////////////////////////////////

#include "CGAskDetailAttrib.h"

BOOL CGAskDetailAttrib::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskDetailAttrib::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskDetailAttrib::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskDetailAttribHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

