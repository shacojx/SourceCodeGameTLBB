#include "stdafx.h"
// GCPlayerShopUpdatePartners.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopUpdatePartners.h"

BOOL GCPlayerShopUpdatePartners::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_PartnerNum), sizeof(BYTE));
	if(m_PartnerNum>0)
		iStream.Read( (CHAR*)(m_PartnerList), sizeof(MsgPartner_t)*m_PartnerNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopUpdatePartners::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_PartnerNum), sizeof(BYTE));
	if(m_PartnerNum>0)
		oStream.Write( (CHAR*)(m_PartnerList), sizeof(MsgPartner_t)*m_PartnerNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopUpdatePartners::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopUpdatePartnersHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

