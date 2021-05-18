#include "stdafx.h"
// CGExchangeOkIII.cpp
// 
/////////////////////////////////////////////////////

#include "CGExchangeOkIII.h"

BOOL CGExchangeOkIII::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGExchangeOkIII::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGExchangeOkIII::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGExchangeOkIIIHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

