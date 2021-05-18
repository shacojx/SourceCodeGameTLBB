#include "stdafx.h"

#include "CGGuildApply.h"

BOOL CGGuildApply::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GuildNameSize), sizeof(BYTE) );
	if(m_GuildNameSize<MAX_GUILD_NAME_SIZE)
	{
		iStream.Read( (CHAR*)(m_GuildName), m_GuildNameSize );
	}
	iStream.Read( (CHAR*)(&m_GuildDescSize), sizeof(BYTE) );
	if(m_GuildDescSize<MAX_GUILD_DESC_SIZE)
	{
		iStream.Read( (CHAR*)(m_GuildDesc), m_GuildDescSize);
	}

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGGuildApply::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_GuildNameSize), sizeof(BYTE) );
	if(m_GuildNameSize<MAX_GUILD_NAME_SIZE)
	{
		oStream.Write( (CHAR*)(m_GuildName), m_GuildNameSize );
	}
	oStream.Write( (CHAR*)(&m_GuildDescSize), sizeof(BYTE) );
	if(m_GuildDescSize<MAX_GUILD_DESC_SIZE)
	{
		oStream.Write( (CHAR*)(m_GuildDesc), m_GuildDescSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGGuildApply::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGGuildApplyHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
