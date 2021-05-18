#include "stdafx.h"
#include "LCRetDeleteChar.h"


BOOL LCRetDeleteChar::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)(&Result),sizeof(ASKDELETECHAR_RESULT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LCRetDeleteChar::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)(&Result),sizeof(ASKDELETECHAR_RESULT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LCRetDeleteChar::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LCRetDeleteCharHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	LCRetDeleteChar::GetAccount()	const
{
	return szAccount;
}
VOID			LCRetDeleteChar::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

ASKDELETECHAR_RESULT	LCRetDeleteChar::GetResult() const
{
	return Result;
}

VOID LCRetDeleteChar::SetResult(ASKDELETECHAR_RESULT result)
{
	Result = result;
}
