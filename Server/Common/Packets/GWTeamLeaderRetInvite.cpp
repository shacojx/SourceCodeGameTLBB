#include "stdafx.h"
#include "stdafx.h"
#include  "GWTeamLeaderRetInvite.h"


BOOL		GWTeamLeaderRetInvite::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)&m_Return, sizeof(BYTE) );
		iStream.Read((CHAR*)&m_SourGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_DestGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_LeaderGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	GWTeamLeaderRetInvite::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)&m_Return, sizeof(BYTE) );
		oStream.Write((CHAR*)&m_SourGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_DestGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_LeaderGUID,sizeof(GUID_t));

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	GWTeamLeaderRetInvite::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return GWTeamLeaderRetInviteHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

