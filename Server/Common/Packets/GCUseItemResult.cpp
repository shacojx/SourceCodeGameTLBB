#include "stdafx.h"
#include "GCUseItemResult.h"

BOOL	GCUseItemResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION
			
			iStream.Read((CHAR*)&m_Result,sizeof(USEITEM_RESULT));		
			return TRUE;

	__LEAVE_FUNCTION
			return	FALSE;
}

BOOL	GCUseItemResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
			
		oStream.Write((CHAR*)&m_Result,sizeof(USEITEM_RESULT));
		return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}


UINT	GCUseItemResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCUseItemResultHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION
		return FALSE;
}

