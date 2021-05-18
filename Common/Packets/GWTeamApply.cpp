#include "stdafx.h"
#include "stdafx.h"
#include  "GWTeamApply.h"


BOOL	GWTeamApply::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	m_DestNameSize = 0;
	memset((void*)m_DestName, 0, MAX_CHARACTER_NAME);

	iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));
	iStream.Read((CHAR*)&m_DestNameSize, sizeof(UCHAR));
	iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));

	if ( m_DestNameSize>0 )
	{
		iStream.Read((CHAR*)&m_DestName, m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GWTeamApply::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));
	oStream.Write((CHAR*)&m_DestNameSize, sizeof(UCHAR));
	oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));

	if ( m_DestNameSize>0 )
	{
		oStream.Write((CHAR*)&m_DestName, m_DestNameSize);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GWTeamApply::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWTeamApplyHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

