#include "stdafx.h"
// CGShopRepair.cpp
// 
/////////////////////////////////////////////////////

#include "CGShopRepair.h"

BOOL CGShopRepair::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_bRepairAllFlag), sizeof(BOOL));
		iStream.Read( (CHAR*)(&m_nBagIndex), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_Opt), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_UniqueID), sizeof(UINT));


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGShopRepair::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_bRepairAllFlag), sizeof(BOOL));
		oStream.Write( (CHAR*)(&m_nBagIndex), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_Opt), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_UniqueID), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGShopRepair::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGShopRepairHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

