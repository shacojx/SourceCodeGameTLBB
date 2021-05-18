#include "stdafx.h"
// GCStallEstablish.cpp
// 
/////////////////////////////////////////////////////

#include "GCStallEstablish.h"

BOOL GCStallEstablish::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCStallEstablish::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCStallEstablish::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCStallEstablishHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

