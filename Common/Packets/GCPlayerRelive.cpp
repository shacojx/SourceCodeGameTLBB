#include "stdafx.h"
// GCPlayerRelive.cpp

#include "GCPlayerRelive.h"

using namespace Packets;

BOOL GCPlayerRelive::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCPlayerRelive::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCPlayerRelive::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPlayerReliveHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

