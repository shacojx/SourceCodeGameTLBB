#include "stdafx.h"
#include  "CGTeamDismiss.h"


BOOL		CGTeamDismiss::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	CGTeamDismiss::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	CGTeamDismiss::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return CGTeamDismissHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

