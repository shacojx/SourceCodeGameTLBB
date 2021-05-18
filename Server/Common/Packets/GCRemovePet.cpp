#include "stdafx.h"
// GCRemovePet.cpp

#include "GCRemovePet.h"

using namespace Packets;

BOOL GCRemovePet::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCRemovePet::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCRemovePet::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCRemovePetHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

