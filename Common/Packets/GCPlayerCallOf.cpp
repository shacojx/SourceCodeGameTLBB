#include "stdafx.h"
// GCPlayerCallOf.cpp

#include "GCPlayerCallOf.h"

using namespace Packets;

BOOL GCPlayerCallOf::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_uTime), sizeof(m_uTime));
	iStream.Read( (CHAR*)(&m_guidCaller), sizeof(m_guidCaller));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCPlayerCallOf::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_uTime), sizeof(m_uTime));
	oStream.Write( (CHAR*)(&m_guidCaller), sizeof(m_guidCaller));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCPlayerCallOf::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCPlayerCallOfHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

