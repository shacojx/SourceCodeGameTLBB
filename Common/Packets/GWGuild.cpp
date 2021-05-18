#include "stdafx.h"
#include "GWGuild.h"

BOOL GWGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
