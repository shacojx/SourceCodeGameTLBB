#include "stdafx.h"
#include "stdafx.h"
#include  "WGTeamAskApply.h"


BOOL	WGTeamAskApply::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)(&m_SourPlayerID), sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)&m_SourGUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_DestGUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SourNameSize, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_DestNameSize, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );

	if ( m_SourNameSize>0 )
	{
		iStream.Read((CHAR*)&m_SourName, m_SourNameSize);
	}

	if ( m_DestNameSize>0 )
	{
		iStream.Read((CHAR*)&m_DestName, m_DestNameSize);
	}

	iStream.Read( (CHAR*)&m_uFamily, sizeof(UINT) );
	iStream.Read( (CHAR*)&m_Scene, sizeof(SceneID_t) );
	iStream.Read( (CHAR*)&m_Level, sizeof(INT) );
	iStream.Read( (CHAR*)&m_uDataID, sizeof(m_uDataID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	WGTeamAskApply::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)(&m_SourPlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)&m_SourGUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_DestGUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SourNameSize, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_DestNameSize, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );

	if ( m_SourNameSize>0 )
	{
		oStream.Write((CHAR*)&m_SourName, m_SourNameSize);
	}

	if ( m_DestNameSize>0 )
	{
		oStream.Write((CHAR*)&m_DestName, m_DestNameSize);
	}

	oStream.Write( (CHAR*)&m_uFamily, sizeof(UINT) );
	oStream.Write( (CHAR*)&m_Scene, sizeof(SceneID_t) );
	oStream.Write( (CHAR*)&m_Level, sizeof(INT) );
	oStream.Write( (CHAR*)&m_uDataID, sizeof(m_uDataID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGTeamAskApply::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamAskApplyHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

