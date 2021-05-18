#include "stdafx.h"
// CGPlayerShopAcquireItemList.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopAcquireItemList.h"

BOOL CGPlayerShopAcquireItemList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_nStallIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nSign), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopAcquireItemList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_nStallIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nSign), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopAcquireItemList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopAcquireItemListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

