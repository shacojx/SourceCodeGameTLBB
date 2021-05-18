#include "stdafx.h"

#include "LCRetConnect.h"

BOOL LCRetConnect::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&Result,sizeof(LOGIN_CONNECT_RESULT));
		iStream.Read((CHAR*)&LoginPort,sizeof(UINT));
		iStream.Read((CHAR*)&LoginIP,sizeof(CHAR)*IP_SIZE);

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LCRetConnect::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&Result,sizeof(LOGIN_CONNECT_RESULT));
		oStream.Write((CHAR*)&LoginPort,sizeof(UINT));
		oStream.Write((CHAR*)&LoginIP,sizeof(CHAR)*IP_SIZE);

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LCRetConnect::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LCRetConnectHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
