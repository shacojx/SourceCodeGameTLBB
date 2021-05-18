#include "stdafx.h"
#include "GCTeamList.h"

BOOL	GCTeamList::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_TeamID, sizeof(m_TeamID) );
	iStream.Read( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_Members[i].Read(iStream);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GCTeamList::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_TeamID, sizeof(m_TeamID) );
	oStream.Write( (CHAR*)&m_nMemberCount, sizeof(m_nMemberCount) );

	for( INT i=0; i<m_nMemberCount; ++i )
	{
		m_Members[i].Write(oStream);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT	GCTeamList::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCTeamListHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

