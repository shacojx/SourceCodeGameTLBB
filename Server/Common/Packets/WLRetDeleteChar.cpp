#include "stdafx.h"
#include "WLRetDeleteChar.h"

BOOL WLRetDeleteChar::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)&Result,sizeof(ASKDELETECHAR_RESULT));
	iStream.Read((CHAR*)&szAccount,sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)&PlayerID,sizeof(PlayerID_t));
	iStream.Read((CHAR*)&PlayerGUID,sizeof(GUID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WLRetDeleteChar::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&Result,sizeof(ASKDELETECHAR_RESULT));
	oStream.Write((CHAR*)&szAccount,sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)&PlayerID,sizeof(PlayerID_t));
	oStream.Write((CHAR*)&PlayerGUID,sizeof(GUID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT WLRetDeleteChar::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	WLRetDeleteCharHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	WLRetDeleteChar::GetAccount()	const
{
	return szAccount;
}
VOID			WLRetDeleteChar::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

