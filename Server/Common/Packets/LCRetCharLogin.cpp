#include "stdafx.h"

#include "LCRetCharLogin.h"


BOOL LCRetCharLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read((CHAR*)&Result,sizeof(ASKCHARLOGIN_RESULT));
	iStream.Read((CHAR*)&ServerPort,sizeof(UINT));
	iStream.Read((CHAR*)&ServerIP,sizeof(CHAR)*IP_SIZE);
	iStream.Read((CHAR*)&Key,sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LCRetCharLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&Result,sizeof(ASKCHARLOGIN_RESULT));
	oStream.Write((CHAR*)&ServerPort,sizeof(UINT));
	oStream.Write((CHAR*)&ServerIP,sizeof(CHAR)*IP_SIZE);
	oStream.Write((CHAR*)&Key,sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LCRetCharLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LCRetCharLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


