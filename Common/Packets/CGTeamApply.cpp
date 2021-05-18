#include "stdafx.h"
#include  "CGTeamApply.h"


BOOL		CGTeamApply::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_SourGUID, sizeof(GUID_t));
	iStream.Read((CHAR*)&m_DestNameSize, sizeof(UCHAR));

	if ( m_DestNameSize>0 && m_DestNameSize<=MAX_CHARACTER_NAME)
	{
		iStream.Read((CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGTeamApply::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_SourGUID, sizeof(GUID_t));
	oStream.Write((CHAR*)&m_DestNameSize, sizeof(UCHAR));

	if ( m_DestNameSize>0 && m_DestNameSize<=MAX_CHARACTER_NAME)
	{
		oStream.Write((CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGTeamApply::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGTeamApplyHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

