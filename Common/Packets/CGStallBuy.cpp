#include "stdafx.h"
// CGStallBuy.cpp
// 
/////////////////////////////////////////////////////

#include "CGStallBuy.h"

BOOL CGStallBuy::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_objID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	iStream.Read( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	iStream.Read( (CHAR*)(&m_nSerial), sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGStallBuy::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_objID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_ItemGuid), sizeof(_ITEM_GUID));
	oStream.Write( (CHAR*)(&m_PetGuid), sizeof(PET_GUID_t));
	oStream.Write( (CHAR*)(&m_nSerial), sizeof(UINT));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGStallBuy::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGStallBuyHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

