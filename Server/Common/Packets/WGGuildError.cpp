#include "stdafx.h"
#include "WGGuildError.h"

BOOL	WGGuildError::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_Error), sizeof(m_Error) );
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGGuildError::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_Error), sizeof(m_Error) );
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGGuildError::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildErrorHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
