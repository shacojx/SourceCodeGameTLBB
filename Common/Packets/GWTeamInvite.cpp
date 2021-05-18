#include "stdafx.h"
#include "stdafx.h"
#include  "GWTeamInvite.h"


BOOL	GWTeamInvite::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_SourGUID, sizeof(GUID_t));
	iStream.Read((CHAR*)&m_DestNameSize, sizeof(UCHAR));
	iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));

	if ( m_DestNameSize>0 )
	{
		iStream.Read((CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GWTeamInvite::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_SourGUID, sizeof(GUID_t));
	oStream.Write((CHAR*)&m_DestNameSize, sizeof(UCHAR));
	oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));

	if ( m_DestNameSize>0 )
	{
		oStream.Write((CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GWTeamInvite::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWTeamInviteHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

