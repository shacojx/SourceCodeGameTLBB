#include "stdafx.h"
// CGStallShopName.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopDesc.h"

BOOL CGPlayerShopDesc::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
		iStream.Read( (CHAR*)(&m_PlayerShopDescSize), sizeof(BYTE));
		iStream.Read( (CHAR*)(m_PlayerShopDesc), sizeof(CHAR)*m_PlayerShopDescSize ) ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopDesc::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
		oStream.Write( (CHAR*)(&m_PlayerShopDescSize), sizeof(BYTE));
		oStream.Write( (CHAR*)(m_PlayerShopDesc), sizeof(CHAR)*m_PlayerShopDescSize ) ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopDesc::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopDescHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

