#include "stdafx.h"
// GCStallClose.cpp
// 
/////////////////////////////////////////////////////

#include "GCStallClose.h"

BOOL GCStallClose::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCStallClose::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCStallClose::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCStallCloseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

