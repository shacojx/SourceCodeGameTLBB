#include "stdafx.h"
#include "stdafx.h"
#include "WGTeamError.h"


BOOL	WGTeamError::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	iStream.Read( (CHAR*)(&m_Error), sizeof(BYTE) ) ;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	WGTeamError::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	oStream.Write( (CHAR*)(&m_Error), sizeof(BYTE) ) ;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGTeamError::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamErrorHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

