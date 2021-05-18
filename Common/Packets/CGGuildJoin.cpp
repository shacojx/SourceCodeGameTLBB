#include "stdafx.h"

#include "CGGuildJoin.h"

BOOL CGGuildJoin::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GuildID), sizeof(GuildID_t) );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGGuildJoin::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GuildID), sizeof(GuildID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGGuildJoin::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGGuildJoinHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
