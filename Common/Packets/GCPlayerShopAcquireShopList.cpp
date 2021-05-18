#include "stdafx.h"
// GCPlayerShopAcquireShopList.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopAcquireShopList.h"

BOOL GCPlayerShopAcquireShopList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
	iStream.Read( (CHAR*)(&m_NpcId), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_ShopNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ComFactor), sizeof(FLOAT));
	iStream.Read( (CHAR*)(m_ShopList), sizeof(SHOP_INFO_t)*m_ShopNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopAcquireShopList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_NpcId), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_ShopNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ComFactor), sizeof(FLOAT));
	oStream.Write( (CHAR*)(m_ShopList), sizeof(SHOP_INFO_t)*m_ShopNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopAcquireShopList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopAcquireShopListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

