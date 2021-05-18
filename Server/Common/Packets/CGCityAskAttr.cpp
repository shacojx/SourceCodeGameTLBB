#include "stdafx.h"
#include "CGCityAskAttr.h"

BOOL CGCityAskAttr::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGCityAskAttr::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGCityAskAttr::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGCityAskAttrHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


