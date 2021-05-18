#include "stdafx.h"
#include "WGGuildReturn.h"

BOOL	WGGuildReturn::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildReturn.Read(iStream);
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGGuildReturn::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildReturn.Write(oStream);
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGGuildReturn::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildReturnHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
