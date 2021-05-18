#include "stdafx.h"
#include "GCRemoveGemResult.h"

BOOL	GCRemoveGemResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_Result,sizeof(REMOVEGEM_RESULT));		

	return TRUE;

	__LEAVE_FUNCTION
	return	FALSE;
}

BOOL	GCRemoveGemResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_Result,sizeof(REMOVEGEM_RESULT));
	return TRUE;
	__LEAVE_FUNCTION
	return	FALSE;
}


UINT	GCRemoveGemResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

	return GCRemoveGemResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
	return FALSE;
}

