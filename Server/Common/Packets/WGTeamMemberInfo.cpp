#include "stdafx.h"
#include "stdafx.h"
// WGTeamMemberInfo.cpp

#include "WGTeamMemberInfo.h"

BOOL WGTeamMemberInfo::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	iStream.Read( (CHAR*)(&m_uFamily), sizeof(m_uFamily) );
	iStream.Read( (CHAR*)(&m_Level), sizeof(m_Level) );
	iStream.Read( (CHAR*)(&m_bDeadLinkFlag), sizeof(m_bDeadLinkFlag) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL WGTeamMemberInfo::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	oStream.Write( (CHAR*)(&m_uFamily), sizeof(m_uFamily) );
	oStream.Write( (CHAR*)(&m_Level), sizeof(m_Level) );
	oStream.Write( (CHAR*)(&m_bDeadLinkFlag), sizeof(m_bDeadLinkFlag) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT WGTeamMemberInfo::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamMemberInfoHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
