#include "stdafx.h"
// GCPlayerShopStallStatus.cpp
// 
/////////////////////////////////////////////////////

#include "GCPlayerShopStallStatus.h"

BOOL GCPlayerShopStallStatus::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_nStallIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_bStatus), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_FinalStallNum), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCPlayerShopStallStatus::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_nStallIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_bStatus), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_FinalStallNum), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCPlayerShopStallStatus::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCPlayerShopStallStatusHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
