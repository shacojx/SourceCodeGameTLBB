#include "stdafx.h"
#include  "WGTeamAskInvite.h"


BOOL	WGTeamAskInvite::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)&m_dGUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_sGUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_uMemberSize, sizeof(UCHAR) );

	for( INT i=0; i<m_uMemberSize; ++i )
	{
		iStream.Read( (CHAR*)&(m_InviterInfo[i].m_PlayerID), sizeof(PlayerID_t) );
		iStream.Read( (CHAR*)&(m_InviterInfo[i].m_NickSize), sizeof(UCHAR) );

		if( m_InviterInfo[i].m_NickSize>0 )
		{
			iStream.Read( (CHAR*)&(m_InviterInfo[i].m_szNick), sizeof(CHAR) * m_InviterInfo[i].m_NickSize );
		}

		iStream.Read( (CHAR*)&m_InviterInfo[i].m_uFamily, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_InviterInfo[i].m_Scene, sizeof(SceneID_t) );
		iStream.Read( (CHAR*)&m_InviterInfo[i].m_Level, sizeof(INT) );
		iStream.Read( (CHAR*)&m_InviterInfo[i].m_uDataID, sizeof(USHORT) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	WGTeamAskInvite::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)&m_dGUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_sGUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_uMemberSize, sizeof(UCHAR) );

	for( INT i=0; i<m_uMemberSize; ++i )
	{
		oStream.Write( (CHAR*)&(m_InviterInfo[i].m_PlayerID), sizeof(PlayerID_t) );
		oStream.Write( (CHAR*)&(m_InviterInfo[i].m_NickSize), sizeof(UCHAR) );

		if( m_InviterInfo[i].m_NickSize>0 )
		{
			oStream.Write( (CHAR*)&(m_InviterInfo[i].m_szNick), sizeof(CHAR) * m_InviterInfo[i].m_NickSize );
		}

		oStream.Write( (CHAR*)&m_InviterInfo[i].m_uFamily, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_InviterInfo[i].m_Scene, sizeof(SceneID_t) );
		oStream.Write( (CHAR*)&m_InviterInfo[i].m_Level, sizeof(INT) );
		oStream.Write( (CHAR*)&m_InviterInfo[i].m_uDataID, sizeof(USHORT) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGTeamAskInvite::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamAskInviteHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

