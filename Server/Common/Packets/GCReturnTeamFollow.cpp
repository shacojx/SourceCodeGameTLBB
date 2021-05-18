#include "stdafx.h"
#include "GCReturnTeamFollow.h"


BOOL	GCReturnTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Return, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_GUID,sizeof(GUID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCReturnTeamFollow::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Return, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_GUID,sizeof(GUID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCReturnTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCReturnTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

