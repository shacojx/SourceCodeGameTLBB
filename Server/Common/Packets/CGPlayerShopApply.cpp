#include "stdafx.h"
// CGPlayerShopApply.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopApply.h"

BOOL CGPlayerShopApply::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopApply::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopApply::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopApplyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

