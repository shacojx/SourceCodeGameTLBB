#include "stdafx.h"

#include "CLAskDeleteChar.h"

BOOL CLAskDeleteChar::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)&m_GUID,sizeof(GUID_t));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CLAskDeleteChar::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&m_GUID,sizeof(GUID_t));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLAskDeleteChar::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CLAskDeleteCharHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	CLAskDeleteChar::GetAccount()	const
{
	return szAccount;
}
VOID			CLAskDeleteChar::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = '\0' ;
}


