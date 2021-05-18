#include "stdafx.h"
#include "GCUseGemResult.h"

BOOL	GCUseGemResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_Result,sizeof(USEGEM_RESULT));		
	
	return TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	GCUseGemResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_Result,sizeof(USEGEM_RESULT));
		return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCUseGemResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCUseGemResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}

