#include "stdafx.h"

#include "LCStatus.h"


BOOL LCStatus::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&TurnNumber),sizeof(USHORT));
	iStream.Read((CHAR*)(&ClientStatus),sizeof(CLIENT_TURN_STATUS));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LCStatus::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION


	oStream.Write((CHAR*)(&TurnNumber),sizeof(USHORT));
	oStream.Write((CHAR*)(&ClientStatus),sizeof(CLIENT_TURN_STATUS));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LCStatus::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LCStatusHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
