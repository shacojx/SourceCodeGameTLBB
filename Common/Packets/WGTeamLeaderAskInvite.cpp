#include "stdafx.h"
#include "stdafx.h"
#include  "WGTeamLeaderAskInvite.h"


BOOL	WGTeamLeaderAskInvite::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)&m_SourGUID,sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_DestGUID,sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SourNameSize, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_DestNameSize, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );

	if ( m_SourNameSize>0 )
	{
		iStream.Read( (CHAR*)&m_SourName, sizeof(CHAR) * m_SourNameSize );
	}

	if ( m_DestNameSize>0 )
	{
		iStream.Read( (CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	WGTeamLeaderAskInvite::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)&m_SourGUID,sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_DestGUID,sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SourNameSize, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_DestNameSize, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );

	if ( m_SourNameSize>0 )
	{
		oStream.Write( (CHAR*)&m_SourName, sizeof(CHAR) * m_SourNameSize );
	}

	if ( m_DestNameSize>0 )
	{
		oStream.Write( (CHAR*)&m_DestName, sizeof(CHAR) * m_DestNameSize );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGTeamLeaderAskInvite::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamLeaderAskInviteHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

