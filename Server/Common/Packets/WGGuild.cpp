#include "stdafx.h"
#include "WGGuild.h"

BOOL WGGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildPacket.Read( iStream );
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL WGGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildPacket.Write( oStream );
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT WGGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
