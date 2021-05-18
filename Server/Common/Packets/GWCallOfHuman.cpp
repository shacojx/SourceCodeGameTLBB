#include "stdafx.h"
#include "stdafx.h"
// GWCallOfHuman.cpp

#include "GWCallOfHuman.h"

using namespace Packets;

BOOL GWCallOfHuman::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_guidTarget), sizeof(m_guidTarget) );
	iStream.Read( (CHAR*)(&m_infoCallOf), sizeof(m_infoCallOf) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWCallOfHuman::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_guidTarget), sizeof(m_guidTarget) );
	oStream.Write( (CHAR*)(&m_infoCallOf), sizeof(m_infoCallOf) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWCallOfHuman::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWCallOfHumanHandler::Execute( this, pPlayer );
	
__LEAVE_FUNCTION

	return FALSE;
}
