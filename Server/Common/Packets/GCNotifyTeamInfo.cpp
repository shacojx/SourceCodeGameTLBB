#include "stdafx.h"
#include  "GCNotifyTeamInfo.h"


BOOL	GCNotifyTeamInfo::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_ObjID, sizeof(ObjID_t) );
	iStream.Read( (CHAR*)&m_uTeamFlag, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_uTeamLeaderFlag, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_uTeamFullFlag, sizeof(UCHAR) );
//	iStream.Read( (CHAR*)&m_uTeamFollowFlag, sizeof(m_uTeamFollowFlag) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCNotifyTeamInfo::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_ObjID, sizeof(ObjID_t) );
	oStream.Write( (CHAR*)&m_uTeamFlag, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_uTeamLeaderFlag, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_uTeamFullFlag, sizeof(UCHAR) );
//	oStream.Write( (CHAR*)&m_uTeamFollowFlag, sizeof(m_uTeamFollowFlag) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCNotifyTeamInfo::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNotifyTeamInfoHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

