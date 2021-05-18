#include "stdafx.h"
#include "GCTeamFollowErr.h"


BOOL	GCTeamFollowErr::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_uErr, sizeof(UCHAR) );
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GCTeamFollowErr::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_uErr, sizeof(UCHAR) );
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GCTeamFollowErr::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCTeamFollowErrHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return FALSE;
}

