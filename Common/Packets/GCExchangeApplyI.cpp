#include "stdafx.h"
// GCExchangeApplyI.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeApplyI.h"

BOOL GCExchangeApplyI::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeApplyI::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeApplyI::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeApplyIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

