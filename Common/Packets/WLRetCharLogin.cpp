#include "stdafx.h"
#include "WLRetCharLogin.h"

BOOL WLRetCharLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&Result,sizeof(ASKCHARLOGIN_RESULT));
	iStream.Read((CHAR*)&szAccount,sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)&PlayerID,sizeof(PlayerID_t));
	iStream.Read((CHAR*)&HoldStatus,sizeof(BOOL));
	iStream.Read((CHAR*)&PlayerGUID,sizeof(GUID_t));
	iStream.Read((CHAR*)&ServerID,sizeof(ID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WLRetCharLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&Result,sizeof(ASKCHARLOGIN_RESULT));
	oStream.Write((CHAR*)&szAccount,sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)&PlayerID,sizeof(PlayerID_t));
	oStream.Write((CHAR*)&HoldStatus,sizeof(BOOL));
	oStream.Write((CHAR*)&PlayerGUID,sizeof(GUID_t));
	oStream.Write((CHAR*)&ServerID,sizeof(ID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT WLRetCharLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	WLRetCharLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	WLRetCharLogin::GetAccount()	const
{
	return szAccount;
}
VOID			WLRetCharLogin::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

