#include "stdafx.h"
#include  "CGAskTeamFollow.h"


BOOL	CGAskTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGAskTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGAskTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

