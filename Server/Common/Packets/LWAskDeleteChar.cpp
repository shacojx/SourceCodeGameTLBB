#include "stdafx.h"
#include "LWAskDeleteChar.h"
#include "DB_Struct.h"

BOOL LWAskDeleteChar::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	iStream.Read( (CHAR*)(&PlayerGUID), sizeof(GUID_t));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL  LWAskDeleteChar::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	oStream.Write( (CHAR*)(&PlayerGUID), sizeof(GUID_t));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT  LWAskDeleteChar::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LWAskDeleteCharHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


const	CHAR*	LWAskDeleteChar::GetAccount()	const
{
	return szAccount;
}
VOID			LWAskDeleteChar::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

