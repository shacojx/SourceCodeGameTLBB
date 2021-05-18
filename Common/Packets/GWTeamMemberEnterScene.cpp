#include "stdafx.h"
#include "stdafx.h"
#include "GWTeamMemberEnterScene.h"

BOOL GWTeamMemberEnterScene::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_PlayerID, sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SceneID, sizeof(SceneID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL GWTeamMemberEnterScene::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SceneID, sizeof(SceneID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT GWTeamMemberEnterScene::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWTeamMemberEnterSceneHandler::Execute(this, pPlayer);
	
__LEAVE_FUNCTION

	return FALSE;
}
