#include "stdafx.h"
#include "GCUseEquipResult.h"


using namespace Packets;


BOOL GCUseEquipResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	iStream.Read((CHAR*)(&m_ItemID),sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)(&m_EquipID),sizeof(_ITEM_GUID));
	iStream.Read((CHAR*)(&m_ItemResId),sizeof(UINT));
	iStream.Read((CHAR*)(&m_EquipResId),sizeof(UINT));
	

	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCUseEquipResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	oStream.Write((CHAR*)(&m_ItemID),sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)(&m_EquipID),sizeof(_ITEM_GUID));
	oStream.Write((CHAR*)(&m_ItemResId),sizeof(UINT));
	oStream.Write((CHAR*)(&m_EquipResId),sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCUseEquipResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCUseEquipResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}