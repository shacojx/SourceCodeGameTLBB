#include "stdafx.h"
#include "GCLevelUpResult.h"



BOOL		GCLevelUpResult::Read(SocketInputStream& iStream )
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&m_Result,sizeof(LEVELUP_RESULT));
		iStream.Read((CHAR*)&m_nRemainExp,sizeof(UINT));
		
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		GCLevelUpResult::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&m_Result,sizeof(LEVELUP_RESULT));	
		oStream.Write((CHAR*)&m_nRemainExp,sizeof(UINT));	
		
		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT		GCLevelUpResult::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCLevelUpResultHandler::Execute(this,pPlayer);
	__LEAVE_FUNCTION
		return	FALSE;
}