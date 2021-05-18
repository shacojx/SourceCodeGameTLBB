#include "stdafx.h"
// CGExchangeCancel.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeCancel.h"

BOOL CGExchangeCancel::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeCancel::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeCancel::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeCancelHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

