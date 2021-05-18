#include "stdafx.h"
// GCStallOpen.cpp
// 
/////////////////////////////////////////////////////

#include "GCStallOpen.h"

BOOL GCStallOpen::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nStallItemNum), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_StallItemList), sizeof(_STALL_ITEM)*m_nStallItemNum);
	iStream.Read( (CHAR*)(m_ShopName), sizeof(CHAR)*MAX_STALL_NAME);
	iStream.Read( (CHAR*)(&m_nFirstPage), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_OwnerObjId), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_OwnerGuid), sizeof(GUID_t));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCStallOpen::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nStallItemNum), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_StallItemList), sizeof(_STALL_ITEM)*m_nStallItemNum);
	oStream.Write( (CHAR*)(m_ShopName), sizeof(CHAR)*MAX_STALL_NAME);
	oStream.Write( (CHAR*)(&m_nFirstPage), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_OwnerObjId), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_OwnerGuid), sizeof(GUID_t));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCStallOpen::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCStallOpenHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

