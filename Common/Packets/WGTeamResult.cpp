#include "stdafx.h"
#include "stdafx.h"
#include  "WGTeamResult.h"


BOOL	WGTeamResult::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_TeamID, sizeof(TeamID_t) );
	iStream.Read( (CHAR*)&m_GUIDEx, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SceneID, sizeof(SceneID_t) );
	iStream.Read( (CHAR*)&m_NameSize, sizeof(UCHAR) );

	if ( m_NameSize>0 )
	{
		iStream.Read( (CHAR*)&m_Name, sizeof(CHAR) * m_NameSize );
	}

	iStream.Read( (CHAR*)(&m_nPortrait), sizeof(m_nPortrait) );
	iStream.Read( (CHAR*)&m_uDataID, sizeof(USHORT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	WGTeamResult::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_Return), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_TeamID, sizeof(TeamID_t) );
	oStream.Write( (CHAR*)&m_GUIDEx, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SceneID, sizeof(SceneID_t) );
	oStream.Write( (CHAR*)&m_NameSize, sizeof(UCHAR) );

	if ( m_NameSize>0 )
	{
		oStream.Write( (CHAR*)&m_Name, sizeof(CHAR) * m_NameSize );
	}

	oStream.Write( (CHAR*)(&m_nPortrait), sizeof(m_nPortrait) );
	oStream.Write( (CHAR*)&m_uDataID, sizeof(USHORT) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGTeamResult::Execute(Player* pPlayer )
{
__ENTER_FUNCTION
		return WGTeamResultHandler::Execute(this,pPlayer);
__LEAVE_FUNCTION
		return FALSE;
}

