#include "stdafx.h"

#include "LCRetCreateChar.h"


BOOL LCRetCreateChar::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		

		iStream.Read((CHAR*)(&Result),sizeof(ASKCREATECHAR_RESULT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LCRetCreateChar::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)(&Result),sizeof(ASKCREATECHAR_RESULT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LCRetCreateChar::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LCRetCreateCharHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

ASKCREATECHAR_RESULT	LCRetCreateChar::GetResult() const
{
	return Result;
}

VOID LCRetCreateChar::SetResult(ASKCREATECHAR_RESULT result)
{
	Result = result;
}

const	CHAR*	LCRetCreateChar::GetAccount()	const
{
	return szAccount;
}
VOID			LCRetCreateChar::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}
