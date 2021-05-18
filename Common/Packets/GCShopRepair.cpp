#include "stdafx.h"
// GCShopRepair.cpp
// 
/////////////////////////////////////////////////////

#include "GCShopRepair.h"

BOOL GCShopRepair::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_bRepairAllFlag), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_nRepairedListNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_aRepairedList), sizeof(REPAIRED_ITEM)*m_nRepairedListNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCShopRepair::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_bRepairAllFlag), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nRepairedListNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_aRepairedList), sizeof(REPAIRED_ITEM)*m_nRepairedListNum);

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCShopRepair::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCShopRepairHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

