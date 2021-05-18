#include "stdafx.h"
#include "stdafx.h"
#include  "GWTeamLeave.h"


BOOL		GWTeamLeave::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GWTeamLeave::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GWTeamLeave::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return GWTeamLeaveHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

