#include "stdafx.h"
// CGStallShopName.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopName.h"

BOOL CGPlayerShopName::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
		iStream.Read( (CHAR*)(&m_PlayerShopNameSize), sizeof(BYTE));
		iStream.Read( (CHAR*)(m_PlayerShopName), sizeof(CHAR)*m_PlayerShopNameSize ) ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopName::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
		oStream.Write( (CHAR*)(&m_PlayerShopNameSize), sizeof(BYTE));
		oStream.Write( (CHAR*)(m_PlayerShopName), sizeof(CHAR)*m_PlayerShopNameSize ) ;

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopName::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopNameHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

