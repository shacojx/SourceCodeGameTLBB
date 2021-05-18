#include "stdafx.h"

#include "LBAskAuth.h"

BOOL LBAskAuth::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	iStream.Read( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_PASSWORD);
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));

	return TRUE ;
	
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LBAskAuth::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_PASSWORD);
	oStream.Write((CHAR*)(&PlayerID),sizeof(PlayerID_t));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBAskAuth::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAskAuthHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	LBAskAuth::GetAccount()	const
{
	return szAccount;
}
VOID			LBAskAuth::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szPassWord[MAX_ACCOUNT] = 0 ;
}

const	CHAR*	LBAskAuth::GetPassWord()	const
{
	return szPassWord;
}
VOID			LBAskAuth::SetPassWord(const CHAR*	pPassWord)
{
	Assert(pPassWord);
	strncpy(szPassWord,pPassWord,MAX_PASSWORD*sizeof(CHAR));
	szPassWord[MAX_PASSWORD] = 0 ;
}
const PlayerID_t LBAskAuth::GetPlayerID() const
{
	return PlayerID;
}

VOID		LBAskAuth::SetPlayerID(PlayerID_t pID)
{
	PlayerID = pID;
}
