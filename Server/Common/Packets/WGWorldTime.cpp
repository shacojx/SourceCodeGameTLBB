#include "stdafx.h"
#include "WGWorldTime.h"


BOOL	WGWorldTime::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)(&m_Time),sizeof(WORLD_TIME));
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	WGWorldTime::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)(&m_Time),sizeof(WORLD_TIME));
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT WGWorldTime::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGWorldTimeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}