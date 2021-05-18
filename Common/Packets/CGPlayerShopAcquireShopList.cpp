#include "stdafx.h"
// CGPlayerShopAcquireShopList.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopAcquireShopList.h"

BOOL CGPlayerShopAcquireShopList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopAcquireShopList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopAcquireShopList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopAcquireShopListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

