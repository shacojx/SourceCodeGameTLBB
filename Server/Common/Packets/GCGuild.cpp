#include "stdafx.h"
#include "GCGuild.h"

BOOL GCGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GCGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
