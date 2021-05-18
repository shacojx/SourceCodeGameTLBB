#include "stdafx.h"
// CGExchangeReplyI.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeReplyI.h"

BOOL CGExchangeReplyI::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeReplyI::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeReplyI::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeReplyIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

