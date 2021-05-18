#include "stdafx.h"
// GCExchangeCancel.cpp
// 
/////////////////////////////////////////////////////

#include "GCExchangeCancel.h"

BOOL GCExchangeCancel::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCExchangeCancel::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCExchangeCancel::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCExchangeCancelHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

