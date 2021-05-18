#include "stdafx.h"
#include  "CGTeamLeave.h"


BOOL		CGTeamLeave::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	CGTeamLeave::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	CGTeamLeave::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return CGTeamLeaveHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

