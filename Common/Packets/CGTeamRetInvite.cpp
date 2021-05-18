#include "stdafx.h"
#include  "CGTeamRetInvite.h"


BOOL		CGTeamRetInvite::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_Return, sizeof(BYTE) );
		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	CGTeamRetInvite::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_Return, sizeof(BYTE) );
		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	CGTeamRetInvite::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return CGTeamRetInviteHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

