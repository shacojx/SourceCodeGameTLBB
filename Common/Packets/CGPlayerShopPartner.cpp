#include "stdafx.h"
// CGPlayerShopPartner.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopPartner.h"

BOOL CGPlayerShopPartner::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_PartnerID), sizeof(GUID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopPartner::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_PartnerID), sizeof(GUID_t));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopPartner::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopPartnerHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

