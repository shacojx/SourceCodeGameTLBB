#include "stdafx.h"

#include "CLAskLogin.h"


BOOL CLAskLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
		iStream.Read( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_PASSWORD);
		iStream.Read( (CHAR*)(&uVersion), sizeof(UINT));
			
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CLAskLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_PASSWORD);
	oStream.Write( (CHAR*)(&uVersion), sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CLAskLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLAskLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}

const	CHAR*	CLAskLogin::GetAccount()	const
{
	return szAccount;
}
VOID			CLAskLogin::SetAccount(CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = 0 ;
}

const	CHAR*	CLAskLogin::GetPassWord()	const
{
	return szPassWord;
}
VOID			CLAskLogin::SetPassWord(CHAR*	pPassWord)
{
	Assert(pPassWord);
	strncpy(szPassWord,pPassWord,MAX_PASSWORD);
	szPassWord[MAX_PASSWORD] = 0 ;
}

UINT			CLAskLogin::GetVersion() const
{
	return	uVersion;
}

VOID			CLAskLogin::SetVersion(UINT version)
{
	uVersion = version;
}

