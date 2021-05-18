#include "stdafx.h"
// GCStallBuy.cpp
// 
/////////////////////////////////////////////////////

#include "GCStallBuy.h"

BOOL GCStallBuy::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	iStream.Read( (CHAR*)(&m_nSerial), sizeof(UINT));
	iStream.Read( (CHAR*)(&m_ToIndex), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ToType), sizeof(BYTE));


	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL GCStallBuy::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	oStream.Write( (CHAR*)(&m_nSerial), sizeof(UINT));
	oStream.Write( (CHAR*)(&m_ToIndex), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ToType), sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT GCStallBuy::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCStallBuyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

