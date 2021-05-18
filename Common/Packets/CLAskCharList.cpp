#include "stdafx.h"

#include "CLAskCharList.h"


BOOL CLAskCharList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CLAskCharList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CLAskCharList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CLAskCharListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


const	CHAR*	CLAskCharList::GetAccount()	const
{
	return szAccount;
}
VOID			CLAskCharList::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = '\0' ;
}

