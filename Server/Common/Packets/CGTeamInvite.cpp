#include "stdafx.h"
#include  "CGTeamInvite.h"


BOOL		CGTeamInvite::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_SourObjID, sizeof(ObjID_t));
	iStream.Read((CHAR*)&m_DestNameSize, sizeof(UCHAR));

	if ( m_DestNameSize>0 )
	{
		iStream.Read((CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGTeamInvite::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_SourObjID, sizeof(ObjID_t));
	oStream.Write((CHAR*)&m_DestNameSize, sizeof(UCHAR));

	if ( m_DestNameSize>0 )
	{
		oStream.Write((CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGTeamInvite::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGTeamInviteHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

