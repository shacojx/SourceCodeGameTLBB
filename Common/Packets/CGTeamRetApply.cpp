#include "stdafx.h"
#include  "CGTeamRetApply.h"


BOOL		CGTeamRetApply::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_Return, sizeof(BYTE) );
		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	CGTeamRetApply::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_Return, sizeof(BYTE) );
		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	CGTeamRetApply::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return CGTeamRetApplyHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

