#include "stdafx.h"
#include "CGReqLevelUp.h"


BOOL	CGReqLevelUp::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION
		return TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}



BOOL	CGReqLevelUp::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		return	TRUE;
	__LEAVE_FUNCTION
		return FALSE;
}


UINT CGReqLevelUp::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return	CGReqLevelUpHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return	FALSE;
}