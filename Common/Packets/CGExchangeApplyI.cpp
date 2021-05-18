#include "stdafx.h"
// CGExchangeApplyI.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeApplyI.h"

BOOL CGExchangeApplyI::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeApplyI::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeApplyI::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeApplyIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

