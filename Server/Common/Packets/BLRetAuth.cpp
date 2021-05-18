#include "stdafx.h"

#include "BLRetAuth.h"


BOOL BLRetAuth::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)(&Result),sizeof(LOGIN_RESULT));
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	iStream.Read( (CHAR*)(&PlayerGUID), sizeof(GUID_t));
	iStream.Read( (CHAR*)(&m_Age), sizeof(m_Age));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetAuth::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)(&Result),sizeof(LOGIN_RESULT));
	oStream.Write((CHAR*)(&PlayerID),sizeof(PlayerID_t));
	oStream.Write((CHAR*)(&PlayerGUID),sizeof(GUID_t));
	oStream.Write((CHAR*)(&m_Age),sizeof(m_Age));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetAuth::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetAuthHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	BLRetAuth::GetAccount()	const
{
	return szAccount;
}
VOID			BLRetAuth::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

LOGIN_RESULT	BLRetAuth::GetResult() const
{
	return Result;
}

VOID BLRetAuth::SetResult(LOGIN_RESULT result)
{
	Result = result;
}

const PlayerID_t BLRetAuth::GetPlayerID() const
{
	return PlayerID;
}

VOID		BLRetAuth::SetPlayerID(PlayerID_t pID)
{
	PlayerID = pID;
}

const GUID_t BLRetAuth::GetPlayerGUID() const
{
	return PlayerGUID;
}

VOID		BLRetAuth::SetPlayerGUID(GUID_t guid)
{
	PlayerGUID	 = guid;
}


SHORT		BLRetAuth::GetAge()const
{
	return m_Age;
}
VOID		BLRetAuth::SetAge(SHORT NewAge)
{
	m_Age = NewAge;
}
