#include "stdafx.h"
// CGAskTeamInfo.cpp

#include  "CGAskTeamInfo.h"

BOOL CGAskTeamInfo::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL CGAskTeamInfo::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT CGAskTeamInfo::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGAskTeamInfoHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return FALSE;
}
