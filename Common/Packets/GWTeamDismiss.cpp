#include "stdafx.h"
#include "stdafx.h"
#include  "GWTeamDismiss.h"


BOOL		GWTeamDismiss::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GWTeamDismiss::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GWTeamDismiss::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return GWTeamDismissHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

