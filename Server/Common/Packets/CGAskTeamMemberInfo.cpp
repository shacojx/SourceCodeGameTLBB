#include "stdafx.h"
// CGAskTeamMemberInfo.cpp

#include "CGAskTeamMemberInfo.h"

BOOL CGAskTeamMemberInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SceneID, sizeof(SceneID_t) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskTeamMemberInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SceneID, sizeof(SceneID_t) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskTeamMemberInfo::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskTeamMemberInfoHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

