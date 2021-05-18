#include "stdafx.h"
// CGPlayerShopAskForRecord.cpp
// 
/////////////////////////////////////////////////////

#include "CGPlayerShopAskForRecord.h"

BOOL CGPlayerShopAskForRecord::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	iStream.Read( (CHAR*)(&m_bType), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_bPage), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGPlayerShopAskForRecord::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ShopID), sizeof(_PLAYERSHOP_GUID));
	oStream.Write( (CHAR*)(&m_bType), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_bPage), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGPlayerShopAskForRecord::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGPlayerShopAskForRecordHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

