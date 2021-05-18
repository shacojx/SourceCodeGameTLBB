#include "stdafx.h"
#include "WGTeamList.h"

BOOL	WGTeamList::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)&m_TeamID, sizeof(m_TeamID) );
	iStream.Read( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_Members[i].Read(iStream);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGTeamList::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)&m_TeamID, sizeof(m_TeamID) );
	oStream.Write( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_Members[i].Write(oStream);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT	WGTeamList::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamListHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

