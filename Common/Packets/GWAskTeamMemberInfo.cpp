#include "stdafx.h"
#include "stdafx.h"
// GWAskTeamMemberInfo.cpp

#include "GWAskTeamMemberInfo.h"

BOOL GWAskTeamMemberInfo::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GWAskTeamMemberInfo::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) ) ;
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWAskTeamMemberInfo::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWAskTeamMemberInfoHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
