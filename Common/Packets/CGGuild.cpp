#include "stdafx.h"
#include "CGGuild.h"

BOOL CGGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
