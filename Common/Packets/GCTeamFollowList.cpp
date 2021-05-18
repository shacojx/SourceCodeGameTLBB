#include "stdafx.h"
#include "GCTeamFollowList.h"


BOOL	GCTeamFollowList::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_ObjID, sizeof(m_ObjID) );
	iStream.Read( (CHAR*)&m_Count, sizeof(UCHAR) );

	if ( m_Count>0 && m_Count<=MAX_TEAM_MEMBER )
	{
		iStream.Read( (CHAR*)m_GUIDs, m_Count * sizeof(GUID_t) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCTeamFollowList::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_ObjID, sizeof(m_ObjID) );
	oStream.Write( (CHAR*)&m_Count, sizeof(UCHAR) );

	if ( m_Count>0 && m_Count<=MAX_TEAM_MEMBER )
	{
		oStream.Write( (CHAR*)m_GUIDs, m_Count * sizeof(GUID_t) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCTeamFollowList::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCTeamFollowListHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

