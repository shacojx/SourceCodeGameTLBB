#include "stdafx.h"


#include "CGAskSetting.h"



BOOL CGAskSetting::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGAskSetting::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGAskSetting::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGAskSettingHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


