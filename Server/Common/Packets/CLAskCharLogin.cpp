#include "stdafx.h"

#include "CLAskCharLogin.h"


BOOL CLAskCharLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&CharGuid), sizeof(GUID_t));
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CLAskCharLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&CharGuid), sizeof(GUID_t));
	oStream.Write( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLAskCharLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CLAskCharLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


GUID_t			CLAskCharLogin::GetCharGuid() const
{
	return	CharGuid;
}

VOID			CLAskCharLogin::SetCharGuid(GUID_t guid)
{
	CharGuid	= guid;
}

