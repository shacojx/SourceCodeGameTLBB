#include "stdafx.h"
// CGShopClose.cpp
// 
/////////////////////////////////////////////////////

#include "CGShopClose.h"

BOOL CGShopClose::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION
	return FALSE ;
}

BOOL CGShopClose::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGShopClose::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGShopCloseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

