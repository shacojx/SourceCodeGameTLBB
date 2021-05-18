#include "stdafx.h"
// CGStallClose.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallClose.h"

BOOL CGStallClose::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallClose::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallClose::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallCloseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

