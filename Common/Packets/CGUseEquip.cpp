#include "stdafx.h"
#include "CGUseEquip.h"


using namespace Packets;


BOOL CGUseEquip::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL CGUseEquip::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_BagIndex), sizeof(BYTE));
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGUseEquip::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGUseEquipHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}