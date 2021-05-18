#include "stdafx.h"
#include  "CGReqResetAttr.h"


BOOL		CGReqResetAttr::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	CGReqResetAttr::Write(SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


UINT	CGReqResetAttr::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
			return CGReqResetAttrHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
			return FALSE;
}

