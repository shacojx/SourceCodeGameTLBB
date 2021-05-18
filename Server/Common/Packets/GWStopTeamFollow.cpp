#include "stdafx.h"
#include  "GWStopTeamFollow.h"


BOOL	GWStopTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GWStopTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GWStopTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWStopTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

