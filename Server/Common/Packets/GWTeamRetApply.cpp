#include "stdafx.h"
#include "stdafx.h"
#include  "GWTeamRetApply.h"


BOOL		GWTeamRetApply::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_Return, sizeof(BYTE) );
		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_LeaderGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GWTeamRetApply::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_Return, sizeof(BYTE) );
		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_LeaderGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GWTeamRetApply::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return GWTeamRetApplyHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

