#include "stdafx.h"
#include "GCDiscardEquipResult.h"

using namespace Packets;


BOOL GCDiscardEquipResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read( (CHAR*)(&m_Result), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_ItemTableIndex), sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCDiscardEquipResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
	oStream.Write( (CHAR*)(&m_Result), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_EquipPoint), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_ItemTableIndex), sizeof(UINT));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCDiscardEquipResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCDiscardEquipResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}