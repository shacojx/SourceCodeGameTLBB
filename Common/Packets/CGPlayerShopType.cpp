#include "stdafx.h"
// CGPlayerShopType.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopType.h"

BOOL CGPlayerShopType::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopType::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopType::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopTypeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

