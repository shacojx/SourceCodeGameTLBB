#include "stdafx.h"

#include "LCRetLogin.h"


BOOL LCRetLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)(&Result),sizeof(LOGIN_RESULT));
	
	return TRUE ;
	
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LCRetLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)(&Result),sizeof(LOGIN_RESULT));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LCRetLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return LCRetLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}

const	CHAR*	LCRetLogin::GetAccount()	const
{
	return szAccount;
}
VOID			LCRetLogin::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

LOGIN_RESULT	LCRetLogin::GetResult() const
{
	return Result;
}

VOID LCRetLogin::SetResult(LOGIN_RESULT result)
{
	Result = result;
}
