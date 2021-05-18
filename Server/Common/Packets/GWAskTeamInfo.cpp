#include "stdafx.h"
#include "stdafx.h"
// GWAskTeamInfo.cpp

#include  "GWAskTeamInfo.h"

BOOL GWAskTeamInfo::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
		iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL GWAskTeamInfo::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
		oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT GWAskTeamInfo::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return GWAskTeamInfoHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}
