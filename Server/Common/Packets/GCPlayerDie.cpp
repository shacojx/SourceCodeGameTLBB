#include "stdafx.h"
// GCPlayerDie.cpp

#include "GCPlayerDie.h"

using namespace Packets;

BOOL GCPlayerDie::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bCanRelive), sizeof(m_bCanRelive));
	iStream.Read( (CHAR*)(&m_uTime), sizeof(m_uTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCPlayerDie::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_bCanRelive), sizeof(m_bCanRelive));
	oStream.Write( (CHAR*)(&m_uTime), sizeof(m_uTime));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCPlayerDie::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPlayerDieHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

