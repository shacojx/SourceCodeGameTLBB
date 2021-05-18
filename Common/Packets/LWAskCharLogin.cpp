#include "stdafx.h"
#include "LWAskCharLogin.h"
#include "DB_Struct.h"

BOOL LWAskCharLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	iStream.Read( (CHAR*)(&PlayerGUID), sizeof(GUID_t));
	iStream.Read( (CHAR*)(&uKey), sizeof(UINT));
	iStream.Read( (CHAR*)(&AskStatus), sizeof(ASK_LOGINSTATUS));
	if(AskStatus == ALS_SENDDATA)
	{
		iStream.Read( (CHAR*)(&UserData), sizeof(FULLUSERDATA));
	}
	iStream.Read( (CHAR*)(&Age), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL  LWAskCharLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	oStream.Write( (CHAR*)(&PlayerGUID), sizeof(GUID_t));
	oStream.Write( (CHAR*)(&uKey), sizeof(UINT));
	oStream.Write( (CHAR*)(&AskStatus), sizeof(ASK_LOGINSTATUS));
	if(AskStatus == ALS_SENDDATA)
	{
		oStream.Write( (CHAR*)(&UserData), sizeof(FULLUSERDATA));
	}
	oStream.Write( (CHAR*)(&Age), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT  LWAskCharLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LWAskCharLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


const	CHAR*	LWAskCharLogin::GetAccount()	const
{
	return szAccount;
}
VOID			LWAskCharLogin::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

