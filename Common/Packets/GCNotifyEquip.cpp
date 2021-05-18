#include "stdafx.h"
#include "GCNotifyEquip.h"


using namespace Packets;


BOOL GCNotifyEquip::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_BagIndex), sizeof(WORD));
	iStream.Read( (CHAR*)(&m_Item), sizeof(_ITEM));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNotifyEquip::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_BagIndex), sizeof(WORD));
	oStream.Write( (CHAR*)(&m_Item), sizeof(_ITEM));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNotifyEquip::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCNotifyEquipHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}